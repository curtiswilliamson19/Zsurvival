#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"

constexpr float ZOMBIE_WALK_SPEED = 2.0f;

void UpdateZombieMovement() 
{

	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);

	std::vector<int> vZombies = Play::CollectGameObjectIDsByType(TYPE_ZOMBIE);

	for (int id : vZombies)
	{
		GameObject& obj_zombie = Play::GetGameObject(id);
		Play::PointGameObject(obj_zombie, ZOMBIE_WALK_SPEED, obj_player.pos.x, obj_player.pos.y);
		obj_zombie.pos += obj_zombie.velocity;
	}
	
}
