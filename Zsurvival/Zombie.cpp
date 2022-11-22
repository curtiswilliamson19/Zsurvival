#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"

void UpdateZombie()
{

	GameObject& obj_zombie = Play::GetGameObjectByType(TYPE_ZOMBIE);
	obj_zombie.scale = 0.35f;
	Play::DrawObjectRotated(obj_zombie);
}