#pragma once
#include "Data.h"
class CChatClientDlg;

// CImageView ���Դϴ�.

class CImageView : public CView
{
	DECLARE_DYNCREATE(CImageView)

protected:
	CImageView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CImageView();

public:
	CChatClientDlg *m_CChatClient;
	CPtrList m_dataList;
	Image *image;
	int printImage;

	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	void SetView(CChatClientDlg *pCChatClient);
	void PicView();
	void ProcessErase();
	void ProcessImage();
	void ProcessPoint();
	void ProcessClose();
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


