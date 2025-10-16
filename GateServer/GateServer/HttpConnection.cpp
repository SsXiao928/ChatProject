#include "HttpConnection.h"
//HttpConnection::HttpConnection(tcp::socket socket) :_socket(std::move(socket))
//{
//
//}
HttpConnection::HttpConnection(boost::asio::io_context& ioc) :_socket(ioc)
{

}
void HttpConnection::start() {
	auto self = shared_from_this();
	//为什么ec可以作为bool变量使用？
	//因为在boost中，error_code是一个类，重载了()运算符，所以可以直接作为bool变量使用。
	//_buffer存放原始的未解析的字节流，_request接收从_buffer中读取的数据并解析成更高层次的HTTP请求对象
	http::async_read(_socket, _buffer, _request, [self](beast::error_code ec, std::size_t bytes_transferred) {
		try {
			if (ec) {
				std::cout << "http read err is " << ec.what() << std::endl;
				return;
			}
			//消除未使用变量引起的编译器警告，同时保持代码的简洁和可读性。
			boost::ignore_unused(bytes_transferred);
			self->HandleReq();
			self->CheckDeadline();//超时就关闭
		}
		catch (std::exception& exp) {
			std::cout << "exception is " << exp.what() << std::endl;

		}
	});
}
//char 转为16进制
unsigned char ToHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}

unsigned char FromHex(unsigned char x)
{
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	else assert(0);
	return y;
}
//将字符串进行url编码
std::string UrlEncode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		//判断是否仅有数字和字母构成
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ') //为空字符
			strTemp += "+";
		else
		{
			//其他字符需要提前加%并且高四位和低四位分别转为16进制
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] & 0x0F);
		}
	}
	return strTemp;
}
//将字符串进行url解码
std::string UrlDecode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		//还原+为空
		if (str[i] == '+') strTemp += ' ';
		//遇到%将后面的两个字符从16进制转为char再拼接
		else if (str[i] == '%')
		{
			assert(i + 2 < length);
			unsigned char high = FromHex((unsigned char)str[++i]);
			unsigned char low = FromHex((unsigned char)str[++i]);
			strTemp += high * 16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
}

void HttpConnection::PreParseGetParam() {
	// 提取 URI  
	//获取请求路径，如/home
	//http://localhost:8080/get_test?key1=value1&key2=value2
	//request uri is  get_test?key1=value1&key2=value2
	auto uri = _request.target(); 
	std::cout << "request uri is " << uri << std::endl;
	// 查找查询字符串的开始位置（即 '?' 的位置）  
	auto query_pos = uri.find('?');
	if (query_pos == std::string::npos) {//npos表示没有找到
		_get_url = uri;
		return;
	}
	_get_url = uri.substr(0, query_pos);
	//get url is /get_test
	std::cout << "get url is " << _get_url << std::endl;
	std::string query_string = uri.substr(query_pos + 1);
	std::string key;
	std::string value;
	size_t pos = 0;
	while ((pos = query_string.find('&')) != std::string::npos) {
		auto pair = query_string.substr(0, pos);
		size_t eq_pos = pair.find('=');
		if (eq_pos != std::string::npos) {
			key = UrlDecode(pair.substr(0, eq_pos)); // 假设有 url_decode 函数来处理URL解码  
			value = UrlDecode(pair.substr(eq_pos + 1));
			_get_params[key] = value;
		}
		query_string.erase(0, pos + 1);
	}
	// 处理最后一个参数对（如果没有 & 分隔符）  
	if (!query_string.empty()) {
		size_t eq_pos = query_string.find('=');
		if (eq_pos != std::string::npos) {
			key = UrlDecode(query_string.substr(0, eq_pos));
			value = UrlDecode(query_string.substr(eq_pos + 1));
			_get_params[key] = value;
		}
	}
}


void HttpConnection::HandleReq() {
	//设置版本
	_response.version(_request.version());
	_response.keep_alive(false);
	if (_request.method() == http::verb::get) {
		PreParseGetParam();
		bool success = LogicSystem::GetInstance()->HandleGet(_get_url, shared_from_this()); 
		if (!success) {
			_response.result(http::status::not_found);
			_response.set(http::field::content_type, "text/plain");//设置回应类型
			//写回应内容
			beast::ostream(_response.body()) << "url not found!\r\n";
			WriteResponse();
			return;
		}
		//类型和body在处理时已写
		_response.result(http::status::ok);
		_response.set(http::field::server, "GateServer");
		WriteResponse();
		return;
	}

	if (_request.method() == http::verb::post) {
		bool success = LogicSystem::GetInstance()->HandlePost(_request.target(), shared_from_this());
		if (!success) {
			_response.result(http::status::not_found);
			_response.set(http::field::content_type, "text/plain");
			beast::ostream(_response.body()) << "url not found!\r\n";
			WriteResponse();
			return;
		}
		_response.result(http::status::ok);
		_response.set(http::field::server, "GateServer");
		WriteResponse();
		return;
	}

}

void HttpConnection::WriteResponse()
{
	auto self = shared_from_this();
	_response.content_length(_response.body().size());//设置长度
	http::async_write(_socket, _response, [self](beast::error_code ec, std::size_t bytes_transferred) {
		self->_socket.shutdown(tcp::socket::shutdown_send, ec);//关闭发送端
		self->deadline_.cancel(); //取消定时器
		});
}

void HttpConnection::CheckDeadline() {
	auto self = shared_from_this();
	deadline_.async_wait([self](beast::error_code ec) {
		if (!ec) {
			//未出错
			self->_socket.close();
		}
		});
}