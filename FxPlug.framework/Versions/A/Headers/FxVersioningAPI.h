/*!
	@header		FxVersioningAPI.h
	@abstract	Defines the FxVersioningAPI protocol.
	@discussion	Defines the FxVersioningAPI host-API protocol, which allows a plug-in to manage any
				backward compatibility issues.
*/

/* Copyright © 2005-2025 Apple Inc. All rights reserved. */



/*!
	@protocol   FxVersioningAPI
    @abstract   A protocol that defines the methods a host application implements to identify the
                plug-in version used by a project when it was first created.
	@discussion This protocol allows a plug-in instance to determine the version of the original
                instance of the plug-in. For example, if a user creates a project using version 1
                of a plug-in, and then updates to version 2, the plug-in determines the old plug-in
                version and chooses which version to render.
                This protocol was first released in FxPlug framework version 1.2.1.
*/

@protocol FxVersioningAPI


/*!
	@method		versionAtCreation
	@abstract   Provides the version number of a plug-in that a project uses.
	@result		The version number.
	@discussion	Typically, a plug-in’s version increments with each release. The exact semantics
                are up to the plug-in developer.
                To support versioning, each plug-in within a bundle defines its current version
                number as a string value with a "version" key in the plug-in's entry in the
                ProPlugPlugInList array in the plug-in’s Info.plist. When a project is first
                created, the host application remembers the version number of each plug-in used in
                that project. Later, when the user opens the project, a plug-in can query the value
                of versionAtCreation and then handle any backward-compatibility issues.
*/
- (unsigned int)versionAtCreation;



/*!
 @method		updateVersionAtCreation
 @abstract      Updates the version of the plug-in that was stored when the plug-in was added to
                the clip.
 @param         newVersion      The new version number.
 @result		If the update was successful, this method will return YES; otherwise, NO.
 @discussion	If a plug-in would like to update its version at creation value, use this method.
                No information about previous backward compatibility will be stored, and the plug-in
                will be treated by the host app as if it were created with the specified version
                number. Updating will fail if the specified number is lower than the already-saved
                version number.
 */
- (BOOL)updateVersionAtCreation:(UInt32)newVersion;

@end
