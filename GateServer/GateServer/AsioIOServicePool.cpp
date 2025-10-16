#include "AsioIOServicePool.h"
#include <iostream>
using namespace std;
AsioIOServicePool::AsioIOServicePool(std::size_t size):_ioServices(size),
_works(0), _nextIOService(0) {
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
	std::cout << "AsioIOServicePool destruct" << std::endl;
}

boost::asio::io_context& AsioIOServicePool::GetIOService() {
    auto& service = _ioServices[_nextIOService];
    if (_nextIOService == _ioServices.size()) {
		_nextIOService = 0;
    }
    return service;
}

void AsioIOServicePool::Stop() {
    // ��ȡ������ work_guard���� io_context ���Զ��˳� run(
    for (auto& work : _works) {
        work->reset();
    }
    // ȷ�� io_context �Լ�Ҳ stop ������ֹ��������������
    for (auto& io : _ioServices) {
        io.stop();
    }
    for (auto& thread : _threads) {
        //�߳̿��ܻ�����
        if (thread.joinable()) {
            thread.join();
        }
    }
} 