#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ClientSocket.h"


// CChatConnector 대화 상자입니다.

class CChatConnector : public CDialogEx
{
	DECLARE_DYNAMIC(CChatConnector)

public:
	CChatConnector(CClientSocket *pclient_socket, CClientSocket* pclient_board_socket, CClientSocket* pclient_cam_socket, TCHAR *id);
	CChatConnector(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CChatConnector();
	TCHAR *p_id;
	CClientSocket *m_client_socket;
	CClientSocket *m_client_board_socket;
	CClientSocket *m_client_cam_socket;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CEdit edt_id;
	CIPAddressCtrl edt_ip;
	CEdit edt_port;
};
