#include "stdafx.h"
//#include "SGSV02.h"
#include "MyGraphDisplay.h"
#include "LogAxis.h"              // SGS 

#include <math.h>
#include <float.h>
#include <stdlib.h>

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////
//// ADDED by LSS  for SGS Tripartite type graph
///////////////////////////////////////////////////////////////////////////////////////////////////////

MyGraphDisplay::MyGraphDisplay()
{
	m_bTripartite = FALSE;
}

MyGraphDisplay::~MyGraphDisplay()
{
}

void MyGraphDisplay::SetTripartite(BOOL bTripartite)
{
	m_bTripartite = bTripartite;
}

void MyGraphDisplay::SetDiagGrid(BOOL bDiagGridFlag)
{
	m_bDiagGridFlag = bDiagGridFlag;
}

BOOL MyGraphDisplay::IsTripartite()
{
	return m_bTripartite;
}

void MyGraphDisplay::PutTicks()
{
	int				i, stage=0, tick_num, rank;
	CPoint			s, e, cross;
	CString			lbl;
	double			minx, maxx, xrange, miny, maxy, yrange;
	double			Sd=0.00001, Sa=0.00001, add_factor=0.00001, val1, val2;
	CPen			Apen,Dpen, *pOldPen;
	CRect			dr;

	CScale Scale = GetScaleX();

	FindLogAxis();

	minx = log10( GetScaleX().Min());
	maxx = log10( GetScaleX().Max());
	xrange = maxx - minx;
	miny = log10( GetScaleY().Min());
	maxy = log10( GetScaleY().Max());
	yrange = maxy - miny;

	dr =  GetDisplayRect();

	Dpen.CreatePen(PS_SOLID, 1, RGB(100,100,255));
	Apen.CreatePen(PS_SOLID, 1, RGB(255,90,90));

	if ( m_bDiagGridFlag==TRUE ) 
	{
		// Deformation
		pOldPen = m_pDC->SelectObject(&Dpen);
		s.x = -1;
		m_LADeform.initialize();
		tick_num = 0;
		for (i = 0; ;i++) 
		{
			if (stage == 0)
			{
				if ((val1=GetDeformParms(-2000, miny, Sd)) >= minx)
				{
					i--;
					stage++;
					continue;
				}
			}
			else if (stage == 1)
			{
				if ((val1=GetDeformParms(-2000, miny, Sd)) >= maxx) 
				{
					i--;
					stage++;
					continue;
				}
				s.x = dr.left + (int) (((double) dr.Width())*((val1-minx)/xrange));
				s.y = dr.bottom;
				m_pDC->MoveTo(s);
				if ((val2=GetDeformParms(minx, -2000, Sd)) <= maxy) 
				{
					e.x = dr.left;
					e.y = dr.bottom - (int) (((double) dr.Height())*((val2-miny)/yrange));
				}
				else
				{
					val2 = GetDeformParms(-2000, maxy, Sd);
					e.x = dr.left + (int) (((double) dr.Width())*((val2-minx)/xrange));
					e.y = dr.top;
				}
				m_pDC->LineTo(e);
			}
			else if (stage == 2)
			{
				if ((val1=GetDeformParms(maxx, -2000, Sd)) >= maxy)
				{
					break;
				}
				s.x = dr.right;
				s.y = dr.bottom - (int) (((double) dr.Height())*((val1-miny)/yrange));
				m_pDC->MoveTo(s);
				if ((val2=GetDeformParms(minx, -2000, Sd)) <= maxy) 
				{
					e.x = dr.left;
					e.y = dr.bottom - (int) (((double) dr.Height())*((val2-miny)/yrange));
				}
				else
				{
					val2 = GetDeformParms(-2000, maxy, Sd);
					e.x = dr.left + (int) (((double) dr.Width())*((val2-minx)/xrange));
					e.y = dr.top;
				}
				m_pDC->LineTo(e);
			}

			/*  make a tick at every 5.

			if ((i%5) == 0) {
			if ((i%10) == 0) {
			if (i != 0)
			add_factor *= 10.0;
			}
			if (s.x >= 0) {
			cross = m_LADeform.FindCrossPoint(s, e);
			if (IsInside(cross)) {
			if (m_LADeform.m_IsCreated && (m_LADeform.m_TickPos.GetSize()>tick_num)) {
			m_LADeform.m_TickPos[tick_num] = cross;
			tick_num++;
			}
			else {
			rank = GetFloatRank(Sd);
			lbl.Format(_T("%f"), Sd);
			if (rank >= 0)
			m_LADeform.AddLabel(lbl.Left(rank+2), cross);
			else
			m_LADeform.AddLabel(lbl.Left(-rank+2), cross);
			tick_num++;
			}
			}
			}
			}
			if ((i%10) != 0)
			Sd += add_factor;
			*/

			/// make a tick at every 10.		


			if ((i%10) == 0) 
			{
				if (i != 0)
					add_factor *= 10.0;

				if (s.x >= 0) 
				{
					cross = m_LADeform.FindCrossPoint(s, e);
					if (IsInside(cross)) 
					{
						if (m_LADeform.m_IsCreated && (m_LADeform.m_TickPos.GetSize()>tick_num)) 
						{
							m_LADeform.m_TickPos[tick_num] = cross;
							tick_num++;
						}
						else 
						{
							rank = GetFloatRank(Sd);
							lbl.Format(_T("%f"), Sd);
							if (rank >= 0)
								m_LADeform.AddLabel(lbl.Left(rank+2), cross);
							else
								m_LADeform.AddLabel(lbl.Left(-rank+2), cross);
							tick_num++;
						}
					}
				}
			}
			else Sd += add_factor;

		}
		m_LADeform.m_IsCreated = TRUE;

		Dpen.DeleteObject();

		m_pDC->SelectObject(&Apen);

		// Acceleration
		stage = 0;
		add_factor = 0.00001;
		s.x = -1;
		m_LAAccel.initialize();
		tick_num = 0;
		for (i=0;;i++) 
		{
			if (stage == 0) 
			{
				if ((val1=GetAccelParms(-2000, miny, Sa)) <= maxx)
				{
					i--;
					stage++;
					continue;
				}
			}
			else if (stage == 1) 
			{
				if ((val1=GetAccelParms(-2000, miny, Sa)) <= minx) 
				{
					i--;
					stage++;
					continue;
				}
				s.x = dr.left + (int) (((double) dr.Width())*((val1-minx)/xrange));
				s.y = dr.bottom;
				m_pDC->MoveTo(s);
				if ((val2=GetAccelParms(maxx, -2000, Sa)) <= maxy) 
				{
					e.x = dr.right;
					e.y = dr.bottom - (int) (((double) dr.Height())*((val2-miny)/yrange));
				}
				else 
				{
					val2 = GetAccelParms(-2000, maxy, Sa);
					e.x = dr.left + (int) (((double) dr.Width())*((val2-minx)/xrange));
					e.y = dr.top;
				}
				m_pDC->LineTo(e);
			}
			else if (stage == 2) 
			{
				if ((val1=GetAccelParms(minx, -2000, Sa)) >= maxy) 
					break;

				s.x = dr.left;
				s.y = dr.bottom - (int) (((double) dr.Height())*((val1-miny)/yrange));
				m_pDC->MoveTo(s);
				if ((val2=GetAccelParms(maxx, -2000, Sa)) <= maxy) 
				{
					e.x = dr.right;
					e.y = dr.bottom - (int) (((double) dr.Height())*((val2-miny)/yrange));
				}
				else 
				{
					val2 = GetAccelParms(-2000, maxy, Sa);
					e.x = dr.left + (int) (((double) dr.Width())*((val2-minx)/xrange));
					e.y = dr.top;
				}
				m_pDC->LineTo(e);
			}


			if ((i%10) == 0) 
			{
				if (i != 0)
					add_factor *= 10.0;

				if (s.x >= 0) 
				{
					cross = m_LAAccel.FindCrossPoint(s, e);
					if (IsInside(cross))
					{
						if (m_LAAccel.m_IsCreated && (m_LAAccel.m_TickPos.GetSize()>tick_num)) 
						{
							m_LAAccel.m_TickPos[tick_num] = cross;
							tick_num++;
						}
						else 
						{
							rank = GetFloatRank(Sa);
							lbl.Format(_T("%f"), Sa);
							if (rank >= 0)
								m_LAAccel.AddLabel(lbl.Left(rank+2), cross);
							else
								m_LAAccel.AddLabel(lbl.Left(-rank+2), cross);
							tick_num++;

						}
					}
				}
			}
			else Sa += add_factor;
		}	
		m_LAAccel.m_IsCreated = TRUE;

		m_pDC->SelectObject(pOldPen);
		Apen.DeleteObject();
	}

	m_LADeform.DrawAllLabels(m_pDC);
	m_LAAccel.DrawAllLabels(m_pDC);
}

void MyGraphDisplay::FindLogAxis()
{
	int		rank;
	double	minx, maxx, xrange, miny, maxy, yrange;
	double	s, val;
	CRect	dr;

	minx = log10(GetScaleX().Min());
	maxx = log10(GetScaleX().Max());
	xrange = maxx - minx;
	miny = log10(GetScaleY().Min());
	maxy = log10(GetScaleY().Max());
	yrange = maxy - miny;

	dr = GetDisplayRect();

	/////////////////////////////////////////////////
	//	Deformation Axis
	s = GetAccelParms((minx+maxx)/2.0, (miny+maxy)/2.0, -2000.0);
	rank = GetFloatRank(s);
	s = pow(10.0, rank)*2.0;

	if ((val=GetAccelParms(-2000.0, miny, s)) >= minx) {
		m_LADeform.m_AxisP[0].x = dr.left + (int) (((double) dr.Width())*((val-minx)/xrange));
		m_LADeform.m_AxisP[0].y = dr.bottom;
	}
	else {
		m_LADeform.m_AxisP[0].x = dr.left;
		m_LADeform.m_AxisP[0].y = dr.bottom - (int) (((double) dr.Height())*((GetAccelParms(minx, -2000.0, s)-miny)/yrange));
	}
	if ((val=GetAccelParms(maxx, -2000.0, s)) <= maxy) {
		m_LADeform.m_AxisP[1].x = dr.right;
		m_LADeform.m_AxisP[1].y = dr.bottom - (int) (((double) dr.Height())*((val-miny)/yrange));
	}
	else {
		m_LADeform.m_AxisP[1].x = dr.left + (int) (((double) dr.Width())*((GetAccelParms(-2000.0, maxy, s)-minx)/xrange));
		m_LADeform.m_AxisP[1].y = dr.top;
	}

	m_LADeform.m_LabelSlope = (int) ((1800.0/3.14159)*
		atan(((double) (m_LADeform.m_AxisP[1].y-m_LADeform.m_AxisP[0].y))/
		((double) (m_LADeform.m_AxisP[0].x-m_LADeform.m_AxisP[1].x))));
	if (m_LADeform.m_LabelSlope < 0.0)
		m_LADeform.m_LabelSlope += 3600;
	m_LADeform.m_TickSlope = 3600 - m_LADeform.m_LabelSlope;
	//
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	//	Acceleration Axis
	s = GetDeformParms((minx+maxx)/2.0, (miny+maxy)/2.0, -2000.0);
	rank = GetFloatRank(s);
	s = pow(10.0, rank)*2.0;

	if ((val=GetDeformParms(-2000.0, miny, s)) <= maxx) {
		m_LAAccel.m_AxisP[0].x = dr.left + (int) (((double) dr.Width())*((val-minx)/xrange));
		m_LAAccel.m_AxisP[0].y = dr.bottom;
	}
	else {
		m_LAAccel.m_AxisP[0].x = dr.right;
		m_LAAccel.m_AxisP[0].y = dr.bottom - (int) (((double) dr.Height())*((GetDeformParms(maxx, -2000.0, s)-miny)/yrange));
	}
	if ((val=GetDeformParms(minx, -2000.0, s)) <= maxy) {
		m_LAAccel.m_AxisP[1].x = dr.left;
		m_LAAccel.m_AxisP[1].y = dr.bottom - (int) (((double) dr.Height())*((val-miny)/yrange));
	}
	else {
		m_LAAccel.m_AxisP[1].x = dr.left + (int) (((double) dr.Width())*((GetDeformParms(-2000.0, maxy, s)-minx)/xrange));
		m_LAAccel.m_AxisP[1].y = dr.top;
	}

	m_LAAccel.m_LabelSlope = (int) ((1800.0/3.14159)*
		atan(((double) (m_LAAccel.m_AxisP[1].y-m_LAAccel.m_AxisP[0].y))/
		((double) (m_LAAccel.m_AxisP[0].x-m_LAAccel.m_AxisP[1].x))));
	if (m_LAAccel.m_LabelSlope < 0.0)
		m_LAAccel.m_LabelSlope += 3600;
	m_LAAccel.m_TickSlope = 3600 - m_LAAccel.m_LabelSlope;
	//
	/////////////////////////////////////////////////

}


BOOL MyGraphDisplay::IsInside(CPoint p)
{
	CRect dr;

	dr = GetDisplayRect();
	if ((p.x>dr.left)&&(p.x<dr.right)&&(p.y>dr.top)&&(p.y<dr.bottom)) 	return TRUE;
	return FALSE;
}

int MyGraphDisplay::GetFloatRank(double d)
{
	int	rank=0;

	if (d > 1.05) 
	{
		while (1) 
		{
			if ((d=d/10.0) >= 0.95)
				rank++;
			else
				break;
		}
	}
	else if (d < 0.95) 
	{
		while (1)
		{
			if ((d=d*10.0) < 9.5)
				rank--;
			else
				break;
		}
	}

	return rank;
}

double MyGraphDisplay::GetDeformParms(double x, double y, double d)
{
	double	Pi=3.14159; 

	if (x < -1000.0) {
		x = -y + log10(2.0*Pi*d);
		return x;
	}
	else if (y < -1000.0) {
		y = -x + log10(2.0*Pi*d);
		return y;
	}
	else if (d < -1000.0) {
		d = pow(10.0, x+y)/(2.0*Pi);
		return d;
	}
	return -2000.0;

}

double MyGraphDisplay::GetAccelParms(double x, double y, double d)
{
	double	Pi=3.141592, Grav;

	//        mm     cm      m   inch  feet
	double Gravity[5] = { 9815.0, 98.15, 9.815, 386.4, 32.2 };

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX UnitIndex;	
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);

	int nIndex = 0;

	switch(UnitIndex.nBase_Length)
	{
	case D_UNITSYS_LENGTH_INDEX_MM:
		nIndex = 0;
		break;
	case D_UNITSYS_LENGTH_INDEX_CM:
		nIndex = 1;
		break;
	case D_UNITSYS_LENGTH_INDEX_M:
		nIndex = 2;
		break;
	case D_UNITSYS_LENGTH_INDEX_IN:
		nIndex = 3;
		break;
	case D_UNITSYS_LENGTH_INDEX_FT:
		nIndex = 4;
		break;
	}

	Grav = Gravity[nIndex];

	if (x < -1000.0) {
		x = y - log10((d*Grav)/(2.0*Pi));
		return x;
	}
	else if (y < -1000.0) {
		y = x + log10((d*Grav)/(2.0*Pi));
		return y;
	}
	else if (d < -1000.0) {
		d = 2.0*Pi*pow(10.0, y-x)/Grav;
		return d;
	}
	return -2000.0;
}

void MyGraphDisplay::DrawClassicAxis()
{
	CPen *GridPen,*FramePen,*OldPen;
	CBrush FillBrush,*OldBrush;
	SRGTickMarks Ticks;

	// get a brush for the fill
	FillBrush.CreateSolidBrush(GetStyle()->GetColor());

	if(GetStyle()->GetAxisFillStyle())
		OldBrush=(CBrush *)m_pDC->SelectObject(&FillBrush);
	else
		OldBrush=(CBrush *)m_pDC->SelectStockObject(NULL_BRUSH);


	// draw the frame
	FramePen=GetPen(PS_SOLID,GetStyle()->GetFrameWidth(),GetStyle()->GetFrameColor());
	m_pDC->SelectObject(FramePen);
	m_pDC->Rectangle(m_DisplayRect.left,m_DisplayRect.top,m_DisplayRect.right+1,m_DisplayRect.bottom+1); // CDC::Rectangle really is the most annoying function ever!


	// draw Y tick marks
	if(GetStyle()->GetShowYTicks() && !m_YTickList.IsEmpty())
	{
		// This now uses the SRGTickMarks class
		Ticks.SetParent(GetParent());
		Ticks.DrawingUsing(GetpDC(),GetpCWnd());
		Ticks.SetptrTickPositionList(&m_YTickList);
		Ticks.SetMajorTickSize((int)GetTickSize());
		Ticks.SetMinorTickRatio(0);
		Ticks.SetMajorTickColor(GetStyle()->GetFrameColor());
		Ticks.SetPass(1);
		Ticks.Draw(GetpDC(),GetpCWnd());
	}

	// dray X tick marks
	if(GetStyle()->GetShowXTicks() && !m_XTickList.IsEmpty())
	{
		Ticks.SetParent(GetParent());
		Ticks.DrawingUsing(GetpDC(),GetpCWnd());
		Ticks.SetptrTickPositionList(&m_XTickList);
		Ticks.SetMajorTickSize((int)GetTickSize());
		Ticks.SetMinorTickRatio(0);
		Ticks.SetMajorTickColor(GetStyle()->GetFrameColor());
		Ticks.SetPass(1);
		Ticks.Draw(GetpDC(),GetpCWnd());
	}

	// Draw the grid
	GridPen=GetPen(PS_SOLID,1,GetStyle()->GetGridColor());
	OldPen=(CPen *)m_pDC->SelectObject(GridPen);

	if(GetStyle()->GetShowYGrid() && !m_YTickList.IsEmpty())
	{
		for(POSITION p=m_YTickList.GetHeadPosition();p!=NULL;)
		{
			CPoint * pP=(CPoint *)m_YTickList.GetNext(p);
			m_pDC->MoveTo(m_DisplayRect.left,pP->y);
			m_pDC->LineTo(m_DisplayRect.right,pP->y);
		}

	}

	if(GetStyle()->GetShowXGrid() && !m_XTickList.IsEmpty())
	{
		for(POSITION p=m_XTickList.GetHeadPosition();p!=NULL;)
		{
			CPoint * pP=(CPoint *)m_XTickList.GetNext(p);
			m_pDC->MoveTo(pP->x,m_DisplayRect.bottom);
			m_pDC->LineTo(pP->x,m_DisplayRect.top);
		}
	}

	// draw X grid for SGS tripartite graph 

	if( m_bTripartite )	PutTicks();


	// done -- clean up
	m_pDC->SelectObject(OldPen);
	m_pDC->SelectObject(OldBrush);

	//FramePen.DeleteObject();
	//GridPen.DeleteObject();

	FillBrush.DeleteObject();

}




/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


