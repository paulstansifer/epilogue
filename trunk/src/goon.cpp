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
#include "goon.h"
#include "wave.h"
#include "paint.h"
#include "shot.h"
#include "explosion.h"
#include "epilogue.h"

#include "text.h"

#include <string>
#include <fstream>

void goon::create_goon_squad(int size, int x) {
	for(int i = 0; i < size; i++) {
		new goon(x, min_row() - 2*i);
	}
}

void goon::create_goon_squad(int size) {
	create_goon_squad(size, (max_col() + min_col()) / 2);
}

void goon::create_goons(ifstream * in) {
	int shields, qty;
	char shot_image, color;
	float force, shot_force;
	string * image_str = new string(); //we are so going to leak this.
	
	(*in) >> qty >> shields >> force >> shot_force >> shot_image >> color >> *image_str;

	for(int i = 0; i < qty/2; i++)
		new goon(min_col() + 5 + i, min_row() - 3*i, shields, force, shot_force, shot_image, color, image_str->c_str());
	
	for(int i = 0; i < (qty+1)/2; i++)
		new goon(max_col() - 5 - i, min_row() - 3*i, shields, force, shot_force, shot_image, color, image_str->c_str());

}

goon::goon(float x, float y) : ship(x, y, 3.0, 2.0) {
	shields = misc_rng(3) + 3;
	live_enemies++;
	this->shot_force = 2.5;
	this->shot_image = '.';
	this->shot_color = 'Y';
	this->image = "[=]";
	jiggly = true;
}

goon::goon(float x, float y, int shields, float force, float shot_force, int shot_image, int shot_color, 
					 const char * image) : ship(x, y, force, 2.0) {
	this->shields = misc_rng(shields/2) + shields;
	live_enemies++;
	this->shot_force = shot_force;
	this->shot_image = shot_image;
	this->shot_color = shot_color;
	this->image = image;
	jiggly = true;
}

void goon::draw() {
	set_gray();
	mvpaintstr(row(), col()-1, image, this);
}

bool goon::tick() {
	 cool();
	if( (the_hero->y - y) / fabs(the_hero->x - x) > 1 && rand() % (15 + 3*(int)(the_hero->y - y)) == 0) {
		/*if(rand() % 5 == 0)
			new lightning_bolt(x, y+1, 4, velx / 3.0 , vely / 3.0 + 0.3, (x-hero->x) * 0.0, 1.5, 30);
		else*/
			new shot(x, y+1, shot_force, velx/2.0, vely+1.2, shot_image, shot_color);
	}

	get_repulsion(velx, vely, true);

	if(y < min_row()) {
		if(run_away && y < min_row() - 2) {
			live_enemies--;
			return false;
		} else 
			vely = 0.75;
	}

	if(y > max_row())
		vely = -0.75;
/*
  if(x < min_col())
		velx = 1.5;
	if(x > max_col())
		velx = -1.5;
			*/
	
	if(run_away)
		vely = -0.75;

	if(velx > 1.5)
		velx = 1.5;
	else if (velx < -1.5)
		velx = -1.5;
	if(vely > 0.75)
		vely = 0.75;
	else if (vely < -0.75)
		vely = -0.75;

	if(shields <= 0) {
		new explosion(x,y, velx, vely);
		live_enemies--;
		return false;
	}
	return true;
}
