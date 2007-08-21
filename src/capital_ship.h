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
#ifndef CAPITAL_SHIP_H
#define CAPITAL_SHIP_H

#include "phys_obj.h"
#include "sprite.h"

#include <string>
#include <vector>
#include <list>
/**
@author Paul Stansifer
*/

using namespace std;

class capital_ship;

class ship_part {
public:
	int x, y;
	capital_ship* parent;
	ship_part(int x, int y, capital_ship* parent);
	virtual void tick();
	virtual ~ship_part();
};

class capital_ship : public phys_obj {
	public:
		void collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push);

		capital_ship(sprite & image, int x, int y, int shield);
		
		static void create_capitals(ifstream * in);

		float xoff, yoff;
		int structural_integrity;
		int cockpits;
		int engines, max_engines;
		
		sprite image;
	protected:
		bool tick();
		void draw();
		void find_stuff();
		
		vector<ship_part*>* parts; //why can't I make this not a pointer?  ("insufficient context to determine type")
		

		int overall_shield, max_overall_shield;


};




class gun : public ship_part {
public:
	gun(int x, int y, capital_ship* parent, float force, float speed, int bullet_image, int bullet_color, int reload_time);
	void tick();
protected:
	float force, speed;
	int image, color, heat;
	int reload_time;
};

class cockpit : public ship_part {
public:
	cockpit(int x, int y, capital_ship* parent);
	void tick();
	~cockpit();
};

class engine : public ship_part {
public:
	engine(int x, int y, capital_ship* parent);
	void tick();
	~engine();
};

#endif
