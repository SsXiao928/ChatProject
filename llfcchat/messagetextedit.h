#ifndef MESSAGETEXTEDIT_H
#define MESSAGETEXTEDIT_H

#include <QObject>
#include <QTextEdit>
#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMimeType>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QPainter>
#include <QVector>
#include "global.h"


class MessageTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit MessageTextEdit(QWidget *parent = nullptr);

    ~MessageTextEdit();

    QVector<MsgInfo> getMsgList();
    //插入文件
    void insertFileFromUrl(const QStringList &urls);
signals:
    void send();//发送时发出这个信号

protected:
    void dragEnterEvent(QDragEnterEvent *event);//文件拖到这里处理
    void dropEvent(QDropEvent *event);//放下做哪些处理
    void keyPressEvent(QKeyEvent *e);//按回车处理

private:
    void insertImages(const QString &url);//插入图片
    void insertTextFile(const QString &url);//插入文本
    bool canInsertFromMimeData(const QMimeData *source) const;//能否插入多媒体资源
    void insertFromMimeData(const QMimeData *source);

private:
    bool isImage(QString url);//判断文件是否为图片
    void insertMsgList(QVector<MsgInfo> &list,QString flag, QString text, QPixmap pix);//把消息插入到消息列表

    QStringList getUrl(QString text);
    QPixmap getFileIconPixmap(const QString &url);//获取文件图标及大小信息，并转化成图片
    QString getFileSize(qint64 size);//获取文件大小

private slots:
    void textEditChanged();//输入变化

private:
    QVector<MsgInfo> mMsgList;//管理消息列表
    QVector<MsgInfo> mGetMsgList;//获取消息列表
};

#endif // MESSAGETEXTEDIT_H
