/*!
    @header     FxDynamicParameterAPI.h

    @abstract   Defines an API for dynamically creating and removing parameters in an FxPlug. 

*/

/* Copyright © 2005-2025 Apple Inc. All rights reserved. */

/* Earlier versions of these APIs are not compatible with FxPlug 4.x and have been removed from
   the FxPlug SDK
 */

#import <Foundation/Foundation.h>
#import <FxPlug/FxTypes.h>
#import <FxPlug/FxKeyframeAPI.h>



/*!
    @protocol   FxDynamicParameterAPI_v3
    @abstract   Allows your plugin to create parameters on-the-fly
    @discussion With this API your plugin can create and remove parameters outside of its
                -addParameters method. It can also get and set various properties of parameters
                during run-time, as well, such as the minimum and maximum allowable values.
                NOTE: You should only implement this protocol in plug-ins that use FxPlug 4
                or later. It will not be called in plug-ins that are written with FxPlug 2 or 3.
*/
@protocol FxDynamicParameterAPI_v3 // does not inherit from FxDynamicParameterAPI_v2 intentionally

/*!
    @method     parameterCount
    @abstract   Returns the number of parameters your plugin currently has
*/
- (UInt32)parameterCount;

/*!
    @method     parameterIDAtIndex:
    @abstract   Returns the ID of the parameter at the given index
    @param      index   The 0-based index of the parameter whose ID you wish to get
    @discussion During -addParameters: your plugin tells the host to create parameters. Later,
                while running, your plugin can create new parameters using the
                FxParameterCreationAPI (just like in -addParameters), or it can remove them
                using the -removeParameter: method of this protocol. Each parameter must have
                a unique ID within the plugin. This method allows you to retrieve the ID of a
                parameter at a given index in the list of parameters. The IDs need not be
                sequential or even increasing. You could for example have the following:
<pre>@textblock
                index   ID      parameter
                -----   --      ---------
                0       1       slider
                1       1000    checkbox
                2       10      popup menu
@/textblock</pre>
*/
- (UInt32)parameterIDAtIndex:(UInt32)index;

/*!
    @method     removeParameter:
    @abstract   Removes the parameter with the passed-in ID
    @param      parameterID the ID (not index) of the parameter you wish to remove
    @discussion Removes the parameter with the passed-in ID from your plugin's list of parameters.
                Returns any errors which it encountered.
*/
- (NSError*)removeParameter:(UInt32)parameterID;

/*!
    @method     parameter:name:
    @abstract   Get a parameter's name
*/
- (NSError*)parameter:(UInt32)parameterID
                 name:(NSString**)parameterName;

/*!
    @method     setParameter:name:
    @abstract   Set a parameter's name
*/
- (NSError*)setParameter:(UInt32)parameterID
                    name:(NSString*)newName;

/*!
    @method     parameter:floatMinimum:maximum:sliderMinimum:sliderMaximum:
    @abstract   Get a parameter's bounds as floating point values
*/
- (NSError*)parameter:(UInt32)parameterID
         floatMinimum:(double*)min
              maximum:(double*)max
        sliderMinimum:(double*)sliderMin
        sliderMaximum:(double*)sliderMax;

/*!
    @method     setParameter:floatMinimum:maximum:sliderMinimum:sliderMaximum:
    @abstract   Set a parameter's bounds using floating point values
*/
- (NSError*)setParameter:(UInt32)parameterID
            floatMinimum:(double)min
                 maximum:(double)max
           sliderMinimum:(double)sliderMin
           sliderMaximum:(double)sliderMax;

/*!
    @method     parameter:intMinimum:maximum:sliderMinimum:sliderMaximum:
    @abstract   Get a parameter's bounds as integer values
*/
- (NSError*)parameter:(UInt32)parameterID
           intMinimum:(int *)min
              maximum:(int *)max
        sliderMinimum:(int *)sliderMin
        sliderMaximum:(int *)sliderMax;

/*!
    @method     setParameter:intMinimum:maximum:sliderMinimum:sliderMaximum:
    @abstract   Set a parameter's bounds using integer values
*/
- (NSError*)setParameter:(UInt32)parameterID
              intMinimum:(int)min
                 maximum:(int)max
           sliderMinimum:(int)sliderMin
           sliderMaximum:(int)sliderMax;

/*!
    @method     setPopupMenuParameter:entries:defaultValue:
    @abstract   Set the menu entries in a popup menu
*/
- (NSError*)setPopupMenuParameter:(UInt32)parameterID
                          entries:(NSArray*)newEntries
                     defaultValue:(UInt32)defaultIndex;

/*!
     @method     setAsDefaultsAtTime:withError:
     @param      time    - The time that contains the defaults you wish to set
     @param      error   - A pointer to an NSError*. It will be filled out if the function
                           returns NO
     @abstract   Tell the host app that the settings at the given time should be considered
                 the default settings for this parameter. If anything goes wrong, the return
                 value will be "NO" and if error is non-NULL, it will point to an error explaining
                 the problem.
 */
- (BOOL)setAsDefaultsAtTime:(CMTime)time
                  withError:(NSError**)error;

@end

