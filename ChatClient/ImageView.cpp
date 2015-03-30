// ImageView.cpp : ���� �����Դϴ�.
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


// CImageView �׸����Դϴ�.

void CImageView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// CImageView �����Դϴ�.

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


// CImageView �޽��� ó�����Դϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}

void CImageView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CView::OnPaint()��(��) ȣ������ ���ʽÿ�.

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
	CFileDialog fileDialog(TRUE, NULL, NULL, 0, "�׸� ����|*.jpg;*.jpeg;*.bmp;*.png;*.gif|Bitmap (*.bmp)|*.bmp|Jpeg Image (*.jpg, *.jpeg)|*.jpg;*.jpeg|All Files(*.*)|*.*||");
	if (fileDialog.DoModal()==IDOK)
	{
		CFileStatus fs;
		CString filepath=fileDialog.GetPathName();
		if (!CFile::GetStatus(filepath, fs))
			return;
		WCHAR wtext[200]={0};
		MultiByteToWideChar(CP_ACP,0, filepath,-1,wtext, 200);

		CFile file(filepath, CFile::modeRead);
		// ���� ũ�� ��´�.
		int BitmapSize = file.GetLength();

		// ���� ���� ���� �����
		byte *b_image = new byte[BitmapSize];

		// ���� �б�
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
	// ���� �ϳ� �����
	CByteArray a;

	// ��â ����
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
