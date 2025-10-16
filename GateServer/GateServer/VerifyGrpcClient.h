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
		//VarifyService::NewStub(channel)������ʱ������pushͨ���ƶ�����������
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
				return true; //���ֹͣ�ˣ�ֱ�ӷ���
			}
			//������в��գ�����false�������̹߳��𲢽������������ִ�У�		
			//�ڱ���߳�ͨ��notify_all����֮ǰ���̻߳�һֱ����������
			//���Ѻ��̻߳����»�ȡ��
			return !connections_.empty(); 
			});
		if (b_stop_) {
			return nullptr; //���ֹͣ�ˣ����ؿ�ָ��
		}
		//t������в��գ�����һ�����ӣ�move����Ϊ���ܿ�����
		auto context = std::move(connections_.front());
		connections_.pop();
		return context;
	}

	void returnConnection(std::unique_ptr<VarifyService::Stub> connection) {
		std::lock_guard<std::mutex> lock(mutex_);
		if (b_stop_) {
			return; //���ֹͣ�ˣ�ֱ�ӷ���
		}
		if (connections_.size() < poolSize_) {
			connections_.push(std::move(connection)); //�������δ�����Ż�����
		} else {
			//����������ˣ���������
			connection.reset();
		}
		cond_.notify_one(); //����һ���ȴ����߳�
	}

private:
	std::atomic<bool> b_stop_;//�Ƿ�ֹͣ
	size_t poolSize_;
	std::string host_;
	std::string port_;
	//Stub����unique_ptr����
	std::queue<std::unique_ptr<VarifyService::Stub>> connections_;
	std::condition_variable cond_;
	//���ƶ��е��̰߳�ȫ
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
		//grpcͨ�����ͨ���ͷ�����ͨ��
		//std::shared_ptr<Channel> channel = grpc::CreateChannel("127.0.0.1:50051",
		//	grpc::InsecureChannelCredentials());
		//stub_ = VarifyService::NewStub(channel);
	//}
	//std::unique_ptr<VarifyService::Stub> stub_;
	VerifyGrpcClient();
	std::unique_ptr<RPConPool> pool_;
};

