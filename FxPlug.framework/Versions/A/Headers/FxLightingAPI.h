/*!
    @header		FxLightingAPI.h
 
    @abstract	Defines the protocol for the FxLighting host API.
 */

/* Copyright © 2005-2025 Apple Inc. All rights reserved. */

/* Earlier versions of these APIs are not compatible with FxPlug 4.x and have been removed from
   the FxPlug SDK
 */

#ifndef __FX_LIGHTINGAPI_H__
#define __FX_LIGHTINGAPI_H__

#import <Cocoa/Cocoa.h>
#import <FxPlug/FxTypes.h>


/*!
    @define     kFxLight_V3
    @abstract   Defines a structure that contains lighting information as a version 3 structure.
*/
#define kFxLight_V3 3

/*!
 @define     FxLight_CurrentVersion
 @abstract   Used to define a structure that contains lighting information as being the
             most current version.
 @discussion Introduced in FxPlug 4.
 */
#define FxLight_CurrentVersion  kFxLight_V3

/*!
    @define     FxInitLight
    @abstract   Initializes a structure that contains lighting information to be empty.
    @discussion Introduced in FxPlug 4.
*/
#define FxInitLight(light) { memset(&light, 0, sizeof(light)); }

/*!
    @enum       FxLightType
    @constant   kFxLightType_Ambient    An light that illuminates all objects in the scene evenly
    @constant   kFxLightType_Directional A light that comes from a particular direction, but which
                                        is infinitely far away. Often used to simulate the sun, it
                                        casts light rays which are parallel at every point in space.
    @constant   kFxLightType_Point      A light which exists at a point and illuminates in all 
                                        directions. Unlike ambient light, it can be occluded.
    @constant   k FxLightType_Spot      A light which has a limiting cone, and which can be
                                        positioned and pointed in a particular direction. This is
                                        directly analogous to a spot light in real life.
*/
enum {
    kFxLightType_Ambient     = 0,
    kFxLightType_Directional = 1,
    kFxLightType_Point       = 2,
    kFxLightType_Spot        = 3
};
typedef NSUInteger FxLightType;

/*!
    @struct     FxLight
    @field      version                 Which version of the light structure this is.
    @field      time                    The CMTime at which this information is valid.
    @field      lightType               The type of light, whether ambient, directional, point,
                                        or spot
    @field      color                   The color of the light
    @field      intensity               The brightness with which the light illuminates objects
    @field      constantAttenuation     Lighting falloff is governed by an equation that contains
                                        constant, linear, and quadratic terms. This is the constant 
                                        term of the lighting falloff equation
    @field      linearAttenuation       The linear component of the lighting falloff equation
    @field      quadraticAttenuation    The quadratic component of the lighting falloff equation
    @field      castsShadows            A Boolean value that indicates whether a light casts shadows
    @field      spotCutoff              The angle, in radians, of the cone cutoff for spotlights
    @field      spotPenumbraCutoff      The angle, in radians, of the cone penumbra cutoff for
                                        spotlights
    @field      position                The location of the light
    @field      direction               The direction in which the light points
    @discussion This is not an FxLightInfo type and should only be used for FxPlug 4 style plug-ins.
*/

typedef struct FxLight {
    NSUInteger      version;
    CMTime          time;
    FxLightType     lightType;
    __unsafe_unretained NSColor*        color;
    float           intensity;
    float           constantAttenuation;
    float           linearAttenuation;
    float           quadraticAttenuation;
    BOOL            castsShadows;
    float           spotCutoff;             // Angle in radians
    float           spotPenumbraCutoff;     // Angle in radians
    FxPoint3D       position;
    FxPoint3D       direction;
} FxLight;


/*!
    @protocol   FxLightingAPI_v3
    @abstract   An API you use to get information about lights in a scene in a Motion project.
    @discussion Allows a plugin to get the number of lights in a scene, and then to get information
                about each light at a given time. Only available in FxPlug 4-based plug-ins or later
 */
@protocol FxLightingAPI_v3

/*!
    @method     numberOfLightsAtTime:
    @abstract   Retrieves the number of lights at the passed-in time
    @param      time    The time at which you wish to obtain the number of lights
 */
- (NSUInteger)numberOfLightsAtTime:(CMTime)time;

/*!
    @method     lightInfo:forLight:atTime:error:
    @abstract   Returns information about a light at a particular time
    @param      light       A pointer to an FxLight structure to receive information about
                            a particular light in the scene
    @param      lightIndex  The index of the light whose information you wish to retrieve
    @param      time        The time for which you want lighting information
    @param      error       If any errors occur during processing, this contains a pointer
                            to an error object upon return.
 */
- (BOOL)lightInfo:(FxLight*)light
         forLight:(NSUInteger)lightIndex
           atTime:(CMTime)time
            error:(NSError**)error;
@end

#endif // __FX_LIGHTINGAPI_H__
