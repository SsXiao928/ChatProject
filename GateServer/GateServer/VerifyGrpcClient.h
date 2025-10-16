#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "const.h"
#include "Singleton.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

class RPConPool {
public:
	RPConPool(size_t poolsize, std::string host, std::string port):
	poolSize_(poolsize),host_(host), port_(port), b_stop_(false) {
		std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port,
			grpc::InsecureChannelCredentials());
		//VarifyService::NewStub(channel)返回临时变量，push通过移动构造存入队列
		connections_.push(VarifyService::NewStub(channel));
	}
	~RPConPool() {
		std::lock_guard<std::mutex> lock(mutex_);
		Close();
		while(!connections_.empty()) {
			connections_.pop();
		}
	}
	void Close() {
		b_stop_ = true;
		cond_.notify_all();
	}
	std::unique_ptr<VarifyService::Stub> GetConnection() {
		std::unique_lock<std::mutex> lock(mutex_);
		cond_.wait(lock, [this]() {
			if (b_stop_) {
				return true; //如果停止了，直接返回
			}
			//如果队列不空，返回false，整个线程挂起并解锁（不会继续执行）		
			//在别的线程通过notify_all唤醒之前，线程会一直阻塞在这里
			//唤醒后线程会重新获取锁
			return !connections_.empty(); 
			});
		if (b_stop_) {
			return nullptr; //如果停止了，返回空指针
		}
		//t如果队列不空，返回一个连接（move，因为不能拷贝）
		auto context = std::move(connections_.front());
		connections_.pop();
		return context;
	}

	void returnConnection(std::unique_ptr<VarifyService::Stub> connection) {
		std::lock_guard<std::mutex> lock(mutex_);
		if (b_stop_) {
			return; //如果停止了，直接返回
		}
		if (connections_.size() < poolSize_) {
			connections_.push(std::move(connection)); //如果队列未满，放回连接
		} else {
			//如果队列满了，销毁连接
			connection.reset();
		}
		cond_.notify_one(); //唤醒一个等待的线程
	}

private:
	std::atomic<bool> b_stop_;//是否停止
	size_t poolSize_;
	std::string host_;
	std::string port_;
	//Stub就是unique_ptr类型
	std::queue<std::unique_ptr<VarifyService::Stub>> connections_;
	std::condition_variable cond_;
	//控制队列的线程安全
	std::mutex mutex_;
};

class VerifyGrpcClient:public Singleton<VerifyGrpcClient>
{
	friend class Singleton<VerifyGrpcClient>;
public:
	GetVarifyRsp GetVarifyCode(std::string email) {
		ClientContext context;
		GetVarifyRsp reply;
		GetVarifyReq request;
		request.set_email(email);
		auto stub = pool_->GetConnection();
		Status status = stub->GetVarifyCode(&context, request, &reply);
		if (status.ok()) {
			pool_->returnConnection(std::move(stub));
			return reply;
		}
		else {
			pool_->returnConnection(std::move(stub));
			reply.set_error(ErrorCodes::RPCFailed);
			return reply;
		}
	}

private:
	//VerifyGrpcClient() {
		//grpc通过这个通道和服务器通信
		//std::shared_ptr<Channel> channel = grpc::CreateChannel("127.0.0.1:50051",
		//	grpc::InsecureChannelCredentials());
		//stub_ = VarifyService::NewStub(channel);
	//}
	//std::unique_ptr<VarifyService::Stub> stub_;
	VerifyGrpcClient();
	std::unique_ptr<RPConPool> pool_;
};

