/*!
	@header		FxOnScreenControl.h
	@abstract	Defines the FxOnScreenControl protocol.
*/

/* Copyright © 2005-2025 Apple Inc. All rights reserved. */

/* Earlier versions of these APIs are not compatible with FxPlug 4.x and have been removed from
   the FxPlug SDK
 */

#include <FxPlug/FxTypes.h>

@class FxImageTile;

/*!
	@enum FxDrawingCoordinates
	@abstract Constants used to identify coordinate spaces.
	@discussion Y increases upwardly in all these coordinate spaces.
	@constant kFxDrawingCoordinates_CANVAS			Canvas window coordinates.
	@constant kFxDrawingCoordinates_DOCUMENT		Movie canvas coordinates.
	@constant kFxDrawingCoordinates_OBJECT			Object-specific coordinates
*/
enum
{
    kFxDrawingCoordinates_CANVAS = 0,
    kFxDrawingCoordinates_DOCUMENT = 1,
    kFxDrawingCoordinates_OBJECT = 2,
};
typedef NSUInteger FxDrawingCoordinates;


/*!
	@enum FxModifierKeys
	@discussion Flags used to identify modifier keys held down while an event is generated.
	@constant kFxModifierKey_SHIFT		Left or right Shift key
	@constant kFxModifierKey_CONTROL	Left or right Control key
	@constant kFxModifierKey_OPTION		Left or right Option key
	@constant kFxModifierKey_COMMAND	Command key
	@constant kFxModifierKey_CAPS_LOCK	Caps-Lock
*/
enum
{
    kFxModifierKey_SHIFT = 1 << 0,
    kFxModifierKey_CONTROL = 1 << 1,
    kFxModifierKey_OPTION = 1 << 2,
    kFxModifierKey_COMMAND = 1 << 3,
    kFxModifierKey_CAPS_LOCK = 1 << 4
};
typedef NSUInteger FxModifierKeys;


@protocol FxOnScreenControl_v4
/*!
    @method        drawingCoordinates
    @abstract      Defines the coordinate space for an event.
    @result        Returns the appropriate coordinate space.
 */
- (FxDrawingCoordinates)drawingCoordinates;

/*!
    @method       drawOSCWithWidth:height:activePart:destinationImage:atTime:
    @abstract     Draws a control on an image. This is the main method the host application calls
                  to draw a plug-in parameter control.
    @param        width             Width of the current image where the control will be drawn.
    @param        height            Height of the current image where the control will be drawn.
    @param        activePart        The active part of the control to draw.
    @param        destinationImage  An ioSurface into which you should draw your OSCs.
    @param        time              The time at which to draw the control.
    @discussion   This method is called when the host application wants you to draw your onscreen
                  controls. You should draw them into the destination image. The "activePart"
                  parameter tells which part of the control is currently selected so you can draw
                  that control with proper highlighting.
 */
- (void)drawOSCWithWidth:(NSInteger)width
                  height:(NSInteger)height
              activePart:(NSInteger)activePart
        destinationImage:(FxImageTile*)destinationImage
                  atTime:(CMTime)time;

/*!
    @method     hitTestOSCAtMousePositionX:mousePositionY:activePart:atTime:
    @abstract   Figures out which part of the onscreen control was hit
    @param      mousePositionX  The x position of the mouse
    @param      mousePositionY  The y position of the mouse
    @param      activePart      The part that was hit - set to 0 if no part was hit
    @param      time            The time at which we are checking for a hit
    @discussion This method is called by the host to determine which part of your controls were
                hit by the mouse. Return a constant that represents a part number for your
                onscreen control in the activePart parameter. It will be sent to you in the
                -draw, -mouse, and -key methods so you can handle those events properly.
 */
- (void)hitTestOSCAtMousePositionX:(double)mousePositionX
                    mousePositionY:(double)mousePositionY
                        activePart:(NSInteger*)activePart
                            atTime:(CMTime)time;

/*!
    @method     mouseDownAtPositionX:positionY:activePart:modifiers:forceUpdate:atTime:
    @abstract   Provides information when the left mouse button is pressed and the pointer is on an
                image with an active plug-in control.
    @param      mousePositionX   X position of the pointer (in 'drawingCoordinates' space).
    @param      mousePositionY   Y position of the pointer (in 'drawingCoordinates' space)
    @param      activePart       The active part of the control that currently contains the pointer.
    @param      modifiers        The keyboard modifiers currently pressed.
    @param      forceUpdate      Set to YES if the filter/generator associated with this control
                                 needs to be re-calculated; NO otherwise.
    @param      time             The current time when the mouse button was pressed.
    @discussion Depending on the host application, the time value may be relative to the start of
                the timeline or to the start of the clip.
                See the header file FxPlug/FxHostCapabilities.h.
 */
- (void)mouseDownAtPositionX:(double)mousePositionX
                   positionY:(double)mousePositionY
                  activePart:(NSInteger)activePart
                   modifiers:(FxModifierKeys)modifiers
                 forceUpdate:(BOOL *)forceUpdate
                      atTime:(CMTime)time;

/*!
    @method     mouseDraggedAtPositionX:positionY:activePart:modifiers:forceUpdate:atTime:
    @abstract   Provides information when the left mouse button is pressed and the pointer is
                moved with the mouse button still down.
    @param      mousePositionX  X position of the pointer (in 'drawingCoordinates' space).
    @param      mousePositionY  Y position of the pointer (in 'drawingCoordinates' space).
    @param      activePart      The active part of the control that currently contains the pointer.
    @param      modifiers       The keyboard modifiers currently pressed.
    @param      forceUpdate     Set to YES if the filter/generator associated with this control
                                needs to be re-calculated; NO otherwise.
    @param      time            The current time when the pointer was moved.
    @discussion Depending on the host application, the time value may be relative to the start of the
                timeline or to the start of the clip. See the header file FxPlug/FxHostCapabilities.h.
 */
- (void)mouseDraggedAtPositionX:(double)mousePositionX
                      positionY:(double)mousePositionY
                     activePart:(NSInteger)activePart
                      modifiers:(FxModifierKeys)modifiers
                    forceUpdate:(BOOL *)forceUpdate
                         atTime:(CMTime)time;

/*!
    @method       mouseUpAtPositionX:positionY:activePart:modifiers:forceUpdate:atTime:
    @abstract     Provides information when the left mouse button is released.
    @param        mousePositionX  X position of the pointer (in 'drawingCoordinates' space).
    @param        mousePositionY  Y position of the pointer (in 'drawingCoordinates' space).
    @param        activePart      The active part of the control that currently contains the pointer.
    @param        modifiers       The keyboard modifiers currently pressed.
    @param        forceUpdate     Set to YES if the filter/generator associated with this control
                                  needs to be re-calculated; NO otherwise.
    @param        time            The current time when the mouse button was released.
    @discussion   Depending on the host application, the time value may be relative to the start of the
                  timeline or to the start of the clip. See the header file FxPlug/FxHostCapabilities.h.
 */
- (void)mouseUpAtPositionX:(double)mousePositionX
                 positionY:(double)mousePositionY
                activePart:(NSInteger)activePart
                 modifiers:(FxModifierKeys)modifiers
               forceUpdate:(BOOL *)forceUpdate
                    atTime:(CMTime)time;

/*!
    @method       keyDownAtPositionX:positionY:keyPressed:modifiers:forceUpdate:didHandle:atTime:
    @abstract     Provides information when a key is pressed.
    @param        mousePositionX  X position of the pointer (in 'drawingCoordinates' space).
    @param        mousePositionY  Y position of the pointer (in 'drawingCoordinates' space).
    @param        asciiKey        The ASCII representation of the key pressed.
    @param        modifiers       The keyboard modifiers currently pressed.
    @param        forceUpdate     Set to YES if the filter/generator associated with this control
                                  needs to be re-calculated; NO otherwise.
    @param        didHandle       Set to YES if the control performed an action because of this key
                                  press;  otherwise, the event is passed back up the object tree.
    @param        time            The current time when the key press happened.
    @discussion   Depending on the host application, the time value may be relative to the start of the
                  timeline or to the start of the clip. See the header file FxPlug/FxHostCapabilities.h.
 */
- (void)keyDownAtPositionX:(double)mousePositionX
                 positionY:(double)mousePositionY
                keyPressed:(unsigned short)asciiKey
                 modifiers:(FxModifierKeys)modifiers
               forceUpdate:(BOOL *)forceUpdate
                 didHandle:(BOOL *)didHandle
                    atTime:(CMTime)time;

/*!
    @method       keyUpAtPositionX:positionY:keyPressed:modifiers:forceUpdate:didHandle:atTime:
    @abstract     Provides information when a key is released.
    @param        mousePositionX  X position of the pointer (in 'drawingCoordinates' space).
    @param        mousePositionY  Y position of the pointer (in 'drawingCoordinates' space).
    @param        asciiKey        The ASCII representation of the key released.
    @param        modifiers       The keyboard modifiers currently pressed.
    @param        forceUpdate     Set to YES if the filter/generator associated with this control
                                  needs to be re-calculated; NO otherwise.
    @param        didHandle       Set to YES if the plug-in control performed an action because of the
                                  key release; otherwise, the event is passed back up the object tree.
    @param        time            The current time when the key release happened.
    @discussion   Depending on the host application, the time value may be relative to the start of the
                  timeline or to the start of the clip. See the header file FxPlug/FxHostCapabilities.h.
 */
- (void)keyUpAtPositionX:(double)mousePositionX
               positionY:(double)mousePositionY
              keyPressed:(unsigned short)asciiKey
               modifiers:(FxModifierKeys)modifiers
             forceUpdate:(BOOL *)forceUpdate
               didHandle:(BOOL *)didHandle
                  atTime:(CMTime)time;

@optional
/*!
    @method       mouseMovedAtPositionX:positionY:activePart:modifiers:forceUpdate:atTime:
    @abstract     Provides information when the mouse pointer changes location.
    @param        mousePositionX    X position of the pointer (in 'drawingCoordinates' space).
    @param        mousePositionY    Y position of the pointer (in 'drawingCoordinates' space).
    @param        activePart        The active part of the control that currently contains the pointer.
    @param        modifiers         The keyboard modifiers currently pressed.
    @param        forceUpdate       Set to YES if the filter/generator associated with this control
                                    needs to be re-calculated; NO otherwise.
    @param        time              The current time when the mouse moved.
 */
- (void)mouseMovedAtPositionX:(double)mousePositionX
                    positionY:(double)mousePositionY
                   activePart:(NSInteger)activePart
                    modifiers:(FxModifierKeys)modifiers
                  forceUpdate:(BOOL *)forceUpdate
                       atTime:(CMTime)time;

/*!
    @method       mouseEnteredWithPositionX:positionY:modifiers:forceUpdate:atTime:
    @abstract     Provides information when the mouse pointer enters the view.
    @param        mousePositionX    X position of the pointer (in 'drawingCoordinates' space).
    @param        mousePositionY    Y position of the pointer (in 'drawingCoordinates' space).
    @param        modifiers         The keyboard modifiers currently pressed.
    @param        forceUpdate       Set to YES if the filter/generator associated with this control
                                    needs to be re-calculated; NO otherwise.
    @param        time              The current time when the mouse entered the OSC
 */
- (void)mouseEnteredAtPositionX:(double)mousePositionX
                      positionY:(double)mousePositionY
                      modifiers:(FxModifierKeys)modifiers
                    forceUpdate:(BOOL *)forceUpdate
                         atTime:(CMTime)time;

/*!
    @method       mouseExitedWithPositionX:positionY:modifiers:forceUpdate:atTime:
    @abstract     Provides information when the mouse pointer leaves the view.
    @param        mousePositionX   X position of the pointer (in 'drawingCoordinates' space).
    @param        mousePositionY   Y position of the pointer (in 'drawingCoordinates' space).
    @param        modifiers        The keyboard modifiers currently pressed.
    @param        forceUpdate      Set to YES if the filter/generator associated with this control
                                   needs to be re-calculated; NO otherwise.
    @param        time             The current time when the mouse exited the OSC
 */
- (void)mouseExitedAtPositionX:(double)mousePositionX
                     positionY:(double)mousePositionY
                     modifiers:(FxModifierKeys)modifiers
                   forceUpdate:(BOOL *)forceUpdate
                        atTime:(CMTime)time;

@end

