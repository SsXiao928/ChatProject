#include "loadingdlg.h"
#include "ui_loadingdlg.h"
#include <QMovie>

LoadingDlg::LoadingDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoadingDlg)
{
    ui->setupUi(this);
    //Qt::WindowStaysOnTopHint 一直处于最上层
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    //获取屏幕尺寸
    setFixedSize(parent->size());//设置对话框尺寸为全屏尺寸

    QMovie *movie = new QMovie(":/res/loading.gif");
    //loading_lb析构时，movie也会析构
    ui->loading_lb->setMovie(movie);
    movie->start();

}

LoadingDlg::~LoadingDlg()
{
    delete ui;
}
