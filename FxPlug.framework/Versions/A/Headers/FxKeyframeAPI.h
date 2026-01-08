/*!
    @header     FxKeyframeAPI.h
    
    @abstract   Allows plugins to set and retrieve keyframes for the parameters they have.
    @discussion With the FxKeyframeAPI, plugins can query the host application to find out whether
                keyframes exist and if so, how many there are, what times they are placed at, and
                what values parameters have at those keyframes. It also allows plugins to create
                new keyframes with the desired value at any given time.
                Introduced in FxPlug 2.0.
*/

/* Copyright © 2005-2025 Apple Inc. All rights reserved. */

/* Earlier versions of these APIs are not compatible with FxPlug 4.x and have been removed from
   the FxPlug SDK
 */

#ifndef __FXKEYFRAMEAPI_H__
#define __FXKEYFRAMEAPI_H__

#import <FxPlug/FxTypes.h>

NS_ASSUME_NONNULL_BEGIN


@protocol FxKeyframeAPI_v3

/*!
    @method     channelCount:forParameter:
    @abstract   Returns the number of channels in a parameter.
    @param      count           Receives the count of channels in the parameter
    @param      parameterID     The ID of the parameter whose channels you want to count
    @discussion Returns the number of channels in a parameter (sliders = 1, 2D points = 2,
                RGBA colors = 4, etc.) in the count parameter. If any errors occur, the return
                value will be a non-nil pointer to an NSError containing information about the
                error.
*/
- (NSError* _Nullable)channelCount:(NSUInteger*)count
                      forParameter:(NSUInteger)parameterID;

/*!
    @method     keyframeCount:forParameter:andChannel
    @abstract   Returns the number of keyframes for a parameter's channel.
    @param      count           Receives the number of keyframes in the given channel
    @param      parameterID     The ID of the parameter you want to get the keyframe count for
    @param      channelIndex    The index of the channel within the parameter that you want to
                                retrieve the keyframe count from.
    @discussion Given a parameter, you can get the number of keyframes for any one of its channels.
                For a slider, toggle button (checkbox), or angle parameter, there will be only
                1 channel (indexed starting at 0), whereas a point parameter will have 2 channels
                (x and y), and a color channel will have 4 (R, G, B, and A).If any errors occur
                while getting the keyframe count, the return value will be a pointer to an NSError
                describing the problem. Otherwise, the return value is nil.
 
*/
- (NSError* _Nullable)keyframeCount:(NSUInteger*)count
                       forParameter:(NSUInteger)parameterID
                         andChannel:(NSUInteger)channelIndex;

/*!
    @method     keyframe:forParameter:channel:andIndex:
    @abstract   Returns the keyframe info for the keyframe at a given index
    @param      keyframe        A pointer to an FxKeyframeInfo structure to be filled out with the
                                information about the keyframe in question.
    @param      parameterID     The ID of the parameter you want info about
    @param      channelIndex    The index of the channel you want info about
    @param      keyframeIndex   The index of the keyframe you wish to retrieve
 
*/
- (NSError* _Nullable)keyframe:(FxKeyframe*)keyframe
                  forParameter:(NSUInteger)parameterID
                       channel:(NSUInteger)channelIndex
                      andIndex:(NSUInteger)keyframeIndex;

/*!
    @method     setKeyframeIndex:withKeyframe:forParameter:andChannel:
    @abstract   Sets the keyframe at the given index to have the given info
    @param      keyframeIndex   The index of the keyframe you want to set
    @param      keyframe        An FxKeyframe structure with the information you want to
                                apply to the keyframe at the above index
    @param      parameterID     The ID of the parameter you want to set a keyframe for
    @param      channelIndex    The index of the channel of the parameter you want to set the
                                keyframe for
*/
- (NSError* _Nullable)setKeyframeIndex:(NSUInteger)keyframeIndex
                          withKeyframe:(const FxKeyframe*)keyframe
                          forParameter:(NSUInteger)parameterID
                            andChannel:(NSUInteger)channelIndex;

/*!
    @method     addKeyframe:toParameter:andChannel:
    @abstract   Adds a keyframe at the given time
    @param      keyframe        An FxKeyframe structure describing the keyframe you wish to add
                                to the parameter
    @param      parameterID     The ID of the parameter you wish to add a keyframe to
    @param      channelIndex    The index of the channel of the parameter you wish to add a
                                keyframe to. For parameters that only have one channel (sliders,
                                angle knobs, checkboxes, etc.) this will be 0. For compound
                                parameters, it will be the sub-channel. For example, if you wish
                                to keyframe the red channel of a color, it would be channel index
                                0, whereas the green channel would be channel index 1, etc.
*/
- (NSError* _Nullable)addKeyframe:(const FxKeyframe*)keyframe
                      toParameter:(NSUInteger)parameterID
                       andChannel:(NSUInteger)channelIndex;

/*!
    @method     removeKeyframeAtIndex:fromParameter:andChannel:
    @abstract   Removes the keyframe at the given index
    @param      keyframeIndex       The index of the keyframe you wish to remove
    @param      parameterID         The ID of the parameter whose keyframe you wish to remove
    @param      channelIndex        The index of the channel of the parameter whose keyframe you
                                    wish to remove
*/
- (NSError* _Nullable)removeKeyframeAtIndex:(NSUInteger)keyframeIndex
                              fromParameter:(NSUInteger)parameterID
                                 andChannel:(NSUInteger)channelIndex;

/*!
    @method     removeAllKeyframesForParameter:andChannel:
    @abstract   Remove all keyframes from the given parameter's channel
    @param      parameterID     The ID of the parameter whose keyframes you want to remove
    @param      channelIndex    The index of the channel of the parameter you wish to remove the
                                keyframes from
*/
- (NSError* _Nullable)removeAllKeyframesForParameter:(NSUInteger)parameterID
                                          andChannel:(NSUInteger)channelIndex;


/*!
    @method     parameter:channel:hasKeyframe:atTime:
    @abstract   Tells whether there is a keyframe at a given time
    @param      parameterID     The ID of the parameter you wish to query
    @param      channelIndex    The index of the channel of the parameter you wish to query
    @param      hasKeyframe     Returns whether or not the channel of the parameter specified
                                has a keyframe at the time specified
    @param      time            The time at which you wish to know if there's a keyframe
*/
- (NSError* _Nullable)parameter:(NSUInteger)parameterID
                        channel:(NSUInteger)channelIndex
                    hasKeyframe:(BOOL*)hasKeyframe
                         atTime:(CMTime)time;

/*!
    @method     keyframe:atOrBeforeTime:fromParameter:andChannel:
    @abstract   Returns the info about the nearest previous keyframe
    @param      keyframe        A pointer to a struct which will receive information about the
                                keyframe at or before the given time.
    @param      time            The time you want a keyframe at or before.
    @param      parameterID     The ID of the parameter for which you want the keyframe information.
    @param      channelIndex    The index of the channel of the parameter for which you want
                                the keyframe information.
*/
- (NSError* _Nullable)keyframe:(FxKeyframe*)keyframe
                atOrBeforeTime:(CMTime)time
                 fromParameter:(NSUInteger)parameterID
                    andChannel:(NSUInteger)channelIndex;

/*!
    @method     keyframe:atOrAfterTime:fromParameter:andChannel:
    @abstract   Returns the info about the nearest next keyframe
    @param      keyframe        A pointer to a struct which will receive information about the
                                keyframe at or after the given time
    @param      time            The time you want a keyframe at or after
    @param      parameterID     The ID of the parameter for which you want the keyframe information
    @param      channelIndex    The index of the channel of the parameter for which you want
                                the keyframe information
*/
- (NSError* _Nullable)keyframe:(FxKeyframe*)keyframe
                 atOrAfterTime:(CMTime)time
                 fromParameter:(NSUInteger)parameterID
                    andChannel:(NSUInteger)channelIndex;
@end

NS_ASSUME_NONNULL_END

#endif // __FXKEYFRAMEAPI_H__
