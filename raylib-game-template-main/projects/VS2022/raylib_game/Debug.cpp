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
	if ((color.r && color.g && color.b && color.a) == Color{RED}.r, Color{ RED }.g, Color{ RED }.b, Color{ RED }.a)
	{
		out << "RED";
	}
	else
	{
		out << "BLUE";
	}
	return out;
}


