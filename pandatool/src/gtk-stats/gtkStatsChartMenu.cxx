/**
 * PANDA 3D SOFTWARE
 * Copyright (c) Carnegie Mellon University.  All rights reserved.
 *
 * All use of this software is subject to the terms of the revised BSD
 * license.  You should have received a copy of this license along
 * with this source code in a file named "LICENSE."
 *
 * @file gtkStatsChartMenu.cxx
 * @author drose
 * @date 2006-01-16
 */

#include "gtkStatsChartMenu.h"
#include "gtkStatsMonitor.h"

/**
 *
 */
GtkStatsChartMenu::
GtkStatsChartMenu(GtkStatsMonitor *monitor, int thread_index) :
  _monitor(monitor),
  _thread_index(thread_index)
{
  _menu = gtk_menu_new();
  gtk_widget_show(_menu);
  do_update();
}

/**
 *
 */
GtkStatsChartMenu::
~GtkStatsChartMenu() {
}

/**
 * Returns the gtk widget for this particular menu.
 */
GtkWidget *GtkStatsChartMenu::
get_menu_widget() {
  return _menu;
}

/**
 * Adds the menu to the end of the indicated menu bar.
 */
void GtkStatsChartMenu::
add_to_menu_bar(GtkWidget *menu_bar, int position) {
  const PStatClientData *client_data = _monitor->get_client_data();
  std::string thread_name;
  if (_thread_index == 0) {
    // A special case for the main thread.
    thread_name = "Graphs";
  } else {
    thread_name = client_data->get_thread_name(_thread_index);
  }

  _menu_item = gtk_menu_item_new_with_label(thread_name.c_str());
  gtk_widget_show(_menu_item);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(_menu_item), _menu);

  gtk_menu_shell_insert(GTK_MENU_SHELL(menu_bar), _menu_item, position);
}

/**
 * Removes the menu from the menu bar.
 */
void GtkStatsChartMenu::
remove_from_menu_bar(GtkWidget *menu_bar) {
  gtk_container_remove(GTK_CONTAINER(menu_bar), _menu_item);
}

/**
 * Checks to see if the menu needs to be updated (e.g.  because of new data
 * from the client), and updates it if necessary.
 */
void GtkStatsChartMenu::
check_update() {
  PStatView &view = _monitor->get_view(_thread_index);
  if (view.get_level_index() != _last_level_index) {
    do_update();
  }
}

/**
 * Unconditionally updates the menu with the latest data from the client.
 */
void GtkStatsChartMenu::
do_update() {
  PStatView &view = _monitor->get_view(_thread_index);
  _last_level_index = view.get_level_index();

  // First, remove all of the old entries from the menu.
  gtk_container_foreach(GTK_CONTAINER(_menu), remove_menu_child, _menu);

  // Now rebuild the menu with the new set of entries.

  if (_thread_index == 0) {
    // Timeline goes first.
    {
      GtkStatsMonitor::MenuDef smd(_thread_index, -1, GtkStatsMonitor::CT_timeline, false);
      const GtkStatsMonitor::MenuDef *menu_def = _monitor->add_menu(smd);

      GtkWidget *menu_item = gtk_menu_item_new_with_label("Timeline");
      gtk_widget_show(menu_item);
      gtk_menu_shell_append(GTK_MENU_SHELL(_menu), menu_item);

      g_signal_connect(G_OBJECT(menu_item), "activate",
                       G_CALLBACK(GtkStatsMonitor::menu_activate),
                       (void *)menu_def);
    }

    // And the piano roll (even though it's not very useful nowadays)
    {
      GtkStatsMonitor::MenuDef smd(_thread_index, -1, GtkStatsMonitor::CT_piano_roll, false);
      const GtkStatsMonitor::MenuDef *menu_def = _monitor->add_menu(smd);

      GtkWidget *menu_item = gtk_menu_item_new_with_label("Piano Roll");
      gtk_widget_show(menu_item);
      gtk_menu_shell_append(GTK_MENU_SHELL(_menu), menu_item);

      g_signal_connect(G_OBJECT(menu_item), "activate",
                       G_CALLBACK(GtkStatsMonitor::menu_activate),
                       (void *)menu_def);
    }

    GtkWidget *sep = gtk_separator_menu_item_new();
    gtk_widget_show(sep);
    gtk_menu_shell_append(GTK_MENU_SHELL(_menu), sep);
  }

  // The menu item(s) for the thread's frame time goes second.
  add_view(_menu, view.get_top_level(), false);

  bool needs_separator = true;

  // And then the menu item(s) for each of the level values.
  const PStatClientData *client_data = _monitor->get_client_data();
  int num_toplevel_collectors = client_data->get_num_toplevel_collectors();
  for (int tc = 0; tc < num_toplevel_collectors; tc++) {
    int collector = client_data->get_toplevel_collector(tc);
    if (client_data->has_collector(collector) &&
        client_data->get_collector_has_level(collector, _thread_index)) {

      // We put a separator between the above frame collector and the first
      // level collector.
      if (needs_separator) {
        GtkWidget *sep = gtk_separator_menu_item_new();
        gtk_widget_show(sep);
        gtk_menu_shell_append(GTK_MENU_SHELL(_menu), sep);

        needs_separator = false;
      }

      PStatView &level_view = _monitor->get_level_view(collector, _thread_index);
      add_view(_menu, level_view.get_top_level(), true);
    }
  }

  // For the main thread menu, also some options relating to all graph windows.
  if (_thread_index == 0) {
    GtkWidget *sep = gtk_separator_menu_item_new();
    gtk_widget_show(sep);
    gtk_menu_shell_append(GTK_MENU_SHELL(_menu), sep);

    {
      GtkWidget *menu_item = gtk_menu_item_new_with_label("Close All Graphs");
      gtk_widget_show(menu_item);
      gtk_menu_shell_append(GTK_MENU_SHELL(_menu), menu_item);

      g_signal_connect(G_OBJECT(menu_item), "activate",
                       G_CALLBACK(activate_close_all),
                       (void *)_monitor);
    }

    {
      GtkWidget *menu_item = gtk_menu_item_new_with_label("Reopen Default Graphs");
      gtk_widget_show(menu_item);
      gtk_menu_shell_append(GTK_MENU_SHELL(_menu), menu_item);

      g_signal_connect(G_OBJECT(menu_item), "activate",
                       G_CALLBACK(activate_reopen_default),
                       (void *)_monitor);
    }

    {
      GtkWidget *menu_item = gtk_menu_item_new_with_label("Save Current Layout as Default");
      gtk_widget_show(menu_item);
      gtk_menu_shell_append(GTK_MENU_SHELL(_menu), menu_item);

      g_signal_connect(G_OBJECT(menu_item), "activate",
                       G_CALLBACK(activate_save_default),
                       (void *)_monitor);
    }
  }
}

/**
 * Adds a new entry or entries to the menu for the indicated view and its
 * children.
 */
void GtkStatsChartMenu::
add_view(GtkWidget *parent_menu, const PStatViewLevel *view_level,
         bool show_level) {
  int collector = view_level->get_collector();

  const PStatClientData *client_data = _monitor->get_client_data();
  std::string collector_name = client_data->get_collector_name(collector);

  int num_children = view_level->get_num_children();
  if (show_level && num_children == 0) {
    // For a level collector without children, no point in making a submenu.
    GtkStatsMonitor::MenuDef smd(_thread_index, collector, GtkStatsMonitor::CT_strip_chart, show_level);
    const GtkStatsMonitor::MenuDef *menu_def = _monitor->add_menu(smd);

    GtkWidget *menu_item = gtk_menu_item_new_with_label(collector_name.c_str());
    gtk_widget_show(menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(parent_menu), menu_item);

    g_signal_connect(G_OBJECT(menu_item), "activate",
                     G_CALLBACK(GtkStatsMonitor::menu_activate),
                     (void *)menu_def);
    return;
  }

  GtkWidget *menu;
  if (!show_level && collector == 0 && num_children == 0) {
    // Root collector without children, just add the options directly to the
    // parent menu.
    menu = parent_menu;
  }
  else {
    // Create a submenu.
    GtkWidget *submenu_item = gtk_menu_item_new_with_label(collector_name.c_str());
    gtk_widget_show(submenu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(parent_menu), submenu_item);

    menu = gtk_menu_new();
    gtk_widget_show(menu);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(submenu_item), menu);
  }

  {
    GtkStatsMonitor::MenuDef smd(_thread_index, collector, GtkStatsMonitor::CT_strip_chart, show_level);
    const GtkStatsMonitor::MenuDef *menu_def = _monitor->add_menu(smd);

    GtkWidget *menu_item = gtk_menu_item_new_with_label("Open Strip Chart");
    gtk_widget_show(menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    g_signal_connect(G_OBJECT(menu_item), "activate",
                     G_CALLBACK(GtkStatsMonitor::menu_activate),
                     (void *)menu_def);
  }

  if (!show_level) {
    if (collector == 0 && num_children == 0) {
      collector = -1;
    }

    GtkStatsMonitor::MenuDef smd(_thread_index, collector, GtkStatsMonitor::CT_flame_graph, show_level);
    const GtkStatsMonitor::MenuDef *menu_def = _monitor->add_menu(smd);

    GtkWidget *menu_item = gtk_menu_item_new_with_label("Open Flame Graph");
    gtk_widget_show(menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    g_signal_connect(G_OBJECT(menu_item), "activate",
                     G_CALLBACK(GtkStatsMonitor::menu_activate),
                     (void *)menu_def);
  }

  if (num_children > 0) {
    GtkWidget *sep = gtk_separator_menu_item_new();
    gtk_widget_show(sep);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), sep);

    // Reverse the order since the menus are listed from the top down; we want
    // to be visually consistent with the graphs, which list these labels from
    // the bottom up.
    for (int c = num_children - 1; c >= 0; c--) {
      add_view(menu, view_level->get_child(c), show_level);
    }
  }
}

/**
 * Removes a previous menu child from the menu.
 */
void GtkStatsChartMenu::
remove_menu_child(GtkWidget *widget, gpointer data) {
  GtkWidget *menu = (GtkWidget *)data;
  gtk_container_remove(GTK_CONTAINER(menu), widget);
}

/**
 * Callback for Close All Graphs.
 */
void GtkStatsChartMenu::
activate_close_all(GtkWidget *widget, gpointer data) {
  GtkStatsMonitor *monitor = (GtkStatsMonitor *)data;
  monitor->remove_all_graphs();
}

/**
 * Callback for Reopen Default Graphs.
 */
void GtkStatsChartMenu::
activate_reopen_default(GtkWidget *widget, gpointer data) {
  GtkStatsMonitor *monitor = (GtkStatsMonitor *)data;
  monitor->remove_all_graphs();
  monitor->open_default_graphs();
}

/**
 * Callback for Save Current Layout as Default.
 */
void GtkStatsChartMenu::
activate_save_default(GtkWidget *widget, gpointer data) {
  GtkStatsMonitor *monitor = (GtkStatsMonitor *)data;
  monitor->save_default_graphs();
}
