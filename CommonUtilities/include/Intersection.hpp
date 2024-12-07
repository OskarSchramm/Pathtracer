#pragma once
#include "Vector3.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Ray.hpp"
#include "AABB3D.hpp"
#include <cmath>

namespace CommonUtilities
{
	template <typename T>
	bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		T dot = aRay.GetDirection().Dot(aPlane.GetNormal());
		if (dot == T(0)) return false;
		T t = (aPlane.GetSignedDistanceToOrigin() - aRay.GetOrigin().Dot(aPlane.GetNormal())) / dot;
		if (t < T(0)) return false;
		aOutIntersectionPoint = aRay.GetOrigin() + t * aRay.GetDirection();
		return true;
	}
	// If the ray intersects the AABB, true is returned, if not, false is returned.
	// Any ray starting on the inside is considered to intersect the AABB
	template <typename T>
	bool IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		bool isInside = true;

		const auto& ori = aRay.GetOrigin();
		const auto& dir = aRay.GetDirection();
		const auto& min = aAABB.GetMin();
		const auto& max = aAABB.GetMax();

		T xt = T(-1);
		if (ori.x < min.x)
		{
			if (dir.x < T(0)) return false;
			xt = min.x - ori.x;
			isInside = false;
		}
		else if (max.x < ori.x)
		{
			if (dir.x > T(0)) return false;
			xt = ori.x - max.x;
			isInside = false;
		}

		T yt = T(-1);
		if (ori.y < min.y)
		{
			if (dir.y < T(0)) return false;
			yt = min.y - ori.y;
			isInside = false;
		}
		else if (max.y < ori.y)
		{
			if (dir.y > T(0)) return false;
			yt = ori.y - max.y;
			isInside = false;
		}

		T zt = T(-1);
		if (ori.z < min.z)
		{
			if (dir.z < T(0)) return false;
			zt = min.z - ori.z;
			isInside = false;
		}
		else if (max.z < ori.z)
		{
			if (dir.z > T(0)) return false;
			zt = ori.z - max.z;
			isInside = false;
		}

		if (isInside)
		{
			aOutIntersectionPoint = aRay.GetOrigin();
			return true;
		}

		T t = xt < yt ? yt : xt;
		t = t < zt ? zt : t;

		Vector3<T> p = aRay.GetOrigin() + t * aRay.GetDirection();

		if (aAABB.IsInside(p))
		{
			aOutIntersectionPoint = p;
			return true;
		}

		return false;
	}

	// If the ray intersects the AABB, true is returned, if not, false is returned.
	// Any ray starting on the inside is considered to intersect the AABB
	template <typename T>
	bool IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint, Vector3<T>& anOutNormal)
	{
		auto ori = aRay.GetOrigin();
		auto dir = aRay.GetDirection();
		const auto min = aAABB.GetMin();
		const auto max = aAABB.GetMax();

		bool hasGotod = false;

		start:
		bool isInside = true;

		T xt, xn;
		if (ori.x < min.x)
		{
			if (dir.x < T(0)) return false;
			xt = min.x - ori.x;
			xt /= dir.x;
			xn = -1.f;
			isInside = false;
		}
		else if (max.x < ori.x)
		{
			if (dir.x > T(0)) return false;
			xt = max.x - ori.x;
			xt /= dir.x;
			xn = 1.f;
			isInside = false;
		}
		else
			xt = -1.f;

		T yt, yn;
		if (ori.y < min.y)
		{
			if (dir.y < T(0)) return false;
			yt = min.y - ori.y;
			yt /= dir.y;
			yn = -1.f;
			isInside = false;
		}
		else if (max.y < ori.y)
		{
			if (dir.y > T(0)) return false;
			yt = max.y - ori.y;
			yt /= dir.y;
			yn = 1.f;
			isInside = false;
		}
		else
			yt = -1.f;

		T zt, zn;
		if (ori.z < min.z)
		{
			if (dir.z < T(0)) return false;
			zt = min.z - ori.z;
			zt /= dir.z;
			zn = -1.f;
			isInside = false;
		}
		else if (max.z < ori.z)
		{
			if (dir.z > T(0)) return false;
			zt = max.z - ori.z;
			zt /= dir.z;
			zn = 1.f;
			isInside = false;
		}
		else
			zt = -1.f;

		if (isInside)
		{
			assert(!hasGotod);
			auto size = max - min;
			ori += dir * (size.x + size.y + size.z);
			dir = -dir;
			hasGotod = true;
			goto start;
		}

		enum class Plane { YZ, ZX, XY } which = Plane::YZ;
		float t = xt;
		if (t < yt)
		{
			t = yt;
			which = Plane::ZX;
		}
		if (t < zt)
		{ 
			t = zt;
			which = Plane::XY;
		}

		Vector3<T> p = aRay.GetOrigin() + t * aRay.GetDirection();

		if (!aAABB.IsInside(p))
			return false;

		aOutIntersectionPoint = p;

		switch (which)
		{
		case Plane::YZ:
			anOutNormal = { xn, 0.f, 0.f };
			break;
		case Plane::ZX:
			anOutNormal = { 0.f, yn, 0.f };
			break;
		case Plane::XY:
			anOutNormal = { 0.f, 0.f, zn };
			break;
		}

		return true;
	}

	// If the ray intersects the sphere, true is returned, if not, false is returned.
	// Any ray starting on the inside is considered to intersect the sphere
	template <typename T>
	bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		T a = (aSphere.GetCenter() - aRay.GetOrigin()).Dot(aRay.GetDirection());
		T ac2 = (aRay.GetOrigin() + a * aRay.GetDirection() - aSphere.GetCenter()).LengthSqr();
		if (ac2 > aSphere.GetRadius() * aSphere.GetRadius()) return false;
		T b = a - sqrt(aSphere.GetRadius() * aSphere.GetRadius() - ac2);
		if (b < T(0)) return false;
		aOutIntersectionPoint = aRay.GetOrigin() + b * aRay.GetDirection();
		return true;
	}
}