#include "Application.h"
#include "Logger.h"
#include "Utils.h"
#include "Collisions.h"
#include "LevelGenerator.h"
#include "Options.h"

Application* Application::instance = NULL;

/* Constructor */
Application::Application()
{
	player = NULL;
    cpDeath = 0;
	appWindow = NULL;
	appRender = NULL;
}

/* Get the instance of the application */
Application* Application::GetInstance(void)
{
    if (!instance)
        instance = new Application();
	return instance;
}

/* Destroy the current instance */
void Application::DestroyInstance(void)
{
    delete instance;
    instance = NULL;
}

/* Initialize the size and position of the window */
bool Application::InitWindow(void)
{
    appWindow = SDL_CreateWindow("Super Bernard Land",
                                 200, 200, WINDOWX, WINDOWY,
                                 SDL_WINDOW_SHOWN);
    appRender = SDL_CreateRenderer(appWindow, -1,
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    this->isStarted = false;
    logging("Window and renderer initiated");
    return true;
}

void Application::ShowMenu(void)
{
    if (!isStarted)
        MENU->PressStartMenu();
    MENU->Show();
}

/*
charge l'image du niveau et init
charge player et init
charge le bg
charge le sol
charge les box "?"s
*/
void Application::Start(void)
{
    if (!isStarted)
    {
        logging("Starting app...");

		this->player = Player::GetInstance();
		this->visionRect = { 0, 6, 32, 24 };

        LG->InitLevel();
		this->sprite = Image("./images/Yoshi2.png");
		this->monst = Image("./images/spiny.png");
		this->wood = Image("./images/floor.png", TILESIZE, TILESIZE);
        this->background = Image("./images/back.png", WINDOWX, WINDOWY);
        this->floor = Image("./images/floor2.png", TILESIZE, TILESIZE);
    	this->cloud = Image("./images/nuage.png", TILESIZE * 2, TILESIZE * 3);
        this->box = Image("./images/hitBox.png", TILESIZE, TILESIZE);
    	this->trap = Image("./images/spikes.png", TILESIZE, TILESIZE);
		this->endFlag = Image("./images/flag.png", TILESIZE * 6, TILESIZE * 2);
		this->plant = Image("./images/plants.png");

        this->startedTime = SDL_GetTicks();
        this->isStarted = true;

		this->daMusic = Music::GetInstance();
		daMusic->load_files();
		daMusic->play_Music();

		this->changeDifficult = false;
    }

	while (MainLoop() == CONTINUE);
}

void Application::Stop(void)
{
    // Clean monsters
    while (!monsters.empty())
        monsters.pop_front();
	this->background.Close();
	this->box.Close();
	this->cloud.Close();
	this->trap.Close();
	this->endFlag.Close();
	this->floor.Close();
    Player::DestroyInstance();
	Music::DestroyInstance();
    isStarted = false;
}

void Application::ShowToolTip(string str, SDL_Color c)
{
    SDL_Rect rect = { -1, 210, 642, 50 };
    SDL_SetRenderDrawColor(appRender, 0, 0, 0, 0);
    SDL_RenderFillRect(appRender, &rect);

    SDL_SetRenderDrawColor(appRender, c.r, c.g, c.b, c.a);
    SDL_RenderDrawRect(appRender, &rect);

    Font font = Font(DIEFONT, appRender);
    font.SetStyle(false, false, 35);
    font.SetPos(320, 240, true);
    font.SetColors(c, SHADOWBLACK);
    font.DrawText(str);
}

bool Application::IsFloorType(t_tile tile){
	if (tile == FLOORTOP || tile == FLOORLEFT || tile == FLOORRIGHT ||
		tile == FLOORSIDELEFT || tile == FLOORSIDERIGHT || tile == FLOOR || tile == WOOD)
		return true;
	else
		return false;
}

bool Application::CheckCollision(int x, int y, int sizex, int sizey)
{
	if (!IsFloorType(LG->GetTile(this->visionRect, player->space, x + 5, y + sizey)) &&
		!IsFloorType(LG->GetTile(this->visionRect, player->space, x + sizex - 5, y + sizey)))
		return false;
	return true;
}

void Application::CheckFinish(int x)
{
    if (WINDOWX - x < 30)
    {
        Stop();
        OPTIONS->UpLevel();
        Start();
    }
}

void Application::CheckDeath(int x, int y)
{
    if (this->player->hitbox.y > WINDOWY ||
        (LG->GetTile(this->visionRect, player->space, x + 4, y + PLAYERSIZEY / 2) == TRAP ||
         LG->GetTile(this->visionRect, player->space, x + PLAYERSIZEX - 4, y + PLAYERSIZEY / 2) == TRAP))
    {
		this->player->Die();
    }
}

void Application::CheckDeathByMonster(SDL_Rect p, SDL_Rect m)
{
    if (Collisions::CollisionBoxes(p, m))
	{
		if (this->player->Die() == RESTARTGAME)
		{
			Stop();
			Start();
		}
	}
}

int Application::MainLoop(void)
{
	beginTime = SDL_GetTicks();

	while(SDL_PollEvent(&appEvent))
    {
		switch (appEvent.type)
		{
		case SDL_QUIT:
			EndApplication();
			break;
		case SDL_KEYDOWN:
			this->currentEvent = HandleKeyDown();
			break;
		case SDL_KEYUP:
			this->currentEvent = HandleKeyUp();
			break;
		default:
			break;
		}
	}

	this->background.Draw();

	int pos = player->hPastWay / TILESIZE;
	player->space = player->hPastWay % TILESIZE;
    this->visionRect.x = pos;

	// Allow to draw past textures correctly
	int initValueX = 0;
	if  (this->visionRect.x == 1)
		initValueX = - 1;
	else if (this->visionRect.x == 2)
		initValueX = -2;
	else if (this->visionRect.x >= 3)
		initValueX = -3;

    for (int x = initValueX; x <= this->visionRect.w; x++)
    	for (int y = 0; y <= this->visionRect.h; y++)
    	{
    		int vx = x + this->visionRect.x;
    		int vy = y + this->visionRect.y;
    		if (LG->tileTypes[vx][vy] == BOX)
    			this->box.DrawAt(x*TILESIZE - player->space, y*TILESIZE);
    		else if (LG->tileTypes[vx][vy] == WOOD)
    			this->wood.DrawAt(x*TILESIZE - player->space, y*TILESIZE);
    		else if (LG->tileTypes[vx][vy] == FLOORTOP)
    			this->floor.DrawSpriteAt({ x*TILESIZE - player->space, y*TILESIZE, TILESIZE, TILESIZE }, { 17, 0, 15, 15 }, false);
    		else if (LG->tileTypes[vx][vy] == FLOORLEFT)
    			this->floor.DrawSpriteAt({ x*TILESIZE - player->space, y*TILESIZE, TILESIZE, TILESIZE }, { 0, 0, 15, 15 }, false);
    		else if (LG->tileTypes[vx][vy] == FLOORRIGHT)
    			this->floor.DrawSpriteAt({ x*TILESIZE - player->space, y*TILESIZE, TILESIZE, TILESIZE }, { 34, 0, 15, 15 }, false);
    		else if (LG->tileTypes[vx][vy] == FLOORSIDELEFT)
    			this->floor.DrawSpriteAt({ x*TILESIZE - player->space, y*TILESIZE, TILESIZE, TILESIZE }, { 0, 17, 15, 15 }, false);
    		else if (LG->tileTypes[vx][vy] == FLOORSIDERIGHT)
    			this->floor.DrawSpriteAt({ x*TILESIZE - player->space, y*TILESIZE, TILESIZE, TILESIZE }, { 34, 17, 15, 15 }, false);
    		else if (LG->tileTypes[vx][vy] == FLOOR)
    			this->floor.DrawSpriteAt({ x*TILESIZE - player->space, y*TILESIZE, TILESIZE, TILESIZE }, { 17, 17, 15, 15 }, false);
    		else if (LG->tileTypes[vx][vy] == CLOUD)
    			this->cloud.DrawAt(x*TILESIZE - player->space, y*TILESIZE);
    		else if (LG->tileTypes[vx][vy] == TRAP)
    			this->trap.DrawAt(x*TILESIZE - player->space, y*TILESIZE);
    		else if (LG->tileTypes[vx][vy] == ENDFLAG)
    			this->endFlag.DrawAt(x*TILESIZE - player->space, y*TILESIZE);
    		else if (LG->tileTypes[vx][vy] == PLANT)
    			this->plant.DrawSpriteAt({ x*TILESIZE - player->space, y*TILESIZE, TILESIZE * 2, TILESIZE}, { 260, 102, 47, 23 }, false);
    		else if (LG->tileTypes[vx][vy] == PLANT2)
    			this->plant.DrawSpriteAt({ x*TILESIZE - player->space, y*TILESIZE, TILESIZE * 2, TILESIZE }, { 444, 100, 47, 27 }, false);
    		else if (LG->tileTypes[vx][vy] == PLANT3)
    			this->plant.DrawSpriteAt({ x*TILESIZE - player->space, y*TILESIZE, TILESIZE, TILESIZE }, { 226, 280, 17, 33 }, false);
        }

    list<Monster*>::iterator i;
    for (i = monsters.begin(); i != monsters.end(); ++i)
    {
		Monster* t = *i;

		if (changeDifficult){
			t_difficulty diff = OPTIONS->GetDifficulty();
			if (diff == EASY)
				t->realSpeed = 1;
			else if (diff == NORMAL)
				t->realSpeed = 2;
			else if (diff == HARD)
				t->realSpeed = 4;
		}

        int x = t->hitbox.x;
        int y = t->hitbox.y;

    	if (!CheckCollision(x, y, MONSTERSIZEX, MONSTERSIZEY)){
    		t->Fall();
    	}
    	else
    	{
    		int diff = y % TILESIZE;
    		if (diff > 0)
    			t->hitbox.y -= diff;
    		t->canJump = true;
    	}

		t->hPastWay = player->hPastWay;
		t->space = player->space;
		t->Move();
        t->Draw();

		CheckDeathByMonster(player->hitbox, t->hitbox);
    }

	int x = this->player->hitbox.x;
    int y = this->player->hitbox.y;

	if (!CheckCollision(x, y, PLAYERSIZEX, PLAYERSIZEY)){
		this->player->Fall();
	}
	else
    {
        int diff = y % TILESIZE;
        if (diff > 0)
            this->player->hitbox.y -= diff;
		this->player->canJump = true;
    }

    CheckFinish(x);

	player->Draw();
	player->Move();

	if (beginTime - startedTime < 1000){
		ShowToolTip(OPTIONS->to_string(), YELLOW);
		changeDifficult = true;
	}

	if (player->moveEnable != true){
		if (this->player->Die() == RESTARTGAME)
		{
			Stop();
			Start();
		}
	}
	else{
		CheckDeath(x, y);
	}

	APP->Update();

    elapsedTime = SDL_GetTicks() - beginTime;
    if (elapsedTime < 16)
        SDL_Delay(16 - elapsedTime);

	return CONTINUE;
}

/* Return the "rendu" of the application ? */
SDL_Renderer* Application::GetRenderer(void)
{
	return this->appRender;
}

/* Clear the render */
void Application::ClearRender(void)
{
	SDL_RenderClear(this->appRender);
}

/* Update the render */
void Application::Update(void)
{
	SDL_RenderPresent(this->appRender);
}

/* Close and destroy the render and the window */
void Application::Close(void)
{
    if (this->isStarted)
    {
        Stop();
    }
    SDL_DestroyRenderer(appRender);
    SDL_DestroyWindow(appWindow);
}


// Private

/* Handle when user presses a button */
int Application::HandleKeyDown(void)
{
    switch (appEvent.key.keysym.sym)
    {
    case SDLK_ESCAPE:
		ShowMenu();

	case SDLK_RETURN:
        return QUITAPP;

	case SDLK_k:
		if (!player->speed)
		{
			player->speed = true;
		}
		break;

	case SDLK_o:
		if (!player->jump)
		{
			player->jump = true;
		}
		break;

    case SDLK_z:
        if (!player->walkUp)
        {
            player->walkUp = true;
        }
        break;

    case SDLK_s:
        if (!player->walkDown)
        {
            player->walkDown = true;
        }
        break;

    case SDLK_q:
        if (!player->walkLeft)
        {
            player->walkLeft = true;
        }
        break;

    case SDLK_d:
        if (!player->walkRight)
        {
            player->walkRight = true;
        }
        break;

    default:
        return appEvent.key.keysym.sym;
    }

	return appEvent.key.keysym.sym;
}

/* Handle when user release a button */
int Application::HandleKeyUp(void)
{
   switch (appEvent.key.keysym.sym)
    {
   case SDLK_k:
	   if (player->speed)
	   {
		   player->speed = false;
	   }
	   break;

   case SDLK_o:
	   if (player->jump)
	   {
		   player->jump = false;
	   }
	   break;

    case SDLK_z:
        if (player->walkUp)
        {
            player->walkUp = false;
        }
        break;

    case SDLK_s:
        if (player->walkDown)
        {
            player->walkDown = false;
        }
        break;

    case SDLK_q:
        if (player->walkLeft)
        {
            player->walkLeft = false;
        }
        break;

    case SDLK_d:
        if (player->walkRight)
        {
            player->walkRight = false;
        }
        break;

    default:
        return appEvent.key.keysym.sym;
    }

   return appEvent.key.keysym.sym;
}

void Application::EndApplication(void)
{
    // Ferme les SDL et quitte le programme
    logging("tchao a+");
    APP->Close();
    MENU->Close();
    Logger::GetInstance()->Close();
    exit (0);
}