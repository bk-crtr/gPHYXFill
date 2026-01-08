/*!
	@header		FxOnScreenControlAPI.h
	@abstract	Defines the FxOnScreenControl host API protocol.
*/

/* Copyright © 2005-2025 Apple Inc. All rights reserved. */

#import <FxPlug/FxMatrix.h>
#import <FxPlug/FxOnScreenControl.h>

/*!
	@protocol   FxOnScreenControlAPI
	@abstract   Defines the methods the host application provides to manage conversions between
				various OnScreen coordinate spaces.
*/
@protocol FxOnScreenControlAPI

/*!
	@method		convertPointFromSpace:fromX:fromY:toSpace:toX:toY:
	@abstract   Converts a position from object space to canvas space.
	@param		fromSpace	The coordinate space to convert from.
	@param		fromX		X coordinate of the position to convert from.
	@param		fromY		Y coordinate of the position to convert from.
	@param		toSpace		The coordinate space to convert to.
	@param		toX			Pointer for the x coordinate of the computed position.
	@param		toY			Pointer for the y coordinate of the computed position.
*/
- (void)convertPointFromSpace:(FxDrawingCoordinates)fromSpace
						fromX:(double)fromX
						fromY:(double)fromY
					  toSpace:(FxDrawingCoordinates)toSpace
						  toX:(double *)toX
						  toY:(double *)toY;

@end


/*!
    @protocol   FxOnScreenControlAPI_v2
    @abstract   Provides additional methods for discovering information about the canvas
                and objects needed to draw onscreen controls.
*/
@protocol FxOnScreenControlAPI_v2 <FxOnScreenControlAPI>

/*!
    @method     canvasZoom
    @abstract   Returns the zoom factor the user has set the canvas to. Can generally range
                from 1% to 1600%.
*/
- (double)canvasZoom;

/*!
    @method     canvasPixelAspectRatio
    @abstract   Returns the pixel aspect ratio of the canvas. This can change if the user
                sets the canvas to be viewed with or without pixel aspect ratio correction, 
                so do not cache this value.
*/
- (double)canvasPixelAspectRatio;

/*!
    @method     objectBounds
    @abstract   Returns the bounds of the object that the onscreen control applies to. If it
                is a generator, then it will be the bounds of the generator. If it is a filter
                or transition then it will be the bounds of the input to the filter or transition.
*/
- (NSRect)objectBounds;

/*!
    @method     inputBounds
    @abstract   Returns the bounds of the input to the filter or transition.
*/
- (NSRect)inputBounds;

/*!
    @method     pixelAspectRatio
    @abstract   Returns the pixel aspect ratio of the object that the onscreen control applies to.
*/
- (double)pixelAspectRatio;

/*!
    @method     objectWidth:height:pixelAspectRatio:
    @abstract   Returns the width, height, and pixel aspect ratio of the object the onscreen 
                control is applied to.
    @param      outputWidth     receives the width of the object
    @param      outputHeight    receives the height of the object
    @param      pixelAspectRatio    receives the pixel aspect ratio of the object
*/
- (void)objectWidth:(NSUInteger*)outputWidth
             height:(NSUInteger*)outputHeight
   pixelAspectRatio:(double*)pixelAspectRatio;

/*!
    @method     inputWidth:height:pixelAspectRatio:
    @abstract   Returns the width, height, and pixel aspect ratio of the input to the filter
                or transition that the onscreen control is applied to.
    @param      inputWidth     receives the width of the object
    @param      inputHeight    receives the height of the object
    @param      pixelAspectRatio    receives the pixel aspect ratio of the object
*/
- (void)inputWidth:(NSUInteger*)inputWidth
            height:(NSUInteger*)inputHeight
  pixelAspectRatio:(double*)pixelAspectRatio;

/*!
    @method     objectToScreenTransform
    @abstract   Returns a 4x4 matrix describing the transformation from object normalized
                space (0-1 in both width and height) to screen space.
*/
- (FxMatrix44*)objectToScreenTransform;

@end


@protocol FxOnScreenControlAPI_v3 <FxOnScreenControlAPI_v2>

/*!
    @method     backingScaleFactor
    @abstract   Returns the scale factor for drawing the OSC to the screen it's currently on. Vertex
                coordinates will already scale properly, but this method can be used for other
                types of drawing (for example, if you are preparing a texture on the CPU and
                uploading it, this can tell you how large to make the texture before uploading it).
*/
- (double)backingScaleFactor;

@end

@protocol FxOnScreenControlAPI_v4 <FxOnScreenControlAPI_v3>

/*!
    @method     setCursor:
    @abstract   Allows the plug-in OSC to set the cursor the user will see in the canvas
    @param      newCursor - the new cursor to display to the user
*/
- (void)setCursor:(NSCursor*)newCursor;

@end
