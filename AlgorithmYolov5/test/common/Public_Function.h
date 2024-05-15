/*
 ====================================================================
 Name         : predict_comm_func.h
 Author       : lll
 Version      : 0.1
 Copyright    :
 Description  : 公用接口文件
 ====================================================================
 */
#ifndef COMM_FUNC_H
#define COMM_FUNC_H


#ifndef _STRING_
#include <string>
#endif

#ifndef X_TYPEH
#include "x_type.h"
#endif 

using namespace std;

#define COMM_BASE64_FLAG_NONE	0
#define COMM_BASE64_FLAG_NOPAD	1
#define COMM_BASE64_FLAG_NOCRLF  2

/**
	@ingroup  公共函数模块
	@brief 给路径添加斜杠 如window下 "c:\test"添加后为"c:\test\",linux下为"/root/x1"添加后为"/root/x1/"
	@param path 路径
	@param path_size 路径大小
	@retval TRUE 成功
	@retval FALSE 失败
*/
BOOL
add_path_slash(char* path, int32 path_size);

/**
	@ingroup  公共函数模块
	@brief 合并路径,保证合并后的路径只会有一个路径符或者没有(取决于附加串最后有无路径符),如果附加路径为NULL,则最后为路径符
	@param path 路径
	@param size 路径大小
	@param append 附加路径
	@retval TRUE 成功
	@retval FALSE 失败
*/
BOOL
path_append(char* path, char* append);

/**
	@ingroup  公共函数模块
	@brief 从字符串中查找“/”或者“\”后的文件名称，并截取
	@param srcfilename 待截取文件名称
	@param size 路径大小
	@retval TRUE 成功
	@retval FALSE 失败
*/
BOOL
path_find_filename(char* path, OUT char* filename, int32 size);


/**
	@ingroup  公共函数模块
	@brief 从字符串中去掉租后“\”或者“/”后的路劲
	@param srcfilename 待截取文件名称
	@param size 路径大小
	@retval TRUE 成功
	@retval FALSE 失败
*/
BOOL
path_remove_file_spec(char* path);

/**
	@ingroup  公共函数模块
	@brief 判断一个路径为目录或者文件夹
	@param path 待判断路径
	@retval TRUE 成功
	@retval FALSE 失败
*/
BOOL
path_is_directory(char* path);

/**
	@ingroup  公共函数模块
	@brief 创建目录(多级目录)
	@param dir 目标路径
	@retval TRUE 成功
	@retval FALSE 失败
*/
BOOL
make_dir(char* dir);

/**
	@ingroup  公共函数模块
	@brief 删除目录或文件(同步)
	@param dir 目标路径
	@retval TRUE 成功
	@retval FALSE 失败
*/
BOOL
delete_dir(char* dir);

/************************************************************************
Function：     GetExName
Description：  获得应用程序名称
Input：
Output：
Return：       应用程序名称
Others：
************************************************************************/
std::string GetExName();

/************************************************************************
Function：    ExtractFilePath
Description： 获取全路径
Input：
Output：
Return：
Others：
************************************************************************/
std::string ExtractFilePath(std::string _strFullName);

/************************************************************************
Function：    ExecuteDirAppend
Description： 获取相对程序目录子目录全路径
Input：
Output：
Return：
Others：
************************************************************************/
std::string ExecuteDirAppend(std::string _strSubPath);


BOOL
base64_encode(const char* src, int32 src_len, char* dst, OUT int32* dst_len);

BOOL
base64_decoder(char* src, int32 src_len, char* dst, OUT int32* dst_len);

#endif
