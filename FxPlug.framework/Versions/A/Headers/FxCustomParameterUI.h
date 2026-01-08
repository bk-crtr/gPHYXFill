/*!
	@header		FxCustomParameterUI.h
	@abstract	Defines the plug-in protocol and host-API protocol for Custom Parameter View
				support.
*/

/* Copyright © 2005-2025 Apple Inc. All rights reserved. */

/* Earlier versions of these APIs are not compatible with FxPlug 4.x and have been removed from
   the FxPlug SDK
 */

#import <Cocoa/Cocoa.h>
#import <FxPlug/FxTypes.h>


@protocol FxCustomParameterViewHost_v2

/*!
	@method		createViewForParameterID:
	@abstract   Provides an NSView to be associated with the given parameter.
	@param		parameterID -  The ID of the parameter to be associated with the custom UI.
	@result		An NSView or subclass of NSView.
	@discussion	This plug-in method is called by the host application during the parameter-list
				setup sequence, once for each plug-in parameter that has the
				@c kFxParameterFlag_CUSTOM_UI parameter flag set. The view may be created
                dynamically, or, more commonly, retrieved from a NIB file in the plug-in's resources
                directory.
 
				NOTE: The object returned by this method should not be autoreleased. It should be
				allocated by the method, and will be released by the caller. The implementation
				may look something like this:
<pre>@textblock
if ( parmId == kMyViewParmID )
return [[MyView alloc] init];
@/textblock</pre>
*/
- (NSView *)createViewForParameterID:(UInt32)parameterID NS_RETURNS_RETAINED;

@end


/*!
    @protocol   FxCustomParameterActionAPI_v4
 	@abstract   Defines the methods the host application provides to support a custom parameter
				view.
	@discussion	Because custom-parameter views may get user events at any time, this protocol
				provides methods to prepare the host application for parameter changes or other
				actions at arbitrary times.
 
				NOTE: It is NOT safe to set or get parameter values at arbitrary times outside of a
				startAction:/endAction: pair. For example, when a custom-parameter view receives a
				keyDown event, it may want to change its parameter value. It would then call the
				following sequence:
 
<pre>@textblock
id <FxCustomParameterActionAPI> actionAPI = [apiManager apiForProtocol:FxCustomParameterActionAPI];
id <FxParameterSettingAPI> settingAPI = [apiManager apiForProtocol:FxParameterSettingAPI];
CMTime time = [actionAPI currentTime];
 
[actionAPI startAction:self];
[settingAPI setCustomParameterValue:myObject toParameter:myParameterID atTime:time];
[actionAPI endAction:self];
@/textblock</pre>
 
                This protocol is only available to FxPlug 4 and later plug-ins.
 */
@protocol FxCustomParameterActionAPI_v4

/*!
	@method		startAction:
	@abstract   Prepares the host to access parameters.
	@param		sender	The view class that implements this protocol. Typically passed as self.
    @discussion It is important to note that OSC plug-ins (implementing @c FxOnScreenControl) should
                not use @c startAction / @c endAction, as the host is already aware that parameters
                are likely to change.
*/
- (void)startAction:(id)sender;

/*!
	@method		endAction:
	@abstract   Tells the host application that the plug-in is done accessing parameters.
	@param		sender	The view class that implements this protocol. Typically passed as self.
    @discussion It is important to note that OSC plug-ins (implementing @c FxOnScreenControl) should
                not use @c startAction / @c endAction, as the host is already aware that parameters
                are likely to change.
*/
- (void)endAction:(id)sender;

/*!
	@method		currentTime
	@abstract   Returns the current time, expressed as a rational time.
	@result		The current time, expressed as a rational time.
	@discussion	This is useful when a custom parameter view needs to set a parameter value in
				response to a user event. Depending on the host application, the time value
				may be relative to the start of the timeline or to the start of the clip.
 
                CAUTION: Only use -currentTime from within methods that are not provided the time
                by the host. For example, when updating a custom view.
*/
- (CMTime)currentTime;

@end
