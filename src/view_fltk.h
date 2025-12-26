/*
 *  Leafpad - FLTK based simple text editor
 */

#ifndef VIEW_FLTK_H
#define VIEW_FLTK_H

#include <FL/Fl_Text_Editor.H>

class EditorView : public Fl_Text_Editor {
public:
    EditorView(int x, int y, int w, int h, const char* label = 0);
};

#endif // VIEW_FLTK_H
