
// ChatClientDlg.h : 헤더 파일
//

#pragma once
#include "ClientSocket.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ImageView.h"

#define TIMER_FRAMERATE 1
UINT ThreadReceive(void* pParam);
// CChatClientDlg 대화 상자
class CChatClientDlg : public CDialogEx
{
// 생성입니다.
public:
	CChatClientDlg(CClientSocket *pclient_socket, CClientSocket *pclient_board_socket, CClientSocket *pclient_cam_socket, TCHAR *id);
	CChatClientDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
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

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ChatClient_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
