#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template <typename T>
	class AABB3D
	{
	public:
		AABB3D() = default;
		AABB3D(const AABB3D<T>& aAABB3D) = default;
		AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax) : myMin(aMin), myMax(aMax) {}
		void InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax)
		{
			myMin = aMin;
			myMax = aMax;
		}
		bool IsInside(const Vector3<T>& aPosition) const
		{
			return (aPosition.x >= myMin.x) && (aPosition.x <= myMax.x) &&
				(aPosition.y >= myMin.y) && (aPosition.y <= myMax.y) &&
				(aPosition.z >= myMin.z) && (aPosition.z <= myMax.z);
		}
		inline const Vector3<T> GetMin() const { return myMin; }
		inline const Vector3<T> GetMax() const { return myMax; }

	private:
		Vector3<T> myMin;
		Vector3<T> myMax;
	};
}
