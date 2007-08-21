/***************************************************************************
 *   Copyright (C) 2005 by Paul Stansifer   *
 *   07pns@williams.edu   *
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
#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <phys_obj.h>

/**
@author Paul Stansifer
*/
class explosion : public phys_obj {
public:
    //explosion(float x, float y);
    explosion(float x, float y, float velx = 0.0, float vely = 0.0);

    virtual void collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push);

protected:
    virtual bool tick();
    virtual void draw();

    int frame;
    int rand_bits;

    //not exactly 50-50, but requires fewer rand()s
  	inline bool rand_bool() {
			bool ret_val = rand_bits % 2 == 1;
			rand_bits /= 2;
			if(rand_bits == 0) //drat! we ran out
				rand_bits = rand();
			return ret_val;
		}

 		void fire_debris();
};

//class big_explosion : public phys_obj {
//public:
//		big_explosion(float x, float y, float velx = 0.0, float vely = 0.0);



#endif
