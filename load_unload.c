#include "main.h"

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

void count_lines(GameState* game) {
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
		while (line[index] != '\n' && line[index] != '\0') {
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
		printf("%d %d %d %d %d %d\n", gamestate->platform[x].x, gamestate->platform[x].y, gamestate->platform[x].width, gamestate->platform[x].height, gamestate->platform[x].type, gamestate->platform[x].val);
	}

	//missile stuff
	gamestate->missile_count = 7;
	gamestate->missile = (Missile*)malloc(gamestate->missile_count * sizeof(Missile));
	for (int x = 0; x < gamestate->missile_count; x++) {
		load_missile(gamestate, x);
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