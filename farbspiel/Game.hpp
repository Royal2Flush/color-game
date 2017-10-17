#ifndef GAME_HPP
#define GAME_HPP

#include "SDL.h"
#include "AdditionalFunctions.hpp"
#include "Framework.hpp"
#include "Timer.hpp"
#include "Square.hpp"
#include "Text.hpp"

#define ANIMATION_SPEED 4
#define ANIMATION_START_OPACITY 254
#define ANIMATION_END_OPACITY 30
#define HOVER_SCALE 4
//#define LOSE_INLAY_SCALE 50
#define SAVE_FILENAME "stats.rofl"
#define MAX_LEVEL 20
#define DIFF_SQUARES_LEVEL 1
#define TIME_PER_LEVEL 3.0f
#define FONTFILE "Fonts/PontanoSans.ttf"

struct statistics
{
	float timeArray[MAX_LEVEL + DIFF_SQUARES_LEVEL];
	int level;
};

class CGame
{
public:
    /*
     * Konstruktor
     */
	CGame();

	int Init(int screenWidth, int ScreenHeight);
	void Run();
	void Quit();

	/*
     * Größe des gehoverten Quadrats ändern
     */
    void hoverAction( int* hoverID );

    /*
     * Gibt ID des Quadrats zurück, auf dem sich der Mauszeiger befindet
     */
    int getIdFromMousePos();

private:
	void ProcessEvents();
	void ClickEvent(int x, int y);
	void Draw();
	bool LoadData(string filename, statistics* target);
	bool SaveData(string filename, statistics* target);

	//void YouLose();
	int GenerateColor(int rightValue, int* maxDiff);
	int GenerateThirdColor(int rightValue, int diff);
	void InitStatistics(statistics* target);
	void CheckHighscore();
	void changeGUITextcolor(int r, int g, int b);
	void RenderGUI();
	void PositionTimer();

	SDL_Renderer *m_pRenderer;

	CSquare refSquare;
	CSquare m_squareList[MAX_LEVEL + DIFF_SQUARES_LEVEL];

	CText m_UILevel;
	CText m_LabelLevel;
	CText m_UIBestLevel;
	CText m_LabelBestLevel;
	CText m_UITimerSeconds;
	CText m_UITimerDot;
	CText m_UITimerTenth;
	CText m_LabelTimer;
	CText m_UITimerSecondsBest;
	CText m_UITimerDotBest;
	CText m_UITimerTenthBest;
	CText m_LabelBestTime;
	CText m_LabelTutorial;
	CText m_LabelWinState;
	CText m_UIEndXAxis;
	CText m_UIEndYAxis;

	int r;
	int g;
	int b;

	enum {play, loseWrongChoice, loseTimeout, win, statistic} gamestate;
	bool m_bRunning;
	bool m_bLostByTimeout;
	/*bool m_bLoseWrongChoice;
	bool m_bLoseTimeout;
	bool m_bWinState;*/
	int m_Level;
	int m_sWidth;
	int m_sHeight;
	int m_maxRows;
	int m_graphLowerBond;
	int m_graphUpperBond;
	int m_graphLeftBond;
	int m_graphRightBond;
	float m_fElapsedTime;
	float m_fOverTime;
	float m_fLevelTime;
	bool m_bTextColorSet;
	bool m_bNeedNewSquares;
	int m_size;
	int m_distance;
	int m_clickedElementID;
	int rightOne;
	int opacityCounter;

	statistics m_currentRun;
	statistics m_bestRun;
};

#endif
