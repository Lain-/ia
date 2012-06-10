#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#include "ConstTypes.h"
#include "ConstDungeonSettings.h"

using namespace std;

class Config {
public:
	Config();

	~Config() {
	}
	const string GAME_TITLE;
	const string GAME_VERSION;

	const string TILES_IMAGE_NAME;

	string FONT_IMAGE_NAME;

private:
	const int LOG_X_CELLS_OFFSET;
	const int LOG_Y_CELLS_OFFSET;

public:
	const int LOG_X_CELLS;

	char WALL_SYMBOL_FULL_SQUARE;

	bool SKIP_INTRO_LEVEL;

	bool ALWAYS_ASK_DIRECTION;

	bool AUTO_RELOAD_GUNS;

	int MAINSCREEN_WIDTH, MAINSCREEN_HEIGHT;

	int LOG_WIDTH, LOG_HEIGHT;

private:
	const int CHARACTER_LINES_Y_CELLS_OFFSET;

public:
	const int CHARACTER_LINES_Y_CELLS;
	int CHARACTER_LINES_HEIGHT;

	//int SCREEN_X_CELLS;
	//int SCREEN_Y_CELLS;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

	//int SCREEN_X_CELLS_HALF;
	//int SCREEN_Y_CELLS_HALF;

	const int SCREEN_BPP;
	const int FRAMES_PER_SECOND;
	bool FULLSCREEN;

	const int PLAYER_START_X;
	const int PLAYER_START_Y;

	int KEY_REPEAT_DELAY;
	int KEY_REPEAT_INTERVAL;
	int DELAY_PROJECTILE_DRAW;
	int DELAY_SHOTGUN;
	int DELAY_EXPLOSION;

	bool BOT_PLAYING;

	bool USE_TILE_SET;

private:
	friend class Renderer;
	friend class Art;
	friend class Popup;

	int LOG_X_OFFSET, LOG_Y_OFFSET;
	const int MAINSCREEN_Y_CELLS_OFFSET;
	int MAINSCREEN_Y_OFFSET;


	int CHARACTER_LINES_Y_OFFSET;
	int CELL_WIDTH_TEXT, CELL_HEIGHT_TEXT;
	int CELL_WIDTH_MAP, CELL_HEIGHT_MAP;

	Config& operator=(const Config& other) {
		(void)other;
		return *this;
	}

	void read();
	void trySetVariableFromLine(string line);

};

#endif