#include "CServer.h"
#include "HttpConnection.h"
#include "AsioIOServicePool.h"
CServer::CServer(boost::asio::io_context& ioc, unsigned short& port):_ioc(ioc),
_acceptor(ioc, tcp::endpoint(tcp::v4(), port))
{
	
}

void CServer::start()
{
	auto self = shared_from_this();
	auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
	std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(io_context);
	//����ecΪtrue��δ����Ϊfalse
	_acceptor.async_accept(new_con->GetSocket(), [self, new_con](beast::error_code ec) {
		try {
			//�������������ӣ�����������������
			if(ec){
				self->start();
				return;
			}
			//���������ӣ����Ҵ���HttpConnection������������
			//std::make_shared<HttpConnection>(std::move(self->_socket))->start();
			new_con->start();

			//����������������
			self->start();
		}
		catch (std::exception& e) {
		}
	});
}