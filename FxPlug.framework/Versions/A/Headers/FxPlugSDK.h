/*!
	@header		FxPlugSDK.h
	@abstract	Umbrella header for the FxPlug SDK.
	@discussion	FxPlugSDK.h imports all of the headers for the FxPlug SDK, as well as for the
				PluginManager framework. Here is a list of the header files included by
				FxPlugSDK.h:
	<br/>
	<br/>		ProPlug Plug-In Manager
	<br/>		-  PROAPIAccessing.h
	<br/>		-  PROPlugInBundleRegistration.h
	<br/>
	<br/>		Common Types
	<br/>		-  FxTypes.h
	<br/>
	<br/>		Plug-ins
    <br/>       -  FxTileableEffect.h
	<br/>
	<br/>		Optional Custom UI support
	<br/>		-  FxOnScreenControl.h
	<br/>		-  FxCustomParameterUI.h
	<br/>
	<br/>		Host APIs
	<br/>		-  FxOnScreenControlAPI.h
	<br/>		-  FxParameterAPI.h
	<br/>         -  FxDynamicParameterAPI.h
	<br/>		-  FxLayerInfoAPI.h
	<br/>		-  FxTimingAPI.h
	<br/>		-  Fx3DAPI.h
	<br/>		-  FxVersioningAPI.h
    <br/>       -  FxKeyframeAPI.h
    <br/>       -  FxPathAPI.h
	<br/>
*/

/* Copyright © 2005-2025 Apple Inc. All rights reserved. */

#ifndef __FXPLUG_SDK_H__
#define __FXPLUG_SDK_H__

/* ProPlug */
#import <PluginManager/PROAPIAccessing.h>
#import <PluginManager/PROPlugInBundleRegistration.h>

/* Types */
#import <FxPlug/FxTypes.h>
#import <FxPlug/FxMatrix.h>

/* Plug-ins */
#if !STUDIO_LITE
#import <FxPlug/FxTileableEffect.h>
#endif

/* Optional Custom UI support */
#import <FxPlug/FxOnScreenControl.h>
#if !STUDIO_LITE   // FIXME iOS
#import <FxPlug/FxCustomParameterUI.h>
#endif

/* Host APIs */
#if !STUDIO_LITE   // FIXME iOS
#import <FxPlug/FxOnScreenControlAPI.h>
#endif
#import <FxPlug/FxParameterAPI.h>
#import <FxPlug/FxDynamicParameterAPI.h>
#import <FxPlug/FxTimingAPI.h>
#import <FxPlug/Fx3DAPI.h>
#import <FxPlug/FxVersioningAPI.h>
#import <FxPlug/FxKeyframeAPI.h>
#if !STUDIO_LITE   // FIXME iOS
#import <FxPlug/FxLightingAPI.h>
#endif
#import <FxPlug/FxUndoAPI.h>
#if !STUDIO_LITE   // FIXME iOS
#import <FxPlug/FxRemoteWindowAPI.h>
#endif
#import <FxPlug/FxPathAPI.h>
#if !STUDIO_LITE   // FIXME iOS
#import <FxPlug/FxPrincipalAPI.h>
#endif
#import <FxPlug/FxColorGamutAPI.h>

/* Images (Bitmaps and Textures) */
#if !STUDIO_LITE
#import <FxPlug/FxImageTile.h>
#import <FxPlug/FxAnalysis.h>
#import <FxPlug/FxCommandAPI.h>
#import <FxPlug/FxProjectAPI.h>
#endif // !STUDIO_LITE

#endif
