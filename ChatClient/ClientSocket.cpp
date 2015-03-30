// ClientSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ClientSocket.h"
#include "ChatClientDlg.h"


// CClientSocket

CClientSocket::CClientSocket()
{
	m_pCChatClient = NULL;
	m_ImageView = NULL;
	m_pFile = NULL;
	m_pArchiveIn = NULL;
	m_chat = NULL;
}

CClientSocket::~CClientSocket()
{
}

void CClientSocket::setdlg(CChatClientDlg *pChatClientDlg, bool chat)
{
	m_pCChatClient = pChatClientDlg;
	m_chat = chat;
}
void CClientSocket::setview(CImageView *pImageView)
{
	m_ImageView = pImageView;
}

// CClientSocket 멤버 함수


void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(m_pCChatClient)
	{
		if(m_chat)
			m_pCChatClient->ProcessRecieve();
		else
		{
			m_pCChatClient->ProcessCam();
		}
	}
	else if(m_ImageView)
	{
		//DWORD dwBytes;
		//VERIFY(IOCtl(FIONREAD, &dwBytes));
		//if (0 == dwBytes) // if nothing to read, return
			//return;

		AsyncSelect(NULL);
		int command;
		m_pArchiveIn->Read(&command, sizeof(int));

		switch (command)
		{
			case NET_IMAGE :
				m_ImageView->ProcessImage();
				break;

			case NET_POINT:
				m_ImageView->ProcessPoint();
				break;

			case NET_ERASE:
				m_ImageView->ProcessErase();
				break;

			default :
				break;

		}
		AsyncSelect(FD_READ|FD_CLOSE);
	}

	CSocket::OnReceive(nErrorCode);
}


void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(m_pCChatClient)
		m_pCChatClient->ProcessClose();
	else if(m_ImageView)
		m_ImageView->ProcessClose();

	CSocket::OnClose(nErrorCode);
}
void CClientSocket::Init()
{
	m_pFile = new CSocketFile(this);
	m_pArchiveIn = new CArchive(m_pFile, CArchive::load);
}
void CClientSocket::ReceiveData(CData* pData)
{
	if (m_pArchiveIn != NULL)
	{
		pData->Serialize(*m_pArchiveIn);
	}
}