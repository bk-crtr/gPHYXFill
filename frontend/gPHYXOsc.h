#import <Cocoa/Cocoa.h>
#import <CoreGraphics/CoreGraphics.h>
#import <Foundation/Foundation.h>
#import <FxPlug/FxPlugSDK.h>
#import <Metal/Metal.h>
#import <PluginManager/PROAPIAccessing.h>

#ifdef __cplusplus
#import <vector>
#endif

struct BezierControlPoint {
  CGPoint anchor;    // Normalized (0..1)
  CGPoint inHandle;  // Offset from anchor (normalized)
  CGPoint outHandle; // Offset from anchor (normalized)
  BOOL isCurve;
  BOOL handlesBroken; // Smooth vs Cusp/Broken
};

@interface gPHYXOsc : NSObject <FxOnScreenControl_v4> {
  id<PROAPIAccessing> _apiManager;
  void *_nodesPtr;
  NSInteger _selectedIndex;
  NSInteger _selectedPart;
  NSInteger _hoverIndex;

  id<MTLDevice> _device;
  id<MTLCommandQueue> _commandQueue;

  float _drawHomography[9];
}

@property(nonatomic, assign) void *nodesPtr; // std::vector<BezierControlPoint>*
@property(nonatomic, assign) NSInteger selectedIndex;
@property(nonatomic, assign) NSInteger selectedPart; // 0=anchor, 1=in, 2=out
@property(nonatomic, assign) NSInteger hoverIndex;
@property(nonatomic, assign) BOOL hidden;

- (instancetype)initWithAPIManager:(id<PROAPIAccessing>)apiManager;
- (void)addNode:(CGPoint)pt;
- (void)clearNodes;
- (id<MTLTexture>)getMaskTextureForDevice:(id<MTLDevice>)device
                                    width:(NSUInteger)width
                                   height:(NSUInteger)height
                               apiManager:(id<PROAPIAccessing>)apiManager
                                   atTime:(CMTime)time;

- (void)setDrawHomography:(float *)matrix;
#ifdef __cplusplus
- (std::vector<BezierControlPoint> &)getCppNodes;
#endif

@property(nonatomic, retain) NSArray<NSValue *> *maskPoints;
@end
