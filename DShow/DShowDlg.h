
// DShowDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CDShowDlg �Ի���
class CDShowDlg : public CDialogEx
{
// ����
public:
	CDShowDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DSHOW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

	CComboBox m_pCBVideoDev;			//��Ƶ�豸�б��

	
public:
	afx_msg void OnCbnSelchangeCbvideodev();
};
