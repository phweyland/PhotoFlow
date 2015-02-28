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

#include "../../base/imageprocessor.hh"
#include "../../operations/draw.hh"

#include "draw_config.hh"


PF::DrawConfigDialog::DrawConfigDialog( PF::Layer* layer ):
  OperationConfigDialog( layer, "Draw" ),
  pen_color_label("Pen color:              "),
  bgd_color_label("Background color: "),
#ifdef GTKMM_2
  pen_color_button( Gdk::Color("white") ),
  bgd_color_button( Gdk::Color("black") ),
#endif
#ifdef GTKMM_3
  pen_color_button( Gdk::RGBA("white") ),
  bgd_color_button( Gdk::RGBA("black") ),
#endif
  pen_size( this, "pen_size", "Pen size: ", 5, 0, 1000000, 1, 10, 1),
  pen_opacity( this, "pen_opacity", "Pen opacity: ", 100, 0, 100, 0.1, 1, 100)
{
  colorButtonsBox1.pack_start( bgd_color_label, Gtk::PACK_SHRINK );
  colorButtonsBox1.pack_start( bgd_color_button, Gtk::PACK_SHRINK );
  colorButtonsBox2.pack_start( pen_color_label, Gtk::PACK_SHRINK );
  colorButtonsBox2.pack_start( pen_color_button, Gtk::PACK_SHRINK );
  colorButtonsBox2.pack_start( pen_size, Gtk::PACK_SHRINK );
  controlsBox.pack_start( colorButtonsBox1 );
  controlsBox.pack_start( colorButtonsBox2 );
  //penBox.pack_start( pen_opacity );
  controlsBox.pack_start( penBox );

  /*
  controlsBox.pack_start( pen_grey );
  controlsBox.pack_start( pen_R );
  controlsBox.pack_start( pen_G );
  controlsBox.pack_start( pen_B );
  
  controlsBox.pack_start( bgd_grey );
  controlsBox.pack_start( bgd_R );
  controlsBox.pack_start( bgd_G );
  controlsBox.pack_start( bgd_B );
  */  

  pen_color_button.signal_color_set().
    connect( sigc::mem_fun(this, &PF::DrawConfigDialog::on_pen_color_changed) );
  bgd_color_button.signal_color_set().
    connect( sigc::mem_fun(this, &PF::DrawConfigDialog::on_bgd_color_changed) );

  add_widget( controlsBox );
}



void PF::DrawConfigDialog::open()
{
  if( get_layer() && get_layer()->get_image() && 
      get_layer()->get_processor() &&
      get_layer()->get_processor()->get_par() ) {
    /*
    pen_grey.init();
    pen_R.init();
    pen_G.init();
    pen_B.init();

    bgd_grey.init();
    bgd_R.init();
    bgd_G.init();
    bgd_B.init();
    */
    pen_size.init();
    pen_opacity.init();
  }
  OperationConfigDialog::open();
}


void PF::DrawConfigDialog::on_pen_color_changed()
{
  // Pointer to the associated Layer object
  PF::Layer* layer = get_layer();
  if( !layer ) return;

  // First of all, the new stroke is recorded by the "master" operation
  PF::ProcessorBase* processor = layer->get_processor();
  if( !processor || !(processor->get_par()) ) return;
  
  PF::DrawPar* par = dynamic_cast<PF::DrawPar*>( processor->get_par() );
  if( !par ) return;
  
#ifdef GTKMM_2
  float value = pen_color_button.get_color().get_red();
  par->get_pen_color().get().r = value/65535;
  value = pen_color_button.get_color().get_green();
  par->get_pen_color().get().g = value/65535;
  value = pen_color_button.get_color().get_blue();
  par->get_pen_color().get().b = value/65535;
#endif

#ifdef GTKMM_3
  par->get_pen_color().get().r = pen_color_button.get_rgba().get_red();
  par->get_pen_color().get().g = pen_color_button.get_rgba().get_green();
  par->get_pen_color().get().b = pen_color_button.get_rgba().get_blue();
#endif

  if( layer->get_image() )
    layer->get_image()->update();
}


void PF::DrawConfigDialog::on_bgd_color_changed()
{
  // Pointer to the associated Layer object
  PF::Layer* layer = get_layer();
  if( !layer ) return;

  // First of all, the new stroke is recorded by the "master" operation
  PF::ProcessorBase* processor = layer->get_processor();
  if( !processor || !(processor->get_par()) ) return;
  
  PF::DrawPar* par = dynamic_cast<PF::DrawPar*>( processor->get_par() );
  if( !par ) return;
  
#ifdef GTKMM_2
  float value = pen_color_button.get_color().get_red();
  par->get_pen_color().get().r = value/65535;
  value = pen_color_button.get_color().get_green();
  par->get_pen_color().get().g = value/65535;
  value = pen_color_button.get_color().get_blue();
  par->get_pen_color().get().b = value/65535;
#endif

#ifdef GTKMM_3
  par->get_bgd_color().get().r = bgd_color_button.get_rgba().get_red();
  par->get_bgd_color().get().g = bgd_color_button.get_rgba().get_green();
  par->get_bgd_color().get().b = bgd_color_button.get_rgba().get_blue();
#endif

  if( layer->get_image() )
    layer->get_image()->update();
	std::cout<<"DrawConfigDialog::on_bgd_color_changed(): image updated"<<std::endl;
}


void PF::DrawConfigDialog::start_stroke()
{
  // Pointer to the associated Layer object
  PF::Layer* layer = get_layer();
  if( !layer ) return;

  // First of all, the new stroke is recorded by the "master" operation
  PF::ProcessorBase* processor = layer->get_processor();
  if( !processor || !(processor->get_par()) ) return;
  
  PF::DrawPar* par = dynamic_cast<PF::DrawPar*>( processor->get_par() );
  if( !par ) return;
  
  //par->start_stroke( get_pen_size(), get_pen_opacity() );
  par->start_stroke();


  // Then we loop over all the operations associated to the 
  // layer in the different pipelines and we let them record the stroke as well
  PF::Image* image = layer->get_image();
  if( !image ) return;

  for( unsigned int vi = 0; vi < image->get_npipelines(); vi++ ) {
    PF::Pipeline* pipeline = image->get_pipeline( vi );
    if( !pipeline ) continue;

    PF::PipelineNode* node = pipeline->get_node( layer->get_id() );
    if( !node ) continue;

    processor = node->processor;
    if( !processor || !(processor->get_par()) ) continue;

    par = dynamic_cast<PF::DrawPar*>( processor->get_par() );
    if( !par ) continue;

    //par->start_stroke( get_pen_size(), get_pen_opacity() );
    par->start_stroke();
  }
}


void PF::DrawConfigDialog::draw_point( double x, double y )
{
  // Pointer to the associated Layer object
  PF::Layer* layer = get_layer();
  if( !layer ) return;

  // First of all, the new stroke is recorded by the "master" operation
  PF::ProcessorBase* processor = layer->get_processor();
  if( !processor || !(processor->get_par()) ) return;
  
  PF::DrawPar* par = dynamic_cast<PF::DrawPar*>( processor->get_par() );
  if( !par ) return;
  
  VipsRect update = {0,0,0,0};
  //std::cout<<"DrawConfigDialog::draw_point( "<<x<<", "<<y<<" )"<<std::endl;
  par->draw_point( x, y, update );

  if( (update.width < 1) || (update.height < 1) )  
    return;

  // Then we loop over all the operations associated to the 
  // layer in the different pipelines and we let them record the stroke as well
  PF::Image* image = layer->get_image();
  if( !image ) return;

#ifndef NDEBUG
	std::cout<<"PF::DrawConfigDialog::draw_point(): npipelines="<<image->get_npipelines()<<std::endl;
#endif
  for( unsigned int vi = 0; vi < image->get_npipelines(); vi++ ) {
    PF::Pipeline* pipeline = image->get_pipeline( vi );
    if( !pipeline ) continue;

    PF::PipelineNode* node = pipeline->get_node( layer->get_id() );
    if( !node ) continue;

    processor = node->processor;
    if( !processor || !(processor->get_par()) ) continue;

    par = dynamic_cast<PF::DrawPar*>( processor->get_par() );
    if( !par ) continue;

    par->draw_point( x, y, update );
		//continue;

		/**/
    if( (update.width > 0) &&
				(update.height > 0) ) {
      if( PF::PhotoFlow::Instance().is_batch() ) {
				pipeline->sink( update );	
      } else {
				ProcessRequestInfo request;
				request.pipeline = pipeline;
				request.request = PF::IMAGE_UPDATE;
				request.area.left = update.left;
				request.area.top = update.top;
				request.area.width = update.width;
				request.area.height = update.height;
#ifndef NDEBUG
				std::cout<<"PF::DrawConfigDialog::draw_point(): submitting rebuild request with area."<<std::endl;
#endif
				PF::ImageProcessor::Instance().submit_request( request );
      }
    }
		/**/
  }
	//exit(1);

	/*
	std::cout<<"DrawConfigDialog::draw_point("<<x<<","<<y<<"): area = "
					 <<update.width<<","<<update.height<<"+"<<update.left<<","<<update.top<<std::endl;
	image->update( &update );
	//image->update_all();
	*/
	return;
}


bool PF::DrawConfigDialog::pointer_press_event( int button, double x, double y, int mod_key )
{
  if( button != 1 ) return false;
  start_stroke();
  draw_point( x, y );
  return false;
}


bool PF::DrawConfigDialog::pointer_release_event( int button, double x, double y, int mod_key )
{
  if( button != 1 ) return false;
  //draw_point( x, y );
  return false;
}


bool PF::DrawConfigDialog::pointer_motion_event( int button, double x, double y, int mod_key )
{
  mouse_x = x; mouse_y = y;
  if( button != 1 ) return true;
#ifndef NDEBUG
  std::cout<<"PF::DrawConfigDialog::pointer_motion_event() called."<<std::endl;
#endif
  draw_point( x, y );
  return true;
}




bool PF::DrawConfigDialog::modify_preview( PixelBuffer& buf_in, PixelBuffer& buf_out,
                                           float scale, int xoffset, int yoffset )
{
  if( !get_mapped() )
    return false;

  if( !get_layer() ) return false;
  if( !get_layer()->get_image() ) return false;
  if( !get_layer()->get_processor() ) return false;
  if( !get_layer()->get_processor()->get_par() ) return false;

  PF::OpParBase* par = get_layer()->get_processor()->get_par();

  // Resize the output buffer to match the input one
  buf_out.resize( buf_in.get_rect() );
  // Copy pixel data from input to output
  buf_out.copy( buf_in );

  guint8* px = buf_out.get_pxbuf()->get_pixels();
  const int rs = buf_out.get_pxbuf()->get_rowstride();
  const int bl = 3; /*buf->get_byte_length();*/

  int buf_left = buf_out.get_rect().left;
  int buf_right = buf_out.get_rect().left+buf_out.get_rect().width-1;
  int buf_top = buf_out.get_rect().top;
  int buf_bottom = buf_out.get_rect().top+buf_out.get_rect().height-1;

  int pensize = pen_size.get_adjustment()->get_value()*scale;
  int pen_size2 = pensize*pensize;

  int x0 = mouse_x*scale + xoffset;
  int y0 = mouse_y*scale + yoffset;

  for( int y = 0; y <= pensize; y++ ) {
    int row1 = y0 - y;
    int row2 = y0 + y;
    //int L = pen.get_size() - y;
    int D = (int)(sqrt( pen_size2 - y*y )-0.00);
    int left = x0 - D;
    if( left < buf_left )
      left = buf_left;
    int right = x0 + D;
    if( right >= buf_right )
      right = buf_right;
    int colspan = (right + 1 - left)*3;

    int left2 = right+1;
    int right2 = left-1;
    if( y < pensize ) {
      int D2 = (int)(sqrt( pen_size2 - (y+1)*(y+1) )-0.00);
      left2 = x0 - D2;
      if( left2 < buf_left )
        left2 = buf_left;
      right2 = x0 + D2;
      if( right2 >= buf_right )
        right2 = buf_right;
    }


    //endcol = x0;

    /*
      std::cout<<"x0="<<x0<<"  y0="<<y0<<"  D="<<D<<std::endl;
      std::cout<<"row1="<<row1<<"  row2="<<row2<<"  startcol="<<startcol<<"  endcol="<<endcol<<"  colspan="<<colspan<<std::endl;
      std::cout<<"point_clip.left="<<point_clip.left<<"  point_clip.top="<<point_clip.top
               <<"  point_clip.width="<<point_clip.width<<"  point_clip.height="<<point_clip.height<<std::endl;
     */
    /**/
    if( (row1 >= buf_top) && (row1 <= buf_bottom) ) {
      guint8* p = px + rs*(row1-buf_top) + (left-buf_left)*bl;
      if( left2 <= right ) {
        for( int x = left; x <= left2; x++, p += bl ) {
          p[0] = 255-p[0]; p[1] = 255-p[1]; p[2] = 255-p[2];
        }
        p = px + rs*(row1-buf_top) + (right2+1-buf_left)*bl;
        for( int x = right2; x <= right; x++, p += bl ) {
          p[0] = 255-p[0]; p[1] = 255-p[1]; p[2] = 255-p[2];
        }
      } else {
        for( int x = left; x <= right; x++, p += bl ) {
          p[0] = 255-p[0]; p[1] = 255-p[1]; p[2] = 255-p[2];
        }
      }
    }
    if( (row2 != row1) && (row2 >= buf_top) && (row2 <= buf_bottom) ) {
      guint8* p = px + rs*(row2-buf_top) + (left-buf_left)*bl;
      if( left2 <= right ) {
        for( int x = left; x <= left2; x++, p += bl ) {
          p[0] = 255-p[0]; p[1] = 255-p[1]; p[2] = 255-p[2];
        }
        p = px + rs*(row2-buf_top) + (right2+1-buf_left)*bl;
        for( int x = right2; x <= right; x++, p += bl ) {
          p[0] = 255-p[0]; p[1] = 255-p[1]; p[2] = 255-p[2];
        }
      } else {
        for( int x = left; x <= right; x++, p += bl ) {
          p[0] = 255-p[0]; p[1] = 255-p[1]; p[2] = 255-p[2];
        }
      }
    }
  }

  return true;
}