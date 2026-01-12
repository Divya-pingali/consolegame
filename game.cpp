#include <iostream>
#include <ncurses.h>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "popup.h"
using namespace std;

#define DELAY 15000 //define the delay between each iteration in the main while loop (tuned for better responsiveness)

int game_Window_Size_X = 19;
int game_Window_Size_Y = 50;
int score = 0;
int lives = 10;

class Monster{
public:
    int monsterx, monstery;
    int move_counter = 0;
    vector<string> monster_shape = {
        "[o_o]",
        " /|\\",
        " / \\"
    };
    bool correct_shot = false;

    void initialise(int monsterx, int monstery){
        this->monsterx = monsterx;
        this->monstery = monstery;
    }
    void update(WINDOW* window){
        move_counter++;
        if (move_counter >= 3) {
            monsterx--;
            move_counter = 0;
        }
    }
    void draw(WINDOW* window){
        for (int i = 0; i < 3; ++i) {
            mvwprintw(window, monstery + i, monsterx, "%s", monster_shape[i].c_str());
        }
    }
    void erase(WINDOW* window){
        for (int i = 0; i < 3; ++i) {
            mvwprintw(window, monstery + i, monsterx, "%s", "     ");
        }
    }
    int monsterX(){ return monsterx; }
    int monsterY(){ return monstery; }
    int width() const { return 5; }
    int height() const { return 3; }
    // Check if a given (x, y) hits any part of the monster
    bool isHit(int x, int y) const {
        return (x >= monsterx && x < monsterx + width() &&
                y >= monstery && y < monstery + height());
    }
    // Check if this monster overlaps with another
    bool overlapsWith(const Monster& other) const {
        for (int i = 0; i < height(); ++i) {
            for (int j = 0; j < width(); ++j) {
                int x1 = monsterx + j;
                int y1 = monstery + i;
                for (int oi = 0; oi < other.height(); ++oi) {
                    for (int oj = 0; oj < other.width(); ++oj) {
                        int x2 = other.monsterx + oj;
                        int y2 = other.monstery + oi;
                        if (x1 == x2 && y1 == y2) return true;
                    }
                }
            }
        }
        return false;
    }
};
class Bullet{
public:
    int bulletx, bullety;
    string bullet_string = "=";

    void initialise(int bulletx, int bullety){
        this -> bulletx = bulletx;
        this -> bullety = bullety;
    }

    void draw(WINDOW* window){
        mvwprintw(window, bullety, bulletx,"%s",bullet_string.c_str());
    }

    void update(WINDOW* window){
            //usleep(DELAY);
            bulletx += 1;
    }

    void erase(WINDOW* window){
        mvwprintw(window, bullety, bulletx, " ");
    }

    int bulletX(){
        return bulletx;
    }
    int bulletY(){
        return bullety;
    }

};
class Pistol{
public:
    int pistolx, pistoly;
    string pistol_string_1 = "_______";
    string pistol_string_2 = "| _____|";
    string pistol_string_3 = "|_|";

    void initialise(int pistolx, int pistoly){
        this -> pistoly = pistoly;
        this -> pistolx = pistolx;
    }

    void draw(WINDOW* window) {
        mvwprintw(window, pistoly, pistolx,"%s",pistol_string_1.c_str());
        mvwprintw(window, pistoly + 1, pistolx,"%s",pistol_string_2.c_str());
        mvwprintw(window, pistoly + 2, pistolx,"%s",pistol_string_3.c_str());
    }

    void updatePistol(int input) {
        switch(input) {
            case KEY_UP:
                if(pistoly > 1) {
                    pistoly--;
                }
                break;
            case KEY_DOWN:
                if(pistoly < game_Window_Size_X -5) {
                    pistoly++;
                }
                break;
            default:
                break;
        }
    }
    int pistolY(){
        return pistoly;
    }
    void erase(WINDOW *window){
        mvwprintw(window, pistoly, pistolx,"%s", "       ");
        mvwprintw(window, pistoly + 1, pistolx,"%s", "        ");
        mvwprintw(window, pistoly + 2, pistolx,"%s", "       ");
    }
};

int game_main(){
    bool isLost = false;

    WINDOW* gameWindow = newwin(game_Window_Size_X, game_Window_Size_Y, 3,0);
    WINDOW* scoreWindow = newwin(3, game_Window_Size_Y, 0,0);
    WINDOW* instructionsWindow = newwin(game_Window_Size_X + 3, 35, 0, 50);

    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    box(gameWindow, 0, 0);
    box(scoreWindow, 0, 0);
    box(instructionsWindow, 0, 0);
    srand(time(NULL));

    //const strings
    mvwprintw(gameWindow, 8, 18, "%s", "PRESS S TO START");
    mvwprintw(scoreWindow, 1, 2, "%s", "Score:");
    mvwprintw(scoreWindow, 1, 40, "%s", "Lives:");
    mvwprintw(instructionsWindow, 1, 1, "%s", "GAME MANUAL:");
    mvwprintw(instructionsWindow, 2, 1, "%s", "Start : S");
    mvwprintw(instructionsWindow, 3, 1, "%s", "Move Pistol up : Up Arrow");
    mvwprintw(instructionsWindow, 4, 1, "%s", "Move Pistol down : Down Arrow");
    mvwprintw(instructionsWindow, 5, 1, "%s", "Shoot : SpaceBar");
    mvwprintw(instructionsWindow, 6, 1, "%s", "Quit : Q");
    mvwprintw(instructionsWindow, 7, 1, "%s", "-----------------------------");
    mvwprintw(instructionsWindow, 8, 1, "%s", "HOW TO PLAY:");
    mvwprintw(instructionsWindow, 9, 1, "%s", "Shoot the monsters with the ");
    mvwprintw(instructionsWindow, 10, 1, "%s", "pistol. Your score increments");
    mvwprintw(instructionsWindow, 11, 1, "%s", "by 3 everytime you hit a ");
    mvwprintw(instructionsWindow, 12, 1, "%s", "monster. You initially have ");
    mvwprintw(instructionsWindow, 13, 1, "%s", "10 lives and you will lose one");
    mvwprintw(instructionsWindow, 14, 1, "%s", "life everytime you miss the ");
    mvwprintw(instructionsWindow, 15, 1, "%s", "target.");

    //refresh all windows to display
    wrefresh(gameWindow);
    wrefresh(scoreWindow);
    wrefresh(instructionsWindow);
    
    //initialise pistol
    Pistol pistol;
    pistol.initialise(1,1);

    //initialise bullets and monsters
    vector<Bullet> bullets;
    vector<Monster> monsters;

    wrefresh(gameWindow);

    //handle condition for starting the game
    int pre_game_input = 0;
    bool start_game = false;
    while (!start_game) {
        pre_game_input = getch();
        if (pre_game_input == 's' || pre_game_input == 'S') {
            mvwprintw(gameWindow, 8, 18, "%s", "                ");
            wrefresh(gameWindow);
            start_game = true;
        } else if (pre_game_input == 'q' || pre_game_input == 'Q') {
            delwin(gameWindow);
            delwin(scoreWindow);
            delwin(instructionsWindow);
            endwin();
            return 0;
        }
        usleep(10000);
    }

    while(start_game){
        //score window updates
        mvwprintw(scoreWindow, 1, 9, "%s", "         ");
        mvwprintw(scoreWindow, 1, 9, "%i", score);
        mvwprintw(scoreWindow, 1, 47, "%s", "  ");
        mvwprintw(scoreWindow, 1, 47, "%i", lives);
        wrefresh(scoreWindow);

        //game window pistol print and monster initialisation
        pistol.draw(gameWindow);
        int monster_ypos = rand() % (game_Window_Size_X - 5); // leave space for 3-line monster
        int isMonsterCreate = rand() % 49;
        if (isMonsterCreate == 1) {
            Monster newMonster;
            newMonster.initialise(game_Window_Size_Y - 7, monster_ypos + 2); // leave space for border
            // Check for overlap
            bool overlap = false;
            for (const auto& m : monsters) {
                if (abs(m.monsterx - newMonster.monsterx) < newMonster.width() &&
                    abs(m.monstery - newMonster.monstery) < newMonster.height()) {
                    overlap = true;
                    break;
                }
            }
            if (!overlap) {
                monsters.push_back(newMonster);
                newMonster.draw(gameWindow);
            }
        }

    //handle monster updates
    if(!monsters.empty()){
        for(auto itm = monsters.begin(); itm != monsters.end(); ){
        Monster& monster = *itm;
            if(monster.correct_shot == false){
                //update monster if target is not hit
                if(monster.monsterX() >= 0){
                    monster.erase(gameWindow);
                    monster.update(gameWindow);
                    monster.draw(gameWindow);
	                if(monster.monsterX() <= 10) {
                        //handle monster reaching the boundary
                        pistol.draw(gameWindow);
                        box(gameWindow, 0, 0);
	                }
	                ++itm;
                } else if(monster.monsterX() < 2){
                if(lives > 0){
                    //decrease lives if monster is not hit
                    lives--;
                    if(lives == 0){
                        isLost = true;
                    }
                }

                //delete from vector
                monster.erase(gameWindow);
	            itm = monsters.erase(itm);
                } 
            }
        }

        if(isLost){
            break;
        }
    }

    //handle bullet initialisation
    int input = getch();
    if(input == KEY_UP || input == KEY_DOWN){
    pistol.erase(gameWindow);
    pistol.updatePistol(input);
    pistol.draw(gameWindow);
    wrefresh(gameWindow);
    }
    if(input == ' '){
        Bullet bullet;
        bullet.initialise(8, pistol.pistolY() + 1);
        bullets.push_back(bullet);
        bullet.draw(gameWindow);
    }

    //handle bullet updates
    for(auto itb = bullets.begin(); itb != bullets.end(); ){
      //update bullet in each iteration
      Bullet& bullet = *itb;
      bullet.erase(gameWindow);
      bullet.update(gameWindow);
      bullet.draw(gameWindow);
      if(bullet.bulletX() >= game_Window_Size_Y -2){
        //handle bullet reaching the boundary
        bullet.erase(gameWindow);
	    itb = bullets.erase(itb);
      } else {
	    ++itb;
      }
    }

    //handle correct target being hit
    for(auto itb = bullets.begin(); itb != bullets.end(); ){
	Bullet& bullet = *itb;
	bool isHit = false;
        for(auto itm = monsters.begin(); itm != monsters.end(); ){
	    Monster& monster = *itm; 
            if (monster.isHit(bullet.bulletX(), bullet.bulletY())) {
                if (score < 10000) {
            popup_main();
        }
        return 0;
		itm = monsters.erase(itm);
		isHit = true;
		break;
            } else {
		    ++itm;
	    }
        }
	if(isHit) {
	  itb = bullets.erase(itb);
	} else {
	  ++itb;
	}
    }

    //refresh window
    wrefresh(gameWindow);

    if(input == 'q' || input == 'Q'){
        break;
    }
    flushinp();
    usleep(DELAY);
    };
    if(isLost){
        popup_main();
    }
    endwin();
    return 0;
};
