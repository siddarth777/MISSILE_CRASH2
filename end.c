#include"main.h"

void end_process(GameState* game) {
	SDL_Event event;
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) game->state = QUIT;
	if ((event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)) {
		loadgame(game);
		game->state = MAIN;
	}
}

void end_render(GameState* game) {
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
	SDL_RenderClear(game->renderer);
	SDL_Rect rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };  // x, y, width, height
	SDL_RenderCopy(game->renderer, game->end_text, NULL, &rect);
	SDL_RenderPresent(game->renderer);
}

void end_loop(GameState* game) {
	end_process(game);
	end_render(game);
}