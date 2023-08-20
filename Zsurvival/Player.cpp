#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"
#include "Player.h"

//constructors
Player::Player()
{
	this->health = 100;
	this->speed = 3.5f;
	this->ammo = 10;
}

Player::Player(int health, float speed, int ammo)
{
	this->health = health;
	this->speed = speed;
	this->ammo = ammo;
}

//getters
int Player::getHealth()
{
	return this->health;
}

float Player::getSpeed()
{
	return this->speed;
}

int Player::getAmmo()
{
	return this->ammo;
}

//setters
void Player::setHealth(int health)
{
	this->health = health;
}

void Player::setSpeed(float speed)
{
	this->speed = speed;
}

void Player::setAmmo(int ammo)
{
	this->ammo = ammo;
}

//increase ammo by specified amount
void Player::increaseAmmo(int ammo)
{
	this->ammo += ammo;
}

//decrease ammo by specified amount
void Player::decreaseAmmo(int ammo)
{
	this->ammo -= ammo;
}

//other
void UpdatePlayer() 
{
	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);
	obj_player.scale = 0.35f;
	obj_player.radius = 1.0f;
	Play::DrawObjectRotated(obj_player);
}

//this moves the origin of the player sprite so that the bullet comes from the players gun
//to the right and up
void MovePlayerSpriteOrigin()
{
	Play::MoveSpriteOrigin("survivor", 55, -35);
	Play::MoveSpriteOrigin("animation", 55, -35);
	Play::MoveSpriteOrigin("survivor_move", 55, -35);
}
