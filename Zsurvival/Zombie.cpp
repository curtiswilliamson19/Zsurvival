#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"

#define TOP_LEFT Point2D{0,0}
#define TOP_RIGHT Point2D{1280,0}
#define BOTTOM_LEFT Point2D{0,720}
#define BOTTOM_RIGHT Point2D{1280,720}

//Zombie class

//constructors
Zombie::Zombie()
{
	this->health = 100;
	this->speed = 2.0f;
	this->id = -1;
}


Zombie::Zombie(int id, int health)
{
	this->health = health;
	this->speed = 2.0f;
	this->id = id;
}

Zombie::Zombie(int health, float speed)
{
	this->health = health;
	this->speed = speed;
}

//getters
int Zombie::getHealth()
{
	return this->health;
}

float Zombie::getSpeed()
{
	return this->speed;
}

int Zombie::getId()
{
	return this->id;
}

//setters
void Zombie::setHealth(int health)
{
	this->health = health;
}

void Zombie::setSpeed(float speed)
{
	this->speed = speed;
}

void Zombie::setId(int id)
{
	this->id = id;
}

//other
void Zombie::increaseHealth(int health)
{
	this->health += health;
}

void Zombie::decreaseHealth(int health)
{
	this->health -= health;
}


//Zombie spawn, make sound, handle sprite
void UpdateZombie()
{
	SpawnZombie();

	std::vector<int> vZombies = Play::CollectGameObjectIDsByType(TYPE_ZOMBIE);

	bool hasCollided = false;

	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);

	for (int id : vZombies)
	{
		GameObject& obj_zombie = Play::GetGameObject(id);
		obj_zombie.scale = 0.35f;
		Play::DrawObjectRotated(obj_zombie);

		MakeZombieSound();

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

//Removes zombie from vector and destorys object
void RemoveZombieFromVector(int id)
{
	for (int i = 0; i < gameState.vZombies.size(); i++)
	{
		if (id == gameState.vZombies[i].getId())
		{
			Play::DestroyGameObject(id); //destroy zombie
			gameState.vZombies.erase(gameState.vZombies.begin() + i);
			i--;
		}
	}
}

//Spawn zombies
void SpawnZombie()
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

		int id = Play::CreateGameObject(TYPE_ZOMBIE, spawnPoint, 50, "zombie");

		gameState.vZombies.emplace_back(Zombie(id, 100));
	}
}

//Make a random zombie sound
void MakeZombieSound()
{
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
}