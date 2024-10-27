#include "main.h"

int missile_check(GameState* game, int index, char type) {
	return 0;
}

void load_missile(GameState* gamestate, int index) {
	gamestate->missile[index].speed = rand() % 6 + 2 + gamestate->dif;
	int X = gamestate->player.px - 700, Y = gamestate->player.py - 450;
	if (X > 0) {
		if (Y < 0) {
			if (rand() == 0) gamestate->missile[index].type = 1;
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
		gamestate->missile[index].x = WINDOW_WIDTH + rand() % 200;
		do { gamestate->missile[index].y = 20 + rand() % (WINDOW_HEIGHT - 40); } while (missile_check(gamestate, index, 'x'));
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

void missile_update(GameState* game) {
	int time = (SDL_GetTicks() - game->start_tick) / 1000;
	if (time > 2 && time < 10) game->dif = 1;
	else if (time > 10 && time < 20 && game->dif == 1) game->dif = 2;
	else if (time > 20 && time < 30 && game->dif == 2) game->dif = 3;
	else if (time > 30 && time < 50 && game->dif == 3) {
		printf("increased dif\n");
		game->dif = 4;
		game->missile_count++;
		game->missile = (Missile*)realloc(game->missile, game->missile_count * sizeof(Missile));
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
			if (collision(game, game->missile[x].x + 5, game->missile[x].y + 2, 16, 30) >= 0) {
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
			if (game->missile[x].x < WINDOW_WIDTH + 40) {
				game->missile[x].x += game->missile[x].speed;
			}
			else {
				load_missile(game, x);
			}
			break;
		case 2:
			//DOWN
			if (collision(game, game->missile[x].x - 2, game->missile[x].y + 5, 30, 16) >= 0) {
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
