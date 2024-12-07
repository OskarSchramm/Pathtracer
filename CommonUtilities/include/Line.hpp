#pragma once
#include "Vector2.hpp"

namespace CommonUtilities
{
	template <typename T> class Line
	{
	public:
		Line() = default;
		Line(const Line <T>& aLine) = default;
		Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1) : myPoint(aPoint0), myNormal(-(aPoint1 - aPoint0).y, (aPoint1 - aPoint0).x) { myNormal.Normalize(); }
		void InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
		{
			myPoint = aPoint0;
			myNormal = Vector2<T>(-(aPoint1 - aPoint0).y, (aPoint1 - aPoint0).x).GetNormalized();
		}
		void InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection)
		{
			myPoint = aPoint;
			myNormal = Vector2<T>(-aDirection.y, aDirection.x).GetNormalized();
		}
		inline bool IsInside(const Vector2<T>& aPosition) const { return (aPosition - myPoint).Dot(myNormal) <= T(0); }
		Vector2<T> GetDirection() const { return Vector2<T>(myNormal.y, -myNormal.x); }
		inline const Vector2<T>& GetNormal() const { return myNormal; }

	private:
		Vector2<T> myPoint;
		Vector2<T> myNormal;
	};

}