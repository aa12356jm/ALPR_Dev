#include "easypr/util/kv.h"
#include "easypr/util/util.h"

namespace easypr {

Kv::Kv() { }

//加载文件，一行一行读取内容，将每行内容以键值对形式保存到data_变量中
void Kv::load(const std::string &file) 
{
  this->clear();
  std::ifstream reader(file);
  assert(reader);

  if (reader.is_open()) {
	  while (!reader.eof())
	  {
		  std::string line;
		  std::getline(reader, line);
		  if (line.empty()) continue;

		  //parse函数，解析每行内容
		  const auto parse = [](const std::string &str)
		  {
			  std::string tmp, key, value;
			  for (size_t i = 0, len = str.length(); i < len; ++i)
			  {
				  const char ch = str[i];
				  if (ch == ' ')
				  {
					  if (i > 0 && str[i - 1] != ' ' && key.empty())
					  {
						  key = tmp;
						  tmp.clear();
					  }
				  }
				  else
				  {
					  tmp.push_back(ch);
				  }
				  if (i == len - 1)
				  {
					  value = tmp;
				  }
			  }
			  return std::make_pair(key, value);
		  };

		  auto kv = parse(line);//kv中保存每行的键值对
		  this->add(kv.first, kv.second);//将键值对保存到data_中
	  }
  }
  reader.close();
}
//根据输入的键来查找对应的值
std::string Kv::get(const std::string &key) 
{
	//遍历vector之后没有发现这个键值
  if (data_.find(key) == data_.end()) 
  {
    std::cerr << "[Kv] cannot find " << key << std::endl;
    return "";
  }
  return data_.at(key);
}
//将键值对添加到data_中保存
void Kv::add(const std::string &key, const std::string &value) 
{
  if (data_.find(key) != data_.end()) 
  {
    fprintf(stderr,"[Kv] find duplicate: %s = %s , ignore\n",key.c_str(),value.c_str());
  } 
  else 
  {
    std::string v(value);
#ifdef OS_WINDOWS
    v = utils::utf8_to_gbk(value.c_str());
#endif
    data_[key] = v;
  }
}
//将此key移除掉
void Kv::remove(const std::string &key) {
  if (data_.find(key) == data_.end()) {
    std::cerr << "[Kv] cannot find " << key << std::endl;
    return;
  }
  data_.erase(key);
}

//清空map内容
void Kv::clear() {
  data_.clear();
}

}
