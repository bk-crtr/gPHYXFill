/*!
    @header     FxPathAPI.h
    @abstract   Defines the FxPathAPI protocol
    @discussion An API that defines the methods to retrieve information about paths, shapes, and
                masks the user has drawn on an object.

*/

/* Copyright © 2010-2025 Apple Inc. All rights reserved. */

/* Earlier versions of these APIs are not compatible with FxPlug 4.x and have been removed from
   the FxPlug SDK
 */


#ifndef __FX_PATH_API_H__
#define __FX_PATH_API_H__


/*!
    @enum       FxPathStyle
    @discussion Identifies the style of the path, which is linear, bezier or b-spline
    @constant   kFxPathStyle_Linear      the path is composed of linear segments
    @constant   kFxPathStyle_Bezier      the path is composed of bezier segments
    @constant   kFxPathStyle_XSpline     the path is an X-spline path
    @constant   kFxPathStyle_SuperEllipse     the path is a superellipse path
    @constant   kFxPathStyle_Rectangle   the path is a rectangle path
*/
enum {
    kFxPathStyle_Linear  = 0,
    kFxPathStyle_Bezier,
    kFxPathStyle_XSpline,
    kFxPathStyle_SuperEllipse,
    kFxPathStyle_Rectangle
    
};
typedef NSUInteger FxPathStyle;



/*!
    @typedef    FxVertex
    @abstract   Defines the properties of one vertex in a path.
    @field      location        the 2D location of the vertex on the object. This is in the
                                object's full-sized space. So for a vertex on a DV frame of video,
                                x will be between 0 and 720 and Y will be between 0 and 480.
    @field      inTangent       The x and y vectors of the incoming tangent
    @field      outTangent      The x and y vectors of the outgoing tangent
    @field      xSplineWeight   The weight of the x-spline at this point
    @field      interpStyle     The style of interpolation at this vertex
*/
typedef struct FxVertex {
    FxPoint2D   location;       // The x/y location of the point in the 2D image
    FxPoint2D   inTangent;      // The x and y vectors of the incoming tangent
    FxPoint2D   outTangent;     // The x and y vectors of the outgoing tangent
    double      xSplineWeight;  // The weight of the x-spline at this point
    FxPathStyle interpStyle;    // The style of interpolation at this vertex
} FxVertex;


/*!
    @typedef    FxPathGeometryInfo
    @abstract   Defines the geometry information of the path.
    @field      curvature   The curvature of the ellipse, if applicable
    @field      radius      The radius of the ellipse, if applicable
    @field      size        The size of the rectangle, if applicable
*/
typedef struct FxPathGeometryInfo {
    double      curvature;                      // The curvature of the ellipse, if applicable
    FxPoint2D   radius;                         // The radius of the ellipse, if applicable
    FxPoint2D   size;                           // The size of the rectangle, if applicable
    __unsafe_unretained FxMatrix44* transform;  // The shape's transformation matrix
} FxPathGeometryInfo;

/*!
    @enum       FxPathBlendMode
    @discussion The type of blending the user selects for the mask that a particular path describes.
*/

enum {
    kFxPathBlendMode_Add    = 0,
    kFxPathBlendMode_Subtract,
    kFxPathBlendMode_Replace,
    kFxPathBlendMode_Intersect
};
typedef NSUInteger FxPathBlendMode;

/*!
    @enum       FxPathColor
    @discussion Allows you to change the color the host app uses to display a path to the user.
    @constant   kFxPathColor_Red        Red
    @constant   kFxPathColor_Green      Green
    @constant   kFxPathColor_Blue       Blue
    @constant   kFxPathColor_Purple     Purple
    @constant   kFxPathColor_Orange     Orange
    @constant   kFxPathColor_Magenta    Magenta
    @constant   kFxPathColor_Yellow     Yellow
    @constant   kFxPathColor_Cyan       Cyan
*/

enum {
    kFxPathColor_Red    = 0,
    kFxPathColor_Green,
    kFxPathColor_Blue,
    kFxPathColor_Purple,
    kFxPathColor_Orange,
    kFxPathColor_Magenta,
    kFxPathColor_Yellow,
    kFxPathColor_Cyan
};
typedef NSUInteger FxPathColor;





/*!
    @protocol       FxPathAPI_v3
    @abstract       An API for retrieving information about paths, shapes, and masks the user has
                    drawn on an object.
    @discussion     This API is used to get information about a path that the user has drawn on
                    an object in the canvas. You can get the number of paths applied to the object,
                    and then enumerate through them, or pick a specific one. You can use this API
                    to retrieve the control points along the path and use that information to
                    guide the rendering of your plug-in.
 
                    NOTE: This API is only available to plug-ins built in the FxPlug 4 style and later.
                    This API does *NOT* inherit from the FxPathAPI. It is a new API that uses CMTime
                    and handles NSErrors consistently with other familiar APIs.
*/
@protocol FxPathAPI_v3

/*!
    @method     numberOfPathsAtTime:
    @abstract   Provides the number of paths on the object to which your filter is applied, or
                on your generator.
    @param      time        The time at which you wish to know the number of paths
    @result     The number of paths available to the plug-in.
*/
- (NSUInteger)numberOfPathsAtTime:(CMTime)time;

/*!
    @method     pathID:forIndex:error:
    @abstract   Returns the unique ID of the path at the index requested.
    @param      pathID  Receives the ID of path at the given index.
    @param      index   The index of the that you want the ID of. This value is 0-based, so
                        if there are n paths, you can retrieve paths at indexes 0 through n-1.
    @param      err     If this is not nil, then any errors that occur in trying to retrieve the
                        path ID will be returned in this variable.
    @result     Returns YES if retrieving the ID was successful, NO otherwise. If it returns NO
                and err was not nil, then *err will point to an NSError object containing
                information about what went wrong.
*/
- (BOOL)pathID:(FxPathID*)pathID
      forIndex:(NSUInteger)index
         error:(NSError**)err;

/*!
    @method     name:ofPath:error:
    @abstract   Returns the name the user has entered for the path with the given ID
    @param      name        Receives the name of the path
    @param      pathID      The ID of the path whose name you want to retrieve
    @param      error       If this is not nil, then any errors which occurred while retrieving
                            the name.
    @result     Returns YES if retrieving the name was successful, NO otherwise. If it returns NO
                and err was not nil, then *err will point to an NSError object containing
                information about what went wrong.
*/
- (BOOL)name:(NSString**)name
      ofPath:(FxPathID)pathID
       error:(NSError**)error;

/*!
    @method     isPath:open:atTime:error:
    @abstract   Tells whether the path is opened or closed
    @param      pathID      The ID of the path you want to know about
    @param      isOpen      Upon return, contains whether the path is open or not
    @param      time        The time at which you wish to query the path
    @param      err         If this is not nil, then any errors which occur while checking the path.
    @result     Returns YES if retrieving the openness was successful, NO otherwise. If it returns NO
                and err was not nil, then *err will point to an NSError object containing
                information about what went wrong.
*/

- (BOOL)isPath:(FxPathID)pathID
          open:(BOOL*)isOpen
        atTime:(CMTime)time
         error:(NSError**)err;

/*!
    @method     isPath:inverted:atTime:error:
    @abstract   Tells whether the path is inverted or not
    @param      pathID      The ID of the path you want to know about
    @param      isInverted  Upon return, contains whether the path is inverted or not
    @param      time        The time at which you wish to query the path
    @param      err         If this is not nil, then any errors which occur while checking the path
    @result     Returns YES if retrieving the inversion was successful, NO otherwise. If it returns NO
                and err was not nil, then *err will point to an NSError object containing
                information about what went wrong.
*/
- (BOOL)isPath:(FxPathID)pathID
      inverted:(BOOL*)isInverted
        atTime:(CMTime)time
         error:(NSError**)err;

/*!
    @method     style:ofPath:error:
    @abstract   Returns the style of the path, which is linear, bezier, or b-spline
    @param      style       Upon return, contains the style of the path
    @param      pathID      The ID of the path to query
    @param      err         If this is not nil, then any errors which occurred while querying
                            the path
    @result     Returns YES if retrieving the style was successful, NO otherwise. If it returns NO
                and err was not nil, then *err will point to an NSError object containing
                information about what went wrong.
*/
- (BOOL)style:(FxPathStyle*)style
       ofPath:(FxPathID)pathID
        error:(NSError**)err;

/*!
    @method     length:ofPath:atTime:error:
    @abstract   Returns the length of the entire path
    @param      length      Upon return, contains the length of the path
    @param      pathID      The ID of the path to query
    @param      time        The time at which you wish to find the length of the path
    @param      err         If this is not nil, then any errors which occur while checking the path
    @result     Returns YES if retrieving the length of the path was successful, NO otherwise. If
                it returns NO and err was not nil, then *err will point to an NSError object
                containing information about what went wrong.
*/
- (BOOL)length:(double*)length
        ofPath:(FxPathID)pathID
        atTime:(CMTime)time
         error:(NSError**)err;

/*!
    @method     length:ofSegment:ofPath:atTime:error:
    @abstract   Returns the length of 1 segment of the entire path
    @param      length              Upon return, contains the length (in pixels) of the desired
                                    segment
    @param      segmentIndex        Which segment of the path you wish to evaluate the length of
    @param      pathID              The ID of the path whose segment you wish to evaluate
    @param      time                The time at which to evaluate the length of a segment of a path
    @param      err                 If this is not nil, then any errors which occur when evaluating
                                    the length of a segment of the path
    @result     Returns YES if retrieving the length of the segment was successful, NO otherwise.
                If it returns NO and err was not nil, then *err will point to an NSError object
                containing information about what went wrong.
*/
- (BOOL)length:(double*)length
     ofSegment:(NSUInteger)segmentIndex
        ofPath:(FxPathID)pathID
        atTime:(CMTime)time
         error:(NSError**)err;

/*!
    @method     numberOfVertices:inPath:atTime:error:
    @abstract   Returns the number of vertices in the given path at the given time
    @param      numVertices Upon return, contains the number of vertices in the path
    @param      pathID      The ID of the path to query
    @param      time        The time at which you wish to find the number of vertices in the path
    @param      err         If this is not nil, then any errors which occur while checking the path
    @result     Returns YES if retrieving the number of vertices was successful, NO otherwise.
                If it returns NO and err was not nil, then *err will point to an NSError object
                containing information about what went wrong.
*/
- (BOOL)numberOfVertices:(NSUInteger*)numVertices
                  inPath:(FxPathID)pathID
                  atTime:(CMTime)time
                   error:(NSError**)err;

/*!
    @method     vertex:atIndex:ofPath:atTime:error:
    @abstract   Returns information about a single vertex on the path at the given time.
    @param      vertex      A pointer to an FxVertex structure to hold the information about the
                            vertex in question
    @param      index       The index of the vertex you want to retrieve. This is 0-based, so if
                            the path has n vertices, this will be between 0 and n-1
    @param      pathID      The ID of the path from which you wish to retrieve a vertex
    @param      time        The time at which to retrieve the vertex information
    @param      err         If this is not nil, then any errors which occur which querying the path
    @result     Returns YES if retrieving the vertex information was successful, NO otherwise.
                If it returns NO and err was not nil, then *err will point to an NSError object
                containing information about what went wrong.
*/
- (BOOL)vertex:(FxVertex*)vertex
       atIndex:(NSUInteger)index
        ofPath:(FxPathID)pathID
        atTime:(CMTime)time
         error:(NSError**)err;

/*!
    @method     point:atPercentageOfDistance:alongPath:atTime:error:
    @abstract   Returns a 2D point containing the location of the point on the path that is
                percentDist percent along the path's length.
    @param      point           Upon return, contains the point that is the desired percentage
                                of distance along the path.
    @param      percentDist     The percentage of the distance along the path where you wish to
                                find a point
    @param      pathID          The ID of the path you want to query
    @param      time            The time at which you wish to query the path
    @param      err             If this is not nil, then information about any errors that occur
    @result     Returns YES if retrieving the point was successful, NO otherwise.
                If it returns NO and err was not nil, then *err will point to an NSError object
                containing information about what went wrong.
*/
- (BOOL)point:(FxPoint2D*)point
atPercentageOfDistance:(double)percentDist
    alongPath:(FxPathID)pathID
       atTime:(CMTime)time
        error:(NSError**)err;

/*!
    @method     point:atPercentageOfParameter:alongPath:atTime:error:
    @abstract   Provides a 2D point that contains the location of the point on the path, at a
                distance along the path's parametric distance equal to the percent value of
                percentParameter.

    @param      point               Upon return, contains the point that is th desired percentage of
                                    the parameter value along the path
    @param      percentParameter    The percentage of the parameter along the path where you wish to
                                    find a point
    @param      pathID              The ID of the path you want to query
    @param      time                The time at which you wish to query the path
    @param      err                 If this is not nil, then information about any errors that occur
    @result     Returns YES if retrieving the percentage was successful, NO otherwise.
                If it returns NO and err was not nil, then *err will point to an NSError object
                containing information about what went wrong.
    @discussion This is not the same as the percentage of distance along the path. (For that, use
                pointAtPercentageOfDistance:alongPath:atTime:error:) In areas of high curvature
                along the path, there will be a greater percentage of points along the curve in
                parametric space, whereas in areas of low curvature, there will be fewer points
                along the curve.
*/
- (BOOL)point:(FxPoint2D*)point
atPercentageOfParameter:(double)percentParameter
    alongPath:(FxPathID)pathID
       atTime:(CMTime)time
        error:(NSError**)err;

/*!
    @method     blendMode:ofPath:error:
    @abstract   Gets the blending mode for a given path
    @param      blendMode   Upon return, contains the blend mode of the path
    @param      pathID      The ID of the path whose blending mode you want to retrieve
    @param      err         If this is not nil, then any errors that occurred while retrieving the
                            blending mode
    @result     Returns YES if retrieving the blend mode was successful, NO otherwise.
                If it returns NO and err was not nil, then *err will point to an NSError object
                containing information about what went wrong.
*/
- (BOOL)blendMode:(FxPathBlendMode*)blendMode
           ofPath:(FxPathID)pathID
            error:(NSError**)err;


/*!
    @method     setPath:toColor:error:
    @abstract   Sets the color the host app uses to draw the path with the given ID
    @param      pathID      The ID of the path you want to draw in a different color
    @param      newColor    The FxPathColor to use to draw the path in
    @param      err         If this is not nil, then any errors which occur when setting the
                            path drawing color
    @result     Returns YES if setting the color was successful, NO otherwise.
                If it returns NO and err was not nil, then *err will point to an NSError object
                containing information about what went wrong.
*/
- (BOOL)setPath:(FxPathID)pathID
        toColor:(FxPathColor)newColor
          error:(NSError**)err;

/*!
    @method     pathGeometryInfo:forPath:atTime:error:
    @abstract   Gets geometry information about a given path
    @param      info    The geometry information associated to the path with the given ID
    @param      pathID  The ID of the path whose blending mode you want to retrieve
    @param      time    The time at which you wish to query the path
    @param      error   If this is not nil, then any errors that occurred while retrieving the
                        path geometry
    @result     Returns YES if retrieving the information was successful, NO otherwise. If it
                        returns NO and err was not nil, then *err will point to an NSError object
                        containing information about what went wrong.
*/
- (BOOL)pathGeometryInfo:(FxPathGeometryInfo*)info
                 forPath:(FxPathID)pathID
                  atTime:(CMTime)time
                   error:(NSError**)error;
@end

#endif // __FX_PATH_API_H__
