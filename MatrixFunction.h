#pragma once
#include "matrix.h"
#include <Matrix4x4.h>
#include <assert.h>
#include <cmath>
#include <math.h>
#include <Vector3.h>


Matrix4x4 Add(Matrix4x4 matrix1, Matrix4x4 matrix2);

Matrix4x4 Subtract(Matrix4x4 matrix1, Matrix4x4 matrix2);

Matrix4x4 Multiply(Matrix4x4 matrix1, Matrix4x4 matrix2);

Matrix4x4 Inverse(Matrix4x4 matrix);

Matrix4x4 Transpose(Matrix4x4 matrix);

Matrix4x4 Scalar(double k, Matrix4x4 matrix);

Matrix4x4 MakeIdentity4x4();
// アフィン変換関係

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 MakeRotateMatrix(const Vector3& rotate);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float l, float t, float r, float b, float zn, float zf);

// ビューポート変換行列
Matrix4x4
    MakeViewportMatrix(float left, float top, float width, float height, float minD, float maxD);


Vector3 TransformNormal(const Vector3& v, const Matrix4x4&m);



Matrix4x4 operator+(Matrix4x4 m1, Matrix4x4 m2); 
Matrix4x4 operator-(Matrix4x4 m1, Matrix4x4 m2);
Matrix4x4 operator*(const Matrix4x4& m1,const Matrix4x4& m2);
Matrix4x4 operator+=(Matrix4x4 m1, Matrix4x4 m2);
Matrix4x4 operator-=(Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 operator*=(Matrix4x4& m1, const Matrix4x4& m2);
