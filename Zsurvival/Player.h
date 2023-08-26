#pragma once
class Player
{
private:
	int health;
	float speed;
	int ammo;
public:
	//constructors
	Player();
	Player(int health, float speed, int ammo);

	//getters
	int getHealth();
	float getSpeed();
	int getAmmo();

	//setters
	void setHealth(int health);
	void setSpeed(float speed);
	void setAmmo(int ammo);

	//other
	void increaseAmmo(int ammo);
	void decreaseAmmo(int ammo);
};

void UpdatePlayer();

void MoveLegsSpriteOrigin();
