#include "Game.hpp"
#include "Text.hpp"

CGame::CGame()
{
}

int CGame::Init( int screenWidth, int ScreenHeight )
{
    m_pRenderer = g_pFramework->GetScreen();

    m_sWidth = screenWidth;
    m_sHeight = ScreenHeight;
	int effSize = sqrt(ScreenHeight * screenWidth);
    m_size = (int)effSize/7.2; //110
    m_distance = (int)effSize/128; //6
	m_maxRows = floor(screenWidth / (m_size + m_distance));
	m_graphLowerBond = 0.85*m_sHeight;
	m_graphUpperBond = 0.35*m_sHeight;
	m_graphLeftBond = 0.07*m_sWidth;
	m_graphRightBond = 0.85*m_sWidth;

    srand((unsigned)time(NULL));
    m_fElapsedTime = 0;

    m_bRunning = true;
    /*m_bLoseWrongChoice = false;
    m_bLoseTimeout = false;
    m_bWinState = false;*/
	gamestate = play;
	m_bLostByTimeout = false;
    m_bNeedNewSquares = true;
    m_Level = 1;
    m_fLevelTime = TIME_PER_LEVEL;
    m_fOverTime = 0;
    opacityCounter = ANIMATION_START_OPACITY;
    m_clickedElementID = -1;

    InitStatistics(&m_currentRun);
    if (!LoadData(SAVE_FILENAME, &m_bestRun))
    {
        InitStatistics(&m_bestRun);
    }

	int statSize = (int)m_size / 2; //50
	int labelSize = (int)m_size / 7; //15
	int tutorialSize = (int)labelSize * 2; //30

    m_UILevel.Init(FONTFILE, statSize, toString(m_Level), 200, 200, 200, 255);
    m_UILevel.setMiddle((m_sWidth - m_size) * 1 / 6, m_size / 2);
    m_LabelLevel.Init(FONTFILE, labelSize, "Level", 200, 200, 200, 180);
    m_LabelLevel.setMiddle((m_sWidth - m_size) * 1 / 6, m_size / 5 * 4);

	m_LabelBestLevel.Init(FONTFILE, labelSize, "Best", 200, 200, 200, 180);
	m_LabelBestLevel.setMiddle((m_sWidth - m_size) * 2 / 6, m_size / 5 * 4);
	m_UIBestLevel.Init(FONTFILE, statSize, "BEST", 200, 200, 200, 255);
	m_UIBestLevel.setText(toString(m_bestRun.level));
	m_UIBestLevel.setMiddle((m_sWidth - m_size) * 2 / 6, m_size / 2);

    m_UITimerSeconds.Init(FONTFILE, statSize, "S", 200, 200, 200, 255);
	m_UITimerSeconds.setMiddle((m_sWidth - m_size) * 5 / 6, m_size / 2);
	m_UITimerDot.Init(FONTFILE, statSize, ".", 200, 200, 200, 255);
	m_UITimerDot.setMiddle((m_sWidth - m_size) * 5 / 6, m_size / 2);
	m_UITimerTenth.Init(FONTFILE, statSize, "T", 200, 200, 200, 255);
	m_UITimerTenth.setMiddle((m_sWidth - m_size) * 5 / 6, m_size / 2);
	m_LabelTimer.Init(FONTFILE, labelSize, "Time", 200, 200, 200, 180);
    //m_LabelTimer.setMiddle(m_sWidth * 3 / 4 + m_size / 4, m_size / 5 * 4);
	m_LabelTimer.setMiddle((m_sWidth - m_size) * 5 / 6, m_size / 5 * 4);

	m_UITimerSecondsBest.Init(FONTFILE, statSize, "S", 200, 200, 200, 255);
	m_UITimerSecondsBest.setMiddle((m_sWidth - m_size) * 6 / 6, m_size / 2);
	m_UITimerDotBest.Init(FONTFILE, statSize, ".", 200, 200, 200, 255);
	m_UITimerDotBest.setMiddle((m_sWidth - m_size) * 6 / 6, m_size / 2);
	m_UITimerTenthBest.Init(FONTFILE, statSize, "T", 200, 200, 200, 255);
	m_UITimerTenthBest.setMiddle((m_sWidth - m_size) * 6 / 6, m_size / 2);   
	m_LabelBestTime.Init(FONTFILE, labelSize, "Best", 200, 200, 200, 180);
	m_LabelBestTime.setMiddle((m_sWidth - m_size) * 6 / 6, m_size / 5 * 4);

	m_LabelTutorial.Init(FONTFILE, tutorialSize, "Choose the square with the same color as the one above", 200, 200, 200, 255);
	m_LabelTutorial.setMiddle(m_sWidth / 2, m_sHeight / 2 - m_size);
	m_LabelWinState.Init(FONTFILE, statSize, "CONGRATULATIONS", 200, 200, 200, 255);
	m_LabelWinState.setMiddle(m_sWidth / 2, m_sHeight / 5 * 2);

	m_UIEndXAxis.Init(FONTFILE, statSize/2, "Level", 200, 200, 200, 255);
	m_UIEndXAxis.setMiddle(m_graphRightBond + m_UIEndXAxis.getRect().w / 1.7, m_graphLowerBond);
	m_UIEndYAxis.Init(FONTFILE, statSize/2, "Time", 200, 200, 200, 255);
	m_UIEndYAxis.setMiddle(m_graphLeftBond, 0.95*m_graphUpperBond);

	int secondsLeftBest = floor(m_bestRun.timeArray[m_Level - 1]);
	m_UITimerSecondsBest.setText(toString(secondsLeftBest, 0));
	m_UITimerTenthBest.setText(toString(floor((m_bestRun.timeArray[m_Level - 1] - secondsLeftBest) * 10), 0));

    return 0;
}

void CGame::Quit()
{
}

void CGame::Run()
{
    while( m_bRunning )
    {
        ProcessEvents();

		//cout << gamestate;

        g_pFramework->Update();
        g_pFramework->Clear();
        m_fElapsedTime += g_pTimer->GetElapsed();

        if( m_Level > MAX_LEVEL && gamestate == play ) //gewonnen
        {
			gamestate = win;
            CheckHighscore();
        }

        if( gamestate == play )
        {
            m_fLevelTime -= g_pTimer->GetElapsed(); //verbliebene Zeit updaten
            if (m_fLevelTime < 0)//verloren
            {
                m_fLevelTime = 0;
                gamestate = loseTimeout;
				m_bLostByTimeout = true;
                CheckHighscore();
            }
			int secondsLeft = floor(m_fLevelTime);
            m_UITimerSeconds.setText(toString(secondsLeft,0));
			m_UITimerTenth.setText(toString(floor((m_fLevelTime-secondsLeft) * 10),0));
			PositionTimer();
        }

        float fpscap = ( 0.0167f - g_pTimer->GetElapsed()) * 1000; //Auf 60 fps hardlocken; Übrige Zeit warten
        if( fpscap > 0 )
        {
            SDL_Delay( fpscap );
        }

		if (m_bNeedNewSquares && gamestate == play) //neues Level, neue Quadrate
        {
            m_bNeedNewSquares = false;
            if( m_Level > 1 )
            {
                m_fOverTime = m_fLevelTime;
            }

            m_fLevelTime = TIME_PER_LEVEL + m_fOverTime;
            m_bTextColorSet = false;
            m_UILevel.setText( toString( m_Level ) );
            m_UILevel.setMiddle((m_sWidth - m_size) / 2 / 3, m_size / 2);

            //Die Farbe aussuchen
            r = round( rand() % 256 );
            g = round( rand() % 256 );
            b = round( rand() % 256 );

            refSquare.Create( m_sWidth / 2 - m_size / 2, m_distance, m_size, r,
                              g, b, false ); //Referenzquadrat oben in der Mitte

            int numSquares = m_Level + 1;
            rightOne = rand() % numSquares;
            int colorDiff = 300 - 50 * sqrt( m_Level );
            int cols = 0;
            int rows = 0;
            int temp_mod = 0;
            int temp_num = 6;
            int rest = 1;

            //Spalten- und Zeilenzahl generieren
            for( int i = 3; i <= m_maxRows; i++ )
            {
                if( ( (float) numSquares / (float) i ) > 6 ) //Reihen nicht über 6
                {
                    continue;
                }
                if( numSquares == 2 )
                {
                    cols = 2;
                    rows = 1;
                    break;
                }
                if( numSquares % i == 0 )
                {
                    rest = 0;
                    if( abs( ( ( numSquares / i ) - i ) ) <= temp_num ) //möglichst quadratisch und breit
                    {
                        cols = i;
                        rows = numSquares / i;
                        temp_num = rows - cols;
                    }
                }
                else
                {
                    if( ( numSquares % i > temp_mod ) && ( rest == 1 ) ) //kleinster Unterschied der letzten Reihe
                    {
                        temp_mod = numSquares % i;
                        cols = i;
                        rows = ceil( (float) numSquares / (float) i );
                    }
                }
            } //Ende der for-Schleife für Zeilen- und Spaltenzahl

            //Position des 1. Quadrats berechnen
            int xPos = m_sWidth / 2
                    - ( cols * m_size + ( cols - 1 ) * m_distance ) / 2;
            int yPos = m_sHeight / 2
                    - ( ( rows - 1 ) * m_size + ( rows - 2 ) * m_distance ) / 2;

            //Quadrate generieren
            for( int i = 0; i < numSquares; i++ )
            {
                if( i != rightOne )
                {

                    //Farben generieren
                    int diffLeft = colorDiff;
                    int beginWith = rand() % 3;
                    int thisR = 0;
                    int thisG = 0;
                    int thisB = 0;

                    switch( beginWith )
                    {
                        case 0: //rot
                            thisR = GenerateColor( r, &diffLeft );
                            if( ( rand() % 2 ) == 0 ) //grün zuerst
                            {
                                thisG = GenerateColor( g, &diffLeft );
                                thisB = GenerateThirdColor( b, diffLeft );
                            }
                            else //blau zuerst
                            {
                                thisB = GenerateColor( b, &diffLeft );
                                thisG = GenerateThirdColor( g, diffLeft );
                            }
                            break;

                        case 1: //grün
                            thisG = GenerateColor( g, &diffLeft );
                            if( ( rand() % 2 ) == 0 ) //rot zuerst
                            {
                                thisR = GenerateColor( r, &diffLeft );
                                thisB = GenerateThirdColor( b, diffLeft );
                            }
                            else //blau zuerst
                            {
                                thisB = GenerateColor( b, &diffLeft );
                                thisR = GenerateThirdColor( r, diffLeft );
                            }
                            break;

                        case 2:  //blau
                            thisB = GenerateColor( b, &diffLeft );
                            if( ( rand() % 2 ) == 0 ) //grün zuerst
                            {
                                thisG = GenerateColor( g, &diffLeft );
                                thisR = GenerateThirdColor( r, diffLeft );
                            }
                            else //rot zuerst
                            {
                                thisR = GenerateColor( r, &diffLeft );
                                thisG = GenerateThirdColor( g, diffLeft );
                            }
                            break;
                    }
                    m_squareList[i].Create( xPos, yPos, m_size, thisR, thisG,
                                            thisB, false );
                }
                else // i == rightOne
                {
                    m_squareList[i].Create( xPos, yPos, m_size, r, g, b, true );
                }

                //Position fuer NAECHSTES Quadrat generieren
                if( ( i + 1 ) % cols == 0 ) // Ende der Reihe
                {
                    yPos += m_size + m_distance;
                    if( i + cols >= numSquares ) //letzte Reihe
                    {
                        int remaining = numSquares - i - 1;
                        xPos = m_sWidth / 2
                                - (int) ( (float) remaining / 2 * m_size )
                                - (int) ( (float) ( remaining - 1 ) / 2
                                        * m_distance );
                    }
                    else
                    {
                        xPos =
                                m_sWidth / 2
                                        - (int) ( (float) cols / 2 * m_size )
                                        - (int) ( (float) ( cols - 1 ) / 2
                                                * m_distance );
                    }
                }
                else //nicht Ende der Reihe
                {
                    xPos += m_size + m_distance;
                }
            } //Ende der for-Schleife für Quadrate generieren
        } //Ende von if(m_bNeedNewSquares)

        Draw();
        g_pFramework->Render();
    }
}

void CGame::ProcessEvents()
{
    SDL_Event Event;

    if( SDL_PollEvent( &Event ) )
    {
        // Ja, also schauen welches
        switch( Event.type )
        {
            // Beenden?
            case ( SDL_QUIT ):
            {
                m_bRunning = false;

            }
                break;

                // Wurde eine Taste gedrückt?
            case ( SDL_KEYDOWN ):
            {
                switch( Event.key.keysym.sym )
                {
                    case ( SDLK_ESCAPE ):
                    {
                        m_bRunning = false;

                    }
                        break;
                }
            }
                break;
            case ( SDL_MOUSEBUTTONUP ):
            {
                int mouseX, mouseY;
                SDL_GetMouseState( &mouseX, &mouseY );
                ClickEvent( mouseX, mouseY );
            }
                break;
        }
    }
}

void CGame::ClickEvent( int x, int y )
{
    if( gamestate == statistic ) //alles zurück auf Anfang
    {
		gamestate = play;
		m_bLostByTimeout = false;
        m_Level = 1;
        InitStatistics( &m_currentRun );
        opacityCounter = ANIMATION_START_OPACITY;
        for( int j = 0; j < MAX_LEVEL + DIFF_SQUARES_LEVEL; j++ )
        {
            m_squareList[j].Destroy();
        }
        m_bNeedNewSquares = true;
        m_fLevelTime = TIME_PER_LEVEL;
        m_fOverTime = 0;
		int secondsLeftBest = floor(m_bestRun.timeArray[m_Level - 1]);
		m_UITimerSecondsBest.setText(toString(secondsLeftBest, 0));
		m_UITimerTenthBest.setText(toString(floor((m_bestRun.timeArray[m_Level - 1] - secondsLeftBest) * 10), 0));
		PositionTimer();
    }
	else if (gamestate == play)
    {
        for( int i = 0; i < MAX_LEVEL + DIFF_SQUARES_LEVEL; i++ ) //welches Quadrat wurde gedrückt?
        {
            if( m_squareList[i].Exists() ) //nur die Quadrate prüfen, die gerade dabei sind
            {
                SDL_Rect Pos = m_squareList[i].getPos();
                if( x >= Pos.x && x <= Pos.x + Pos.w && y >= Pos.y
                        && y <= Pos.y + Pos.h )
                {
                    m_currentRun.timeArray[m_Level - 1] = m_fLevelTime;
                    if( m_squareList[i].isCorrect() ) //richtiges Quadrat
                    {
                        m_Level++;
						int secondsLeftBest = floor(m_bestRun.timeArray[m_Level - 1]);
						m_UITimerSecondsBest.setText(toString(secondsLeftBest, 0));
						m_UITimerTenthBest.setText(toString(floor((m_bestRun.timeArray[m_Level - 1] - secondsLeftBest) * 10), 0));
                        m_bNeedNewSquares = true;
                        break;
                    }
                    else //falsches Quadrat
                    {
                        m_clickedElementID = i;
                        gamestate = loseWrongChoice;
                        CheckHighscore();
                        break;
                    }
                } //if(x >= ...)
            } //if(m_squareList[i].Exists())
        } //for
    }
	else //gamestate either win or lose
	{
		gamestate = statistic;
	} //else / if(gamestate)
}

void CGame::Draw()
{
    int color = round( ( ( float( r ) + float( g ) + float( b ) ) / 765 ) * 255 );

    if( gamestate != statistic )
    {
        if( color <= 75 )
        {
            SDL_SetRenderDrawColor( m_pRenderer, 200, 200, 200, 255 );
            if( !m_bTextColorSet )
            {
				changeGUITextcolor(50, 50, 50);
                m_bTextColorSet = true;
            }
        }
        else
        {
            SDL_SetRenderDrawColor( m_pRenderer, 50, 50, 50, 255 );
            if( !m_bTextColorSet )
            {
                changeGUITextcolor(200, 200, 200);
                m_bTextColorSet = true;
            }
        }

        SDL_RenderClear( m_pRenderer );

        refSquare.Draw( m_pRenderer );

        if( gamestate != play)
        {
            for( int j = 0; j < MAX_LEVEL + DIFF_SQUARES_LEVEL; j++ )
            {
                if( m_squareList[j].Exists() )
                {
                    if( j != m_clickedElementID || j != rightOne )
                    {
                        m_squareList[j].animate( m_pRenderer, opacityCounter );
                    }
                }
            }
            if( opacityCounter >= ANIMATION_END_OPACITY )
            {
                opacityCounter -= ANIMATION_SPEED;
            }

            if( gamestate == loseWrongChoice )
            {
                m_squareList[m_clickedElementID].Draw( m_pRenderer );

                //INLAY in flasches Quadrat
                SDL_Rect position;
				int loseInlayScale = (int)m_size / 2; //50

                position.x = m_squareList[m_clickedElementID].m_position.x
                        + loseInlayScale / 2;
                position.y = m_squareList[m_clickedElementID].m_position.y
                        + loseInlayScale / 2;
                position.h = m_squareList[m_clickedElementID].m_position.h
                        - loseInlayScale;
                position.w = m_squareList[m_clickedElementID].m_position.w
                        - loseInlayScale;

                SDL_SetRenderDrawColor( m_pRenderer, r, g, b, 255 );
                SDL_RenderFillRect( m_pRenderer, &position );
            }
            m_squareList[rightOne].Draw( m_pRenderer );
        }
        else // if gamestate == play
        {
            for( int i = 0; i < MAX_LEVEL + DIFF_SQUARES_LEVEL; i++ )
            {

                if( m_squareList[i].Exists() )
                {
                    int hoverID = -2;
                    hoverAction( &hoverID );

                    if( hoverID == i )
                    {
                        continue;
                    }
                    m_squareList[i].Draw( m_pRenderer );
                }
            }
        }
		RenderGUI();
    }
    else //if gamestate == statistic
    {
        /*if( color <= 75 )
        {
            SDL_SetRenderDrawColor( m_pRenderer, 200, 200, 200, 255 );
            m_LabelWinState.setColor( 50, 50, 50, 255 );
        }
        else
        {
            SDL_SetRenderDrawColor( m_pRenderer, 50, 50, 50, 255 );
            m_LabelWinState.setColor( 200, 200, 200, 255 );
        }
        SDL_SetRenderDrawColor( m_pRenderer, 50, 50, 50, 255 );
        SDL_RenderClear( m_pRenderer );
		m_UITimerSeconds.Render();
		m_UITimerDot.Render();
		m_UITimerTenth.Render();
        m_LabelTimer.Render();
        m_LabelWinState.Render();*/

		SDL_SetRenderDrawColor(m_pRenderer, 50, 50, 50, 255);
		SDL_RenderClear(m_pRenderer);

		m_UILevel.setColor(0, 255, 0, 255);
		m_LabelLevel.setColor(0, 255, 0, 180);
		m_UIBestLevel.setColor(0, 0, 255, 255);
		m_LabelBestLevel.setColor(0, 0, 255, 180);
		m_UITimerSeconds.setColor(0, 255, 0, 255);
		m_UITimerDot.setColor(0, 255, 0, 255);
		m_UITimerTenth.setColor(0, 255, 0, 255);
		m_LabelTimer.setColor(0, 255, 0, 180);
		m_UITimerSecondsBest.setColor(0, 0, 255, 255);
		m_UITimerDotBest.setColor(0, 0, 255, 255);
		m_UITimerTenthBest.setColor(0, 0, 255, 255);
		m_LabelBestTime.setColor(0, 0, 255, 180);

		RenderGUI();

		float xdiff = (m_graphRightBond - m_graphLeftBond) / MAX_LEVEL;
		float ymax = 0;
		for (int i = 0; i < MAX_LEVEL; i++)
		{
			if (m_bestRun.timeArray[i] > ymax)
				ymax = m_bestRun.timeArray[i];
		}
		for (int i = 0; i < MAX_LEVEL; i++)
		{
			if (m_currentRun.timeArray[i] > ymax)
				ymax = m_currentRun.timeArray[i];
		}
		SDL_Point bestData[MAX_LEVEL];
		SDL_Point currentData[MAX_LEVEL];
		for (int i = 0; i < MAX_LEVEL; i++)
		{
			bestData[i].x = m_graphLeftBond + i*xdiff;
			bestData[i].y = m_graphLowerBond + (m_graphUpperBond - m_graphLowerBond) * m_bestRun.timeArray[i] / ymax;
		}
		for (int i = 0; i < MAX_LEVEL; i++)
		{
			currentData[i].x = m_graphLeftBond + i*xdiff;
			currentData[i].y = m_graphLowerBond + (m_graphUpperBond - m_graphLowerBond) * m_currentRun.timeArray[i] / ymax;
		}

		SDL_SetRenderDrawColor(m_pRenderer, 200, 200, 200, 255);
		SDL_RenderDrawLine(m_pRenderer, m_graphLeftBond, m_graphLowerBond, m_graphRightBond, m_graphLowerBond);
		SDL_RenderDrawLine(m_pRenderer, m_graphLeftBond, m_graphLowerBond, m_graphLeftBond, m_graphUpperBond);
		m_UIEndXAxis.Render();
		m_UIEndYAxis.Render();

		SDL_SetRenderDrawColor(m_pRenderer, 50, 50, 255, 255);
		SDL_RenderDrawLines(m_pRenderer, bestData, MAX_LEVEL);
		SDL_SetRenderDrawColor(m_pRenderer, 0, 255, 0, 255);
		SDL_RenderDrawLines(m_pRenderer, currentData, MAX_LEVEL);
    }
}

int CGame::GenerateColor( int rightValue, int* maxDiff )
{
    float num = rand() / (double) RAND_MAX;
    int diff = round( ( num - 0.5 ) * 2 * *maxDiff );
    *maxDiff -= abs( diff );

    int res = rightValue + diff;
    if( res > 255 || res < 0 ) //zu viel, probiere andere Richtung
    {
        res = rightValue - diff;
        if( res > 255 || res < 0 ) //wohl auch zuviel, setze auf 0 und passe diffLeft an
        {
            res = 0;
            *maxDiff += abs( diff );
        }
    }

    return res;
}

int CGame::GenerateThirdColor( int rightValue, int diff )
{
    int res = rightValue + diff;
    if( res > 255 || res < 0 ) //zu viel, probiere andere Richtung
    {
        res = rightValue - diff;
        if( res > 255 || res < 0 ) //wohl auch zuviel, setze auf 0 und passe diffLeft an
        {
            res = 0;
        }
    }

    return res;
}

void CGame::hoverAction( int* hoverID )
{
    int squareid = getIdFromMousePos();     // ID
    *hoverID = squareid;                    // ID für die Draw Funktion

    // Wenn sich der Mauszeiger auf einem Quadrat befindet

    SDL_Rect position;

    // neue Position und Farbe des Quadrats festlegen
    position.x = m_squareList[squareid].m_position.x + HOVER_SCALE / 2;
    position.y = m_squareList[squareid].m_position.y + HOVER_SCALE / 2;
    position.h = m_squareList[squareid].m_position.h - HOVER_SCALE;
    position.w = m_squareList[squareid].m_position.w - HOVER_SCALE;
    int red = m_squareList[squareid].m_r;
    int green = m_squareList[squareid].m_g;
    int blue = m_squareList[squareid].m_b;

    // Quadrat zeichnen
    SDL_SetRenderDrawColor( m_pRenderer, red, green, blue, 255 );
    SDL_RenderFillRect( m_pRenderer, &position );
}

int CGame::getIdFromMousePos()
{
    int x = 0;                  // X Koordinate
    int y = 0;                  // Y Koordinate
    int squareID = -2;          // -2 bedeutet: kein Quadrat gehovert

    SDL_GetMouseState( &x, &y );  // X und Y Koordinate holen

    // Alle Quadrate durchgehen und auf Übereinstimmung mit Maus Position prüfen
    for( int i = 0; i < MAX_LEVEL + DIFF_SQUARES_LEVEL; i++ )
    {
        if( m_squareList[i].Exists() )   // ?? Überflüssig
        {
            SDL_Rect Pos = m_squareList[i].getPos();
            if( x >= Pos.x && x <= Pos.x + Pos.w && y >= Pos.y
                    && y <= Pos.y + Pos.h )
            {
                squareID = i;
            }
        }
    }
    return squareID;
}

bool CGame::LoadData( string filename, statistics* target )
{
    ifstream file;
    file.open( filename, ios::binary );
    if( file.is_open() )
    {
        file.read( (char*) target, sizeof( *target ) );
        file.close();
        return true;
    }
    else
    {
        cout << "file " << filename << " not found" << endl;
        return false;
    }
}

bool CGame::SaveData( string filename, statistics* target )
{
    ofstream file;
    file.open( filename, ios::binary );
    if( file.is_open() )
    {
        file.write( (char*) target, sizeof( *target ) );
        file.close();
        return true;
    }
    else
    {
        cout << "Couldn't write into file " << filename << endl;
        return false;
    }
}

void CGame::InitStatistics( statistics* target )
{
    target->level = 0;
    for( int i = 0; i < MAX_LEVEL + DIFF_SQUARES_LEVEL; i++ )
    {
        target->timeArray[i] = 0;
    }
}

void CGame::CheckHighscore()
{
    if( m_Level > MAX_LEVEL ) // gewonnen -> Zeit vergleichen
    {
        m_UIBestLevel.setText(toString(MAX_LEVEL));
        m_UIBestLevel.setMiddle((m_sWidth - m_size) / 2 /3 * 2, m_size / 2);

        m_currentRun.level = MAX_LEVEL;
		m_currentRun.timeArray[MAX_LEVEL - 1] = m_fLevelTime;
		cout << "best Time: " << m_bestRun.timeArray[MAX_LEVEL - 1] << endl;
		if (m_currentRun.timeArray[MAX_LEVEL - 1] > m_bestRun.timeArray[MAX_LEVEL - 1L])
        {
            SaveData( SAVE_FILENAME, &m_currentRun );
            m_bestRun = m_currentRun;
        }
        // Anzeige und so
    }
    else // verloren -> Level vergleichen
    {

        m_currentRun.level = m_Level;
        cout << "best Level: " << m_bestRun.level << endl;
        if( m_currentRun.level > m_bestRun.level )
        {
            m_UIBestLevel.setText(toString(m_Level));
            m_UIBestLevel.setMiddle((m_sWidth - m_size) / 2 /3 * 2, m_size / 2);

            SaveData( SAVE_FILENAME, &m_currentRun );
            m_bestRun = m_currentRun;
        }
    }
}


void CGame::changeGUITextcolor(int r, int g, int b)
{
	m_UILevel.setColor(r, g, b, 255);
	m_LabelLevel.setColor(r, g, b, 180);
	m_UIBestLevel.setColor(r, g, b, 255);
	m_LabelBestLevel.setColor(r, g, b, 180);
	m_UITimerSeconds.setColor(r, g, b, 255);
	m_UITimerDot.setColor(r, g, b, 255);
	m_UITimerTenth.setColor(r, g, b, 255);
	m_LabelTimer.setColor(r, g, b, 180);
	m_UITimerSecondsBest.setColor(r, g, b, 255);
	m_UITimerDotBest.setColor(r, g, b, 255);
	m_UITimerTenthBest.setColor(r, g, b, 255);
	m_LabelBestTime.setColor(r, g, b, 180);
	if (m_Level == 1)
	{
		m_LabelTutorial.setColor(r, g, b, 255);
	}
}


void CGame::RenderGUI()
{
	m_UILevel.Render();
	m_LabelLevel.Render();
	m_UIBestLevel.Render();
	m_LabelBestLevel.Render();
	m_UITimerSeconds.Render();
	m_UITimerDot.Render();
	m_UITimerTenth.Render();
	m_LabelTimer.Render();
	m_UITimerSecondsBest.Render();
	m_UITimerDotBest.Render();
	m_UITimerTenthBest.Render();
	m_LabelBestTime.Render();
	if (m_Level == 1)
	{
		m_LabelTutorial.Render();
	}
}


void CGame::PositionTimer()
{
	m_UITimerSeconds.setMiddle((m_sWidth - m_size) * 5 / 6 - m_UITimerDot.getRect().w / 2 - m_UITimerSeconds.getRect().w / 2, m_size / 2);
	m_UITimerTenth.setMiddle((m_sWidth - m_size) * 5 / 6 + m_UITimerDot.getRect().w / 2 + m_UITimerTenth.getRect().w / 2, m_size / 2);
	m_UITimerSecondsBest.setMiddle((m_sWidth - m_size) * 6 / 6 - m_UITimerDotBest.getRect().w / 2 - m_UITimerSecondsBest.getRect().w / 2, m_size / 2);
	m_UITimerTenthBest.setMiddle((m_sWidth - m_size) * 6 / 6 + m_UITimerDotBest.getRect().w / 2 + m_UITimerTenthBest.getRect().w / 2, m_size / 2);
}