#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template <typename T>
	class Sphere
	{
	public:
		Sphere() = default;
		Sphere(const Sphere<T>& aSphere) = default;
		Sphere(const Vector3<T>& aCenter, T aRadius) : myCenter(aCenter), myRadius(aRadius) {}
		void InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius)
		{
			myCenter = aCenter;
			myRadius = aRadius;
		}
		inline bool IsInside(const Vector3<T>& aPosition) const
		{
			return (aPosition - myCenter).LengthSqr() <= myRadius * myRadius;
		}
		inline const Vector3<T>& GetCenter() const { return myCenter; }
		inline const T& GetRadius() const { return myRadius; }

	private:
		Vector3<T> myCenter;
		T myRadius;
	};
}