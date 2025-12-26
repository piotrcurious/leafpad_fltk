/*
 *  Leafpad - FLTK based simple text editor
 */

#include "callback_fltk.h"
#include <FL/fl_ask.H>
#include <FL/Fl_File_Chooser.H>
#include <stdio.h>

void on_file_new(Fl_Widget* w, void* v) {
    MainWindow* window = (MainWindow*)v;
    window->editor->buffer()->text("");
    window->current_filename = NULL;
}

void on_file_open(Fl_Widget* w, void* v) {
    MainWindow* window = (MainWindow*)v;
    char* filename = fl_file_chooser("Open File", "*", "");
    if (filename) {
        window->editor->buffer()->loadfile(filename);
        window->current_filename = filename;
    }
}

void on_file_save(Fl_Widget* w, void* v) {
    MainWindow* window = (MainWindow*)v;
    if (!window->current_filename) {
        char* filename = fl_file_chooser("Save File", "*", "");
        if (filename) {
            window->current_filename = filename;
        } else {
            return;
        }
    }
    window->editor->buffer()->savefile(window->current_filename);
}

void on_file_save_as(Fl_Widget* w, void* v) {
    MainWindow* window = (MainWindow*)v;
    char* filename = fl_file_chooser("Save File As", "*", "");
    if (filename) {
        window->current_filename = filename;
        window->editor->buffer()->savefile(window->current_filename);
    }
}

#include <stdlib.h>

void on_file_quit(Fl_Widget* w, void* v) {
    exit(0);
}
