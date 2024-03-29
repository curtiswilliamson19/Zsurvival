#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"

//Randomly spawns ammo somewhere on screen, increases player ammo if collides with ammo box
void UpdateAmmo()
{

	if (Play::RandomRoll(400) == 1)
	{
		int randX = Play::RandomRollRange(100, 1000);
		int randY = Play::RandomRollRange(100, 500);

		Point2D spawnPoint = {randX, randY};
		
		Play::CreateGameObject(TYPE_AMMO, spawnPoint, 50, "ammo");	
	}

	std::vector<int> vAmmo = Play::CollectGameObjectIDsByType(TYPE_AMMO);
	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);
	bool hasCollided = false;

	for (int id : vAmmo)
	{
		GameObject& obj_ammo = Play::GetGameObject(id);
		Play::DrawObjectRotated(obj_ammo);

		if (Play::IsColliding(obj_player, obj_ammo))
		{
			hasCollided = true;
			player.increaseAmmo(3);
			Play::PlayAudio("reload");
			Play::DestroyGameObject(id);
		}

		Play::UpdateGameObject(obj_ammo);
	}
	
}
