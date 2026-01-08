/*!
 @header        FxProjectAPI.h
 @abstract      Defines the FxProjectAPI protocols to get information about the project in which
                your plug-in instance is running.
 */
/* Copyright © 2020-2025 Apple Inc. All rights reserved. */

#ifndef FxProjectAPI_h
#define FxProjectAPI_h

#import <Foundation/Foundation.h>

/*!
    @protocol   FxProjectAPI
    @abstract   Methods you use to get information about the project in which your plug-in instance
                is running.
 */

@protocol FxProjectAPI

/*!
    @method     -mediaFolderURL:error:
    @abstract   Provides the security-scoped URL for a plug-in data folder within a project’s media folder.
    @discussion Use this method to get the URL for a plug-in-specific directory within the media
                folder of a Motion project. The mediaURL will be nil if the user has not saved the
                project or the user did not select the “Collect Media” feature when saving.
                A sandboxed plug-in needs to add the appropriate entitlements to use security-scoped
                bookmarks to use this URL.
    @result     Returns YES if the mediaURL exists and your plug-in can write to it. Returns NO if
                the host can't retrieve the mediaURL or some other error occurred while retrieving it.
*/
- (BOOL)mediaFolderURL:(NSURL**)mediaURL
                 error:(NSError**)error;

/*!
    @method     -documentID:error:
    @abstract   Provides the document ID number of the host’s project.
    @discussion Use this method to get the document ID number of the host application's project.
    @result     Returns YES if the documentID exists. Returns NO if the host can’t retrieve the
                documentID or some other error occurred while retrieving it.
*/
- (BOOL)documentID:(NSUInteger*)documentID
             error:(NSError**)error;

@end


/*!
    @protocol   FxProjectAPI_v2
    @abstract       Extends the FxProjectAPI with an additional method for obtaining information about
                the project your plug-in is contained in.
*/
@protocol FxProjectAPI_v2 <FxProjectAPI>

/*!
    @method     -projectAspectRatio:error:
    @abstract   Provides the frame's aspect ratio for the curent project
    @discussion Use this method to get the aspect ratio of the current project. Note that this is not the pixel
                aspect ratio. Also note that footage your plug-in is applied to or generating may not be the
                same aspect ratio as the project or sequence it sits in. This method returns the image space
                aspect ratio, meaning that it returns the aspect ratio the project will be displayed at after
                any non-square pixels and fields are accounted for. So a DV SD project that's 720x480 with
                10:11 pixel aspect ratio will return (720 * 10/11) / 480 = ~1.36363636... as the aspect ratio, not 1.5.
    @result     Returns YES if it was able to successfully retrieve the project aspect ratio. Otherwise it
                returns NO and sets the error appropriately.
*/

- (BOOL)projectAspectRatio:(float*)aspectRatio
                     error:(NSError**)error;

@end

#endif /* FxProjectAPI_h */
