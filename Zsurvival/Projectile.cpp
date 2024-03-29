#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"
#include "Projectile.h"

//All of this can be replaced with a much more basic function, CreateProjectileBasic - This is a demonstration of creating a function to do this.
void CreateProjectile()
{
	
	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);

	Point2D cursorPos = Play::GetMousePos();

	int id_bullet = Play::CreateGameObject(TYPE_PROJECTILE, obj_player.pos, 2, "bullet");

	float ratioX, ratioY, bulletX, bulletY;

	//the distance between two points in the X axis = Adjacent side of a triangle. 
	//the distance between two points in the Y axis = Opposite side of a triangle.
	
	//the hypotenuse is the shortest distance between the two points
	// 
	//this hypotenuse is the distance between the 2 points 
	float hypotenuse = sqrt((abs(obj_player.pos.x - cursorPos.x) * abs(obj_player.pos.x - cursorPos.x)) + (abs(obj_player.pos.y - cursorPos.y) * abs(obj_player.pos.y - cursorPos.y))); 
	ratioY = abs(obj_player.pos.y - cursorPos.y) / hypotenuse; //opposite / hypotenuse - SOH
	ratioX = abs(obj_player.pos.x - cursorPos.x) / hypotenuse; //adjacent / hypotenuse - CAH
	//x = cos, y=sin
	bulletX = ratioX * 20; //this essentially sets the speed of the bullet, 20 is speed
	bulletY = ratioY * 20;

	switch (abs(obj_player.pos.x) - abs(cursorPos.x) > 0) { 

	case true:
		Play::GetGameObject(id_bullet).velocity.x = 0 - bulletX; //move left
		break;

	case false:
		Play::GetGameObject(id_bullet).velocity.x = bulletX; //move right
		break;

	}

	switch (abs(obj_player.pos.y) - abs(cursorPos.y) > 0) {

	case true:
		Play::GetGameObject(id_bullet).velocity.y = 0 - bulletY; //move up
		break;

	case false:
		Play::GetGameObject(id_bullet).velocity.y = bulletY; //move down
		break;

	}
}

//Basic version of projectile function using PlayBuffer
void CreateProjectileBasic()
{

	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);

	Point2D cursorPos = Play::GetMousePos();

	int id_bullet = Play::CreateGameObject(TYPE_PROJECTILE, obj_player.pos, 2, "bullet");

	Play::PointGameObject(Play::GetGameObject(id_bullet), 20, cursorPos.x, cursorPos.y);
}

//Handles projectiles, collision with zombies, destruction of projectile
void UpdateProjectile()
{
	std::vector<int> vProjectile = Play::CollectGameObjectIDsByType(TYPE_PROJECTILE);
	std::vector<int> vZombies = Play::CollectGameObjectIDsByType(TYPE_ZOMBIE);
	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);

	for (int id_projectile : vProjectile)
	{
		GameObject& obj_projectile = Play::GetGameObject(id_projectile);
		obj_projectile.rotation = obj_player.rotation;
		bool hasCollided = false;
		for (int id_zombie : vZombies)
		{
			GameObject& obj_zombie = Play::GetGameObject(id_zombie);
			if (Play::IsColliding(obj_projectile, obj_zombie))
			{
				obj_projectile.velocity = { 0, 0 };
				int bloodID = Play::CreateGameObject(TYPE_BLOOD, obj_projectile.pos, 0, "blood_6");
				Play::SetSprite(Play::GetGameObject(bloodID), "blood_6", 0.5f);
				hasCollided = true;
				UpdateZombieHealth(id_zombie);
			}
		}
		
		Play::UpdateGameObject(obj_projectile);
		Play::DrawObjectRotated(obj_projectile);
		if (!Play::IsVisible(obj_projectile) || hasCollided)
			Play::DestroyGameObject(id_projectile);
	}
}


//this moves the origin of the bullet sprite so that the bullet comes from the players gun
void MoveProjectileSpriteOrigin()
{
	Play::MoveSpriteOrigin("bullet", -20, 0);
}
