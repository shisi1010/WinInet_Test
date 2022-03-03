
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


string UTF8ToGB(const char* str)
{
	string result;
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


string JsonToString(const Json::Value & root)
{
	static Json::Value def = []() {
		Json::Value def;
		Json::StreamWriterBuilder::setDefaults(&def);
		def["emitUTF8"] = true;
		return def;
	}();

	ostringstream stream;
	Json::StreamWriterBuilder stream_builder;
	stream_builder.settings_ = def;//Config emitUTF8
	unique_ptr<Json::StreamWriter> writer(stream_builder.newStreamWriter());
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
	hClient.OpenRequest(lUrl); // ����url
	hClient.SendRequest(NULL, 0); // ��������ͷ
	hClient.SendRequestEx(0); // ���������ͷ
	DWORD dwSendLen = 0;
	hClient.SendReqBodyData(NULL, 0, dwSendLen); // ������
	hClient.EndSendRequest(); // �������

	DWORD resCode = hClient.GetRespCode(); // HTTP��Ӧ�� 200����ɹ� 404 not find 500 ����������
	string resHeader = hClient.GetRespHeader(); // ����ͷ

	DWORD dwResLen = 0;
	DWORD dwLen = 1024;
	CHAR* resData = new CHAR[dwLen];
	
	BOOL resBool = hClient.GetRespBodyData(resData, dwLen, dwResLen); // ��ȡHTTP��Ӧ��Ϣ�����ݣ��ڱ�����ΪJSON����

	hClient.CloseRequest(); // �ر�HTTP������

	// ����json����
	
	string resStrData = string(resData);
	CString strTemp;
	string value;


	Json::CharReaderBuilder builder;
	const unique_ptr<Json::CharReader> reader(builder.newCharReader());
	JSONCPP_STRING errs; // JsonCpp��0.11.z�汾�Ķ�ȡ��ʽ���ɰ汾API���ٱ�֧��
	Json::Value root;

	if (reader->parse(resStrData.c_str(), resStrData.c_str() + resStrData.length(), &root, &errs))  // reader��Json�ַ���������root��root������Json��������Ԫ��  
	{
		int errNum = root["code"].asInt(); // ��code���ݽ���Ϊint��ʽ�� Լ��20000Ϊ��ȷ����ֵ
		CString resCStr;
		Json::Value list = root["data"]["items"];  // data ������ items ����

		// ��root���������Ĵ�ӡ��
		string ss = JsonToString(root);
		ss = UTF8ToGB(ss.c_str());
		resCStr = ss.c_str();
		GetDlgItem(IDC_STATIC)->SetWindowText(resCStr);
	}

	delete[] resData; // ��ֹ�ڴ�й¶
}


string UnicodeToUTF8(const wstring& str)
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
	string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}


void CWinInet_TestDlg::OnBnClickedButtonPost()
{
	// HTTP POST����ʵ��
	CString CStrUrl;
	GetDlgItem(IDC_EDIT_URL)->GetWindowText(CStrUrl);
	LPCTSTR lUrl;
	if (CStrUrl == "")
	{
		lUrl = "http://127.0.0.1:8000/user/list"; // ע��API������ͬ
	}
	else
	{
		lUrl = (LPCTSTR)CStrUrl;
	}

	// ��װpost����JSON��ע�⽫�ַ�ת��ΪUTF-8����
	Json::Value sendData;
	sendData["user_name"] = UnicodeToUTF8(L"editor");
	sendData["password"] = UnicodeToUTF8(L"123456");
	sendData["user_type"] = UnicodeToUTF8(L"���ֲ���");
	sendData["user_shift"] = UnicodeToUTF8(L"�װ�");
	sendData["user_team"] = UnicodeToUTF8(L"�װ�");
	sendData["available"] = UnicodeToUTF8(L"True");

	// JsonCpp �����ã��鿴Json��װЧ��
	/*string ss = sendData.toStyledString();
	ss = (ss.c_str());
	CString resCStr = ss.c_str();
	GetDlgItem(IDC_STATIC)->SetWindowText(resCStr);*/

	// ��ʽ��Json�ַ���
	Json::StreamWriterBuilder builder;
	builder["emitUTF8"] = true;
	builder["indentation"] = "";  // assume default for comments is None
	string reqStr = Json::writeString(builder, sendData);

	// ��ʼ���� HTTP �ͻ���
	CHttpClient hClient;

	hClient.SetConnectTimeOut(5000); // �������ӳ�ʱ(��λ������)
	hClient.OpenRequest(lUrl, REQ_METHOD_POST); // ��������ʽΪPOST
	hClient.SendRequest((reqStr.c_str()), reqStr.length()); // ������ͷ�м���������ַ������ַ�������
	DWORD dwSendLen = 0;
	hClient.SendReqBodyData(NULL, 0, dwSendLen);

	DWORD resCode = hClient.GetRespCode(); // HTTP��Ӧ�� 200����ɹ� 404 not find 500 ����������
	string resHeader = hClient.GetRespHeader(); // ����ͷ

	DWORD dwResLen = 0;
	DWORD dwLen = 1024;
	CHAR* resData = new CHAR[dwLen];

	BOOL resBool = hClient.GetRespBodyData(resData, dwLen, dwResLen); // ��ȡHTTP��Ӧ��Ϣ�����ݣ��ڱ�����ΪJSON���ݣ�����Լ��Ϊ�޸ĵ�����

	hClient.CloseRequest(); // �ر�HTTP������

	// JsonCpp��0.11.z�汾�Ķ�ȡ��ʽ���ɰ汾API���ٱ�֧��

	string resStrData = string(resData);
	Json::CharReaderBuilder geter;
	const unique_ptr<Json::CharReader> reader(geter.newCharReader());
	JSONCPP_STRING errs; 
	Json::Value root;

	if (reader->parse(resStrData.c_str(), resStrData.c_str() + resStrData.length(), &root, &errs))  // reader��Json�ַ���������root��root������Json��������Ԫ��  
	{
		CString csResData;
		int errNum = root["code"].asInt(); // ��code���ݽ���Ϊint��ʽ�� Լ��20000Ϊ��ȷ����ֵ
		
		// ��code ��Ϊ20000ʱ�� ����ͨ��message ��ȡ������Ϣ
		
		csResData.Format("%d", errNum);
		string ss = JsonToString(root);
		ss = UTF8ToGB(ss.c_str());
		csResData = csResData+": "+ ss.c_str();
		GetDlgItem(IDC_STATIC)->SetWindowText(csResData);
	}

	delete[] resData; // ��ֹ�ڴ�й¶
}
