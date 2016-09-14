/* ************************************************************* *
 *   The Amulet User Interface Development Environment           *
 * ************************************************************* *
 *   Created automatically by the Gilt program in Amulet.        *
 *   Do not edit this file directly.                             *
 *   For more information on Amulet, contact amulet@cs.cmu.edu   *
 * ************************************************************* *
 *   Generated on Tue Aug  5 00:24:40 1997

 *   Amulet version 3.0
 * ************************************************************* */

#include <amulet.h>
#include <amulet/object.h>         // am_object
#include <amulet/opal.h>           // am_window
#include <amulet/standard_slots.h> // am_destroy_window_method
#include <amulet/widgets.h>        // am_default_pop_up_window_destroy_method

Am_Object am_askclickpos;

Am_Object
am_askclickpos_Initialize()
{
  am_askclickpos = Am_Window.Create(DSTR("am_askclickpos"))
                       .Set(Am_DESTROY_WINDOW_METHOD,
                            Am_Default_Pop_Up_Window_Destroy_Method)
                       .Set(Am_FILL_STYLE, Am_Amulet_Purple)
                       .Set(Am_TITLE, "Specify Location")
                       .Set(Am_ICON_TITLE, "Specify Location")
                       .Set(Am_WIDTH, 361)
                       .Set(Am_HEIGHT, 102)
                       .Add(Am_VALUE, (0L));
  am_askclickpos
      .Add_Part(
          Am_LABEL,
          Am_Text.Create()
              .Set(Am_LEFT, 31)
              .Set(Am_TOP, 14)
              .Set(Am_WIDTH, 475)
              .Set(Am_HEIGHT, 15)
              .Set(Am_TEXT, "Click in the window to define a new position")
              .Set(Am_LINE_STYLE, Am_Black)
              .Set(Am_FILL_STYLE, Am_No_Style))
      .Add_Part(Am_Text.Create()
                    .Set(Am_LEFT, 31)
                    .Set(Am_TOP, 29)
                    .Set(Am_WIDTH, 91)
                    .Set(Am_HEIGHT, 15)
                    .Set(Am_TEXT, "or hit cancel")
                    .Set(Am_LINE_STYLE, Am_Black)
                    .Set(Am_FILL_STYLE, Am_No_Style))
      .Add_Part(Am_Button_Panel.Create()
                    .Set(Am_LEFT, Am_Center_X_Is_Center_Of_Owner)
                    .Set(Am_TOP, 48)
                    .Set(Am_FILL_STYLE, Am_Amulet_Purple)
                    .Set(Am_LAYOUT, Am_Horizontal_Layout)
                    .Set(Am_H_SPACING, 0)
                    .Set(Am_V_SPACING, 0)
                    .Set(Am_MAX_RANK, 0)
                    .Set(Am_ITEMS, Am_Value_List().Add(
                                       Am_Standard_Cancel_Command.Create()
                                           .Set(Am_DEFAULT, true)
                                           .Set(Am_ACCELERATOR,
                                                Am_Input_Char("RETURN")))));
  return am_askclickpos;
}