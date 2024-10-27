#include "main.h"

int min_index(int* array, int size) {
	int index = 0;
	for (int x = 1; x < size; x++) {
		if (array[x] < array[index]) index = x;
	}
	return index;
}

int collision(GameState* game, int bx, int by, int bh, int bw) {
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
		return min_index(overlap, 4) * 10;

	}
	else return -1;
}

void process_input(GameState* game) {
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
	if (((event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) || keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W]) && game->player.jump_count < 2 && game->player.vy >= 0) {
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
