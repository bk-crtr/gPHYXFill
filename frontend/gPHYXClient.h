#import <Foundation/Foundation.h>
#import <FxPlugSDK.h>

@interface gPHYXClient : NSObject

@property(nonatomic, strong) NSString *backendURL;

- (instancetype)initWithURL:(NSString *)url;

// New Clean Room API
- (void)initializeTrackingWithFrame:(FxImageTile *)tile
                         maskPoints:(NSArray<NSValue *> *)points
                         completion:(void (^)(BOOL success,
                                              NSInteger pointsCount))completion;

- (void)trackFrame:(FxImageTile *)tile
        completion:(void (^)(float *matrix9))completion;

- (void)inpaintWithFrame:(FxImageTile *)tile
              maskPoints:(NSArray<NSValue *> *)points
              completion:(void (^)(NSData *))completion;

@end
