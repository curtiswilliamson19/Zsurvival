#pragma once

class Zombie
{
private:
	int health;
	float speed;
	float scale;
	int id = -1;
public:
	//enum
	enum ZombieType
	{
		STANDARD,
		BIG
	};

	//getters
	int getHealth();
	float getSpeed();
	float getScale();
	int getId();
	Zombie::ZombieType getType();

	//setters
	void setHealth(int health);
	void setSpeed(float speed);
	void setScale(float scale);
	void setId(int id);

	//other
	void increaseHealth(int health);
	void decreaseHealth(int health);
	void setType(Zombie::ZombieType type);

	//constructors
	Zombie();
	Zombie(int id, int health, float speed, float scale, Zombie::ZombieType type);

//enum
private:
	Zombie::ZombieType zombieType;
};

void UpdateZombie();

void RemoveZombieFromVector(int id);

void SpawnZombie();

void MakeZombieSound();
