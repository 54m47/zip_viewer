#include <QApplication>
#include "../include/zipviewer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow *main_window = create_zip_viewer_window();
    main_window->show();

    return app.exec();
}