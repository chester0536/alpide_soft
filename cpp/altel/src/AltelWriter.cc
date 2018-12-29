#include "JadeWriter.hh"

//++++++++++++++++++++++++++++++++++
//AltelWriter.hh
class AltelWriter: public JadeWriter{
 public:
  AltelWriter(const JadeOption &opt);
  ~AltelWriter() override {}; 

  void Open() override;
  void Close() override;
  void Write(JadeDataFrameSP df) override;
 private:
  FILE* m_fd;
  std::string m_path;
  JadeOption m_opt;
  bool m_disable_file_write;
};

//+++++++++++++++++++++++++++++++++++++++++
//AltelWriter.cc
namespace{
  auto _test_index_ = JadeUtils::SetTypeIndex(std::type_index(typeid(AltelWriter)));
  auto _test_ = JadeFactory<JadeWriter>::Register<AltelWriter, const JadeOption&>(typeid(AltelWriter));
}

AltelWriter::AltelWriter(const JadeOption &opt)
  :m_opt(opt), m_fd(0), m_disable_file_write(false), JadeWriter(opt){
  m_disable_file_write = m_opt.GetBoolValue("DISABLE_FILE_WRITE");
}

void AltelWriter::Open(){
  if(m_path.empty())
    m_path = m_opt.GetStringValue("PATH"); 

  if(m_disable_file_write)
    return;
  std::time_t time_now = std::time(nullptr);
  char time_buff[13];
  time_buff[12] = 0;
  std::strftime(time_buff, sizeof(time_buff),
		"%y%m%d%H%M%S", std::localtime(&time_now));
  std::string time_str(time_buff);
  std::string data_path = m_path+"_"+time_str +".df";
  m_fd = std::fopen(data_path.c_str(), "wb" );
  if(m_fd == NULL){
    std::cerr<<"JadeWrite: Failed to open/create file: "<<data_path<<"\n";
    throw;
  }
}

void AltelWriter::Close(){
  if(m_disable_file_write)
    return;
  if(m_fd){
    std::fclose(m_fd);
    m_fd = 0;
  }
}

void AltelWriter::Write(JadeDataFrameSP df){
  if(m_disable_file_write)
    return;
  if(!df){
    std::cerr<<"JadeWrite: File is not opened/created before writing\n";
    throw;
  }
  //TODO
  std::string &rawstring = df->RawData();
  if(rawstring.size()){
    std::fwrite(&(rawstring.at(0)), 1, rawstring.size(), m_fd);
  }
}