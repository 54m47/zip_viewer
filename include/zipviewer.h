#ifndef ZIPVIEWER_H
#define ZIPVIEWER_H

#include <QMainWindow>
#include <QTreeView>
#include <QPushButton>

QMainWindow *create_zip_viewer_window();
void open_zip_file(QTreeView *tree_view);
void extract_selected_file(QTreeView *tree_view);

#endif