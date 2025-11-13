#include "../math/math.h"
#include "../math/fixed_point.h"

#define STARTPOS VEC3INIT
#define THRUSTACC fpInt(0, 1)
#define PITCHSPEED fpInt(1, 0)
#define YAWSPEED fpInt(1, 0)

typedef struct
{
    Vec3 pos;
    Vec3 vel;
    Vec3 dir;
} Player;

#define PLAYERINIT (Player){STARTPOS, VEC3INIT, (Vec3){fpInt(1, 0), 0, 0}}

void useThruster(Player* player);

void updatePitch(Player* player);

void updateYaw(Player* player);