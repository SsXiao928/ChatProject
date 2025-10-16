#pragma once
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory> // for std::enable_shared_from_this
#include <iostream>
#include "Singleton.h"
#include <functional>
#include <map>
#include <unordered_map>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <boost/filesystem.hpp>//处理配置文件
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp> // read_ini的解析器
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <hiredis.h>
#include <cassert>
#include <memory>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

enum ErrorCodes {
	Success = 0,
	Error_Json = 1001, //Json解析错误
	RPCFailed = 1002, //RPC请求错误
	VarifyExpired = 1003,  // 验证码过期
	VarifyCodeErr = 1004, //验证码错误
	UserExist = 1005, //用户已存在
	PasswdErr = 1006, //密码错误
	EmailNotMatch = 1007, //邮箱不匹配
	PasswdUpFailed = 1008, //更新密码失败
	PasswdInvalid = 1009, //密码更新失败
	RPCGetFailed = 1010, //获取RPC请求失败
};


//Defer类
class Defer {
public:
	//接受一个lambda表达式或指针
	Defer(std::function<void()> func) :func_(func) {}
	//析构函数中执行传入的函数
	~Defer() {
		func_();
	}
private:
	std::function<void()> func_;
};


#define CODEPREFIX "code_"

//数据库中的密文密码转明文
//std::function<std::string(std::string)> xorString = [](std::string input) {
//	std::string result = input;// 复制原始字符串，以便修改
//	int length = input.length();
//	length = length % 255;
//	for (int i = 0; i < length; ++i) {
//		//对每个字符进行异或操作
//		//注意：这里假设字符都是ASCII，因此直接转换为QChar
//		//字符的 Unicode 值通常是通过 wchar_t 类型（宽字符）来表示的。可以通过将 wchar_t 转换为整数来获取其 Unicode 值
//		wchar_t c = input[i];
//		result[i] = char(static_cast<unsigned short>(static_cast<int>(c) ^ static_cast<unsigned short>(length)));
//	}
//	return result;
//	};

//这里是非单例模式实现，后面改成单例模式了
//class ConfigMgr; //前向声明，避免互相包含头文件
//extern ConfigMgr gCfgMgr; //全局配置管理器,在这里声明，实际在GateServer.cpp中定义（main函数）