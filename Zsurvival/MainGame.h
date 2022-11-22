#pragma once

struct GameState
{
	float timer = 0;
	int spriteId = 0;
};


enum GameObjectType
{
	TYPE_NULL = -1,
	TYPE_PLAYER,
	TYPE_ZOMBIE
};

//void UpdatePlayer();
