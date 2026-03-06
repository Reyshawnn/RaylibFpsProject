#include "Target.h"

void Target::updateTarget()
{
    box.min =
    {
        position.x - size.x * 0.5f,
        position.y - size.y * 0.5f,
        position.z - size.z * 0.5f
    };

    box.max =
    {
        position.x + size.x * 0.5f,
        position.y + size.y * 0.5f,
        position.z + size.z * 0.5f
    };
}
