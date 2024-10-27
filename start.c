#include"main.h"

int hover(Button* button) {
	int X, Y;
	SDL_GetMouseState(&X, &Y);
	if (X > button->x && X < button->x + button->width) {
		if (Y > button->y && Y < button->y + button->height) {
			return 1;
		}
	}
	return 0;
}

void start_init(Button* button) {
	button[0].x = 600;
	button[0].y = 400;
	button[0].width = 100;
	button[0].height = 50;
	button[0].text = "start";
}

void start_process(GameState* game, Button* button) {
	SDL_Event event;
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) game->state = QUIT;
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
		for (int x = 0; x < BUTTON_COUNT; x++) {
			if (hover(&button[x])) {
				switch (x) {
				case 0:
					game->state = MAIN;
					loadgame(game);
					//start button
					break;
				}
			}
		}
	}

}

void start_render(GameState* game, Button* button) {
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
	SDL_RenderClear(game->renderer);
	SDL_Rect rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };  // x, y, width, height
	SDL_RenderCopy(game->renderer, game->start_text, NULL, &rect);
	for (int x = 0; x < BUTTON_COUNT; x++) {
		SDL_Color color;
		if (hover(&button[x])) { color.r = 0; color.g = 0; color.b = 255; color.a = 255; }
		else { color.r = 255; color.g = 255; color.b = 255; color.a = 255; }
		SDL_Surface* textsurface = TTF_RenderText_Solid(game->font, button[x].text, color);
		SDL_Texture* texttexture = SDL_CreateTextureFromSurface(game->renderer, textsurface);
		SDL_Rect rect = { button[x].x,button[x].y,textsurface->w,textsurface->h };
		SDL_RenderFillRect(game->renderer, &rect);

		SDL_FreeSurface(textsurface);
		SDL_RenderCopy(game->renderer, texttexture, NULL, &rect);
	}
	SDL_RenderPresent(game->renderer);
}

void start_loop(GameState* game, Button* button) {
	start_process(game, button);
	start_render(game, button);
}