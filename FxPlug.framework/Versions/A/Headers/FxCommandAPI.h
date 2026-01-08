/*!
 @header        FxCommandAPI.h
 @abstract      Defines the FxCommandAPI protocols for commands that you can request the host to
                perform.
 */
/* Copyright © 2020-2025 Apple Inc. All rights reserved. */

#ifndef FxCommand_h
#define FxCommand_h

/*!
    @enum       FxCommand
    @abstract   Commands that you can request the host perform.
    @constant   kFxCommand_PreviousFrame            Move the playhead back 1 frame.
    @constant   kFxCommand_NextFrame                Move the playhead ahead 1 frame.
    @constant   kFxCommand_PreviousEdit             Move the playhead to the previous edit in FCPX or the selection in point in Motion.
    @constant   kFxCommand_NextEdit                 Move the playhead to the next edit in FCPX or the selection out point in Motion.
    @constant   kFxCommand_PreviousMarker           Move the playhead to the previous marker of the currently selected object.
    @constant   kFxCommand_NextMarker               Move the playhead to the next marker of the currently selected object.
    @constant   kFxCommand_PreviousKeyframe         Move the playhead to the previous keyframe of the currently selected object.
    @constant   kFxCommand_NextKeyframe             Move the playhead to the next keyframe of the currently selected object.
    @constant   kFxCommand_ReversePlayback          Jog in reverse (unavailable in Motion).
    @constant   kFxCommand_PausePlayback            Pauses playback in FCPX. In Motion, use TogglePlayback instead.
    @constant   kFxCommand_ForwardPlayback          Jog forward (use TogglePlayback in Motion).
    @constant   kFxCommand_TogglePlayback           If paused, starts playback; if playing back, pauses.
    @constant   kFxCommand_Undo                     Undo the last change.
    @constant   kFxCommand_Redo                     Redo the last undone change.
    @constant   kFxCommand_ToggleWaveforms          Show the waveforms scope in FCPX (unavailable in Motion).
    @constant   kFxCommand_ToggleInspectorHeight    Toggles the full-height inspector in FCPX (Motion's Inspector is always full-height).
    @constant   kFxCommand_ToggleSidebar            Toggles the Sidebar in FCPX or the Library in Motion.
    @constant   kFxCommand_ToggleEvents             Toggles the event viewer in FCPX or the Media tab in Motion.
    @constant   kFxCommand_Preferences              Shows the application's preferences.
 */

typedef NS_ENUM(NSUInteger, FxCommand) {
    kFxCommand_PreviousFrame,
    kFxCommand_NextFrame,
    kFxCommand_PreviousEdit,    // Selection In Point in Motion
    kFxCommand_NextEdit,        // Selection Out Point in Motion
    kFxCommand_PreviousMarker,
    kFxCommand_NextMarker,
    kFxCommand_PreviousKeyframe,
    kFxCommand_NextKeyframe,
    
    kFxCommand_ReversePlayback, // Unavailable in Motion
    kFxCommand_PausePlayback,   // use TogglePlayback in Motion
    kFxCommand_ForwardPlayback, // use TogglePlayback in Motion
    kFxCommand_TogglePlayback,
    
    kFxCommand_Undo,
    kFxCommand_Redo,
    
    kFxCommand_ToggleWaveforms,         // Unavailable in Motion
    kFxCommand_ToggleInspectorHeight,   // Motion Inspector is always full height
    kFxCommand_ToggleSidebar,           // Show Library in Motion
    kFxCommand_ToggleEvents,            // Show Media tab in Motion
    kFxCommand_Preferences,
};

extern NSString*    kKey_Command;     // The FxCommand that will be invoked by this keypress.
extern NSString*    kKey_Character;   // The character the user must press to invoke the command.
                                      // If this value is 0, then there is no key assigned to this
                                      // command.
extern NSString*    kKey_Modifiers;   // The modifiers the user must include in the keypress to
                                      // invoke the command. See FxOnScreenControl.h for the
                                      // FxModifierKeys definitions.

// Some useful key characters you'll need to extract:
extern unichar  kKeyChar_UpArrow;       // Unicode 0x21de
extern unichar  kKeyChar_LeftArrow;     // Unicode 0x21e0
extern unichar  kKeyChar_DownArrow;     // Unicode 0x21df
extern unichar  kKeyChar_RightArrow;    // Unicode 0x21e2

/*!
    @protocol   FxCommandHandler
    @abstract   Defines the methods a plug-in must implement to be notified of the application's key binding for common
                commands.
 */

@protocol FxCommandHandler

/*!
    @method     -setHostKeyCommands:
    @param      newCommands - An array of dictionaries mapping FxCommands to keys and modifiers.
                            Each element of the array is an NSDictionary containing 3 key-value pairs for
                            the keys: kKey_Command, kKey_Character, and kKey_Modifiers.
                            Note that if a command does not have a key equivalent assigned in the host application,
                            the kKey_Character key will have a value of 0. Each application may implement
                            only a subset of the desired commands. Furthermore, there may be more than one
                            key combination assigned to a given function. You should support all of them.
    @discussion If your plug-in displays its own custom window, you may wish to support the passing of some key presses
                to the application in order for the application to handle them. This method gives you a mapping from
                some common functions that plug-ins would like to pass on to the host application to their key commands.
                This method will be called whenever the user changes the key bindings for the application.
 */
- (void)setHostKeyCommands:(NSArray<NSDictionary*>*)newCommands;

@end


/*!
    @protocol   FxCommandAPI
    @abstract   Allows a plug-in to send key commands to the host application.
 */
@protocol FxCommandAPI

/*!
    @method     -performCommand:error:
    @abstract   Instructs the host application to perform a command.
    @param      command     The command you wish the host application to perform.
    @param      error       Any errors encountered while attempting to perform the command.
    @result     A boolean indicating whether the application successfully performed the command. If it did not, the
                error parameter will be filled out with an error explaining what went wrong.
 */
- (BOOL)performCommand:(FxCommand)command
                 error:(NSError**)error;

@end


/*!
    @protocol   FxCommandAPI_v2
    @abstract   Adds functionality to move the playhead to a specific timeline time.
 */
@protocol FxCommandAPI_v2 <FxCommandAPI>

/*!
    @method     -movePlayheadToTime:error:
    @abstract   Moves the playhead to the desired time.
    @param      playheadTime    The time at which you want the playhead to be positioned.
    @param      error           Returns information about what went wrong upon failure.
    @result     A Boolean that indicates whether the application successfully moved the playhead.
                If the result is NO, the error parameter will have a description of what went wrong.
 */
- (BOOL)movePlayheadToTime:(CMTime)playheadTime
                     error:(NSError**)error;

@end

#endif /* FxCommand_h */
