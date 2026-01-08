/*!
	@header		FxTypes.h
	
	@abstract	Defines common structures and enumerated types.
	@discussion	This header file provides common definitions used by other header files in the
				FxPlug SDK.
*/

/* Copyright © 2005-2025 Apple Inc. All rights reserved. */

#ifndef __FXTYPES_H__
#define __FXTYPES_H__

#import <Foundation/Foundation.h>
#if STUDIO_LITE
#import <CoreGraphics/CoreGraphics.h>
#else
#import <OpenGL/OpenGL.h>
#endif
#import <CoreMedia/CMTime.h>

/**** PROPERTIES KEYS ***/

/*!
    @const        kFxPropertyKey_MayRemapTime
    @abstract    Key for properties dictionary
    @discussion    The value for this key is a Boolean NSNumber indicating whether or not this plug-in
                may try to sample its input image(s) at any time other than the output time. The
                default value is YES.
*/
#define kFxPropertyKey_MayRemapTime                @"MayRemapTime"

/*!
    @const      kFxPropertyKey_UsesNonmatchingTextureLayout
    @abstract   Whether your plug-in uses textures with one pixel layout as if they were
                a different pixel layout, such as putting 4:2:2 pixels into an RGBA buffer.
    @discussion If you use a single pixel in a texture to store more than 1 pixel, set this flag
                to YES. Otherwise set it to NO. In FxPlug 3 the default is YES, whereas in
                FxPlug 4 the default is NO.
 */
#define kFxPropertyKey_UsesNonmatchingTextureLayout @"UsesNonmatchingTextureLayout"

/*!
    @constant   kFxPropertyKey_DrawsInScreenSpace   Tells the host application that your plug-in
                                                    draws in screen space - it is always directly
                                                    in front of the camera and facing the camera
*/
#define kFxPropertyKey_DrawsInScreenSpace   @"TransformsFromLocalToScreenSpace"

/*!
	@enum FxQuality
	@discussion Constants used to identify rendering quality.
	@constant kFxQuality_LOW		Lowest quality, fastest render.
	@constant kFxQuality_MEDIUM		Medium quality, medium render.
	@constant kFxQuality_HIGH		High quality, slowest render.
*/
enum {
	kFxQuality_LOW    = 0,
	kFxQuality_MEDIUM = 1,
	kFxQuality_HIGH   = 2
};
typedef NSUInteger FxQuality;


/*!
	@enum FxField
	@discussion Constants used to identify a field. NOTE: Before FxPlug 1.2, this type was used to
				identify field order. As of 1.2, it is used to identify the field of an image.
	@constant kFxField_NONE		Full frame, progressive (no fields).
	@constant kFxField_UPPER	Upper field.
	@constant kFxField_LOWER	Lower field.
*/
enum {
    kFxField_NONE  = 0,
    kFxField_UPPER = 1,
    kFxField_LOWER = 2
};
typedef NSUInteger FxField;

/*!
	@enum FxFieldOrder
	@discussion Constants used to identify the field order of an image stream.
	@constant kFxFieldOrder_PROGRESSIVE		Full frame, progressive (no fields).
	@constant kFxFieldOrder_UPPER_FIRST		Upper field first.
	@constant kFxFieldOrder_LOWER_FIRST		Lower field first.
*/
enum {
    kFxFieldOrder_PROGRESSIVE = 0,
    kFxFieldOrder_UPPER_FIRST = 1,
    kFxFieldOrder_LOWER_FIRST = 2
};
typedef NSUInteger FxFieldOrder;



/*!
	@enum FxDepth
	@discussion Constants used to identify bit depth.
	@constant kFxDepth_UINT8	Each component is an 8-bit integer.
	@constant kFxDepth_FLOAT16	Each component is a 16-bit float.
	@constant kFxDepth_FLOAT32	Each component is a 32-bit float.
*/
enum {
	kFxDepth_UINT8   = 0,
	kFxDepth_FLOAT16 = 2,
	kFxDepth_FLOAT32 = 3
};
typedef NSUInteger FxDepth;

/*!
    @typedef    FxPoint2D
    @discussion A 2D point representation.
*/
typedef CGPoint FxPoint2D;


/*!
    @typedef    FxSize
    @discussion A structure to store width and height values.
*/
typedef CGSize FxSize;


/*!
    @struct     FxPoint3D
    @discussion A 3D point representation.
    @field      x   The x coordinate of the 3D point.
    @field      y   The y coordinate of the 3D point.
    @field      z   The z coordinate of the 3D point.
*/

typedef struct FxPoint3D {
    CGFloat x;
    CGFloat y;
    CGFloat z;
} FxPoint3D;


/*!
    @struct     FxRect
    @discussion A 2D axis-aligned rectangle with integer coordinates.
*/

typedef struct FxRect {
    SInt32  left;
    SInt32  bottom;
    SInt32  right;
    SInt32  top;
} FxRect;

extern FxRect kFxRect_Infinite;
extern FxRect kFxRect_Empty;

bool FxRectsAreEqual(FxRect thisRect, FxRect thatRect);


/*!
    @enum       FxKeyframeStyle
    @discussion Constants for specifying the type of keyframe
    @constant   kFxKeyframeStyle_Constant   Also known as "Hold," constant keyframes have a constant 
                                            value between keyframes.
    @constant   kFxKeyframeStyle_Linear     Values are interpolated along a straight line between 
                                            keyframes
    @constant   kFxKeyframeStyle_Quadratic  Values are interpolated along a quadratic curve
    @constant   kFxKeyframeStyle_Cubic      Values are interpolated along a cubic curve
    @constant   kFxKeyframeStyle_Bezier     Values are interpolated along a cubic curve the user
                                            can control by varying the tangent handles
    @constant   kFxKeyframeStyle_EaseIn     Values are interpolated along a precalculated cubic
                                            curve that smoothly transitions into the next value
    @constant   kFxKeyframeStyle_EaseOut    Values are interpolated along a precalculated cubic
                                            curve that smoothly transitions out of the previous 
                                            value
    @constant   kFxKeyframeStyle_BSpline    Values are interpolated along a B-Spline
    @constant   kFxKeyframeStyle_XSpline    Values are interpolated along a weighted X-Spline
    @constant   kFxKeyframeStyle_Exponential    Values are interpolated along an exponential curve
    @constant   kFxKeyframeStyle_Logarithmic    Values are interpolated along a logarithmic curve
 
    @constant   kFxKeyframeStyle_Mixed      Keyframe is a mixture of 2 of the above types
    @constant   kFxKeyframeStyle_All        A mask for all types
*/

enum {
    kFxKeyframeStyle_Constant   = 0x0001,
    kFxKeyframeStyle_Linear     = 0x0002,
    kFxKeyframeStyle_Quadratic  = 0x0004,
    kFxKeyframeStyle_Cubic      = 0x0008,
    kFxKeyframeStyle_Bezier     = 0x0010,
    kFxKeyframeStyle_EaseIn     = 0x0080,
    kFxKeyframeStyle_EaseOut    = 0x0100,
    kFxKeyframeStyle_BSpline    = 0x0400,
    kFxKeyframeStyle_XSpline    = 0x1000,
    kFxKeyframeStyle_Exponential= 0x2000,
    kFxKeyframeStyle_Logarithmic= 0x4000,
    
    kFxKeyframeStyle_Mixed  = 0x80000000,
    kFxKeyframeStyle_All    = 0x7fff,
};
typedef NSUInteger FxKeyframeStyle;

/*!
    @enum       FxKeyframeSegmentStyle
    @abstract   Styles for the segment between keyframes.
    @discussion Each style describes the segment between the keyframe you're working with and the next
                keyframe.
    @constant   kFxKeyframeSegmentStyle_Constant    A parameter value that is constant between
                this keyframe and the next one. At the next keyframe, its value will jump to the value
                at that keyframe.
    @constant   kFxKeyframeSegmentStyle_Linear  A parameter value that changes linearly between
                this keyframe and the next.
    @constant   kFxKeyframeSegmentStyle_Bezier  A parameter value that changes along a cubic
                Bézier curve defined by the keypoints and tangents in the FxKeyframe structure.
    @constant   kFxKeyframeSegmentStyle_Continuous  A parameter value that changes along a cubic
                Catmull-Rom spline defined by the the keypoints and tangents in the FxKeyframe structure.
    @constant   kFxKeyframeSegmentStyle_EaseInNext A parameter value that performs a gentle
                ease-in interpolation to the next keyframe value.
    @constant   kFxKeyframeSegmentStyle_EaseOutCurrent  A parameter value that performs a gentle
                ease-out interpolation from the current keyframe value.
    @constant   kFxKeyframeSegmentStyle_EaseOutCurrentEaseInNext    A parameter value that performs
                a gentle ease-out interpolation from the current keyframe value and a gentle ease-in
                interpolation to the next keyframe value.
    @constant   kFxKeyframeSegmentStyle_Exponential A parameter value that increases or decreases
                exponentially between the current keyframe and the next.
    @constant   kFxKeyframeSegmentStyle_Logarithmic A parameter value that increases or decreases
                logarithmically between the current keyframe and the next.
 */

typedef NS_ENUM(NSUInteger, FxKeyframeSegmentStyle)
{
    kFxKeyframeSegmentStyle_Constant                    = 0,
    kFxKeyframeSegmentStyle_Linear                      = 1,
    kFxKeyframeSegmentStyle_Bezier                      = 2,
    kFxKeyframeSegmentStyle_Continuous                  = 3,
    kFxKeyframeSegmentStyle_EaseInNext                  = 4,
    kFxKeyframeSegmentStyle_EaseOutCurrent              = 5,
    kFxKeyframeSegmentStyle_EaseOutCurrentEaseInNext    = 6,
    kFxKeyframeSegmentStyle_Exponential                 = 7,
    kFxKeyframeSegmentStyle_Logarithmic                 = 8
};

/*!
    @define     kFxKeyframe_V3
    @abstract   Defines a keyframe information structure as a version 3 structure.
*/
#define kFxKeyframe_V3  3

/*!
    @define     kFxKeyframe_CurrentVersion
    @abstract   Used to define a keyframe information structure as being the most current version.
    @discussion Introduced in FxPlug 4.
*/
#define kFxKeyframe_CurrentVersion  kFxKeyframe_V3

/*!
    @define     FxInitKeyframe
    @abstract   Initializes a keyframe information structure to be empty.
    @discussion Introduced in FxPlug 4.
*/
#define FxInitKeyframe(kf,vers) {\
    memset(&kf,0,sizeof(kf)); \
    kf.version = vers;\
}

/*!
    @struct     FxKeyframe
    @field      version         Which version of the keyframe structure this is.
    @field      time            The CMTime at which this information is valid.
    @field      segmentStyle    The type of segment between this keyframe and the next.
    @field      inTangentX      The incoming tangent's x coordinate.
    @field      inTangentY      The incoming tangent's y coordinate.
    @field      outTangentX     The outgoing tangent's x coordinate.
    @field      outTangentY     The outgoing tangent's y coordinate.
    @discussion Contains information about a single keyframe.
                Note that this is not an FxKeyframeInfo type and should only be used for
                FxPlug 4 style plug-ins.
*/
typedef struct FxKeyframe {
    NSUInteger              version;        // Version of this structure
    CMTime                  time;           // Time of the keyframe
    FxKeyframeSegmentStyle  segmentStyle;   // Type of segment between this keyframe and the next
    double                  inTangentX;     // The incoming tangent's x coord
    double                  inTangentY;     // The incoming tangent's y coord
    double                  outTangentX;    // The outgoing tangent's x coord
    double                  outTangentY;    // The outgoing tangent's y coord
} FxKeyframe;

/*!
    @enum FxParameterFlags
    @discussion Bit flags used to control characteristics of parameters.
    @const kFxParameterFlag_DEFAULT            Convenience constant. No flags set.
    @const kFxParameterFlag_NOT_ANIMATABLE    This parameter's value is constant across time and
                                            cannot be animated.
    @const kFxParameterFlag_HIDDEN            This parameter displays no UI to the user, but its value
                                            is saved/loaded. Note that this visibility control is
                                            independent of the DONT_DISPLAY_IN_DASHBOARD flag. It is
                                            possible to have a parameter that appears in the
                                            dashboard but not in the inspector, or vice versa.
    @const kFxParameterFlag_DISABLED        This parameter is dimmed and disabled.
    @const kFxParameterFlag_COLLAPSED        This Group parameter's subgroup is closed and its
                                            subparameters are not visible in the Inspector.
    @const kFxParameterFlag_DONT_SAVE        This parameter's values are not saved/loaded.
    @const kFxParameterFlag_DONT_DISPLAY_IN_DASHBOARD    This parameter is not visible in the
                                            dashboard controls.
    @const kFxParameterFlag_CUSTOM_UI        This parameter uses custom UI that replaces the standard
                                            parameter UI. The plug-in must provide a custom view for
                                            this parameter. (See the FxCustomParameterActionAPI and
                                            FxCustomParameterViewHost protocols, defined in
                                            FxCustomParameterUI.h.)
    @const kFxParameterFlag_IGNORE_MINMAX    This parameter's range is unbounded. Ignore its Min and
                                            Max values.
    @const  kFxParameterFlag_CURVE_EDITOR_HIDDEN    This parameter is not displayed in the keyframe
                                            editor
    @const  kFxParameterFlag_DONT_REMAP_COLORS    This parameter is a color channel which does not
                                            logically represent a color, and should not be color
                                            managed; its component values will be retrieved by the
                                            parameter API as-is without converting them to the
                                            working color space. (Note that this flag is ignored
                                            for all other channel types.)
    @const  kFxParameterFlag_USE_FULL_VIEW_WIDTH    Causes the inspector to use the full width for displaying this view. This
                                            flag is only valid for parameters that also set
                                            kFxParameterFlag_CUSTOM_UI.
    @discussion    Of course, not all of the flags apply meaningfully to all of the parameter types.
                The only flags, for example, that apply to Group parameters are COLLAPSED,
                DONT_DISPLAY_IN_DASHBOARD, HIDDEN, and DISABLED. You can't give a group parameter
                a custom UI.
*/
enum {
    kFxParameterFlag_DEFAULT                    = 0,
    kFxParameterFlag_NOT_ANIMATABLE                = 1 << 0,
    kFxParameterFlag_HIDDEN                        = 1 << 1,
    kFxParameterFlag_DISABLED                    = 1 << 2,
    kFxParameterFlag_COLLAPSED                    = 1 << 3,
    kFxParameterFlag_DONT_SAVE                    = 1 << 4,
    kFxParameterFlag_DONT_DISPLAY_IN_DASHBOARD    = 1 << 5,
    kFxParameterFlag_CUSTOM_UI                    = 1 << 6,
    kFxParameterFlag_IGNORE_MINMAX                = 1 << 8,
    kFxParameterFlag_CURVE_EDITOR_HIDDEN        = 1 << 9,
    kFxParameterFlag_DONT_REMAP_COLORS          = 1 << 10,
    kFxParameterFlag_USE_FULL_VIEW_WIDTH        = 1 << 11,
    
};
typedef UInt32 FxParameterFlags;

/*
    The following constant is provided for backward compatibility: FxPlug 1.0 had misnamed the flag
    that indicates that a group parameter is collapsed. New plug-ins should use _COLLAPSED rather
    than _EXPANDED.
*/
#define kFxParameterFlag_EXPANDED    kFxParameterFlag_COLLAPSED

/*!
    @constant   FxPlugErrorDomain
    @discussion The NSError error domain for FxPlug-related errors.
*/
extern NSString  *FxPlugErrorDomain;

/*!
    @enum       FxError
    @discussion Some common errors which can be returned by FxPlug hosts.
    @constant   kFxError_Success  No error occurred. The call succeeded.
    @constant   kFxError_InvalidParameterID             An invalid parameter ID was used
    @constant   kFxError_InvalidParameterChannelIndex   An invalid channel number for a parameter
                                                        was used
    @constant   kFxError_InvalidKeyframeIndex           An invalid index was used
    @constant   kFxError_InvalidTime                    An invalid time was used
    @constant   kFxError_InvalidParameter               A parameter sent to an API method was invalid
                                                        (nil pointer, object doesn't implement a
                                                        protocol, etc.)
    @constant   kFxError_OutOfMemory                    Unable to allocate memory
    @constant   kFxError_MemoryNotAllocated             Trying to free memory not allocated
    @constant   kFxError_OpenGLError                    OpenGL returned an error
    @constant   kFxError_InvalidPathID                  No path with that ID exists
    @constant   kFxError_InvalidPathIndex               The index is greater than the number of paths
    @constant   kFxError_InvalidSegmentIndex            The index of the segment of the path is
                                                        greater than the number of segments in the path
	@constant	kFxError_InvalidLightAccess				An attempt was made to access lights without
														first calling -beginLightAccess
	@constant	kFxError_InvalidPathStyle				An invalid path style was used
    @constant   kFxError_APIUnavailable                 The requested API cannot be used at this time
    @constant   kFxError_InvalidDataLength              An invalid data length was found while
                                                        decoding data from the remote side
    @constant   kFxError_PluginNotFound                 The plug-in could not be found by the host
    @constant   kFxError_NotYetImplemented              The API in question has not yet been
                                                        implemented in the host application
    @constant   kFxError_UnableToCreateDynamicRegistrationEndpoint
                                                        The XPC service was unable to create a new
                                                        XPC Listener Endpoint for communication
                                                        with the host application
    @constant   kFxError_UnableToInstantiateDynamicRegistrar
                                                        The XPC service was unable to instantiate
                                                        a dynamic registrar object either because
                                                        no class name was listed in the XPC's
                                                        Info.plist file, or because the class listed
                                                        was incorrect or had an error during
                                                        instantiation
 
    @constant   kFxError_LostConnectionToPlugin         The connection between the host and the plug-in was
                                            lost so the operation failed.
 
    @constant   kFxError_AnalysisError                          Return this error whenever something goes wrong with
                                            analysis
    @constant   kFxError_HostUnreachable                The host application cannot be reached to complete an operation
    @constant   kFxError_UserCancelled                      Return this error when you want to stop analysis, but don't wish
                                            to display an error alert to the user
    @constant   kFxError_NoMediaFolder                  The project has no media folder either because it hasn't been saved
                                            yet or because it was saved without the user checking "Collect Media"
    @constant   kFxError_NoDocumentFound        The FxPlug is not in a project. It may be loaded in memory but not applied,
                                            or it may be applied to a clip or other object that's not in a document,
                                            such as a compound clip in Final Cut Pro

    @constant   kFxError_UnableToMovePlayhead       Attempting to move the playhead failed
 
    @constant   kFxError_InvalidTiming                          Attempting to get FxTimingAPI info from the host failed
 
    @constant   kFxError_InvalidColorGamut                  Attempting to get FxColorGamutAPI info from the host failed

    @constant   kFxError_InvalidPaths                            Attempting to get FxPathAPI info from the host failed
        
    @constant   kFxError_CommandNotProcessed    Unable to complete the command sent to the FxCommandAPI
 
    @constant   kFxError_UnableToObtainProjectAspectRatio Unable to obtain the project's aspect ratio

    @constant   kFxError_ThirdPartyDeveloperStart       All 3rd party error values should be greater
                                                        than or equal to this value
*/
enum {
    kFxError_Success   = 0,
    
    // Various error codes we will define during development
    kFxError_InvalidParameterID,
    kFxError_InvalidParameterChannelIndex,
    kFxError_InvalidKeyframeIndex,
    kFxError_InvalidTime,
    kFxError_InvalidParameter,
    kFxError_OutOfMemory,
    kFxError_MemoryNotAllocated,
    kFxError_OpenGLError,
    kFxError_InvalidPathID,
    kFxError_InvalidPathIndex,
    kFxError_InvalidSegmentIndex,
	kFxError_InvalidLightAccess,
    kFxError_InvalidPathStyle,
    kFxError_APIUnavailable,
    kFxError_InvalidDataLength,
    kFxError_PluginNotFound,
    kFxError_AnalysisExtensionNotFound,
    kFxError_NotYetImplemented,
    kFxError_UnableToCreateDynamicRegistrationEndpoint,
    kFxError_UnableToInstantiateDynamicRegistrar,
    kFxError_LostConnectionToPlugin,
    kFxError_AnalysisError,
    kFxError_HostUnreachable,
    kFxError_UserCancelled,
    kFxError_NoMediaFolder,
    kFxError_NoDocumentFound,
    kFxError_UnableToMovePlayhead,
    kFxError_NoViewFound,
    kFxError_InvalidTiming,
    kFxError_InvalidColorGamut,
    kFxError_InvalidPaths,
    kFxError_CommandNotProcessed,
    kFxError_UnableToObtainProjectAspectRatio,
    
    // All 3rd party error values should be greater than or equal
    // to this value if none of the above error values are sufficient
    kFxError_ThirdPartyDeveloperStart  = 100000
};
typedef NSInteger  FxError;



/*! 
    @typedef    FxPathID
    @abstract   A unique identifier for a path
*/
typedef void*  FxPathID;

/*!
     @enum       FxHistogramChannel
     @discussion Identifies which channel a histogram operation should apply to
     @constant   kFxHistogramChannel_RGB     All 3 of the red, green, and blue channels
     @constant   kFxHistogramChannel_Red     The red channel
     @constant   kFxHistogramChannel_Green   The green channel
     @constant   kFxHistogramChannel_Blue    The blue channel
     @constant   kFxHistogramChannel_Alpha   The alpha channel
 */
enum {
    kFxHistogramChannel_RGB     = 0,
    kFxHistogramChannel_Red     = 1,
    kFxHistogramChannel_Green   = 2,
    kFxHistogramChannel_Blue    = 3,
    kFxHistogramChannel_Alpha   = 4
};
typedef NSUInteger  FxHistogramChannel;

/*!
     @enum       FxEyeType
     @discussion Identifies which eye is being sent from a stereoscopic project
     @constant   kFxEyeType_MONO     Monoscopic only
     @constant   kFxEyeType_LEFT     The left eye portion of the stereoscopic image
     @constant   kFxEyeType_RIGHT    The right eye portion of the stereoscopic image
 */
typedef enum
{
    kFxEyeType_MONO,
    kFxEyeType_LEFT,
    kFxEyeType_RIGHT
} FxEyeType;

/*!
    @enum       FxAnalysisLocation
    @discussion Whether you want to do analysis on the CPU or GPU
    @constant   kFxAnalysisLocation_GPU     Use a texture on the GPU for analysis images
    @constant   kFxAnalysisLocation_CPU     Use a bitmap in main memory for analysis images
 */
typedef NSInteger FxAnalysisLocation;
NS_ENUM(FxAnalysisLocation) {
    kFxAnalysisLocation_GPU = 0,
    kFxAnalysisLocation_CPU
};

/*!
    @enum FxImageOrigin
    
    @constant kFxImageOrigin_BOTTOM_LEFT    Right-handed "bottom-up" coordinate system where Y
                                            values increase upwardly.
    @constant kFxImageOrigin_TOP_LEFT        Left-handed "top-down" coordinate system where Y
                                            values increase downwardly.
    @discussion Identifies the orientation of an image's coordinate system. Images in Motion have
                a bottom-left origin. Images in Final Cut have a top-left origin. Introduced
                in FxImage version 0.
*/
enum {
    kFxImageOrigin_BOTTOM_LEFT = 0,
    kFxImageOrigin_TOP_LEFT = 2
};
typedef NSUInteger FxImageOrigin;


/*!
    @enum FxImageColorInfo

    @constant kFxImageColorInfo_RGB_LINEAR                                        RGB linear (not gamma corrected)
    @constant kFxImageColorInfo_RGB_GAMMA_VIDEO                                    RGB at video gamma (2.2)
    @discussion Identifies some color properties of an FxImage. These include the color space, and
                gamma level. Introduced in FxImage version 0.
*/
enum {
    kFxImageColorInfo_RGB_LINEAR = 0,
    kFxImageColorInfo_RGB_GAMMA_VIDEO = 2
};
typedef NSUInteger FxImageColorInfo;

/*!
 @const        kFxPropertyKey_DesiredProcessingColorInfo
 @abstract    Key for properties dictionary
 @discussion    The value for this key is an NSNumber indicating the colorspace
                that the plug-in would like to process in. This color space is
                expressed as an FxImageColorInfo enum. If a plug-in specifies this,
                and the host supports it, all inputs will be in this colorspace,
                and the output must also be in this colorspace. This may not
                be supported by all hosts, so the plug-in should still check
                the colorInfo of its input and output images.
 */
#define kFxPropertyKey_DesiredProcessingColorInfo    @"DesiredProcessingColorInfo"

/*!
 +    @typedef    FxColorPrimaries
 +    @discussion An enum that represents a set of color primaries
 +*/

enum {
    kFxColorPrimaries_Rec709 = 0,
    kFxColorPrimaries_Rec2020
};
typedef NSUInteger FxColorPrimaries;

/*!
    @enum   FxPixelTransformSupport
 
    @constant   kFxPixelTransform_Scale                 Handles only (non-uniform) scaling - this is
                                                        the minimum required
    @constant   kFxPixelTransform_ScaleTranslate        Handles non-uniform scaling, and arbitrary
                                                        translation
    @constont   kFxPixelTransform_Full                  Handles full perspective transforms
*/
enum {
    kFxPixelTransform_Scale             = 1,
    kFxPixelTransform_ScaleTranslate    = 3,
    kFxPixelTransform_Full              = 6
};
typedef NSUInteger FxPixelTransformSupport;

/*!
    @constant   kFxPropertyKey_PixelTransformSupport    Tells the host app which type of pixel
                                                        transformations your plug-in supports.
                                                        Defaults to kFxPixelTransform_ScaleTranslate.
*/
#define kFxPropertyKey_PixelTransformSupport  @"PixelTransformSupport"

#endif
