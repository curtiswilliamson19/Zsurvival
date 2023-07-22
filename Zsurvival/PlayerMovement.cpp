#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"
#include "Projectile.h"

void UpdatePlayerMovement(char leftKey, char rightKey, char upKey, char downKey)
{
	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);
	Point2D CursorPos = Play::GetMousePos();

	Play::PointGameObject(obj_player, 5, CursorPos.x, CursorPos.y);

	//Very basic movement, does not use acceleration & velocity
	//PointGameObject uses the velocity of the object and interferes
	//with this type of movement

	if (Play::KeyDown(upKey))
	{
		obj_player.pos.y -= player.getSpeed();
	}

	if (Play::KeyDown(downKey))
	{
		obj_player.pos.y += player.getSpeed();
	}


	if (Play::KeyDown(leftKey))
	{
		obj_player.pos.x -= player.getSpeed();
	}

	if (Play::KeyDown(rightKey))
	{
		obj_player.pos.x += player.getSpeed();
	}

	if (Play::KeyDown(upKey) || Play::KeyDown(downKey) || Play::KeyDown(leftKey) || Play::KeyDown(rightKey))
	{
		Play::SetSprite(obj_player, "survivor_move", 0.0f);

		obj_player.frame + 0.25;
	}
	else 
	{
		bool playerAnimComplete = Play::IsAnimationComplete(obj_player);

		if (!playerAnimComplete)
		{
			obj_player.animSpeed = 0.0f;
			Play::SetSprite(obj_player, "survivor", 0.0f);
		}
	}

	if (Play::KeyPressed(VK_LBUTTON))
	{
		
		Play::SetSprite(obj_player, "animation", 0.0f); 

		if (player.getAmmo() > 0)
		{
			player.decreaseAmmo(1);

			CreateProjectile();

			Play::PlayAudio("shoot");
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

	bool shootAnimComplete = Play::IsAnimationComplete(obj_player);

	if (!shootAnimComplete)
	{
		obj_player.frame++;
	}
	else
	{
		Play::SetSprite(obj_player, "survivor", 0.0f);
	}
	
}
