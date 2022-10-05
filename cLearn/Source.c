#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include<time.h>

typedef struct
{
	int x, y;
	float dy;
	short life;
	char *name;
} Man;
typedef struct {
	int x, y;
} Star;
typedef struct {
	int x, y, w, h;
} Ledge;
typedef struct
{
	Man man;

	Star stars[100];

	Ledge ledges[100];

	SDL_Texture *star;
	SDL_Texture *manFrames[7];
	SDL_Texture *bridge;
	SDL_Renderer *renderer;
} GameState;

void collisionDetect(GameState *game)
{
	for (int i = 0; i < 100; i++)
	{
		float mw = 64, mh = 64;
		float mx = game->man.x, my = game->man.y;
		float bx = game->ledges[i].x, by = game->ledges[i].y, bw = game->ledges[i].w, bh = game->ledges[i].h;

		if (my + mh > by && my < by + bh)
		{
			if (mx < bx+bw && mx+mw > bx+bw)
			{
				game->man.x = bx + bw;
				mx = bx + bw;
			}
			else if(mx+mw > bx && mx < bx)
			{
				game->man.x = bx - mw;
				mx = bx - mw;
			}
		}

		if (mx + mw > bx && mx < bx + bw)
		{
			if (my < by + bh && my > by) {
				game->man.y = by + bh;
				game->man.dy = 0;
			}
			else if (my+mh > by && my < by)
			{
				game->man.y = by - mh;
				game->man.dy = 0;
			}
		}
	}
}

void loadGame(GameState* game) 
{
	SDL_Surface* surface = NULL;

	surface = IMG_Load("./images/star.png");
	if (surface == NULL)
	{
		printf("Cannot find star.png \n\n");
		SDL_Quit();
		exit(1);
	}
	game->star = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("./images/bridge.png");
	if (surface == NULL)
	{
		printf("Cannot find bridge.png \n\n");
		SDL_Quit();
		exit(1);
	}
	game->bridge = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("./images/idle-01.png");
	if (surface == NULL)
	{
		printf("Cannot find idle-01.png \n\n");
		SDL_Quit();
		exit(1);
	}
	game->manFrames[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("./images/run-00.png");
	if (surface == NULL)
	{
		printf("Cannot find run-00.png \n\n");
		SDL_Quit();
		exit(1);
	}
	game->manFrames[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("./images/run-01.png");
	if (surface == NULL)
	{
		printf("Cannot find run-01.png \n\n");
		SDL_Quit();
		exit(1);
	}
	game->manFrames[2] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("./images/run-02.png");
	if (surface == NULL)
	{
		printf("Cannot find run-02.png \n\n");
		SDL_Quit();
		exit(1);
	}
	game->manFrames[3] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("./images/run-03.png");
	if (surface == NULL)
	{
		printf("Cannot find run-03.png \n\n");
		SDL_Quit();
		exit(1);
	}
	game->manFrames[4] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("./images/run-04.png");
	if (surface == NULL)
	{
		printf("Cannot find run-04.png \n\n");
		SDL_Quit();
		exit(1);
	}
	game->manFrames[5] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("./images/run-05.png");
	if (surface == NULL)
	{
		printf("Cannot find run-05.png \n\n");
		SDL_Quit();
		exit(1);
	}
	game->manFrames[6] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	game->man.x = 320 - 40;
	game->man.y = 240 - 40;

	for (int i = 0; i < 100; i++) 
	{
		game->stars[i].x = rand()%640;
		game->stars[i].y = rand()%480;
	}

	for (int i = 0; i < 100; i++) {
		game->ledges[i].w = 256;
		game->ledges[i].h = 64;
		game->ledges[i].x = i*256;
		game->ledges[i].y = 400;
	}
	game->ledges[99].x = 350;
	game->ledges[99].y = 200;
}

int processEvent(SDL_Window *window, GameState *game)
{
	int done = 0;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT_CLOSE:
		{
			if (window)
			{
				SDL_DestroyWindow(window);
				window = NULL;
			}
		}
		break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				done = 1;
			break;
			}
		}
		break;
		case SDL_QUIT:
			done = 1;
			break;
		}
	}

	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_LEFT])
		game->man.x -= 10;
	if (state[SDL_SCANCODE_RIGHT])
		game->man.x += 10;
	if (state[SDL_SCANCODE_UP])
		game->man.y -= 10;
	if (state[SDL_SCANCODE_DOWN])
		game->man.y += 10;
	return done;
}

void doRender(SDL_Renderer *renderer, GameState *game)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_Rect rect = { game->man.x, game->man.y, 64, 64 };
	SDL_RenderCopyEx(renderer, game->manFrames[0], NULL, &rect, 0, NULL, 0);
	//SDL_RenderFillRect(renderer, &rect);

	for (int i = 0; i < 1; i++)
	{
		SDL_Rect ledgeRect = { game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h };
		SDL_RenderCopy(renderer, game->bridge, NULL, &ledgeRect);
	}

	for(int i = 0; i < 1; i++)
	{
		SDL_Rect starRect = { game->stars[i].x, game->stars[i].y, 64, 64 };
		SDL_RenderCopy(renderer, game->star, NULL, &starRect);
	}

	SDL_RenderPresent(renderer);
}

int main() 
{
	GameState gameState;
	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_Init(SDL_INIT_VIDEO);

	srand((int)time(NULL));

	window = SDL_CreateWindow("C Language Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	gameState.renderer = renderer;
	loadGame(&gameState);

	int done = 0;

	while (!done)
	{
		done = processEvent(window, &gameState);
		
		collisionDetect(&gameState);

		doRender(renderer, &gameState);
	}

	/*SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_Rect rect = { 220, 140, 200, 200 };
	SDL_RenderFillRect(renderer, &rect);

	SDL_RenderPresent(renderer);

	SDL_Delay(2000);*/

	SDL_DestroyTexture(gameState.star);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	SDL_Quit();

	return 0;
}