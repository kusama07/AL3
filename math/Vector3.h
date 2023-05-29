#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	inline const Vector3 operator+(const Vector3& other) const {
		Vector3 result;
		result.x = x + other.x;
		result.y = y + other.y;
		result.z = z + other.z;
		return result;
	}

	inline Vector3& operator+=(const Vector3& other) {
		
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}
};