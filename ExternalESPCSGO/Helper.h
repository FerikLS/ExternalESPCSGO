#include <iostream>
#include <Windows.h>
#include "DataVectors.h"

void ConvertToRange(Vec2& Point)
{
	Point.X /= 1920.0f;
	Point.X *= 2.0f;
	Point.X -= 1.0f;

	Point.Y /= 1080.0f;
	Point.Y *= 2.0f;
	Point.Y -= 1.0f;

}

bool W2S(const Vec3& VecOrigin, Vec2& VecScreen, float* matrix)
{
	VecScreen.X = VecOrigin.X * matrix[0] + VecOrigin.Y * matrix[1] + VecOrigin.Z * matrix[2] + matrix[3];
	VecScreen.Y = VecOrigin.X * matrix[4] + VecOrigin.Y * matrix[5] + VecOrigin.Z * matrix[6] + matrix[7];
	float W = VecOrigin.X * matrix[12] + VecOrigin.Y * matrix[13] + VecOrigin.Z * matrix[14] + matrix[15];

	if (W < 0.01f)
		return false;

	Vec2 NDC{};
	NDC.X = VecScreen.X / W;
	NDC.Y = VecScreen.Y / W;

	VecScreen.X = (1920 / 2 * NDC.X) + (NDC.X + 1920 / 2);
	VecScreen.Y = (1080 / 2 * NDC.Y) + (NDC.Y + 1080 / 2);

	ConvertToRange(VecScreen);

	return true;
}