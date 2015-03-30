#pragma once
#include "Data.h"
class CChatClientDlg;

// CImageView 뷰입니다.

class CImageView : public CView
{
	DECLARE_DYNCREATE(CImageView)

protected:
	CImageView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CImageView();

public:
	CChatClientDlg *m_CChatClient;
	CPtrList m_dataList;
	Image *image;
	int printImage;

	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
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


