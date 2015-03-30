#pragma once

// CClientSocket 명령 대상입니다.
class CChatClientDlg;
class CImageView;
#include "Data.h"

class CClientSocket : public CSocket
{
public:
	CChatClientDlg *m_pCChatClient;
	CSocketFile* m_pFile;
    CArchive* m_pArchiveIn;
	CImageView		*m_ImageView;
	bool m_chat;
	CClientSocket();
	virtual ~CClientSocket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	void setdlg(CChatClientDlg *pChatClientDlg, bool chat);
	void Init();
	void ReceiveData(CData* pData);
	void setview(CImageView *pImageView);
};


