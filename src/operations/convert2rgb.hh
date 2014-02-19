/* 
 */

/*

    Copyright (C) 2014 Ferrero Andrea

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.


 */

/*

    These files are distributed with PhotoFlow - http://aferrero2707.github.io/PhotoFlow/

 */

#ifndef CONVERT_2_RGB_H
#define CONVERT_2_RGB_H

#include <iostream>

#include <lcms2.h>

#include "../base/format_info.hh"
#include "../base/operation.hh"

namespace PF 
{

  class Convert2RGBPar: public OpParBase
  {
    cmsHPROFILE profile_in;
    cmsHPROFILE profile_out;
    cmsHTRANSFORM transform;

  public:
    Convert2RGBPar();
    bool has_imap() { return false; }
    bool has_omap() { return false; }
    bool needs_input() { return true; }

    cmsHTRANSFORM get_transform() { return transform; }

    void set_image_hints( VipsImage* img )
    {
      if( !img ) return;
      OpParBase::set_image_hints( img );
      rgb_image( get_xsize(), get_ysize(), get_format() );
    }

    VipsImage* build(std::vector<VipsImage*>& in, int first, VipsImage* imap, VipsImage* omap);
  };

  

  template < OP_TEMPLATE_DEF > 
  class Convert2RGBProc
  {
  public: 
    void render(VipsRegion** in, int n, int in_first,
		VipsRegion* imap, VipsRegion* omap, 
		VipsRegion* out, Convert2RGBPar* par);
  };


  template< OP_TEMPLATE_DEF >
  void Convert2RGBProc< OP_TEMPLATE_IMP >::
  render(VipsRegion** ir, int n, int in_first,
	 VipsRegion* imap, VipsRegion* omap, 
	 VipsRegion* oreg, Convert2RGBPar* par)
  {
    Rect *r = &oreg->valid;
    int width = r->width;
    int height = r->height;
    int line_size = width * oreg->im->Bands; //layer->in_all[0]->Bands; 
    cmsHTRANSFORM transform = par->get_transform();

    T* p;    
    T* pout;
    int y;

    for( y = 0; y < height; y++ ) {
      
      p = ir ? (T*)VIPS_REGION_ADDR( ir[0], r->left, r->top + y ) : NULL; 
      pout = (T*)VIPS_REGION_ADDR( oreg, r->left, r->top + y ); 
      if(transform) cmsDoTransform( transform, p, pout, width );
      else memcpy( pout, p, sizeof(T)*line_size );
    }
  };


}

#endif 


