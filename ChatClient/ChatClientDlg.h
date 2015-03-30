
// ChatClientDlg.h : ��� ����
//

#pragma once
#include "ClientSocket.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ImageView.h"

#define TIMER_FRAMERATE 1
UINT ThreadReceive(void* pParam);
// CChatClientDlg ��ȭ ����
class CChatClientDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CChatClientDlg(CClientSocket *pclient_socket, CClientSocket *pclient_board_socket, CClientSocket *pclient_cam_socket, TCHAR *id);
	CChatClientDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	CClientSocket *m_client_socket;
	CClientSocket *m_client_board_socket;
	CClientSocket *m_client_cam_socket;
	TCHAR *p_id;

	CSocketFile* m_p_board_File;
	CArchive* m_p_board_ArchiveOut;
	
	CSocketFile* m_p_cam_File;
	CArchive* m_p_cam_ArchiveOut;

	CCreateContext         m_context;
	CImageView	          *m_pView;
	COLORREF			   m_crCurColor;
	COLORREF getColor()
	{
		return m_crCurColor;
	}
	CvCapture* capture;
	IplImage *frame;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ChatClient_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CEdit edt_text;
	CRichEditCtrl edt_list;
	void ProcessRecieve();
	void ProcessClose();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedButtonColor();
	afx_msg void OnBnClickedButtonErase();
	afx_msg void OnBnClickedButtonImageload();
	void SendData(CData *pData);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_pic;
	void ProcessCam();
	IplImage* remotecam;
	CStatic m_remote;
	SOCKET recvsocket;
	IplImage* resizing (IplImage *source);
};
