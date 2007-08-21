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
#include "explosion.h"

#include "paint.h"
#include "shot.h"
#include "epilogue.h"

//TODO: make debris non-effects, so they can be shot down
void explosion::fire_debris() {
	float xdir, ydir, xvel, yvel;

	if(rand_bool())
		xdir = -1.0;
	else
		xdir = 1.0;

	if(rand_bool())
		ydir = -1.0;
	else
		ydir = 1.0;
	
	do {
		xvel = misc_rng(10)/5.0 * xdir;
		yvel = misc_rng(10)/10.0 * ydir;
	} while( fabsf(xvel) + fabsf(yvel) < 0.6 ); 

	if(rand_bool())
		new shot(x+xdir, y+ydir, 2.0, xvel, yvel, '.', 'W');
	else
		new shot(x+xdir, y+ydir, 7.0, xvel, yvel, '%', 'w');
}

explosion::explosion(float x, float y, float velx, float vely) : phys_obj(x, y, 25.0, 1) {
	frame = 0;
	this->velx = velx;
	this->vely = vely;
	rand_bits = rand();

	for(int i = 0; i < 4; i++)
		fire_debris();
}

bool explosion::tick() {
	force = (11.0-frame) / 11.0; // decreasing potency as time goes on

	if(!rand_bool()) // true is more likely, for rand_bool()
		frame--;
	
	//Uh, what was this for?
	//if(frame < 12 && frame % 2 == 1 && rand_bool()) {
	//}
	
	if(frame > 9){
		new smoke(x,y,0,velx,vely,40);

	}	
	

	return frame++ < 11;
}

void explosion::collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push) {
	//pfft!  we're already an explosion
}

void explosion::draw() {
	if(frame < 4)
		set_white();
	else if(frame < 8)
		set_yellow();
	else //if(frame < 15)
		set_red();
/*	else if(frame < 19)
		set_brown();
	else if(frame < 22)
		set_gray();
	else
		set_dark_gray();*/

	switch(frame/2) {
	case 0: case 1:
		mvpaint_effect(row(), col(), '*', this);        break;
	case 2: case 4:
		mvpaintstr_effect(row(), col()-1, "()", this);  break;
	case 3:
		mvpaintstr_effect(row(), col(), "()", this);    break;
	case 5: case 6:
		mvpaintstr_effect(row(), col()-1, "( )", this); break;
	case 7: case 9:
		mvpaintstr_effect(row(), col()-1, ">*<", this); break;
	case 8: case 10:
	  mvpaint_effect(row()-1, col(), '|', this);
	  mvpaintstr_effect(row(), col()-1, ")x(", this);
	  mvpaint_effect(row()+1, col(), '|', this); break;
	/*case 9: case 10: case 11:
		if(rand_bool()) {
	  	mvpaintstr_effect(row()-1, col()-1, ".:,", this);
	  	mvpaintstr_effect(row(), col()-1,   ";%.", this);
	  	mvpaintstr_effect(row()+1, col()-1, "`;'", this);
	  } else {
	  	mvpaintstr_effect(row()-1, col()-1, ",;.", this);
	  	mvpaintstr_effect(row(), col()-2,  ".:%:'", this);
	  	mvpaintstr_effect(row()+1, col()-1, "':'", this);
		}
	  break;
	default:
		if(rand_bool()) {
			mvpaintstr_effect(row()-1, col()-1, "o0)", this);
			mvpaintstr_effect(row(), col()-2,  "'(oO)o", this);
			mvpaintstr_effect(row()+1, col()-1, "o)o`", this);
		} else {
			mvpaintstr_effect(row()-1, col()-1, "O)o", this);
			mvpaintstr_effect(row(), col()-2,  ".o(Oo)", this);
			mvpaintstr_effect(row()+1, col()-1, "'('o", this);
		}*/
	}
}

