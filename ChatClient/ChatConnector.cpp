// ChatConnector.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ChatConnector.h"
#include "afxdialogex.h"


// CChatConnector 대화 상자입니다.

IMPLEMENT_DYNAMIC(CChatConnector, CDialogEx)

CChatConnector::CChatConnector(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatConnector::IDD, pParent)
{

}

CChatConnector::CChatConnector(CClientSocket *pclient_socket, CClientSocket *pclient_board_socket, CClientSocket* pclient_cam_socket, TCHAR *id)
	: CDialogEx(CChatConnector::IDD, NULL)
{
	m_client_socket= pclient_socket;
	m_client_board_socket = pclient_board_socket;
	m_client_cam_socket = pclient_cam_socket;
	p_id = id;
}

CChatConnector::~CChatConnector()
{
}

void CChatConnector::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, edt_id);
	DDX_Control(pDX, IDC_IPADDRESS1, edt_ip);
	DDX_Control(pDX, IDC_EDIT2, edt_port);
}


BEGIN_MESSAGE_MAP(CChatConnector, CDialogEx)
	ON_BN_CLICKED(IDOK, &CChatConnector::OnBnClickedOk)
END_MESSAGE_MAP()


// CChatConnector 메시지 처리기입니다.


void CChatConnector::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TCHAR t_ip[20];
	TCHAR t_port[10];
	int port;
	edt_id.GetWindowText(p_id,20);
	edt_ip.GetWindowText(t_ip,20);
	edt_port.GetWindowText(t_port, 10);
#ifdef UNICODE
	port = _wtoi(t_port);
#else
	port = atoi(t_port);
#endif

	if(!m_client_socket->Create() || !m_client_board_socket->Create() || !m_client_cam_socket->Create())
	{
		AfxMessageBox(_T("소켓 생성을 실패했습니다"));
		return;
	}
	if(!m_client_socket->Connect(t_ip,port) || !m_client_board_socket->Connect(t_ip, port+1) || !m_client_cam_socket->Connect(t_ip, port+2))
	{
		AfxMessageBox(_T("서버와 연결을 실패했습니다"));
		m_client_socket->Close();
		return;
	}
	m_client_board_socket->Init();
	m_client_cam_socket->Init();
	CDialogEx::OnOK();
}
