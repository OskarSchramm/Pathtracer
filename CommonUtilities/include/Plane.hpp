#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template <typename T> class Plane
	{
	public:
		Plane() = default;
		Plane(const Plane& aPlane) = default;
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
			: myPoint(aPoint0), myNormal((aPoint1 - aPoint0).Cross(aPoint2 - aPoint0))
		{
			myNormal.Normalize();
		}
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aNormal) : myPoint(aPoint0), myNormal(aNormal.GetNormalized()) {}
		void InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
		{
			myPoint = aPoint0;
			myNormal = (aPoint1 - aPoint0).Cross(aPoint2 - aPoint0).GetNormalized();
		}
		void InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
		{
			myPoint = aPoint;
			myNormal = aNormal.GetNormalized();
		}
		inline bool IsInside(const Vector3<T>& aPosition) const { return (aPosition - myPoint).Dot(myNormal) <= T(0); }
		inline const Vector3<T>& GetNormal() const { return myNormal; }
		inline T GetSignedDistanceToOrigin() const { return myPoint.Dot(myNormal); }

	private:
		Vector3<T> myPoint;
		Vector3<T> myNormal;
	};
}