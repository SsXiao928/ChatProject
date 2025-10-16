#include "AsioIOServicePool.h"
#include <iostream>
using namespace std;
AsioIOServicePool::AsioIOServicePool(std::size_t size):_ioServices(size),
_works(size), _nextIOService(0){
	for (size_t i = 0; i < size; ++i) {
		_works.push_back(std::make_unique<Work>(boost::asio::make_work_guard(_ioServices[i])));
	}

	for (std::size_t i = 0; i < size; ++i) {
		_threads.emplace_back([this, i]() {
			_ioServices[i].run();
			});
	}
}

AsioIOServicePool::~AsioIOServicePool() {
	Stop();
	std::cout << "AsioIOServicePool destruct" << endl;
}

boost::asio::io_context& AsioIOServicePool::GetIOService() {
	auto& service = _ioServices[_nextIOService++];
	if (_nextIOService == _ioServices.size()) {
		_nextIOService = 0;
	}
	return service;
}

//void AsioIOServicePool::Stop(){
//	//因为仅仅执行work.reset并不能让iocontext从run的状态中退出
//	//当iocontext已经绑定了读或写的监听事件后，还需要手动stop该服务。
//	for (auto& work : _works) {
//		//把服务先停止
//		work->get_io_context().stop();
//		work.reset();
//	}
//
//	for (auto& t : _threads) {
//		t.join();
//	}
//}

void AsioIOServicePool::Stop() {
	// 先取消所有 work_guard，让 io_context 能自动退出 run(
	// 指针指针已经析构
	//for (auto& work : _works) {
	//    work->reset();
	//}
	// 确保 io_context 自己也 stop 掉（防止有其他阻塞任务）
	for (auto& io : _ioServices) {
		io.stop();
	}
	for (auto& thread : _threads) {
		if (thread.joinable()) {
			thread.join();
		}
	}
}