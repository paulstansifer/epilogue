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
#include "text.h"

#include "paint.h"

text::text(const char * text) : phys_obj(0.0, 0.0, 0.0001, 0.0){
	printing = (char *)malloc(strlen(text)+1);
	strcpy(printing, text);
}

void text::draw() {
  //int offset = (max_col() - 41) / 2;
	set_white();
	mvpaintstr(row()-5, 3, printing, NULL); //so it won't collide with anything
}

void text::collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push) {
}

bool text::tick() {
  vely = 0.2 + 0.7 * (fabs(y - (max_row() - y))) / max_row();
	if(y > max_row() + 5) {
		free(printing);
		return false;
	}
	return true;
}
