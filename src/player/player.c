#include "player.h"

void useThruster(Player* player) {
    //first needs vector normalization
}

void updatePitch(Player* player) {
    player->dir = rotateYVec3(player->dir, PITCHSPEED);
}

void updateYaw(Player* player) {
    player->dir = rotateZVec3(player->dir, YAWSPEED);
}