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
#ifndef WAVE_H
#define WAVE_H

#include <phys_obj.h>

/**
@author Paul Stansifer
*/
class wave : public phys_obj
{
public:
	wave(int goons, int asteroid_improbability);
	wave();
	
	static void make_waves(char * filename);

	void collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push);
protected:
  void draw();

  bool tick();

  int asteroid_improbability;
	int goons;


};

extern int live_enemies;
extern int capital_ships;
extern bool run_away;

#endif
