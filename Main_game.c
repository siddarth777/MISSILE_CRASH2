#include"main.h"
#include "debug.h"
int main(int argc,char* args[]) {
	GameState gamestate;
	Bug_Test bug_test;
	Button start_buttons[1];
	init(&gamestate);
	loadgame(&gamestate);
	start_init(start_buttons);
	//bug_init(&bug_test);
	//main loop
	while (gamestate.state!=QUIT) {
		switch (gamestate.state) {
		case START:
			start_loop(&gamestate,start_buttons);
			break;
		case MAIN:
			main_loop(&gamestate);
			break;
		case END:
			end_loop(&gamestate);
		break;
		}
		SDL_Delay(15);
	}
	quit(&gamestate);
	//bug_quit(&bug_test);
	return 0;
}