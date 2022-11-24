#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"

#include <chrono>
#include <thread>

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

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

	//not working as intended
	if (Play::KeyPressed(VK_SPACE))
	{
		
		Play::SetSprite(obj_player, "animation", 1.0f); 

		bool shootAnimComplete = Play::IsAnimationComplete(obj_player);
		
		if (shootAnimComplete)
		{
			Play::SetSprite(obj_player, "survivor", 0.0f);
		}

		Play::UpdateGameObject(obj_player);

	}
	
}
