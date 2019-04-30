//Final Project:Dig for treasure!
//Nov 2017
#include "dig.h"

//The name of window in system
const char g_szClassName[] = "Project D";

HWND hwnd;

//width & height of map block
const int block_wide= wdwidth/64;
const int block_high= wdheight/64;
//width & height of map block
const int blockwide = wdwidth/64;
const int blockhigh = wdheight/64;

//fire container
list<Fire*> fire;
//timer
int timer;

//player
Player gameplayer= Player(20,UP,5*64,11*64,16,5,8);
//Player lifes
bool player_death=false;
bool scoreclean = false;	
int player_num;
int player_time;
const int player_timemax=100;
//map
int wintime = 0;
const int wintimemax=8000;
const int tipmax = 3;
int tipcount = tipmax;
int *map;
int score=0;
char *scorebuffer;	
char timebuffer[100];

//Main Function!
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    
    MSG Msg;
  
    //Register Window
    RegisterMyWindow(hInstance);
    //Create Window
    hwnd =  CreateMyWindow(hInstance, nCmdShow);

	PicProc(hInstance);
		
	MessageBox(hwnd,"\
Dig for treasure!\n\
Made by Zhenkai Han\n\
	Xunkai Chen\n\
Nov 2017\
","About",MB_OK);
		MessageBox(hwnd, "\
Dig for treasure as much as you can!\n\
DON'T TOUCH MINES!\n\
You only have about 80 seconds!\n\
¡ü¡ý¡û¡ú£º move\n\
Space : DIG!\n\
T key:MAPTIP!(3 times only)\n\
H key:Help\
","Help",MB_OK);

	wdstart();

    //Message loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}

  
//Register Window
void RegisterMyWindow(HINSTANCE hInstance)
{
    WNDCLASSEX wc;  
  
    //Game window setting
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = MyWindowProc; 
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
  
    // 2)Register
    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, TEXT("Register Error!"), TEXT("Error"), MB_ICONEXCLAMATION | MB_OK);
        exit(0);
    }
}

  
//Create Window
HWND CreateMyWindow(HINSTANCE hInstance, int nCmdShow)
{
		HWND hwnd;
	    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,g_szClassName,TEXT("Dig for treasure!"),WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 960+20, 704+40, // width 960 height 704
        NULL, NULL, hInstance, NULL);
  
    if(hwnd == NULL)
    {
        MessageBox(NULL, TEXT("Create Window error!"), TEXT("Error"), MB_ICONEXCLAMATION | MB_OK);
        exit(0); 
    }
  
    //Show window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
  
    return hwnd;
}

//Window process
LRESULT CALLBACK MyWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    switch(msg)
    {
	case WM_ACTIVATE:
		if(wParam!=WA_INACTIVE){
			SetTimer(hwnd, gameid, gametime, NULL);
			
		}
		else
			KillTimer(hwnd,gameid);
			
		break;
		//window minimize 
	case WM_SIZE:
		if(wParam==SIZE_RESTORED)
			SetTimer(hwnd,gameid,gametime,NULL);
		else if(wParam==SIZE_MINIMIZED)
			KillTimer(hwnd,gameid);
		break;
	case WM_TIMER:
		switch(wParam)
		{
		case gameid:				
			//Game state
			switch (game_state)
			{		
			case game:
				Game();
				break;
			case pause:
				MessageBox(hwnd,"Sit back and relax.","Pause",MB_OK);
				game_state=game;
				break;
			case gameover:				
				MessageBox(hwnd,"Game Over. Try again!","Oops£¡",MB_OK);
				scoreclean = true;
				wdstart();
				break;
			}

		}
		break;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case 'H':				
		MessageBox(hwnd, "\
Dig for treasure as much as you can!\n\
DON'T TOUCH MINES!\n\
You only have about 80 seconds!\n\
¡ü¡ý¡û¡ú£º move\n\
Space : DIG!\n\
T key:MAPTIP!(3 times only)\n\
H key:Help\
","Help",MB_OK);			
			break;
		case 'T':
			if (tipcount > 0) {
				tipflash();
				tipcount--;
			}
			else {
				MessageBox(hwnd,"You have no chance to look tip!","Emmm..",MB_OK);	
			}
			
			break;
		case 'P':
			if(game_state==game)				
				game_state=pause;
			break;			
		}
		break;
        // close window
     case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        // destroy window
     case WM_DESTROY:
            PostQuitMessage(0); // Post quit message to System
        break;
        
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void Game()
{
	//Key action
	if(player_death==false)
		Key();
	//Draw game
	DrawGame();
	//player death
	if(player_death==true)
	{
		if(--player_time==0)
		{
			if (player_num == 0) {
			game_state=gameover;
			}	
			else
			{
				//player next
				player_death=false;
				gameplayer= Player(20,UP,5*64,11*64,16,5,8);
			}
		}
	}	
	wintime++;
	if (wintime == wintimemax) {
			MessageBox(hwnd,"Times up!","!!!",MB_OK);	
			game_state=gameover;
		}
	
}

//Key action
void Key()
{
	//player  motion
	if(gameplayer.move==0)
	{	
		//move
		if(KEYDOWN(VK_DOWN))
		{
			gameplayer.Change(DOWN);
			gameplayer.move=64/gameplayer.speed;
		}
		else if(KEYDOWN(VK_LEFT))
		{
			gameplayer.Change(LEFT);
			gameplayer.move=64/gameplayer.speed;
		}
		else if(KEYDOWN(VK_UP))
		{
			gameplayer.Change(UP);
			gameplayer.move=64/gameplayer.speed;
		}
		else if(KEYDOWN(VK_RIGHT))
		{
			gameplayer.Change(RIGHT);
			gameplayer.move=64/gameplayer.speed;
		}
			//if(sound)PLAYB(MOTOR);		
		
	}
	if(gameplayer.move>0)
	{
		--gameplayer.move;
		if (++gameplayer.movestep>2)				
			gameplayer.movestep=0;			
		gameplayer.Move();
		if(gameplayer.move==0)
		{
			gameplayer.x=(gameplayer.x+32)/64*64;
			gameplayer.y=(gameplayer.y+32)/64*64;
		}
	}

	//fire!
	if(KEYDOWN(VK_SPACE)){
		int xblock = gameplayer.x / 64;
		int yblock = (gameplayer.y / 64)+1;
		int blocknum = (yblock - 1) * 15 + xblock;
		int treasure = map[blocknum];
		map[blocknum] = 8;
		switch (treasure) {
		case MINE:
			gameplayer.life -= 20;
			if(gameplayer.life<=0)
				{
					//player die
					//if(sound)PLAYA(BOMB);
					fire.push_back(new Fire(gameplayer.x, gameplayer.y));
					player_death=true;
					--player_num;
					player_time=player_timemax;
				}
			break;
		case EMPTY:
			break;
		case DEGGED:
			break;
		default:
			score = score + treasure;
		}

	}
}

void DrawGame()
{
	//Background drawing
	Drawbkgd();;
	//maptip drawing
	if (tipstate == tipon) {
		Drawtreasure();
	}
	//DrawBlock();
	Drawpit();
	//information drawing
	if (scoreclean == true) {
		scorereset();
	}
	Drawinfo();

	//playerTank process & drawing
	if (player_death == false) {
		gameplayer.Draw(writeDC,gameplayerDC);
	}	
	//Explosion process & drawing
	DrawFire();	
	//Output to screen	
	Print();
}

void tipflash() {
	tipstate = tipon;
	double start;
	for (int i = 3; i >= 0; i--){// i equals to 
		start = (double)clock() / CLOCKS_PER_SEC;//When start showing tipmap,begin the countdown. 
		while ((double)clock() / CLOCKS_PER_SEC - start <= 1) { //update loop once per second
			if (i > 0){		
				DrawGame();
			}
			else {
				tipstate = tipoff;		
			}
		}
	}	
}

void Drawinfo() {
	wsprintf(scorebuffer, "Score :%d",score);
	wsprintf(timebuffer, "Time  :%d",(wintimemax-wintime)/100);

	TextOut(backgroundDC, 0,0, scorebuffer, strlen(scorebuffer));
	TextOut(backgroundDC, 0,15, timebuffer, strlen(timebuffer));

}

void scorereset() {
	BitBlt(backgroundDC, 52, 0,16, 16, blockDC,64,0, SRCCOPY);
}

void DrawFire()
{
	for(list<Fire*>::iterator iter=fire.begin();iter!=fire.end();)
	{
		(*iter)->Draw(writeDC,fireDC);
		bool erase=false;
		if((*iter)->end==true)
		{
			delete (*iter);
			iter=fire.erase(iter);
			erase=true;
		}
		if(erase==false)++iter;
	}
}


//draw background
void Drawbkgd()
{
    BitBlt(writeDC,0,0,wdwidth,wdheight,backgroundDC,0,0,SRCCOPY);
}

//output writing to screen
void Print()	
{
	BitBlt(winDC,0,0,wdwidth,wdheight,writeDC,0,0,SRCCOPY);
}

//Pictures loading & setting
void PicProc(HINSTANCE hInstance) {
	//PAINTSTRUCT score;

	//Get window DC
	winDC=GetDC(hwnd);

	//Apply memory space for writeDC in WinDC
	writeDC=CreateCompatibleDC(winDC);
	//Apply memory space for backgroundDC in WinDC
	backgroundDC=CreateCompatibleDC(winDC);
	//Create memory for block
	blockDC=CreateCompatibleDC(winDC);
	//Create memory for fire
	fireDC=CreateCompatibleDC(winDC);
	//Create memory for gameplayer
	gameplayerDC=CreateCompatibleDC(winDC);

	//Create BMP for write
	writeBMP=CreateCompatibleBitmap(winDC,wdwidth,wdheight);
	//load background BMP from resource file(Get BMP(pigment)!)
	backgroundBMP=(HBITMAP)LoadImage(hInstance,MAKEINTRESOURCE(bkgd),IMAGE_BITMAP,wdwidth,wdheight,LR_DEFAULTCOLOR);
	//load player BMP from resource file
	gameplayerBMP=(HBITMAP)LoadImage(hInstance,MAKEINTRESOURCE(player),IMAGE_BITMAP,256,10280,LR_DEFAULTCOLOR);
	//load block BMP from resource file
	blockBMP=(HBITMAP)LoadImage(hInstance,MAKEINTRESOURCE(mapblock),IMAGE_BITMAP,256,256,LR_DEFAULTCOLOR);
	//load fire BMP from resource file
	fireBMP=(HBITMAP)LoadImage(hInstance,MAKEINTRESOURCE(boom),IMAGE_BITMAP,528,132,LR_DEFAULTCOLOR);	

	//load BMP to memory space
	SelectObject(writeDC,writeBMP);
	SelectObject(backgroundDC,backgroundBMP);
	SelectObject(gameplayerDC,gameplayerBMP);
	SelectObject(blockDC,blockBMP);
	SelectObject(fireDC,fireBMP);

	
	
}


//Game start
void wdstart()
{
	
	MapInit();
	SetTimer(hwnd,gameid,gametime,NULL);
	player_num = 1;
	gameplayer.life = 20;
	game_state=game;
	gamestart();
	
	
}

void gamestart() {
	char s[100];
	if(game_state==game)	
	{
		sprintf_s(s,"Be PREPARED for the map tip!");		
	}
	MessageBox(hwnd,s,"",MB_OK);
	
	scorebuffer = new char[100];
	score = 0;
	tipflash();
	gameplayer = Player(gameplayer.life,UP,5*64,11*64,64/gameplayer.speed,gameplayer.speed,gameplayer.digspeed);
	wintime = 0;

	//player relive
	gameplayer.life = 20;
	player_death=false;
	player_num = 1;
	player_time=0;
	scoreclean = false;
	
}


//Map initialization
void MapInit()
{	
	map = new int[11 * 15];
	//random seed
	srand((unsigned)time(0));
	int minemax = 10;
	int mineblock;
	
	//random mines
	for (int i = 0; i < minemax; i++) {
		mineblock = (rand() % (165));
		map[mineblock] = 1;
	}
	//random other block
	for (int j = 0; j < 165; j++) {
		if (map[j] != 1) {
			map[j]=(rand() % (6))+ 2;
		}
	}	

}

void Drawtreasure() {	
	int x;
	int y;
	int x1;
	int y1;

	for (int i = 0; i < 165; i++) {
		x=i%block_wide*64;
		y=i/block_wide*64;
		x1 = ((map[i] - 1) % 4) * 64;
		y1 = ((map[i] - 1) / 4) * 2 * 64;
		BitBlt(writeDC,x,y,64,64,blockDC,x1,y1+64,SRCPAINT);
		BitBlt(writeDC,x,y,64,64,blockDC,x1,y1,SRCAND);

	}
}

void Drawpit() {
	int x;
	int y;
		for (int i = 0; i < 165; i++) {
			if (map[i] == 8) {
				x = i%block_wide * 64;
				y = i / block_wide * 64;				
				if (x == 0 && y == 0) {
					BitBlt(writeDC, x, y+30, 64, 34, blockDC,192,128, SRCCOPY);
				}
				else {
					BitBlt(writeDC, x, y, 64, 64, blockDC,192,128, SRCCOPY);
				}				
			}
	}
}


//Player Class
Player::Player(int lifes,int faces,int xs,int ys,int moves, int speeds, int digspeeds):
		life(lifes),
		face(faces),
		x(xs),
		y(ys),
		move(moves),
		movestep(0),		
		speed(speeds),
		realspeed(speed),
		digspeed (digspeeds)
		{
		}

Player::~Player(void) {}

void Player::Move()
{
	if ( face == DOWN ) y+=realspeed;
	else if ( face == LEFT ) x-=realspeed;
	else if ( face == UP ) y-=realspeed;
	else x+=realspeed;
	if (++movestep>2) movestep=0;
}

void Player::Change(int _face)
{
	face=_face;
	if (++movestep>2) movestep=0;
}

void Player::Draw(HDC &DC1,HDC &DC2)
{
	//Print digger
	BitBlt(DC1,x,y,64,64,DC2,face*64,64,SRCPAINT);
	BitBlt(DC1,x,y,64,64,DC2,face*64,0,SRCAND);

}

Fire::Fire(
	int _x,
	int _y):
x(_x),
	y(_y),
	step(0),
	end(false)
{
}

Fire::~Fire(void){}

void Fire::Draw(HDC& DC1,HDC& DC2)
{
	if(end==false)
	{
		BitBlt(DC1,x,y,66,66,DC2,step/3*66,66,SRCPAINT);
		BitBlt(DC1,x,y,66,66,DC2,step/3*66,0,SRCAND);
		if(++step==24)end=true;
	}
}

