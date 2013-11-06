#ifndef DEFINES_H
#define DEFINES_H

#define KSILIT_VERSION                  "0.1.0"
#define KSILIT_ORGANIZATION_NAME        "ixotum"
#define KSILIT_APPLICATION_NAME         "ksilit"
#define KSILIT_LOG_FILE_NAME            "ksilit.log"
#define KSILIT_DEFAULT_DATA_BASE_FILTER "Ksilit data base (*.ksi);;All files(*)"
#define KSILIT_DEFAULT_DATA_BASE_SUFFIX "ksi"
#define KSILIT_DEFAULT_DATA_BASE_NAME   "ksilit"
#define KSILIT_WARNING_QUIT_SAVE        "Save changes before closing?"

#define KSILIT_SETTING_CATEGORY_GENERAL "General"
#define KSILIT_SETTING_DEFAULT_PATH     "defaultPath"

#define KSILIT_MAIN_WINDOW_MENU_FILE         "&File"
#define KSILIT_MAIN_WINDOW_MENU_FILE_NEW     "&New"
#define KSILIT_MAIN_WINDOW_MENU_FILE_OPEN    "&Open"
#define KSILIT_MAIN_WINDOW_MENU_FILE_SAVE    "&Save"
#define KSILIT_MAIN_WINDOW_MENU_FILE_SAVE_AS "Save As..."
#define KSILIT_MAIN_WINDOW_MENU_FILE_QUIT    "&Quit"

#define KSILIT_MAIN_WINDOW_MENU_EDIT "&Edit"

#define KSILIT_MAIN_WINDOW_MENU_HELP       "&Help"
#define KSILIT_MAIN_WINDOW_MENU_HELP_ABOUT "&About"

#define KSILIT_JOTTER_ACTION_ADD_NOTE            "Add Note"
#define KSILIT_JOTTER_ACTION_ADD_SUB_NOTE_TEXT   "Add Sub Note"
#define KSILIT_JOTTER_ACTION_MOVE_UP             "Move Up"
#define KSILIT_JOTTER_ACTION_MOVE_DOWN           "Move Down"
#define KSILIT_JOTTER_ACTION_DELETE_NOTE_TEXT    "Delete Note"
#define KSILIT_JOTTER_HEADER_COLUMN_NAME_NUMBER  0
#define KSILIT_JOTTER_HEADER_COLUMN_NAME_TEXT    "Notes"
#define KSILIT_JOTTER_NEW_NOTE_TEXT              "New Note"
#define KSILIT_JOTTER_DELETE_REQUEST             "Would you like to delete selected item?"

#define KSILIT_DOM_TAG                           "Ksilit"
#define KSILIT_JOTTER_DOM_HEADER_TAG             "JotterHeader"
#define KSILIT_JOTTER_DOM_JOT_COUNTER            "jotCounter"
#define KSILIT_JOTTER_DOM_BODY_TAG               "JotterBody"
#define KSILIT_JOTTER_DOM_ELEMENT_TAG            "Note"
#define KSILIT_JOTTER_DOM_ELEMENT_ATTRIBUTE_ID   "id"
#define KSILIT_JOTTER_DOM_ELEMENT_ATTRIBUTE_NAME "name"
#define KSILIT_JOTTER_DOM_ELEMENT_ATTRIBUTE_TEXT "text"

#define ALL_OK                0
#define ERROR_UNKNOWN_ERROR   1
#define ERROR_OPENING_FILE    2
#define ERROR_ACTION_CANCELED 3

#endif // DEFINES_H
