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

#include <vips/cimg_funcs.h>

#include "denoise.hh"
#include "../base/new_operation.hh"
#include "../operations/convertformat.hh"
#include "../operations/blender.hh"


PF::DenoisePar::DenoisePar(): 
  OpParBase(),
  iterations("iterations",this,1),
  amplitude("amplitude",this,100),
  sharpness("sharpness",this,0.9), 
  anisotropy("anisotropy",this,0.15),
  alpha("alpha",this,0.6),
  sigma("sigma",this,1.1),
	nr_mode("nr_mode",this,PF_NR_ANIBLUR,"ANIBLUR","Anisotropic Blur (G'Mic)")
{	
  set_type( "denoise" );
}



VipsImage* PF::DenoisePar::build(std::vector<VipsImage*>& in, int first, 
				   VipsImage* imap, VipsImage* omap, 
				   unsigned int& level)
{
  VipsImage* srcimg = NULL;
  if( in.size() > 0 ) srcimg = in[0];
  VipsImage* mask;
  VipsImage* out = srcimg;

  if( !out ) return NULL;

	if( (get_render_mode() == PF_RENDER_PREVIEW && level>0) ) {
		PF_REF( out, "PF::DenoisePar::build(): out ref" );
		return out;
	}

  int fast_approx = 0;
	if( (get_render_mode() == PF_RENDER_PREVIEW && level>0) ) 
    fast_approx = 1;
  out = vips_image_new();
  if( im_greyc_mask( srcimg, out, NULL, 
                     iterations.get(), amplitude.get(),
                     sharpness.get(), anisotropy.get(),
                     alpha.get(), sigma.get(), 0.8, 30, 2, 1, fast_approx ) )
    return NULL;

	return out;
}


PF::ProcessorBase* PF::new_denoise()
{
  return( new PF::Processor<PF::DenoisePar,PF::DenoiseProc>() );
}
