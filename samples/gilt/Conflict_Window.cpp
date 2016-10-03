/* ************************************************************* *
 *   The Amulet User Interface Development Environment           *
 * ************************************************************* *
 *   Created automatically by the Gilt program in Amulet.        *
 *   Do not edit this file directly.                             *
 *   For more information on Amulet, contact amulet@cs.cmu.edu   *
 * ************************************************************* *
 *   Generated on Thu Aug 07 12:56:21 1997

 *   Amulet version 4.0 Development
 * ************************************************************* */

#include <amulet.h>
#include "externs.h"
#include "constraint.h"
#include <amulet/am_strstream.h>
#include <amulet/debugger.h>

using namespace std;

#define TEXT_HEIGHT 13
#define SLOT_TEXT 3

extern Am_Slot_Key get_slot_key(Which_Constraint_Handle wh);
extern Am_Style very_light_gray;
Am_Constraint_Iterator find_constraint_in(Am_Object &obj, Am_Slot_Key key);

Am_Slot_Key BUTTON_GROUP = Am_Register_Slot_Name("BUTTON_GROUP");
Am_Slot_Key WORK_OBJECT = Am_Register_Slot_Name("WORK_OBJECT");
Am_Slot_Key WORK_HANDLE = Am_Register_Slot_Name("WORK_HANDLE");
Am_Slot_Key MAIN_TEXT_GROUP = Am_Register_Slot_Name("MAIN_TEXT_GROUP");
Am_Slot_Key DETAIL_TEXT_GROUP = Am_Register_Slot_Name("DETAIL_TEXT_GROUP");
Am_Slot_Key OBJECT_LIST = Am_Register_Slot_Name("OBJECT_LIST");
Am_Slot_Key CONSTRAINT_LIST = Am_Register_Slot_Name("CONSTRAINT_LIST");
Am_Slot_Key LIST_INDEX = Am_Register_Slot_Name("LIST_INDEX");
Am_Slot_Key SLOT_LIST = Am_Register_Slot_Name("SLOT_LIST");

Am_Object Conflict_Window;
Am_Object Scrolling_Text;
Am_Object general_text_proto;
Am_Object Pop_Up_Detail_Window;
// Line styles exclusively created by Gilt

Am_Object detail_interactor_proto;
Am_Style Am_White_2 = Am_Style::Am_Style(1, 1, 1, 2, Am_CAP_BUTT, Am_JOIN_MITER,
                                         Am_LINE_SOLID, Am_DEFAULT_DASH_LIST);

Am_Define_Formula(bool, wider_than)
{
  int inner_w = self.Get(Am_INNER_WIDTH);
  int outter_w = self.Get(Am_WIDTH);
  if (inner_w > outter_w)
    return true;
  else
    return false;
}

Am_Define_Formula(bool, longer_than)
{
  int inner_h = self.Get(Am_INNER_HEIGHT);
  int outter_h = self.Get(Am_HEIGHT);
  if (inner_h > outter_h)
    return true;
  else
    return false;
}

Am_String
print_value(Am_Value value)
{
  char buff[250] = {0};
  OSTRSTREAM_CONSTR(oss, buff, 250, ios::out);
  oss << value << ends;
  OSTRSTREAM_COPY(oss, buff, 250);
  return (Am_String)buff;
}

Am_Define_String_Formula(get_working_object)
{
  Am_Object wo = Conflict_Window.Get_Object(WORK_OBJECT);
  return print_value((Am_Value)wo);
}

Am_String
enum_to_handle_string(Which_Constraint_Handle wh)
{
  switch (wh.value) {
  case CH_RIGHT_val:
    return (Am_String) "RIGHT_HANDLE";
  case CH_LEFT_val:
    return (Am_String) "LEFT_HANDLE";
  case CH_TOP_val:
    return (Am_String) "TOP_HANDLE";
  case CH_BOTTOM_val:
    return (Am_String) "BOTTOM_HANDLE";
  case CH_CENTER_X_val:
    return (Am_String) "CENTER_X_HANDLE";
  case CH_CENTER_Y_val:
    return (Am_String) "CENTER_Y_HANDLE";
  case CH_WIDTH_val:
    return (Am_String) "WIDTH_HANDLE";
  case CH_HEIGHT_val:
    return (Am_String) "HEIGHT_HANDLE";
  case CH_X1_val:
    return (Am_String) "X1_HANDLE";
  case CH_X2_val:
    return (Am_String) "X2_HANDLE";
  case CH_Y1_val:
    return (Am_String) "Y1_HANDLE";
  case CH_Y2_val:
    return (Am_String) "Y2_HANDLE";
  default:
    return (Am_String) "";
  }
}

Am_Define_Method(Am_Object_Method, void, setup_new_method, (Am_Object /*cmd*/))
{
  Am_Finish_Pop_Up_Waiting(Conflict_Window, CONSTRAINT_SETUP);
}

Am_Define_Method(Am_Object_Method, void, abort_current_setup,
                 (Am_Object /*cmd*/))
{
  Am_Finish_Pop_Up_Waiting(Conflict_Window, ABORT_SETUP);
}

Am_Define_Style_Formula(get_text_line_style)
{
  int type = self.Get(Am_ID);

  if (type == CONSTRAINT_TEXT)
    return Am_Blue;
  else if (type == OBJECT_TEXT)
    return Am_Red;
  else
    return Am_Black;
}

//////////////////////////////  Displaying information stuff //////////////////

Am_Slot_Key
get_slot_key_always(Which_Constraint_Handle wh)
{
  Am_Slot_Key key;

  if (wh == CH_CENTER_X || wh == CH_RIGHT)
    key = Am_LEFT;
  else if (wh == CH_BOTTOM || wh == CH_CENTER_Y)
    key = Am_TOP;
  else
    key = get_slot_key(wh);

  return key;
}

Am_Object
find_next_text_obj(Am_Object text_group, Am_Value_List &list, int &left,
                   int &top)
{
  Am_Object tmp;

  if (!list.Last()) {
    tmp = (Am_Object)list.Get(); // if a text is there, use it
    list.Next();
  } else {
    tmp = general_text_proto.Create();
    text_group.Add_Part(tmp);
  }
  tmp.Set(Am_LEFT, left).Set(Am_TOP, top);
  if (!(bool)tmp.Get(Am_VISIBLE))
    tmp.Set(Am_VISIBLE, true);
  return tmp;
}

void
add_plain_text(Am_Object text_obj, const char *text, int &left, int &top,
               Am_Value_List &list)
{
  Am_Object tmp = find_next_text_obj(text_obj, list, left, top);

  tmp.Get_Object(Am_INACTIVE_COMMANDS).Set(Am_SELECT_INACTIVE, true);
  tmp.Set(Am_TEXT, text);
  tmp.Set(Am_ID, SIMPLE_TEXT);
  left += (int)tmp.Get(Am_WIDTH);
}

void
new_line(int &left, int &top)
{
  top += TEXT_HEIGHT;
  left = 0;
}

Am_String
get_handle_slot_name(Which_Constraint_Handle wh)
{
  char buffer[40];
  Am_Slot_Key key = get_slot_key_always(wh);

  sprintf(buffer, "%s(%s)", (const char *)(Am_String)Am_Get_Slot_Name(key),
          (const char *)(Am_String)enum_to_handle_string(wh));
  return Am_String(buffer);
}

void
add_object(Am_Object text_group, Am_Object &obj, int &left, int &top,
           Am_Value_List &list, int index)
{
  Am_Object tmp = find_next_text_obj(text_group, list, left, top);

  tmp.Get_Object(Am_INACTIVE_COMMANDS).Set(Am_SELECT_INACTIVE, false);
  tmp.Set(Am_TEXT, print_value((Am_Value)obj));
  tmp.Set(Am_ID, OBJECT_TEXT);
  tmp.Set(LIST_INDEX, index);
  index++;

  Am_Value_List obj_list = text_group.Get(OBJECT_LIST);
  obj_list.Add(obj);
  text_group.Set(OBJECT_LIST, obj_list);

  left += (int)tmp.Get(Am_WIDTH);
}

void
add_constraint_list(Am_Object text_group, Am_Object obj,
                    Which_Constraint_Handle wh, int &left, int &top,
                    Am_Value_List &list, int &index)
{
  Am_Object tmp;
  Am_Slot_Key key = get_slot_key_always(wh);
  Am_Constraint *constr;
  char buffer[100];
  Am_Value_List const_list = text_group.Get(CONSTRAINT_LIST);

  Am_Constraint_Iterator const_iter = find_constraint_in(obj, key);
  for (const_iter.Start(); !const_iter.Last(); const_iter.Next()) {
    tmp = find_next_text_obj(text_group, list, left, top);
    constr = const_iter.Get();
    const_list.Add((Am_Ptr)constr);

    sprintf(buffer, "%s%p", constr->Get_Name(), constr);
    tmp.Set(Am_TEXT, Am_String(buffer));
    tmp.Get_Object(Am_INACTIVE_COMMANDS).Set(Am_SELECT_INACTIVE, false);
    tmp.Set(Am_ID, CONSTRAINT_TEXT);
    tmp.Set(LIST_INDEX, index);
    index++;
  }
  text_group.Set(CONSTRAINT_LIST, const_list);
}

void
add_constraint(Am_Object text_group, Am_Constraint *constr, int &left, int &top,
               Am_Value_List &list, int &index)
{
  Am_Object tmp;
  char buffer[50];
  Am_Value_List const_list = text_group.Get(CONSTRAINT_LIST);

  tmp = find_next_text_obj(text_group, list, left, top);
  sprintf(buffer, "%s%p", constr->Get_Name(), constr);
  tmp.Set(Am_TEXT, Am_String(buffer));
  tmp.Set(Am_ID, CONSTRAINT_TEXT);
  tmp.Get_Object(Am_INACTIVE_COMMANDS).Set(Am_SELECT_INACTIVE, false);
  tmp.Set(LIST_INDEX, index);
  const_list.Add((Am_Ptr)constr);
  text_group.Set(CONSTRAINT_LIST, const_list);
  left += (int)tmp.Get(Am_WIDTH);
  index++;
}

void
add_slot(Am_Object text_group, Am_Slot_Key key, int &left, int &top,
         Am_Value_List &list, int &index)
{
  char buffer[50];
  Am_Object tmp = find_next_text_obj(text_group, list, left, top);
  Am_Value_List slot_list = text_group.Get(SLOT_LIST);

  sprintf(buffer, "%s", (const char *)(Am_String)Am_Get_Slot_Name(key));
  tmp.Set(Am_TEXT, Am_String(buffer));
  tmp.Set(Am_ID, SLOT_TEXT);
  tmp.Get_Object(Am_INACTIVE_COMMANDS).Set(Am_SELECT_INACTIVE, false);
  tmp.Set(LIST_INDEX, index);
  slot_list.Add(key);
  index++;
  left += (int)tmp.Get(Am_WIDTH);
  text_group.Set(SLOT_LIST, slot_list);
}

void
add_slot_and_object(Am_Object text_group, Am_Slot_Key key, Am_Object for_obj,
                    int &left, int &top, Am_Value_List &list, int &slot_index,
                    int &obj_index)
{
  char buffer[100];

  add_slot(text_group, key, left, top, list, slot_index);
  sprintf(buffer, " of ");
  add_plain_text(text_group, buffer, left, top, list);
  add_object(text_group, for_obj, left, top, list, obj_index);
}

void
add_value(Am_Object text_group, Am_Object obj, Am_Slot_Key key, int &left,
          int &top, Am_Value_List &list)
{
  Am_Value v = obj.Get(key);
  add_plain_text(text_group, " = ", left, top, list);
  add_plain_text(text_group, (const char *)print_value(v), left, top, list);
}

void
add_constraint_and_value(Am_Object text_group,
                         Am_Constraint_Iterator const_iter, Am_Object obj,
                         Am_Slot_Key key, int &left, int &top,
                         Am_Value_List &list, int &const_index)
{
  Am_Constraint *constr;
  const char *buffer = "Contains constraint(s)";
  const char *prolog = "(";
  const char *epilog = ")";

  add_plain_text(text_group, buffer, left, top, list);
  for (const_iter.Start(); !const_iter.Last(); const_iter.Next()) {
    constr = const_iter.Get();
    add_plain_text(text_group, prolog, left, top, list);
    add_constraint(text_group, constr, left, top, list, const_index);
    add_plain_text(text_group, epilog, left, top, list);
  }
  add_value(text_group, obj, key, left, top, list);
}

Which_Constraint_Handle
get_alternative_handle_id(Which_Constraint_Handle wh, Am_Object obj)
{
  Am_Value_List const_info = obj.Get(CONSTRAINT_INFO);
  Am_Value_List tmp;
  Which_Constraint_Handle h;
  bool found = false;

  if (wh == CH_LEFT || wh == CH_RIGHT || wh == CH_CENTER_X) {
    for (const_info.Start(); !const_info.Last() && !found; const_info.Next()) {
      tmp = const_info.Get();
      tmp.Start();
      h = (Which_Constraint_Handle)tmp.Get();
      if (wh == CH_LEFT || wh == CH_RIGHT || wh == CH_CENTER_X)
        found = true;
    }
  } else {
    for (const_info.Start(); !const_info.Last() && !found; const_info.Next()) {
      tmp = const_info.Get();
      tmp.Start();
      h = (Which_Constraint_Handle)tmp.Get();
      if (wh == CH_TOP || wh == CH_CENTER_Y || wh == CH_BOTTOM)
        found = true;
    }
  }
  if (found)
    return h;
  else
    return wh;
}

void
clean_up(Am_Value_List &list)
{
  Am_Object tmp;

  for (; !list.Last(); list.Next()) {
    tmp = (Am_Object)list.Get();
    tmp.Set(Am_VISIBLE, false);
  }
}

void
clean_up_and_remove(Am_Object group, Am_Value_List &list)
{
  Am_Object tmp;

  for (; !list.Last(); list.Next()) {
    tmp = (Am_Object)list.Get();
    group.Remove_Part(tmp);
  }
}

Am_Define_Method(Am_Object_Method, void, display_conflict_info,
                 (Am_Object text_obj))
{
  int left = 0, top = 0;
  Am_Value_List g_list = text_obj.Get(Am_GRAPHICAL_PARTS);
  char buffer[100];
  int obj_index = 0, const_index = 0;
  Am_Object window = text_obj.Get_Owner();
  text_obj.Set(SLOT_LIST, Am_No_Value_List);
  text_obj.Set(CONSTRAINT_LIST, Am_No_Value_List);
  text_obj.Set(OBJECT_LIST, Am_No_Value_List);

  g_list.Start();
  Which_Constraint_Handle wh =
      (Which_Constraint_Handle)text_obj.Get_Owner().Get(WORK_HANDLE);
  Am_Object working_obj = text_obj.Get_Owner().Get(WORK_OBJECT);

  sprintf(buffer, "Setting up New Constraint to Slot ");
  add_plain_text(text_obj, buffer, left, top, g_list);
  new_line(left, top);

  sprintf(buffer, "%s of Object ", (const char *)get_handle_slot_name(wh));
  add_plain_text(text_obj, buffer, left, top, g_list);

  add_object(text_obj, working_obj, left, top, g_list, obj_index);
  new_line(left, top);
  sprintf(buffer, "Conflicts with Old Constraint ");
  add_plain_text(text_obj, buffer, left, top, g_list);

  add_constraint_list(text_obj, working_obj, wh, left, top, g_list,
                      const_index);
  new_line(left, top);
  Which_Constraint_Handle other_h;
  if (wh == CH_CENTER_X || wh == CH_CENTER_Y || wh == CH_RIGHT ||
      wh == CH_BOTTOM || wh == CH_LEFT || wh == CH_TOP)
    other_h = get_alternative_handle_id(wh, working_obj);
  else
    other_h = wh;

  sprintf(buffer, "In Slot %s of Object ",
          (const char *)get_handle_slot_name(other_h));
  add_plain_text(text_obj, buffer, left, top, g_list);

  add_object(text_obj, working_obj, left, top, g_list, obj_index);
  clean_up(g_list);

  Am_Object prev_obj = text_obj.Get_Object(CHOICE_INTER).Get_Object(Am_VALUE);
  if (prev_obj.Valid()) {
    prev_obj.Set(Am_SELECTED, false);
    text_obj.Get_Object(CHOICE_INTER).Set(Am_VALUE, Am_No_Value);
    text_obj.Get_Object(CHOICE_INTER)
        .Get_Object(Am_COMMAND)
        .Set(Am_VALUE, Am_No_Value);
  }
  window.Set(Am_WIDTH, 550);
  window.Set(Am_HEIGHT, 160);
  window.Get_Object(DETAIL_TEXT_GROUP).Set(Am_HEIGHT, 0);
}

Am_Value_List
display_dependency_list(Am_Object &text_group, Am_Value_List &dep_info,
                        Am_Constraint *constr)
{

  int left = 0;
  int top = 0;
  char buffer[100];
  Am_Value_List g_list = (Am_Value_List)text_group.Get(Am_GRAPHICAL_PARTS);
  int const_index = -1;
  int object_index = 0;
  int slot_index = 0;
  Am_Object tmp;
  Am_Slot_Key key;
  Am_Value_List one_info;
  Am_Value_List total_info;
  Am_Constraint_Iterator const_iter;

  text_group.Set(SLOT_LIST, Am_No_Value_List);
  text_group.Set(CONSTRAINT_LIST, Am_No_Value_List);
  text_group.Set(OBJECT_LIST, Am_No_Value_List);
  g_list.Start();

  sprintf(buffer, "Dependencies of Constraint ");
  add_plain_text(text_group, buffer, left, top, g_list);
  add_constraint(text_group, constr, left, top, g_list, const_index);

  for (dep_info.Start(); !dep_info.Last(); dep_info.Next()) {
    new_line(left, top);

    one_info = dep_info.Get();
    tmp = one_info.Get_Nth(0);
    key = (Am_Slot_Key)(int)one_info.Get_Nth(1);

    add_slot_and_object(text_group, key, tmp, left, top, g_list, slot_index,
                        object_index);
    const_iter = find_constraint_in(tmp, key);
    const_iter.Start();
    if (const_iter.Last())
      add_value(text_group, tmp, key, left, top, g_list);
    else {
      new_line(left, top);
      add_constraint_and_value(text_group, const_iter, tmp, key, left, top,
                               g_list, const_index);
    }
    new_line(left, top);
  }
  return g_list;
}

///////////////////////////////// Interactor stuff /////////////////////////////////////

Am_Define_Formula(Am_Value, compute_selected_object)
{
  Am_Object ci = self.Get_Object(CHOICE_INTER);
  Am_Value selected;

  if (ci.Valid()) {
    selected = ci.Get_Object(Am_VALUE); //evaluate only when Am_VALUE changes
    if (selected.Valid()) {
      Am_Object window = self.Get_Owner();
      int index = (int)((Am_Object)selected).Get(LIST_INDEX, Am_NO_DEPENDENCY);
      int id = (int)((Am_Object)selected).Get(Am_ID, Am_NO_DEPENDENCY);
      Am_Value_List list;

      if (id == OBJECT_TEXT)
        list = self.Get(OBJECT_LIST, Am_NO_DEPENDENCY);
      else if (id == CONSTRAINT_TEXT)
        list = self.Get(CONSTRAINT_LIST, Am_NO_DEPENDENCY);
      else
        list = self.Get(SLOT_LIST, Am_NO_DEPENDENCY);
      if (index == -1) {
        ci.Set(Am_VALUE, Am_No_Object);
        Am_Object(selected).Set(Am_SELECTED, false);
        ci.Get_Object(Am_COMMAND).Set(Am_VALUE, Am_No_Object);
      } else
        selected = list.Get_Nth(index);
    }
  }
  return selected;
}

Am_Define_Method(Am_Object_Method, void, show_details, (Am_Object inter))
{
  Am_Object group = inter.Get_Owner();
  Am_Value selected = group.Get(Am_VALUE);
  Am_Object detail_group = group.Get_Sibling(DETAIL_TEXT_GROUP);

  if (Am_Object::Test(selected))
    Am_Flash((Am_Object)selected);
  else {
    Am_Value_List dep_info = group.Get_Owner().Get(CONSTRAINT_INFO);
    detail_group.Set(Am_HEIGHT, Am_Same_As(Am_INNER_HEIGHT, 20));
    detail_group.Get_Owner().Set(Am_HEIGHT, Am_Height_Of_Parts);
    dep_info = display_dependency_list(detail_group, dep_info,
                                       (Am_Constraint *)(Am_Ptr)selected);
    clean_up(dep_info);
  }
}

Am_Define_Method(Am_Object_Method, void, pop_detail_window, (Am_Object inter))
{
  Am_Object group = inter.Get_Owner();
  Am_Value selected = group.Get(Am_VALUE);
  Am_Object window = group.Get_Owner();

  if (selected.Valid()) {
    if (Am_Object::Test(selected))
      Am_Flash((Am_Object)selected);
    else if (selected.type == Am_INT)
      Am_Show_Alert_Dialog("Slot selected");
    else {
      Am_Constraint *constr = (Am_Constraint *)(Am_Ptr)selected;
      Am_Value_List dep_list;

      if (Am_Formula_Advanced::Test(constr)) {
        Am_Formula_Advanced *form = Am_Formula_Advanced::Narrow(constr);
        dep_list = get_depends_list(form);
        dep_list = display_dependency_list(Pop_Up_Detail_Window, dep_list,
                                           (Am_Constraint *)(Am_Ptr)selected);
        clean_up_and_remove(Pop_Up_Detail_Window, dep_list);
        Pop_Up_Detail_Window.Set(Am_TOP, (int)window.Get(Am_TOP) + 10)
            .Set(Am_LEFT, (int)window.Get(Am_LEFT) + 10)
            .Set(Am_VISIBLE, true);
      } else
        Am_Error("Selected object is not recognized");
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////

Am_Object
Conflict_Window_Initialize()
{
  Pop_Up_Detail_Window =
      Am_Window.Create()
          .Set(Am_WIDTH, Am_Width_Of_Parts)
          .Set(Am_HEIGHT, Am_Height_Of_Parts)
          .Set(Am_FILL_STYLE, very_light_gray)
          .Set(Am_DESTROY_WINDOW_METHOD, Am_Window_Hide_Method)
          .Add(CONSTRAINT_LIST, Am_No_Value_List)
          .Add(SLOT_LIST, Am_No_Value_List)
          .Add(OBJECT_LIST, Am_No_Value_List)
          .Set(Am_VISIBLE, false);

  Am_Screen.Add_Part(Pop_Up_Detail_Window);

  general_text_proto =
      Am_Text.Create()
          .Set(Am_FILL_STYLE, Am_No_Style)
          .Set(Am_LINE_STYLE, get_text_line_style)
          .Add(Am_ID, SIMPLE_TEXT)
          .Add(LIST_INDEX, -1)
          .Add(Am_SELECTED, false)
          .Set(Am_INVERT, Am_Same_As(Am_SELECTED))
          .Add_Part(Am_INACTIVE_COMMANDS,
                    Am_Command.Create().Add(Am_SELECT_INACTIVE, false));

  Am_Object text_group =
      Am_Scrolling_Group.Create()
          .Set(Am_INNER_FILL_STYLE, Am_Same_As(Am_FILL_STYLE))
          .Set(Am_INNER_WIDTH, Am_Width_Of_Parts)
          .Set(Am_INNER_HEIGHT, Am_Height_Of_Parts)
          .Set(Am_H_SCROLL_BAR, wider_than)
          .Set(Am_V_SCROLL_BAR, longer_than)
          .Set(Am_LINE_STYLE, n2l[9][3])
          .Set(Am_TOP, 10)
          .Set(Am_LEFT, 10)
          .Set(Am_FILL_STYLE, Am_From_Owner(Am_FILL_STYLE))
          .Set(Am_WIDTH, Am_From_Owner(Am_WIDTH, -20))
          .Set(Am_HEIGHT, 100)
          .Add(Am_DO_METHOD, display_conflict_info)
          .Add(Am_VALUE, compute_selected_object)
          .Add(OBJECT_LIST, Am_No_Value_List)
          .Add(CONSTRAINT_LIST, Am_No_Value_List)
          .Add(SLOT_LIST, Am_No_Value_List)
          .Add_Part(CHOICE_INTER,
                    Am_Choice_Interactor.Create("text_group_choice_inter")
                        .Set(Am_START_WHEN, "LEFT_DOWN")
                        .Set(Am_PRIORITY, 2.0)
                        .Set(Am_START_WHERE_TEST, Am_Inter_In_Text_Leaf)
                        .Set(Am_HOW_SET, Am_CHOICE_SET));

  Am_Object detail_interactor =
      Am_One_Shot_Interactor.Create("detail_interactor")
          .Set(Am_START_WHEN, "DOUBLE_LEFT_DOWN")
          .Set(Am_PRIORITY, 1.0)
          .Set(Am_START_WHERE_TEST, Am_Inter_In_Text_Leaf);

  Conflict_Window = Am_Window.Create("Conflict_Window")
                        .Add(WORK_OBJECT, nullptr)
                        .Add(WORK_HANDLE, nullptr)
                        .Add(CONSTRAINT_INFO, nullptr)
                        .Set(Am_DESTROY_WINDOW_METHOD,
                             Am_Default_Pop_Up_Window_Destroy_Method)
                        .Set(Am_FILL_STYLE, very_light_gray)
                        .Set(Am_TITLE, "Gilt : Conflict/Loop Found")
                        .Set(Am_ICON_TITLE, "Gilt : Conflict/Loop Found")
                        .Set(Am_WIDTH, 550)
                        .Set(Am_HEIGHT, 160);

  Conflict_Window
      .Add_Part(
          Am_Border_Rectangle.Create()
              .Set(Am_FILL_STYLE, Am_Motif_Gray)
              .Set(Am_WIDTH, Am_From_Owner(Am_WIDTH, -6))
              .Set(Am_HEIGHT, Am_From_Sibling(MAIN_TEXT_GROUP, Am_HEIGHT, 14))
              .Set(Am_LEFT, 3)
              .Set(Am_TOP, 3))
      .Add_Part(MAIN_TEXT_GROUP,
                text_group.Create().Add_Part(
                    detail_interactor.Copy("main_text_interactor")
                        .Set(Am_DO_METHOD, show_details)))
      .Add_Part(
          Am_Border_Rectangle.Create()
              .Set(Am_FILL_STYLE, Am_Motif_Gray)
              .Set(Am_WIDTH, Am_From_Owner(Am_WIDTH, -6))
              .Set(Am_HEIGHT, Am_From_Sibling(DETAIL_TEXT_GROUP, Am_HEIGHT, 14))
              .Set(Am_LEFT, 3)
              .Set(Am_TOP, Am_From_Sibling(DETAIL_TEXT_GROUP, Am_TOP, -7)))
      .Add_Part(DETAIL_TEXT_GROUP,
                text_group.Create()
                    .Set(Am_HEIGHT, 0)
                    .Set(Am_TOP, 165)
                    .Add_Part(detail_interactor.Copy("detail_text_inter")
                                  .Set(Am_DO_METHOD, pop_detail_window)))
      .Add_Part(
          Am_Border_Rectangle.Create()
              .Set(Am_FILL_STYLE, Am_Motif_Gray)
              .Set(Am_WIDTH, Am_From_Owner(Am_WIDTH, -6))
              .Set(Am_HEIGHT, Am_From_Sibling(BUTTON_GROUP, Am_HEIGHT, 14))
              .Set(Am_LEFT, 3)
              .Set(Am_TOP, Am_From_Sibling(BUTTON_GROUP, Am_TOP, -7)))
      .Add_Part(
          BUTTON_GROUP,
          Am_Button_Panel.Create()
              .Set(Am_LEFT, Get_Data_And_Adjust(Conflict_Window, Am_NO_SLOT, Am_WIDTH,
                                                0, 0.5, Am_WIDTH, -0.5))
              .Set(Am_TOP, 124)
              .Set(Am_FILL_STYLE, very_light_gray)
              .Set(Am_ITEMS,
                   Am_Value_List()
                       .Add(Am_Command.Create()
                                .Set(Am_LABEL, "Remove Old & Setup New")
                                .Set(Am_DO_METHOD, setup_new_method)
                                .Set(Am_ID, 1))
                       .Add(Am_Command.Create()
                                .Set(Am_LABEL, "Abort New")
                                .Set(Am_DO_METHOD, abort_current_setup)
                                .Set(Am_ID, 2))
                       .Add(Am_Command.Create()
                                .Set(Am_LABEL, "See Details")
                                .Set(Am_ID, 4)))
              .Set(Am_LAYOUT, Am_Horizontal_Layout)
              .Set(Am_H_SPACING, 2)
              .Set(Am_MAX_RANK, 0));
  return Conflict_Window;
}
