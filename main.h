//#pragma once
#ifndef main_H
#define main_H
#include<stdio.h>
#include<sdl.h>
#include<stdlib.h>
#include<sdl_image.h>
#include<sdl_mixer.h>
#include<sdl_ttf.h>
#include<time.h>
#define TRUE 1
#define FALSE 0
#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 900
#define PLAYER_WIDTH 30
#define PLAYER_HEIGHT 60
#define GRAVITY 1
#define SPEED 7
#define AIR_SPEED 6
#define TERMINAL_VELOCITY 15
#define JUMP_SPEED 15
#define HORIZONTAL_BOUNCE 1
#define VERTICAL_BOUNCE 2
#define LOCK_PERIOD 300
#define Q1MOVING 3
#define Q1MAX 350
#define Q1MIN 450
#define Q1MEAN 350
#define Q1HORIZONTAL 4
#define Q2HORIZONTAL 5
#define LEFT 0
#define RIGHT 10
#define TOP 20
#define BOTTOM 30
#define START 100
#define MAIN 200
#define END 300
#define QUIT 1000

//button
typedef struct {
	int x;
	int y;
	int width;
	int height;
	char* text;
}Button;

//Player structure
typedef struct {
	float px;
	float py;
	float vx;
	float vy;
	float ax;
	float ay;
	int width;
	int height;
	int on_ground;
	int jump_count;
	int walk_lock;
	Uint32 lock_time;
	SDL_Texture* player_texture;
}Player;

//platform strucuture
typedef struct {
	int x;
	int y;
	int width;
	int height;
	int type;
	int val;
}Platform;

//missile structure
typedef struct {
	int x;
	int y;
	int type;
	int speed;
}Missile;

//overall gamestate
typedef struct {
	int start_tick;
	Player player;
	int state;
	Platform *platform;
	int missile_count;
	Missile *missile;
	int dif;
	int platform_count;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* missile_text[4];
	SDL_Texture* start_text;
	SDL_Texture* end_text;
	Mix_Chunk* jump_sound;
	TTF_Font* font;
}GameState;

void init(GameState* gamestate);
int min_index(int* array, int size);
void missile_update(GameState* game);
void count_lines(GameState* game);
void loadmap(GameState* game);
void quit(GameState* gamestate);
void loadgame(GameState* gamestate);
int collision(GameState* game, int bx,int by,int bh,int bw);
void process_input(GameState* game);
void update(GameState* game);
void render(GameState* game);
void main_loop(GameState* gamestate);
void load_missile(GameState* gamestate, int index);
int missile_check(GameState* game, int index, char type);


//START FUNCTIONS
#define BUTTON_COUNT 1
void start_process(GameState* game, Button* button);
void start_render(GameState* game, Button* button);
void start_loop(GameState* game, Button* button);
void start_init(Button* button);
int hover(Button* button);


//end functions
void end_process(GameState* game);
void end_render(GameState* game);
void end_loop(GameState* game);

/*int missile_check(GameState* game, int index, char type) {
	return 0;
}

void load_missile(GameState* gamestate, int index) {
	gamestate->missile[index].speed = rand() % 6 +2+gamestate->dif;
	int X = gamestate->player.px - 700, Y = gamestate->player.py - 450;
	if (X > 0) {
		if (Y < 0) {
			if(rand() == 0) gamestate->missile[index].type = 1;
			else gamestate->missile[index].type = 3;
		}
		else {
			if (rand() % 2 == 0) gamestate->missile[index].type = 2;
			else gamestate->missile[index].type = 1;
		}
	}
	else {
		if (Y < 0) {
			if (rand() % 2 == 0) gamestate->missile[index].type = 0;
			else gamestate->missile[index].type = 2;
		}
		else {
			if (rand() % 2 == 0) gamestate->missile[index].type = 0;
			else gamestate->missile[index].type = 2;
		}
	}
	switch (gamestate->missile[index].type) {
	case 0:
		//LEFT
		gamestate->missile[index].x = WINDOW_WIDTH+rand()%200;
		do { gamestate->missile[index].y = 20 + rand() % (WINDOW_HEIGHT - 40); } while (missile_check(gamestate,index,'x'));
		gamestate->missile[index].speed *= (-1);
		break;
	case 1:
		//RIGHT
		gamestate->missile[index].x = -40 - rand() % 200;
		do { gamestate->missile[index].y = 20 + rand() % (WINDOW_HEIGHT - 40); } while (1 == 0);
		break;
	case 2:
		//DOWN
		do { gamestate->missile[index].x = 10 + rand() % (WINDOW_WIDTH - 20); } while (1 == 0);
		gamestate->missile[index].y = -40 - rand() % 200;
		break;
	case 3:
		//UP
		gamestate->missile[index].x = 10 + rand() % (WINDOW_WIDTH - 20);
		do { gamestate->missile[index].y = WINDOW_HEIGHT + rand() % 200; } while (1 == 0);
		gamestate->missile[index].speed *= (-1);
		break;
	}
}

void init(GameState* gamestate) {
	srand(time(NULL));
	//high priotiy
	gamestate->window = NULL;
	gamestate->renderer = NULL;
	gamestate->state = START;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("ERROR INITIALISING SDL\n");
		gamestate->state = QUIT;
	}
	gamestate->window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
	);
	if (!gamestate->window) {
		printf("ERROR INITIALISING WINDOW\n");
		gamestate->state = QUIT;
	}
	gamestate->renderer = SDL_CreateRenderer(gamestate->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!gamestate->renderer) {
		printf("ERROR INITIALISING RENDERER\n");
		gamestate->state = QUIT;
	}

	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 512);
	gamestate->font = TTF_OpenFont("images/Millennia.otf", 48);
}

int min_index(int* array, int size) {
	int index = 0;
	for (int x = 1; x < size; x++) {
		if (array[x] < array[index]) index = x;
	}
	return index;
}

void missile_update(GameState* game) {
	int time = (SDL_GetTicks()-game->start_tick)/1000;
	if (time > 2 && time < 10) game->dif = 1;
	else if (time > 10 && time < 20 &&game->dif==1) game->dif = 2;
	else if (time > 20 && time < 30 && game->dif == 2) game->dif = 3;
	else if (time > 30 && time < 50 && game->dif == 3) {
		printf("increased dif\n");
		game->dif = 4;
		game->missile_count++;
		game->missile = (Missile*)realloc(game->missile,game->missile_count * sizeof(Missile));
		if (game->missile == NULL) exit(1);
		load_missile(game, game->missile_count - 1);
	}
	else if (time > 50 && time < 70 && game->dif == 4) game->dif = 5;
	else if (time > 70 && time < 100 && game->dif == 5) {
		printf("increased dif\n");
		game->dif = 6;
		game->missile_count++;
		game->missile = (Missile*)realloc(game->missile, game->missile_count * sizeof(Missile));
		if (game->missile == NULL) exit(1);
		load_missile(game, game->missile_count - 1);
	}
	else if (time > 100 && time < 150 && game->dif == 6) {
		printf("increased dif\n");
		game->dif = 7;
		game->missile_count++;
		game->missile = (Missile*)realloc(game->missile, game->missile_count * sizeof(Missile));
		if (game->missile == NULL) exit(1);
		load_missile(game, game->missile_count - 1);
	}
	else if (time > 200 && game->dif == 7) {
		printf("increased dif\n");
		game->dif = 8;
		game->missile_count++;
		game->missile = (Missile*)realloc(game->missile, game->missile_count * sizeof(Missile));
		if (game->missile == NULL) exit(1);
		load_missile(game, game->missile_count - 1);
	}
	if (game->dif == 0) return;
	for (int x = 0; x < game->missile_count; x++) {
		switch (game->missile[x].type) {
		case 0:
			//LEFT
			if (collision(game, game->missile[x].x+5 , game->missile[x].y+2 , 16, 30) >= 0) {
				//SDL_Delay(500);
				//game->state = END;
			}
			if (game->missile[x].x > -40) {
				game->missile[x].x += game->missile[x].speed;
			}
			else {
				load_missile(game, x);
			}
			break;
		case 1:
			//RIGHT
			if (collision(game, game->missile[x].x + 5, game->missile[x].y + 2, 16, 30) >= 0) {
				//SDL_Delay(500);
				//game->state = END;
			}
			if (game->missile[x].x <WINDOW_WIDTH+40) {
				game->missile[x].x += game->missile[x].speed;
			}
			else {
				load_missile(game, x);
			}
			break;
		case 2:
			//DOWN
			if (collision(game, game->missile[x].x-2, game->missile[x].y+5, 30, 16) >= 0) {
				//SDL_Delay(500);
				//game->state = END;
			}
			if (game->missile[x].y < WINDOW_HEIGHT + 40) {
				game->missile[x].y += game->missile[x].speed;
			}
			else {
				load_missile(game, x);
			}
			break;
		case 3:
			//UP
			if (collision(game, game->missile[x].x - 2, game->missile[x].y + 5, 30, 16) >= 0) {
				//SDL_Delay(500);
				//game->state = END;
			}
			if (game->missile[x].y > -40) {
				game->missile[x].y += game->missile[x].speed;
			}
			else {
				load_missile(game, x);
			}
			break;
		}
	}
}

void count_lines(GameState * game) {
	game->platform_count = 0;
	FILE* file = fopen("images/map.txt", "r");
	char line[256];
	int platform_no = 0;
	while (fgets(line, sizeof(line), file)) {
		if (line[0] == 'x') continue;
		game->platform_count++;
	}
	fclose(file);
}

void loadmap(GameState* game) {
	FILE* file = fopen("images/map.txt", "r");
	char line[256];
	int platform_no = 0;
	while (fgets(line, sizeof(line), file)) {
		int index = 0, property = 0, val = 0;
		if (line[0] == 'x') continue;
		while (line[index]!='\n'&& line[index]!='\0') {
			if (line[index] == ' ') {
				switch (property) {
				case 0:
					game->platform[platform_no].x = val;
					break;
				case 1:
					game->platform[platform_no].y = val;
					break;
				case 2:
					game->platform[platform_no].width = val;
					break;
				case 3:
					game->platform[platform_no].height = val;
					break;
				case 4:
					game->platform[platform_no].type = val;
					break;
				case 5:
					game->platform[platform_no].val = val;
				}
				property++;
				val = 0;
			}
			else {
				val = (val * 10) + (int)line[index] - 48;
			}
			index++;
		}
		platform_no++;
	}
	fclose(file);
}

void quit(GameState* gamestate) {
	free(gamestate->platform);
	Mix_FreeChunk(gamestate->jump_sound);
	SDL_DestroyTexture(gamestate->player.player_texture);
	SDL_DestroyTexture(gamestate->end_text);
	SDL_DestroyTexture(gamestate->end_text);
	for (int x = 0; x < 4; x++) {
		SDL_DestroyTexture(gamestate->missile_text[x]);
	}
	IMG_Quit();
	Mix_CloseAudio();
	SDL_DestroyRenderer(gamestate->renderer);
	SDL_DestroyWindow(gamestate->window);
	SDL_Quit();
}

void loadgame(GameState* gamestate) {

	gamestate->start_tick = SDL_GetTicks();

	//player stats
	gamestate->player.px = 300;
	gamestate->player.py = 500;
	gamestate->player.vx = 0;
	gamestate->player.vy = 0;
	gamestate->player.ax = 0;
	gamestate->player.ay = 0;
	gamestate->player.height = PLAYER_HEIGHT;
	gamestate->player.width = PLAYER_WIDTH;
	gamestate->player.on_ground = FALSE;
	gamestate->player.jump_count = 0;
	gamestate->player.walk_lock = FALSE;

	//PLATFORM STUFF
	count_lines(gamestate);
	gamestate->platform = (Platform*)malloc(gamestate->platform_count * sizeof(Platform));
	if (gamestate->platform == NULL) {
		gamestate->state = FALSE;
		fprintf(stderr, "ERROR INITIALISING RENDERER\n");
		exit(0);
	}
	loadmap(gamestate);
	for (int x = 0; x < gamestate->platform_count; x++) {
		printf("PLATFORM: %d :-\n", x);
		printf("%d %d %d %d %d %d\n", gamestate->platform[x].x, gamestate->platform[x].y, gamestate->platform[x].width, gamestate->platform[x].height, gamestate->platform[x].type,gamestate->platform[x].val);
	}

	//missile stuff
	gamestate->missile_count = 7;
	gamestate->missile=(Missile*)malloc(gamestate->missile_count * sizeof(Missile));
	for (int x = 0; x < gamestate->missile_count; x++) {
		load_missile(gamestate, x);
		/*gamestate->missile[x].x = WINDOW_WIDTH - 100;
		gamestate->missile[x].y = rand()%WINDOW_HEIGHT;
		gamestate->missile[x].type = 2;
		gamestate->missile[x].speed = 5;
	}
	gamestate->dif = 0;

	//texture loading
	SDL_Surface* player_surface = IMG_Load("images/PLAYER_PIC.png");
	SDL_SetColorKey(player_surface, SDL_TRUE, SDL_MapRGB(player_surface->format, 255, 255, 255));
	gamestate->player.player_texture = SDL_CreateTextureFromSurface(gamestate->renderer, player_surface);
	SDL_FreeSurface(player_surface);

	SDL_Surface* start_surface = IMG_Load("images/START.png");
	gamestate->start_text = SDL_CreateTextureFromSurface(gamestate->renderer, start_surface);
	SDL_FreeSurface(start_surface);


	SDL_Surface* end_surface = IMG_Load("images/END.png");
	gamestate->end_text = SDL_CreateTextureFromSurface(gamestate->renderer, end_surface);
	SDL_FreeSurface(end_surface);

	SDL_Surface* left = IMG_Load("images/MISSILE_LEFT.png");
	SDL_SetColorKey(left, SDL_TRUE, SDL_MapRGB(left->format, 255, 255, 255));
	gamestate->missile_text[0] = SDL_CreateTextureFromSurface(gamestate->renderer, left);
	SDL_FreeSurface(left);

	SDL_Surface* right = IMG_Load("images/MISSILE_RIGHT.png");
	SDL_SetColorKey(right, SDL_TRUE, SDL_MapRGB(right->format, 255, 255, 255));
	gamestate->missile_text[1] = SDL_CreateTextureFromSurface(gamestate->renderer, right);
	SDL_FreeSurface(right);

	SDL_Surface* down = IMG_Load("images/MISSILE_DOWN.png");
	SDL_SetColorKey(down, SDL_TRUE, SDL_MapRGB(down->format, 255, 255, 255));
	gamestate->missile_text[2] = SDL_CreateTextureFromSurface(gamestate->renderer, down);
	SDL_FreeSurface(down);

	SDL_Surface* up = IMG_Load("images/MISSILE_UP.png");
	SDL_SetColorKey(up, SDL_TRUE, SDL_MapRGB(up->format, 255, 255, 255));
	gamestate->missile_text[3] = SDL_CreateTextureFromSurface(gamestate->renderer, up);
	SDL_FreeSurface(up);

	//sound mixer
	gamestate->jump_sound = Mix_LoadWAV("Sounds/test.wav");
}

int collision(GameState* game,int bx,int by,int bh,int bw) {
	float px = game->player.px, py = game->player.py, ph = game->player.height, pw = game->player.width;
	if (px <= bx + bw &&
		px + pw >= bx &&
		py <= by + bh &&
		py + ph >= by) {
		int overlap[4];
		overlap[0] = (px + pw) - bx;  // left
		overlap[1] = (bx + bw) - px;  // right
		overlap[2] = (py + ph) - by;  // top
		overlap[3] = (by + bh) - py; //bottom
		return min_index(overlap, 4)*10;

	}
	else return -1;
	}

void process_input(GameState* game){
	SDL_Event event;
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) game->state = QUIT;
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	const Uint8 left = keyState[SDL_SCANCODE_A] | keyState[SDL_SCANCODE_LEFT], right = keyState[SDL_SCANCODE_D] | keyState[SDL_SCANCODE_RIGHT];
	if (left && !game->player.walk_lock) {
		if (game->player.on_ground) game->player.vx = -SPEED;
		else game->player.vx = -AIR_SPEED;
	}
	if (right && !game->player.walk_lock) {
		if (game->player.on_ground) game->player.vx = SPEED;
		else game->player.vx = AIR_SPEED;
	}
	if (!(left ^ right) & !game->player.walk_lock) {
		game->player.vx = 0;
	}
	if (( (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) || keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W])&& game->player.jump_count<2 && game->player.vy>=0 ) {
		game->player.jump_count++;
		game->player.vy = -JUMP_SPEED;
	}
}

void update(GameState* game) {
	game->player.px += game->player.vx;
	game->player.py += game->player.vy;
	game->player.on_ground = FALSE;
	for (int x = 0; x < game->platform_count; x++) {
		int bx = game->platform[x].x, by = game->platform[x].y, bh = game->platform[x].height, bw = game->platform[x].width;
		int hit = collision(game, bx, by, bh, bw);
		switch (hit) {
		case LEFT:
			game->player.px = bx - game->player.width;
			break;

		case RIGHT:
			game->player.px = bx + bw;
			break;

		case TOP:
			game->player.py = by - game->player.height;
			game->player.on_ground = TRUE;
			break;

		case BOTTOM:
			game->player.vy = 0;
			if (game->platform[x].type != 3) {
				game->player.py = by + bh + game->player.vy + 1;
			}
			else {
				game->player.py = by + bh + game->player.vy + 5;
			}
			break;
		}
		if (game->platform[x].type == HORIZONTAL_BOUNCE) {
				switch (hit) {
				case TOP:
					//top edge
					game->player.on_ground = FALSE;
					game->player.jump_count = 1;
					game->player.py = by - game->player.height - 1;
					game->player.vy = game->player.vy * ((float)game->platform[x].val / 10) * -1;
					break;
				}
			}
		if (game->platform[x].type == VERTICAL_BOUNCE) {
				switch (collision(game, bx, by + 5, bh - 5, bw)) {
				case LEFT:
					game->player.px -= 1;
					game->player.lock_time = SDL_GetTicks();
					game->player.walk_lock = TRUE;
					game->player.vx *= (-1);
					break;

				case RIGHT:
					//right
					game->player.px += 1;
					game->player.lock_time = SDL_GetTicks();
					game->player.walk_lock = TRUE;
					game->player.vx *= (-2);
					break;
				}
			}
		if (game->platform[x].type == Q1MOVING) {
				if (game->platform[x].y < 350) game->platform[x].val *= -1;
				else if (game->platform[x].y > 450) game->platform[x].val *= -1;
				game->platform[x].y += game->platform[x].val;
				if (hit == TOP) { 
					game->player.on_ground = TRUE; 
					game->player.py += game->platform[x].val;
				}
			}
		if (game->platform[x].type == Q1HORIZONTAL) {
				if (game->platform[x].x == 250)game->platform[x].val *= -1;
				else if (game->platform[x].x == 5)game->platform[x].val *= -1;
				else if (game->platform[x].x == 350)game->platform[x].val *= -1;
				else if (game->platform[x].x == 595)game->platform[x].val *= -1;
				game->platform[x].x += game->platform[x].val;
				switch (hit) {
				case TOP:
					game->player.px += game->platform[x].val;
					break;
				}
			}
		if (game->platform[x].type == Q2HORIZONTAL) {
				if (game->platform[x].x == 950)game->platform[x].val *= -1;
				else if (game->platform[x].x == 705)game->platform[x].val *= -1;
				else if (game->platform[x].x == 1050)game->platform[x].val *= -1;
				else if (game->platform[x].x == 1295)game->platform[x].val *= -1;
				game->platform[x].x += game->platform[x].val;
				switch (hit) {
				case TOP:
					game->player.px += game->platform[x].val;
					break;
				}
			}
	}
	if (game->player.on_ground) {
		if (game->player.vy > 0) Mix_PlayChannel(-1, game->jump_sound, 0);
		game->player.vy = 0;
		game->player.ay = 0;
		game->player.jump_count = 0;
	}
	else {
		game->player.ay = GRAVITY;
	}
	if (game->player.walk_lock) {
		if (SDL_GetTicks() - game->player.lock_time > LOCK_PERIOD) game->player.walk_lock = FALSE;
	}
	game->player.vx += game->player.ax;
	game->player.vy += game->player.ay;
	if (game->player.vy > TERMINAL_VELOCITY) game->player.vy = TERMINAL_VELOCITY;
	if (game->player.vx > TERMINAL_VELOCITY) game->player.vx = TERMINAL_VELOCITY;
	missile_update(game);
}

void render(GameState* game) {
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
	SDL_Rect player_rect = { game->player.px, game->player.py, game->player.width, game->player.height };  // x, y, width, height
	SDL_RenderClear(game->renderer);
	for (int x = 0; x < game->platform_count; x++) {
		SDL_Rect rect = { game->platform[x].x, game->platform[x].y, game->platform[x].width, game->platform[x].height };
		switch (game->platform[x].type) {
		case 0:
			SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
			break;
		case 1:
			SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255);
			break;
		case 2:
			SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255);
			break;
		case 3:
			SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
			break;
		case 4:
			SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
			break;
		case 5:
			SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
			break;
		}
		SDL_RenderFillRect(game->renderer, &rect);
	}
	for (int x = 0; x < game->missile_count; x++) {
		if (game->missile[x].type == 0 || game->missile[x].type == 1) {
			SDL_Rect rect = { game->missile[x].x, game->missile[x].y, 40, 20 };
			SDL_RenderCopy(game->renderer, game->missile_text[game->missile[x].type], NULL, &rect);
		}
		else {
			SDL_Rect rect = { game->missile[x].x, game->missile[x].y, 20, 40 };
			SDL_RenderCopy(game->renderer, game->missile_text[game->missile[x].type], NULL, &rect);
		}
	}
	SDL_RenderCopy(game->renderer, game->player.player_texture, NULL, &player_rect);
	SDL_Color color = { 255,255,255,255 };
	SDL_Surface* textsurface = TTF_RenderText_Solid(game->font,"Score: ", color);
	SDL_Texture* texttexture = SDL_CreateTextureFromSurface(game->renderer, textsurface);
	SDL_Rect rect = { 10,10,textsurface->w,textsurface->h };
	SDL_FreeSurface(textsurface);
	SDL_RenderCopy(game->renderer, texttexture, NULL, &rect);
	SDL_RenderPresent(game->renderer);
}

void main_loop(GameState* gamestate) {
	process_input(gamestate);
	update(gamestate);
	render(gamestate);
}
*/
#endif // main_H
