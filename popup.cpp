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
    WINDOW* popUp = newwin(23, 85, 0, 0);
    box(popUp, 0, 0);
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    mvwprintw(popUp, 10, 36, "%s", "GAME OVER");
    mvwprintw(popUp, 11, 30 , "%s", "Your score was : ");
    mvwprintw(popUp, 11, 47, "%i", score);
    mvwprintw(popUp, 12, 25 , "%s", "PRESS Q TO EXIT THE GAME");

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
