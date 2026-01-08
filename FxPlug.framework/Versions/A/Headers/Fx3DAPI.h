/*!
	@header		Fx3DAPI.h
	@abstract	Defines the Fx3DAPI host API protocol.
*/

/* Copyright © 2005-2025 Apple Inc. All rights reserved. */

/* Earlier versions of these APIs are not compatible with FxPlug 4.x and have been removed from
   the FxPlug SDK
 */

#import <FxPlug/FxTypes.h>
#import <FxPlug/FxParameterAPI.h>
#import <FxPlug/FxMatrix.h>



/*!
     @protocol   Fx3DAPI_v5
     @abstract   Defines the methods the host application provides to get information about the
     3D environment, including camera and object transforms.
     
     NOTE: This API is only available to plug-ins built in the FxPlug 4.1 style and later.
     It intentionally does not inherit from earlier versions and is a completely new API.
 */
@protocol Fx3DAPI_v5

/*!
     @method        focalLengthAtTime:error:
     @abstract      Gets the focal length.
     @param         time    Specified time, expressed as a CMTime, to retrieve the focal length.
     @param         error   Description of the problem if you are unable to retrieve the value.
     @result        The value of the focal length.
     @description   The reported focal length is in millimeters and has an equivalent vertical field
                    of view of a lens of that focal length on a 35mm camera.
 */
- (double)focalLengthAtTime:(CMTime)time
                      error:(NSError**)error;

/*!
    @method		layerMatrixAtTime:error
    @abstract   Gets the 4x4 layer matrix, which is equivalent to the model matrix for the object
                that the effect is applied to.
    @param      time    Specified time, expressed as a CMTime, to retrieve the matrix.
    @param      error   Description of the problem if you are unable to retrieve the matrix.
    @result		An FxMatrix44 containing the matrix.
 */
- (FxMatrix44*)layerMatrixAtTime:(CMTime)time
                           error:(NSError**)error;

/*!
    @method     viewMatrixAtTime:error
    @abstract   Gets the 4x4 view matrix.
    @param      time    Specified time, expressed as a CMTime, to retrieve the matrix.
    @param      error   Description of the problem if you are unable to retrieve the matrix.
    @result		An FxMatrix44 containing the matrix.
 */
- (FxMatrix44*)viewMatrixAtTime:(CMTime)time
                          error:(NSError**)error;

/*!
    @method     metalProjectionMatrixAtTime:error
    @abstract   Gets the 4x4 Metal projection matrix.
    @param      time    Specified time, expressed as a CMTime, to retrieve the matrix.
    @param      error   Description of the problem if you are unable to retrieve the matrix.
    @result		An FxMatrix44 containing the matrix.
 */
- (FxMatrix44*)metalProjectionMatrixAtTime:(CMTime)time
                                     error:(NSError**)error;

/*!
    @method     frustumLeft:right:bottom:top:near:far:atTime:error
    @abstract   Describes the bounds of the viewing solid for generating a projection matrix.
    @param      left    The x coordinate for the left vertical clipping plane.
    @param      right   The x coordinate for the right vertical clipping plane.
    @param      bottom  The y coordinate for the bottom horizontal clipping plane.
    @param      top     The y coordinate for the top horizontal clipping plane.
    @param      near    The z distance to the near depth clipping plane. Value must be positive.
    @param      far     The z distance to the far depth clipping plane. Value must be positive.
    @param      time    Specified time, expressed as a CMTime, to retrieve the matrix.
    @param      error   Description of the problem if you are unable to retrieve the matrix.
    @result		YES if the the frustum was retrieved, NO otherwise.
 */
- (BOOL)frustumLeft:(double*)left
              right:(double*)right
             bottom:(double*)bottom
                top:(double*)top
               near:(double*)near
                far:(double*)far
             atTime:(CMTime)time
              error:(NSError**)error;

@end
