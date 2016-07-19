
// DShowDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CDShowDlg 对话框
class CDShowDlg : public CDialogEx
{
// 构造
public:
	CDShowDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DSHOW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	BOOL _InitCapGraphBuider();
	BOOL _EnumDevices(CComboBox& pCombox);
private:
	IGraphBuilder* m_pGraphBuider;
	IMediaControl* m_pMediaCtl;
	IMediaEvent*	m_pEvent;
	ICaptureGraphBuilder2 *m_pCapGraph;
	IMoniker * m_pMoniker;

	CComboBox m_pCBVideoDev;			//视频设备列表框

	
public:
	afx_msg void OnCbnSelchangeCbvideodev();
};
