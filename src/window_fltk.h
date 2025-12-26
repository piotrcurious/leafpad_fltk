/*
 *  Leafpad - FLTK based simple text editor
 */

#ifndef WINDOW_FLTK_H
#define WINDOW_FLTK_H

#include <FL/Fl_Window.H>
#include "view_fltk.h"

class MainWindow : public Fl_Window {
public:
    EditorView* editor;
    char* current_filename;

    MainWindow(int w, int h, const char* title);
};

#endif // WINDOW_FLTK_H
