
// WinInet_TestDlg.cpp : 实现文件
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CWinInet_TestDlg 对话框



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


// CWinInet_TestDlg 消息处理程序

BOOL CWinInet_TestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWinInet_TestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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

	// 创建HTTP链接请求
	CHttpClient hClient;

	hClient.SetConnectTimeOut(5000); // 设置连接超时(单位：毫秒)
	hClient.OpenRequest(lUrl); // 设置url
	hClient.SendRequest(NULL, 0); // 设置请求头
	hClient.SendRequestEx(0); // 额外的请求头
	DWORD dwSendLen = 0;
	hClient.SendReqBodyData(NULL, 0, dwSendLen); // 请求体
	hClient.EndSendRequest(); // 请求结束

	DWORD resCode = hClient.GetRespCode(); // HTTP响应码 200代表成功 404 not find 500 服务器错误
	string resHeader = hClient.GetRespHeader(); // 返回头

	DWORD dwResLen = 0;
	DWORD dwLen = 1024;
	CHAR* resData = new CHAR[dwLen];
	
	BOOL resBool = hClient.GetRespBodyData(resData, dwLen, dwResLen); // 获取HTTP响应消息体数据，在本例中为JSON数据

	hClient.CloseRequest(); // 关闭HTTP请求函数

	// 解析json数据
	
	string resStrData = string(resData);
	CString strTemp;
	string value;


	Json::CharReaderBuilder builder;
	const unique_ptr<Json::CharReader> reader(builder.newCharReader());
	JSONCPP_STRING errs; // JsonCpp在0.11.z版本的读取方式，旧版本API不再被支持
	Json::Value root;

	if (reader->parse(resStrData.c_str(), resStrData.c_str() + resStrData.length(), &root, &errs))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
	{
		int errNum = root["code"].asInt(); // 将code数据解析为int格式， 约定20000为正确返回值
		CString resCStr;
		Json::Value list = root["data"]["items"];  // data 数据在 items 项中

		// 将root对象完整的打印出
		string ss = JsonToString(root);
		ss = UTF8ToGB(ss.c_str());
		resCStr = ss.c_str();
		GetDlgItem(IDC_STATIC)->SetWindowText(resCStr);
	}

	delete[] resData; // 防止内存泄露
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
	// HTTP POST请求实例
	CString CStrUrl;
	GetDlgItem(IDC_EDIT_URL)->GetWindowText(CStrUrl);
	LPCTSTR lUrl;
	if (CStrUrl == "")
	{
		lUrl = "http://127.0.0.1:8000/user/list"; // 注意API有所不同
	}
	else
	{
		lUrl = (LPCTSTR)CStrUrl;
	}

	// 组装post参数JSON，注意将字符转换为UTF-8编码
	Json::Value sendData;
	sendData["user_name"] = UnicodeToUTF8(L"editor");
	sendData["password"] = UnicodeToUTF8(L"123456");
	sendData["user_type"] = UnicodeToUTF8(L"出钢操作");
	sendData["user_shift"] = UnicodeToUTF8(L"白班");
	sendData["user_team"] = UnicodeToUTF8(L"甲班");
	sendData["available"] = UnicodeToUTF8(L"True");

	// JsonCpp 调试用，查看Json组装效果
	/*string ss = sendData.toStyledString();
	ss = (ss.c_str());
	CString resCStr = ss.c_str();
	GetDlgItem(IDC_STATIC)->SetWindowText(resCStr);*/

	// 格式化Json字符串
	Json::StreamWriterBuilder builder;
	builder["emitUTF8"] = true;
	builder["indentation"] = "";  // assume default for comments is None
	string reqStr = Json::writeString(builder, sendData);

	// 开始创建 HTTP 客户端
	CHttpClient hClient;

	hClient.SetConnectTimeOut(5000); // 设置连接超时(单位：毫秒)
	hClient.OpenRequest(lUrl, REQ_METHOD_POST); // 设置请求方式为POST
	hClient.SendRequest((reqStr.c_str()), reqStr.length()); // 在请求头中加入请求的字符串与字符串长度
	DWORD dwSendLen = 0;
	hClient.SendReqBodyData(NULL, 0, dwSendLen);

	DWORD resCode = hClient.GetRespCode(); // HTTP响应码 200代表成功 404 not find 500 服务器错误
	string resHeader = hClient.GetRespHeader(); // 返回头

	DWORD dwResLen = 0;
	DWORD dwLen = 1024;
	CHAR* resData = new CHAR[dwLen];

	BOOL resBool = hClient.GetRespBodyData(resData, dwLen, dwResLen); // 获取HTTP响应消息体数据，在本例中为JSON数据，这里约定为修改的数据

	hClient.CloseRequest(); // 关闭HTTP请求函数

	// JsonCpp在0.11.z版本的读取方式，旧版本API不再被支持

	string resStrData = string(resData);
	Json::CharReaderBuilder geter;
	const unique_ptr<Json::CharReader> reader(geter.newCharReader());
	JSONCPP_STRING errs; 
	Json::Value root;

	if (reader->parse(resStrData.c_str(), resStrData.c_str() + resStrData.length(), &root, &errs))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
	{
		CString csResData;
		int errNum = root["code"].asInt(); // 将code数据解析为int格式， 约定20000为正确返回值
		
		// 当code 不为20000时， 可以通过message 获取错误信息
		
		csResData.Format("%d", errNum);
		string ss = JsonToString(root);
		ss = UTF8ToGB(ss.c_str());
		csResData = csResData+": "+ ss.c_str();
		GetDlgItem(IDC_STATIC)->SetWindowText(csResData);
	}

	delete[] resData; // 防止内存泄露
}
