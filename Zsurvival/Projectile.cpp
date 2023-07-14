#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"

void CreateProjectile()
{
	
	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);

	Point2D cursorPos = Play::GetMousePos();

	int id_bullet = Play::CreateGameObject(TYPE_PROJECTILE, obj_player.pos, 2, "bullet");

	float ratioX, ratioY, bulletX, bulletY;

	float hypotenuse = sqrt((abs(obj_player.pos.x - cursorPos.x) * abs(obj_player.pos.x - cursorPos.x)) + (abs(obj_player.pos.y - cursorPos.y) * abs(obj_player.pos.y - cursorPos.y)));
	ratioY = abs(obj_player.pos.y - cursorPos.y) / hypotenuse;
	ratioX = abs(obj_player.pos.x - cursorPos.x) / hypotenuse;

	bulletX = ratioX * 20;
	bulletY = ratioY * 20;

	switch (abs(obj_player.pos.x) - abs(cursorPos.x) > 0) {

	case true:
		Play::GetGameObject(id_bullet).velocity.x = 0 - bulletX;
		break;

	case false:
		Play::GetGameObject(id_bullet).velocity.x = bulletX;
		break;

	}

	switch (abs(obj_player.pos.y) - abs(cursorPos.y) > 0) {

	case true:
		Play::GetGameObject(id_bullet).velocity.y = 0 - bulletY;
		break;

	case false:
		Play::GetGameObject(id_bullet).velocity.y = bulletY;
		break;

	}
}

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
				hasCollided = true;
				UpdateZombieHealth(id_zombie);
				gameState.score += 100;
			}
		}
		
		if (gameState.score < 0)
			gameState.score = 0;
		Play::UpdateGameObject(obj_projectile);
		Play::DrawObjectRotated(obj_projectile);
		if (!Play::IsVisible(obj_projectile) || hasCollided)
			Play::DestroyGameObject(id_projectile);
	}
}
