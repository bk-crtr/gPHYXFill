/*
 *  PluginManager.h
 *  ProPlug
 *
 *  Created by Dan Schimpf on 8/20/04.
 *  Copyright 2004 Apple Computer, Inc. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>

FOUNDATION_EXPORT double PluginManagerVersionNumber;
FOUNDATION_EXPORT const unsigned char PluginManagerVersionString[];

#import <PluginManager/PROAPIAccessing.h>
#import <PluginManager/PROPlugInBundleRegistration.h>

#if !STUDIO_LITE

#import <PluginManager/PROPlugProtocols.h>

#endif
