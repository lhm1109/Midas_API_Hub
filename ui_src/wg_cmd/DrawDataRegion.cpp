#include "Stdafx.h"
#include "wg_cmd.h"

#include "DrawDataRegion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void CDrawRegion::Init()
{
	memset(this, 0, sizeof(CDrawRegion));

	m_nMode = MM_LOMETRIC;
	m_nRate = 10;	// mm
	m_nFont = 40;

	m_IsIso = TRUE;
	m_scale = 1.0;
	m_wxBay = 1.0;
	m_wyBay = 1.0;

	m_screen.left = 0;
	m_screen.top = 0;
	m_screen.right = 10;
	m_screen.bottom = -10;
	m_curRect.Set(0., 0., 10., -10.);
	m_limRect.Set(0., 0., 10., -10.);	
}

void CDrawRegion::Clear()
{
}

void CDrawRegion::SetSize(RECT& view, T_RECT& real)
{
	double	rx = (double)real.Width();			// x_length of real_rect
	double	ry = (double)real.Height();			// y_length of real_rect
	double	lx = (double)(view.right - view.left + 1) / m_nRate;	// x_length of view_rect
	double	ly = (double)(view.top - view.bottom + 1) / m_nRate;	// y_length of view_rect
	double	add;

	if ((ry / rx) >= (ly / lx))
	{
		m_scale = ry / ly;
		if (fabs(ry / rx - ly / lx) > 0.001)
		{
			add = (lx * m_scale - rx) / 2.0;
			real.left -= add;
			real.right += add;
		}
	}
	else
	{
		m_scale = rx / lx;
		if (fabs(ry / rx - ly / lx) > 0.001)
		{
			add = (ly * m_scale - ry) / 2.0;
			real.top += add;
			real.bottom -= add;
		}
	}

	m_screen = view;
	m_curRect= real;
	m_uFont  = (short)((double)m_nFont / m_scale);

	if (m_curRect.left < m_limRect.left)
	{
		if (m_curRect.Width() < m_limRect.Width())
		{
			m_curRect.right = m_limRect.left + m_curRect.Width();
			m_curRect.left = m_limRect.left;
		}
		else
		{
			double	add = (m_curRect.Width() - m_limRect.Width()) / 2.0;
			m_curRect.left = m_limRect.left = m_limRect.left - add;
			m_curRect.right = m_limRect.right = m_limRect.right + add;
		}
	}
	if (m_curRect.right > m_limRect.right)
	{
		if (m_curRect.Width() < m_limRect.Width())
		{
			m_curRect.left = m_limRect.right - m_curRect.Width();
			m_curRect.right = m_limRect.right;
		}
		else
		{
			double add = (m_curRect.Width() - m_limRect.Width()) / 2.0;
			m_curRect.left = m_limRect.left = m_limRect.left - add;
			m_curRect.right = m_limRect.right = m_limRect.right + add;
		}
	}
	if (m_curRect.top > m_limRect.top)
	{
		if (m_curRect.Height() < m_limRect.Height())
		{
			m_curRect.bottom = m_limRect.top - m_curRect.Height();
			m_curRect.top = m_limRect.top;
		}
		else
		{
			double	add = (m_curRect.Height() - m_limRect.Height()) / 2.0;
			m_curRect.top = m_limRect.top = m_limRect.top + add;
			m_curRect.bottom = m_limRect.bottom = m_limRect.bottom - add;
		}
	}
	if (m_curRect.bottom < m_limRect.bottom)
	{
		if (m_curRect.Height() < m_limRect.Height())
		{
			m_curRect.top = m_limRect.bottom + m_curRect.Height();
			m_curRect.bottom = m_limRect.bottom;
		}
		else
		{
			double	add = (m_curRect.Height() - m_limRect.Height()) / 2.0;
			m_curRect.top = m_limRect.top = m_limRect.top + add;
			m_curRect.bottom = m_limRect.bottom = m_limRect.bottom - add;
		}
	}

	m_wxBay = (m_curRect.right - m_curRect.left) / (double)(m_screen.right - m_screen.left + 1);
	m_wyBay = (m_curRect.top - m_curRect.bottom) / (double)(m_screen.top - m_screen.bottom + 1);
}

void CDrawRegion::SetScreen(RECT& view, T_RECT& real)
{
	double	rx = (double)real.Width();			// x_length of real_rect
	double	ry = (double)real.Height();			// y_length of real_rect
	double	lx = (double)(view.right - view.left + 1) / m_nRate;	// x_length of view_rect
	double	ly = (double)(view.top - view.bottom + 1) / m_nRate;	// y_length of view_rect
	double	add;

	if (rx < ONE_EPSILON || ry < ONE_EPSILON) return;
	if (lx < ONE_EPSILON || ly < ONE_EPSILON) return;

	if ( (ry / rx) >= (ly / lx) )
	{
		m_scale = ry / ly;
		if (fabs(ry / rx - ly / lx) > 0.001)
		{
			add = (lx * m_scale - rx) / 2.0;
			real.left -= add;
			real.right += add;
		}
	}
	else
	{
		m_scale = rx / lx;
		if (fabs(ry / rx - ly / lx) > 0.001)
		{
			add = (ly * m_scale - ry) / 2.0;
			real.top += add;
			real.bottom -= add;
		}
	}

	m_screen = view;
	m_curRect= real;
	m_limRect = m_curRect;
	m_uFont  = (short)( (double)m_nFont / m_scale);

	m_wxBay = (m_curRect.right - m_curRect.left) / (double)(m_screen.right - m_screen.left + 1);
	m_wyBay = (m_curRect.top - m_curRect.bottom) / (double)(m_screen.top - m_screen.bottom + 1);
}

void CDrawRegion::SetNotIso(RECT& view, T_RECT& real)
{
	double	rx = (double)real.Width();			// x_length of real_rect
	double	ry = (double)real.Height();			// y_length of real_rect
	double	lx = (double)(view.right - view.left + 1) / m_nRate;	// x_length of view_rect
	double	ly = (double)(view.top - view.bottom + 1) / m_nRate;	// y_length of view_rect

	if (rx < ONE_EPSILON || ry < ONE_EPSILON) return;
	if (lx < ONE_EPSILON || ly < ONE_EPSILON) return;

	if((ry / rx) >= (ly / lx))
	{
		m_scale = ry / ly;
	}
	else
	{
		m_scale = rx / lx;
	}

	m_IsIso  = FALSE;
	m_screen = view;
	m_curRect = real;
	m_limRect = m_curRect;
	m_uFont = (short)((double)m_nFont / m_scale);

	m_wxBay = (m_curRect.right - m_curRect.left) / (double)(m_screen.right - m_screen.left + 1);
	m_wyBay = (m_curRect.top - m_curRect.bottom) / (double)(m_screen.top - m_screen.bottom + 1);
}