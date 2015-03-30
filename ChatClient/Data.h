#pragma once

// CData 명령 대상입니다.
#define NET_ERASE 0x00001000
#define NET_POINT 0x00002000
#define NET_IMAGE 0x00003000
#define NET_CAMID 0x00004000

class CData : public CObject
{
public:
	CPoint m_ptPos ;
	COLORREF m_crColor;
	int m_line;
	CData();
	CData(CPoint ptPos, COLORREF crColor, int m_line);
	CData(CData& data);
	virtual ~CData();
	virtual void Serialize(CArchive& ar);
};


