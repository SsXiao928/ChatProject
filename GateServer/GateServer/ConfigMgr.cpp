#include "ConfigMgr.h"
//��boost��ȡ�����ļ�����Ϊgrpc��֧��c++17��filesystem��
ConfigMgr::ConfigMgr() {
	boost::filesystem::path current_path = boost::filesystem::current_path();//��ȡ��ǰ·��
	boost::filesystem::path config_path = current_path / "config.ini"; //�����ļ�·��,'/'ƴ��·��
	std::cout << "Config file path: " << config_path << std::endl;

	boost::property_tree::ptree pt;//��������ʽ���ļ�
	boost::property_tree::read_ini(config_path.string(), pt);//��ȡ�����ļ�

	//section_pair��[GateServer]��[VarifyServer]��[Database]��
	for(const auto& section_pair : pt) {
		const ::std::string& section_name = section_pair.first;
		//section_tree��[GateServer]�������еļ�ֵ��
		const boost::property_tree::ptree& section_tree = section_pair.second; 
		std::map<std::string, std::string> section_config;
		for(const auto& key_value_pair : section_tree) {
			const std::string& key = key_value_pair.first;
			const std::string& value = key_value_pair.second.get_value<std::string>();//secondʵ����Ҳ��һ��ptree����ͨ��get_value<std::string>()תstring
			section_config[key] = value; //����ֵ�Դ���section_config
		}

		SectionInfo sectionInfo;
		sectionInfo._section_datas = section_config; //��section_config��ֵ��SectionInfo�ĳ�Ա����
		_config_map[section_name] = sectionInfo; //��SectionInfo����_config_map
	}

	// ������е�section��key-value��  
	for (const auto& section_entry : _config_map) {
		const std::string& section_name = section_entry.first;
		SectionInfo section_config = section_entry.second;
		std::cout << "[" << section_name << "]" << std::endl;
		for (const auto& key_value_pair : section_config._section_datas) {
			std::cout << key_value_pair.first << "=" << key_value_pair.second << std::endl;
		}
	}

}
//����������ʽ����
//gCfgMgr["GateServer"]["port"]