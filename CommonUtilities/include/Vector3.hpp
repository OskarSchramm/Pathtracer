#pragma once
#include <cmath>

namespace CommonUtilities
{
	template <class T> class Vector3
	{
	public:
		T x;
		T y;
		T z;

		Vector3<T>() : x(0), y(0), z(0) {}
		Vector3<T>(const T& aX, const T& aY, const T& aZ) : x(aX), y(aY), z(aZ) {}
		Vector3<T>(const Vector3<T>& aVector) = default;
		Vector3<T>& operator=(const Vector3<T>& aVector3) = default;
		Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }
		bool operator==(const Vector3<T>& aVector) const
		{
			return (x == aVector.x) && (y == aVector.y) && (z == aVector.z);
		}
		bool operator!=(const Vector3<T>& aVector) const { return !(*this == aVector); }
		~Vector3<T>() = default;
		T LengthSqr() const { return x * x + y * y + z * z; }
		T Length() const { return sqrt(LengthSqr()); }
		Vector3<T> GetNormalized() const
		{
			if (x == 0 && y == 0 && z == 0)
				return Vector3<T>();
			return Vector3<T>(x / Length(), y / Length(), z / Length());
		}
		void Normalize()
		{
			if (x == 0 && y == 0 && z == 0)
				return;
			T length = Length();
			x /= length;
			y /= length;
			z /= length;
		}
		T Dot(const Vector3<T>& aVector) const { return x * aVector.x + y * aVector.y + z * aVector.z; }
		Vector3<T> Cross(const Vector3<T>& aVector) const
		{
			return Vector3<T>(y * aVector.z - z * aVector.y, z * aVector.x - x * aVector.z, x * aVector.y - y * aVector.x);
		}
	};

	template <class T> Vector3<T> operator+(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return Vector3<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z);
	}
	template <class T> Vector3<T> operator-(const Vector3<T>& aVector0, const Vector3<T>& aVector1) { return aVector0 + (-aVector1); }
	template <class T> Vector3<T> operator*(const Vector3<T>& aVector, const T& aScalar)
	{
		return Vector3<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar);
	}
	template <class T> Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector) { return aVector * aScalar; }
	template <class T> Vector3<T> operator*(const Vector3<T>& aLVector, const Vector3<T>& aRVector) { return Vector3<T>(aLVector.x * aRVector.x, aLVector.y * aRVector.y, aLVector.z * aRVector.z); }
	template <class T> Vector3<T> operator/(const Vector3<T>& aVector, const T& aScalar) { return aVector * (T(1) / aScalar); }
	template <class T> void operator+=(Vector3<T>& aVector0, const Vector3<T>& aVector1) 
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		aVector0.z += aVector1.z;
	}
	template <class T> void operator-=(Vector3<T>& aVector0, const Vector3<T>& aVector1) { aVector0 += (-aVector1); }
	template <class T> void operator*=(Vector3<T>& aVector, const T& aScalar) 
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
		aVector.z *= aScalar;
	}
	template <class T> void operator/=(Vector3<T>& aVector, const T& aScalar) { aVector *= (T(1) / aScalar); }
}