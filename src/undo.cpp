#include <FL/Fl.H>
#include <FL/Fl_Text_Buffer.H>
#include "undo.h"
#include "view_fltk.h"
#include "window_fltk.h"
#include <list>
#include <string>
#include <cstring>

#define DV(x)

typedef struct {
	char command;
	int start;
	int end;
	bool seq; // sequency flag
	char *str;
} UndoInfo;

enum {
	INS = 0,
	BS,
	DEL
};

static Fl_Text_Buffer* text_buffer = NULL;
static EditorView* editor_view = NULL;
static std::list<UndoInfo*> undo_list;
static std::list<UndoInfo*> redo_list;
static std::string undo_str;
static UndoInfo *ui_tmp;
static int modified_step;
static unsigned int prev_keyval;
static bool seq_reserve = false;

// Stubs for functions that need to be re-implemented
static int get_current_keyval() {
    if (editor_view) {
        return editor_view->last_key;
    }
    return 0;
}
static void clear_current_keyval() {
    if (editor_view) {
        editor_view->last_key = 0;
    }
}
static void scroll_to_cursor(Fl_Text_Buffer*, double) { }

static void undo_flush_temporal_buffer();
static void undo_clear_info_list(std::list<UndoInfo*>& info_list);
static void undo_append_undo_info(char command, int start, int end, char *str);
static void undo_create_undo_info(char command, int start, int end, const char* text);
static bool undo_undo_real();
static bool undo_redo_real();

static void buffer_modified_cb(int pos, int nInserted, int nDeleted, int nRestyled, const char* deletedText, void* cbArg)
{
    MainWindow* window = (MainWindow*)cbArg;
    window->changed = true;

    if (nInserted > 0) {
        char* text = text_buffer->text_range(pos, pos + nInserted);
        undo_create_undo_info(INS, pos, pos + nInserted, text);
        free(text);
    } else if (nDeleted > 0) {
        int keyval = get_current_keyval();
        char command = (keyval == FL_BackSpace) ? BS : DEL;
        undo_create_undo_info(command, pos, pos + nDeleted, deletedText);
    }
}

void undo_init(Fl_Text_Buffer *buffer, void* view, void* window)
{
	text_buffer = buffer;
    editor_view = (EditorView*)view;
	text_buffer->add_modify_callback(buffer_modified_cb, window);
	ui_tmp = (UndoInfo*)malloc(sizeof(UndoInfo));
	undo_clear_all();
}

void undo_clear_all()
{
	undo_clear_info_list(undo_list);
	undo_clear_info_list(redo_list);
	undo_reset_modified_step();

	ui_tmp->command = INS;
	undo_str.clear();
	prev_keyval = 0;
}

void undo_reset_modified_step()
{
	undo_flush_temporal_buffer();
	modified_step = undo_list.size();
}

void undo_set_sequency(bool seq)
{
	if (!undo_list.empty())
		undo_list.back()->seq = seq;
}

void undo_set_sequency_reserve()
{
	seq_reserve = true;
}

static void undo_flush_temporal_buffer()
{
	if (!undo_str.empty()) {
		undo_append_undo_info(ui_tmp->command, ui_tmp->start, ui_tmp->end, strdup(undo_str.c_str()));
		undo_str.clear();
	}
}

void undo_undo()
{
	while (undo_undo_real()) {};
}

void undo_redo()
{
	while (undo_redo_real()) {};
}

static bool undo_undo_real()
{
	undo_flush_temporal_buffer();
	if (!undo_list.empty()) {
		UndoInfo *ui = undo_list.back();
		switch (ui->command) {
		case INS:
			text_buffer->remove(ui->start, ui->end);
			break;
		default:
			text_buffer->insert(ui->start, ui->str);
		}
		redo_list.push_back(ui);
		undo_list.pop_back();

		if (!undo_list.empty()) {
			if (undo_list.back()->seq)
				return true;
		}
		if (ui->command == DEL)
			text_buffer->select(ui->start, ui->start);
		scroll_to_cursor(text_buffer, 0.05);
	}
	return false;
}

static bool undo_redo_real()
{
	if (!redo_list.empty()) {
		UndoInfo *ri = redo_list.back();
		switch (ri->command) {
		case INS:
			text_buffer->insert(ri->start, ri->str);
			break;
		default:
			text_buffer->remove(ri->start, ri->end);
		}
		undo_list.push_back(ri);
		redo_list.pop_back();

		if (ri->seq) {
			undo_set_sequency(true);
			return true;
		}
		text_buffer->select(ri->start, ri->start);
		scroll_to_cursor(text_buffer, 0.05);
	}
	return false;
}

static void undo_clear_info_list(std::list<UndoInfo*>& info_list)
{
	for (auto const& i : info_list) {
		free(i->str);
		free(i);
	}
	info_list.clear();
}

static void undo_append_undo_info(char command, int start, int end, char *str)
{
	UndoInfo *ui = (UndoInfo*)malloc(sizeof(UndoInfo));

	ui->command = command;
	ui->start = start;
	ui->end = end;
	ui->seq = seq_reserve;
	ui->str = str;

	seq_reserve = false;

	undo_list.push_back(ui);
}

static void undo_create_undo_info(char command, int start, int end, const char* text)
{
	bool seq_flag = false;
	char *str = strdup(text);
	int keyval = get_current_keyval();

	if (!undo_str.empty()) {
		if ((strlen(text) == 1) && (command == ui_tmp->command)) {
			switch (keyval) {
			case FL_BackSpace:
				if (end == ui_tmp->start)
					seq_flag = true;
				break;
			case FL_Delete:
				if (start == ui_tmp->start)
					seq_flag = true;
				break;
			case FL_Tab:
			case ' ':
				if (start == ui_tmp->end)
					seq_flag = true;
				break;
			default:
				if (start == ui_tmp->end)
					if (keyval && keyval < 0xF000)
						switch (prev_keyval) {
						case FL_Enter:
						case FL_Tab:
						case ' ':
							break;
						default:
							seq_flag = true;
						}
			}
		}
		if (seq_flag) {
			switch (command) {
			case BS:
				undo_str.insert(0, str);
				ui_tmp->start--;
				break;
			default:
				undo_str.append(str);
				ui_tmp->end++;
			}
			undo_clear_info_list(redo_list);
			prev_keyval = keyval;
			free(str);
			return;
		}
		undo_append_undo_info(ui_tmp->command, ui_tmp->start, ui_tmp->end, strdup(undo_str.c_str()));
		undo_str.clear();
	}

	if (!keyval && prev_keyval)
		undo_set_sequency(true);

	if (strlen(text) == 1 &&
		((keyval && keyval < 0xF000) ||
		  keyval == FL_BackSpace || keyval == FL_Delete || keyval == FL_Tab)) {
		ui_tmp->command = command;
		ui_tmp->start = start;
		ui_tmp->end = end;
		undo_str.clear();
		undo_str.append(str);
	} else
		undo_append_undo_info(command, start, end, strdup(str));

	undo_clear_info_list(redo_list);
	prev_keyval = keyval;
	clear_current_keyval();
	free(str);
}
