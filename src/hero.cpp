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
#include "hero.h"

#include <ncurses.h>
#include "epilogue.h"
#include "paint.h"
#include "shot.h"
#include "ship.h"

#include "asteroid.h" //debugging

phys_obj* phys_obj::the_hero;
                /*shields*/
hero::hero() : ship(30, (min_col() + max_col()) / 2, max_row(), 10.0, 2) {
	max_shields = 30;
	max_energy = 30.0;
	recharge = 0.5;
	phys_obj::the_hero = this;

	energy = 10;
	
	key = ' ';
	effective_velx = 0;
}

void hero::collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push) {
  velx += x_push;
  vely += y_push;
  if(other->force > collision_cooler) {
    shields -= other->force - collision_cooler;
    collision_cooler = other->force;
  }
}

void hero::boost() {
	max_energy += 10.0;
	recharge += 0.15;
}

void hero::input_and_refresh() {
	key = getch(); //this refreshes the screen.
}

void hero::emit(phys_obj* p) {
  p->velx += effective_velx;
  p->vely += vely;
  p->y -= 0.5;
}

bool hero::tick() {
  ship::cool();
	//velx *= 0.95; vely *= 0.95;
	
	//phys_obj::nudge_all(-velx,-vely);
	
	float x_prop = (velx > 0 ? max_col()-x : x-min_col()) / (max_col() - min_col())  * 2; //system to scroll with the hero, the more it goes off to the side.
	
	phys_obj::nudge_all(-velx * (1-x_prop),0);
	//y_prop = min(x-min_col(), max_col()-x) / (max_col() - min_col())  * 2;
	
	effective_velx = effective_velx * 0.85 + velx * 0.15;
	
	energy += recharge;
	if(energy > max_energy) {
		shields += (energy - max_energy) * 0.10;
		energy = max_energy;
		
		if(shields > max_shields)
			shields = max_shields;
	}
  
	
	//new asteroid(2,1.0); //debugging -- why does hitting it destroy everything?


	//process input
	if(key == KEY_LEFT){
		if(velx > 0.0)
			velx = 0.0;
		else
			velx = -1.25;
	} else if(key == KEY_RIGHT){
		if(velx < 0.0)
			velx = 0.0;
		else
			velx = 1.25;
	} else if(key == KEY_DOWN) {
		if(vely < 0.0)
			vely = 0.0;
		else
			vely = 0.75;
	} else if(key == KEY_UP) {

		if(vely > 0.0)
			vely = 0.0;
		else
			vely = -0.75;
	} else if (key == ' ') {
		velx = vely = 0;
	} else if (key == 'a' && energy*5 >= max_energy) {
    float power = (max_energy / 5.0);
    int extras = 2 + (int)(max_energy/50);
    if(extras > 5)
      extras = 5;
    //emit(new shot(x, y, power, 0,     -1.0, '\'', 'W'));
    for(int i = 0; i < extras; i++) {
      emit(new shot(x-0.8*i - 0.4, y, power, -.2*i - .1, -1.0+ .02*i*i, '\'', 'W'));
      emit(new shot(x+0.8*i + 0.4, y, power,  .2*i + .1, -1.0+ .02*i*i, '\'', 'W'));
    }

		energy -= power;
	} else if (key == 's' && energy*2 >= max_energy) {
    float power = (max_energy / 2.0);
		new shot(x-1.0, y-0.5, power*1.4, effective_velx, vely - 3.0, '|', 'Y');
		new shot(x+1.0, y-0.5, power*1.4, effective_velx, vely - 3.0, '|', 'Y');
  
    energy -= power;
	} else if (key == 'm' && energy >= 45) {
		float ast_x = x + misc_rng(40) - 20;
		float ast_y = min_col()-1;  //min_col() - 20 - misc_rng(10);
		new big_asteroid(ast_x, ast_y, (x-ast_x)/18.0, (y-ast_y)/18.0, 12, 6);
		
		energy -= 45;
	} else if(key ==  '1' && energy >= 5) {
		new shot(x-1.0, y-0.5, 2.5, effective_velx, -1.0 + vely, '|', 'W');
		new shot(x+1.0, y-0.5, 2.5, effective_velx, -1.0 + vely, '|', 'W');
		energy -= 5;
	} else if(key == '2' && energy >= 25) {
		new thrasher(x, y-2.5, effective_velx, vely - 0.5);
		energy -= 25;
	} else if(key == '3' && energy >= 30) {
		new shot(x-1.0, y-0.6, 7.5, effective_velx, -0.8 + vely, '"', 'r');
		new shot(x+1.0, y-0.6, 7.5, effective_velx, -0.8 + vely, '"', 'r');
		energy -= 30;
	} else if(key == 'l' && energy >= 50) {
		new lightning_bolt(x, y-2.0, 6.0, effective_velx, vely, 0, -1.5, 30);
		energy -= 50;
	} else if (key == 'C') {
		max_energy += 20.0;
		recharge += 0.5;

	} else if (key == 'q') {
	  end_everything();
	}
	
	if(x < min_col() + 10) {
		phys_obj::nudge_all(1,0);
	} else if (x > max_col() - 10) {
		phys_obj::nudge_all(-1,0);
	}
/*	if(x < min_col()+1) {
		x = min_col()+1; velx = 0;
	}
	if(x > max_col()-2) {
	  x = max_col()-2; velx = 0;
	}  */
	
	if(y < min_row()) {
		y = min_row(); vely = 0;
	}
	if(y > max_row()) {
	  y = max_row(); vely = 0;
	}  

	if(shields <= 0) {
		new explosion(x, y, velx, vely);
		end_everything();
		return false;
	}
	return true;
}

void hero::draw() {
	set_white();
	mvpaintstr(row(), col()-1, "(-)", this);

	if((float)shields / (float)max_shields > 0.7)
		set_green();
	else if((float)shields / (float)max_shields > 0.35)
		set_yellow();
	else
		set_pink();
	for(int i = 0; i < ((float)shields / (float)max_shields) * max_row(); i++)
		mvaddch(i, COLS-1, ACS_BLOCK);
	
	set_blue();
	
	for(int i = 0; i < ((float)energy / (float)max_energy) * max_row(); i++)
		mvaddch(i, 0, ACS_BLOCK);
}



