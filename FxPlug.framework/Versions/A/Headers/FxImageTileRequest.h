/*!
 @header        FxImageTileRequest.h
 @abstract      Defines an image tile request object that is passed between an out-of-process FxPlug
                plug-in and a host application.
 */
/* Copyright © 2019-2025 Apple Inc. All rights reserved. */

#import <Foundation/Foundation.h>
#import <CoreMedia/CoreMedia.h>

NS_ASSUME_NONNULL_BEGIN

typedef enum : NSUInteger {
    kFxImageTileRequestSourceNone,          // Not associated with an effect or parameter
    kFxImageTileRequestSourceEffectClip,    // Request frame associated with source clip
    kFxImageTileRequestSourceParameter,     // Request frame associated with an image well parameter
    kFxImageTileRequestSourceOutput,        // The output of a plugin
} FxImageTileRequestSource;

/*!
 @class      FxImageTileRequest
 @abstract   An image tile request object that is passed between an out-of-process FxPlug plug-in
             and a host application.
 */
@interface FxImageTileRequest : NSObject <NSSecureCoding>

@property (readonly, nonatomic) FxImageTileRequestSource source;

@property (readonly, nonatomic) CMTime requestTime;

@property (readonly, nonatomic) BOOL includeLeadingFilters;

@property (readonly, nonatomic) UInt32 parameterID; // 0 for effect source, otherwise the ID of the image well parameter whose frame you are requesting

- (nullable instancetype)initWithSource:(FxImageTileRequestSource)requestSource
                                   time:(CMTime)requestTime
                         includeFilters:(BOOL)includeFilters
                            parameterID:(UInt32)parameterID;

@end

NS_ASSUME_NONNULL_END
