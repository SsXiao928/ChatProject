#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_get_code_clicked();

    void on_sure_btn_clicked();

    void on_pushButton_clicked();
    void on_cancel_btn_clicked();

public slots:
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);

private:
    //检测信息释放合法
    bool checkUserValid();
    bool checkEmailValid();
    bool checkPassValid();
    bool checkVarifyValid();
    bool checkConfirmValid();
    void AddTipErr(TipErr te, QString tips);
    void DelTipErr(TipErr te);

    //登陆后切换提示界面
    void ChangeTipPage();

    //初始化http请求处理器
    void initHttpHandlers();
    void showTip(QString str, bool b_ok);
    QMap<TipErr, QString> _tip_errs;//存储填写注册信息时的不合法类型
    Ui::RegisterDialog *ui;
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;

    //返回计时
    QTimer *_countdown_timer;//计时器
    int _countdown;//倒计时数字
signals:
    void sigSwitchLogin();
};

#endif // REGISTERDIALOG_H
