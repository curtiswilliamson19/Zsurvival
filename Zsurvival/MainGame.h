#pragma once

enum PlayState
{
	STATE_START = 0,
	STATE_APPEAR,
	STATE_PLAY,
	STATE_DEAD,
	STATE_WAIT,
	STATE_WIN
};

struct GameState
{
	float timer = 0;
	int spriteId = 0;
	int score = 0;
	PlayState playState = STATE_START;
};



enum GameObjectType
{
	TYPE_NULL = -1,
	TYPE_PLAYER,
	TYPE_ZOMBIE
};

extern int DISPLAY_WIDTH;
extern int DISPLAY_HEIGHT;
extern int DISPLAY_SCALE;

extern GameState gameState;

void UpdateGamePlayState();
void UpdatePlayer();
void UpdatePlayerMovement(char leftKey, char rightKey, char upKey, char downKey);
void UpdateZombie();
void UpdateZombieMovement();
