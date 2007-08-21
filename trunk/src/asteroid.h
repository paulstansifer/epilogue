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
#ifndef ASTEROID_H
#define ASTEROID_H

#include "phys_obj.h"
#include "sprite.h"
#include "explosion.h"

/**
@author Paul Stansifer
*/
class asteroid : public phys_obj {
	public:
    asteroid(float x, float y);
		asteroid(float x, float y, float force, float repel_radius, float vely);
		asteroid(float x, float y, float force, float repel_radius, float velx, float vely);

		virtual void draw();
		virtual void collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push);
		virtual bool tick();

		
};

class big_asteroid : public asteroid {	
public:
	big_asteroid(float x, float y, int xsize, int ysize);
	big_asteroid(float x, float y, float xvel, float yvel, int xsize, int ysize);
	
	void draw();
	void collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push);
	bool tick();
protected:
	sprite * image;
	int structural_integrity;
	int xsize, ysize;
};

void make_random_big_asteroid();
#endif
