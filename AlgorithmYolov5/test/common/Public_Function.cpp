#pragma warning (disable:4200)
#pragma warning (disable:4996)
#pragma warning (disable:4101)
#pragma warning (disable:4244)
#pragma warning (disable:4018)
#include "pch.h"
#pragma comment(lib,"imagehlp.lib") 

#include "Public_Function.h"
#include <time.h>
#include <Shlwapi.h>
#include <atlenc.h>

#ifdef _WIN32
#include <imagehlp.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>
#endif
#include <afxinet.h>
#include <iostream>
#include <sstream>
#include <complex>

using namespace std;

#define MIN_STRTIME_LEN 20 
#define MAX_TIME_LEN_COMM 20


BOOL 
add_path_slash(char* path, int32 path_size)
{
	if (path == NULL) {
		return FALSE;
	}
	char tmp[MAX_PATH] = {0};
#ifdef _WIN32
	if (path[strlen(path)-1] != '\\') {
		if (strlen(path) + 1 >= path_size) {
			return FALSE;
		}
		
		int32 count = strlen(path);
		path[count] = '\\';
		path[count+1] = '\0';
		strcpy(tmp, path);
		memset(path, 0, path_size);
		strcpy(path, tmp);
	}
#else
	if (path[strlen(path)-1] != '/') {
		if (strlen(path) + 1 >= path_size) {
			log_msg_ex("strlen(path) + 1 >= path_size");
			return FALSE;
		}

		int count = strlen(path);
		path[count] = '/';
		path[count+1] = '\0';
	}
#endif

	return TRUE;
}

BOOL 
path_append(char* path, char* append)
{
	if (path == NULL) {
		return FALSE;
	}

	if (append == NULL) {
		append = "";
	}

	if (strlen(path) == 0) {
		strcpy(path, append);
		return TRUE;
	}

	char* tmp = append;
	while (*tmp == '\\' || *tmp == '/') {
		tmp++;
	}

	if (!::PathAppend(path, tmp)) {
		return FALSE;
	}

	return TRUE;
}

BOOL 
path_find_filename(char* path, OUT char* filename, int32 size)
{
	if (path == NULL || filename == NULL) {
		return FALSE;
	}	

	char* name = PathFindFileName(path);
	if (strlen(name) == 0 ||
		strlen(name) >= size) {
		return FALSE;
	}

	strcpy(filename, name);

	return TRUE;
}

BOOL 
base64_encode(const char *src, int32 src_len, char* dst, OUT int32 *dst_len)
{
	if (src == NULL || dst == NULL || dst_len == NULL) {
		return FALSE;
	}
	return Base64Encode((byte*)src, (int)src_len,  dst, (int*)dst_len);
}

BOOL base64_decoder(char* src, int32 src_len, char *dst, OUT int32 *dst_len)
{
	if (src == NULL || dst == NULL || dst_len == NULL) {
		return FALSE;
	}
	return Base64Decode(src, (int)src_len,  (byte*)dst, (int*)dst_len);
}

BOOL 
path_remove_file_spec(char* path)
{
	if (path == NULL) {
		return FALSE;
	}
	char tmp[MAX_PATH] = {0};	
	int32 len = strlen(path) + 1;
	if (!PathRemoveFileSpec(path)) {
		return FALSE;
	}

	strcpy_s(tmp, MAX_PATH, path);
	add_path_slash(tmp, MAX_PATH);
	strcpy_s(path, len, tmp);
	return TRUE;
}

BOOL 
path_is_directory(char* path)
{
	if (path == NULL) {
		return FALSE;
	}

	char* find = PathFindExtension(path);
	if (strlen(find)>0) {
		return FALSE;
	}

	return TRUE;
}

BOOL
make_dir(char* dir)
{
	if (dir == NULL) {
		return FALSE;
	}
	
	char path[MAX_PATH] = {0};
	strcpy_s(path, MAX_PATH, dir);
	add_path_slash(path, MAX_PATH);
	if (!MakeSureDirectoryPathExists(path)) {
		return FALSE;
	}

	return TRUE;
}

BOOL
delete_dir(char* dir) 
{	
	if (dir == NULL) {
		return FALSE;
	}

	if (!path_is_directory(dir)) { //文件直接删除
		remove(dir);
		return TRUE;
	}

	char finddir[MAX_PATH]  = {0};
	strcpy_s(finddir, MAX_PATH, dir);
	path_append(finddir, "*.*");
	intptr_t file;
	struct _finddata_t data;
	file = _findfirst(finddir, &data);
	if (file == -1) {
		return FALSE;
	}
	while (_findnext(file, &data) == 0) {
		if(data.attrib == _A_SUBDIR) {
			if (strcmp(data.name, ".") == 0 || strcmp(data.name, "..") ==0) {
				continue;
			}
			char subdir[MAX_PATH] = {0};
			strcpy_s(subdir, MAX_PATH,dir);
			path_append(subdir, data.name);			
			delete_dir(subdir);
		} else {
			char filepath[MAX_PATH] = {0};
			strcpy_s(filepath, MAX_PATH, dir);
			path_append(filepath, data.name);
			remove(filepath);
		}

	}
	_findclose(file);
	_rmdir(dir);
	return TRUE;
}

std::string GetExName()
{
	char strFilePath[2048];
	memset(strFilePath, 0, 2048);
	::GetModuleFileName(NULL, strFilePath, 2048);
	std::string strRet(strFilePath);
	return strRet;
}

std::string ExtractFilePath(std::string _strFullName)
{
	std::string strRet = "";
	std::string strFileName(_strFullName);

	std::string::size_type iIndex = strFileName.find_last_of("\\");
	if (iIndex != std::string::npos)
	{
		strRet = strFileName.substr(0, iIndex + 1);
	}
	return strRet;
}

std::string ExecuteDirAppend(std::string _strSubPath)
{
	string path = ExtractFilePath(GetExName());
	path.append(_strSubPath.c_str());
	return path;
}


