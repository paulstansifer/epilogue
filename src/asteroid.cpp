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
#include "asteroid.h"
#include "epilogue.h"
#include "paint.h"

asteroid::asteroid(float x, float y) : phys_obj(x, y, 5.0, 1) {
	vely = 0.3;
}

asteroid::asteroid(float x, float y, float force, float repel_radius, float vely) : phys_obj(x,y,force, repel_radius){
	this->vely = vely;
	this->velx = (misc_rng(10)-5)/25.0;
}

asteroid::asteroid(float x, float y, float force, float repel_radius, float velx, float vely) : phys_obj(x,y,force, repel_radius){
	this->vely = vely;
	this->velx = velx;
}


inline int max(int a, int b) {
	return a > b ? a : b;
}

void make_random_big_asteroid() {
	int x = misc_rng(max_col() - min_col()) + min_col();
	int xsize = misc_rng(15) + 2;
	int ysize = misc_rng(7) + 1;
	new big_asteroid(x, min_row(), xsize, ysize);
}

void make_random_asteroid(int max_x_size) {
	int x = misc_rng(max_col() - min_col()) + min_col();
	int xsize = misc_rng(max_x_size-2) + 2;
	int ysize = misc_rng(max_x_size/2 - 1) + 1;
	new big_asteroid(x, min_row(), xsize, ysize);
}

									 

big_asteroid::big_asteroid(float x, float y, int xsize, int ysize) : asteroid(x, y, xsize*ysize, max(xsize, ysize)*1.2, 0.3 + (misc_rng(10)-5)/25.0) {
	structural_integrity = (int) ((xsize*ysize)*0.8);
	image = new sprite(xsize, ysize);
	this->xsize = xsize;
	this->ysize = ysize;
}

big_asteroid::big_asteroid(float x, float y, float xvel, float yvel, int xsize, int ysize) : asteroid(x, y, xsize*ysize, max(xsize, ysize)*1.2, xvel, yvel) {
	structural_integrity = (int) ((xsize*ysize)*0.8);
	image = new sprite(xsize, ysize);
	this->xsize = xsize;
	this->ysize = ysize;
}


void asteroid::draw() {
	set_gray();
	mvpaint(row(), col(), '*', this);
}

void big_asteroid::draw() {
	image->draw((int)x-xsize/2, (int)y-ysize/2, this);
}

void asteroid::collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push) {
	dead = true;
}

void big_asteroid::collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push) {
	velx += x_push;
	vely += y_push;
	
	structural_integrity -= (int)other->force;
}

bool asteroid::tick() {
	if(y >= max_row())
		return false;
	return true;
}

bool big_asteroid::tick() {
	if(structural_integrity < 0) {
		new explosion(x, y, velx, vely);
		delete image;
		return false;
	}
	if(y >= max_row() + ysize || x >= max_col() + xsize ||
		 y < min_row() - ysize || x < min_col() - xsize)
		return false;
	return true;
}


