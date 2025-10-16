#pragma once
#include "const.h"
#include "LogicSystem.h"
class HttpConnection :public std::enable_shared_from_this<HttpConnection>
{
public:
	friend class LogicSystem;
	//HttpConnection(tcp::socket socket);
	HttpConnection(boost::asio::io_context& ioc);
	void start();
	tcp::socket& GetSocket() {
		return _socket;
	}
private:
	void CheckDeadline();//��ⳬʱ
	void WriteResponse();//Ӧ��
	void HandleReq();//��������
	void PreParseGetParam();

	tcp::socket _socket;
	beast::flat_buffer _buffer{ 8192 };
	http::request<http::dynamic_body> _request;
	http::response<http::dynamic_body> _response;
	//����ڹ涨�� 60 ����û�в����������Զ��ر�����
	net::steady_timer deadline_{
		//���Ƚ�����������Ϊsocket��ִ����,�����ó�ʱʱ��Ϊ60��
		_socket.get_executor(), std::chrono::seconds(60)
	};//�����ǹ���
	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;

};

