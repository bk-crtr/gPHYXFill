#import "gPHYXOsc.h"
#import <Cocoa/Cocoa.h>
#import <FxPlug/FxPlugSDK.h>
#import <Metal/Metal.h>

@class gPHYXOsc;
@class gPHYXVisionTracker;

@interface gPHYXFillEffect : NSObject <FxTileableEffect, FxAnalyzer> {
  id<PROAPIAccessing> _apiManager;
  gPHYXOsc *_osc;
  BOOL _isTracking;
  BOOL _shouldInitTracking;
  BOOL _shouldInpaint;
  float _currentMatrix[9];

  // Metal
  id<MTLDevice> _device;
  id<MTLComputePipelineState> _inpaintPipeline;
  id<MTLCommandQueue> _commandQueue;

  // Vision & Tracking
  gPHYXVisionTracker *_visionTracker;
  CVPixelBufferRef _referenceBuffer;
  float _homography[9];

  // Analysis Cache
  NSMutableDictionary<NSNumber *, NSData *> *_homographyCache;
}

@property(nonatomic, assign) NSInteger referenceFrameIndex;
@property(nonatomic, assign) BOOL shouldSetReferenceFrame;

- (nullable instancetype)initWithAPIManager:(id<PROAPIAccessing>)apiManager;

@end
