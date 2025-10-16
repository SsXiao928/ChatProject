import redis

#验证码前缀
CODE_PREFIX = "code_"

# 方式1：直接创建连接（简单场景）
r = redis.Redis(
    host='127.0.0.1',  # Redis服务器地址，默认本地
    port=6380,         # 端口，默认6379
    db=0,              # 数据库编号（默认0，Redis支持16个数据库）
    password='123456',     # 若Redis设置了密码，需填写
    decode_responses=True  # 自动将返回值转为字符串（默认返回bytes）
)

def setRedisExpire(key,value, exptime):
    r.set(key, value, exptime)  # ex=秒，px=毫秒

#默认返回None
def getRedis(key):
    res = r.get(key)#控制返回None
    if res is None:
        print('key: ' + key + "This key cannot be find...")
    else:
        print('key: ' + key + "  value: " + res)
    print(res is None)
    return res;