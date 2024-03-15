

#头文件搜索路径
#include_directories()

#库文件搜索路径
#link_directories()

#设置需要编译原文件列表 这里定义了srclist
set(SRC_LIST ./muduo_server.cpp)


#把.指定路径下所有元文件名字放入变量名
#aux_source_directory(. SRC_LIST)


#设置可执行文件最终存储路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


#生成可执行文件server，由srclist变量定义元文件编译而来
#通过m_s.cpp 生成server
add_executable(server ${SRC_LIST})

#server这个目标程序需要链接哪些库
target_link_libraries(server muduo_net muduo_base pthread)

#生成库文件
#add_library()



create user myhost@'%' identified by '123456';
grant all on *.* to myhost@'%';