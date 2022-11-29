#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"
#include "Projectile.h"

constexpr float PLAYER_WALK_SPEED = 3.5f;

void UpdatePlayerMovement(char leftKey, char rightKey, char upKey, char downKey)
{
	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);
	Point2D CursorPos = Play::GetMousePos();

	Play::PointGameObject(obj_player, 5, CursorPos.x, CursorPos.y);

	if (Play::KeyDown(upKey))
	{
		obj_player.pos.y -= PLAYER_WALK_SPEED;
	}

	if (Play::KeyDown(downKey))
	{
		obj_player.pos.y += PLAYER_WALK_SPEED;
	}


	if (Play::KeyDown(leftKey))
	{
		obj_player.pos.x -= PLAYER_WALK_SPEED;
	}

	if (Play::KeyDown(rightKey))
	{
		obj_player.pos.x += PLAYER_WALK_SPEED;
	}

	if (Play::KeyDown(upKey) || Play::KeyDown(downKey) || Play::KeyDown(leftKey) || Play::KeyDown(rightKey))
	{
		
		Play::SetSprite(obj_player, "survivor_move", 0.0f);

		obj_player.frame + 0.25;
		
		
		//Play::UpdateGameObject(obj_player);
	
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

	//not working as intended
	if (Play::KeyPressed(VK_SPACE))
	{
		
		Play::SetSprite(obj_player, "animation", 0.0f); 

		if (gameState.ammo > 0) 
		{
			gameState.ammo--;

			CreateProjectile();
		}
		else
		{
			if (gameState.ammo < 0)
			{
				gameState.ammo = 0;
			}

			Play::PlayAudio("tool");
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
	


	//Play::SetSprite(obj_player, "animation", 1.0f);

	//bool shootAnimComplete = Play::IsAnimationComplete(obj_player);

	////obj_player.animSpeed = 1.0f;

	//if (shootAnimComplete)
	//{
	//	Play::SetSprite(obj_player, "survivor", 0.0f);
	//}

	//Play::UpdateGameObject(obj_player);
}
