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
	void CheckDeadline();//检测超时
	void WriteResponse();//应答
	void HandleReq();//处理请求
	void PreParseGetParam();

	tcp::socket _socket;
	beast::flat_buffer _buffer{ 8192 };
	http::request<http::dynamic_body> _request;
	http::response<http::dynamic_body> _response;
	//如果在规定的 60 秒内没有操作，它会自动关闭连接
	net::steady_timer deadline_{
		//首先将调度器设置为socket的执行器,再设置超时时间为60秒
		_socket.get_executor(), std::chrono::seconds(60)
	};//这里是构造
	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;

};

