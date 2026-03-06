#pragma once
#include<raylib.h>
//throwaway class
struct Target
{
    Vector3 position{ 5.0f,2.5f,5.0f };
    Vector3 size{ 3.0f,3.0f,3.0f };
    BoundingBox box;
    void updateTarget();
};



