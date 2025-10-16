#include "ConfigMgr.h"
//用boost读取配置文件，因为grpc不支持c++17的filesystem库
ConfigMgr::ConfigMgr() {
	boost::filesystem::path current_path = boost::filesystem::current_path();//获取当前路径
	boost::filesystem::path config_path = current_path / "config.ini"; //配置文件路径,'/'拼接路径
	std::cout << "Config file path: " << config_path << std::endl;

	boost::property_tree::ptree pt;//按数的形式读文件
	boost::property_tree::read_ini(config_path.string(), pt);//读取配置文件

	//section_pair是[GateServer]、[VarifyServer]和[Database]等
	for(const auto& section_pair : pt) {
		const ::std::string& section_name = section_pair.first;
		//section_tree是[GateServer]下面所有的键值对
		const boost::property_tree::ptree& section_tree = section_pair.second; 
		std::map<std::string, std::string> section_config;
		for(const auto& key_value_pair : section_tree) {
			const std::string& key = key_value_pair.first;
			const std::string& value = key_value_pair.second.get_value<std::string>();//second实际上也是一个ptree对象，通过get_value<std::string>()转string
			section_config[key] = value; //将键值对存入section_config
		}

		SectionInfo sectionInfo;
		sectionInfo._section_datas = section_config; //将section_config赋值给SectionInfo的成员变量
		_config_map[section_name] = sectionInfo; //将SectionInfo存入_config_map
	}

	// 输出所有的section和key-value对  
	for (const auto& section_entry : _config_map) {
		const std::string& section_name = section_entry.first;
		SectionInfo section_config = section_entry.second;
		std::cout << "[" << section_name << "]" << std::endl;
		for (const auto& key_value_pair : section_config._section_datas) {
			std::cout << key_value_pair.first << "=" << key_value_pair.second << std::endl;
		}
	}

}
//以这样的形式访问
//gCfgMgr["GateServer"]["port"]