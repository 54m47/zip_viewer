#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include "../include/zipviewer.h"
#include "../include/zipmodel.h"
#include "../include/ziputils.h"

QMainWindow *create_zip_viewer_window()
{
    QMainWindow *main_window = new QMainWindow();
    main_window->setWindowTitle("ZIP Viewer");
    main_window->resize(800, 600);

    QTreeView *tree_view = new QTreeView(main_window);
    tree_view->setAlternatingRowColors(true);
    tree_view->setRootIsDecorated(false);
    tree_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    tree_view->setSelectionMode(QAbstractItemView::SingleSelection);

    QStandardItemModel *model = new QStandardItemModel(tree_view);
    model->setHorizontalHeaderLabels({"Full Path", "Uncompressed Size", "Compressed Size"});
    tree_view->setModel(model);

    // Set column alignment
    tree_view->header()->setStretchLastSection(false);
    tree_view->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    tree_view->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tree_view->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    // Set column alignment for the second and third columns
    tree_view->setColumnWidth(1, 150);
    tree_view->setColumnWidth(2, 150);

    QPushButton *open_button = new QPushButton("Open", main_window);
    QPushButton *extract_button = new QPushButton("Extract", main_window);

    QWidget *central_widget = new QWidget(main_window);
    QVBoxLayout *layout = new QVBoxLayout(central_widget);
    layout->addWidget(tree_view);

    QHBoxLayout *button_layout = new QHBoxLayout();
    button_layout->addWidget(open_button);
    button_layout->addWidget(extract_button);

    layout->addLayout(button_layout);

    main_window->setCentralWidget(central_widget);

    QObject::connect(open_button, &QPushButton::clicked, [tree_view]()
    {
        open_zip_file(tree_view);
    });

    QObject::connect(extract_button, &QPushButton::clicked, [tree_view]()
    {
        extract_selected_file(tree_view);
    });

    return main_window;
}

void open_zip_file(QTreeView *tree_view)
{
    QString file_name = QFileDialog::getOpenFileName(tree_view, "Open ZIP File", "", "ZIP Files (*.zip)");
    if (!file_name.isEmpty())
    {
        QStandardItemModel *model = qobject_cast<QStandardItemModel *>(tree_view->model());
        model->clear();
        model->setHorizontalHeaderLabels({"Full Path", "Uncompressed Size", "Compressed Size"});

        zip_file_info *files = nullptr;
        size_t num_files = 0;

        if (open_zip_file(file_name.toLocal8Bit().data(), &files, &num_files))
        {
            for (size_t i = 0; i < num_files; ++i)
            {
                QList<QStandardItem *> row_items;
                row_items.append(new QStandardItem(QString::fromLocal8Bit(files[i].file_name)));
                row_items.append(new QStandardItem(QString::number(files[i].uncompressed_size)));
                row_items.append(new QStandardItem(QString::number(files[i].compressed_size)));
                model->appendRow(row_items);
            }

            free_zip_file_info_array(files, num_files);
        }

        // Adjust column widths based on content
        tree_view->resizeColumnToContents(0);
        tree_view->resizeColumnToContents(1);
        tree_view->resizeColumnToContents(2);

        // Set column alignment
        tree_view->header()->setStretchLastSection(false);
        tree_view->header()->setSectionResizeMode(0, QHeaderView::Stretch);
        tree_view->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        tree_view->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    }
}

void extract_selected_file(QTreeView *tree_view)
{
    QModelIndexList selected_indexes = tree_view->selectionModel()->selectedIndexes();
    if (selected_indexes.isEmpty())
    {
        QMessageBox::warning(tree_view, "No Selection", "Please select a file to extract.");
        return;
    }

    QModelIndex selected_index = selected_indexes.first();
    QString file_name = selected_index.data(Qt::DisplayRole).toString();

    QString zip_file_name = QFileDialog::getOpenFileName(tree_view, "Select ZIP File", "", "ZIP Files (*.zip)");
    if (zip_file_name.isEmpty())
    {
        return;
    }

    QString output_path = QFileDialog::getExistingDirectory(tree_view, "Select Output Directory");
    if (output_path.isEmpty())
    {
        return;
    }

    if (extract_file(zip_file_name.toLocal8Bit().data(), file_name.toLocal8Bit().data(), output_path.toLocal8Bit().data()))
    {
        QMessageBox::information(tree_view, "Extraction Successful", "The selected file has been extracted successfully.");
    }
    else
    {
        QMessageBox::critical(tree_view, "Extraction Failed", "Failed to extract the selected file.");
    }
}