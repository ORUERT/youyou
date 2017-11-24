#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font  = qApp->font();
    font.setFamily("Calibri");
    qApp->setFont(font);

    QString FileName = QCoreApplication::applicationDirPath() + "/config.ini";
    if(false == QFile::exists(FileName))
    {
        QSettings *pConfig = new QSettings(FileName, QSettings::IniFormat);
        pConfig->setValue("Basic/Hide", false);
        pConfig->setValue("Basic/Color", "255 214 228");
        pConfig->setValue("Basic/UploadFlow",0);
        pConfig->setValue("Basic/DownloadFlow",0);
        pConfig->setValue("Basic/preUploadFlow",0);
        pConfig->setValue("Basic/preDownloadFlow",0);
        pConfig->setValue("Basic/DisplaySize",100);
        pConfig->setValue("Other/Version", 1.0);
        delete pConfig;
    }
    Widget w;
    w.setWindowFlags(w.windowFlags() | Qt::WindowStaysOnTopHint);
    w.show();

    return a.exec();
}
