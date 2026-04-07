#include "Debug.h"

std::ostream& operator<<(std::ostream& out, Vector3 vec)
{
	out << vec.x << " " << vec.y << " " << vec.z;
	return out;
}

std::ostream& operator<<(std::ostream& out, tempWeapInfo& info)
{
	out << info.radiSize;
	return out;
}

std::ostream& operator<<(std::ostream& out, Color color)
{
	if (((color.r == Color{ RED }.r) && (color.g == Color{ RED }.g) && (color.b == Color{ RED }.b) && (color.a == Color{ RED }.a)))
	{
		out << "RED";
	}
	else
	{
		out << "BLUE";
	}
	return out;
}


