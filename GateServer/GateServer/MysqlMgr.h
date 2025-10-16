#pragma once
#include "const.h"
#include "MysqlDao.h"
//这里是service层，dao层直接操作数据库
//用来管理MysqlDao层
class MysqlMgr : public Singleton<MysqlMgr>
{
    friend class Singleton<MysqlMgr>;
public:
    ~MysqlMgr();
    int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
    bool CheckEmail(const std::string& name, const std::string& email);
    bool UpdatePwd(const std::string& name, const std::string& newpwd);
    bool CheckPwd(const std::string& name, const std::string& pwd, UserInfo& userInfo);
private:
    MysqlMgr();
    MysqlDao  _dao;
};