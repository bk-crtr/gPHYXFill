//
//  FxPrincipalAPI.h
//  FxPlug
//
/* Copyright © 2013-2025 Apple Inc. All rights reserved. */


#ifndef FxPlug_FxPrincipalAPI_h
#define FxPlug_FxPrincipalAPI_h

#import <PluginManager/PROPlugProtocols.h>

/*!
    @protocol           FxPrincipalDelegate
    @abstract           Implement this protocol if you'd like to receive information about the host application which launched
                        your XPC service.
    @discussion         When the host application first launches your XPC Service, your XPC will call
                        +startServicePrincipalWithDelegate: to get communication with the host application started.
                        Passing in a delegate that implements this protocol allows you to know which host application and
                        version launched your XPC service.
 */
@protocol FxPrincipalDelegate
@optional

/*!
    @method             didEstablishConnectionWithHost:version:
    @abstract           Called when the service principal establishes a connection with the host.
    @param              hostBundleIdentifier    - The NSBundleIdentifier of the host application that launched this
                                               XPC service.
    @param              hostVersionString - The NSShortVersionString of the host application that launched this XPC
                                           service.
    @discussion         Your delegate can implement this protocol to be informed of which host application is
                        calling your plug-in.
 */
- (void)didEstablishConnectionWithHost:(NSString*)hostBundleIdentifier
                               version:(NSString*)hostVersionString;

@end

/*!
    @protocol           FxPrincipalAPI
    @abstract           A protocol that provides an interface that the host application implements
                        to retrieve an XPC proxy object for communicating with its XPC.
    @discussion         The plug-in should never use this protocol. It’s called by the host
                        application only.
 */
@protocol FxPrincipalAPI

/*!
    @method             servicePrincipal
    @abstract           Returns a pointer to the proxy object used for sandboxed out-of-process tasks.
    @discussion         It is important to always check that the returned pointer is non-nil.
                        If this method returns nil, the host app is unable to communicate with your
                        plug-in’s XCP service.
    @result             An object used for XPC communication with the out-of-process
                        service principal.
 */

- (id)servicePrincipal;

@end

/*!
    @interface      FxPrincipal
    @abstract       Singleton object that starts your XPC service running and can return the
                    embedded (in-process) component of your plug-in.
*/
@interface FxPrincipal : NSObject <PROXPCProtocol>

/*!
    @method         embeddedPrincipal
    @abstract       Returns a pointer to the proxy object used for the sandboxed in-process tasks.
    @discussion     If this method returns nil, the host app is unable to communicate with your
                    plug-in’s XCP service.
    @result         An object used for XPC communication with the in-process embedded principal.
*/
+(id)embeddedPrincipal;

/*!
    @method         startServicePrincipal
    @abstract       Starts your XPC service listening for connections from the host application.
*/
+(void)startServicePrincipal;

/*!
    @method         startServicePrincipalWithDelegate:
    @param          delegate    - An object that implements the FxPrincipalDelegate or nil
    @discussion     Starts your XPC service listening for connections from the host application. Once a connection is
                    established, your delegate will be informed of the host application's ID and version.
 */
+(void)startServicePrincipalWithDelegate:(id<FxPrincipalDelegate>)delegate;

@end


#endif
