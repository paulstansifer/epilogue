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
#ifndef GOON_H
#define GOON_H

#include "phys_obj.h"
#include "ship.h"
/**
@author Paul Stansifer
*/
class goon : public ship {
	public:
		static void create_goon_squad(int size);
		static void create_goon_squad(int size, int x);
		
		static void create_goons(ifstream * in);
		goon(float x, float y, int shields, float force, float shot_force, int shot_image, int shot_color, const char * image);
		
    //now in _ship_
		//void collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push);
	protected:
    goon(float x, float y);

    void draw();
    bool tick();

		int color;
		const char * image;
		float top_speed_x; //the y direction is proportional
		int shot_image, shot_color;
		float shot_force;
};

#endif
