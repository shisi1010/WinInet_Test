/**
* WinInet��װ�࣬������windows��http����, HttpClient.h
* zhangyl 2016.07.15 mail: balloonwj@qq.com
*/

#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include "stdafx.h"
#include <WinInet.h>
#include <string>
#include <vector>

#pragma comment(lib, "wininet.lib")


#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

// IE8 on Windows 7
#define     IE8_USER_AGENT  _T("Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1;Trident/4.0)")

// HTTP���󷽷�
enum HTTP_REQ_METHOD
{
	REQ_METHOD_GET,
	REQ_METHOD_POST,
};

// HTTP״̬�¼���ʶ
enum HTTP_STATUS_EVENT
{
	CONNECTED_EVENT,
	REQUEST_OPENED_EVENT,
	REQUEST_COMPLETE_EVENT,
	USER_CANCEL_EVENT
};

// WinInet Http�첽��װ��
class CHttpClient
{
public:
	CHttpClient(void);
	~CHttpClient(void);

public:
	BOOL OpenRequest(LPCTSTR lpszUrl, HTTP_REQ_METHOD nReqMethod = REQ_METHOD_GET); // ��HTTP������
	BOOL AddReqHeaders(LPCTSTR lpHeaders);                                          // ���һ������HTTP����ͷ����
	BOOL SendRequest(const CHAR* lpData, DWORD dwLen);                              // ����HTTP������
	BOOL SendRequestEx(DWORD dwLen);                                                // ��ʼ����HTTP������
	BOOL SendReqBodyData(const CHAR* lpBuf, DWORD dwLen, DWORD& dwSendLen);         // ����HTTP������Ϣ�����ݺ���
	BOOL EndSendRequest();                                                          // ��������HTTP������
	DWORD GetRespCode();                                                            // ��ȡHTTP��Ӧ�뺯��
	tstring GetRespHeader();                                                        // ��ȡȫ��HTTPͷ
	tstring GetRespHeader(LPCTSTR lpszName, int nIndex = 0);                        // ��ȡHTTPͷָ���ֶ����Ƶ�ֵ
	int GetRespHeaderByInt(LPCTSTR lpszName, int nIndex = 0);                       // ��ȡHTTPͷָ���ֶ����Ƶ�ֵ(Int)
	BOOL GetRespBodyData(CHAR * lpBuf, DWORD dwLen, DWORD& dwRecvLen);              // ��ȡHTTP��Ӧ��Ϣ�����ݺ���
	void CloseRequest();                                                            // �ر�HTTP������
	void SetConnectTimeOut(DWORD dwTimeOut);                                        // �������ӳ�ʱ(��λ������)
	void SetCancalEvent();                                                          // ����ȡ���¼�����
	void SetProxy(LPCTSTR lpszServer, WORD nPort, LPCTSTR lpszUser = NULL, LPCTSTR lpszPwd = NULL);// ����HTTP���������

private:
	// ״̬�ص�����
	static void __stdcall StatusCallback(HINTERNET hInternet, DWORD dwContext, DWORD dwInternetStatus, LPVOID lpStatusInfo, DWORD dwStatusInfoLen);
	// ����Url����(Э�飬���������˿ڣ��ļ�·��)
	BOOL ParseURL(LPCTSTR lpszUrl, LPTSTR lpszScheme, DWORD dwSchemeLength, LPTSTR lpszHostName, DWORD dwHostNameLength, WORD& nPort, LPTSTR lpszUrlPath, DWORD dwUrlPathLength);
	// �ȴ��¼�����
	BOOL WaitForEvent(HTTP_STATUS_EVENT nEvent, DWORD dwTimeOut);
	// ��ȡHTTP��Ӧͷ����
	DWORD __GetRespHeaderLen();
	// ��ȡHTTP��Ӧͷ�����б�����m_arrRespHeader����
	BOOL __GetRespHeader();

private:
	HINTERNET m_hInternet;                  // �Ự���
	HINTERNET m_hConnect;                   // ���Ӿ��
	HINTERNET m_hRequest;                   // ������

	HANDLE    m_hCompleteEvent;             // ��������¼����
	HANDLE    m_hCancelEvent;               // �ⲿȡ���¼����

	DWORD     m_dwConnectTimeOut;           // ���ӳ�ʱʱ��
	DWORD     m_dwContext;                  // ��ǰ����������

	std::vector<tstring> m_arrRespHeader;   // Http��Ӧͷ����

	tstring m_strProxy;                     // ���������(������:�˿�)
	tstring m_strUser, m_strPwd;            // ������������û���������
};

#endif //!__HTTP_CLIENT_H__