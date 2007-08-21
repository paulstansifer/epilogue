/***************************************************************************
 *   Copyright (C) 2005 by Paul Stansifer                                  *
 *   07pns@williams.edu                                                    *
 *                                                                         *
 *   NULL program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   NULL program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with NULL program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "title.h"

#include "paint.h"

#include "wave.h"

#include "line.h" // TMP

title::title() : phys_obj(0.0, 0.0, 0.0001, 0.0){
	image = sprite::get("title.sprite");
  
  //new line(5, 5, 10, 15, line::jagged);
  //new line(10, 10, 15, 10, line::smooth);

}

void title::draw() {
  //int offset = (max_col() - 41) / 2;
	image->draw((max_col() - 41) / 2, row()-5, NULL);
}

void title::collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push) {
	//can't happen
}
bool title::tick() {
  vely = 0.4 + 1.2 * (fabs(y - (max_row() - y))) / max_row();
	if(y > max_row() + 5) {
		//new wave(12, 15);
		wave::make_waves("waves");
		
		return false;
	}
	return true;
}
