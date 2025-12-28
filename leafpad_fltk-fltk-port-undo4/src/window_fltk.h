/*
 *  Leafpad - FLTK based simple text editor
 */

#ifndef WINDOW_FLTK_H
#define WINDOW_FLTK_H

#include <FL/Fl_Window.H>
#include "view_fltk.h"
#include "UndoManager.h"

class MainWindow : public Fl_Window {
public:
    EditorView* editor;
    char* current_filename;
    UndoManager* undo_manager;
    bool changed;

    MainWindow(int w, int h, const char* title);
};

#endif // WINDOW_FLTK_H
