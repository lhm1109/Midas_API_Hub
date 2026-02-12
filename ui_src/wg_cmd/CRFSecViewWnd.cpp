#include "stdafx.h"
#include "wg_cmd.h"
#include "CRFSecViewWnd.h"

#include "..\wg_base\CompFunc.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_EditData.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_Db\wg_db_SecShape.h"

#include <float.h>  // for DBL_MAX

#include "..\wg_base\wg_base_I_Factory_DlgViewPanel.h"
#include "..\wg_base\Registry.h"

#include "..\..\dgnengine\idesign\DGN_lib\DGNCompare.h"

#ifndef M_PI
#define M_PI        3.14159265358979323846
#endif

#define CCM_WHITE     COLORREF(RGB(255, 255, 255))
#define CCM_BLACK     COLORREF(RGB(0, 0, 0))
#define CCM_REDGREEN  COLORREF(RGB(128, 128, 0))
#define CCM_GRAY      COLORREF(RGB(128, 128, 128))
#define CCM_BLUE      COLORREF(RGB(0, 0, 255))
#define CCM_DARKBLUE  COLORREF(RGB(0, 0, 128))
#define CCM_YELLOW    COLORREF(RGB(255, 255, 0))
#define CCM_CYAN      COLORREF(RGB(0, 255, 255))
#define CCM_RED       COLORREF(RGB(255, 0, 0))

#define CCM_BORDER        CCM_WHITE
#define CCM_FILL          CCM_REDGREEN
#define CCM_HOLE          CCM_BLACK
#define CCM_SRC_FILL      CCM_GRAY

#define CCM_SECTI_BORDER  CCM_WHITE
#define CCM_SECTI_FILL    CCM_REDGREEN
#define CCM_SECTJ_BORDER  CCM_WHITE
#define CCM_SECTJ_FILL    CCM_GRAY
#include "..\wg_base\wg_base_MsgDll.h"  

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace dgn;

CRFSecViewWnd::CRFSecViewWnd()
{
	m_pDataSrc = NULL;
	m_bAfter = FALSE;
	m_bReverse = FALSE;
	m_bCentroid = FALSE;
	m_bShearCheck = FALSE;
	m_bChangeAxis = FALSE;

	m_pDataSrcRebar = 0;
	m_bEndJ = FALSE;

	m_bComGenJ = FALSE;
}

CRFSecViewWnd::~CRFSecViewWnd()
{
}

BOOL CRFSecViewWnd::Init(CWnd* pParentWnd)
{
	CWnd* pSubclass = CWnd::FromHandlePermanent(pParentWnd->m_hWnd);
	HWND hWnd = pParentWnd->m_hWnd;
	if (pSubclass != nullptr)
		hWnd = pSubclass->UnsubclassWindow();

	if(!SubclassWindow(hWnd))
	{
		TRACE(_LS(IDS_CMD0417__Fail_To_Subclassing_n));
		return FALSE;
	}

	LONG dwStyle;
	dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	dwStyle |= WS_CLIPCHILDREN;
	SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);

	CRect r;
	GetClientRect(r);
	m_DrawArea = r;

	return TRUE;
}

void CRFSecViewWnd::SetDataSource(T_SECT_D* pOriginSect, T_RFST_D* pReinforceSect, BOOL bAfter /* = FALSE */, BOOL bChangeAxis /* = FALSE */)
{
	m_pDataSrc = pOriginSect;
	m_pReinforceSect = pReinforceSect;
	m_bAfter = bAfter;
	m_bChangeAxis = bChangeAxis;
}

BEGIN_MESSAGE_MAP(CRFSecViewWnd, CWnd)
	//{{AFX_MSG_MAP(CRFSecViewWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CRFSecViewWnd message handlers
BOOL CRFSecViewWnd::OnEraseBkgnd(CDC* pDC)
{
	EraseBkgnd(pDC);
	return CWnd::OnEraseBkgnd(pDC);
}

void CRFSecViewWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if(!m_pDataSrc->SName.IsEmpty())
	{
		CCMGeom2DArray aGeom;
		MakePicture(m_pDataSrc, aGeom);
		BOOL bCentroid = FALSE;
		BOOL bOffset = FALSE;
		BOOL bShearCheck = FALSE;
		int nRebarNum = 0;
		DrawSection(&dc, aGeom, bCentroid, bOffset, bShearCheck, nRebarNum, m_bChangeAxis);
	}
}

void CRFSecViewWnd::GetRegularData(double& H, double& B1, double& tw, double& tf1, double& B2, double& tf2)
{
	H = m_pDataSrc->SectBefore.SectI.Size[0];
	B1 = m_pDataSrc->SectBefore.SectI.Size[1];
	tw = m_pDataSrc->SectBefore.SectI.Size[2];
	tf1 = m_pDataSrc->SectBefore.SectI.Size[3];
	B2 = m_pDataSrc->SectBefore.SectI.Size[4];
	tf2 = m_pDataSrc->SectBefore.SectI.Size[5];
}

bool CRFSecViewWnd::EQ(double d1, double d2, double dTol/*=1.0e-7*/)
{
	return (fabs(d1 - d2) < fabs(dTol));
}

void CRFSecViewWnd::MakeReinforcementHBeam(CCMGeom2DArray& aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	GetRegularData(H, B1, tw, tf1, B2, tf2);
	CCMPolygon* pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);

	if(B2 == 0.0) B2 = B1;
	if(tf2 == 0.0) tf2 = tf1;
	// Sect Data
	pPolygon->Add(SCMPoint2D(0.0, 0.0));
	pPolygon->Add(SCMPoint2D(0.0, tf1));
	pPolygon->Add(SCMPoint2D(fabs((B1 - tw) / 2.0), tf1));
	pPolygon->Add(SCMPoint2D(fabs((B1 - tw) / 2.0), fabs(H - tf2)));
	pPolygon->Add(SCMPoint2D((B1) / 2.0 - (B2) / 2.0, fabs(H - tf2)));
	pPolygon->Add(SCMPoint2D((B1) / 2.0 - (B2) / 2.0, H));
	pPolygon->Add(SCMPoint2D((B1) / 2.0 + (B2) / 2.0, H));
	pPolygon->Add(SCMPoint2D((B1) / 2.0 + (B2) / 2.0, fabs(H - tf2)));
	pPolygon->Add(SCMPoint2D(fabs((B1 + tw) / 2.0), fabs(H - tf2)));
	pPolygon->Add(SCMPoint2D(fabs((B1 + tw) / 2.0), tf1)); // MQC:2031-JHYUN-20080721
	//pPolygon->Add(SCMPoint2D((B1+tw)/2.0, tf1));
	pPolygon->Add(SCMPoint2D(B1, tf1));
	pPolygon->Add(SCMPoint2D(B1, 0.0));
	SCMRect2D mbr = pPolygon->GetMBR();
	pPolygon->Transfer(SCMPoint2D(-mbr.min.x, -mbr.min.y));
	aGeom.Add(pPolygon);
	// Reinforce Slab
	if(m_pReinforceSect->nReinforceType == 1)
	{
		double dRT1 = m_pReinforceSect->Sect[0];
		double dRB1 = m_pReinforceSect->Sect[1];
		double dRT2 = m_pReinforceSect->Sect[2];
		double dRB2 = m_pReinforceSect->Sect[3];
		if(dRT1 > 0.0 && dRB1 > 0.0)
		{
			CCMPolygon* pPolygon2 = new CCMPolygon(CCM_BORDER, CCM_GRAY);

			pPolygon2->Add(SCMPoint2D((B1 - dRB1) / 2.0, 0.0));
			pPolygon2->Add(SCMPoint2D((B1 + dRB1) / 2.0, 0.0));
			pPolygon2->Add(SCMPoint2D((B1 + dRB1) / 2.0, -dRT1));
			pPolygon2->Add(SCMPoint2D((B1 - dRB1) / 2.0, -dRT1));
			pPolygon2->Transfer(SCMPoint2D(-mbr.min.x, -mbr.min.y));
			aGeom.Add(pPolygon2);
		}
		if(dRT2 > 0.0 && dRB2 > 0.0)
		{
			CCMPolygon* pPolygon3 = new CCMPolygon(CCM_BORDER, CCM_GRAY);

			pPolygon3->Add(SCMPoint2D((B2 - dRB2) / 2.0, H+dRT2));
			pPolygon3->Add(SCMPoint2D((B2 + dRB2) / 2.0, H + dRT2));
			pPolygon3->Add(SCMPoint2D((B2 + dRB2) / 2.0, H));
			pPolygon3->Add(SCMPoint2D((B2 - dRB2) / 2.0, H));
			pPolygon3->Transfer(SCMPoint2D(-mbr.min.x, -mbr.min.y));
			aGeom.Add(pPolygon3);
		}
	}
	else if(m_pReinforceSect->nReinforceType == 2)
	{
		double dRH = m_pReinforceSect->Sect[0];
		double dRB = m_pReinforceSect->Sect[1];
		double dRtw = m_pReinforceSect->Sect[2];
		double dRtf = m_pReinforceSect->Sect[3];
		if(dRB < (B1 - tw) / 2.0 && dRB < (B2 - tw) / 2.0 && dRH * 2 < H - tf1 - tf2 && dRH > 0.0 && dRB > 0.0 && dRtw > 0.0 && dRtf > 0.0)
		{
			CCMPolygon* pPolygon[4];
			pPolygon[0] = new CCMPolygon(CCM_BORDER, CCM_GRAY);
			pPolygon[1] = new CCMPolygon(CCM_BORDER, CCM_GRAY);
			pPolygon[2] = new CCMPolygon(CCM_BORDER, CCM_GRAY);
			pPolygon[3] = new CCMPolygon(CCM_BORDER, CCM_GRAY);

			pPolygon[0]->Add(SCMPoint2D((B1 - tw) / 2.0 - dRB, tf1));
			pPolygon[0]->Add(SCMPoint2D((B1 - tw) / 2.0 - dRB, tf1 + dRH));
			pPolygon[0]->Add(SCMPoint2D((B1 - tw) / 2.0, tf1 + dRH));
			pPolygon[0]->Add(SCMPoint2D((B1 - tw) / 2.0, tf1 + dRH - dRtf));
			pPolygon[0]->Add(SCMPoint2D((B1 - tw) / 2.0 - dRB + dRtw, tf1 + dRH - dRtf));
			pPolygon[0]->Add(SCMPoint2D((B1 - tw) / 2.0 - dRB + dRtw, tf1));

			pPolygon[1]->Add(SCMPoint2D((B1 + tw) / 2.0 + dRB, tf1));
			pPolygon[1]->Add(SCMPoint2D((B1 + tw) / 2.0 + dRB - dRtw, tf1));
			pPolygon[1]->Add(SCMPoint2D((B1 + tw) / 2.0 + dRB - dRtw, tf1 + dRH - dRtf));
			pPolygon[1]->Add(SCMPoint2D((B1 + tw) / 2.0, tf1 + dRH - dRtf));
			pPolygon[1]->Add(SCMPoint2D((B1 + tw) / 2.0, tf1 + dRH));
			pPolygon[1]->Add(SCMPoint2D((B1 + tw) / 2.0 + dRB, tf1 + dRH));

			pPolygon[2]->Add(SCMPoint2D((B1 - tw) / 2.0 - dRB, H - tf2 - dRH));
			pPolygon[2]->Add(SCMPoint2D((B1 - tw) / 2.0 - dRB, H - tf2));
			pPolygon[2]->Add(SCMPoint2D((B1 - tw) / 2.0 - dRB + dRtw, H - tf2));
			pPolygon[2]->Add(SCMPoint2D((B1 - tw) / 2.0 - dRB + dRtw, H - tf2 - dRH + dRtf));
			pPolygon[2]->Add(SCMPoint2D((B1 - tw) / 2.0, H - tf2 - dRH + dRtf));
			pPolygon[2]->Add(SCMPoint2D((B1 - tw) / 2.0, H - tf2 - dRH));

			pPolygon[3]->Add(SCMPoint2D((B2 + tw) / 2.0 + dRB, H - tf2 - dRH));
			pPolygon[3]->Add(SCMPoint2D((B2 + tw) / 2.0, H - tf2 - dRH));
			pPolygon[3]->Add(SCMPoint2D((B2 + tw) / 2.0, H - tf2 - dRH + dRtf));
			pPolygon[3]->Add(SCMPoint2D((B2 + tw) / 2.0 + dRB - dRtw, H - tf2 - dRH + dRtf));
			pPolygon[3]->Add(SCMPoint2D((B2 + tw) / 2.0 + dRB - dRtw, H - tf2));
			pPolygon[3]->Add(SCMPoint2D((B2 + tw) / 2.0 + dRB, H - tf2));

			for(int i = 0;i < 4;i++)
				aGeom.Add(pPolygon[i]);
		}
	}
	else if(m_pReinforceSect->nReinforceType == 3)
	{
		double dRT1 = m_pReinforceSect->Sect[0];
		double dRB1 = m_pReinforceSect->Sect[1];
		double dRT2 = m_pReinforceSect->Sect[2];
		double dRB2 = m_pReinforceSect->Sect[3];
		if(dRT1 > 0.0 && dRB1 > 0.0)
		{
			CCMPolygon* pPolygon2 = new CCMPolygon(CCM_BORDER, CCM_GRAY);
			pPolygon2->Add(SCMPoint2D(-dRT1, (tf1 - dRB1) / 2.0));
			pPolygon2->Add(SCMPoint2D(-dRT1, (tf1 + dRB1) / 2.0));
			pPolygon2->Add(SCMPoint2D(0.0, (tf1 + dRB1) / 2.0));
			pPolygon2->Add(SCMPoint2D(0.0, (tf1 - dRB1) / 2.0));
			pPolygon2->Transfer(SCMPoint2D(-mbr.min.x, -mbr.min.y));
			aGeom.Add(pPolygon2);

			CCMPolygon* pPolygon3 = new CCMPolygon(CCM_BORDER, CCM_GRAY);
			pPolygon3->Add(SCMPoint2D(B1 - dRT1, (tf1 - dRB1) / 2.0));
			pPolygon3->Add(SCMPoint2D(B1 - dRT1, (tf1 + dRB1) / 2.0));
			pPolygon3->Add(SCMPoint2D(B1, (tf1 + dRB1) / 2.0));
			pPolygon3->Add(SCMPoint2D(B1, (tf1 - dRB1) / 2.0));
			pPolygon3->Transfer(SCMPoint2D(-mbr.min.x, -mbr.min.y));
			aGeom.Add(pPolygon3);
		}
		if(dRT2 > 0.0 && dRB2 > 0.0)
		{
			CCMPolygon* pPolygon[2];
			for(int i = 0; i < 2; i++)
			{
				pPolygon[i] = new CCMPolygon(CCM_BORDER, CCM_GRAY);
				pPolygon[i]->Add(SCMPoint2D(0.0, 0.0));
				pPolygon[i]->Add(SCMPoint2D(0.0, dRB2));
				pPolygon[i]->Add(SCMPoint2D(dRT2, dRB2));
				pPolygon[i]->Add(SCMPoint2D(dRT2, 0.0));
			}

			pPolygon[0]->Transfer(SCMPoint2D((B1 - B2) / 2.0 - dRT2, H - tf2 - (dRB2 - tf2) / 2.0));
			pPolygon[1]->Transfer(SCMPoint2D((B1 + B2) / 2.0 - dRT2, H - tf2 - (dRB2 - tf2) / 2.0));
			aGeom.Add(pPolygon[0]);
			aGeom.Add(pPolygon[1]);
		}
	}
	else if(m_pReinforceSect->nReinforceType == 4)
	{
		double dRH1 = m_pReinforceSect->Sect[0];
		double dRB1 = m_pReinforceSect->Sect[1];
		double dRH2 = m_pReinforceSect->Sect[2];
		double dRB2 = m_pReinforceSect->Sect[3];
		double dRC1 = m_pReinforceSect->Sect[4];
		double dRC2 = m_pReinforceSect->Sect[5];
		auto calculateDisplacements = [](double b1, double b2, double b3,double& dlambdaX,double& dlambdaY)
		{
			double b4 = std::sqrt(b2 * b2 - b1 * b1);
			dlambdaX = (b1 * b3) / b2;
			dlambdaY = (b4 * b3) / b2;
		};

		double db1 = (B1 - tw) / 2.0 - dRC1;
		double db3 = (B2 - tw) / 2.0 - dRC2;
		double dlambdaX1, dlambdaY1, dlambdaX2, dlambdaY2;

		if(dRC1 < (B1 - tw) / 2.0 && dRH1 > db1)
		{
			calculateDisplacements(db1, dRH1, dRB1, dlambdaX1, dlambdaY1);
			CCMPolygon* pPolygon[2];
			for(int i = 0; i < 2; i++)
			{
				pPolygon[i] = new CCMPolygon(CCM_BORDER, CCM_GRAY);
			}
			double db2 = std::sqrt(dRH1 * dRH1 - db1 * db1);
			pPolygon[0]->Add(SCMPoint2D(0.0, 0.0));
			pPolygon[0]->Add(SCMPoint2D((B1 - tw) / 2.0 - dRC1, db2));
			pPolygon[0]->Add(SCMPoint2D((B1 - tw) / 2.0 - dRC1, db2 - dlambdaY1));
			pPolygon[0]->Add(SCMPoint2D(dlambdaX1, 0.0));
			pPolygon[0]->Transfer(SCMPoint2D(dRC1, tf1));
			
			pPolygon[1]->Add(SCMPoint2D(0.0, 0.0));
			pPolygon[1]->Add(SCMPoint2D(-dlambdaX1, 0.0));
			pPolygon[1]->Add(SCMPoint2D((tw - B1) / 2.0 + dRC1, db2 - dlambdaY1));
			pPolygon[1]->Add(SCMPoint2D((tw - B1) / 2.0 + dRC1, db2));
			pPolygon[1]->Transfer(SCMPoint2D(B1 - dRC1, tf1));
			aGeom.Add(pPolygon[0]);
			aGeom.Add(pPolygon[1]);
		}
		if(dRC2 < (B2 - tw) / 2.0 && dRH2 > db3)
		{
			calculateDisplacements(db3, dRH2, dRB2, dlambdaX2, dlambdaY2);
			CCMPolygon* pPolygon[2];
			for(int i = 0; i < 2; i++)
			{
				pPolygon[i] = new CCMPolygon(CCM_BORDER, CCM_GRAY);
			}
			double db4 = std::sqrt(dRH2 * dRH2 - db3 * db3);
			pPolygon[0]->Add(SCMPoint2D(0.0, 0.0));
			pPolygon[0]->Add(SCMPoint2D(dlambdaX2, 0.0));
			pPolygon[0]->Add(SCMPoint2D((B2 - tw) / 2.0 - dRC2, dlambdaY2 - db4));
			pPolygon[0]->Add(SCMPoint2D((B2 - tw) / 2.0 - dRC2, -db4));
			pPolygon[0]->Transfer(SCMPoint2D((B1 - B2) / 2.0 + dRC2, H - tf2));

			pPolygon[1]->Add(SCMPoint2D(0.0, 0.0));
			pPolygon[1]->Add(SCMPoint2D((tw - B2) / 2.0 + dRC2, -db4));
			pPolygon[1]->Add(SCMPoint2D((tw - B2) / 2.0 + dRC2, dlambdaY2 - db4));
			pPolygon[1]->Add(SCMPoint2D(-dlambdaX2, 0.0));
			pPolygon[1]->Transfer(SCMPoint2D((B1 + B2) / 2.0 - dRC2, H - tf2));
			aGeom.Add(pPolygon[0]);
			aGeom.Add(pPolygon[1]);
		}
	}
	else if(m_pReinforceSect->nReinforceType == 5)
	{
		double dRH1 = m_pReinforceSect->Sect[0];
		double dRT1 = m_pReinforceSect->Sect[1];
		double dRH2 = m_pReinforceSect->Sect[2];
		double dRT2 = m_pReinforceSect->Sect[3];
		double dRC1 = m_pReinforceSect->Sect[4];
		double dRC2 = m_pReinforceSect->Sect[5];

		if(dRC1 > dRT1 + tw && dRC1 < B1 && dRH1 > 0.0)
		{
			CCMPolygon* pPolygon[2];
			for(int i = 0; i < 2; i++)
			{
				pPolygon[i] = new CCMPolygon(CCM_BORDER, CCM_GRAY);
				pPolygon[i]->Add(SCMPoint2D(0.0, 0.0));
				pPolygon[i]->Add(SCMPoint2D(0.0, dRH1));
				pPolygon[i]->Add(SCMPoint2D(dRT1, dRH1));
				pPolygon[i]->Add(SCMPoint2D(dRT1, 0.0));
			}
			pPolygon[0]->Transfer(SCMPoint2D((B1 - dRC1 - dRT1) / 2.0, tf1));
			pPolygon[1]->Transfer(SCMPoint2D((B1 + dRC1 - dRT1) / 2.0, tf1));
			aGeom.Add(pPolygon[0]);
			aGeom.Add(pPolygon[1]);
		}

		if(dRC2 > dRT2 + tw && dRC2 < B2 && dRH2 > 0.0)
		{
			CCMPolygon* pPolygon[2];
			for(int i = 0; i < 2; i++)
			{
				pPolygon[i] = new CCMPolygon(CCM_BORDER, CCM_GRAY);
				pPolygon[i]->Add(SCMPoint2D(0.0, 0.0));
				pPolygon[i]->Add(SCMPoint2D(0.0, dRH2));
				pPolygon[i]->Add(SCMPoint2D(dRT2, dRH2));
				pPolygon[i]->Add(SCMPoint2D(dRT2, 0.0));
			}
			pPolygon[0]->Transfer(SCMPoint2D((B1 - dRC2 - dRT2) / 2.0, H - tf2 - dRH2));
			pPolygon[1]->Transfer(SCMPoint2D((B1 + dRC2 - dRT2) / 2.0, H - tf2 - dRH2));
			aGeom.Add(pPolygon[0]);
			aGeom.Add(pPolygon[1]);
		}
	}
	else if(m_pReinforceSect->nReinforceType == 6)
	{
		double dRB = m_pReinforceSect->Sect[0];
		double dRtf = m_pReinforceSect->Sect[1];
		double dRH = m_pReinforceSect->Sect[2];
		double dRtw = m_pReinforceSect->Sect[3];
		if(dRB > 0.0 && dRtf > 0.0 && dRH > 0.0 && dRtw > 0.0)
		{
			CCMPolygon* pPolygon2 = new CCMPolygon(CCM_BORDER, CCM_GRAY);
			pPolygon2->Add(SCMPoint2D(0.0, 0.0));
			pPolygon2->Add(SCMPoint2D(0.0, dRH - dRtf));
			pPolygon2->Add(SCMPoint2D((dRtw - dRB)/2.0, dRH - dRtf));
			pPolygon2->Add(SCMPoint2D((dRtw - dRB)/2.0, dRH));
			pPolygon2->Add(SCMPoint2D((dRtw + dRB)/2.0, dRH));
			pPolygon2->Add(SCMPoint2D((dRtw + dRB) / 2.0, dRH - dRtf));
			pPolygon2->Add(SCMPoint2D(dRtw, dRH - dRtf));
			pPolygon2->Add(SCMPoint2D(dRtw, 0.0));
			pPolygon2->Transfer(SCMPoint2D((B1 - tw + dRtw) / 2.0, H));
			aGeom.Add(pPolygon2);
		}
	}
}

void CRFSecViewWnd::MakePicture(T_SECT_D* pDataSrc, CCMGeom2DArray& aGeom)
{
	if(pDataSrc->nStype == D_SECT_TYPE_REGULAR && m_pReinforceSect->nReinforceType > 0 && m_pReinforceSect->nReinforceType < 7)
	{
		CString SectionShape = pDataSrc->SectBefore.Shape;
		if(CSectUtil::GetShapeIndexFromNameReg(SectionShape) == 2)
		{
			MakeReinforcementHBeam(aGeom);
			return;
		}
	}
	BOOL bAfter = FALSE;
	BOOL bReverse = FALSE;
	BOOL bCentroid = TRUE;
	BOOL bOffset = FALSE;
	BOOL bShearCheck = FALSE;
	T_RPSC_D* pDataSrcRebar = NULL;
	BOOL bEndJ = FALSE;
	int nRebarNum;
	m_GeomMaker.MakePicture(pDataSrc, aGeom, bAfter, bReverse, bCentroid, bOffset, bShearCheck, pDataSrcRebar, bEndJ, &nRebarNum);
}

//Draw
void CRFSecViewWnd::EraseBkgnd(CDC* pDC)
{
	if(m_DrawArea.left >= m_DrawArea.right) return;
	if(m_DrawArea.top >= m_DrawArea.bottom) return;

	int nSavedDC = pDC->SaveDC();

	CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
	CBrush backBrush(RGB(0, 0, 0));
	pDC->SelectObject(&pen);
	pDC->SelectObject(&backBrush);

	CRect rect;
	rect = m_DrawArea;
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);

	BOOL bDrawGrid = FALSE;
	if(!bDrawGrid) { pDC->RestoreDC(nSavedDC); return; }

	//	Grid Point를 그린다.
	COLORREF color = CCM_WHITE;
	int XIndex, YIndex;
	double CenX, CenY, Space = 15., X, Y;

	CenX = rect.right / 2.;		CenY = rect.bottom / 2.;
	XIndex = (int)((rect.right / Space) / 2);	YIndex = (int)((rect.bottom / Space) / 2);

	// 1, 4 사분면
	for(int i = 0; i < XIndex + 1; i++)
	{
		for(int j = 0; j < YIndex + 1; j++)
		{
			X = CenX + Space * i;	Y = CenY + Space * j;
			pDC->SetPixel((int)X, (int)Y, color);
		}
		for(int j = 0; j < YIndex + 1; j++)
		{
			X = CenX + Space * i;	Y = CenY - Space * j;
			pDC->SetPixel((int)X, (int)Y, color);
		}
	}
	// 2, 3 사분면
	for(int i = 0; i < XIndex + 1; i++)
	{
		for(int j = 0; j < YIndex + 1; j++)
		{
			X = CenX - Space * i;	Y = CenY + Space * j;
			pDC->SetPixel((int)X, (int)Y, color);
		}
		for(int j = 0; j < YIndex + 1; j++)
		{
			X = CenX - Space * i;	Y = CenY - Space * j;
			pDC->SetPixel((int)X, (int)Y, color);
		}
	}
	pDC->RestoreDC(nSavedDC);
}

void CRFSecViewWnd::DrawSection(CDC* pDC, CCMGeom2DArray& aGeom, BOOL bCentroid, BOOL bOffset, BOOL bShearCheck, int nRebarNum, BOOL bChangeAxis)
{
	if(m_DrawArea.left >= m_DrawArea.right) return;
	if(m_DrawArea.top >= m_DrawArea.bottom) return;

	CCMGeom2D* pOffset, * pCentroid, * pStress[4], * pShear[3];
	CArray<CCMGeom2D*, CCMGeom2D*> apRebar;
	BOOL bExist = aGeom.GetSize() > 0;
	SCMRect2D mbr;
	if(bExist)
	{
		if(bOffset)
		{
			pOffset = aGeom.GetAt(aGeom.GetSize() - 1);
			aGeom.RemoveAt(aGeom.GetSize() - 1);
		}
		if(bCentroid)
		{
			for(int i = 0; i < 4; i++)
			{
				pStress[i] = aGeom.GetAt(aGeom.GetSize() - 1);
				aGeom.RemoveAt(aGeom.GetSize() - 1);
			}
		}
		if(bCentroid)
		{
			pCentroid = aGeom.GetAt(aGeom.GetSize() - 1);
			aGeom.RemoveAt(aGeom.GetSize() - 1);
		}
		if(bShearCheck)
		{
			for(int i = 0; i < 3; i++)
			{
				pShear[i] = aGeom.GetAt(aGeom.GetSize() - 1);
				aGeom.RemoveAt(aGeom.GetSize() - 1);
			}
		}
		// mbr 구할 때 Rebar는 빼고 구한다.
		for(int i = 0; i < nRebarNum; i++)
		{
			apRebar.Add(aGeom.GetAt(aGeom.GetSize() - 1));
			aGeom.RemoveAt(aGeom.GetSize() - 1);
		}
		mbr = aGeom.GetMBR();
	}

	CPoint org, offset;
	double scale;

	CRect rect;
	rect = m_DrawArea;
	CalcScaleOrgOff(rect, aGeom, scale, org, offset);

	aGeom.Draw(pDC, rect, scale, offset);

	// Rebar를 그린다.
	if(nRebarNum > 0)
	{
		CPoint newOffset = offset;
		newOffset.x += (long)((mbr.max.x - mbr.min.x) / 2.0 * scale);
		newOffset.y += (long)((mbr.max.y - mbr.min.y) / 2.0 * scale);
		for(int i = 0; i < nRebarNum; i++)
		{
			apRebar[i]->Transfer(SCMPoint2D(-(mbr.max.x + mbr.min.x) / 2.0, -(mbr.max.y + mbr.min.y) / 2.0));
			apRebar[i]->Draw(pDC, rect, scale, newOffset, apRebar[i]->PenColor, apRebar[i]->BrushColor);
			aGeom.Add(apRebar[i]);  // aGeom이 삭제될 때 pRebar도 함께 삭제되도록 다시 add한다.
		}
	}

	// offset point를 그린다.
	if(bExist)
	{
		// offset point를 그린다.
		if(bOffset)
		{
			pOffset->Transfer(SCMPoint2D(-(mbr.max.x + mbr.min.x) / 2.0, -(mbr.max.y + mbr.min.y) / 2.0));
			CPoint newOffset = offset;
			newOffset.x += (long)((mbr.max.x - mbr.min.x) / 2.0 * scale);
			newOffset.y += (long)((mbr.max.y - mbr.min.y) / 2.0 * scale);
			pOffset->Draw(pDC, rect, scale, newOffset, pOffset->PenColor, pOffset->BrushColor);
			aGeom.Add(pOffset); // aGeom이 삭제될 때 pOffset도 함께 삭제되도록 다시 add한다.
		}
		// Centroid를 그린다.
		if(bCentroid)
		{
			pCentroid->Transfer(SCMPoint2D(-(mbr.max.x + mbr.min.x) / 2.0, -(mbr.max.y + mbr.min.y) / 2.0));
			CPoint newCentroid = offset;
			newCentroid.x += (long)((mbr.max.x - mbr.min.x) / 2.0 * scale);
			newCentroid.y += (long)((mbr.max.y - mbr.min.y) / 2.0 * scale);
			pCentroid->Draw(pDC, rect, scale, newCentroid, pCentroid->PenColor, pCentroid->BrushColor);
			aGeom.Add(pCentroid); // aGeom이 삭제될 때 pCentroid 함께 삭제되도록 다시 add한다.
			// 응력 위치 출력 
			CString csLabel;
			double x, y;
			SCMRect2D textmbr;
			CPen penStressPt(PS_SOLID, 1, RGB(255, 0, 0));
			for(int i = 0; i < 4; i++)
			{
				if(pStress[i])
				{
					pStress[i]->Transfer(SCMPoint2D(-(mbr.max.x + mbr.min.x) / 2.0, -(mbr.max.y + mbr.min.y) / 2.0));
					CPoint newStress = offset;
					newStress.x += (long)((mbr.max.x - mbr.min.x) / 2.0 * scale);
					newStress.y += (long)((mbr.max.y - mbr.min.y) / 2.0 * scale);
					pStress[i]->Draw(pDC, rect, scale, newStress, pStress[i]->PenColor, pStress[i]->BrushColor);
					aGeom.Add(pStress[i]); // aGeom이 삭제될 때 pStress 함께 삭제되도록 다시 add한다.
					// 번호 출력
					textmbr = pStress[i]->GetMBR();
					x = (textmbr.min.x + textmbr.max.x) / 2.0;
					y = (textmbr.min.y + textmbr.max.y) / 2.0;
					x = RoundUp(x * scale) + (int)newStress.x;
					y = RoundUp(y * scale) + (int)newStress.y;
					if(i == 2 || i == 3) y -= 15;
					if(i == 0 || i == 3) x -= 10;
					int nSavedDC = pDC->SaveDC();
					pDC->SelectObject(&penStressPt);
					pDC->SetBkMode(TRANSPARENT);
					pDC->SetTextColor(RGB(255, 0, 0));
					csLabel.Format(_T("%d"), 4 - i);
					pDC->TextOut(x, y, csLabel);
					pDC->RestoreDC(nSavedDC);
				}
			}
		}
		// 전단 검토 위치를 그린다
		if(bShearCheck)
		{
			CString csLabel;
			double x, y;
			SCMRect2D textmbr;
			CPen penShear(PS_SOLID, 1, CCM_CYAN);
			for(int i = 0; i < 3; i++)
			{
				aGeom.Add(pShear[i]); // aGeom이 삭제될 때 pShear 함께 삭제되도록 다시 add한다.
				if(pShear[i] && pShear[i]->BrushColor != CCM_BLACK)
				{
					pShear[i]->Transfer(SCMPoint2D(-(mbr.max.x + mbr.min.x) / 2.0, -(mbr.max.y + mbr.min.y) / 2.0));
					CPoint newShear = offset;
					newShear.x += (long)((mbr.max.x - mbr.min.x) / 2.0 * scale);
					newShear.y += (long)((mbr.max.y - mbr.min.y) / 2.0 * scale);
					pShear[i]->Draw(pDC, rect, scale, newShear, pShear[i]->PenColor, pShear[i]->BrushColor);
					// Label 출력
					textmbr = pShear[i]->GetMBR();
					x = textmbr.max.x;
					y = textmbr.max.y;
					x = RoundUp(x * scale) + (int)newShear.x;
					y = RoundUp(y * scale) + (int)newShear.y;
					x += 5;
					y -= 8;
					int nSavedDC = pDC->SaveDC();
					pDC->SelectObject(&penShear);
					pDC->SetBkMode(TRANSPARENT);
					pDC->SetTextColor(CCM_CYAN);
					csLabel.Format(_T("Z%d"), 3 - i);
					pDC->TextOut(x, y, csLabel);
					pDC->RestoreDC(nSavedDC);
				}
			}
		}
	}
	// 축을 그린다.
	DrawAxis(pDC, org, bChangeAxis);
}

void CRFSecViewWnd::CalcScaleOrgOff(CRect& rect, CCMGeom2DArray& aGeom, double& scale, CPoint& org, CPoint& offset)
{
	SCMRect2D mbr = aGeom.GetMBR();

	// calculate scale
	double xRatio, yRatio;
	if(mbr.Width() == 0.0) xRatio = 0.0; //xRatio = DBL_MAX;
	else xRatio = ((double)(rect.Width())) / mbr.Width() * 0.7;
	if(mbr.Height() == 0.0) yRatio = 0.0; //yRatio = DBL_MAX;
	else yRatio = ((double)(rect.Height())) / mbr.Height() * 0.7;
	scale = min(xRatio, yRatio);
	if(scale == 0.0) scale = max(xRatio, yRatio);

	// calculate origin's screen coordinate
	SCMPoint2D dOff;

	dOff.x = (mbr.Width() * scale / 2.0);
	dOff.y = (mbr.Height() * scale / 2.0);

	offset.x = rect.Width() / 2 - (int)dOff.x;
	offset.y = rect.Height() / 2 - (int)dOff.y;

	org.x = rect.Width() / 2; // - RoundUp(mbr.Width()/2.0*scale);
	org.y = rect.Height() / 2; // + RoundUp(mbr.Height()/2.0*scale);
}

void CRFSecViewWnd::DrawAxis(CDC* pDC, CPoint& org, BOOL bChangeAxis)
{
	CPen pen(PS_SOLID, 1, RGB(0, 0, 255));
	int nSavedDC = pDC->SaveDC();

	pDC->SelectObject(&pen);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0, 0, 255));

	pDC->Ellipse(org.x - 2, org.y - 2, org.x + 2, org.y + 2);

	pDC->MoveTo(org.x, org.y);
	pDC->LineTo(org.x, org.y - 15);
	pDC->MoveTo(org.x, org.y - 20);
	pDC->LineTo(org.x - 2, org.y - 15);
	pDC->MoveTo(org.x - 2, org.y - 15);
	pDC->LineTo(org.x + 2, org.y - 15);
	pDC->MoveTo(org.x + 2, org.y - 15);
	pDC->LineTo(org.x, org.y - 20);
	if(bChangeAxis) pDC->TextOut(org.x - 4, org.y - 36, _T("y"), 1);
	else            pDC->TextOut(org.x - 4, org.y - 36, _T("z"), 1);

	pDC->MoveTo(org.x, org.y);
	pDC->LineTo(org.x + 15, org.y);
	pDC->MoveTo(org.x + 20, org.y);
	pDC->LineTo(org.x + 15, org.y - 2);
	pDC->MoveTo(org.x + 20, org.y);
	pDC->LineTo(org.x + 15, org.y + 2);
	pDC->MoveTo(org.x + 15, org.y - 2);
	pDC->LineTo(org.x + 15, org.y + 2);
	if(bChangeAxis) pDC->TextOut(org.x + 25, org.y - 8, _T("x"), 1);
	else            pDC->TextOut(org.x + 25, org.y - 8, _T("y"), 1);

	pDC->RestoreDC(nSavedDC);
}

int CRFSecViewWnd::RoundUp(double dVal)
{
	if(dVal > 0.0)
		return (int)floor(dVal + (double)0.5);
	else
		return (int)ceil(dVal - (double)0.5);
}
