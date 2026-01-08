/*!
 @header        FxAnalysis.h
 @abstract      Defines the FxAnalysisAPI and FxAnalyzer protocols for analyzing frames of the
                video clip that the FxPlug plug-in is applied to.
 */
/* Copyright © 2019-2025 Apple Inc. All rights reserved. */


#ifndef FxAnalysis_h
#define FxAnalysis_h

#import <Foundation/Foundation.h>
#import <CoreMedia/CMTimeRange.h>
#import <FxPlug/FxImageTile.h>

/*!
	@protocol	FxAnalyzer
	@abstract	A protocol your plug-in implements to analyze frames that the
                plug-in is applied to.
	@discussion	Implement this protocol if your plug-in needs to analyze any frames your plug-in
                is applied to before it can render those frames.
*/
@protocol FxAnalyzer

/*!
	@method 	desiredAnalysisTimeRange:forInputWithTimeRange:error:
	@abstract	Requests the specified time range of frames you want to analyze.
	@param		desiredRange	 The specified time range.
    @param      inputTimeRange   The input time range.
    @param      error            Description of errors that occurred while calling this method.
	@result		Return YES if you were able to get the desired time range.
                Return NO if you were not able to for some reason. When returning NO, be sure to
                also set the error parameter to a descriptive value.
*/
- (BOOL)desiredAnalysisTimeRange:(CMTimeRange*)desiredRange
           forInputWithTimeRange:(CMTimeRange)inputTimeRange
                           error:(NSError**)error;

/*!
    @method      setupAnalysisForTimeRange:frameDuration:error:
    @abstract    Completes any setup work before starting the frame analysis.
    @param       analysisRange    The analysis time range.
    @param       frameDuration    The frame duration.
    @param       error            Descriptions of errors that occurred while calling this method.
    @result      YES if you set up analysis successfully for the given time range, or NO if you didn't.
                 When returning NO, be sure to also set the error parameter to a descriptive value.


*/
- (BOOL)setupAnalysisForTimeRange:(CMTimeRange)analysisRange
                    frameDuration:(CMTime)frameDuration
                            error:(NSError**)error;

/*!
    @method      analyzeFrame:atTime:error:
    @abstract    Analyze the next frame.
    @param       frame      The current frame to analyze.
    @param       frameTime  The time of the frame you're being passed in the "frame" parameter.
    @param       error      Return any errors that occurred while calling this method.
    @result      YES if you successfully analyzed the frame, NO if you didn't. When returning NO,
                 be sure to also set the error parameter to a descriptive value. Returning NO has
                 the effect of canceling the rest of the analysis.
*/
- (BOOL)analyzeFrame:(FxImageTile*)frame
              atTime:(CMTime)frameTime
               error:(NSError**)error;

/*!
    @method      cleanupAnalysis:
    @abstract    Completes any necessary cleanup work after your plug-in's frame analysis.
    @param       error      Return any errors that occurred while calling this method.
    @result      Return YES if you were able to cleanup anything you set up in
                 -setupAnalysisForTimeRange:frameDuration:error:.
                 Return NO if you were not able to for some reason. When returning NO, be sure to
                 also set the error parameter to a descriptive value.
    @discussion  This work is often paired with setup work done in
                 -setupAnalysisForTimeRange:frameDuration:error:.
*/
- (BOOL)cleanupAnalysis:(NSError**)error;

@end

/*!
    @enum       FxAnalysisState
    @abstract   The possible states of frame analysis, as reported by the host application when
                queried with analysisStateForEffect().
    @constant   kFxAnalysisState_NotAnalyzing           Not currently analyzing.
    @constant   kFxAnalysisState_AnalysisRequested      Analysis requested.
    @constant   kFxAnalysisState_AnalysisStarted        Analysis started.
    @constant   kFxAnalysisState_AnalysisCompleted      Analysis completed.
    @constant   kFxAnalysisState_AnalysisInterrupted    Analysis interrupted.
*/
typedef NSUInteger FxAnalysisState;
NS_ENUM (FxAnalysisState) {
    kFxAnalysisState_NotAnalyzing,
    kFxAnalysisState_AnalysisRequested,
    kFxAnalysisState_AnalysisStarted,
    kFxAnalysisState_AnalysisCompleted,
    kFxAnalysisState_AnalysisInterrupted
};

/*!
    @protocol    FxAnalysisAPI
    @abstract    A protocol that host applications implement to provide information to plug-ins that
                 support the FxAnalyzer protocol.
    @discussion  Host applications implement this protocol to provide frame analysis information to
                 plug-ins that support the FxAnalyzer protocol.
*/
@protocol FxAnalysisAPI

/*!
    @method      analysisStateForEffect:
    @abstract    Fetches the state of frame analysis for your plug-in.
    @result      An analysis state (FxAnalysisState) of running, completed, or interrupted.
*/
- (FxAnalysisState)analysisStateForEffect;

/*!
    @method      startForwardAnalysis:
    @abstract    Requests that the application begin a forward analysis of a clip from start to end.
    @param       location   FxAnalysisLocation type to choose to do analysis on either GPU or CPU.
    @param       error      Descriptions of any errors that occurred while calling this method.
    @result      YES if the host successfully started forward analysis of the clip, or NO if it
                 didn't. When returning NO, be sure to also set the error parameter to a descriptive
                 value.
*/
- (BOOL)startForwardAnalysis:(FxAnalysisLocation)location
                       error:(NSError**)error;

/*!
    @method      startBackwardAnalysis:
    @abstract    Requests that the application begin a backwards analysis of clip from end to start.
    @param       location   FxAnalysisLocation type to choose to do analysis on either GPU or CPU.
    @param       error      Descriptions of any errors that occurred while calling this method.
    @result      YES if the host successfully started backward analysis of the clip, or NO if it
                 didn’t. When returning NO, be sure to also set the error parameter to a descriptive
                 value.
*/
- (BOOL)startBackwardAnalysis:(FxAnalysisLocation)location
                        error:(NSError**)error;

@end

@protocol FxAnalysisAPI_v2 <FxAnalysisAPI>

/*!
    @method      startForwardAnalysis:ofParameter:error:
    @abstract    Requests that the application begin a forward analysis of a clip from start to end.
    @param       location   FxAnalysisLocation type to choose to do analysis on either GPU or CPU.
    @param       parameterID The ID of the image parameter you want to analyze
    @param       error      Descriptions of any errors that occurred while calling this method.
    @result      YES if the host successfully started forward analysis of the clip, or NO if it
                 didn't. The result will be NO if the parameter ID does not correspond to an image wel parameter.
*/
- (BOOL)startForwardAnalysis:(FxAnalysisLocation)location
                 ofParameter:(UInt32)parameterID
                       error:(NSError**)error;

/*!
    @method      startBackwardAnalysis:ofParameter:error:
    @abstract    Requests that the application begin a backwards analysis of clip from end to start.
    @param       location   FxAnalysisLocation type to choose to do analysis on either GPU or CPU.
    @param       parameterID The ID of the image parameter you want to analyze
    @param       error      Descriptions of any errors that occurred while calling this method.
    @result      YES if the host successfully started backward analysis of the clip, or NO if it
                 didn’t. The result will be NO if the parameter ID does not correspond to an image wel parameter.
*/
- (BOOL)startBackwardAnalysis:(FxAnalysisLocation)location
                  ofParameter:(UInt32)parameterID
                        error:(NSError**)error;


@end

#endif /* FxAnalysis_h */
