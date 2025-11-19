#include "chunks.h"
#include "rasterization.h"
#include "../math/math.h"
#include "../math/fixed_point.h"

void setObjectPosition(ObjectHandle* handle, Vec3* newPosition);
void setObjectRotation(ObjectHandle* handle, Vec3* newRotation);
void setObjectScale(ObjectHandle* handle, Vec3* newScale);