#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"

constexpr float ZOMBIE_WALK_SPEED = 2.0f;

void UpdateZombieMovement() 
{
	GameObject& obj_zombie = Play::GetGameObjectByType(TYPE_ZOMBIE);
	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);
	Play::PointGameObject(obj_zombie, 2, obj_player.pos.x, obj_player.pos.y);

	obj_zombie.pos += obj_zombie.velocity;
}