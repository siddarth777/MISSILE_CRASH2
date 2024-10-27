#pragma once
#include "main.h"
typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
}Bug_Test;

void bug_init(Bug_Test* bug);
void bug_quit(Bug_Test* bug);

void bug_init(Bug_Test* bug) {
	//high priotiy
	bug->window = NULL;
	bug->renderer = NULL;
	bug->window = SDL_CreateWindow(
		"Debug Menu",               
		SDL_WINDOWPOS_CENTERED,          
		SDL_WINDOWPOS_CENTERED,             
		640, 480,                           
		SDL_WINDOW_SHOWN                    
	);
	if (!bug->window) {
		printf("ERROR INITIALISING WINDOW\n");
		exit(0);
	}
	bug->renderer = SDL_CreateRenderer(bug->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!bug->renderer) {
		printf("ERROR INITIALISING RENDERER\n");
		exit(0);
	}
}

void show_stats(Bug_Test* bug, GameState* game) {
	
}

void bug_quit(Bug_Test* bug) {
	SDL_DestroyRenderer(bug->renderer);
	SDL_DestroyWindow(bug->window);
	SDL_Quit();
}
