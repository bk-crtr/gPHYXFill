//
//  PROPlugProtocols.h
//  PluginManager
//
//  Copyright © 2019-2023 Apple Inc. All rights reserved.

#ifndef PROPlugProtocols_h
#define PROPlugProtocols_h

/*!
    @protocol           FxXPCProtocol
    @abstract           The protocol used by all FxPlug 4 plug-ins to initiate contact with
                        the host application.
    @discussion         The plug-in should never use this protocol. It’s called by the host
                        application only.
 */
@protocol FxXPCProtocol

/*!
    @method         -createNewConnectionForProcessID:minimumVersion:maximumVersion:hostCapabilities:reply:
    @abstract       Accepts a connection from the host application.
    @discussion     The host application automatically calls this so plug-ins should not call it.
 */
- (void)createNewConnectionForProcessID:(pid_t)processID
                         minimumVersion:(NSUInteger)minVers
                         maximumVersion:(NSUInteger)maxVers
                       hostCapabilities:(NSDictionary*)remoteCapabilities
                                  reply:(void(^)(NSXPCListenerEndpoint*, NSError*))reply;

/*!
    @method         -viewServiceListenerEndPoint:
    @abstract       Obtains the NSXPCListenerEndpoint for ViewBridge to communicate with custom views.
    @discussion     The host application automatically calls this so plug-ins should not call it.
 */
- (void)viewServiceListenerEndPoint:(void (^)(NSXPCListenerEndpoint*))reply;

@end

/*!
    @protocol       PROXPCProtocol
    @abstract       The protocol used by PluginManager plug-ins that also handle dynamic registration.
    @discussion     The plug-in should never directly use this protocol. It's called by the host
                    application only. Plug-ins should, however, put this protocol name as the
                    value for the Protocl key in the PluginKit section of their Info.plist file.
 */
@protocol PROXPCProtocol <FxXPCProtocol>

/*!
    @method         -newConnectionForProcessID:minimumVersion:maximumVersion:hostCapabilities:reply:
    @abstract       Accepts a connection from the host application.
    @discussion     The host application automatically calls this so plug-ins should not call it.
 */
- (void)newConnectionForProcessID:(pid_t)processID
                   minimumVersion:(NSUInteger)minVers
                   maximumVersion:(NSUInteger)maxVers
                 hostCapabilities:(NSDictionary*)remoteCapabilities
                            reply:(void(^)(NSXPCListenerEndpoint*, NSUInteger xpcVersion, NSError*))reply;

/*!
    @method     -dynamicRegistrationEndpointForVersion:reply:
    @abstract   Obtains the NSXPCListenerEndpoint for dynamic registration.
    @discussion The host application automatically calls this so plug-ins should not call it.
 */

- (void)dynamicRegistrationEndpointForProcessID:(pid_t)processID
                                        version:(NSInteger)version
                                          reply:(void (^)(NSXPCListenerEndpoint*, NSError*))reply;

@end

#endif /* PROPlugProtocols_h */
