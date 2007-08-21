/***************************************************************************
 *   Copyright (C) 2005 by Paul Stansifer                                  *
 *   07pns@williams.edu                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "paint.h"

#include <stdio.h>

#include <iostream>
#include <ncurses.h>

#include <vector>
#include "phys_obj.h"

using namespace std;

vector< vector<phys_obj*> > collision_map;
vector< vector<float>		> avoidance_map;
int currow, curcol;

//used for color cycling
int tick_number;

void initialize_painter() {
	//int key_repeat[2];
	//key_repeat[0] = 0; key_repeat[1] = 300;
	//if(ioctl(0, EVIOCSREP, key_repeat) == -1) {
	//	throw "unable to set key repeat";
	//}


	initscr();
  keypad(stdscr, TRUE);
	start_color();
	cbreak();
	noecho();
	
	timeout(0); //don't wait for input
	
	
	curs_set(0); //invisible cursor
  
  

	init_pair(0, COLOR_WHITE, COLOR_BLACK);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	init_pair(7, COLOR_BLACK, COLOR_BLACK);

	//TODO: receive notification of resizing

	collision_map.resize(max_row()+1);
	avoidance_map.resize(max_row()+1);
	for(int i = 0; i <= max_row(); i++) {
		collision_map[i].resize(max_col()+1, NULL);
		avoidance_map[i].resize(max_col()+1, 0);
	}

	tick_number = 0;
}

void clean_screen() {
	int imx = max_row();
	int iimn = min_col();
	int iimx = max_col();
	for(int i = min_row(); i <= imx; i++) {
		for(int ii = iimn; ii <= iimx; ii++) {
			collision_map[i][ii] = NULL;
			avoidance_map[i][ii] *= 0.5;
		}
	}

	tick_number++;

	erase();
}

inline int pick_char(int c) {
	//TODO enable this, on platforms it looks good on.
//	switch(c){
//		case '-':
//			return ACS_HLINE;
//		case '|':
//			return ACS_VLINE;
//		default:
			return c;
//	}
}

/*
0.0 0.0 0.0
0.0 0.0 0.0
0.0 1.0 0.0

0.0 0.0 0.0
0.2 0.2 0.2
0.2 0.0 0.2


0.0  0.05 0.0
0.1  0.0  0.1
0.0  0.2  0.0
*/



inline bool prepaint_effect(int row, int col, phys_obj* what) {
	if(row < min_row() || col < min_col() || row > max_row() || col > max_col() )
		return false;
	phys_obj* already_there = collision_map[row][col];
	if(already_there != NULL) {
		already_there->collide(row, col, what);
	}
	return true;
}

inline bool prepaint(int row, int col, phys_obj* who) {
	if(row < min_row() || col < min_col() || row > max_row() || col > max_col() )
		return false;
	phys_obj** already_there = &collision_map[row][col];
	if(*already_there != NULL && who != NULL) {
		(*already_there)->collide(row, col, who);
	}
	*already_there = who;

	avoidance_map[row][col] += 0.5; //?
	return true;
}

void move_cursor(int row, int col) {
  move(row, col);
	currow = row;
	curcol = col;
}

void paint_effect(int c, phys_obj* what) {
	if(prepaint_effect(currow, curcol++, what))
		addch(pick_char(c));
}
void mvpaint_effect(int row, int col, int c, phys_obj* what) {
	if(prepaint_effect(currow = row, curcol = col, what))
		mvaddch(row, col, pick_char(c));
	curcol++;
}

void paint(int c, phys_obj* who) {
	if(prepaint(currow, curcol++, who))
		addch(pick_char(c));
}
void mvpaint(int row, int col, int c, phys_obj* who) {
	if(prepaint(currow = row, curcol = col, who))
		mvaddch(row, col, pick_char(c));
	curcol++;
}

void paintstr(const char* str, phys_obj* who) {
	int i = 0;
	while(str[i] != '\0' && !prepaint(currow, curcol+i, who))
		i++; //off-screen
	if(str[i] != '\0')
		addch(pick_char(str[i++]));

	while(str[i] != '\0' && prepaint(currow, curcol+i, who))
		addch(pick_char(str[i++]));
	curcol += i; //if we're off-screen to the right, curcol doesn't have to indicate exactly how much.
}

void mvpaintstr(int row, int col, const char* str, phys_obj* who) {
	currow = row; curcol = col;
	if(row < min_row())
		return; //if we are in an off-screen row, curcol doesn't need to be correct, because there is no wraping
	int i = 0;
	while(str[i] != '\0' && !prepaint(currow, curcol+i, who))
		i++; //off-screen
	if(str[i] != '\0')
		mvaddch(currow, curcol+i, pick_char(str[i++]));

	while(str[i] != '\0' && prepaint(currow, curcol+i, who))
		addch(pick_char(str[i++]));
	curcol += i; //if we're off-screen to the right, curcol doesn't have to indicate exactly how much.
}

void paintstr_effect(const char* str, phys_obj* who) {
	int i = 0;
	while(str[i] != '\0' && !prepaint_effect(currow, curcol+i, who))
		i++; //off-screen
	if(str[i] != '\0')
		addch(pick_char(str[i++]));

	while(str[i] != '\0' && prepaint_effect(currow, curcol+i, who))
		addch(pick_char(str[i++]));
	curcol += i; //if we're off-screen to the right, curcol doesn't have to indicate exactly how much.
}

void mvpaintstr_effect(int row, int col, const char* str, phys_obj* who) {
	currow = row; curcol = col;
	if(row < min_row())
		return; //if we are in an off-screen row, curcol doesn't need to be correct, because there is no wraping
	int i = 0;
	while(str[i] != '\0' && !prepaint_effect(currow, curcol+i, who))
		i++; //off-screen
	if(str[i] != '\0')
		mvaddch(currow, curcol+i, pick_char(str[i++]));

	while(str[i] != '\0' && prepaint_effect(currow, curcol+i, who))
		addch(pick_char(str[i++]));
	curcol += i; //if we're off-screen to the right, curcol doesn't have to indicate exactly how much.
}


void set_color_cycle(int colors[], int size) {
	set_color(colors[tick_number % size]);
}




void set_color(int c) {
	if(c%2 == 1)
		attron(A_BOLD);
	else
		attroff(A_BOLD);
	color_set(c/2, NULL);
}

void color_code(char c) {
	switch(c) {
		case 'w': set_gray(); break;
		case 'W': set_white(); break;
		case 'd': set_dark_gray(); break;
		case 'r': set_red(); break;
		case 'R': set_pink(); break;
		case 'g': set_green(); break;
		case 'G': set_light_green(); break;
		case 'y': set_brown(); break;
		case 'Y': set_yellow(); break;
		case 'b': set_blue(); break;
		case 'B': set_light_blue(); break;
		case 'p': set_purple(); break;
		case 'P': set_fuscia(); break;
		case 'c': set_cyan(); break;
		case 'C': set_light_cyan(); break;
	}
}

void set_gray() {
	attroff(A_BOLD);
  color_set(0,NULL);
}

void set_white() {
  //attron(A_BOLD); //doesn't work on OS X
	attroff(A_BOLD);
  color_set(0,NULL);
}

void set_red() {
	attroff(A_BOLD);
  color_set(1,NULL);
}

void set_pink() {
	attron(A_BOLD);
  color_set(1,NULL);
}

void set_green() {
	attroff(A_BOLD);
  color_set(2,NULL);
}

void set_light_green() {
	attron(A_BOLD);
  color_set(2,NULL);
}

void set_brown() {
	attroff(A_BOLD);
  color_set(3,NULL);
}

void set_yellow() {
	attron(A_BOLD);
  color_set(3,NULL);
}

void set_blue() {
	attroff(A_BOLD);
  color_set(4,NULL);
}

void set_light_blue() {
	attron(A_BOLD);
  color_set(4,NULL);
}

void set_purple() {
	attroff(A_BOLD);
  color_set(5,NULL);
}

void set_fuscia() {
	attron(A_BOLD);
  color_set(5,NULL);
}

void set_cyan() {
	attroff(A_BOLD);
  color_set(6,NULL);
}

void set_light_cyan() {
	attron(A_BOLD);
  color_set(6,NULL);
}

void set_dark_gray() {
	//attron(A_BOLD);
	//color_set(7,NULL);
	attroff(A_BOLD); //OS X
	color_set(0,NULL);
};






void color_test(){
	for(int i = 0; i < COLORS; i++) {
	  attrset(A_NORMAL);
		color_set(i, NULL);
		mvaddch(i,0,'#');
		attrset(A_STANDOUT);
		color_set(i, NULL);
		mvaddch(i,1,'#');
		attrset(A_UNDERLINE);
		color_set(i, NULL);
		mvaddch(i,2,'#');
		attrset(A_REVERSE);
		color_set(i, NULL);
		mvaddch(i,3,'#');
		attrset(A_BLINK);
		color_set(i, NULL);
		mvaddch(i,4,'#');
		attrset(A_DIM);
		color_set(i, NULL);
		mvaddch(i,5,'#');
		attrset(A_BOLD);
		color_set(i, NULL);
		mvaddch(i,6,'#');
		attrset(A_PROTECT);
		color_set(i, NULL);
		mvaddch(i,7,'#');
		attrset(A_INVIS);
		color_set(i, NULL);
		mvaddch(i,8,'#');
		attrset(A_ALTCHARSET);
		color_set(i, NULL);
		mvaddch(i,9,'#');
	}
	attrset(A_NORMAL);
	color_set(0,NULL);

	for(int i = COLORS; i < COLORS + 2; i++) {
		int x = 0;
		mvaddch(i, x++, ACS_ULCORNER);
		mvaddch(i, x++, ACS_LLCORNER);
		mvaddch(i, x++, ACS_URCORNER);
		mvaddch(i, x++, ACS_LRCORNER);
		mvaddch(i, x++, ACS_LTEE);
		mvaddch(i, x++, ACS_RTEE);
		mvaddch(i, x++, ACS_BTEE);
		mvaddch(i, x++, ACS_TTEE);
		mvaddch(i, x++, ACS_HLINE);
		mvaddch(i, x++, ACS_VLINE);
		mvaddch(i, x++, ACS_PLUS);
		mvaddch(i, x++, ACS_S1);        //
		mvaddch(i, x++, ACS_S9);       //
		mvaddch(i, x++, ACS_DIAMOND); //
		mvaddch(i, x++, ACS_CKBOARD);
		mvaddch(i, x++, ACS_PLMINUS);
		mvaddch(i, x++, ACS_BULLET);

		mvaddch(i, x++, ACS_LARROW);     //
		mvaddch(i, x++, ACS_RARROW);    //
		mvaddch(i, x++, ACS_DARROW);   //
		mvaddch(i, x++, ACS_UARROW);  //
		mvaddch(i, x++, ACS_BOARD);  //
		mvaddch(i, x++, ACS_LANTERN);
		mvaddch(i, x++, ACS_BLOCK);  //


		mvaddch(i, x++, ACS_S3);   //
		mvaddch(i, x++, ACS_S7);  //
		mvaddch(i, x++, ACS_LEQUAL);
		mvaddch(i, x++, ACS_GEQUAL);
		mvaddch(i, x++, ACS_PI);
		mvaddch(i, x++, ACS_NEQUAL);
		mvaddch(i, x++, ACS_STERLING);


	}
	attrset(A_NORMAL);
	for(int i = 0; i < 200; i++)
		addch('@');
}
