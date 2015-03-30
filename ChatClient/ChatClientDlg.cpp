
// ChatClientDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ChatClientDlg.h"
#include "afxdialogex.h"
#include "CvvImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChatClientDlg ��ȭ ����




CChatClientDlg::CChatClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CChatClientDlg::CChatClientDlg(CClientSocket *pclient_socket, CClientSocket *pclient_board_socket, CClientSocket *pclient_cam_socket, TCHAR *id)
	: CDialogEx(CChatClientDlg::IDD, NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_client_socket = pclient_socket;
	m_client_board_socket = pclient_board_socket;
	m_p_board_File = new CSocketFile(pclient_board_socket);
	m_p_board_ArchiveOut = new CArchive(m_p_board_File, CArchive::store);

	m_client_cam_socket = pclient_cam_socket;
	m_p_cam_File = new CSocketFile(pclient_cam_socket);
	m_p_cam_ArchiveOut = new CArchive(m_p_cam_File, CArchive::store);

	//recvsocket = m_client_cam_socket->Detach();
	//AfxBeginThread(ThreadReceive, this);
	m_client_socket->setdlg(this,TRUE);
	m_client_cam_socket->setdlg(this,FALSE);
	p_id = id;
	frame = NULL;
	remotecam=NULL;
}

void CChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, edt_text);
	DDX_Control(pDX, IDC_RICHEDIT22, edt_list);
	DDX_Control(pDX, IDC_PICTURE, m_pic);
	DDX_Control(pDX, IDC_REMOTE, m_remote);
}

BEGIN_MESSAGE_MAP(CChatClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CChatClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, &CChatClientDlg::OnBnClickedButtonColor)
	ON_BN_CLICKED(IDC_BUTTON_ERASE, &CChatClientDlg::OnBnClickedButtonErase)
	ON_BN_CLICKED(IDC_BUTTON_IMAGELOAD, &CChatClientDlg::OnBnClickedButtonImageload)
	ON_BN_CLICKED(IDC_BUTTON1, &CChatClientDlg::OnBnClickedButton1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CChatClientDlg �޽��� ó����

BOOL CChatClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	RECT r = {300, 12, 1000, 535};
	m_context.m_pNewViewClass = RUNTIME_CLASS(CImageView);
	m_pView = (CImageView*)((CFrameWnd*)this)->CreateView(&m_context);
	ASSERT(m_pView);
	m_pView->SetView(this);
	m_pView->OnInitialUpdate();
	m_pView->ShowWindow(SW_NORMAL);
	m_pView->MoveWindow(&r);
	m_client_board_socket->setview(m_pView);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CChatClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if(frame!=NULL)
		{
			CDC*	pDC;
			CRect	rect;
			CvvImage	m_cImage;
			pDC = m_pic.GetDC();
			m_pic.GetClientRect(&rect);
		
			m_cImage.CopyOf(frame);
			m_cImage.DrawToHDC(pDC->m_hDC, rect);
		
			ReleaseDC(pDC);
		}
		if(remotecam!=NULL)
		{
			CDC*	pDC;
			CRect	rect;
			CvvImage	m_cImage;
			pDC = m_remote.GetDC();
			m_remote.GetClientRect(&rect);
		
			m_cImage.CopyOf(remotecam);
			m_cImage.DrawToHDC(pDC->m_hDC, rect);
		
			ReleaseDC(pDC);
		}
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CChatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatClientDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	TCHAR strBuf[100];
	TCHAR temp[100];

	edt_text.GetWindowText(strBuf, 100);

	int nlen;
	nlen = edt_list.GetWindowTextLength();
	edt_list.SetSel(nlen,nlen);
	sprintf(temp,"%s : %s\n",p_id, strBuf);
	m_client_socket->Send(temp,strlen(temp));

	edt_list.ReplaceSel(temp);
	edt_text.SetWindowText(_T(""));
}

void CChatClientDlg::ProcessRecieve()
{
	int nRead;
	int nlen;
	TCHAR rcvBuffer[1024];
	CString strBuffer =_T("");

	nRead = m_client_socket->Receive(rcvBuffer, 1024);
	strBuffer = rcvBuffer;

	nlen = edt_list.GetWindowTextLength();
	edt_list.SetSel(nlen,nlen);
	edt_list.ReplaceSel(strBuffer.Left(nRead));
}
void CChatClientDlg::ProcessClose()
{
	int nlen;
	nlen = edt_list.GetWindowTextLength();
	edt_list.SetSel(nlen,nlen);
	edt_list.ReplaceSel(_T("������ ����Ǿ����ϴ�.\n"));

	m_client_socket->Close();
	delete m_client_socket;

	nlen = edt_list.GetWindowTextLength();
	edt_list.SetSel(nlen,nlen);
	edt_list.ReplaceSel(_T("���� ����\n"));
}

BOOL CChatClientDlg::DestroyWindow()
{
	KillTimer( TIMER_FRAMERATE );
	
	if( capture != NULL )
		cvReleaseCapture( &capture );
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_client_socket->Close();
	delete m_client_socket;

	 m_client_board_socket->Close();
    if(m_client_board_socket != NULL)
    {
        delete m_client_board_socket;
        m_client_board_socket = NULL;
    }
    if(m_p_board_File != NULL)
    {
        delete m_p_board_File;
        m_p_board_File = NULL;
    }
    if (m_p_board_ArchiveOut!=NULL)
    {
		m_p_board_ArchiveOut->Abort();
        delete m_p_board_ArchiveOut;
        m_p_board_ArchiveOut = NULL;
    }

	m_client_cam_socket->Close();
    if(m_client_cam_socket != NULL)
    {
        delete m_client_cam_socket;
        m_client_cam_socket = NULL;
    }
    if(m_p_cam_File != NULL)
    {
        delete m_p_cam_File;
        m_p_cam_File = NULL;
    }
    if (m_p_cam_ArchiveOut!=NULL)
    {
		m_p_cam_ArchiveOut->Abort();
        delete m_p_cam_ArchiveOut;
        m_p_cam_ArchiveOut = NULL;
    }
	return CDialogEx::DestroyWindow();
}

void CChatClientDlg::OnBnClickedButtonColor()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CColorDialog dlg;
    if(dlg.DoModal() != IDOK) return;
    m_crCurColor = dlg.GetColor();	
}


void CChatClientDlg::OnBnClickedButtonErase()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pView->ProcessErase();
	int code = NET_ERASE;
	m_client_board_socket->Send(&code, sizeof(int));
	Invalidate(TRUE);
}


void CChatClientDlg::OnBnClickedButtonImageload()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pView->PicView();
}

void CChatClientDlg::SendData(CData* pData)
{
    if(m_p_board_ArchiveOut == NULL) return;

	int code = NET_POINT;
	m_p_board_ArchiveOut->Write(&code, sizeof(int));
	pData->Serialize(*m_p_board_ArchiveOut);
	m_p_board_ArchiveOut->Flush();
}

void CChatClientDlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	capture = cvCreateCameraCapture(0);

	if( capture == NULL )
	{
		AfxMessageBox("ī�޶� �������� �ʽ��ϴ�.");
	}
	else
	{
		SetTimer(TIMER_FRAMERATE,66,NULL);
		SetTimer(2,300,NULL);
	}
}


void CChatClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	IplImage *temp;
	switch (nIDEvent)
	{
	case TIMER_FRAMERATE:
		{
			if( capture != NULL )
			{
				frame = cvQueryFrame(capture);
			}
			
		}
		break;
	case 2:
		{
			if( frame != NULL )
			{
				temp = resizing(frame);
				CvMat* mat = cvEncodeImage (".jpg",temp);

				int size = mat->cols*mat->rows;
				int code = NET_CAMID;
				vector<byte> v_char;
				m_client_cam_socket->Send(&size, sizeof(int));
				m_client_cam_socket->Send(mat->data.ptr, size);
			}
			
		}
		break;
	default:
		break;
	}
	Invalidate(FALSE);
	CDialogEx::OnTimer(nIDEvent);
}

void CChatClientDlg::ProcessCam()
{
	int readSize;
	int fileSize;
	byte* buffer = new byte[104857];
	m_client_cam_socket->Receive(&fileSize, sizeof(int));
	// ���� �ϳ� �����
	CByteArray a;

	// ��â ����
	while(fileSize > 0)
	{
		readSize = m_client_cam_socket->Receive(buffer, 104857);
		
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
	remotecam = cvDecodeImage(&cvMat(1,a.GetSize(), CV_8UC3,a.GetData()));
	Invalidate(FALSE);
	delete buffer;
}

IplImage* CChatClientDlg::resizing (IplImage *source) {

IplImage *destination = cvCreateImage ( cvSize(100 , 50 ), source->depth, source->nChannels );


cvResize(source, destination);

return destination;

}