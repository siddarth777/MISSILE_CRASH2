#include "main.h"

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