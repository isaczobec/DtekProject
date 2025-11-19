#include "player.h"

void useThruster(Player* player) {
    //first needs vector normalization
}

void updatePitch(Player* player) {
    rotateYVec3(&player->dir, PITCHSPEED);
}

void updateYaw(Player* player) {
    rotateZVec3(&player->dir, YAWSPEED);
}