/*!
    @header     FxColorGamutAPI.h
    
    @abstract   Defines the FxColorGamut host API protocols for retrieving information about
                a project's color gamut and conversions to other color spaces.
 
*/

/* Copyright © 2005-2025 Apple Inc. All rights reserved. */

/* Earlier versions of these APIs are not compatible with FxPlug 4.x and have been removed from
   the FxPlug SDK
 */

#ifndef FxColorGamutAPI_h
#define FxColorGamutAPI_h

#include <FxPlug/FxTypes.h>

@class FxMatrix44;


/*!
    @protocol       FxColorGamutAPI_v2
    @abstract       The FxColorGamutAPI handles plug-in queries of the host for the project's color
                    gamut.
    @discussion     The FxColorGamutAPI can retrieve the matrices for converting between RGB
                    (in the project's color gamut) and YCbCr. Use this protocol when your plug-in
                    converts to YCbCr or from color pixels to grayscale.
 
                    This is version 2 of the Color Gamut API and does not require time.
 
                    NOTE: This API is only available to plug-ins built in the FxPlug 4 style and
                    later.
 */

@protocol FxColorGamutAPI_v2

/*!
    @method     colorMatrixFromDesiredRGBToYCbCr:
    @abstract   Returns a color matrix that is appropriate for converting RGB values in
                the project's working gamut into YCbCr values.
    @result     A 4x4 matrix for converting an RGB value into YCbCr.
    @discussion You can use the first row of the resulting matrix of this function to convert
                from RGB to grayscale.
 */
- (FxMatrix44*)colorMatrixFromDesiredRGBToYCbCr;

/*!
    @method     colorMatrixFromYCbCrToDesiredRGB:
    @abstract   Returns a color matrix that is appropriate for converting RGB values from
                YCbCr to an RGB value in the project's working gamut.
    @result     A 4x4 matrix for converting an RGB value into YCbCr.
 */
- (FxMatrix44*)colorMatrixFromYCbCrToDesiredRGB;

/*!
    @method     colorPrimaries
    @abstract   Provides an enumerated type that describes the color primaries of the project's
                working gamut.
    @result     An FxColorPrimaries value that describes the project's working gamut.
*/
- (FxColorPrimaries)colorPrimaries;

@end


#endif /* FxColorGamutAPI_h */
