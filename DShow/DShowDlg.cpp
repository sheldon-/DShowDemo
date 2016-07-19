
// DShowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DShow.h"
#include "DShowDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDShowDlg �Ի���



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


// CDShowDlg ��Ϣ�������

BOOL CDShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  ��ʼ������

	//��ʼ���ɼ�filter
	_InitCapGraphBuider();
	_EnumDevices(m_pCBVideoDev);





	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

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
	IEnumMoniker*	pEnum = NULL;			//ö������ؽӿ�ָ��

	//����ϵͳ�豸ö����
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum, (void**)&pDevEnum);
	HRCHECK(hr)

	//����һ��ö����Ƶ�豸��ö����
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
