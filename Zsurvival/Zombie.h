#pragma once

class Zombie
{
private:
	int health;
	float speed;
	int id = -1;
public:
	//constructors
	Zombie();
	Zombie(int id);
	Zombie(int health, float speed);

	//getters
	int getHealth();
	float getSpeed();
	int getId();

	//setters
	void setHealth(int health);
	void setSpeed(float speed);
	void setId(int id);

	//other
	void increaseHealth(int health);
	void decreaseHealth(int health);
};

void UpdateZombie();

void RemoveZombieFromVector(int id);

void SpawnZombie();
