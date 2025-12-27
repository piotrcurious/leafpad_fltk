/*
 *  Leafpad - FLTK based simple text editor
 */

#include "view_fltk.h"
#include <FL/Fl.H>

EditorView::EditorView(int x, int y, int w, int h, const char* label) : Fl_Text_Editor(x, y, w, h, label) {
    last_key = 0;
}

int EditorView::handle(int event) {
    if (event == FL_KEYBOARD) {
        last_key = Fl::event_key();
    }
    return Fl_Text_Editor::handle(event);
}
