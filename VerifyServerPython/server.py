# server.py
#from site import PREFIXES

import grpc
import emailsend
import uuid
from concurrent import futures
import redismanager
import message_pb2
import message_pb2_grpc
from emailsend import send_email
from redismanager import setRedisExpire, getRedis


# 实现定义的方法
class VarifyService(message_pb2_grpc.VarifyServiceServicer):
    def GetVarifyCode(self, request, context):
        print(request.email)
        #组装redis的键
        key_ = redismanager.CODE_PREFIX + request.email
        #先在redis中查找是否已发送验证码，验证码的键为code_+email
        code_ = getRedis(key_)

        if code_ is None:
            #从UUID中截取6位数字作为验证码
            uuid_str = str(uuid.uuid4())
            code_ = uuid_str[0:4]
            #验证码存入Redis中
            setRedisExpire(key_, code_, 300)
        print(code_)
        send_email(request.email, code_)
        return message_pb2.GetVarifyRsp(error=0, email=request.email, code=code_)

#HelloResponse(message='Hello {msg}'.format(msg=request.name)))


def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    # 绑定处理器
    message_pb2_grpc.add_VarifyServiceServicer_to_server(VarifyService(), server)

    server.add_insecure_port('[::]:50051')
    server.start()
    print('gRPC 服务端已开启，端口为50051...')
    server.wait_for_termination()


if __name__ == '__main__':
    #send_email()
    serve()