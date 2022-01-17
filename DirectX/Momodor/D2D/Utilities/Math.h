#pragma once
class Math
{
public:
	static const float PI;

	static float ToRadian(float degree);
	static float ToDegree(float radian);

	static int Random(int r1, int r2);
	static float Random(float r1, float r2);

	template<typename T>
	static T Clamp(T value, T min, T max)
	{
		value = value > max ? max : value;
		value = value < min ? min : value;

		return value;
	}
};

