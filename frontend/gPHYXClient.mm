#import "gPHYXClient.h"
#import <AppKit/AppKit.h>
#import <IOSurface/IOSurfaceObjC.h>

@implementation gPHYXClient

- (instancetype)initWithURL:(NSString *)url {
  self = [super init];
  if (self) {
    _backendURL = url;
  }
  return self;
}

- (NSData *)dataFromTile:(FxImageTile *)tile {
  IOSurface *surface = tile.ioSurface;
  if (!surface)
    return nil;

  size_t width = surface.width;
  size_t height = surface.height;
  size_t bpr = surface.bytesPerRow;
  void *base = surface.baseAddress;

  [surface lockWithOptions:kIOSurfaceLockReadOnly seed:NULL];

  // Create BitmapRep. Assuming BGRA 8-bit for simplicity as typical in FCP XPC
  // transitions
  NSBitmapImageRep *rep = [[NSBitmapImageRep alloc]
      initWithBitmapDataPlanes:NULL
                    pixelsWide:width
                    pixelsHigh:height
                 bitsPerSample:8
               samplesPerPixel:4
                      hasAlpha:YES
                      isPlanar:NO
                colorSpaceName:NSDeviceRGBColorSpace
                  bitmapFormat:NSBitmapFormatAlphaFirst |
                               NSBitmapFormatThirtyTwoBitLittleEndian
                   bytesPerRow:bpr
                  bitsPerPixel:32];

  memcpy(rep.bitmapData, base, bpr * height);
  [surface unlockWithOptions:kIOSurfaceLockReadOnly seed:NULL];

  return [rep representationUsingType:NSBitmapImageFileTypeJPEG
                           properties:@{NSImageCompressionFactor : @0.7}];
}

- (void)initializeTrackingWithFrame:(FxImageTile *)tile
                         maskPoints:(NSArray<NSValue *> *)points
                         completion:(void (^)(BOOL, NSInteger))completion {

  NSData *imageData = [self dataFromTile:tile];
  if (!imageData) {
    completion(NO, 0);
    return;
  }

  // Prepare mask points JSON
  NSMutableArray *jsonPts = [NSMutableArray array];
  for (NSValue *val in points) {
    CGPoint p = [val pointValue];
    [jsonPts addObject:@[ @(p.x), @(p.y) ]];
  }
  NSData *ptsData = [NSJSONSerialization dataWithJSONObject:jsonPts
                                                    options:0
                                                      error:nil];
  NSString *ptsStr = [[NSString alloc] initWithData:ptsData
                                           encoding:NSUTF8StringEncoding];

  NSURL *url = [NSURL
      URLWithString:[self.backendURL stringByAppendingString:@"/init_track"]];
  NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
  request.HTTPMethod = @"POST";

  NSString *boundary =
      [NSString stringWithFormat:@"Boundary-%@", [[NSUUID UUID] UUIDString]];
  [request setValue:[NSString
                        stringWithFormat:@"multipart/form-data; boundary=%@",
                                         boundary]
      forHTTPHeaderField:@"Content-Type"];

  NSMutableData *body = [NSMutableData data];
  [body appendData:[[NSString stringWithFormat:@"--%@\r\n", boundary]
                       dataUsingEncoding:NSUTF8StringEncoding]];
  [body appendData:[@"Content-Disposition: form-data; name=\"file\"; "
                    @"filename=\"frame.jpg\"\r\n"
                       dataUsingEncoding:NSUTF8StringEncoding]];
  [body appendData:[@"Content-Type: image/jpeg\r\n\r\n"
                       dataUsingEncoding:NSUTF8StringEncoding]];
  [body appendData:imageData];
  [body appendData:[@"\r\n" dataUsingEncoding:NSUTF8StringEncoding]];

  [body appendData:[[NSString stringWithFormat:@"--%@\r\n", boundary]
                       dataUsingEncoding:NSUTF8StringEncoding]];
  [body appendData:
            [@"Content-Disposition: form-data; name=\"points_json\"\r\n\r\n"
                dataUsingEncoding:NSUTF8StringEncoding]];
  [body appendData:[ptsStr dataUsingEncoding:NSUTF8StringEncoding]];
  [body appendData:[@"\r\n" dataUsingEncoding:NSUTF8StringEncoding]];
  [body appendData:[[NSString stringWithFormat:@"--%@--\r\n", boundary]
                       dataUsingEncoding:NSUTF8StringEncoding]];

  request.HTTPBody = body;

  [[[NSURLSession sharedSession]
      dataTaskWithRequest:request
        completionHandler:^(NSData *data, NSURLResponse *response,
                            NSError *error) {
          if (!data) {
            completion(NO, 0);
            return;
          }
          NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data
                                                               options:0
                                                                 error:nil];
          completion([json[@"status"] isEqualToString:@"success"],
                     [json[@"points_count"] integerValue]);
        }] resume];
}

- (void)trackFrame:(FxImageTile *)tile
        completion:(void (^)(float *))completion {
  NSData *imageData = [self dataFromTile:tile];
  if (!imageData) {
    completion(NULL);
    return;
  }

  NSURL *url = [NSURL
      URLWithString:[self.backendURL stringByAppendingString:@"/track_frame"]];
  NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
  request.HTTPMethod = @"POST";

  NSString *boundary =
      [NSString stringWithFormat:@"Boundary-%@", [[NSUUID UUID] UUIDString]];
  [request setValue:[NSString
                        stringWithFormat:@"multipart/form-data; boundary=%@",
                                         boundary]
      forHTTPHeaderField:@"Content-Type"];

  NSMutableData *body = [NSMutableData data];
  [body appendData:[[NSString stringWithFormat:@"--%@\r\n", boundary]
                       dataUsingEncoding:NSUTF8StringEncoding]];
  [body appendData:[@"Content-Disposition: form-data; name=\"file\"; "
                    @"filename=\"frame.jpg\"\r\n"
                       dataUsingEncoding:NSUTF8StringEncoding]];
  [body appendData:[@"Content-Type: image/jpeg\r\n\r\n"
                       dataUsingEncoding:NSUTF8StringEncoding]];
  [body appendData:imageData];
  [body appendData:[@"\r\n" dataUsingEncoding:NSUTF8StringEncoding]];
  [body appendData:[[NSString stringWithFormat:@"--%@--\r\n", boundary]
                       dataUsingEncoding:NSUTF8StringEncoding]];

  request.HTTPBody = body;

  [[[NSURLSession sharedSession]
      dataTaskWithRequest:request
        completionHandler:^(NSData *data, NSURLResponse *response,
                            NSError *error) {
          if (!data) {
            completion(NULL);
            return;
          }
          NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data
                                                               options:0
                                                                 error:nil];
          NSArray *hArr = json[@"homography"];
          if (hArr && hArr.count == 3) {
            float *m = (float *)malloc(9 * sizeof(float));
            m[0] = [hArr[0][0] floatValue];
            m[1] = [hArr[0][1] floatValue];
            m[2] = [hArr[0][2] floatValue];
            m[3] = [hArr[1][0] floatValue];
            m[4] = [hArr[1][1] floatValue];
            m[5] = [hArr[1][2] floatValue];
            m[6] = [hArr[2][0] floatValue];
            m[7] = [hArr[2][1] floatValue];
            m[8] = [hArr[2][2] floatValue];
            completion(m);
          } else {
            completion(NULL);
          }
        }] resume];
}

- (void)inpaintWithFrame:(FxImageTile *)tile
              maskPoints:(NSArray<NSValue *> *)points
              completion:(void (^)(NSData *))completion {
  NSData *imageData = [self dataFromTile:tile];
  if (!imageData) {
    completion(nil);
    return;
  }

  NSMutableArray *jsonPts = [NSMutableArray array];
  for (NSValue *val in points) {
    CGPoint p = [val pointValue];
    [jsonPts addObject:@[ @(p.x), @(p.y) ]];
  }
  NSData *ptsData = [NSJSONSerialization dataWithJSONObject:jsonPts
                                                    options:0
                                                      error:nil];
  NSString *ptsStr = [[NSString alloc] initWithData:ptsData
                                           encoding:NSUTF8StringEncoding];

  NSURL *url = [NSURL
      URLWithString:[self.backendURL stringByAppendingString:@"/inpaint"]];
  NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
  request.HTTPMethod = @"POST";

  NSString *boundary =
      [NSString stringWithFormat:@"Boundary-%@", [[NSUUID UUID] UUIDString]];
  [request setValue:[NSString
                        stringWithFormat:@"multipart/form-data; boundary=%@",
                                         boundary]
      forHTTPHeaderField:@"Content-Type"];

  NSMutableData *body = [NSMutableData data];
  [body appendData:[[NSString stringWithFormat:@"--%@\r\n", boundary]
                       dataUsingEncoding:NSUTF8StringEncoding]];
  [body appendData:[@"Content-Disposition: form-data; name=\"file\"; "
                    @"filename=\"frame.jpg\"\r\n"
                       dataUsingEncoding:NSUTF8StringEncoding]];
  [body appendData:[@"Content-Type: image/jpeg\r\n\r\n"
                       dataUsingEncoding:NSUTF8StringEncoding]];
  [body appendData:imageData];
  [body appendData:[@"\r\n" dataUsingEncoding:NSUTF8StringEncoding]];

  [body appendData:[[NSString stringWithFormat:@"--%@\r\n", boundary]
                       dataUsingEncoding:NSUTF8StringEncoding]];
  [body appendData:
            [@"Content-Disposition: form-data; name=\"points_json\"\r\n\r\n"
                dataUsingEncoding:NSUTF8StringEncoding]];
  [body appendData:[ptsStr dataUsingEncoding:NSUTF8StringEncoding]];
  [body appendData:[@"\r\n" dataUsingEncoding:NSUTF8StringEncoding]];
  [body appendData:[[NSString stringWithFormat:@"--%@--\r\n", boundary]
                       dataUsingEncoding:NSUTF8StringEncoding]];

  request.HTTPBody = body;

  [[[NSURLSession sharedSession]
      dataTaskWithRequest:request
        completionHandler:^(NSData *data, NSURLResponse *response,
                            NSError *error) {
          completion(data);
        }] resume];
}

@end
