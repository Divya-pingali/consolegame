#include <unistd.h>
#include <ncurses.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include "popup.h"
#include "game.h"
#include <iostream>

using namespace std;

int popup_main(){
    clear();
    int win_height = 23, win_width = 85;
    WINDOW* popUp = newwin(win_height, win_width, 0, 0);
    box(popUp, 0, 0);
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);

    string game_over = "GAME OVER";
    string score_msg = "Your score was : ";
    string exit_msg = "PRESS Q TO EXIT THE GAME";

    int row_game_over = 10;
    int row_score = 11;
    int row_exit = 12;

    int col_game_over = (win_width - game_over.length()) / 2;
    int col_score_msg = (win_width - score_msg.length() - 5) / 2;
    int col_score_val = col_score_msg + score_msg.length();
    int col_exit_msg = (win_width - exit_msg.length()) / 2;

    mvwprintw(popUp, row_game_over, col_game_over, "%s", game_over.c_str());
    mvwprintw(popUp, row_score, col_score_msg, "%s", score_msg.c_str());
    mvwprintw(popUp, row_score, col_score_val, "%i", score);
    mvwprintw(popUp, row_exit, col_exit_msg, "%s", exit_msg.c_str());

    wrefresh(popUp);
    int input = 0;
    while(1){
        input = wgetch(popUp);
        if(input == 'q' || input == 'Q'){
            clear();
            delwin(popUp);
            endwin();
            break;
        }
        usleep(10000);
    }
    return 0;
}
