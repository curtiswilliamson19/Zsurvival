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
}


void UpdateTank(GameObject& obj_tank, char leftKey, char rightKey, char forwardKey, char backKey)
{

	float maxVelocityTank = 4.0f;

	obj_tank.velocity *= 0.93f;


	if (Play::KeyDown(VK_UP))
	{
		obj_tank.rotation -= 0.05f;

	}

	if (Play::KeyDown(VK_DOWN))
	{
		obj_tank.rotation += 0.05f;
	}


	if (Play::KeyDown(VK_LEFT))
	{

		//obj_car.velocity *= 0.99f;
		float speed = obj_tank.velocity.Length();

		if (speed < maxVelocityTank)
		{
			obj_tank.velocity.x += sin(obj_tank.rotation);
			obj_tank.velocity.y -= cos(obj_tank.rotation);
		}

	}

	if (Play::KeyDown(VK_RIGHT))
	{

		//obj_car.velocity *= 0.99f;
		float speed = obj_tank.velocity.Length();

		if (speed < maxVelocityTank)
		{
			obj_tank.velocity.x -= sin(obj_tank.rotation);
			obj_tank.velocity.y += cos(obj_tank.rotation);
		}

	}

}
