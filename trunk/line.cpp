/*
 *  line.cpp
 *  epilogue
 *
 *  Created by Paul Stansifer on 2007-08-19.
 */

#include "line.h"
#include "paint.h"

#include <iostream> //TMP

line::line(float x, float y, float dx, float dy, draw_style style) : phys_obj(x, y, 20.0, 3) {
  if(style == jagged) {
    repel_radius += 1.5;
    force += 5.0;
  }
  this->dx = dx;
  this->dy = dy;
}

bool near(float a, float b) {
  return fabsf(a - b) < 1e-5;
}

void line::draw() {
  //int ix = (int)x, iy = (int)y;
  //int fx = ix+(int)(dx), fy = iy+(int)(dy);
  float ix = x, iy = y;
  float fx = x+dx,  fy = y+dy;
  

  set_red();
  if(fabsf(dx) > fabsf(dy)) {
    int ody = dy > 0 ?  1  :  -1;
    int xdir = dx > 0 ? 1  :  -1;
    for( ; !near(ix, fx) ; ix += xdir) {
      if(fabsf((ix-x)*(dy/dx)+y - iy) > 0.5)
        iy += ody;
      if(fabsf((ix-x)*(dy/dx)+y - iy) > 0.5)
        cout << "["<< (int)(ix - x) <<"[" << (dy/dx) << " " << (ix-x)*(dy/dx) << " " << (ix-x)*(dy/dx)+y - iy <<  "]]]           ";

      mvpaint((int)iy, (int)ix, '*', this);
    }
  } else {
    int odx = dx > 0 ?  1  :  -1;
    int ydir = dy > 0 ? 1  :  -1;
    for( ; !near(iy, fy) ; iy += ydir) {
      if(fabsf((iy-y)*(dx/dy)+x - ix) > 0.5)
        ix += odx;
      mvpaint((int)iy, (int)ix, '*', this);
    }
    //cout << "[[[" << iy << " " << ix << "]]]";
  }
}

void line::effective_loc(float oth_x, float oth_y, float & eff_x, float & eff_y) {
  closest(x, y, dx, dy, oth_x, oth_y, eff_x, eff_y, true);
}

void line::collide(phys_obj* other, int xoff, int yoff, float x_push, float y_push) {
  //bah!  Well, for now.
}
