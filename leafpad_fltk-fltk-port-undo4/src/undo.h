/*
 *  Leafpad - GTK+ based simple text editor
 *  Copyright (C) 2004-2005 Tarot Osuji
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __UNDO_H__
#define __UNDO_H__

#include <stdbool.h>
#include <FL/Fl_Text_Buffer.H>

void undo_init(Fl_Text_Buffer *buffer, void* view, void* window);
void undo_clear_all();
void undo_reset_modified_step();
void undo_set_sequency(bool seq);
void undo_set_sequency_reserve();
void undo_undo();
void undo_redo();

#endif /* __UNDO_H__ */
