//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
// DgnSectionDrawFunc.cpp
// 2002. 5. 8
// For Drawing Section  by TAE
//////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSectionDrawFunc.h"

#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\MathFunc.h"
#include "..\wg_db\MatlDB.h"


#include "DgnDataCtrl.h"
#include "DgnSectionTool.h"
#include "DgnDrawSection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define   DGNDRAW_UFAC    1000   
#define   ZOOM_FACTOR     0.65
#define   DGN_RBAR_SIZE   4
#define   DGN_RBAR_LAYER  5
#define   DGN_RBAR_LAYER_BEAM  10// Seungjun-20140519 : Russia rebar layer 5->10
/////////////////////////////////////////////////////////////////////////////
// CDgnSectionDrawFunc

CDgnSectionDrawFunc::CDgnSectionDrawFunc()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dAngle = 0.0;
	m_nDimSize = 12;
	m_nRebarSize = DGN_RBAR_SIZE;
	m_nChkPointSize = 4;
	m_nLineSize = 1;
	m_dZoomFactor = 0.65;
}

CDgnSectionDrawFunc::~CDgnSectionDrawFunc()
{
}


BEGIN_MESSAGE_MAP(CDgnSectionDrawFunc, CWnd)
	//{{AFX_MSG_MAP(CDgnSectionDrawFunc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Draw_Section
//

//--------------------------------------------------------------------------
//  Set_Canvas
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Set_Canvas(CRect Canvas, int CanvasType, int AxisType, int DimSize, int RebarSize, int LengthUnit, int WidthLengthUnit, int DimType, int DimShape, double ZoomFactor, int ChkPointSize, int nLineSize)
{
	m_Canvas.bottom = Canvas.bottom;// * DGNDRAW_UFAC;
	m_Canvas.top = Canvas.top;// * DGNDRAW_UFAC;
	m_Canvas.right = Canvas.right;// * DGNDRAW_UFAC;
	m_Canvas.left = Canvas.left;// * DGNDRAW_UFAC;
	m_ScaleF = 0;

	m_nCanvasType = CanvasType;
	m_nAxisType = AxisType;
	m_nDimType = DimType;
	m_nDimShape = DimShape;
	m_nDimSize = DimSize;
	m_nRebarSize = RebarSize;
	m_nLineSize = nLineSize;
	m_nChkPointSize = ChkPointSize;
	m_nLengthUnit = LengthUnit;
	m_nWidthLengthUnit = WidthLengthUnit;
	m_dZoomFactor = ZoomFactor;
}

//--------------------------------------------------------------------------
//  Draw (Beam, Column)
//--------------------------------------------------------------------------

void CDgnSectionDrawFunc::Draw(CDC* pDC, CRect Canvas, T_ELEM_K ElemK, int nIMJ, double RotAngle, int CanvasType, int AxisType, int RebarSize, BOOL bShape, BOOL bRebar, int DimSize, BOOL bDimShape, BOOL bDimRebar, BOOL bFillShape, BOOL bFillRebar, int LengthUnit, int WidthLengthUnit, int nRebarDataType, int nTypeRebarView)
{
	Set_Canvas(Canvas, CanvasType, AxisType, DimSize, RebarSize, LengthUnit, WidthLengthUnit);
	Draw_Section(pDC, ElemK, nIMJ, RotAngle, bShape, bRebar, bDimShape, bDimRebar, bFillShape, bFillRebar, nRebarDataType, nTypeRebarView);
}

void CDgnSectionDrawFunc::Draw_Section(CDC* pDC, T_ELEM_K ElemK, int nIMJ, double RotAngle, BOOL bShape, BOOL bRebar, BOOL bDimShape, BOOL bDimRebar, BOOL bFillShape, BOOL bFillRebar, int nRebarDataType, int nTypeRebarView)
{
	T_MBTP_D MbtpD; MbtpD.Initialize();
	BOOL bGetMbtp = m_pDoc->m_pAttrCtrl->GetMbtp(ElemK, MbtpD);
	int iMbtp = (bGetMbtp ? MbtpD.nMbType : m_pDoc->m_pAttrCtrl->GetMemberType(ElemK));

	if (iMbtp == 1) iMbtp = 1;      // 기둥일 경우
	else if (iMbtp == 2) iMbtp = 0;// 보일 경우
	else return;

	if (iMbtp == 1)
	{// 기둥일때
		Draw_Section_For_RcColumn(pDC, ElemK, nIMJ, RotAngle, bShape, bRebar, bDimShape, bDimRebar, bFillShape, bFillRebar);
	}
	else
	{// 보일 경우
		Draw_Section_For_RcBeam(pDC, ElemK, nIMJ, bShape, bRebar, bDimShape, bDimRebar, bFillShape, bFillRebar, nRebarDataType, nTypeRebarView);
	}
	/*
	T_ELEM_D ElemD;
	ElemD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD))	return;
	int SectK = ElemD.elpro;
	T_SECT_D SectD;
	SectD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_MBTP_D MbtpD; MbtpD.Initialize();
	BOOL bGetMbtp = m_pDoc->m_pAttrCtrl->GetMbtp(ElemK,MbtpD);
	int iMbtp = (bGetMbtp ? MbtpD.nMbType : m_pDoc->m_pAttrCtrl->GetMemberType(ElemK));

	if(iMbtp == 1) iMbtp = 1;      // 기둥일 경우
	else if( iMbtp == 2) iMbtp = 0;// 보일 경우
	else return;

	m_dAngle = RotAngle;

	if(bShape)
		Draw_Shape(pDC, iMbtp, SectK, bDimShape, bFillShape, nIMJ);

	if(bRebar)
	{
		if(iMbtp == 1)
		{// 기둥일때
			if(SectD.SectBefore.Shape != D_SECT_SHAPE_REG_GEN)
			{// 정형단면일경우
				T_CPCO_D CpcoD;   CpcoD.Initialize();
				if(!m_pDoc->m_pAttrCtrl->GetCpco(ElemK, CpcoD)) return;

				Draw_ChkRebar(pDC, SectK, nIMJ, &CpcoD, bDimRebar, bFillRebar);
			}
			else
			{
				T_RBTE_D RbteD;   RbteD.Initialize();
				CDgnSectionTool::GetRbte(ElemK, RbteD);
				Draw_ChkRebar_Colm_Gen(pDC, SectK, nIMJ, &RbteD, bDimRebar, bFillRebar);
			}
		}
		else
		{// 보일 경우
			T_CPBE_D CpbeD;    CpbeD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetCpbe(ElemK, CpbeD)) return;
			if(nRebarDataType == 1)
				CpbeD.bNum = TRUE;
			else if(nRebarDataType == 2)
				CpbeD.bNum = FALSE;

			Draw_ChkRebar(pDC, SectK, nIMJ, &CpbeD, bDimRebar, bFillRebar);
		}
	}
	*/
}

//--------------------------------------------------------------------------
//  Draw (Wall)
//--------------------------------------------------------------------------

void CDgnSectionDrawFunc::Draw_Wall(CDC* pDC, CRect Canvas, T_ELEM_K ElemK, int nIMJ, double RotAngle, int CanvasType, int AxisType, int RebarSize, BOOL bShape, BOOL bRebar, int DimSize, BOOL bDimShape, BOOL bDimRebar, BOOL bFillShape, BOOL bFillRebar, int LengthUnit, int WidthLengthUnit, int nRebarDataType, int nTypeRebarView)
{
	Set_Canvas(Canvas, CanvasType, AxisType, DimSize, RebarSize, LengthUnit, WidthLengthUnit);
	Draw_Section_Wall(pDC, ElemK, nIMJ, RotAngle, bShape, bRebar, bDimShape, bDimRebar, bFillShape, bFillRebar, nRebarDataType, nTypeRebarView);
}

void CDgnSectionDrawFunc::Draw_Section_Wall(CDC* pDC, T_ELEM_K ElemK, int nIMJ, double RotAngle, BOOL bShape, BOOL bRebar, BOOL bDimShape, BOOL bDimRebar, BOOL bFillShape, BOOL bFillRebar, int nRebarDataType, int nTypeRebarView)
{
	Draw_Section_For_RcWall(pDC, ElemK, nIMJ, bShape, bRebar, bDimShape, bDimRebar, bFillShape, bFillRebar, nRebarDataType, nTypeRebarView);
	/*
	T_ELEM_D ElemD;
	ElemD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD))	return;
	int SectK = ElemD.elpro;
	T_SECT_D SectD;
	SectD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_MBTP_D MbtpD; MbtpD.Initialize();
	BOOL bGetMbtp = m_pDoc->m_pAttrCtrl->GetMbtp(ElemK,MbtpD);
	int iMbtp = (bGetMbtp ? MbtpD.nMbType : m_pDoc->m_pAttrCtrl->GetMemberType(ElemK));
	T_RCWA_D RcwaD; RcwaD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetRcwa(ElemK, RcwaD)) return;
	double dHaunch = 0.0;
	if(nIMJ==0)      dHaunch = RcwaD.dHaunch_hi/3.0;
	else if(nIMJ==2) dHaunch = RcwaD.dHaunch_hj/3.0;


	T_CPWA_D CpwaD;   CpwaD.Initialize();
	BOOL bChkCpwa = m_pDoc->m_pAttrCtrl->GetCpwa(ElemK, CpwaD);
	if(nRebarDataType == 1)
		CpwaD.bNum = TRUE;
	else if(nRebarDataType == 2)
		CpwaD.bNum = FALSE;

	if(iMbtp != 1) return;      // 기둥멤버일 경우에만 Wall(Frame)임
	if(CpwaD.bUnitWidth)
		iMbtp = 4;
	else
		iMbtp = 41;

	m_dAngle = RotAngle;

	if(bShape)
		Draw_Shape(pDC, iMbtp, SectK, bDimShape, bFillShape, nIMJ, dHaunch);

	if(bRebar && bChkCpwa)
	{
		if(SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SB)
		{// 정형단면일경우
			Draw_ChkRebar(pDC, SectK, nIMJ, &CpwaD, bDimRebar, bFillRebar, dHaunch);
		}
	}
	*/
}

//--------------------------------------------------------------------------
//  Draw (Plate)
//--------------------------------------------------------------------------

void CDgnSectionDrawFunc::Draw_Rcpl(CDC* pDC, CRect Canvas, T_RCPL_K RcplK, double RotAngle, int CanvasType, int AxisType, int RebarSize, BOOL bShape, BOOL bRebar, int DimSize, BOOL bDimShape, BOOL bDimRebar, BOOL bFillShape, BOOL bFillRebar, int nTypeRebarView, int LengthUnit, int WidthLengthUnit, int nRebarDataType)
{
	Set_Canvas(Canvas, CanvasType, AxisType, DimSize, RebarSize, LengthUnit, WidthLengthUnit);
	Draw_Section_Rcpl(pDC, RcplK, RotAngle, bShape, bRebar, bDimShape, bDimRebar, bFillShape, bFillRebar, nTypeRebarView, nRebarDataType);
}

void CDgnSectionDrawFunc::Draw_Section_Rcpl(CDC* pDC, T_RCPL_K RcplK, double RotAngle, BOOL bShape, BOOL bRebar, BOOL bDimShape, BOOL bDimRebar, BOOL bFillShape, BOOL bFillRebar, int nTypeRebarView, int nRebarDataType)
{
	Draw_Section_For_RcPlate(pDC, RcplK, bShape, bRebar, bDimShape, bDimRebar, bFillShape, bFillRebar, nTypeRebarView, nRebarDataType);
	/*
	T_RCPL_D RcplD;
	RcplD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetRcpl(RcplK,RcplD))	return;

	int nElemSize = RcplD.arElemK.GetSize();
	int ThikK = -1;
	T_ELEM_D eData;
	for(int i=0 ; i<nElemSize ; i++)
	{
		eData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetElem(RcplD.arElemK.GetAt(i), eData))	ASSERT(0);
		if(i==0)
		{ ThikK = eData.elpro; }
		else
		{ if(ThikK != eData.elpro) return; }// Thikness Key가 다르면 종료
	}

	int iMbtp = 5; //Plate
	m_dAngle = RotAngle;

	if(bShape)
		Draw_Shape(pDC, iMbtp, ThikK, bDimShape, bFillShape, 0);

	if(bRebar)
	{
		_DGNC_CPPL_D CpplD;   CpplD.Initialize();
		if(!CDgnSectionTool::Get_DgncCpplData(RcplK, CpplD)) return;
		if(nRebarDataType == 1)
			CpplD.bNum = TRUE;
		else if(nRebarDataType == 2)
			CpplD.bNum = FALSE;

		Draw_ChkRebar(pDC, ThikK, &CpplD, bDimRebar, bFillRebar, nTypeRebarView);
	}
	*/
}

void CDgnSectionDrawFunc::Draw_Section_For_Steel(CDC* pDC, T_ELEM_K ElemK, int nIMJ, double RotAngle, BOOL bShape, BOOL bRebar, BOOL bDimShape, BOOL bDimRebar, BOOL bFillShape, BOOL bFillRebar, int nRebarDataType)
{
	T_ELEM_D ElemD;
	ElemD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))	return;
	int SectK = ElemD.elpro;
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	m_dAngle = RotAngle;

	if (bShape)
	{
		Draw_Section_4KR_STL(pDC, SectK, bDimShape, bFillShape, nIMJ);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CDgnSectionDrawFunc::Draw_Shape(CDC* pDC, int nType, int SectK, BOOL bDim, BOOL bFill, int iPosi, double dHaunch)
{
	ASSERT(pDC);

	if (nType == 0)      Draw_Beam(pDC, SectK, bDim, bFill, iPosi);
	else if (nType == 1)  Draw_Colm(pDC, SectK, bDim, bFill, iPosi);
	else if (nType == 41) Draw_Wall(pDC, SectK, bDim, bFill, iPosi, dHaunch, FALSE);
	else if (nType == 4)  Draw_Wall(pDC, SectK, bDim, bFill, iPosi, dHaunch, TRUE);
	else if (nType == 5)  Draw_Plate(pDC, SectK, bDim, bFill, iPosi);
}


//--------------------------------------------------------------------------
// Draw_Beam
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Beam(CDC* pDC, T_SECT_K SectK, BOOL bDim, BOOL bFill, int iPosi)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SB)                                      Draw_Beam_Rect(pDC, SectK, iPosi, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_T)                                  Draw_Beam_Tee(pDC, SectK, iPosi, bDim, bFill);
	else if ((SectD.nStype == D_SECT_TYPE_TAPERED || SectD.nStype == D_SECT_TYPE_PSC) && SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_MID) Draw_Beam_PSC_MID(pDC, SectK, iPosi, bDim, bFill);

}

//--------------------------------------------------------------------------
// Draw_Beam_Rect
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Beam_Rect(CDC* pDC, T_SECT_K SectK, int iPosi, BOOL bDim, BOOL bFill)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, iPosi);
	double dLen = Cng_UnitLen(m_nLengthUnit);

	double H = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double B = m_dB = SectBaseD.Size[1] * DGNDRAW_UFAC;
	double H_Org = SectBaseD.Size[0] * dLen;
	double B_Org = SectBaseD.Size[1] * dLen;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK = FALSE;
	if (bDim) bGetOK = CalcCanvasRange(B, H, -0.5, -0.3);
	else     bGetOK = CalcCanvasRange(B, H);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;  arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt;  arGPt.RemoveAll();

	pt.x = 0;  pt.y = 0;  arLPt.Add(pt);
	pt.x = B;  pt.y = 0;  arLPt.Add(pt);
	pt.x = B;  pt.y = H;  arLPt.Add(pt);
	pt.x = 0;  pt.y = H;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, bFill);

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll();
	arGPt.RemoveAll();

	pt.x = 0 - B / 10;  pt.y = H / 2;      arLPt.Add(pt);
	pt.x = B + B / 10;  pt.y = H / 2;      arLPt.Add(pt);
	pt.x = B / 2;       pt.y = 0 - H / 10;  arLPt.Add(pt);
	pt.x = B / 2;       pt.y = H + H / 10;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if (bDim == FALSE) return;

	CString str;
	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = 0;   pt.y = -0.3*H;  arLPt.Add(pt);
	pt.x = B;   pt.y = -0.3*H;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), B_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = -0.5*B;   pt.y = 0;  arLPt.Add(pt);
	pt.x = -0.5*B;   pt.y = H;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), H_Org);
	Draw_Dimension(pDC, arGPt, str);
}


//--------------------------------------------------------------------------
// Draw_Beam_Tee
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Beam_Tee(CDC* pDC, T_SECT_K SectK, int iPosi, BOOL bDim, BOOL bFill)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, iPosi);
	double dLen = Cng_UnitLen(m_nLengthUnit);

	double dHc = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dBc = m_dB = SectBaseD.Size[1] * DGNDRAW_UFAC;
	double dTw = SectBaseD.Size[2] * DGNDRAW_UFAC;
	double dTf = SectBaseD.Size[3] * DGNDRAW_UFAC;
	double dHc_Org = SectBaseD.Size[0] * dLen;
	double dBc_Org = SectBaseD.Size[1] * dLen;
	double dTw_Org = SectBaseD.Size[2] * dLen;
	double dTf_Org = SectBaseD.Size[3] * dLen;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK = FALSE;
	if (bDim) bGetOK = CalcCanvasRange(dBc, dHc, -0.5, -0.3, 0, 0.3);
	else     bGetOK = CalcCanvasRange(dBc, dHc);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;  arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt;  arGPt.RemoveAll();

	double dShiftY = SectBaseD.Design.YBar * DGNDRAW_UFAC;
	double dShiftZ = 0.0;

	pt.x = -dTw / 2. + dShiftY;  pt.y = 0.0 + dShiftZ;  arLPt.Add(pt);
	pt.x = dTw / 2. + dShiftY;  pt.y = 0.0 + dShiftZ;  arLPt.Add(pt);
	pt.x = dTw / 2. + dShiftY;  pt.y = dHc - dTf + dShiftZ;  arLPt.Add(pt);
	pt.x = dBc / 2. + dShiftY;  pt.y = dHc - dTf + dShiftZ;  arLPt.Add(pt);
	pt.x = dBc / 2. + dShiftY;  pt.y = dHc + dShiftZ;  arLPt.Add(pt);
	pt.x = -dBc / 2. + dShiftY;  pt.y = dHc + dShiftZ;  arLPt.Add(pt);
	pt.x = -dBc / 2. + dShiftY;  pt.y = dHc - dTf + dShiftZ;  arLPt.Add(pt);
	pt.x = -dTw / 2. + dShiftY;  pt.y = dHc - dTf + dShiftZ;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, bFill);

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll();
	arGPt.RemoveAll();

	pt.x = -dBc / 2 + dShiftY - dBc / 10;  pt.y = dHc / 2;               arLPt.Add(pt);
	pt.x = dBc / 2 + dShiftY + dBc / 10;  pt.y = dHc / 2;               arLPt.Add(pt);
	pt.x = dBc / 2;                 pt.y = 0.0 + dShiftZ - dHc / 10;  arLPt.Add(pt);
	pt.x = dBc / 2;                 pt.y = dHc + dShiftZ + dHc / 10;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if (bDim == FALSE) return;

	CString str;
	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = -dBc / 2. + dShiftY;  pt.y = dHc + dShiftZ + 0.3*dHc;  arLPt.Add(pt);
	pt.x = dBc / 2. + dShiftY;  pt.y = dHc + dShiftZ + 0.3*dHc;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dBc_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = (-dBc / 2. + dShiftY) - 0.5*dBc;  pt.y = 0.0 + dShiftZ;  arLPt.Add(pt);
	pt.x = (-dBc / 2. + dShiftY) - 0.5*dBc;  pt.y = dHc + dShiftZ;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dHc_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = (-dBc / 2. + dShiftY) - 0.2*dBc;  pt.y = dHc - dTf + dShiftZ;  arLPt.Add(pt);
	pt.x = (-dBc / 2. + dShiftY) - 0.2*dBc;  pt.y = dHc + dShiftZ;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dTf_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = -dTw / 2. + dShiftY;  pt.y = dShiftZ - 0.3*dHc;  arLPt.Add(pt);
	pt.x = dTw / 2. + dShiftY;  pt.y = dShiftZ - 0.3*dHc;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dTw_Org);
	Draw_Dimension(pDC, arGPt, str);
}

//--------------------------------------------------------------------------
// Draw_Beam_PSC_MID
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Beam_PSC_MID(CDC* pDC, T_SECT_K SectK, int iPosi, BOOL bDim, BOOL bFill)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, iPosi);
	double dLen = Cng_UnitLen(m_nLengthUnit);

	double dHL1 = SectBaseD.Size[1];
	double dHR1 = SectBaseD.Size[16];
	double dBL2 = SectBaseD.Size[11];
	double dBR2 = SectBaseD.Size[26];

	double H = (dHL1 + dHR1) / 2.0 * DGNDRAW_UFAC;
	double B = m_dB = (dBL2 + dBR2)     * DGNDRAW_UFAC;
	double H_Org = (dHL1 + dHR1) / 2.0 * dLen;
	double B_Org = (dBL2 + dBR2)     * dLen;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK = FALSE;
	if (bDim) bGetOK = CalcCanvasRange(B, H, -0.5, -0.3);
	else     bGetOK = CalcCanvasRange(B, H);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;  arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt;  arGPt.RemoveAll();

	pt.x = 0;  pt.y = 0;  arLPt.Add(pt);
	pt.x = B;  pt.y = 0;  arLPt.Add(pt);
	pt.x = B;  pt.y = H;  arLPt.Add(pt);
	pt.x = 0;  pt.y = H;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, bFill);

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll();
	arGPt.RemoveAll();

	pt.x = 0 - B / 10;  pt.y = H / 2;      arLPt.Add(pt);
	pt.x = B + B / 10;  pt.y = H / 2;      arLPt.Add(pt);
	pt.x = B / 2;       pt.y = 0 - H / 10;  arLPt.Add(pt);
	pt.x = B / 2;       pt.y = H + H / 10;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if (bDim == FALSE) return;

	CString str;
	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = 0;   pt.y = -0.3*H;  arLPt.Add(pt);
	pt.x = B;   pt.y = -0.3*H;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), B_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = -0.5*B;   pt.y = 0;  arLPt.Add(pt);
	pt.x = -0.5*B;   pt.y = H;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), H_Org);
	Draw_Dimension(pDC, arGPt, str);
}


//--------------------------------------------------------------------------
// Draw_Colm
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Colm(CDC* pDC, T_SECT_K SectK, BOOL bDim, BOOL bFill, int iPosi)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_B)         Draw_Colm_vRect(pDC, SectK, iPosi, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SB)   Draw_Colm_sRect(pDC, SectK, iPosi, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_P)    Draw_Colm_vCirc(pDC, SectK, iPosi, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SR)   Draw_Colm_sCirc(pDC, SectK, iPosi, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_OCT)  Draw_Colm_vOcta(pDC, SectK, iPosi, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SOCT) Draw_Colm_sOcta(pDC, SectK, iPosi, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_TRK)  Draw_Colm_vTrck(pDC, SectK, iPosi, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_STRK) Draw_Colm_sTrck(pDC, SectK, iPosi, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_HTRK) Draw_Colm_sHTrk(pDC, SectK, iPosi, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_GEN)  Draw_Colm_Gen(pDC, SectK, iPosi, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_H)		 Draw_Colm_H(pDC, SectK, iPosi, bDim, bFill);

}

void CDgnSectionDrawFunc::Draw_Section_4KR_STL(CDC* pDC, T_SECT_K SectK, BOOL bDim, BOOL bFill, int iPosi)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_L)    Draw_Steel_L(pDC, SectK, iPosi, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_C)    Draw_Steel_C(pDC, SectK, iPosi, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_T)    Draw_Beam_Tee(pDC, SectK, iPosi, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_H)		 Draw_Colm_H(pDC, SectK, iPosi, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_B)    Draw_Colm_vRect(pDC, SectK, iPosi, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_P)    Draw_Colm_vCirc(pDC, SectK, iPosi, bDim, bFill);

	//else if(SectD.SectBefore.Shape == D_SECT_SHAPE_REG_GEN)  Draw_Colm_Gen  (pDC, SectK, iPosi, bDim, bFill);

}


//--------------------------------------------------------------------------
// Draw_Colm_sOcta
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Colm_sOcta(CDC* pDC, T_SECT_K SectK, int iPosi, BOOL bDim, BOOL bFill)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, iPosi);
	double dLen = Cng_UnitLen(m_nLengthUnit);


	double dH = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dB = m_dB = SectBaseD.Size[1] * DGNDRAW_UFAC;
	double da = SectBaseD.Size[2] * DGNDRAW_UFAC;
	double db = SectBaseD.Size[3] * DGNDRAW_UFAC;
	double dH_Org = SectBaseD.Size[0] * dLen;
	double dB_Org = SectBaseD.Size[1] * dLen;
	double da_Org = SectBaseD.Size[2] * dLen;
	double db_Org = SectBaseD.Size[3] * dLen;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK = FALSE;




	if (bDim) bGetOK = CalcCanvasRange(dB, dH, -0.5, -0.5);
	else     bGetOK = CalcCanvasRange(dB, dH);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;  arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt;  arGPt.RemoveAll();

	double dShiftY = SectBaseD.Design.YBar * DGNDRAW_UFAC;
	double dShiftZ = 0.0;

	pt.x = -dB / 2. + da + dShiftY;  pt.y = 0.0 + dShiftZ;  arLPt.Add(pt);
	pt.x = dB / 2. - da + dShiftY;  pt.y = 0.0 + dShiftZ;  arLPt.Add(pt);
	pt.x = dB / 2. + dShiftY;  pt.y = db + dShiftZ;  arLPt.Add(pt);
	pt.x = dB / 2. + dShiftY;  pt.y = dH - db + dShiftZ;  arLPt.Add(pt);
	pt.x = dB / 2. - da + dShiftY;  pt.y = dH + dShiftZ;  arLPt.Add(pt);
	pt.x = -dB / 2. + da + dShiftY;  pt.y = dH + dShiftZ;  arLPt.Add(pt);
	pt.x = -dB / 2. + dShiftY;  pt.y = dH - db + dShiftZ;  arLPt.Add(pt);
	pt.x = -dB / 2. + dShiftY;  pt.y = db + dShiftZ;  arLPt.Add(pt);




	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, bFill);

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll();  arGPt.RemoveAll();

	pt.x = -dB / 2 + dShiftY - dB / 9;  pt.y = dH / 2;               arLPt.Add(pt);
	pt.x = dB / 2 + dShiftY + dB / 9;  pt.y = dH / 2;               arLPt.Add(pt);
	pt.x = dB / 2;                 pt.y = 0.0 + dShiftZ - dH / 8;  arLPt.Add(pt);
	pt.x = dB / 2;                 pt.y = dH + dShiftZ + dH / 8;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if (bDim == FALSE) return;

	CString str;

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB / 2. + dShiftY;  pt.y = 0.0 + dShiftZ - 0.5*dH;  arLPt.Add(pt);
	pt.x = dB / 2. + dShiftY;  pt.y = 0.0 + dShiftZ - 0.5*dH;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dB_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB / 2. + dShiftY;  pt.y = 0.0 + dShiftZ - 0.3*dH;  arLPt.Add(pt);
	pt.x = -dB / 2. + da + dShiftY;  pt.y = 0.0 + dShiftZ - 0.3*dH;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), da_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB / 2. + dShiftY - 0.5*dB;  pt.y = 0.0 + dShiftZ;  arLPt.Add(pt);
	pt.x = -dB / 2. + dShiftY - 0.5*dB;  pt.y = dH + dShiftZ;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dH_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB / 2. + dShiftY - 0.2*dB;  pt.y = db + dShiftZ;  arLPt.Add(pt);
	pt.x = -dB / 2. + dShiftY - 0.2*dB;  pt.y = dShiftZ;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), db_Org);
	Draw_Dimension(pDC, arGPt, str);
}

//--------------------------------------------------------------------------
// Draw_Colm_vOcta
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Colm_vOcta(CDC* pDC, T_SECT_K SectK, int iPosi, BOOL bDim, BOOL bFill)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, iPosi);
	double dLen = Cng_UnitLen(m_nLengthUnit);

	double dH = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dB = m_dB = SectBaseD.Size[1] * DGNDRAW_UFAC;
	double da = SectBaseD.Size[2] * DGNDRAW_UFAC;
	double db = SectBaseD.Size[3] * DGNDRAW_UFAC;
	double dt = SectBaseD.Size[4] * DGNDRAW_UFAC;
	double dH_Org = SectBaseD.Size[0] * dLen;
	double dB_Org = SectBaseD.Size[1] * dLen;
	double da_Org = SectBaseD.Size[2] * dLen;
	double db_Org = SectBaseD.Size[3] * dLen;
	double dt_Org = SectBaseD.Size[4] * dLen;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK = FALSE;
	if (bDim) bGetOK = CalcCanvasRange(dB, dH, -0.5, -0.5);
	else     bGetOK = CalcCanvasRange(dB, dH);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;  arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt;  arGPt.RemoveAll();

	// Outer
	double dShiftY = SectBaseD.Design.YBar * DGNDRAW_UFAC;
	double dShiftZ = 0.0;
	double dShiftY_Org = SectBaseD.Design.YBar;
	double dShiftZ_Org = 0.0;

	pt.x = -dB / 2. + da + dShiftY;  pt.y = 0.0 + dShiftZ;  arLPt.Add(pt);
	pt.x = dB / 2. - da + dShiftY;  pt.y = 0.0 + dShiftZ;  arLPt.Add(pt);
	pt.x = dB / 2. + dShiftY;  pt.y = db + dShiftZ;  arLPt.Add(pt);
	pt.x = dB / 2. + dShiftY;  pt.y = dH - db + dShiftZ;  arLPt.Add(pt);
	pt.x = dB / 2. - da + dShiftY;  pt.y = dH + dShiftZ;  arLPt.Add(pt);
	pt.x = -dB / 2. + da + dShiftY;  pt.y = dH + dShiftZ;  arLPt.Add(pt);
	pt.x = -dB / 2. + dShiftY;  pt.y = dH - db + dShiftZ;  arLPt.Add(pt);
	pt.x = -dB / 2. + dShiftY;  pt.y = db + dShiftZ;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, bFill);

	// Inner
	arLPt.RemoveAll();
	arGPt.RemoveAll();

	T_SECT_SECTBASE_D SectData;
	SectData.Initialize();
	SectData.Size[0] = dH;
	SectData.Size[1] = dB;
	SectData.Size[2] = da;
	SectData.Size[3] = db;
	SectData.Size[4] = dt;
	double dH1 = 0.0, dB1 = 0.0, da1 = 0.0, db1 = 0.0, dDist0 = 0.0, dTheta1 = 0.0;
	if (!m_pDoc->m_pSectDB->CalcSectRegular_InOCT(SectData, dH1, dB1, da1, db1, dDist0, dTheta1)) ASSERT(0);

	double dShiftY1 = SectBaseD.Design.YBar * DGNDRAW_UFAC;
	double dShiftZ1 = SectBaseD.Size[4] * DGNDRAW_UFAC;    // dT
	double dShiftY1_Org = SectBaseD.Design.YBar;
	double dShiftZ1_Org = SectBaseD.Size[4];    // dT

	pt.x = -dB1 / 2. + da1 + dShiftY1;  pt.y = 0.0 + dShiftZ1;  arLPt.Add(pt);
	pt.x = -dB1 / 2. + dShiftY1;  pt.y = db1 + dShiftZ1;  arLPt.Add(pt);
	pt.x = -dB1 / 2. + dShiftY1;  pt.y = dH1 - db1 + dShiftZ1;  arLPt.Add(pt);
	pt.x = -dB1 / 2. + da1 + dShiftY1;  pt.y = dH1 + dShiftZ1;  arLPt.Add(pt);
	pt.x = dB1 / 2. - da1 + dShiftY1;  pt.y = dH1 + dShiftZ1;  arLPt.Add(pt);
	pt.x = dB1 / 2. + dShiftY1;  pt.y = dH1 - db1 + dShiftZ1;  arLPt.Add(pt);
	pt.x = dB1 / 2. + dShiftY1;  pt.y = db1 + dShiftZ1;  arLPt.Add(pt);
	pt.x = dB1 / 2. - da1 + dShiftY1;  pt.y = 0.0 + dShiftZ1;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, FALSE);

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll();  arGPt.RemoveAll();

	pt.x = -dB / 2 + dShiftY - dB / 9;  pt.y = dH / 2;               arLPt.Add(pt);
	pt.x = dB / 2 + dShiftY + dB / 9;  pt.y = dH / 2;               arLPt.Add(pt);
	pt.x = dB / 2;                pt.y = 0.0 + dShiftZ - dH / 8;  arLPt.Add(pt);
	pt.x = dB / 2;                pt.y = dH + dShiftZ + dH / 8;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if (bDim == FALSE) return;

	CString str;

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = dB / 2. + dShiftY - dShiftZ1;  pt.y = 0.0 + dShiftZ - 0.3*dH;  arLPt.Add(pt);
	pt.x = dB / 2. + dShiftY;  pt.y = 0.0 + dShiftZ - 0.3*dH;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dShiftZ1_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB / 2. + dShiftY;  pt.y = 0.0 + dShiftZ - 0.5*dH;  arLPt.Add(pt);
	pt.x = dB / 2. + dShiftY;  pt.y = 0.0 + dShiftZ - 0.5*dH;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dB_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB / 2. + dShiftY;  pt.y = 0.0 + dShiftZ - 0.3*dH;  arLPt.Add(pt);
	pt.x = -dB / 2. + da + dShiftY;  pt.y = 0.0 + dShiftZ - 0.3*dH;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), da_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB / 2. + dShiftY - 0.5*dB;  pt.y = 0.0 + dShiftZ;  arLPt.Add(pt);
	pt.x = -dB / 2. + dShiftY - 0.5*dB;  pt.y = dH + dShiftZ;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dH_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB / 2. + dShiftY - 0.2*dB;  pt.y = db + dShiftZ;  arLPt.Add(pt);
	pt.x = -dB / 2. + dShiftY - 0.2*dB;  pt.y = dShiftZ;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), db_Org);
	Draw_Dimension(pDC, arGPt, str);
}

//--------------------------------------------------------------------------
// Draw_Colm_sTrck
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Colm_sTrck(CDC* pDC, T_SECT_K SectK, int iPosi, BOOL bDim, BOOL bFill)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, iPosi);
	double dLen = Cng_UnitLen(m_nLengthUnit);

	double dH = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dB = m_dB = SectBaseD.Size[1] * DGNDRAW_UFAC;
	double dH_Org = SectBaseD.Size[0] * dLen;
	double dB_Org = SectBaseD.Size[1] * dLen;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK = FALSE;
	if (bDim) bGetOK = CalcCanvasRange(dB, dH, -0.4, -0.3);
	else     bGetOK = CalcCanvasRange(dB, dH);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;  arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt;  arGPt.RemoveAll();

	double dShiftY = SectBaseD.Design.YBar * DGNDRAW_UFAC;
	double dShiftZ = SectBaseD.Design.ZBar * DGNDRAW_UFAC;

	int nPontSize = 28;
	double dPi = 4.*atan(1.0);

	for (int i = 0; i < nPontSize; i++)
	{
		int iNo = ((i < nPontSize / 2) ? i : i - 1);
		double dY = sin(2 * dPi*((double)iNo / (nPontSize - 2))) * (dH / 2.0);
		double dZ = (-1)*cos(2 * dPi*((double)iNo / (nPontSize - 2))) * (dH / 2.0);
		double dIncY = ((i < nPontSize / 2) ? 1 : -1) * (dB - dH) / 2.0;

		pt.x = dIncY + dY + dShiftY;
		pt.y = dZ + dShiftZ;
		arLPt.Add(pt);
	}

	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, bFill);

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll();  arGPt.RemoveAll();

	pt.x = -dB / 2 + dShiftY - dB / 9;  pt.y = dH / 2;               arLPt.Add(pt);
	pt.x = dB / 2 + dShiftY + dB / 9;  pt.y = dH / 2;               arLPt.Add(pt);
	pt.x = dB / 2;                pt.y = 0.0 - dH / 8;  arLPt.Add(pt);
	pt.x = dB / 2;                pt.y = dH + dH / 8;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if (bDim == FALSE) return;

	CString str;

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB / 2. + dShiftY;  pt.y = -0.3*dH;  arLPt.Add(pt);
	pt.x = dB / 2. + dShiftY;  pt.y = -0.3*dH;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dB_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB / 2. + dShiftY - 0.5*dB;  pt.y = 0.0;  arLPt.Add(pt);
	pt.x = -dB / 2. + dShiftY - 0.5*dB;  pt.y = dH;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dH_Org);
	Draw_Dimension(pDC, arGPt, str);

}

//--------------------------------------------------------------------------
// Draw_Colm_vTrck
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Colm_vTrck(CDC* pDC, T_SECT_K SectK, int iPosi, BOOL bDim, BOOL bFill)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, iPosi);
	double dLen = Cng_UnitLen(m_nLengthUnit);

	double dH = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dB = m_dB = SectBaseD.Size[1] * DGNDRAW_UFAC;
	double dt = SectBaseD.Size[2] * DGNDRAW_UFAC;
	double dH_Org = SectBaseD.Size[0] * dLen;
	double dB_Org = SectBaseD.Size[1] * dLen;
	double dt_Org = SectBaseD.Size[2] * dLen;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK = FALSE;
	if (bDim) bGetOK = CalcCanvasRange(dB, dH, -0.4, -0.3);
	else     bGetOK = CalcCanvasRange(dB, dH);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;  arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt;  arGPt.RemoveAll();

	double dShiftY = SectBaseD.Design.YBar * DGNDRAW_UFAC;
	double dShiftZ = SectBaseD.Design.ZBar * DGNDRAW_UFAC;

	int nPontSize = 56;
	double dPi = 4.*atan(1.0);

	// Outer
	for (int i = 0; i < nPontSize / 2; i++)
	{
		int iNo = ((i < nPontSize / 4) ? i : i - 1);
		double dY = sin(2 * dPi*((double)iNo / (nPontSize / 2 - 2))) * (dH / 2.0);
		double dZ = (-1)*cos(2 * dPi*((double)iNo / (nPontSize / 2 - 2))) * (dH / 2.0);
		double dIncY = ((i < nPontSize / 4) ? 1 : -1) * (dB - dH) / 2.0;

		pt.x = dIncY + dY + dShiftY;
		pt.y = dZ + dShiftZ;
		arLPt.Add(pt);
	}

	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, bFill);

	// Inner
	arLPt.RemoveAll();
	arGPt.RemoveAll();
	for (int i = nPontSize / 2; i < nPontSize; i++)
	{
		int iNo = ((i < 3 * nPontSize / 4) ? i - 2 : i - 3) - (nPontSize / 2 - 2);
		double dY = (-1)*sin(2 * dPi*((double)iNo / (nPontSize / 2 - 2))) * (dH / 2.0 - dt);
		double dZ = (-1)*cos(2 * dPi*((double)iNo / (nPontSize / 2 - 2))) * (dH / 2.0 - dt);
		double dIncY = ((i < 3 * nPontSize / 4) ? -1 : 1) * (dB - dH) / 2.0;

		pt.x = dIncY + dY + dShiftY;
		pt.y = dZ + dShiftZ;
		arLPt.Add(pt);
	}

	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, FALSE);

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB / 2 + dShiftY - dB / 9;  pt.y = dH / 2;               arLPt.Add(pt);
	pt.x = dB / 2 + dShiftY + dB / 9;  pt.y = dH / 2;               arLPt.Add(pt);
	pt.x = dB / 2;                pt.y = 0.0 - dH / 8;  arLPt.Add(pt);
	pt.x = dB / 2;                pt.y = dH + dH / 8;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if (bDim == FALSE) return;

	CString str;

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB / 2. + dShiftY;  pt.y = -0.3*dH;  arLPt.Add(pt);
	pt.x = dB / 2. + dShiftY;  pt.y = -0.3*dH;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dB_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB / 2. + dShiftY - 0.5*dB;  pt.y = 0.0;  arLPt.Add(pt);
	pt.x = -dB / 2. + dShiftY - 0.5*dB;  pt.y = dH;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dH_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB / 2. + dShiftY - 0.2*dB;  pt.y = 0.0;  arLPt.Add(pt);
	pt.x = -dB / 2. + dShiftY - 0.2*dB;  pt.y = dt;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dt_Org);
	Draw_Dimension(pDC, arGPt, str);
}

//--------------------------------------------------------------------------
// Draw_Colm_sHTrk
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Colm_sHTrk(CDC* pDC, T_SECT_K SectK, int iPosi, BOOL bDim, BOOL bFill)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, iPosi);
	double dLen = Cng_UnitLen(m_nLengthUnit);

	double dH = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dB = m_dB = SectBaseD.Size[1] * DGNDRAW_UFAC;
	double dYBar = SectBaseD.Design.YBar * DGNDRAW_UFAC;
	double dH_Org = SectBaseD.Size[0] * dLen;
	double dB_Org = SectBaseD.Size[1] * dLen;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK = FALSE;
	if (bDim) bGetOK = CalcCanvasRange(dB, dH, -0.5, -0.3);
	else     bGetOK = CalcCanvasRange(dB, dH);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;  arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt;  arGPt.RemoveAll();

	double dShiftY = 0.0;
	double dShiftZ = SectBaseD.Design.ZBar * DGNDRAW_UFAC;

	int nPontSize = 16;
	double dPi = 4.*atan(1.0);

	for (int i = 0; i < nPontSize; i++)
	{
		double dY = 0.0, dZ = 0.0;
		if (i < nPontSize - 2)
		{
			dY = sin(dPi*((double)i / (nPontSize - 3))) * (dH / 2.0);
			dZ = (-1)*cos(dPi*((double)i / (nPontSize - 3))) * (dH / 2.0);
		}
		else if (i == nPontSize - 2) { dY = -(dB - dH / 2.0);  dZ = dH / 2.0; }
		else if (i == nPontSize - 1) { dY = -(dB - dH / 2.0);  dZ = -dH / 2.0; }

		pt.x = (dB - dH / 2.0) + dY + dShiftY;
		pt.y = dZ + dShiftZ;
		arLPt.Add(pt);
	}

	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, bFill);

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB / 9;    pt.y = dH / 2;        arLPt.Add(pt);
	pt.x = dB + dB / 9;  pt.y = dH / 2;        arLPt.Add(pt);
	pt.x = dYBar;    pt.y = 0.0 - dH / 8;    arLPt.Add(pt);
	pt.x = dYBar;     pt.y = dH + dH / 8;    arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if (bDim == FALSE) return;

	CString str;

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = 0.0;  pt.y = -0.3*dH;  arLPt.Add(pt);
	pt.x = dB;   pt.y = -0.3*dH;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dB_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -0.5*dB;  pt.y = 0.0;  arLPt.Add(pt);
	pt.x = -0.5*dB;  pt.y = dH;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dH_Org);
	Draw_Dimension(pDC, arGPt, str);
}

//--------------------------------------------------------------------------
// Draw_Colm_sRect
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Colm_sRect(CDC* pDC, T_SECT_K SectK, int iPosi, BOOL bDim, BOOL bFill)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, iPosi);
	double dLen = Cng_UnitLen(m_nLengthUnit);

	double H = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double B = m_dB = SectBaseD.Size[1] * DGNDRAW_UFAC;
	double H_Org = SectBaseD.Size[0] * dLen;
	double B_Org = SectBaseD.Size[1] * dLen;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK = FALSE;
	if (bDim) bGetOK = CalcCanvasRange(B, H, -0.5, -0.3);
	else     bGetOK = CalcCanvasRange(B, H);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;  arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt;  arGPt.RemoveAll();

	pt.x = 0;  pt.y = 0;  arLPt.Add(pt);
	pt.x = B;  pt.y = 0;  arLPt.Add(pt);
	pt.x = B;  pt.y = H;  arLPt.Add(pt);
	pt.x = 0;  pt.y = H;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, bFill);

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll();
	arGPt.RemoveAll();

	pt.x = 0 - B / 9;  pt.y = H / 2;      arLPt.Add(pt);
	pt.x = B + B / 9;  pt.y = H / 2;      arLPt.Add(pt);
	pt.x = B / 2;      pt.y = 0 - H / 9;  arLPt.Add(pt);
	pt.x = B / 2;      pt.y = H + H / 9;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if (bDim == FALSE) return;

	CString str;
	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = 0;   pt.y = -0.3*H;  arLPt.Add(pt);
	pt.x = B;   pt.y = -0.3*H;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), B_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = -0.5*B;   pt.y = 0;  arLPt.Add(pt);
	pt.x = -0.5*B;   pt.y = H;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), H_Org);
	Draw_Dimension(pDC, arGPt, str);
}

//--------------------------------------------------------------------------
// Draw_Colm_vRect
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Colm_vRect(CDC* pDC, T_SECT_K SectK, int iPosi, BOOL bDim, BOOL bFill)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, iPosi);
	double dLen = Cng_UnitLen(m_nLengthUnit);

	double dHc = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dB1 = m_dB = SectBaseD.Size[1] * DGNDRAW_UFAC;
	double dTw = SectBaseD.Size[2] * DGNDRAW_UFAC;
	double dTf1 = SectBaseD.Size[3] * DGNDRAW_UFAC;
	double dB2 = SectBaseD.Size[4] * DGNDRAW_UFAC;
	double dTf2 = SectBaseD.Size[5] * DGNDRAW_UFAC;
	if (dTf2 == 0) dTf2 = dTf1;
	double dHc_Org = SectBaseD.Size[0] * dLen;
	double dB1_Org = SectBaseD.Size[1] * dLen;
	double dTw_Org = SectBaseD.Size[2] * dLen;
	double dTf1_Org = SectBaseD.Size[3] * dLen;
	double dB2_Org = SectBaseD.Size[4] * dLen;
	double dTf2_Org = SectBaseD.Size[5] * dLen;
	if (dTf2_Org == 0) dTf2_Org = dTf1_Org;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK = FALSE;
	if (bDim) bGetOK = CalcCanvasRange(dB1, dHc, -0.5, -0.5, 0.3, 0);
	else     bGetOK = CalcCanvasRange(dB1, dHc);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;  arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt;  arGPt.RemoveAll();

	double dShiftY = SectBaseD.Design.YBar * DGNDRAW_UFAC;
	double dShiftZ = 0.0;

	// outer
	pt.x = -dB1 / 2. + dShiftY;  pt.y = 0.0 + dShiftZ;  arLPt.Add(pt);
	pt.x = dB1 / 2. + dShiftY;  pt.y = 0.0 + dShiftZ;  arLPt.Add(pt);
	pt.x = dB1 / 2. + dShiftY;  pt.y = dHc + dShiftZ;  arLPt.Add(pt);
	pt.x = -dB1 / 2. + dShiftY;  pt.y = dHc + dShiftZ;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, bFill);

	// Inner
	arLPt.RemoveAll();
	arGPt.RemoveAll();

	pt.x = -dB1 / 2. + dTw + dShiftY;  pt.y = dTf2 + dShiftZ;  arLPt.Add(pt);
	pt.x = -dB1 / 2. + dTw + dShiftY;  pt.y = dHc - dTf1 + dShiftZ;  arLPt.Add(pt);
	pt.x = dB1 / 2. - dTw + dShiftY;  pt.y = dHc - dTf1 + dShiftZ;  arLPt.Add(pt);
	pt.x = dB1 / 2. - dTw + dShiftY;  pt.y = dTf2 + dShiftZ;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, FALSE);

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll();
	arGPt.RemoveAll();

	pt.x = -dB1 / 2. + dShiftY - dB1 / 9;  pt.y = dHc / 2;               arLPt.Add(pt);
	pt.x = dB1 / 2. + dShiftY + dB1 / 9;  pt.y = dHc / 2;               arLPt.Add(pt);
	pt.x = dB1 / 2;                 pt.y = 0.0 + dShiftZ - dHc / 9;  arLPt.Add(pt);
	pt.x = dB1 / 2;                 pt.y = dHc + dShiftZ + dHc / 9;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if (bDim == FALSE) return;

	CString str;
	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB1 / 2. + dShiftY;   pt.y = dShiftZ - 0.5*dHc;  arLPt.Add(pt);
	pt.x = dB1 / 2. + dShiftY;   pt.y = dShiftZ - 0.5*dHc;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dB1_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB1 / 2. + dShiftY;  pt.y = dShiftZ - 0.3*dHc;  arLPt.Add(pt);
	pt.x = -dB1 / 2. + dTw + dShiftY;   pt.y = dShiftZ - 0.3*dHc;  arLPt.Add(pt);
	//pt.x = -(dB2-dTw)/2.+dShiftY;   pt.y = dShiftZ-0.3*dHc;  arLPt.Add(pt); 
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dTw_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	//pt.x =  (dB2-dTw)/2.+dShiftY;   pt.y = dShiftZ-0.3*dHc;  arLPt.Add(pt);
	pt.x = dB1 / 2. - dTw + dShiftY;   pt.y = dShiftZ - 0.3*dHc;  arLPt.Add(pt);
	pt.x = dB1 / 2. + dShiftY;   pt.y = dShiftZ - 0.3*dHc;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dTw_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB1 / 2. + dShiftY - 0.5*dB1;   pt.y = 0.0 + dShiftZ;  arLPt.Add(pt);
	pt.x = -dB1 / 2. + dShiftY - 0.5*dB1;   pt.y = dHc + dShiftZ;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dHc_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = dB1 / 2. + dShiftY + 0.3*dB1;   pt.y = dHc + dShiftZ;  arLPt.Add(pt);
	pt.x = dB1 / 2. + dShiftY + 0.3*dB1;   pt.y = dHc - dTf1 + dShiftZ;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dTf1_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = dB1 / 2. + dShiftY + 0.3*dB1;   pt.y = dTf2 + dShiftZ;  arLPt.Add(pt);
	pt.x = dB1 / 2. + dShiftY + 0.3*dB1;   pt.y = 0.0 + dShiftZ;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dTf2_Org);
	Draw_Dimension(pDC, arGPt, str);
}

//--------------------------------------------------------------------------
// Draw_Colm_H
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Colm_H(CDC* pDC, T_SECT_K SectK, int iPosi, BOOL bDim, BOOL bFill)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, iPosi);
	double dLen = Cng_UnitLen(m_nLengthUnit);

	double dH = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dB1 = SectBaseD.Size[1] * DGNDRAW_UFAC;
	double dTw = SectBaseD.Size[2] * DGNDRAW_UFAC;
	double dTf1 = SectBaseD.Size[3] * DGNDRAW_UFAC;
	double dB2 = SectBaseD.Size[4] * DGNDRAW_UFAC;
	double dTf2 = SectBaseD.Size[5] * DGNDRAW_UFAC;
	if (dTf2 == 0) dTf2 = dTf1;
	if (dB2 == 0)	dB2 = dB1;
	double dH_Org = SectBaseD.Size[0] * dLen;
	double dB1_Org = SectBaseD.Size[1] * dLen;
	double dTw_Org = SectBaseD.Size[2] * dLen;
	double dTf1_Org = SectBaseD.Size[3] * dLen;
	double dB2_Org = SectBaseD.Size[4] * dLen;
	double dTf2_Org = SectBaseD.Size[5] * dLen;
	if (dTf2_Org == 0) dTf2_Org = dTf1_Org;
	if (dB2_Org == 0)	dB2_Org = dB1_Org;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK = FALSE;
	if (bDim) bGetOK = CalcCanvasRange(dB2, dH, -0.5, -0.5, 0.3, 0);
	else     bGetOK = CalcCanvasRange(dB2, dH);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;  arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt;  arGPt.RemoveAll();

	double dShiftY = SectBaseD.Design.YBar * DGNDRAW_UFAC;
	double dShiftZ = 0.0;

	// outer
	pt.x = -dB2 / 2. + dShiftY;  pt.y = 0.0 + dShiftZ;  arLPt.Add(pt);
	pt.x = dB2 / 2. + dShiftY;  pt.y = 0.0 + dShiftZ;  arLPt.Add(pt);
	pt.x = dB2 / 2. + dShiftY;  pt.y = dTf2 + dShiftZ;  arLPt.Add(pt);
	pt.x = dTw / 2. + dShiftY;  pt.y = dTf2 + dShiftZ;  arLPt.Add(pt);
	pt.x = dTw / 2. + dShiftY;  pt.y = dH - dTf1 + dShiftZ;  arLPt.Add(pt);
	pt.x = dB1 / 2. + dShiftY;  pt.y = dH - dTf1 + dShiftZ;  arLPt.Add(pt);
	pt.x = dB1 / 2. + dShiftY;  pt.y = dH + dShiftZ;  arLPt.Add(pt);
	pt.x = -dB1 / 2. + dShiftY;  pt.y = dH + dShiftZ;  arLPt.Add(pt);
	pt.x = -dB1 / 2. + dShiftY;  pt.y = dH - dTf1 + dShiftZ;  arLPt.Add(pt);
	pt.x = -dTw / 2. + dShiftY;  pt.y = dH - dTf1 + dShiftZ;  arLPt.Add(pt);
	pt.x = -dTw / 2. + dShiftY;  pt.y = dTf2 + dShiftZ;  arLPt.Add(pt);
	pt.x = -dB2 / 2. + dShiftY;  pt.y = dTf2 + dShiftZ;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, bFill);

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll();
	arGPt.RemoveAll();

	pt.x = -dB2 / 2. + dShiftY - dB2 / 9;  pt.y = dH / 2;               arLPt.Add(pt);
	pt.x = dB2 / 2. + dShiftY + dB2 / 9;  pt.y = dH / 2;               arLPt.Add(pt);
	pt.x = dB2 / 2;                 pt.y = 0.0 + dShiftZ - dH / 9;  arLPt.Add(pt);
	pt.x = dB2 / 2;                 pt.y = dH + dShiftZ + dH / 9;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if (bDim == FALSE) return;

	/*
	CString str;
	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB1/2.+dShiftY;   pt.y = dShiftZ-0.5*dHc;  arLPt.Add(pt);
	pt.x =  dB1/2.+dShiftY;   pt.y = dShiftZ-0.5*dHc;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dB1_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x =     -dB1/2.+dShiftY;  pt.y = dShiftZ-0.3*dHc;  arLPt.Add(pt);
	pt.x = -dB1/2.+dTw+dShiftY;   pt.y = dShiftZ-0.3*dHc;  arLPt.Add(pt);
	//pt.x = -(dB2-dTw)/2.+dShiftY;   pt.y = dShiftZ-0.3*dHc;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dTw_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	//pt.x =  (dB2-dTw)/2.+dShiftY;   pt.y = dShiftZ-0.3*dHc;  arLPt.Add(pt);
	pt.x =  dB1/2.-dTw+dShiftY;   pt.y = dShiftZ-0.3*dHc;  arLPt.Add(pt);
	pt.x =      dB1/2.+dShiftY;   pt.y = dShiftZ-0.3*dHc;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dTw_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -dB1/2.+dShiftY-0.5*dB1;   pt.y = 0.0+dShiftZ;  arLPt.Add(pt);
	pt.x = -dB1/2.+dShiftY-0.5*dB1;   pt.y = dHc+dShiftZ;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dHc_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = dB1/2.+dShiftY+0.3*dB1;   pt.y = dHc     +dShiftZ;  arLPt.Add(pt);
	pt.x = dB1/2.+dShiftY+0.3*dB1;   pt.y = dHc-dTf1+dShiftZ;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dTf1_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = dB1/2.+dShiftY+0.3*dB1;   pt.y = dTf2+dShiftZ;  arLPt.Add(pt);
	pt.x = dB1/2.+dShiftY+0.3*dB1;   pt.y = 0.0 +dShiftZ;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dTf2_Org);
	Draw_Dimension(pDC, arGPt, str);
	*/
}

void CDgnSectionDrawFunc::Draw_Steel_L(CDC* pDC, T_SECT_K SectK, int iPosi, BOOL bDim, BOOL bFill)
{
	/*
	double dHc        = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dBc = m_dB = SectBaseD.Size[1] * DGNDRAW_UFAC;
	double dTw        = SectBaseD.Size[2] * DGNDRAW_UFAC;
	double dTf        = SectBaseD.Size[3] * DGNDRAW_UFAC;
	double dHc_Org    = SectBaseD.Size[0] * dLen;
	double dBc_Org    = SectBaseD.Size[1] * dLen;
	double dTw_Org    = SectBaseD.Size[2] * dLen;
	double dTf_Org    = SectBaseD.Size[3] * dLen;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK=FALSE;
	if(bDim) bGetOK = CalcCanvasRange(dBc, dHc, -0.5, -0.3, 0, 0.3);
	else     bGetOK = CalcCanvasRange(dBc, dHc);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;  arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt;  arGPt.RemoveAll();

	double dShiftY = SectBaseD.Design.YBar * DGNDRAW_UFAC;
	double dShiftZ = 0.0;

	pt.x = -dTw/2.+dShiftY;  pt.y = 0.0    +dShiftZ;  arLPt.Add(pt);
	pt.x =  dTw/2.+dShiftY;  pt.y = 0.0    +dShiftZ;  arLPt.Add(pt);
	pt.x =  dTw/2.+dShiftY;  pt.y = dHc-dTf+dShiftZ;  arLPt.Add(pt);
	pt.x =  dBc/2.+dShiftY;  pt.y = dHc-dTf+dShiftZ;  arLPt.Add(pt);
	pt.x =  dBc/2.+dShiftY;  pt.y = dHc    +dShiftZ;  arLPt.Add(pt);
	pt.x = -dBc/2.+dShiftY;  pt.y = dHc    +dShiftZ;  arLPt.Add(pt);
	pt.x = -dBc/2.+dShiftY;  pt.y = dHc-dTf+dShiftZ;  arLPt.Add(pt);
	pt.x = -dTw/2.+dShiftY;  pt.y = dHc-dTf+dShiftZ;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, bFill);

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll();
	arGPt.RemoveAll();

	pt.x = -dBc/2+dShiftY-dBc/10;  pt.y = dHc/2;               arLPt.Add(pt);
	pt.x =  dBc/2+dShiftY+dBc/10;  pt.y = dHc/2;               arLPt.Add(pt);
	pt.x =  dBc/2;                 pt.y = 0.0+dShiftZ-dHc/10;  arLPt.Add(pt);
	pt.x =  dBc/2;                 pt.y = dHc+dShiftZ+dHc/10;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if(bDim == FALSE) return;

	CString str;
	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = -dBc/2.+dShiftY;  pt.y = dHc+dShiftZ+0.3*dHc;  arLPt.Add(pt);
	pt.x =  dBc/2.+dShiftY;  pt.y = dHc+dShiftZ+0.3*dHc;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dBc_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = (-dBc/2.+dShiftY)-0.5*dBc;  pt.y = 0.0 +dShiftZ;  arLPt.Add(pt);
	pt.x = (-dBc/2.+dShiftY)-0.5*dBc;  pt.y = dHc +dShiftZ;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dHc_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = (-dBc/2.+dShiftY)-0.2*dBc;  pt.y = dHc-dTf+dShiftZ;  arLPt.Add(pt);
	pt.x = (-dBc/2.+dShiftY)-0.2*dBc;  pt.y = dHc    +dShiftZ;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dTf_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = -dTw/2.+dShiftY;  pt.y = dShiftZ-0.3*dHc;  arLPt.Add(pt);
	pt.x =  dTw/2.+dShiftY;  pt.y = dShiftZ-0.3*dHc;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dTw_Org);
	Draw_Dimension(pDC, arGPt, str);
	*/

}

void CDgnSectionDrawFunc::Draw_Steel_C(CDC* pDC, T_SECT_K SectK, int iPosi, BOOL bDim, BOOL bFill)
{
	/*
	T_SECT_D SectD;
	SectD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, iPosi);
	double dLen = Cng_UnitLen(m_nLengthUnit);

	double dH		= SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dB1	= SectBaseD.Size[1] * DGNDRAW_UFAC;
	double dTw	= SectBaseD.Size[2] * DGNDRAW_UFAC;
	double dTf1	= SectBaseD.Size[3] * DGNDRAW_UFAC;
	double dB2	= SectBaseD.Size[4] * DGNDRAW_UFAC;
	double dTf2 = SectBaseD.Size[5] * DGNDRAW_UFAC;
	if(dTf2 == 0) dTf2 = dTf1;
	if(dB2 == 0)	dB2 = dB1;
	double dH_Org		= SectBaseD.Size[0] * dLen;
	double dB1_Org	= SectBaseD.Size[1] * dLen;
	double dTw_Org	= SectBaseD.Size[2] * dLen;
	double dTf1_Org	= SectBaseD.Size[3] * dLen;
	double dB2_Org	= SectBaseD.Size[4] * dLen;
	double dTf2_Org = SectBaseD.Size[5] * dLen;
	if(dTf2_Org == 0) dTf2_Org = dTf1_Org;
	if(dB2_Org == 0)	dB2_Org = dB1_Org;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK=FALSE;
	if(bDim) bGetOK = CalcCanvasRange(dB2, dH, -0.5, -0.5, 0.3, 0);
	else     bGetOK = CalcCanvasRange(dB2, dH);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;  arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt;  arGPt.RemoveAll();

	double dShiftY = SectBaseD.Design.YBar * DGNDRAW_UFAC;
	double dShiftZ = 0.0;

	// outer
	pt.x = -dB2/2.+dShiftY;  pt.y = 0.0+dShiftZ;  arLPt.Add(pt);
	pt.x =  dB2/2.+dShiftY;  pt.y = 0.0+dShiftZ;  arLPt.Add(pt);
	pt.x =  dB2/2.+dShiftY;  pt.y = dTf2+dShiftZ;  arLPt.Add(pt);
	pt.x =  dTw/2.+dShiftY;  pt.y = dTf2+dShiftZ;  arLPt.Add(pt);
	pt.x =  dTw/2.+dShiftY;  pt.y = dH-dTf1+dShiftZ;  arLPt.Add(pt);
	pt.x =  dB1/2.+dShiftY;  pt.y = dH-dTf1+dShiftZ;  arLPt.Add(pt);
	pt.x =  dB1/2.+dShiftY;  pt.y = dH+dShiftZ;  arLPt.Add(pt);
	pt.x = -dB1/2.+dShiftY;  pt.y = dH+dShiftZ;  arLPt.Add(pt);
	pt.x = -dB1/2.+dShiftY;  pt.y = dH-dTf1+dShiftZ;  arLPt.Add(pt);
	pt.x = -dTw/2.+dShiftY;  pt.y = dH-dTf1+dShiftZ;  arLPt.Add(pt);
	pt.x = -dTw/2.+dShiftY;  pt.y = dTf2+dShiftZ;  arLPt.Add(pt);
	pt.x = -dB2/2.+dShiftY;  pt.y = dTf2+dShiftZ;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, bFill);

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll();
	arGPt.RemoveAll();

	pt.x = -dB2/2.+dShiftY-dB2/9;  pt.y = dH/2;               arLPt.Add(pt);
	pt.x =  dB2/2.+dShiftY+dB2/9;  pt.y = dH/2;               arLPt.Add(pt);
	pt.x =  dB2/2;                 pt.y = 0.0+dShiftZ-dH/9;  arLPt.Add(pt);
	pt.x =  dB2/2;                 pt.y = dH+dShiftZ+dH/9;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if(bDim == FALSE) return;
	*/
}

//--------------------------------------------------------------------------
// Draw_Wall
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Wall(CDC* pDC, T_SECT_K SectK, BOOL bDim, BOOL bFill, int iPosi, double dHaunch, BOOL bUWidth)
{
	if (bUWidth) Draw_Wall_UWidth(pDC, SectK, iPosi, bDim, bFill, dHaunch);
	else        Draw_Wall_Rect(pDC, SectK, iPosi, bDim, bFill, dHaunch);
}

//--------------------------------------------------------------------------
// Draw_Wall_Rect
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Wall_Rect(CDC* pDC, T_SECT_K SectK, int iPosi, BOOL bDim, BOOL bFill, double dHaunch)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, iPosi);
	double dLen = Cng_UnitLen(m_nLengthUnit);

	double H = (SectBaseD.Size[0] + dHaunch) * DGNDRAW_UFAC;
	double B = m_dB = SectBaseD.Size[1] * DGNDRAW_UFAC;
	double H_Org = (SectBaseD.Size[0] + dHaunch) * dLen;
	double B_Org = SectBaseD.Size[1] * dLen;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK = FALSE;
	if (bDim) bGetOK = CalcCanvasRange(B, H, -0.5, -0.3);
	else     bGetOK = CalcCanvasRange(B, H);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;  arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt;  arGPt.RemoveAll();

	pt.x = 0;  pt.y = 0;  arLPt.Add(pt);
	pt.x = B;  pt.y = 0;  arLPt.Add(pt);
	pt.x = B;  pt.y = H;  arLPt.Add(pt);
	pt.x = 0;  pt.y = H;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, bFill);

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll();
	arGPt.RemoveAll();

	pt.x = 0 - B / 10;  pt.y = H / 2;      arLPt.Add(pt);
	pt.x = B + B / 10;  pt.y = H / 2;      arLPt.Add(pt);
	pt.x = B / 2;       pt.y = 0 - H / 10;  arLPt.Add(pt);
	pt.x = B / 2;       pt.y = H + H / 10;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if (bDim == FALSE) return;

	CString str;
	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = 0;   pt.y = -0.3*H;  arLPt.Add(pt);
	pt.x = B;   pt.y = -0.3*H;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), B_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = -0.5*B;   pt.y = 0;  arLPt.Add(pt);
	pt.x = -0.5*B;   pt.y = H;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), H_Org);
	Draw_Dimension(pDC, arGPt, str);
}

//--------------------------------------------------------------------------
// Draw_Wall_UWidth
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Wall_UWidth(CDC* pDC, T_SECT_K SectK, int iPosi, BOOL bDim, BOOL bFill, double dHaunch)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, iPosi);
	double dLen = Cng_UnitLen(m_nLengthUnit);
	double dLenUnitWidth = Cng_UnitLen(m_nWidthLengthUnit);

	double H = (SectBaseD.Size[0] + dHaunch) * DGNDRAW_UFAC;
	double B = m_dB = (dLenUnitWidth == 0 ? 1.0 : 1.0 / dLenUnitWidth) * DGNDRAW_UFAC;
	double H_Org = (SectBaseD.Size[0] + dHaunch) * dLen;
	double B_Org = (dLenUnitWidth == 0 ? 1.0 : 1.0 / dLenUnitWidth) * dLen;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK = FALSE;
	if (bDim) bGetOK = CalcCanvasRange(B, H, -0.5, -0.3);
	else     bGetOK = CalcCanvasRange(B, H);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	double dDelta = B / 17.0;
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;  arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt;  arGPt.RemoveAll();
	//   ⑪                                ⑧
	//   │                                │ 
	//   ⑩────────────────⑨
	//   │                                │ 
	// ⑬┘⑫                            ⑥┘⑦
	//   ＼                                ＼  
	// ⑮┌⑭                            ④┌⑤
	//   │                                │ 
	//   ①────────────────②
	//   │                                │ 
	//   16                                ③
	arLPt.SetSize(16);
	pt.x = 0;         pt.y = 0;             arLPt.SetAt(0, pt);
	pt.x = B;         pt.y = 0;             arLPt.SetAt(1, pt);
	pt.x = B;         pt.y = -dDelta;       arLPt.SetAt(2, pt);
	pt.x = B;         pt.y = H / 2.0 - dDelta;  arLPt.SetAt(3, pt);
	pt.x = B + dDelta;  pt.y = H / 2.0 - dDelta;  arLPt.SetAt(4, pt);
	pt.x = B - dDelta;  pt.y = H / 2.0 + dDelta;  arLPt.SetAt(5, pt);
	pt.x = B;         pt.y = H / 2.0 + dDelta;  arLPt.SetAt(6, pt);
	pt.x = B;         pt.y = H + dDelta;      arLPt.SetAt(7, pt);
	pt.x = B;         pt.y = H;             arLPt.SetAt(8, pt);
	pt.x = 0;         pt.y = H;             arLPt.SetAt(9, pt);
	pt.x = 0;         pt.y = H + dDelta;      arLPt.SetAt(10, pt);
	pt.x = 0;         pt.y = H / 2.0 + dDelta;  arLPt.SetAt(11, pt);
	pt.x = -dDelta;   pt.y = H / 2.0 + dDelta;  arLPt.SetAt(12, pt);
	pt.x = dDelta;    pt.y = H / 2.0 - dDelta;  arLPt.SetAt(13, pt);
	pt.x = 0;         pt.y = H / 2.0 - dDelta;  arLPt.SetAt(14, pt);
	pt.x = 0;         pt.y = -dDelta;       arLPt.SetAt(15, pt);

	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, bFill);

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll();
	arGPt.RemoveAll();

	pt.x = 0 - B / 15.;  pt.y = H / 2.;      arLPt.Add(pt);
	pt.x = B + B / 15.;  pt.y = H / 2.;      arLPt.Add(pt);
	pt.x = B / 2.;       pt.y = 0 - B / 15.;  arLPt.Add(pt);
	pt.x = B / 2.;       pt.y = H + B / 15.;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if (bDim == FALSE) return;

	CString str;
	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = 0;   pt.y = -0.2*B;  arLPt.Add(pt);
	pt.x = B;   pt.y = -0.2*B;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), B_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = -0.4*B;   pt.y = 0;  arLPt.Add(pt);
	pt.x = -0.4*B;   pt.y = H;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), H_Org);
	Draw_Dimension(pDC, arGPt, str);
}

//--------------------------------------------------------------------------
// Draw_Plate
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Plate(CDC* pDC, T_THIK_K ThikK, BOOL bDim, BOOL bFill, int iPosi)
{
	Draw_Plate_UWidth(pDC, ThikK, iPosi, bDim, bFill);
}

//--------------------------------------------------------------------------
// Draw_Plate_UWidth
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Plate_UWidth(CDC* pDC, T_THIK_K ThikK, int iPosi, BOOL bDim, BOOL bFill)
{
	double   dThik = 0.0;
	T_THIK_D ThikD; ThikD.Initialize();
	m_pDoc->m_pAttrCtrl->GetThik(ThikK, ThikD);
	m_pDoc->m_pAttrCtrl->GetThikForDesign(ThikD, dThik);

	double dLen = Cng_UnitLen(m_nLengthUnit);
	double dLenUnitWidth = Cng_UnitLen(m_nWidthLengthUnit);

	double H = dThik * DGNDRAW_UFAC;
	double B = m_dB = (dLenUnitWidth == 0 ? 1.0 : 1.0 / dLenUnitWidth) * DGNDRAW_UFAC;
	double H_Org = dThik * dLen;
	double B_Org = (dLenUnitWidth == 0 ? 1.0 : 1.0 / dLenUnitWidth) * dLen;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK = FALSE;
	if (bDim) bGetOK = CalcCanvasRange(B, H, -0.5, -0.3);
	else     bGetOK = CalcCanvasRange(B, H);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	double dDelta = B / 17.0;
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;  arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt;  arGPt.RemoveAll();
	//   ⑪                                ⑧
	//   │                                │ 
	//   ⑩────────────────⑨
	//   │                                │ 
	// ⑬┘⑫                            ⑥┘⑦
	//   ＼                                ＼  
	// ⑮┌⑭                            ④┌⑤
	//   │                                │ 
	//   ①────────────────②
	//   │                                │ 
	//   16                                ③
	arLPt.SetSize(16);
	pt.x = 0;         pt.y = 0;             arLPt.SetAt(0, pt);
	pt.x = B;         pt.y = 0;             arLPt.SetAt(1, pt);
	pt.x = B;         pt.y = -dDelta;       arLPt.SetAt(2, pt);
	pt.x = B;         pt.y = H / 2.0 - dDelta;  arLPt.SetAt(3, pt);
	pt.x = B + dDelta;  pt.y = H / 2.0 - dDelta;  arLPt.SetAt(4, pt);
	pt.x = B - dDelta;  pt.y = H / 2.0 + dDelta;  arLPt.SetAt(5, pt);
	pt.x = B;         pt.y = H / 2.0 + dDelta;  arLPt.SetAt(6, pt);
	pt.x = B;         pt.y = H + dDelta;      arLPt.SetAt(7, pt);
	pt.x = B;         pt.y = H;             arLPt.SetAt(8, pt);
	pt.x = 0;         pt.y = H;             arLPt.SetAt(9, pt);
	pt.x = 0;         pt.y = H + dDelta;      arLPt.SetAt(10, pt);
	pt.x = 0;         pt.y = H / 2.0 + dDelta;  arLPt.SetAt(11, pt);
	pt.x = -dDelta;   pt.y = H / 2.0 + dDelta;  arLPt.SetAt(12, pt);
	pt.x = dDelta;    pt.y = H / 2.0 - dDelta;  arLPt.SetAt(13, pt);
	pt.x = 0;         pt.y = H / 2.0 - dDelta;  arLPt.SetAt(14, pt);
	pt.x = 0;         pt.y = -dDelta;       arLPt.SetAt(15, pt);

	MappingCoord(arLPt, arGPt);
	Draw_Concrete(pDC, arGPt, bFill);

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll();
	arGPt.RemoveAll();

	pt.x = 0 - B / 15.;  pt.y = H / 2.;      arLPt.Add(pt);
	pt.x = B + B / 15.;  pt.y = H / 2.;      arLPt.Add(pt);
	pt.x = B / 2.;       pt.y = 0 - B / 15.;  arLPt.Add(pt);
	pt.x = B / 2.;       pt.y = H + B / 15.;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if (bDim == FALSE) return;

	CString str;
	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = 0;   pt.y = -0.2*B;  arLPt.Add(pt);
	pt.x = B;   pt.y = -0.2*B;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), B_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = -0.4*B;   pt.y = 0;  arLPt.Add(pt);
	pt.x = -0.4*B;   pt.y = H;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), H_Org);
	Draw_Dimension(pDC, arGPt, str);
}

//--------------------------------------------------------------------------
// Draw_Colm_sCirc
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Colm_sCirc(CDC* pDC, T_SECT_K SectK, int iPosi, BOOL bDim, BOOL bFill)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, iPosi);
	double dLen = Cng_UnitLen(m_nLengthUnit);

	double dHc = m_dB = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dHc_Org = SectBaseD.Size[0] * dLen;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK = FALSE;
	if (bDim) bGetOK = CalcCanvasRange(dHc, dHc, -0.3, -0.3);
	else     bGetOK = CalcCanvasRange(dHc, dHc);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;  arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt;  arGPt.RemoveAll();

	// Outer
	pt.x = 0;    pt.y = dHc;  arLPt.Add(pt);
	pt.x = dHc;  pt.y = 0;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_rConcrete(pDC, arGPt, bFill);

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll(); arGPt.RemoveAll();

	pt.x = 0 - dHc / 9;  pt.y = dHc / 2;       arLPt.Add(pt);
	pt.x = dHc + dHc / 9;  pt.y = dHc / 2;       arLPt.Add(pt);
	pt.x = dHc / 2;      pt.y = 0.0 - dHc / 9;  arLPt.Add(pt);
	pt.x = dHc / 2;      pt.y = dHc + dHc / 9;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if (bDim == FALSE) return;

	CString str;
	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = 0;    pt.y = -0.3*dHc;  arLPt.Add(pt);
	pt.x = dHc;  pt.y = -0.3*dHc;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dHc_Org);
	Draw_Dimension(pDC, arGPt, str);
}


//--------------------------------------------------------------------------
// Draw_Colm_vCirc
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Colm_vCirc(CDC* pDC, T_SECT_K SectK, int iPosi, BOOL bDim, BOOL bFill)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, iPosi);
	double dLen = Cng_UnitLen(m_nLengthUnit);

	double dHc = m_dB = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dB1 = SectBaseD.Size[1] * DGNDRAW_UFAC;
	double dHc_Org = SectBaseD.Size[0] * dLen;
	double dB1_Org = SectBaseD.Size[1] * dLen;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK = FALSE;
	if (bDim) bGetOK = CalcCanvasRange(dHc, dHc, -0.3, -0.3);
	else     bGetOK = CalcCanvasRange(dHc, dHc);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;
	CArray<CPoint, CPoint> arGPt;

	// Outer
	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = 0;    pt.y = dHc;  arLPt.Add(pt);
	pt.x = dHc;  pt.y = 0;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_rConcrete(pDC, arGPt, bFill);

	// Inner
	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = dB1;       pt.y = dHc - dB1;  arLPt.Add(pt);
	pt.x = dHc - dB1;   pt.y = dB1;      arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_rConcrete(pDC, arGPt, FALSE);

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll(); arGPt.RemoveAll();

	pt.x = 0 - dHc / 9;  pt.y = dHc / 2;       arLPt.Add(pt);
	pt.x = dHc + dHc / 9;  pt.y = dHc / 2;       arLPt.Add(pt);
	pt.x = dHc / 2;      pt.y = 0.0 - dHc / 9;  arLPt.Add(pt);
	pt.x = dHc / 2;      pt.y = dHc + dHc / 9;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if (bDim == FALSE) return;

	CString str;
	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = 0;    pt.y = -0.3*dHc;  arLPt.Add(pt);
	pt.x = dHc;  pt.y = -0.3*dHc;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dHc_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -0.2*dHc;  pt.y = dHc;      arLPt.Add(pt);
	pt.x = -0.2*dHc;  pt.y = dHc - dB1;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dB1_Org);
	Draw_Dimension(pDC, arGPt, str);
}
//--------------------------------------------------------------------------
// Draw_Colm_Gen
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_Colm_Gen(CDC* pDC, T_SECT_K SectK, int iPosi, BOOL bDim, BOOL bFill)
{
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, iPosi);
	double dLen = Cng_UnitLen(m_nLengthUnit);

	//
	// 폭과 높이를 표현할 값 만들것
	//
	double dH = (SectBaseD.Stiffness.Czp + SectBaseD.Stiffness.Czm) * DGNDRAW_UFAC;
	double dB = m_dB = (SectBaseD.Stiffness.Cyp + SectBaseD.Stiffness.Cym) * DGNDRAW_UFAC;
	double dYBar = SectBaseD.Design.YBar * DGNDRAW_UFAC;
	double dZBar = SectBaseD.Design.ZBar * DGNDRAW_UFAC;
	double dH_Org = (SectBaseD.Stiffness.Czp + SectBaseD.Stiffness.Czm) * dLen;
	double dB_Org = (SectBaseD.Stiffness.Cyp + SectBaseD.Stiffness.Cym) * dLen;

	//////////////////////////////////////////////////////////////////////
	// Calc Scale Factor
	BOOL bGetOK = FALSE;
	if (bDim) bGetOK = CalcCanvasRange(dB, dH, -0.5, -0.3);
	else     bGetOK = CalcCanvasRange(dB, dH);
	ASSERT(bGetOK);

	//////////////////////////////////////////////////////////////////////
	// Draw Con.
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;      arLPt.RemoveAll();
	CArray<CPoint, CPoint> arLPt_Off;  arLPt_Off.RemoveAll();
	CArray<CPoint, CPoint> arGPt;      arGPt.RemoveAll();


	int iOuterPolygonSize = SectBaseD.aOuterPolygon.GetSize();
	int iInnerPolygonSize = SectBaseD.aInnerPolygon.GetSize();
	if (iOuterPolygonSize < 1)return;
	if (iOuterPolygonSize > 1)ASSERT(0);// 외곽 라인이 1개가 아닐경우 확인요망
	iOuterPolygonSize = 1;// 외곽 라인은 1개로 고정  
	T_GSEC_POLYGON PolygonData;
	T_GSEC_VERTEX vertex2D;
	int iPolygonMemberSize, i, k;
	//외곽라인 입력
	for (i = 0; i < iOuterPolygonSize; i++)
	{
		arLPt.RemoveAll();
		arLPt_Off.RemoveAll();
		arGPt.RemoveAll();

		PolygonData.Initialize();
		PolygonData = SectBaseD.aOuterPolygon.GetAt(i);
		iPolygonMemberSize = PolygonData.aVertex.GetSize();

		for (k = 0; k < iPolygonMemberSize; k++)
		{
			vertex2D = PolygonData.aVertex.GetAt(k);
			pt.x = vertex2D.dX * DGNDRAW_UFAC;      pt.y = vertex2D.dY * DGNDRAW_UFAC;
			arLPt.Add(pt);
		}
		OffsetCoord(arLPt, arLPt_Off, dYBar, dZBar);
		MappingCoord(arLPt_Off, arGPt);
		Draw_Concrete(pDC, arGPt, bFill);
	}
	//안쪽라인들 입력
	for (i = 0; i < iInnerPolygonSize; i++)
	{
		arLPt.RemoveAll();
		arLPt_Off.RemoveAll();
		arGPt.RemoveAll();

		PolygonData.Initialize();
		PolygonData = SectBaseD.aInnerPolygon.GetAt(i);
		iPolygonMemberSize = PolygonData.aVertex.GetSize();

		for (k = 0; k < iPolygonMemberSize; k++)
		{
			vertex2D = PolygonData.aVertex.GetAt(k);
			pt.x = vertex2D.dX * DGNDRAW_UFAC;      pt.y = vertex2D.dY * DGNDRAW_UFAC;
			arLPt.Add(pt);
		}
		OffsetCoord(arLPt, arLPt_Off, dYBar, dZBar);
		MappingCoord(arLPt_Off, arGPt);
		Draw_Concrete(pDC, arGPt, FALSE);
	}

	//////////////////////////////////////////////////////////////////////
	// Draw Axis
	arLPt.RemoveAll();
	arGPt.RemoveAll();

	pt.x = 0 - dB / 9;  pt.y = dZBar;      arLPt.Add(pt);
	pt.x = dB + dB / 9; pt.y = dZBar;      arLPt.Add(pt);
	pt.x = dYBar;     pt.y = 0 - dH / 9;   arLPt.Add(pt);
	pt.x = dYBar;     pt.y = dH + dH / 9;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_AxisYZ(pDC, arGPt);

	//////////////////////////////////////////////////////////////////////
	// Draw Dim.
	if (bDim == FALSE) return;

	CString str;
	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = 0;   pt.y = -0.3*dH;  arLPt.Add(pt);
	pt.x = dB;   pt.y = -0.3*dH;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dB_Org);
	Draw_Dimension(pDC, arGPt, str);

	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = -0.5*dB;   pt.y = 0;  arLPt.Add(pt);
	pt.x = -0.5*dB;   pt.y = dH;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), dH_Org);
	Draw_Dimension(pDC, arGPt, str);
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Draw_DgnRebar
//

//--------------------------------------------------------------------------
// Draw_DgnRebar
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_DgnRebar(CDC* pDC, T_SECT_K SectK, int nIMJ, T_RDGN_D* pRdgnD, BOOL bDim, BOOL bFill)
{
	ASSERT(pDC);

	if (pRdgnD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRdgnD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRdgn(SectK, m_tmpRdgnD);
		if (bGetOK) pRdgnD = &m_tmpRdgnD;
		else       return;
	}
	ASSERT(pRdgnD);

	T_SECT_D SectD; SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	if (pRdgnD->nType == 0)       Draw_DgnRebar_Beam(pDC, SectK, nIMJ, pRdgnD, bDim, bFill); // Beam
	else if (pRdgnD->nType == 1)  Draw_DgnRebar_Colm(pDC, SectK, pRdgnD, bDim, bFill);        // Column/Brace
}

//--------------------------------------------------------------------------
// Draw_DgnRebar_Beam
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_DgnRebar_Beam(CDC* pDC, T_SECT_K SectK, int nIMJ, T_RDGN_D* pRdgnD, BOOL bDim, BOOL bFill)
{
	if (pRdgnD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRdgnD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRdgn(SectK, m_tmpRdgnD);
		if (bGetOK) pRdgnD = &m_tmpRdgnD;
		else       return;
	}
	ASSERT(pRdgnD);

	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SB)     Draw_DgnRebar_Beam_Rect(pDC, SectK, nIMJ, pRdgnD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_T) Draw_DgnRebar_Beam_Tee(pDC, SectK, nIMJ, pRdgnD, bDim, bFill);

}

//--------------------------------------------------------------------------
// Draw_DgnRebar_Beam_Rect
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_DgnRebar_Beam_Rect(CDC* pDC, T_SECT_K SectK, int nIMJ, T_RDGN_D* pRdgnD, BOOL bDim, BOOL bFill)
{
	if (pRdgnD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRdgnD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRdgn(SectK, m_tmpRdgnD);
		if (bGetOK) pRdgnD = &m_tmpRdgnD;
		else       return;
	}
	ASSERT(pRdgnD);

	T_SECT_D SectD; SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);

	double dHc = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dB1 = SectBaseD.Size[1] * DGNDRAW_UFAC;

	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;
	CArray<CPoint, CPoint> arGPt;
	///////////////////////////////////////////////////////////////////////////
	// Rebar

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	// Bottom
	arLPt.RemoveAll();
	arGPt.RemoveAll();

	pt.x = +DGN_RBAR_SIZE;  pt.y = pRdgnD->BEAM.dDb[0]; arLPt.Add(pt);
	pt.x = dB1 - DGN_RBAR_SIZE;  pt.y = pRdgnD->BEAM.dDb[0]; arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_DgnRebar(pDC, arGPt, bFill);

	// Top
	arLPt.RemoveAll();
	arGPt.RemoveAll();

	pt.x = dB1 - DGN_RBAR_SIZE;  pt.y = dHc - pRdgnD->BEAM.dDt[0]; arLPt.Add(pt);
	pt.x = +DGN_RBAR_SIZE;  pt.y = dHc - pRdgnD->BEAM.dDt[0]; arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_DgnRebar(pDC, arGPt, bFill);

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (bDim == FALSE) return;

	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	if (pRdgnD->BEAM.dDb[0] != 0)
	{
		arLPt.RemoveAll();  arGPt.RemoveAll();
		pt.x = -0.2*dB1;    pt.y = 0;  arLPt.Add(pt);
		pt.x = -0.2*dB1;    pt.y = pRdgnD->BEAM.dDb[0];  arLPt.Add(pt);
		MappingCoord(arLPt, arGPt);
		str.Format(_T("%g"), pRdgnD->BEAM.dDb[0] * dLen);
		Draw_Dimension(pDC, arGPt, str);
	}
	if (pRdgnD->BEAM.dDt[0] != 0)
	{
		arLPt.RemoveAll();  arGPt.RemoveAll();
		pt.x = -0.2*dB1;    pt.y = dHc - pRdgnD->BEAM.dDt[0];  arLPt.Add(pt);
		pt.x = -0.2*dB1;    pt.y = dHc;  arLPt.Add(pt);
		MappingCoord(arLPt, arGPt);
		str.Format(_T("%g"), pRdgnD->BEAM.dDt[0] * dLen);
		Draw_Dimension(pDC, arGPt, str);
	}
}

//--------------------------------------------------------------------------
// Draw_DgnRebar_Beam_Tee
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_DgnRebar_Beam_Tee(CDC* pDC, T_SECT_K SectK, int nIMJ, T_RDGN_D* pRdgnD, BOOL bDim, BOOL bFill)
{
	if (pRdgnD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRdgnD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRdgn(SectK, m_tmpRdgnD);
		if (bGetOK) pRdgnD = &m_tmpRdgnD;
		else       return;
	}
	ASSERT(pRdgnD);

	T_SECT_D SectD; SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);

	double dHc = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dBc = SectBaseD.Size[1] * DGNDRAW_UFAC;
	double dTw = SectBaseD.Size[2] * DGNDRAW_UFAC;
	double dTf = SectBaseD.Size[3] * DGNDRAW_UFAC;

	double dStaY = (dBc - dTw) / 2.0;
	double dEndY = (dBc + dTw) / 2.0;

	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;
	CArray<CPoint, CPoint> arGPt;
	///////////////////////////////////////////////////////////////////////////
	// Rebar

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	// Bottom
	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = dStaY + DGN_RBAR_SIZE;  pt.y = pRdgnD->BEAM.dDb[0]; arLPt.Add(pt);
	pt.x = dEndY - DGN_RBAR_SIZE;  pt.y = pRdgnD->BEAM.dDb[0]; arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_DgnRebar(pDC, arGPt, bFill);

	// Top
	arLPt.RemoveAll();
	arGPt.RemoveAll();
	pt.x = dEndY - DGN_RBAR_SIZE;  pt.y = dHc - pRdgnD->BEAM.dDt[0]; arLPt.Add(pt);
	pt.x = dStaY + DGN_RBAR_SIZE;  pt.y = dHc - pRdgnD->BEAM.dDt[0]; arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_DgnRebar(pDC, arGPt, bFill);

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (!bDim) return;
	if (pRdgnD->BEAM.dDb[0] == 0) return;

	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = dStaY - 0.3*dBc;  pt.y = 0; arLPt.Add(pt);
	pt.x = dStaY - 0.3*dBc;  pt.y = pRdgnD->BEAM.dDb[0]; arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pRdgnD->BEAM.dDb[0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}

void CDgnSectionDrawFunc::Draw_DgnRebar_Colm(CDC* pDC, T_SECT_K SectK, T_RDGN_D* pRdgnD, BOOL bDim, BOOL bFill)
{
	if (pRdgnD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRdgnD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRdgn(SectK, m_tmpRdgnD);
		if (bGetOK) pRdgnD = &m_tmpRdgnD;
		else       return;
	}
	ASSERT(pRdgnD);

	T_SECT_D SectD; SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_B)         Draw_DgnRebar_Colm_Rect(pDC, SectK, pRdgnD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SB)   Draw_DgnRebar_Colm_Rect(pDC, SectK, pRdgnD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_P)    Draw_DgnRebar_Colm_Circ(pDC, SectK, pRdgnD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SR)   Draw_DgnRebar_Colm_Circ(pDC, SectK, pRdgnD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_OCT)  Draw_DgnRebar_Colm_Octa(pDC, SectK, pRdgnD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SOCT) Draw_DgnRebar_Colm_Octa(pDC, SectK, pRdgnD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_TRK)  Draw_DgnRebar_Colm_Trck(pDC, SectK, pRdgnD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_STRK) Draw_DgnRebar_Colm_Trck(pDC, SectK, pRdgnD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_HTRK) Draw_DgnRebar_Colm_Htrk(pDC, SectK, pRdgnD, bDim, bFill);
}

void CDgnSectionDrawFunc::Draw_DgnRebar_Colm_Rect(CDC* pDC, T_SECT_K SectK, T_RDGN_D* pRdgnD, BOOL bDim, BOOL bFill)
{
	if (pRdgnD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRdgnD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRdgn(SectK, m_tmpRdgnD);
		if (bGetOK) pRdgnD = &m_tmpRdgnD;
		else       return;
	}
	ASSERT(pRdgnD);
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	if (pRdgnD->COLM.dDc[0] <= 0.0) return;

	T_SECT_D SectD; SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	double dHc = SectD.SectBefore.SectI.Size[0] * DGNDRAW_UFAC;
	double dB1 = SectD.SectBefore.SectI.Size[1] * DGNDRAW_UFAC;

	CPoint pt;
	CArray<CPoint, CPoint> arLPt; arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt; arGPt.RemoveAll();
	///////////////////////////////////////////////////////////////////////////
	// Rebar
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	pt.x = +pRdgnD->COLM.dDc[0];  pt.y = pRdgnD->COLM.dDc[0];  arLPt.Add(pt);
	pt.x = dB1 - pRdgnD->COLM.dDc[0];  pt.y = pRdgnD->COLM.dDc[0];  arLPt.Add(pt);
	pt.x = dB1 - pRdgnD->COLM.dDc[0];  pt.y = dHc - pRdgnD->COLM.dDc[0];  arLPt.Add(pt);
	pt.x = +pRdgnD->COLM.dDc[0];  pt.y = dHc - pRdgnD->COLM.dDc[0];  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_DgnRebar(pDC, arGPt, bFill);

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (!bDim) return;

	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -0.2*dB1; pt.y = 0;                 arLPt.Add(pt);
	pt.x = -0.2*dB1; pt.y = pRdgnD->COLM.dDc[0];  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pRdgnD->COLM.dDc[0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}

void CDgnSectionDrawFunc::Draw_DgnRebar_Colm_Circ(CDC* pDC, T_SECT_K SectK, T_RDGN_D* pRdgnD, BOOL bDim, BOOL bFill)
{
	if (pRdgnD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRdgnD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRdgn(SectK, m_tmpRdgnD);
		if (bGetOK) pRdgnD = &m_tmpRdgnD;
		else       return;
	}
	ASSERT(pRdgnD);

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	
	if (pRdgnD->COLM.dDc[0] <= 0.0) return;

	T_SECT_D SectD; SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	double dHc = SectD.SectBefore.SectI.Size[0] * DGNDRAW_UFAC;

	CPoint pt;
	CArray<CPoint, CPoint> arLPt; arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt; arGPt.RemoveAll();
	///////////////////////////////////////////////////////////////////////////
	// Rebar

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	pt.x = +pRdgnD->COLM.dDc[0];  pt.y = dHc - pRdgnD->COLM.dDc[0];  arLPt.Add(pt);
	pt.x = dHc - pRdgnD->COLM.dDc[0];  pt.y = pRdgnD->COLM.dDc[0];  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_DgnCRebar(pDC, arGPt, bFill);

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (!bDim) return;

	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -0.2*dHc;  pt.y = 0;                 arLPt.Add(pt);
	pt.x = -0.2*dHc;  pt.y = pRdgnD->COLM.dDc[0];  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pRdgnD->COLM.dDc[0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}

void CDgnSectionDrawFunc::Draw_DgnRebar_Colm_Octa(CDC* pDC, T_SECT_K SectK, T_RDGN_D* pRdgnD, BOOL bDim, BOOL bFill)
{
	if (pRdgnD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRdgnD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRdgn(SectK, m_tmpRdgnD);
		if (bGetOK) pRdgnD = &m_tmpRdgnD;
		else       return;
	}
	ASSERT(pRdgnD);

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	
	if (pRdgnD->COLM.dDc[0] <= 0.0) return;

	T_SECT_D SectD; SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	double dH = SectD.SectBefore.SectI.Size[0] * DGNDRAW_UFAC;
	double dB = SectD.SectBefore.SectI.Size[1] * DGNDRAW_UFAC;
	double da = SectD.SectBefore.SectI.Size[2] * DGNDRAW_UFAC;
	double db = SectD.SectBefore.SectI.Size[3] * DGNDRAW_UFAC;

	double dCenY = dB / 2.0;
	double dCenZ = dH / 2.0;
	double dPi = 4.*atan(1.0);

	double dY1, dZ1, dY2, dZ2, dTheta;

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	
	double dDc = pRdgnD->COLM.dDc[0];
	if (dDc == 0) return;

	if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_OCT)
	{
		double dH1 = 0.0, dB1 = 0.0, da1 = 0.0, db1 = 0.0, dDist0 = 0.0, dTheta1 = 0.0, dt = 0.0;

		dt = SectD.SectBefore.SectI.Size[4] * DGNDRAW_UFAC;
		T_SECT_SECTBASE_D SectData;
		SectData.Initialize();
		SectData.Size[0] = dH;
		SectData.Size[1] = dB;
		SectData.Size[2] = da;
		SectData.Size[3] = db;
		SectData.Size[4] = dt;

		if (!m_pDoc->m_pSectDB->CalcSectRegular_InOCT(SectData, dH1, dB1, da1, db1, dDist0, dTheta1))	ASSERT(0);

		dY1 = (dB / 2.0 - da) - ((dB / 2.0 - da) - (dB1 / 2.0 - da1))*(dDc / dt);
		dZ1 = dH / 2.0 - dDc;
		dY2 = dB / 2.0 - dDc;
		dZ2 = (dH / 2.0 - db) - ((dH / 2.0 - db) - (dH1 / 2.0 - db1))*(dDc / dt);
	}
	else
	{
		dTheta = (dPi - atan(db / da)) / 2.0;	// Radian.
		dY1 = (dB / 2.0 - da) - dDc / tan(dTheta);
		dZ1 = dH / 2.0 - dDc;
		dY2 = dB / 2.0 - dDc;
		dZ2 = (dH / 2.0 - db) - dDc / tan(dTheta);
	}

	CPoint pt;
	CArray<CPoint, CPoint> arLPt; arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt; arGPt.RemoveAll();
	///////////////////////////////////////////////////////////////////////////
	// Rebar
	pt.x = dY1 + dCenY;  pt.y = dZ1 + dCenZ;  arLPt.Add(pt);
	pt.x = -dY1 + dCenY;  pt.y = dZ1 + dCenZ;  arLPt.Add(pt);
	pt.x = -dY2 + dCenY;  pt.y = dZ2 + dCenZ;  arLPt.Add(pt);
	pt.x = -dY2 + dCenY;  pt.y = -dZ2 + dCenZ;  arLPt.Add(pt);
	pt.x = -dY1 + dCenY;  pt.y = -dZ1 + dCenZ;  arLPt.Add(pt);
	pt.x = dY1 + dCenY;  pt.y = -dZ1 + dCenZ;  arLPt.Add(pt);
	pt.x = dY2 + dCenY;  pt.y = -dZ2 + dCenZ;  arLPt.Add(pt);
	pt.x = dY2 + dCenY;  pt.y = dZ2 + dCenZ;  arLPt.Add(pt);

	MappingCoord(arLPt, arGPt);
	Draw_DgnCRebar(pDC, arGPt, bFill);

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (!bDim) return;

	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -0.2*dB;  pt.y = dH;                   arLPt.Add(pt);
	pt.x = -0.2*dB;  pt.y = dH - pRdgnD->COLM.dDc[0];  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pRdgnD->COLM.dDc[0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}

void CDgnSectionDrawFunc::Draw_DgnRebar_Colm_Trck(CDC* pDC, T_SECT_K SectK, T_RDGN_D* pRdgnD, BOOL bDim, BOOL bFill)
{
	if (pRdgnD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRdgnD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRdgn(SectK, m_tmpRdgnD);
		if (bGetOK) pRdgnD = &m_tmpRdgnD;
		else       return;
	}
	ASSERT(pRdgnD);
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	
	if (pRdgnD->COLM.dDc[0] <= 0.0) return;

	T_SECT_D SectD; SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	double dH = SectD.SectBefore.SectI.Size[0] * DGNDRAW_UFAC;
	double dB = SectD.SectBefore.SectI.Size[1] * DGNDRAW_UFAC;

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	
	double dDc = pRdgnD->COLM.dDc[0];

	double dCenY = dB / 2.0;
	double dCenZ = dH / 2.0;

	CPoint pt;
	CArray<CPoint, CPoint> arLPt;
	CArray<CPoint, CPoint> arGPt;
	///////////////////////////////////////////////////////////////////////////
	// Rebar
	/*
	arGPt.RemoveAll();
	Draw_LineChkRebar(1, 1, i,  ((dB-dH)/2.0)+dCenY, -((dB-dH)/2.0)+dCenY, dH-dDc, dH-dDc, pRchkD, arGPt);
	Draw_DgnLRebar(pDC, arGPt, bFill);

	arGPt.RemoveAll();
	Draw_CirLChkRebar(1, 2, i, dCenY, dCenY, dH-dDc,  dCenZ, dB-dH, pRchkD, arGPt);
	Draw_DgnLRebar(pDC, arGPt, bFill);

	arGPt.RemoveAll();
	Draw_LineChkRebar(1, 1, i, -((dB-dH)/2.0)+dCenY,  ((dB-dH)/2.0)+dCenY,    dDc,    dDc, pRchkD, arGPt);
	Draw_DgnLRebar(pDC, arGPt, bFill);

	arGPt.RemoveAll();
	Draw_CirRChkRebar(1, 2, i, dCenY, dCenY,    dDc,  dCenZ, dB-dH, pRchkD, arGPt);
	Draw_DgnLRebar(pDC, arGPt, bFill);*/

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (!bDim) return;

	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -0.2*dB;  pt.y = dH;  arLPt.Add(pt);
	pt.x = -0.2*dB;  pt.y = dH - pRdgnD->COLM.dDc[0];  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pRdgnD->COLM.dDc[0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}

void CDgnSectionDrawFunc::Draw_DgnRebar_Colm_Htrk(CDC* pDC, T_SECT_K SectK, T_RDGN_D* pRdgnD, BOOL bDim, BOOL bFill)
{
	if (pRdgnD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRdgnD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRdgn(SectK, m_tmpRdgnD);
		if (bGetOK) pRdgnD = &m_tmpRdgnD;
		else       return;
	}
	ASSERT(pRdgnD);

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	
	if (pRdgnD->COLM.dDc[0] <= 0.0) return;

	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	double dH = SectD.SectBefore.SectI.Size[0] * DGNDRAW_UFAC;
	double dB = SectD.SectBefore.SectI.Size[1] * DGNDRAW_UFAC;

	double dCenY = (dB - dH / 2.0) / 2.0;
	double dCenZ = dH / 2.0;

	CArray<CPoint, CPoint> arGPt;
	///////////////////////////////////////////////////////////////////////////
	// Rebar
	  // Top, Left, Bot, Right (StaY, EndY, StaZ, EndZ).
/*
		arGPt.RemoveAll();
		Draw_LineChkRebar(1, 1, i, (dB-dH/2.0),         dDc, dH-dDc, dH-dDc, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(1, 3, i,         dDc,         dDc, dH-dDc,    dDc, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(1, 1, i,         dDc, (dB-dH/2.0),    dDc,    dDc, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_CirRChkRebar(1, 2, i, dCenY, dCenY, dDc, dCenZ, dB-dH/2.0, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);
		*/

		///////////////////////////////////////////////////////////////////////////
		// Dimension
	if (!bDim) return;

	CPoint pt;
	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	CArray<CPoint, CPoint> arLPt;
	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -0.2*dB;  pt.y = dH;  arLPt.Add(pt);
	pt.x = -0.2*dB;  pt.y = dH - pRdgnD->COLM.dDc[0];  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pRdgnD->COLM.dDc[0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Draw_ChkRebar
//

//--------------------------------------------------------------------------
// Draw_ChkRebar
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_ChkRebar(CDC* pDC, T_SECT_K SectK, int nIMJ, T_RCHK_D* pRchkD, BOOL bDim, BOOL bFill)
{
	ASSERT(pDC);
	if (pRchkD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRchkD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRchk(SectK, m_tmpRchkD);
		if (bGetOK) pRchkD = &m_tmpRchkD;
		else       return;
	}
	ASSERT(pRchkD);

	if (pRchkD->nType == 0)       Draw_ChkRebar_Beam(pDC, SectK, nIMJ, pRchkD, bDim, bFill); // Beam
	else if (pRchkD->nType == 1)  Draw_ChkRebar_Colm(pDC, SectK, pRchkD, bDim, bFill);        // Column/Brace
}
void CDgnSectionDrawFunc::Draw_ChkRebar(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPBE_D* pCpbeD, BOOL bDim, BOOL bFill)
{
	ASSERT(pDC);
	if (pCpbeD == NULL)   // Data를 넘겨주지 않을경우
	{
		ASSERT(0);
		return;
	}
	ASSERT(pCpbeD);

	Draw_ChkRebar_Beam(pDC, SectK, nIMJ, pCpbeD, bDim, bFill);        // Beam
}
// T_CPCO_D 용 (SHIN '06.03.20 추가)
void CDgnSectionDrawFunc::Draw_ChkRebar(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPCO_D* pCpcoD, BOOL bDim, BOOL bFill)
{
	ASSERT(pDC);
	if (pCpcoD == NULL)   // Data를 넘겨주지 않을경우
	{
		ASSERT(0);
		return;
	}
	ASSERT(pCpcoD);

	Draw_ChkRebar_Colm(pDC, SectK, nIMJ, pCpcoD, bDim, bFill);        // Column
}


//--------------------------------------------------------------------------
// Draw_ChkRebar_Beam
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_ChkRebar_Beam(CDC* pDC, T_SECT_K SectK, int nIMJ, T_RCHK_D* pRchkD, BOOL bDim, BOOL bFill)
{
	if (pRchkD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRchkD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRchk(SectK, m_tmpRchkD);
		if (bGetOK) pRchkD = &m_tmpRchkD;
		else       return;
	}
	ASSERT(pRchkD);

	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SB)     Draw_ChkRebar_Beam_Rect(pDC, SectK, nIMJ, pRchkD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_T) Draw_ChkRebar_Beam_Tee(pDC, SectK, nIMJ, pRchkD, bDim, bFill);

}
void CDgnSectionDrawFunc::Draw_ChkRebar_Beam(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPBE_D* pCpbeD, BOOL bDim, BOOL bFill)
{
	if (pCpbeD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		ASSERT(0);
		return;
	}
	ASSERT(pCpbeD);

	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SB)                                      Draw_ChkRebar_Beam_Rect(pDC, SectK, nIMJ, pCpbeD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_T)                                  Draw_ChkRebar_Beam_Tee(pDC, SectK, nIMJ, pCpbeD, bDim, bFill);
	else if ((SectD.nStype == D_SECT_TYPE_TAPERED || SectD.nStype == D_SECT_TYPE_PSC) && SectD.SectBefore.Shape == D_SECT_SHAPE_PSC_MID) Draw_ChkRebar_Beam_PSC_MID(pDC, SectK, nIMJ, pCpbeD, bDim, bFill);
}
//--------------------------------------------------------------------------
// Draw_ChkRebar_Beam_Rect
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_ChkRebar_Beam_Rect(CDC* pDC, T_SECT_K SectK, int nIMJ, T_RCHK_D* pRchkD, BOOL bDim, BOOL bFill)
{
	if (pRchkD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRchkD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRchk(SectK, m_tmpRchkD);
		if (bGetOK) pRchkD = &m_tmpRchkD;
		else       return;
	}
	ASSERT(pRchkD);

	T_SECT_D SectD; SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);

	double dHc = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dB1 = SectBaseD.Size[1] * DGNDRAW_UFAC;

	double dt = 10;
	if (pRchkD->BEAM.dDt[nIMJ][0] > pRchkD->BEAM.dDb[nIMJ][0]) dt = pRchkD->BEAM.dDb[nIMJ][0] * DGNDRAW_UFAC;
	else                                                      dt = pRchkD->BEAM.dDt[nIMJ][0] * DGNDRAW_UFAC;

	if (dt <= 0)
	{
		double dt1 = pRchkD->BEAM.dDb[nIMJ][0] * DGNDRAW_UFAC;
		double dt2 = pRchkD->BEAM.dDt[nIMJ][0] * DGNDRAW_UFAC;
		dt = max(dt1, dt2);
		if (dt <= 0) return;
	}
	double dDb2 = pRchkD->BEAM.dDb[nIMJ][0] * DGNDRAW_UFAC;
	double dDt2 = pRchkD->BEAM.dDt[nIMJ][0] * DGNDRAW_UFAC;
	if (dDb2 == 0.0) dDb2 = dHc / 20.0;
	else if (dDt2 == 0.0) dDt2 = dHc / 20.0;

	///////////////////////////////////////////////////////////////////////////
	// Stirrup Bar
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;
	CArray<CPoint, CPoint> arGPt;

	//   arLPt.RemoveAll(); arGPt.RemoveAll();
	//   pt.x =     dt-DGN_RBAR_SIZE/2;  pt.y =     pRchkD->BEAM.dDb[nIMJ][0]*DGNDRAW_UFAC-DGN_RBAR_SIZE/2; arLPt.Add(pt);
	//   pt.x = dB1-dt+DGN_RBAR_SIZE/2;  pt.y =     pRchkD->BEAM.dDb[nIMJ][0]*DGNDRAW_UFAC-DGN_RBAR_SIZE/2; arLPt.Add(pt);
	//   MappingCoord(arLPt, arGPt);
	//   Draw_StirrupBar(pDC, arGPt, bFill, FALSE);
	//   
	//   arLPt.RemoveAll(); arGPt.RemoveAll();
	//   pt.x = dB1-dt+DGN_RBAR_SIZE/2;  pt.y = dHc-pRchkD->BEAM.dDt[nIMJ][0]*DGNDRAW_UFAC+DGN_RBAR_SIZE/2; arLPt.Add(pt);
	//   pt.x =     dt-DGN_RBAR_SIZE/2;  pt.y = dHc-pRchkD->BEAM.dDt[nIMJ][0]*DGNDRAW_UFAC+DGN_RBAR_SIZE/2; arLPt.Add(pt);
	//   MappingCoord(arLPt, arGPt);
	//   Draw_StirrupBar(pDC, arGPt, bFill, FALSE);
	//   
	//   double dSubBarNum = pRchkD->BEAM.dSubBarNum[nIMJ];
	//   for(int i=0; i<(dSubBarNum-0.99999); i++)
	//   {
	//     arLPt.RemoveAll(); arGPt.RemoveAll();
	//     pt.x = dt-DGN_RBAR_SIZE/2 + (dB1-2*dt+DGN_RBAR_SIZE)*(i/(dSubBarNum-1));  pt.y =     pRchkD->BEAM.dDb[nIMJ][0]*DGNDRAW_UFAC-DGN_RBAR_SIZE/2;  arLPt.Add(pt); 
	//     pt.x = dt-DGN_RBAR_SIZE/2 + (dB1-2*dt+DGN_RBAR_SIZE)*(i/(dSubBarNum-1));  pt.y = dHc-pRchkD->BEAM.dDt[nIMJ][0]*DGNDRAW_UFAC+DGN_RBAR_SIZE/2;  arLPt.Add(pt);
	//     MappingCoord(arLPt, arGPt);
	//     Draw_StirrupBar(pDC, arGPt, bFill, FALSE);
	//   }

	arLPt.RemoveAll(); arGPt.RemoveAll();
	pt.x = dt - DGN_RBAR_SIZE / 2;  pt.y = dDb2 - DGN_RBAR_SIZE / 2; arLPt.Add(pt);
	pt.x = dB1 - dt + DGN_RBAR_SIZE / 2;  pt.y = dDb2 - DGN_RBAR_SIZE / 2; arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	Draw_StirrupBar(pDC, arGPt, bFill, FALSE);

	arLPt.RemoveAll(); arGPt.RemoveAll();
	pt.x = dB1 - dt + DGN_RBAR_SIZE / 2;  pt.y = dHc - dDt2 + DGN_RBAR_SIZE / 2; arLPt.Add(pt);
	pt.x = dt - DGN_RBAR_SIZE / 2;  pt.y = dHc - dDt2 + DGN_RBAR_SIZE / 2; arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	Draw_StirrupBar(pDC, arGPt, bFill, FALSE);

	double dSubBarNum = pRchkD->BEAM.dSubBarNum[nIMJ];
	for (int i = 0; i < (dSubBarNum - 0.99999); i++)
	{
		arLPt.RemoveAll(); arGPt.RemoveAll();
		pt.x = dt - DGN_RBAR_SIZE / 2 + (dB1 - 2 * dt + DGN_RBAR_SIZE)*(i / (dSubBarNum - 1));  pt.y = dDb2 - DGN_RBAR_SIZE / 2;  arLPt.Add(pt);
		pt.x = dt - DGN_RBAR_SIZE / 2 + (dB1 - 2 * dt + DGN_RBAR_SIZE)*(i / (dSubBarNum - 1));  pt.y = dHc - dDt2 + DGN_RBAR_SIZE / 2;  arLPt.Add(pt);
		MappingCoord(arLPt, arGPt);
		Draw_StirrupBar(pDC, arGPt, bFill, FALSE);
	}


	///////////////////////////////////////////////////////////////////////////
	// Rebar
	for (int i = 0; i < DGN_RBAR_LAYER_BEAM; i++)
	{
		double dDT = pRchkD->BEAM.dDt[nIMJ][i] * DGNDRAW_UFAC;
		double dDB = pRchkD->BEAM.dDb[nIMJ][i] * DGNDRAW_UFAC;

		if (dDT > 0.0)
		{
			arGPt.RemoveAll();
			Draw_LineChkRebar(TRUE, nIMJ, 1, i, dt, dB1 - dt, dHc - dDT, dHc - dDT, pRchkD, arGPt);
			if (pRchkD->BEAM.bTopGroup[nIMJ][i]) // Group
			{
				double dGapFromCenter = pRchkD->BEAM.dTopGroupGap[nIMJ][i] * DGNDRAW_UFAC + GetLPByGPForLength(m_nRebarSize);
				for (int j = 0; j < pRchkD->BEAM.nTopGroupNum[nIMJ][i] - 1; j++)
				{
					Draw_LineChkRebar(TRUE, nIMJ, 1, i, dt, dB1 - dt, dHc - dDT - dGapFromCenter * (j + 1), dHc - dDT - dGapFromCenter * (j + 1), pRchkD, arGPt);
				}
			}
			Draw_Rebar(pDC, arGPt, bFill);
		}
		if (dDB > 0.0)
		{
			arGPt.RemoveAll();
			Draw_LineChkRebar(TRUE, nIMJ, 2, i, dt, dB1 - dt, dDB, dDB, pRchkD, arGPt);
			if (pRchkD->BEAM.bBotGroup[nIMJ][i]) // Group
			{
				double dGapFromCenter = pRchkD->BEAM.dBotGroupGap[nIMJ][i] * DGNDRAW_UFAC + GetLPByGPForLength(m_nRebarSize);
				for (int j = 0; j < pRchkD->BEAM.nBotGroupNum[nIMJ][i] - 1; j++)
				{
					Draw_LineChkRebar(TRUE, nIMJ, 2, i, dt, dB1 - dt, dDB + dGapFromCenter * (j + 1), dDB + dGapFromCenter * (j + 1), pRchkD, arGPt);
				}
			}
			Draw_Rebar(pDC, arGPt, bFill);
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (bDim == FALSE) return;

	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);
	if (pRchkD->BEAM.dDb[nIMJ][0] != 0)
	{
		arLPt.RemoveAll();  arGPt.RemoveAll();
		pt.x = -0.2*dB1;   pt.y = 0;  arLPt.Add(pt);
		pt.x = -0.2*dB1;   pt.y = pRchkD->BEAM.dDb[nIMJ][0] * DGNDRAW_UFAC - DGN_RBAR_SIZE / 2;  arLPt.Add(pt);
		MappingCoord(arLPt, arGPt);
		str.Format(_T("%g"), pRchkD->BEAM.dDb[nIMJ][0] * dLen);
		Draw_Dimension(pDC, arGPt, str);
	}
	if (pRchkD->BEAM.dDt[nIMJ][0] != 0)
	{
		arLPt.RemoveAll();  arGPt.RemoveAll();
		pt.x = -0.2*dB1;   pt.y = dHc - pRchkD->BEAM.dDt[nIMJ][0] * DGNDRAW_UFAC + DGN_RBAR_SIZE / 2;  arLPt.Add(pt);
		pt.x = -0.2*dB1;   pt.y = dHc;  arLPt.Add(pt);
		MappingCoord(arLPt, arGPt);
		str.Format(_T("%g"), pRchkD->BEAM.dDt[nIMJ][0] * dLen);
		Draw_Dimension(pDC, arGPt, str);
	}
}

void CDgnSectionDrawFunc::Draw_ChkRebar_Beam_Rect(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPBE_D* pCpbeD, BOOL bDim, BOOL bFill)
{
	if (pCpbeD == NULL)   // Data를 넘겨주지 않을경우
	{
		ASSERT(0);
		return;
	}
	ASSERT(pCpbeD);

	T_SECT_D SectD; SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);

	double dHc = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dB1 = SectBaseD.Size[1] * DGNDRAW_UFAC;
	BOOL bEndCrossMark;

	double dt = 10;
	if (pCpbeD->dDt[nIMJ][0] > 0 && pCpbeD->dDb[nIMJ][0] <= 0) dt = pCpbeD->dDt[nIMJ][0] * DGNDRAW_UFAC;
	else if (pCpbeD->dDt[nIMJ][0] <= 0 && pCpbeD->dDb[nIMJ][0] > 0) dt = pCpbeD->dDb[nIMJ][0] * DGNDRAW_UFAC;
	else if (pCpbeD->dDt[nIMJ][0] > pCpbeD->dDb[nIMJ][0])      dt = pCpbeD->dDb[nIMJ][0] * DGNDRAW_UFAC;
	else                                                      dt = pCpbeD->dDt[nIMJ][0] * DGNDRAW_UFAC;

	if (dt <= 0) return;

	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;
	CArray<CPoint, CPoint> arGPt;

	///////////////////////////////////////////////////////////////////////////
	// Rebar
	for (int i = 0; i < DGN_RBAR_LAYER_BEAM; i++)
	{
		double dDT = pCpbeD->dDt[nIMJ][i] * DGNDRAW_UFAC;
		double dDB = pCpbeD->dDb[nIMJ][i] * DGNDRAW_UFAC;

		if (dDT > 0.0)
		{
			arGPt.RemoveAll();
			if (pCpbeD->bNum)
				//Draw_LineChkRebar(TRUE, nIMJ, 1, i, dt, dB1-dt, dHc-dDT, dHc-dDT, pCpbeD, arGPt, bEndCrossMark);
				Draw_LineChkRebar(TRUE, nIMJ, 1, i, dB1*0.1, dB1*0.9, dHc - dDT, dHc - dDT, pCpbeD, arGPt, bEndCrossMark);  // by SeungJun (`06.03.29) 양사이드의 피복두께는 일정하게 유지하도록..
			else
				Draw_LineChkRebar(TRUE, nIMJ, 1, i, 0.0, dB1, dHc - dDT, dHc - dDT, pCpbeD, arGPt, bEndCrossMark);

			Draw_Rebar(pDC, arGPt, bFill, bEndCrossMark);
		}
		if (dDB > 0.0)
		{
			arGPt.RemoveAll();
			if (pCpbeD->bNum)
				//Draw_LineChkRebar(TRUE, nIMJ, 2, i, dt, dB1-dt,     dDB,     dDB, pCpbeD, arGPt, bEndCrossMark);
				Draw_LineChkRebar(TRUE, nIMJ, 2, i, dB1*0.1, dB1*0.9, dDB, dDB, pCpbeD, arGPt, bEndCrossMark);  // by SeungJun (`06.03.29) 양사이드의 피복두께는 일정하게 유지하도록..
			else
				Draw_LineChkRebar(TRUE, nIMJ, 2, i, 0.0, dB1, dDB, dDB, pCpbeD, arGPt, bEndCrossMark);

			Draw_Rebar(pDC, arGPt, bFill, bEndCrossMark);
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (bDim == FALSE) return;

	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);
	if (pCpbeD->dDb[nIMJ][0] != 0)
	{
		arLPt.RemoveAll();  arGPt.RemoveAll();
		pt.x = -0.2*dB1;    pt.y = 0;                                      arLPt.Add(pt);
		pt.x = -0.2*dB1;    pt.y = pCpbeD->dDb[nIMJ][0] * DGNDRAW_UFAC;      arLPt.Add(pt);
		MappingCoord(arLPt, arGPt);
		str.Format(_T("%g"), pCpbeD->dDb[nIMJ][0] * dLen);
		Draw_Dimension(pDC, arGPt, str);
	}
	if (pCpbeD->dDt[nIMJ][0] != 0)
	{
		arLPt.RemoveAll();  arGPt.RemoveAll();
		pt.x = -0.2*dB1;    pt.y = dHc - pCpbeD->dDt[nIMJ][0] * DGNDRAW_UFAC;  arLPt.Add(pt);
		pt.x = -0.2*dB1;    pt.y = dHc;                                    arLPt.Add(pt);
		MappingCoord(arLPt, arGPt);
		str.Format(_T("%g"), pCpbeD->dDt[nIMJ][0] * dLen);
		Draw_Dimension(pDC, arGPt, str);
	}
}

//--------------------------------------------------------------------------
// Draw_ChkRebar_Beam_Tee
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_ChkRebar_Beam_Tee(CDC* pDC, T_SECT_K SectK, int nIMJ, T_RCHK_D* pRchkD, BOOL bDim, BOOL bFill)
{
	if (pRchkD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRchkD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRchk(SectK, m_tmpRchkD);
		if (bGetOK) pRchkD = &m_tmpRchkD;
		else       return;
	}
	ASSERT(pRchkD);

	T_SECT_D SectD; SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);

	double dHc = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dBc = SectBaseD.Size[1] * DGNDRAW_UFAC;
	double dTw = SectBaseD.Size[2] * DGNDRAW_UFAC;
	double dTf = SectBaseD.Size[3] * DGNDRAW_UFAC;

	double dStaY = (dBc - dTw) / 2.0;
	double dEndY = (dBc + dTw) / 2.0;

	double dt = 10;
	if (pRchkD->BEAM.dDt[nIMJ][0] > pRchkD->BEAM.dDb[nIMJ][0]) dt = pRchkD->BEAM.dDb[nIMJ][0] * DGNDRAW_UFAC;
	else                                                      dt = pRchkD->BEAM.dDt[nIMJ][0] * DGNDRAW_UFAC;

	if (dt <= 0)
	{
		double dt1 = pRchkD->BEAM.dDb[nIMJ][0] * DGNDRAW_UFAC;
		double dt2 = pRchkD->BEAM.dDt[nIMJ][0] * DGNDRAW_UFAC;
		dt = max(dt1, dt2);
		if (dt <= 0) return;
	}
	double dDb2 = pRchkD->BEAM.dDb[nIMJ][0] * DGNDRAW_UFAC;
	double dDt2 = pRchkD->BEAM.dDt[nIMJ][0] * DGNDRAW_UFAC;
	if (dDb2 == 0.0) dDb2 = dDt2;
	else if (dDt2 == 0.0) dDt2 = dDb2;

	///////////////////////////////////////////////////////////////////////////
	// Stirrup Bar
	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;
	CArray<CPoint, CPoint> arGPt;


	//   arLPt.RemoveAll();  arGPt.RemoveAll();
	//   pt.x = dStaY+dt-DGN_RBAR_SIZE/2;  pt.y =     pRchkD->BEAM.dDb[nIMJ][0]*DGNDRAW_UFAC-DGN_RBAR_SIZE/2; arLPt.Add(pt);
	//   pt.x = dEndY-dt+DGN_RBAR_SIZE/2;  pt.y =     pRchkD->BEAM.dDb[nIMJ][0]*DGNDRAW_UFAC-DGN_RBAR_SIZE/2; arLPt.Add(pt);
	//   MappingCoord(arLPt, arGPt);
	//   Draw_StirrupBar(pDC, arGPt, bFill, FALSE);
	// 
	// 
	//   pt.x = dEndY-dt+DGN_RBAR_SIZE/2;  pt.y = dHc-pRchkD->BEAM.dDt[nIMJ][0]*DGNDRAW_UFAC+DGN_RBAR_SIZE/2; arLPt.Add(pt);
	//   pt.x = dStaY+dt-DGN_RBAR_SIZE/2;  pt.y = dHc-pRchkD->BEAM.dDt[nIMJ][0]*DGNDRAW_UFAC+DGN_RBAR_SIZE/2; arLPt.Add(pt);
	//   MappingCoord(arLPt, arGPt);
	//   Draw_StirrupBar(pDC, arGPt, bFill, FALSE);
	// 
	//   double dSubBarNum = pRchkD->BEAM.dSubBarNum[nIMJ];
	//   for(int i=0; i<dSubBarNum-0.99999; i++)
	// 	{
	//     arLPt.RemoveAll(); arGPt.RemoveAll();
	//     pt.x = dStaY+dt-DGN_RBAR_SIZE/2 + (dEndY-dStaY-2*dt+DGN_RBAR_SIZE)*(i/(dSubBarNum-1));  pt.y =     pRchkD->BEAM.dDb[nIMJ][0]*DGNDRAW_UFAC-DGN_RBAR_SIZE/2;  arLPt.Add(pt); 
	//     pt.x = dStaY+dt-DGN_RBAR_SIZE/2 + (dEndY-dStaY-2*dt+DGN_RBAR_SIZE)*(i/(dSubBarNum-1));  pt.y = dHc-pRchkD->BEAM.dDt[nIMJ][0]*DGNDRAW_UFAC+DGN_RBAR_SIZE/2;  arLPt.Add(pt);
	//     MappingCoord(arLPt, arGPt);
	//     Draw_StirrupBar(pDC, arGPt, bFill, FALSE);
	// 	}

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = dStaY + dt - DGN_RBAR_SIZE / 2;  pt.y = dDb2 - DGN_RBAR_SIZE / 2; arLPt.Add(pt);
	pt.x = dEndY - dt + DGN_RBAR_SIZE / 2;  pt.y = dDb2 - DGN_RBAR_SIZE / 2; arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	Draw_StirrupBar(pDC, arGPt, bFill, FALSE);


	pt.x = dEndY - dt + DGN_RBAR_SIZE / 2;  pt.y = dHc - dDt2 + DGN_RBAR_SIZE / 2; arLPt.Add(pt);
	pt.x = dStaY + dt - DGN_RBAR_SIZE / 2;  pt.y = dHc - dDt2 + DGN_RBAR_SIZE / 2; arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	Draw_StirrupBar(pDC, arGPt, bFill, FALSE);

	double dSubBarNum = pRchkD->BEAM.dSubBarNum[nIMJ];
	for (int i = 0; i < dSubBarNum - 0.99999; i++)
	{
		arLPt.RemoveAll(); arGPt.RemoveAll();
		pt.x = dStaY + dt - DGN_RBAR_SIZE / 2 + (dEndY - dStaY - 2 * dt + DGN_RBAR_SIZE)*(i / (dSubBarNum - 1));  pt.y = dDb2 - DGN_RBAR_SIZE / 2;  arLPt.Add(pt);
		pt.x = dStaY + dt - DGN_RBAR_SIZE / 2 + (dEndY - dStaY - 2 * dt + DGN_RBAR_SIZE)*(i / (dSubBarNum - 1));  pt.y = dHc - dDt2 + DGN_RBAR_SIZE / 2;  arLPt.Add(pt);
		MappingCoord(arLPt, arGPt);
		Draw_StirrupBar(pDC, arGPt, bFill, FALSE);
	}

	///////////////////////////////////////////////////////////////////////////
	// Rebar
	for (int i = 0; i < DGN_RBAR_LAYER_BEAM; i++)
	{
		// Covering.
		double dDT = pRchkD->BEAM.dDt[nIMJ][i] * DGNDRAW_UFAC;
		double dDB = pRchkD->BEAM.dDb[nIMJ][i] * DGNDRAW_UFAC;
		// Top, Bot (StaY, EndY, StaZ, EndZ).
		if (dDT > 0.0)
		{
			arGPt.RemoveAll();
			Draw_LineChkRebar(TRUE, nIMJ, 1, i, dStaY + dt, dEndY - dt, dHc - dDT, dHc - dDT, pRchkD, arGPt);
			if (pRchkD->BEAM.bTopGroup[nIMJ][i]) // Group
			{
				double dGapFromCenter = pRchkD->BEAM.dTopGroupGap[nIMJ][i] * DGNDRAW_UFAC + GetLPByGPForLength(m_nRebarSize);
				for (int j = 0; j < pRchkD->BEAM.nTopGroupNum[nIMJ][i] - 1; j++)
				{
					Draw_LineChkRebar(TRUE, nIMJ, 1, i, dStaY + dt, dEndY - dt, dHc - dDT - dGapFromCenter * (j + 1), dHc - dDT - dGapFromCenter * (j + 1), pRchkD, arGPt);
				}
			}
			Draw_Rebar(pDC, arGPt, bFill);
		}
		if (dDB > 0.0)
		{
			arGPt.RemoveAll();
			Draw_LineChkRebar(TRUE, nIMJ, 2, i, dStaY + dt, dEndY - dt, dDB, dDB, pRchkD, arGPt);
			if (pRchkD->BEAM.bBotGroup[nIMJ][i]) // Group
			{
				double dGapFromCenter = pRchkD->BEAM.dBotGroupGap[nIMJ][i] * DGNDRAW_UFAC + GetLPByGPForLength(m_nRebarSize);
				for (int j = 0; j < pRchkD->BEAM.nBotGroupNum[nIMJ][i] - 1; j++)
				{
					Draw_LineChkRebar(TRUE, nIMJ, 2, i, dStaY + dt, dEndY - dt, dDB + dGapFromCenter * (j + 1), dDB + dGapFromCenter * (j + 1), pRchkD, arGPt);
				}
			}
			Draw_Rebar(pDC, arGPt, bFill);
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (!bDim) return;
	if (pRchkD->BEAM.dDb[nIMJ][0] == 0) return;

	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = dStaY - 0.3*dBc;  pt.y = 0; arLPt.Add(pt);
	pt.x = dStaY - 0.3*dBc;  pt.y = pRchkD->BEAM.dDb[nIMJ][0] * DGNDRAW_UFAC + DGN_RBAR_SIZE / 2; arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pRchkD->BEAM.dDt[nIMJ][0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}
void CDgnSectionDrawFunc::Draw_ChkRebar_Beam_Tee(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPBE_D* pCpbeD, BOOL bDim, BOOL bFill)
{
	if (pCpbeD == NULL)   // Data를 넘겨주지 않을경우
	{
		ASSERT(0);
		return;
	}
	ASSERT(pCpbeD);

	T_SECT_D SectD; SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);

	double dHc = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dBc = SectBaseD.Size[1] * DGNDRAW_UFAC;
	double dTw = SectBaseD.Size[2] * DGNDRAW_UFAC;
	double dTf = SectBaseD.Size[3] * DGNDRAW_UFAC;
	BOOL bEndCrossMark;

	double dStaY = (dBc - dTw) / 2.0;
	double dEndY = (dBc + dTw) / 2.0;

	double dt = 10;
	if (pCpbeD->dDt[nIMJ][0] > pCpbeD->dDb[nIMJ][0]) dt = pCpbeD->dDb[nIMJ][0] * DGNDRAW_UFAC;
	else                                            dt = pCpbeD->dDt[nIMJ][0] * DGNDRAW_UFAC;

	if (dt <= 0) return;

	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;
	CArray<CPoint, CPoint> arGPt;

	///////////////////////////////////////////////////////////////////////////
	// Rebar
	for (int i = 0; i < DGN_RBAR_LAYER_BEAM; i++)
	{
		// Covering.
		double dDT = pCpbeD->dDt[nIMJ][i] * DGNDRAW_UFAC;
		double dDB = pCpbeD->dDb[nIMJ][i] * DGNDRAW_UFAC;
		// Top, Bot (StaY, EndY, StaZ, EndZ).
		if (dDT > 0.0)
		{
			arGPt.RemoveAll();
			if (pCpbeD->bNum)
				Draw_LineChkRebar(TRUE, nIMJ, 1, i, dStaY + dt, dEndY - dt, dHc - dDT, dHc - dDT, pCpbeD, arGPt, bEndCrossMark);
			else
				Draw_LineChkRebar(TRUE, nIMJ, 1, i, dStaY, dEndY, dHc - dDT, dHc - dDT, pCpbeD, arGPt, bEndCrossMark);

			Draw_Rebar(pDC, arGPt, bFill, bEndCrossMark);
		}
		if (dDB > 0.0)
		{
			arGPt.RemoveAll();
			if (pCpbeD->bNum)
				Draw_LineChkRebar(TRUE, nIMJ, 2, i, dStaY + dt, dEndY - dt, dDB, dDB, pCpbeD, arGPt, bEndCrossMark);
			else
				Draw_LineChkRebar(TRUE, nIMJ, 2, i, dStaY, dEndY, dDB, dDB, pCpbeD, arGPt, bEndCrossMark);

			Draw_Rebar(pDC, arGPt, bFill, bEndCrossMark);
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (!bDim) return;
	if (pCpbeD->dDb[nIMJ][0] == 0) return;

	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = dStaY - 0.3*dBc;  pt.y = 0;                                    arLPt.Add(pt);
	pt.x = dStaY - 0.3*dBc;  pt.y = pCpbeD->dDb[nIMJ][0] * DGNDRAW_UFAC;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pCpbeD->dDt[nIMJ][0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}


//--------------------------------------------------------------------------
// Draw_ChkRebar_Beam_PSC_MID
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_ChkRebar_Beam_PSC_MID(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPBE_D* pCpbeD, BOOL bDim, BOOL bFill)
{
	if (pCpbeD == NULL)   // Data를 넘겨주지 않을경우
	{
		ASSERT(0);
		return;
	}
	ASSERT(pCpbeD);

	T_SECT_D SectD; SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);

	double dHL1 = SectBaseD.Size[1];
	double dHR1 = SectBaseD.Size[16];
	double dBL2 = SectBaseD.Size[11];
	double dBR2 = SectBaseD.Size[26];

	double dHc = (dHL1 + dHR1) / 2.0 * DGNDRAW_UFAC;
	double dB1 = (dBL2 + dBR2)     * DGNDRAW_UFAC;
	BOOL bEndCrossMark;

	double dt = 10;
	if (pCpbeD->dDt[nIMJ][0] > 0 && pCpbeD->dDb[nIMJ][0] <= 0) dt = pCpbeD->dDt[nIMJ][0] * DGNDRAW_UFAC;
	else if (pCpbeD->dDt[nIMJ][0] <= 0 && pCpbeD->dDb[nIMJ][0] > 0) dt = pCpbeD->dDb[nIMJ][0] * DGNDRAW_UFAC;
	else if (pCpbeD->dDt[nIMJ][0] > pCpbeD->dDb[nIMJ][0])      dt = pCpbeD->dDb[nIMJ][0] * DGNDRAW_UFAC;
	else                                                      dt = pCpbeD->dDt[nIMJ][0] * DGNDRAW_UFAC;

	if (dt <= 0) return;

	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;
	CArray<CPoint, CPoint> arGPt;

	///////////////////////////////////////////////////////////////////////////
	// Rebar
	for (int i = 0; i < DGN_RBAR_LAYER_BEAM; i++)
	{
		double dDT = pCpbeD->dDt[nIMJ][i] * DGNDRAW_UFAC;
		double dDB = pCpbeD->dDb[nIMJ][i] * DGNDRAW_UFAC;

		if (dDT > 0.0)
		{
			arGPt.RemoveAll();
			if (pCpbeD->bNum)
				Draw_LineChkRebar(TRUE, nIMJ, 1, i, dt, dB1 - dt, dHc - dDT, dHc - dDT, pCpbeD, arGPt, bEndCrossMark);
			else
				Draw_LineChkRebar(TRUE, nIMJ, 1, i, 0.0, dB1, dHc - dDT, dHc - dDT, pCpbeD, arGPt, bEndCrossMark);

			Draw_Rebar(pDC, arGPt, bFill, bEndCrossMark);
		}
		if (dDB > 0.0)
		{
			arGPt.RemoveAll();
			if (pCpbeD->bNum)
				Draw_LineChkRebar(TRUE, nIMJ, 2, i, dt, dB1 - dt, dDB, dDB, pCpbeD, arGPt, bEndCrossMark);
			else
				Draw_LineChkRebar(TRUE, nIMJ, 2, i, 0.0, dB1, dDB, dDB, pCpbeD, arGPt, bEndCrossMark);

			Draw_Rebar(pDC, arGPt, bFill, bEndCrossMark);
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (bDim == FALSE) return;

	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);
	if (pCpbeD->dDb[nIMJ][0] != 0)
	{
		arLPt.RemoveAll();  arGPt.RemoveAll();
		pt.x = -0.2*dB1;    pt.y = 0;                                      arLPt.Add(pt);
		pt.x = -0.2*dB1;    pt.y = pCpbeD->dDb[nIMJ][0] * DGNDRAW_UFAC;      arLPt.Add(pt);
		MappingCoord(arLPt, arGPt);
		str.Format(_T("%g"), pCpbeD->dDb[nIMJ][0] * dLen);
		Draw_Dimension(pDC, arGPt, str);
	}
	if (pCpbeD->dDt[nIMJ][0] != 0)
	{
		arLPt.RemoveAll();  arGPt.RemoveAll();
		pt.x = -0.2*dB1;    pt.y = dHc - pCpbeD->dDt[nIMJ][0] * DGNDRAW_UFAC;  arLPt.Add(pt);
		pt.x = -0.2*dB1;    pt.y = dHc;                                    arLPt.Add(pt);
		MappingCoord(arLPt, arGPt);
		str.Format(_T("%g"), pCpbeD->dDt[nIMJ][0] * dLen);
		Draw_Dimension(pDC, arGPt, str);
	}
}

//--------------------------------------------------------------------------
// Draw_ChkRebar_Colm
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_ChkRebar_Colm(CDC* pDC, T_SECT_K SectK, T_RCHK_D* pRchkD, BOOL bDim, BOOL bFill)
{
	if (pRchkD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRchkD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRchk(SectK, m_tmpRchkD);
		if (bGetOK) pRchkD = &m_tmpRchkD;
		else       return;
	}
	ASSERT(pRchkD);

	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_B)         Draw_ChkRebar_Colm_Rect(pDC, SectK, pRchkD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SB)   Draw_ChkRebar_Colm_Rect(pDC, SectK, pRchkD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_P)    Draw_ChkRebar_Colm_Circ(pDC, SectK, pRchkD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SR)   Draw_ChkRebar_Colm_Circ(pDC, SectK, pRchkD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_OCT)  Draw_ChkRebar_Colm_Octa(pDC, SectK, pRchkD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SOCT) Draw_ChkRebar_Colm_Octa(pDC, SectK, pRchkD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_TRK)  Draw_ChkRebar_Colm_Trck(pDC, SectK, pRchkD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_STRK) Draw_ChkRebar_Colm_Trck(pDC, SectK, pRchkD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_HTRK) Draw_ChkRebar_Colm_Htrk(pDC, SectK, pRchkD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_H) Draw_ChkRebar_Colm_H(pDC, SectK, pRchkD, bDim, bFill);
}
// T_CPCO_D 용 (SHIN '06.03.20 추가)
void CDgnSectionDrawFunc::Draw_ChkRebar_Colm(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPCO_D* pCpcoD, BOOL bDim, BOOL bFill)
{
	if (pCpcoD == NULL)   // Data를 넘겨주지 않을경우
	{
		ASSERT(0);
		return;
	}
	ASSERT(pCpcoD);

	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_B)         Draw_ChkRebar_Colm_Rect(pDC, SectK, nIMJ, pCpcoD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SB)   Draw_ChkRebar_Colm_Rect(pDC, SectK, nIMJ, pCpcoD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_P)    Draw_ChkRebar_Colm_Circ(pDC, SectK, nIMJ, pCpcoD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SR)   Draw_ChkRebar_Colm_Circ(pDC, SectK, nIMJ, pCpcoD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_OCT)  Draw_ChkRebar_Colm_Octa(pDC, SectK, nIMJ, pCpcoD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SOCT) Draw_ChkRebar_Colm_Octa(pDC, SectK, nIMJ, pCpcoD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_TRK)  Draw_ChkRebar_Colm_Trck(pDC, SectK, nIMJ, pCpcoD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_STRK) Draw_ChkRebar_Colm_Trck(pDC, SectK, nIMJ, pCpcoD, bDim, bFill);
	else if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_HTRK) Draw_ChkRebar_Colm_Htrk(pDC, SectK, nIMJ, pCpcoD, bDim, bFill);
}

//--------------------------------------------------------------------------
// Draw_ChkRebar_Colm_H
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_ChkRebar_Colm_H(CDC* pDC, T_SECT_K SectK, T_RCHK_D* pRchkD, BOOL bDim, BOOL bFill)
{
	if (pRchkD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRchkD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRchk(SectK, m_tmpRchkD);
		if (bGetOK) pRchkD = &m_tmpRchkD;
		else       return;
	}
	ASSERT(pRchkD);

	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
		
	double dH = SectD.SectBefore.SectI.Size[0] * DGNDRAW_UFAC;
	double dB1 = SectD.SectBefore.SectI.Size[1] * DGNDRAW_UFAC;
	double dB2 = SectD.SectBefore.SectI.Size[4] * DGNDRAW_UFAC;
	if (fabs(dB2) < CMathFunc::m_NormalZero) dB2 = dB1;

	CArray<CPoint, CPoint> arGPt;
	///////////////////////////////////////////////////////////////////////////
	// Rebar

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	for (int m = 0; m < 2; m++)
	{
		for (int i = 0; i < DGN_RBAR_LAYER; i++)
		{
			double dDch = pRchkD->COLM.dDc[0][0] * DGNDRAW_UFAC;
			double dDc = pRchkD->COLM.dDc[0][i] * DGNDRAW_UFAC;
			if (dDc <= 0.0)	break;

			double temp = fabs(dB2 - dB1) / 2;

			arGPt.RemoveAll();
			//Draw_LineChkRebar(TRUE,  m, 1, i, dB1-dDc,     dDc, dH-dDc, dH-dDc, pRchkD, arGPt);
			Draw_LineChkRebar(TRUE, m, 1, i, dB2 - temp - dDch, dDch + temp, dH - dDc, dH - dDc, pRchkD, arGPt);
			Draw_Rebar(pDC, arGPt, bFill);

			arGPt.RemoveAll();
			Draw_LineChkRebar(TRUE, m, 2, i, dDch, dB2 - dDch, dDc, dDc, pRchkD, arGPt);
			Draw_Rebar(pDC, arGPt, bFill);
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (!bDim) return;
	if (pRchkD->COLM.dDc[0][0] == 0) return;

	CArray<CPoint, CPoint>  arLPt;
	CPoint pt;
	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -0.2*dB1; pt.y = 0;  arLPt.Add(pt);
	pt.x = -0.2*dB1; pt.y = pRchkD->COLM.dDc[0][0] * DGNDRAW_UFAC;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pRchkD->COLM.dDc[0][0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}

//--------------------------------------------------------------------------
// Draw_ChkRebar_Colm_Rect
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_ChkRebar_Colm_Rect(CDC* pDC, T_SECT_K SectK, T_RCHK_D* pRchkD, BOOL bDim, BOOL bFill)
{
	if (pRchkD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRchkD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRchk(SectK, m_tmpRchkD);
		if (bGetOK) pRchkD = &m_tmpRchkD;
		else       return;
	}
	ASSERT(pRchkD);

	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	double dHc = SectD.SectBefore.SectI.Size[0] * DGNDRAW_UFAC;
	double dB1 = SectD.SectBefore.SectI.Size[1] * DGNDRAW_UFAC;

	CArray<CPoint, CPoint> arGPt;
	///////////////////////////////////////////////////////////////////////////
	// Rebar

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	for (int m = 0; m < 2; m++)
	{
		for (int i = 0; i < DGN_RBAR_LAYER; i++)
		{
			double dDc = pRchkD->COLM.dDc[0][i] * DGNDRAW_UFAC;
			if (dDc <= 0.0)	break;

			arGPt.RemoveAll();
			Draw_LineChkRebar(TRUE, m, 1, i, dB1 - dDc, dDc, dHc - dDc, dHc - dDc, pRchkD, arGPt);
			Draw_Rebar(pDC, arGPt, bFill);

			arGPt.RemoveAll();
			Draw_LineChkRebar(FALSE, m, 2, i, dDc, dDc, dHc - dDc, dDc, pRchkD, arGPt);
			Draw_Rebar(pDC, arGPt, bFill);

			arGPt.RemoveAll();
			Draw_LineChkRebar(TRUE, m, 1, i, dDc, dB1 - dDc, dDc, dDc, pRchkD, arGPt);
			Draw_Rebar(pDC, arGPt, bFill);

			arGPt.RemoveAll();
			Draw_LineChkRebar(FALSE, m, 2, i, dB1 - dDc, dB1 - dDc, dDc, dHc - dDc, pRchkD, arGPt);
			Draw_Rebar(pDC, arGPt, bFill);
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (!bDim) return;
	if (pRchkD->COLM.dDc[0][0] == 0) return;

	CArray<CPoint, CPoint>  arLPt;
	CPoint pt;
	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -0.2*dB1; pt.y = 0;  arLPt.Add(pt);
	pt.x = -0.2*dB1; pt.y = pRchkD->COLM.dDc[0][0] * DGNDRAW_UFAC;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pRchkD->COLM.dDc[0][0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}
// T_CPCO_D 용 (SHIN '06.03.20 추가)
void CDgnSectionDrawFunc::Draw_ChkRebar_Colm_Rect(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPCO_D* pCpcoD, BOOL bDim, BOOL bFill)
{
	if (pCpcoD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		return;
	}
	ASSERT(pCpcoD);

	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);

	double dHc = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dB1 = SectBaseD.Size[1] * DGNDRAW_UFAC;

	CArray<CPoint, CPoint> arGPt;
	///////////////////////////////////////////////////////////////////////////
	// Rebar
	for (int i = 0; i < DGN_RBAR_LAYER; i++)
	{
		double dDc = pCpcoD->dDc[nIMJ][i] * DGNDRAW_UFAC;
		if (dDc <= 0.0)	break;

		arGPt.RemoveAll();
		Draw_LineChkRebar(TRUE, nIMJ, 1, i, dB1 - dDc, dDc, dHc - dDc, dHc - dDc, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(FALSE, nIMJ, 2, i, dDc, dDc, dHc - dDc, dDc, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(TRUE, nIMJ, 1, i, dDc, dB1 - dDc, dDc, dDc, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(FALSE, nIMJ, 2, i, dB1 - dDc, dB1 - dDc, dDc, dHc - dDc, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (!bDim) return;
	if (pCpcoD->dDc[nIMJ][0] == 0) return;

	CArray<CPoint, CPoint>  arLPt;
	CPoint pt;
	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -0.2*dB1; pt.y = 0;  arLPt.Add(pt);
	pt.x = -0.2*dB1; pt.y = pCpcoD->dDc[nIMJ][0] * DGNDRAW_UFAC;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pCpcoD->dDc[nIMJ][0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}

//--------------------------------------------------------------------------
// Draw_ChkRebar_Colm_Circ
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_ChkRebar_Colm_Circ(CDC* pDC, T_SECT_K SectK, T_RCHK_D* pRchkD, BOOL bDim, BOOL bFill)
{
	if (pRchkD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRchkD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRchk(SectK, m_tmpRchkD);
		if (bGetOK) pRchkD = &m_tmpRchkD;
		else       return;
	}
	ASSERT(pRchkD);

	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	double dHc = SectD.SectBefore.SectI.Size[0] * DGNDRAW_UFAC;

	CArray<CPoint, CPoint> arGPt;
	///////////////////////////////////////////////////////////////////////////
	// Rebar

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	for (int i = 0; i < DGN_RBAR_LAYER; i++)
	{
		double dDc = pRchkD->COLM.dDc[0][i] * DGNDRAW_UFAC;
		if (dDc <= 0.0) break;

		arGPt.RemoveAll();
		Draw_CircChkRebar(1, i, dHc / 2.0, dHc / 2.0, dHc - dDc, dHc / 2.0, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (!bDim) return;
	if (pRchkD->COLM.dDc[0][0] == 0) return;

	CPoint pt;
	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	CArray<CPoint, CPoint> arLPt;
	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -0.2*dHc;  pt.y = 0;                                    arLPt.Add(pt);
	pt.x = -0.2*dHc;  pt.y = pRchkD->COLM.dDc[0][0] * DGNDRAW_UFAC + DGN_RBAR_SIZE / 2;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pRchkD->COLM.dDc[0][0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}
// T_CPCO_D 용 (SHIN '06.03.20 추가)
void CDgnSectionDrawFunc::Draw_ChkRebar_Colm_Circ(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPCO_D* pCpcoD, BOOL bDim, BOOL bFill)
{
	if (pCpcoD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		return;
	}
	ASSERT(pCpcoD);

	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);

	double dHc = SectBaseD.Size[0] * DGNDRAW_UFAC;

	CArray<CPoint, CPoint> arGPt;
	///////////////////////////////////////////////////////////////////////////
	// Rebar
	for (int i = 0; i < DGN_RBAR_LAYER; i++)
	{
		double dDc = pCpcoD->dDc[nIMJ][i] * DGNDRAW_UFAC;
		if (dDc <= 0.0) break;

		arGPt.RemoveAll();
		Draw_CircChkRebar(nIMJ, 1, i, dHc / 2.0, dHc / 2.0, dHc - dDc, dHc / 2.0, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (!bDim) return;
	if (pCpcoD->dDc[nIMJ][0] == 0) return;

	CPoint pt;
	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	CArray<CPoint, CPoint> arLPt;
	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -0.2*dHc;  pt.y = 0;                                    arLPt.Add(pt);
	pt.x = -0.2*dHc;  pt.y = pCpcoD->dDc[nIMJ][0] * DGNDRAW_UFAC + DGN_RBAR_SIZE / 2;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pCpcoD->dDc[nIMJ][0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}

//--------------------------------------------------------------------------
// Draw_ChkRebar_Colm_Octa
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_ChkRebar_Colm_Octa(CDC* pDC, T_SECT_K SectK, T_RCHK_D* pRchkD, BOOL bDim, BOOL bFill)
{
	if (pRchkD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRchkD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRchk(SectK, m_tmpRchkD);
		if (bGetOK) pRchkD = &m_tmpRchkD;
		else       return;
	}
	ASSERT(pRchkD);

	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	double dH = SectD.SectBefore.SectI.Size[0] * DGNDRAW_UFAC;
	double dB = SectD.SectBefore.SectI.Size[1] * DGNDRAW_UFAC;
	double da = SectD.SectBefore.SectI.Size[2] * DGNDRAW_UFAC;
	double db = SectD.SectBefore.SectI.Size[3] * DGNDRAW_UFAC;
	double dH1 = 0.0, dB1 = 0.0, da1 = 0.0, db1 = 0.0, dDist0 = 0.0, dTheta1 = 0.0, dt = 0.0;

	double dCenY = dB / 2.0;
	double dCenZ = dH / 2.0;
	double dPi = 4.*atan(1.0);

	CArray<CPoint, CPoint> arGPt;
	if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_OCT)
	{
		dt = SectD.SectBefore.SectI.Size[4];

		T_SECT_SECTBASE_D SectData;
		SectData.Initialize();
		SectData.Size[0] = dH;
		SectData.Size[1] = dB;
		SectData.Size[2] = da;
		SectData.Size[3] = db;
		SectData.Size[4] = dt;

		if (!m_pDoc->m_pSectDB->CalcSectRegular_InOCT(SectData, dH1, dB1, da1, db1, dDist0, dTheta1))	ASSERT(0);
	}

	///////////////////////////////////////////////////////////////////////////
	// Rebar

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	for (int i = 0; i < DGN_RBAR_LAYER; i++)
	{
		double dY1, dZ1, dY2, dZ2, dTheta;
		double dDc = pRchkD->COLM.dDc[0][i] * DGNDRAW_UFAC;

		if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_OCT)
		{
			dY1 = (dB / 2.0 - da) - ((dB / 2.0 - da) - (dB1 / 2.0 - da1))*(dDc / dt);
			dZ1 = dH / 2.0 - dDc;
			dY2 = dB / 2.0 - dDc;
			dZ2 = (dH / 2.0 - db) - ((dH / 2.0 - db) - (dH1 / 2.0 - db1))*(dDc / dt);
		}
		else
		{
			dTheta = (dPi - atan(db / da)) / 2.0;	// Radian.
			dY1 = (dB / 2.0 - da) - dDc / tan(dTheta);
			dZ1 = dH / 2.0 - dDc;
			dY2 = dB / 2.0 - dDc;
			dZ2 = (dH / 2.0 - db) - dDc / tan(dTheta);
		}
		if (dDc <= 0.0)	break;

		arGPt.RemoveAll();
		Draw_LineChkRebar(TRUE, 1, 1, i, dY1 + dCenY, -dY1 + dCenY, dZ1 + dCenZ, dZ1 + dCenZ, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(FALSE, 1, 2, i, -dY1 + dCenY, -dY2 + dCenY, dZ1 + dCenZ, dZ2 + dCenZ, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(TRUE, 1, 3, i, -dY2 + dCenY, -dY2 + dCenY, dZ2 + dCenZ, -dZ2 + dCenZ, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(FALSE, 1, 2, i, -dY2 + dCenY, -dY1 + dCenY, -dZ2 + dCenZ, -dZ1 + dCenZ, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(TRUE, 1, 1, i, -dY1 + dCenY, dY1 + dCenY, -dZ1 + dCenZ, -dZ1 + dCenZ, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(FALSE, 1, 2, i, dY1 + dCenY, dY2 + dCenY, -dZ1 + dCenZ, -dZ2 + dCenZ, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(TRUE, 1, 3, i, dY2 + dCenY, dY2 + dCenY, -dZ2 + dCenZ, dZ2 + dCenZ, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(FALSE, 1, 2, i, dY2 + dCenY, dY1 + dCenY, dZ2 + dCenZ, dZ1 + dCenZ, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (!bDim) return;
	if (pRchkD->COLM.dDc[0][0] == 0) return;

	CPoint pt;
	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	CArray<CPoint, CPoint> arLPt;
	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -0.2*dB;  pt.y = dH;                                      arLPt.Add(pt);
	pt.x = -0.2*dB;  pt.y = dH - pRchkD->COLM.dDc[0][0] * DGNDRAW_UFAC + DGN_RBAR_SIZE / 2;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pRchkD->COLM.dDc[0][0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}
// T_CPCO_D 용 (SHIN '06.03.20 추가)
void CDgnSectionDrawFunc::Draw_ChkRebar_Colm_Octa(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPCO_D* pCpcoD, BOOL bDim, BOOL bFill)
{
	if (pCpcoD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		return;
	}
	ASSERT(pCpcoD);

	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);


	double dH = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dB = SectBaseD.Size[1] * DGNDRAW_UFAC;
	double da = SectBaseD.Size[2] * DGNDRAW_UFAC;
	double db = SectBaseD.Size[3] * DGNDRAW_UFAC;
	double dH1 = 0.0, dB1 = 0.0, da1 = 0.0, db1 = 0.0, dDist0 = 0.0, dTheta1 = 0.0, dt = 0.0;

	double dCenY = dB / 2.0;
	double dCenZ = dH / 2.0;
	double dPi = 4.*atan(1.0);

	CArray<CPoint, CPoint> arGPt;
	if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_OCT)
	{
		dt = SectBaseD.Size[4];

		T_SECT_SECTBASE_D SectData;
		SectData.Initialize();
		SectData.Size[0] = dH;
		SectData.Size[1] = dB;
		SectData.Size[2] = da;
		SectData.Size[3] = db;
		SectData.Size[4] = dt;

		if (!m_pDoc->m_pSectDB->CalcSectRegular_InOCT(SectData, dH1, dB1, da1, db1, dDist0, dTheta1))	ASSERT(0);
	}

	///////////////////////////////////////////////////////////////////////////
	// Rebar
	for (int i = 0; i < DGN_RBAR_LAYER; i++)
	{
		double dY1, dZ1, dY2, dZ2, dTheta;
		double dDc = pCpcoD->dDc[nIMJ][i] * DGNDRAW_UFAC;

		if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_OCT)
		{
			dY1 = (dB / 2.0 - da) - ((dB / 2.0 - da) - (dB1 / 2.0 - da1))*(dDc / dt);
			dZ1 = dH / 2.0 - dDc;
			dY2 = dB / 2.0 - dDc;
			dZ2 = (dH / 2.0 - db) - ((dH / 2.0 - db) - (dH1 / 2.0 - db1))*(dDc / dt);
		}
		else
		{
			dTheta = (dPi - atan(db / da)) / 2.0;	// Radian.
			dY1 = (dB / 2.0 - da) - dDc / tan(dTheta);
			dZ1 = dH / 2.0 - dDc;
			dY2 = dB / 2.0 - dDc;
			dZ2 = (dH / 2.0 - db) - dDc / tan(dTheta);
		}
		if (dDc <= 0.0)	break;

		arGPt.RemoveAll();
		Draw_LineChkRebar(TRUE, nIMJ, 1, i, dY1 + dCenY, -dY1 + dCenY, dZ1 + dCenZ, dZ1 + dCenZ, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(FALSE, nIMJ, 2, i, -dY1 + dCenY, -dY2 + dCenY, dZ1 + dCenZ, dZ2 + dCenZ, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(TRUE, nIMJ, 3, i, -dY2 + dCenY, -dY2 + dCenY, dZ2 + dCenZ, -dZ2 + dCenZ, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(FALSE, nIMJ, 2, i, -dY2 + dCenY, -dY1 + dCenY, -dZ2 + dCenZ, -dZ1 + dCenZ, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(TRUE, nIMJ, 1, i, -dY1 + dCenY, dY1 + dCenY, -dZ1 + dCenZ, -dZ1 + dCenZ, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(FALSE, nIMJ, 2, i, dY1 + dCenY, dY2 + dCenY, -dZ1 + dCenZ, -dZ2 + dCenZ, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(TRUE, nIMJ, 3, i, dY2 + dCenY, dY2 + dCenY, -dZ2 + dCenZ, dZ2 + dCenZ, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(FALSE, nIMJ, 2, i, dY2 + dCenY, dY1 + dCenY, dZ2 + dCenZ, dZ1 + dCenZ, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (!bDim) return;
	if (pCpcoD->dDc[nIMJ][0] == 0) return;

	CPoint pt;
	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	CArray<CPoint, CPoint> arLPt;
	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -0.2*dB;  pt.y = dH;                                      arLPt.Add(pt);
	pt.x = -0.2*dB;  pt.y = dH - pCpcoD->dDc[nIMJ][0] * DGNDRAW_UFAC + DGN_RBAR_SIZE / 2;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pCpcoD->dDc[nIMJ][0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}


//--------------------------------------------------------------------------
// Draw_ChkRebar_Colm_Trck
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_ChkRebar_Colm_Trck(CDC* pDC, T_SECT_K SectK, T_RCHK_D* pRchkD, BOOL bDim, BOOL bFill)
{
	if (pRchkD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRchkD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRchk(SectK, m_tmpRchkD);
		if (bGetOK) pRchkD = &m_tmpRchkD;
		else       return;
	}
	ASSERT(pRchkD);

	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	double dH = SectD.SectBefore.SectI.Size[0] * DGNDRAW_UFAC;
	double dB = SectD.SectBefore.SectI.Size[1] * DGNDRAW_UFAC;

	double dCenY = dB / 2.0;
	double dCenZ = dH / 2.0;

	CArray<CPoint, CPoint> arGPt;
	///////////////////////////////////////////////////////////////////////////
	// Rebar

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	for (int i = 0; i < DGN_RBAR_LAYER; i++)
	{
		double dDc = pRchkD->COLM.dDc[0][i] * DGNDRAW_UFAC;
		if (dDc <= 0.0)	break;

		arGPt.RemoveAll();
		Draw_LineChkRebar(TRUE, 1, 1, i, ((dB - dH) / 2.0) + dCenY, -((dB - dH) / 2.0) + dCenY, dH - dDc, dH - dDc, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_CirLChkRebar(1, 2, i, dCenY, dCenY, dH - dDc, dCenZ, dB - dH, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(TRUE, 1, 1, i, -((dB - dH) / 2.0) + dCenY, ((dB - dH) / 2.0) + dCenY, dDc, dDc, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_CirRChkRebar(1, 2, i, dCenY, dCenY, dDc, dCenZ, dB - dH, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (!bDim) return;
	if (pRchkD->COLM.dDc[0][0] == 0) return;

	CPoint pt;
	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	CArray<CPoint, CPoint> arLPt;
	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -0.2*dB;  pt.y = dH;  arLPt.Add(pt);
	pt.x = -0.2*dB;  pt.y = dH - pRchkD->COLM.dDc[0][0] * DGNDRAW_UFAC + DGN_RBAR_SIZE / 2;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pRchkD->COLM.dDc[0][0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}
// T_CPCO_D 용 (SHIN '06.03.20 추가)
void CDgnSectionDrawFunc::Draw_ChkRebar_Colm_Trck(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPCO_D* pCpcoD, BOOL bDim, BOOL bFill)
{
	if (pCpcoD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		return;
	}
	ASSERT(pCpcoD);

	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);

	double dH = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dB = SectBaseD.Size[1] * DGNDRAW_UFAC;

	double dCenY = dB / 2.0;
	double dCenZ = dH / 2.0;

	CArray<CPoint, CPoint> arGPt;
	///////////////////////////////////////////////////////////////////////////
	// Rebar
	for (int i = 0; i < DGN_RBAR_LAYER; i++)
	{
		double dDc = pCpcoD->dDc[nIMJ][i] * DGNDRAW_UFAC;
		if (dDc <= 0.0)	break;

		arGPt.RemoveAll();
		Draw_LineChkRebar(TRUE, nIMJ, 1, i, ((dB - dH) / 2.0) + dCenY, -((dB - dH) / 2.0) + dCenY, dH - dDc, dH - dDc, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_CirLChkRebar(nIMJ, 2, i, dCenY, dCenY, dH - dDc, dCenZ, dB - dH, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(TRUE, nIMJ, 1, i, -((dB - dH) / 2.0) + dCenY, ((dB - dH) / 2.0) + dCenY, dDc, dDc, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_CirRChkRebar(nIMJ, 2, i, dCenY, dCenY, dDc, dCenZ, dB - dH, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (!bDim) return;
	if (pCpcoD->dDc[nIMJ][0] == 0) return;

	CPoint pt;
	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	CArray<CPoint, CPoint> arLPt;
	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -0.2*dB;  pt.y = dH;  arLPt.Add(pt);
	pt.x = -0.2*dB;  pt.y = dH - pCpcoD->dDc[nIMJ][0] * DGNDRAW_UFAC + DGN_RBAR_SIZE / 2;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pCpcoD->dDc[nIMJ][0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}

//--------------------------------------------------------------------------
// Draw_ChkRebar_Colm_Htrk
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_ChkRebar_Colm_Htrk(CDC* pDC, T_SECT_K SectK, T_RCHK_D* pRchkD, BOOL bDim, BOOL bFill)
{
	if (pRchkD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		m_tmpRchkD.Initialize();
		BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetRchk(SectK, m_tmpRchkD);
		if (bGetOK) pRchkD = &m_tmpRchkD;
		else       return;
	}
	ASSERT(pRchkD);

	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;

	double dH = SectD.SectBefore.SectI.Size[0] * DGNDRAW_UFAC;
	double dB = SectD.SectBefore.SectI.Size[1] * DGNDRAW_UFAC;

	double dCenY = (dB - dH / 2.0) / 2.0;
	double dCenZ = dH / 2.0;

	CArray<CPoint, CPoint> arGPt;
	///////////////////////////////////////////////////////////////////////////
	// Rebar

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	for (int i = 0; i < DGN_RBAR_LAYER; i++)
	{
		// Covering.
		double dDc = pRchkD->COLM.dDc[0][i] * DGNDRAW_UFAC;
		if (dDc <= 0.0)	break;
		// Top, Left, Bot, Right (StaY, EndY, StaZ, EndZ).

		arGPt.RemoveAll();
		Draw_LineChkRebar(TRUE, 1, 1, i, (dB - dH / 2.0), dDc, dH - dDc, dH - dDc, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(FALSE, 1, 3, i, dDc, dDc, dH - dDc, dDc, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(TRUE, 1, 1, i, dDc, (dB - dH / 2.0), dDc, dDc, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_CirRChkRebar(1, 2, i, dCenY, dCenY, dDc, dCenZ, dB - dH / 2.0, pRchkD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (!bDim) return;
	if (pRchkD->COLM.dDc[0][0] == 0) return;

	CPoint pt;
	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	CArray<CPoint, CPoint> arLPt;
	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -0.2*dB;  pt.y = dH;  arLPt.Add(pt);
	pt.x = -0.2*dB;  pt.y = dH - pRchkD->COLM.dDc[0][0] * DGNDRAW_UFAC + DGN_RBAR_SIZE / 2;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pRchkD->COLM.dDc[0][0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}
// T_CPCO_D 용 (SHIN '06.03.20 추가)
void CDgnSectionDrawFunc::Draw_ChkRebar_Colm_Htrk(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPCO_D* pCpcoD, BOOL bDim, BOOL bFill)
{
	if (pCpcoD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		return;
	}
	ASSERT(pCpcoD);

	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);

	double dH = SectBaseD.Size[0] * DGNDRAW_UFAC;
	double dB = SectBaseD.Size[1] * DGNDRAW_UFAC;

	double dCenY = (dB - dH / 2.0) / 2.0;
	double dCenZ = dH / 2.0;

	CArray<CPoint, CPoint> arGPt;
	///////////////////////////////////////////////////////////////////////////
	// Rebar
	for (int i = 0; i < DGN_RBAR_LAYER; i++)
	{
		// Covering.
		double dDc = pCpcoD->dDc[nIMJ][i] * DGNDRAW_UFAC;
		if (dDc <= 0.0)	break;
		// Top, Left, Bot, Right (StaY, EndY, StaZ, EndZ).

		arGPt.RemoveAll();
		Draw_LineChkRebar(TRUE, nIMJ, 1, i, (dB - dH / 2.0), dDc, dH - dDc, dH - dDc, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(FALSE, nIMJ, 3, i, dDc, dDc, dH - dDc, dDc, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_LineChkRebar(TRUE, nIMJ, 1, i, dDc, (dB - dH / 2.0), dDc, dDc, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);

		arGPt.RemoveAll();
		Draw_CirRChkRebar(nIMJ, 2, i, dCenY, dCenY, dDc, dCenZ, dB - dH / 2.0, pCpcoD, arGPt);
		Draw_Rebar(pDC, arGPt, bFill);
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (!bDim) return;
	if (pCpcoD->dDc[nIMJ][0] == 0) return;

	CPoint pt;
	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);

	CArray<CPoint, CPoint> arLPt;
	arLPt.RemoveAll();  arGPt.RemoveAll();
	pt.x = -0.2*dB;  pt.y = dH;  arLPt.Add(pt);
	pt.x = -0.2*dB;  pt.y = dH - pCpcoD->dDc[nIMJ][0] * DGNDRAW_UFAC + DGN_RBAR_SIZE / 2;  arLPt.Add(pt);
	MappingCoord(arLPt, arGPt);
	str.Format(_T("%g"), pCpcoD->dDc[nIMJ][0] * dLen);
	Draw_Dimension(pDC, arGPt, str);
}

//--------------------------------------------------------------------------
// Draw_ChkRebar_Colm_Gen
//--------------------------------------------------------------------------
void CDgnSectionDrawFunc::Draw_ChkRebar_Colm_Gen(CDC* pDC, T_SECT_K SectK, int nIMJ, T_RBTE_D* pRbteD, BOOL bDim, BOOL bFill)
{
	if (pRbteD == NULL)   // Data를 넘겨주지 않을경우 : Get From DB
	{
		return;
	}
	ASSERT(pRbteD);

	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);

	double dCenY = SectBaseD.Design.YBar * DGNDRAW_UFAC;
	double dCenZ = SectBaseD.Design.ZBar * DGNDRAW_UFAC;

	///////////////////////////////////////////////////////////////////////////
	// Rebar

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	T_RBTE_MABR RbteMabr;
	int iPosi;
	if (nIMJ == 0 || pRbteD->bIJBothMain)
		iPosi = 0;
	else if (nIMJ == 2)
		iPosi = 2;
	else
	{
		ASSERT(0); return;
	}

	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;      arLPt.RemoveAll();
	CArray<CPoint, CPoint> arLPt_Off;  arLPt_Off.RemoveAll();
	CArray<CPoint, CPoint> arGPt;      arGPt.RemoveAll();

	int iRbarNum = pRbteD->arMainRebar[iPosi].GetSize();
	for (int i = 0; i < iRbarNum; i++)
	{
		RbteMabr = pRbteD->arMainRebar[iPosi].GetAt(i);
		pt.x = RbteMabr.dCenterY * DGNDRAW_UFAC;
		pt.y = RbteMabr.dCenterZ * DGNDRAW_UFAC;
		arLPt.Add(pt);
	}
	OffsetCoord(arLPt, arLPt_Off, dCenY, dCenZ);
	MappingCoord(arLPt_Off, arGPt);
	Draw_Rebar(pDC, arGPt, bFill);

	///////////////////////////////////////////////////////////////////////////
	// Dimension
}


// T_CPWA_D & _DGNC_CPPL_D 용 (SHIN '07.07.01 추가)
void CDgnSectionDrawFunc::Draw_ChkRebar(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPWA_D* pCpwaD, BOOL bDim, BOOL bFill, double dHaunch)
{
	ASSERT(pDC);
	if (pCpwaD == NULL)   // Data를 넘겨주지 않을경우
	{
		ASSERT(0);
		return;
	}
	ASSERT(pCpwaD);

	if (pCpwaD->bUnitWidth)
		Draw_ChkRebar_Wall_UWidth(pDC, SectK, nIMJ, pCpwaD, bDim, bFill, dHaunch);
	else
		Draw_ChkRebar_Wall_Rect(pDC, SectK, nIMJ, pCpwaD, bDim, bFill, dHaunch);
}

void CDgnSectionDrawFunc::Draw_ChkRebar(CDC* pDC, T_THIK_K ThikK, _DGNC_CPPL_D* pCpplD, BOOL bDim, BOOL bFill, int nTypeRebarView)
{
	ASSERT(pDC);
	if (pCpplD == NULL)   // Data를 넘겨주지 않을경우
	{
		ASSERT(0);
		return;
	}
	ASSERT(pCpplD);

	Draw_ChkRebar_Plate_UWidth(pDC, ThikK, pCpplD, bDim, bFill, nTypeRebarView);
}

void CDgnSectionDrawFunc::Draw_ChkRebar_Wall_Rect(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPWA_D* pCpwaD, BOOL bDim, BOOL bFill, double dHaunch)
{
	if (pCpwaD == NULL)   // Data를 넘겨주지 않을경우
	{
		ASSERT(0);
		return;
	}
	ASSERT(pCpwaD);

	T_SECT_D SectD; SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);

	double dHc = (SectBaseD.Size[0] + dHaunch) * DGNDRAW_UFAC;
	double dB1 = SectBaseD.Size[1] * DGNDRAW_UFAC;
	BOOL bEndCrossMark;

	double dt = 10;
	if (pCpwaD->dDt[nIMJ][0] > 0 && pCpwaD->dDb[nIMJ][0] <= 0) dt = pCpwaD->dDt[nIMJ][0] * DGNDRAW_UFAC;
	else if (pCpwaD->dDt[nIMJ][0] <= 0 && pCpwaD->dDb[nIMJ][0] > 0) dt = pCpwaD->dDb[nIMJ][0] * DGNDRAW_UFAC;
	else if (pCpwaD->dDt[nIMJ][0] > pCpwaD->dDb[nIMJ][0])      dt = pCpwaD->dDb[nIMJ][0] * DGNDRAW_UFAC;
	else                                                      dt = pCpwaD->dDt[nIMJ][0] * DGNDRAW_UFAC;

	if (dt <= 0) return;

	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;
	CArray<CPoint, CPoint> arGPt;

	///////////////////////////////////////////////////////////////////////////
	// Rebar
	for (int i = 0; i < DGN_RBAR_LAYER; i++)
	{
		double dDT = pCpwaD->dDt[nIMJ][i] * DGNDRAW_UFAC;
		double dDB = pCpwaD->dDb[nIMJ][i] * DGNDRAW_UFAC;

		if (dDT > 0.0)
		{
			arGPt.RemoveAll();
			if (pCpwaD->bNum)
				//Draw_LineChkRebar(TRUE, nIMJ, 1, i, dt, dB1-dt, dHc-dDT, dHc-dDT, pCpwaD, arGPt, bEndCrossMark);
				Draw_LineChkRebar(TRUE, nIMJ, 1, i, dB1*0.1, dB1*0.9, dHc - dDT, dHc - dDT, pCpwaD, arGPt, bEndCrossMark);  // by SeungJun (`06.03.29) 양사이드의 피복두께는 일정하게 유지하도록..
			else
				Draw_LineChkRebar(TRUE, nIMJ, 1, i, 0.0, dB1, dHc - dDT, dHc - dDT, pCpwaD, arGPt, bEndCrossMark);

			Draw_Rebar(pDC, arGPt, bFill, bEndCrossMark);
		}
		if (dDB > 0.0)
		{
			arGPt.RemoveAll();
			if (pCpwaD->bNum)
				//Draw_LineChkRebar(TRUE, nIMJ, 2, i, dt, dB1-dt,     dDB,     dDB, pCpwaD, arGPt, bEndCrossMark);
				Draw_LineChkRebar(TRUE, nIMJ, 2, i, dB1*0.1, dB1*0.9, dDB, dDB, pCpwaD, arGPt, bEndCrossMark);  // by SeungJun (`06.03.29) 양사이드의 피복두께는 일정하게 유지하도록..
			else
				Draw_LineChkRebar(TRUE, nIMJ, 2, i, 0.0, dB1, dDB, dDB, pCpwaD, arGPt, bEndCrossMark);

			Draw_Rebar(pDC, arGPt, bFill, bEndCrossMark);
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (bDim == FALSE) return;

	CString str;
	double dLen = Cng_UnitLen(m_nLengthUnit);
	if (pCpwaD->dDb[nIMJ][0] != 0)
	{
		arLPt.RemoveAll();  arGPt.RemoveAll();
		pt.x = -0.2*dB1;    pt.y = 0;                                      arLPt.Add(pt);
		pt.x = -0.2*dB1;    pt.y = pCpwaD->dDb[nIMJ][0] * DGNDRAW_UFAC;      arLPt.Add(pt);
		MappingCoord(arLPt, arGPt);
		str.Format(_T("%g"), pCpwaD->dDb[nIMJ][0] * dLen);
		Draw_Dimension(pDC, arGPt, str);
	}
	if (pCpwaD->dDt[nIMJ][0] != 0)
	{
		arLPt.RemoveAll();  arGPt.RemoveAll();
		pt.x = -0.2*dB1;    pt.y = dHc - pCpwaD->dDt[nIMJ][0] * DGNDRAW_UFAC;  arLPt.Add(pt);
		pt.x = -0.2*dB1;    pt.y = dHc;                                    arLPt.Add(pt);
		MappingCoord(arLPt, arGPt);
		str.Format(_T("%g"), pCpwaD->dDt[nIMJ][0] * dLen);
		Draw_Dimension(pDC, arGPt, str);
	}
}

void CDgnSectionDrawFunc::Draw_ChkRebar_Wall_UWidth(CDC* pDC, T_SECT_K SectK, int nIMJ, T_CPWA_D* pCpwaD, BOOL bDim, BOOL bFill, double dHaunch)
{
	if (pCpwaD == NULL)   // Data를 넘겨주지 않을경우
	{
		ASSERT(0);
		return;
	}
	ASSERT(pCpwaD);

	T_SECT_D SectD; SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return;
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);

	double dLen = Cng_UnitLen(m_nLengthUnit);
	double dLenUnitWidth = Cng_UnitLen(m_nWidthLengthUnit);

	double dHc = (SectBaseD.Size[0] + dHaunch) * DGNDRAW_UFAC;
	double dB1 = (dLenUnitWidth == 0 ? 1.0 : 1.0 / dLenUnitWidth) * DGNDRAW_UFAC;
	BOOL bEndCrossMark;

	double dt = 10;
	if (pCpwaD->dDt[nIMJ][0] > 0 && pCpwaD->dDb[nIMJ][0] <= 0) dt = pCpwaD->dDt[nIMJ][0] * DGNDRAW_UFAC;
	else if (pCpwaD->dDt[nIMJ][0] <= 0 && pCpwaD->dDb[nIMJ][0] > 0) dt = pCpwaD->dDb[nIMJ][0] * DGNDRAW_UFAC;
	else if (pCpwaD->dDt[nIMJ][0] > pCpwaD->dDb[nIMJ][0])      dt = pCpwaD->dDb[nIMJ][0] * DGNDRAW_UFAC;
	else                                                      dt = pCpwaD->dDt[nIMJ][0] * DGNDRAW_UFAC;

	if (dt <= 0) return;

	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;
	CArray<CPoint, CPoint> arGPt;

	///////////////////////////////////////////////////////////////////////////
	// Rebar
	for (int i = 0; i < DGN_RBAR_LAYER; i++)
	{
		double dDT = pCpwaD->dDt[nIMJ][i] * DGNDRAW_UFAC;
		double dDB = pCpwaD->dDb[nIMJ][i] * DGNDRAW_UFAC;

		if (dDT > 0.0)
		{
			arGPt.RemoveAll();
			Draw_LineChkRebar(TRUE, nIMJ, 1, i, 0.0, dB1, dHc - dDT, dHc - dDT, pCpwaD, arGPt, bEndCrossMark);
			Draw_Rebar(pDC, arGPt, bFill, bEndCrossMark);
		}
		if (dDB > 0.0)
		{
			arGPt.RemoveAll();
			Draw_LineChkRebar(TRUE, nIMJ, 2, i, 0.0, dB1, dDB, dDB, pCpwaD, arGPt, bEndCrossMark);
			Draw_Rebar(pDC, arGPt, bFill, bEndCrossMark);
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (bDim == FALSE) return;

	CString str;
	dLen = Cng_UnitLen(m_nLengthUnit);
	if (pCpwaD->dDb[nIMJ][0] != 0)
	{
		arLPt.RemoveAll();  arGPt.RemoveAll();
		pt.x = -0.15*dB1;    pt.y = 0;                                      arLPt.Add(pt);
		pt.x = -0.15*dB1;    pt.y = pCpwaD->dDb[nIMJ][0] * DGNDRAW_UFAC;      arLPt.Add(pt);
		MappingCoord(arLPt, arGPt);
		str.Format(_T("%g"), pCpwaD->dDb[nIMJ][0] * dLen);
		Draw_Dimension(pDC, arGPt, str);
	}
	if (pCpwaD->dDt[nIMJ][0] != 0)
	{
		arLPt.RemoveAll();  arGPt.RemoveAll();
		pt.x = -0.15*dB1;    pt.y = dHc - pCpwaD->dDt[nIMJ][0] * DGNDRAW_UFAC;  arLPt.Add(pt);
		pt.x = -0.15*dB1;    pt.y = dHc;                                    arLPt.Add(pt);
		MappingCoord(arLPt, arGPt);
		str.Format(_T("%g"), pCpwaD->dDt[nIMJ][0] * dLen);
		Draw_Dimension(pDC, arGPt, str);
	}
}

void CDgnSectionDrawFunc::Draw_ChkRebar_Plate_UWidth(CDC* pDC, T_THIK_K ThikK, _DGNC_CPPL_D* pCpplD, BOOL bDim, BOOL bFill, int nTypeRebarView)
{
	if (pCpplD == NULL)   // Data를 넘겨주지 않을경우
	{
		ASSERT(0);
		return;
	}
	ASSERT(pCpplD);

	double   dThik = 0.0;
	T_THIK_D ThikD; ThikD.Initialize();
	m_pDoc->m_pAttrCtrl->GetThik(ThikK, ThikD);
	m_pDoc->m_pAttrCtrl->GetThikForDesign(ThikD, dThik);

	double dLen = Cng_UnitLen(m_nLengthUnit);
	double dLenUnitWidth = Cng_UnitLen(m_nWidthLengthUnit);

	double dHc = dThik * DGNDRAW_UFAC;
	double dB1 = (dLenUnitWidth == 0 ? 1.0 : 1.0 / dLenUnitWidth) * DGNDRAW_UFAC;
	BOOL bEndCrossMark;

	double dt = 10;
	if (pCpplD->dDt[0] > 0 && pCpplD->dDb[0] <= 0)  dt = pCpplD->dDt[0] * DGNDRAW_UFAC;
	else if (pCpplD->dDt[0] <= 0 && pCpplD->dDb[0] > 0)  dt = pCpplD->dDb[0] * DGNDRAW_UFAC;
	else if (pCpplD->dDt[0] > pCpplD->dDb[0])            dt = pCpplD->dDb[0] * DGNDRAW_UFAC;
	else                                                dt = pCpplD->dDt[0] * DGNDRAW_UFAC;

	if (dt <= 0) return;

	CPoint  pt;
	CArray<CPoint, CPoint> arLPt;
	CArray<CPoint, CPoint> arGPt;

	///////////////////////////////////////////////////////////////////////////
	// Rebar
	for (int i = 0; i < DGN_RBAR_LAYER; i++)
	{
		double dDT = pCpplD->dDt[i] * DGNDRAW_UFAC;
		double dDB = pCpplD->dDb[i] * DGNDRAW_UFAC;

		if (dDT > 0.0 && (nTypeRebarView == 0 || nTypeRebarView == 1))
		{
			arGPt.RemoveAll();
			Draw_LineChkRebar(TRUE, 1, i, 0.0, dB1, dHc - dDT, dHc - dDT, pCpplD, arGPt, bEndCrossMark);
			Draw_Rebar(pDC, arGPt, bFill, bEndCrossMark);
		}
		if (dDB > 0.0 && (nTypeRebarView == 0 || nTypeRebarView == 2))
		{
			arGPt.RemoveAll();
			Draw_LineChkRebar(TRUE, 2, i, 0.0, dB1, dDB, dDB, pCpplD, arGPt, bEndCrossMark);
			Draw_Rebar(pDC, arGPt, bFill, bEndCrossMark);
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Dimension
	if (bDim == FALSE) return;

	CString str;
	dLen = Cng_UnitLen(m_nLengthUnit);
	if (pCpplD->dDb[0] != 0.0 && (nTypeRebarView == 0 || nTypeRebarView == 2))
	{
		arLPt.RemoveAll();  arGPt.RemoveAll();
		pt.x = -0.15*dB1;    pt.y = 0;                                arLPt.Add(pt);
		pt.x = -0.15*dB1;    pt.y = pCpplD->dDb[0] * DGNDRAW_UFAC;      arLPt.Add(pt);
		MappingCoord(arLPt, arGPt);
		str.Format(_T("%g"), pCpplD->dDb[0] * dLen);
		Draw_Dimension(pDC, arGPt, str);
	}
	if (pCpplD->dDt[0] != 0.0 && (nTypeRebarView == 0 || nTypeRebarView == 1))
	{
		arLPt.RemoveAll();  arGPt.RemoveAll();
		pt.x = -0.15*dB1;    pt.y = dHc - pCpplD->dDt[0] * DGNDRAW_UFAC;  arLPt.Add(pt);
		pt.x = -0.15*dB1;    pt.y = dHc;                              arLPt.Add(pt);
		MappingCoord(arLPt, arGPt);
		str.Format(_T("%g"), pCpplD->dDt[0] * dLen);
		Draw_Dimension(pDC, arGPt, str);
	}
}




//--------------------------------------------------------------------------
// Draw_LineChkRebar
//--------------------------------------------------------------------------
BOOL CDgnSectionDrawFunc::Draw_LineChkRebar(BOOL bIncEdge, int iPosi, int iGrup, int iBarLay, double dStaY, double dEndY,
	double dStaZ, double dEndZ, T_RCHK_D* pRchkD, CArray<CPoint, CPoint>& rGPt)
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	int iDivNum = 5;
	if (pRchkD->nType == 0)	    iDivNum = (iGrup == 1 ? pRchkD->BEAM.iTopBarNum[iPosi][iBarLay] : pRchkD->BEAM.iBotBarNum[iPosi][iBarLay]);
	else if (pRchkD->nType == 1)	iDivNum = pRchkD->COLM.iBarNum[0][iGrup - 1][iBarLay];
	else	ASSERT(0);

	return Draw_LineChkRebar(bIncEdge, dStaY, dEndY, dStaZ, dEndZ, iDivNum, rGPt);
}
// T_CPCO_D 용 (SHIN '06.03.20 추가)
BOOL CDgnSectionDrawFunc::Draw_LineChkRebar(BOOL bIncEdge, int iPosi, int iGrup, int iBarLay, double dStaY, double dEndY,
	double dStaZ, double dEndZ, T_CPCO_D* pCpcoD, CArray<CPoint, CPoint>& rGPt)
{
	int iDivNum = 5;
	iDivNum = pCpcoD->iBarNum[iPosi][iGrup - 1][iBarLay];

	return Draw_LineChkRebar(bIncEdge, dStaY, dEndY, dStaZ, dEndZ, iDivNum, rGPt);
}

// 공통 용 (SHIN '06.03.20 추가)
BOOL CDgnSectionDrawFunc::Draw_LineChkRebar(BOOL bIncEdge, double dStaY, double dEndY, double dStaZ, double dEndZ,
	int iDivNum, CArray<CPoint, CPoint>& rGPt)
{
	double dTotLen = sqrt(pow(dStaY - dEndY, 2) + pow(dStaZ - dEndZ, 2));
	if (fabs(dTotLen) < 0)	return FALSE;

	// If Beam, +1.
	CPoint pt;
	CArray<CPoint, CPoint> arLPt; arLPt.RemoveAll();

	//int iTotDivNum = (pRchkD->nType==0 ? iDivNum-1 : iDivNum);

	// changing the setting rebar form by SeungJun (`06.02.24)
	// if bIncEdge is TURE, with the rebar of the edge.
	// if bIncEdge is FALSE, without the rebar of the edge.
	int iTotDivNum = (bIncEdge ? iDivNum - 1 : iDivNum + 1);
	double RebarSeq = 0.0;

	// add by SeungJun (`06.03.29)
	if (iDivNum < CMathFunc::m_NormalZero || iTotDivNum < CMathFunc::m_NormalZero) return FALSE;
	//if(iDivNum == 0) return FALSE;

	for (int i = 0; i < iDivNum; i++)
	{
		RebarSeq = (bIncEdge ? (double)i : (double)i + 1);
		pt.x = dStaY + (dEndY - dStaY)*(RebarSeq / iTotDivNum);
		pt.y = dStaZ + (dEndZ - dStaZ)*(RebarSeq / iTotDivNum);
		arLPt.Add(pt);
	}

	MappingCoord(arLPt, rGPt);

	return TRUE;
}
// T_CPBE_D 용 (SHIN '06.03.27 추가)
BOOL CDgnSectionDrawFunc::Draw_LineChkRebar(BOOL bIncEdge, int iPosi, int iGrup, int iBarLay, double dStaY, double dEndY,
	double dStaZ, double dEndZ, T_CPBE_D* pCpbeD, CArray<CPoint, CPoint>& rGPt, BOOL& bEndCrossMark)
{
	bEndCrossMark = FALSE;//초기화
	double dTotLen = CMathFunc::mathLength(dStaY, dStaZ, dEndY, dEndZ);
	if (fabs(dTotLen) < 0)	return FALSE;

	CPoint pt;
	CArray<CPoint, CPoint> arLPt; arLPt.RemoveAll();

	double dDivNum = 1.;
	if (pCpbeD->bNum)
		dDivNum = (iGrup == 1 ? pCpbeD->dTopBarNum[iPosi][iBarLay] : pCpbeD->dBotBarNum[iPosi][iBarLay]);
	else
	{
		double dCTC = (iGrup == 1 ? pCpbeD->dTopBarCTC[iPosi][iBarLay] : pCpbeD->dBotBarCTC[iPosi][iBarLay]) * DGNDRAW_UFAC;
		if (dCTC < CMathFunc::m_NormalZero) return FALSE;
		dDivNum = dTotLen / dCTC + 1.0;
	}

	double dTotDivNum = (bIncEdge ? dDivNum - 1.0 : dDivNum + 1.0);
	double RebarSeq = 0.0;

	if (dDivNum < CMathFunc::m_NormalZero || dTotDivNum < CMathFunc::m_NormalZero) return FALSE;

	double dLen = 0.0;

	int iNum = int(dDivNum) + 1;

	double dX, dY, dDivLen;
	dX = dY = dDivLen = 0.0;
	for (int i = 0; i < iNum; i++)
	{
		RebarSeq = (bIncEdge ? (double)i : (double)i + 1);
		dX = dStaY + (dEndY - dStaY)*(RebarSeq / dTotDivNum);
		dY = dStaZ + (dEndZ - dStaZ)*(RebarSeq / dTotDivNum);
		dDivLen = CMathFunc::mathLength(dStaY, dStaZ, dX, dY);
		if (dDivLen > dTotLen + CMathFunc::m_NormalZero)
			break;
		pt.x = dX;    pt.y = dY;
		arLPt.Add(pt);
	}
	int count = arLPt.GetSize();
	if (count <= 0) return FALSE;

	dDivLen = CMathFunc::mathLength(dStaY, dStaZ, arLPt[count - 1].x, arLPt[count - 1].y);
	if (dDivLen < dTotLen - CMathFunc::m_NormalZero)
	{
		bEndCrossMark = TRUE;
		pt.x = dEndY;    pt.y = dEndZ;
		arLPt.Add(pt);
	}

	MappingCoord(arLPt, rGPt);

	return TRUE;
}

BOOL CDgnSectionDrawFunc::Draw_LineChkRebar(BOOL bIncEdge, int iPosi, int iGrup, int iBarLay, double dStaY, double dEndY, double dStaZ, double dEndZ,
	T_CPWA_D* pCpwaD, CArray<CPoint, CPoint>& rGPt, BOOL& bEndCrossMark)
{
	bEndCrossMark = FALSE;//초기화
	double dTotLen = CMathFunc::mathLength(dStaY, dStaZ, dEndY, dEndZ);
	if (fabs(dTotLen) < 0)	return FALSE;

	CPoint pt;
	CArray<CPoint, CPoint> arLPt; arLPt.RemoveAll();

	double dDivNum = 1.;
	if (pCpwaD->bNum)
	{
		dDivNum = (iGrup == 1 ? pCpwaD->dTopBarNum[iPosi][iBarLay] : pCpwaD->dBotBarNum[iPosi][iBarLay]);
		if (pCpwaD->bUnitWidth) dDivNum += 1;
	}
	else
	{
		double dCTC = (iGrup == 1 ? pCpwaD->dTopBarCTC[iPosi][iBarLay] : pCpwaD->dBotBarCTC[iPosi][iBarLay]) * DGNDRAW_UFAC;
		if (dCTC < CMathFunc::m_NormalZero) return FALSE;
		dDivNum = dTotLen / dCTC + 1.0;
	}

	double dTotDivNum = (bIncEdge ? dDivNum - 1.0 : dDivNum + 1.0);
	double RebarSeq = 0.0;

	if (dDivNum < CMathFunc::m_NormalZero || dTotDivNum < CMathFunc::m_NormalZero) return FALSE;

	double dLen = 0.0;

	int iNum = int(dDivNum) + 1;

	double dX, dY, dDivLen;
	dX = dY = dDivLen = 0.0;
	for (int i = 0; i < iNum; i++)
	{
		RebarSeq = (bIncEdge ? (double)i : (double)i + 1);
		dX = dStaY + (dEndY - dStaY)*(RebarSeq / dTotDivNum);
		dY = dStaZ + (dEndZ - dStaZ)*(RebarSeq / dTotDivNum);
		dDivLen = CMathFunc::mathLength(dStaY, dStaZ, dX, dY);
		if (dDivLen > dTotLen + CMathFunc::m_NormalZero)
			break;
		pt.x = dX;    pt.y = dY;
		arLPt.Add(pt);
	}
	int count = arLPt.GetSize();
	if (count <= 0) return FALSE;

	dDivLen = CMathFunc::mathLength(dStaY, dStaZ, arLPt[count - 1].x, arLPt[count - 1].y);
	if (dDivLen < dTotLen - CMathFunc::m_NormalZero)
	{
		bEndCrossMark = TRUE;
		pt.x = dEndY;    pt.y = dEndZ;
		arLPt.Add(pt);
	}

	MappingCoord(arLPt, rGPt);

	return TRUE;
}

BOOL CDgnSectionDrawFunc::Draw_LineChkRebar(BOOL bIncEdge, int iGrup, int iBarLay, double dStaY, double dEndY, double dStaZ, double dEndZ,
	_DGNC_CPPL_D* pCpplD, CArray<CPoint, CPoint>& rGPt, BOOL& bEndCrossMark)
{
	bEndCrossMark = FALSE;//초기화
	double dTotLen = CMathFunc::mathLength(dStaY, dStaZ, dEndY, dEndZ);
	if (fabs(dTotLen) < 0)	return FALSE;

	CPoint pt;
	CArray<CPoint, CPoint> arLPt; arLPt.RemoveAll();

	double dDivNum = 1.;
	if (pCpplD->bNum)
	{
		dDivNum = (iGrup == 1 ? pCpplD->dTopBarNum[iBarLay] : pCpplD->dBotBarNum[iBarLay]);
		/*if(pCpplD->bUnitWidth)*/ dDivNum += 1;//Plate는 무조건 단위폭 설계
	}
	else
	{
		double dCTC = (iGrup == 1 ? pCpplD->dTopBarCTC[iBarLay] : pCpplD->dBotBarCTC[iBarLay]) * DGNDRAW_UFAC;
		if (dCTC < CMathFunc::m_NormalZero) return FALSE;
		dDivNum = dTotLen / dCTC + 1.0;
	}

	double dTotDivNum = (bIncEdge ? dDivNum - 1.0 : dDivNum + 1.0);
	double RebarSeq = 0.0;

	if (dDivNum < CMathFunc::m_NormalZero || dTotDivNum < CMathFunc::m_NormalZero) return FALSE;

	double dLen = 0.0;

	int iNum = int(dDivNum) + 1;

	double dX, dY, dDivLen;
	dX = dY = dDivLen = 0.0;
	for (int i = 0; i < iNum; i++)
	{
		RebarSeq = (bIncEdge ? (double)i : (double)i + 1);
		dX = dStaY + (dEndY - dStaY)*(RebarSeq / dTotDivNum);
		dY = dStaZ + (dEndZ - dStaZ)*(RebarSeq / dTotDivNum);
		dDivLen = CMathFunc::mathLength(dStaY, dStaZ, dX, dY);
		if (dDivLen > dTotLen + CMathFunc::m_NormalZero)
			break;
		pt.x = dX;    pt.y = dY;
		arLPt.Add(pt);
	}
	int count = arLPt.GetSize();
	if (count <= 0) return FALSE;

	dDivLen = CMathFunc::mathLength(dStaY, dStaZ, arLPt[count - 1].x, arLPt[count - 1].y);
	if (dDivLen < dTotLen - CMathFunc::m_NormalZero)
	{
		bEndCrossMark = TRUE;
		pt.x = dEndY;    pt.y = dEndZ;
		arLPt.Add(pt);
	}

	MappingCoord(arLPt, rGPt);

	return TRUE;
}

//--------------------------------------------------------------------------
// Draw_CircChkRebar
//--------------------------------------------------------------------------
BOOL CDgnSectionDrawFunc::Draw_CircChkRebar(int iGrup, int iBarLay, double dStaY, double dCenY, double dStaZ, double dCenZ,
	T_RCHK_D* pRchkD, CArray<CPoint, CPoint>& rGPt)
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	int iDivNum = 0;
	iDivNum = pRchkD->COLM.iBarNum[0][iGrup - 1][iBarLay];
	return Draw_CircChkRebar(dStaY, dCenY, dStaZ, dCenZ, iDivNum, rGPt);
}
// T_CPCO_D 용 (SHIN '06.03.20 추가)
BOOL CDgnSectionDrawFunc::Draw_CircChkRebar(int iPosi, int iGrup, int iBarLay, double dStaY, double dCenY, double dStaZ, double dCenZ,
	T_CPCO_D* pCpcoD, CArray<CPoint, CPoint>& rGPt)
{
	int iDivNum = pCpcoD->iBarNum[iPosi][iGrup - 1][iBarLay];
	return Draw_CircChkRebar(dStaY, dCenY, dStaZ, dCenZ, iDivNum, rGPt);
}
// 공통 용 (SHIN '06.03.20 추가)
BOOL CDgnSectionDrawFunc::Draw_CircChkRebar(double dStaY, double dCenY, double dStaZ, double dCenZ,
	int iDivNum, CArray<CPoint, CPoint>& rGPt)
{
	double dRadius = sqrt(pow(dStaY - dCenY, 2) + pow(dStaZ - dCenZ, 2));
	if (fabs(dRadius) < 0)	return FALSE;

	double dPi = 4.*atan(1.0);

	if (iDivNum == 0) return FALSE;

	CPoint pt;
	CArray<CPoint, CPoint> arLPt; arLPt.RemoveAll();
	for (int i = 0; i < iDivNum; i++)
	{
		// Counterclockwise (from Top).
		pt.x = dCenY + (-1)*sin(2 * dPi*((double)i / iDivNum)) * dRadius;
		pt.y = dCenZ + cos(2 * dPi*((double)i / iDivNum)) * dRadius;
		arLPt.Add(pt);
	}

	MappingCoord(arLPt, rGPt);

	return TRUE;
}

//--------------------------------------------------------------------------
// Draw_CircChkRebar
//--------------------------------------------------------------------------
BOOL CDgnSectionDrawFunc::Draw_CirLChkRebar(int iPosi, int iGrup, int iBarLay, double dStaY, double dCenY, double dStaZ,
	double dCenZ, double dCenDist, T_RCHK_D* pRchkD, CArray<CPoint, CPoint>& rGPt)
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	int iDivNum = pRchkD->COLM.iBarNum[0][iGrup - 1][iBarLay];
	return Draw_CirLChkRebar(dStaY, dCenY, dStaZ, dCenZ, dCenDist, iDivNum, rGPt);
}
// T_CPCO_D 용 (SHIN '06.03.20 추가)
BOOL CDgnSectionDrawFunc::Draw_CirLChkRebar(int iPosi, int iGrup, int iBarLay, double dStaY, double dCenY, double dStaZ,
	double dCenZ, double dCenDist, T_CPCO_D* pCpcoD, CArray<CPoint, CPoint>& rGPt)
{
	int iDivNum = pCpcoD->iBarNum[iPosi][iGrup - 1][iBarLay];
	return Draw_CirLChkRebar(dStaY, dCenY, dStaZ, dCenZ, dCenDist, iDivNum, rGPt);
}
// 공통 용 (SHIN '06.03.20 추가)
BOOL CDgnSectionDrawFunc::Draw_CirLChkRebar(double dStaY, double dCenY, double dStaZ, double dCenZ, double dCenDist,
	int iDivNum, CArray<CPoint, CPoint>& rGPt)
{
	double dRadius = sqrt(pow(dStaY - dCenY, 2) + pow(dStaZ - dCenZ, 2));
	if (fabs(dRadius) < 0)	return FALSE;

	double dPi = 4.*atan(1.0);

	if (iDivNum == 0) return FALSE;

	CPoint pt;
	CArray<CPoint, CPoint> arLPt; arLPt.RemoveAll();
	for (int i = 0; i < iDivNum; i++)
	{
		// changing the setting rebar form by SeungJun (`06.02.24)
		// left circle, always without the rebar of the edge
		// so iDivNum+1
		pt.x = dCenY + (-1)*sin(dPi*(((double)i + 1.) / (iDivNum + 1))) * dRadius - (dCenDist / 2.0);
		pt.y = dCenZ + cos(dPi*(((double)i + 1.) / (iDivNum + 1))) * dRadius;
		arLPt.Add(pt);
	}
	MappingCoord(arLPt, rGPt);

	return TRUE;
}

//--------------------------------------------------------------------------
// Draw_CircChkRebar
//--------------------------------------------------------------------------
BOOL CDgnSectionDrawFunc::Draw_CirRChkRebar(int iPosi, int iGrup, int iBarLay, double dStaY, double dCenY, double dStaZ,
	double dCenZ, double dCenDist, T_RCHK_D* pRchkD, CArray<CPoint, CPoint>& rGPt)
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	int iDivNum = pRchkD->COLM.iBarNum[0][iGrup - 1][iBarLay];
	return Draw_CirRChkRebar(dStaY, dCenY, dStaZ, dCenZ, dCenDist, iDivNum, rGPt);
}
// T_CPCO_D 용 (SHIN '06.03.20 추가)
BOOL CDgnSectionDrawFunc::Draw_CirRChkRebar(int iPosi, int iGrup, int iBarLay, double dStaY, double dCenY, double dStaZ,
	double dCenZ, double dCenDist, T_CPCO_D* pCpcoD, CArray<CPoint, CPoint>& rGPt)
{
	int iDivNum = pCpcoD->iBarNum[iPosi][iGrup - 1][iBarLay];
	return Draw_CirRChkRebar(dStaY, dCenY, dStaZ, dCenZ, dCenDist, iDivNum, rGPt);
}
// 공통 용 (SHIN '06.03.20 추가)
BOOL CDgnSectionDrawFunc::Draw_CirRChkRebar(double dStaY, double dCenY, double dStaZ, double dCenZ, double dCenDist,
	int iDivNum, CArray<CPoint, CPoint>& rGPt)
{
	double dRadius = sqrt(pow(dStaY - dCenY, 2) + pow(dStaZ - dCenZ, 2));
	if (fabs(dRadius) < 0)	return FALSE;

	double dPi = 4.*atan(1.0);

	if (iDivNum == 0) return FALSE;

	CPoint pt;
	CArray<CPoint, CPoint> arLPt; arLPt.RemoveAll();
	for (int i = 0; i < iDivNum; i++)
	{
		// changing the setting rebar form by SeungJun (`06.02.24)
		// left circle, always without the rebar of the edge
		// so iDivNum+1
		pt.x = dCenY + sin(dPi*(((double)i + 1.) / (iDivNum + 1))) * dRadius + (dCenDist / 2.0);
		pt.y = dCenZ + (-1)*cos(dPi*(((double)i + 1.) / (iDivNum + 1))) * dRadius;
		arLPt.Add(pt);
	}
	MappingCoord(arLPt, rGPt);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Sub Routines
//

void CDgnSectionDrawFunc::Draw_DgnRebar(CDC* pDC, CArray<CPoint, CPoint>& arGPt, BOOL bFill)
{
	CPen*   pOldPen;
	CPen    LinePen(PS_SOLID, 4, RGB(128, 0, 0));

	pOldPen = pDC->SelectObject(&LinePen);

	int nSize = arGPt.GetSize();
	CPoint* pt = new CPoint[nSize];

	for (int i = 0; i < nSize; i++)
	{
		pt[i].x = arGPt.GetAt(i).x;
		pt[i].y = arGPt.GetAt(i).y;
	}

	pDC->Polyline(pt, nSize);

	pDC->SelectObject(&pOldPen);
	LinePen.DeleteObject();
	delete[] pt;
}
/*
void CDgnSectionDrawFunc::Draw_DgnLHCRebar()
{
}

void CDgnSectionDrawFunc::Draw_DgnHCRRebar()
{
}
*/
void CDgnSectionDrawFunc::Draw_DgnLRebar(CDC* pDC, CArray<CPoint, CPoint>& arGPt, BOOL bFill)
{
	CPen*   pOldPen;
	CPen    LinePen(PS_SOLID, 4, RGB(128, 0, 0));

	pOldPen = pDC->SelectObject(&LinePen);

	pDC->MoveTo(arGPt[0].x, arGPt[0].y);
	for (int i = 1; i < arGPt.GetSize(); i++) pDC->LineTo(arGPt[i].x, arGPt[i].y);

	pDC->SelectObject(&pOldPen);
	LinePen.DeleteObject();
}

void CDgnSectionDrawFunc::Draw_DgnCRebar(CDC* pDC, CArray<CPoint, CPoint>& arGPt, BOOL bFill)
{
	CPen*   pOldPen;
	CPen    LinePen(PS_SOLID, 4, RGB(128, 0, 0));

	pOldPen = pDC->SelectObject(&LinePen);

	pDC->Ellipse(arGPt[0].x, arGPt[0].y, arGPt[1].x, arGPt[1].y);

	pDC->SelectObject(&pOldPen);
	LinePen.DeleteObject();
}

void CDgnSectionDrawFunc::Draw_StirrupBar(CDC* pDC, CArray<CPoint, CPoint>& rGPt, BOOL bFill, BOOL bColse)
{
	CPen*   pOldPen;
	CPen    LinePen(PS_SOLID, 1, RGB(128, 128, 128));

	pOldPen = pDC->SelectObject(&LinePen);

	int nSize = rGPt.GetSize();
	ASSERT(nSize);

	pDC->MoveTo(rGPt[0].x, rGPt[0].y);
	for (int i = 1; i < nSize; i++) pDC->LineTo(rGPt[i].x, rGPt[i].y);
	if (bColse) pDC->LineTo(rGPt[0].x, rGPt[0].y);

	pDC->SelectObject(&pOldPen);
	LinePen.DeleteObject();
}

void CDgnSectionDrawFunc::Draw_Rebar(CDC* pDC, CArray<CPoint, CPoint>& rGPt, BOOL bFill, BOOL bEndCrossMark)
{
	CPen*   pOldPen;
	CBrush* pOldBrh;
	CPen    LinePen(PS_SOLID, 1, RGB(128, 0, 0));
	CBrush  FillBrush(RGB(128, 0, 0));
	CBrush  WhiteBrush(RGB(255, 255, 255));

	pOldPen = pDC->SelectObject(&LinePen);

	if (bFill) pOldBrh = pDC->SelectObject(&FillBrush);
	else      pOldBrh = pDC->SelectObject(&WhiteBrush);

	int nCount = rGPt.GetSize();

	if (bEndCrossMark)
		nCount = nCount - 1;

	int r = m_nRebarSize / 2;
	for (int i = 0; i < nCount; i++)
		pDC->Ellipse(rGPt[i].x - r, rGPt[i].y + r, rGPt[i].x + r, rGPt[i].y - r);

	if (bEndCrossMark && nCount > 0)
	{
		pDC->MoveTo(rGPt[nCount].x - r, rGPt[nCount].y);
		pDC->LineTo(rGPt[nCount].x + r, rGPt[nCount].y);
		pDC->MoveTo(rGPt[nCount].x, rGPt[nCount].y - r);
		pDC->LineTo(rGPt[nCount].x, rGPt[nCount].y + r);
	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrh);

	LinePen.DeleteObject();
	FillBrush.DeleteObject();
	WhiteBrush.DeleteObject();

}

void CDgnSectionDrawFunc::Draw_rConcrete(CDC* pDC, CArray<CPoint, CPoint>& rGPt, BOOL bFill)
{
	CPen*   pOldPen;
	CBrush* pOldBrh;
	CPen    LinePen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush  FillBrush(RGB(255, 253, 215));
	CBrush  WhiteBrush(RGB(255, 255, 255));

	pOldPen = pDC->SelectObject(&LinePen);

	if (bFill) pOldBrh = pDC->SelectObject(&FillBrush);
	else      pOldBrh = pDC->SelectObject(&WhiteBrush);

	pDC->Ellipse(rGPt[0].x, rGPt[0].y, rGPt[1].x, rGPt[1].y);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrh);

	LinePen.DeleteObject();
	FillBrush.DeleteObject();
	WhiteBrush.DeleteObject();
}

void CDgnSectionDrawFunc::Draw_Concrete(CDC* pDC, CArray<CPoint, CPoint>& rGPt, BOOL bFill)
{
	CPen*   pOldPen;
	CBrush* pOldBrh;
	CPen    LinePen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush  FillBrush(RGB(255, 253, 215));
	CBrush  WhiteBrush(RGB(255, 255, 255));

	pOldPen = pDC->SelectObject(&LinePen);

	if (bFill) pOldBrh = pDC->SelectObject(&FillBrush);
	else      pOldBrh = pDC->SelectObject(&WhiteBrush);

	int nSize = rGPt.GetSize();
	CPoint* pGPt = new CPoint[nSize];
	for (int i = 0; i < nSize; i++)
	{
		pGPt[i].x = rGPt.GetAt(i).x;
		pGPt[i].y = rGPt.GetAt(i).y;
	}

	pDC->Polygon(pGPt, nSize);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrh);

	LinePen.DeleteObject();
	FillBrush.DeleteObject();
	WhiteBrush.DeleteObject();
	delete[] pGPt;
}


void CDgnSectionDrawFunc::Draw_AxisYZ(CDC* pDC, CArray<CPoint, CPoint>& rGPt)
{
	CFont*  pOldFont;
	CFont   font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfClipPrecision = CLIP_LH_ANGLES;
	lf.lfWeight = FW_NORMAL;
	wcscpy(lf.lfFaceName, _T("Arial Narrow"));
	lf.lfHeight = m_nDimSize;
	lf.lfWidth = m_nDimSize / 2;

	VERIFY(font.CreateFontIndirect(&lf));
	pOldFont = pDC->SelectObject(&font);

	CString str;
	CPen*   pOldPen;
	CPen    LinePen(PS_SOLID, 1, RGB(0, 0, 0));

	pOldPen = pDC->SelectObject(&LinePen);

	if (m_nAxisType == 1 || m_nAxisType == 2)
	{
		// Axis_Y 
		pDC->MoveTo(rGPt[0].x, rGPt[0].y);
		pDC->LineTo(rGPt[1].x, rGPt[1].y);
		str.Format(_T("y"));
		if (m_nCanvasType == 0)
			pDC->TextOut(rGPt[1].x + m_nDimSize / 2, rGPt[1].y - m_nDimSize / 2, str);
		else
			pDC->TextOut(rGPt[1].x + m_nDimSize / 2, rGPt[1].y + m_nDimSize / 2, str);

		// Axis_Z
		pDC->MoveTo(rGPt[2].x, rGPt[2].y);
		pDC->LineTo(rGPt[3].x, rGPt[3].y);
		str.Format(_T("z"));
		if (m_nCanvasType == 0)
			pDC->TextOut(rGPt[3].x - m_nDimSize / 4, rGPt[3].y - m_nDimSize, str);
		else
			pDC->TextOut(rGPt[3].x - m_nDimSize / 4, rGPt[3].y + m_nDimSize, str);
	}

	CPen    DashPen(PS_DASH, 1, RGB(200, 0, 0));
	pDC->SelectObject(&DashPen);
	if (m_nAxisType == 2)
	{
		// NeutralAxis
		double dCenY = rGPt[2].x;
		double dCenZ = rGPt[0].y;
		double dYmax = max(rGPt[0].x, rGPt[1].x);
		double dYmin = min(rGPt[0].x, rGPt[1].x);
		double dZmax = max(rGPt[2].y, rGPt[3].y);
		double dZmin = min(rGPt[2].y, rGPt[3].y);

		double nAxisLine[2][2];
		double nLine1[2][2];
		double nLine2[2][2];
		nAxisLine[0][0] = dCenY + CMathFunc::mathCos(m_dAngle);
		nAxisLine[0][1] = dCenZ + CMathFunc::mathSin(m_dAngle);
		nAxisLine[1][0] = dCenY - CMathFunc::mathCos(m_dAngle);
		nAxisLine[1][1] = dCenZ - CMathFunc::mathSin(m_dAngle);
		double crossP1[2] = { dCenY, dCenZ };
		double crossP2[2] = { dCenY, dCenZ };

		// Y축과 평행한 상하단의 Line과의 교차점 검토
		nLine1[0][0] = dYmin;   nLine1[0][1] = dZmax;
		nLine1[1][0] = dYmax;   nLine1[1][1] = dZmax;
		nLine2[0][0] = dYmin;   nLine2[0][1] = dZmin;
		nLine2[1][0] = dYmax;   nLine2[1][1] = dZmin;

		if (CMathFunc::mathLineLineCross2D(nAxisLine, nLine1, crossP1) != 0 && CMathFunc::mathLineLineCross2D(nAxisLine, nLine2, crossP2) != 0)
		{
			if (crossP1[0] <= dYmax && crossP1[0] >= dYmin)
			{// crossP1 검토
				if (nAxisLine[0][1] > nAxisLine[1][1])// 가장 위쪽 점으로 대체
				{
					nAxisLine[0][0] = crossP1[0];   nAxisLine[0][1] = crossP1[1];
				}
				else
				{
					nAxisLine[1][0] = crossP1[0];   nAxisLine[1][1] = crossP1[1];
				}
			}

			if (crossP2[0] <= dYmax && crossP2[0] >= dYmin)
			{// crossP2 검토
				if (nAxisLine[0][1] <= nAxisLine[1][1])// 가장 아래쪽 점으로 대체
				{
					nAxisLine[0][0] = crossP2[0];   nAxisLine[0][1] = crossP2[1];
				}
				else
				{
					nAxisLine[1][0] = crossP2[0];   nAxisLine[1][1] = crossP2[1];
				}
			}
		}

		// Z축과 평행한 좌우측의 Line과의 교차점 검토
		nLine1[0][0] = dYmin;   nLine1[0][1] = dZmax;
		nLine1[1][0] = dYmin;   nLine1[1][1] = dZmin;
		nLine2[0][0] = dYmax;   nLine2[0][1] = dZmax;
		nLine2[1][0] = dYmax;   nLine2[1][1] = dZmin;

		if (CMathFunc::mathLineLineCross2D(nAxisLine, nLine1, crossP1) != 0 && CMathFunc::mathLineLineCross2D(nAxisLine, nLine2, crossP2) != 0)
		{
			if (crossP1[1] <= dZmax && crossP1[1] >= dZmin)
			{// crossP1 검토
				if (nAxisLine[0][0] < nAxisLine[1][0])// 가장 좌측 점으로 대체
				{
					nAxisLine[0][0] = crossP1[0];   nAxisLine[0][1] = crossP1[1];
				}
				else
				{
					nAxisLine[1][0] = crossP1[0];   nAxisLine[1][1] = crossP1[1];
				}
			}

			if (crossP2[1] <= dZmax && crossP2[1] >= dZmin)
			{// crossP2 검토
				if (nAxisLine[0][0] >= nAxisLine[1][0])// 가장 우측 점으로 대체
				{
					nAxisLine[0][0] = crossP2[0];   nAxisLine[0][1] = crossP2[1];
				}
				else
				{
					nAxisLine[1][0] = crossP2[0];   nAxisLine[1][1] = crossP2[1];
				}
			}
		}
		pDC->MoveTo(int(nAxisLine[0][0]), int(nAxisLine[0][1]));
		pDC->LineTo(int(nAxisLine[1][0]), int(nAxisLine[1][1]));

	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
	LinePen.DeleteObject();
	font.DeleteObject();
}


// 현재 DrawDimesion은 수직, 수평만 된다.
void CDgnSectionDrawFunc::Draw_Dimension(CDC* pDC, CArray<CPoint, CPoint>& rGPt, CString& strTxt)
{
	// Font
	CFont*  pOldFont;
	CFont   font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfClipPrecision = CLIP_LH_ANGLES;
	lf.lfWeight = FW_NORMAL;
	wcscpy(lf.lfFaceName, _T("Arial Narrow"));

	CPen*   pOldPen;
	CBrush* pOldBrh;
	CPen    DimPen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush  Brush(RGB(0, 0, 0));

	pOldPen = pDC->SelectObject(&DimPen);
	pOldBrh = pDC->SelectObject(&Brush);

	double dx = 0;
	double dy = 0;

	double cx = (rGPt[0].x + rGPt[1].x) / 2.0;
	double cy = (rGPt[0].y + rGPt[1].y) / 2.0;

	int nLen = strTxt.GetLength();

	if (fabs(rGPt[0].x - rGPt[1].x) < 0.1)       // 수직
	{
		dx = 6;   dy = 0;
		if (m_nCanvasType == 0)
			lf.lfEscapement = 900;
		else
			lf.lfEscapement = -900;

		lf.lfHeight = m_nDimSize;
		lf.lfWidth = m_nDimSize / 2;
		dx = m_nDimSize / 2;

		VERIFY(font.CreateFontIndirect(&lf));
		pOldFont = pDC->SelectObject(&font);

		if (m_nCanvasType == 0)
			pDC->TextOut(cx - m_nDimSize - max(1, m_nDimSize*0.1), cy + dx / 2.0*nLen, strTxt, nLen);
		else
			pDC->TextOut(cx - m_nDimSize - max(1, m_nDimSize*0.1), cy - dx / 2.0*nLen, strTxt, nLen);

	}
	else if (fabs(rGPt[0].y - rGPt[1].y) < 0.1) // 수평
	{
		dx = 0;   dy = 6;
		lf.lfOrientation = 0;
		lf.lfHeight = m_nDimSize;
		lf.lfWidth = m_nDimSize / 2;
		dy = m_nDimSize / 2;

		VERIFY(font.CreateFontIndirect(&lf));
		pOldFont = pDC->SelectObject(&font);

		if (m_nCanvasType == 0)
			pDC->TextOut(cx - dy / 2.0*nLen, cy - m_nDimSize - max(1, m_nDimSize*0.1), strTxt, nLen);
		else
			pDC->TextOut(cx - dy / 2.0*nLen, cy + m_nDimSize + max(1, m_nDimSize*0.1), strTxt, nLen);
	}

	pDC->MoveTo(rGPt[0].x, rGPt[0].y);
	pDC->LineTo(rGPt[1].x, rGPt[1].y);

	pDC->MoveTo(rGPt[0].x + dx, rGPt[0].y + dy);
	pDC->LineTo(rGPt[0].x - dx, rGPt[0].y - dy);

	pDC->MoveTo(rGPt[1].x + dx, rGPt[1].y + dy);
	pDC->LineTo(rGPt[1].x - dx, rGPt[1].y - dy);

	pDC->Ellipse(rGPt[0].x - 2, rGPt[0].y - 2, rGPt[0].x + 2, rGPt[0].y + 2);
	pDC->Ellipse(rGPt[1].x - 2, rGPt[1].y - 2, rGPt[1].x + 2, rGPt[1].y + 2);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrh);
	pDC->SelectObject(pOldFont);

	DimPen.DeleteObject();
	Brush.DeleteObject();
	font.DeleteObject();
}


BOOL CDgnSectionDrawFunc::CalcScaleFactor(double left, double top, double right, double bottom)
{

	double Gx = fabs(m_Canvas.right - m_Canvas.left)  * m_dZoomFactor;
	double Gy = fabs(m_Canvas.top - m_Canvas.bottom)* m_dZoomFactor;
	if (Gx == 0 || Gy == 0) return FALSE;

	double Lx = right - left;
	double Ly = top - bottom;
	if (Lx == 0 || Ly == 0) return FALSE;

	double Fx = 0.;
	double Fy = 0.;

	if (Lx != 0 && Ly != 0) {
		Fx = Gx / Lx;
		Fy = Gy / Ly;
	}
	else if (Lx != 0 && Ly == 0) {
		Fx = Gx / Lx;
		Fy = Fx;
	}
	else if (Lx == 0 && Ly != 0) {
		Fy = Gy / Ly;
		Fx = Fy;
	}

	m_ScaleF = min(Fx, Fy);

	if (m_nCanvasType == 0)
	{
		m_dX = (m_Canvas.right + m_Canvas.left) / 2.0 - (right + left) / 2.0*m_ScaleF - m_Canvas.left;
		m_dY = (m_Canvas.top + m_Canvas.bottom) / 2.0 - (top + bottom) / 2.0*m_ScaleF - m_Canvas.top;
	}
	else
	{
		m_dX = (m_Canvas.right + m_Canvas.left) / 2.0 - (right + left) / 2.0*m_ScaleF - m_Canvas.left;
		m_dY = (m_Canvas.top + m_Canvas.bottom) / 2.0 - (top + bottom) / 2.0*m_ScaleF - min(m_Canvas.bottom, m_Canvas.top);
	}


	/*
	double Gx = (m_Canvas.right - m_Canvas.left)  * m_dZoomFactor;
	double Gy = (m_Canvas.top   - m_Canvas.bottom)* m_dZoomFactor;
	if(Gx == 0 || Gy == 0) return FALSE;

	double Lx = right - left;
	double Ly = top   - bottom;
	if(Lx == 0 || Ly == 0) return FALSE;

	double Fx = 0.;
	double Fy = 0.;

	if(Lx != 0 && Ly != 0){
		Fx =  Gx / Lx;
		Fy = -Gy / Ly;
	}
	else if(Lx != 0 && Ly == 0){
		Fx =  Gx / Lx;
		Fy = -Fx;
	}
	else if(Lx == 0 && Ly != 0){
		Fy = -Gy / Ly;
		Fx = -Fy;
	}

	m_ScaleF = min(Fx, Fy);

	m_dX = (m_Canvas.right+m_Canvas.left)/2.0 - (right+left)/2.0*m_ScaleF - m_Canvas.left;
	m_dY = (m_Canvas.top+m_Canvas.bottom)/2.0 - (top+bottom)/2.0*m_ScaleF - m_Canvas.top;
	*/


	return TRUE;
}
BOOL CDgnSectionDrawFunc::CalcCanvasRange(double left, double top, double right, double bottom, double Lx, double By, double Rx, double Ty, double dRotAngle)
{
	double dB = fabs(right - left);
	double dH = fabs(top - bottom);
	double dleft = (left + Lx * dB);
	double dbottom = (bottom + By * dH);
	double dright = (right + Rx * dB);
	double dtop = (top + Ty * dH);

	CArray<CPoint, CPoint> arLPt;  arLPt.RemoveAll();
	CArray<CPoint, CPoint> arGPt;  arGPt.RemoveAll();

	CPoint pt;
	pt.x = dleft;  pt.y = dbottom;  arLPt.Add(pt);
	pt.x = dright; pt.y = dbottom;  arLPt.Add(pt);
	pt.x = dright; pt.y = dtop;     arLPt.Add(pt);
	pt.x = dleft;  pt.y = dtop;     arLPt.Add(pt);

	RotationCoord(arLPt, arGPt, dRotAngle);
	dleft = dbottom = dright = dtop = 0.0; // 초기화 
	for (int i = 0; i < 4; i++)
	{
		pt = arGPt.GetAt(i);
		dleft = min(dleft, pt.x);
		dright = max(dright, pt.x);
		dtop = max(dtop, pt.y);
		dbottom = min(dbottom, pt.y);
	}

	if (CalcScaleFactor(left, top, right, bottom)) return TRUE;

	return FALSE;
}
BOOL CDgnSectionDrawFunc::CalcCanvasRange(double dB, double dH, double Lx, double By, double Rx, double Ty)
{
	double left = (0 + Lx * dB);
	double bottom = (0 + By * dH);
	double right = (dB + Rx * dB);
	double top = (dH + Ty * dH);
	if (CalcScaleFactor(left, top, right, bottom)) return TRUE;

	return FALSE;
}

double CDgnSectionDrawFunc::GetLPByGPForLength(LONG dValGP)
{
	// dValGP에 GP상의 길이를 넘겨주면 LP상의 길이를 Return 해주는 함수
	return dValGP / m_ScaleF;
}

void CDgnSectionDrawFunc::MappingCoord(CArray<CPoint, CPoint>& rLPt, CArray<CPoint, CPoint>& rGPt)
{
	CPoint pt;
	if (m_nCanvasType == 0)
	{
		for (int i = 0; i < rLPt.GetSize(); i++)
		{
			pt.x = m_Canvas.left + rLPt[i].x *  m_ScaleF + m_dX;
			pt.y = m_Canvas.bottom + rLPt[i].y * -m_ScaleF - m_dY;

			rGPt.Add(pt);
		}
	}
	else
	{
		for (int i = 0; i < rLPt.GetSize(); i++)
		{
			pt.x = m_Canvas.left + rLPt[i].x *  m_ScaleF + m_dX;
			pt.y = -max(m_Canvas.bottom, m_Canvas.top) + rLPt[i].y *  m_ScaleF + m_dY;

			rGPt.Add(pt);
		}
	}
}
void CDgnSectionDrawFunc::OffsetCoord(CArray<CPoint, CPoint>& rLPt, CArray<CPoint, CPoint>& rGPt, double dx, double dy)
{
	CPoint pt;
	for (int i = 0; i < rLPt.GetSize(); i++)
	{
		pt.x = rLPt[i].x + dx;
		pt.y = rLPt[i].y + dy;

		rGPt.Add(pt);
	}
}
void CDgnSectionDrawFunc::RotationCoord(CArray<CPoint, CPoint>& rLPt, CArray<CPoint, CPoint>& rGPt, double dAngle)
{
	CPoint pt;
	if (dAngle == 0)
	{
		for (int i = 0; i < rLPt.GetSize(); i++)
			rGPt.Add(rLPt.GetAt(i));
		return;// 각도가 0일경우 복사만 하고 종료
	}
	dAngle = dAngle * CMathFunc::m_trang;// degree값으로 변환
	double dx, dy, dz;
	dz = 0.0;
	for (int i = 0; i < rLPt.GetSize(); i++)
	{
		pt = rLPt.GetAt(i);
		dx = pt.x;  dy = pt.y;
		CMathFunc::mathRotateZ(dAngle, dx, dy, dz);
		pt.x = dx;  pt.y = dy;
		rGPt.Add(pt);
	}
}
double CDgnSectionDrawFunc::Cng_UnitLen(int iCodeUnitLen)
{
	if (iCodeUnitLen < 0) return 1.0;

	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
	// by Current Unit.
	int iCurrUnitLen = CurrUnit.nBase_Length;
	double dCurrLenFactor = 0.0;
	if (iCurrUnitLen == D_UNITSYS_LENGTH_INDEX_MM)       dCurrLenFactor = 1.0;
	else if (iCurrUnitLen == D_UNITSYS_LENGTH_INDEX_CM)  dCurrLenFactor = 10.0;
	else if (iCurrUnitLen == D_UNITSYS_LENGTH_INDEX_M)   dCurrLenFactor = 1000.0;
	else if (iCurrUnitLen == D_UNITSYS_LENGTH_INDEX_FT)  dCurrLenFactor = 304.8;
	else if (iCurrUnitLen == D_UNITSYS_LENGTH_INDEX_IN)  dCurrLenFactor = 25.4;
	else  ASSERT(0);
	// by Code Unit.
	double dCodeLenFactor = 0.0;
	if (iCodeUnitLen == D_UNITSYS_LENGTH_INDEX_MM)       dCodeLenFactor = 1.0;
	else if (iCodeUnitLen == D_UNITSYS_LENGTH_INDEX_CM)  dCodeLenFactor = 10.0;
	else if (iCodeUnitLen == D_UNITSYS_LENGTH_INDEX_M)   dCodeLenFactor = 1000.0;
	else if (iCodeUnitLen == D_UNITSYS_LENGTH_INDEX_FT)  dCodeLenFactor = 304.8;
	else if (iCodeUnitLen == D_UNITSYS_LENGTH_INDEX_IN)  dCodeLenFactor = 25.4;
	else  ASSERT(0);

	return dCurrLenFactor / dCodeLenFactor;
}

T_SECT_SECTBASE_D CDgnSectionDrawFunc::GetSectBaseD(T_SECT_D& SectD, int nIMJ)
{
	CDgnDrawSection DgnDrawSect;
	return DgnDrawSect.GetSectBaseD(SectD, nIMJ);
	//	T_SECT_SECTBASE_D SectBaseD;
	//  SectBaseD.Initialize();
	//
	//	if(SectD.nStype!=D_SECT_TYPE_TAPERED)
	//	  nIMJ = 0;			
	//  
	//  if     (nIMJ == 0) // I단
	//    SectBaseD = SectD.SectBefore.SectI;
	//  else if(nIMJ == 2) // J단
	//    SectBaseD = SectD.SectBefore.SectJ;
	//  else if(nIMJ == 1) // M단
	//  {
	//    SectBaseD = SectD.SectBefore.SectI;
	//    // Size 평균
	//    for(int i=0 ; i<D_SECT_SIZE_NUM_MAX ; i++)
	//      SectBaseD.Size[i] = (SectD.SectBefore.SectI.Size[i]+SectD.SectBefore.SectJ.Size[i])/2.0;
	//
	//    // Stiffness 평균
	//    SectBaseD.Stiffness.Area =(SectD.SectBefore.SectI.Stiffness.Area +SectD.SectBefore.SectJ.Stiffness.Area )/2.0;
	//    SectBaseD.Stiffness.Asy  =(SectD.SectBefore.SectI.Stiffness.Asy  +SectD.SectBefore.SectJ.Stiffness.Asy  )/2.0;
	//    SectBaseD.Stiffness.Asz  =(SectD.SectBefore.SectI.Stiffness.Asz  +SectD.SectBefore.SectJ.Stiffness.Asz  )/2.0;
	//    SectBaseD.Stiffness.Rxx  =(SectD.SectBefore.SectI.Stiffness.Rxx  +SectD.SectBefore.SectJ.Stiffness.Rxx  )/2.0;
	//    SectBaseD.Stiffness.Ryy  =(SectD.SectBefore.SectI.Stiffness.Ryy  +SectD.SectBefore.SectJ.Stiffness.Ryy  )/2.0;
	//    SectBaseD.Stiffness.Rzz  =(SectD.SectBefore.SectI.Stiffness.Rzz  +SectD.SectBefore.SectJ.Stiffness.Rzz  )/2.0;
	//    SectBaseD.Stiffness.Cyp  =(SectD.SectBefore.SectI.Stiffness.Cyp  +SectD.SectBefore.SectJ.Stiffness.Cyp  )/2.0;
	//    SectBaseD.Stiffness.Cym  =(SectD.SectBefore.SectI.Stiffness.Cym  +SectD.SectBefore.SectJ.Stiffness.Cym  )/2.0;
	//    SectBaseD.Stiffness.Czp  =(SectD.SectBefore.SectI.Stiffness.Czp  +SectD.SectBefore.SectJ.Stiffness.Czp  )/2.0;
	//    SectBaseD.Stiffness.Czm  =(SectD.SectBefore.SectI.Stiffness.Czm  +SectD.SectBefore.SectJ.Stiffness.Czm  )/2.0;
	//    SectBaseD.Stiffness.Qyb  =(SectD.SectBefore.SectI.Stiffness.Qyb  +SectD.SectBefore.SectJ.Stiffness.Qyb  )/2.0;
	//    SectBaseD.Stiffness.Qzb  =(SectD.SectBefore.SectI.Stiffness.Qzb  +SectD.SectBefore.SectJ.Stiffness.Qzb  )/2.0;
	//    SectBaseD.Stiffness.WArea=(SectD.SectBefore.SectI.Stiffness.WArea+SectD.SectBefore.SectJ.Stiffness.WArea)/2.0;
	//    SectBaseD.Stiffness.dx1  =(SectD.SectBefore.SectI.Stiffness.dx1  +SectD.SectBefore.SectJ.Stiffness.dx1  )/2.0;
	//    SectBaseD.Stiffness.dx2  =(SectD.SectBefore.SectI.Stiffness.dx2  +SectD.SectBefore.SectJ.Stiffness.dx2  )/2.0;
	//    SectBaseD.Stiffness.dx3  =(SectD.SectBefore.SectI.Stiffness.dx3  +SectD.SectBefore.SectJ.Stiffness.dx3  )/2.0;
	//    SectBaseD.Stiffness.dx4  =(SectD.SectBefore.SectI.Stiffness.dx4  +SectD.SectBefore.SectJ.Stiffness.dx4  )/2.0;
	//    SectBaseD.Stiffness.dy1  =(SectD.SectBefore.SectI.Stiffness.dy1  +SectD.SectBefore.SectJ.Stiffness.dy1  )/2.0;
	//    SectBaseD.Stiffness.dy2  =(SectD.SectBefore.SectI.Stiffness.dy2  +SectD.SectBefore.SectJ.Stiffness.dy2  )/2.0;
	//    SectBaseD.Stiffness.dy3  =(SectD.SectBefore.SectI.Stiffness.dy3  +SectD.SectBefore.SectJ.Stiffness.dy3  )/2.0;
	//    SectBaseD.Stiffness.dy4  =(SectD.SectBefore.SectI.Stiffness.dy4  +SectD.SectBefore.SectJ.Stiffness.dy4  )/2.0;
	//    // Degin 평균
	//    SectBaseD.Design.YBar    =(SectD.SectBefore.SectI.Design.YBar    +SectD.SectBefore.SectJ.Design.YBar    )/2.0;
	//    SectBaseD.Design.ZBar    =(SectD.SectBefore.SectI.Design.ZBar    +SectD.SectBefore.SectJ.Design.ZBar    )/2.0;
	//    SectBaseD.Design.Syy     =(SectD.SectBefore.SectI.Design.Syy     +SectD.SectBefore.SectJ.Design.Syy     )/2.0;
	//    SectBaseD.Design.Szz     =(SectD.SectBefore.SectI.Design.Szz     +SectD.SectBefore.SectJ.Design.Szz     )/2.0;
	//    SectBaseD.Design.Zyy     =(SectD.SectBefore.SectI.Design.Zyy     +SectD.SectBefore.SectJ.Design.Zyy     )/2.0;
	//    SectBaseD.Design.Zzz     =(SectD.SectBefore.SectI.Design.Zzz     +SectD.SectBefore.SectJ.Design.Zzz     )/2.0;
	//    SectBaseD.Design.Roy     =(SectD.SectBefore.SectI.Design.Roy     +SectD.SectBefore.SectJ.Design.Roy     )/2.0;
	//    SectBaseD.Design.Roz     =(SectD.SectBefore.SectI.Design.Roz     +SectD.SectBefore.SectJ.Design.Roz     )/2.0;
	//    SectBaseD.Design.Cwp     =(SectD.SectBefore.SectI.Design.Cwp     +SectD.SectBefore.SectJ.Design.Cwp     )/2.0;
	//    SectBaseD.Design.TanA    =(SectD.SectBefore.SectI.Design.TanA    +SectD.SectBefore.SectJ.Design.TanA    )/2.0;
	//    SectBaseD.Design.Rop     =(SectD.SectBefore.SectI.Design.Rop     +SectD.SectBefore.SectJ.Design.Rop     )/2.0;
	//    SectBaseD.Design.XO      =(SectD.SectBefore.SectI.Design.XO      +SectD.SectBefore.SectJ.Design.XO      )/2.0;
	//    SectBaseD.Design.RO      =(SectD.SectBefore.SectI.Design.RO      +SectD.SectBefore.SectJ.Design.RO      )/2.0;
	//    SectBaseD.Design.Beta    =(SectD.SectBefore.SectI.Design.Beta    +SectD.SectBefore.SectJ.Design.Beta    )/2.0;
	//    
	//    // 외곽 Polygon 평균
	//    if(SectD.SectBefore.SectI.aOuterPolygon.GetSize() != SectD.SectBefore.SectJ.aOuterPolygon.GetSize())
	//      return SectBaseD;
	//    int nOutPolygon = SectBaseD.aOuterPolygon.GetSize();
	//
	//    int nPoint, k, n;
	//    for(k=0 ; k<nOutPolygon ; k++)
	//    {
	//      if(SectD.SectBefore.SectI.aOuterPolygon[k].aVertex.GetSize() != SectD.SectBefore.SectJ.aOuterPolygon[k].aVertex.GetSize())
	//        return SectBaseD;
	//
	//      nPoint = SectBaseD.aOuterPolygon[k].aVertex.GetSize();
	//      for(n=0 ; n<nPoint ; n++)
	//      {
	//        SectBaseD.aOuterPolygon[k].aVertex[n].dX = (SectD.SectBefore.SectI.aOuterPolygon[k].aVertex[n].dX+SectD.SectBefore.SectJ.aOuterPolygon[k].aVertex[n].dX)/2.0;
	//        SectBaseD.aOuterPolygon[k].aVertex[n].dY = (SectD.SectBefore.SectI.aOuterPolygon[k].aVertex[n].dY+SectD.SectBefore.SectJ.aOuterPolygon[k].aVertex[n].dY)/2.0;
	//      }
	//    }
	//    // 내부 Polygon 평균
	//    if(SectD.SectBefore.SectI.aInnerPolygon.GetSize() != SectD.SectBefore.SectJ.aInnerPolygon.GetSize())
	//      return SectBaseD;
	//    int nInPolygon = SectBaseD.aInnerPolygon.GetSize();
	//    
	//    for(k=0 ; k<nInPolygon ; k++)
	//    {
	//      if(SectD.SectBefore.SectI.aInnerPolygon[k].aVertex.GetSize() != SectD.SectBefore.SectJ.aInnerPolygon[k].aVertex.GetSize())
	//        return SectBaseD;
	//
	//      nPoint = SectBaseD.aInnerPolygon[k].aVertex.GetSize();
	//      for(n=0 ; n<nPoint ; n++)
	//      {
	//        SectBaseD.aInnerPolygon[k].aVertex[n].dX = (SectD.SectBefore.SectI.aInnerPolygon[k].aVertex[n].dX+SectD.SectBefore.SectJ.aInnerPolygon[k].aVertex[n].dX)/2.0;
	//        SectBaseD.aInnerPolygon[k].aVertex[n].dY = (SectD.SectBefore.SectI.aInnerPolygon[k].aVertex[n].dY+SectD.SectBefore.SectJ.aInnerPolygon[k].aVertex[n].dY)/2.0;
	//      }
	//    }    
	//  }
	//
	//	if(SectBaseD.Shape.GetLength() == 0)
	//	{
	//		// 단면의 형상은 TAPERED의 유무와 상관없이 SectBefore.Shape를 따라 간다.	
	//		SectBaseD.Shape = SectD.SectBefore.Shape;
	//	}
	//	
	//  return SectBaseD;  
}

T_SECT_SECTBASE_D CDgnSectionDrawFunc::GetSectBaseD_For_ElemK(T_ELEM_K ElemK, int nIMJ, int MatlType)
{
	CDgnDrawSection DgnDrawSect;
	return DgnDrawSect.GetSectBaseD_For_ElemK(ElemK, nIMJ, TRUE, MatlType);

	//	T_SECT_D SectD; 
	//  SectD.Initialize();
	//	T_SECT_SECTBASE_D SectBaseD;
	//  SectBaseD.Initialize();
	//	
	//	if(MatlType == 2)
	//	{
	//		CDgnDataCtrl DgnDataCtrl;
	//		if(!DgnDataCtrl.Get_DgnStlSect(ElemK, SectD)) return SectBaseD;
	//	}
	//	else
	//	{ 
	//		T_ELEM_D ElemD;
	//		ElemD.Initialize();
	//
	//		if(m_pDoc->m_pAttrCtrl->GetElemTsgrKey(ElemK) != 0)
	//		{				
	//			if(!m_pDoc->m_pPostCtrl->GetElemPost(ElemK,  ElemD)) return SectBaseD;
	//			if(!m_pDoc->m_pPostCtrl->GetSectPost(ElemD.elpro, SectD)) return SectBaseD;
	//		}
	//		else 
	//		{
	//			if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD))	return SectBaseD;	
	//
	//			T_SECT_K SectK = ElemD.elpro;  
	//			if(!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return SectBaseD; 
	//		}		
	//	}
	//
	//	return GetSectBaseD(SectD, nIMJ);
}

T_SECT_SECTBASE_D CDgnSectionDrawFunc::GetSectBaseD_For_SectK(T_SECT_K SectK, int nIMJ)
{
	CDgnDrawSection DgnDrawSect;
	return DgnDrawSect.GetSectBaseD_For_SectK(SectK, nIMJ);

	//	T_SECT_D SectD; 
	//  SectD.Initialize();
	//	T_SECT_SECTBASE_D SectBaseD;
	//  SectBaseD.Initialize();
	//
	//	if(!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return SectBaseD; 
	//
	//	return GetSectBaseD(SectD, nIMJ);
}

// Main Draw (Dll방식)////////////////////////////////////
void CDgnSectionDrawFunc::Draw_Section_For_ElemK(CDC* pDC, T_ELEM_K ElemK, int nIMJ, int MatlType, BOOL bDim, BOOL bFill, BOOL bChkPoint)
{
	T_SECT_SECTBASE_D SectBaseD = GetSectBaseD_For_ElemK(ElemK, nIMJ, MatlType);

	DGNDRAW_SECT        SectData;
	DGNDRAW_SECT_OPTION SectOption; SectOption.Initialize();

	BOOL bChk = ConvertToDLL_SectData(SectBaseD, SectData);
	SectData.MatlType = MatlType;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX CurIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	BOOL bLineType = (SectBaseD.Shape == D_SECT_SHAPE_REG_GEN && SectBaseD.aLine.GetSize() > 0);
	// 그림을 그릴 공간정보
	SectOption.Canvas = m_Canvas;               // CDC에 그릴 영역(CanvasType과 상관 없이 좌측 상단점을 0,0 으로 좌하단을 +방향으로 한다.)
	SectOption.CanvasType = m_nCanvasType;          // 0:Dlg형식, 1:Meta형식
	SectOption.AxisType = bChk ? m_nAxisType : 3; // 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시, 3:단면표시 없이 축만표시 
	SectOption.DimType = m_nDimType;
	SectOption.DimShape = m_nDimShape;
	SectOption.DimSize = m_nDimSize;             // 표현할 치수선의 글자 크기
	SectOption.SectLineSize = (bLineType ? m_nRebarSize : m_nLineSize);          // Line 형식 표현시 선의 두께
	SectOption.ChkPointSize = m_nChkPointSize;
	SectOption.LengthUnit_Data = ConvertToDLL_LengthUnitID(CurIndex.nBase_Length);  // 입력정보의 표현 단위계 ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.LengthUnit_Draw = ConvertToDLL_LengthUnitID(m_nLengthUnit < 0 ? CurIndex.nBase_Length : m_nLengthUnit); // 치수선의 표현 단위계   ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.ZoomFactor = m_dZoomFactor;
	// 그림을 그리기위한 세부설정
	SectOption.bReDraw = FALSE;                  // 다시그릴지 여부
	SectOption.bShape = TRUE;                   // 단면을 그릴지 여부
	SectOption.bDimShape = bDim;                   // 단면의 치수선을 그릴지 여부
	SectOption.bFillShape = bFill;                  // 단면의 내부에 색을 채울지 여부
	SectOption.bChkPoint = TRUE;//bChkPoint;

	m_DrawManager.GDraw_Section(pDC, SectData, SectOption);
}
void CDgnSectionDrawFunc::Draw_Section_For_SectK(CDC* pDC, T_SECT_K SectK, int nIMJ, int MatlType, BOOL bDim, BOOL bFill, BOOL bChkPoint)
{
	T_SECT_SECTBASE_D SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);

	DGNDRAW_SECT        SectData;
	DGNDRAW_SECT_OPTION SectOption; SectOption.Initialize();

	BOOL bChk = ConvertToDLL_SectData(SectBaseD, SectData);
	SectData.MatlType = MatlType;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX CurIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	BOOL bLineType = (SectBaseD.Shape == D_SECT_SHAPE_REG_GEN && SectBaseD.aLine.GetSize() > 0);
	// 그림을 그릴 공간정보
	SectOption.Canvas = m_Canvas;               // CDC에 그릴 영역(CanvasType과 상관 없이 좌측 상단점을 0,0 으로 좌하단을 +방향으로 한다.)
	SectOption.CanvasType = m_nCanvasType;          // 0:Dlg형식, 1:Meta형식
	SectOption.AxisType = bChk ? m_nAxisType : 3; // 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시, 3:단면표시 없이 축만표시 
	SectOption.DimType = m_nDimType;
	SectOption.DimShape = m_nDimShape;
	SectOption.DimSize = m_nDimSize;             // 표현할 치수선의 글자 크기
	SectOption.SectLineSize = (bLineType ? m_nRebarSize : m_nLineSize);          // Line 형식 표현시 선의 두께
	SectOption.ChkPointSize = m_nChkPointSize;
	SectOption.LengthUnit_Data = ConvertToDLL_LengthUnitID(CurIndex.nBase_Length);  // 입력정보의 표현 단위계 ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.LengthUnit_Draw = ConvertToDLL_LengthUnitID(m_nLengthUnit < 0 ? CurIndex.nBase_Length : m_nLengthUnit); // 치수선의 표현 단위계   ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.ZoomFactor = m_dZoomFactor;
	// 그림을 그리기위한 세부설정
	SectOption.bReDraw = FALSE;                  // 다시그릴지 여부
	SectOption.bShape = TRUE;                   // 단면을 그릴지 여부
	SectOption.bDimShape = bDim;                   // 단면의 치수선을 그릴지 여부
	SectOption.bFillShape = bFill;                  // 단면의 내부에 색을 채울지 여부
	SectOption.bChkPoint = TRUE;//bChkPoint;

	m_DrawManager.GDraw_Section(pDC, SectData, SectOption);
}

void CDgnSectionDrawFunc::Draw_Section_For_RcBeam(CDC* pDC, T_ELEM_K ElemK, int nIMJ, BOOL bShape, BOOL bRebar, BOOL bDimShape, BOOL bDimRebar, BOOL bFillShape, BOOL bFillRebar, int nRebarDataType, int nTypeRebarView)
{
	T_ELEM_D ElemD;
	ElemD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))	return;

	int SectK = ElemD.elpro;
	T_SECT_SECTBASE_D SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);

	T_MBTP_D MbtpD; MbtpD.Initialize();
	BOOL bGetMbtp = m_pDoc->m_pAttrCtrl->GetMbtp(ElemK, MbtpD);
	int iMbtp = (bGetMbtp ? MbtpD.nMbType : m_pDoc->m_pAttrCtrl->GetMemberType(ElemK));
	if (iMbtp != 2) return;// 보일 경우에만

	T_CPBE_D CpbeD;    CpbeD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetCpbe(ElemK, CpbeD)) { bRebar = bDimRebar = FALSE; }
	if (nRebarDataType == 1)
		CpbeD.bNum = TRUE;
	else if (nRebarDataType == 2)
		CpbeD.bNum = FALSE;

	// Dll에 넘겨줄 정보 가공
	DGNDRAW_SECT_BEAM        SectData;
	DGNDRAW_EX_RCSECT_OPTION SectOption; SectOption.Initialize();

	BOOL bChk = ConvertToDLL_RcBeamData(SectBaseD, nIMJ, CpbeD, SectData);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX CurIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	// 그림을 그릴 공간정보
	SectOption.Canvas = m_Canvas;               // CDC에 그릴 영역(CanvasType과 상관 없이 좌측 상단점을 0,0 으로 좌하단을 +방향으로 한다.)
	SectOption.CanvasType = m_nCanvasType;          // 0:Dlg형식, 1:Meta형식
	SectOption.AxisType = bChk ? m_nAxisType : 3; // 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시, 3:단면표시 없이 축만표시 
	SectOption.DimType = m_nDimType;             // 1:전체치수선 표시, 2:B,H치수선만 표시
	SectOption.DimShape = m_nDimShape;            // 1:Text선위(값), 2:Text중앙(값), 3:Text선위(기호), 4:Text중앙(기호)
	SectOption.DimSize = m_nDimSize;             // 표현할 치수선의 글자 크기
	SectOption.RebarSize = m_nRebarSize;           // 표현한 철근그림의 직경
	SectOption.RebarShape = 1;                      // 1:철근만 표시, 2:철근+철근영역 표시(선), 3:철근+철근영역 표시(영역)
	SectOption.SectLineSize = m_nLineSize;            // 단면의 선 두께(or LineType 선두께)
	SectOption.ChkPointSize = m_nChkPointSize;        // 표현한 ChkPoint의 직경
	SectOption.LengthUnit_Data = ConvertToDLL_LengthUnitID(CurIndex.nBase_Length);  // 입력정보의 표현 단위계 ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.LengthUnit_Draw = ConvertToDLL_LengthUnitID(m_nLengthUnit < 0 ? CurIndex.nBase_Length : m_nLengthUnit); // 치수선의 표현 단위계   ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.ZoomFactor = m_dZoomFactor;
	// 그림을 그리기위한 세부설정
	SectOption.nTypeRebarView = nTypeRebarView;
	SectOption.bReDraw = FALSE;       // 다시그릴지 여부
	SectOption.bShape = bShape;     // 단면을 그릴지 여부
	SectOption.bRebar = bRebar;     // 철근을 그릴지 여부
	SectOption.bRebarPosi = FALSE;       // 철근의 위치정보를 그릴지 여부
	SectOption.bDimShape = bDimShape;  // 단면의 치수선을 그릴지 여부
	SectOption.bDimRebar = bDimRebar;  // 철근의 치수선을 그릴지 여부
	SectOption.bFillShape = bFillShape; // 단면의 내부에 색을 채울지 여부
	SectOption.bFillRebar = bFillRebar; // 철근의 내부에 색을 채울지 여부

	m_DrawManager.Draw_Section_Beam(pDC, SectData, SectOption);
}

void CDgnSectionDrawFunc::Draw_Section_For_RcColumn(CDC* pDC, T_ELEM_K ElemK, int nIMJ, double RotAngle, BOOL bShape, BOOL bRebar, BOOL bDimShape, BOOL bDimRebar, BOOL bFillShape, BOOL bFillRebar)
{
	T_ELEM_D ElemD;
	ElemD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))	return;

	int SectK = ElemD.elpro;
	T_SECT_SECTBASE_D SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);

	T_MBTP_D MbtpD; MbtpD.Initialize();
	BOOL bGetMbtp = m_pDoc->m_pAttrCtrl->GetMbtp(ElemK, MbtpD);
	int iMbtp = (bGetMbtp ? MbtpD.nMbType : m_pDoc->m_pAttrCtrl->GetMemberType(ElemK));
	if (iMbtp != 1) return;// 기둥일 경우에만

	BOOL bGenType = (SectBaseD.Shape == D_SECT_SHAPE_REG_GEN);
	T_CPCO_D CpcoD;   CpcoD.Initialize();
	T_RBTE_D RbteD;   RbteD.Initialize();
	if (!bGenType)
	{
		if (!m_pDoc->m_pAttrCtrl->GetCpco(ElemK, CpcoD)) { bRebar = bDimRebar = FALSE; }
	}
	else
	{
		if (!CDgnSectionTool::GetRbte(ElemK, RbteD)) { bRebar = bDimRebar = FALSE; }
	}

	// Dll에 넘겨줄 정보 가공
	DGNDRAW_SECT_COLUMN      SectData;
	DGNDRAW_EX_RCSECT_OPTION SectOption; SectOption.Initialize();

	BOOL bChk = FALSE;
	if (!bGenType)
		bChk = ConvertToDLL_RcColumnData(SectBaseD, nIMJ, CpcoD, SectData);
	else
		bChk = ConvertToDLL_RcColumnData_Gen(SectBaseD, nIMJ, RbteD, SectData);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX CurIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	// 그림을 그릴 공간정보
	SectOption.Canvas = m_Canvas;               // CDC에 그릴 영역(CanvasType과 상관 없이 좌측 상단점을 0,0 으로 좌하단을 +방향으로 한다.)
	SectOption.CanvasType = m_nCanvasType;          // 0:Dlg형식, 1:Meta형식
	SectOption.AxisType = bChk ? m_nAxisType : 3; // 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시, 3:단면표시 없이 축만표시 
	SectOption.DimType = m_nDimType;             // 1:전체치수선 표시, 2:B,H치수선만 표시
	SectOption.DimShape = m_nDimShape;            // 1:Text선위(값), 2:Text중앙(값), 3:Text선위(기호), 4:Text중앙(기호)
	SectOption.DimSize = m_nDimSize;             // 표현할 치수선의 글자 크기
	SectOption.RebarSize = m_nRebarSize;           // 표현한 철근그림의 직경
	SectOption.RebarShape = 1;                      // 1:철근만 표시, 2:철근+철근영역 표시(선), 3:철근+철근영역 표시(영역)
	SectOption.SectLineSize = m_nLineSize;            // 단면의 선 두께(or LineType 선두께)
	SectOption.ChkPointSize = m_nChkPointSize;        // 표현한 ChkPoint의 직경
	SectOption.LengthUnit_Data = ConvertToDLL_LengthUnitID(CurIndex.nBase_Length);  // 입력정보의 표현 단위계 ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.LengthUnit_Draw = ConvertToDLL_LengthUnitID(m_nLengthUnit < 0 ? CurIndex.nBase_Length : m_nLengthUnit); // 치수선의 표현 단위계   ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.ZoomFactor = m_dZoomFactor;
	SectOption.RotAngle = RotAngle;
	// 그림을 그리기위한 세부설정
	SectOption.bReDraw = FALSE;       // 다시그릴지 여부
	SectOption.bShape = bShape;     // 단면을 그릴지 여부
	SectOption.bRebar = bRebar;     // 철근을 그릴지 여부
	SectOption.bRebarPosi = FALSE;       // 철근의 위치정보를 그릴지 여부
	SectOption.bDimShape = bDimShape;  // 단면의 치수선을 그릴지 여부
	SectOption.bDimRebar = bDimRebar;  // 철근의 치수선을 그릴지 여부
	SectOption.bFillShape = bFillShape; // 단면의 내부에 색을 채울지 여부
	SectOption.bFillRebar = bFillRebar; // 철근의 내부에 색을 채울지 여부

	m_DrawManager.Draw_Section_Column(pDC, SectData, SectOption);
}

void CDgnSectionDrawFunc::Draw_Section_For_RcWall(CDC* pDC, T_ELEM_K ElemK, int nIMJ, BOOL bShape, BOOL bRebar, BOOL bDimShape, BOOL bDimRebar, BOOL bFillShape, BOOL bFillRebar, int nRebarDataType, int nTypeRebarView)
{
	// DB에서 값 가져오기
	T_ELEM_D ElemD;   ElemD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))	return;

	int SectK = ElemD.elpro;
	T_SECT_SECTBASE_D SectBaseD = GetSectBaseD_For_SectK(SectK, nIMJ);

	T_MBTP_D MbtpD;   MbtpD.Initialize();
	BOOL bGetMbtp = m_pDoc->m_pAttrCtrl->GetMbtp(ElemK, MbtpD);
	int iMbtp = (bGetMbtp ? MbtpD.nMbType : m_pDoc->m_pAttrCtrl->GetMemberType(ElemK));
	if (iMbtp != 1) return;      // 기둥멤버일 경우에만 Wall(Frame)임

	T_RCWA_D RcwaD;   RcwaD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetRcwa(ElemK, RcwaD)) return;

	T_CPWA_D CpwaD;   CpwaD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetCpwa(ElemK, CpwaD)) { bRebar = bDimRebar = FALSE; }
	if (nRebarDataType == 1)
		CpwaD.bNum = TRUE;
	else if (nRebarDataType == 2)
		CpwaD.bNum = FALSE;

	// Dll에 넘겨줄 정보 가공
	DGNDRAW_SECT_WALL        SectData;
	DGNDRAW_EX_RCSECT_OPTION SectOption; SectOption.Initialize();

	BOOL bChk = ConvertToDLL_RcWallData(SectBaseD, nIMJ, RcwaD, CpwaD, SectData);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX CurIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	// 그림을 그릴 공간정보
	SectOption.Canvas = m_Canvas;               // CDC에 그릴 영역(CanvasType과 상관 없이 좌측 상단점을 0,0 으로 좌하단을 +방향으로 한다.)
	SectOption.CanvasType = m_nCanvasType;          // 0:Dlg형식, 1:Meta형식
	SectOption.AxisType = bChk ? m_nAxisType : 3; // 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시, 3:단면표시 없이 축만표시 
	SectOption.DimType = m_nDimType;             // 1:전체치수선 표시, 2:B,H치수선만 표시
	SectOption.DimShape = m_nDimShape;            // 1:Text선위(값), 2:Text중앙(값), 3:Text선위(기호), 4:Text중앙(기호)
	SectOption.DimSize = m_nDimSize;             // 표현할 치수선의 글자 크기
	SectOption.RebarSize = m_nRebarSize;           // 표현한 철근그림의 직경
	SectOption.RebarShape = 1;                      // 1:철근만 표시, 2:철근+철근영역 표시(선), 3:철근+철근영역 표시(영역)
	SectOption.SectLineSize = m_nLineSize;            // 단면의 선 두께(or LineType 선두께)
	SectOption.ChkPointSize = m_nChkPointSize;        // 표현한 ChkPoint의 직경
	SectOption.LengthUnit_Data = ConvertToDLL_LengthUnitID(CurIndex.nBase_Length);  // 입력정보의 표현 단위계 ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.LengthUnit_Draw = ConvertToDLL_LengthUnitID(m_nLengthUnit < 0 ? CurIndex.nBase_Length : m_nLengthUnit); // 치수선의 표현 단위계   ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.WidthLengthUnit = ConvertToDLL_LengthUnitID(m_nWidthLengthUnit);
	SectOption.ZoomFactor = m_dZoomFactor;
	// 그림을 그리기위한 세부설정
	SectOption.nTypeRebarView = nTypeRebarView;
	SectOption.bReDraw = FALSE;       // 다시그릴지 여부
	SectOption.bShape = bShape;     // 단면을 그릴지 여부
	SectOption.bRebar = bRebar;     // 철근을 그릴지 여부
	SectOption.bRebarPosi = FALSE;       // 철근의 위치정보를 그릴지 여부
	SectOption.bDimShape = bDimShape;  // 단면의 치수선을 그릴지 여부
	SectOption.bDimRebar = bDimRebar;  // 철근의 치수선을 그릴지 여부
	SectOption.bFillShape = bFillShape; // 단면의 내부에 색을 채울지 여부
	SectOption.bFillRebar = bFillRebar; // 철근의 내부에 색을 채울지 여부

	m_DrawManager.Draw_Section_Wall(pDC, SectData, SectOption);
}
void CDgnSectionDrawFunc::Draw_Section_For_RcPlate(CDC* pDC, T_RCPL_K RcplK, BOOL bShape, BOOL bRebar, BOOL bDimShape, BOOL bDimRebar, BOOL bFillShape, BOOL bFillRebar, int nTypeRebarView, int nRebarDataType)
{
	// DB에서 값 가져오기
	T_RCPL_D RcplD;
	RcplD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetRcpl(RcplK, RcplD))	return;

	int nElemSize = RcplD.arElemK.GetSize();
	int ThikK = -1;
	T_ELEM_D eData;
	for (int i = 0; i < nElemSize; i++)
	{
		eData.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetElem(RcplD.arElemK.GetAt(i), eData))	ASSERT(0);
		if (i == 0)
		{
			ThikK = eData.elpro;
		}
		else
		{
			if (ThikK != eData.elpro) return;
		}// Thikness Key가 다르면 종료
	}
	T_THIK_D ThikD; ThikD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetThik(ThikK, ThikD)) return;

	_DGNC_CPPL_D CpplD;   CpplD.Initialize();
	if (!CDgnSectionTool::Get_DgncCpplData(RcplK, CpplD)) { bRebar = bDimRebar = FALSE; }
	if (nRebarDataType == 1)
		CpplD.bNum = TRUE;
	else if (nRebarDataType == 2)
		CpplD.bNum = FALSE;

	// Dll에 넘겨줄 정보 가공
	DGNDRAW_SECT_PLATE        SectData;
	DGNDRAW_EX_RCSECT_OPTION SectOption; SectOption.Initialize();

	BOOL bChk = ConvertToDLL_RcPlateData(ThikD, CpplD, SectData);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX CurIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	// 그림을 그릴 공간정보
	SectOption.Canvas = m_Canvas;               // CDC에 그릴 영역(CanvasType과 상관 없이 좌측 상단점을 0,0 으로 좌하단을 +방향으로 한다.)
	SectOption.CanvasType = m_nCanvasType;          // 0:Dlg형식, 1:Meta형식
	SectOption.AxisType = bChk ? m_nAxisType : 3; // 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시, 3:단면표시 없이 축만표시 
	SectOption.DimType = m_nDimType;             // 1:전체치수선 표시, 2:B,H치수선만 표시
	SectOption.DimShape = m_nDimShape;            // 1:Text선위(값), 2:Text중앙(값), 3:Text선위(기호), 4:Text중앙(기호)
	SectOption.DimSize = m_nDimSize;             // 표현할 치수선의 글자 크기
	SectOption.RebarSize = m_nRebarSize;           // 표현한 철근그림의 직경
	SectOption.RebarShape = 1;                      // 1:철근만 표시, 2:철근+철근영역 표시(선), 3:철근+철근영역 표시(영역)
	SectOption.SectLineSize = m_nLineSize;            // 단면의 선 두께(or LineType 선두께)
	SectOption.ChkPointSize = m_nChkPointSize;        // 표현한 ChkPoint의 직경
	SectOption.LengthUnit_Data = ConvertToDLL_LengthUnitID(CurIndex.nBase_Length);  // 입력정보의 표현 단위계 ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.LengthUnit_Draw = ConvertToDLL_LengthUnitID(m_nLengthUnit < 0 ? CurIndex.nBase_Length : m_nLengthUnit); // 치수선의 표현 단위계   ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.WidthLengthUnit = ConvertToDLL_LengthUnitID(m_nWidthLengthUnit);
	SectOption.ZoomFactor = m_dZoomFactor;
	// 그림을 그리기위한 세부설정
	SectOption.nTypeRebarView = nTypeRebarView;
	SectOption.bReDraw = FALSE;       // 다시그릴지 여부
	SectOption.bShape = bShape;     // 단면을 그릴지 여부
	SectOption.bRebar = bRebar;     // 철근을 그릴지 여부
	SectOption.bRebarPosi = FALSE;       // 철근의 위치정보를 그릴지 여부
	SectOption.bDimShape = bDimShape;  // 단면의 치수선을 그릴지 여부
	SectOption.bDimRebar = bDimRebar;  // 철근의 치수선을 그릴지 여부
	SectOption.bFillShape = bFillShape; // 단면의 내부에 색을 채울지 여부
	SectOption.bFillRebar = bFillRebar; // 철근의 내부에 색을 채울지 여부

	m_DrawManager.Draw_Section_Plate(pDC, SectData, SectOption);
}

// Data Convert (Dll용)/////////////////////////////////////////////////////////////////////

BOOL CDgnSectionDrawFunc::ConvertToDLL_Polygon(T_GSEC_POLYGON& InData, DGNDRAW_GSEC_POLYGON& OutData)
{
	OutData.Initialize();
	int nSize = InData.aVertex.GetSize();
	OutData.aVertex.SetSize(nSize);
	DGNDRAW_GSEC_VERTEX    VertexUnit;
	for (int i = 0; i < nSize; i++)
	{
		VertexUnit.Set(InData.aVertex[i].dX, InData.aVertex[i].dY);
		OutData.aVertex.SetAt(i, VertexUnit);
	}
	return TRUE;
}
BOOL CDgnSectionDrawFunc::ConvertToDLL_Line(T_GSEC_LINE& InData, DGNDRAW_GSEC_LINE& OutData)
{
	OutData.Initialize();
	OutData.v1 = InData.v1;
	OutData.v2 = InData.v2;
	OutData.dThik = InData.dThik;
	OutData.nAlign = InData.nAlign;
	return TRUE;
}
BOOL CDgnSectionDrawFunc::ConvertToDLL_SectData(T_SECT_SECTBASE_D& InData, DGNDRAW_SECT& OutData)
{
	CDgnDrawSection DgnDrawSect;
	return DgnDrawSect.ConvertToDLL_SectData(InData, OutData);
	//	OutData.Initialize();
	//	int i, nSize;
	//	OutData.Shape = ConvertToDLL_ShapeName(InData.Shape);          // 단면형상 
	//	for(i=0 ; i<8 ; i++)
	//		OutData.dSize[i] = InData.Size[i];   // 단면치수 ([0]:H, [1]:B)
	//	OutData.dYBar = InData.Design.YBar;
	//	OutData.dZBar = InData.Design.ZBar;
	//	
	//	DGNDRAW_GSEC_VERTEX ChkPoint;
	//	OutData.arChkPoint.SetSize(4);
	//	ChkPoint.dX = InData.Stiffness.dx1;  ChkPoint.dY = InData.Stiffness.dy1;  OutData.arChkPoint.SetAt(0, ChkPoint);
	//	ChkPoint.dX = InData.Stiffness.dx2;  ChkPoint.dY = InData.Stiffness.dy2;  OutData.arChkPoint.SetAt(1, ChkPoint);
	//	ChkPoint.dX = InData.Stiffness.dx3;  ChkPoint.dY = InData.Stiffness.dy3;  OutData.arChkPoint.SetAt(2, ChkPoint);
	//	ChkPoint.dX = InData.Stiffness.dx4;  ChkPoint.dY = InData.Stiffness.dy4;  OutData.arChkPoint.SetAt(3, ChkPoint);
	//
	//	//Gen형식
	//	OutData.dSize_Gen[0] = fabs(InData.Stiffness.Czp) + fabs(InData.Stiffness.Czm);   // 단면치수 ([0]:H, [1]:B)	
	//	OutData.dSize_Gen[1] = fabs(InData.Stiffness.Cyp) + fabs(InData.Stiffness.Cym);   
	//	DGNDRAW_GSEC_POLYGON DrawPolygon;
	//	DGNDRAW_GSEC_LINE    DrawLine;
	//	nSize = InData.aOuterPolygon.GetSize();
	//	OutData.aOuterPolygon.SetSize(nSize);	
	//	for(i=0 ; i<nSize ; i++)
	//	{
	//		if(!ConvertToDLL_Polygon(InData.aOuterPolygon[i], DrawPolygon)) DrawPolygon.Initialize();
	//		OutData.aOuterPolygon.SetAt(i, DrawPolygon);		
	//	}
	//	nSize = InData.aInnerPolygon.GetSize();
	//	OutData.aInnerPolygon.SetSize(nSize);
	//	for(i=0 ; i<nSize ; i++)
	//	{
	//		if(!ConvertToDLL_Polygon(InData.aInnerPolygon[i], DrawPolygon)) DrawPolygon.Initialize();
	//		OutData.aInnerPolygon.SetAt(i, DrawPolygon);		
	//	}
	//	nSize = InData.aLine.GetSize();
	//	OutData.aLine.SetSize(nSize);
	//	for(i=0 ; i<nSize ; i++)
	//	{
	//		if(!ConvertToDLL_Line(InData.aLine[i], DrawLine)) DrawLine.Initialize();
	//		OutData.aLine.SetAt(i, DrawLine);
	//	}
	//
	return TRUE;
}

BOOL CDgnSectionDrawFunc::ConvertToDLL_RcBeamData(T_SECT_SECTBASE_D& SectData, int nIMJ, T_CPBE_D& RbarData, DGNDRAW_SECT_BEAM&   OutData)
{
	// Section
	OutData.Initialize();
	int i;

	if (SectData.Shape == D_SECT_TYPE_PSC || SectData.Shape == D_SECT_SHAPE_PSC_MID)
	{
		double dHL1 = SectData.Size[1];
		double dHR1 = SectData.Size[16];
		double dBL2 = SectData.Size[11];
		double dBR2 = SectData.Size[26];
		OutData.Shape = CT2W(ConvertToDLL_ShapeName(D_SECT_SHAPE_REG_SB));          // 단면형상 
		OutData.dSize[0] = (dHL1 + dHR1) / 2.;
		OutData.dSize[1] = dBL2 + dBR2;
	}
	else
	{
		OutData.Shape = CT2W(ConvertToDLL_ShapeName(SectData.Shape));          // 단면형상 
		for (i = 0; i < 8; i++)
			OutData.dSize[i] = SectData.Size[i];   // 단면치수 ([0]:H, [1]:B)		
	}
	OutData.dYBar = SectData.Design.YBar;
	OutData.dZBar = SectData.Design.ZBar;

	// Rebar
	if (RbarData.bNum) OutData.iArrangeType = 0;
	else              OutData.iArrangeType = 1;

	DGNDRAW_MBAR_BEAM MRbar;

	for (i = 0; i < DGN_RBAR_LAYER_BEAM; i++)
	{
		double dDT = RbarData.dDt[nIMJ][i];
		double dDB = RbarData.dDb[nIMJ][i];

		if (dDT > 0.0)
		{
			MRbar.Initialize();
			MRbar.dDc = dDT;
			MRbar.dSpace = RbarData.dTopBarCTC[nIMJ][i];
			MRbar.dNum = RbarData.dTopBarNum[nIMJ][i];
			OutData.arTopBar.Add(MRbar);
		}
		if (dDB > 0.0)
		{
			MRbar.Initialize();
			MRbar.dDc = dDB;
			MRbar.dSpace = RbarData.dBotBarCTC[nIMJ][i];
			MRbar.dNum = RbarData.dBotBarNum[nIMJ][i];
			OutData.arBotBar.Add(MRbar);
		}
	}

	return TRUE;
}
BOOL CDgnSectionDrawFunc::ConvertToDLL_RcColumnData(T_SECT_SECTBASE_D& SectData, int nIMJ, T_CPCO_D& RbarData, DGNDRAW_SECT_COLUMN& OutData)
{
	// Section
	OutData.Initialize();
	int i;
	OutData.Shape = CT2W(ConvertToDLL_ShapeName(SectData.Shape));          // 단면형상 
	for (i = 0; i < 8; i++)
		OutData.dSize[i] = SectData.Size[i];   // 단면치수 ([0]:H, [1]:B)
	OutData.dYBar = SectData.Design.YBar;
	OutData.dZBar = SectData.Design.ZBar;

	DGNDRAW_GSEC_VERTEX ChkPoint;
	OutData.arChkPoint.SetSize(4);
	ChkPoint.dX = SectData.Stiffness.dx1;  ChkPoint.dY = SectData.Stiffness.dy1;  OutData.arChkPoint.SetAt(0, ChkPoint);
	ChkPoint.dX = SectData.Stiffness.dx2;  ChkPoint.dY = SectData.Stiffness.dy2;  OutData.arChkPoint.SetAt(1, ChkPoint);
	ChkPoint.dX = SectData.Stiffness.dx3;  ChkPoint.dY = SectData.Stiffness.dy3;  OutData.arChkPoint.SetAt(2, ChkPoint);
	ChkPoint.dX = SectData.Stiffness.dx4;  ChkPoint.dY = SectData.Stiffness.dy4;  OutData.arChkPoint.SetAt(3, ChkPoint);

	// Rebar
	DGNDRAW_MBAR_COLUMN MBar;
	for (i = 0; i < DGN_RBAR_LAYER; i++)
	{
		if (RbarData.dDc[nIMJ][i] > 0.0)
		{
			MBar.Initialize();
			MBar.dDc = RbarData.dDc[nIMJ][i];
			MBar.iNum1 = RbarData.iBarNum[nIMJ][0][i];
			MBar.iNum2 = RbarData.iBarNum[nIMJ][1][i];
			MBar.iNum3 = RbarData.iBarNum[nIMJ][2][i];
			OutData.arMainBar.Add(MBar);
		}
	}

	return TRUE;
}
BOOL CDgnSectionDrawFunc::ConvertToDLL_RcColumnData_Gen(T_SECT_SECTBASE_D& SectData, int nIMJ, T_RBTE_D& RbarData, DGNDRAW_SECT_COLUMN& OutData)
{
	// Section
	OutData.Initialize();
	int i, nSize;
	OutData.Shape = CT2W(ConvertToDLL_ShapeName(SectData.Shape));          // 단면형상 
	for (i = 0; i < 8; i++)
		OutData.dSize[i] = SectData.Size[i];   // 단면치수 ([0]:H, [1]:B)
	OutData.dYBar = SectData.Design.YBar;
	OutData.dZBar = SectData.Design.ZBar;

	DGNDRAW_GSEC_VERTEX ChkPoint;
	OutData.arChkPoint.SetSize(4);
	ChkPoint.dX = SectData.Stiffness.dx1;  ChkPoint.dY = SectData.Stiffness.dy1;  OutData.arChkPoint.SetAt(0, ChkPoint);
	ChkPoint.dX = SectData.Stiffness.dx2;  ChkPoint.dY = SectData.Stiffness.dy2;  OutData.arChkPoint.SetAt(1, ChkPoint);
	ChkPoint.dX = SectData.Stiffness.dx3;  ChkPoint.dY = SectData.Stiffness.dy3;  OutData.arChkPoint.SetAt(2, ChkPoint);
	ChkPoint.dX = SectData.Stiffness.dx4;  ChkPoint.dY = SectData.Stiffness.dy4;  OutData.arChkPoint.SetAt(3, ChkPoint);

	OutData.dSize_Gen[0] = fabs(SectData.Stiffness.Czp) + fabs(SectData.Stiffness.Czm);   // 단면치수 ([0]:H, [1]:B)	
	OutData.dSize_Gen[1] = fabs(SectData.Stiffness.Cyp) + fabs(SectData.Stiffness.Cym);
	DGNDRAW_GSEC_POLYGON DrawPolygon;
	nSize = SectData.aOuterPolygon.GetSize();
	OutData.aOuterPolygon.SetSize(nSize);
	for (i = 0; i < nSize; i++)
	{
		if (!ConvertToDLL_Polygon(SectData.aOuterPolygon[i], DrawPolygon)) DrawPolygon.Initialize();
		OutData.aOuterPolygon.SetAt(i, DrawPolygon);
	}
	nSize = SectData.aInnerPolygon.GetSize();
	OutData.aInnerPolygon.SetSize(nSize);
	for (i = 0; i < nSize; i++)
	{
		if (!ConvertToDLL_Polygon(SectData.aInnerPolygon[i], DrawPolygon)) DrawPolygon.Initialize();
		OutData.aInnerPolygon.SetAt(i, DrawPolygon);
	}

	// Rebar
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	int iPosi;
	if (nIMJ == 0 || RbarData.bIJBothMain)
		iPosi = 0;
	else if (nIMJ == 2)
		iPosi = 2;
	else
	{
		ASSERT(0); return FALSE;
	}

	DGNDRAW_GSEC_VERTEX MBar;
	T_RBTE_MABR         MBarDB;
	nSize = RbarData.arMainRebar[iPosi].GetSize();
	OutData.arMainBar_Gen.SetSize(nSize);
	for (i = 0; i < nSize; i++)
	{
		MBarDB = RbarData.arMainRebar[iPosi].GetAt(i);
		MBar.Initialize();
		MBar.dX = MBarDB.dCenterY;
		MBar.dY = MBarDB.dCenterZ;
		OutData.arMainBar_Gen.SetAt(i, MBar);
	}

	return TRUE;
}
BOOL CDgnSectionDrawFunc::ConvertToDLL_RcWallData(T_SECT_SECTBASE_D& SectData, int nIMJ, T_RCWA_D& WallData, T_CPWA_D& RbarData, DGNDRAW_SECT_WALL&   OutData)
{
	// Section
	OutData.Initialize();
	int i;
	OutData.Shape = CT2W(ConvertToDLL_ShapeName(SectData.Shape));          // 단면형상 
	for (i = 0; i < 8; i++)
		OutData.dSize[i] = SectData.Size[i];   // 단면치수 ([0]:H, [1]:B)

	OutData.dHaunch = 0.0;
	if (nIMJ == 0)      OutData.dHaunch = WallData.dHaunch_hi / 3.0;
	else if (nIMJ == 2) OutData.dHaunch = WallData.dHaunch_hj / 3.0;

	OutData.bUnitWidth = RbarData.bUnitWidth;

	// Rebar
	if (RbarData.bNum) OutData.iArrangeType = 0;
	else              OutData.iArrangeType = 1;

	DGNDRAW_MBAR_BEAM MRbar;

	for (i = 0; i < DGN_RBAR_LAYER; i++)
	{
		double dDT = RbarData.dDt[nIMJ][i];
		double dDB = RbarData.dDb[nIMJ][i];

		if (dDT > 0.0)
		{
			MRbar.Initialize();
			MRbar.dDc = dDT;
			MRbar.dSpace = RbarData.dTopBarCTC[nIMJ][i];
			MRbar.dNum = RbarData.dTopBarNum[nIMJ][i];
			OutData.arTopBar.Add(MRbar);
		}
		if (dDB > 0.0)
		{
			MRbar.Initialize();
			MRbar.dDc = dDB;
			MRbar.dSpace = RbarData.dBotBarCTC[nIMJ][i];
			MRbar.dNum = RbarData.dBotBarNum[nIMJ][i];
			OutData.arBotBar.Add(MRbar);
		}
	}

	return TRUE;
}
BOOL CDgnSectionDrawFunc::ConvertToDLL_RcPlateData(T_THIK_D& ThikData, _DGNC_CPPL_D& RbarData, DGNDRAW_SECT_PLATE&  OutData)
{
	// Section
	OutData.Shape = CT2W(ConvertToDLL_ShapeName(D_SECT_SHAPE_REG_SB));
	m_pDoc->m_pAttrCtrl->GetThikForDesign(ThikData, OutData.dThik);

	// Rebar
	if (RbarData.bNum) OutData.iArrangeType = 0;
	else              OutData.iArrangeType = 1;

	DGNDRAW_MBAR_BEAM MRbar;

	for (int i = 0; i < DGN_RBAR_LAYER; i++)
	{
		double dDT = RbarData.dDt[i];
		double dDB = RbarData.dDb[i];

		if (dDT > 0.0)
		{
			MRbar.Initialize();
			MRbar.dDc = dDT;
			MRbar.dSpace = RbarData.dTopBarCTC[i];
			MRbar.dNum = RbarData.dTopBarNum[i];
			OutData.arTopBar.Add(MRbar);
		}
		if (dDB > 0.0)
		{
			MRbar.Initialize();
			MRbar.dDc = dDB;
			MRbar.dSpace = RbarData.dBotBarCTC[i];
			MRbar.dNum = RbarData.dBotBarNum[i];
			OutData.arBotBar.Add(MRbar);
		}
	}

	return TRUE;
}

CString  CDgnSectionDrawFunc::ConvertToDLL_ShapeName(CString Shape)
{
	// DLL에서 사용하는 String값으로 변환 (현재08.01.10 wGen과 동일)
	return Shape;
}
int  CDgnSectionDrawFunc::ConvertToDLL_LengthUnitID(int UnitID)
{
	// DLL에서 사용하는 ID값으로 변환 (현재08.01.10 wGen과 동일)
	return UnitID;
}
