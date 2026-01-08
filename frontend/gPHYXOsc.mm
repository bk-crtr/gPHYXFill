#import "gPHYXOsc.h"
#import <FxPlug/FxImageTile.h>
#import <FxPlug/FxOnScreenControl.h>
#import <FxPlug/FxOnScreenControlAPI.h>
#import <FxPlug/FxPlugSDK.h>
#import <IOSurface/IOSurfaceObjC.h>
#import <cmath>
#import <vector>

static int gOscInstanceCount = 0;

@implementation gPHYXOsc {
  NSUInteger _canvasWidth;
  NSUInteger _canvasHeight;
}

- (instancetype)initWithAPIManager:(id<PROAPIAccessing>)apiManager {
  self = [super init];
  if (self) {
    gOscInstanceCount++;
    _selectedIndex = -1;
    _selectedPart = 0;
    _hoverIndex = -1;
    _hidden = NO;
    _canvasWidth = 1920;
    _canvasHeight = 1080;
    _nodesPtr = new std::vector<BezierControlPoint>();

    _device = MTLCreateSystemDefaultDevice();
    _commandQueue = [_device newCommandQueue];

    for (int i = 0; i < 9; i++) {
      _drawHomography[i] = (i % 4 == 0) ? 1.0f : 0.0f;
    }

    // Тестовые точки
    [self addNode:CGPointMake(0.25, 0.25)];
    [self addNode:CGPointMake(0.75, 0.25)];
    [self addNode:CGPointMake(0.75, 0.75)];
    [self addNode:CGPointMake(0.25, 0.75)];

    NSLog(@"╔══════════════════════════════════════════════════════════╗");
    NSLog(@"║ [gPHYXOsc] Instance #%d CREATED with %lu test points    ║",
          gOscInstanceCount, (unsigned long)[self getCppNodes].size());
    NSLog(@"╚══════════════════════════════════════════════════════════╝");
  }
  return self;
}

- (void)dealloc {
  if (_nodesPtr) {
    delete (std::vector<BezierControlPoint> *)_nodesPtr;
    _nodesPtr = NULL;
  }
}

- (std::vector<BezierControlPoint> &)getCppNodes {
  return *(std::vector<BezierControlPoint> *)_nodesPtr;
}

- (void)setDrawHomography:(float *)matrix {
  if (matrix) {
    memcpy(_drawHomography, matrix, sizeof(float) * 9);
  } else {
    for (int i = 0; i < 9; i++) {
      _drawHomography[i] = (i % 4 == 0) ? 1.0f : 0.0f;
    }
  }
}

- (void)addNode:(CGPoint)pt {
  std::vector<BezierControlPoint> &nodes = [self getCppNodes];
  BezierControlPoint node;
  node.anchor = pt;
  node.inHandle = CGPointMake(-0.05, 0);
  node.outHandle = CGPointMake(0.05, 0);
  node.isCurve = NO;
  node.handlesBroken = NO;
  nodes.push_back(node);
  NSLog(@"[gPHYXOsc] ✓ Point added: (%.3f, %.3f) - Total: %lu", pt.x, pt.y,
        (unsigned long)nodes.size());
}

- (void)clearNodes {
  std::vector<BezierControlPoint> &nodes = [self getCppNodes];
  nodes.clear();
  _selectedIndex = -1;
  NSLog(@"[gPHYXOsc] ✗ All points cleared");
}

#pragma mark - FxOnScreenControl_v4 Protocol

- (FxDrawingCoordinates)drawingCoordinates {
  NSLog(@"[gPHYXOsc] 📐 drawingCoordinates -> kFxDrawingCoordinates_CANVAS");
  return kFxDrawingCoordinates_CANVAS;
}

- (void)drawOSCWithWidth:(NSInteger)width
                  height:(NSInteger)height
              activePart:(NSInteger)activePart
        destinationImage:(FxImageTile *)destinationImage
                  atTime:(CMTime)time {

  _canvasWidth = width;
  _canvasHeight = height;

  if (_hidden || !destinationImage)
    return;

  IOSurfaceRef surface = (__bridge IOSurfaceRef)destinationImage.ioSurface;
  if (!surface)
    return;

  IOSurfaceLock(surface, 0, NULL);
  void *baseAddress = IOSurfaceGetBaseAddress(surface);
  size_t bytesPerRow = IOSurfaceGetBytesPerRow(surface);
  size_t surfaceWidth = IOSurfaceGetWidth(surface);
  size_t surfaceHeight = IOSurfaceGetHeight(surface);

  if (baseAddress) {
    uint8_t *pixels = (uint8_t *)baseAddress;
    // Simple yellow corner border as placeholder
    int margin = 50;
    int thickness = 5;
    for (int x = margin; x < (int)surfaceWidth - margin; x++) {
      for (int t = 0; t < thickness; t++) {
        size_t off1 = (margin + t) * bytesPerRow + x * 4;
        size_t off2 = (surfaceHeight - margin - t - 1) * bytesPerRow + x * 4;
        pixels[off1 + 0] = 0;
        pixels[off1 + 1] = 255;
        pixels[off1 + 2] = 255;
        pixels[off1 + 3] = 255;
        pixels[off2 + 0] = 0;
        pixels[off2 + 1] = 255;
        pixels[off2 + 2] = 255;
        pixels[off2 + 3] = 255;
      }
    }
  }

  // Draw points and lines from external editor if available
  if (self.maskPoints && self.maskPoints.count > 0) {
    uint8_t *pixels = (uint8_t *)baseAddress;

    // 1. Draw connecting lines
    for (int i = 0; i < (int)self.maskPoints.count; i++) {
      NSPoint p1 = [self.maskPoints[i] pointValue];
      NSPoint p2 =
          [self.maskPoints[(i + 1) % self.maskPoints.count] pointValue];

      int x1 = (int)(p1.x * surfaceWidth);
      int y1 = (int)(p1.y * surfaceHeight);
      int x2 = (int)(p2.x * surfaceWidth);
      int y2 = (int)(p2.y * surfaceHeight);

      // Simple Bresenham or just DDA for lines
      int dx = abs(x2 - x1);
      int dy = abs(y2 - y1);
      int sx = (x1 < x2) ? 1 : -1;
      int sy = (y1 < y2) ? 1 : -1;
      int err = dx - dy;

      int cx = x1, cy = y1;
      while (true) {
        if (cx >= 0 && cx < (int)surfaceWidth && cy >= 0 &&
            cy < (int)surfaceHeight) {
          size_t off = cy * bytesPerRow + cx * 4;
          pixels[off + 0] = 255; // B (Magenta: B=255, G=0, R=255)
          pixels[off + 1] = 0;   // G
          pixels[off + 2] = 255; // R
          pixels[off + 3] = 255; // A
        }
        if (cx == x2 && cy == y2)
          break;
        int e2 = 2 * err;
        if (e2 > -dy) {
          err -= dy;
          cx += sx;
        }
        if (e2 < dx) {
          err += dx;
          cy += sy;
        }
      }
    }

    // 2. Draw points (vertex handles)
    for (NSValue *val in self.maskPoints) {
      NSPoint pt = [val pointValue];
      int px = (int)(pt.x * surfaceWidth);
      int py = (int)(pt.y * surfaceHeight);

      // Draw a small 5x5 blue square for each point
      for (int dy = -2; dy <= 2; dy++) {
        for (int dx = -2; dx <= 2; dx++) {
          int nx = px + dx;
          int ny = py + dy;
          if (nx >= 0 && nx < (int)surfaceWidth && ny >= 0 &&
              ny < (int)surfaceHeight) {
            size_t off = ny * bytesPerRow + nx * 4;
            pixels[off + 0] = 255; // B
            pixels[off + 1] = 0;   // G
            pixels[off + 2] = 0;   // R
            pixels[off + 3] = 255; // A
          }
        }
      }
    }
  }

  IOSurfaceUnlock(surface, 0, NULL);
}

- (void)drawOSC:(id<FxOnScreenControlAPI_v4>)api
     activePart:(NSInteger)activePart
         atTime:(CMTime)time {
  NSLog(@"[gPHYXOsc] ⚠️ Legacy drawOSC called (not typically used)");
}

- (void)hitTestOSCAtMousePositionX:(double)mousePositionX
                    mousePositionY:(double)mousePositionY
                        activePart:(NSInteger *)activePart
                            atTime:(CMTime)time {
  NSLog(@"[gPHYXOsc] 🖱️ hitTest: (%.1f, %.1f)", mousePositionX, mousePositionY);

  std::vector<BezierControlPoint> &nodes = [self getCppNodes];
  for (size_t i = 0; i < nodes.size(); i++) {
    CGPoint pt = CGPointMake(nodes[i].anchor.x * (double)_canvasWidth,
                             nodes[i].anchor.y * (double)_canvasHeight);
    double dx = mousePositionX - pt.x;
    double dy = mousePositionY - pt.y;
    double dist = sqrt(dx * dx + dy * dy);
    if (dist < 25.0) {
      *activePart = (NSInteger)((i + 1) * 10);
      NSLog(@"[gPHYXOsc] ✓ HIT point %zu (activePart=%ld)", i,
            (long)*activePart);
      return;
    }
  }

  *activePart = 9999;
  NSLog(@"[gPHYXOsc] → Background (9999)");
}

- (void)mouseDownAtPositionX:(double)x
                   positionY:(double)y
                  activePart:(NSInteger)activePart
                   modifiers:(FxModifierKeys)modifiers
                 forceUpdate:(BOOL *)forceUpdate
                      atTime:(CMTime)time {

  NSLog(@"[gPHYXOsc] 🖱️ mouseDown: (%.1f, %.1f) activePart=%ld", x, y,
        (long)activePart);

  if (activePart == 9999) {
    double normX = x / (double)_canvasWidth;
    double normY = y / (double)_canvasHeight;
    [self addNode:CGPointMake(normX, normY)];
    *forceUpdate = YES;
  } else if (activePart >= 10) {
    _selectedIndex = (activePart / 10) - 1;
    _selectedPart = 0;
    *forceUpdate = YES;
    NSLog(@"[gPHYXOsc] ✓ Selected point %ld", (long)_selectedIndex);
  }
}

- (void)mouseDraggedAtPositionX:(double)x
                      positionY:(double)y
                     activePart:(NSInteger)activePart
                      modifiers:(FxModifierKeys)modifiers
                    forceUpdate:(BOOL *)forceUpdate
                         atTime:(CMTime)time {
  if (_selectedIndex >= 0) {
    std::vector<BezierControlPoint> &nodes = [self getCppNodes];
    if (_selectedIndex < nodes.size()) {
      double normX = x / (double)_canvasWidth;
      double normY = y / (double)_canvasHeight;
      nodes[_selectedIndex].anchor = CGPointMake(normX, normY);
      *forceUpdate = YES;
    }
  }
}

- (void)mouseUpAtPositionX:(double)x
                 positionY:(double)y
                activePart:(NSInteger)activePart
                 modifiers:(FxModifierKeys)modifiers
               forceUpdate:(BOOL *)forceUpdate
                    atTime:(CMTime)time {
  _selectedIndex = -1;
}

- (void)keyDownAtPositionX:(double)x
                 positionY:(double)y
                keyPressed:(unsigned short)asciiKey
                 modifiers:(FxModifierKeys)modifiers
               forceUpdate:(BOOL *)forceUpdate
                 didHandle:(BOOL *)didHandle
                    atTime:(CMTime)time {
  if (asciiKey == 127 || asciiKey == 8) {
    if (_selectedIndex >= 0) {
      std::vector<BezierControlPoint> &nodes = [self getCppNodes];
      if (_selectedIndex < nodes.size()) {
        nodes.erase(nodes.begin() + _selectedIndex);
        _selectedIndex = -1;
        *forceUpdate = YES;
        *didHandle = YES;
        NSLog(@"[gPHYXOsc] ✗ Point deleted");
      }
    }
  }
}

- (void)keyUpAtPositionX:(double)x
               positionY:(double)y
              keyPressed:(unsigned short)asciiKey
               modifiers:(FxModifierKeys)modifiers
             forceUpdate:(BOOL *)forceUpdate
               didHandle:(BOOL *)didHandle
                  atTime:(CMTime)time {
}

- (CGRect)getOnScreenControlBounds:(id<FxOnScreenControlAPI_v4>)api {
  NSLog(@"[gPHYXOsc] 📐 getOnScreenControlBounds called");
  NSUInteger w, h;
  double par;
  [api inputWidth:&w height:&h pixelAspectRatio:&par];
  NSLog(@"[gPHYXOsc] → Bounds: %lux%lu", (unsigned long)w, (unsigned long)h);
  return CGRectMake(0, 0, w, h);
}

- (id<MTLTexture>)getMaskTextureForDevice:(id<MTLDevice>)device
                                    width:(NSUInteger)width
                                   height:(NSUInteger)height
                               apiManager:(id<PROAPIAccessing>)apiManager
                                   atTime:(CMTime)time {
  NSLog(@"[gPHYXOsc] 🎭 getMaskTexture: %lux%lu", (unsigned long)width,
        (unsigned long)height);

  if (!device || width == 0 || height == 0) {
    NSLog(@"[gPHYXOsc] ❌ Invalid device or dimensions");
    return nil;
  }

  if (width > 8192 || height > 8192) {
    NSLog(@"[gPHYXOsc] ⚠️ Size too large, clamping");
    return nil;
  }

  // Get path data from parameter
  id<FxParameterRetrievalAPI_v6> paramAPI =
      [apiManager apiForProtocol:@protocol(FxParameterRetrievalAPI_v6)];
  id<FxPathAPI_v3> pathAPI =
      [apiManager apiForProtocol:@protocol(FxPathAPI_v3)];

  if (!paramAPI || !pathAPI) {
    NSLog(@"[gPHYXOsc] ⚠️ No Path API, using fallback ellipse");
    return [self createFallbackMaskForDevice:device width:width height:height];
  }

  // Get path ID from parameter kParam_ShowOSC (defined as 12)
  FxPathID pathID = 0;
  if (![paramAPI getPathID:&pathID fromParameter:12 atTime:time]) {
    NSLog(@"[gPHYXOsc] ⚠️ No path set, using fallback");
    return [self createFallbackMaskForDevice:device width:width height:height];
  }

  // Get number of vertices
  NSUInteger numVertices = 0;
  NSError *error = nil;
  if (![pathAPI numberOfVertices:&numVertices
                          inPath:pathID
                          atTime:time
                           error:&error]) {
    NSLog(@"[gPHYXOsc] ⚠️ Failed to get vertices: %@",
          error.localizedDescription);
    return [self createFallbackMaskForDevice:device width:width height:height];
  }

  if (numVertices == 0) {
    NSLog(@"[gPHYXOsc] ⚠️ Path has no vertices");
    return [self createFallbackMaskForDevice:device width:width height:height];
  }

  NSLog(@"[gPHYXOsc] ✓ Path has %lu vertices", (unsigned long)numVertices);

  // Create bitmap context
  size_t bytesPerRow = width;
  uint8_t *bitmapData = (uint8_t *)calloc(bytesPerRow * height, 1);

  if (!bitmapData) {
    NSLog(@"[gPHYXOsc] ❌ calloc failed!");
    return nil;
  }

  CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceGray();
  CGContextRef context = CGBitmapContextCreate(
      bitmapData, width, height, 8, bytesPerRow, colorSpace, kCGImageAlphaNone);
  CGColorSpaceRelease(colorSpace);

  if (!context) {
    free(bitmapData);
    return nil;
  }

  // Fill background with black (0)
  CGContextSetGrayFillColor(context, 0.0, 1.0);
  CGContextFillRect(context, CGRectMake(0, 0, width, height));

  // Draw Bezier path from vertices
  CGContextBeginPath(context);

  for (NSUInteger i = 0; i < numVertices; i++) {
    FxVertex vertex;
    if (![pathAPI vertex:&vertex
                 atIndex:i
                  ofPath:pathID
                  atTime:time
                   error:&error]) {
      NSLog(@"[gPHYXOsc] ⚠️ Failed to get vertex %lu", (unsigned long)i);
      continue;
    }

    // Convert from image space to normalized 0-1, then to pixel coords
    double x = vertex.location.x * width;
    double y = vertex.location.y * height;

    if (i == 0) {
      CGContextMoveToPoint(context, x, y);
    } else {
      // Check if Bezier curve or linear
      if (vertex.interpStyle == kFxPathStyle_Bezier) {
        // Use Bezier curve with tangents
        // Previous vertex's outTangent + current inTangent
        FxVertex prevVertex;
        if ([pathAPI vertex:&prevVertex
                    atIndex:i - 1
                     ofPath:pathID
                     atTime:time
                      error:nil]) {
          double cp1x =
              (prevVertex.location.x + prevVertex.outTangent.x) * width;
          double cp1y =
              (prevVertex.location.y + prevVertex.outTangent.y) * height;
          double cp2x = (vertex.location.x + vertex.inTangent.x) * width;
          double cp2y = (vertex.location.y + vertex.inTangent.y) * height;

          CGContextAddCurveToPoint(context, cp1x, cp1y, cp2x, cp2y, x, y);
        } else {
          CGContextAddLineToPoint(context, x, y);
        }
      } else {
        // Linear segment
        CGContextAddLineToPoint(context, x, y);
      }
    }
  }

  CGContextClosePath(context);

  // Fill path with white (1)
  CGContextSetGrayFillColor(context, 1.0, 1.0);
  CGContextFillPath(context);

  // Create Metal texture
  MTLTextureDescriptor *texDesc = [MTLTextureDescriptor
      texture2DDescriptorWithPixelFormat:MTLPixelFormatR8Unorm
                                   width:width
                                  height:height
                               mipmapped:NO];
  texDesc.usage = MTLTextureUsageShaderRead;
  id<MTLTexture> texture = [device newTextureWithDescriptor:texDesc];

  [texture replaceRegion:MTLRegionMake2D(0, 0, width, height)
             mipmapLevel:0
               withBytes:bitmapData
             bytesPerRow:bytesPerRow];

  CGContextRelease(context);
  free(bitmapData);

  NSLog(@"[gPHYXOsc] ✅ Mask texture created from path");
  return texture;
}

// Fallback ellipse mask for when no path is set
- (id<MTLTexture>)createFallbackMaskForDevice:(id<MTLDevice>)device
                                        width:(NSUInteger)width
                                       height:(NSUInteger)height {
  size_t bytesPerRow = width;
  uint8_t *bitmapData = (uint8_t *)calloc(bytesPerRow * height, 1);
  if (!bitmapData)
    return nil;

  CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceGray();
  CGContextRef context = CGBitmapContextCreate(
      bitmapData, width, height, 8, bytesPerRow, colorSpace, kCGImageAlphaNone);
  CGColorSpaceRelease(colorSpace);

  if (!context) {
    free(bitmapData);
    return nil;
  }

  // Draw ellipse in center
  CGContextSetGrayFillColor(context, 0.0, 1.0);
  CGContextFillRect(context, CGRectMake(0, 0, width, height));

  CGContextSetGrayFillColor(context, 1.0, 1.0);
  CGRect ellipseRect =
      CGRectMake(width * 0.25, height * 0.25, width * 0.5, height * 0.5);
  CGContextFillEllipseInRect(context, ellipseRect);

  MTLTextureDescriptor *texDesc = [MTLTextureDescriptor
      texture2DDescriptorWithPixelFormat:MTLPixelFormatR8Unorm
                                   width:width
                                  height:height
                               mipmapped:NO];
  texDesc.usage = MTLTextureUsageShaderRead;
  id<MTLTexture> texture = [device newTextureWithDescriptor:texDesc];

  [texture replaceRegion:MTLRegionMake2D(0, 0, width, height)
             mipmapLevel:0
               withBytes:bitmapData
             bytesPerRow:bytesPerRow];

  CGContextRelease(context);
  free(bitmapData);

  NSLog(@"[gPHYXOsc] ✅ Fallback ellipse mask created");
  return texture;
}

@end
