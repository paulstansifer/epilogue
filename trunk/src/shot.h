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
#ifndef SHOT_H
#define SHOT_H

#include <phys_obj.h>
#include "sprite.h"

/**
@author Paul Stansifer
*/
class shot : public phys_obj {
public:
  shot(float x, float y, float force, float xvel, float yvel, int image, int color);
  shot(float x, float y, float force, float velx, float vely, int image, int color, int life);


  void collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push);
protected:

  void draw();
  bool tick();

  int color, image;
  int life;
};


class lightning_bolt : public phys_obj {
	public:
    lightning_bolt(float x, float y, float force, float xvel, float yvel, float end_xvel, float end_yvel, int life);

		void collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push);
	protected:

    void draw();
    bool tick();


    int random_dot();

    float end_xvel, end_yvel;
    float end_x, end_y;
		int life;

		sprite frame1;
		sprite frame2;
};

class smoke : public phys_obj {
	public:
    smoke(float x, float y, float force, float xvel, float yvel, int life);
		smoke(float x, float y, float force, int life);


		void collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push);
	protected:

    void draw();
    bool tick();

    int life;
};

class thrasher : public phys_obj {
	public:
    thrasher(float x, float y, float xvel, float yvel);

		void collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push);
	protected:

    void draw();
    bool tick();
    
    int life;
};

#endif
