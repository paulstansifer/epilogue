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
#ifndef PAINT_H
#define PAINT_H


#include "phys_obj.h"
#include <ncurses.h>
#include <curses.h>
/**
@author Paul Stansifer
*/


void initialize_painter();
void clean_screen();

void move_cursor(int row, int col);

void paint(int c, phys_obj* who);
void mvpaint(int row, int col, int c, phys_obj* who);

void paint_effect(int c, phys_obj* who);
void mvpaint_effect(int row, int col, int c, phys_obj* who);

void paintstr(const char* str, phys_obj* who);
void mvpaintstr(int row, int col, const char* str, phys_obj* who);

void paintstr_effect(const char* str, phys_obj* who);
void mvpaintstr_effect(int row, int col, const char* str, phys_obj* who);


void set_color(int c);
void set_color_cycle(int colors[], int size);
void color_code(char c);

inline int max_row() {return LINES-1;}
inline int min_row() {return 0;}

inline int max_col() {return COLS-2;}
inline int min_col() {return 1;}

inline int rand_col() {return rand() % (max_col() - min_col()) + min_col();}

/* These probably aren't needed.  Use set_???() instead.  */

const int GRAY = 0;
const int WHITE = 1;
const int RED = 2;
const int PINK = 3;
const int GREEN = 4;
const int LIGHT_GREEN = 5;
const int BROWN = 6;
const int YELLOW = 7;
const int BLUE = 8;
const int LIGHT_BLUE = 9;
const int PURPLE = 10;
const int FUSCIA = 11;
const int CYAN = 12;
const int LIGHT_CYAN = 13;
const int DARK_GRAY = 14;

void set_gray();
void set_white();
void set_red();
void set_pink();
void set_green();
void set_light_green();
void set_brown();
void set_yellow();
void set_blue();
void set_light_blue();
void set_purple();
void set_fuscia();
void set_cyan();
void set_light_cyan();
void set_dark_gray();


void color_test();

#endif
