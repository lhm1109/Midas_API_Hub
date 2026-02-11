// LogAxis.h: interface for the CLogAxis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGAXIS_H__50AC0A21_35D3_11D3_B00F_0000C0B9C58C__INCLUDED_)
#define AFX_LOGAXIS_H__50AC0A21_35D3_11D3_B00F_0000C0B9C58C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLogAxis
{
public:
	void initialize();
	CPoint FindCrossPoint(CPoint, CPoint);
	BOOL		m_IsCreated;
	int			m_TickNum;
	int			m_LabelSlope, m_TickSlope;
	CPoint		m_AxisP[2];
	CString		m_LabelText;

	CArray <CString, CString>	m_TickText;
	CArray <CPoint, CPoint>		m_TickPos;

	void	DrawAllLabels(CDC *);
	void	AddLabel(CString, CPoint);

	CLogAxis();
	virtual ~CLogAxis();

};

#endif // !defined(AFX_LOGAXIS_H__50AC0A21_35D3_11D3_B00F_0000C0B9C58C__INCLUDED_)
