#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "MainGame.h"


int DISPLAY_WIDTH = 1280;
int DISPLAY_HEIGHT = 720;
int DISPLAY_SCALE = 1;

GameState gameState;
Player player;


// Entry point
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
	Play::CentreAllSpriteOrigins();
	Play::LoadBackground("Data\\Backgrounds\\background.jpg");
}

// Called every frame
bool MainGameUpdate( float elapsedTime )
{
	gameState.timer += elapsedTime;

	Play::DrawBackground();

	UpdateGamePlayState();

	if (Play::KeyPressed(VK_SPACE))
		gameState.spriteId++;

	Play::PresentDrawingBuffer();

	return Play::KeyDown( VK_ESCAPE );
}

// Called once when the player quits
int MainGameExit( void )
{
	Play::DestroyManager();
	return PLAY_OK;
}
