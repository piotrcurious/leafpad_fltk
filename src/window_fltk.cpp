/*
 *  Leafpad - FLTK based simple text editor
 */

#include "window_fltk.h"
#include "view_fltk.h"
#include "callback_fltk.h"
#include "dialog_fltk.h"
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Text_Buffer.H>

// Callback for the "About" menu item
static void on_about_cb(Fl_Widget*, void*) {
    show_about_dialog();
}

MainWindow::MainWindow(int w, int h, const char* title) : Fl_Window(w, h, title) {
    current_filename = NULL;

    begin();

    editor = new EditorView(0, 30, w, h - 30);
    Fl_Text_Buffer *buff = new Fl_Text_Buffer();
    editor->buffer(buff);

    Fl_Menu_Item menu_items[] = {
        { "&File", 0, 0, 0, FL_SUBMENU },
            { "&New", FL_CTRL + 'n', (Fl_Callback *)on_file_new, this },
            { "&Open...", FL_CTRL + 'o', (Fl_Callback *)on_file_open, this },
            { "&Save", FL_CTRL + 's', (Fl_Callback *)on_file_save, this },
            { "Save &As...", FL_CTRL + FL_SHIFT + 's', (Fl_Callback *)on_file_save_as, this },
            { "&Quit", FL_CTRL + 'q', (Fl_Callback *)on_file_quit, this },
            { 0 },
        { "&Edit", 0, 0, 0, FL_SUBMENU },
            // Fl_Text_Editor provides default key bindings for these
            { "Undo", FL_CTRL + 'z', 0, 0, 0 },
            { "Redo", FL_CTRL + FL_SHIFT + 'z', 0, 0, 0 },
            { "Cu&t", FL_CTRL + 'x', 0, 0, 0 },
            { "&Copy", FL_CTRL + 'c', 0, 0, 0 },
            { "&Paste", FL_CTRL + 'v', 0, 0, 0 },
            { "Select &All", FL_CTRL + 'a', 0, 0, 0 },
            { 0 },
        { "&Help", 0, 0, 0, FL_SUBMENU },
            { "&About", 0, (Fl_Callback*)on_about_cb },
            { 0 },
        { 0 }
    };

    Fl_Menu_Bar* menu = new Fl_Menu_Bar(0, 0, w, 30);
    menu->copy(menu_items);

    end();
    resizable(editor);
}
