#pragma once

#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>

Vector3 Add(const Vector3& v1, const Vector3& v2);

Vector3 Subtract(const Vector3& v1, const Vector3& v2);

Vector3 VectorScale(const Vector3& v1, const float& v2);

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Vector3 Normalize(const Vector3& v);

Vector3 magnitude(const Vector3& v1,const Vector3&v2);
