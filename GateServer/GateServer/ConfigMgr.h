#pragma once
//管理配置文件config.ini
#include "const.h"
struct SectionInfo {
	SectionInfo(){}
	~SectionInfo() { _section_datas.clear(); }

	//拷贝构造
	SectionInfo(const SectionInfo& src) {
		_section_datas = src._section_datas;
	}
	//拷贝赋值
	SectionInfo& operator = (const SectionInfo& src) {
		if (&src == this) {
			return *this;
		}
		this->_section_datas = src._section_datas;
		return *this;
	}
	std::map<std::string, std::string> _section_datas;
	//重载[]运算符
	std::string operator[](const std::string& key) {
		if (_section_datas.find(key) == _section_datas.end()) {
			return "";//没找到
		}

		return _section_datas[key];
	}
};
//单例
class ConfigMgr
{
public:
	~ConfigMgr() {
		_config_map.clear();
	}
	SectionInfo operator[](const std::string& section) {
		if (_config_map.find(section) == _config_map.end()) {
			return SectionInfo();
		}
		return _config_map[section];
	}

	static ConfigMgr& Inst() {
		//局部静态变量生命周期和进程同步，可见范围在局部
		//静态变量在第一次调用时初始化，之后的调用都返回同一个实例
		//C++11后局部静态变量是线程安全的
		static ConfigMgr cfg_mgr;
		return cfg_mgr;
	}

	ConfigMgr(const ConfigMgr& src) {
		_config_map = src._config_map;
	}

	ConfigMgr& operator = (const ConfigMgr& src) {
		if(&src == this) {
			return *this;
		}

		_config_map = src._config_map;
		return *this;
	}
private:
	ConfigMgr();
	//存储配置文件的所有section和对应的键值对
	std::map<std::string, SectionInfo> _config_map;
};

