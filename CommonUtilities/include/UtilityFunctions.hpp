#pragma once
#include <cassert>

namespace CommonUtilities
{
	template<typename T>
	T Min(const T& aFirstValue, const T& aSecondValue)
	{
		return (aFirstValue < aSecondValue) ? aFirstValue : aSecondValue;
	}

	template<typename T>
	T Max(const T& aFirstValue, const T& aSecondValue)
	{
		return (aFirstValue > aSecondValue) ? aFirstValue : aSecondValue;
	}

	template<typename T>
	T Abs(T& aValue)
	{
		return (aValue < 0) ? aValue *= -1.0f : aValue;
	}

	template<typename T>
	T Clamp(const T& aMin, const T& aMax, const T& aValue)
	{
		assert(aMin <= aMax && aMax >= aMin && L"Fail when checking: Min: aMax , Max: aMin");

		if (aValue < aMin)
			return aMin;
		else if (aValue > aMax)
			return aMax;
		else
			return aValue;
	}

	template<typename T>
	T Lerp(const T& aFirstValue, const T& aSecondValue, const float somePercent)
	{
		assert(somePercent >= .0f && "Percent is less than 0");

		return (aFirstValue + somePercent * (aSecondValue - aFirstValue));
	}

	template<typename T>
	void Swap(T& aFirstValue, T& aSecondValue)
	{
		T temp = aFirstValue;
		aFirstValue = aSecondValue;
		aSecondValue = temp;
	}
}