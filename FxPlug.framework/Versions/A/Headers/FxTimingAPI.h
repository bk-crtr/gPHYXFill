/*!
	@header		FxTimingAPI.h
	@abstract	Defines the FxTimingAPI protocol.
	@discussion	Defines the FxTimingAPI host-API protocol.
*/

/* Copyright © 2005-2025 Apple Inc. All rights reserved. */

/* Earlier versions of these APIs are not compatible with FxPlug 4.x and have been removed from
   the FxPlug SDK
 */

#if !STUDIO_LITE

/*!
	@protocol   FxTimingAPI_v4
	@abstract   A protocol that defines the methods provided by the host, so a plug-in can
                query the timing properties of its input.
	@discussion	Using the FxTimingAPI, your plug-in can query the timing properties of its input
                image or images, image parameters, effect, timeline, and in or out points.
                Note: This version of the API is only available to plug-ins built in the FxPlug 4
                style and later, and it intentionally doesn't inherit from previous versions.
*/
@protocol FxTimingAPI_v4

/*!
	@method		frameDuration:
	@abstract   Provides the native frame duration of the object to which the effect is applied.
	@param		duration       The frame duration time, expressed in CMTime.
    @discussion Motion provides the frame duration of the project. Final Cut Pro provides the frame
                duration of the clip to which the effect is applied, not the frame duration for the
                timeline. Note that the value that the host provides is unaffected by any retiming
                the host may do and is always the native frame duration (before retiming) of the
                clip to which the effect is applied. To determine the timeline’s frame duration,
                divide the value from @c -timelineFpsDenominatorForEffect: by
                @c -timelineFpsNumeratorForEffect:.
*/
- (void)frameDuration:(CMTime*)duration;

/*!
	@method		sampleDuration:
	@abstract   Provides the native sample duration of the object to which the effect is applied.
	@param		duration       The sample duration time, expressed in CMTime.
    @discussion Motion provides the sample duration of the project. Final Cut Pro provides the
                sample duration of the clip to which the effect is applied, not the sample duration
                for the timeline. Note that in Final Cut Pro, the sample duration is equal to the
                object’s native frame duration for progressive clips, but half of the frame duration
                for interlaced clips.
*/
- (void)sampleDuration:(CMTime*)duration;

/*!
	@method		startTimeForEffect:
	@abstract   Provides the start time for the effect.
	@param		startTime       The start time for the effect, expressed in CMTime.
    @discussion In Motion, this is the time the effect starts relative to timeline time, but in
                Final Cut Pro this is the time the effect starts relative to the input object’s
                native start time. For example, if an object’s head is trimmed by 2 seconds and the
                effect starts at the head of the template, then -startTimeForEffect: will return 2
                seconds. This value changes when the user retimes the clip in Final Cut Pro, so a
                200-percent increase in speed halves the start time to 1 second.
*/
- (void)startTimeForEffect:(CMTime*)startTime;

/*!
	@method		durationTimeForEffect:
	@abstract   Provides the duration time for the effect.
	@param		duration        The duration time for the effect, expressed in CMTime.
    @discussion In Motion, the value this method provides specifies the duration of the effect in
                the project. In Final Cut Pro, because the effects you use in a template don’t need
                to cover the whole effect source clip, this value represents the duration of the
                effect inside the template, not the duration of the template when the user applies
                it to a clip on the timeline. However, in most use-cases the effect duration is
                equal to the template duration.

*/
- (void)durationTimeForEffect:(CMTime*)duration;

/*!
	@method		startTimeOfInputToFilter:
	@abstract   Provides the start time of the filter's image input clip.
	@param		startTime		The start time of the clip, in CMTime.
    @discussion In Motion, this value represents the start time of the clip to which the user
                applies the effect, relative to the timeline time. This start time accounts for any
                trimming or retiming. It is the start time on the timeline of the first visible
                frame of the object.
                In Final Cut Pro, this value represents the start time of the object relative to its
                native start, after retiming or trimming. For example, if the user trimmed the
                clip’s head by 10 seconds, then the start time is 10 seconds. If the user then
                retimes the clip by 200-percent, the start time is 5 seconds.
*/
- (void)startTimeOfInputToFilter:(CMTime*)startTime;

/*!
	@method		durationTimeOfInputToFilter:
	@abstract   Provides the duration of the filter's image input clip.
	@param		duration		The duration of the clip, in CMTime.
    @discussion In Motion, the value represents the duration of the object to which the user applies
                the effect, after retiming and trimming. In other words, this value is the duration
                of the visible frames. In Final Cut Pro, the value represents the duration of the
                object to which the user applies the effect template, regardless of the length of
                the Effect Source inside the template.
*/
- (void)durationTimeOfInputToFilter:(CMTime*)duration;

/*!
	@method		startTime:ofImageParameter:
	@abstract   Provides the start time of the clip the user assigned to the given image parameter
				for the effect.
    @param      startTime           The start time of the clip, in CMTime.
	@param		parameterID			The ID of the image parameter.
    @discussion In Motion, startTime represents the native start time of the image well input,
                relative to timeline time.
*/
- (void)startTime:(CMTime*)startTime
 ofImageParameter:(UInt32)parameterID;

/*!
	@method		durationTime:ofImageParameter:
	@abstract   Provides the duration of the clip the user assigns to the given image parameter
				for the effect.
    @param      duration            The duration of the clip, in CMTime.
	@param		parameterID			The ID of the image parameter.
    @discussion In Motion, duration represents the duration of the the image well input after the
                user has trimmed or retimed it.
*/
- (void)durationTime:(CMTime*)duration
    ofImageParameter:(UInt32)parameterID;

/*!
	@method		inPointTimeOfTimelineForEffect:
	@abstract   Provides the in point of the timeline on which the user applies the effect.
	@param		inPoint			The in time, in CMTime.
*/
- (void)inPointTimeOfTimelineForEffect:(CMTime*)inPoint;

/*!
	@method		outPointTimeOfTimelineForEffect:
	@abstract   Provides the out point of the timeline on which the user applies the effect.
	@param		outPoint		The out time, in CMTime.
*/
- (void)outPointTimeOfTimelineForEffect:(CMTime*)outPoint;

/*!
	@method		timelineTime:fromInputTime:
	@abstract   Converts from input time of the filter's image input to timeline time.
	@param		timelineTime	The converted time, in CMTime.
	@param		time			The time of the input, in CMTime.
    @discussion In Motion, this timeline time and input time are always equal. In Final Cut Pro,
                this will provides the calculated timeline time for any given input time.
*/
- (void)timelineTime:(CMTime*)timelineTime
       fromInputTime:(CMTime)time;

/*!
	@method		timelineTime:fromImageTime:forParameterID:
	@abstract   Converts from image time of the given parameter to timeline time.
	@param		timelineTime	The converted time, in CMTime.
	@param		time			The parameter's time, in CMTime.
    @param      parameterID     The ID of the parameter.
*/
- (void)timelineTime:(CMTime*)timelineTime
       fromImageTime:(CMTime)time
      forParameterID:(UInt32)parameterID;

/*!
	@method		inputTime:fromTimelineTime:
	@abstract   Converts from timeline time to time of the filter's image input.
	@param		inputTime	    The converted time, in CMTime.
	@param		time			The timeline time, in CMTime.
    @discussion In Motion, this timeline time and input time are always equal. In Final Cut Pro,
                this method provides the calculated input time for any given timeline time.
*/
- (void)inputTime:(CMTime*)inputTime
 fromTimelineTime:(CMTime)time;

/*!
	@method		imageTime:fromParameterID:fromTimelineTime:
	@abstract   Converts from timeline time of the given parameter to image time.
	@param		imageTime	    The converted time, in CMTime.
    @param      parameterID     The ID of the parameter.
    @param      time            The parameter's time, in CMTime.
*/
- (void)imageTime:(CMTime*)imageTime
   forParameterID:(UInt32)parameterID
 fromTimelineTime:(CMTime)time;

/*!
	@method		fieldOrderForInputToFilter:
	@abstract   Return the field order of the filter input.
	@param		filter			The plug-in object.
	@result		An FxFieldOrder value (@c kFxFieldOrder_PROGRESSIVE, @c kFxFieldOrder_TOP_FIRST,
				or @c kFxFieldOrder_LOWER_FIRST).
    @discussion This method’s value refers to the field order of the input clip to which the user
                applies the effect, not the field order of the timeline.
*/
-(FxFieldOrder)fieldOrderForInputToFilter:(id<FxTileableEffect>)filter;

/*!
	@method		timelineFpsNumeratorForEffect:
	@abstract   Provides the numerator of the timeline's frame rate where the user applies the
                effect.
	@param		effect			The plug-in object.
	@result		The numerator of the frame rate.
	@discussion	You can use the numerator value with the companion denominator value to determine
                frames per second, or frame duration, of the timeline. For example, for 29.97 fps
                timelines, you might see values such as 30000 for the numerator, and 1001 for the
                denominator.
*/
- (NSUInteger)timelineFpsNumeratorForEffect:(id<FxTileableEffect>)effect;

/*!
	@method		timelineFpsDenominatorForEffect:
	@abstract   Provides the denominator of the timeline's frame rate where the user applies the
                effect.
	@param		effect			The plug-in object.
	@result		The denominator of the frame rate.
	@discussion	You can use the denominator value with the companion numerator value to determine
                frames per second, or frame duration, of the timeline. For example, for 29.97 fps
                timelines, you might see values such as 30000 for the numerator, and 1001 for the
                denominator.
*/
- (NSUInteger)timelineFpsDenominatorForEffect:(id<FxTileableEffect>)effect;

@end
#endif // !STUDIO_LITE
