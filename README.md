# gPHYX Fill v2.28

**Advanced Object Removal & Inpainting Plugin for Final Cut Pro & Motion**

gPHYX Fill is a professional FxPlug 4 plugin that provides Mocha-style tracking and inpainting capabilities directly within Final Cut Pro and Apple Motion.

## Features

### ✨ External Editor
- **Professional Tracking Interface**: Standalone SwiftUI editor with full video playback
- **Interactive Mask Drawing**: Click to add points, drag to move, right-click to delete
- **Advanced Tracking Controls**:
  - Step-by-step tracking (forward/backward by 1 frame)
  - Continuous tracking (to end/beginning of video)
  - Full auto-tracking (forward then backward)
- **Undo/Redo Support**: Full Command+Z integration for all mask operations
- **Timeline Scrubber**: Navigate through video frames with precision

### 🎯 Tracking Engine
- **Vision Framework Integration**: Powered by Apple's VNTrackObjectRequest
- **Automatic Point Tracking**: Track mask points across video frames
- **ROI Optimization**: Configurable region of interest for better tracking accuracy

### 🎨 Inpainting
- **PatchMatch Algorithm**: Intelligent content-aware fill
- **Clean Plate Support**: Use reference frames for better results
- **Real-time Preview**: See results directly in Motion/FCP

## Installation

### Requirements
- macOS 12.0 or later
- Apple Motion (for framework dependencies)
- Xcode 14+ (for building from source)

### Quick Install
```bash
./install_to_system.sh
```

This script will:
1. Build the project with `xcodegen`
2. Embed required FxPlug frameworks
3. Install to `/Applications/gPHYXFill.app`
4. Register the plugin with PlugInKit

### Manual Installation
See [plugin-reliability.md](.agent/workflows/plugin-reliability.md) for detailed troubleshooting steps.

## Usage

1. **In Motion/FCP**: Apply "gPHYX Fill v2.28" effect from "gPHYX Final 228" category
2. **Set Source Video**: Point to your video file on disk
3. **Click "Open Editor"**: Launch the external tracking editor
4. **Draw Mask**: Click to add points around the object to remove
5. **Track**: Use tracking buttons to follow the object across frames
6. **Save**: Click "Save and Close" to apply the mask back to Motion/FCP

## Project Structure

```
gPHYXFill/
├── frontend/           # FxPlug XPC Service (Objective-C++)
│   ├── gPHYXFillEffect.mm    # Main plugin logic
│   ├── gPHYXOsc.mm           # On-Screen Control overlay
│   ├── InpaintKernel.metal   # Metal shader for inpainting
│   └── XPCInfo.plist         # Plugin metadata
├── editor/             # External SwiftUI Editor
│   ├── EditorApp.swift       # App entry point
│   └── ContentView.swift     # Main UI & ViewModel
├── backend/            # Python inpainting server
│   └── server.py             # PatchMatch implementation
├── templates/          # Motion Templates
└── project.yml         # XcodeGen configuration
```

## Technical Details

- **Bundle ID**: `com.gphyx.FillEffect228`
- **Plugin UUID**: `7088C7B4-B963-4E43-B64C-7D6AC367AA38`
- **FxPlug Version**: 4.0 (XPC-based)
- **Minimum Motion Version**: 5.0

## Known Issues

1. **Tracking Accuracy**: Points may not stick perfectly to fast-moving objects
2. **Mask Visibility**: Saved masks may not immediately appear in Motion (requires refresh)

See [user_requests_analysis.md](.gemini/antigravity/brain/.../user_requests_analysis.md) for full status.

## Development

### Building
```bash
xcodegen generate
xcodebuild -project gPHYXFill.xcodeproj -scheme gPHYXFill -configuration Debug build
```

### Debugging
- Check Console.app for logs prefixed with `[gPHYX]`
- Editor logs are prefixed with `[gPHYX Editor]`

## License

Proprietary - gPHYX Labs

## Version History

### v2.28 (Current)
- ✅ Fixed plugin visibility (XPC in XPCServices, not PlugIns)
- ✅ Added full tracking controls (step, continuous, auto)
- ✅ Implemented drag-and-drop for mask points
- ✅ Added Undo/Redo support
- ✅ Force-restart editor for fresh video loading
- ❌ Tracking accuracy needs improvement
- ❌ Mask visibility in Motion needs debugging

### v2.27
- Nuclear reset of Bundle IDs for cache invalidation
- New category: "gPHYX Final 228"

### v2.26 and earlier
- Initial external editor implementation
- Basic tracking and inpainting
