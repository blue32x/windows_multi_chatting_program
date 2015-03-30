// ImageView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ImageView.h"
#include "ChatClientDlg.h"



// CImageView

IMPLEMENT_DYNCREATE(CImageView, CView)

CImageView::CImageView()
{
	image = NULL;
	printImage = FALSE;

}

CImageView::~CImageView()
{
}

BEGIN_MESSAGE_MAP(CImageView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CImageView 그리기입니다.

void CImageView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CImageView 진단입니다.

#ifdef _DEBUG
void CImageView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CImageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CImageView 메시지 처리기입니다.
void CImageView::SetView(CChatClientDlg *pCChatClient)
{
	m_CChatClient = pCChatClient;
}

void CImageView::ProcessErase()
{
	image=NULL;
	m_dataList.RemoveAll();
	Invalidate(TRUE);
}

void CImageView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCapture();
	CData *pData = new CData(point, m_CChatClient->getColor(), FALSE);
	m_dataList.AddTail(pData);
	printImage = FALSE;
	m_CChatClient->SendData(pData);
	Invalidate(FALSE);
	CView::OnLButtonDown(nFlags, point);
}


void CImageView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}

void CImageView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(!GetCapture()) return;
	//m_Data.Add(point);
	CData *pData = new CData(point, m_CChatClient->getColor(), TRUE);
	m_dataList.AddTail(pData);
	printImage = FALSE;
	m_CChatClient->SendData(pData);
	Invalidate(FALSE);

	CView::OnMouseMove(nFlags, point);
}


void CImageView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CView::OnPaint()을(를) 호출하지 마십시오.

	if(image != NULL && printImage == TRUE)
	{
		Graphics g(this->GetDC()->m_hDC);
		CRect m_rect;	

		GetWindowRect(m_rect);
		ScreenToClient(m_rect);

		g.DrawImage(image, m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height());
		g.~Graphics();
	}


	for (POSITION pos = m_dataList.GetHeadPosition(); pos != NULL; )
    {
		CData *pData = (CData *)(m_dataList.GetNext(pos));
		CPen pen, *pOldPen;
		pen.CreatePen(PS_SOLID, 2, pData->m_crColor);
		pOldPen = dc.SelectObject(&pen);
		if(!pData->m_line)
		{				
			dc.MoveTo(pData->m_ptPos);
			dc.LineTo(pData->m_ptPos);
		}
		else if(pData->m_line && pos != NULL)
		{
			CPoint temp = pData->m_ptPos;
			POSITION temppos = pos;
			pData = (CData *)(m_dataList.GetNext(pos));
			if(pData->m_line)
			{
				dc.MoveTo(temp);
				dc.LineTo(pData->m_ptPos);
			}
			pos = temppos;
		}
		dc.SelectObject(pOldPen);
    }
}


void CImageView::PicView()
{
	CFileDialog fileDialog(TRUE, NULL, NULL, 0, "그림 파일|*.jpg;*.jpeg;*.bmp;*.png;*.gif|Bitmap (*.bmp)|*.bmp|Jpeg Image (*.jpg, *.jpeg)|*.jpg;*.jpeg|All Files(*.*)|*.*||");
	if (fileDialog.DoModal()==IDOK)
	{
		CFileStatus fs;
		CString filepath=fileDialog.GetPathName();
		if (!CFile::GetStatus(filepath, fs))
			return;
		WCHAR wtext[200]={0};
		MultiByteToWideChar(CP_ACP,0, filepath,-1,wtext, 200);

		CFile file(filepath, CFile::modeRead);
		// 파일 크기 얻는다.
		int BitmapSize = file.GetLength();

		// 파일 보낼 버퍼 만든다
		byte *b_image = new byte[BitmapSize];

		// 파일 읽기
		file.Read(b_image, BitmapSize);
		file.Close();

		image = Image::FromFile(wtext);
		printImage = TRUE;

		int code = NET_IMAGE;
		m_CChatClient->m_client_board_socket->Send(&code, sizeof(int));
		m_CChatClient->m_client_board_socket->Send(&BitmapSize, sizeof(int));
		m_CChatClient->m_client_board_socket->Send(b_image, BitmapSize);
		Invalidate(FALSE);
	}
}

void CImageView::ProcessImage()
{
	int readSize;
	int fileSize;
	byte* buffer = new byte[4096];
	m_CChatClient->m_client_board_socket->Receive(&fileSize, sizeof(int));
	// 파일 하나 만들어
	CByteArray a;

	// 줄창 저장
	while(fileSize > 0)
	{
		readSize = m_CChatClient->m_client_board_socket->Receive(buffer, 4096);
		
		if (fileSize < readSize)
		{
			for(int i = 0; i<fileSize; i++)
				a.Add(buffer[i]);
		}
		else
		{
			for(int i = 0; i<readSize; i++)
				a.Add(buffer[i]);
		}
		
		fileSize -= readSize;
	}

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, a.GetSize());
	void* pData = GlobalLock(hGlobal);
	memcpy(pData, a.GetData(), a.GetSize());
	GlobalUnlock(hGlobal);

	IStream* pStream = NULL;

	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
	{
		image = Image::FromStream(pStream);
	}

	delete buffer;
	printImage = TRUE;
	Invalidate(FALSE);
}

void CImageView::ProcessPoint()
{
	do
	{
		CData* pData = new CData;
		m_CChatClient->m_client_board_socket->ReceiveData(pData);
		m_dataList.AddTail(pData);
	}while(!m_CChatClient->m_client_board_socket->m_pArchiveIn->IsBufferEmpty());
	printImage = FALSE;
	Invalidate(FALSE);
}

void CImageView::ProcessClose()
{
	m_CChatClient->m_client_board_socket->Close();
	delete m_CChatClient->m_client_board_socket;
}
