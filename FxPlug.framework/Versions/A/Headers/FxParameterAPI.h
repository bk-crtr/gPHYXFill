/*!
	@header		FxParameterAPI.h

	@abstract	Defines the FxParameter host API protocols for creating parameters,
				and for getting and setting parameter values.
*/

/* Copyright © 2005-2025 Apple Inc. All rights reserved. */

/* Earlier versions of these APIs are not compatible with FxPlug 4.x and have been removed from
   the FxPlug SDK
 */

#import <Foundation/Foundation.h>
#import <FxPlug/FxTypes.h>

NS_ASSUME_NONNULL_BEGIN

/*!
 @protocol      FxParameterCreationAPI_v5
 @abstract      Defines the methods provided by the host application to create parameters.
 @discussion    This is version 5 of the Parameter Creation API. This version 5 protocol is not a
                superset of version 4.
 
                This API is only available to plug-ins built in the FxPlug 4 style and later.
 
                NOTE: Parameter IDs must be in the range of [1, 9998]. IDs outside of this range
                are invalid.
 
 */
@protocol FxParameterCreationAPI_v5 // does not inherit from v4 intentionally

/*!
 @method       addFloatSliderWithName:parameterID:defaultValue:parameterMin:parameterMax:sliderMin:sliderMax:delta:parameterFlags:
 @abstract     Creates a floating point slider parameter and adds it to the plug-in's parameter list.
 @param        name             Name of the parameter in the user interface.
 @param        parameterID      Unique identifier for a parameter. Used to retrieve, set, or
                                validate the parameter.
 @param        defaultValue     Initial value for the parameter.
 @param        min              Minimum value for the parameter.
 @param        max              Maximum value for the parameter.
 @param        sliderMin        Minimum value for the parameter's slider control.
 @param        sliderMax        Maximum value for the parameter's slider control.
 @param        sliderDelta      Step value added or removed when the slider is moved.
 @param        flags            Flags that encode various state values for the parameter. Use
                                @c kFxParameterFlag_IGNORE_MINMAX to create a slider with unbounded
                                range. In this case, the min, max, sliderMin, and sliderMax values
                                are ignored.
 @result       Returns YES if the application successfully creates a floating point slider
               parameter; NO otherwise.
 */
- (BOOL)addFloatSliderWithName:(NSString *)name
                   parameterID:(UInt32)parameterID
                  defaultValue:(double)defaultValue
                  parameterMin:(double)min
                  parameterMax:(double)max
                     sliderMin:(double)sliderMin
                     sliderMax:(double)sliderMax
                         delta:(double)sliderDelta
                parameterFlags:(FxParameterFlags)flags;

/*!
 @method       addIntSliderWithName:parameterID:defaultValue:parameterMin:parameterMax:sliderMin:sliderMax:delta:parameterFlags:
 @abstract     Creates an integer slider parameter and adds it to the plug-in's parameter list.
 @param        name             Name of the parameter in the user interface.
 @param        parameterID      Unique identifier for a parameter. Used to retrieve, set, or validate the
                                parameter.
 @param        defaultValue     Initial value for the parameter.
 @param        min              Minimum value for the parameter.
 @param        max              Maximum value for the parameter.
 @param        sliderMin        Minimum value for the parameter's slider control.
 @param        sliderMax        Maximum value for the parameter's slider control.
 @param        sliderDelta      Step value added or removed when the slider is moved.
 @param        flags            Flags that encode various state values for the parameter. Use
                                @c kFxParameterFlag_IGNORE_MINMAX to create a slider with unbounded
                                range. In this case, the min, max, sliderMin, and sliderMax values
                                are ignored.
 @result       Returns YES if the application successfully creates an integer slider parameter; NO
               otherwise.
 */
- (BOOL)addIntSliderWithName:(NSString *)name
                 parameterID:(UInt32)parameterID
                defaultValue:(int)defaultValue
                parameterMin:(int)min
                parameterMax:(int)max
                   sliderMin:(int)sliderMin
                   sliderMax:(int)sliderMax
                       delta:(int)sliderDelta
              parameterFlags:(FxParameterFlags)flags;

/*!
 @method       addToggleButtonWithName:parameterID:defaultValue:parameterFlags:
 @abstract     Creates a checkbox toggle button parameter and adds it to the plug-in's parameter
 list.
 @param        name            Name of the parameter in the user interface.
 @param        parameterID     Unique identifier for a parameter. Used to retrieve, set, or
                               validate the parameter.
 @param        defaultValue    Initial value for the parameter.
 @param        flags           Flags that encode various state values for the parameter.
 @result       Returns YES if the application successfully creates a toggle parameter; NO otherwise.
 */
- (BOOL)addToggleButtonWithName:(NSString *)name
                    parameterID:(UInt32)parameterID
                   defaultValue:(BOOL)defaultValue
                 parameterFlags:(FxParameterFlags)flags;

/*!
 @method       addAngleSliderWithName:parameterID:defaultValue:parameterMin:parameterMax:parameterFlags:
 @abstract     Creates an angle slider parameter and adds it to the plug-in's parameter list.
 @param        name            Name of the parameter in the user interface.
 @param        parameterID     Unique identifier for a parameter. Used to retrieve, set, or
                               validate the parameter.
 @param        defaultDegrees  Initial angle value for the parameter, in counter-clockwise degrees.
 @param        minDegrees      Minimum value for the parameter, in counter-clockwise degrees.
 @param        maxDegrees      Maximum value for the parameter, in counter-clockwise degrees.
 @param        flags           Flags that encode various state values for the parameter.
 @result       Returns YES if the application successfully creates an angle slider parameter; NO
               otherwise.
 @discussion   The min, max, and defaultValue are all expressed in degrees.
               Use -getFloatValue: to retrieve the angle parameter value in radians.
 */
- (BOOL)addAngleSliderWithName:(NSString *)name
                   parameterID:(UInt32)parameterID
                defaultDegrees:(double)defaultDegrees
           parameterMinDegrees:(double)minDegrees
           parameterMaxDegrees:(double)maxDegrees
                parameterFlags:(FxParameterFlags)flags;

/*!
 @method       addColorParameterWithName:parameterID:defaultRed:defaultGreen:defaultBlue:parameterFlags:
 @abstract     Creates an RGB color parameter and adds it to the plug-in's parameter list.
 @param        name        Name of the parameter in the user interface.
 @param        parameterID Unique identifier for a parameter. Used to retrieve, set, or validate
                           the parameter.
 @param        red         Initial red value of the parameter in float ( 0.0 - 1.0 ) space.
 @param        green       Initial green value of the parameter in float ( 0.0 - 1.0 ) space.
 @param        blue        Initial blue value of the parameter in float ( 0.0 - 1.0 ) space.
 @param        flags       Flags that encode various state values for the parameter.
 @result       Returns YES if the application successfully creates an RGB color parameter; NO
               otherwise.
 */
- (BOOL)addColorParameterWithName:(NSString *)name
                      parameterID:(UInt32)parameterID
                       defaultRed:(double)red
                     defaultGreen:(double)green
                      defaultBlue:(double)blue
                   parameterFlags:(FxParameterFlags)flags;

/*!
 @method       addColorParameterWithName:parameterID:defaultRed:defaultGreen:defaultBlue:defaultAlpha:parameterFlags:
 @abstract     Creates an RGBA color value and adds it to the plug-in's parameter list.
 @param        name        Name of the parameter in the user interface.
 @param        parameterID Unique identifier for a parameter. Used to retrieve, set, or validate
                           the parameter.
 @param        red         Initial red value of the parameter in float ( 0.0 - 1.0 ) space.
 @param        green       Initial green value of the parameter in float ( 0.0 - 1.0 ) space.
 @param        blue        Initial blue value of the parameter in float ( 0.0 - 1.0 ) space.
 @param        alpha       Initial alpha value of the parameter in float ( 0.0 - 1.0 ) space.
 @param        flags       Flags that encode various state values for the parameter.
 @result       Returns YES if the application successfully creates an RGBA color parameter; NO
               otherwise.
 */
- (BOOL)addColorParameterWithName:(NSString *)name
                      parameterID:(UInt32)parameterID
                       defaultRed:(double)red
                     defaultGreen:(double)green
                      defaultBlue:(double)blue
                     defaultAlpha:(double)alpha
                   parameterFlags:(FxParameterFlags)flags;

/*!
 @method       addPointParameterWithName:parameterID:defaultX:defaultY:parameterFlags:
 @abstract     Creates a position point parameter and adds it to the plug-in's parameter list.
 @param        name        Name of the parameter in the user interface.
 @param        parameterID Unique identifier for a parameter. Used to retrieve, set, or validate
                           the parameter.
 @param        defaultX    Initial x value of the position parameter in float ( 0.0 - 1.0 from
                           bottom-left ) space
 @param        defaultY    Initial y value of the position parameter in float ( 0.0 - 1.0 from
                           bottom-left ) space
 @param        flags       Flags that encode various state values for the parameter.
 @result       Returns YES if the application successfully creates a position parameter; NO
               otherwise.
 */
- (BOOL)addPointParameterWithName:(NSString *)name
                      parameterID:(UInt32)parameterID
                         defaultX:(double)defaultX
                         defaultY:(double)defaultY
                   parameterFlags:(FxParameterFlags)flags;

/*!
 @method       addPopupMenuWithName:parameterID:defaultValue:menuEntries:parameterFlags:
 @abstract     Creates a popup menu parameter and adds it to the plug-in's parameter list.
 @param        name            Name of the parameter in the user interface.
 @param        parameterID     Unique identifier for a parameter. Used to retrieve, set, or validate
                               the parameter.
 @param        defaultValue    Index of the initial entry for the parameter.
 @param        entries         Array of NSStrings representing the entries in the menu.
 @param        flags           Flags that encode various state values for the parameter.
 @result       Returns YES if the application successfully creates a popup menu; NO otherwise.
 @discussion   Use -getIntValue: to retrieve menu parameter index value as an integrer.
 */
- (BOOL)addPopupMenuWithName:(NSString *)name
                 parameterID:(UInt32)parameterID
                defaultValue:(UInt32)defaultValue
                 menuEntries:(NSArray *)entries
              parameterFlags:(FxParameterFlags)flags;

/*!
 @method       addImageReferenceWithName:parameterID:parameterFlags:
 @abstract     Creates an image reference parameter and adds it to the plug-in's parameter list.
 @param        name        Name of the parameter in the user interface.
 @param        parameterID Unique identifier for a parameter. Used to retrieve, set, or validate the
               parameter.
 @param        flags       Flags that encode various state values for the parameter.
 @result       Returns YES if the application successfully creates an image reference parameter; NO
               otherwise.
 */
- (BOOL)addImageReferenceWithName:(NSString *)name
                      parameterID:(UInt32)parameterID
                   parameterFlags:(FxParameterFlags)flags;

/*!
 @method     addCustomParameterWithName:parameterID:defaultValue:parameterFlags:
 @abstract   Creates a custom parameter and adds it to the plug-in's parameter list.
 @param      name            Name of the parameter in the user interface.
 @param      parameterID     Unique identifier for a parameter. Used to retrieve, set, or
                             validate the parameter.
 @param      defaultValue    Initial entry for the parameter.
 @param      flags           Flags that encode various state values for the parameter.
 @result     Returns YES if the application successfully creates a custom parameter; NO otherwise.
 @discussion The defaultValue argument is an instance, created by the plug-in, of the custom
             parameter's class, which must conform to the NSCoding protocol. NSCoding is used
             for serializing and unserializing the custom parameter's value object for saving
             and loading.
 
             NOTE: Your implementation of the NSSecureCoding methods (@c -encodeWithCoder: and
             @c -initWithCoder:) must use keyed coding.
 
             NOTE: If you have a custom parameter, you must implement the
             @c FxCustomParameterInterpolation_v2 protocol for your custom class
 
             NOTE: If you have a custom parameter, you must implement the
             @c -classesForCustomParameterID: method in your plug-in
 */
- (BOOL)addCustomParameterWithName:(NSString *)name
                       parameterID:(UInt32)parameterID
                      defaultValue:(NSObject<NSSecureCoding, NSCopying>*)defaultValue
                    parameterFlags:(FxParameterFlags)flags;

/*!
 @method       startParameterSubGroup:parameterID:parameterFlags:
 @abstract     Starts a new parameter subgroup. All subsequent parameter additions are placed in
               this group until you send an endParameterSubGroup message.
 @param        name        Name of the parameter in the user interface.
 @param        parameterID Unique identifier for a parameter. Used to retrieve, set, or validate
                           the parameter.
 @param        flags       Flags that encode various state values for the parameter.
 @result       Returns YES if the application successfully creates a parameter sub group; NO
               otherwise.
 */
- (BOOL)startParameterSubGroup:(NSString *)name
                   parameterID:(UInt32)parameterID
                parameterFlags:(FxParameterFlags)flags;

/*!
 @method        endParameterSubGroup
 @abstract      Closes current parameter subgroup. You should always pair this with a preceding
                startParameterSubGroup message.
 @result        Returns YES if the application successfully closes the current parameter sub group;
                NO otherwise.
 */
- (BOOL)endParameterSubGroup;


/*!
 @method       addStringParameterWithName:parameterID:defaultValue:parameterFlags:
 @abstract     Creates a string parameter and adds it to the plug-in's parameter list.
 @param        name            Name of the parameter in the user interface.
 @param        parameterID     Unique identifier for a parameter. Used to retrieve, set, or
                               validate the parameter.
 @param        defaultValue    Initial value for the parameter.
 @param        flags           Flags that encode various state values for the parameter.
 @result       Returns YES if the application successfully creates a floating point slider
               parameter; NO otherwise.
 */
- (BOOL)addStringParameterWithName:(NSString *)name
                       parameterID:(UInt32)parameterID
                      defaultValue:(NSString *)defaultValue
                    parameterFlags:(FxParameterFlags)flags;

/*!
 @method       addPercentSliderWithName:parameterID:defaultValue:parameterMin:parameterMax:sliderMin:sliderMax:delta:parameterFlags:
 @abstract     Creates a percentage floating point slider parameter and adds it to the plug-in's parameter
               list.  A parameter value of 1.0 corresponds to a slider value of 100%
 @param        name            Name of the parameter in the user interface.
 @param        parameterID     Unique identifier for a parameter. Used to retrieve, set, or
                               validate the parameter.
 @param        defaultValue    Initial value for the parameter.
 @param        min             Minimum value for the parameter.
 @param        max             Maximum value for the parameter.
 @param        sliderMin       Minimum value for the parameter's slider control.
 @param        sliderMax       Maximum value for the parameter's slider control.
 @param        sliderDelta     Step value added or removed when the slider is moved.
 @param        flags           Flags that encode various state values for the parameter.
 @result       Returns YES if the application successfully creates a floating point slider
               parameter; NO otherwise.
 */
- (BOOL)addPercentSliderWithName:(NSString *)name
                     parameterID:(UInt32)parameterID
                    defaultValue:(double)defaultValue
                    parameterMin:(double)min
                    parameterMax:(double)max
                       sliderMin:(double)sliderMin
                       sliderMax:(double)sliderMax
                           delta:(double)sliderDelta
                  parameterFlags:(FxParameterFlags)flags;

/*!
 @method     addPathPickerWithName:parameterID:parameterFlags:
 @abstract   Creates a parameter for choosing an image mask path.
 @param      name         Name of the parameter in the user interface.
 @param      parameterID  Unique identifier for a parameter. Used to retrieve, set, or validate the parameter.
 @param      flags        Flags that encode various state values for the parameter.
 @result     Returns YES if the application successfully creates a path picker parameter;
             NO otherwise.
 */
- (BOOL)addPathPickerWithName:(NSString*)name
                  parameterID:(UInt32)parameterID
               parameterFlags:(FxParameterFlags)flags;

/*!
 @method       addPushButtonWithName:parameterID:callBackFunc:object:parameterFlags:
 @abstract     Creates a push button parameter and adds it to the plug-in's parameter list.
 @param        name            Name of the parameter in the user interface.
 @param        parameterID     Unique identifier for a parameter. Used to retrieve, set, or
                               validate the parameter.
 @param        selector        Selector that will be called when the button is pressed. Note that the selector should
                            take no parameters.
 @param        flags           Flags that encode various state values for the parameter.
 @result       Returns YES if the application successfully creates a push button parameter; NO otherwise.
 */
- (BOOL)addPushButtonWithName: (NSString *)name
                  parameterID: (UInt32)parameterID
                     selector: (SEL)selector
               parameterFlags: (FxParameterFlags)flags;

/*!
 @method       addHelpButtonWithName:parameterID:callBackFunc:object:parameterFlags:
 @abstract     Creates a help push button parameter and adds it to the plug-in's parameter list.
 @param        name            Name of the parameter in the user interface.
 @param        parameterID     Unique identifier for a parameter. Used to retrieve, set, or
                               validate the parameter.
 @param        selector        Selector that will be called when the button is pressed
 @param        flags           Flags that encode various state values for the parameter.
 @result       Returns YES if the application successfully creates a help button parameter; NO otherwise.
 */
- (BOOL)addHelpButtonWithName: (NSString *)name
                  parameterID: (UInt32)parameterID
                     selector: (SEL)selector
               parameterFlags: (FxParameterFlags)flags;


/*!
 @method       addFontMenuWithName:parameterID:family:typeface:parameterFlags:
 @abstract     Creates a font menu parameter and adds it to the plug-in's parameter
 list.
 @param        name            Name of the parameter in the user interface.
 @param        parameterID     Unique identifier for a parameter. Used to retrieve, set, or
                               validate the parameter.
 @param        fontName        Font name
 @param        flags           Flags that encode various state values for the parameter.
 @result       Returns YES if the application successfully creates a font parameter; NO otherwise.
 */
- (BOOL)addFontMenuWithName:(NSString*)name
                parameterID:(UInt32)parameterID
                   fontName:(NSString*)fontName
             parameterFlags:(FxParameterFlags)flags;

/*!
 @method       addHistogramWithName:parameterID:parameterFlags:
 @abstract     Creates a histogram parameter.
 @param        name         Name of the parameter in the user interface.
 @param        parameterID  Unique identifier for a parameter. Used to retrieve, set, or validate the
                            parameter.
 @param        flags        Flags that encode various state values for the parameter.
 @result       Returns YES if the application successfully creates a histogram parameter; NO
               otherwise.
 */
- (BOOL)addHistogramWithName:(NSString *)name
                 parameterID:(UInt32)parameterID
              parameterFlags:(FxParameterFlags)flags;

/*!
 @method       addGradientWithName:parameterID:parameterFlags:
 @abstract     Creates a gradient parameter.
 @param        name           Name of the parameter as it appears in the user interface.
 @param        parameterID    Unique identifier for a parameter. Used to retrieve, set, or validate
                              the parameter.
 @param        flags          Flags that encode various state values for the parameter.
 @result       Returns YES if the application successfully creates a gradient parameter; NO
               otherwise.
 */
- (BOOL)addGradientWithName:(NSString *)name
                parameterID:(UInt32)parameterID
             parameterFlags:(FxParameterFlags)flags;

@end


/*!
 @class     FxTaggedMenuEntry
 @abstract  An object that holds a menu item name and tag
 @discussion    Pass an array of FxTaggedMenuEntry objects to -[FxParameterCreationAPI_v6
                addTaggedPopupMenuWithName:parameterID:defaultValue:menuEntries:parameterFlags:]
                to create a popup menu with tagged items that can be accessed by the host app using
                tags instead of menu item position. This will allow you to change the order of menu items or insert menu
                items in the future without affecting existing user documents. You can convert an existing non-tagged popup menu
                parameter into a tagged one by calling the above method instead of -addPopupMenuWithName:::: in the next
                version of your plug-in.
 */
@interface FxTaggedMenuEntry : NSObject

@property (nonnull, readonly) NSString *menuItemName;
@property (readonly) NSUInteger tag;

+ (FxTaggedMenuEntry*)taggedMenuEntryWithName:(NSString*)itemName
                                          tag:(NSUInteger)tag;

@end

/*!
 */

@protocol FxParameterCreationAPI_v6 <FxParameterCreationAPI_v5>

/*!
 @method        -addTaggedPopupMenuWithName:parameterID:defaultValue:menuEntries:parameterFlags:
 @abstract      Creates a tagged popup menu parameter and adds it to the plug-in's parameter list.
 @param         name                     Name of the parameter in the user interface.
 @param         parameterID      Unique identifier for a parameter. Used to retrieve, set, or validate
                              the parameter.
 @param         defaultValue    Index of the initial entry for the parameter.
 @param         entries      Array of FxTaggedMenuEntry objects containing the text and tag for each menu item in the popup
 @param         flags  Flags that encode various state values for the parameter.
 @result        Returns YES if the application successfully creates a popup menu; NO otherwise.
 @discussion    If you use a tagged popup menu, then the value returned by -getIntValue::: will be
                the item's tag rather than its position in the menu. If an earlier version of your
                plug-in was created using a regular popup menu, you can use tagged menu
                items in newer versions of your plug-in and values will be translated properly.
                This allows you to insert items into the menu in any position without causing
                problems for projects created with older versions of your plug-in.
 */

- (BOOL)addTaggedPopupMenuWithName:(NSString *)name
                       parameterID:(UInt32)parameterID
                      defaultValue:(UInt32)defaultValue
                       menuEntries:(NSArray<FxTaggedMenuEntry*>*)entries
                    parameterFlags:(FxParameterFlags)flags;

@end


/*!
 @protocol   FxParameterRetrievalAPI_v6
 @abstract    Defines the functions the application provides to retrieve parameter values.
 @discussion    This is version 6 of the FxParameterRetrievalAPI protocol. It is NOT a superset
 of the v5 API.
 
 NOTE: This API is only available to plug-ins built in the FxPlug 4 style and later.
 
 */

@protocol FxParameterRetrievalAPI_v6 <NSObject> // NOTE: This does not inherit from the v5 API intentionally

/*!
     @method        getFloatValue:fromParameter:atTime:
     @abstract      Gets a floating point value from a parameter.
     @param         value        Pointer to the plug-in variable for the value.
     @param         parameterID  Unique identifier for the parameter.
     @param         time         Specified time to get the value.
     @result        Returns YES if the value is retrieved successfully; NO otherwise.
     @discussion    When used to get the value of an angle parameter, the result will be expressed
                    in counter-clockwise radians. Depending on the host application, the time value
                    may be relative to the start of the timeline or to the start of the clip. See
                    the header file FxPlug/FxHostCapabilities.h.
 */
- (BOOL)getFloatValue:(double *)value
        fromParameter:(UInt32)parameterID
               atTime:(CMTime)time;

/*!
     @method        getIntValue:fromParameter:atTime:
     @abstract      Gets an integer value from a parameter.
     @param         value        Pointer to the plug-in variable for the value.
     @param         parameterID  Unique identifier for the parameter.
     @param         time        Specified time to get the value.
     @result        Returns YES if the value is retrieved successfully; NO otherwise.
     @discussion    Depending on the host application, the time value may be relative to the start of the
                    timeline or to the start of the clip. See the header file FxPlug/FxHostCapabilities.h.
 */
- (BOOL)getIntValue:(int *)value
      fromParameter:(UInt32)parameterID
             atTime:(CMTime)time;

/*!
     @method        getBoolValue:fromParameter:atTime:
     @abstract      Gets a boolean value from a parameter.
     @param         value        Pointer to the plug-in variable for the value.
     @param         parameterID  Unique identifier for the parameter.
     @param         time         Specified time to get the value.
     @result        Returns YES if the value is retrieved successfully; NO otherwise.
     @discussion    Depending on the host application, the time value may be relative to the start of the
                    timeline or to the start of the clip. See the header file FxPlug/FxHostCapabilities.h.
 */
- (BOOL)getBoolValue:(BOOL *)value
       fromParameter:(UInt32)parameterID
              atTime:(CMTime)time;

/*!
     @method        getRedValue:GreenValue:BlueValue:fromParameter:atTime:
     @abstract      Gets RGB color values (in 0.0 - 1.0 floating point) from a parameter.
     @param         red         Pointer to the plug-in variable for the red value.
     @param         green       Pointer to the plug-in variable for the green value.
     @param         blue        Pointer to the plug-in variable for the blue value.
     @param         parameterID Unique identifier for the parameter.
     @param         time        Specified time to get the values.
     @result        Returns YES if the values are retrieved successfully; NO otherwise.
     @discussion    Depending on the host application, the time value may be relative to the start of the
                    timeline or to the start of the clip. See the header file FxPlug/FxHostCapabilities.h.
 */
- (BOOL)getRedValue:(double *)red
         greenValue:(double *)green
          blueValue:(double *)blue
      fromParameter:(UInt32)parameterID
             atTime:(CMTime)time;

/*!
     @method        getRedValue:GreenValue:BlueValue:AlphaValue:fromParameter:atTime:
     @abstract      Gets RGBA color value (in 0.0 - 1.0 floating point) from a parameter.
     @param         red          Pointer to the plug-in variable for the red value.
     @param         green        Pointer to the plug-in variable for the green value.
     @param         blue         Pointer to the plug-in variable for the blue value.
     @param         alpha        Pointer to the plug-in variable for the alpha value.
     @param         parameterID  Unique identifier for the parameter.
     @param         time         Specified time to get the values.
     @result        Returns YES if the values are retrieved successfully; NO otherwise.
     @discussion    Depending on the host application, the time value may be relative to the start of the
                    timeline or to the start of the clip. See the header file FxPlug/FxHostCapabilities.h.
 */
- (BOOL)getRedValue:(double *)red
         greenValue:(double *)green
          blueValue:(double *)blue
         alphaValue:(double *)alpha
      fromParameter:(UInt32)parameterID
             atTime:(CMTime)time;

/*!
     @method        getXValue:YValue:fromParameter:atTime:
     @abstract      Gets the xy position value (in 0.0 - 1.0 floating point) from a parameter.
     @param         x            Pointer to the plug-in variable for the x value.
     @param         y            Pointer to the plug-in variable for the y value.
     @param         parameterID  Unique identifier for the parameter.
     @param         time         Specified time to get the values.
     @result        Returns YES if the values are retrieved successfully; NO otherwise.
     @discussion    Depending on the host application, the time value may be relative to the start of the
                    timeline or to the start of the clip. See the header file FxPlug/FxHostCapabilities.h.
 */
- (BOOL)getXValue:(double *)x
           YValue:(double *)y
    fromParameter:(UInt32)parameterID
           atTime:(CMTime)time;

/*!
     @method        getParameterFlags:fromParameter:
     @abstract      Gets current state of a parameter.
     @param         flags           Flags that encode various state values for the parameter.
     @param         parameterID     Unique identifier for the parameter.
     @result        Returns YES if the state information is retrieved successfully; NO otherwise.
 */
- (BOOL)getParameterFlags:(FxParameterFlags *)flags
            fromParameter:(UInt32)parameterID;

/*!
     @method        getStringParameterValue:fromParameter:
     @abstract      Gets a string value from a string parameter.
     @param         string        Pointer to the NSString object pointer to receive the string value.
     @param         parameterID   Unique identifier for the parameter.
     @result        Returns YES if the value is retrieved successfully; NO otherwise.
 */
- (BOOL)getStringParameterValue:(NSString * _Nonnull * _Nullable)string
                  fromParameter:(UInt32)parameterID;


/*!
     @method     getCustomParameterValue:fromParameter:atTime:
     @abstract   Retrieves the (possibly keyframed) value of a custom parameter at a given time.
     @param      value          Receives the value of the parameter at the given time
     @param      parameterID    The ID of the parameter whose value you want to retrieve
     @param      time           Specified time to get the value.
     @discussion Depending on the host application, the time value may be relative to the start of the
                 timeline or to the start of the clip. See the header file FxPlug/FxHostCapabilities.h.
 */
- (BOOL)getCustomParameterValue:(NSObject<NSSecureCoding, NSCopying>* _Nullable * _Nonnull)value
                  fromParameter:(UInt32)parameterID
                         atTime:(CMTime)time;

/*!
     @method     getPathID:fromParameter:atTime:
     @abstract   Retrieves the (possibly keyframed) value of a pathID parameter at a given time.
     @param      pathID      Receives the path ID of the parameter at the given time
     @param      parameterID The ID of the parameter whose value you want to retrieve
     @param      time        Specified time to get the value.
 */
- (BOOL)getPathID:(FxPathID _Nullable * _Nonnull)pathID
    fromParameter:(UInt32)parameterID
           atTime:(CMTime)time;

/*!
     @method     getFontName:fromParameter:atTime:
     @abstract   Retrieves the family and typeface of a given font
     @param      fontName    Pointer to the font's name (ex. Helvetica, etc)
     @param      parameterID The ID of the parameter whose value you want to retrieve
     @param      time        Specified time to get the value.
 */
- (BOOL)  getFontName:(NSString* _Nullable * _Nonnull)fontName
        fromParameter:(UInt32)parameterID
               atTime:(CMTime)time;


/*!
     @method        getHistogramBlackIn:BlackOut:WhiteIn:WhiteOut:Gamma:forChannel:fromParameter:atTime:
     @abstract      Gets the Histogram parameter settings.
     @param         blackIn     Pointer for the parameter black in level value.
     @param         blackOut    Pointer for the parameter black out level value.
     @param         whiteIn     Pointer for the parameter white in level value.
     @param         whiteOut    Pointer for the parameter white out level value.
     @param         gamma       Pointer for the parameter gamma level value.
     @param         channel     Specified channel for the histogram information
                                (RGB: 0, Red: 1, Green: 2, Blue: 3, Alpha: 4).
     @param         parameterID Parameter identifier assigned upon creation.
     @param         time        Specified time to get the values.
     @result        Returns YES if the value is successfully retrieved; NO otherwise.
     @discussion    Depending on the host application, the time value may be relative to the start of the
                    timeline or to the start of the clip. See the header file FxPlug/FxHostCapabilities.h.
 */
- (BOOL)getHistogramBlackIn:(double *)blackIn
                   BlackOut:(double *)blackOut
                    WhiteIn:(double *)whiteIn
                   WhiteOut:(double *)whiteOut
                      Gamma:(double *)gamma
                 forChannel:(FxHistogramChannel)channel
              fromParameter:(UInt32)parameterID
                     atTime:(CMTime)time;

/*!
     @method        getGradientSamples:numSamples:depth:fromParameter:atTime:
     @abstract      Gets the Gradient parameter settings.
     @param         samples     Pointer to a buffer of size (4 * numSamples * sampleDepth). It is
                                populated with samples of RGBA values across the gradient. The number of
                                samples is set by numSamples.
     @param         numSamples  Number of samples from the gradient. More samples provide a more
                                accurate representation, but require more time.
     @param         sampleDepth Number of bytes per channel (@c kFxDepth_FLOAT32, for example)
     @param         parameterID Parameter identifier assigned upon creation.
     @param         time        Specified time to get the values.
     @result        Returns YES if the values are successfully retrieved; NO otherwise.
     @discussion    Depending on the host application, the time value may be relative to the start of the
                    timeline or to the start of the clip. See the header file FxPlug/FxHostCapabilities.h.
 */
- (BOOL)getGradientSamples:(void *)samples
                numSamples:(NSUInteger)numSamples
                     depth:(FxDepth)sampleDepth
             fromParameter:(UInt32)parameterID
                    atTime:(CMTime)time;

@end

/*!
    @protocol   FxParameterRetrievalAPI_v7
    @abstract   Adds a method to retrieve the image information about an image parameter.
 */
@protocol FxParameterRetrievalAPI_v7 <FxParameterRetrievalAPI_v6>

/*!
    @method     imageSize:fromParameter:atTime:error:
    @abstract   Provides the dimensions of the image well image when it is at 100-percent scale
                and has square pixels.
    @param      imageSize       The size of the image in image space.
    @param      parameterID     The ID of the image parameter for which you want to retrieve size.
    @param      time            The time at which you want to get the image size.
    @param      error           Contains any errors that occurred while getting the image size.
    @result     Returns YES if it successfully retrieves the image dimensions. Returns NO otherwise
                and sets the error to the reason for the failure.
    @discussion Removes the effects of pixel aspect ratio, fields, proxy resolution scaling, or
                user scaling, and provides the dimensions of the image well image in image space.

 */
- (BOOL)imageSize:(CGSize*)imageSize
    fromParameter:(UInt32)parameterID
           atTime:(CMTime)time
            error:(NSError**)error;

@end


/*!
 @protocol   FxParameterSettingAPI_v5
 @abstract   Defines the functions the application provides to set parameter values.
 @discussion    This is version 5 of the FxParameterSettingAPI protocol.
 
 NOTE: This API is only available to plug-ins built in the FxPlug 4 style and later.
 
 */

@protocol FxParameterSettingAPI_v5 <NSObject> // NOTE: This does not inherit from v4, but is an entirely new API

/*!
     @method        setFloatValue:toParameter:atTime:
     @abstract      Sets the value of a floating point parameter.
     @param         value        Specified value for the parameter.
     @param         parameterID  Unique identifier for the parameter.
     @param         time         Specified time to set the value.
     @result        Returns YES if the value is set successfully; NO otherwise.
     @discussion    When used to set the value of an angle parameter, the result will be expressed
                    in counter-clockwise degrees. Depending on the host application, the time value
                    may be relative to the start of the timeline or to the start of the clip. See
                    the header file FxPlug/FxHostCapabilities.h.
                    <br/>
                    In FxPlug 1.2, the documentation for this method incorrectly stated that
                    angle values were returned in radians.
 */

- (BOOL)setFloatValue:(double)value
          toParameter:(UInt32)parameterID
               atTime:(CMTime)time;

/*!
     @method        setIntValue:toParameter:atTime:
     @abstract      Sets the value of an integer parameter.
     @param         value        Specified value for the parameter.
     @param         parameterID  Unique identifier for the parameter.
     @param         time         Specified time to set the value.
     @result        Returns YES if the value is set successfully; NO otherwise.
     @discussion    Depending on the host application, the time value may be relative to the start of the
                    timeline or to the start of the clip. See the header file FxPlug/FxHostCapabilities.h.
 */
- (BOOL)setIntValue:(int)value
        toParameter:(UInt32)parameterID
             atTime:(CMTime)time;

/*!
     @method        setBoolValue:toParameter:atTime:
     @abstract      Sets the value of a boolean parameter.
     @param         value        Specified value for the parameter.
     @param         parameterID  Unique identifier for the parameter.
     @param         time         Specified time to set the value.
     @result        Returns YES if the value is set successfully; NO otherwise.
     @discussion    Depending on the host application, the time value may be relative to the start of the
                    timeline or to the start of the clip. See the header file FxPlug/FxHostCapabilities.h.
 */
- (BOOL)setBoolValue:(BOOL)value
         toParameter:(UInt32)parameterID
              atTime:(CMTime)time;

/*!
     @method        setRedValue:GreenValue:BlueValue:toParameter:atTime:
     @abstract      Set the values of an RGB color parameter.
     @param         red          Specified red value for the parameter.
     @param         green        Specified green value for the parameter.
     @param         blue         Specified blue value for the parameter.
     @param         parameterID  Unique identifier for the parameter.
     @param         time         Specified time to set the values.
     @result        Returns YES if the values are set successfully; NO otherwise.
     @discussion    Depending on the host application, the time value may be relative to the start of the
                    timeline or to the start of the clip. See the header file FxPlug/FxHostCapabilities.h.
 */
- (BOOL)setRedValue:(double)red
         greenValue:(double)green
          blueValue:(double)blue
        toParameter:(UInt32)parameterID
             atTime:(CMTime)time;


/*!
     @method        setRedValue:GreenValue:BlueValue:AlphaValue:toParameter:atTime:
     @abstract      Sets the value of an RGBA color parameter
     @param         red          Specified red value for the parameter.
     @param         green        Specified green value for the parameter.
     @param         blue         Specified blue value for the parameter.
     @param         alpha        Specified alpha value for the parameter.
     @param         parameterID  Unique identifier for the parameter.
     @param         time         Specified time to set the values.
     @result        Returns YES if the values are set successfully; NO otherwise.
     @discussion    Depending on the host application, the time value may be relative to the start of the
                    timeline or to the start of the clip. See the header file FxPlug/FxHostCapabilities.h.
 */
- (BOOL)setRedValue:(double)red
         greenValue:(double)green
          blueValue:(double)blue
         alphaValue:(double)alpha
        toParameter:(UInt32)parameterID
             atTime:(CMTime)time;

/*!
     @method        setXValue:YValue:toParameter:atTime:
     @abstract      Sets the xy values for a position parameter.
     @param         x            Specified x value (0.0 - 1.0) in floating point space.
     @param         y            Specified y value (0.0 - 1.0) in floating point space.
     @param         parameterID  Unique identifier for the parameter.
     @param         time         Specified time to set the values.
     @result        Returns YES if the values are set successfully; NO otherwise.
     @discussion    Depending on the host application, the time value may be relative to the start of the
                    timeline or to the start of the clip. See the header file FxPlug/FxHostCapabilities.h.
 */
- (BOOL)setXValue:(double)x
           YValue:(double)y
      toParameter:(UInt32)parameterID
           atTime:(CMTime)time;

/*!
     @method        setParameterFlags:toParameter:
     @abstract      Sets the flags for the state of a parameter.
     @param         flags        Specified flag values for the parameter.
     @param         parameterID  Unique identifier for the parameter.
     @result        Returns YES if the flags are set successfully; NO otherwise.
     @discussion    Setting or resetting the @c kFxParameterFlag_HIDDEN flag on a ParameterSubGroup
                    parameter will also hide or reveal all of the controls within the group.
                    Likewise, changing the @c kFxParameterFlag_DISABLED flag value affects all of the
                    controls within the group as well.
 */
- (BOOL)setParameterFlags:(FxParameterFlags)flags
              toParameter:(UInt32)parameterID;

/*!
     @method        setStringParameterValue:toParameter:
     @abstract      Sets a string value for a string parameter.
     @param         string        Specified string value for the parameter
     @param         parameterID   Unique identifier for the parameter.
     @result        Returns YES if the value is set successfully; NO otherwise.
 */
- (BOOL)setStringParameterValue:(NSString *)string
                    toParameter:(UInt32)parameterID;
/*!
     @method        setCustomParameterValue:toParameter:atTime:
     @abstract      Sets a custom value for a custom parameter.
     @param         value        Specified custom value for the parameter
     @param         parameterID  Unique identifier for the parameter.
     @param         time         Specified time to set the value.
     @result        Returns YES if the value is set successfully; NO otherwise.
 */
- (BOOL)setCustomParameterValue:(NSObject<NSSecureCoding, NSCopying>*)value
                    toParameter:(UInt32)parameterID
                         atTime:(CMTime)time;

/*!
     @method        setPathID:toParameter:atTime:
     @abstract      Sets a custom value for a custom parameter.
     @param         pathID        Specified path ID for the parameter
     @param         parameterID   Unique identifier for the parameter.
     @param         time          Specified time to set the value.
     @result        Returns YES if the pathID is set successfully; NO otherwise.
 */
- (BOOL)setPathID:(FxPathID)pathID
      toParameter:(UInt32)parameterID
           atTime:(CMTime)time;

/*!
     @method        setHistogramBlackIn:BlackOut:WhiteIn:WhiteOut:Gamma:forChannel:fromParameter:atTime:
     @abstract      Sets the values in a histogram parameter group.
     @param         blackIn     The new black in level value.
     @param         blackOut    The new black out level value.
     @param         whiteIn     The new white in level value.
     @param         whiteOut    The new white out level value.
     @param         gamma       The new gamma value.
     @param         channel     Specified channel for new histogram values
                                (RGB: 0, Red : 1, Green: 2, Blue: 3, Alpha: 4).
     @param         parameterID Parameter identifier assigned upon creation.
     @param         time        Specified time to set the values.
     @result        Returns YES if the values are successfully set; NO if the parameter cannot be found.
     @discussion    Depending on the host application, the time value may be relative to the start of the
     timeline or to the start of the clip. See the header file FxPlug/FxHostCapabilities.h.
 */
- (BOOL)setHistogramBlackIn:(double)blackIn
                   blackOut:(double)blackOut
                    whiteIn:(double)whiteIn
                   whiteOut:(double)whiteOut
                      gamma:(double)gamma
                 forChannel:(FxHistogramChannel)channel
              fromParameter:(UInt32)parameterID
                     atTime:(CMTime)time;

@end

@protocol FxParameterSettingAPI_v6 <FxParameterSettingAPI_v5>

/*!
    @method     addFlags:toParameter:
    @abstract   Add the passed-in flags to the current parameter flags
    @param      flags   The new flags to add. Other flags will not be touched
    @param      parameterID     The parameter whose flags you want to add to.
    @result     Returns YES if it successfully added the flags to the parameter. Returns NO otherwise.
    @discussion Use this method to set one or more parameter flags without changing the value of other flags.
 */
- (BOOL)addFlags:(FxParameterFlags)flags
     toParameter:(UInt32)parameterID;

/*!
    @method     removeFlags:fromParameter:
    @abstract   Remove the passed-in flags from the current parameter flags
    @param      flags   The new flags to remove. Other flags will not be touched
    @param      parameterID The ID of the parameter whose flags you wish to clear
    @result     Returns YES if the flags were successfully removed. Returns NO otherwise.
    @discussion Use this method to remove one or more paraemeter flags without changing the value of other flags.
 */
- (BOOL)removeFlags:(FxParameterFlags)flags
     fromParameter:(UInt32)parameterID;

@end


/*!
    @protocol   FxCustomParameterInterpolation_v2
    @abstract   Defines the methods a plug-in needs to implement to support keyframing custom
                parameters.
    @discussion	An FxPlug 4 or later that wants to support keyframing custom parameters must
                conform to the @c FxCustomParameterInterpolation_v2 or later protocol. This protocol
                allows the plug-in to generate interpolated (in-between) values between keyframes.
                Note that this protocol intentionally does not inherit from the
                @c FxCustomParameterInterpolation protocol which is for older-style plug-ins.
*/

@protocol FxCustomParameterInterpolation_v2

/*!
    @method		interpolateBetween:withWeight:
    @abstract   Returns a new parameter value which has been calculated by interpolating between
                two key values.
    @param		rightValue  The value of the custom parameter at the keypoint to the right of the
                            current time in the timeline.
    @param		weight	    A floating-point value between 0 and 1 representing the weight of the
                            keyframe to the right of the current time vs the keyframe to the left of
                            the current time. A weight of zero means the same value as the keyframe
                            to the left; a weight of one means the same value as the keyframe to
                            the right.
    @result		A new custom parameter value.
    @discussion	This plug-in method is called by the host application when the user has added
                keyframes to a custom parameter, and the application needs to know the value of
                the parameter between keyframes. This may be when the application is rendering a
                frame between keyframes, or when the plugin requests the value of its parameter at
                a time between keyframes, or when the application displays UI.
*/
- (NSObject<NSSecureCoding, NSCopying>*)interpolateBetween:(NSObject<NSSecureCoding, NSCopying>*)rightValue
                                                withWeight:(float)weight;

/*!
    @method     isEqual:
    @abstract   Returns whether one custom parameter value is equal to another.
    @param      rightValue  -> The value to the right of the equal sign in the equation
                                (leftValue == rightValue).
    @result     True if the two values are equal, false otherwise.
    @discussion The host application will sometimes need to know whether two custom parameters
                are equivalent. When it needs to know that, it will call this method on one custom
                parameter object, and pass it the other as "rightValue".
*/
- (BOOL)isEqual:(NSObject<NSSecureCoding, NSCopying>*)rightValue;

@end

NS_ASSUME_NONNULL_END
