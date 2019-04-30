//Compile only once
#pragma once
//Include winnm.lib, a Windows multimedia library
#pragma comment(lib, "winmm")
//Include Win32 API
#include <windows.h>
//Include time
#include <time.h>
#include <ctime>
//Include string
#include <string>
//Include vector
#include <vector>
#include <list>
//Include MFC resource list
#include "resource.h"
#include <stdlib.h> 

//namespace
using namespace std;

//Game window & map block size
#define wdwidth  960
#define wdheight  704

//Game timer
#define gameid 1
#define gametime 10


//Tip state
#define tipon 1
#define tipoff 0

//Game state
#define game 0
#define pause 1
#define gameover 2

//player direction
#define DOWN 0
#define LEFT 1
#define UP 2
#define RIGHT 3

//map state
#define MINE 1
#define EMPTY 2
#define DEGGED 8


//Compatible Device Context(Container of BMP(paper))
HDC winDC;
HDC writeDC;
HDC backgroundDC;
HDC gameplayerDC;
HDC blockDC;
HDC fireDC;

//Compatible BMP(Pen for printing BMP on HDC!)
HBITMAP backgroundBMP;
HBITMAP writeBMP;
HBITMAP gameplayerBMP;
HBITMAP blockBMP;
HBITMAP fireBMP;

//Register Window
void RegisterMyWindow(HINSTANCE);
//Create Window
HWND CreateMyWindow(HINSTANCE, int);
//Message processing function
LRESULT CALLBACK MyWindowProc(HWND, UINT, WPARAM, LPARAM);

//Pictures loading & setting
void PicProc(HINSTANCE);
//Game start
void wdstart();
//Game loop
void Game();
//Key action
void Key();
//Initialization
void gamestart();
//Game Window drawing
void Print();
//Game drawing
void DrawGame();
//Background drawing
void Drawbkgd();
//treasure drawing
void Drawtreasure();
//Explosion drawing
void DrawFire();
//pit drawing
void Drawpit();
//info drawing
void Drawinfo();
void scorereset();
//tip flash
void tipflash();

//Initialization
void MapInit();

//game status
int game_state;
//tip state
int tipstate;

//Key map
#define KEYDOWN(key) GetAsyncKeyState(key)&0x8000

//Player class
class Player {
public:
	
	Player(int lifes,int faces,int xs,int ys,int moves, int speeds, int digspeeds) ;
	
	~Player(void);

	void Move();
	void Change(int);
	void Draw(HDC &,HDC &);

	int x, y,face, life, speed, digspeed;
	int realspeed,move;
	int movestep;

	
};

//Fire class
class Fire
{
public:
	Fire(
		int _x=0,
		int _y=0);

	~Fire(void);

	int x;
	int y;
	int step;
	bool end;
	void Draw(HDC &,HDC &);

};