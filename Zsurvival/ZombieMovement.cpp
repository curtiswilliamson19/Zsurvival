#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"

constexpr float ZOMBIE_WALK_SPEED = 2.0f;
constexpr float ZOMBIE_BIG_WALK_SPEED = 1.0f;

//Handles zombie movement, follow player
void UpdateZombieMovement() 
{
	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);

	std::vector<int> vZombies = Play::CollectGameObjectIDsByType(TYPE_ZOMBIE);

	for (int id : vZombies)
	{
		GameObject& obj_zombie = Play::GetGameObject(id);

		for (int i = 0; i < gameState.vZombies.size(); i++)
		{
			if (id == gameState.vZombies[i].getId())
			{
				Play::PointGameObject(obj_zombie, gameState.vZombies[i].getSpeed(), obj_player.pos.x, obj_player.pos.y);
			}
		}
		obj_zombie.pos += obj_zombie.velocity;
	}
}
