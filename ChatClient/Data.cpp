// Data.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ChatClient.h"
#include "Data.h"


// CData

CData::CData()
{
}

CData::CData(CPoint ptPos, COLORREF crColor, int line)
{
	m_crColor = crColor; 
    m_ptPos = ptPos; 
	m_line = line;
}
CData::CData(CData& data)
{
    m_crColor = data.m_crColor; 
    m_ptPos = data.m_ptPos; 
	m_line = data.m_line;
}

CData::~CData()
{
}


// CData 멤버 함수


void CData::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
        ar << m_ptPos; 
        ar << m_crColor;
		ar << m_line; 
	}
	else
	{	// loading code
        ar >> m_ptPos; 
        ar >> m_crColor; 
		ar >> m_line;
	}
}
