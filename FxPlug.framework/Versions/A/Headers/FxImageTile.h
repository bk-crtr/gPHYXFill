/*!
 @header        FxImageTile.h
 @abstract      Defines an image tile object that is passed between an out-of-process FxPlug plug-in
                and a host application.
 */
/* Copyright © 2019-2025 Apple Inc. All rights reserved. */

#import <IOSurface/IOSurfaceObjC.h>
#import <OpenGL/OpenGL.h>
#import <Metal/Metal.h>

#import <FxPlug/FxTypes.h>
#import <FxPlug/FxMatrix.h>
#import <FxPlug/FxImageTileRequest.h>

/*!
    @class      FxImageTile
    @abstract   An image tile object that is passed between an out-of-process FxPlug plug-in and a
                host application.
    @discussion An FxTileableEffect will receive all images as FxImageTile objects. These are
                passed to effects to describe tiles within the full image that need to be
                rendered. The image data is contained in the ioSurface property. This property may
                be nil during calls to the plug-in to gather information rather than rendering.
 */
@interface FxImageTile : NSObject <NSSecureCoding>

// The pixel bounds of this tile
@property (readonly, nonatomic) FxRect tilePixelBounds;

// The pixel bounds of the entire image
@property (readonly, nonatomic) FxRect imagePixelBounds;

// The transformation between pixel units and idealized, 100% scale, square pixel image units
@property (readonly, nonatomic, copy) FxMatrix44*  pixelTransform;

// The inverse of the pixel transform
@property (readonly, nonatomic, copy) FxMatrix44*  inversePixelTransform;

// The IOSurface object (can be cast to an IOSurfaceRef) - may be nil during non-render calls
@property (readonly, nonatomic) IOSurface* ioSurface;

// The registryID of the Metal device or OpenGL renderer used to create this texture
@property (readonly, nonatomic) uint64_t   deviceRegistryID;

// Which field of the image this is
@property (readonly, nonatomic) FxField field;

// What the field order of the image is
@property (readonly, nonatomic) FxFieldOrder   fieldOrder;

// The origin of the image tile
@property (readonly, nonatomic) FxImageOrigin  imageOrigin;

// The stereoscopic eye type of the image tile
@property (readonly, nonatomic) FxEyeType  eyeType;

// Whether this image tile is from an effect source or an image well parameter
@property (readonly, nonatomic) FxImageTileRequestSource imageSource;

// The parameter ID of the image well parameter of the source of this image tile. 0 if
// it is the effect source and not from a parameter
@property (readonly, nonatomic) UInt32 parameterID;

// The media time of this image tile
@property (readonly, nonatomic) CMTime mediaTime;

// Any errors that occurred from the image request. Can be nil if there were no errors
// or errors are not applicable
@property (readonly, nonatomic, copy) NSError* requestError;

// The CGColorSpaceRef that matches the color space used in creating the image
@property (readonly, nonatomic) CGColorSpaceRef colorSpace;

// Retrieve an OpenGL texture from the IOSurface for rendering with the passed-in context.
// You are responsible for deleting this texture.
- (GLuint)openGLTextureForContext:(CGLContextObj)openGLContext
__attribute__((deprecated("OpenGL is deprecated. Please update your plug-ins to work with Metal")));

// Retrieve a Metal texture from the IOSurface for rendering on the passed-in device
// The returned texture is autoreleased
- (id<MTLTexture>)metalTextureForDevice:(id<MTLDevice>)metalDevice;

// The scale values. NOTE: The pixel aspect ratio is not taken into account when the scaleX & scaleY
// values are computed.
@property (readonly, nonatomic) double  scaleX
__attribute__ ((deprecated("Use pixelTransform and inversePixelTransform instead.")));
@property (readonly, nonatomic) double  scaleY
__attribute__ ((deprecated("Use pixelTransform and inversePixelTransform instead.")));

// The pixel aspect ratio.
@property (readonly, nonatomic) double  pixelAspect
__attribute__ ((deprecated("Use pixelTransform and inversePixelTransform instead.")));

@end
