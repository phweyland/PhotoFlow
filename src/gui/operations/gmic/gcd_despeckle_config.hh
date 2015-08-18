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

#ifndef GMIC_GCD_DESPECKLE_CONFIG_DIALOG_HH
#define GMIC_GCD_DESPECKLE_CONFIG_DIALOG_HH

#include <gtkmm.h>

#include "../../operation_config_gui.hh"


namespace PF {

  class GmicGcdDespeckleConfigGUI: public OperationConfigGUI
  {
    Gtk::VBox controlsBox;
    
    Slider iterations_slider;
    Slider prop_tolerance_slider;
    Slider prop_max_area_slider;
       
  public:
    GmicGcdDespeckleConfigGUI( Layer* l );
    
    void open();
  };

}

#endif
