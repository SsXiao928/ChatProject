#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include "global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //读取qss样式文件
    QFile qss(":/style/stylesheet.qss");
    if(qss.open(QFile::ReadOnly)){
        qDebug("open success");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }else{
        qDebug("Open failed");
    }
    //读取配置文件
    QString fileName = "config.ini";
    QString app_path = QCoreApplication::applicationDirPath();//应用执行的目录

    //QDir::separator()是分隔符，linux下为斜杠，windows为反斜杠
    QString config_path = QDir::toNativeSeparators(app_path + QDir::separator() + fileName);//转本地
    //读取config.ini配置
    QSettings settings(config_path, QSettings::IniFormat);// QSettings::IniFormat表示读取的是一个ini文件
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();
    gate_url_prefix = "http://" + gate_host + ":" + gate_port;

    MainWindow w;
    w.show();
    return a.exec();
}
