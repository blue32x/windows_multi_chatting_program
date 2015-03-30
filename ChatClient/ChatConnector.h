#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ClientSocket.h"


// CChatConnector ��ȭ �����Դϴ�.

class CChatConnector : public CDialogEx
{
	DECLARE_DYNAMIC(CChatConnector)

public:
	CChatConnector(CClientSocket *pclient_socket, CClientSocket* pclient_board_socket, CClientSocket* pclient_cam_socket, TCHAR *id);
	CChatConnector(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CChatConnector();
	TCHAR *p_id;
	CClientSocket *m_client_socket;
	CClientSocket *m_client_board_socket;
	CClientSocket *m_client_cam_socket;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CEdit edt_id;
	CIPAddressCtrl edt_ip;
	CEdit edt_port;
};
