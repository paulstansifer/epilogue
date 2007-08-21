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
#include "shot.h"
#include "paint.h"

#include "epilogue.h"

#include <limits.h>

void make_shot(float a, float b, float c, float d, float e, int f, int g, int h) { //TMP
  new shot(a, b, c, d, e, f, g, h);
}

//static, one-character shot (TODO: enable multiple frames: spinning blades)
shot::shot(float x, float y, float force, float velx, float vely, int image, int color)
		: phys_obj(x,y,force, 1.0, true) {
	this->velx = velx; this->vely = vely;
	this->image = image;
	this->color = color;
  this->life = INT_MAX;
}

shot::shot(float x, float y, float force, float velx, float vely, int image, int color, int life)
: phys_obj(x,y,force, 1.0, true) {
	this->velx = velx; this->vely = vely;
	this->image = image;
	this->color = color;
  this->life = life;
}

void shot::collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push) {
	dead = true;
}
void shot::draw() {
	color_code(color);
	mvpaint_effect(row(), col(), image, this);
}

bool shot::tick() {
  if(life-- < 0)
    return false;
	if(x < min_col() || x > max_col() || y < min_row() || y > max_row())
		return false;
	return true;
}


/*
 * lightning_bolt
 */

lightning_bolt::lightning_bolt(float x, float y, float force, float velx, float vely, float end_velx, float end_vely, int life)
		: phys_obj(x,y,force, 7.0 /*Stay away!*/, true) {
	this->velx = velx; this->vely = vely;
	this->end_xvel = end_velx; this->end_yvel = end_vely;
	this->life = life;

	end_x = x; end_y = y;
}


void lightning_bolt::collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push) {
	//bah!
}
void lightning_bolt::draw() {
  if(misc_rng(3) == 0)
  	return; //flicker out
	if(life % 2 == 1) {
		frame1.draw(0,0,this);
	} else {
		frame2.draw(0,0,this);
	}
}

int lightning_bolt::random_dot() {
	switch(misc_rng(6)){
	case 0: case 1: return '.';
	case 2: return ':';
	case 3: return '\'';
	case 4: return ',';
	case 5: return '`';
	default: return 'x'; //can't happen
	}
}

bool lightning_bolt::tick() {
	float xd = end_xvel + misc_rng(100) / 50.0 - 1.0;
	float yd = end_yvel + misc_rng(100) / 50.0 - 1.0;

	if(xd == 0 && yd == 0) {
		frame1.add_character(end_x, end_y+yd, '*', 'W');
		frame2.add_character(end_x+xd, end_y, '*', 'W');
	} else if(xd == 0 || fabsf(xd / yd) < 0.3) {
		frame1.add_character(end_x, end_y+yd, ACS_VLINE, 'W');
		frame2.add_character(end_x+xd, end_y, ACS_VLINE, 'W');
	} else if(yd == 0 || fabsf(yd / yd) < 0.3) {
	  frame1.add_character(end_x, end_y+yd, ACS_HLINE, 'W');
	  frame2.add_character(end_x+xd, end_y, ACS_HLINE, 'W');
	} else if(xd/yd > 0) {
	  frame1.add_character(end_x, end_y+yd, '\\', 'W');
	  frame2.add_character(end_x+xd, end_y, '\\', 'W');
	} else {
	  frame1.add_character(end_x, end_y+yd, '/', 'W');
	  frame2.add_character(end_x+xd, end_y, '/', 'W');
	}


	end_x += xd; end_y += yd;

	if(misc_rng(2) == 0) {
		frame1.add_character(end_x   -1, end_y+yd + misc_rng(5)-2, random_dot(), 'C');
		frame2.add_character(end_x+xd+1, end_y    + misc_rng(5)-2, random_dot(), 'C');
	} else {
		frame1.add_character(end_x   +1, end_y+yd + misc_rng(5)-2, random_dot(), 'c');
		frame2.add_character(end_x+xd-1, end_y    + misc_rng(5)-2, random_dot(), 'c');
	}

	if(misc_rng(6) == 0) {
		//fork!
		life = (int)(life * 0.75);
		new lightning_bolt(end_x, end_y, force, velx, vely, end_xvel + misc_rng(40) / 10.0 - 2.0 , end_yvel + misc_rng(10) / 10.0 - 0.5, life + misc_rng(3) - 1 );
	}

	return --life > 0;
}

/*
 *  smoke
 */

smoke::smoke(float x, float y, float force, float xvel, float yvel, int life)
		: phys_obj(x,y, force, 1.0) {
	this->velx = misc_rng(10)/20.0 - 0.25 + xvel; 
	this->vely = misc_rng(10)/40.0 - 0.125 + yvel;
	this->life = life;
}

smoke::smoke(float x, float y, float force, int life) : phys_obj(x, y, force, 1.0) {
	this->velx = misc_rng(10)/20.0 - 0.25; 
	this->vely = misc_rng(10)/40.0 - 0.125;
	this->life = life;
}


void smoke::collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push) {
	//life *= 0.5; //smoke can be torn through eventually
}
void smoke::draw() {
	switch(life % 4) {
		case 0: set_purple();
		case 3: set_gray();
		default: set_brown();
	//case 0: case 1: set_gray(); break;
	//case 2: set_brown(); break;
	//case 3: set_dark_gray(); break;
	}

  if(life > 15) {
  	switch(misc_rng(9)){
  	case 0: mvpaint(row(), col(), '0', this); break;
  	case 1: mvpaint(row(), col(), 'O', this); break;
  	case 2: mvpaint(row(), col(), 'o', this); break;
  	case 3: mvpaint(row(), col(), '(', this); break;
  	case 4: mvpaint(row(), col(), 'O', this); break;
  	case 5: mvpaint(row(), col(), ')', this); break;
  	case 6: mvpaint(row(), col(), '0', this); break;
  	case 7: mvpaint(row(), col(), '&', this); break;
  	case 8: mvpaint(row(), col(), '%', this); break;
  	}
  } else {
		switch(misc_rng(3)) {
		case 0: mvpaint(row(), col(), 'o', this); break;
		case 1: mvpaint(row(), col(), '.', this); break;
		case 2: mvpaint(row(), col(), ':', this); break;
		}
	}
}

bool smoke::tick() {
	if(x < min_col() || x > max_col() || y < min_row() || y > max_row())
		return false;
	return life-- > 0;
}

/*
 * thrasher
 */

thrasher::thrasher(float x, float y, float velx, float vely)
		: phys_obj(x, y, 3.0, 1.0, true) {
	this->velx = velx; this->vely = vely;
	life = 70;
}


void thrasher::collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push) {
 //HACK: don't allow shoving, to prevent the smoke from messing it up.
/*	velx += x_push / force;
	vely += y_push / force;*/
// 	life -= other->force;
}
void thrasher::draw() {
	set_green();
	mvpaint(row(), col(), '*', this);
}

bool thrasher::tick() {
	new smoke(x-velx, y-vely, 1, 0, -.05, 40 + misc_rng(5));
	vely += misc_rng(10)/10.0 - 0.5;
	velx += misc_rng(30)/10.0 - 1.5;

	vely *= 0.65;
	velx *= 0.65;

	return life-- > 0;
}
