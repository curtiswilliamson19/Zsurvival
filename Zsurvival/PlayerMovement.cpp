#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"
#include "Projectile.h"

//Handles player movement, shooting, and the different sprites and animations that relate to this
void UpdatePlayerMovement(char leftKey, char rightKey, char upKey, char downKey)
{
	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);
	GameObject& obj_player_legs = Play::GetGameObjectByType(TYPE_PLAYER_LEGS);
	Point2D CursorPos = Play::GetMousePos();

	Play::PointGameObject(obj_player, 0, CursorPos.x, CursorPos.y);
	Play::PointGameObject(obj_player_legs, 0, CursorPos.x, CursorPos.y);

	bool playerIsMoving = false;
	bool playerShot = false;

	//Very basic movement, does not use acceleration & velocity
	//PointGameObject uses the velocity of the object and interferes
	//with this type of movement

	if (Play::KeyDown(upKey))
	{
		obj_player.pos.y -= player.getSpeed();
		obj_player_legs.pos.y -= player.getSpeed();
	}

	if (Play::KeyDown(downKey))
	{
		obj_player.pos.y += player.getSpeed();
		obj_player_legs.pos.y += player.getSpeed();
	}


	if (Play::KeyDown(leftKey))
	{
		obj_player.pos.x -= player.getSpeed();
		obj_player_legs.pos.x -= player.getSpeed();
	}

	if (Play::KeyDown(rightKey))
	{
		obj_player.pos.x += player.getSpeed();
		obj_player_legs.pos.x += player.getSpeed();
	}

	if (Play::KeyDown(upKey) || Play::KeyDown(downKey) || Play::KeyDown(leftKey) || Play::KeyDown(rightKey))
	{
		playerIsMoving = true;
	}
	else
	{
		playerIsMoving = false;
	}

	if (playerIsMoving)
	{
		Play::SetSprite(obj_player_legs, "survivor_move", 0.5f);
	}
	else if (!playerIsMoving)
	{
		bool playerAnimComplete = Play::IsAnimationComplete(obj_player);

		if (playerAnimComplete)
		{
			obj_player.animSpeed = 0.0f;
			Play::SetSprite(obj_player_legs, "survivor_idle", 0.0f);
		}
	}

	if (Play::KeyPressed(VK_LBUTTON))
	{
		
		if (player.getAmmo() > 0)
		{
			playerShot = true;
		}
		else
		{
			if (player.getAmmo() < 0)
			{
				player.setAmmo(0);
			}
			Play::PlayAudio("click");
		}
	}

	if (playerShot)
	{
		Play::SetSprite(obj_player, "animation", 0.2f);

		player.decreaseAmmo(1);

		//This can be called to demonstrate working of more complex projectile function, ensure to comment out CreateProjectileBasic() if doing so
		//CreateProjectile();

		CreateProjectileBasic();

		Play::PlayAudio("shoot");

		playerShot = false;
	}

	bool shootAnimComplete = Play::IsAnimationComplete(obj_player);

	if (shootAnimComplete)
	{
		obj_player.animSpeed = 0.0f;
		Play::SetSprite(obj_player, "survivor", 0.0f);
	}

	Play::UpdateGameObject(obj_player_legs);
	Play::UpdateGameObject(obj_player);
}
