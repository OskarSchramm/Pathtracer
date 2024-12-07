#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template <typename T>
	class Ray
	{
	public:
		Ray() = default;
		Ray(const Ray<T>& aRay) = default;
		Ray(const Vector3<T>& aOrigin, const Vector3<T>& aPoint) : myOrigin(aOrigin), myDirection((aPoint - aOrigin).GetNormalized()) {}
		// Init the ray with two points, the same as the constructor above.
		void InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint)
		{
			myOrigin = aOrigin;
			myDirection = (aPoint - aOrigin).GetNormalized();
		}
		// Init the ray with an origin and a direction.
		void InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection)
		{
			myOrigin = aOrigin;
			myDirection = aDirection.GetNormalized();
		}
		inline const Vector3<T>& GetOrigin() const { return myOrigin; }
		inline const Vector3<T>& GetDirection() const { return myDirection; }
	private:
		Vector3<T> myOrigin;
		Vector3<T> myDirection;
	};
}