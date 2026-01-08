/*!
	@header		FxUndoAPI.h
	@abstract	Defines the plug-in protocol and host-API protocol for handling undo
				support.
*/

/* Copyright © 2005-2025 Apple Inc. All rights reserved. */

#import <Foundation/Foundation.h>
#import <FxPlug/FxTypes.h>

/*!
 @protocol   FxUndoAPI
 @abstract   Defines the methods provided by the host application 
             to let a plug-in manage the host application's undo queue.
 @discussion NOTE: Some host applications may not implement this protocol.
*/

@protocol FxUndoAPI

/*!
 @method     startUndoGroup
 @abstract	 Begins a new undo group.  Subsequent undo operations will be
             coalesced into this group until -endUndoGroup is called. Each
             call to -startUndoGroup must be balanced with a call to 
             -endUndoGroup.
 @param		 name	The name of the undo group. Some hosts may display this
                    name in their undo menu (i.e. "Undo Apply Preset").
                    This name should be localized and suitable for user display.
 @result     YES if the undo group was successfully started. NO otherwise.
 @discussion A plug-in can use this method to collapse multiple operations
             into one undo operation. For example, a plug-in may implement
             a "preset" popup, which changes the values of 20 other parameters
             using the FxParameterSettingAPI. By nesting the parameter changes
             in a startUndoGroup/endUndoGroup block, the 20 changes will be
             collapsed into one undo operation. This allows the user to undo
             the preset application by hitting undo once, instead of once per
             each changed parameter.
*/
- (BOOL)startUndoGroup: (NSString*) name;

/*!
 @method     endUndoGroup:
 @abstract   Ends the current undo group. Each call to -startUndoGroup must be 
             balanced with a call to -endUndoGroup.
 @result     YES if the undo group was successfully ended. NO otherwise.
 @discussion Closes the current undo group (see -startUndoGroup above).
*/
- (BOOL)endUndoGroup;

@end
