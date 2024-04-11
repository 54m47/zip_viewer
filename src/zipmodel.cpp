#include <QStandardItemModel>
#include <QFont>
#include "../include/zipmodel.h"

struct zip_model_private
{
    zip_file_info *files;
    size_t num_files;
};

zip_model_private *create_zip_model_private(const char *zip_file_name)
{
    zip_model_private *private_data = new zip_model_private;
    private_data->files = nullptr;
    private_data->num_files = 0;

    if (open_zip_file(zip_file_name, &private_data->files, &private_data->num_files))
    {
        // Zip file opened successfully
    }
    else
    {
        delete private_data;
        private_data = nullptr;
    }

    return private_data;
}

void free_zip_model_private(zip_model_private *private_data)
{
    if (private_data)
    {
        free_zip_file_info_array(private_data->files, private_data->num_files);
        delete private_data;
    }
}

QStandardItemModel *create_zip_model(const char *zip_file_name, QObject *parent)
{
    QStandardItemModel *model = new QStandardItemModel(parent);
    zip_model_private *private_data = create_zip_model_private(zip_file_name);
    if (private_data)
    {
        model->setProperty("private_data", QVariant::fromValue(static_cast<void *>(private_data)));
        model->setHorizontalHeaderLabels({"Full Path", "Uncompressed Size", "Compressed Size"});
    }
    else
    {
        delete model;
        model = nullptr;
    }

    return model;
}

void free_zip_model(QStandardItemModel *model)
{
    zip_model_private *private_data = static_cast<zip_model_private *>(model->property("private_data").value<void *>());
    free_zip_model_private(private_data);
    delete model;
}

int zip_model_row_count(QStandardItemModel *model, const QModelIndex &parent)
{
    if (parent.isValid())
    {
        return 0;
    }

    zip_model_private *private_data = static_cast<zip_model_private *>(model->property("private_data").value<void *>());
    return private_data->num_files;
}

int zip_model_column_count(QStandardItemModel *model, const QModelIndex &parent)
{
    if (parent.isValid())
    {
        return 0;
    }

    return 3;
}

QVariant zip_model_data(QStandardItemModel *model, const QModelIndex &index, int role)
{
    if (!index.isValid())
    {
        return QVariant();
    }

    zip_model_private *private_data = static_cast<zip_model_private *>(model->property("private_data").value<void *>());
    if (role == Qt::DisplayRole)
    {
        int row = index.row();
        int column = index.column();
        if (row >= 0 && row < private_data->num_files)
        {
            switch (column)
            {
            case 0:
                return QString::fromLocal8Bit(private_data->files[row].file_name);
            case 1:
                return QString::number(private_data->files[row].uncompressed_size);
            case 2:
                return QString::number(private_data->files[row].compressed_size);
            }
        }
    }

    return QVariant();
}

QVariant zip_model_header_data(QStandardItemModel *model, int section, Qt::Orientation orientation, int role)
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return "Full Path";
        case 1:
            return "Uncompressed Size";
        case 2:
            return "Compressed Size";
        }
    }

    return QVariant();
}