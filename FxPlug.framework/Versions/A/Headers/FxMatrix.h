/*!
    @header     FxMatrix.h
 
    @abstract   A class for doing simple matrix transformations and passing matrix data between
                the host and plugin.
 
    @discussion Several FxPlug protocols have methods which take or return a 4x4 matrix. It can
                also be useful to transform 2D and 3D points using such a matrix.
                NOTE: FxMatrix44 stores its data in row major order, which is different from OpenGL.
                Introduced in FxPlug 2.0.
*/

/*  Copyright © 2009-2025 Apple Inc. All rights reserved. */

#ifndef __FXMATRIX_H__
#define __FXMATRIX_H__

#import <FxPlug/FxTypes.h>


/*!
    @typedef    Matrix44Data
    @discussion A 4x4 matrix of doubles.
*/

typedef double  Matrix44Data[4][4];



/*!
    @class      FxMatrix44
    @abstract   A 4x4 matrix object.
    @discussion A class for doing basic operations on a 4x4 matrix, such as inversion, and 
                transforming 2D and 3D points.
*/
@interface FxMatrix44 : NSObject <NSSecureCoding>
{
    Matrix44Data    _mat;
}

/*!
    @method     init
    @discussion Initializes the matrix to the identity matrix.
*/
- (id)init;

/*!
    @method     initWithMatrix44Data:
    @param      newMatrix   - The new matrix data you want to put into the object
    @discussion Creates a new matrix with the data supplied.
*/
- (id)initWithMatrix44Data:(Matrix44Data)newMatrix;

/*!
    @method     initWithColorMatrix44Data:
    @param      newMatrix   - The new color matrix data you want to put into the object
    @discussion Creates a new matrix with the data supplied, but does not treat the data
                as a spatial transformation matrix, but rather as a color matrix.
*/
- (id)initWithColorMatrix44Data:(Matrix44Data)newMatrix;

/*!
    @method     initWithFxMatrix:
    @param      newFxMatrix - A matrix to make a copy of
    @discussion Creates a new matrix with the same data as the one passed in
*/
- (id)initWithFxMatrix:(FxMatrix44*)newFxMatrix;

/*!
    @method     initWithInverseOfFxMatrix:
    @param      matrixToInvert - A matrix to take the inverse of
    @discussion Creates a new matrix with the inverse of the passed in matrix
 */
- (instancetype)initWithInverseOfFxMatrix:(FxMatrix44*)matrixToInvert;

/*!
    @method     setToIdentity
    @discussion Resets the matrix to the identity matrix
*/
- (void)setToIdentity;

/*!
    @method     setMatrix:
    @param      newMatrix - The new matrix data you want to put into the object
    @discussion Sets an existing matrix to have new values
*/
- (void)setMatrix:(Matrix44Data)newMatrix;

/*!
    @method     matrix
    @discussion returns the raw matrix data in a natural format
*/
- (Matrix44Data*)matrix;

/*!
    @method     invert
    @discussion Inverts the matrix if possible. Returns a value indicating whether it
                succeeded in inverting the matrix or not.
*/
- (BOOL)invert;

/*!
    @method     invertColorMatrixWithTolerance:
    @param      tolerance -> A threshold for determining whether inversion is possible.
                             If set to 0, then inversion will only fail if the determininant
                             of the matrix is exactly 0.0.
    @discussion Inverts the matrix if possible. Returns a value indicating whether it
                succeeded in inverting the matrix or not.
 */
- (BOOL)invertColorMatrixWithTolerance:(double)tolerance;

/*!
    @method     transform2DPoint:
    @param      inPoint - the 2D point to be transformed
    @discussion Transforms a 2D point using the matrix.
*/
- (FxPoint2D)transform2DPoint:(FxPoint2D)inPoint;

/*!
    @method     transform3DPoint:
    @param      inPoint - the 3D point to be transformed
    @discussion Transforms a 3D point using the matrix.
*/
- (FxPoint3D)transform3DPoint:(FxPoint3D)inPoint;

/*!
    @method     transpose
    @discussion Transposes the matrix.
*/
- (void)transpose;

@end

#endif // __FXMATRIX_H__
