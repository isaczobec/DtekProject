#include "chunks.h"
#include "rasterization.h"
#include "../math/math.h"
#include "../math/fixed_point.h"

/* Sets the position of the object pointed to by the `handle`
to `newPosition`. */
void setObjectPosition(ObjectHandle* handle, Vec3* newPosition); 
/* Sets the rotation of the object pointed to by the `handle`
to `newRotation`. */
void setObjectRotation(ObjectHandle* handle, Vec3* newRotation);
/* Sets the scale of the object pointed to by the `handle`
to `newScale`. */
void setObjectScale(ObjectHandle* handle, Vec3* newScale);