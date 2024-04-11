#ifndef ZIPMODEL_H
#define ZIPMODEL_H

#include <QStandardItemModel>
#include <QModelIndex>
#include <QVariant>
#include "ziputils.h"

struct zip_model_private;

QStandardItemModel *create_zip_model(const char *zip_file_name, QObject *parent = nullptr);
void free_zip_model(QStandardItemModel *model);

int zip_model_row_count(QStandardItemModel *model, const QModelIndex &parent);
int zip_model_column_count(QStandardItemModel *model, const QModelIndex &parent);
QVariant zip_model_data(QStandardItemModel *model, const QModelIndex &index, int role);
QVariant zip_model_header_data(QStandardItemModel *model, int section, Qt::Orientation orientation, int role);

#endif