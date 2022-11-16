#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"
#include "Player.h"

int DISPLAY_WIDTH = 1280;
int DISPLAY_HEIGHT = 720;
int DISPLAY_SCALE = 1;

GameState gameState;


// The entry point for a PlayBuffer program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
	Play::CentreAllSpriteOrigins();
	Play::LoadBackground("Data\\Backgrounds\\background.jpg");

	Play::CreateGameObject(TYPE_PLAYER, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, 50, "survivor");
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{

	gameState.timer += elapsedTime;
	Play::DrawBackground();

	/*Play::DrawDebugText({ 10, 10 },
		Play::GetSpriteName(gameState.spriteId),
		Play::cWhite);*/

	//NEW
	UpdatePlayer();

	if (Play::KeyPressed(VK_SPACE))
		gameState.spriteId++;

	Play::PresentDrawingBuffer();
	return Play::KeyDown( VK_ESCAPE );
}

// Gets called once when the player quits the game 
int MainGameExit( void )
{
	Play::DestroyManager();
	return PLAY_OK;
}
