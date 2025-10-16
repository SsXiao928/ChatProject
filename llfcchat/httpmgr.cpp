#include "httpmgr.h"

HttpMgr::~HttpMgr()
{

}

HttpMgr::HttpMgr()
{
    connect(this, &HttpMgr::sig_http_finish, this, &HttpMgr::slot_http_finish);
}

void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)
{
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkRequest request(url);
    //http请求类型
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length()));
    auto self = shared_from_this();
    //connect 函数是用来将信号与槽连接的关键函数。当一个信号被触发时，它会通知与之关联的槽函数执行相应的操作。
    //QObject::connect(sender, SIGNAL(signal), receiver, SLOT(slot));
    //sender：信号的发送者（发出信号的对象）。
    //SIGNAL(signal)：信号，通常是类中定义的某个信号。
    //receiver：信号的接收者（处理信号的对象）。
    //SLOT(slot)：槽，接收到信号时调用的函数
    //发送请求后收到回应
    QNetworkReply* reply = _manager.post(request, data);
    //finished() 信号是由 QNetworkReply 对象发出的，它表示与网络相关的操作已完成。
    //网络请求完成后（无论成功还是失败），QNetworkReply 会发出 finished 信号。
    //发送完后，确定是否完成
    QObject::connect(reply, &QNetworkReply::finished, [self, reply, req_id, mod](){
        //处理错误情况
        if(reply->error() != QNetworkReply::NoError){
            qDebug() << reply->errorString();
            //发送信号通知完成
            emit self->sig_http_finish(req_id, "", ErrorCodes::ERR_NETWORK, mod);
            //回收reply
            reply->deleteLater();
            return;
        }

        //无错误
        QString res = reply->readAll();
        //发送信号通知完成
        emit self->sig_http_finish(req_id, res, ErrorCodes::SUCCESS, mod);
        reply->deleteLater();
        return;

    });//收到应答后reply会发出QNetworkReply信号
    //当一个 HTTP 请求完成（无论是成功还是失败）时，QNetworkReply 会自动触发QNetworkReply::finished信号
}

//收到的回复派发给指定模块
void HttpMgr::slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod)
{
    if(mod == Modules::REGISTERMOD)
    {
       //发送信号通知指定模块http的响应结束
        //信号的参数 在信号被发射时会传递给 槽函数的参数
        emit sig_reg_mod_finish(id, res, err);
    }

    if(mod == Modules::RESETMOD)
    {
        emit sig_reset_mod_finish(id, res, err);
    }

    if(mod == Modules::LOGINMOD)
    {
        emit sig_login_mod_finish(id, res, err);
    }
}
