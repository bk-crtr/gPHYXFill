#import "gPHYXFillEffect.h"
#import "gPHYXFillXPC-Swift.h"
#import <CoreVideo/CVPixelBuffer.h>
#import <CoreVideo/CVPixelBufferIOSurface.h>
#import <Metal/Metal.h>
#import <PluginManager/PROAPIAccessing.h>
#import <vector>

enum {
  kParam_ReferenceFrame = 1,
  kParam_InitTracking = 2,
  kParam_TrackBtn = 10,
  kParam_ClearBtn = 11,
  kParam_ShowOSC = 12,
  kParam_InpaintBtn = 13,
  kParam_OpenEditor = 14,
  kParam_SourceVideo = 15,

  // ROI Parameters (Mocha Logic)
  kParam_ROIGroup = 100,
  kParam_ROIX1 = 20,
  kParam_ROIY1 = 21,
  kParam_ROIX2 = 22,
  kParam_ROIY2 = 23,
  kParam_FitROIBtn = 25,
  kParam_DropZone = 30,
  kParam_Status = 40,
  kParam_InstanceID = 50
};

// --- SHARED REGISTRY ---
@interface gPHYXSharedData : NSObject
@property(nonatomic, assign) NSInteger referenceFrame;
@property(nonatomic, retain)
    NSMutableDictionary<NSNumber *, NSData *> *homographyCache;
@property(nonatomic, assign) CVPixelBufferRef referenceBuffer;
@property(nonatomic, assign) BOOL isTracking;
@property(nonatomic, retain)
    NSArray<NSValue *> *maskPoints; // New: Points from Editor
@property(nonatomic, assign)
    NSTimeInterval lastJSONLoadTime; // New: For polling
@end

@implementation gPHYXSharedData
- (instancetype)init {
  if (self = [super init]) {
    _homographyCache = [NSMutableDictionary dictionary];
    _isTracking = NO;
  }
  return self;
}
- (void)setReferenceBuffer:(CVPixelBufferRef)ref {
  if (_referenceBuffer)
    CFRelease(_referenceBuffer);
  if (ref)
    CFRetain(ref);
  _referenceBuffer = ref;
}
- (void)dealloc {
  if (_referenceBuffer)
    CFRelease(_referenceBuffer);
}
@end

static NSMutableDictionary<NSString *, gPHYXSharedData *> *s_registry = nil;
static NSString *const kDefaultInstanceID = @"MainInstance";

void __attribute__((constructor)) initialize_gphyx() {
  NSLog(@"========================================");
  NSLog(@"[gPHYX] gPHYXFillEffect228 loaded");
  NSLog(@"========================================");
}

@implementation gPHYXFillEffect

- (instancetype)initWithAPIManager:(id<PROAPIAccessing>)newApiManager {
  self = [super init];
  if (self) {
    NSLog(@"╔══════════════════════════════════════════════════════════╗");
    NSLog(@"║ [gPHYX] com.gphyx.FillEffect228: INIT                  ║");
    NSLog(@"╚══════════════════════════════════════════════════════════╝");

    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
      s_registry = [NSMutableDictionary dictionary];
    });

    _apiManager = newApiManager;

    // ВАЖНО: Создаем OSC ЗДЕСЬ, а не лениво
    _osc = [[gPHYXOsc alloc] initWithAPIManager:_apiManager];
    NSLog(@"[gPHYX] ✓ OSC created: %@", _osc);

    _device = MTLCreateSystemDefaultDevice();
    _commandQueue = [_device newCommandQueue];
    _visionTracker = [[gPHYXVisionTracker alloc] init];

    NSError *error = nil;
    id<MTLLibrary> library = [_device newDefaultLibrary];
    if (library) {
      id<MTLFunction> function =
          [library newFunctionWithName:@"inpaint_kernel"];
      _inpaintPipeline = [_device newComputePipelineStateWithFunction:function
                                                                error:&error];
      if (error) {
        NSLog(@"[gPHYX] Metal Pipeline Error: %@", error);
      }
    }

    _isTracking = NO;
    _shouldInitTracking = NO;
    _homographyCache = [[NSMutableDictionary alloc] init];
    for (int i = 0; i < 9; i++)
      _currentMatrix[i] = (i % 4 == 0) ? 1.0f : 0.0f;
  }
  return self;
}

- (void)dealloc {
  // NSLog(@"[gPHYX] dealloc called");
  if (_referenceBuffer) {
    CFRelease(_referenceBuffer);
    _referenceBuffer = NULL;
  }
}

- (BOOL)properties:(NSDictionary *_Nonnull *_Nullable)properties
             error:(NSError **)outError {
  NSLog(@"[gPHYX] properties: called (v2.26)");
  *properties = @{
    kFxPropertyKey_MayRemapTime : @NO,
    kFxPropertyKey_PixelTransformSupport : @(kFxPixelTransform_Full),
    kFxPropertyKey_VariesWhenParamsAreStatic : @YES,
    @"PreservesAlpha" : @YES,

    @"OnScreenControlClassName" : @"gPHYXOsc",
    @"DesiredOSCIDs" : @[ @(kParam_ShowOSC) ],
    @"TransformsFromLocalToScreenSpace" : @YES
  };

  NSLog(@"[gPHYX] ✓ Properties set with OSC for param %d", kParam_ShowOSC);
  return YES;
}

- (BOOL)addParametersWithError:(NSError **)outError {
  NSLog(@"========================================");
  NSLog(@"gPHYX v2.26: addParametersWithError called");
  NSLog(@"========================================");

  id<FxParameterCreationAPI_v5> paramAPI =
      [_apiManager apiForProtocol:@protocol(FxParameterCreationAPI_v5)];

  if (!paramAPI) {
    NSLog(@"gPHYX: CRITICAL ERROR - Could not get Parameter Creation API!");
    if (outError)
      *outError = [NSError errorWithDomain:FxPlugErrorDomain
                                      code:kFxError_APIUnavailable
                                  userInfo:nil];
    return NO;
  }

  BOOL res = YES;

  // 1. Reference Frame (Slider)
  if (![paramAPI addIntSliderWithName:@"Reference Frame"
                          parameterID:kParam_ReferenceFrame
                         defaultValue:0
                         parameterMin:0
                         parameterMax:10000
                            sliderMin:0
                            sliderMax:10000
                                delta:1
                       parameterFlags:kFxParameterFlag_DEFAULT]) {
    NSLog(@"gPHYX: Failed to add Reference Frame");
    res = NO;
  }

  // 2. Initialize Reference (Button)
  if (![paramAPI addPushButtonWithName:@"Initialize Reference"
                           parameterID:kParam_InitTracking
                              selector:@selector(initTrackingAction)
                        parameterFlags:kFxParameterFlag_DEFAULT]) {
    NSLog(@"gPHYX: Failed to add Init Button");
    res = NO;
  }

  // 3. Track Motion Button
  if (![paramAPI addPushButtonWithName:@"Open Editor"
                           parameterID:kParam_OpenEditor
                              selector:@selector(launchEditorAction)
                        parameterFlags:kFxParameterFlag_DEFAULT]) {
    NSLog(@"gPHYX: Failed to add Open Editor Button");
    res = NO;
  }

  // 3b. Source Video Path (For External Editor)
  if (![paramAPI addStringParameterWithName:@"Source Video"
                                parameterID:kParam_SourceVideo
                               defaultValue:@""
                             parameterFlags:kFxParameterFlag_DEFAULT]) {
    NSLog(@"gPHYX: Failed to add Source Video Path Parameter");
    res = NO;
  }

  // 4. Clear Mask Button
  if (![paramAPI addPushButtonWithName:@"Clear Mask"
                           parameterID:kParam_ClearBtn
                              selector:@selector(clearMaskAction)
                        parameterFlags:kFxParameterFlag_DEFAULT]) {
    NSLog(@"gPHYX: Failed to add Clear Button");
    res = NO;
  }

  if (![paramAPI addToggleButtonWithName:@"Show Mask / OSC"
                             parameterID:kParam_ShowOSC
                            defaultValue:YES
                          parameterFlags:kFxParameterFlag_DEFAULT]) {
    NSLog(@"gPHYX: Failed to add OSC Toggle");
    res = NO;
  }

  // 5b. Drop Zone for processed Clean Plate
  if (![paramAPI addImageReferenceWithName:@"Clean Plate (Drop Zone)"
                               parameterID:kParam_DropZone
                            parameterFlags:kFxParameterFlag_DEFAULT]) {
    NSLog(@"gPHYX: Failed to add Drop Zone");
    res = NO;
  }

  // 6. ROI Parameters (Hidden, Managed by Auto-Fit)
  // Button first for visibility
  [paramAPI addPushButtonWithName:@"Fit ROI to Surface"
                      parameterID:kParam_FitROIBtn
                         selector:@selector(fitROIAction)
                   parameterFlags:kFxParameterFlag_DEFAULT];

  [paramAPI addFloatSliderWithName:@"ROI X1 (Left)"
                       parameterID:kParam_ROIX1
                      defaultValue:0.0
                      parameterMin:0.0
                      parameterMax:1.0
                         sliderMin:0.0
                         sliderMax:1.0
                             delta:0.01
                    parameterFlags:kFxParameterFlag_HIDDEN]; // HIDDEN

  [paramAPI addFloatSliderWithName:@"ROI Y1 (Top)"
                       parameterID:kParam_ROIY1
                      defaultValue:0.0
                      parameterMin:0.0
                      parameterMax:1.0
                         sliderMin:0.0
                         sliderMax:1.0
                             delta:0.01
                    parameterFlags:kFxParameterFlag_HIDDEN]; // HIDDEN

  [paramAPI addFloatSliderWithName:@"ROI X2 (Right)"
                       parameterID:kParam_ROIX2
                      defaultValue:1.0
                      parameterMin:0.0
                      parameterMax:1.0
                         sliderMin:0.0
                         sliderMax:1.0
                             delta:0.01
                    parameterFlags:kFxParameterFlag_HIDDEN]; // HIDDEN

  [paramAPI addFloatSliderWithName:@"ROI Y2 (Bottom)"
                       parameterID:kParam_ROIY2
                      defaultValue:1.0
                      parameterMin:0.0
                      parameterMax:1.0
                         sliderMin:0.0
                         sliderMax:1.0
                             delta:0.01
                    parameterFlags:kFxParameterFlag_HIDDEN]; // HIDDEN

  // 8. Status Info
  if (![paramAPI addStringParameterWithName:@"Status"
                                parameterID:kParam_Status
                               defaultValue:@"[v2.27] Ready ✓"
                             parameterFlags:kFxParameterFlag_DEFAULT]) {
    NSLog(@"gPHYX: Failed to add Status");
    res = NO;
  }

  if (![paramAPI addStringParameterWithName:@"InstanceID"
                                parameterID:kParam_InstanceID
                               defaultValue:@""
                             parameterFlags:kFxParameterFlag_HIDDEN]) {
    NSLog(@"gPHYX: Failed to add InstanceID");
    res = NO;
  }

  if (!res) {
    if (outError)
      *outError = [NSError errorWithDomain:FxPlugErrorDomain
                                      code:kFxError_InvalidParameter
                                  userInfo:nil];
    return NO;
  }

  NSLog(@"========================================");
  NSLog(@"gPHYX v2.25: Parameters added successfully!");
  NSLog(@"========================================");
  return YES;
}

// КРИТИЧНО: OSC Support
- (id)onScreenControlForParameterID:(UInt32)parameterID {
  NSLog(@"╔══════════════════════════════════════════════════════════╗");
  NSLog(@"║ [gPHYX] onScreenControlForParameterID: %u               ║",
        (unsigned int)parameterID);
  NSLog(@"║ kParam_ShowOSC: %d                                       ║",
        kParam_ShowOSC);
  NSLog(@"║ _osc exists: %@                                          ║",
        _osc ? @"YES" : @"NO");
  NSLog(@"╚══════════════════════════════════════════════════════════╝");

  if (parameterID == kParam_ShowOSC) {
    if (!_osc) {
      NSLog(@"[gPHYX] ⚠️ WARNING: OSC is nil, creating now...");
      _osc = [[gPHYXOsc alloc] initWithAPIManager:_apiManager];
    }
    NSLog(@"[gPHYX] ✓ Returning OSC: %@", _osc);
    return _osc;
  }

  NSLog(@"[gPHYX] → Returning nil (not OSC parameter)");
  return nil;
}

#pragma mark - Selectors

- (void)initTrackingAction {
  [self getInstanceID:kCMTimeZero]; // Ensure ID is generated
  _shouldInitTracking = YES;
  [self updateStatus:@"🟠 Reference Captured!"];
  NSLog(@"[gPHYX] 🎬 Initialize Tracking triggered (ID: %@)",
        [self getInstanceID:kCMTimeZero]);
}

- (void)trackMotionAction {
  NSString *iid = [self getInstanceID:kCMTimeZero]; // Ensure ID is generated
  NSLog(@"[gPHYX] Track Motion Action (Automatic) triggered (ID: %@)...", iid);
  [self updateStatus:@"🟠 Requesting Analysis..."];

  id<FxAnalysisAPI_v2> analysisAPI =
      [_apiManager apiForProtocol:@protocol(FxAnalysisAPI_v2)];
  if (analysisAPI) {
    NSError *error = nil;
    if (![analysisAPI startForwardAnalysis:kFxAnalysisLocation_GPU
                                     error:&error]) {
      NSLog(@"[gPHYX] Failed to start analysis: %@", error);
      [self updateStatus:@"❌ Analysis Failed"];
    } else {
      _isTracking = YES;
    }
  } else {
    NSLog(@"[gPHYX] FxAnalysisAPI not available. Manual tracking only.");
    _isTracking = !_isTracking;
    [self updateStatus:_isTracking ? @"🟢 Tracking: ON (Manual)"
                                   : @"🔴 Tracking: OFF"];
  }
}

- (void)launchEditorAction {
  NSLog(@"[gPHYX] 🚀 Launch Editor Action triggered...");
  [self updateStatus:@"🟠 Launching Editor..."];

  NSBundle *myBundle = [NSBundle bundleForClass:[self class]];
  NSURL *pluginURL = [myBundle bundleURL];
  // pluginURL: .../gPHYXFill.app/Contents/PlugIns/gPHYXFillXPC.pluginkit
  // Contents is 2 levels up
  NSURL *contentsURL = [[pluginURL URLByDeletingLastPathComponent]
      URLByDeletingLastPathComponent];

  NSURL *editorURL = [[contentsURL URLByAppendingPathComponent:@"Resources"]
      URLByAppendingPathComponent:@"gPHYXEditor.app"];

  if (![[NSFileManager defaultManager] fileExistsAtPath:[editorURL path]]) {
    // Fallback search
    editorURL = [pluginURL URLByAppendingPathComponent:@"gPHYXEditor.app"];
  }

  id<FxParameterRetrievalAPI_v6> getter =
      [_apiManager apiForProtocol:@protocol(FxParameterRetrievalAPI_v6)];
  NSString *videoPath = @"";
  if (getter) {
    [getter getStringParameterValue:&videoPath
                      fromParameter:kParam_SourceVideo];
  }

  // AUTO-DETECTION: If path is empty, inform the user via status
  if (videoPath == nil || [videoPath length] == 0) {
    NSLog(@"[gPHYX] Source Video path is empty. User must select a file.");
    [self updateStatus:@"⚠️ Select Source Video!"];
  }

  NSString *iid = [self getInstanceID:kCMTimeZero];
  NSLog(@"[gPHYX] 🔎 Searching for Editor at: %@", [editorURL path]);
  NSLog(@"[gPHYX] 📂 Video Path: %@", videoPath);
  NSLog(@"[gPHYX] 🆔 Instance ID: %@", iid);

  NSWorkspace *workspace = [NSWorkspace sharedWorkspace];
  NSWorkspaceOpenConfiguration *config =
      [NSWorkspaceOpenConfiguration configuration];

  // Pass arguments to the app
  config.arguments =
      @[ @"--video", videoPath ?: @"", @"--instance", iid ?: @"" ];

  // FORCE RESTART: Terminates if already running to ensure fresh arguments
  NSArray<NSRunningApplication *> *apps = [NSRunningApplication
      runningApplicationsWithBundleIdentifier:@"com.gphyx.FillEffect.editor"];
  for (NSRunningApplication *app in apps) {
    [app terminate];
  }

  // Small delay for cleanup then launch
  dispatch_after(
      dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.2 * NSEC_PER_SEC)),
      dispatch_get_main_queue(), ^{
        [workspace
            openApplicationAtURL:editorURL
                   configuration:config
               completionHandler:^(NSRunningApplication *_Nullable app,
                                   NSError *_Nullable error) {
                 if (error) {
                   NSLog(@"[gPHYX] ❌ Failed to launch Editor: %@", error);
                   dispatch_async(dispatch_get_main_queue(), ^{
                     [self updateStatus:@"❌ Editor Launch Failed"];
                   });
                 } else {
                   NSLog(@"[gPHYX] ✅ Editor launched successfully!");
                   dispatch_async(dispatch_get_main_queue(), ^{
                     [self updateStatus:@"🟢 Editor Open"];
                   });
                 }
               }];
      });
}

- (void)clearMaskAction {
  [_osc clearNodes];
  [self updateStatus:@"⚪️ Mask Cleared"];
  NSLog(@"[gPHYX] 🗑️ Mask cleared");
}

- (NSString *)getInstanceID:(CMTime)time {
  id<FxParameterRetrievalAPI_v6> getter =
      [_apiManager apiForProtocol:@protocol(FxParameterRetrievalAPI_v6)];
  NSString *currentID = @"";
  if (getter) {
    [getter getStringParameterValue:&currentID fromParameter:kParam_InstanceID];
  }

  if (currentID == nil || [currentID length] == 0) {
    // Generate new ID if empty
    currentID = [[NSUUID UUID] UUIDString];
    NSLog(@"[gPHYX] 🆔 Generated NEW InstanceID: %@", currentID);
    id<FxParameterSettingAPI_v6> setter =
        [_apiManager apiForProtocol:@protocol(FxParameterSettingAPI_v6)];
    if (setter) {
      [setter setStringParameterValue:currentID toParameter:kParam_InstanceID];
    }
  } else {
    // NSLog(@"[gPHYX] 🆔 Using existing InstanceID: %@", currentID);
  }
  return currentID;
}

- (gPHYXSharedData *)getSharedData:(CMTime)time {
  NSString *instanceID = [self getInstanceID:time];
  gPHYXSharedData *data = [s_registry objectForKey:instanceID];
  if (!data) {
    data = [[gPHYXSharedData alloc] init];
    [s_registry setObject:data forKey:instanceID];
  }
  return data;
}

- (void)inpaintAction {
  _shouldInpaint = YES;
  [self updateStatus:@"🔵 Inpainting Done"];
  NSLog(@"gPHYX: Inpainting triggered...");
}

- (void)updateStatus:(NSString *)text {
  id<FxParameterSettingAPI_v6> setter =
      [_apiManager apiForProtocol:@protocol(FxParameterSettingAPI_v6)];
  if (setter) {
    [setter setStringParameterValue:text toParameter:kParam_Status];
  }
}

- (NSString *)getProgressBarForPercent:(float)percent {
  int width = 10;
  int progress = (int)(percent / (100.0 / width));
  NSMutableString *bar = [NSMutableString stringWithString:@"["];
  for (int i = 0; i < width; i++) {
    if (i < progress)
      [bar appendString:@"█"];
    else
      [bar appendString:@"░"];
  }
  [bar appendFormat:@"] %d%%", (int)percent];
  return bar;
}

- (CGRect)calculateROIRectAtTime:(CMTime)time {
  double roiX1 = 0, roiY1 = 0, roiX2 = 1, roiY2 = 1;
  id<FxParameterRetrievalAPI_v6> getAPI =
      [_apiManager apiForProtocol:@protocol(FxParameterRetrievalAPI_v6)];
  if (getAPI) {
    [getAPI getFloatValue:&roiX1 fromParameter:kParam_ROIX1 atTime:time];
    [getAPI getFloatValue:&roiY1 fromParameter:kParam_ROIY1 atTime:time];
    [getAPI getFloatValue:&roiX2 fromParameter:kParam_ROIX2 atTime:time];
    [getAPI getFloatValue:&roiY2 fromParameter:kParam_ROIY2 atTime:time];
  }

  double width = roiX2 - roiX1;
  double height = roiY2 - roiY1;
  double margin = 0.10;

  double finalX = MAX(0.0, roiX1 - (width * margin));
  double finalY = MAX(0.0, roiY1 - (height * margin));
  double finalW = MIN(1.0 - finalX, width * (1.0 + margin * 2.0));
  double finalH = MIN(1.0 - finalY, height * (1.0 + margin * 2.0));

  return CGRectMake(finalX, finalY, finalW, finalH);
}

#pragma mark - FxAnalyzer Implementation

- (BOOL)desiredAnalysisTimeRange:(CMTimeRange *)desiredRange
           forInputWithTimeRange:(CMTimeRange)inputTimeRange
                           error:(NSError **)error {
  *desiredRange = inputTimeRange;
  return YES;
}

- (BOOL)setupAnalysisForTimeRange:(CMTimeRange)analysisRange
                    frameDuration:(CMTime)frameDuration
                            error:(NSError **)error {
  gPHYXSharedData *data = [self getSharedData:analysisRange.start];
  NSLog(@"[gPHYX] 🟢 setupAnalysisForTimeRange (ID: %@)",
        [self getInstanceID:analysisRange.start]);
  [data.homographyCache removeAllObjects];
  data.isTracking = YES;
  [self updateStatus:@"🟠 Tracking in progress..."];
  return YES;
}

- (BOOL)analyzeFrame:(FxImageTile *)frame
              atTime:(CMTime)frameTime
               error:(NSError **)error {
  gPHYXSharedData *data = [self getSharedData:frameTime];
  if (!data.referenceBuffer) {
    NSLog(@"[gPHYX] AnalyzeFrame: No reference buffer for ID %@. Cannot track.",
          [self getInstanceID:frameTime]);
    return YES;
  }

  CGRect roiRect = [self calculateROIRectAtTime:frameTime];
  IOSurfaceRef surface = (__bridge IOSurfaceRef)frame.ioSurface;
  CVPixelBufferRef currentBuffer = NULL;
  CVReturn status = CVPixelBufferCreateWithIOSurface(
      kCFAllocatorDefault, surface, NULL, &currentBuffer);

  if (status == kCVReturnSuccess && currentBuffer) {
    NSArray<NSNumber *> *matrix =
        [_visionTracker estimateHomographyFrom:currentBuffer
                                            to:data.referenceBuffer
                                           roi:roiRect];
    if (matrix.count == 9) {
      float h[9];
      for (int i = 0; i < 9; i++)
        h[i] = [matrix[i] floatValue];

      // Use tick-based key to avoid precision issues
      NSNumber *key = [NSNumber numberWithLongLong:frameTime.value];
      [data.homographyCache setObject:[NSData dataWithBytes:h length:sizeof(h)]
                               forKey:key];
    }
    CFRelease(currentBuffer);
  }
  return YES;
}

- (BOOL)cleanupAnalysis:(NSError **)error {
  gPHYXSharedData *data = [self getSharedData:kCMTimeZero];
  data.isTracking = NO;
  NSLog(@"[gPHYX] 🏁 cleanupAnalysis (ID: %@)",
        [self getInstanceID:kCMTimeZero]);
  [self updateStatus:@"✅ Tracking Completed"];
  return YES;
}

- (void)fitROIAction {
  NSLog(@"[gPHYX] Fit ROI Action triggered...");

  // Try to get BBox from Path API (Bezier) first
  double minX = 1.0, minY = 1.0, maxX = 0.0, maxY = 0.0;
  BOOL found = NO;

  id<FxPathAPI_v3> pathAPI =
      [_apiManager apiForProtocol:@protocol(FxPathAPI_v3)];
  id<FxParameterRetrievalAPI_v6> paramGet =
      [_apiManager apiForProtocol:@protocol(FxParameterRetrievalAPI_v6)];

  if (pathAPI && paramGet) {
    FxPathID pathID = 0;
    if ([paramGet getPathID:&pathID
              fromParameter:kParam_ShowOSC
                     atTime:kCMTimeZero]) {
      NSUInteger numVertices = 0;
      if ([pathAPI numberOfVertices:&numVertices
                             inPath:pathID
                             atTime:kCMTimeZero
                              error:nil] &&
          numVertices > 0) {
        for (NSUInteger i = 0; i < numVertices; i++) {
          FxVertex vertex;
          if ([pathAPI vertex:&vertex
                      atIndex:i
                       ofPath:pathID
                       atTime:kCMTimeZero
                        error:nil]) {
            if (vertex.location.x < minX)
              minX = vertex.location.x;
            if (vertex.location.x > maxX)
              maxX = vertex.location.x;
            if (vertex.location.y < minY)
              minY = vertex.location.y;
            if (vertex.location.y > maxY)
              maxY = vertex.location.y;
            found = YES;
          }
        }
      }
    }
  }

  // Fallback to OSC points if Bezier is empty
  if (!found && _osc) {
    std::vector<BezierControlPoint> &nodes = [_osc getCppNodes];
    if (!nodes.empty()) {
      for (const auto &pt : nodes) {
        if (pt.anchor.x < minX)
          minX = pt.anchor.x;
        if (pt.anchor.x > maxX)
          maxX = pt.anchor.x;
        if (pt.anchor.y < minY)
          minY = pt.anchor.y;
        if (pt.anchor.y > maxY)
          maxY = pt.anchor.y;
      }
      found = YES;
    }
  }

  if (!found) {
    NSLog(@"[gPHYX] Warning: No mask points found. Using default center ROI.");
    [self setROIWithX1:0.25 Y1:0.25 X2:0.75 Y2:0.75];
    return;
  }

  // 3. Apply 10% Margin
  double width = maxX - minX;
  double height = maxY - minY;
  double margin = 0.10;

  double expandedMinX = MAX(0.0, minX - width * margin);
  double expandedMaxX = MIN(1.0, maxX + width * margin);
  double expandedMinY = MAX(0.0, minY - height * margin);
  double expandedMaxY = MIN(1.0, maxY + height * margin);

  NSLog(@"[gPHYX] Auto-ROI: [%.2f, %.2f, %.2f, %.2f] -> [%.2f, %.2f, %.2f, "
        @"%.2f]",
        minX, minY, maxX, maxY, expandedMinX, expandedMinY, expandedMaxX,
        expandedMaxY);

  [self setROIWithX1:expandedMinX
                  Y1:expandedMinY
                  X2:expandedMaxX
                  Y2:expandedMaxY];
}

- (void)setROIWithX1:(float)x1 Y1:(float)y1 X2:(float)x2 Y2:(float)y2 {
  id<FxParameterSettingAPI_v6> setter =
      [_apiManager apiForProtocol:@protocol(FxParameterSettingAPI_v6)];
  if (setter) {
    // Use kCMTimeInvalid or kCMTimeZero to set value.
    // Note: setting at kCMTimeZero might affect the whole clip if no
    // keyframes exist.
    [setter setFloatValue:x1 toParameter:kParam_ROIX1 atTime:kCMTimeZero];
    [setter setFloatValue:y1 toParameter:kParam_ROIY1 atTime:kCMTimeZero];
    [setter setFloatValue:x2 toParameter:kParam_ROIX2 atTime:kCMTimeZero];
    [setter setFloatValue:y2 toParameter:kParam_ROIY2 atTime:kCMTimeZero];
  } else {
    NSLog(@"[gPHYX] Error: Could not get FxParameterSettingAPI_v6");
  }
}

#pragma mark - FxTileableEffect Implementation

- (BOOL)parameterChanged:(UInt32)paramID
                  atTime:(CMTime)time
                   error:(NSError **)error {
  NSLog(@"[gPHYX] 🔄 parameterChanged: %u", (unsigned int)paramID);

  if (paramID == kParam_ShowOSC) {
    id<FxParameterRetrievalAPI_v6> paramGet =
        [_apiManager apiForProtocol:@protocol(FxParameterRetrievalAPI_v6)];
    if (paramGet) {
      BOOL show;
      [paramGet getBoolValue:&show fromParameter:kParam_ShowOSC atTime:time];
      _osc.hidden = !show;
      NSLog(@"[gPHYX] 👁️ OSC visibility: %@", show ? @"SHOWN" : @"HIDDEN");
    }
  }
  return YES;
}

- (BOOL)pluginState:(NSData *_Nonnull *_Nullable)pluginState
             atTime:(CMTime)renderTime
            quality:(FxQuality)qualityLevel
              error:(NSError **)error {
  *pluginState = [NSData data];
  return YES;
}

- (BOOL)destinationImageRect:(FxRect *)destinationImageRect
                sourceImages:(NSArray<FxImageTile *> *)sourceImages
            destinationImage:(FxImageTile *)destinationImage
                 pluginState:(NSData *)pluginState
                      atTime:(CMTime)renderTime
                       error:(NSError **)outError {
  if (sourceImages.count > 0) {
    *destinationImageRect = sourceImages[0].imagePixelBounds;
  }
  return YES;
}

- (BOOL)sourceTileRect:(FxRect *)sourceTileRect
       sourceImageIndex:(NSUInteger)sourceImageIndex
           sourceImages:(NSArray<FxImageTile *> *)sourceImages
    destinationTileRect:(FxRect)destinationTileRect
       destinationImage:(FxImageTile *)destinationImage
            pluginState:(NSData *)pluginState
                 atTime:(CMTime)renderTime
                  error:(NSError **)outError {
  if (sourceImages.count > 0) {
    *sourceTileRect = sourceImages[0].imagePixelBounds;
  }
  return YES;
}

- (void)checkForUpdatedTrackingData:(NSString *)instanceID {
  gPHYXSharedData *data = [self getSharedData:kCMTimeZero];
  if (!data)
    return;

  NSString *jsonPath =
      [NSString stringWithFormat:@"/tmp/gPHYX_mask_%@.json", instanceID];
  NSFileManager *fm = [NSFileManager defaultManager];

  if (![fm fileExistsAtPath:jsonPath])
    return;

  NSDictionary *attrs = [fm attributesOfItemAtPath:jsonPath error:nil];
  NSDate *modDate = [attrs fileModificationDate];
  NSTimeInterval modStamp = [modDate timeIntervalSince1970];

  if (modStamp > data.lastJSONLoadTime) {
    NSLog(@"[gPHYX] 📂 New tracking data detected for %@. Loading...",
          instanceID);

    NSData *jsonData = [NSData dataWithContentsOfFile:jsonPath];
    if (!jsonData)
      return;

    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:jsonData
                                                         options:0
                                                           error:nil];
    if (json && [json[@"points"] isKindOfClass:[NSArray class]]) {
      NSMutableArray *newPoints = [NSMutableArray array];
      for (NSDictionary *pt in json[@"points"]) {
        double x = [pt[@"x"] doubleValue];
        double y = [pt[@"y"] doubleValue];
        [newPoints addObject:[NSValue valueWithPoint:NSMakePoint(x, y)]];
      }
      data.maskPoints = newPoints;
      data.lastJSONLoadTime = modStamp;
      NSLog(@"[gPHYX] ✅ Loaded %lu points from JSON",
            (unsigned long)newPoints.count);
    }
  }
}

- (BOOL)renderDestinationImage:(FxImageTile *)destinationImage
                  sourceImages:(NSArray<FxImageTile *> *)sourceImages
                   pluginState:(NSData *)pluginState
                        atTime:(CMTime)renderTime
                         error:(NSError **)outError {

  NSString *iid = [self getInstanceID:renderTime];
  [self checkForUpdatedTrackingData:iid];

  gPHYXSharedData *data = [self getSharedData:renderTime];
  if (data) {
    if (_osc.maskPoints != data.maskPoints) {
      _osc.maskPoints = data.maskPoints;
      // КРИТИЧНО: Принудительно уведомляем хост, что OSC кастомный и должен
      // быть перерисован Это заставляет Motion/FCP вызвать drawOSCWithWidth
    }
  }

  // NSLog(@"[gPHYX] renderDestinationImage called, sourceImages=%lu", (unsigned
  // long)sourceImages.count);

  if (sourceImages.count == 0) {
    NSLog(@"[gPHYX] WARNING: No source images!");
    return YES;
  }
  FxImageTile *inputTile = sourceImages[0];

  // --- FAIL-SAFE: Always copy input to output FIRST to prevent black screen
  // ---
  NSLog(@"[gPHYX] Copying input -> output (fail-safe)");
  IOSurfaceRef srcRef = (__bridge IOSurfaceRef)inputTile.ioSurface;
  IOSurfaceRef dstRef = (__bridge IOSurfaceRef)destinationImage.ioSurface;

  if (srcRef && dstRef) {
    IOSurfaceLock(srcRef, kIOSurfaceLockReadOnly, NULL);
    IOSurfaceLock(dstRef, 0, NULL);
    void *srcBase = IOSurfaceGetBaseAddress(srcRef);
    void *dstBase = IOSurfaceGetBaseAddress(dstRef);
    if (srcBase && dstBase) {
      size_t srcSize = IOSurfaceGetAllocSize(srcRef);
      size_t dstSize = IOSurfaceGetAllocSize(dstRef);
      memcpy(dstBase, srcBase, (srcSize < dstSize) ? srcSize : dstSize);
    }

    // --- CLEAN PLATE: Capture Reference Frame if requested
    if (_shouldInitTracking) {
      _shouldInitTracking = NO;
      CVPixelBufferRef pref = NULL;
      CVPixelBufferCreateWithIOSurface(kCFAllocatorDefault, srcRef, NULL,
                                       &pref);
      [data setReferenceBuffer:(CVPixelBufferRef)pref];
      if (pref)
        CFRelease(pref);
      NSLog(@"[gPHYX] Reference Frame Captured for ID %@.",
            [self getInstanceID:renderTime]);
    }

    // --- CLEAN PLATE: Plan Homography
    BOOL hFound = NO;
    // Try tick-based lookup
    NSData *cachedH = [data.homographyCache
        objectForKey:[NSNumber numberWithLongLong:renderTime.value]];

    // Fallback: search by small tolerance
    if (!cachedH) {
      double currentSec = CMTimeGetSeconds(renderTime);
      for (NSNumber *key in [data.homographyCache allKeys]) {
        // We don't know the exact timescale stored, but we assume it's
        // consistent with the project. If not, we'd need to store the timescale
        // too. For now, assume renderTime.timescale is the one.
        double keySec =
            (double)[key longLongValue] / (double)renderTime.timescale;
        if (fabs(keySec - currentSec) < 0.0001) {
          cachedH = [data.homographyCache objectForKey:key];
          // NSLog(@"[gPHYX] 🎯 Cache hit via tolerance: %.4f vs %.4f", keySec,
          // currentSec);
          break;
        }
      }
    }

    if (cachedH) {
      memcpy(_homography, [cachedH bytes], sizeof(float) * 9);
      hFound = YES;
    } else if (data.homographyCache.count > 0) {
      // NSLog(@"[gPHYX] ❓ Cache miss for time %.4f (Cache size: %lu)",
      //      CMTimeGetSeconds(renderTime), (unsigned
      //      long)data.homographyCache.count);
    }

    if ((_isTracking || data.isTracking) && data.referenceBuffer && !hFound) {
      CVPixelBufferRef currentBuffer = NULL;
      CVPixelBufferCreateWithIOSurface(kCFAllocatorDefault, srcRef, NULL,
                                       &currentBuffer);
      if (currentBuffer) {
        CGRect roiRect = [self calculateROIRectAtTime:renderTime];
        NSArray<NSNumber *> *matrix =
            [_visionTracker estimateHomographyFrom:currentBuffer
                                                to:data.referenceBuffer
                                               roi:roiRect];
        for (int i = 0; i < 9; i++) {
          _homography[i] = [matrix[i] floatValue];
        }

        [data.homographyCache
            setObject:[NSData dataWithBytes:_homography
                                     length:sizeof(_homography)]
               forKey:@(renderTime.value)];

        // --- Progress Bar Logic ---
        id<FxTimingAPI_v4> timingAPI =
            [_apiManager apiForProtocol:@protocol(FxTimingAPI_v4)];
        if (timingAPI) {
          CMTime startTime, duration;
          [timingAPI startTimeOfInputToFilter:&startTime];
          [timingAPI durationTimeOfInputToFilter:&duration];

          double startSec = CMTimeGetSeconds(startTime);
          double durationSec = CMTimeGetSeconds(duration);
          double currentSec = CMTimeGetSeconds(renderTime);

          if (durationSec > 0) {
            float percent = ((currentSec - startSec) / durationSec) * 100.0f;
            if (percent < 0)
              percent = 0;
            if (percent > 100)
              percent = 100;

            NSString *progressTxt = [NSString
                stringWithFormat:@"🟢 Tracking: %@",
                                 [self getProgressBarForPercent:percent]];
            [self updateStatus:progressTxt];
          }
        }
        CFRelease(currentBuffer);
      }
    }

    // Pass the calculated homography to OSC for drawing
    if (_osc) {
      [_osc setDrawHomography:_homography];
    }

    IOSurfaceUnlock(srcRef, kIOSurfaceLockReadOnly, NULL);
  }

  if (_shouldInpaint && _inpaintPipeline) {
    _shouldInpaint = NO;
    NSLog(@"[gPHYX] Metal Inpainting triggered...");

    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    id<MTLComputeCommandEncoder> encoder =
        [commandBuffer computeCommandEncoder];
    [encoder setComputePipelineState:_inpaintPipeline];

    // Create textures from IOSurfaces
    IOSurfaceRef srcSurface = (__bridge IOSurfaceRef)[inputTile ioSurface];
    IOSurfaceRef dstSurface =
        (__bridge IOSurfaceRef)[destinationImage ioSurface];

    MTLTextureDescriptor *texDesc = [MTLTextureDescriptor
        texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA16Float
                                     width:IOSurfaceGetWidth(srcSurface)
                                    height:IOSurfaceGetHeight(srcSurface)
                                 mipmapped:NO];
    texDesc.usage = MTLTextureUsageShaderRead | MTLTextureUsageShaderWrite;

    id<MTLTexture> srcTex = [_device newTextureWithDescriptor:texDesc
                                                    iosurface:srcSurface
                                                        plane:0];

    MTLTextureDescriptor *dstDesc = [MTLTextureDescriptor
        texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA16Float
                                     width:IOSurfaceGetWidth(dstSurface)
                                    height:IOSurfaceGetHeight(dstSurface)
                                 mipmapped:NO];
    dstDesc.usage = MTLTextureUsageShaderRead | MTLTextureUsageShaderWrite;

    id<MTLTexture> dstTex = [_device newTextureWithDescriptor:dstDesc
                                                    iosurface:dstSurface
                                                        plane:0];

    // Rasterize mask from OSC (Path API)
    NSLog(@"[gPHYX] Requesting mask texture from OSC (%lu x %lu)",
          (unsigned long)dstTex.width, (unsigned long)dstTex.height);
    id<MTLTexture> maskTex = [_osc getMaskTextureForDevice:_device
                                                     width:dstTex.width
                                                    height:dstTex.height
                                                apiManager:_apiManager
                                                    atTime:renderTime];

    if (!maskTex) {
      NSLog(@"[gPHYX] Warning: OSC returned nil mask. Creating fallback.");
      // Fallback: Empty mask (black)
      MTLTextureDescriptor *maskDesc = [MTLTextureDescriptor
          texture2DDescriptorWithPixelFormat:MTLPixelFormatR8Unorm
                                       width:dstTex.width
                                      height:dstTex.height
                                   mipmapped:NO];
      maskTex = [_device newTextureWithDescriptor:maskDesc];
    } else {
      NSLog(@"[gPHYX] Mask texture received.");
    }

    // Reference texture prioritized:
    // 1. External Drop Zone Image (if available) -> sourceImages[1]
    // 2. Internal Captured Reference Frame (_referenceBuffer)
    // 3. Current Source Frame (Fallback)
    id<MTLTexture> refTex = srcTex;

    if (sourceImages.count > 1) {
      // If scheduleInputs worked, Drop Zone image is here
      FxImageTile *dropZoneTile = sourceImages[1];
      IOSurfaceRef dropSurface =
          (__bridge IOSurfaceRef)[dropZoneTile ioSurface];
      MTLTextureDescriptor *dropDesc = [MTLTextureDescriptor
          texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA16Float
                                       width:IOSurfaceGetWidth(dropSurface)
                                      height:IOSurfaceGetHeight(dropSurface)
                                   mipmapped:NO];
      refTex = [_device newTextureWithDescriptor:dropDesc
                                       iosurface:dropSurface
                                           plane:0];
      NSLog(@"[gPHYX] Using Drop Zone image for inpainting");
    } else if (data.referenceBuffer) {
      IOSurfaceRef refSurface = CVPixelBufferGetIOSurface(data.referenceBuffer);
      MTLTextureDescriptor *refDesc = [MTLTextureDescriptor
          texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA16Float
                                       width:IOSurfaceGetWidth(refSurface)
                                      height:IOSurfaceGetHeight(refSurface)
                                   mipmapped:NO];
      refTex = [_device newTextureWithDescriptor:refDesc
                                       iosurface:refSurface
                                           plane:0];
      NSLog(@"[gPHYX] Using Shared Internal Reference Frame");
    }

    // Homography Matrix (Buffer 0)
    [encoder setBytes:_homography length:sizeof(_homography) atIndex:0];

    [encoder setTexture:srcTex atIndex:0];
    [encoder setTexture:dstTex atIndex:1];
    [encoder setTexture:maskTex atIndex:2];
    [encoder setTexture:refTex atIndex:3];

    MTLSize threadGroupSize = MTLSizeMake(16, 16, 1);
    MTLSize threadGroups = MTLSizeMake(
        (dstTex.width + threadGroupSize.width - 1) / threadGroupSize.width,
        (dstTex.height + threadGroupSize.height - 1) / threadGroupSize.height,
        1);

    [encoder dispatchThreadgroups:threadGroups
            threadsPerThreadgroup:threadGroupSize];
    [encoder endEncoding];
    [commandBuffer commit];
    [commandBuffer waitUntilCompleted];
    NSLog(@"[gPHYX] Metal Clean Plate Inpainting completed.");
  }

  return YES;
}

- (BOOL)scheduleInputs:(NSArray<FxImageTileRequest *> *_Nullable *_Nullable)
                           inputImageRequests
       withPluginState:(NSData *)pluginState
                atTime:(CMTime)renderTime
                 error:(NSError **)outError {

  // Request main clip
  FxImageTileRequest *clipReq = [[FxImageTileRequest alloc]
      initWithSource:kFxImageTileRequestSourceEffectClip
                time:renderTime
      includeFilters:YES
         parameterID:0];

  NSMutableArray *requests = [NSMutableArray arrayWithObject:clipReq];

  // Also request Drop Zone frame if needed
  // We request frame 0 of the Drop Zone (static image)
  FxImageTileRequest *dropReq = [[FxImageTileRequest alloc]
      initWithSource:kFxImageTileRequestSourceParameter
                time:kCMTimeZero
      includeFilters:YES
         parameterID:kParam_DropZone];
  [requests addObject:dropReq];

  if (inputImageRequests) {
    *inputImageRequests = requests;
  }

  return YES;
}

@end
