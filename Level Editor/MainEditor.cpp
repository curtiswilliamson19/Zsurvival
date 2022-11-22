////////////////////////////////////////////////////////////////////////////////////
// A simple platform game editor for Brake Pass using the PlayBuffer framework.
// Copyright 2020 Sumo Digital Limited
///////////////////////////////////////////////////////////////////////////////////
//
// Note that this editor loads all its sprites from the Baamageddon project 
// directory and modifies the data in Brake Pass\Level.lev to change the level.
// Debugging->Working Directory should be to be set to ..\Brake Pass
//
///////////////////////////////////////////////////////////////////////////////////
#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

constexpr int DISPLAY_WIDTH = 1280;
constexpr int DISPLAY_HEIGHT = 720;
constexpr int DISPLAY_SCALE = 1;

constexpr int SNAP_PIXELS = 32;
constexpr int CAMERA_SPEED = SNAP_PIXELS * 2;

const Point2f HALF_DISPLAY{ DISPLAY_WIDTH / 2.0f, DISPLAY_HEIGHT / 2.0f };


constexpr const char* PLAYER_A_SPR_NAME = "survivor";


enum GameObjectType
{
	TYPE_NOONE = -1,
	TYPE_PLAYER,
	TYPE_ZOMBIE,
	TOTAL_TYPES
};

const char* SPRITE_NAMES[TOTAL_TYPES][3] =
{
	{ PLAYER_A_SPR_NAME },
};

struct EditorState
{
	int score = 0;
	GameObjectType editMode = TYPE_PLAYER;
	Point2f cameraTarget{ 0.0f, 0.0f };
	float zoom = 1.0f;
	int selectedObj = -1;
	Point2f selectedOffset{ 0.0f, 0.0f };
	int saveCooldown = 0;
	int rotation = 0;
	int spriteId = -1;
};

EditorState editorState;

void HandleControls();
void DrawScene();
void DrawGrid( int gridSize );
void DrawUserInterface();
bool PointInsideSpriteBounds( Point2f testPos, GameObject& obj );
void DrawObjectsOfType( GameObjectType type );
void SaveLevel();
void LoadLevel();

//-------------------------------------------------------------------------
// The entry point for a PlayBuffer program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
	Play::CentreAllSpriteOrigins();
	//Play::LoadBackground( "Data\\Backgrounds\\background.png" );
	Play::LoadBackground("Data\\Backgrounds\\spr_background.png"); //here???
	editorState.cameraTarget = HALF_DISPLAY;
	Play::ColourSprite( "64px", Play::cBlack );
	LoadLevel();
}

//-------------------------------------------------------------------------
// Called by the PlayBuffer once for each frame of the game (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	static float fTotalGameTime = 0.f;
	fTotalGameTime += elapsedTime;

	HandleControls();
	DrawScene();
	DrawUserInterface();

	Play::PresentDrawingBuffer();
	return Play::KeyDown( VK_ESCAPE );
}

//-------------------------------------------------------------------------
// Gets called once when the player quits the game 
int MainGameExit( void )
{
	Play::DestroyManager();
	return PLAY_OK;
}

//-------------------------------------------------------------------------
// Camera movement, object placement / deletion etc.
void HandleControls( void )
{
	static bool drag = false;
	static Point2f dragOrigin{ 0.0f, 0.0f };

	if( Play::KeyPressed( 'S' ) && editorState.saveCooldown < 0 )
		SaveLevel();

	if( Play::KeyPressed( VK_RIGHT ) )
		editorState.cameraTarget.x += CAMERA_SPEED / editorState.zoom;

	if( Play::KeyPressed( VK_LEFT ) )
		editorState.cameraTarget.x -= CAMERA_SPEED / editorState.zoom;

	if( Play::KeyPressed( VK_UP ) )
		editorState.cameraTarget.y -= CAMERA_SPEED / editorState.zoom;

	if( Play::KeyPressed( VK_DOWN ) )
		editorState.cameraTarget.y += CAMERA_SPEED / editorState.zoom;

	if( Play::KeyPressed( VK_OEM_MINUS ) )
		editorState.zoom -= 0.1f;

	if( Play::KeyPressed( VK_OEM_PLUS ) )
		editorState.zoom += 0.1f;

	if( editorState.zoom < 0.2f )
		editorState.zoom = 0.2f;

	if( editorState.zoom > 1.0f )
		editorState.zoom = 1.0f;

	if( Play::KeyPressed( VK_OEM_PERIOD ) )
		editorState.rotation++;

	if( Play::KeyPressed( VK_OEM_COMMA ) )
		editorState.rotation--;

	if( editorState.rotation == 4 )
		editorState.rotation = 0;

	if( editorState.rotation == -1 )
		editorState.rotation = 3;


	if( Play::KeyPressed( VK_SPACE ) )
	{
		switch( editorState.editMode )
		{
			case TYPE_PLAYER: editorState.editMode = TYPE_ZOMBIE; break;
		}
		editorState.selectedObj = -1;
		editorState.spriteId = -1;
	}

	Point2f mouseWorldPos = ( Play::GetMousePos() + Play::GetCameraPosition() ) / editorState.zoom;
	Point2f mouseWorldSnapPos = mouseWorldPos;
	mouseWorldSnapPos.x -= (int)mouseWorldSnapPos.x % SNAP_PIXELS;
	mouseWorldSnapPos.y -= (int)mouseWorldSnapPos.y % SNAP_PIXELS;

	if( Play::GetMouseButton( Play::LEFT ) )
	{
		// If we've not currently got an object selected then we check if we've clicked on one
		if( editorState.selectedObj == -1 )
		{
			for( int id : Play::CollectGameObjectIDsByType( editorState.editMode ) )
			{
				GameObject& obj = Play::GetGameObject( id );
				if( PointInsideSpriteBounds( mouseWorldPos, obj ) )
				{
					// Take the properties of the selected object
					editorState.selectedObj = obj.GetId();
					editorState.selectedOffset = obj.pos - mouseWorldSnapPos;
					editorState.spriteId = obj.spriteId;

					// Use rotated versions of the sprite (A-D.png) if they exist
					std::string sSprite = Play::GetSpriteName( obj.spriteId );

					if( sSprite[sSprite.length() - 2] == '-' )
					{
						switch( sSprite[sSprite.length() - 1] )
						{
							case 'A':  editorState.rotation = 0;  break;
							case 'B':  editorState.rotation = 1;  break;
							case 'C':  editorState.rotation = 2;  break;
							case 'D':  editorState.rotation = 3;  break;
						}
					}
				}
			}

			// If we've not clicked on an existing object then we create a new one
			if( editorState.selectedObj == -1 )
			{
				switch( editorState.editMode )
				{
					case TYPE_PLAYER:
						if( Play::CollectGameObjectIDsByType( TYPE_PLAYER ).size() == 0 )
							Play::CreateGameObject( TYPE_PLAYER, mouseWorldSnapPos, 50, SPRITE_NAMES[static_cast<int>( TYPE_PLAYER )][0] );
						else
							Play::GetGameObjectByType( TYPE_PLAYER ).pos = mouseWorldPos;
						break;
					default:

						editorState.selectedObj = Play::CreateGameObject( editorState.editMode, mouseWorldSnapPos, 50, SPRITE_NAMES[static_cast<int>( editorState.editMode )][0] );
						
						// Use the last sprite selected (reset by changing modes)
						if( editorState.spriteId != -1 )
							Play::GetGameObject( editorState.selectedObj ).spriteId = editorState.spriteId;
						
						editorState.selectedOffset = { 0.0f, 0.0f };
;						
						break;
				}
			}

		}
		else // If we have got an object currently selected
		{
			GameObject& obj = Play::GetGameObject( editorState.selectedObj );
			obj.pos = mouseWorldSnapPos + editorState.selectedOffset;

			int index = -1;

			if( Play::KeyDown( '1' ) ) { index = 0; };
			if( Play::KeyDown( '2' ) ) { index = 1; };
			if( Play::KeyDown( '3' ) ) { index = 2; };
			if( Play::KeyDown( '4' ) ) { index = 3; };
			if( Play::KeyDown( '5' ) ) { index = 4; };
			if( Play::KeyDown( '6' ) ) { index = 5; };
			if( Play::KeyDown( '7' ) ) { index = 6; };
			if( Play::KeyDown( '8' ) ) { index = 7; };
			if( Play::KeyDown( '9' ) ) { index = 8; };

			if( index != -1 )
			{
				obj.spriteId = Play::GetSpriteId( SPRITE_NAMES[static_cast<int>( editorState.editMode )][index] );
				editorState.spriteId = obj.spriteId;
			}


			// Use rotated versions of the sprite (A-D.png) if they exist
			std::string sSprite = Play::GetSpriteName( obj.spriteId );

			if( sSprite[sSprite.length() - 2] == '-' )
			{
				sSprite.erase( sSprite.length() - 1, 1 );

				switch( editorState.rotation )
				{
					case 0: sSprite += "A";  break;
					case 1: sSprite += "B";  break;
					case 2: sSprite += "C";  break;
					case 3: sSprite += "D";  break;
				}

				int rotSpriteId = Play::GetSpriteId( sSprite.c_str() );

				if( rotSpriteId != -1 )
					obj.spriteId = rotSpriteId;
			}

		}
	}
	else
	{
		editorState.selectedObj = -1;
	}

	if( Play::GetMouseButton( Play::RIGHT ) )
	{
		for( int id : Play::CollectGameObjectIDsByType( editorState.editMode ) )
		{
			GameObject& obj = Play::GetGameObject( id );
			if( PointInsideSpriteBounds( mouseWorldPos, obj ) )
			{
				if( obj.type != TYPE_PLAYER )
					Play::DestroyGameObject( id );
			}
		}
	}

	Play::SetCameraPosition( ( editorState.cameraTarget * editorState.zoom ) - HALF_DISPLAY );
}
//-------------------------------------------------------------------------
void DrawVisibleFloor( int floorSpriteId )
{
	Play::SetDrawingSpace( Play::SCREEN );

	int gridWidth = Play::GetSpriteWidth( floorSpriteId );
	int gridHeight = Play::GetSpriteWidth( floorSpriteId );

	for( int y = -gridHeight; y < DISPLAY_HEIGHT + (gridHeight*2); y += gridHeight )
	{
		for( int x = -gridWidth; x < DISPLAY_WIDTH + (gridWidth*2); x += gridWidth )
		{
			int offX = x - ((int)Play::cameraPos.x % gridWidth);
			int offY = y - ((int)Play::cameraPos.y % gridHeight);
			Play::DrawSprite( floorSpriteId, { offX, offY }, 0 );
		}
	}

	Play::SetDrawingSpace( Play::WORLD );
}


//-------------------------------------------------------------------------
void DrawScene( void )
{
	Play::ClearDrawingBuffer( Play::cBlack );
	DrawVisibleFloor( Play::GetSpriteId("grass") ); //here???
	DrawObjectsOfType( TYPE_PLAYER );
	DrawObjectsOfType( TYPE_ZOMBIE );


	if( editorState.selectedObj != -1 )
	{
		GameObject& obj = Play::GetGameObject( editorState.selectedObj );
		Point2f origin = Play::GetSpriteOrigin( obj.spriteId );
		Point2f size = { Play::GetSpriteWidth( obj.spriteId ), Play::GetSpriteHeight( obj.spriteId ) };

		// Corners of sprite drawing area
		Point2D p0 = -origin;
		Point2D p2 = { size.width - origin.x, size.height - origin.y };
		Point2D p1 = { p2.x, p0.y };
		Point2D p3 = { p0.x, p2.y };

		Matrix2D mTrans = MatrixRotation( obj.rotation ) * MatrixScale( obj.scale, obj.scale );

		p0 = (mTrans.Transform( p0 ) + obj.pos) * editorState.zoom;
		p1 = (mTrans.Transform( p1 ) + obj.pos) * editorState.zoom;
		p2 = (mTrans.Transform( p2 ) + obj.pos) * editorState.zoom;
		p3 = (mTrans.Transform( p3 ) + obj.pos) * editorState.zoom;

		DrawLine( p0, p1, Play::cWhite );
		DrawLine( p1, p2, Play::cWhite );
		DrawLine( p2, p3, Play::cWhite );
		DrawLine( p3, p0, Play::cWhite );

		std::string s = "X:" + std::to_string( (int)( obj.pos.x + 0.5f ) ) + " / Y:" + std::to_string( (int)( obj.pos.y + 0.5f ) );
		Play::DrawDebugText( ( obj.pos - origin + Point2f( size.x / 2.0f, -10.0f / editorState.zoom ) ) * editorState.zoom, s.c_str(), Play::cWhite );
	}

}

//-------------------------------------------------------------------------
void DrawUserInterface( void )
{
	Play::SetDrawingSpace( Play::SCREEN );
	std::string sMode;
	switch( editorState.editMode )
	{
		case TYPE_PLAYER: sMode = "PLAYER"; break;
		case TYPE_ZOMBIE: sMode = "ZOMBIE"; break;
	}

	Play::DrawRect( { 0, 0 }, { DISPLAY_WIDTH, 50 }, Play::cYellow, true );
	Play::DrawFontText( "64px", "MODE : " + sMode, { DISPLAY_WIDTH / 2, 25 }, Play::CENTRE );
	Play::DrawFontText( "64px", std::to_string( (int)( ( editorState.zoom * 100.0f ) + 0.5f ) ) + "%", { DISPLAY_WIDTH / 6, 25 }, Play::CENTRE );
	Play::DrawFontText( "64px", std::to_string( Play::CollectGameObjectIDsByType( editorState.editMode ).size() ) + " " + sMode, { ( DISPLAY_WIDTH * 5 ) / 6, 25 }, Play::CENTRE );

	Play::DrawDebugText( { 20, DISPLAY_HEIGHT - 20 }, "HOLD 'H' FOR CONTROLS", Play::cBlack, false );

	if( Play::KeyDown( 'H' ) )
	{
		Play::DrawRect( { DISPLAY_WIDTH / 4, DISPLAY_HEIGHT / 4 }, { DISPLAY_WIDTH * 3 / 4, DISPLAY_HEIGHT * 3 / 4 }, Play::cBlack, true );
		Play::DrawRect( { DISPLAY_WIDTH / 4, DISPLAY_HEIGHT / 4 }, { DISPLAY_WIDTH * 3 / 4, DISPLAY_HEIGHT * 3 / 4 }, Play::cMagenta, false );
		int y = ( DISPLAY_HEIGHT / 4 ) + 50;
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y }, "LEVEL EDITOR HELP", Play::cMagenta );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 20 }, "---------------------", Play::cWhite );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 40 }, "NOTE: YOU CAN ONLY INTERACT WITH OBJECTS SPECIFIED BY THE MODE", Play::cWhite );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 40 }, "SPACE BAR = CHANGE OBJECT MODE", Play::cMagenta );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 20 }, "LEFT MOUSE BUTTON = ADD OR SELECT OBJECT", Play::cWhite );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 20 }, "LEFT MOUSE DRAG = MOVE OBJECT", Play::cMagenta );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 20 }, "LEFT MOUSE DRAG AND KEYS 1-9 = CHANGE OBJECT SPRITE", Play::cWhite );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 20 }, "RIGHT MOUSE BUTTON = DELETE OBJECT", Play::cMagenta );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 20 }, "ARROW KEYS = SCROLL", Play::cWhite );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 20 }, "PLUS AND MINUS KEYS = ZOOM IN AND OUT", Play::cMagenta );
		Play::DrawDebugText( { DISPLAY_WIDTH / 2, y += 20 }, "F1 = SHOW DEBUG INFO", Play::cWhite );
	}

	if( --editorState.saveCooldown > 0 )
	{
		Play::DrawRect( { 0, DISPLAY_HEIGHT - 50 }, { DISPLAY_WIDTH, DISPLAY_HEIGHT }, Play::cOrange, true );
		Play::DrawFontText( "64px", "OVERWRITING LEVEL", { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 25 }, Play::CENTRE );
	}

	Play::SetDrawingSpace( Play::WORLD );
}


//-------------------------------------------------------------------------
void DrawObjectsOfType( GameObjectType type )
{
	for( int id : Play::CollectGameObjectIDsByType( type ) )
	{
		GameObject& obj = Play::GetGameObject( id );
		Play::DrawSpriteRotated( obj.spriteId, obj.pos * editorState.zoom, 0, obj.rotation, 1.0f * editorState.zoom );
	}
}

//-------------------------------------------------------------------------
bool PointInsideSpriteBounds( Point2f testPos, GameObject& obj )
{
	Point2f origin = Play::GetSpriteOrigin( obj.spriteId );
	Point2f size = { Play::GetSpriteWidth( obj.spriteId ), Play::GetSpriteHeight( obj.spriteId ) };
	Point2f topLeft = obj.pos - origin;
	Point2f botRight = topLeft + size;
	return testPos.x > topLeft.x && testPos.x < botRight.x&& testPos.y > topLeft.y && testPos.y < botRight.y;
}


//-------------------------------------------------------------------------
// Loads the objects from the Brake Pass\Level.lev file
void LoadLevel( void )
{
	std::ifstream levelfile;
	levelfile.open( "Level.lev" );

	std::string sType, sX, sY, sSprite, sRot;

	std::getline( levelfile, sType );

	while( !levelfile.eof() )
	{
		std::getline( levelfile, sType );
		std::getline( levelfile, sX );
		std::getline( levelfile, sY );
		std::getline( levelfile, sSprite );

		int id = -1;

		if( sType == "TYPE_PLAYER" )
			id = Play::CreateGameObject( TYPE_PLAYER, { std::stof( sX ), std::stof( sY ) }, 50, sSprite.c_str() );

		if( sType == "TYPE_ZOMBIE" )
			id = Play::CreateGameObject( TYPE_ZOMBIE, { std::stof( sX ), std::stof( sY ) }, 0, sSprite.c_str() );

	}

	levelfile.close();
}

//-------------------------------------------------------------------------
// Outputs the objects to the Brake Pass\Level.lev file
void SaveLevel( void )
{
	std::ofstream levelfile;
	levelfile.open( "Level.lev" );

	levelfile << "// This file is auto-generated by the Level Editor - it's not advisable to edit it directly as changes may be overwritten!\n";

	for( int id : Play::CollectAllGameObjectIDs() )
	{
		GameObject& obj = Play::GetGameObject( id );
		switch( obj.type )
		{
			case TYPE_PLAYER:
				levelfile << "TYPE_PLAYER\n";
				break;
			case TYPE_ZOMBIE:
				levelfile << "TYPE_ZOMBIE\n";
				break;
		}
		levelfile << std::to_string( obj.pos.x ) + "f\n" << std::to_string( obj.pos.y ) + "f\n";
		levelfile << Play::GetSpriteName( obj.spriteId ) << "\n";
	}
	
	levelfile.close();

	editorState.saveCooldown = 100;
}