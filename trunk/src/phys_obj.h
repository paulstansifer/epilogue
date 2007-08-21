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
#ifndef PHYS_OBJ_H
#define PHYS_OBJ_H

#include <math.h>
#include <list>

using namespace std;

const float speed_scale = 0.7;

/**
 @author Paul Stansifer
 */
class phys_obj{
	public:
		float x, y;
		float velx, vely;
		
		inline int col() {return lrintf(x);}
		inline int row() {return lrintf(y);}
		virtual void effective_loc(float oth_x, float oth_y, float & eff_x, float & eff_y);

		/// Proportional to the damage done by a collision
		float force;

    phys_obj(float x, float y, float force, float repel_radius);
    phys_obj(float x, float y, float force, float repel_radius, bool back);

    ///generates a phys_obj isn't really physical.
    phys_obj();

    virtual ~phys_obj();

    static void tick_world();
		static void nudge_all(float dx, float dy);

    virtual void collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push) = 0;
 		//void internal_collide(phys_obj* other, int xoff, int yoff);

		void collide(int row, int col, phys_obj* other);
  protected:
		void get_repulsion(float & x, float & y, bool follow_hero);

  	static list<phys_obj*> world;

  	virtual void draw() = 0;
    ///Returns true if the object continues to exist
    /*virtual bool tick() = 0;*/
		virtual bool tick() {return true;} //for some reason, the above won't link


		///is this item ready to be removed?
    bool dead;

		float repel_radius;
		bool jiggly;

		static phys_obj* the_hero; //Sort of a hack: hero registers itself so that others know who it is.

  private:

    inline bool internal_tick() {
			x += velx * speed_scale;
			y += vely * speed_scale;
			return tick();
		}

};

inline float max(float a, float b) {
	if(a > b)
		return a;
	return b;
}

inline float min(float a, float b) {
	if(a < b)
		return a;
	return b;
}

void make_shot(float, float, float, float, float, int, int, int); //TMP

//closest point on the line defined by _x_, _y_, _dx_, and _dx_, to _px_ and _py_
inline void closest(float x, float y, float dx, float dy, float px, float py, float & cx, float &cy, bool segment) {
	//equations from http://local.wasp.uwa.edu.au/~pbourke/geometry/pointline/
	float udenom = dx*dx + dy*dy;
	if(udenom < 1e-4) { //that's no line...
		cx = x;  //fortunately, there's some kind of sort of well-defined answer 
		cy = y;
		return;
	}
	float u = ((px - x)*dx + (py - y)*dy) / udenom;
	if(segment) { //constrain the answer to the segment
		u = max(min(u, 1.0), 0.0);
	}
	cx = x + u*dx;
	cy = y + u*dy;
	
	
	/*
	int color;
	if(u < 0.25)
		color = 'R';
	else if(u < 0.75)
		color = 'Y';
	else
		color = 'G';
	
	for(float i = 0.0; i <= 0.9; i += 0.3) {
		float j = 1.0 - i;
		make_shot(px*i + cx*j, py*i + cy*j, 0.0, 0, 0, '.', color, 1);
	}
	make_shot(cx, cy, 0, 0, 0, 'X', color, 1);
	 //*/
}

inline float clip(float f, float max) {
	if(f > max)
		return max;
	if(f < -max)
		return -max;
	return f;
}




inline void phys_obj::collide(int row, int col, phys_obj* other) {
	//float x_diff = other->x - x;
	//float y_diff = other->y - y;
	float x_portion = 0, y_portion = 0;
	/*if(x_diff == 0 && y_diff == 0) {
		x_portion = 0; y_portion = 0;
	} else {
		x_portion = x_diff / (x_diff*x_diff + x_diff*x_diff);
		y_portion = y_diff / (y_diff*y_diff + y_diff*y_diff);
	}*/

	//cache the values at the moment of collision
	float other_velx = other->velx;
	float other_vely = other->vely;
	
	//Here we switch notation from "row (=y), col (=x)"  to  "x (=col), y (=row)"

	other->collide(this, lrint(col-other->x), lrint(row-other->y),   
								 clip((x_portion + velx/2.0)*force, 3)   / max(other->force, 0.3),  
								 clip((y_portion + vely/2.0)*force, 1.5) / max(other->force, 0.3));
	collide(other, lrint(col-x), lrint(row-y),  
					clip((-x_portion + other_velx/2.0)*other->force, 3)   / max(other->force, 0.3), 
					clip((-y_portion + other_vely/2.0)*other->force, 1.5) / max(other->force, 0.3));
}


#endif
