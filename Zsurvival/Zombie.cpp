#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"

#define TOP_LEFT Point2D{0,0}
#define TOP_RIGHT Point2D{1280,0}
#define BOTTOM_LEFT Point2D{0,720}
#define BOTTOM_RIGHT Point2D{1280,720}

void UpdateZombie()
{
	//random roll for zombie spawn point
	if (Play::RandomRoll(150) == 1)
	{
		int rand = Play::RandomRollRange(0, 3);

		Point2D spawnPoint;

		switch (rand)
		{
		case 0:
			spawnPoint = TOP_LEFT;
			break;
		case 1:
			spawnPoint = TOP_RIGHT;
			break;
		case 2:
			spawnPoint = BOTTOM_LEFT;
			break;
		case 3:
			spawnPoint = BOTTOM_RIGHT;
			break;
		}

		Play::CreateGameObject(TYPE_ZOMBIE, spawnPoint, 50, "zombie");
	}

	std::vector<int> vZombies = Play::CollectGameObjectIDsByType(TYPE_ZOMBIE);

	bool hasCollided = false;

	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);

	for (int id : vZombies)
	{
		GameObject& obj_zombie = Play::GetGameObject(id);
		obj_zombie.scale = 0.35f;
		Play::DrawObjectRotated(obj_zombie);

		//random roll for zombie audio
		if (Play::RandomRoll(150) == 1)
		{
			int rand = Play::RandomRollRange(0, 2);

			std::string audioString;

			switch (rand)
			{
			case 0:
				Play::PlayAudio("zombie1");
				break;
			case 1:
				Play::PlayAudio("zombie2");
				break;
			case 2:
				Play::PlayAudio("zombie3");
				break;
			}
		}

		//Gets X and Y position of zombie and player

		Point2D playerPos = obj_player.pos;
		Point2D zombiePos = obj_zombie.pos;

		float playerX = playerPos.x;
		float playerY = playerPos.y;

		float zombieX = zombiePos.x;
		float zombieY = zombiePos.y;

		//Gets difference in distance between X and Y
		float diffX = playerX - zombieX;
		float diffY = playerY - zombieY;

		//Uses pythagoras theorem to work out the distance between the zombie and player

		float distance = sqrt((diffX * diffX) + (diffY * diffY));

		if (distance < 150)
		{
			Play::SetSprite(obj_zombie, "zombie_attack", 0.1f);
		}
		else if (distance > 150)
		{
			Play::SetSprite(obj_zombie, "walk", 0.2f);

			bool zombieAnimComplete = Play::IsAnimationComplete(obj_zombie);

			if (zombieAnimComplete)
			{
				obj_zombie.animSpeed = 0.0f;
				Play::SetSprite(obj_zombie, "zombie", 0.0f);
			}
		}
		
		if (Play::IsColliding(obj_player, obj_zombie))
		{
			hasCollided = true;
			
			gameState.playState = STATE_DEAD;
		}

		Play::UpdateGameObject(obj_zombie);
	}
	
}
