#include "stdafx.h"
#include "TestPublic.h"

#include "public/Socket.h"
#include "public/RegUtil.h"
//#include "public/IniUtil.h"
#include "public/PathUtil.h"
#include <iostream>
using namespace std;

#define TRACE_MAX_LEN 0x400   //最大TRACE长度为1024
//#define TRACE_MAX_LEN 0xa   //最大TRACE长度为1024
//#define RUNINDEBUGMODE
#define USES_RELEASE_TRACE
#define TRACE_LOG_FILE_PATH _T("Log.txt")


#include <TraceUtil.h>

int TraceUtil()
{
	TCHAR tszTem[32] = {_T("Test^^^^^^TCHAR")};


	RG::TRACE(_T("TCHAR……\r\n"));
	for (int i=1; i<=15;i++)
	{
		RG::TRACE(_T("%02d、%s\n"), i, tszTem);
	}

	return 1;
}
//////int IniUtil()
//////{
//////	QNA::CIniUtil clsIniUtil;
//////
//////
//////	return 1;
//////}
//////
//////int FileUtil()
//////{
//////	TCHAR tszPath[MAX_PATH] = {0};
//////	if (!QNA::GetExePath(tszPath))
//////	{
//////		return -1;
//////	}
//////
//////	_tcscat_s(tszPath, _T("\\123"));
//////	
//////	if (!QNA::CreateMultipleDirectory(tszPath))
//////	{
//////		return -2;
//////	}
//////
//////	_tcscpy_s(tszPath, _T("D:\\123"));
//////
//////	if (!QNA::CreateMultipleDirectory(tszPath))
//////	{
//////		return -2;
//////	}
//////	return 1;
//////}


#include "public/FileVersionInfo.h"


//VI_STR_COMMENTS - Comments
//VI_STR_COMPANYNAME - CompanyName
//VI_STR_FILEDESCRIPTION - FileDescription
//VI_STR_FILEVERSION - FileVersion
//VI_STR_INTERNALNAME - InternalName
//VI_STR_LEGALCOPYRIGHT - LegalCopyright
//VI_STR_LEGALTRADEMARKS - LegalTrademarks
//VI_STR_ORIGINALFILENAME - OriginalFilename
//VI_STR_PRIVATEBUILD - PrivateBuild
//VI_STR_PRODUCTNAME - ProductName
//VI_STR_PRODUCTVERSION - ProductVersion
//VI_STR_SPECIALBUILD - SpecialBuild
//VI_STR_OLESELFREGISTER - OLESelfRegister 
int TestFileVersionInfo()
{
	//CFileVersionInfo	cls_fvi;
	//cls_fvi.Open(_T("C:\\windows\\explorer.exe"));
	//cls_fvi.GetVersionInfo();
	//cls_fvi.GetLIDName()

	return 1;
}


//int TestPathUtil()
//{
//	CPath
//	CPathUtil
//}

#define MAX_URL_LENGTH    255
#define HTTP_OK _T("HTTP/1.1 200 OK")

const TCHAR g_szHttpHead[] = 
_T("HEAD / HTTP/1.1\r\n\
Host: %s\r\n\
User-Agent: msnbot/1.0 (+http://search.msn.com/msnbot.htm)\r\n\
Accept: */*\r\n\
Connection: close\r\n\
\r\n");


int CheckHTTPHEAD(char* pstrDomain, DWORD dwIPValue, UINT uiPort)
{
	int iRet = 0;
	int iLen = 0;
	int iNum = 0;
	TCHAR tszTem[16] = {0};
	TCHAR tszSend[1024] = {0};
	QNA::CSocket clsSocket;
	iRet = clsSocket.Connect(dwIPValue, 80);
	if (1 != iRet)
	{
		RG::TRACE(_T("连接出错值:%d(1 成功 -1 调用出错 -2 Socket 创建失败 -3 设置发送超时失败 -4 设置接收超时失败 -5 连接超时 -6 连接出错)\r\n"), iRet);
		return -1;
	}
	iLen = _stprintf(tszSend, g_szHttpHead, pstrDomain);
	iRet = clsSocket.Send(clsSocket.GetSocket(), (PBYTE)tszSend, iLen);
	RG::TRACE(tszSend);
	if (1 != iRet)
	{
		RG::TRACE(_T("发送出错值:%d(1成功， -1发送指针为空， -2 长度错误 -3 SOCKET_ERROR -4 WSA_IO_PENDING -5 iSent)\r\n"), iRet);
		return -2;
	}
	ZeroMemory(tszSend, sizeof(tszSend));
	iLen = clsSocket.Recv(clsSocket.GetSocket(), (PBYTE)tszSend, sizeof(tszSend));
	if (0 > iLen)	return -3;		//接收出错  
	RG::TRACE(tszSend);

	ZeroMemory(tszTem, sizeof(tszTem));
	iLen = _tcslen(HTTP_OK);
	_tcsncpy(tszTem, (TCHAR*)tszSend, iLen);
	if (!_tcsicmp(tszTem, HTTP_OK))  return 1;	
	RG::TRACE(_T("HTTP HEAD 可行\r\n"));
	return 0;
}

//发送HTTP HEAD请求 成功返回1
int CheckIsEffective(char* pstrDomain, char* pstrIP)
{
	int iRet = 0;
	int iNum = 0;
	//struct in_addr addr;
	WSADATA WSAData = {0};
	HOSTENT *host_entry = NULL;   //结构指针
	//QNA::CIpLookupSocket clsIpLookupSocket;

	WSAStartup(MAKEWORD(2, 2), &WSAData);

	/* 即要解析的域名或主机名 */
	host_entry = ::gethostbyname(pstrDomain);
	if (NULL == host_entry)	return -1;
	while(host_entry->h_addr_list[iNum])
	{		
		//addr.S_un.S_addr = *(u_long *)host_entry->h_addr_list[iNum];
		//RG::TRACE(_T("IP地址:%s\r"), inet_ntoa(addr));
		iRet = /*clsIpLookupSocket.*/CheckHTTPHEAD(pstrDomain, *(u_long *)host_entry->h_addr_list[iNum], 80);
		if (1 == iRet)
		{
			return 1;
		}
		iNum++;
	}	
	return 0;
}

int TestSocket(TCHAR* pstrDomain, TCHAR* pstrIP)
{
	int iRet = 0;
	int iLen = 0;
	int iNum = 0;
	TCHAR tszTem[16] = {0};
	TCHAR tszSend[1024] = {0};
	struct in_addr addr;
	HOSTENT *host_entry = NULL;   //结构指针
	char szDomian[128] = {0};
	QNA::CSocket clsSocket;
	/* 即要解析的域名或主机名 */

	wcstombs(szDomian, pstrDomain, _tcslen(pstrDomain)*2);
	host_entry = gethostbyname(szDomian);
	if (NULL == host_entry)
	{
		return -1;
	}
	while(host_entry->h_addr_list[iNum])
	{		
		addr.S_un.S_addr = *(u_long *)host_entry->h_addr_list[iNum];
		RG::TRACE(_T("IP地址:%s\r"), inet_ntoa(addr));
		iRet = clsSocket.Connect(*(u_long *)host_entry->h_addr_list[iNum], 80);
		if (1 != iRet)
		{
			RG::TRACE(_T("连接出错值:%d(1 成功 -1 调用出错 -2 Socket 创建失败 -3 设置发送超时失败 -4 设置接收超时失败 -5 连接超时 -6 连接出错)\r\n"), iRet);
			return -2;
		}
		iLen = _stprintf(tszSend, g_szHttpHead, pstrDomain);
		iRet = clsSocket.Send(clsSocket.GetSocket(), (PBYTE)tszSend, iLen);
		RG::TRACE(tszSend);
		if (1 != iRet)
		{
			RG::TRACE(_T("发送出错值:%d(1成功， -1发送指针为空， -2 长度错误 -3 SOCKET_ERROR -4 WSA_IO_PENDING -5 iSent)\r\n"), iRet);
			return -3;
		}
		ZeroMemory(tszSend, sizeof(tszSend));
		iLen = clsSocket.Recv(clsSocket.GetSocket(), (PBYTE)tszSend, sizeof(tszSend));
		iLen = clsSocket.Recv(clsSocket.GetSocket(), (PBYTE)tszSend, sizeof(tszSend));
		if (0 < iLen)
		{
			return -4;
		}

		RG::TRACE(tszSend);

		ZeroMemory(tszTem, sizeof(tszTem));
		iLen = _tcslen(HTTP_OK);
		_tcsncpy(tszTem, (TCHAR*)tszSend, iLen);
		if (!_tcsicmp(tszTem, HTTP_OK))
		{
			RG::TRACE(_T("HTTP HEAD 可行\r\n"));
			//if (!_tcsncmp(pstrIP, inet_ntoa(addr), _tcslen(pstrIP)))
			//{
			//	RG::TRACE("####################HTTP HEAD 检查成功#######################\r\n");
			//	break;
			//}
			return 1;
		}
		else
		{
			RG::TRACE(_T("HTTP HEAD 不可行\r\n"));
		}
		
		iNum++;
	}	
	return 0;
}



int TestRegUtil()
{
	TCHAR tszTem1[MAX_PATH] = {0};
	TCHAR tszTem2[MAX_PATH] = {_T("in, out ,retval")};
	RG::CRegUtil clsRegUtil;
	clsRegUtil.GetRegistryValue(_T("SOFTWARE\\RainSoft\\RSDataToDb"), _T("test"), REG_SZ, (PVOID)tszTem1);


	clsRegUtil.SetRegistryValue(_T("SOFTWARE\\RainSoft"), _T("RSDataToDb"), _T("test"), REG_SZ, (PBYTE)tszTem2, _tcslen(tszTem2)*2);

	DWORD dwInsertDatabaseFileSize = 0;
	clsRegUtil.GetRegistryValue(_T("SOFTWARE\\RainSoft\\BJCultureBackGround"), _T("InsertDatabaseFileSize"), REG_DWORD,  (PVOID)&dwInsertDatabaseFileSize);

	clsRegUtil.GetRegistryValue(_T("SOFTWARE\\RainSoft\\RSDataToDb"), _T("test"), REG_SZ,  (PVOID)tszTem1);

	TCHAR tszUserPath[MAX_PATH] = {0};

	clsRegUtil.GetRegistryValue(HKEY_CURRENT_USER, _T("SOFTWARE\\China Mobile\\Fetion"), _T("UserPath"), REG_SZ, (PVOID)tszUserPath);
	if (0 == _tcslen(tszUserPath))
	{
		clsRegUtil.GetRegistryValue(_T("SOFTWARE\\China Mobile\\Fetion"), _T("UserPath"), REG_SZ, (PVOID)tszUserPath);
	}


	return 1;
}

int TestPathUtil()
{
	if (!RG::CheckDirExist(_T("D:\\WinPath\\desktop\\2")))
	{
		::MessageBoxA(NULL, "目录不存在", "D:\\WinPath\\desktop\\2", MB_OK);
	}

	if (!RG::CheckDirExist(_T("D:")))
	{
		::MessageBoxA(NULL, "目录不存在", "D:", MB_OK);
	}

	if (!RG::CreateMultipleDirectory(_T("D:\\WinPath\\desktop\\2\\1\\1\\2\\3\\4")))
	{
		::MessageBoxA(NULL, "创建多级目录失败", "D:\\WinPath\\desktop\\2", MB_OK);
	}

	TCHAR tszExePath[MAX_PATH] = {0};
	if (!RG::GetExePath(tszExePath))
	{
		::MessageBox(NULL, tszExePath, _T("获取Exe路径失败"), MB_OK);
	}


	TCHAR tszTemp[MAX_PATH] = {0};
	//获取当前目录  //这里是获取当前进程文件的完整路径 
	GetModuleFileName(NULL, tszTemp, MAX_PATH);
	
	DWORD GetFileLen = RG::GetFileLen(tszTemp);

	//RG::CPathUtil clsPathUtil;
	//clsPathUtil.GetSelectDirDlg(this->m_hWnd, IDC_EDIT_ECODE_PATH, _T("选择路径2"));

	//RG::CPathUtil clsPathUtil;
	//clsPathUtil.GetSelectDirDlg(this->m_hWnd, IDC_EDIT_DCODE_PATH);

	

	return 1;
}

#include "DirectoryTraversalFile.h"

int TestDirectoryTraversalUtil()
{
	//由于时间关系未测试完成 只测试了部分  余下等以后再测
	CDirTraversal clsDirTraversal;
	clsDirTraversal.SetTraversalPath(_T("G:\\BackUp\\Fetion\\Fetion"));
	clsDirTraversal.StartTraversal();

	clsDirTraversal.StopTraversal();

	return 1;
}