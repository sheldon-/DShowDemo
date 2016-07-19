
// DShowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DShow.h"
#include "DShowDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDShowDlg 对话框



CDShowDlg::CDShowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDShowDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBVIDEODEV, m_pCBVideoDev);
}

BEGIN_MESSAGE_MAP(CDShowDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_CBVIDEODEV, &CDShowDlg::OnCbnSelchangeCbvideodev)
END_MESSAGE_MAP()


// CDShowDlg 消息处理程序

BOOL CDShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  初始化操作

	//初始化采集filter
	_InitCapGraphBuider();
	_EnumDevices(m_pCBVideoDev);





	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDShowDlg::OnCancel()
{

	m_pGraphBuider->Release();
	m_pCapGraph->Release();
	CoUninitialize();

}
void CDShowDlg::OnPaint()
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
HCURSOR CDShowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CDShowDlg::_InitCapGraphBuider()
{
	HRESULT hr = CoInitialize(NULL);
	HRCHECK(hr)

	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL,
		CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&m_pCapGraph);
	HRCHECK(hr)

	hr = CoCreateInstance(CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder,
		(void**)&m_pGraphBuider);
	HRCHECK(hr)

	m_pCapGraph->SetFiltergraph(m_pGraphBuider);
	
	return TRUE;
}

BOOL CDShowDlg::_EnumDevices(CComboBox& pCombox)
{
	ICreateDevEnum* pDevEnum = NULL;			//
	IEnumMoniker*	pEnum = NULL;			//枚举器监控接口指针

	//创建系统设备枚举器
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum, (void**)&pDevEnum);
	HRCHECK(hr)

	//创建一个枚举视频设备的枚举器
	hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnum, NULL);
	HRCHECK(hr)

	while (pEnum->Next(1, &m_pMoniker, NULL) == S_OK)
	{
		IPropertyBag* pPropery = NULL;
		hr = m_pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&pPropery);
		if (FAILED(hr))
		{
			m_pMoniker->Release();
			continue;
		}

		VARIANT varName;
		VariantInit(&varName);
		hr = pPropery->Read(L"Description", &varName, NULL);
		if (FAILED(hr))
		{
			hr = pPropery->Read(L"FriendlyName", &varName, NULL);
		}

		if (SUCCEEDED(hr))
		{
			pCombox.AddString(varName.bstrVal);
			VariantClear(&varName);
		}
		
		pPropery->Release();
		m_pMoniker->Release();

	}
	pCombox.SetCurSel(0);

	IBaseFilter* pBaseFilter;
	hr = m_pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pBaseFilter);
	if (SUCCEEDED(hr))
	{
		hr = m_pGraphBuider->AddFilter(pBaseFilter, L"Capture Filter");
	}



	pEnum->Release();
	pDevEnum->Release();


	return TRUE;

}

void CDShowDlg::OnCbnSelchangeCbvideodev()
{
	IBaseFilter* pBaseFilter = NULL;
	
}
