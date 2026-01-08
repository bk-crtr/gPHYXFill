/*!
 @header        FxRemoteWindowAPI.h
 @abstract      Defines the FxRemoteWindowAPI protocols for requesting the host to create a window.
 */
/* Copyright © 2020-2025 Apple Inc. All rights reserved. */

#ifndef FxRemoteWindowAPI_h
#define FxRemoteWindowAPI_h

/*!
    @protocol   FxRemoteWindowAPI
    @abstract   Allows the plug-in to request the host create a window
 */

@protocol FxRemoteWindowAPI

/*!
    @method     remoteWindowOfSize:reply:
    @abstract   Requests that the host application creates a window to display some custom UI.
    @param      contentSize -> The desired size for the content rect of the window
    @param      reply   <-  Called when the window has been created
    @discussion Call this method to request that the host creates a new window for you to display custom UI.
                The reply block will get the window's remote content view for your plug-in to put its views into.
                If there are any problems creating the window, parentView will be nil and error will contain
                an NSError describing the problem.
                Note that each plug-in instance can only have a single window. Subsequent calls to this method
                without the user closing the window will result in the same parent view being returned.
 */
- (void)remoteWindowOfSize:(CGSize)contentSize
                     reply:(void (^)(NSView* parentView, NSError* error))reply;

@end

@protocol FxRemoteWindowAPI_v2 <FxRemoteWindowAPI>

/*!
    @method     remoteWindowWithMinimumSize:maximumSize:reply:
    @abstract   Requests that the host application creates a window with a minimum and maximum size to display some custom
                UI.
    @param      minContentSize  -> The minimum content size requested
    @param      maxContentSize  -> The maximum content size for the window
    @param      reply <- Called when the window has been created, or if an error occurs during window creation
    @discussion Call this method to request that the host creates a new window for your plug-in to display custom user interface
                elements. The maximum size will be capped at 80% of the size of the host application's window size. The reply
                block will supply the view that your custom view should be put into. If the parent view is nil, the error parameter
                will contain the reason why.
                Note that each plug-in instance can only have a single window. Subsequent calls to this method without the
                user closing the window will result in the same parent view being returned.
 */
- (void)remoteWindowWithMinimumSize:(CGSize)minContentSize
                        maximumSize:(CGSize)maxContentSize
                              reply:(void(^)(NSView* parentView, NSError* error))reply;

@end

@protocol FxRemoteWindowAPI_v3 <FxRemoteWindowAPI_v2>

/*!
    @method     closeRemoteWindow
    @abstract   Requests that the host close the remote window it created for the plug-in during
                a call to -remoteWindowOfSize:: or -remoteWindowWithMinimumSize:::.
    @discussion Call this method to request that the host close the window associated with your plug-in. Each plug-in instance is
                limited to a single window.
 
 */
- (void)closeRemoteWindow;

@end

#endif /* FxRemoteWindowAPI_h */
