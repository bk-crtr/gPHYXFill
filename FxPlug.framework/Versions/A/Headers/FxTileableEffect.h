/*!
 @header        FxTileableEffect.h
 @abstract      Defines the FxTileableEffect API protocols for rendering only certain portions of
                the plug-in's output, referred to as tiles.
 */
/* Copyright © 2005-2025 Apple Inc. All rights reserved. */

#ifndef FxTileableBaseEffect_h
#define FxTileableBaseEffect_h

#import <FxPlug/FxTypes.h>
#import <FxPlug/FxImageTile.h>

NS_ASSUME_NONNULL_BEGIN


/*!
    @const      kFxPropertyKey_NeedsFullBuffer
    @abstract   A key that determines whether the plug-in needs the entire image to do its
                processing, and can't tile its rendering.
    @discussion This value of this key is a Boolean NSNumber indicating whether this plug-in
                needs the entire image to do its processing. Note that setting this value to
                YES incurs a significant performance penalty and makes your plug-in
                unable to render large input images. The default value is NO.
*/
#define kFxPropertyKey_NeedsFullBuffer  @"NeedsFullBuffer"

/*!
    @const      kFxPropertyKey_VariesWhenParamsAreStatic
    @abstract   A key that determines whether your rendering varies even when
                the parameters remain the same.
    @discussion The value for this key is a Boolean NSNumber indicating whether this effect
                changes its rendering even when the parameters don't change. This can happen if
                your rendering is based on timing in addition to parameters, for example. Note that
                this property is only checked once when the filter is applied, so it
                should go in static properties rather than dynamic properties.
 */
#define kFxPropertyKey_VariesWhenParamsAreStatic    @"VariesWhenParamsAreStatic"

/*!
    @const      kFxPropertyKey_ChangesOutputSize
    @abstract   A key that determines whether your filter has the ability to change the size
                of its output to be different than the size of its input.
    @discussion The value of this key is a Boolean NSNumber indicating whether your filter
                returns an output that has a different size than the input. If not, return "NO"
                and your filter's @c -destinationImageRect:sourceImages:pluginState:atTime:error:
                method will not be called.
 */
#define kFxPropertyKey_ChangesOutputSize    @"ChangesOutputSize"


/*!
    @protocol   FxTileableEffect
    @abstract   Describes a plug-in video effect which is able to work efficiently on tiles
                of the input and output image.
    @discussion A tileable effect is one which can operate on image tiles or an entire image
                at once to work more efficiently. FxTileableEffects work in their own process
                outside of the host application's memory and process space. They must be able to
                calculate the bounds of their output given an input, and they must be able to
                calculate the bounds of an input rectangle needed to render a given output
                rectangle. Rendering state is gathered ahead of time so that no APIs are needed
                in order to do rendering.
 */
@protocol FxTileableEffect <NSObject>

/*!
    @method     -initWithAPIManager:
    @abstract   Initializes the API manager for your plug-in.
    @param      apiManager  -   An object that obtains the various API objects your plug-in
                                needs for communicating with the host application.
    @discussion Save a weak reference to the apiManager object to use in subsequent calls.
                The plug-in initializes its properties and instance variables and returns a pointer
                to self. If any part of initialization fails in an unrecoverable way,
                this method returns nil.

 */
- (nullable instancetype)initWithAPIManager:(id<PROAPIAccessing>)apiManager;

/*!
    @method     -addParametersWithError:
    @abstract   Tells the host application what parameters your plug-in requires.
    @param      error   -   Description of the problem if you are unable to add all of your
                parameters. After describing the problem in this NSError, return NO.
                Use the FxPlugErrorDomain when creating the error.
    @discussion Use the apiManager object that was passed to your @c -initWithAPIManager: method
                to obtain an @c FxParameterCreation* object. Tell the parameter creation API object
                which types of parameters your plug-in needs. This method is only called
                at most once per instance of your plug-in. It will not be called when loading
                a document which contains your plug-in.
    @result     This method returns YES if it successfully creates the parameters the
                plug-in needs. If not, it returns NO and fills out the "error" parameter
                with the details of what went wrong.
 */
- (BOOL)addParametersWithError:(NSError**)error;

/*!
    @method     -properties:error:
    @abstract   Tells the host application about what the plug-in needs and supports, such as
                whether it renders in a gamma-corrected color space or a linear one.
    @param      properties      An NSDictionary containing the values of any properties you
                                want to tell the host you support or don't support.
    @param      error   -       Description of the problem if you are unable to add all of your
                                parameters. After describing the problem in this NSError, return NO.
                                Use the FxPlugErrorDomain when creating the error.
    @discussion This method is called once per plug-in instantiation and the result is cached
                for later use by the host application. The dictionary returned by this method
                should contain various kFxPropertyKeys and their corresponding values.
    @result     This method returns YES if it successfully created its properties dictionary
                and NO otherwise. If it returns NO, it fills out the error pointed to by
                the error parameter.
 */
- (BOOL)properties:(NSDictionary* _Nonnull * _Nullable)properties
             error:(NSError**)error;

/*!
    @method     -pluginState:atTime:quality:error:
    @abstract   Retrieves the plug-in's parameter values, performs any calculations it needs to
                from those values, and packages up the result to be used later with rendering.
    @param      pluginState     The state of the plug-in. Your plug-in should return an autoreleased
                                NSData that the host retains. Once created, your plug-in should not
                                hold on to this pointer. The host returns this data (or copy of it)
                                to your plug-in during other methods, such as
                                the renderDestinationImage:sourceImages:pluginState:atTime:error:
                                method and scheduleInputs:withPluginState:atTime:error: methods.
    @param      renderTime      The rational time at which your plug-in is being asked to gather
                                its rendering state.
    @param      qualityLevel    The requested render quality (@c kFxQuality_LOW,
                                @c kFxQuality_MEDIUM, or @c kFxQuality_HIGH).
    @param      error   -       Description of the problem if you are unable to add all of your
                                parameters. After describing the problem in this NSError, return NO.
                                Use the FxPlugErrorDomain when creating the error.
    @discussion The host application calls this method before rendering. The
                @c FxParameterRetrievalAPI* is valid during this call. Use it to get the values of
                your plug-in's parameters, then put those values or the results of any calculations
                you need to do with those parameters to render into an NSData that you return
                to the host application. The host passes it back to you during subsequent calls.
                Do not re-use the NSData; always create a new one, as this method may be called
                on multiple threads at the same time.
    @result     An NSData that contains whatever data you need to render or calculate input and
                output tile bounds.
 */
- (BOOL)pluginState:(NSData* _Nonnull * _Nullable)pluginState
             atTime:(CMTime)renderTime
            quality:(FxQuality)qualityLevel
              error:(NSError**)error;

/*!
    @method     -destinationImageRect:sourceImages:pluginState:atTime:error:
    @abstract   Calculates the bounds of the output image determined by the various inputs and
                plug-in state at the given render time.
    @param      destinationImageRect        The bounds of the output image. Your plug-in should
                                            calculate this value
    @param      sourceImages                An array of images that represent the input images.
                                            They will have proper bounding rectangles and pixel
                                            transforms, but no pixel data will be given yet.
    @param      pluginState                 The NSData returned from your plug-in's
                                            @c -pluginState:atTime:quality:error: method.
    @param      renderTime                  The rational time at which the output will be rendered.
    @param      outError                    An NSError your plug-in can fill out with the details
                                            of what went wrong when a failure occurs in this method.
                                            If there was an error, your plug-in should also return
                                            NO from this method.
    @discussion This method is called when the plug-in wants to calculate the output bounds of
                your plug-in when rendering at a particular time. It will pass in an array of
                images, the plug-in state returned from your plug-in's @c -pluginState:atTime:quality:error:
                method, and the render time. If your plug-in is unable to calculate the output
                rectangle at the given time, it should return NO and fill out the outError
                parameter with an NSError that uses the FxPlugErrorDomain and appropriate (possibly
                custom) error code.
    @result     YES if the method successfully calculates the output image bounds, NO otherwise.
                When returning NO, be sure to fill out the outError parameter.
 */
- (BOOL)destinationImageRect:(FxRect*)destinationImageRect
                sourceImages:(NSArray<FxImageTile*>*)sourceImages
            destinationImage:(FxImageTile*)destinationImage
                 pluginState:(nullable NSData*)pluginState
                      atTime:(CMTime)renderTime
                       error:(NSError**)outError;

/*!
    @method     -sourceTileRect:sourceImageIndex:sourceImages:forDestinationTileRect:destinationImage:pluginState:atTime:error:
    @abstract   Calculate the input rectangle needed for the given image input and the output tile
                to be rendered.
    @param      sourceTileRect          The bounds of input tile for this input that will be needed
                                        to render the output tile. This method must calculate this
                                        value.
    @param      sourceImageIndex        The index of the input image for which the tile bounds have
                                        been requested. This index matches the order of images in
                                        the inputImages array below.
    @param      sourceImages            An NSArray of image tiles representing the input images.
                                        This image will not yet contain any pixel data, just the
                                        image bounds, pixel transforms, and other data describing
                                        the image.
    @param      destinationTileRect     The bounds of the output tile, given to you by the host
                                        application.
    @param      destinationImage        The output image. This image will not yet contain any pixel
                                        data, just the image bounds and other data about the output
                                        image.
    @param      pluginState             The NSData return by your plug-in's
                                        @c -pluginState:atTime:quality:error: method.
    @param      renderTime              The rational time at which the output will be rendered.
    @param      outError                An NSError your plug-in can fill out with the details of
                                        what went wrong when a failure occurs in this method. If
                                        there was an error, your plug-in should also return NO from
                                        this method.
    @discussion This method is called for each image input your plug-in has requested.
                Use the passed-in information to calculate the portion of the current input
                image that you'll need to render the output tile that will be rendered next.
    @result     The input tile bounds are returned in the @c inputTileRect parameter. The method
                returns YES if calculating the input tile bounds succeeded and NO otherwise.
                When returning NO, be sure to fill out the outError parameter.
 */
- (BOOL)sourceTileRect:(nonnull FxRect*)sourceTileRect
      sourceImageIndex:(NSUInteger)sourceImageIndex
          sourceImages:(NSArray<FxImageTile*>*)sourceImages
   destinationTileRect:(FxRect)destinationTileRect
      destinationImage:(FxImageTile*)destinationImage
           pluginState:(nullable NSData*)pluginState
                atTime:(CMTime)renderTime
                 error:(NSError**)outError;

/*!
    @method     -renderDestinationImage:sourceImages:pluginState:atTime:error:

    @abstract   Sends a request that the host wants your plug-in to do its rendering for a given
                output image tile.
    @param      destinationImage        The output image tile that your plug-in should render to.
    @param      sourceImages            An array of image tiles. Each one will have the size
                                        calculated in your plug-in's
                                        @c -sourceTileRect:sourceImageIndex::::::: method.
    @param      pluginState             The NSData created in your plug-in's
                                        @c -pluginState:atTime:quality:error: method.
    @param      renderTime              The rational time at which to render your output.
    @param      outError                Any errors that occurred during rendering.
    @discussion The host calls this method when it wants your plug-in to render an image
                tile of the output image. It passes in each of the input tiles needed as well
                as the plug-in state needed for the calculations. Your plug-in does all its
                rendering in this method. Do not attempt to use the @c
                FxParameterRetrievalAPI* object, as it is invalid at this time. Note that this
                method will be called on multiple threads at the same time.
    @result     If rendering succeeded, return YES, otherwise return NO. If your plug-in returns
                NO, create an NSError with the FxPlugErrorDomain for the outError parameter.
 */
- (BOOL)renderDestinationImage:(FxImageTile*)destinationImage
                  sourceImages:(NSArray<FxImageTile*>*)sourceImages
                   pluginState:(nullable NSData*)pluginState
                        atTime:(CMTime)renderTime
                         error:(NSError**)outError;

@optional

/*!
    @method     -finishInitialSetup:
    @abstract   Finishes the initial setup and is called exactly once on first application of your
                plug-in to allow it to do anything else it needs to in order to finish setting
                itself up.
    @param      error   -   Any errors that occurred doing initial setup
    @discussion This is called after @c -initWithAPIManager:, @c -addParametersWithError:, and
                @c -properties:error:. Note that it will never be called by FCPX as it was called
                when the plug-in was initially added to the template in Motion.
    @result     Return YES if your plug-in was successfully able to set itself up. Return NO
                otherwise and set the error parameter to an NSError created with the
                FxPlugErrorDomain.
 */
- (BOOL)finishInitialSetup:(NSError**)error;

/*!
    @method     -pluginInstanceAddedToDocument
    @abstract   Notifies your plug-in when it becomes part of user's document.
    @discussion Called when a new plug-in instance is created or a document is loaded and an
                existing instance is deserialized. When the host calls this method, the plug-in is
                a part of the document and the various API objects work as expected.
 */

- (void)pluginInstanceAddedToDocument;

/*!
    @method     -parameterChanged:atTime:error:
    @abstract   Executes when the host detects that a parameter has changed.
    @param      paramID     The ID of the parameter that changed.
    @param      time        The rational time at which the parameter changed.
    @param      error       Return any errors that occurred in this parameter.
    @discussion Use this method to change, enable, disable, hide, or show other parameters.
                This method is called each time the user changes a parameter. You can use this
                method to update other parameters (such as hiding or showing them). You have full
                access to @c FxParameterCreationAPI_v5 or later, @c FxParameterRetrievalAPI_v6 or
                later, and @c FxParameterSettingAPI_v5 or later, within this method.
    @result     Return YES if you successfully handled the parameter change. Return NO otherwise.
                If you return NO, also fill out the error parameter by creating an NSError with
                the FxPlugErrorDomain.
 */
- (BOOL)parameterChanged:(UInt32)paramID
                  atTime:(CMTime)time
                   error:(NSError**)error;

/*!
    @method     -scheduleInputs:withPluginState:atTime:error:
    @abstract   Tells the host application how many frames from the given input sources
                your plug-in requires in order to render at the given render time.
    @param      inputImageRequests      An NSArray that you fill out with requests for input image
                                        sources, input image times, whether to include leading
                                        filters, and the parameter IDs you wish to schedule.
    @param      pluginState             The plug-in state returned by your plug-in's
                                        @c -pluginState:atTime:quality:error: method.
    @param      renderTime              The time at which your plug-in will be asked to render with
                                        the frames being scheduled.
    @param      error                   A detailed error describing what went wrong if your plug-in
                                        is unable to calculate the needed frame times.
    @discussion The host calls this method before rendering. Fill out the inputImageRequests
                parameter with an array of image requests. Each request represents one frame
                your plug-in needs to render. These can be frames of the effect source or they
                can be frames from image well parameters. If you don't implement this method,
                the application will assume that a filter needs only the current frame of its
                effect source.
                When instantiating an FxImageTileRequest array using
                @c initWithSource:time:includeFilters:parameterID:, the currently supported keys for
                FxImageTileRequestSource are:
 
                @c kFxImageTileRequestSourceNone       - Not associated with an effect or parameter.
 
                @c kFxImageTileRequestSourceEffectClip - Request frame from the source clip.
 
                @c kFxImageTileRequestSourceParameter  - Request frame from an image well parameter.
 
                @c kFxImageTileRequestSourceOutput     - The output of the plugin.
 
                @c time should indicate the requested CMTime of the input, @c includeFilters will
                indicate if the plugin requires frames after leading filter effects have already
                been applied, and @c parameterID will indicate the ID of the effect source (0) or
                image well.
    @result     Return YES if you were able to successfully create the inputImageRequests array.
                Return NO if you were not able to for some reason. When returning NO, be sure to
                also set the error parameter to a descriptive value.
 */
- (BOOL)scheduleInputs:(NSArray<FxImageTileRequest*>* _Nullable * _Nullable)inputImageRequests
       withPluginState:(NSData* _Nullable)pluginState
                atTime:(CMTime)renderTime
                 error:(NSError**)error;

/*!
    @method     -classForCustomParameterID:
    @abstract   Returns the class of the object contained in the custom parameter with the given ID.
    @param      parameterID            The parameter ID of the class to be determined.
    @discussion When you make custom parameters and the host stores them for you, it needs to know
                the class of the parameters to unarchive them from disk. This method returns the
                Objective-C class of which they are members.
    @result     An Objective-C class of the specified parameterID.

 */
- (Class)classForCustomParameterID:(UInt32)parameterID;

/*!
    @method     -classesForCustomParameterID:
    @abstract   Returns the set of classes of the objects contained in the custom parameter with the given ID.
    @param      parameterID            The parameter ID of the classes to be determined.
    @discussion When you make custom parameters and the host stores them for you, it needs to know
                the classes of the parameters to unarchive them from disk. This method returns the
                Objective-C set of classes of which they are members.
    @result     An NSSet of classes of the specified parameterID.

 */
- (NSSet<Class>*)classesForCustomParameterID:(UInt32)parameterID;

@end

NS_ASSUME_NONNULL_END

#endif /* FxTileableBaseEffect_h */
