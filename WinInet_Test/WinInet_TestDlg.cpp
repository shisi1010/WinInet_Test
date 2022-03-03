
// WinInet_TestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WinInet_Test.h"
#include "WinInet_TestDlg.h"
#include "afxdialogex.h"
#include <iostream>

#include "HttpClient.h"
#include "json.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWinInet_TestDlg �Ի���



CWinInet_TestDlg::CWinInet_TestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWinInet_TestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinInet_TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWinInet_TestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_GET, &CWinInet_TestDlg::OnBnClickedButtonGet)
	ON_BN_CLICKED(IDC_BUTTON_POST, &CWinInet_TestDlg::OnBnClickedButtonPost)
END_MESSAGE_MAP()


// CWinInet_TestDlg ��Ϣ�������

BOOL CWinInet_TestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CWinInet_TestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWinInet_TestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CWinInet_TestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


std::string UTF8ToGB(const char* str)
{
	std::string result;
	WCHAR *strSrc;
	LPSTR szRes;

	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete[]strSrc;
	delete[]szRes;
	return result;
}


std::string JsonToString(const Json::Value & root)
{
	static Json::Value def = []() {
		Json::Value def;
		Json::StreamWriterBuilder::setDefaults(&def);
		def["emitUTF8"] = true;
		return def;
	}();

	std::ostringstream stream;
	Json::StreamWriterBuilder stream_builder;
	stream_builder.settings_ = def;//Config emitUTF8
	std::unique_ptr<Json::StreamWriter> writer(stream_builder.newStreamWriter());
	writer->write(root, &stream);
	return stream.str();
}


void CWinInet_TestDlg::OnBnClickedButtonGet()
{
	CString CStrUrl;
	GetDlgItem(IDC_EDIT_URL)->GetWindowText(CStrUrl);
	LPCTSTR lUrl;
	if (CStrUrl == "")
	{
		lUrl = "http://127.0.0.1:8000/user/list";
	}
	else
	{
		lUrl = (LPCTSTR)CStrUrl;
	}

	// ����HTTP��������
	CHttpClient hClient;

	hClient.SetConnectTimeOut(5000); // �������ӳ�ʱ(��λ������)
	hClient.OpenRequest(lUrl);
	hClient.SendRequest(NULL, 0);
	hClient.SendRequestEx(0);
	DWORD dwSendLen = 0;
	hClient.SendReqBodyData(NULL, 0, dwSendLen);
	hClient.EndSendRequest();

	DWORD resCode = hClient.GetRespCode();
	std::string resHeader = hClient.GetRespHeader();

	DWORD dwResLen = 0;
	DWORD dwLen = 1024;
	CHAR* resData = new CHAR[dwLen];
	
	BOOL resBool = hClient.GetRespBodyData(resData, dwLen, dwResLen); // ��ȡHTTP��Ӧ��Ϣ�����ݺ���
	
	// ����json����
	
	string resStrData = string(resData);
	CString strTemp;
	std::string value;

	Json::CharReaderBuilder builder;
	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

	JSONCPP_STRING errs;
	Json::Value root;

	if (reader->parse(resStrData.c_str(), resStrData.c_str() + resStrData.length(), &root, &errs))  // reader��Json�ַ���������root��root������Json��������Ԫ��  
	{
		int errNum = root["code"].asInt();
		CString resCStr;
		Json::Value list = root["data"]["items"];  

		//std::string ss = root.toStyledString();
		std::string ss = JsonToString(root);
		
		ss = UTF8ToGB(ss.c_str());
		resCStr = ss.c_str();

		GetDlgItem(IDC_STATIC)->SetWindowText(resCStr);
	}
	
	hClient.CloseRequest(); // �ر�HTTP������

}


std::string UnicodeToUTF8(const std::wstring& str)
{
	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_UTF8,
		0,
		str.c_str(),
		-1,
		NULL,
		0,
		NULL,
		NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char)* (iTextLen + 1));
	::WideCharToMultiByte(CP_UTF8,
		0,
		str.c_str(),
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL);
	std::string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}


void CWinInet_TestDlg::OnBnClickedButtonPost()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString CStrUrl;
	GetDlgItem(IDC_EDIT_URL)->GetWindowText(CStrUrl);
	LPCTSTR lUrl;
	if (CStrUrl == "")
	{
		lUrl = "http://127.0.0.1:8000/user/list";
	}
	else
	{
		lUrl = (LPCTSTR)CStrUrl;
	}

	Json::Value sendData;
	sendData["user_name"] = UnicodeToUTF8(L"editor");
	sendData["password"] = UnicodeToUTF8(L"123456");
	sendData["user_type"] = UnicodeToUTF8(L"���ֲ���");
	sendData["user_shift"] = UnicodeToUTF8(L"�װ�");
	sendData["user_team"] = UnicodeToUTF8(L"�װ�");
	sendData["available"] = UnicodeToUTF8(L"True");

	//std::string ss = sendData.toStyledString();
	//ss = (ss.c_str());
	//CString resCStr = ss.c_str();
	//GetDlgItem(IDC_STATIC)->SetWindowText(resCStr);

	std::string reqStr;

	//Json::StreamWriterBuilder jswBuilder;
	//jswBuilder["emitUTF8"] = true;
	//std::unique_ptr<Json::StreamWriter> jsWriter(jswBuilder.newStreamWriter());
	//std::ostringstream os;
	//jsWriter->write(sendData, &os);


	Json::StreamWriterBuilder builder;
	builder["emitUTF8"] = true;
	builder["indentation"] = "";  // assume default for comments is None
	std::string str = Json::writeString(builder, sendData);



	reqStr = str;
	GetDlgItem(IDC_STATIC)->SetWindowText(reqStr.c_str());
	CHttpClient hClient;

	hClient.SetConnectTimeOut(5000); // �������ӳ�ʱ(��λ������)
	hClient.OpenRequest(lUrl, REQ_METHOD_POST);
	hClient.SendRequest((reqStr.c_str()), reqStr.length());
	DWORD dwSendLen = 0;
	hClient.SendReqBodyData(NULL, 0, dwSendLen);
	hClient.EndSendRequest();

}
