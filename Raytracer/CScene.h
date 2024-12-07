#pragma once

#include "Util.h"

// CommonUtilities
#include "Vector3.hpp"
#include "Vector2.hpp"
#include "AABB3D.hpp"
#include "Intersection.hpp"
#include "Plane.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "UtilityFunctions.hpp"

// stdlib
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <limits>
#include <iostream>

constexpr float PI = 3.14159265358979323846f;

using Vector3f = CommonUtilities::Vector3<float>;
using Vector2f = CommonUtilities::Vector2<float>;
using Ray = CommonUtilities::Ray<float>;

Vector2f RandomVector2OnDisc()
{
	float randomRadian = RandomFloat() * 2.f * PI;
	float randomRadius = std::sqrtf(RandomFloat());
	return { randomRadius * std::cosf(randomRadian), randomRadius * std::sinf(randomRadian) };
}

Vector3f RandomUnitVector3()
{
	float z = RandomFloat() * 2.0f - 1.0f;
	float a = RandomFloat() * 2.0f * PI;
	float r = sqrtf(1.0f - z * z);
	float x = r * cosf(a);
	float y = r * sinf(a);
	return Vector3f(x, y, z);
}

enum class MaterialType
{
	Normal,
	Mirror,
	Emissive,
	Glass
};

struct Primitive
{
	virtual ~Primitive() = default;

	virtual bool Hit(const Ray& aRay, Vector3f& hit, Vector3f& normal) const = 0;

	virtual Vector3f GetColor() const = 0;
	virtual MaterialType GetMaterialType() const = 0;
	virtual void SetMaterialType(const MaterialType) = 0;
	virtual float GetRefractiveIndex() const = 0;
	virtual void SetRefractionIndex(const float) = 0;
};

struct Sphere : public Primitive
{
	virtual Vector3f GetColor() const override
	{
		return myColor;
	}
	virtual MaterialType GetMaterialType() const override
	{
		return myType;
	}
	virtual void SetMaterialType(const MaterialType aType)
	{
		myType = aType;
	}
	virtual float GetRefractiveIndex() const override
	{
		return myRefractiveIndex;
	}
	virtual void SetRefractionIndex(const float aRefrIndex)
	{
		myRefractiveIndex = aRefrIndex;
	}

	virtual bool Hit(const Ray& aRay, Vector3f& hit, Vector3f& normal) const
	{
		if (!CommonUtilities::IntersectionSphereRay(mySphere, aRay, hit))
			return false;
		normal = (hit - mySphere.GetCenter()).GetNormalized();
		return true;
	}

	CommonUtilities::Sphere<float> mySphere;
	Vector3f myColor;
	MaterialType myType;
	float myRefractiveIndex = 1.f;
};

struct AABB : public Primitive
{
	virtual Vector3f GetColor() const override
	{
		return myColor;
	}
	virtual MaterialType GetMaterialType() const override
	{
		return myType;
	}
	virtual float GetRefractiveIndex() const override
	{
		return myRefractiveIndex;
	}
	virtual void SetMaterialType(const MaterialType aType)
	{
		myType = aType;
	}
	virtual void SetRefractionIndex(const float aRefrIndex)
	{
		myRefractiveIndex = aRefrIndex;
	}

	virtual bool Hit(const Ray& aRay, Vector3f& hit, Vector3f& normal) const
	{
		return CommonUtilities::IntersectionAABBRay(myAABB, aRay, hit, normal);
	}

	CommonUtilities::AABB3D<float> myAABB;
	Vector3f myColor;
	MaterialType myType;
	float myRefractiveIndex = 1.f;
};

struct Camera
{
	Vector3f myPos;
	Vector3f myRight;
	Vector3f myUp;
	Vector3f myForward;
};

struct Sky
{
	Vector3f myHorizonColor;
	Vector3f myZenithColor;
};

struct Light
{
	Vector3f myDir;
	Vector3f myColor;
};


class CScene
{
public:
	CScene(int width, int height);
	bool Load(const char* filename);
	inline SRGB Raytrace(int x, int y);
	inline Vector3f Raytrace(const Ray& aRay, int aRemainingBounces);
	inline Vector3f CalculateSkyColor(const float anY);
	inline bool Hit(const Ray& aRay, Primitive*& aOutPrimitive, Vector3f& aOutHit, Vector3f& anOutNormal);

private:
	int myWidth;
	int myHeight;
	int myRaysPerPixel = 100;
	int myMaxBounces = 2;

	bool myHasDirectionalLight = false;

	// Add member variables to store scene here
	std::vector<Primitive*> myPrimitives;
	std::vector<Sphere> mySpheres;
	std::vector<AABB> myAABBs;

	Camera myCamera;
	Sky mySky;
	Light myLight;

	float myDepthOfField = 3.f;
	float myLensRadius = 0.04f;
};

CScene::CScene(int width, int height) : myWidth(width), myHeight(height) {}

namespace
{
	inline std::stringstream& operator>>(std::stringstream& aStream, Vector3f& aVec)
	{
		aStream >> aVec.x >> aVec.y >> aVec.z;
		return aStream;
	}

	inline std::ostream& operator<<(std::ostream& aStream, const Vector3f& aVec)
	{
		aStream << "[" << aVec.x << ", " << aVec.y << ", " << aVec.z << "]";
		return aStream;
	}

	inline std::ostream& operator<<(std::ostream& aStream, const Camera& aCam)
	{
		std::cout << "Camera at position " << aCam.myPos << std::endl;
		std::cout << "With orientation: \n";
		std::cout << aCam.myRight << std::endl;
		std::cout << aCam.myUp << std::endl;
		std::cout << aCam.myForward << std::endl;
		return aStream;
	}

	inline std::ostream& operator<<(std::ostream& aStream, const Light& aLight)
	{
		std::cout << "Light with direction " << aLight.myDir << std::endl;
		std::cout << "and color: " << aLight.myColor << std::endl;
		return aStream;
	}

	inline std::ostream& operator<<(std::ostream& aStream, const Sky& aSky)
	{
		std::cout << "Sky with horizon color" << aSky.myHorizonColor << std::endl;
		std::cout << "and zenith color: " << aSky.myZenithColor << std::endl;
		return aStream;
	}

	inline std::ostream& operator<<(std::ostream& aStream, const MaterialType aType)
	{
		switch (aType)
		{
		case MaterialType::Normal:
			aStream << "normal";
			break;
		case MaterialType::Mirror:
			aStream << "mirror";
			break;
		case MaterialType::Emissive:
			aStream << "emmisive";
			break;
		case MaterialType::Glass:
			aStream << "glass";
			break;
		}
		return aStream;
	}

	inline std::ostream& operator<<(std::ostream& aStream, const Sphere& aSphere)
	{
		std::cout << "Sphere at center " << aSphere.mySphere.GetCenter() << std::endl;
		std::cout << "With radius: " << aSphere.mySphere.GetRadius() << std::endl;
		std::cout << "and color: " << aSphere.myColor << std::endl;
		std::cout << "and material: " << aSphere.GetMaterialType() << std::endl;
		if (aSphere.myType == MaterialType::Glass)
			std::cout << "and refraction index: " << aSphere.myRefractiveIndex << std::endl;
		return aStream;
	}

	inline std::ostream& operator<<(std::ostream& aStream, const AABB& anAABB)
	{
		auto min = anAABB.myAABB.GetMin();
		auto max = anAABB.myAABB.GetMax();
		auto center = (min + max) / 2.f;
		auto size = max - min;
		std::cout << "AABB at center " << center << std::endl;
		std::cout << "With width: " << size << std::endl;
		std::cout << "and color: " << anAABB.myColor << std::endl;
		std::cout << "and material: " << anAABB.GetMaterialType() << std::endl;
		if (anAABB.myType == MaterialType::Glass)
			std::cout << "and refraction index: " << anAABB.myRefractiveIndex << std::endl;
		return aStream;
	}

	inline std::ostream& operator<<(std::ostream& aStream, Primitive* aPrimitive)
	{
		if (auto sphere = dynamic_cast<Sphere*>(aPrimitive))
			std::cout << *sphere << std::endl;
		else if (auto aabb = dynamic_cast<AABB*>(aPrimitive))
			std::cout << *aabb << std::endl;
		return aStream;
	}
}

bool CScene::Load(const char* aFilename)
{
	std::ifstream file(aFilename);
	if (!file.is_open())
		return false;

	std::string str;
	while (std::getline(file, str))
	{
		if (str.size() < 2 || str.substr(0, 2) == "//")
			continue;

		std::stringstream ss(str);
		std::string objectType;
		ss >> objectType;

		if (objectType == "camera")
		{
			ss >> myCamera.myPos >> myCamera.myRight >> myCamera.myUp >> myCamera.myForward;
			std::cout << myCamera << std::endl;
		}

		if (objectType == "directional_light")
		{
			myHasDirectionalLight = true;
			ss >> myLight.myDir >> myLight.myColor;
			std::cout << myLight << std::endl;
			myLight.myDir.Normalize();
		}

		if (objectType == "sky")
		{
			ss >> mySky.myHorizonColor >> mySky.myZenithColor;
			std::cout << mySky << std::endl;
		}

		if (objectType != "sphere" && objectType != "aabb")
			continue;

		Primitive* primitive = nullptr;
		std::string materialType;
		ss >> materialType;

		if (objectType == "sphere")
		{
			Sphere sphere;
			Vector3f center;
			float radius;

			ss >> center >> radius;
			ss >> sphere.myColor >> sphere.myRefractiveIndex;

			sphere.mySphere.InitWithCenterAndRadius(center, radius);
			mySpheres.emplace_back(sphere);

			primitive = &mySpheres.back();
		}

		if (objectType == "aabb")
		{
			AABB aabb;
			Vector3f center;
			Vector3f width;

			ss >> center >> width >> aabb.myColor;

			aabb.myAABB.InitWithMinAndMax(center - width / 2.f, center + width / 2.f);
			myAABBs.emplace_back(aabb);

			primitive = &myAABBs.back();
		}

		if (materialType == "normal") primitive->SetMaterialType(MaterialType::Normal);
		else if (materialType == "mirror") primitive->SetMaterialType(MaterialType::Mirror);
		else if (materialType == "emissive") primitive->SetMaterialType(MaterialType::Emissive);
		else if (materialType == "glass")
		{
			primitive->SetMaterialType(MaterialType::Glass);
			float refractionIndex;
			ss >> refractionIndex;
			primitive->SetRefractionIndex(refractionIndex);
		}
		else primitive->SetMaterialType(MaterialType::Normal);

		ss << primitive;
	}

	for (auto& s : mySpheres)
		myPrimitives.push_back(&s);

	for (auto& a : myAABBs)
		myPrimitives.push_back(&a);

	return true;
}

SRGB CScene::Raytrace(int x, int y)
{
	Vector3f sum;
	for (size_t i = 0; i < myRaysPerPixel; i++)
	{
		// anti-aliasing
		auto aaX = x + RandomFloat();
		auto aaY = y + RandomFloat();

		float newX = 2 * (aaX / (float)myWidth - 0.5f);
		float newY = 2 * (aaY / (float)myHeight - 0.5f) * myHeight / (float)myWidth;

		Vector3f dir = myCamera.myForward + newX * myCamera.myRight + newY * myCamera.myUp;
		Vector3f pointOnDof = myCamera.myPos + dir * myDepthOfField;

		auto bokehOffset = RandomVector2OnDisc() * myLensRadius;
		auto origin = myCamera.myPos + bokehOffset.x * myCamera.myRight + bokehOffset.y * myCamera.myUp; // bokeh

		sum += Raytrace(Ray(origin, pointOnDof), myMaxBounces);
	}

	return { sum.x / myRaysPerPixel, sum.y / myRaysPerPixel, sum.z / myRaysPerPixel };
}

namespace
{
	inline Ray ReflectRay(const Ray& aRay, const Vector3f& aHit, const Vector3f& normal)
	{
		auto reflectionDir = (2.0f * -aRay.GetDirection()).Dot(normal) * normal - (-aRay.GetDirection());
		Ray reflectionRay;
		reflectionRay.InitWithOriginAndDirection(aHit + reflectionDir * 0.001f, reflectionDir);
		return reflectionRay;
	}

	inline Ray DiffuseRay(const Ray& aRay, const Vector3f& aHit, const Vector3f& normal)
	{
		Ray ray;
		ray.InitWithOriginAndDirection(aHit + normal * 0.001f, (normal + RandomUnitVector3()).GetNormalized());
		return ray;
	}

	inline float CalcReflCoeff(const float aRefIndex, const float aCosValue)
	{
		float R0Sqrt = (1.f - aRefIndex) / (1.f + aRefIndex);
		float R0 = R0Sqrt * R0Sqrt;
		float t = 1.f - aCosValue;
		return R0 + (1.f - R0) * t * t * t * t * t;
	}

	inline Ray FresnelRay(const Ray& aRay, const Vector3f& aHit, const Vector3f& normal, const float aRefIndex)
	{
		float c = normal.Dot(-aRay.GetDirection());
		float r = c > 0.f ? 1.f / aRefIndex : aRefIndex;
		c = c > 0.f ? c : -c;
		float radicand = 1.f - r * r * (1.f - c * c);

		if (radicand < 0.f || RandomFloat() < CalcReflCoeff(aRefIndex, c))
			return ReflectRay(aRay, aHit, normal);
		else
		{
			Vector3f refractDir = r * aRay.GetDirection() + (r * c - std::sqrtf(radicand)) * normal;
			Ray refractRay;
			refractRay.InitWithOriginAndDirection(aHit + refractDir * 0.001f, refractDir);
			return refractRay;
		}
	}
}

Vector3f CScene::Raytrace(const Ray& aRay, int aRemainingBounces)
{
	if (aRemainingBounces <= 0)
		return Vector3f();

	Primitive* primitive = nullptr;
	Vector3f hit;
	Vector3f normal;

	if (!Hit(aRay, primitive, hit, normal))
		return CalculateSkyColor(aRay.GetDirection().y);

	--aRemainingBounces;
	auto matColor = primitive->GetColor();

	switch (primitive->GetMaterialType())
	{
	case MaterialType::Emissive:
		return matColor;
	case MaterialType::Mirror:
		return matColor * Raytrace(ReflectRay(aRay, hit, normal), aRemainingBounces);
	case MaterialType::Glass:
		return Raytrace(FresnelRay(aRay, hit, normal, primitive->GetRefractiveIndex()), aRemainingBounces);
	case MaterialType::Normal:
	{
		auto color = matColor * Raytrace(DiffuseRay(aRay, hit, normal), aRemainingBounces);

		if (!myHasDirectionalLight)
			return color;
		else
		{
			Primitive* other = nullptr;
			Vector3f dummyHit;
			Vector3f dummyNormal;

			if (!Hit(Ray(hit, hit - myLight.myDir), other, dummyHit, dummyNormal) || other == primitive)
			{
				float lambertFactor = CommonUtilities::Max((normal.Dot(-myLight.myDir)), 0.0f);
				color += matColor * myLight.myColor * lambertFactor;
			}
		}

		return color;
	}
	}
}

Vector3f CScene::CalculateSkyColor(const float anY)
{
	return (1.0f - anY) * mySky.myHorizonColor + anY * mySky.myZenithColor;
}

bool CScene::Hit(const Ray& aRay, Primitive*& aOutPrimitive, Vector3f& aOutHit, Vector3f& anOutNormal)
{
	bool isHit = false;
	float distToNearest = std::numeric_limits<float>::infinity();
	for (auto& primitive : myPrimitives)
	{
		Vector3f nearestHit;
		Vector3f nearestNormal;
		if (primitive->Hit(aRay, nearestHit, nearestNormal))
		{
			isHit = true;
			float dist = (nearestHit - aRay.GetOrigin()).LengthSqr();
			if (dist < distToNearest)
			{
				aOutPrimitive = primitive;
				aOutHit = nearestHit;
				anOutNormal = nearestNormal;
				distToNearest = dist;
			}
		}
	}
	return isHit;
}