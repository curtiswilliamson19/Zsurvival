#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"

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

	if (Play::KeyPressed(VK_SPACE))
	{
		
		obj_player.animSpeed = 0.2f;

		if (Play::IsAnimationComplete(obj_player)) {
			obj_player.animSpeed = 0.f;
		}

		Play::UpdateGameObject(obj_player);

	}
}