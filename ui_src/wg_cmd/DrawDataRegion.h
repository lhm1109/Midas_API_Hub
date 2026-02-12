#pragma once

//#pragma comment(lib, "gdiplus.lib")
// #include <gdiplus.h>
// using namespace Gdiplus;

#include "DrawDataObject.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDrawRegion
{
public:
	// Constructions
	CDrawRegion()  { Init(); }
	~CDrawRegion() { Clear(); }
	void Init(); 
	void Clear(); 

	RECT	m_screen;	// screen rectangle of computer
	T_RECT	m_curRect;	// real rectangle matched with screen
	T_RECT	m_limRect;	// limited real rectangle
	BOOL	m_IsIso;	// Isotropic or anisotropic ?
	BOOL	m_IsPrint;	// Is printing ?
	BOOL    m_IsAngleReverse;
	
	double	m_scale;	// scale : real/screen
	double	m_wxBay;	// x-direction bay for speed-up
	double	m_wyBay;	// y-direction bay for speed-up

	short	m_nMode;	// Mapping Mode : MM_LOMETRIC or MM_LOENGLISH
	short	m_nRate;	// MM_LOMETRIC(mm=10,cm=100,m=10000) MM_LOENGLISH(in=100,ft=1200)
	short	m_nFont;	// MM_LOMETRIC=32, MM_LOENGLISH=15
	short	m_uFont;	// default font size according to scale
	
	// 1. setting
	void  SetSize(RECT& view, T_RECT& real); 
	void  SetScreen(RECT& view, T_RECT& real);
	void  SetNotIso(RECT& view, T_RECT& real);

	void LPtoTP(const POINT& p, T_POINT& xp);
	void TPtoLP(const T_POINT& xp, POINT& p);
	//void GPtoTP(const Gdiplus::Point& p, T_POINT& xp);
	//void TPtoGP(const T_POINT& xp, Gdiplus::Point& p);
};
#include "HeaderPost.h"

inline void CDrawRegion::LPtoTP(const POINT& p, T_POINT& xp)
{
	xp.x = m_curRect.left + m_wxBay * 0.5 + (double)(p.x - m_screen.left) * m_wxBay;
	xp.y = m_curRect.bottom + m_wyBay * 0.5 + (double)(p.y - m_screen.bottom) * m_wyBay;
}

inline void CDrawRegion::TPtoLP(const T_POINT& xp, POINT& p)
{
	p.x = m_screen.left + int((xp.x - m_curRect.left) / m_wxBay);
	p.y = m_screen.bottom + int((xp.y - m_curRect.bottom) / m_wyBay);
}

// inline void CDrawRegion::GPtoTP(const Gdiplus::Point& p, T_POINT& xp)
// {
// 	xp.x = m_curRect.left + m_wxBay * 0.5 + (double)(p.X - m_screen.left) * m_wxBay;
// 	xp.y = m_curRect.bottom + m_wyBay * 0.5 + (double)(p.Y - m_screen.bottom) * m_wyBay;
// }
// 
// inline void CDrawRegion::TPtoGP(const T_POINT& xp, Gdiplus::Point& p)
// {
// 	p.X = m_screen.left + int((xp.x - m_curRect.left) / m_wxBay);
// 	p.Y = m_screen.bottom + int((xp.y - m_curRect.bottom) / m_wyBay);
// }
