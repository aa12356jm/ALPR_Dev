#include "easypr/util/kv.h"
#include "easypr/util/util.h"

namespace easypr {

Kv::Kv() { }

//$)A<STXND<~#,R;PPR;PP6AH!DZH]#,=+C?PPDZH]RT<|V56TPNJ=1#4f5=data_1dA?VP
void Kv::load(const std::string &file) 
{
  this->clear();
  std::ifstream reader(file);
  assert(reader);

  if (reader.is_open()) {
    while (!reader.eof()) {
      std::string line;
      std::getline(reader, line);
      if (line.empty()) continue;

	//parse$)A:/J}#,=bNvC?PPDZH]
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

    auto kv = parse(line);//kv$)AVP1#4fC?PP5D<|V56T
    this->add(kv.first, kv.second);//$)A=+<|V56T1#4f5=data_VP
  }
  reader.close();
}
//$)A8y>]JdHk5D<|@42iUR6TS&5DV5
std::string Kv::get(const std::string &key) 
{
	//$)A1i@zvectorV.:sC;SP7"OVUb8v<|V5
  if (data_.find(key) == data_.end()) 
  {
    std::cerr << "[Kv] cannot find " << key << std::endl;
    return "";
  }
  return data_.at(key);
}
//$)A=+<|V56TLm<S5=data_VP1#4f
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
//$)A=+4KkeyRF3}5t
void Kv::remove(const std::string &key) {
  if (data_.find(key) == data_.end()) {
    std::cerr << "[Kv] cannot find " << key << std::endl;
    return;
  }
  data_.erase(key);
}

//$)AGe?UmapDZH]
void Kv::clear() {
  data_.clear();
}

}
