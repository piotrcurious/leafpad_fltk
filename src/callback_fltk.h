/*
 *  Leafpad - FLTK based simple text editor
 */

#ifndef CALLBACK_FLTK_H
#define CALLBACK_FLTK_H

#include "window_fltk.h"

void on_file_new(Fl_Widget*, void*);
void on_file_open(Fl_Widget*, void*);
void on_file_save(Fl_Widget*, void*);
void on_file_save_as(Fl_Widget*, void*);
void on_file_quit(Fl_Widget*, void*);

#endif // CALLBACK_FLTK_H
