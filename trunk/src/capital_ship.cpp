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
#include "capital_ship.h"
#include "wave.h"
#include "sprite.h"
#include "paint.h"
#include "explosion.h"
#include "epilogue.h"
#include "shot.h"
#include "text.h"

#include <fstream>

capital_ship::capital_ship(sprite & image, int x, int y, int shield) : phys_obj(x,y, 0, 0) {
	this->image = image;
	xoff = -image.xsize / 2;
	yoff = -image.ysize / 2;

	repel_radius = (image.xsize > image.ysize?  image.xsize : image.ysize) * 2;
	jiggly = true;
	force = image.xsize + image.ysize;

	overall_shield = shield;
	max_overall_shield = shield;

	structural_integrity = shield / 2;
	
	parts = new vector<ship_part*>();
	cockpits = 0;
	find_stuff();
	
	live_enemies++;
	capital_ships++;
	
}

void capital_ship::create_capitals(ifstream * in) {
	int shields, qty;

	(*in) >> qty >> shields;
	
	sprite cap_image = sprite(*in); //capital_ship() copies its sprite
	
	for(int i = 0; i < qty; i++)
		new capital_ship(cap_image, (min_col() + max_col())/2, min_row() - 10*i, shields);
}


void capital_ship::find_stuff() {
	for(int x = 0; x < image.xsize; x++) {
		for(int y = 0; y < image.ysize; y++) {
			switch(image.char_at(x, y)) {
				case 'o':
					parts->push_back(new cockpit(x, y, this)); break;
				case 'p': case 'q':
					parts->push_back(new gun(x, y, this, 6, 0.8, '\'', 'B', 50)); break;
				case '"':
					parts->push_back(new gun(x, y, this, 6, 0.8, ':', 'r', 50)); break;
				case '[':
					if(x + 4 >= image.xsize)
						break;
					if(image.char_at(x+1, y) == ' ' &&
						 image.char_at(x+2, y) == ' ' &&
						 image.char_at(x+3, y) == ' ' &&
						 image.char_at(x+4, y) == ']') {
						
					}
					break;
				case '~': case '6': case '9':
					parts->push_back(new engine(x, y, this)); break;
			}
		}
	}
}

bool capital_ship::tick() {
	for(vector<ship_part*>::iterator i = parts->begin(); i != parts->end(); i++) {
		(*i)->tick();
	}
	get_repulsion(velx, vely, true);

	if(y < min_row())
		vely = 1.5;
	if(y > max_row())
		vely = -1.5;
	if(x < min_col())
		velx = 1.5;
	if(x > max_col())
		velx = -1.5;


	if(velx > 1.5)
		velx = 1.5;
	else if (velx < -1.5)
		velx = -1.5;
	if(vely > 1.5)
		vely = 1.5;
	else if (vely < -1.5)
		vely = -1.5;

	if(structural_integrity <= 0) {
		new explosion(x, y, velx-0.5, vely-0.3);
		new explosion(x, y, velx+0.5, vely-0.3);
		new explosion(x, y, velx-0.5, vely+0.3);
		new explosion(x, y, velx+0.5, vely+0.3);
		capital_ships--;
		live_enemies--;
		return false;
	}
	return true;
}

void capital_ship::draw() {
	image.draw(col()+(int)xoff, row()+(int)yoff, this);
}

void capital_ship::collide(phys_obj* other, int xoff_sprite, int yoff_sprite, float x_push, float y_push){
  velx += x_push;
  vely += y_push;
	if(overall_shield > 0)
		overall_shield -= (int)other->force;
	else {
		int x_hit = xoff_sprite-lrint(xoff);
		int y_hit = yoff_sprite-lrint(yoff);
		
		//sometimes x_hit or y_hit are actually off the image, which would cause a crash
		//TODO improve the calculations so they don't do that.
		if(x_hit >= 0 && y_hit >= 0 && x_hit < image.xsize && y_hit < image.ysize) { 
			image.remove_character(x_hit, y_hit);
			for(vector<ship_part*>::iterator i = parts->begin(); i != parts->end(); i++) {
				if((*i)->x == x_hit && (*i)->y == y_hit) {
					delete (*i);
					parts->erase(i);
					break;
				}
			}
		}
		structural_integrity -= (int)other->force;
	}
	
}

ship_part::ship_part(int x, int y, capital_ship * parent) {
	this->x = x;
	this->y = y;
	this->parent = parent;
}

//default, do nothing
ship_part::~ship_part(){
}

void ship_part::tick() {
}

gun::gun(int x, int y, capital_ship* parent, float force, float speed, int bullet_image, int bullet_color, int reload_time) : ship_part(x,y, parent) {
	this->force = force;
	this->speed = speed;
	image = bullet_image;
	color = bullet_color;
	
	this->reload_time = reload_time;
	heat = 0;
}

void gun::tick() {
	if(heat > 0) {
		heat--;
		return;
	}
	
	int actual_x = (int)(parent->x + parent->xoff + x);
	int actual_y = (int)(parent->y + parent->yoff + y);
	
	if( (protagonist->y - actual_y) / fabs(actual_x - protagonist->x) > 1) {		
		new shot(actual_x, actual_y+1, force, parent->velx/2.0, parent->vely + speed, image, color);
		
		heat = reload_time;
	}
}

cockpit::cockpit(int x, int y, capital_ship * parent) : ship_part(x,y,parent) {
	parent->cockpits++;
}

void cockpit::tick() {
}

cockpit::~cockpit() {
	if(--parent->cockpits)
		parent->structural_integrity = 0;
}



engine::engine(int x, int y, capital_ship * parent) : ship_part(x,y,parent) {
	parent->engines++;
	parent->max_engines++;
}

void engine::tick() {
	int color = parent->image.color_at(x,y);
	int character = parent->image.char_at(x,y);
	if(color == 'r')
		color = 'y';
	else if(color == 'y')
		color = 'r';
	else if(color == 'b')
		color = 'g';
	else if(color == 'g')
		color = 'b';
	
	if(character == '6')
		character = '9';
	else if(character == '9')
		character = '6';
	
	parent->image.edit(x,y,character,color);
}

engine::~engine() {
	--parent->engines;
	//TODO: do something different
}