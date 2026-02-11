// CMSecViewWnd.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSecViewWnd.h"

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

/////////////////////////////////////////////////////////////////////////////
// ICMPainter

ICMPainter::ICMPainter()
{
}

ICMPainter::~ICMPainter()
{
}

/////////////////////////////////////////////////////////////////////////////
// ICMPainter public interface functions
void ICMPainter::SetDrawingArea(CRect& r)
{
	// save drawing area
	// drawing area is used in EraseBkGnd, DrawSection
	// must be called before DrawSection
	m_DrawArea = r; 
}

void ICMPainter::EraseBkgnd(CDC* pDC)
{
	if (m_DrawArea.left >= m_DrawArea.right) return;
	if (m_DrawArea.top >= m_DrawArea.bottom) return;

	int nSavedDC = pDC->SaveDC();

	CPen pen(PS_SOLID,1,RGB(255,255,255));
	CBrush backBrush(RGB(0,0,0));
	pDC->SelectObject(&pen);
	pDC->SelectObject(&backBrush);

	CRect rect;
	rect = m_DrawArea;
	pDC->PatBlt(rect.left,rect.top,rect.Width(),rect.Height(),PATCOPY);

	BOOL bDrawGrid = FALSE;
	if (!bDrawGrid) { pDC->RestoreDC(nSavedDC); return; }

	//	Grid Point를 그린다.
	COLORREF color=CCM_WHITE;
	int XIndex,YIndex;
	double CenX,CenY,Space=15.,X,Y;

	CenX = rect.right/2.;		CenY = rect.bottom/2.;
	XIndex = (int)((rect.right/Space)/2);	YIndex = (int)((rect.bottom/Space)/2);

	// 1, 4 사분면
	for(int i=0; i<XIndex+1; i++)
	{
		for(int j=0; j<YIndex+1; j++)
		{
			X = CenX+Space*i;	Y = CenY+Space*j;
			pDC->SetPixel((int)X,(int)Y,color);
		}
		for(int j=0; j<YIndex+1; j++)
		{
			X = CenX+Space*i;	Y = CenY-Space*j;
			pDC->SetPixel((int)X,(int)Y,color);
		}
	}
	// 2, 3 사분면
	for(int i=0; i<XIndex+1; i++)
	{
		for(int j=0; j<YIndex+1; j++)
		{
			X = CenX-Space*i;	Y = CenY+Space*j;
			pDC->SetPixel((int)X,(int)Y,color);
		}
		for(int j=0; j<YIndex+1; j++)
		{
			X = CenX-Space*i;	Y = CenY-Space*j;
			pDC->SetPixel((int)X,(int)Y,color);
		}
	}
	pDC->RestoreDC(nSavedDC);
}

void ICMPainter::DrawSection(CDC* pDC, CCMGeom2DArray &aGeom, BOOL bCentroid, BOOL bOffset, BOOL bShearCheck, int nRebarNum, BOOL bChangeAxis)
{
	/*
	CString cssTemp;
	cssTemp.Format(_T("Geom=%d: bOffset=%d, bCentroid=%d, bShearCheck=%d, nRebar=%d"), 
										aGeom.GetSize(), bOffset, bCentroid, bShearCheck, nRebarNum);
	AfxMessageBox(cssTemp);
	*/

	if (m_DrawArea.left >= m_DrawArea.right) return;
	if (m_DrawArea.top >= m_DrawArea.bottom) return;

	// offset point용 geometry는 뺀다.
	CCMGeom2D *pOffset, *pCentroid, *pStress[4], *pShear[3];
	CArray<CCMGeom2D*, CCMGeom2D*> apRebar;
	BOOL bExist = aGeom.GetSize() > 0;
	SCMRect2D mbr;
	if (bExist) 
	{
		if (bOffset)
		{
			pOffset = aGeom.GetAt(aGeom.GetSize()-1);
			aGeom.RemoveAt(aGeom.GetSize()-1);
		}
		if (bCentroid)
		{
			for (int i=0; i<4; i++)
			{
				pStress[i] = aGeom.GetAt(aGeom.GetSize()-1);
				aGeom.RemoveAt(aGeom.GetSize()-1);
			}
		}
		if (bCentroid)
		{
			pCentroid = aGeom.GetAt(aGeom.GetSize()-1);
			aGeom.RemoveAt(aGeom.GetSize()-1);
		}
		if (bShearCheck)
		{
			for (int i = 0; i < 3; i++)
			{
				pShear[i] = aGeom.GetAt(aGeom.GetSize()-1);
				aGeom.RemoveAt(aGeom.GetSize()-1);
			}
		}
		// mbr 구할 때 Rebar는 빼고 구한다.
		for (int i = 0; i < nRebarNum; i++)
		{
			apRebar.Add(aGeom.GetAt(aGeom.GetSize()-1));
			aGeom.RemoveAt(aGeom.GetSize()-1);
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
	if (nRebarNum > 0)
	{
		CPoint newOffset = offset;
		newOffset.x += (long)((mbr.max.x-mbr.min.x)/2.0*scale);
		newOffset.y += (long)((mbr.max.y-mbr.min.y)/2.0*scale);
		for (int i = 0; i < nRebarNum; i++) 
		{
			apRebar[i]->Transfer(SCMPoint2D(-(mbr.max.x+mbr.min.x)/2.0, -(mbr.max.y+mbr.min.y)/2.0));
			apRebar[i]->Draw(pDC, rect, scale, newOffset, apRebar[i]->PenColor, apRebar[i]->BrushColor);
			aGeom.Add(apRebar[i]);  // aGeom이 삭제될 때 pRebar도 함께 삭제되도록 다시 add한다.
		}
	}

	// offset point를 그린다.
	if (bExist)
	{
		// offset point를 그린다.
		if (bOffset)
		{
			pOffset->Transfer(SCMPoint2D(-(mbr.max.x+mbr.min.x)/2.0, -(mbr.max.y+mbr.min.y)/2.0));
			CPoint newOffset = offset;
			newOffset.x += (long)((mbr.max.x-mbr.min.x)/2.0*scale);
			newOffset.y += (long)((mbr.max.y-mbr.min.y)/2.0*scale);
			pOffset->Draw(pDC, rect, scale, newOffset, pOffset->PenColor, pOffset->BrushColor);
			aGeom.Add(pOffset); // aGeom이 삭제될 때 pOffset도 함께 삭제되도록 다시 add한다.
		}
		// Centroid를 그린다.
		if (bCentroid)
		{
			pCentroid->Transfer(SCMPoint2D(-(mbr.max.x+mbr.min.x)/2.0, -(mbr.max.y+mbr.min.y)/2.0));
			CPoint newCentroid = offset;
			newCentroid.x += (long)((mbr.max.x-mbr.min.x)/2.0*scale);
			newCentroid.y += (long)((mbr.max.y-mbr.min.y)/2.0*scale);
			pCentroid->Draw(pDC, rect, scale, newCentroid, pCentroid->PenColor, pCentroid->BrushColor);
			aGeom.Add(pCentroid); // aGeom이 삭제될 때 pCentroid 함께 삭제되도록 다시 add한다.
			// 응력 위치 출력 
			CString csLabel;
			double x, y;
			SCMRect2D textmbr;
	    CPen penStressPt(PS_SOLID, 1, RGB(255,0,0));
			for (int i=0; i<4; i++)
			{
				if (pStress[i])
				{
					pStress[i]->Transfer(SCMPoint2D(-(mbr.max.x+mbr.min.x)/2.0, -(mbr.max.y+mbr.min.y)/2.0));
					CPoint newStress = offset;
					newStress.x += (long)((mbr.max.x-mbr.min.x)/2.0*scale);
					newStress.y += (long)((mbr.max.y-mbr.min.y)/2.0*scale);
					pStress[i]->Draw(pDC, rect, scale, newStress, pStress[i]->PenColor, pStress[i]->BrushColor);
					aGeom.Add(pStress[i]); // aGeom이 삭제될 때 pStress 함께 삭제되도록 다시 add한다.
					// 번호 출력
					textmbr = pStress[i]->GetMBR();
					x = (textmbr.min.x+textmbr.max.x)/2.0;
					y = (textmbr.min.y+textmbr.max.y)/2.0;
					x = RoundUp(x*scale)+(int)newStress.x;
					y = RoundUp(y*scale)+(int)newStress.y;
					if (i == 2 || i == 3) y -= 15;
					if (i == 0 || i == 3) x -= 10;
					int nSavedDC = pDC->SaveDC();
					pDC->SelectObject(&penStressPt);
					pDC->SetBkMode(TRANSPARENT);
					pDC->SetTextColor(RGB(255,0,0));
					csLabel.Format(_T("%d"), 4-i);
					pDC->TextOut(x, y, csLabel);
	        pDC->RestoreDC(nSavedDC);
				}
			}
		}
		// 전단 검토 위치를 그린다
		if (bShearCheck)
		{
			CString csLabel;
			double x, y;
			SCMRect2D textmbr;
	    CPen penShear(PS_SOLID, 1, CCM_CYAN);
			for (int i=0; i<3; i++)
			{
				aGeom.Add(pShear[i]); // aGeom이 삭제될 때 pShear 함께 삭제되도록 다시 add한다.
				if (pShear[i] && pShear[i]->BrushColor != CCM_BLACK)
				{
					pShear[i]->Transfer(SCMPoint2D(-(mbr.max.x+mbr.min.x)/2.0, -(mbr.max.y+mbr.min.y)/2.0));
					CPoint newShear = offset;
					newShear.x += (long)((mbr.max.x-mbr.min.x)/2.0*scale);
					newShear.y += (long)((mbr.max.y-mbr.min.y)/2.0*scale);
					pShear[i]->Draw(pDC, rect, scale, newShear, pShear[i]->PenColor, pShear[i]->BrushColor);
					// Label 출력
					textmbr = pShear[i]->GetMBR();
					x = textmbr.max.x;
					y = textmbr.max.y;
					x = RoundUp(x*scale)+(int)newShear.x;
					y = RoundUp(y*scale)+(int)newShear.y;
					x += 5;
					y -= 8;
					int nSavedDC = pDC->SaveDC();
					pDC->SelectObject(&penShear);
					pDC->SetBkMode(TRANSPARENT);
					pDC->SetTextColor(CCM_CYAN);
					csLabel.Format(_T("Z%d"), 3-i);
					pDC->TextOut(x, y, csLabel);
	        pDC->RestoreDC(nSavedDC);
				}
			}
		}
	}  
	// 축을 그린다.
	DrawAxis(pDC, org, bChangeAxis);
}

/////////////////////////////////////////////////////////////////////////////
// ICMPainter implementation functions
int ICMPainter::RoundUp(double dVal)
{
	if(dVal > 0.0)
	   return (int)floor(dVal+(double)0.5);
	else
	   return (int)ceil(dVal-(double)0.5);
}

void ICMPainter::DrawAxis(CDC * pDC, CPoint &org, BOOL bChangeAxis)
{
	CPen pen(PS_SOLID, 1, RGB(0,0,255));
	int nSavedDC = pDC->SaveDC();

	pDC->SelectObject(&pen);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0,0,255));
	
	pDC->Ellipse(org.x-2, org.y-2, org.x+2, org.y+2);

	pDC->MoveTo(org.x   , org.y   );
	pDC->LineTo(org.x   , org.y-15);
	pDC->MoveTo(org.x   , org.y-20);
	pDC->LineTo(org.x-2 , org.y-15);
	pDC->MoveTo(org.x-2 , org.y-15);
	pDC->LineTo(org.x+2 , org.y-15);
	pDC->MoveTo(org.x+2 , org.y-15);
	pDC->LineTo(org.x   , org.y-20);
	if(bChangeAxis) pDC->TextOut(org.x-4, org.y-36, _T("y"), 1);
	else            pDC->TextOut(org.x-4, org.y-36, _T("z"), 1);  

	pDC->MoveTo(org.x    , org.y  );
	pDC->LineTo(org.x+15 , org.y  );
	pDC->MoveTo(org.x+20 , org.y  );
	pDC->LineTo(org.x+15 , org.y-2);
	pDC->MoveTo(org.x+20 , org.y  );
	pDC->LineTo(org.x+15 , org.y+2);
	pDC->MoveTo(org.x+15 , org.y-2);
	pDC->LineTo(org.x+15 , org.y+2);
	if(bChangeAxis) pDC->TextOut(org.x+25, org.y-8, _T("x"), 1);
	else            pDC->TextOut(org.x+25, org.y-8, _T("y"), 1);	

	pDC->RestoreDC(nSavedDC);
}

void ICMPainter::CalcScaleOrgOff(CRect &rect, CCMGeom2DArray &aGeom, 
																 double &scale, CPoint &org, CPoint &offset)
{
	SCMRect2D mbr = aGeom.GetMBR();
	
	// calculate scale
	double xRatio, yRatio;
	if (mbr.Width() == 0.0) xRatio = 0.0; //xRatio = DBL_MAX;
	else xRatio = ((double)(rect.Width()))/mbr.Width()*0.7;
	if (mbr.Height() == 0.0) yRatio = 0.0; //yRatio = DBL_MAX;
	else yRatio = ((double)(rect.Height()))/mbr.Height()*0.7;
	scale = min(xRatio, yRatio);
	if (scale == 0.0) scale = max(xRatio, yRatio);

	// calculate origin's screen coordinate
	SCMPoint2D dOff;

	dOff.x = (mbr.Width()*scale/2.0);
	dOff.y = (mbr.Height()*scale/2.0);

	offset.x = rect.Width()/2-(int)dOff.x;
	offset.y = rect.Height()/2-(int)dOff.y;

	org.x = rect.Width()/2; // - RoundUp(mbr.Width()/2.0*scale);
	org.y = rect.Height()/2; // + RoundUp(mbr.Height()/2.0*scale);
}

/////////////////////////////////////////////////////////////////////////////
// ICMGeomMaker

ICMGeomMaker::ICMGeomMaker()
{
	m_pDataSrc = 0;
}

ICMGeomMaker::~ICMGeomMaker()
{
}

/////////////////////////////////////////////////////////////////////////////
// ICMGeomMaker public interface functions
void ICMGeomMaker::MakePicture(T_SECT_D *pDataSrc, CCMGeom2DArray &aGeom,
															 BOOL bAfter, BOOL bReverse, 
															 BOOL bCentroid, BOOL bOffset, BOOL bShearCheck,
															 T_RPSC_D *pDataSrcRebar, BOOL bEndJ, int* pnRebarNum, BOOL bCGI/*=FALSE*/)
{
	if (pnRebarNum) *pnRebarNum = 0;
	// set data source to use implementation functions
	m_pDataSrc = pDataSrc;
	if (m_pDataSrc == 0) return;
	CSectUtil SecUtil;

	CCMGeom2DArray aGeomPart1[2], aGeomPart2[2]; // composite 단면용, 1:girder, 2:slab
	aGeomPart1[0].RemoveAll(), aGeomPart1[1].RemoveAll();
	aGeomPart2[0].RemoveAll(), aGeomPart2[1].RemoveAll();
	// build geometry by section type
	switch(m_pDataSrc->nStype)
	{
	case D_SECT_TYPE_PSC    :
		{
			MakePicturePSC(*m_pDataSrc,aGeom);
		}
		break;
	case D_SECT_TYPE_REGULAR:
	case D_SECT_TYPE_USER:
		{
			CString SectionShape = pDataSrc->SectBefore.Shape;
			int i = SecUtil.GetShapeIndexFromNameReg(SectionShape);
			switch(i)
			{
			case 0 : MakePictureAngle       (aGeom, bReverse); break; // case of thickness, bReverse = TRUE
			case 1 : MakePictureChannel     (aGeom); break;
			case 2 : MakePictureHBeam       (aGeom); break;
			case 3 : MakePictureTee         (aGeom, bReverse); break; // case of thickness, bReverse = TRUE
			case 4 : MakePictureBox         (aGeom); break;
			case 5 : MakePicturePipe        (aGeom); break;
			case 6 : MakePictureDblAngle    (aGeom, bReverse); break; // case of thickness, bReverse = TRUE
			case 7 : MakePictureDblChannel  (aGeom); break;
			case 8 : MakePictureSolidRect   (aGeom); break;
			case 9 : MakePictureSolidCircle (aGeom); break;
			case 10: MakePictureRegColdFormedChannel(aGeom); break;
			case 11: MakePictureRegURib     (aGeom); break;
			case 12: MakePictureOctagon     (aGeom); break;
			case 13: MakePictureSolidOctagon(aGeom); break;
			case 14: MakePictureTrack       (aGeom); break;
			case 15: MakePictureSolidTrack  (aGeom); break;
			case 16: MakePictureHalfTrack   (aGeom); break;
			case 17: MakePictureCrossAngle  (aGeom); break;
			case 18: MakePicturePSC(*m_pDataSrc,aGeom); break;  // R Octagon을 PSC같이 취급함 
			case 19: MakePicturePSC(*m_pDataSrc,aGeom); break;  // Box with Stiffener를 PSC같이 취급함
			case 20: MakePicturePSC(*m_pDataSrc,aGeom); break;  // Pipe with Stiffener를 PSC같이 취급함
			case 21: 
				{
					T_SECT_D TempSectD = *m_pDataSrc;
					TempSectD.nStype = D_SECT_TYPE_PSC;
					TempSectD.SectBefore.Shape = D_SECT_SHAPE_PSC_VALUE;
					MakePicturePSC(TempSectD,aGeom);
				}
				break;
			case 22: MakePictureUpsideDownTee         (aGeom, bReverse); break; // case of thickness, bReverse = TRUE
			case 23: MakePictureRegColdFormedBox(aGeom); break;             // MNET:4258-JWKWON-20110108 : 일본 2차 설계 
			case 24: MakePictureUpright  (aGeom); break;
			case 25: MakePictureZ  (aGeom); break;
			case 26: MakePictureRegColdFormedChannelHat(aGeom); break;
			case 27: MakePictureDblChannelBox(aGeom); break;
			default: ASSERT(0); break;
			}
		}
		break;
	case D_SECT_TYPE_SRC:
		{
			CString SectionShape = pDataSrc->SectBefore.Shape;
			int i = SecUtil.GetShapeIndexFromNameSrc(SectionShape);
			switch(i)
			{
			case 0: MakePictureRectBox(aGeom, TRUE); break;
			case 1: MakePictureRectBox(aGeom, FALSE); break;
			case 2: MakePictureRectPipe(aGeom, TRUE); break;
			case 3: MakePictureRectPipe(aGeom, FALSE); break;
			case 4: MakePictureCircleBox(aGeom, TRUE); break;
			case 5: MakePictureCircleBox(aGeom, FALSE); break;
			case 6: MakePictureCirclePipe(aGeom, TRUE); break;
			case 7: MakePictureCirclePipe(aGeom, FALSE); break;
			case 8: MakePictureSrcBox(aGeom); break;
			case 9: MakePictureSrcPipe(aGeom); break;
			case 10: MakePictureRectHBeam(aGeom); break;
			case 11: MakePictureCircleHBeam(aGeom); break;
			case 12: MakePictureRectH2TBeam(aGeom); break;
			case 13: MakePictureRectHTBeam(aGeom); break;
			case 14: MakePicturePSC(*m_pDataSrc,aGeom); break;
			case 15: MakePicturePSC(*m_pDataSrc,aGeom); break;
			case 16: MakePictureSrcPipe(aGeom); break;
			case 17: MakePictureSrcDumbbell(*m_pDataSrc, aGeom); break;
			}
		}
		break;
	case D_SECT_TYPE_COMBINED:
		if (SecUtil.IsCombinedAType(*pDataSrc))   // combined A type
		{
			CString SectionShape = pDataSrc->SectBefore.Shape;
			int i = SecUtil.GetShapeIndexFromNameComA(SectionShape);
			switch(i)
			{
			case 0: MakePicture2H(aGeom); break;
			case 1: MakePictureHC1(aGeom); break;
			case 2: MakePictureHC2(aGeom); break;
			case 3: MakePictureHT(aGeom); break;
			case 4: MakePicutre2T1(aGeom); break;
			case 5: MakePicture2T2(aGeom); break;
			case 6: MakePictureH2T(aGeom); break;
			case 7: MakePictureWOH(aGeom); break;
			case 8: MakePictureFOH(aGeom); break;
			}
		}
		else  // combined B type
		{
			CString SectionShape = pDataSrc->SectBefore.Shape;
			int i = SecUtil.GetShapeIndexFromNameComB(SectionShape);
			switch(i)
			{
			case 0: MakePicture2CW1(aGeom); break;
			case 1: MakePicture2CW2(aGeom); break;
			case 2: MakePicture1CW1(aGeom); break;
			case 3: MakePicture1CW2(aGeom); break;
			case 4: MakePicture4L(aGeom); break;
			case 5: MakePictureHP(aGeom); break;
			}
		}
		break;
	case D_SECT_TYPE_TAPERED:
		{
			CString SectionShape = pDataSrc->SectBefore.Shape;
			if (pDataSrc->SectBefore.nStype == D_SECT_TYPE_PSC)
			{
				MakePictureTaperedPsc(*m_pDataSrc, aGeom);
			}
			else if(pDataSrc->SectBefore.nStype==D_SECT_TYPE_COMPO_B ||
							pDataSrc->SectBefore.nStype==D_SECT_TYPE_COMPO_I ||
							pDataSrc->SectBefore.nStype==D_SECT_TYPE_COMPO_CI ||
							pDataSrc->SectBefore.nStype==D_SECT_TYPE_COMPO_CT ||
							pDataSrc->SectBefore.nStype==D_SECT_TYPE_COMPO_TUB)
			{
				int nType = SecUtil.GetShapeIndexFromNameComp(SectionShape);
				switch(nType)
				{
				case 0: MakePictureTaperedSteelBox(aGeom, aGeomPart1, aGeomPart2);  break;
				case 1: MakePictureTaperedSteelI(aGeom, aGeomPart1, aGeomPart2);    break;
				case 2: MakePictureTaperedConcreteI(aGeom, aGeomPart1, aGeomPart2); break;
				case 3: MakePictureTaperedConcreteT(aGeom, aGeomPart1, aGeomPart2); break;
				case 5: MakePictureTaperedSteelTub(aGeom, aGeomPart1, aGeomPart2);  break;
				default: ASSERT(0); break;
				}
			}
			else if (pDataSrc->SectBefore.nStype==D_SECT_TYPE_COMPO_G)
			{
				if (bCGI)
				{
					T_SECT_D TempSectD = *m_pDataSrc;
					TempSectD.nStype = D_SECT_TYPE_TAPERED;
					TempSectD.SectBefore.Shape = D_SECT_SHAPE_PSC_VALUE;
					
					for(int i=0 ; i<TempSectD.SectBefore.SectJ.aGeneralPart.GetSize() ; ++i)
					{
						TempSectD.SectBefore.SectI.aOuterPolygon.RemoveAll();
						TempSectD.SectBefore.SectI.aInnerPolygon.RemoveAll();
						TempSectD.SectBefore.SectI.aLine.RemoveAll();
						
						int nStart=0 , nEnd=0 ;
						nStart = TempSectD.SectBefore.SectJ.aGeneralPart[i].IdxStart[1];
						nEnd   = TempSectD.SectBefore.SectJ.aGeneralPart[i].IdxEnd[1];
						for(int j=nStart ; j<nEnd ; ++j)
							//for(int j=nEnd-1 ; j>=nStart ; --j)
							TempSectD.SectBefore.SectI.aInnerPolygon.Add( (*m_pDataSrc).SectBefore.SectJ.aInnerPolygon[j] );
						
						nStart = TempSectD.SectBefore.SectJ.aGeneralPart[i].IdxStart[2];
						nEnd   = TempSectD.SectBefore.SectJ.aGeneralPart[i].IdxEnd[2];
						for(int  j=nStart ; j<nEnd ; ++j)
							//for( j=nEnd-1 ; j>=nStart ; --j)
							TempSectD.SectBefore.SectI.aLine.Add( (*m_pDataSrc).SectBefore.SectJ.aLine[j] );
						
						nStart = TempSectD.SectBefore.SectJ.aGeneralPart[i].IdxStart[0];
						nEnd   = TempSectD.SectBefore.SectJ.aGeneralPart[i].IdxEnd[0];
						for(int  j=nStart ; j<nEnd ; ++j)
							//for( j=nEnd-1 ; j>=nStart ; --j)
						{
							TempSectD.SectBefore.SectI.aOuterPolygon.RemoveAll();
							TempSectD.SectBefore.SectI.aOuterPolygon.Add( (*m_pDataSrc).SectBefore.SectJ.aOuterPolygon[j] );
							MakePicturePSC(TempSectD,aGeom); // Current Part 에 대해 생성
						}
					}
				}
				else
				{          
					T_SECT_D TempSectD = *m_pDataSrc;
					TempSectD.nStype = D_SECT_TYPE_PSC;
					TempSectD.SectBefore.Shape = D_SECT_SHAPE_PSC_VALUE;
					
					for(int i=0 ; i<TempSectD.SectBefore.SectI.aGeneralPart.GetSize() ; ++i)
					{
						TempSectD.SectBefore.SectI.aOuterPolygon.RemoveAll();
						TempSectD.SectBefore.SectI.aInnerPolygon.RemoveAll();
						TempSectD.SectBefore.SectI.aLine.RemoveAll();
						
						int nStart=0 , nEnd=0 ;
						nStart = TempSectD.SectBefore.SectI.aGeneralPart[i].IdxStart[1];
						nEnd   = TempSectD.SectBefore.SectI.aGeneralPart[i].IdxEnd[1];
						for(int j=nStart ; j<nEnd ; ++j)
							TempSectD.SectBefore.SectI.aInnerPolygon.Add( (*m_pDataSrc).SectBefore.SectI.aInnerPolygon[j] );
						
						nStart = TempSectD.SectBefore.SectI.aGeneralPart[i].IdxStart[2];
						nEnd   = TempSectD.SectBefore.SectI.aGeneralPart[i].IdxEnd[2];
						for(int  j=nStart ; j<nEnd ; ++j)
							TempSectD.SectBefore.SectI.aLine.Add( (*m_pDataSrc).SectBefore.SectI.aLine[j] );
						
						nStart = TempSectD.SectBefore.SectI.aGeneralPart[i].IdxStart[0];
						nEnd   = TempSectD.SectBefore.SectI.aGeneralPart[i].IdxEnd[0];
						for(int j=nStart ; j<nEnd ; ++j)
						{
							TempSectD.SectBefore.SectI.aOuterPolygon.RemoveAll();
							TempSectD.SectBefore.SectI.aOuterPolygon.Add( (*m_pDataSrc).SectBefore.SectI.aOuterPolygon[j] );
							MakePicturePSC(TempSectD,aGeom); // Current Part 에 대해 생성
						}
					}
				}
			}
			else if(pDataSrc->SectBefore.nStype==D_SECT_TYPE_STLG_B ||
				      pDataSrc->SectBefore.nStype==D_SECT_TYPE_STLG_I ||
							pDataSrc->SectBefore.nStype==D_SECT_TYPE_STLG_MCELL)
			{
				int nType = SecUtil.GetShapeIndexFromNameStlGirder(SectionShape);
				switch(nType)
				{
					case 0: MakePictureTaperedStlGirderBox(aGeom);   break;
					case 1: MakePictureTaperedStlGirderI(aGeom);     break;
					case 2: MakePictureTaperedStlGirderMCell(aGeom); break;
					default: ASSERT(0); break;
				}
			}
			else if(pDataSrc->SectBefore.nStype==D_SECT_TYPE_COMPO_STLG_B)
			{
				MakePictureTaperedStlGirderBox(aGeom, aGeomPart1, aGeomPart2);
			}
			else if(pDataSrc->SectBefore.nStype==D_SECT_TYPE_COMPO_STLG_I)
			{
				MakePictureTaperedStlGirderI(aGeom, aGeomPart1, aGeomPart2);
			}
			else if(pDataSrc->SectBefore.nStype==D_SECT_TYPE_COMPO_STLG_TUB)
			{
				MakePictureTaperedStlGirderTub(aGeom, aGeomPart1, aGeomPart2);
			}
			else
			{
				int i = SecUtil.GetShapeIndexFromNameReg(SectionShape);
				switch(i)
				{
				case  0: MakePictureTaperedAngle(aGeom); break;
				case  1: MakePictureTaperedChannel(aGeom); break;
				case  2: MakePictureTaperedHBeam(aGeom); break;
				case  3: MakePictureTaperedTee(aGeom); break;
				case  4: MakePictureTaperedBox(aGeom); break;
				case  5: MakePictureTaperedPipe(aGeom); break;
				case  6: MakePictureTaperedDblAngle(aGeom); break;
				case  7: MakePictureTaperedDblChannel(aGeom); break;
				case  8: MakePictureTaperedSolidRect(aGeom); break;
				case  9: MakePictureTaperedSolidCircle(aGeom); break;
				case 12: MakePictureTaperedOctagon(aGeom); break;
				case 13: MakePictureTaperedSolidOctagon(aGeom); break;
				case 14: MakePictureTaperedTrack(aGeom); break;
				case 15: MakePictureTaperedSolidTrack(aGeom); break;
				case 16: MakePictureTaperedHalfTrack(aGeom); break;
				case 21:
					{
						T_SECT_D TempSectD = *m_pDataSrc;
						TempSectD.nStype = D_SECT_TYPE_TAPERED;
						TempSectD.SectBefore.nStype = D_SECT_TYPE_PSC;
						TempSectD.SectBefore.Shape = D_SECT_SHAPE_PSC_VALUE;
						MakePictureTaperedPsc(TempSectD,aGeom);
					}
					break;
				}
			}
		}
		break;
	case D_SECT_TYPE_CONSTRUCTION:
		{
			T_SECT_D Sect;
			CSectUtil::GetSectFromConPart(*pDataSrc, Sect, bAfter);
			//MakePicture(&Sect, aGeom, FALSE, FALSE, TRUE, TRUE, NULL, FALSE, 0, 0);
			MakePicture(&Sect, aGeom, FALSE, FALSE, bCentroid, bOffset, NULL, FALSE, 0, 0); // MQC:2646 mylee 20090525
			m_pDataSrc = pDataSrc;
		}
		break;
	case D_SECT_TYPE_COMPO_B:
		MakePictureCompoB(aGeom, aGeomPart1, aGeomPart2);
		break;
	case D_SECT_TYPE_COMPO_I:
		MakePictureCompoI(aGeom, aGeomPart1, aGeomPart2);
		break;
	case D_SECT_TYPE_COMPO_TUB:
		MakePictureCompoTub(aGeom, aGeomPart1, aGeomPart2);
		break;
	case D_SECT_TYPE_COMPO_CI:
		MakePictureCompoCI(aGeom, aGeomPart1, aGeomPart2);
		break;
	case D_SECT_TYPE_COMPO_CT:
		MakePictureCompoCT(aGeom, aGeomPart1, aGeomPart2);
		break;
	case D_SECT_TYPE_COMPO_PC:
		MakePictureCompoPC(aGeom, aGeomPart1, aGeomPart2);
		break;
	case D_SECT_TYPE_COMPO_G: // 20080223 mylee
		{
			T_SECT_D TempSectD = *m_pDataSrc;
			TempSectD.nStype = D_SECT_TYPE_PSC;
			TempSectD.SectBefore.Shape = D_SECT_SHAPE_PSC_VALUE;

			for(int i=0 ; i<TempSectD.SectBefore.SectI.aGeneralPart.GetSize() ; ++i)
			{
				TempSectD.SectBefore.SectI.aOuterPolygon.RemoveAll();
				TempSectD.SectBefore.SectI.aInnerPolygon.RemoveAll();
				TempSectD.SectBefore.SectI.aLine.RemoveAll();

				int nStart=0 , nEnd=0 ;
				nStart = TempSectD.SectBefore.SectI.aGeneralPart[i].IdxStart[1];
				nEnd   = TempSectD.SectBefore.SectI.aGeneralPart[i].IdxEnd[1];
				for(int j=nStart ; j<nEnd ; ++j)
					TempSectD.SectBefore.SectI.aInnerPolygon.Add( (*m_pDataSrc).SectBefore.SectI.aInnerPolygon[j] );

				nStart = TempSectD.SectBefore.SectI.aGeneralPart[i].IdxStart[2];
				nEnd   = TempSectD.SectBefore.SectI.aGeneralPart[i].IdxEnd[2];
				for(int j=nStart ; j<nEnd ; ++j)
					TempSectD.SectBefore.SectI.aLine.Add( (*m_pDataSrc).SectBefore.SectI.aLine[j] );

				nStart = TempSectD.SectBefore.SectI.aGeneralPart[i].IdxStart[0];
				nEnd   = TempSectD.SectBefore.SectI.aGeneralPart[i].IdxEnd[0];
				for(int j=nStart ; j<nEnd ; ++j)
				{
					TempSectD.SectBefore.SectI.aOuterPolygon.RemoveAll();
					TempSectD.SectBefore.SectI.aOuterPolygon.Add( (*m_pDataSrc).SectBefore.SectI.aOuterPolygon[j] );
					MakePicturePSC(TempSectD,aGeom); // Current Part 에 대해 생성
				}
			}   
		}
		break;
	case D_SECT_TYPE_COLDFORMED:
		{
			CString SectionShape = pDataSrc->SectBefore.Shape;
			int i = SecUtil.GetShapeIndexFromNameCfm(SectionShape);
			switch(i)
			{
			case 0: MakePictureColdFormedChannel(aGeom); break;
			case 1: MakePictureColdFormedChannel2I(aGeom); break;
			case 2: MakePictureColdFormedChannel2B(aGeom); break;
			case 3: MakePictureColdFormedChannel3(aGeom); break;
			case 4: MakePictureColdFormedChannel4(aGeom); break;
			}
		}
		break;
	case D_SECT_TYPE_STLG_B:
	case D_SECT_TYPE_STLG_I:
	case D_SECT_TYPE_STLG_MCELL:
		{
			CString SectionShape = pDataSrc->SectBefore.Shape;
			int nType = SecUtil.GetShapeIndexFromNameStlGirder(SectionShape);
			switch(nType)
			{
				case 0: MakePictureStlGirderBox(aGeom);   break;
				case 1: MakePictureStlGirderI(aGeom);     break;
				case 2: MakePictureStlGirderMCell(aGeom); break;
				default: ASSERT(0); break;
			}
		}
		break;
	case D_SECT_TYPE_COMPO_STLG_B:
		MakePictureCompoStlGirderBox(aGeom, aGeomPart1, aGeomPart2);
		break;
	case D_SECT_TYPE_COMPO_STLG_I:
		MakePictureCompoStlGirderI(aGeom, aGeomPart1, aGeomPart2);
		break;
	case D_SECT_TYPE_COMPO_STLG_TUB:
		MakePictureCompoStlGirderTub(aGeom, aGeomPart1, aGeomPart2);
		break;
	default:
		return;
		//ASSERT(0);
		//break;
	}

	if (m_pDataSrc->bPricipalAxis)
	{
		double dTheta = m_pDataSrc->SectBefore.SectI.Size[5];
		dTheta *= -1;	// 반시계방향으로 +방향 보정
		aGeom.Rotate(dTheta);
	}

	// Shape에 대한 정보만 가지고 mbr을 만든다. (mbr에 따라 scale 결정)
	SCMRect2D mbr;
	if (m_pDataSrc->nStype == D_SECT_TYPE_CONSTRUCTION) return;
	if (m_pDataSrc->nStype == D_SECT_TYPE_TAPERED && m_pDataSrc->SectBefore.nStype!=D_SECT_TYPE_COMPO_G) mbr = m_TaperedMBR;
	else mbr = aGeom.GetMBR();

	// mbr 먼저 구한 후에 Rebar 정보 추가한다.
	// Rebar 정보 있는 경우
	if (pDataSrcRebar)
	{
		SCMPoint2D pt1, pt2;
		CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* paMbar;
		if (bEndJ)
		{
			if (pDataSrcRebar->bMbarJ) paMbar = &pDataSrcRebar->Mbar[1]; // J 따로
			else paMbar = &pDataSrcRebar->Mbar[0];
		}
		else paMbar = &pDataSrcRebar->Mbar[0];
		SCMRect2D mbr_sub = aGeom.GetMBR();
		// BOOL bPart1 = (bEndJ) ? (aGeomPart1[1].GetSize()>0) : (aGeomPart1[0].GetSize()>0);
		// BOOL bPart2 = (bEndJ) ? (aGeomPart2[1].GetSize()>0) : (aGeomPart2[0].GetSize()>0);
		// changed by mylee 20070307 J단인 경우도 aGeomPartX[0] 으로 넘어옴
		BOOL bPart1 = (aGeomPart1[0].GetSize()>0);
		BOOL bPart2 = (aGeomPart2[0].GetSize()>0);

		for (int i = 0; i < paMbar->GetSize(); i++)
		{
			CCMLine *pRebar = new CCMLine(CCM_YELLOW, CCM_WHITE);
			pRebar->SetPtList((*paMbar)[i].iBarNum);
			double dRebarLen = (*paMbar)[i].dPitch*((*paMbar)[i].iBarNum-1);

			double dOrgX = mbr_sub.min.x, dCntX = m_pDataSrc->SectBefore.SectI.Design.YBar;
			double dMinY = mbr_sub.min.y, dMaxY = mbr_sub.max.y;
			int nPart = (*paMbar)[i].nPart;
			if(nPart==1 && bPart1)
			{
				// SCMRect2D mbrPart = (bEndJ) ? aGeomPart1[1].GetMBR() : aGeomPart1[0].GetMBR();
				SCMRect2D mbrPart = aGeomPart1[0].GetMBR();
				dOrgX = mbr_sub.min.x + fabs(mbr_sub.min.x-mbrPart.min.x);
				dCntX = CSectUtil::GetPartCentroidY(m_pDataSrc, nPart);
				dMinY = mbr_sub.min.y + fabs(mbr_sub.min.y-mbrPart.min.y);
				dMaxY = mbr_sub.max.y - fabs(mbr_sub.max.y-mbrPart.max.y);
			}
			if(nPart==2 && bPart2)
			{
				// SCMRect2D mbrPart = (bEndJ) ? aGeomPart2[1].GetMBR() : aGeomPart2[0].GetMBR();
				SCMRect2D mbrPart = aGeomPart2[0].GetMBR();
				dOrgX = mbr_sub.min.x + fabs(mbr_sub.min.x-mbrPart.min.x);
				dCntX = CSectUtil::GetPartCentroidY(m_pDataSrc, nPart);
				dMinY = mbr_sub.min.y + fabs(mbr_sub.min.y-mbrPart.min.y);
				dMaxY = mbr_sub.max.y - fabs(mbr_sub.max.y-mbrPart.max.y);
			}
			if ((*paMbar)[i].iStaYRef == 0)  // Centroid
			{
				double dCentroidX = dOrgX + dCntX; // mbr.min.x + m_pDataSrc->SectBefore.SectI.Design.YBar;
				pt1.x = dCentroidX + (*paMbar)[i].dStaY - dRebarLen/2.0;
				pt2.x = pt1.x + dRebarLen;
			}
			else  // left
			{
				pt1.x = dOrgX + (*paMbar)[i].dStaY;
				pt2.x = pt1.x + dRebarLen;
			}
			// min, max 방향 유의 (min.y가 max.y보다 그림상 위에 간다.)
			if ((*paMbar)[i].iStaZRef == 0) // top
			{
				pt1.y = pt2.y = dMinY + (*paMbar)[i].dStaZ; 
			}
			else
			{
				pt1.y = pt2.y = dMaxY - (*paMbar)[i].dStaZ;
			}
			pRebar->Set(pt1, pt2);
			aGeom.Add(pRebar);
			if (pnRebarNum) *pnRebarNum += 1;
		}
	}
	aGeomPart1[0].RemoveAll(), aGeomPart1[1].RemoveAll();
	aGeomPart2[0].RemoveAll(), aGeomPart2[1].RemoveAll();

	// 전단 검토 위치 만든다.
	if (bShearCheck)
	{
		SCMRect2D mbr_sub = aGeom.GetMBR();
		SCMPoint2D pt1, pt2;
		int ix = (bEndJ) ? 1 : 0;
		for (int i = 0; i < 3; i++)
		{
			CCMLine *pShear = new CCMLine(CCM_CYAN, CCM_WHITE);
			if (m_pDataSrc->SectBefore.dShearCheckPos[ix][i] == 0.0) 
				pShear->BrushColor = CCM_BLACK;  // 안그리기 플래그
			else
			{
				// 좌표축 유의(min.y가 위, max.y가 밑이 되어 있다.)
				pt1.x = mbr_sub.min.x;
				pt1.y = mbr_sub.max.y - m_pDataSrc->SectBefore.dShearCheckPos[ix][i];
				pt2.x = mbr_sub.max.x;
				pt2.y = pt1.y;
				pShear->Set(pt1, pt2);
			}
			aGeom.Add(pShear);
		}
	}

	// centroid, offset point를 위한 geometry를 만들어 넣는다.
	double x = 0.0;
	double y = 0.0;
	double centroidX = 0.0;
	double centroidY = 0.0;
	// centroid point를 위한 geometry를 만들어 넣는다.
	// 단, centroid 계산이 가능한 경우에만 
	if (bCentroid)
	{
		CCMPolygon *pCentroid = new CCMPolygon(CCM_WHITE, CCM_BLUE);

		if (m_pDataSrc->nStype == D_SECT_TYPE_SRC ||
				m_pDataSrc->nStype == D_SECT_TYPE_COMBINED)
		{
			x = mbr.min.x + m_pDataSrc->SectBefore.Design.YBar;
			y = (mbr.min.y + (mbr.Height() - m_pDataSrc->SectBefore.Design.ZBar)) * (bReverse?-1:1);

			// MQC-6054
			if (SecUtil.IsCombinedAType(*pDataSrc))   // combined A type
			{
				CString SectionShape = pDataSrc->SectBefore.Shape;
				int nIndex = SecUtil.GetShapeIndexFromNameComA(SectionShape);
				if(nIndex == 1 || nIndex == 2) x = pDataSrc->SectBefore.Design.YBar;
			}
		}
		else if (m_pDataSrc->nStype == D_SECT_TYPE_COMPO_I ||
						 m_pDataSrc->nStype == D_SECT_TYPE_COMPO_B || 
						 m_pDataSrc->nStype == D_SECT_TYPE_COMPO_TUB ||
						 m_pDataSrc->nStype == D_SECT_TYPE_COMPO_STLG_B ||
						 m_pDataSrc->nStype == D_SECT_TYPE_COMPO_STLG_I || 
						 m_pDataSrc->nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
						 m_pDataSrc->nStype == D_SECT_TYPE_COMPO_CI || 
						 m_pDataSrc->nStype == D_SECT_TYPE_COMPO_CT || 
						 m_pDataSrc->nStype == D_SECT_TYPE_COMPO_PC ||
						 m_pDataSrc->nStype == D_SECT_TYPE_CONSTRUCTION)
		{
			x = mbr.min.x + m_pDataSrc->SectAfter.Design.YBar;
			y = (mbr.min.y + (mbr.Height() - m_pDataSrc->SectAfter.Design.ZBar)) * (bReverse?-1:1);
		}
		else if(m_pDataSrc->nStype == D_SECT_TYPE_COMPO_G)
		{
			x = mbr.min.x + m_pDataSrc->SectAfter.SectI.Design.YBar;
			y = (mbr.min.y + (mbr.Height() - m_pDataSrc->SectAfter.SectI.Design.ZBar)) * (bReverse?-1:1);
		}
		else
		{
			if(m_pDataSrc->nStype==D_SECT_TYPE_TAPERED &&
				 m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_I ||
				 m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_B || 
				 m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_TUB || 
				 m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B ||
				 m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I || 
				 m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
				 m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_CI || 
				 m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_CT || 
				 /*m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_G  || */
				 m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_CONSTRUCTION)
			{
				x = mbr.min.x + m_pDataSrc->SectAfter.SectI.Design.YBar;
				y = (mbr.min.y + (mbr.Height() - m_pDataSrc->SectAfter.SectI.Design.ZBar)) * (bReverse?-1:1);
			}
			else if(m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_G)
			{
				if (bCGI)
				{
					x = mbr.min.x + m_pDataSrc->SectAfter.SectJ.Design.YBar;
					y = (mbr.min.y + (mbr.Height() - m_pDataSrc->SectAfter.SectJ.Design.ZBar)) * (bReverse?-1:1);
				}
				else
				{
					x = mbr.min.x + m_pDataSrc->SectAfter.SectI.Design.YBar;
					y = (mbr.min.y + (mbr.Height() - m_pDataSrc->SectAfter.SectI.Design.ZBar)) * (bReverse?-1:1);
				}
			}
			else
			{
				if (bCGI)
				{
					x = mbr.min.x + m_pDataSrc->SectBefore.SectJ.Design.YBar;
					y = (mbr.min.y + (mbr.Height() - m_pDataSrc->SectBefore.SectJ.Design.ZBar)) * (bReverse?-1:1);
				}
				else
				{
					x = mbr.min.x + m_pDataSrc->SectBefore.SectI.Design.YBar;
					y = (mbr.min.y + (mbr.Height() - m_pDataSrc->SectBefore.SectI.Design.ZBar)) * (bReverse?-1:1);
				}
			}
		}

		SCMRect2D mbr_sub = aGeom.GetMBR();
		
		double size = (mbr_sub.Width() > mbr_sub.Height()) ? mbr_sub.Width()*0.03 : mbr_sub.Height()*0.03;
		if (m_pDataSrc->nStype == D_SECT_TYPE_PSC) size *= 0.8;
		_MakeRect(x-size, y-size, x+size, y+size,  pCentroid);
		aGeom.Add(pCentroid);

		// 응력출력위치 표시 - 응력위치는 도심에 대한 상대좌표
		centroidX = x; centroidY = y;
		size *= 0.8;

		T_SECT_STIFFNESS* pStiff;
		if (m_pDataSrc->nStype == D_SECT_TYPE_TAPERED)
		{
			if (m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_I ||
					m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_B || 
					m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_TUB || 
					m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B || 
					m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I || 
					m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB || 
					m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_CI || 
					m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_CT || 
					/*m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_G  || */
					m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_CONSTRUCTION)
			{
				pStiff = &(m_pDataSrc->SectAfter.SectI.Stiffness);
			}
			else if( m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_G)
			{
				if (bCGI)
					pStiff = &(m_pDataSrc->SectAfter.SectJ.Stiffness);
				else
					pStiff = &(m_pDataSrc->SectAfter.SectI.Stiffness);
			}
			else
			{
				if (bCGI)
					pStiff = &(m_pDataSrc->SectBefore.SectJ.Stiffness);
				else
					pStiff = &(m_pDataSrc->SectBefore.SectI.Stiffness);
			}
		}
		else if (m_pDataSrc->SectBefore.Shape == D_SECT_SHAPE_SRC_DUMB || m_pDataSrc->SectBefore.Shape == D_SECT_SHAPE_SRC_STUB)
		{
			pStiff = CSectUtil::GetProperStiffPosition(*m_pDataSrc, FALSE);  // 삽도용으로는 before
		}
		else 
			pStiff = CSectUtil::GetProperStiffPosition(*m_pDataSrc, TRUE);
		if (pStiff == NULL) { ASSERT(0); return; }
// 
// // MNET:XXXX-HSSHIM-20130801
// // Arup사 요청한 import시 value형 단면 속성 자동 재계산시 SectBefore.SectI에 대해 계산하는데,
// // CSectUtil::GetProperStiffPosition()에서 SectAfter.SectI를 반환해주고 있어서 
// // 응력 point 위치표시할 때는 SectBefore.SectI를 사용하도록 임시 처리.
// if (CRegistry::AppGetProfileString(_LSX(COMMON),_LSX(MctReCalcSect),_LSX(no), TRUE).CompareNoCase(_LSX(yes)) == 0)  
// {
//   if (m_pDataSrc->nStype == D_SECT_TYPE_COMPO_G)
//     pStiff = &(m_pDataSrc->SectBefore.SectI.Stiffness);
// }

		CString csText;
		double aX[4] = { pStiff->dx1, pStiff->dx2, pStiff->dx3, pStiff->dx4 };
		double aY[4] = { pStiff->dy1, pStiff->dy2, pStiff->dy3, pStiff->dy4 }; 
		for (int i=0; i<4; i++)
		{
			CCMEllipse* pStress = new CCMEllipse(CCM_WHITE, CCM_RED);
			x =  centroidX + aX[i]; 
			y = (centroidY - aY[i]) * (bReverse?-1:1);
			SCMPoint2D min, max;
			min.x = x-size; min.y = y-size;
			max.x = x+size; max.y = y+size;
			pStress->SetRect(min, max);
			aGeom.Add(pStress);
		}
	}

	if (bOffset)
	{
		CCMPolygon* pPolygon = new CCMPolygon(CCM_WHITE, CCM_RED);

		int nOffsetPoint = m_pDataSrc->SectBefore.nOffsetPoint;
		const int nHorzOption = m_pDataSrc->SectBefore.nHorzOffsetOpt;
		const int nVertOption = m_pDataSrc->SectBefore.nVertOffsetOpt;
		const int nOffsetCenter = m_pDataSrc->SectBefore.nOffsetCenter;
		const int nUserOffsetRef = m_pDataSrc->SectBefore.nUserOffsetRef;

		if (nOffsetPoint < 1 || nOffsetPoint > 9)
			nOffsetPoint = 5;

		double centX, centY;
		if (m_pDataSrc->SectBefore.nHorzOffsetOpt == 1) centX = centroidX;
		else
		{
			if (m_pDataSrc->SectBefore.nOffsetCenter == 0) centX = centroidX;
			else centX = (mbr.min.x + mbr.max.x) / 2.0;
		}
		if (m_pDataSrc->SectBefore.nVertOffsetOpt == 1) centY = centroidY;
		else
		{
			if (m_pDataSrc->SectBefore.nOffsetCenter == 0) centY = centroidY;
			else centY = (mbr.min.y + mbr.max.y) / 2.0;
		}

		/*
		if (m_pDataSrc->SectBefore.nOffsetCenter == 0)  // centroid
		{
			centX = centroidX;
			centY = centroidY;
		}
		else    // center of size
		{
			centX = (mbr.min.x+mbr.max.x)/2.0;
			centY = (mbr.min.y+mbr.max.y)/2.0;
		}
		*/

		double optionX = m_pDataSrc->SectBefore.dUserDefOffsetY[0];
		double optionY = m_pDataSrc->SectBefore.dUserDefOffsetZ[0] * (bReverse ? -1 : 1);

		switch (nOffsetPoint)
		{
		case 1: case 4: case 7:
			//x = (nHorzOption == 0) ? mbr.min.x : centX - optionX ; break;
			if (nHorzOption == 0) x = mbr.min.x;
			else
			{
				if (nUserOffsetRef == 0) x = centX - optionX;
				else x = mbr.min.x + optionX;
			}
			break;
		case 2: case 5: case 8:
			x = centX; break;
		case 3: case 6: case 9:
			//x = (nHorzOption == 0) ? mbr.max.x : centX + optionX; break;
			if (nHorzOption == 0) x = mbr.max.x;
			else
			{
				if (nUserOffsetRef == 0) x = centX + optionX;
				else x = mbr.max.x - optionX;
			}
			break;
		}

		switch (nOffsetPoint)
		{
		case 1: case 2: case 3:
			//y = (nVertOption == 0) ? mbr.min.y : centY - optionY; break;
			if (nVertOption == 0) y = mbr.min.y;
			else
			{
				if (nUserOffsetRef == 0) y = centY - optionY;
				else y = mbr.min.y + optionY;
			}
			break;
		case 4: case 5: case 6:
			y = centY; break;
		case 7: case 8: case 9:
			//y = (nVertOption == 0) ? mbr.max.y : centY + optionY; break;
			if (nVertOption == 0) y = mbr.max.y;
			else
			{
				if (nUserOffsetRef == 0) y = centY + optionY;
				else y = mbr.max.y - optionY;
			}
			break;
		}

		SCMRect2D mbr_sub = aGeom.GetMBR();
		if (m_pDataSrc->nStype == D_SECT_TYPE_TAPERED) mbr_sub = aGeom.GetMBR();
		double size = (mbr_sub.Width() > mbr_sub.Height()) ? mbr_sub.Width()*0.03 : mbr_sub.Height()*0.03;
		if (m_pDataSrc->nStype == D_SECT_TYPE_PSC) size *= 0.8;
		_MakeRect(x-size, y-size, x+size, y+size,  pPolygon);
		aGeom.Add(pPolygon);
	}
}

/////////////////////////////////////////////////////////////////////////////
// ICMPainter implementation functions
void ICMGeomMaker::_MakeLeftAngle(double H, double B1, double tw, double tf1, 
																	CCMPolygon* pPolygon, BOOL bangle)
{
	pPolygon->Add( SCMPoint2D(0.0, 0.0)                   );
	pPolygon->Add( SCMPoint2D(0.0, tf1 * (bangle?-1:1))   );
	pPolygon->Add( SCMPoint2D(B1-tw, tf1 * (bangle?-1:1)) );
	pPolygon->Add( SCMPoint2D(B1-tw, H * (bangle?-1:1))   );
	pPolygon->Add( SCMPoint2D(B1, H * (bangle?-1:1))      );
	pPolygon->Add( SCMPoint2D(B1, 0.0));
}

void ICMGeomMaker::_MakeRightAngle(double H, double B1, double tw, double tf1, 
																	 CCMPolygon* pPolygon, BOOL bangle)
{
	pPolygon->Add( SCMPoint2D(0.0, 0.0)                );
	pPolygon->Add( SCMPoint2D(0.0, H * (bangle?-1:1))  );
	pPolygon->Add( SCMPoint2D(tw, H * (bangle?-1:1))   );
	pPolygon->Add( SCMPoint2D(tw, tf1 * (bangle?-1:1)) );
	pPolygon->Add( SCMPoint2D(B1, tf1 * (bangle?-1:1)) );
	pPolygon->Add( SCMPoint2D(B1, 0.0)                 );
}

void ICMGeomMaker::_MakeLeftChannel(double H, double B1, double tw, double tf1,
																		double B2, double tf2, CCMPolygon* pPolygon)
{
	if (B2 == 0.0) B2 = B1;
	if (tf2 == 0.0) tf2 = tf1;

	pPolygon->Add(SCMPoint2D(0.0, 0.0));
	pPolygon->Add(SCMPoint2D(0.0, tf1));
	pPolygon->Add(SCMPoint2D(B1-tw, tf1));
	pPolygon->Add(SCMPoint2D(B1-tw, H-tf2));
	pPolygon->Add(SCMPoint2D(B1-B2, H-tf2));
	pPolygon->Add(SCMPoint2D(B1-B2, H));
	pPolygon->Add(SCMPoint2D(B1, H));
	pPolygon->Add(SCMPoint2D(B1, 0.0));
	SCMRect2D mbr = pPolygon->GetMBR();
	pPolygon->Transfer(SCMPoint2D(-mbr.min.x, -mbr.min.y));
}

void ICMGeomMaker::_MakeRightChannel(double H, double B1, double tw, double tf1,
																		 double B2, double tf2, CCMPolygon* pPolygon)
{
	if (B2 == 0.0) B2 = B1;
	if (tf2 == 0.0) tf2 = tf1;

	pPolygon->Add(SCMPoint2D(0.0, 0.0));
	pPolygon->Add(SCMPoint2D(B1, 0.0));	
	pPolygon->Add(SCMPoint2D(B1, tf1));
	pPolygon->Add(SCMPoint2D(tw, tf1));
	pPolygon->Add(SCMPoint2D(tw, fabs(H-tf2)));
	pPolygon->Add(SCMPoint2D(B2, fabs(H-tf2)));
	pPolygon->Add(SCMPoint2D(B2, H));
	pPolygon->Add(SCMPoint2D(0.0, H));
}

void ICMGeomMaker::_MakeHBeam(double H, double B1, double tw, double tf1, 
															double B2, double tf2, CCMPolygon* pPolygon)
{
	if (B2 == 0.0) B2 = B1;
	if (tf2 == 0.0) tf2 = tf1;

	pPolygon->Add(SCMPoint2D(0.0, 0.0));	
	pPolygon->Add(SCMPoint2D(0.0, tf1));
	pPolygon->Add(SCMPoint2D(fabs((B1-tw)/2.0), tf1));
	pPolygon->Add(SCMPoint2D(fabs((B1-tw)/2.0), fabs(H-tf2)));
	pPolygon->Add(SCMPoint2D((B1)/2.0-(B2)/2.0, fabs(H-tf2)));
	pPolygon->Add(SCMPoint2D((B1)/2.0-(B2)/2.0, H));
	pPolygon->Add(SCMPoint2D((B1)/2.0+(B2)/2.0, H));
	pPolygon->Add(SCMPoint2D((B1)/2.0+(B2)/2.0, fabs(H-tf2)));
	pPolygon->Add(SCMPoint2D(fabs((B1+tw)/2.0), fabs(H-tf2)));
	pPolygon->Add(SCMPoint2D(fabs((B1+tw)/2.0), tf1)); // MQC:2031-JHYUN-20080721
	//pPolygon->Add(SCMPoint2D((B1+tw)/2.0, tf1));
	pPolygon->Add(SCMPoint2D(B1, tf1));
	pPolygon->Add(SCMPoint2D(B1, 0.0));
	SCMRect2D mbr = pPolygon->GetMBR();
	pPolygon->Transfer(SCMPoint2D(-mbr.min.x, -mbr.min.y));
}

void ICMGeomMaker::_MakeIBeam(double H, double B1, double tw, double tf1, double B2, double tf2, CCMPolygon* pPolygon)
{
	_MakeHBeam(H, B1, tw, tf1, B2, tf2, pPolygon);

	pPolygon->Rotate(90);
}

void ICMGeomMaker::_MakeTee(double H, double B1, double tw, double tf1,
														CCMPolygon* pPolygon, BOOL bangle)
{
	pPolygon->Add( SCMPoint2D(0.0, 0.0)                         );	
	pPolygon->Add( SCMPoint2D(0.0, tf1 * (bangle?-1:1))         );	
	pPolygon->Add( SCMPoint2D((B1-tw)/2.0, tf1 * (bangle?-1:1)) );
	pPolygon->Add( SCMPoint2D((B1-tw)/2.0, H * (bangle?-1:1))   );	
	pPolygon->Add( SCMPoint2D((B1+tw)/2.0, H * (bangle?-1:1))   );
	pPolygon->Add( SCMPoint2D((B1+tw)/2.0, tf1 * (bangle?-1:1)) );	
	pPolygon->Add( SCMPoint2D(B1, tf1 * (bangle?-1:1))          );
	pPolygon->Add( SCMPoint2D(B1, 0.0)                          );
}

void ICMGeomMaker::_MakeUpsideDownTee(double H, double B1, double B2, double tw, double tf1,
														CCMPolygon* pPolygon, BOOL bangle)
{
	// pPolygon->Add( SCMPoint2D(0.0, 0.0)                         );	
	// pPolygon->Add( SCMPoint2D(0.0, tf1 * (bangle?-1:1))         );	
	// pPolygon->Add( SCMPoint2D((B1-tw)/2.0, tf1 * (bangle?-1:1)) );
	// pPolygon->Add( SCMPoint2D((B1-tw)/2.0, H * (bangle?-1:1))   );	
	// pPolygon->Add( SCMPoint2D((B1+tw)/2.0, H * (bangle?-1:1))   );
	// pPolygon->Add( SCMPoint2D((B1+tw)/2.0, tf1 * (bangle?-1:1)) );	
	// pPolygon->Add( SCMPoint2D(B1, tf1 * (bangle?-1:1))          );
	// pPolygon->Add( SCMPoint2D(B1, 0.0)                          );

	pPolygon->Add( SCMPoint2D(0.0, 0.0)                         );
	pPolygon->Add( SCMPoint2D(0.0, (H-tf1) * (bangle?-1:1))     );
	pPolygon->Add( SCMPoint2D(-B1, (H-tf1) * (bangle?-1:1))     );
	pPolygon->Add( SCMPoint2D(-B1, H * (bangle?-1:1))           );
	pPolygon->Add( SCMPoint2D(B2+tw, H * (bangle?-1:1))         );
	pPolygon->Add( SCMPoint2D(B2+tw, (H-tf1) * (bangle?-1:1))   );
	pPolygon->Add( SCMPoint2D(tw, (H-tf1) * (bangle?-1:1))      );
	pPolygon->Add( SCMPoint2D(tw, 0.0)                          );
}

void ICMGeomMaker::_MakeRect(double xmin, double ymin, double xmax, double ymax, CCMPolygon* pPolygon)
{
	pPolygon->Add(SCMPoint2D(xmin, ymin));	
	pPolygon->Add(SCMPoint2D(xmin, ymax));
	pPolygon->Add(SCMPoint2D(xmax, ymax));
	pPolygon->Add(SCMPoint2D(xmax, ymin));
}

void ICMGeomMaker::_MakeRect_RoundType(double H, double B, double tw, double r, CCMPolygon* pPolygon)
{
	auto _L_AddNode2Polygon = [pPolygon] (double x, double y)
	{
		pPolygon->Add(SCMPoint2D(x, y));
	};

	if ( LE0(H) ) H = 0.0;
	if ( LE0(B) ) B = 0.0;
	if ( LE0(tw) ) tw = 0.0;
	if ( LE0(r) ) r = 0.0;

	if ( MT(2*(r+tw), H) || MT(2*(r+tw), B) ) r = 0.0;

	SCMPoint2D mid[8];
	int nOut = 5;
	int nIn = 3;
	if ( r > 0.0 )
	{
		for ( auto i = 0; i < nOut; i++ )
		{
			mid[i].x = (r+tw)*cos(90.0/(nOut+1)*(i+1)*M_PI/180.0);
			mid[i].y = (r+tw)*sin(90.0/(nOut+1)*(i+1)*M_PI/180.0);
		}
		for ( auto i = 0; i < nIn; i++ )
		{
			mid[nOut+i].x = r*cos(90.0/(nIn+1)*(i+1)*M_PI/180.0);
			mid[nOut+i].y = r*sin(90.0/(nIn+1)*(i+1)*M_PI/180.0);
		}
	}

	if ( MT0(r) )
	{
		_L_AddNode2Polygon(tw+r, tw);
		for ( auto i = 0; i < nIn; i++ )
		{
			_L_AddNode2Polygon((tw+r)-mid[nOut+nIn-1-i].x, (tw+r)-mid[nOut+nIn-1-i].y);
		}
		_L_AddNode2Polygon(tw, tw+r);
	}
	else _L_AddNode2Polygon(tw, tw);

	if ( MT0(r) )
	{
		_L_AddNode2Polygon(tw, H-(tw+r));
		for ( auto i = 0; i < nIn; i++ )
		{
			_L_AddNode2Polygon((tw+r)-mid[nOut+i].x, H-(tw+r)+mid[nOut+i].y);
		}
		_L_AddNode2Polygon(tw+r, H-tw);
	}
	else _L_AddNode2Polygon(tw+r, H-tw);

	if ( MT0(r) )
	{
		_L_AddNode2Polygon(B-(tw+r), H-tw);
		for ( auto i = 0; i < nIn; i++ )
		{
			_L_AddNode2Polygon(B-(tw+r)+mid[nOut+nIn-1-i].x, H-(tw+r)+mid[nOut+nIn-1-i].y);
		}
		_L_AddNode2Polygon(B-tw, H-(tw+r));
	}
	else _L_AddNode2Polygon(B-tw, H-tw);

	if ( MT0(r) )
	{
		_L_AddNode2Polygon(B-tw, tw+r);
		for ( auto i = 0; i < nIn; i++ )
		{
			_L_AddNode2Polygon(B-(tw+r)+mid[nOut+i].x, (tw+r)-mid[nOut+i].y);
		}
		_L_AddNode2Polygon(B-(tw+r), tw);
	}
	else _L_AddNode2Polygon(B-(tw+r), tw);
}

void ICMGeomMaker::_MakeBox(double H, double B, double tw, double tf1,
														double C, double tf2, CCMPolygon* pPolygon)
{
	double tm1 = (B-C-tw)/2.0;
	pPolygon->Add(SCMPoint2D(0.0, 0.0));
	pPolygon->Add(SCMPoint2D(0.0, tf1));
	pPolygon->Add(SCMPoint2D(tm1, tf1));
	pPolygon->Add(SCMPoint2D(tm1, H-tf2));
	pPolygon->Add(SCMPoint2D(0.0, H-tf2));
	pPolygon->Add(SCMPoint2D(0.0, H));
	pPolygon->Add(SCMPoint2D(B, H));
	pPolygon->Add(SCMPoint2D(B, H-tf2));
	pPolygon->Add(SCMPoint2D(B-tm1, H-tf2));
	pPolygon->Add(SCMPoint2D(B-tm1, tf1));
	pPolygon->Add(SCMPoint2D(B, tf1));
	pPolygon->Add(SCMPoint2D(B, 0.0));
}

void ICMGeomMaker::_MakeBox_HSQ(double H, double B, double tw, double tf1,
														double C, double tf2, double B1_bot, double B2_bot, CCMPolygon* pPolygon)
{
	double tm1 = (B-C-tw)/2.0;
	pPolygon->Add(SCMPoint2D(B1_bot, 0.0));
	pPolygon->Add(SCMPoint2D(B1_bot, tf1));
	pPolygon->Add(SCMPoint2D(B1_bot+tm1, tf1));
	pPolygon->Add(SCMPoint2D(B1_bot+tm1, H-tf2));
	pPolygon->Add(SCMPoint2D(0.0, H-tf2));
	pPolygon->Add(SCMPoint2D(0.0, H));
	pPolygon->Add(SCMPoint2D(B1_bot+B2_bot+B, H));
	pPolygon->Add(SCMPoint2D(B1_bot+B2_bot+B, H-tf2));
	pPolygon->Add(SCMPoint2D(B1_bot+B-tm1, H-tf2));
	pPolygon->Add(SCMPoint2D(B1_bot+B-tm1, tf1));
	pPolygon->Add(SCMPoint2D(B1_bot+B, tf1));
	pPolygon->Add(SCMPoint2D(B1_bot+B, 0.0));
}

void ICMGeomMaker::_MakeBox_RoundType(double H, double B, double tw, double r, CCMPolygon* pPolygon)
{
	auto _L_AddNode2Polygon = [pPolygon] (double x, double y)
	{
		pPolygon->Add(SCMPoint2D(x, y));
	};

	if ( LE0(H) ) H = 0.0;
	if ( LE0(B) ) B = 0.0;
	if ( LE0(tw) ) tw = 0.0;
	if ( LE0(r) ) r = 0.0;

	if ( MT(2*(r+tw), H) || MT(2*(r+tw), B) ) r = 0.0;

	SCMPoint2D mid[8];
	int nOut = 5;
	int nIn = 3;
	if ( r > 0.0 )
	{
		for ( auto i = 0; i < nOut; i++ )
		{
			mid[i].x = (r+tw)*cos(90.0/(nOut+1)*(i+1)*M_PI/180.0);
			mid[i].y = (r+tw)*sin(90.0/(nOut+1)*(i+1)*M_PI/180.0);
		}
		for ( auto i = 0; i < nIn; i++ )
		{
			mid[nOut+i].x = r*cos(90.0/(nIn+1)*(i+1)*M_PI/180.0);
			mid[nOut+i].y = r*sin(90.0/(nIn+1)*(i+1)*M_PI/180.0);
		}
	}

	if ( MT0(r) )
	{
		_L_AddNode2Polygon(r+tw, 0.0);
		for ( auto i = 0; i < nOut; i++ )
		{
			_L_AddNode2Polygon((r+tw)-mid[nOut-1-i].x, (r+tw)-mid[nOut-1-i].y);
		}
		_L_AddNode2Polygon(0, r+tw);
	}
	else _L_AddNode2Polygon(0.0, 0.0);

	if ( MT0(r) )
	{
		_L_AddNode2Polygon(0.0, H-(r+tw));
		for ( auto i = 0; i < nOut; i++ )
		{
			_L_AddNode2Polygon((r+tw)-mid[i].x, H-(r+tw)+mid[i].y);
		}
		_L_AddNode2Polygon(r+tw, H);
	}
	else _L_AddNode2Polygon(0.0, H);

	if ( MT0(r) )
	{
		_L_AddNode2Polygon(B-(r+tw), H);
		for ( auto i = 0; i < nOut; i++ )
		{
			_L_AddNode2Polygon(B-(r+tw)+mid[nOut-1-i].x, H-(r+tw)+mid[nOut-1-i].y);
		}
		_L_AddNode2Polygon(B, H-(r+tw));
	}
	else _L_AddNode2Polygon(B, H);

	if ( MT0(r) )
	{
		_L_AddNode2Polygon(B, r+tw);
		for ( auto i = 0; i < nOut; i++ )
		{
			_L_AddNode2Polygon(B-(r+tw)+mid[i].x, (r+tw)-mid[i].y);
		}
		_L_AddNode2Polygon(B-(r+tw), 0.0);
	}
	else _L_AddNode2Polygon(B, 0.0);
}

//-----------------------------------------------------------------------
void ICMGeomMaker::GetRegularData(double &H, double &B1, double &tw, 
																	double &tf1, double &B2, double &tf2)
{
	H = m_pDataSrc->SectBefore.SectI.Size[0];
	B1 = m_pDataSrc->SectBefore.SectI.Size[1];
	tw = m_pDataSrc->SectBefore.SectI.Size[2];
	tf1 = m_pDataSrc->SectBefore.SectI.Size[3];
	B2 = m_pDataSrc->SectBefore.SectI.Size[4];
	tf2 = m_pDataSrc->SectBefore.SectI.Size[5];
}

void ICMGeomMaker::GetRegularDataJ(double &H, double &B1, double &tw, 
																	 double &tf1, double &B2, double &tf2)
{
	H = m_pDataSrc->SectBefore.SectJ.Size[0];
	B1 = m_pDataSrc->SectBefore.SectJ.Size[1];
	tw = m_pDataSrc->SectBefore.SectJ.Size[2];
	tf1 = m_pDataSrc->SectBefore.SectJ.Size[3];
	B2 = m_pDataSrc->SectBefore.SectJ.Size[4];
	tf2 = m_pDataSrc->SectBefore.SectJ.Size[5];
}

void ICMGeomMaker::GetRegularDataS(double &H, double &B1, double &tw, 
																	 double &tf1, double &B2, double &tf2)
{
	H = m_pDataSrc->SectBefore.SectJ.Size[0];
	B1 = m_pDataSrc->SectBefore.SectJ.Size[1];
	tw = m_pDataSrc->SectBefore.SectJ.Size[2];
	tf1 = m_pDataSrc->SectBefore.SectJ.Size[3];
	B2 = m_pDataSrc->SectBefore.SectJ.Size[4];
	tf2 = m_pDataSrc->SectBefore.SectJ.Size[5];
}

void ICMGeomMaker::MakePictureAngle(CCMGeom2DArray &aGeom, BOOL bRev)
{
	double H, B1, tw, tf1, B2, tf2;
	GetRegularData(H, B1, tw, tf1, B2, tf2);

	CCMPolygon *pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeRightAngle(H, B1, tw, tf1, pPolygon, bRev);
// 	if (m_pDataSrc->bPricipalAxis) pPolygon->Rotate(tf2);
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureChannel(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	GetRegularData(H, B1, tw, tf1, B2, tf2);

	CCMPolygon *pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeRightChannel(H, B1, tw, tf1, B2, tf2, pPolygon);
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureHBeam(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	GetRegularData(H, B1, tw, tf1, B2, tf2);

	CCMPolygon *pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeHBeam(H, B1, tw, tf1, B2, tf2, pPolygon);
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureTee(CCMGeom2DArray &aGeom, BOOL bRev)
{
	double H, B1, tw, tf1, B2, tf2;
	GetRegularData(H, B1, tw, tf1, B2, tf2);

	CCMPolygon *pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeTee(H, B1, tw, tf1, pPolygon, bRev);
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureUpsideDownTee(CCMGeom2DArray &aGeom, BOOL bRev)
{
	double H, B1, tw, tf1, B2, tf2;
	GetRegularData(H, B1, B2, tw, tf1, tf2);

	CCMPolygon *pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeUpsideDownTee(H, B1, B2, tw, tf1, pPolygon, bRev);
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureBox(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	GetRegularData(H, B1, tw, tf1, B2, tf2);
	if (B2 == 0.0) B2 = B1 - tw;
	if (tf2 == 0.0) tf2 = tf1;

	// HSQ 인 경우 하부 길이 
	double B1_bot = 0.0;
	double B2_bot = 0.0;
	if (m_pDataSrc->bHSQType)
	{
		B1_bot = m_pDataSrc->SectBefore.SectI.Size[7];
		B2_bot = m_pDataSrc->SectBefore.SectI.Size[8];
	}

	// Round Type 고려
	double r = 0.0;
	if ( m_pDataSrc->bRoundType )
	{
		r = m_pDataSrc->SectBefore.SectI.Size[10];
		tf1 = tw;
		tf2 = tw;
	}

	// outer box
	CCMPolygon *pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	//_MakeRect(0.0, 0.0, B1, H, pPolygon);
	if ( m_pDataSrc->bRoundType )		_MakeBox_RoundType(H, B1, tw, r, pPolygon);
	else if ( m_pDataSrc->bHSQType )	_MakeBox_HSQ(H, B1, tw, tf1, B2, tf2, B1_bot, B2_bot, pPolygon);
	else								_MakeBox(H, B1, tw, tf1, B2, tf2, pPolygon);
	aGeom.Add(pPolygon);

	// rect hole
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_HOLE);
	if ( m_pDataSrc->bRoundType )
	{
		_MakeRect_RoundType(H, B1, tw, r, pPolygon);
	}
	else
	{
		double tm1 = (B1-B2-tw)/2.0+tw;
		_MakeRect(B1_bot+tm1, tf1, B1_bot+B1-tm1, H-tf2, pPolygon);
	}

	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePicturePipe(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	GetRegularData(H, B1, tw, tf1, B2, tf2);

	// outer circle
	CCMEllipse *pEllipse = new CCMEllipse(CCM_BORDER, CCM_FILL);
	pEllipse->SetRect(SCMPoint2D(0.0, 0.0), SCMPoint2D(H, H));
	aGeom.Add(pEllipse);

	// inner circle
	pEllipse = new CCMEllipse(CCM_BORDER, CCM_HOLE);
	pEllipse->SetRect(SCMPoint2D(B1, B1), SCMPoint2D(H-B1, H-B1));
	aGeom.Add(pEllipse);
}

void ICMGeomMaker::MakePictureDblAngle(CCMGeom2DArray &aGeom, BOOL bRev)
{
	double H, B1, tw, tf1, B2, tf2;
	GetRegularData(H, B1, tw, tf1, B2, tf2);

	double ref=B1+B2;

	// left angle
	CCMPolygon *pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeLeftAngle(H, B1, tw, tf1, pPolygon, bRev);
	aGeom.Add(pPolygon);

	// right angle
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeRightAngle(H, B1, tw, tf1, pPolygon, bRev);
	pPolygon->Transfer(SCMPoint2D(ref, 0.0));
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureDblChannel(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	GetRegularData(H, B1, tw, tf1, B2, tf2);

	double ref=B1+B2;

	// left channel : keep in mind that B1 = B2, tf1 = tf2
	CCMPolygon *pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeLeftChannel(H, B1, tw, tf1, B1, tf1, pPolygon);
	aGeom.Add(pPolygon);

	// right channel : keep in mind that B1 = B2, tf1 = tf2
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeRightChannel(H, B1, tw, tf1, B1, tf1, pPolygon);
	pPolygon->Transfer(SCMPoint2D(ref, 0.0));
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureDblChannelBox(CCMGeom2DArray& aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	GetRegularData(H, B1, tw, tf1, B2, tf2);

	double ref = B1 + B2;

	// right channel : keep in mind that B1 = B2, tf1 = tf2
	CCMPolygon* pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeRightChannel(H, B1, tw, tf1, B1, tf1, pPolygon);
	aGeom.Add(pPolygon);

	// left channel : keep in mind that B1 = B2, tf1 = tf2
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeLeftChannel(H, B1, tw, tf1, B1, tf1, pPolygon);
	pPolygon->Transfer(SCMPoint2D(ref, 0.0));
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureSolidRect(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	GetRegularData(H, B1, tw, tf1, B2, tf2);

	CCMPolygon *pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeRect(0.0, 0.0, B1, H, pPolygon);
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureSolidCircle(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	GetRegularData(H, B1, tw, tf1, B2, tf2);

	CCMEllipse *pEllipse = new CCMEllipse(CCM_BORDER, CCM_FILL);
	pEllipse->SetRect(SCMPoint2D(0.0, 0.0), SCMPoint2D(H, H));
	aGeom.Add(pEllipse);
}

void ICMGeomMaker::MakePictureRegColdFormedChannel(CCMGeom2DArray &aGeom)
{
	double H, B, tw, r, d, tf2;
	GetRegularData(H, B, tw, r, d, tf2);

	CCMPolygon *pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);

	_MakePictureColdFormedChannel(H, B, tw, r, d, pPolygon);
	aGeom.Add(pPolygon);

	// cc관련
	CString csCCShape = m_pDataSrc->SectBefore.SectI.Shape;
	if (csCCShape == _T("")) return;

	int nCCNum = _tstof(csCCShape);
	csCCShape = csCCShape.Mid(1);
	if (nCCNum <= 1) return;

	switch(nCCNum)
	{
	case 2:
		pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
		_MakePictureColdFormedChannel(H, B, tw, r, d, pPolygon);
		pPolygon->Mirror(1);
		if (csCCShape == _T("BS") || csCCShape == _T("BW"))
			pPolygon->Transfer(SCMPoint2D(2.0*B, 0.0));
		aGeom.Add(pPolygon);
		break;
	case 3:
		pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
		_MakePictureColdFormedChannel(H, B, tw, r, d, pPolygon);
		pPolygon->Transfer(SCMPoint2D(B, 0.0));
		aGeom.Add(pPolygon);
		pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
		_MakePictureColdFormedChannel(H, B, tw, r, d, pPolygon);
		pPolygon->Mirror(1);
		pPolygon->Transfer(SCMPoint2D(3.0*B, 0.0));
		aGeom.Add(pPolygon);
		break;
	case 4:
		pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
		_MakePictureColdFormedChannel(H, B, tw, r, d, pPolygon);
		pPolygon->Transfer(SCMPoint2D(B, 0.0));
		aGeom.Add(pPolygon);
		pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
		_MakePictureColdFormedChannel(H, B, tw, r, d, pPolygon);
		pPolygon->Transfer(SCMPoint2D(2.0*B, 0.0));
		aGeom.Add(pPolygon);
		pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
		_MakePictureColdFormedChannel(H, B, tw, r, d, pPolygon);
		pPolygon->Mirror(1);
		pPolygon->Transfer(SCMPoint2D(4.0*B, 0.0));
		aGeom.Add(pPolygon);
		break;
	}
}

void ICMGeomMaker::MakePictureRegColdFormedBox(CCMGeom2DArray &aGeom)
{
	double H, B, tw, r, d, tf2;
	GetRegularData(H, B, tw, r, d, tf2);
	B = B/2.0;  // MNET:XXXX-JWKWON-20110108 : 그림을 그리기 위해서는 B의 길이가 절반이 되어야 함 

	CCMPolygon *pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	
	_MakePictureColdFormedChannel(H, B, tw, r, d, pPolygon, TRUE);
	aGeom.Add(pPolygon);
	
	// cc관련
	CString csCCShape = m_pDataSrc->SectBefore.SectI.Shape;
	
	if (csCCShape == _T("")) { csCCShape = _T("2BS"); }
	
	int nCCNum = _tstof(csCCShape);
	csCCShape = csCCShape.Mid(1);
	if (nCCNum <= 1) return;
	
	switch(nCCNum)
	{
	case 2:
		pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
		_MakePictureColdFormedChannel(H, B, tw, r, d, pPolygon, TRUE);
		pPolygon->Mirror(1);
		if (csCCShape == _T("BS") || csCCShape == _T("BW"))
			pPolygon->Transfer(SCMPoint2D(2.0*B, 0.0));
		aGeom.Add(pPolygon);
		break;
	case 3:
		ASSERT(0);
		break;
	case 4:
		ASSERT(0);
		break;
	}
}

void ICMGeomMaker::MakePictureUpright(CCMGeom2DArray &aGeom)
{
	double H, B, tw, Hw1, Hw2, B1, B2, B3, Bf3, d;
	H   = m_pDataSrc->SectBefore.SectI.Size[0];
	B   = m_pDataSrc->SectBefore.SectI.Size[1];
	tw  = m_pDataSrc->SectBefore.SectI.Size[2];
	Hw1 = m_pDataSrc->SectBefore.SectI.Size[3];
	Hw2 = m_pDataSrc->SectBefore.SectI.Size[4];
	B1  = m_pDataSrc->SectBefore.SectI.Size[5];
	B2  = m_pDataSrc->SectBefore.SectI.Size[6];
	B3  = m_pDataSrc->SectBefore.SectI.Size[7];
	Bf3 = m_pDataSrc->SectBefore.SectI.Size[8];
	d   = m_pDataSrc->SectBefore.SectI.Size[9];

	CCMPolygon *pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureUpright(H, B, tw, Hw1, Hw2, B1, B2, B3, Bf3, d, pPolygon);
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureZ(CCMGeom2DArray &aGeom)
{
	double H, B, tw, r, d, th;
	H   = m_pDataSrc->SectBefore.SectI.Size[0];
	B   = m_pDataSrc->SectBefore.SectI.Size[1];
	tw  = m_pDataSrc->SectBefore.SectI.Size[2];
	r   = m_pDataSrc->SectBefore.SectI.Size[3];
	d   = m_pDataSrc->SectBefore.SectI.Size[4];
	th  = m_pDataSrc->SectBefore.SectI.Size[5];

	CCMPolygon *pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureZ(H, B, tw, r, d, th, pPolygon);
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureRegColdFormedChannelHat(CCMGeom2DArray &aGeom)
{
	double H, B, tw, r, d, tf2;
	GetRegularData(H, B, tw, r, d, tf2);

	CCMPolygon *pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);

	_MakePictureColdFormedChannelHat(H, B, tw, r, d, pPolygon);
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureRegURib(CCMGeom2DArray &aGeom)
{
	double H, B1, B2, t, r, tf2;
	GetRegularData(H, B1, B2, t, r, tf2);

	CCMPolygon *pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureURib(H, B1, B2, t, r, pPolygon);
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureOctagon(CCMGeom2DArray &aGeom)
{
	double H, B, a, b, t, t1;
	GetRegularData(H, B, a, b, t, t1);

	CCMPolygon *pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureOctagon(H, B, a, b, pPolygon);
	aGeom.Add(pPolygon);

	double a1, b1;
	if (a == 0.0 || b == 0.0) { a1 = 0.0; b1 = 0.0; }
	else
	{
		double c = b + (t/a)*sqrt(a*a+b*b);
		a1 = (a/b)*(c-t)-t;
		b1 = -(b/a)*t+c-t;
	}
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_HOLE);
	_MakePictureOctagon(H-2*t, B-2*t, a1, b1, pPolygon);
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureSolidOctagon(CCMGeom2DArray &aGeom)
{
	double H, B, a, b, t1, t2;
	GetRegularData(H, B, a, b, t1, t2);

	CCMPolygon *pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureOctagon(H, B, a, b, pPolygon);
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureTrack(CCMGeom2DArray &aGeom)
{
	double H, B, t, t1, t2, t3;
	GetRegularData(H, B, t, t1, t2, t3);

	CCMPolygon *pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureTrack(H, B-H, pPolygon);
	aGeom.Add(pPolygon);

	pPolygon = new CCMPolygon(CCM_BORDER, CCM_HOLE);
	_MakePictureTrack(H-2*t, B-H, pPolygon);
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureSolidTrack(CCMGeom2DArray &aGeom)
{
	double H, B, t0, t1, t2, t3;
	GetRegularData(H, B, t0, t1, t2, t3);

	CCMPolygon *pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureTrack(H, B-H, pPolygon);
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureHalfTrack(CCMGeom2DArray &aGeom)
{
	double H, B, t0, t1, t2, t3;
	GetRegularData(H, B, t0, t1, t2, t3);

	CCMPolygon *pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureHalfTrack(H, B-H/2.0, pPolygon);
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureCrossAngle(CCMGeom2DArray &aGeom)
{
	// JWKWON - MakePicture4L 함수를 참조하여 만듬
	double H, B1, tw, tf1, B2, tf2;
	GetRegularData(H, B1, tw, tf1, B2, tf2);

	B2 /= 2.;
	CCMPolygon *pPolygonS = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeLeftAngle(H, B1, tw, tf1, pPolygonS);

	CCMPolygon *pPolygonF2 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeRightAngle(H, B1, tw, tf1, pPolygonF2);


	SCMRect2D mbr;
	pPolygonF2->Mirror(0);  // x axis mirror

	mbr = pPolygonF2->GetMBR();
	pPolygonF2->Transfer(SCMPoint2D(-mbr.min.x, -mbr.min.y));

	pPolygonS->Transfer(SCMPoint2D(0.0, H+B2*2));
	pPolygonF2->Transfer(SCMPoint2D(B1+B2*2, 0.0));


	aGeom.Add(pPolygonS);
	aGeom.Add(pPolygonF2);
}

//-----------------------------------------------------------------------
void ICMGeomMaker::MakePictureColdFormedChannel(CCMGeom2DArray &aGeom)
{
	double H, B, tw, r, d, tf2;
	GetRegularData(H, B, tw, r, d, tf2);

	CCMPolygon *pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);

	_MakePictureColdFormedChannel(H, B, tw, r, d, pPolygon);
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureColdFormedChannel2I(CCMGeom2DArray &aGeom)
{
	double H, B, tw, r, d, tf2;
	GetRegularData(H, B, tw, r, d, tf2);

	// build right opened channel
	CCMPolygon* pPolygonR;
	pPolygonR = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureColdFormedChannel(H, B, tw, r, d, pPolygonR);

	// build left open channel
	CCMPolygon* pPolygonL;
	pPolygonL = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureColdFormedChannel(H, B, tw, r, d, pPolygonL);
	pPolygonL->Mirror(1); // y axis mirror

	aGeom.Add(pPolygonR);
	aGeom.Add(pPolygonL);

	SCMRect2D mbr = aGeom.GetMBR();
	double gap = (mbr.Width() > mbr.Height()) ? mbr.Width()*0.05 : mbr.Height()*0.05;

	pPolygonR->Transfer(SCMPoint2D(gap, 0.0));
	pPolygonL->Transfer(SCMPoint2D(-gap, 0.0));
}

void ICMGeomMaker::MakePictureColdFormedChannel2B(CCMGeom2DArray &aGeom)
{
	double H, B, tw, r, d, tf2;
	GetRegularData(H, B, tw, r, d, tf2);

	// build right opened channel
	CCMPolygon* pPolygonR;
	pPolygonR = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureColdFormedChannel(H, B, tw, r, d, pPolygonR);

	// build left open channel
	CCMPolygon* pPolygonL;
	pPolygonL = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureColdFormedChannel(H, B, tw, r, d, pPolygonL);
	pPolygonL->Mirror(1); // y axis mirror

	aGeom.Add(pPolygonR);
	aGeom.Add(pPolygonL);

	SCMRect2D mbrC = pPolygonL->GetMBR();

	SCMRect2D mbr = aGeom.GetMBR();
	double gap = (mbr.Width() > mbr.Height()) ? mbr.Width()*0.05 : mbr.Height()*0.05;

	pPolygonR->Transfer(SCMPoint2D(-mbrC.Width()-gap, 0.0));
	pPolygonL->Transfer(SCMPoint2D(mbrC.Width()+gap, 0.0));
}

void ICMGeomMaker::MakePictureColdFormedChannel3(CCMGeom2DArray &aGeom)
{
	double H, B, tw, r, d, tf2;
	GetRegularData(H, B, tw, r, d, tf2);
	B = B*3;

	CCMPolygon* pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeBox(H, B, tw, tw, B-tw, tw, pPolygon);
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureColdFormedChannel4(CCMGeom2DArray &aGeom)
{
	double H, B, tw, r, d, tf2;
	GetRegularData(H, B, tw, r, d, tf2);
	B = B*4;

	CCMPolygon* pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeBox(H, B, tw, tw, B-tw, tw, pPolygon);
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::_MakePictureColdFormedChannel(
		 double H, double B, double tw, double r, double d, CCMPolygon* pPolygon, BOOL bBox)
{
	if (H <= 0.0) H = 0.0;
	if (B <= 0.0) B = 0.0;
	if (tw <= 0.0) tw = 0.0;
	if (r <= 0.0) r = 0.0;
	if (d <= 0.0) d = 0.0;

	if(!bBox) // MNET:4258-JWKWON-20110110 : 2차 설계 냉간성형강 일때는 검사 하지 않음
	{
		if ((r+tw)*2 > B || (r+tw)*2 > H) return;
	}

	SCMPoint2D mid[8];
	int nOut = 5;
	int nIn = 3;
	// 휘어지는 부분 중간 값을 계산해 놓는다.
	int i;
	if (r > 0.0) 
	{
		for (i = 0; i < nOut; i++)
		{
			mid[i].x = (r+tw)*cos(90.0/(nOut+1)*(i+1)*M_PI/180.0);
			mid[i].y = (r+tw)*sin(90.0/(nOut+1)*(i+1)*M_PI/180.0);
		}
		for (i = 0; i < nIn; i++)
		{
			mid[nOut+i].x = r*cos(90.0/(nIn+1)*(i+1)*M_PI/180.0);
			mid[nOut+i].y = r*sin(90.0/(nIn+1)*(i+1)*M_PI/180.0);
		}
	}

	if (r > 0.0)
	{
		pPolygon->Add(SCMPoint2D(0.0, H-(r+tw)));
		for (i = 0; i < nOut; i++)
			pPolygon->Add(SCMPoint2D(r+tw-mid[i].x, H-(r+tw)+mid[i].y));
		pPolygon->Add(SCMPoint2D(r+tw, H));
	}
	else pPolygon->Add(SCMPoint2D(0.0, H));

	if (r > 0.0 && d > r+tw)
	{
		pPolygon->Add(SCMPoint2D(B-(r+tw), H));
		for (i = 0; i < nOut; i++)
			pPolygon->Add(SCMPoint2D(B-(r+tw)+mid[nOut-1-i].x, H-((r+tw)-mid[nOut-1-i].y)));
		pPolygon->Add(SCMPoint2D(B, H-(r+tw)));
	}
	else pPolygon->Add(SCMPoint2D(B, H));
	if (d > r+tw)
	{
		pPolygon->Add(SCMPoint2D(B, H-d));
		pPolygon->Add(SCMPoint2D(B-tw, H-d));
	}
	else pPolygon->Add(SCMPoint2D(B, H-tw));

	if (r > 0.0 && d > r+tw)
	{
		pPolygon->Add(SCMPoint2D(B-tw, H-(r+tw)));
		for (i = 0; i < nIn; i++)
			pPolygon->Add(SCMPoint2D(B-tw-(r-mid[nOut+i].x), H-tw-(r-mid[nOut+i].y)));
		pPolygon->Add(SCMPoint2D(B-(r+tw), H-tw));
	}
	else pPolygon->Add(SCMPoint2D(B-tw, H-tw));

	if (r > 0.0)
	{
		pPolygon->Add(SCMPoint2D(r+tw, H-tw));
		for (i = 0; i < nIn; i++)
			pPolygon->Add(SCMPoint2D(r+tw-mid[nOut+nIn-1-i].x, H-tw-(r-mid[nOut+nIn-1-i].y)));
		pPolygon->Add(SCMPoint2D(tw, H-tw-r));
	}
	else pPolygon->Add(SCMPoint2D(tw, H-tw));

	if (r > 0.0)
	{
		pPolygon->Add(SCMPoint2D(tw, r+tw));
		for (i = 0; i < nIn; i++)
			pPolygon->Add(SCMPoint2D(r+tw-mid[nOut+i].x, tw+(r-mid[nOut+i].y)));
		pPolygon->Add(SCMPoint2D(r+tw, tw));
	}
	else pPolygon->Add(SCMPoint2D(tw, tw));

	if (r > 0.0 && d > r+tw)
	{
		pPolygon->Add(SCMPoint2D(B-(r+tw), tw));
		for (i = 0; i < nIn; i++)
			pPolygon->Add(SCMPoint2D(B-(r+tw)+mid[nOut+nIn-1-i].x, r+tw-mid[nOut+nIn-1-i].y));
		pPolygon->Add(SCMPoint2D(B-tw, r+tw));
	}
	else pPolygon->Add(SCMPoint2D(B-tw, tw));
	if (d > r+tw)
	{
		pPolygon->Add(SCMPoint2D(B-tw, d));
		pPolygon->Add(SCMPoint2D(B, d));
	}
	else pPolygon->Add(SCMPoint2D(B, tw));

	if (r > 0.0 && d > r+tw)
	{
		pPolygon->Add(SCMPoint2D(B, r+tw));
		for (i = 0; i < nOut; i++)
			pPolygon->Add(SCMPoint2D(B-(r+tw-mid[i].x), r+tw-mid[i].y));
		pPolygon->Add(SCMPoint2D(B-(r+tw), 0.0));
	}
	else pPolygon->Add(SCMPoint2D(B, 0.0));

	if (r > 0.0)
	{
		pPolygon->Add(SCMPoint2D(r+tw, 0.0));
		for (i = 0; i < nOut; i++)
			pPolygon->Add(SCMPoint2D(r+tw-mid[nOut-1-i].x, r+tw-mid[nOut-1-i].y));
		pPolygon->Add(SCMPoint2D(0.0, r+tw));
	}
	else pPolygon->Add(SCMPoint2D(0.0, 0.0));
}

void ICMGeomMaker::_MakePictureColdFormedChannelHat(double H, double B, double tw, double r, double d, CCMPolygon* pPolygon, BOOL bBox/*=FALSE*/)
{
	if ( LE0(H) ) H = 0.0;
	if ( LE0(B) ) B = 0.0;
	if ( LE0(tw) ) tw = 0.0;
	if ( LE0(r) ) r = 0.0;
	if ( LE0(d) ) d = 0.0;

	if ( !bBox ) // MNET:4258-JWKWON-20110110 : 2차 설계 냉간성형강 일때는 검사 하지 않음
	{
		if ( MT(2*(r+tw), B) || MT(2*(r+tw), H) ) return;
	}

	SCMPoint2D mid[8];
	int nOut = 5;
	int nIn = 3;
	// 휘어지는 부분 중간 값을 계산해 놓는다.
	if ( r > 0.0 )
	{
		for ( auto i = 0; i < nOut; i++ )
		{
			mid[i].x = (r+tw)*cos(90.0/(nOut+1)*(i+1)*M_PI/180.0);
			mid[i].y = (r+tw)*sin(90.0/(nOut+1)*(i+1)*M_PI/180.0);
		}
		for ( auto i = 0; i < nIn; i++ )
		{
			mid[nOut+i].x = r*cos(90.0/(nIn+1)*(i+1)*M_PI/180.0);
			mid[nOut+i].y = r*sin(90.0/(nIn+1)*(i+1)*M_PI/180.0);
		}
	}

	if ( MT0(r) )
	{
		pPolygon->Add(SCMPoint2D(0.0, H-(r+tw)));
		for ( auto i = 0; i < nOut; i++ )
		{
			pPolygon->Add(SCMPoint2D((r+tw)-mid[i].x, H-(r+tw)+mid[i].y));
		}
		pPolygon->Add(SCMPoint2D(r+tw, H));
	}
	else pPolygon->Add(SCMPoint2D(0.0, H));

	if ( MT0(r) && MT(d, r+tw) )
	{
		pPolygon->Add(SCMPoint2D(B-tw-(r), H));
		for ( auto i = 0; i < nIn; i++ )
		{
			pPolygon->Add(SCMPoint2D(B-tw-(r)+mid[nOut+nIn-1-i].x, H+(r-mid[nOut+nIn-1-i].y)));
		}
		pPolygon->Add(SCMPoint2D(B-tw, H+r));
	}
	else pPolygon->Add(SCMPoint2D(B-tw, H));

	if ( MT(d, r+tw) )
	{
		pPolygon->Add(SCMPoint2D(B-tw, H+d-tw));
		pPolygon->Add(SCMPoint2D(B, H+d-tw));
	}
	else pPolygon->Add(SCMPoint2D(B, H));

	if ( MT0(r) && MT(d, r+tw) )
	{
		pPolygon->Add(SCMPoint2D(B, H-tw+(r+tw)));
		for ( auto i = 0; i < nOut; i++ )
		{
			pPolygon->Add(SCMPoint2D(B-(r+tw)+mid[i].x, H-tw+(r+tw)-mid[i].y));
		}
		pPolygon->Add(SCMPoint2D(B-(r+tw), H-tw));
	}
	else pPolygon->Add(SCMPoint2D(B, H-tw));

	if ( MT0(r) )
	{
		pPolygon->Add(SCMPoint2D(tw+(r), H-tw));
		for ( auto i = 0; i < nIn; i++ )
		{
			pPolygon->Add(SCMPoint2D(tw+r-mid[nOut+nIn-1-i].x, H-tw-r+mid[nOut+nIn-1-i].y));
		}
		pPolygon->Add(SCMPoint2D(tw, H-tw-(r)));
	}
	else pPolygon->Add(SCMPoint2D(tw, H-tw));

	if ( MT0(r) )
	{
		pPolygon->Add(SCMPoint2D(tw, tw+(r)));
		for ( auto i = 0; i < nIn; i++ )
		{
			pPolygon->Add(SCMPoint2D(tw+r-mid[nOut+i].x, tw+(r)-mid[nOut+i].y));
		}
		pPolygon->Add(SCMPoint2D(tw+(r), tw));
	}
	else pPolygon->Add(SCMPoint2D(tw, tw));

	if ( MT0(r) && MT(d, r+tw) )
	{
		pPolygon->Add(SCMPoint2D(B-(r+tw), tw));
		for ( auto i = 0; i < nOut; i++ )
		{
			pPolygon->Add(SCMPoint2D(B-(r+tw)+mid[nOut-1-i].x, tw-(r+tw)+mid[nOut-1-i].y));
		}
		pPolygon->Add(SCMPoint2D(B, tw-(r+tw)));
	}
	else pPolygon->Add(SCMPoint2D(B, tw));

	if ( MT(d, r+tw) )
	{
		pPolygon->Add(SCMPoint2D(B, tw-d));
		pPolygon->Add(SCMPoint2D(B-tw, tw-d));
	}
	else pPolygon->Add(SCMPoint2D(B, 0.0));

	if ( MT0(r) && MT(d, r+tw) )
	{
		pPolygon->Add(SCMPoint2D(B-tw, -r));
		for ( auto i = 0; i < nIn; i++ )
		{
			pPolygon->Add(SCMPoint2D(B-tw-r+mid[nOut+i].x, -(r)+mid[nOut+i].y));
		}
		pPolygon->Add(SCMPoint2D(B-tw-(r), 0.0));
	}
	else pPolygon->Add(SCMPoint2D(B-tw, 0.0));

	if ( MT0(r) )
	{
		pPolygon->Add(SCMPoint2D(r+tw, 0.0));
		for ( auto i = 0; i < nOut; i++ )
		{
			pPolygon->Add(SCMPoint2D(r+tw-mid[nOut-1-i].x, r+tw-mid[nOut-1-i].y));
		}
		pPolygon->Add(SCMPoint2D(0.0, r+tw));
	}
	else pPolygon->Add(SCMPoint2D(0.0, 0.0));

	return;
}

void ICMGeomMaker::_MakePictureUpright(double H, double B, double tw, double Hw1, double Hw2, double B1, double B2, double B3, double Bf3, double d, CCMPolygon* pPolygon)
{
	//	double dPoint[20][2];
	double tw_f3 = 0.;
	if(Bf3>0.0)
	{
		tw_f3 = tw;
	}

	/* 0*/pPolygon->Add(SCMPoint2D(0.0, 0.0));
	/* 1*/pPolygon->Add(SCMPoint2D(0.0, Hw1));
	/* 2*/pPolygon->Add(SCMPoint2D((B-B3)*0.5-tw, H-tw_f3-Hw2));
	/* 3*/pPolygon->Add(SCMPoint2D((B-B3)*0.5-tw, H-tw_f3));
	if(Bf3>0.0)
	{
		/* n=3 */
		/* 4*/pPolygon->Add(SCMPoint2D((B-B3)*0.5-Bf3,	H-tw));
		/* 5*/pPolygon->Add(SCMPoint2D((B-B3)*0.5-Bf3,	H));
		/* 6*/pPolygon->Add(SCMPoint2D((B-B3)*0.5,			H));
	}
	else
	{
		/* n=1 */
		/* 4*/pPolygon->Add(SCMPoint2D((B-B3)*0.5, H));
	}
	/* 4+n*/pPolygon->Add(SCMPoint2D((B-B3)*0.5, H-tw_f3-Hw2-tw));
	/* 5+n*/pPolygon->Add(SCMPoint2D(tw, Hw1-tw));
	/* 6+n*/pPolygon->Add(SCMPoint2D(tw, tw));

	if(d>0.0)
	{
		/* m=4 */
		/* 7+n*/pPolygon->Add(SCMPoint2D(B1-tw, tw));
		/* 8+n*/pPolygon->Add(SCMPoint2D(0.5*B-0.5*B2-tw, d+tw));
		/* 9+n*/pPolygon->Add(SCMPoint2D(0.5*B+0.5*B2+tw, d+tw));
		/*10+n*/pPolygon->Add(SCMPoint2D(B-B1+tw, tw));
	}
	/* 7+n+m */pPolygon->Add(SCMPoint2D(B-tw, tw));
	/* 8+n+m */pPolygon->Add(SCMPoint2D(B-tw, Hw1-tw));
	/* 9+n+m */pPolygon->Add(SCMPoint2D((B+B3)*0.5, H-tw_f3-Hw2-tw));

	if(Bf3>0.0)
	{
		/* l=3 */
		/* 10+n+m */pPolygon->Add(SCMPoint2D((B+B3)*0.5, H));
		/* 11+n+m */pPolygon->Add(SCMPoint2D((B+B3)*0.5+Bf3, H));
		/* 12+n+m */pPolygon->Add(SCMPoint2D((B+B3)*0.5+Bf3, H-tw));
	}
	else
	{
		/* l=1 */
		/* 10+n+m */pPolygon->Add(SCMPoint2D((B+B3)*0.5, H));
	}

	/* 10+n+m+l */pPolygon->Add(SCMPoint2D((B+B3)*0.5+tw, H-tw_f3));
	/* 11+n+m+l */pPolygon->Add(SCMPoint2D((B+B3)*0.5+tw, H-tw_f3-Hw2));
	/* 12+n+m+l */pPolygon->Add(SCMPoint2D(B, Hw1));
	/* 13+n+m+l */pPolygon->Add(SCMPoint2D(B, 0.0));

	if(d>0.0)
	{
		/* k=4 */
		/*14+n+m+l*/pPolygon->Add(SCMPoint2D(B-B1, 0.0));
		/*15+n+m+l*/pPolygon->Add(SCMPoint2D(0.5*B+0.5*B2, d));
		/*16+n+m+l*/pPolygon->Add(SCMPoint2D(0.5*B-0.5*B2, d));
		/*17+n+m+l*/pPolygon->Add(SCMPoint2D(B1, 0.0));
	}

	/* 14+n+m+l+k */pPolygon->Add(SCMPoint2D(0.0, 0.0));
}

void ICMGeomMaker::_MakePictureZ(double h, double b, double tw, double r, double d, double th, CCMPolygon* pPolygon)
{
	double rin = r;
	double rout=rin+tw;

	double oz, oy;
	static double sin22=sin(CMathFunc::m_trrad*22.), sin45=sin(CMathFunc::m_trrad*45.), sin68=sin(CMathFunc::m_trrad*68.);
	static double cos22=cos(CMathFunc::m_trrad*22.), cos45=cos(CMathFunc::m_trrad*45.), cos68=cos(CMathFunc::m_trrad*68.);

	if(d > 0.)
	{
		double dh=CMathFunc::mathSin(CMathFunc::m_trrad*th)*d;
		double db=CMathFunc::mathCos(CMathFunc::m_trrad*th)*d;
		double dh1=CMathFunc::mathCos(CMathFunc::m_trrad*th)*tw;
		double db1=CMathFunc::mathSin(CMathFunc::m_trrad*th)*tw;
		double dh2=dh+dh1-tw;
		double db2=tan(CMathFunc::m_trrad*th)>0.0? dh2/tan(CMathFunc::m_trrad*th) : 0.0;

		/* 0*/pPolygon->Add(SCMPoint2D(2*b+2*db-tw, dh));
		/* 1*/pPolygon->Add(SCMPoint2D(2*b+1*db-tw, 0.0));

		if(rin>0.0)
		{
			oy=b+db-tw+rout;
			oz=rout;
			/* 2*/pPolygon->Add(SCMPoint2D(oy,            oz-rout));
			/* 3*/pPolygon->Add(SCMPoint2D(oy-rout*cos68, oz-rout*sin68));
			/* 4*/pPolygon->Add(SCMPoint2D(oy-rout*cos45, oz-rout*sin45));
			/* 5*/pPolygon->Add(SCMPoint2D(oy-rout*cos22, oz-rout*sin22));
			/* 6*/pPolygon->Add(SCMPoint2D(oy-rout,       oz));

			oy=b+db-tw-rin;
			oz=h-tw-rin;
			/* 7*/pPolygon->Add(SCMPoint2D(oy+rin,        oz));
			/* 8*/pPolygon->Add(SCMPoint2D(oy+rin*cos22,  oz+rin*sin22));
			/* 9*/pPolygon->Add(SCMPoint2D(oy+rin*cos45,  oz+rin*sin45));
			/*10*/pPolygon->Add(SCMPoint2D(oy+rin*cos68,  oz+rin*sin68));
			/*11*/pPolygon->Add(SCMPoint2D(oy,            oz+rin));
		}
		else
		{
			oy=b+db-tw+rout;
			oz=rout;
			/* 2*/pPolygon->Add(SCMPoint2D(oy,            oz-rout));
			/* 3*/pPolygon->Add(SCMPoint2D(oy-rout,       oz-rout));
			/* 4*/pPolygon->Add(SCMPoint2D(oy-rout,       oz-rout));
			/* 5*/pPolygon->Add(SCMPoint2D(oy-rout,       oz-rout));
			/* 6*/pPolygon->Add(SCMPoint2D(oy-rout,       oz));

			oy=b+db-tw-rin;
			oz=h-tw-rin;
			/* 7*/pPolygon->Add(SCMPoint2D(oy+rin,        oz));
			/* 8*/pPolygon->Add(SCMPoint2D(oy+rin,        oz+rin));
			/* 9*/pPolygon->Add(SCMPoint2D(oy+rin,        oz+rin));
			/*10*/pPolygon->Add(SCMPoint2D(oy+rin,        oz+rin));
			/*11*/pPolygon->Add(SCMPoint2D(oy,            oz+rin));
		}

		/*12*/pPolygon->Add(SCMPoint2D(db1+db2, h-tw));
		/*13*/pPolygon->Add(SCMPoint2D(db1,     h-dh-dh1));

		/*14*/pPolygon->Add(SCMPoint2D(0.0,     h-dh));
		/*15*/pPolygon->Add(SCMPoint2D(db,      h));

		if(rin>0.0)
		{
			oy=b+db-rout;
			oz=h-rout;
			/*16*/pPolygon->Add(SCMPoint2D(oy,            oz+rout));
			/*17*/pPolygon->Add(SCMPoint2D(oy+rout*cos68, oz+rout*sin68));
			/*18*/pPolygon->Add(SCMPoint2D(oy+rout*cos45, oz+rout*sin45));
			/*19*/pPolygon->Add(SCMPoint2D(oy+rout*cos22, oz+rout*sin22));
			/*20*/pPolygon->Add(SCMPoint2D(oy+rout,       oz));

			oy=b+db+rin;
			oz=tw+rin;
			/*21*/pPolygon->Add(SCMPoint2D(oy-rin,        oz));
			/*22*/pPolygon->Add(SCMPoint2D(oy-rin*cos22,  oz-rin*sin22));
			/*23*/pPolygon->Add(SCMPoint2D(oy-rin*cos45,  oz-rin*sin45));
			/*24*/pPolygon->Add(SCMPoint2D(oy-rin*cos68,  oz-rin*sin68));
			/*25*/pPolygon->Add(SCMPoint2D(oy,            oz-rin));
		}
		else
		{
			oy=b+db-rout;
			oz=h-rout;
			/*16*/pPolygon->Add(SCMPoint2D(oy,            oz+rout));
			/*17*/pPolygon->Add(SCMPoint2D(oy+rout,       oz+rout));
			/*18*/pPolygon->Add(SCMPoint2D(oy+rout,       oz+rout));
			/*19*/pPolygon->Add(SCMPoint2D(oy+rout,       oz+rout));
			/*20*/pPolygon->Add(SCMPoint2D(oy+rout,       oz));

			oy=b+db+rin;
			oz=tw+rin;
			/*21*/pPolygon->Add(SCMPoint2D(oy-rin,        oz));
			/*22*/pPolygon->Add(SCMPoint2D(oy-rin,        oz-rin));
			/*23*/pPolygon->Add(SCMPoint2D(oy-rin,        oz-rin));
			/*24*/pPolygon->Add(SCMPoint2D(oy-rin,        oz-rin));
			/*25*/pPolygon->Add(SCMPoint2D(oy,            oz-rin));
		}

		/*26*/pPolygon->Add(SCMPoint2D(2*b+2*db-tw-db1-db2, tw));
		/*27*/pPolygon->Add(SCMPoint2D(2*b+2*db-tw-db1,     dh+dh1));
	}
	else
	{
		/* 0*/pPolygon->Add(SCMPoint2D(2*b-tw, 0.0));

		if(rin>0.0)
		{
			oy=b-tw+rout;
			oz=rout;
			/* 1*/pPolygon->Add(SCMPoint2D(oy,            oz-rout));
			/* 2*/pPolygon->Add(SCMPoint2D(oy-rout*cos68, oz-rout*sin68));
			/* 3*/pPolygon->Add(SCMPoint2D(oy-rout*cos45, oz-rout*sin45));
			/* 4*/pPolygon->Add(SCMPoint2D(oy-rout*cos22, oz-rout*sin22));
			/* 5*/pPolygon->Add(SCMPoint2D(oy-rout,       oz));

			oy=b-tw-rin;
			oz=h-tw-rin;
			/* 6*/pPolygon->Add(SCMPoint2D(oy+rin,        oz));
			/* 7*/pPolygon->Add(SCMPoint2D(oy+rin*cos22,  oz+rin*sin22));
			/* 8*/pPolygon->Add(SCMPoint2D(oy+rin*cos45,  oz+rin*sin45));
			/* 9*/pPolygon->Add(SCMPoint2D(oy+rin*cos68,  oz+rin*sin68));
			/*10*/pPolygon->Add(SCMPoint2D(oy,            oz+rin));
		}
		else
		{
			oy=b-tw;
			/* 1*/pPolygon->Add(SCMPoint2D(oy+b*0.50, (h-tw)*0.00));
			/* 2*/pPolygon->Add(SCMPoint2D(oy+b*0.25, (h-tw)*0.00));
			/* 3*/pPolygon->Add(SCMPoint2D(oy+b*0.00, (h-tw)*0.00));
			/* 4*/pPolygon->Add(SCMPoint2D(oy+b*0.00, (h-tw)*0.10));
			/* 5*/pPolygon->Add(SCMPoint2D(oy+b*0.00, (h-tw)*0.20));

			/* 6*/pPolygon->Add(SCMPoint2D(oy-(b-tw)*0.00, (h-tw)*0.80));
			/* 7*/pPolygon->Add(SCMPoint2D(oy-(b-tw)*0.00, (h-tw)*0.90));
			/* 8*/pPolygon->Add(SCMPoint2D(oy-(b-tw)*0.00, (h-tw)*1.00));
			/* 9*/pPolygon->Add(SCMPoint2D(oy-(b-tw)*0.25, (h-tw)*1.00));
			/*10*/pPolygon->Add(SCMPoint2D(oy-(b-tw)*0.50, (h-tw)*1.00));
		}

		/*11*/pPolygon->Add(SCMPoint2D(0.0, h-tw));
		
		/*12*/pPolygon->Add(SCMPoint2D(0.0, h));

		if(rin>0.0)
		{
			oy=b-rout;
			oz=h-rout;
			/*13*/pPolygon->Add(SCMPoint2D(oy,            oz+rout));
			/*14*/pPolygon->Add(SCMPoint2D(oy+rout*cos68, oz+rout*sin68));
			/*15*/pPolygon->Add(SCMPoint2D(oy+rout*cos45, oz+rout*sin45));
			/*16*/pPolygon->Add(SCMPoint2D(oy+rout*cos22, oz+rout*sin22));
			/*17*/pPolygon->Add(SCMPoint2D(oy+rout,       oz));

			oy=b+rin;
			oz=tw+rin;
			/*18*/pPolygon->Add(SCMPoint2D(oy-rin,        oz));
			/*19*/pPolygon->Add(SCMPoint2D(oy-rin*cos22,  oz-rin*sin22));
			/*20*/pPolygon->Add(SCMPoint2D(oy-rin*cos45,  oz-rin*sin45));
			/*21*/pPolygon->Add(SCMPoint2D(oy-rin*cos68,  oz-rin*sin68));
			/*22*/pPolygon->Add(SCMPoint2D(oy,            oz-rin));
		}
		else
		{
			/*13*/pPolygon->Add(SCMPoint2D(b*0.50, tw+(h-tw)*1.00));
			/*14*/pPolygon->Add(SCMPoint2D(b*0.75, tw+(h-tw)*1.00));
			/*15*/pPolygon->Add(SCMPoint2D(b*1.00, tw+(h-tw)*1.00));
			/*16*/pPolygon->Add(SCMPoint2D(b*1.00, tw+(h-tw)*0.90));
			/*17*/pPolygon->Add(SCMPoint2D(b*1.00, tw+(h-tw)*0.80));

			oy=b;
			/*18*/pPolygon->Add(SCMPoint2D(oy+(b-tw)*0.00, tw+(h-tw)*0.20));
			/*19*/pPolygon->Add(SCMPoint2D(oy+(b-tw)*0.00, tw+(h-tw)*0.10));
			/*20*/pPolygon->Add(SCMPoint2D(oy+(b-tw)*0.00, tw+(h-tw)*0.00));
			/*21*/pPolygon->Add(SCMPoint2D(oy+(b-tw)*0.25, tw+(h-tw)*0.00));
			/*22*/pPolygon->Add(SCMPoint2D(oy+(b-tw)*0.50, tw+(h-tw)*0.00));
		}

		/*23*/pPolygon->Add(SCMPoint2D(2*b-tw, tw));
	}
}

void ICMGeomMaker::_MakePictureURib(double H, double B1, double B2, double t, double r, CCMPolygon* pPolygon)
{
/*
	pPolygon->Add(SCMPoint2D(0, 0));
	pPolygon->Add(SCMPoint2D((B1-B2)*0.5, H));
	pPolygon->Add(SCMPoint2D(B1-((B1-B2)*0.5), H));
	pPolygon->Add(SCMPoint2D(B1, 0));
*/
	double dPi	 = 4.*atan(1.);
	double dAng	 = (fabs(H) < 1.0e-10) ? 0.0 : atan((B1-B2)/(2.*H));
	double dAngR = dPi/2. - dAng;
	double dB		 = B2 - 2.*(r+t)*tan(dAngR/2.);

	double dPoint[20][2];	// 6(Point Number), 2(X,Y).
	
	dPoint[0][0]	= dB/2.;
	dPoint[0][1] 	= t;

	dPoint[1][0]	= dB/2. + r*sin(dAngR*1/3);
	dPoint[1][1] 	= (r+t) - r*cos(dAngR*1/3);
	dPoint[2][0]	= dB/2. + r*sin(dAngR*2/3);
	dPoint[2][1] 	= (r+t) - r*cos(dAngR*2/3);

	dPoint[3][0]	= dB/2. + r*sin(dAngR);
	dPoint[3][1] 	= (r+t) - r*cos(dAngR);
	dPoint[4][0]	= B1/2. - t/cos(dAng);
	dPoint[4][1] 	= H;
	dPoint[5][0]	= B1/2. - t*sin(dAng)*tan(dAng);
	dPoint[5][1] 	= H - t*sin(dAng);
	dPoint[6][0]	= dB/2. + (r+t)*sin(dAngR);
	dPoint[6][1] 	= (r+t) - (r+t)*cos(dAngR);

	dPoint[7][0]	= dB/2. + (r+t)*sin(dAngR*2/3);
	dPoint[7][1] 	= (r+t) - (r+t)*cos(dAngR*2/3);
	dPoint[8][0]	= dB/2. + (r+t)*sin(dAngR*1/3);
	dPoint[8][1] 	= (r+t) - (r+t)*cos(dAngR*1/3);

	dPoint[9][0]	= dB/2.;
	dPoint[9][1] 	= 0.0;



	dPoint[10][0]	= -dPoint[9][0];
	dPoint[10][1] =  dPoint[9][1];

	dPoint[11][0]	= -dPoint[8][0];
	dPoint[11][1] =  dPoint[8][1];
	dPoint[12][0]	= -dPoint[7][0];
	dPoint[12][1] =  dPoint[7][1];

	dPoint[13][0]	= -dPoint[6][0];
	dPoint[13][1] =  dPoint[6][1];
	dPoint[14][0]	= -dPoint[5][0];
	dPoint[14][1] =  dPoint[5][1];
	dPoint[15][0]	= -dPoint[4][0];
	dPoint[15][1] =  dPoint[4][1];
	dPoint[16][0]	= -dPoint[3][0];
	dPoint[16][1] =  dPoint[3][1];

	dPoint[17][0]	= -dPoint[2][0];
	dPoint[17][1] =  dPoint[2][1];
	dPoint[18][0]	= -dPoint[1][0];
	dPoint[18][1] =  dPoint[1][1];

	dPoint[19][0]	= -dPoint[0][0];
	dPoint[19][1] =  dPoint[0][1];

	for(int i=0; i < 20; i++)
		pPolygon->Add(SCMPoint2D(dPoint[i][0], (-1.)*(dPoint[i][1]-H/2.)));
}

void ICMGeomMaker::_MakePictureOctagon(double H, double B, double a, double b, CCMPolygon* pPolygon)
{
	pPolygon->Add(SCMPoint2D(-(B/2.0-a),  (H/2.0)   ));
	pPolygon->Add(SCMPoint2D(-(B/2.0)  ,  (H/2.0-b) ));
	pPolygon->Add(SCMPoint2D(-(B/2.0)  , -(H/2.0-b) ));
	pPolygon->Add(SCMPoint2D(-(B/2.0-a), -(H/2.0)   ));
	pPolygon->Add(SCMPoint2D( (B/2.0-a), -(H/2.0)   ));
	pPolygon->Add(SCMPoint2D( (B/2.0)  , -(H/2.0-b) ));
	pPolygon->Add(SCMPoint2D( (B/2.0)  ,  (H/2.0-b) ));
	pPolygon->Add(SCMPoint2D( (B/2.0-a),  (H/2.0)   ));
}

void ICMGeomMaker::_MakePictureTrack(double H, double B, CCMPolygon* pPolygon)
{
	SCMPoint2D mid[17];
	int nNum = 17;
	// Round 부분 중간 값을 계산해 놓는다.
	int i;
	double r = H/2.0;
	double angle = 180.0/(nNum+1);
	for (i = 0; i < nNum; i++)
	{
		mid[i].x = r*sin(angle*(i+1)*M_PI/180.0);
		mid[i].y = r*cos(angle*(i+1)*M_PI/180.0);
	}

	double x, y;
	pPolygon->Add(SCMPoint2D(-(B/2.0),  (H/2.0)));
	x = -(B/2.0); y = (H/2.0);
	for (i = 0; i < nNum; i++)
		pPolygon->Add(SCMPoint2D(x-mid[i].x, mid[i].y));
	pPolygon->Add(SCMPoint2D(-(B/2.0), -(H/2.0) ));
	pPolygon->Add(SCMPoint2D( (B/2.0), -(H/2.0) ));
	x = (B/2.0); y = -(H/2.0);
	for (i = 0; i < nNum; i++)
		pPolygon->Add(SCMPoint2D(x+mid[i].x, -mid[i].y));
	pPolygon->Add(SCMPoint2D( (B/2.0),  (H/2.0) ));
}

void ICMGeomMaker::_MakePictureHalfTrack(double H, double B, CCMPolygon* pPolygon)
{
	SCMPoint2D mid[17];
	int nNum = 17;
	// Round 부분 중간 값을 계산해 놓는다.
	int i;
	double r = H/2.0;
	double angle = 180.0/(nNum+1);
	for (i = 0; i < nNum; i++)
	{
		mid[i].x = r*sin(angle*(i+1)*M_PI/180.0);
		mid[i].y = r*cos(angle*(i+1)*M_PI/180.0);
	}

	double x, y;
	pPolygon->Add(SCMPoint2D(-(B/2.0),  (H/2.0)));
	pPolygon->Add(SCMPoint2D(-(B/2.0), -(H/2.0) ));
	pPolygon->Add(SCMPoint2D( (B/2.0), -(H/2.0) ));
	x = (B/2.0); y = -(H/2.0);
	for (i = 0; i < nNum; i++)
		pPolygon->Add(SCMPoint2D(x+mid[i].x, -mid[i].y));
	pPolygon->Add(SCMPoint2D( (B/2.0),  (H/2.0) ));
}

void ICMGeomMaker::MakePictureH_Stiffener(double H, double B, double theta, double dy_org, double dz_org, int nPos, CCMPolygon* pPolygon)
{
	double dy, dz;

	dy = B * 0.5;
	dz = 0.0;
	pPolygon->Add(SCMPoint2D(dy, dz));
	
	dy = B * 0.5;
	dz = -H;
	pPolygon->Add(SCMPoint2D(dy, dz));
	
	dy = -(B * 0.5);
	dz = -H;
	pPolygon->Add(SCMPoint2D(dy, dz));
	
	dy = -(B * 0.5);
	dz = 0.0;
	pPolygon->Add(SCMPoint2D(dy, dz));

	if     (nPos == 0) pPolygon->Rotate(theta * CMathFunc::m_trang);
	else if(nPos == 1) pPolygon->Rotate(-(theta * CMathFunc::m_trang));
	else if(nPos == 3) pPolygon->Rotate(theta * CMathFunc::m_trang);
		
	pPolygon->Transfer(SCMPoint2D(dy_org, dz_org));
}

void ICMGeomMaker::MakePictureT_Stiffener(double H, double B, double tw, double tf, double theta, double dy_org, double dz_org, int nPos, CCMPolygon* pPolygon)
{
	double dy, dz;

	dy = -(tw * 0.5);
	dz = 0.0;
	pPolygon->Add(SCMPoint2D(dy, dz));
	
	dy = -(tw * 0.5);
	dz = -(H - tf);
	pPolygon->Add(SCMPoint2D(dy, dz));
	
	dy = -(B * 0.5);
	dz = -(H - tf);
	pPolygon->Add(SCMPoint2D(dy, dz));
	
	dy = -(B * 0.5);
	dz = -H;
	pPolygon->Add(SCMPoint2D(dy, dz));	
	
	dy = +(B * 0.5);
	dz = -H;
	pPolygon->Add(SCMPoint2D(dy, dz));	
	
	dy = +(B * 0.5);
	dz = -(H - tf);
	pPolygon->Add(SCMPoint2D(dy, dz));
	
	dy = +(tw * 0.5);
	dz = -(H - tf);
	pPolygon->Add(SCMPoint2D(dy, dz));
	
	dy = +(tw * 0.5);
	dz = 0.0;
	pPolygon->Add(SCMPoint2D(dy, dz));
	
	if     (nPos == 0) pPolygon->Rotate(theta * CMathFunc::m_trang);
	else if(nPos == 1) pPolygon->Rotate(-(theta * CMathFunc::m_trang));
	else if(nPos == 3) pPolygon->Rotate(theta * CMathFunc::m_trang);

	pPolygon->Transfer(SCMPoint2D(dy_org, dz_org));
}

void ICMGeomMaker::MakePictureURib_Stiffener(double H, double B1, double B2, double t, double R, double theta, double dy_org, double dz_org, double tf, int nPos, CCMPolygon* pPolygon)
{
	double dPi	 = 4.*atan(1.);
	double dAng	 = (fabs(H) < 1.0e-10) ? 0.0 : atan((B1-B2)/(2.*H));
	double dAngR = dPi/2. - dAng;
	double dB		 = B2 - 2.*(R+t)*tan(dAngR/2.);
	
	double dPoint[20][2];
	
	dPoint[0][0]	= dB/2.;
	dPoint[0][1] 	= t;
	
	dPoint[1][0]	= dB/2. + R*sin(dAngR*1/3);
	dPoint[1][1] 	= (R+t) - R*cos(dAngR*1/3);
	dPoint[2][0]	= dB/2. + R*sin(dAngR*2/3);
	dPoint[2][1] 	= (R+t) - R*cos(dAngR*2/3);
	
	dPoint[3][0]	= dB/2. + R*sin(dAngR);
	dPoint[3][1] 	= (R+t) - R*cos(dAngR);
	dPoint[4][0]	= B1/2. - t/cos(dAng);
	dPoint[4][1] 	= H;
	
	dPoint[5][0]	= B1/2. - t*sin(dAng)*tan(dAng);
	dPoint[5][1] 	= H - t*sin(dAng);
	dPoint[6][0]	= dB/2. + (R+t)*sin(dAngR);
	dPoint[6][1] 	= (R+t) - (R+t)*cos(dAngR);
	
	dPoint[7][0]	= dB/2. + (R+t)*sin(dAngR*2/3);
	dPoint[7][1] 	= (R+t) - (R+t)*cos(dAngR*2/3);
	dPoint[8][0]	= dB/2. + (R+t)*sin(dAngR*1/3);
	dPoint[8][1] 	= (R+t) - (R+t)*cos(dAngR*1/3);
	
	dPoint[9][0]	= dB/2.;
	dPoint[9][1] 	= 0.0;
		
	
	dPoint[10][0]	= -dPoint[9][0];
	dPoint[10][1] =  dPoint[9][1];
	
	dPoint[11][0]	= -dPoint[8][0];
	dPoint[11][1] =  dPoint[8][1];
	dPoint[12][0]	= -dPoint[7][0];
	dPoint[12][1] =  dPoint[7][1];
	
	dPoint[13][0]	= -dPoint[6][0];
	dPoint[13][1] =  dPoint[6][1];
	dPoint[14][0]	= -dPoint[5][0];
	dPoint[14][1] =  dPoint[5][1];
	dPoint[15][0]	= -dPoint[4][0];
	dPoint[15][1] =  dPoint[4][1];
	dPoint[16][0]	= -dPoint[3][0];
	dPoint[16][1] =  dPoint[3][1];
	
	dPoint[17][0]	= -dPoint[2][0];
	dPoint[17][1] =  dPoint[2][1];
	dPoint[18][0]	= -dPoint[1][0];
	dPoint[18][1] =  dPoint[1][1];
	
	dPoint[19][0]	= -dPoint[0][0];
	dPoint[19][1] =  dPoint[0][1];
	
	for(int i=0; i<20; ++i)
		pPolygon->Add(SCMPoint2D(dPoint[i][0], dPoint[i][1]));

	pPolygon->Transfer(SCMPoint2D(0.0, -H));

	if     (nPos == 0) pPolygon->Rotate(theta * CMathFunc::m_trang);
	else if(nPos == 1) pPolygon->Rotate(-(theta * CMathFunc::m_trang));
	else if(nPos == 3) pPolygon->Rotate(theta * CMathFunc::m_trang);
	
	pPolygon->Transfer(SCMPoint2D(dy_org, dz_org));
}

void ICMGeomMaker::MakePictureH_SODStiffener(double H, double B, double theta, double dy_org, double dz_org, CCMPolygon* pPolygon)
{
	double dy, dz;
	
	dy = B * 0.5;
	dz = 0.0;
	pPolygon->Add(SCMPoint2D(dy, dz));
	
	dy = B * 0.5;
	dz = -H;
	pPolygon->Add(SCMPoint2D(dy, dz));
	
	dy = -(B * 0.5);
	dz = -H;
	pPolygon->Add(SCMPoint2D(dy, dz));
	
	dy = -(B * 0.5);
	dz = 0.0;
	pPolygon->Add(SCMPoint2D(dy, dz));

	pPolygon->Rotate(theta * CMathFunc::m_trang);
 	pPolygon->Transfer(SCMPoint2D(dy_org, dz_org));
}

void ICMGeomMaker::MakePictureT_SODStiffener(double H, double B, double tw, double tf, double theta, double dy_org, double dz_org, CCMPolygon* pPolygon)
{
	double dy, dz;
	
	dy = -(tw * 0.5);
	dz = 0.0;
	pPolygon->Add(SCMPoint2D(dy, dz));
	
	dy = -(tw * 0.5);
	dz = -(H - tf);
	pPolygon->Add(SCMPoint2D(dy, dz));
	
	dy = -(B * 0.5);
	dz = -(H - tf);
	pPolygon->Add(SCMPoint2D(dy, dz));
	
	dy = -(B * 0.5);
	dz = -H;
	pPolygon->Add(SCMPoint2D(dy, dz));	
	
	dy = +(B * 0.5);
	dz = -H;
	pPolygon->Add(SCMPoint2D(dy, dz));	
	
	dy = +(B * 0.5);
	dz = -(H - tf);
	pPolygon->Add(SCMPoint2D(dy, dz));
	
	dy = +(tw * 0.5);
	dz = -(H - tf);
	pPolygon->Add(SCMPoint2D(dy, dz));
	
	dy = +(tw * 0.5);
	dz = 0.0;
	pPolygon->Add(SCMPoint2D(dy, dz));
		
	pPolygon->Rotate(theta * CMathFunc::m_trang);
	pPolygon->Transfer(SCMPoint2D(dy_org, dz_org));
}

void ICMGeomMaker::MakePictureL_SODStiffener(double H, double B, double tw, double tf, int nDir, double theta, double dy_org, double dz_org, CCMPolygon* pPolygon)
{

	double dy, dz;

	if(nDir==0) // Left
	{
		dy = -(tw * 0.5);
		dz = 0.0;
		pPolygon->Add(SCMPoint2D(dy, dz));

		dy = -(tw * 0.5);
		dz = -H;
		pPolygon->Add(SCMPoint2D(dy, dz));

		dy = +(B - tw * 0.5);
		dz = -H;
		pPolygon->Add(SCMPoint2D(dy, dz));	

		dy = +(B - tw * 0.5);
		dz = -(H - tf);
		pPolygon->Add(SCMPoint2D(dy, dz));

		dy = +(tw * 0.5);
		dz = -(H - tf);
		pPolygon->Add(SCMPoint2D(dy, dz));

		dy = +(tw * 0.5);
		dz = 0.0;
		pPolygon->Add(SCMPoint2D(dy, dz));
	}
	else if(nDir==1) // Right
	{
		dy = -(tw * 0.5);
		dz = 0.0;
		pPolygon->Add(SCMPoint2D(dy, dz));

		dy = -(tw * 0.5);
		dz = -(H - tf);
		pPolygon->Add(SCMPoint2D(dy, dz));

		dy = -(B - tw * 0.5);
		dz = -(H - tf);
		pPolygon->Add(SCMPoint2D(dy, dz));	

		dy = -(B - tw * 0.5);
		dz = -H;
		pPolygon->Add(SCMPoint2D(dy, dz));

		dy = +(tw * 0.5);
		dz = -H;
		pPolygon->Add(SCMPoint2D(dy, dz));

		dy = +(tw * 0.5);
		dz = 0.0;
		pPolygon->Add(SCMPoint2D(dy, dz));

	}
	else
		ASSERT(0);


	pPolygon->Rotate(theta * CMathFunc::m_trang);
	pPolygon->Transfer(SCMPoint2D(dy_org, dz_org));

}

void ICMGeomMaker::MakePictureURib_SODStiffener(double H, double B1, double B2, double t, double R, double theta, double dy_org, double dz_org, CCMPolygon* pPolygon)
{
	double dPi	 = 4.*atan(1.);
	double dAng	 = (fabs(H) < 1.0e-10) ? 0.0 : atan((B1-B2)/(2.*H));
	double dAngR = dPi/2. - dAng;
	double dB		 = B2 - 2.*(R+t)*tan(dAngR/2.);
	
	double dPoint[20][2];
	
	dPoint[0][0]	= dB/2.;
	dPoint[0][1] 	= t;
	
	dPoint[1][0]	= dB/2. + R*sin(dAngR*1/3);
	dPoint[1][1] 	= (R+t) - R*cos(dAngR*1/3);
	dPoint[2][0]	= dB/2. + R*sin(dAngR*2/3);
	dPoint[2][1] 	= (R+t) - R*cos(dAngR*2/3);
	
	dPoint[3][0]	= dB/2. + R*sin(dAngR);
	dPoint[3][1] 	= (R+t) - R*cos(dAngR);
	dPoint[4][0]	= B1/2. - t/cos(dAng);
	dPoint[4][1] 	= H;
	
	dPoint[5][0]	= B1/2. - t*sin(dAng)*tan(dAng);
	dPoint[5][1] 	= H - t*sin(dAng);
	dPoint[6][0]	= dB/2. + (R+t)*sin(dAngR);
	dPoint[6][1] 	= (R+t) - (R+t)*cos(dAngR);
	
	dPoint[7][0]	= dB/2. + (R+t)*sin(dAngR*2/3);
	dPoint[7][1] 	= (R+t) - (R+t)*cos(dAngR*2/3);
	dPoint[8][0]	= dB/2. + (R+t)*sin(dAngR*1/3);
	dPoint[8][1] 	= (R+t) - (R+t)*cos(dAngR*1/3);
	
	dPoint[9][0]	= dB/2.;
	dPoint[9][1] 	= 0.0;
		
	
	dPoint[10][0]	= -dPoint[9][0];
	dPoint[10][1] =  dPoint[9][1];
	
	dPoint[11][0]	= -dPoint[8][0];
	dPoint[11][1] =  dPoint[8][1];
	dPoint[12][0]	= -dPoint[7][0];
	dPoint[12][1] =  dPoint[7][1];
	
	dPoint[13][0]	= -dPoint[6][0];
	dPoint[13][1] =  dPoint[6][1];
	dPoint[14][0]	= -dPoint[5][0];
	dPoint[14][1] =  dPoint[5][1];
	dPoint[15][0]	= -dPoint[4][0];
	dPoint[15][1] =  dPoint[4][1];
	dPoint[16][0]	= -dPoint[3][0];
	dPoint[16][1] =  dPoint[3][1];
	
	dPoint[17][0]	= -dPoint[2][0];
	dPoint[17][1] =  dPoint[2][1];
	dPoint[18][0]	= -dPoint[1][0];
	dPoint[18][1] =  dPoint[1][1];
	
	dPoint[19][0]	= -dPoint[0][0];
	dPoint[19][1] =  dPoint[0][1];
	
	for(int i=0; i<20; ++i)
		pPolygon->Add(SCMPoint2D(dPoint[i][0], dPoint[i][1]));
	
	pPolygon->Transfer(SCMPoint2D(0.0, -H));
	
	pPolygon->Rotate(theta * CMathFunc::m_trang);
	pPolygon->Transfer(SCMPoint2D(dy_org, dz_org));
}

void ICMGeomMaker::MakePictureAngle_Rivet(double H, double B, double tw, double tf, int nAngleType, double dy_org, double dz_org, CCMPolygon* pPolygon)
{
	//   nAngleType 
	//    0    1    2    3
	//  __           __
	//    |    |    |    |
	//    |  __|    |    |__
	//

	double dy, dz;
	double dF = -1.0;
	if (nAngleType == 0)
	{
		dy = 0.0;
		dz = 0.0;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = -B;
		dz = 0.0;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = -B;
		dz = -tf;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = -tw;
		dz = -tf;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = -tw;
		dz = -H;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = 0.0;
		dz = -H;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));
	}
	else if (nAngleType == 1)
	{
		dy = 0.0;
		dz = 0.0;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = 0.0;
		dz = H;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = -tw;
		dz = H;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = -tw;
		dz =  tf;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = -B;
		dz = tf;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = -B;
		dz = 0.0;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));
	}
	else if (nAngleType == 2)
	{
		dy = 0.0;
		dz = 0.0;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = 0.0;
		dz = -H;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = tw;
		dz = -H;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = tw;
		dz = -tf;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = B;
		dz = -tf;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = B;
		dz = 0.0;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));
	}
	else if (nAngleType == 3)
	{
		dy = 0.0;
		dz = 0.0;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = B;
		dz = 0.0;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = B;
		dz = tf;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = tw;
		dz = tf;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = tw;
		dz = H;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));

		dy = 0.0;
		dz = H;
		pPolygon->Add(SCMPoint2D(dy, dF*dz));
	}
	else
		ASSERT(0);

	pPolygon->Transfer(SCMPoint2D(dy_org, dz_org));

}

//-----------------------------------------------------------------------
void ICMGeomMaker::GetSrcData(double &HC, double &BC, double &H, double &B, 
															double &tw, double &tf1, double &C, double &tf2)
{
	H = m_pDataSrc->SectBefore.SectI.Size[0];
	B = m_pDataSrc->SectBefore.SectI.Size[1];
	tw = m_pDataSrc->SectBefore.SectI.Size[2];
	tf1 = m_pDataSrc->SectBefore.SectI.Size[3];
	C = m_pDataSrc->SectBefore.SectI.Size[4];
	tf2 = m_pDataSrc->SectBefore.SectI.Size[5];

	HC = m_pDataSrc->SectBefore.SectJ.Size[0];
	BC = m_pDataSrc->SectBefore.SectJ.Size[1];
}

void ICMGeomMaker::GetSrcData2(double &HC, double &BC,   
															 double &H, double &B, double &tw,  double &tf,  
															 double &H_, double &B_,double &tw_,  double &tf_)
{
	H   = m_pDataSrc->SectBefore.SectI.Size[0];
	B   = m_pDataSrc->SectBefore.SectI.Size[1];
	tw  = m_pDataSrc->SectBefore.SectI.Size[2];
	tf  = m_pDataSrc->SectBefore.SectI.Size[3];
	H_  = m_pDataSrc->SectBefore.SectI.Size[4];
	B_  = m_pDataSrc->SectBefore.SectI.Size[5];
	tw_ = m_pDataSrc->SectBefore.SectI.Size[6];
	tf_ = m_pDataSrc->SectBefore.SectI.Size[7];
	
	HC = m_pDataSrc->SectBefore.SectJ.Size[0];
	BC = m_pDataSrc->SectBefore.SectJ.Size[1];
}

void ICMGeomMaker::MakePictureRectBox(CCMGeom2DArray &aGeom, BOOL bOpen)
{
	double HC, BC, H, B, tw, tf1, C, tf2;
	GetSrcData(HC, BC, H, B, tw, tf1, C, tf2);

	// concrete
	CCMPolygon *pPolygon = new CCMPolygon(CCM_BORDER, CCM_SRC_FILL);
	_MakeRect(0.0, 0.0, BC, HC, pPolygon);
	aGeom.Add(pPolygon);

	if(C == 0.0) C = B-tw;
	if(tf2 == 0.0) tf2 = tf1;

	// steel
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeBox(H, B, tw, tf1, C, tf2, pPolygon);
	pPolygon->Transfer(SCMPoint2D((BC-B)/2.0, (HC-H)/2.0));
	aGeom.Add(pPolygon);

	// rect hole
	double tm1 = (B-C-tw)/2.0+tw;
	pPolygon = new CCMPolygon(CCM_BORDER, (bOpen)?CCM_HOLE:CCM_SRC_FILL);
	_MakeRect(tm1, tf1, B-tm1, H-tf2, pPolygon);
	pPolygon->Transfer(SCMPoint2D((BC-B)/2.0, (HC-H)/2.0));
	pPolygon->PenColor = CCM_BORDER;
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureRectPipe(CCMGeom2DArray &aGeom, BOOL bOpen)
{
	double HC, BC, H, B, tw, tf1, C, tf2;
	GetSrcData(HC, BC, H, B, tw, tf1, C, tf2);

	// concrete
	CCMPolygon *pPolygon = new CCMPolygon(CCM_BORDER, CCM_SRC_FILL);
	_MakeRect(0.0, 0.0, BC, HC, pPolygon);
	aGeom.Add(pPolygon);

	// outer ring of pipe
	CCMEllipse *pEllipse = new CCMEllipse(CCM_BORDER, CCM_FILL);
	pEllipse->SetRect(SCMPoint2D(0.0, 0.0), SCMPoint2D(H, H));
	pEllipse->Transfer(SCMPoint2D((BC-H)/2.0, (HC-H)/2.0));
	aGeom.Add(pEllipse);

	// inner ring of pipe
	double R2 = H-B*2;
	pEllipse = new CCMEllipse(CCM_BORDER, (bOpen)?CCM_HOLE:CCM_SRC_FILL);
	pEllipse->SetRect(SCMPoint2D(0.0, 0.0), SCMPoint2D(R2, R2));
	pEllipse->Transfer(SCMPoint2D((BC-R2)/2.0, (HC-R2)/2.0));
	aGeom.Add(pEllipse);
}

void ICMGeomMaker::MakePictureRectHBeam(CCMGeom2DArray &aGeom)
{
	double HC, BC, H, B, tw, tf1, C, tf2;
	GetSrcData(HC, BC, H, B, tw, tf1, C, tf2);

	// concrete
	CCMPolygon *pPolygon = new CCMPolygon(CCM_BORDER, CCM_SRC_FILL);
	_MakeRect(0.0, 0.0, BC, HC, pPolygon);
	aGeom.Add(pPolygon);

	// steel(H-Beam)
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeHBeam(H, B, tw, tf1, C, tf2, pPolygon);
	if(B>C) { pPolygon->Transfer(SCMPoint2D((BC-B)/2.0, (HC-H)/2.0)); } // MQC:2031-JHYUN-20080721
	else { pPolygon->Transfer(SCMPoint2D((BC-C)/2.0, (HC-H)/2.0)); } 
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureRectH2TBeam(CCMGeom2DArray &aGeom)
{
	double HC, BC, H, B, tw, tf;
	double _H, _B, _tw, _tf;
	GetSrcData2(HC, BC, H, B, tw, tf, _H, _B, _tw, _tf);

	// concrete
	CCMPolygon *pPolygon = new CCMPolygon(CCM_BORDER, CCM_SRC_FILL);
	_MakeRect(0.0, 0.0, BC, HC, pPolygon);
	aGeom.Add(pPolygon);

	double B2,tf2;
	B2 = B; tf2 = tf;
	CCMPolygon *pPolygonF = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeHBeam(H, B, tw, tf, B2, tf2, pPolygonF);
	pPolygonF->Transfer(SCMPoint2D((BC-B)/2.0, (HC-H)/2.0));

	CCMPolygon *pPolygonS = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeTee(_H, _B, _tw, _tf, pPolygonS);
	CCMPolygon *pPolygonT = new CCMPolygon(CCM_BORDER,
 CCM_FILL);
	_MakeTee(_H, _B, _tw, _tf, pPolygonT);

	pPolygonS->Rotate(90.0);
	pPolygonT->Rotate(270.0);

	SCMRect2D mbrS = pPolygonS->GetMBR();
	//pPolygonS->Transfer(SCMPoint2D((BC-_H)/2.0, (HC-_B)/2.0));
	pPolygonS->Transfer(SCMPoint2D(-mbrS.min.x, -mbrS.min.y));
	pPolygonS->Transfer(SCMPoint2D(BC/2.0+tw/2.0, HC/2.0-_B/2.0));
	SCMRect2D mbrT = pPolygonT->GetMBR();
	//pPolygonT->Transfer(SCMPoint2D((BC-_H)/2.0, (HC-_B)/2.0));
	pPolygonT->Transfer(SCMPoint2D(-mbrT.min.x, -mbrT.min.y));
	pPolygonT->Transfer(SCMPoint2D(BC/2.0-tw/2.0-_H, HC/2.0-_B/2.0));

	aGeom.Add(pPolygonF);
	aGeom.Add(pPolygonS);
	aGeom.Add(pPolygonT);
}

void ICMGeomMaker::MakePictureRectHTBeam(CCMGeom2DArray &aGeom)
{
	double HC, BC, H, B, tw, tf;
	double _H, _B, _tw, _tf;
	//GetSrcData2(HC, BC, H, B, tw, tf, _H, _B, _tw, _tf);
	GetSrcData2(HC, BC, _H, _B, _tw, _tf, H, B, tw, tf);

	// concrete
	CCMPolygon *pPolygon = new CCMPolygon(CCM_BORDER, CCM_SRC_FILL);
	_MakeRect(0.0, 0.0, BC, HC, pPolygon);
	aGeom.Add(pPolygon);

	// T
	CCMPolygon *pPolygonF = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeTee(H, B, tw, tf, pPolygonF);
	
	// H
	double _B2,_tf2;
	_B2 = _B; _tf2 = _tf;
	CCMPolygon *pPolygonS = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeHBeam(_H, _B, _tw, _tf, _B2, _tf2, pPolygonS);

	//pPolygonS->Rotate(90.0);
	SCMRect2D mbrS = pPolygonS->GetMBR();
	pPolygonS->Transfer(SCMPoint2D(-mbrS.min.x, -mbrS.min.y));

	//pPolygonF->Rotate(180.0);
	pPolygonF->Rotate(-90.0);
	SCMRect2D mbrF = pPolygonF->GetMBR();
	pPolygonF->Transfer(SCMPoint2D(-mbrF.min.x, -mbrF.min.y));
	//pPolygonF->Transfer(SCMPoint2D(_H/2.0-B/2.0,_B/2.0-_tw/2.0));
	//pPolygonF->Transfer(SCMPoint2D(-(_B/2.0-H)-_tw/2.0,_H/2.0-B/2.0));
	pPolygonF->Transfer(SCMPoint2D(-(H - _B/2.0)-_tw/2.0,_H/2.0-B/2.0));

	mbrS = pPolygonS->GetMBR();
	mbrF = pPolygonF->GetMBR();

	// HT의 Boundary계산 
	SCMRect2D mbrHT;
	mbrHT.min.x = min(mbrS.min.x,mbrF.min.x);
	mbrHT.min.y = min(mbrS.min.y,mbrF.min.y);
	mbrHT.max.x = max(mbrS.max.x,mbrF.max.x);
	mbrHT.max.y = max(mbrS.max.y,mbrF.max.y);

	// Rect의 Boundary계산 
	SCMRect2D mbrRect = pPolygon->GetMBR();

	// 이동거리계산 
	double dX = (mbrRect.max.x+mbrRect.min.x)/2.-(mbrHT.max.x+mbrHT.min.x)/2.;
	double dY = (mbrRect.max.y+mbrRect.min.y)/2.-(mbrHT.max.y+mbrHT.min.y)/2.;

	pPolygonF->Transfer(SCMPoint2D(dX,dY));
	pPolygonS->Transfer(SCMPoint2D(dX,dY));
			
	aGeom.Add(pPolygonF);
	aGeom.Add(pPolygonS);
}

void ICMGeomMaker::MakePictureCircleBox(CCMGeom2DArray &aGeom, BOOL bOpen)
{
	double HC, BC, H, B, tw, tf1, C, tf2;
	GetSrcData(HC, BC, H, B, tw, tf1, C, tf2);

	// concrete
	CCMEllipse *pEllipse = new CCMEllipse(CCM_BORDER, CCM_SRC_FILL);
	pEllipse->SetRect(SCMPoint2D(0.0, 0.0), SCMPoint2D(HC, HC));
	aGeom.Add(pEllipse);

	if(C == 0.0) 	C = B-tw;
	if(tf2 == 0.0)	tf2 = tf1;

	// steel
	CCMPolygon *pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeBox(H, B, tw, tf1, C, tf2, pPolygon);
	pPolygon->Transfer(SCMPoint2D((HC-B)/2.0, (HC-H)/2.0));
	aGeom.Add(pPolygon);

	// rect hole
	double tm1 = (B-C-tw)/2.0+tw;
	pPolygon = new CCMPolygon(CCM_BORDER, (bOpen)?CCM_HOLE:CCM_SRC_FILL);
	_MakeRect(tm1, tf1, B-tm1, H-tf2, pPolygon);
	pPolygon->Transfer(SCMPoint2D((HC-B)/2.0, (HC-H)/2.0));
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureCirclePipe(CCMGeom2DArray &aGeom, BOOL bOpen)
{
	double HC, BC, H, B, tw, tf1, C, tf2;
	GetSrcData(HC, BC, H, B, tw, tf1, C, tf2);

	// concrete
	CCMEllipse *pEllipse = new CCMEllipse(CCM_BORDER, CCM_SRC_FILL);
	pEllipse->SetRect(SCMPoint2D(0.0, 0.0), SCMPoint2D(HC, HC));
	aGeom.Add(pEllipse);

	// outer ring of pipe
	pEllipse = new CCMEllipse(CCM_BORDER, CCM_FILL);
	pEllipse->SetRect(SCMPoint2D(0.0, 0.0), SCMPoint2D(H, H));
	pEllipse->Transfer(SCMPoint2D((HC-H)/2.0, (HC-H)/2.0));
	aGeom.Add(pEllipse);

	// inner ring of pipe
	double R2 = H-B*2;
	pEllipse = new CCMEllipse(CCM_BORDER, (bOpen)?CCM_HOLE:CCM_SRC_FILL);
	pEllipse->SetRect(SCMPoint2D(0.0, 0.0), SCMPoint2D(R2, R2));
	pEllipse->Transfer(SCMPoint2D((HC-R2)/2.0, (HC-R2)/2.0));
	aGeom.Add(pEllipse);
}

void ICMGeomMaker::MakePictureCircleHBeam(CCMGeom2DArray &aGeom)
{
	double HC, BC, H, B, tw, tf1, C, tf2;
	GetSrcData(HC, BC, H, B, tw, tf1, C, tf2);

	// concrete
	CCMEllipse *pEllipse = new CCMEllipse(CCM_BORDER, CCM_SRC_FILL);
	pEllipse->SetRect(SCMPoint2D(0.0, 0.0), SCMPoint2D(HC, HC));
	aGeom.Add(pEllipse);

	// steel(H-Beam)
	CCMPolygon* pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeHBeam(H, B, tw, tf1, C, tf2, pPolygon);
	pPolygon->Transfer(SCMPoint2D((HC-B)/2.0, (HC-H)/2.0));
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureSrcBox(CCMGeom2DArray &aGeom)
{
	double HC, BC, H, B, tw, tf1, C, tf2;
	GetSrcData(HC, BC, H, B, tw, tf1, C, tf2);

	if(C == 0.0) 	C = B-tw;
	if(tf2 == 0.0)	tf2 = tf1;

	// steel
	CCMPolygon *pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeBox(H, B, tw, tf1, C, tf2, pPolygon);
	pPolygon->Transfer(SCMPoint2D((HC-B)/2.0, (HC-H)/2.0));
	aGeom.Add(pPolygon);

	// concrete
	double tm1 = (B-C-tw)/2.0+tw;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_SRC_FILL);
	_MakeRect(tm1, tf1, B-tm1, H-tf2, pPolygon);
	pPolygon->Transfer(SCMPoint2D((HC-B)/2.0, (HC-H)/2.0));
	aGeom.Add(pPolygon);
}

void ICMGeomMaker::MakePictureSrcPipe(CCMGeom2DArray &aGeom)
{
	double HC, BC, H, B, tw, tf1, C, tf2;
	GetSrcData(HC, BC, H, B, tw, tf1, C, tf2);
	
	// steel
	CCMEllipse *pEllipse = new CCMEllipse(CCM_BORDER, CCM_FILL);
	pEllipse->SetRect(SCMPoint2D(0.0, 0.0), SCMPoint2D(H, H));
	aGeom.Add(pEllipse);

	// concrete
	pEllipse = new CCMEllipse(CCM_BORDER, CCM_SRC_FILL);
	pEllipse->SetRect(SCMPoint2D(B, B), SCMPoint2D(H-B, H-B));
	aGeom.Add(pEllipse);
}

//-----------------------------------------------------------------------
void ICMGeomMaker::AdjustCenter(CCMGeom2D* pGeomI, CCMGeom2D* pGeomJ)
{
	SCMRect2D mbrI = pGeomI->GetMBR();
	SCMRect2D mbrJ = pGeomJ->GetMBR();
	if (mbrI.Width() > mbrJ.Width())
		pGeomJ->Transfer(SCMPoint2D((mbrI.Width()-mbrJ.Width())/2.0, 0.0));
	else
		pGeomI->Transfer(SCMPoint2D((mbrJ.Width()-mbrI.Width())/2.0, 0.0));

	if (mbrI.Height() > mbrJ.Height())
		pGeomJ->Transfer(SCMPoint2D(0.0, (mbrI.Height()-mbrJ.Height())/2.0));
	else
		pGeomI->Transfer(SCMPoint2D(0.0, (mbrJ.Height()-mbrI.Height())/2.0));
}

void ICMGeomMaker::AdjustPosition(CCMGeom2D* pGeomI, CCMGeom2D* pGeomJ)
{
	SCMRect2D mbrI = pGeomI->GetMBR();
	SCMRect2D mbrJ = pGeomJ->GetMBR();
	pGeomJ->Transfer(SCMPoint2D(
		((mbrI.Width()+mbrJ.Width())/4.0+mbrI.max.x),
	 -((mbrI.Height()+mbrJ.Height())/4.0+mbrJ.max.y)
	));
}

void ICMGeomMaker::AdjustPosition(CCMGeom2DArray& aGeomI, CCMGeom2DArray& aGeomJ)
{
	SCMRect2D mbrI = aGeomI.GetMBR();
	SCMRect2D mbrJ = aGeomJ.GetMBR();
	//aGeomJ.Transfer(SCMPoint2D(
	//  ((mbrI.Width()+mbrJ.Width())/4.0+mbrI.max.x), 
	// -((mbrI.Height()+mbrJ.Height())/4.0+mbrJ.max.y)
	//));
	double dWidth = mbrI.Width()+mbrJ.Width();
	double dHeight = mbrI.Height()+mbrJ.Height();
	double dMax = (dWidth > dHeight) ? dWidth : dHeight;
	//aGeomJ.Transfer(SCMPoint2D(dMax/4.0+mbrI.max.x, -(dMax/4.0+mbrJ.max.y)));
	aGeomJ.Transfer(SCMPoint2D(dMax/8.0+mbrI.Width(), -(dMax/4.0+mbrJ.max.y)));
}

void ICMGeomMaker::MakePictureTaperedAngle(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	CCMPolygon *pPolygonI = new CCMPolygon(CCM_SECTI_BORDER, CCM_SECTI_FILL);
	_MakeRightAngle(H, B1, tw, tf1, pPolygonI);
 
	GetRegularDataJ(H, B1, tw, tf1, B2, tf2);
	CCMPolygon *pPolygonJ = new CCMPolygon(CCM_SECTJ_BORDER, CCM_SECTJ_FILL);
	_MakeRightAngle(H, B1, tw, tf1, pPolygonJ);

	//AdjustCenter(pPolygonI, pPolygonJ);
	AdjustPosition(pPolygonI, pPolygonJ);

	aGeom.Add(pPolygonJ);
	aGeom.Add(pPolygonI);
	m_TaperedMBR = pPolygonI->GetMBR();
}

void ICMGeomMaker::MakePictureTaperedChannel(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	CCMPolygon *pPolygonI = new CCMPolygon(CCM_SECTI_BORDER, CCM_SECTI_FILL);
	_MakeRightChannel(H, B1, tw, tf1, B2, tf2, pPolygonI);

	GetRegularDataJ(H, B1, tw, tf1, B2, tf2);
	CCMPolygon *pPolygonJ = new CCMPolygon(CCM_SECTJ_BORDER, CCM_SECTJ_FILL);
	_MakeRightChannel(H, B1, tw, tf1, B2, tf2, pPolygonJ);

	//AdjustCenter(pPolygonI, pPolygonJ);
	AdjustPosition(pPolygonI, pPolygonJ);

	aGeom.Add(pPolygonJ);
	aGeom.Add(pPolygonI);
	m_TaperedMBR = pPolygonI->GetMBR();
}

void ICMGeomMaker::MakePictureTaperedHBeam(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	CCMPolygon *pPolygonI = new CCMPolygon(CCM_SECTI_BORDER, CCM_SECTI_FILL);
	_MakeHBeam(H, B1, tw, tf1, B2, tf2, pPolygonI);

	GetRegularDataJ(H, B1, tw, tf1, B2, tf2);
	CCMPolygon *pPolygonJ = new CCMPolygon(CCM_SECTJ_BORDER, CCM_SECTJ_FILL);
	_MakeHBeam(H, B1, tw, tf1, B2, tf2, pPolygonJ);

	//AdjustCenter(pPolygonI, pPolygonJ);
	AdjustPosition(pPolygonI, pPolygonJ);

	aGeom.Add(pPolygonJ);
	aGeom.Add(pPolygonI);
	m_TaperedMBR = pPolygonI->GetMBR();
}

void ICMGeomMaker::MakePictureTaperedTee(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	CCMPolygon *pPolygonI = new CCMPolygon(CCM_SECTI_BORDER, CCM_SECTI_FILL);
	_MakeTee(H, B1, tw, tf1, pPolygonI);

	GetRegularDataJ(H, B1, tw, tf1, B2, tf2);
	CCMPolygon *pPolygonJ = new CCMPolygon(CCM_SECTJ_BORDER, CCM_SECTJ_FILL);
	_MakeTee(H, B1, tw, tf1, pPolygonJ);

	//AdjustCenter(pPolygonI, pPolygonJ);
	AdjustPosition(pPolygonI, pPolygonJ);

	aGeom.Add(pPolygonJ);
	aGeom.Add(pPolygonI);
	m_TaperedMBR = pPolygonI->GetMBR();
}

void ICMGeomMaker::MakePictureTaperedBox(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	CCMGeom2DArray aGeom1, aGeom2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	if (B2 == 0.0) B2 = B1 - tw;
	if (tf2 == 0.0) tf2 = tf1;

	// Section I -----------------------------
	// outer box
	CCMPolygon *pPolygon = new CCMPolygon(CCM_SECTI_BORDER, CCM_SECTI_FILL);
	_MakeBox(H, B1, tw, tf1, B2, tf2, pPolygon);
	aGeom1.Add(pPolygon);

	// rect hole
	double tm1 = (B1-B2-tw)/2.0+tw;
	pPolygon = new CCMPolygon(CCM_SECTI_BORDER, CCM_HOLE);
	_MakeRect(tm1, tf1, B1-tm1, H-tf2, pPolygon);
	aGeom1.Add(pPolygon);

	// Section J -----------------------------
	GetRegularDataJ(H, B1, tw, tf1, B2, tf2);
	if (B2 == 0.0) B2 = B1 - tw;
	if (tf2 == 0.0) tf2 = tf1;
	// outer box
	CCMPolygon *pPolygonJ = new CCMPolygon(CCM_SECTJ_BORDER, CCM_SECTJ_FILL);
	_MakeBox(H, B1, tw, tf1, B2, tf2, pPolygonJ);
	aGeom2.Add(pPolygonJ);

	// rect hole
	tm1 = (B1-B2-tw)/2.0+tw;
	pPolygonJ = new CCMPolygon(CCM_SECTJ_BORDER, CCM_HOLE);
	_MakeRect(tm1, tf1, B1-tm1, H-tf2, pPolygonJ);
	aGeom2.Add(pPolygonJ);

	AdjustPosition(aGeom1, aGeom2);
	m_TaperedMBR = aGeom1.GetMBR();

	aGeom.AddDel(aGeom2);
	aGeom.AddDel(aGeom1);
}

void ICMGeomMaker::MakePictureTaperedPipe(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	CCMGeom2DArray aGeom1, aGeom2;

	// Section I -----------------------------
	GetRegularData(H, B1, tw, tf1, B2, tf2);

	// outer circle
	CCMEllipse *pEllipseI = new CCMEllipse(CCM_SECTI_BORDER, CCM_SECTI_FILL);
	pEllipseI->SetRect(SCMPoint2D(0.0, 0.0), SCMPoint2D(H, H));
	aGeom1.Add(pEllipseI);

	// inner circle
	pEllipseI = new CCMEllipse(CCM_SECTI_BORDER, CCM_HOLE);
	pEllipseI->SetRect(SCMPoint2D(B1, B1), SCMPoint2D(H-B1, H-B1));
	aGeom1.Add(pEllipseI);

	// Section J -----------------------------
	GetRegularDataJ(H, B1, tw, tf1, B2, tf2);

	// outer circle
	CCMEllipse *pEllipseJ = new CCMEllipse(CCM_SECTJ_BORDER, CCM_SECTJ_FILL);
	pEllipseJ->SetRect(SCMPoint2D(0.0, 0.0), SCMPoint2D(H, H));
	aGeom2.Add(pEllipseJ);

	// inner circle
	pEllipseJ = new CCMEllipse(CCM_SECTJ_BORDER, CCM_HOLE);
	pEllipseJ->SetRect(SCMPoint2D(B1, B1), SCMPoint2D(H-B1, H-B1));
	aGeom2.Add(pEllipseJ);

	AdjustPosition(aGeom1, aGeom2);
	m_TaperedMBR = aGeom1.GetMBR();

	aGeom.AddDel(aGeom2);
	aGeom.AddDel(aGeom1);
}

void ICMGeomMaker::MakePictureTaperedDblAngle(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	CCMGeom2DArray aGeom1, aGeom2;

	// Section I -----------------------------
	GetRegularData(H, B1, tw, tf1, B2, tf2);
	double ref=B1+B2;

	// left angle
	CCMPolygon *pPolygonI = new CCMPolygon(CCM_SECTI_BORDER, CCM_SECTI_FILL);
	_MakeLeftAngle(H, B1, tw, tf1, pPolygonI);
	aGeom1.Add(pPolygonI);

	// right angle
	pPolygonI = new CCMPolygon(CCM_SECTI_BORDER, CCM_SECTI_FILL);
	_MakeRightAngle(H, B1, tw, tf1, pPolygonI);
	pPolygonI->Transfer(SCMPoint2D(ref, 0.0));
	aGeom1.Add(pPolygonI);

	// Section J -----------------------------
	GetRegularDataJ(H, B1, tw, tf1, B2, tf2);
	ref=B1+B2;

	// left angle
	CCMPolygon *pPolygonJ = new CCMPolygon(CCM_SECTJ_BORDER, CCM_SECTJ_FILL);
	_MakeLeftAngle(H, B1, tw, tf1, pPolygonJ);
	aGeom2.Add(pPolygonJ);

	// right angle
	pPolygonJ = new CCMPolygon(CCM_SECTJ_BORDER, CCM_SECTJ_FILL);
	_MakeRightAngle(H, B1, tw, tf1, pPolygonJ);
	pPolygonJ->Transfer(SCMPoint2D(ref, 0.0));
	aGeom2.Add(pPolygonJ);

	AdjustPosition(aGeom1, aGeom2);
	m_TaperedMBR = aGeom1.GetMBR();

	aGeom.AddDel(aGeom2);
	aGeom.AddDel(aGeom1);
}

void ICMGeomMaker::MakePictureTaperedDblChannel(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	CCMGeom2DArray aGeom1, aGeom2;

	// Section I -----------------------------
	GetRegularData(H, B1, tw, tf1, B2, tf2);
	double ref=B1+B2;

	// left channel : keep in mind that B1 = B2, tf1 = tf2
	CCMPolygon *pPolygonI = new CCMPolygon(CCM_SECTI_BORDER, CCM_SECTI_FILL);
	_MakeLeftChannel(H, B1, tw, tf1, B1, tf1, pPolygonI);
	aGeom1.Add(pPolygonI);

	// right channel : keep in mind that B1 = B2, tf1 = tf2
	pPolygonI = new CCMPolygon(CCM_SECTI_BORDER, CCM_SECTI_FILL);
	_MakeRightChannel(H, B1, tw, tf1, B1, tf1, pPolygonI);
	pPolygonI->Transfer(SCMPoint2D(ref, 0.0));
	aGeom1.Add(pPolygonI);

	// Section J -----------------------------
	GetRegularDataJ(H, B1, tw, tf1, B2, tf2);
	ref=B1+B2;

	// left channel : keep in mind that B1 = B2, tf1 = tf2
	CCMPolygon *pPolygonJ = new CCMPolygon(CCM_SECTJ_BORDER, CCM_SECTJ_FILL);
	_MakeLeftChannel(H, B1, tw, tf1, B1, tf1, pPolygonJ);
	aGeom2.Add(pPolygonJ);

	// right channel : keep in mind that B1 = B2, tf1 = tf2
	pPolygonJ = new CCMPolygon(CCM_SECTJ_BORDER, CCM_SECTJ_FILL);
	_MakeRightChannel(H, B1, tw, tf1, B1, tf1, pPolygonJ);
	pPolygonJ->Transfer(SCMPoint2D(ref, 0.0));
	aGeom2.Add(pPolygonJ);

	AdjustPosition(aGeom1, aGeom2);
	m_TaperedMBR = aGeom1.GetMBR();

	aGeom.AddDel(aGeom2);
	aGeom.AddDel(aGeom1);
}

void ICMGeomMaker::MakePictureTaperedSolidRect(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	CCMPolygon *pPolygonI = new CCMPolygon(CCM_SECTI_BORDER, CCM_SECTI_FILL);
	_MakeRect(0.0, 0.0, B1, H, pPolygonI);

	GetRegularDataJ(H, B1, tw, tf1, B2, tf2);
	CCMPolygon *pPolygonJ = new CCMPolygon(CCM_SECTJ_BORDER, CCM_SECTJ_FILL);
	_MakeRect(0.0, 0.0, B1, H, pPolygonJ);

	//AdjustCenter(pPolygonI, pPolygonJ);
	AdjustPosition(pPolygonI, pPolygonJ);

	aGeom.Add(pPolygonJ);
	aGeom.Add(pPolygonI);
	m_TaperedMBR = pPolygonI->GetMBR();
}

void ICMGeomMaker::MakePictureTaperedSolidCircle(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	CCMEllipse *pEllipseI = new CCMEllipse(CCM_SECTI_BORDER, CCM_SECTI_FILL);
	pEllipseI->SetRect(SCMPoint2D(0.0, 0.0), SCMPoint2D(H, H));

	GetRegularDataJ(H, B1, tw, tf1, B2, tf2);
	CCMEllipse *pEllipseJ = new CCMEllipse(CCM_SECTJ_BORDER, CCM_SECTJ_FILL);
	pEllipseJ->SetRect(SCMPoint2D(0.0, 0.0), SCMPoint2D(H, H));

	//AdjustCenter(pEllipseI, pEllipseJ);
	AdjustPosition(pEllipseI, pEllipseJ);

	aGeom.Add(pEllipseJ);
	aGeom.Add(pEllipseI);
	m_TaperedMBR = pEllipseI->GetMBR();
}

void ICMGeomMaker::MakePictureTaperedOctagon(CCMGeom2DArray &aGeom)
{
	double H, B, a, b, t, t1;
	CCMGeom2DArray aGeom1, aGeom2;

	// Section I -----------------------------
	GetRegularData(H, B, a, b, t, t1);

	CCMPolygon *pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureOctagon(H, B, a, b, pPolygon);
	aGeom1.Add(pPolygon);

	double a1, b1;
	if (a == 0.0 || b == 0.0) { a1 = 0.0; b1 = 0.0; }
	else
	{
		double c = b + (t/a)*sqrt(a*a+b*b);
		a1 = (a/b)*(c-t)-t;
		b1 = -(b/a)*t+c-t;
	}
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_HOLE);
	_MakePictureOctagon(H-2*t, B-2*t, a1, b1, pPolygon);
	aGeom1.Add(pPolygon);

	// Section J -----------------------------
	GetRegularDataJ(H, B, a, b, t, t1);

	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureOctagon(H, B, a, b, pPolygon);
	aGeom2.Add(pPolygon);

	if (a == 0.0 || b == 0.0) { a1 = 0.0; b1 = 0.0; }
	else
	{
		double c = b + (t/a)*sqrt(a*a+b*b);
		a1 = (a/b)*(c-t)-t;
		b1 = -(b/a)*t+c-t;
	}
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_HOLE);
	_MakePictureOctagon(H-2*t, B-2*t, a1, b1, pPolygon);
	aGeom2.Add(pPolygon);

	// ---------------------------------------
	AdjustPosition(aGeom1, aGeom2);
	m_TaperedMBR = aGeom1.GetMBR();

	aGeom.AddDel(aGeom2);
	aGeom.AddDel(aGeom1);
}

void ICMGeomMaker::MakePictureTaperedSolidOctagon(CCMGeom2DArray &aGeom)
{
	double H, B, a, b, t1, t2;
	CCMGeom2DArray aGeom1, aGeom2;

	// Section I -----------------------------
	GetRegularData(H, B, a, b, t1, t2);

	CCMPolygon *pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureOctagon(H, B, a, b, pPolygon);
	aGeom1.Add(pPolygon);

	// Section J -----------------------------
	GetRegularDataJ(H, B, a, b, t1, t2);

	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureOctagon(H, B, a, b, pPolygon);
	aGeom2.Add(pPolygon);

	// ---------------------------------------
	AdjustPosition(aGeom1, aGeom2);
	m_TaperedMBR = aGeom1.GetMBR();

	aGeom.AddDel(aGeom2);
	aGeom.AddDel(aGeom1);
}

void ICMGeomMaker::MakePictureTaperedTrack(CCMGeom2DArray &aGeom)
{
	double H, B, t, t1, t2, t3;
	CCMGeom2DArray aGeom1, aGeom2;

	// Section I -----------------------------
	GetRegularData(H, B, t, t1, t2, t3);

	CCMPolygon *pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureTrack(H, B-H, pPolygon);
	aGeom1.Add(pPolygon);

	pPolygon = new CCMPolygon(CCM_BORDER, CCM_HOLE);
	_MakePictureTrack(H-2*t, B-H, pPolygon);
	aGeom1.Add(pPolygon);

	// Section J -----------------------------
	GetRegularDataJ(H, B, t, t1, t2, t3);

	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureTrack(H, B-H, pPolygon);
	aGeom2.Add(pPolygon);

	pPolygon = new CCMPolygon(CCM_BORDER, CCM_HOLE);
	_MakePictureTrack(H-2*t, B-H, pPolygon);
	aGeom2.Add(pPolygon);

	// ---------------------------------------
	AdjustPosition(aGeom1, aGeom2);
	m_TaperedMBR = aGeom1.GetMBR();

	aGeom.AddDel(aGeom2);
	aGeom.AddDel(aGeom1);
}

void ICMGeomMaker::MakePictureTaperedSolidTrack(CCMGeom2DArray &aGeom)
{
	double H, B, t0, t1, t2, t3;
	CCMGeom2DArray aGeom1, aGeom2;

	// Section I -----------------------------
	GetRegularData(H, B, t0, t1, t2, t3);

	CCMPolygon *pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureTrack(H, B-H, pPolygon);
	aGeom1.Add(pPolygon);

	// Section J -----------------------------
	GetRegularDataJ(H, B, t0, t1, t2, t3);

	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureTrack(H, B-H, pPolygon);
	aGeom2.Add(pPolygon);

	// ---------------------------------------
	AdjustPosition(aGeom1, aGeom2);
	m_TaperedMBR = aGeom1.GetMBR();

	aGeom.AddDel(aGeom2);
	aGeom.AddDel(aGeom1);
}

void ICMGeomMaker::MakePictureTaperedHalfTrack(CCMGeom2DArray &aGeom)
{
	double H, B, t0, t1, t2, t3;
	CCMGeom2DArray aGeom1, aGeom2;

	// Section I -----------------------------
	GetRegularData(H, B, t0, t1, t2, t3);

	CCMPolygon *pPolygon;
	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureHalfTrack(H, B-H/2.0, pPolygon);
	aGeom1.Add(pPolygon);

	// Section J -----------------------------
	GetRegularDataJ(H, B, t0, t1, t2, t3);

	pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakePictureHalfTrack(H, B-H/2.0, pPolygon);
	aGeom2.Add(pPolygon);

	// ---------------------------------------
	AdjustPosition(aGeom1, aGeom2);
	m_TaperedMBR = aGeom1.GetMBR();

	aGeom.AddDel(aGeom2);
	aGeom.AddDel(aGeom1);
}

void ICMGeomMaker::MakePictureTaperedPsc(T_SECT_D& rTaperedSectD, CCMGeom2DArray &aGeom)
{
	CCMGeom2DArray aGeom1, aGeom2;
	T_SECT_D Sect;

	// I를 PSC로 만들어 MakePicturePSC 호출
	// Section I -----------------------------
	CSectUtil::GetSectFromTapPsc(rTaperedSectD, Sect, TRUE);
	MakePicturePSC(Sect, aGeom1);

	// Section J -----------------------------
	CSectUtil::GetSectFromTapPsc(rTaperedSectD, Sect, FALSE);
	MakePicturePSC(Sect, aGeom2);

	AdjustPosition(aGeom1, aGeom2);
	m_TaperedMBR = aGeom1.GetMBR();

	aGeom.AddDel(aGeom2);
	aGeom.AddDel(aGeom1);
}

void ICMGeomMaker::MakePictureTaperedSteelBox(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2])
{
	CCMGeom2DArray aGeomI, aGeomJ;
	aGeomI.RemoveAll(), aGeomJ.RemoveAll();
	
	// Section I -----------------------------
	MakePictureCompoB(aGeomI, aGeomPart1, aGeomPart1, FALSE);

	// Section J -----------------------------
	MakePictureCompoB(aGeomJ, aGeomPart1, aGeomPart1, TRUE);

	AdjustPosition(aGeomI, aGeomJ);
	m_TaperedMBR = aGeomI.GetMBR();

	aGeom.AddDel(aGeomJ);
	aGeom.AddDel(aGeomI);
}
void ICMGeomMaker::MakePictureTaperedSteelI(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2])
{
	CCMGeom2DArray aGeomI, aGeomJ;
	aGeomI.RemoveAll(), aGeomJ.RemoveAll();
	
	// Section I -----------------------------
	MakePictureCompoI(aGeomI, aGeomPart1, aGeomPart1, FALSE);

	// Section J -----------------------------
	MakePictureCompoI(aGeomJ, aGeomPart1, aGeomPart1, TRUE);

	AdjustPosition(aGeomI, aGeomJ);
	m_TaperedMBR = aGeomI.GetMBR();

	aGeom.AddDel(aGeomJ);
	aGeom.AddDel(aGeomI);
}
void ICMGeomMaker::MakePictureTaperedSteelTub(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2])
{
	CCMGeom2DArray aGeomI, aGeomJ;
	aGeomI.RemoveAll(), aGeomJ.RemoveAll();
	
	// Section I -----------------------------
	MakePictureCompoTub(aGeomI, aGeomPart1, aGeomPart1, FALSE);
	
	// Section J -----------------------------
	MakePictureCompoTub(aGeomJ, aGeomPart1, aGeomPart1, TRUE);
	
	AdjustPosition(aGeomI, aGeomJ);
	m_TaperedMBR = aGeomI.GetMBR();
	
	aGeom.AddDel(aGeomJ);
	aGeom.AddDel(aGeomI);
}
void ICMGeomMaker::MakePictureTaperedStlGirderBox(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2])
{
	CCMGeom2DArray aGeomI, aGeomJ;
	aGeomI.RemoveAll(), aGeomJ.RemoveAll();

	// Section I -----------------------------
	MakePictureCompoStlGirderBox(aGeomI, aGeomPart1, aGeomPart1, FALSE);

	// Section J -----------------------------
	MakePictureCompoStlGirderBox(aGeomJ, aGeomPart1, aGeomPart1, TRUE);

	AdjustPosition(aGeomI, aGeomJ);
	m_TaperedMBR = aGeomI.GetMBR();

	aGeom.AddDel(aGeomJ);
	aGeom.AddDel(aGeomI);
}
void ICMGeomMaker::MakePictureTaperedStlGirderI(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2])
{
	CCMGeom2DArray aGeomI, aGeomJ;
	aGeomI.RemoveAll(), aGeomJ.RemoveAll();

	// Section I -----------------------------
	MakePictureCompoStlGirderI(aGeomI, aGeomPart1, aGeomPart1, FALSE);

	// Section J -----------------------------
	MakePictureCompoStlGirderI(aGeomJ, aGeomPart1, aGeomPart1, TRUE);

	AdjustPosition(aGeomI, aGeomJ);
	m_TaperedMBR = aGeomI.GetMBR();

	aGeom.AddDel(aGeomJ);
	aGeom.AddDel(aGeomI);
}
void ICMGeomMaker::MakePictureTaperedStlGirderTub(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2])
{
	CCMGeom2DArray aGeomI, aGeomJ;
	aGeomI.RemoveAll(), aGeomJ.RemoveAll();

	// Section I -----------------------------
	MakePictureCompoStlGirderTub(aGeomI, aGeomPart1, aGeomPart1, FALSE);

	// Section J -----------------------------
	MakePictureCompoStlGirderTub(aGeomJ, aGeomPart1, aGeomPart1, TRUE);

	AdjustPosition(aGeomI, aGeomJ);
	m_TaperedMBR = aGeomI.GetMBR();

	aGeom.AddDel(aGeomJ);
	aGeom.AddDel(aGeomI);
}
void ICMGeomMaker::MakePictureTaperedConcreteI(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2])
{
	CCMGeom2DArray aGeomI, aGeomJ;
	aGeomI.RemoveAll(), aGeomJ.RemoveAll();

	// Section I -----------------------------
	MakePictureCompoCI(aGeomI, aGeomPart1, aGeomPart2, FALSE);
	// Section J -----------------------------
	MakePictureCompoCI(aGeomJ, aGeomPart1, aGeomPart2, TRUE);

	AdjustPosition(aGeomI, aGeomJ);
	m_TaperedMBR = aGeomI.GetMBR();

	aGeom.AddDel(aGeomJ);
	aGeom.AddDel(aGeomI);
}
void ICMGeomMaker::MakePictureTaperedConcreteT(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2])
{
	CCMGeom2DArray aGeomI, aGeomJ;
	aGeomI.RemoveAll(), aGeomJ.RemoveAll();

	// Section I -----------------------------
	MakePictureCompoCT(aGeomI, aGeomPart1, aGeomPart2, FALSE);
	// Section J -----------------------------
	MakePictureCompoCT(aGeomJ, aGeomPart1, aGeomPart2, TRUE);

	AdjustPosition(aGeomI, aGeomJ);
	m_TaperedMBR = aGeomI.GetMBR();

	aGeom.AddDel(aGeomJ);
	aGeom.AddDel(aGeomI);
}

void ICMGeomMaker::MakePictureTaperedStlGirderBox(CCMGeom2DArray &aGeom)
{
	CCMGeom2DArray aGeomI, aGeomJ;
	aGeomI.RemoveAll(), aGeomJ.RemoveAll();

	// Section I -----------------------------
	MakePictureStlGirderBox(aGeomI, FALSE);

	// Section J -----------------------------
	MakePictureStlGirderBox(aGeomJ, TRUE);

	AdjustPosition(aGeomI, aGeomJ);
	m_TaperedMBR = aGeomI.GetMBR();

	aGeom.AddDel(aGeomJ);
	aGeom.AddDel(aGeomI);
}

void ICMGeomMaker::MakePictureTaperedStlGirderI(CCMGeom2DArray &aGeom)
{
	CCMGeom2DArray aGeomI, aGeomJ;
	aGeomI.RemoveAll(), aGeomJ.RemoveAll();

	// Section I -----------------------------
	MakePictureStlGirderI(aGeomI, FALSE);

	// Section J -----------------------------
	MakePictureStlGirderI(aGeomJ, TRUE);

	AdjustPosition(aGeomI, aGeomJ);
	m_TaperedMBR = aGeomI.GetMBR();

	aGeom.AddDel(aGeomJ);
	aGeom.AddDel(aGeomI);
}

void ICMGeomMaker::MakePictureTaperedStlGirderMCell(CCMGeom2DArray &aGeom)
{
	CCMGeom2DArray aGeomI, aGeomJ;
	aGeomI.RemoveAll(), aGeomJ.RemoveAll();

	// Section I -----------------------------
	MakePictureStlGirderMCell(aGeomI, FALSE);

	// Section J -----------------------------
	MakePictureStlGirderMCell(aGeomJ, TRUE);

	AdjustPosition(aGeomI, aGeomJ);
	m_TaperedMBR = aGeomI.GetMBR();

	aGeom.AddDel(aGeomJ);
	aGeom.AddDel(aGeomI);
}

//-----------------------------------------------------------------------
void ICMGeomMaker::MakePicture2H(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	double _H, _B1, _tw, _tf1, _B2, _tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	B2 = B1; tf2 = tf1;
	CCMPolygon *pPolygonF = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeHBeam(H, B1, tw, tf1, B2, tf2, pPolygonF);

	GetRegularDataS(_H, _B1, _tw, _tf1, _B2, _tf2);
	_B2 = _B1; _tf2 = _tf1;
	CCMPolygon *pPolygonS = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeHBeam(_H, _B1, _tw, _tf1, _B2, _tf2, pPolygonS);

	pPolygonS->Rotate(90.0);
	SCMRect2D mbrS = pPolygonS->GetMBR();
	pPolygonS->Transfer(SCMPoint2D(-mbrS.min.x, -mbrS.min.y));
	pPolygonF->Transfer(SCMPoint2D(_H/2.0-B1/2.0, -(H-(_B1/2.0-_tw/2.0))));

	aGeom.Add(pPolygonF);
	aGeom.Add(pPolygonS);
}

void ICMGeomMaker::MakePictureHC1(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	double _H, _B1, _tw, _tf1, _B2, _tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	B2 = B1; tf2 = tf1;
	CCMPolygon *pPolygonF = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeHBeam(H, B1, tw, tf1, B2, tf2, pPolygonF);

	GetRegularDataS(_H, _B1, _tw, _tf1, _B2, _tf2);
	_B2 = _B1; _tf2 = _tf1;
	CCMPolygon *pPolygonS = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeLeftChannel(_H, _B1, _tw, _tf1, _B2, _tf2, pPolygonS);

	pPolygonS->Rotate(270.0);

	SCMRect2D mbrS = pPolygonS->GetMBR();    
	pPolygonS->Transfer(SCMPoint2D(-mbrS.min.x, -mbrS.min.y));
	pPolygonF->Transfer(SCMPoint2D(_H/2.0-B1/2.0, _tw));
//   if(B1 < _H)
//   {
//     pPolygonS->Transfer(SCMPoint2D(-mbrS.min.x, -mbrS.min.y));
//     pPolygonF->Transfer(SCMPoint2D(_H/2.0-B1/2.0, _tw));
//   }
//   else
//   {
//     SCMRect2D mbrF = pPolygonF->GetMBR();   
//     pPolygonS->Transfer(SCMPoint2D(B1/2.0-_H/2.0, -mbrS.min.y));
//     pPolygonF->Transfer(SCMPoint2D(-mbrF.min.x, _tw));
//   }

	aGeom.Add(pPolygonF);
	aGeom.Add(pPolygonS);
}

void ICMGeomMaker::MakePictureHC2(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	double _H, _B1, _tw, _tf1, _B2, _tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	B2 = B1; tf2 = tf1;
	CCMPolygon *pPolygonF = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeHBeam(H, B1, tw, tf1, B2, tf2, pPolygonF);

	GetRegularDataS(_H, _B1, _tw, _tf1, _B2, _tf2);
	_B2 = _B1; _tf2 = _tf1;
	CCMPolygon *pPolygonS = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeLeftChannel(_H, _B1, _tw, _tf1, _B2, _tf2, pPolygonS);

	pPolygonS->Rotate(90.0);

	SCMRect2D mbrS = pPolygonS->GetMBR();
	pPolygonS->Transfer(SCMPoint2D(-mbrS.min.x, -mbrS.min.y));
	pPolygonF->Transfer(SCMPoint2D(_H/2.0-B1/2.0, _B1));

	aGeom.Add(pPolygonF);
	aGeom.Add(pPolygonS);
}

void ICMGeomMaker::MakePictureHT(CCMGeom2DArray &aGeom) 
{
	double H, B1, tw, tf1, B2, tf2;
	double _H, _B1, _tw, _tf1, _B2, _tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	CCMPolygon *pPolygonF = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeTee(H, B1, tw, tf1, pPolygonF);

	GetRegularDataS(_H, _B1, _tw, _tf1, _B2, _tf2);
	_B2 = _B1; _tf2 = _tf1;
	CCMPolygon *pPolygonS = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeHBeam(_H, _B1, _tw, _tf1, _B2, _tf2, pPolygonS);

	pPolygonS->Rotate(90.0);
	SCMRect2D mbrS = pPolygonS->GetMBR();
	pPolygonS->Transfer(SCMPoint2D(-mbrS.min.x, -mbrS.min.y));
	pPolygonF->Transfer(SCMPoint2D(_H/2.0-B1/2.0, -(H-(_B1/2.0-_tw/2.0))));

	aGeom.Add(pPolygonF);
	aGeom.Add(pPolygonS);
}

void ICMGeomMaker::MakePicutre2T1(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	double _H, _B1, _tw, _tf1, _B2, _tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	CCMPolygon *pPolygonF = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeTee(H, B1, tw, tf1, pPolygonF);

	GetRegularDataS(_H, _B1, _tw, _tf1, _B2, _tf2);
	_B2 = _B1; _tf2 = _tf1;
	CCMPolygon *pPolygonS = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeTee(_H, _B1, _tw, _tf1, pPolygonS);

	pPolygonS->Rotate(180.0);
	SCMRect2D mbrS = pPolygonS->GetMBR();
	pPolygonS->Transfer(SCMPoint2D(-mbrS.min.x, -mbrS.min.y));
	pPolygonF->Transfer(SCMPoint2D(_B1/2.0-B1/2.0, -(H+B2)));

	aGeom.Add(pPolygonF);
	aGeom.Add(pPolygonS);
}

void ICMGeomMaker::MakePicture2T2(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	double _H, _B1, _tw, _tf1, _B2, _tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	CCMPolygon *pPolygonF = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeTee(H, B1, tw, tf1, pPolygonF);

	GetRegularDataS(_H, _B1, _tw, _tf1, _B2, _tf2);
	_B2 = _B1; _tf2 = _tf1;
	CCMPolygon *pPolygonS = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeTee(_H, _B1, _tw, _tf1, pPolygonS);

	pPolygonF->Rotate(180.0);
	SCMRect2D mbrF = pPolygonF->GetMBR();
	pPolygonF->Transfer(SCMPoint2D(-mbrF.min.x, -mbrF.min.y));
	pPolygonS->Transfer(SCMPoint2D(B1/2.0-_B1/2.0, (H+B2)));

	aGeom.Add(pPolygonF);
	aGeom.Add(pPolygonS);
}

void ICMGeomMaker::MakePictureH2T(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	double _H, _B1, _tw, _tf1, _B2, _tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	B2 = B1; tf2 = tf1;
	CCMPolygon *pPolygonF = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeHBeam(H, B1, tw, tf1, B2, tf2, pPolygonF);

	GetRegularDataS(_H, _B1, _tw, _tf1, _B2, _tf2);
	CCMPolygon *pPolygonS = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeTee(_H, _B1, _tw, _tf1, pPolygonS);

	CCMPolygon *pPolygonT = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeTee(_H, _B1, _tw, _tf1, pPolygonT);

	pPolygonS->Rotate(90.0);
	pPolygonT->Rotate(270.0);

	SCMRect2D mbrS = pPolygonS->GetMBR();
	pPolygonS->Transfer(SCMPoint2D(-mbrS.min.x, -mbrS.min.y));
	pPolygonS->Transfer(SCMPoint2D(B1/2.0+tw/2.0, H/2.0-_B1/2.0));
	SCMRect2D mbrT = pPolygonT->GetMBR();
	pPolygonT->Transfer(SCMPoint2D(-mbrT.min.x, -mbrT.min.y));
	pPolygonT->Transfer(SCMPoint2D(B1/2.0-tw/2.0-_H, H/2.0-_B1/2.0));

	aGeom.Add(pPolygonF);
	aGeom.Add(pPolygonS);
	aGeom.Add(pPolygonT);
}

void ICMGeomMaker::MakePictureWOH(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2, C;
	double _H, _B1, _tw, _tf1, _B2, _tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	C = B2; B2 = B1; tf2 = tf1;
	CCMPolygon *pPolygonF = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeHBeam(H, B1, tw, tf1, B2, tf2, pPolygonF);

	GetRegularDataS(_H, _B1, _tw, _tf1, _B2, _tf2);
	_B2 = _B1; _tf2 = _tf1;
	CCMPolygon *pPolygonS = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeHBeam(_H, _B1, _tw, _tf1, _B2, _tf2, pPolygonS);

	pPolygonS->Transfer(SCMPoint2D(B1/2.0-_B1/2.0, H+C));

	aGeom.Add(pPolygonF);
	aGeom.Add(pPolygonS);
}


void ICMGeomMaker::MakePictureFOH(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2, C;
	double _H, _B1, _tw, _tf1, _B2, _tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	C = B2; B2 = B1; tf2 = tf1;
	CCMPolygon *pPolygonF = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeIBeam(H, B1, tw, tf1, B2, tf2, pPolygonF);

	GetRegularDataS(_H, _B1, _tw, _tf1, _B2, _tf2);
	_B2 = _B1; _tf2 = _tf1;
	CCMPolygon *pPolygonS = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeIBeam(_H, _B1, _tw, _tf1, _B2, _tf2, pPolygonS);

	pPolygonS->Transfer(SCMPoint2D(_H/2-H/2, B1+C));

	aGeom.Add(pPolygonF);
	aGeom.Add(pPolygonS);
}

//-----------------------------------------------------------------------
void ICMGeomMaker::MakePicture2CW1(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	double _H, _B1, _tw, _tf1, _B2, _tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	B2 = B1; tf2 = tf1;
	CCMPolygon *pPolygonF = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeLeftChannel(H, B1, tw, tf1, B2, tf2, pPolygonF);

	CCMPolygon *pPolygonS = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeLeftChannel(H, B1, tw, tf1, B2, tf2, pPolygonS);

	GetRegularDataS(_H, _B1, _tw, _tf1, _B2, _tf2);
	CCMPolygon *pPolygonT = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeRect(0.0, 0.0, _B1, _H, pPolygonT);

	pPolygonF->Rotate(90.0);
	pPolygonS->Rotate(270.0);
	SCMRect2D mbr;
	mbr = pPolygonF->GetMBR();
	pPolygonF->Transfer(SCMPoint2D(-mbr.min.x, -mbr.min.y));
	mbr = pPolygonS->GetMBR();
	pPolygonS->Transfer(SCMPoint2D(-mbr.min.x, -mbr.min.y));

	pPolygonT->Transfer(SCMPoint2D(H/2.0-_B1/2.0, B1));
	pPolygonS->Transfer(SCMPoint2D(0.0, B1+_H));

	aGeom.Add(pPolygonF);
	aGeom.Add(pPolygonS);
	aGeom.Add(pPolygonT);
}

void ICMGeomMaker::MakePicture2CW2(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	double _H, _B1, _tw, _tf1, _B2, _tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	B2 = B1; tf2 = tf1;
	CCMPolygon *pPolygonF = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeLeftChannel(H, B1, tw, tf1, B2, tf2, pPolygonF);

	CCMPolygon *pPolygonS = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeLeftChannel(H, B1, tw, tf1, B2, tf2, pPolygonS);

	GetRegularDataS(_H, _B1, _tw, _tf1, _B2, _tf2);
	CCMPolygon *pPolygonT = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeRect(0.0, 0.0, _B1, _H, pPolygonT);

	pPolygonF->Rotate(270.0);
	pPolygonS->Rotate(90.0);
	SCMRect2D mbr;
	mbr = pPolygonF->GetMBR();
	pPolygonF->Transfer(SCMPoint2D(-mbr.min.x, -mbr.min.y));
	mbr = pPolygonS->GetMBR();
	pPolygonS->Transfer(SCMPoint2D(-mbr.min.x, -mbr.min.y));

	pPolygonT->Transfer(SCMPoint2D(H/2.0-_B1/2.0, tw));
	pPolygonS->Transfer(SCMPoint2D(0.0, _H+2*tw-B1));

	aGeom.Add(pPolygonF);
	aGeom.Add(pPolygonS);
	aGeom.Add(pPolygonT);
}

void ICMGeomMaker::MakePicture1CW1(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	double _H, _B1, _tw, _tf1, _B2, _tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	B2 = B1; tf2 = tf1;
	CCMPolygon *pPolygonF = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeLeftChannel(H, B1, tw, tf1, B2, tf2, pPolygonF);

	GetRegularDataS(_H, _B1, _tw, _tf1, _B2, _tf2);
	CCMPolygon *pPolygonT = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeRect(0.0, 0.0, _B1, _H, pPolygonT);

	pPolygonF->Rotate(270.0);
	SCMRect2D mbr;
	mbr = pPolygonF->GetMBR();
	pPolygonF->Transfer(SCMPoint2D(-mbr.min.x, -mbr.min.y));

	pPolygonT->Transfer(SCMPoint2D(H/2.0-_B1/2.0, tw));

	aGeom.Add(pPolygonF);
	aGeom.Add(pPolygonT);
}

void ICMGeomMaker::MakePicture1CW2(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	double _H, _B1, _tw, _tf1, _B2, _tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	B2 = B1; tf2 = tf1;
	CCMPolygon *pPolygonF = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeLeftChannel(H, B1, tw, tf1, B2, tf2, pPolygonF);

	GetRegularDataS(_H, _B1, _tw, _tf1, _B2, _tf2);
	CCMPolygon *pPolygonT = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeRect(0.0, 0.0, _B1, _H, pPolygonT);

	pPolygonF->Rotate(90.0);
	SCMRect2D mbr;
	mbr = pPolygonF->GetMBR();
	pPolygonF->Transfer(SCMPoint2D(-mbr.min.x, -mbr.min.y));

	pPolygonT->Transfer(SCMPoint2D(H/2.0-_B1/2.0, B1));

	aGeom.Add(pPolygonF);
	aGeom.Add(pPolygonT);
}

void ICMGeomMaker::MakePicture4L(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	double _H, _B1, _tw, _tf1, _B2, _tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	CCMPolygon *pPolygonF = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeRightAngle(H, B1, tw, tf1, pPolygonF);
	CCMPolygon *pPolygonS = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeLeftAngle(H, B1, tw, tf1, pPolygonS);

	CCMPolygon *pPolygonF2 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeRightAngle(H, B1, tw, tf1, pPolygonF2);
	CCMPolygon *pPolygonS2 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeLeftAngle(H, B1, tw, tf1, pPolygonS2);

	GetRegularDataS(_H, _B1, _tw, _tf1, _B2, _tf2);

	SCMRect2D mbr;
	pPolygonF2->Mirror(0);  // x axis mirror
	pPolygonS2->Mirror(0);  // x axis mirror
	mbr = pPolygonF2->GetMBR();
	pPolygonF2->Transfer(SCMPoint2D(-mbr.min.x, -mbr.min.y));
	mbr = pPolygonS2->GetMBR();
	pPolygonS2->Transfer(SCMPoint2D(-mbr.min.x, -mbr.min.y));

	pPolygonF->Transfer(SCMPoint2D(B1+_H, H+_H));
	pPolygonS->Transfer(SCMPoint2D(0.0, H+_H));
	pPolygonF2->Transfer(SCMPoint2D(B1+_H, 0.0));

	aGeom.Add(pPolygonF);
	aGeom.Add(pPolygonS);
	aGeom.Add(pPolygonF2);
	aGeom.Add(pPolygonS2);
}

void ICMGeomMaker::MakePictureHP(CCMGeom2DArray &aGeom)
{
	double H, B1, tw, tf1, B2, tf2;
	double _H, _B1, _tw, _tf1, _B2, _tf2;

	GetRegularData(H, B1, tw, tf1, B2, tf2);
	B2 = B1; tf2 = tf1;
	CCMPolygon *pPolygonF = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeHBeam(H, B1, tw, tf1, B2, tf2, pPolygonF);

	GetRegularDataS(_H, _B1, _tw, _tf1, _B2, _tf2);
	CCMPolygon *pPolygonS = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeRect(0.0, 0.0, _B1, H-tf1*2, pPolygonS);
	CCMPolygon *pPolygonT = new CCMPolygon(CCM_BORDER, CCM_FILL);
	_MakeRect(0.0, 0.0, _B1, H-tf1*2, pPolygonT);

	pPolygonS->Transfer(SCMPoint2D((B1-_H-_B1)/2.0, tf1));
	pPolygonT->Transfer(SCMPoint2D((B1+_H-_B1)/2.0, tf1));

	aGeom.Add(pPolygonF);
	aGeom.Add(pPolygonS);
	aGeom.Add(pPolygonT);
}

void ICMGeomMaker::MakePictureCompoB(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ)
{
	CCMPolygon *pPolygon1 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon2 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon3 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon4 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon5 = new CCMPolygon(CCM_BORDER, CCM_FILL);

	double SW, CTC, Bc, tc, Hh, Hw, tw, B1, Bf1, tf1, B2, Bf2, tf2, Hr1, Hr2, tr1, tr2, E, D;
	int    GNum, N1, N2;
	double twp = 0.0;
	double theta = 0.0;
	double dy_org, dz_org;
	double dSpaing;

	// read ///////////////////////////////////////
	SW   = m_pDataSrc->SectAfter.SectI.Size[0];
	CTC  = m_pDataSrc->SectAfter.SectI.Size[1];
	GNum = m_pDataSrc->SectAfter.SectI.BuiltUpFlag;

	Bc  = m_pDataSrc->SectAfter.SectJ.Size[0];
	tc  = m_pDataSrc->SectAfter.SectJ.Size[1];
	Hh  = m_pDataSrc->SectAfter.SectJ.Size[2];

	BOOL bTapJ = (m_pDataSrc->nStype==D_SECT_TYPE_TAPERED && bJ);
	if(bTapJ)
	{
		Hw  = m_pDataSrc->CmpTapJ.Size[0];
		tw  = m_pDataSrc->CmpTapJ.Size[1];
		B1  = m_pDataSrc->CmpTapJ.Size[2];
		Bf1 = m_pDataSrc->CmpTapJ.Size[3];
		tf1 = m_pDataSrc->CmpTapJ.Size[4];
		B2  = m_pDataSrc->CmpTapJ.Size[5];
		Bf2 = m_pDataSrc->CmpTapJ.Size[6];
		tf2 = m_pDataSrc->CmpTapJ.Size[7];
	}
	else
	{
		Hw  = m_pDataSrc->SectBefore.SectI.Size[0];
		tw  = m_pDataSrc->SectBefore.SectI.Size[1];
		B1  = m_pDataSrc->SectBefore.SectI.Size[2];
		Bf1 = m_pDataSrc->SectBefore.SectI.Size[3];
		tf1 = m_pDataSrc->SectBefore.SectI.Size[4];
		B2  = m_pDataSrc->SectBefore.SectI.Size[5];
		Bf2 = m_pDataSrc->SectBefore.SectI.Size[6];
		tf2 = m_pDataSrc->SectBefore.SectI.Size[7];
	}

	N1  = (double)m_pDataSrc->SectBefore.nStiffNum[0];//.nLytype;
	N2  = (double)m_pDataSrc->SectBefore.nStiffNum[1];//.nLztype;
	Hr1 = m_pDataSrc->SectBefore.SectJ.Size[2];
	Hr2 = m_pDataSrc->SectBefore.SectJ.Size[3];
	tr1 = m_pDataSrc->SectBefore.SectJ.Size[4];
	tr2 = m_pDataSrc->SectBefore.SectJ.Size[5];
	E   = m_pDataSrc->SectBefore.Matl_Elast;
	D   = m_pDataSrc->SectBefore.Matl_Density;
	///////////////////////////////////////////////


	// draw ///////////////////////////////////////
	double  leng[3], tempx, tempy, tempx1, tempy1;
	leng[0] = Bc;
	leng[1] = Bf1 + B1 + Bf1;
	leng[2] = Bf2 + B2 + Bf2;

	tempx = 0;
	tempy = 0;
	pPolygon1->Add(SCMPoint2D(tempx, tempy));
	pPolygon1->Add(SCMPoint2D(tempx+leng[0], tempy));
	pPolygon1->Add(SCMPoint2D(tempx+leng[0], tc));
	pPolygon1->Add(SCMPoint2D(tempx, tc));
	if(bTapJ) aGeomPart2[1].Add(pPolygon1); // slab
	else      aGeomPart2[0].Add(pPolygon1); // slab
	
	tempx = (leng[0] - leng[1]) * 0.5;
	tempy = tc + Hh;
	pPolygon2->Add(SCMPoint2D(tempx, tempy));
	pPolygon2->Add(SCMPoint2D(tempx+leng[1], tempy));
	pPolygon2->Add(SCMPoint2D(tempx+leng[1], tempy+tf1));
	pPolygon2->Add(SCMPoint2D(tempx, tempy+tf1));
	if(bTapJ) aGeomPart1[1].Add(pPolygon2); // girder
	else      aGeomPart1[0].Add(pPolygon2); // girder
	
	tempx = (leng[0] - leng[2]) * 0.5;
	tempy = tc + Hh + tf1 + Hw;
	pPolygon3->Add(SCMPoint2D(tempx, tempy));
	pPolygon3->Add(SCMPoint2D(tempx+leng[2], tempy));
	pPolygon3->Add(SCMPoint2D(tempx+leng[2], tempy+tf2));
	pPolygon3->Add(SCMPoint2D(tempx, tempy+tf2));
	if(bTapJ) aGeomPart1[1].Add(pPolygon3); // girder
	else      aGeomPart1[0].Add(pPolygon3); // girder

	if(Hw > 0.0)
	{
		twp = tw/Hw * sqrt(Hw*Hw + ((B1-B2)/2.) * ((B1-B2)/2.));
	}
	
	double tmp = (B1 - B2) * 0.5;
	if(fabs(tmp) > 0.0)
	{
		if(Hw > 0.0)
		{
			theta = atan(Hw/tmp);

			// 각이 (-) 일때 뒤집어 준다.
			if(tmp < 0.0)
			{
				theta = (180.0 * CMathFunc::m_trrad) + theta;
			}
		}
	}
	else
	{
		theta = (90.0 * CMathFunc::m_trrad);
	}
		
	tempx = (leng[0] - leng[1]) * 0.5 + Bf1;
	tempy = tc + Hh + tf1;
	tempx1 = (leng[0] - leng[2]) * 0.5 + Bf2;
	tempy1 = tc + Hh + tf1 + Hw;
	pPolygon4->Add(SCMPoint2D(tempx, tempy));
	pPolygon4->Add(SCMPoint2D(tempx-twp, tempy));
	pPolygon4->Add(SCMPoint2D(tempx1-twp, tempy1));
	pPolygon4->Add(SCMPoint2D(tempx1, tempy1));
	if(bTapJ) aGeomPart1[1].Add(pPolygon4); // girder
	else      aGeomPart1[0].Add(pPolygon4); // girder
	
	tempx = (leng[0] - leng[1]) * 0.5 + Bf1 + B1;
	tempy = tc + Hh + tf1;
	tempx1 = (leng[0] - leng[2]) * 0.5 + Bf2 + B2;
	tempy1 = tc + Hh + tf1 + Hw;
	pPolygon5->Add(SCMPoint2D(tempx, tempy));
	pPolygon5->Add(SCMPoint2D(tempx+twp, tempy));
	pPolygon5->Add(SCMPoint2D(tempx1+twp, tempy1));
	pPolygon5->Add(SCMPoint2D(tempx1, tempy1));
	if(bTapJ) aGeomPart1[1].Add(pPolygon4); // girder
	else      aGeomPart1[0].Add(pPolygon4); // girder

	///////////////////////////////////////////////
	// Rib ////////////////////////////////////////
//   if(B1 > 0)
//   {
//     double term, ribhw, ribh, x;
//     term = B1 / (N1 + 1);
//     tempx = (leng[0] - leng[1]) * 0.5 + Bf1;
//     tempy = tc + Hh + tf1;
//     ribhw = tr1 * 0.5;
//     ribh  = Hr1;
//     CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
//     for(int i=0; i < N1; i++)
//     {
//       x = tempx + (term * (i+1));
//       pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);
//       pPoly->Add(SCMPoint2D(x-ribhw, tempy));
//       pPoly->Add(SCMPoint2D(x+ribhw, tempy));
//       pPoly->Add(SCMPoint2D(x+ribhw, tempy+ribh));
//       pPoly->Add(SCMPoint2D(x-ribhw, tempy+ribh));
//       aGeom.Add(pPoly);
//     }
//   }
//   if(B2 > 0)
//   {
//     double term, ribhw, ribh, x;
//     term = B2 / (N2 + 1);
//     tempx = (leng[0] - leng[2]) * 0.5 + Bf2;
//     tempy = tc + Hh + tf1 + Hw;
//     ribhw = tr2 * 0.5;
//     ribh  = Hr2;
//     CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
//     for(int i=0; i < N2; i++)
//     {
//       x = tempx + (term * (i+1));
//       pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);
//       pPoly->Add(SCMPoint2D(x-ribhw, tempy));
//       pPoly->Add(SCMPoint2D(x+ribhw, tempy));
//       pPoly->Add(SCMPoint2D(x+ribhw, tempy-ribh));
//       pPoly->Add(SCMPoint2D(x-ribhw, tempy-ribh));
//       aGeom.Add(pPoly);
//     }
//   }
	///////////////////////////////////////////////


	///////////////////////////////////////////////
	// Stiffener //////////////////////////////////

	double dS_H  = 0.0;
	double dS_B  = 0.0;
	double dS_B1 = 0.0;
	double dS_B2 = 0.0;
	double dS_t  = 0.0;
	double dS_tw = 0.0;
	double dS_tf = 0.0;
	double dS_R  = 0.0;

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Left
	tempx = (leng[0] - leng[1]) * 0.5 + Bf1;
	tempy = tc + Hh + tf1;

	int nLSize = 0;

// 	if(bTapJ) nLSize = m_pDataSrc->CmpTapJ.Stiffener.nStiffNum[0];
// 	else      nLSize = m_pDataSrc->SectBefore.SectI.Stiffener.nStiffNum[0];

	nLSize = m_pDataSrc->SectBefore.SectI.Stiffener.nStiffNum[0];

	dSpaing = 0.0;
	double dSpaingTemp = 0.0;
	int nRefPos2 = m_pDataSrc->SectBefore.SectI.Stiffener.nRefPos2;      // 0: Top  1: Bottom

	for(int i=0; i<nLSize; ++i)
	{
		T_STIFF_PROFILE_D Stiffener; Stiffener.Initialize();
		T_STIFF_SHAPE_D Shape; Shape.Initialize();

// 		if(bTapJ)
// 		{
// 			Stiffener = m_pDataSrc->CmpTapJ.Stiffener.aStiffLeft[i];
// 			Shape = m_pDataSrc->CmpTapJ.Stiffener.aStiffShape[Stiffener.nShape];
// 		}
// 		else
// 		{
// 			Stiffener = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffLeft[i];
// 			Shape = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape[Stiffener.nShape];
// 		}

		Stiffener = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffLeft[i];

		if(Stiffener.nShape > -1 && m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape.GetSize() > Stiffener.nShape)
		{
			Shape = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape[Stiffener.nShape];
		}

		if(nRefPos2==0)
		{
			dSpaing += Stiffener.dSpacing;
		}
		else
		{
			dSpaingTemp += Stiffener.dSpacing;
			dSpaing = Hw - dSpaingTemp;	
		}

		CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
		pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);

		double dtemp_x = 0.0;

		if(theta == 0.0) dtemp_x = dSpaing;
		else             dtemp_x = dSpaing / tan(theta);

		///////////////////////////
		dy_org = tempx + dtemp_x;
		dz_org = tempy + dSpaing;
		///////////////////////////

		if(Shape.nType == 0) // I
		{
// 				strSize1 = _T("H");
// 				strSize2 = _T("B");
			dS_H = Shape.dSize[0];
			dS_B = Shape.dSize[1];

			MakePictureH_Stiffener(dS_H, dS_B, theta, dy_org, dz_org, 0, pPoly);
			aGeom.Add(pPoly);
		}
		else if(Shape.nType == 1) // T
		{
// 				strSize1 = _T("H");
// 				strSize2 = _T("B");
// 				strSize3 = _T("tw");
// 				strSize4 = _T("tf");
			dS_H  = Shape.dSize[0];
			dS_B  = Shape.dSize[1];
			dS_tw = Shape.dSize[2];
			dS_tf = Shape.dSize[3];

			MakePictureT_Stiffener(dS_H, dS_B, dS_tw, dS_tf, theta, dy_org, dz_org, 0, pPoly);
			aGeom.Add(pPoly);
		}
		else if(Shape.nType == 2) // U-Rib
		{
// 				strSize1 = _T("H");
// 				strSize2 = _T("B1");
// 				strSize3 = _T("B2");
// 				strSize4 = _T("t");
// 				strSize5 = _T("R");
			dS_H  = Shape.dSize[0];
			dS_B1 = Shape.dSize[1];
			dS_B2 = Shape.dSize[2];
			dS_t  = Shape.dSize[3];
			dS_R  = Shape.dSize[4];

			MakePictureURib_Stiffener(dS_H, dS_B1, dS_B2, dS_t, dS_R, theta, dy_org, dz_org, tf2, 0, pPoly);
			aGeom.Add(pPoly);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////

	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Right
	tempx = (leng[0] - leng[1]) * 0.5 + Bf1 + B1;
	tempy = tc + Hh + tf1;
	
	int nRSize = 0;
	
// 	if(bTapJ) nRSize = m_pDataSrc->CmpTapJ.Stiffener.nStiffNum[1];
// 	else      nRSize = m_pDataSrc->SectBefore.SectI.Stiffener.nStiffNum[1];

	nRSize = m_pDataSrc->SectBefore.SectI.Stiffener.nStiffNum[1];
	
	dSpaing = 0.0;
	dSpaingTemp = 0.0;
	
	for(int i=0; i<nRSize; ++i)
	{
		T_STIFF_PROFILE_D Stiffener; Stiffener.Initialize();
		T_STIFF_SHAPE_D Shape; Shape.Initialize();
		
// 		if(bTapJ)
// 		{
// 			Stiffener = m_pDataSrc->CmpTapJ.Stiffener.aStiffRight[i];
// 			Shape = m_pDataSrc->CmpTapJ.Stiffener.aStiffShape[Stiffener.nShape];
// 		}
// 		else
// 		{
// 			Stiffener = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffRight[i];
// 			Shape = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape[Stiffener.nShape];
// 		}

		Stiffener = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffRight[i];

		if(Stiffener.nShape > -1 && m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape.GetSize() > Stiffener.nShape)
		{
			Shape = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape[Stiffener.nShape];
		}		
		
		if (nRefPos2 == 0)
		{
			dSpaing += Stiffener.dSpacing;
		}
		else
		{
			dSpaingTemp += Stiffener.dSpacing;
			dSpaing = Hw - dSpaingTemp;
		}
		
		CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
		pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);

		double dtemp_x = 0.0;
		
		if(theta == 0.0) dtemp_x = dSpaing;
		else             dtemp_x = dSpaing / tan(theta);
		
		///////////////////////////
		dy_org = tempx - dtemp_x;
		dz_org = tempy + dSpaing;
		///////////////////////////
		
		if(Shape.nType == 0) // I
		{
			// 				strSize1 = _T("H");
			// 				strSize2 = _T("B");
			dS_H = Shape.dSize[0];
			dS_B = Shape.dSize[1];
			
			MakePictureH_Stiffener(dS_H, dS_B, theta, dy_org, dz_org, 1, pPoly);
			aGeom.Add(pPoly);
		}
		else if(Shape.nType == 1) // T
		{
			// 				strSize1 = _T("H");
			// 				strSize2 = _T("B");
			// 				strSize3 = _T("tw");
			// 				strSize4 = _T("tf");
			dS_H  = Shape.dSize[0];
			dS_B  = Shape.dSize[1];
			dS_tw = Shape.dSize[2];
			dS_tf = Shape.dSize[3];
			
			MakePictureT_Stiffener(dS_H, dS_B, dS_tw, dS_tf, theta, dy_org, dz_org, 1, pPoly);
			aGeom.Add(pPoly);
		}
		else if(Shape.nType == 2) // U-Rib
		{
			// 				strSize1 = _T("H");
			// 				strSize2 = _T("B1");
			// 				strSize3 = _T("B2");
			// 				strSize4 = _T("t");
			// 				strSize5 = _T("R");
			dS_H  = Shape.dSize[0];
			dS_B1 = Shape.dSize[1];
			dS_B2 = Shape.dSize[2];
			dS_t  = Shape.dSize[3];
			dS_R  = Shape.dSize[4];
			
			MakePictureURib_Stiffener(dS_H, dS_B1, dS_B2, dS_t, dS_R, theta, dy_org, dz_org, tf2, 1, pPoly);
			aGeom.Add(pPoly);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Bottom
	tempx = (leng[0] - leng[2]) * 0.5 + Bf2;
	tempy = tc + Hh + tf1 + Hw;

	int nBSize = 0;
	
// 	if(bTapJ) nBSize = m_pDataSrc->CmpTapJ.Stiffener.nStiffNum[2];
// 	else      nBSize = m_pDataSrc->SectBefore.SectI.Stiffener.nStiffNum[2];

	nBSize = m_pDataSrc->SectBefore.SectI.Stiffener.nStiffNum[2];
	
	dSpaing = 0.0;

	for(int i=0; i<nBSize; ++i)
	{
		T_STIFF_PROFILE_D Stiffener; Stiffener.Initialize();
		T_STIFF_SHAPE_D Shape; Shape.Initialize();
		
// 		if(bTapJ)
// 		{
// 			Stiffener = m_pDataSrc->CmpTapJ.Stiffener.aStiffBottom[i];
// 			Shape = m_pDataSrc->CmpTapJ.Stiffener.aStiffShape[Stiffener.nShape];
// 		}
// 		else
// 		{
// 			Stiffener = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffBottom[i];
// 			Shape = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape[Stiffener.nShape];
// 		}

		Stiffener = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffBottom[i];

		if(Stiffener.nShape > -1 && m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape.GetSize() > Stiffener.nShape)
		{
			Shape = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape[Stiffener.nShape];
		}
		
		dSpaing += Stiffener.dSpacing;

		CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
		pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);
		
		///////////////////////////
		dy_org = tempx + dSpaing;
		dz_org = tempy;
		///////////////////////////
		
		if(Shape.nType == 0) // I
		{
			// 				strSize1 = _T("H");
			// 				strSize2 = _T("B");
			dS_H = Shape.dSize[0];
			dS_B = Shape.dSize[1];
			
			MakePictureH_Stiffener(dS_H, dS_B, 0.0, dy_org, dz_org, 2, pPoly);
			aGeom.Add(pPoly);
		}
		else if(Shape.nType == 1) // T
		{
			// 				strSize1 = _T("H");
			// 				strSize2 = _T("B");
			// 				strSize3 = _T("tw");
			// 				strSize4 = _T("tf");
			dS_H  = Shape.dSize[0];
			dS_B  = Shape.dSize[1];
			dS_tw = Shape.dSize[2];
			dS_tf = Shape.dSize[3];
			
			MakePictureT_Stiffener(dS_H, dS_B, dS_tw, dS_tf, 0.0, dy_org, dz_org, 2, pPoly);
			aGeom.Add(pPoly);
		}
		else if(Shape.nType == 2) // U-Rib
		{
			// 				strSize1 = _T("H");
			// 				strSize2 = _T("B1");
			// 				strSize3 = _T("B2");
			// 				strSize4 = _T("t");
			// 				strSize5 = _T("R");
			dS_H  = Shape.dSize[0];
			dS_B1 = Shape.dSize[1];
			dS_B2 = Shape.dSize[2];
			dS_t  = Shape.dSize[3];
			dS_R  = Shape.dSize[4];
			
			MakePictureURib_Stiffener(dS_H, dS_B1, dS_B2, dS_t, dS_R, 0.0, dy_org, dz_org, tf2, 2, pPoly);
			aGeom.Add(pPoly);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Top
	tempx = (leng[0] - leng[1]) * 0.5 + Bf1;
	tempy = tc + Hh + tf1;
	
	int nTSize = 0;
	
// 	if(bTapJ) nTSize = m_pDataSrc->CmpTapJ.Stiffener.nStiffNum[3];
// 	else      nTSize = m_pDataSrc->SectBefore.SectI.Stiffener.nStiffNum[3];

	nTSize = m_pDataSrc->SectBefore.SectI.Stiffener.nStiffNum[3];
	
	dSpaing = 0.0;
	
	for(int i=0; i<nTSize; ++i)
	{
		T_STIFF_PROFILE_D Stiffener; Stiffener.Initialize();
		T_STIFF_SHAPE_D Shape; Shape.Initialize();
		
// 		if(bTapJ)
// 		{
// 			Stiffener = m_pDataSrc->CmpTapJ.Stiffener.aStiffTop[i];
// 			Shape = m_pDataSrc->CmpTapJ.Stiffener.aStiffShape[Stiffener.nShape];
// 		}
// 		else
// 		{
// 			Stiffener = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffTop[i];
// 			Shape = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape[Stiffener.nShape];
// 		}

		Stiffener = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffTop[i];

		if(Stiffener.nShape > -1 && m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape.GetSize() > Stiffener.nShape)
		{
			Shape = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape[Stiffener.nShape];
		}
		
		dSpaing += Stiffener.dSpacing;
		
		CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
		pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);
		
		///////////////////////////
		dy_org = tempx + dSpaing;
		dz_org = tempy;
		///////////////////////////
		
		if(Shape.nType == 0) // I
		{
			// 				strSize1 = _T("H");
			// 				strSize2 = _T("B");
			dS_H = Shape.dSize[0];
			dS_B = Shape.dSize[1];
			
			MakePictureH_Stiffener(dS_H, dS_B, 180.0 * CMathFunc::m_trrad, dy_org, dz_org, 3, pPoly);
			aGeom.Add(pPoly);
		}
		else if(Shape.nType == 1) // T
		{
			// 				strSize1 = _T("H");
			// 				strSize2 = _T("B");
			// 				strSize3 = _T("tw");
			// 				strSize4 = _T("tf");
			dS_H  = Shape.dSize[0];
			dS_B  = Shape.dSize[1];
			dS_tw = Shape.dSize[2];
			dS_tf = Shape.dSize[3];
			
			MakePictureT_Stiffener(dS_H, dS_B, dS_tw, dS_tf, 180.0 * CMathFunc::m_trrad, dy_org, dz_org, 3, pPoly);
			aGeom.Add(pPoly);
		}
		else if(Shape.nType == 2) // U-Rib
		{
			// 				strSize1 = _T("H");
			// 				strSize2 = _T("B1");
			// 				strSize3 = _T("B2");
			// 				strSize4 = _T("t");
			// 				strSize5 = _T("R");
			dS_H  = Shape.dSize[0];
			dS_B1 = Shape.dSize[1];
			dS_B2 = Shape.dSize[2];
			dS_t  = Shape.dSize[3];
			dS_R  = Shape.dSize[4];
			
			MakePictureURib_Stiffener(dS_H, dS_B1, dS_B2, dS_t, dS_R, 180.0 * CMathFunc::m_trrad, dy_org, dz_org, tf1, 3, pPoly);
			aGeom.Add(pPoly);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////


	aGeom.Add(pPolygon1);
	aGeom.Add(pPolygon2);
	aGeom.Add(pPolygon3);
	aGeom.Add(pPolygon4);
	aGeom.Add(pPolygon5);
}

void ICMGeomMaker::MakePictureCompoI(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ)
{
	CCMPolygon *pPolygon1 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon2 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon3 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon4 = new CCMPolygon(CCM_BORDER, CCM_FILL);

	double SW, CTC, Bc, tc, Hh, Hw, tw, B1, tf1, B2, tf2, E, D;
	int    GNum;
	double dy_org, dz_org;
	double dSpaing;

	BOOL bTapJ = (m_pDataSrc->nStype==D_SECT_TYPE_TAPERED && bJ);

	// read ///////////////////////////////////////
	SW   = m_pDataSrc->SectAfter.SectI.Size[0];
	CTC  = m_pDataSrc->SectAfter.SectI.Size[1];
	GNum = m_pDataSrc->SectAfter.SectI.BuiltUpFlag;

	Bc  = m_pDataSrc->SectAfter.SectJ.Size[0];
	tc  = m_pDataSrc->SectAfter.SectJ.Size[1];
	Hh  = m_pDataSrc->SectAfter.SectJ.Size[2];

	if (bTapJ)
	{
		Hw  = m_pDataSrc->CmpTapJ.Size[0];
		tw  = m_pDataSrc->CmpTapJ.Size[1];
		B1  = m_pDataSrc->CmpTapJ.Size[2];
		tf1 = m_pDataSrc->CmpTapJ.Size[3];
		B2  = m_pDataSrc->CmpTapJ.Size[4];
		tf2 = m_pDataSrc->CmpTapJ.Size[5];
	}
	else
	{
		Hw  = m_pDataSrc->SectBefore.SectI.Size[0];
		tw  = m_pDataSrc->SectBefore.SectI.Size[1];
		B1  = m_pDataSrc->SectBefore.SectI.Size[2];
		tf1 = m_pDataSrc->SectBefore.SectI.Size[3];
		B2  = m_pDataSrc->SectBefore.SectI.Size[4];
		tf2 = m_pDataSrc->SectBefore.SectI.Size[5];
	}

	E   = m_pDataSrc->SectBefore.Matl_Elast;
	D   = m_pDataSrc->SectBefore.Matl_Density;
	///////////////////////////////////////////////


	// draw ///////////////////////////////////////
	double  leng[3], tempx, tempy;
	leng[0] = Bc;
	leng[1] = B1;
	leng[2] = B2;

	tempx = 0;
	tempy = 0;
	pPolygon1->Add(SCMPoint2D(tempx, tempy));
	pPolygon1->Add(SCMPoint2D(tempx+leng[0], tempy));
	pPolygon1->Add(SCMPoint2D(tempx+leng[0], tc));
	pPolygon1->Add(SCMPoint2D(tempx, tc));
	if(bTapJ) aGeomPart2[1].Add(pPolygon1); // slab
	else      aGeomPart2[0].Add(pPolygon1); // slab

	tempx = (leng[0] - leng[1]) * 0.5;
	tempy = tc + Hh;
	pPolygon2->Add(SCMPoint2D(tempx, tempy));
	pPolygon2->Add(SCMPoint2D(tempx+leng[1], tempy));
	pPolygon2->Add(SCMPoint2D(tempx+leng[1], tempy+tf1));
	pPolygon2->Add(SCMPoint2D(tempx, tempy+tf1));
	if(bTapJ) aGeomPart1[1].Add(pPolygon2); // girder
	else      aGeomPart1[0].Add(pPolygon2); // girder

	tempx = (leng[0] - leng[2]) * 0.5;
	tempy = tc + Hh + tf1 + Hw;
	pPolygon3->Add(SCMPoint2D(tempx, tempy));
	pPolygon3->Add(SCMPoint2D(tempx+leng[2], tempy));
	pPolygon3->Add(SCMPoint2D(tempx+leng[2], tempy+tf2));
	pPolygon3->Add(SCMPoint2D(tempx, tempy+tf2));
	if(bTapJ) aGeomPart1[1].Add(pPolygon3); // girder
	else      aGeomPart1[0].Add(pPolygon3); // girder

	tempx = leng[0] * 0.5 - tw * 0.5;
	tempy = tc + Hh + tf1;
	pPolygon4->Add(SCMPoint2D(tempx, tempy));
	pPolygon4->Add(SCMPoint2D(tempx+tw, tempy));
	pPolygon4->Add(SCMPoint2D(tempx+tw, tempy+Hw));
	pPolygon4->Add(SCMPoint2D(tempx, tempy+Hw));
	if(bTapJ) aGeomPart1[1].Add(pPolygon4); // girder
	else      aGeomPart1[0].Add(pPolygon4); // girder
	///////////////////////////////////////////////
	// Stiffener //////////////////////////////////
	
	double dS_H  = 0.0;
	double dS_B  = 0.0;
	double dS_B1 = 0.0;
	double dS_B2 = 0.0;
	double dS_t  = 0.0;
	double dS_tw = 0.0;
	double dS_tf = 0.0;
	double dS_R  = 0.0;

	BOOL bLeft  = FALSE;
	BOOL bRight = FALSE;

	if     (m_pDataSrc->SectBefore.SectI.Stiffener.nPosition == 0) { bLeft = TRUE;  bRight = TRUE;  }
	else if(m_pDataSrc->SectBefore.SectI.Stiffener.nPosition == 1) { bLeft = TRUE;  bRight = FALSE; }
	else if(m_pDataSrc->SectBefore.SectI.Stiffener.nPosition == 2) { bLeft = FALSE; bRight = TRUE;  }
	else ASSERT(0);

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Left
	if(bLeft)
	{
		tempx = leng[0] * 0.5 - tw * 0.5;
		tempy = tc + Hh + tf1;
		
		int nLSize = 0;
		
		// 	if(bTapJ) nRSize = m_pDataSrc->CmpTapJ.Stiffener.nStiffNum[1];
		// 	else      nRSize = m_pDataSrc->SectBefore.SectI.Stiffener.nStiffNum[1];
		
		nLSize = m_pDataSrc->SectBefore.SectI.Stiffener.nStiffNum[0];
		
		dSpaing = 0.0;
		double dSpaingTemp = 0.0;
		int nRefPos2 = m_pDataSrc->SectBefore.SectI.Stiffener.nRefPos2;      // 0: Top  1: Bottom

		for(int i=0; i<nLSize; ++i)
		{
			T_STIFF_PROFILE_D Stiffener; Stiffener.Initialize();
			T_STIFF_SHAPE_D Shape; Shape.Initialize();
			
			// 		if(bTapJ)
			// 		{
			// 			Stiffener = m_pDataSrc->CmpTapJ.Stiffener.aStiffLeft[i];
			// 			Shape = m_pDataSrc->CmpTapJ.Stiffener.aStiffShape[Stiffener.nShape];
			// 		}
			// 		else
			// 		{
			// 			Stiffener = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffLeft[i];
			// 			Shape = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape[Stiffener.nShape];
			// 		}
			
			Stiffener = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffLeft[i];
			
			if(Stiffener.nShape > -1 && m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape.GetSize() > Stiffener.nShape)
			{
				Shape = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape[Stiffener.nShape];
			}
			
			if (nRefPos2 == 0)
			{
				dSpaing += Stiffener.dSpacing;
			}
			else
			{
				dSpaingTemp += Stiffener.dSpacing;
				dSpaing = Hw - dSpaingTemp;
			}
			
			CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
			pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);
			
			///////////////////////////
			dy_org = tempx;
			dz_org = tempy + dSpaing;
			///////////////////////////
			
			if(Shape.nType == 0) // I
			{
				// 				strSize1 = _T("H");
				// 				strSize2 = _T("B");
				dS_H = Shape.dSize[0];
				dS_B = Shape.dSize[1];
				
				MakePictureH_Stiffener(dS_H, dS_B, 90.0 * CMathFunc::m_trrad, dy_org, dz_org, 1, pPoly);
				aGeom.Add(pPoly);
			}
			else if(Shape.nType == 1) // T
			{
				// 				strSize1 = _T("H");
				// 				strSize2 = _T("B");
				// 				strSize3 = _T("tw");
				// 				strSize4 = _T("tf");
				dS_H  = Shape.dSize[0];
				dS_B  = Shape.dSize[1];
				dS_tw = Shape.dSize[2];
				dS_tf = Shape.dSize[3];
				
				MakePictureT_Stiffener(dS_H, dS_B, dS_tw, dS_tf, 90.0 * CMathFunc::m_trrad, dy_org, dz_org, 1, pPoly);
				aGeom.Add(pPoly);
			}
			else if(Shape.nType == 2) // U-Rib
			{
				// 				strSize1 = _T("H");
				// 				strSize2 = _T("B1");
				// 				strSize3 = _T("B2");
				// 				strSize4 = _T("t");
				// 				strSize5 = _T("R");
				dS_H  = Shape.dSize[0];
				dS_B1 = Shape.dSize[1];
				dS_B2 = Shape.dSize[2];
				dS_t  = Shape.dSize[3];
				dS_R  = Shape.dSize[4];
				
				MakePictureURib_Stiffener(dS_H, dS_B1, dS_B2, dS_t, dS_R, 90.0 * CMathFunc::m_trrad, dy_org, dz_org, tf2, 1, pPoly);
				aGeom.Add(pPoly);
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////



	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Right
	if(bRight)
	{
		tempx = leng[0] * 0.5 + tw * 0.5;
		tempy = tc + Hh + tf1;
		
		int nRSize = 0;
		
		// 	if(bTapJ) nLSize = m_pDataSrc->CmpTapJ.Stiffener.nStiffNum[0];
		// 	else      nLSize = m_pDataSrc->SectBefore.SectI.Stiffener.nStiffNum[0];
		
		nRSize = m_pDataSrc->SectBefore.SectI.Stiffener.nStiffNum[1];
		
		dSpaing = 0.0;
		double dSpaingTemp = 0.0;
		int nRefPos2 = m_pDataSrc->SectBefore.SectI.Stiffener.nRefPos2;      // 0: Top  1: Bottom

		for(int i=0; i<nRSize; ++i)
		{
			T_STIFF_PROFILE_D Stiffener; Stiffener.Initialize();
			T_STIFF_SHAPE_D Shape; Shape.Initialize();
			
			// 		if(bTapJ)
			// 		{
			// 			Stiffener = m_pDataSrc->CmpTapJ.Stiffener.aStiffRight[i];
			// 			Shape = m_pDataSrc->CmpTapJ.Stiffener.aStiffShape[Stiffener.nShape];
			// 		}
			// 		else
			// 		{
			// 			Stiffener = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffRight[i];
			// 			Shape = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape[Stiffener.nShape];
			// 		}
			
			Stiffener = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffRight[i];
			
			if(Stiffener.nShape > -1 && m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape.GetSize() > Stiffener.nShape)
			{
				Shape = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape[Stiffener.nShape];
			}
			
			if (nRefPos2 == 0)
			{
				dSpaing += Stiffener.dSpacing;
			}
			else
			{
				dSpaingTemp += Stiffener.dSpacing;
				dSpaing = Hw - dSpaingTemp;
			}

			CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
			pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);
			
			///////////////////////////
			dy_org = tempx;
			dz_org = tempy + dSpaing;
			///////////////////////////
			
			if(Shape.nType == 0) // I
			{
				// 				strSize1 = _T("H");
				// 				strSize2 = _T("B");
				dS_H = Shape.dSize[0];
				dS_B = Shape.dSize[1];
				
				MakePictureH_Stiffener(dS_H, dS_B, 90.0 * CMathFunc::m_trrad, dy_org, dz_org, 0, pPoly);
				aGeom.Add(pPoly);
			}
			else if(Shape.nType == 1) // T
			{
				// 				strSize1 = _T("H");
				// 				strSize2 = _T("B");
				// 				strSize3 = _T("tw");
				// 				strSize4 = _T("tf");
				dS_H  = Shape.dSize[0];
				dS_B  = Shape.dSize[1];
				dS_tw = Shape.dSize[2];
				dS_tf = Shape.dSize[3];
				
				MakePictureT_Stiffener(dS_H, dS_B, dS_tw, dS_tf, 90.0 * CMathFunc::m_trrad, dy_org, dz_org, 0, pPoly);
				aGeom.Add(pPoly);
			}
			else if(Shape.nType == 2) // U-Rib
			{
				// 				strSize1 = _T("H");
				// 				strSize2 = _T("B1");
				// 				strSize3 = _T("B2");
				// 				strSize4 = _T("t");
				// 				strSize5 = _T("R");
				dS_H  = Shape.dSize[0];
				dS_B1 = Shape.dSize[1];
				dS_B2 = Shape.dSize[2];
				dS_t  = Shape.dSize[3];
				dS_R  = Shape.dSize[4];
				
				MakePictureURib_Stiffener(dS_H, dS_B1, dS_B2, dS_t, dS_R, 90.0 * CMathFunc::m_trrad, dy_org, dz_org, tf2, 0, pPoly);
				aGeom.Add(pPoly);
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////

	aGeom.Add(pPolygon1);
	aGeom.Add(pPolygon2);
	aGeom.Add(pPolygon3);
	aGeom.Add(pPolygon4);
}

void ICMGeomMaker::MakePictureCompoTub(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ)
{
	CCMPolygon *pPolygon1 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon2 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon3 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon4 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon5 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon6 = new CCMPolygon(CCM_BORDER, CCM_FILL);

	double SW, CTC, Bc, tc, Hh, Hw, tw, B1, Bf1, tf1, B2, Bf2, tf2, Bf3, E, D;
	int    GNum;
	double twp = 0.0;
	double theta = 0.0;
	double dy_org, dz_org;
	double dSpaing;

	// read ///////////////////////////////////////
	SW   = m_pDataSrc->SectAfter.SectI.Size[0];
	CTC  = m_pDataSrc->SectAfter.SectI.Size[1];
	GNum = m_pDataSrc->SectAfter.SectI.BuiltUpFlag;

	Bc  = m_pDataSrc->SectAfter.SectJ.Size[0];
	tc  = m_pDataSrc->SectAfter.SectJ.Size[1];
	Hh  = m_pDataSrc->SectAfter.SectJ.Size[2];

	BOOL bTapJ = (m_pDataSrc->nStype==D_SECT_TYPE_TAPERED && bJ);
	if(bTapJ)
	{
		Hw  = m_pDataSrc->CmpTapJ.Size[0];
		tw  = m_pDataSrc->CmpTapJ.Size[1];
		B1  = m_pDataSrc->CmpTapJ.Size[2];
		Bf1 = m_pDataSrc->CmpTapJ.Size[3];
		tf1 = m_pDataSrc->CmpTapJ.Size[4];
		B2  = m_pDataSrc->CmpTapJ.Size[5];
		Bf2 = m_pDataSrc->CmpTapJ.Size[6];
		tf2 = m_pDataSrc->CmpTapJ.Size[7];
		Bf3 = m_pDataSrc->CmpTapJ.Size[8];
	}
	else
	{
		Hw  = m_pDataSrc->SectBefore.SectI.Size[0];
		tw  = m_pDataSrc->SectBefore.SectI.Size[1];
		B1  = m_pDataSrc->SectBefore.SectI.Size[2];
		Bf1 = m_pDataSrc->SectBefore.SectI.Size[3];
		tf1 = m_pDataSrc->SectBefore.SectI.Size[4];
		B2  = m_pDataSrc->SectBefore.SectI.Size[5];
		Bf2 = m_pDataSrc->SectBefore.SectI.Size[6];
		tf2 = m_pDataSrc->SectBefore.SectI.Size[7];
		Bf3 = m_pDataSrc->SectBefore.SectI.Size[8];
	}

	E   = m_pDataSrc->SectBefore.Matl_Elast;
	D   = m_pDataSrc->SectBefore.Matl_Density;
	///////////////////////////////////////////////


	// draw ///////////////////////////////////////
	double  leng[3], tempx, tempy, tempx1, tempy1;
	leng[0] = Bc;
	leng[1] = Bf1 + B1 + Bf1;
	leng[2] = Bf2 + B2 + Bf2;

	tempx = 0;
	tempy = 0;
	pPolygon1->Add(SCMPoint2D(tempx, tempy));
	pPolygon1->Add(SCMPoint2D(tempx+leng[0], tempy));
	pPolygon1->Add(SCMPoint2D(tempx+leng[0], tc));
	pPolygon1->Add(SCMPoint2D(tempx, tc));
	if(bTapJ) aGeomPart2[1].Add(pPolygon1); // slab
	else      aGeomPart2[0].Add(pPolygon1); // slab

	tempx = (leng[0] - leng[1]) * 0.5;
	tempy = tc + Hh;
	pPolygon2->Add(SCMPoint2D(tempx, tempy));
	pPolygon2->Add(SCMPoint2D(tempx+Bf1, tempy));
	pPolygon2->Add(SCMPoint2D(tempx+Bf1, tempy+tf1));
	pPolygon2->Add(SCMPoint2D(tempx, tempy+tf1));
	if(bTapJ) aGeomPart1[1].Add(pPolygon2); // girder
	else      aGeomPart1[0].Add(pPolygon2); // girder

	tempx = (leng[0] - leng[1]) * 0.5 + Bf1 + B1;
	tempy = tc + Hh;
	pPolygon3->Add(SCMPoint2D(tempx, tempy));
	pPolygon3->Add(SCMPoint2D(tempx+Bf1, tempy));
	pPolygon3->Add(SCMPoint2D(tempx+Bf1, tempy+tf1));
	pPolygon3->Add(SCMPoint2D(tempx, tempy+tf1));
	if(bTapJ) aGeomPart1[1].Add(pPolygon3); // girder
	else      aGeomPart1[0].Add(pPolygon3); // girder

	tempx = (leng[0] - leng[2]) * 0.5;
	tempy = tc + Hh + tf1 + Hw;
	pPolygon4->Add(SCMPoint2D(tempx, tempy));
	pPolygon4->Add(SCMPoint2D(tempx+leng[2], tempy));
	pPolygon4->Add(SCMPoint2D(tempx+leng[2], tempy+tf2));
	pPolygon4->Add(SCMPoint2D(tempx, tempy+tf2));
	if(bTapJ) aGeomPart1[1].Add(pPolygon4); // girder
	else      aGeomPart1[0].Add(pPolygon4); // girder

	double B1_UF = B1+2*(Bf1-Bf3); // Web 사이의 거리

	if(Hw > 0.0)
	{
		twp = tw/Hw * sqrt(Hw*Hw + ((B1_UF-B2)/2.) * ((B1_UF-B2)/2.));
	}

	double tmp = (B1_UF - B2) * 0.5;
	if(fabs(tmp) > 0.0) 
	{
		if(Hw > 0.0)
		{
			theta = atan(Hw/tmp);
			
			// 각이 (-) 일때 뒤집어 준다.
			if(tmp < 0.0)
			{
				theta = (180.0 * CMathFunc::m_trrad) + theta;
			}
		}
	}
	else
	{
		theta = (90.0 * CMathFunc::m_trrad);
	}

	tempx = (leng[0] - leng[1]) * 0.5 + Bf3 - twp;
	tempy = tc + Hh + tf1;
	tempx1 = (leng[0] - leng[2]) * 0.5 + Bf2 - twp;
	tempy1 = tc + Hh + tf1 + Hw;
	pPolygon5->Add(SCMPoint2D(tempx, tempy));
	pPolygon5->Add(SCMPoint2D(tempx+twp, tempy));
	pPolygon5->Add(SCMPoint2D(tempx1+twp, tempy1));
	pPolygon5->Add(SCMPoint2D(tempx1, tempy1));
	if(bTapJ) aGeomPart1[1].Add(pPolygon5); // girder
	else      aGeomPart1[0].Add(pPolygon5); // girder

	tempx = (leng[0] - leng[1]) * 0.5 + Bf1 + B1 + Bf1 - Bf3 + twp;
	tempy = tc + Hh + tf1;
	tempx1 = (leng[0] - leng[2]) * 0.5 + Bf2 + B2 + twp;
	tempy1 = tc + Hh + tf1 + Hw;
	pPolygon6->Add(SCMPoint2D(tempx, tempy));
	pPolygon6->Add(SCMPoint2D(tempx-twp, tempy));
	pPolygon6->Add(SCMPoint2D(tempx1-twp, tempy1));
	pPolygon6->Add(SCMPoint2D(tempx1, tempy1));
	if(bTapJ) aGeomPart1[1].Add(pPolygon6); // girder
	else      aGeomPart1[0].Add(pPolygon6); // girder
	///////////////////////////////////////////////
	// Stiffener //////////////////////////////////

	double dS_H  = 0.0;
	double dS_B  = 0.0;
	double dS_B1 = 0.0;
	double dS_B2 = 0.0;
	double dS_t  = 0.0;
	double dS_tw = 0.0;
	double dS_tf = 0.0;
	double dS_R  = 0.0;

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Left
	tempx = (leng[0] - leng[1]) * 0.5 + Bf3;
	tempy = tc + Hh + tf1;

	int nLSize = 0;

// 	if(bTapJ) nLSize = m_pDataSrc->CmpTapJ.Stiffener.nStiffNum[0];
// 	else      nLSize = m_pDataSrc->SectBefore.SectI.Stiffener.nStiffNum[0];

	nLSize = m_pDataSrc->SectBefore.SectI.Stiffener.nStiffNum[0];

	dSpaing = 0.0;
	double dSpaingTemp = 0.0;
	int nRefPos2 = m_pDataSrc->SectBefore.SectI.Stiffener.nRefPos2;      // 0: Top  1: Bottom

	for(int i=0; i<nLSize; ++i)
	{
		T_STIFF_PROFILE_D Stiffener; Stiffener.Initialize();
		T_STIFF_SHAPE_D Shape; Shape.Initialize();

// 		if(bTapJ)
// 		{
// 			Stiffener = m_pDataSrc->CmpTapJ.Stiffener.aStiffLeft[i];
// 			Shape = m_pDataSrc->CmpTapJ.Stiffener.aStiffShape[Stiffener.nShape];
// 		}
// 		else
// 		{
// 			Stiffener = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffLeft[i];
// 			Shape = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape[Stiffener.nShape];
// 		}

		Stiffener = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffLeft[i];

		if(Stiffener.nShape > -1 && m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape.GetSize() > Stiffener.nShape)
		{
			Shape = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape[Stiffener.nShape];
		}

		if (nRefPos2 == 0)
		{
			dSpaing += Stiffener.dSpacing;
		}
		else
		{
			dSpaingTemp += Stiffener.dSpacing;
			dSpaing = Hw - dSpaingTemp;
		}

		CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
		pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);

		double dtemp_x = 0.0;
		
		if(theta == 0.0) dtemp_x = dSpaing;
		else             dtemp_x = dSpaing / tan(theta);

		///////////////////////////
		dy_org = tempx + dtemp_x;
		dz_org = tempy + dSpaing;
		///////////////////////////

		if(Shape.nType == 0) // I
		{
// 				strSize1 = _T("H");
// 				strSize2 = _T("B");
			dS_H = Shape.dSize[0];
			dS_B = Shape.dSize[1];

			MakePictureH_Stiffener(dS_H, dS_B, theta, dy_org, dz_org, 0, pPoly);
			aGeom.Add(pPoly);
		}
		else if(Shape.nType == 1) // T
		{
// 				strSize1 = _T("H");
// 				strSize2 = _T("B");
// 				strSize3 = _T("tw");
// 				strSize4 = _T("tf");
			dS_H  = Shape.dSize[0];
			dS_B  = Shape.dSize[1];
			dS_tw = Shape.dSize[2];
			dS_tf = Shape.dSize[3];

			MakePictureT_Stiffener(dS_H, dS_B, dS_tw, dS_tf, theta, dy_org, dz_org, 0, pPoly);
			aGeom.Add(pPoly);
		}
		else if(Shape.nType == 2) // U-Rib
		{
// 				strSize1 = _T("H");
// 				strSize2 = _T("B1");
// 				strSize3 = _T("B2");
// 				strSize4 = _T("t");
// 				strSize5 = _T("R");
			dS_H  = Shape.dSize[0];
			dS_B1 = Shape.dSize[1];
			dS_B2 = Shape.dSize[2];
			dS_t  = Shape.dSize[3];
			dS_R  = Shape.dSize[4];

			MakePictureURib_Stiffener(dS_H, dS_B1, dS_B2, dS_t, dS_R, theta, dy_org, dz_org, tf2, 0, pPoly);
			aGeom.Add(pPoly);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////

	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Right
	tempx = (leng[0] - leng[1]) * 0.5 + 2 * Bf1 + B1 - Bf3;
	tempy = tc + Hh + tf1;
	
	int nRSize = 0;
	
// 	if(bTapJ) nRSize = m_pDataSrc->CmpTapJ.Stiffener.nStiffNum[1];
// 	else      nRSize = m_pDataSrc->SectBefore.SectI.Stiffener.nStiffNum[1];

	nRSize = m_pDataSrc->SectBefore.SectI.Stiffener.nStiffNum[1];
	
	dSpaing = 0.0;
	dSpaingTemp = 0.0;
	
	for(int i=0; i<nRSize; ++i)
	{
		T_STIFF_PROFILE_D Stiffener; Stiffener.Initialize();
		T_STIFF_SHAPE_D Shape; Shape.Initialize();
		
// 		if(bTapJ)
// 		{
// 			Stiffener = m_pDataSrc->CmpTapJ.Stiffener.aStiffRight[i];
// 			Shape = m_pDataSrc->CmpTapJ.Stiffener.aStiffShape[Stiffener.nShape];
// 		}
// 		else
// 		{
// 			Stiffener = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffRight[i];
// 			Shape = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape[Stiffener.nShape];
// 		}

		Stiffener = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffRight[i];

		if(Stiffener.nShape > -1 && m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape.GetSize() > Stiffener.nShape)
		{
			Shape = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape[Stiffener.nShape];
		}
		
		if (nRefPos2 == 0)
		{
			dSpaing += Stiffener.dSpacing;
		}
		else
		{
			dSpaingTemp += Stiffener.dSpacing;
			dSpaing = Hw - dSpaingTemp;
		}
		
		CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
		pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);
		
		double dtemp_x = 0.0;
		
		if(theta == 0.0) dtemp_x = dSpaing;
		else             dtemp_x = dSpaing / tan(theta);
		
		///////////////////////////
		dy_org = tempx - dtemp_x;
		dz_org = tempy + dSpaing;
		///////////////////////////
		
		if(Shape.nType == 0) // I
		{
			// 				strSize1 = _T("H");
			// 				strSize2 = _T("B");
			dS_H = Shape.dSize[0];
			dS_B = Shape.dSize[1];
			
			MakePictureH_Stiffener(dS_H, dS_B, theta, dy_org, dz_org, 1, pPoly);
			aGeom.Add(pPoly);
		}
		else if(Shape.nType == 1) // T
		{
			// 				strSize1 = _T("H");
			// 				strSize2 = _T("B");
			// 				strSize3 = _T("tw");
			// 				strSize4 = _T("tf");
			dS_H  = Shape.dSize[0];
			dS_B  = Shape.dSize[1];
			dS_tw = Shape.dSize[2];
			dS_tf = Shape.dSize[3];
			
			MakePictureT_Stiffener(dS_H, dS_B, dS_tw, dS_tf, theta, dy_org, dz_org, 1, pPoly);
			aGeom.Add(pPoly);
		}
		else if(Shape.nType == 2) // U-Rib
		{
			// 				strSize1 = _T("H");
			// 				strSize2 = _T("B1");
			// 				strSize3 = _T("B2");
			// 				strSize4 = _T("t");
			// 				strSize5 = _T("R");
			dS_H  = Shape.dSize[0];
			dS_B1 = Shape.dSize[1];
			dS_B2 = Shape.dSize[2];
			dS_t  = Shape.dSize[3];
			dS_R  = Shape.dSize[4];
			
			MakePictureURib_Stiffener(dS_H, dS_B1, dS_B2, dS_t, dS_R, theta, dy_org, dz_org, tf2, 1, pPoly);
			aGeom.Add(pPoly);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Bottom
	tempx = (leng[0] - leng[2]) * 0.5 + Bf2;
	tempy = tc + Hh + tf1 + Hw;

	int nBSize = 0;
	
// 	if(bTapJ) nBSize = m_pDataSrc->CmpTapJ.Stiffener.nStiffNum[2];
// 	else      nBSize = m_pDataSrc->SectBefore.SectI.Stiffener.nStiffNum[2];

	nBSize = m_pDataSrc->SectBefore.SectI.Stiffener.nStiffNum[2];
	
	dSpaing = 0.0;

	for(int i=0; i<nBSize; ++i)
	{
		T_STIFF_PROFILE_D Stiffener; Stiffener.Initialize();
		T_STIFF_SHAPE_D Shape; Shape.Initialize();
		
// 		if(bTapJ)
// 		{
// 			Stiffener = m_pDataSrc->CmpTapJ.Stiffener.aStiffBottom[i];
// 			Shape = m_pDataSrc->CmpTapJ.Stiffener.aStiffShape[Stiffener.nShape];
// 		}
// 		else
// 		{
// 			Stiffener = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffBottom[i];
// 			Shape = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape[Stiffener.nShape];
// 		}

		Stiffener = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffBottom[i];

		if(Stiffener.nShape > -1 && m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape.GetSize() > Stiffener.nShape)
		{
			Shape = m_pDataSrc->SectBefore.SectI.Stiffener.aStiffShape[Stiffener.nShape];
		}
		
		dSpaing += Stiffener.dSpacing;

		CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
		pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);
		
		///////////////////////////
		dy_org = tempx + dSpaing;
		dz_org = tempy;
		///////////////////////////
		
		if(Shape.nType == 0) // I
		{
			// 				strSize1 = _T("H");
			// 				strSize2 = _T("B");
			dS_H = Shape.dSize[0];
			dS_B = Shape.dSize[1];
			
			MakePictureH_Stiffener(dS_H, dS_B, theta, dy_org, dz_org, 2, pPoly);
			aGeom.Add(pPoly);
		}
		else if(Shape.nType == 1) // T
		{
			// 				strSize1 = _T("H");
			// 				strSize2 = _T("B");
			// 				strSize3 = _T("tw");
			// 				strSize4 = _T("tf");
			dS_H  = Shape.dSize[0];
			dS_B  = Shape.dSize[1];
			dS_tw = Shape.dSize[2];
			dS_tf = Shape.dSize[3];
			
			MakePictureT_Stiffener(dS_H, dS_B, dS_tw, dS_tf, theta, dy_org, dz_org, 2, pPoly);
			aGeom.Add(pPoly);
		}
		else if(Shape.nType == 2) // U-Rib
		{
			// 				strSize1 = _T("H");
			// 				strSize2 = _T("B1");
			// 				strSize3 = _T("B2");
			// 				strSize4 = _T("t");
			// 				strSize5 = _T("R");
			dS_H  = Shape.dSize[0];
			dS_B1 = Shape.dSize[1];
			dS_B2 = Shape.dSize[2];
			dS_t  = Shape.dSize[3];
			dS_R  = Shape.dSize[4];
			
			MakePictureURib_Stiffener(dS_H, dS_B1, dS_B2, dS_t, dS_R, theta, dy_org, dz_org, tf2, 2, pPoly);
			aGeom.Add(pPoly);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////

	aGeom.Add(pPolygon1);
	aGeom.Add(pPolygon2);
	aGeom.Add(pPolygon3);
	aGeom.Add(pPolygon4);
	aGeom.Add(pPolygon5);
	aGeom.Add(pPolygon6);
}

void ICMGeomMaker::MakePictureCompoCI(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ)
{
	/*
	T_SECT_D GirderSect;
	CSectUtil::GetSectFromCompoCI(*m_pDataSrc, GirderSect);
	MakePicturePSC(GirderSect, aGeom);

	CCMPolygon *pPolygon1 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	T_SECT_COMPO_CI rSect;
	rSect.Set(*m_pDataSrc);
	double cx, cy, h, w;
	SCMRect2D GirderMBR = aGeom.GetMBR();
	cx = (GirderMBR.min.x+GirderMBR.max.x)/2.0;
	cy = (GirderMBR.min.y+GirderMBR.max.y)/2.0;
	h = rSect.Slab.tc;
	w = rSect.Slab.Bc;

	pPolygon1->Add(SCMPoint2D(cx-h/2.0, GirderMBR.max.y+rSect.Slab.Hh));
	pPolygon1->Add(SCMPoint2D(cx+h/2.0, GirderMBR.max.y+rSect.Slab.Hh));
	pPolygon1->Add(SCMPoint2D(cx+h/2.0, GirderMBR.max.y+rSect.Slab.Hh+rSect.Slab.tc));
	pPolygon1->Add(SCMPoint2D(cx-h/2.0, GirderMBR.max.y+rSect.Slab.Hh+rSect.Slab.tc));

	aGeom.Add(pPolygon1);
	*/
	
	MakePictureCompo(*m_pDataSrc,aGeom, aGeomPart1, aGeomPart2, bJ);
}

void ICMGeomMaker::MakePictureCompoCT(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ)
{
	/*
	T_SECT_D GirderSect;
	CSectUtil::GetSectFromCompoCT(*m_pDataSrc, GirderSect);
	MakePicturePSC(GirderSect, aGeom);

	CCMPolygon *pPolygon1 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	T_SECT_COMPO_CI rSect;
	rSect.Set(*m_pDataSrc);
	double cx, cy, h, w;
	SCMRect2D GirderMBR = aGeom.GetMBR();
	cx = (GirderMBR.min.x+GirderMBR.max.x)/2.0;
	cy = (GirderMBR.min.y+GirderMBR.max.y)/2.0;
	h = rSect.Slab.tc;
	w = rSect.Slab.Bc;

	pPolygon1->Add(SCMPoint2D(cx-h/2.0, GirderMBR.max.y+rSect.Slab.Hh));
	pPolygon1->Add(SCMPoint2D(cx+h/2.0, GirderMBR.max.y+rSect.Slab.Hh));
	pPolygon1->Add(SCMPoint2D(cx+h/2.0, GirderMBR.max.y+rSect.Slab.Hh+rSect.Slab.tc));
	pPolygon1->Add(SCMPoint2D(cx-h/2.0, GirderMBR.max.y+rSect.Slab.Hh+rSect.Slab.tc));

	aGeom.Add(pPolygon1);
	*/
	MakePictureCompo(*m_pDataSrc,aGeom, aGeomPart1, aGeomPart2, bJ);
}

void ICMGeomMaker::MakePictureCompoPC(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ)
{
	//MakePictureCompo(*m_pDataSrc,aGeom, aGeomPart1, aGeomPart2, bJ);

	// girder
	T_SECT_D SectD = (*m_pDataSrc);
	SectD.SectBefore.Shape = D_SECT_SHAPE_PSC_VALUE;
	MakePicturePSC(SectD, aGeom);
	int nSize = aGeom.GetSize();

	SCMRect2D MBR = aGeom.GetMBR();
	
	for(int i=0; i<nSize; ++i)
	{
		aGeomPart1[0].Add(aGeom[i]);
	}

	// slab
	double Bc = m_pDataSrc->SectAfter.SectJ.Size[0];
	double tc = m_pDataSrc->SectAfter.SectJ.Size[1];
	double Hh = m_pDataSrc->SectAfter.SectJ.Size[2];
	double Sg = m_pDataSrc->SectAfter.SectJ.Size[3];
	
	if(Bc!=0.0 && tc!=0.0)
	{
		double dH = MBR.min.y;

		// step1
		T_SECT_POLYGON Slab;
		CArray<T_SECT_POLYGON, T_SECT_POLYGON&> aPolygon;
		Slab.dCoor[1] =  Bc/2.0 + Sg, Slab.dCoor[2]=dH-(Hh+tc), aPolygon.Add(Slab);
		Slab.dCoor[1] = -Bc/2.0 + Sg, Slab.dCoor[2]=dH-(Hh+tc), aPolygon.Add(Slab);
		Slab.dCoor[1] = -Bc/2.0 + Sg, Slab.dCoor[2]=dH-Hh,      aPolygon.Add(Slab);
		Slab.dCoor[1] =  Bc/2.0 + Sg, Slab.dCoor[2]=dH-Hh,      aPolygon.Add(Slab);
		
		// step2
		CArray<T_2COOR, T_2COOR&> aVertex;
		int nSize = aPolygon.GetSize();
		aVertex.SetSize(nSize);
		for(int i=0; i<nSize; ++i)
		{
			aVertex[i].x = aPolygon[i].dCoor[1];
			aVertex[i].y = aPolygon[i].dCoor[2];
		}

		// step3
		CCMPolygon *pPoly2 = new CCMPolygon(CCM_BORDER, CCM_FILL);
		for(int i=0; i<nSize; ++i)
		{
			pPoly2->Add(SCMPoint2D(aVertex[i].x,aVertex[i].y));
		}
		if(pPoly2->m_aVertex.GetSize()>0)
		{
			aGeom.Add(pPoly2);
			aGeomPart2[0].Add(pPoly2);
		}
		else delete pPoly2;
	}
	/*
	if(bJ)
	{
		aGeomPart1[1].Add(aGeom[0]);
		aGeomPart2[1].Add(pPoly);
	}
	else
	{
		aGeomPart1[0].Add(aGeom[0]);
		aGeomPart2[0].Add(pPoly2);
	}
	*/
}

// DATE : 2002.11. by KYE-HONG
// FUNC : Composite Section CI,CT
void ICMGeomMaker::MakePictureCompo(T_SECT_D& SectData, CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ)
{
	int i; 
	CArray<T_2COOR,T_2COOR&>Comp1;
	CArray<T_2COOR,T_2COOR&>Comp2;
	CArray<T_2COOR,T_2COOR&>Comp3;
	CArray<T_2COOR,T_2COOR&>Comp4;
	
	BOOL bTapJ = (m_pDataSrc->nStype==D_SECT_TYPE_TAPERED && bJ);
	CDBDoc::GetDocPoint()->makeCompShape(SectData,Comp1,Comp2,Comp3,Comp4,bTapJ);
	
	CCMPolygon *pC1Poly=  new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pC2Poly=  new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pC3Poly=  new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pC4Poly=  new CCMPolygon(CCM_BORDER, CCM_FILL);
	
	int nC1 = Comp1.GetSize();
	int nC2 = Comp2.GetSize();
	int nC3 = Comp3.GetSize();
	int nC4 = Comp4.GetSize();

	for(i=0; i<nC1; i++)pC1Poly->Add(SCMPoint2D(Comp1[i].x,-Comp1[i].y));
	for(i=0; i<nC2; i++)pC2Poly->Add(SCMPoint2D(Comp2[i].x,-Comp2[i].y));
	for(i=0; i<nC3; i++)pC3Poly->Add(SCMPoint2D(Comp3[i].x,-Comp3[i].y));
	for(i=0; i<nC4; i++)pC4Poly->Add(SCMPoint2D(Comp4[i].x,-Comp4[i].y));
	
	if(pC1Poly->m_aVertex.GetSize()) aGeom.Add(pC1Poly);
	else  delete pC1Poly;
	if(pC2Poly->m_aVertex.GetSize()) aGeom.Add(pC2Poly);
	else  delete pC2Poly;
	if(pC3Poly->m_aVertex.GetSize()) aGeom.Add(pC3Poly);
	else  delete pC3Poly;
	if(pC4Poly->m_aVertex.GetSize()) aGeom.Add(pC4Poly);
	else  delete pC4Poly;

	if(bTapJ)
	{
		aGeomPart1[1].Add(pC1Poly); // girder
		aGeomPart2[1].Add(pC2Poly); // slab
	}
	else
	{
		aGeomPart1[0].Add(pC1Poly); // girder
		aGeomPart2[0].Add(pC2Poly); // slab
	}
}

void ICMGeomMaker::MakePictureStlGirderBox(CCMGeom2DArray &aGeom, BOOL bJ)
{
	CCMPolygon *pPolygon1 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon2 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon3 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon4 = new CCMPolygon(CCM_BORDER, CCM_FILL);

	double Top=0.0, Bot=0.0;
	double B1=0.0, B2=0.0, B3=0.0, B4=0.0, B5=0.0, B6=0.0;
	double H=0.0, t1=0.0, t2=0.0, tw1=0.0, tw2=0.0;

	BOOL bTapJ = (m_pDataSrc->nStype==D_SECT_TYPE_TAPERED && bJ);

	if(bTapJ)
	{
		Top = m_pDataSrc->SectAfter.SectJ.Size[0];
		Bot = m_pDataSrc->SectAfter.SectJ.Size[1];

		B1  = m_pDataSrc->SectBefore.SectJ.Size[0];
		B2  = m_pDataSrc->SectBefore.SectJ.Size[1];
		B3  = m_pDataSrc->SectBefore.SectJ.Size[2];
		B4  = m_pDataSrc->SectBefore.SectJ.Size[3];
		B5  = m_pDataSrc->SectBefore.SectJ.Size[4];
		B6  = m_pDataSrc->SectBefore.SectJ.Size[5];
		H   = m_pDataSrc->SectBefore.SectJ.Size[6];
		t1  = m_pDataSrc->SectBefore.SectJ.Size[7];
		t2  = m_pDataSrc->SectBefore.SectJ.Size[8];
		tw1 = m_pDataSrc->SectBefore.SectJ.Size[9];
		tw2 = m_pDataSrc->SectBefore.SectJ.Size[10];
	}
	else
	{
		Top = m_pDataSrc->SectAfter.SectI.Size[0];
		Bot = m_pDataSrc->SectAfter.SectI.Size[1];

		B1  = m_pDataSrc->SectBefore.SectI.Size[0];
		B2  = m_pDataSrc->SectBefore.SectI.Size[1];
		B3  = m_pDataSrc->SectBefore.SectI.Size[2];
		B4  = m_pDataSrc->SectBefore.SectI.Size[3];
		B5  = m_pDataSrc->SectBefore.SectI.Size[4];
		B6  = m_pDataSrc->SectBefore.SectI.Size[5];
		H   = m_pDataSrc->SectBefore.SectI.Size[6];
		t1  = m_pDataSrc->SectBefore.SectI.Size[7];
		t2  = m_pDataSrc->SectBefore.SectI.Size[8];
		tw1 = m_pDataSrc->SectBefore.SectI.Size[9];
		tw2 = m_pDataSrc->SectBefore.SectI.Size[10];
	}	

	double tempx = 0.0;
	double tempy = 0.0;
	double twp_L = 0.0;
	double twp_R = 0.0;

	// Top Flange
	tempx = Top;          tempy = 0.0; pPolygon1->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1+B2+B3; tempy = 0.0; pPolygon1->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1+B2+B3; tempy = t1;  pPolygon1->Add(SCMPoint2D(tempx, tempy));
	tempx = Top;          tempy = t1;  pPolygon1->Add(SCMPoint2D(tempx, tempy));
	aGeom.Add(pPolygon1);

	// Bottom Flange
	tempx = Bot;          tempy = t1+H;    pPolygon2->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B4+B5+B6; tempy = t1+H;    pPolygon2->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B4+B5+B6; tempy = t1+H+t2; pPolygon2->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot;          tempy = t1+H+t2; pPolygon2->Add(SCMPoint2D(tempx, tempy));
	aGeom.Add(pPolygon2);

	// Left Web
	// 1. Web 의 높이가 한변을 이루고 있는 직각 삼각형의 한변을 구한다. (Top + B1) - (Bot + B4) 
	double dSide_L = fabs((Top + B1) - (Bot + B4));

	// 2. 비례식을 이용해서 변환된 Web 의 길이를 구한다.
	if(H > 0.0)
	{
		twp_L = tw1/H * sqrt(H*H + dSide_L*dSide_L);
	}

	tempx = Top+B1-twp_L; tempy = t1;   pPolygon3->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1;       tempy = t1;   pPolygon3->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B4;       tempy = t1+H; pPolygon3->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B4-twp_L; tempy = t1+H; pPolygon3->Add(SCMPoint2D(tempx, tempy));
	aGeom.Add(pPolygon3);
	
	// Right Web
	// 1. Web 의 높이가 한변을 이루고 있는 직각 삼각형의 한변을 구한다. (Top + B1 + B2) - (Bot + B4 + B5) 
	double dSide_R = fabs((Top + B1 + B2) - (Bot + B4 + B5));
	
	// 2. 비례식을 이용해서 변환된 Web 의 길이를 구한다.
	if(H > 0.0)
	{
		twp_R = tw2/H * sqrt(H*H + dSide_R*dSide_R);
	}
	
	tempx = Top+B1+B2;       tempy = t1;   pPolygon4->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1+B2+twp_R; tempy = t1;   pPolygon4->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B4+B5+twp_R; tempy = t1+H; pPolygon4->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B4+B5;       tempy = t1+H; pPolygon4->Add(SCMPoint2D(tempx, tempy));
	aGeom.Add(pPolygon4);

	T_SECT_SOD_STIFFNER_B Size; Size.Initialize();
	Size.Top = Top;
	Size.Bot = Bot;
	Size.B1 = B1;
	Size.B2 = B2;
	Size.B3 = B3;
	Size.B4 = B4;
	Size.B5 = B5;
	Size.B6 = B6;
	Size.H = H;
	Size.t1 = t1;
	Size.t2 = t2;
	Size.twp_L = twp_L;
	Size.twp_R = twp_R;
	MakePictureSODStiffner_B(aGeom, Size, 0.0);
}

void ICMGeomMaker::MakePictureStlGirderI(CCMGeom2DArray &aGeom, BOOL bJ)  
{
	CCMPolygon *pPolygon1 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon2 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon3 = new CCMPolygon(CCM_BORDER, CCM_FILL);

	double Top=0.0, Bot=0.0;
	double B1=0.0, B2=0.0, B3=0.0, B4=0.0;
	double H=0.0, t1=0.0, t2=0.0, tw=0.0;

	BOOL bTapJ = (m_pDataSrc->nStype==D_SECT_TYPE_TAPERED && bJ);

	if(bTapJ)
	{
		Top = m_pDataSrc->SectAfter.SectJ.Size[0];
		Bot = m_pDataSrc->SectAfter.SectJ.Size[1];

		B1 = m_pDataSrc->SectBefore.SectJ.Size[0];
		B2 = m_pDataSrc->SectBefore.SectJ.Size[1];
		B3 = m_pDataSrc->SectBefore.SectJ.Size[2];
		B4 = m_pDataSrc->SectBefore.SectJ.Size[3];
		H  = m_pDataSrc->SectBefore.SectJ.Size[4];
		t1 = m_pDataSrc->SectBefore.SectJ.Size[5];
		t2 = m_pDataSrc->SectBefore.SectJ.Size[6];
		tw = m_pDataSrc->SectBefore.SectJ.Size[7];
	}
	else
	{
		Top = m_pDataSrc->SectAfter.SectI.Size[0];
		Bot = m_pDataSrc->SectAfter.SectI.Size[1];

		B1 = m_pDataSrc->SectBefore.SectI.Size[0];
		B2 = m_pDataSrc->SectBefore.SectI.Size[1];
		B3 = m_pDataSrc->SectBefore.SectI.Size[2];
		B4 = m_pDataSrc->SectBefore.SectI.Size[3];
		H  = m_pDataSrc->SectBefore.SectI.Size[4];
		t1 = m_pDataSrc->SectBefore.SectI.Size[5];
		t2 = m_pDataSrc->SectBefore.SectI.Size[6];
		tw = m_pDataSrc->SectBefore.SectI.Size[7];
	}	
	
	double tempx = 0.0;
	double tempy = 0.0;
	
	// Top Flange
	tempx = Top;       tempy = 0.0; pPolygon1->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1+B2; tempy = 0.0; pPolygon1->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1+B2; tempy = t1;  pPolygon1->Add(SCMPoint2D(tempx, tempy));
	tempx = Top;       tempy = t1;  pPolygon1->Add(SCMPoint2D(tempx, tempy));
	aGeom.Add(pPolygon1);
	
	// Bottom Flange
	tempx = Bot;       tempy = t1+H;    pPolygon2->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B3+B4; tempy = t1+H;    pPolygon2->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B3+B4; tempy = t1+H+t2; pPolygon2->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot;       tempy = t1+H+t2; pPolygon2->Add(SCMPoint2D(tempx, tempy));
	aGeom.Add(pPolygon2);
	
	// Web
	// 1. Web 의 높이가 한변을 이루고 있는 직각 삼각형의 한변을 구한다. (Top + B1) - (Bot + B3) 
	double twp = 0.0;
	double dSide = fabs((Top + B1) - (Bot + B3));
	
	// 2. 비례식을 이용해서 변환된 Web 의 길이를 구한다.
	if(H > 0.0)
	{
		twp = tw/H * sqrt(H*H + dSide*dSide);
	}
		
	tempx = Top+B1-(twp/2); tempy = t1;   pPolygon3->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1+(twp/2); tempy = t1;   pPolygon3->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B3+(twp/2); tempy = t1+H; pPolygon3->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B3-(twp/2); tempy = t1+H; pPolygon3->Add(SCMPoint2D(tempx, tempy));
	aGeom.Add(pPolygon3);

	T_SECT_SOD_STIFFNER_I Size; Size.Initialize();
	Size.Top = Top;
	Size.Bot = Bot;
	Size.B1 = B1;
	Size.B2 = B2;
	Size.B3 = B3;
	Size.B4 = B4;
	Size.H = H;
	Size.t1 = t1;
	Size.t2 = t2;
	Size.twp = twp;
	MakePictureSODStiffner_I(aGeom, Size, 0.0);
	MakePictureRivet_I(aGeom, Size);
}

void ICMGeomMaker::MakePictureStlGirderMCell(CCMGeom2DArray &aGeom, BOOL bJ)  
{
	CCMPolygon *pPolygon;

	double dTol = 1.0e-10;

	BOOL bTapJ = (m_pDataSrc->nStype==D_SECT_TYPE_TAPERED && bJ);
	
	double B1=0.0, B2=0.0, B3=0.0, B4=0.0, B5=0.0, B6=0.0;
	double Br1=0.0, Br2=0.0, Br3=0.0, Br4=0.0, Br5=0.0;
	double H=0.0, tf1=0.0, tf2=0.0, tw1=0.0, tw2=0.0;
	double rh=0.0, rb=0.0, rt=0.0, rhr=0.0, rbr=0.0, rtr=0.0;
	double rh1=0.0, rh2=0.0, rh3=0.0, rt1=0.0, rt2=0.0, rhr1=0.0, rhr2=0.0, rhr3=0.0, rtr1=0.0, rtr2=0.0;
	double jh1=0.0, jh2=0.0, jb1=0.0, jb2=0.0, jhr1=0.0, jhr2=0.0, jbr1=0.0, jbr2=0.0;
	BOOL   J1=FALSE, J2=FALSE, Jr1=FALSE, Jr2=FALSE;

	if(m_pDataSrc->SectBefore.nFlangeShape == 0 ||  // General 
		 m_pDataSrc->SectBefore.nFlangeShape == 1 ||  // Reinforced-1
		 m_pDataSrc->SectBefore.nFlangeShape == 2)    // Reinforced-2
	{
		if(bTapJ)
		{
			B1  = m_pDataSrc->SectBefore.SectJ.Size[0];
			B2  = m_pDataSrc->SectBefore.SectJ.Size[1];
			B3  = m_pDataSrc->SectBefore.SectJ.Size[2];
			B4  = m_pDataSrc->SectBefore.SectJ.Size[3];
			B5  = m_pDataSrc->SectBefore.SectJ.Size[4];
			B6  = m_pDataSrc->SectBefore.SectJ.Size[5];
			Br1 = m_pDataSrc->SectBefore.SectJ.Size[6];
			Br2 = m_pDataSrc->SectBefore.SectJ.Size[7];
			Br3 = m_pDataSrc->SectBefore.SectJ.Size[8];
			Br4 = m_pDataSrc->SectBefore.SectJ.Size[9];
			Br5 = m_pDataSrc->SectBefore.SectJ.Size[10];
			H   = m_pDataSrc->SectBefore.SectJ.Size[11];
			tf1 = m_pDataSrc->SectBefore.SectJ.Size[12];
			tw1 = m_pDataSrc->SectBefore.SectJ.Size[13];	
			tf2 = m_pDataSrc->SectBefore.SectJ.Size[14];
			tw2 = m_pDataSrc->SectBefore.SectJ.Size[15];

			if(m_pDataSrc->SectBefore.nFlangeShape == 1)
			{
				rh  = m_pDataSrc->SectBefore.SectJ.Size[16];
				rt  = m_pDataSrc->SectBefore.SectJ.Size[17];
				rb  = m_pDataSrc->SectBefore.SectJ.Size[18];		
				rhr = m_pDataSrc->SectBefore.SectJ.Size[19];
				rtr = m_pDataSrc->SectBefore.SectJ.Size[20];
				rbr = m_pDataSrc->SectBefore.SectJ.Size[21];
			}
			
			if(m_pDataSrc->SectBefore.nFlangeShape == 2)
			{
				rh1  = m_pDataSrc->SectBefore.SectJ.Size[16];
				rh2  = m_pDataSrc->SectBefore.SectJ.Size[17];
				rh3  = m_pDataSrc->SectBefore.SectJ.Size[18];
				rb   = m_pDataSrc->SectBefore.SectJ.Size[19];
				rt1  = m_pDataSrc->SectBefore.SectJ.Size[20];
				rt2  = m_pDataSrc->SectBefore.SectJ.Size[21];
				rhr1 = m_pDataSrc->SectBefore.SectJ.Size[22];
				rhr2 = m_pDataSrc->SectBefore.SectJ.Size[23];
				rhr3 = m_pDataSrc->SectBefore.SectJ.Size[24];
				rbr  = m_pDataSrc->SectBefore.SectJ.Size[25];
				rtr1 = m_pDataSrc->SectBefore.SectJ.Size[26];
				rtr2 = m_pDataSrc->SectBefore.SectJ.Size[27];
			}
		}
		else
		{
			B1  = m_pDataSrc->SectBefore.SectI.Size[0];
			B2  = m_pDataSrc->SectBefore.SectI.Size[1];
			B3  = m_pDataSrc->SectBefore.SectI.Size[2];
			B4  = m_pDataSrc->SectBefore.SectI.Size[3];
			B5  = m_pDataSrc->SectBefore.SectI.Size[4];
			B6  = m_pDataSrc->SectBefore.SectI.Size[5];
			Br1 = m_pDataSrc->SectBefore.SectI.Size[6];
			Br2 = m_pDataSrc->SectBefore.SectI.Size[7];
			Br3 = m_pDataSrc->SectBefore.SectI.Size[8];
			Br4 = m_pDataSrc->SectBefore.SectI.Size[9];
			Br5 = m_pDataSrc->SectBefore.SectI.Size[10];
			H   = m_pDataSrc->SectBefore.SectI.Size[11];
			tf1 = m_pDataSrc->SectBefore.SectI.Size[12];
			tw1 = m_pDataSrc->SectBefore.SectI.Size[13];	
			tf2 = m_pDataSrc->SectBefore.SectI.Size[14];
			tw2 = m_pDataSrc->SectBefore.SectI.Size[15];

			if(m_pDataSrc->SectBefore.nFlangeShape == 1)
			{
				rh  = m_pDataSrc->SectBefore.SectI.Size[16];
				rt  = m_pDataSrc->SectBefore.SectI.Size[17];
				rb  = m_pDataSrc->SectBefore.SectI.Size[18];		
				rhr = m_pDataSrc->SectBefore.SectI.Size[19];
				rtr = m_pDataSrc->SectBefore.SectI.Size[20];
				rbr = m_pDataSrc->SectBefore.SectI.Size[21];
			}

			if(m_pDataSrc->SectBefore.nFlangeShape == 2)
			{
				rh1  = m_pDataSrc->SectBefore.SectI.Size[16];
				rh2  = m_pDataSrc->SectBefore.SectI.Size[17];
				rh3  = m_pDataSrc->SectBefore.SectI.Size[18];
				rb   = m_pDataSrc->SectBefore.SectI.Size[19];
				rt1  = m_pDataSrc->SectBefore.SectI.Size[20];
				rt2  = m_pDataSrc->SectBefore.SectI.Size[21];
				rhr1 = m_pDataSrc->SectBefore.SectI.Size[22];
				rhr2 = m_pDataSrc->SectBefore.SectI.Size[23];
				rhr3 = m_pDataSrc->SectBefore.SectI.Size[24];
				rbr  = m_pDataSrc->SectBefore.SectI.Size[25];
				rtr1 = m_pDataSrc->SectBefore.SectI.Size[26];
				rtr2 = m_pDataSrc->SectBefore.SectI.Size[27];
			}
		}
	}
	else if(m_pDataSrc->SectBefore.nFlangeShape == 3) // Closed
	{
		if(bTapJ)
		{
			B1   = m_pDataSrc->SectBefore.SectJ.Size[0];
			B2   = m_pDataSrc->SectBefore.SectJ.Size[1];
			B3   = m_pDataSrc->SectBefore.SectJ.Size[2];
			B4   = m_pDataSrc->SectBefore.SectJ.Size[3];
			Br1  = m_pDataSrc->SectBefore.SectJ.Size[4];
			Br2  = m_pDataSrc->SectBefore.SectJ.Size[5];
			Br3  = m_pDataSrc->SectBefore.SectJ.Size[6];
			H    = m_pDataSrc->SectBefore.SectJ.Size[7];
			tf1  = m_pDataSrc->SectBefore.SectJ.Size[8];
			tw1  = m_pDataSrc->SectBefore.SectJ.Size[9];
			tf2  = m_pDataSrc->SectBefore.SectJ.Size[10];
			tw2  = m_pDataSrc->SectBefore.SectJ.Size[11];
			jh1  = m_pDataSrc->SectBefore.SectJ.Size[12];
			jb1  = m_pDataSrc->SectBefore.SectJ.Size[13];
			jh2  = m_pDataSrc->SectBefore.SectJ.Size[14];
			jb2  = m_pDataSrc->SectBefore.SectJ.Size[15];		
			jhr1 = m_pDataSrc->SectBefore.SectJ.Size[16];
			jbr1 = m_pDataSrc->SectBefore.SectJ.Size[17];
			jhr2 = m_pDataSrc->SectBefore.SectJ.Size[18];
			jbr2 = m_pDataSrc->SectBefore.SectJ.Size[19];	
		}
		else
		{
			B1   = m_pDataSrc->SectBefore.SectI.Size[0];
			B2   = m_pDataSrc->SectBefore.SectI.Size[1];
			B3   = m_pDataSrc->SectBefore.SectI.Size[2];
			B4   = m_pDataSrc->SectBefore.SectI.Size[3];
			Br1  = m_pDataSrc->SectBefore.SectI.Size[4];
			Br2  = m_pDataSrc->SectBefore.SectI.Size[5];
			Br3  = m_pDataSrc->SectBefore.SectI.Size[6];
			H    = m_pDataSrc->SectBefore.SectI.Size[7];
			tf1  = m_pDataSrc->SectBefore.SectI.Size[8];
			tw1  = m_pDataSrc->SectBefore.SectI.Size[9];
			tf2  = m_pDataSrc->SectBefore.SectI.Size[10];
			tw2  = m_pDataSrc->SectBefore.SectI.Size[11];
			jh1  = m_pDataSrc->SectBefore.SectI.Size[12];
			jb1  = m_pDataSrc->SectBefore.SectI.Size[13];
			jh2  = m_pDataSrc->SectBefore.SectI.Size[14];
			jb2  = m_pDataSrc->SectBefore.SectI.Size[15];		
			jhr1 = m_pDataSrc->SectBefore.SectI.Size[16];
			jbr1 = m_pDataSrc->SectBefore.SectI.Size[17];
			jhr2 = m_pDataSrc->SectBefore.SectI.Size[18];
			jbr2 = m_pDataSrc->SectBefore.SectI.Size[19];	
		}

		J1  = m_pDataSrc->SectBefore.nJoint & 0x00000001 ? TRUE : FALSE;
		J2  = m_pDataSrc->SectBefore.nJoint & 0x00000002 ? TRUE : FALSE;
		Jr1 = m_pDataSrc->SectBefore.nJoint & 0x00000004 ? TRUE : FALSE;
		Jr2 = m_pDataSrc->SectBefore.nJoint & 0x00000008 ? TRUE : FALSE;
	}
	else
	{
		ASSERT(0);
		return;
	}

	double tempx = 0.0;
	double tempy = 0.0;
	double twp_L = 0.0;
	double twp_R = 0.0;
	double rt2p = 0.0;
	double rtr2p = 0.0;
	double Top = 0.0;
	double Bot = 0.0;
	double sideB = 0.0;
	double sideH = 0.0;	
	double sideBr = 0.0;
	double sideHr = 0.0;	
	double Joint1 = 0.0;
	double Joint2 = 0.0;
	double Joint1r = 0.0;
	double Joint2r = 0.0;

	if(m_pDataSrc->SectBefore.nFlangeShape == 0 ||
		 m_pDataSrc->SectBefore.nFlangeShape == 1 ||
		 m_pDataSrc->SectBefore.nFlangeShape == 2)
	{
		if(m_pDataSrc->SectBefore.nCellNum == 0) // 0 Cell 
		{
			if(m_pDataSrc->SectBefore.nCellType == 0) // Left
			{
				Bot = B1+B3-B4;

				// Top Flange
				pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
				tempx = 0.0;          tempy = 0.0;   pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = B1+B2;        tempy = 0.0;   pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = B1+B2;        tempy = tf1;   pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = 0.0;          tempy = tf1;   pPolygon->Add(SCMPoint2D(tempx, tempy));
				aGeom.Add(pPolygon);

				// Bot Flange
				pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
				tempx = Bot;           tempy = tf1+H;      pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Bot+B4+B5;     tempy = tf1+H;      pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Bot+B4+B5;     tempy = tf1+H+tf2;  pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Bot;           tempy = tf1+H+tf2;  pPolygon->Add(SCMPoint2D(tempx, tempy));
				aGeom.Add(pPolygon);

				// Web
				if(H > 0.0)
				{
					twp_L = tw1/H * sqrt(H*H + B3*B3);
				}

				pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
				tempx = B1;             tempy = tf1;     pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = B1+twp_L;       tempy = tf1;     pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = B1+B3+twp_L;    tempy = tf1+H;   pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = B1+B3;          tempy = tf1+H;   pPolygon->Add(SCMPoint2D(tempx, tempy));
				aGeom.Add(pPolygon);

				if(m_pDataSrc->SectBefore.nFlangeShape == 1) // Reinforced-1
				{
					pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
					tempx = rb;       tempy = tf1;      pPolygon->Add(SCMPoint2D(tempx, tempy));
					tempx = rb+rt;    tempy = tf1;      pPolygon->Add(SCMPoint2D(tempx, tempy));
					tempx = rb+rt;    tempy = tf1+rh;   pPolygon->Add(SCMPoint2D(tempx, tempy));
					tempx = rb;       tempy = tf1+rh;   pPolygon->Add(SCMPoint2D(tempx, tempy));
					aGeom.Add(pPolygon);
				}

				if(m_pDataSrc->SectBefore.nFlangeShape == 2) // Reinforced-2
				{
					pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
					tempx = rb;        tempy = tf1;           pPolygon->Add(SCMPoint2D(tempx, tempy));
					tempx = rb+rt1;    tempy = tf1;           pPolygon->Add(SCMPoint2D(tempx, tempy));
					tempx = rb+rt1;    tempy = tf1+rh1+rh2;   pPolygon->Add(SCMPoint2D(tempx, tempy));
					tempx = rb;        tempy = tf1+rh1+rh2;   pPolygon->Add(SCMPoint2D(tempx, tempy));
					aGeom.Add(pPolygon);

					if(H > 0.0)
					{
						sideB = B1-rb-rt1+((B3*rh3)/H);
						sideH = rh3-rh1;

						if(sideB > 0.0)
						{
							rt2p = rt2/sideB * sqrt(sideB*sideB + sideH*sideH);
						}					

						double line1[2][2], line2[2][2], cross1[2], cross2[2];
						cross1[0] = cross1[1] = cross2[0] = cross2[1] = 0.0;

						line1[0][0] = rb+rt1;                     line2[0][0] = B1;
						line1[0][1] = tf1+rh1-rt2p;			  				line2[0][1] = tf1; 
						line1[1][0] = B1+((B3*rh3)/H);      			line2[1][0] = B1+B3;
						line1[1][1] = tf1+rh3-rt2p;								line2[1][1] = tf1+H;
						CMathFunc::mathLineLineCross2D(line1, line2, cross1);

						line1[0][0] = rb+rt1;               line2[0][0] = B1;
						line1[0][1] = tf1+rh1;				      line2[0][1] = tf1; 
						line1[1][0] = B1+((B3*rh3)/H);      line2[1][0] = B1+B3;
						line1[1][1] = tf1+rh3;				      line2[1][1] = tf1+H;
						CMathFunc::mathLineLineCross2D(line1, line2, cross2);

						pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
						tempx = rb+rt1;         tempy = tf1+rh1-rt2p;    pPolygon->Add(SCMPoint2D(tempx, tempy));
						tempx = cross1[0];      tempy = cross1[1];       pPolygon->Add(SCMPoint2D(tempx, tempy));
						tempx = cross2[0];      tempy = cross2[1];       pPolygon->Add(SCMPoint2D(tempx, tempy));
						tempx = rb+rt1;         tempy = tf1+rh1;         pPolygon->Add(SCMPoint2D(tempx, tempy));
						aGeom.Add(pPolygon);
					}
				}
			}
			else // Right
			{
				double Max = __max(Br2, (Br5+Br3));
				Top = Max-Br2;
				Bot = Max-Br5-Br3;
				
				// Top Flange
				pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);				
				tempx = Top;            tempy = 0.0;    pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Top+Br2+Br1;    tempy = 0.0;    pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Top+Br2+Br1;    tempy = tf1;    pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Top;            tempy = tf1;    pPolygon->Add(SCMPoint2D(tempx, tempy));
				aGeom.Add(pPolygon);

				// Bot Flange
				pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
				tempx = Bot;            tempy = tf1+H;      pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Bot+Br5+Br4;    tempy = tf1+H;      pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Bot+Br5+Br4;    tempy = tf1+H+tf2;  pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Bot;            tempy = tf1+H+tf2;  pPolygon->Add(SCMPoint2D(tempx, tempy));
				aGeom.Add(pPolygon);

				// Web
				if(H > 0.0)
				{
					twp_R = tw1/H * sqrt(H*H + Br3*Br3);
				}

				pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
				tempx = Top+Br2-twp_R;    tempy = tf1;      pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Top+Br2;          tempy = tf1;      pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Bot+Br5;          tempy = tf1+H;    pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Bot+Br5-twp_R;    tempy = tf1+H;    pPolygon->Add(SCMPoint2D(tempx, tempy));
				aGeom.Add(pPolygon);
				
				if(m_pDataSrc->SectBefore.nFlangeShape == 1) // Reinforced-1
				{
					pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
					tempx = Top+Br2+Br1-rbr-rtr;       tempy = tf1;       pPolygon->Add(SCMPoint2D(tempx, tempy));
					tempx = Top+Br2+Br1-rbr;           tempy = tf1;       pPolygon->Add(SCMPoint2D(tempx, tempy));
					tempx = Top+Br2+Br1-rbr;           tempy = tf1+rhr;   pPolygon->Add(SCMPoint2D(tempx, tempy));
					tempx = Top+Br2+Br1-rbr-rtr;       tempy = tf1+rhr;   pPolygon->Add(SCMPoint2D(tempx, tempy));
					aGeom.Add(pPolygon);
				}

				if(m_pDataSrc->SectBefore.nFlangeShape == 2) // Reinforced-2
				{
					pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
					tempx = Top+Br2+Br1-rbr-rtr1;       tempy = tf1;             pPolygon->Add(SCMPoint2D(tempx, tempy));
					tempx = Top+Br2+Br1-rbr;            tempy = tf1;             pPolygon->Add(SCMPoint2D(tempx, tempy));
					tempx = Top+Br2+Br1-rbr;            tempy = tf1+rhr1+rhr2;   pPolygon->Add(SCMPoint2D(tempx, tempy));
					tempx = Top+Br2+Br1-rbr-rtr1;       tempy = tf1+rhr1+rhr2;   pPolygon->Add(SCMPoint2D(tempx, tempy));
					aGeom.Add(pPolygon);
					
					if(H > 0.0)
					{
						sideBr = Br1-rbr-rtr1+((Br3*rhr3)/H);
						sideHr = rhr3-rhr1;

						if(sideBr > 0.0)
						{
							rtr2p = rtr2/sideBr * sqrt(sideBr*sideBr + sideHr*sideHr);
						}						

						double line1[2][2], line2[2][2], cross1[2], cross2[2];
						cross1[0] = cross1[1] = cross2[0] = cross2[1] = 0.0;
						
						line1[0][0] = Top+Br2+Br1-rbr-rtr1;               line2[0][0] = Top+Br2;
						line1[0][1] = tf1+rhr1-rtr2p;					       			line2[0][1] = tf1; 
						line1[1][0] = Top+Br2-((Br3*rhr3)/H);       			line2[1][0] = Bot+Br5;
						line1[1][1] = tf1+rhr3-rtr2p;			     						line2[1][1] = tf1+H;
						CMathFunc::mathLineLineCross2D(line1, line2, cross1);
						
						line1[0][0] = Top+Br2+Br1-rbr-rtr1;               line2[0][0] = Top+Br2;
						line1[0][1] = tf1+rhr1;  							      			line2[0][1] = tf1; 
						line1[1][0] = Top+Br2-((Br3*rhr3)/H);       			line2[1][0] = Bot+Br5;
						line1[1][1] = tf1+rhr3;	   			      						line2[1][1] = tf1+H;
						CMathFunc::mathLineLineCross2D(line1, line2, cross2);

						pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
						tempx = Top+Br2+Br1-rbr-rtr1;         tempy = tf1+rhr1-rtr2p;  pPolygon->Add(SCMPoint2D(tempx, tempy));
						tempx = cross1[0];                    tempy = cross1[1];       pPolygon->Add(SCMPoint2D(tempx, tempy));
						tempx = cross2[0];                    tempy = cross2[1];       pPolygon->Add(SCMPoint2D(tempx, tempy));
						tempx = Top+Br2+Br1-rbr-rtr1;         tempy = tf1+rhr1;        pPolygon->Add(SCMPoint2D(tempx, tempy));
						aGeom.Add(pPolygon);
					}
				}
			}
		}
		else
		{
			int nCellNum = m_pDataSrc->SectBefore.nCellNum;
			int nWebNum = m_pDataSrc->SectBefore.nCellNum - 1; // 반복되는 Web 수
			int nDuplCell = nCellNum - 2;

			if(nCellNum < 0) nCellNum = 0;
			if(nWebNum  < 0) nWebNum = 0;
			if(nDuplCell < 0) nDuplCell = 0;

			double MaxTop = B1+B2+(nDuplCell*B6)+Br2+Br1;
			double MaxBot = B4+B5+(nDuplCell*B6)+Br5+Br4;
			Bot = B1+B2-B5-B4;
			
			// Top Flange
			pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);				
			tempx = 0.0;      tempy = 0.0;   pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = MaxTop;   tempy = 0.0;   pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = MaxTop;   tempy = tf1;   pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = 0.0;      tempy = tf1;   pPolygon->Add(SCMPoint2D(tempx, tempy));
			aGeom.Add(pPolygon);

			// Bot Flange
			pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
			tempx = Bot;           tempy = tf1+H;      pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = Bot+MaxBot;    tempy = tf1+H;      pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = Bot+MaxBot;    tempy = tf1+H+tf2;  pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = Bot;           tempy = tf1+H+tf2;  pPolygon->Add(SCMPoint2D(tempx, tempy));
			aGeom.Add(pPolygon);

			// Left Web
			if(H > 0.0)
			{
				twp_L = tw1/H * sqrt(H*H + (B2-B5)*(B2-B5));
			}

			pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
			tempx = B1;              tempy = tf1;     pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = B1+twp_L;        tempy = tf1;     pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = Bot+B4+twp_L;    tempy = tf1+H;   pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = Bot+B4;          tempy = tf1+H;   pPolygon->Add(SCMPoint2D(tempx, tempy));
			aGeom.Add(pPolygon);

			// Right Web
			if(H > 0.0)
			{
				twp_R = tw1/H * sqrt(H*H + (Br2-Br5)*(Br2-Br5));
			}

			pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
			tempx = MaxTop-Br1-twp_R;        tempy = tf1;      pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = MaxTop-Br1;              tempy = tf1;      pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = Bot+MaxBot-Br4;          tempy = tf1+H;    pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = Bot+MaxBot-Br4-twp_R;    tempy = tf1+H;    pPolygon->Add(SCMPoint2D(tempx, tempy));
			aGeom.Add(pPolygon);

			// 반복되는 Web
			for(int i=0; i<nWebNum; i++)
			{
				pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
				tempx = B1+B2+(i*B6)-(tw2*0.5);        tempy = tf1;      pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = B1+B2+(i*B6)+(tw2*0.5);        tempy = tf1;      pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Bot+B4+B5+(i*B6)+(tw2*0.5);    tempy = tf1+H;    pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Bot+B4+B5+(i*B6)-(tw2*0.5);    tempy = tf1+H;    pPolygon->Add(SCMPoint2D(tempx, tempy));
				aGeom.Add(pPolygon);
			}

			if(m_pDataSrc->SectBefore.nFlangeShape == 1) // Reinforced-1
			{
				// Left
				pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
				tempx = rb;       tempy = tf1;      pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = rb+rt;    tempy = tf1;      pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = rb+rt;    tempy = tf1+rh;   pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = rb;       tempy = tf1+rh;   pPolygon->Add(SCMPoint2D(tempx, tempy));
				aGeom.Add(pPolygon);

				// Right 
				pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
				tempx = MaxTop-rbr;       tempy = tf1;       pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = MaxTop-rbr-rtr;   tempy = tf1;       pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = MaxTop-rbr-rtr;   tempy = tf1+rhr;   pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = MaxTop-rbr;       tempy = tf1+rhr;   pPolygon->Add(SCMPoint2D(tempx, tempy));
				aGeom.Add(pPolygon);
			}

			if(m_pDataSrc->SectBefore.nFlangeShape == 2) // Reinforced-2
			{
				// Left
				pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
				tempx = rb;        tempy = tf1;           pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = rb+rt1;    tempy = tf1;           pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = rb+rt1;    tempy = tf1+rh1+rh2;   pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = rb;        tempy = tf1+rh1+rh2;   pPolygon->Add(SCMPoint2D(tempx, tempy));
				aGeom.Add(pPolygon);

				if(H > 0.0)
				{
					sideB = B1-rb-rt1+((B3*rh3)/H);
					sideH = rh3-rh1;

					if(sideB > 0.0)
					{
						rt2p = rt2/sideB * sqrt(sideB*sideB + sideH*sideH);
					}					

					double line1[2][2], line2[2][2], cross1[2], cross2[2];
					cross1[0] = cross1[1] = cross2[0] = cross2[1] = 0.0;

					line1[0][0] = rb+rt1;                     line2[0][0] = B1;
					line1[0][1] = tf1+rh1-rt2p;			  				line2[0][1] = tf1; 
					line1[1][0] = B1+((B3*rh3)/H);      			line2[1][0] = B1+B3;
					line1[1][1] = tf1+rh3-rt2p;								line2[1][1] = tf1+H;
					CMathFunc::mathLineLineCross2D(line1, line2, cross1);

					line1[0][0] = rb+rt1;               line2[0][0] = B1;
					line1[0][1] = tf1+rh1;				      line2[0][1] = tf1; 
					line1[1][0] = B1+((B3*rh3)/H);      line2[1][0] = B1+B3;
					line1[1][1] = tf1+rh3;				      line2[1][1] = tf1+H;
					CMathFunc::mathLineLineCross2D(line1, line2, cross2);

					pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
					tempx = rb+rt1;         tempy = tf1+rh1-rt2p;    pPolygon->Add(SCMPoint2D(tempx, tempy));
					tempx = cross1[0];      tempy = cross1[1];       pPolygon->Add(SCMPoint2D(tempx, tempy));
					tempx = cross2[0];      tempy = cross2[1];       pPolygon->Add(SCMPoint2D(tempx, tempy));
					tempx = rb+rt1;         tempy = tf1+rh1;         pPolygon->Add(SCMPoint2D(tempx, tempy));
					aGeom.Add(pPolygon);
				}

				// Right
				pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
				tempx = MaxTop-rbr-rtr1;     tempy = tf1;             pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = MaxTop-rbr;          tempy = tf1;             pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = MaxTop-rbr;          tempy = tf1+rhr1+rhr2;   pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = MaxTop-rbr-rtr1;     tempy = tf1+rhr1+rhr2;   pPolygon->Add(SCMPoint2D(tempx, tempy));
				aGeom.Add(pPolygon);

				if(H > 0.0)
				{
					sideBr = Br1-rbr-rtr1+((Br3*rhr3)/H);
					sideHr = rhr3-rhr1;

					if(sideBr > 0.0)
					{
						rtr2p = rtr2/sideBr * sqrt(sideBr*sideBr + sideHr*sideHr);
					}						

					double line1[2][2], line2[2][2], cross1[2], cross2[2];
					cross1[0] = cross1[1] = cross2[0] = cross2[1] = 0.0;

					line1[0][0] = MaxTop-rbr-rtr1;               line2[0][0] = MaxTop-Br1;
					line1[0][1] = tf1+rhr1-rtr2p;					     	 line2[0][1] = tf1; 
					line1[1][0] = MaxTop-Br1-((Br3*rhr3)/H);     line2[1][0] = Bot+MaxBot-Br4;
					line1[1][1] = tf1+rhr3-rtr2p;			     			 line2[1][1] = tf1+H;
					CMathFunc::mathLineLineCross2D(line1, line2, cross1);

					line1[0][0] = MaxTop-rbr-rtr1;               line2[0][0] = MaxTop-Br1;
					line1[0][1] = tf1+rhr1;  							       line2[0][1] = tf1; 
					line1[1][0] = MaxTop-Br1-((Br3*rhr3)/H);     line2[1][0] = Bot+MaxBot-Br4;
					line1[1][1] = tf1+rhr3;	   			      			 line2[1][1] = tf1+H;
					CMathFunc::mathLineLineCross2D(line1, line2, cross2);

					pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
					tempx = MaxTop-rbr-rtr1;         tempy = tf1+rhr1-rtr2p;  pPolygon->Add(SCMPoint2D(tempx, tempy));
					tempx = cross1[0];               tempy = cross1[1];       pPolygon->Add(SCMPoint2D(tempx, tempy));
					tempx = cross2[0];               tempy = cross2[1];       pPolygon->Add(SCMPoint2D(tempx, tempy));
					tempx = MaxTop-rbr-rtr1;         tempy = tf1+rhr1;        pPolygon->Add(SCMPoint2D(tempx, tempy));
					aGeom.Add(pPolygon);
				}
			}
		}
	}
	else if(m_pDataSrc->SectBefore.nFlangeShape == 3)
	{
		double OutWebLine1_L[2][2], OutWebLine2_L[2][2], OutWebLine3_L[2][2];
		double OutWebLine1_R[2][2], OutWebLine2_R[2][2], OutWebLine3_R[2][2];
		double InWebLine1_L[2][2], InWebLine2_L[2][2], InWebLine3_L[2][2];
		double InWebLine1_R[2][2], InWebLine2_R[2][2], InWebLine3_R[2][2];
		double InTop_L[2], InJoint1_L[2], InJoint2_L[2], InBot_L[2];
		double InTop_R[2], InJoint1_R[2], InJoint2_R[2], InBot_R[2];
		double line[2][2], temp[2];

		BOOL WebInsideTop_L = FALSE; // Joint 1 이 Top Flange 안쪽에 위치할때
		BOOL WebInsideTop_R = FALSE; // Joint 1 이 Top Flange 안쪽에 위치할때
		BOOL WebInsideBot_L = FALSE; // Joint 2 가 Bot Flange 안쪽에 위치할때
		BOOL WebInsideBot_R = FALSE; // Joint 2 가 Bot Flange 안쪽에 위치할때
		
		if(m_pDataSrc->SectBefore.nCellNum == 0) // 0 Cell 
		{
			if(m_pDataSrc->SectBefore.nCellType == 0) // Left
			{
				double Max = 0.0;
				if(J2) Max = __max(B1, __max(jb1+B3, jb2+B3));
				else   Max = __max(B1, jb1+B3);

				Top = Max-B1;
				Bot = Max-B3;
				Joint1 = Max-jb1-B3;
				if(J2) Joint2 = Max-jb2-B3;
				
				if(Top    < 0.0) Top = 0.0;
				if(Bot    < 0.0) Bot = 0.0;
				if(Joint1 < 0.0) Joint1 = 0.0;
				if(Joint2 < 0.0) Joint2 = 0.0;
				
				if(CCompFunc::CompRealTol(Joint1, Top, dTol)==0 || 
					 CCompFunc::CompRealTol(Joint1, Top, dTol)==1) WebInsideTop_L = TRUE; //if(Joint1 >= Top)

				if(J2)
				{
					if(CCompFunc::CompRealTol(Joint2, Bot, dTol)==0 || 
						 CCompFunc::CompRealTol(Joint2, Bot, dTol)==1) WebInsideBot_L = TRUE; //if(Joint2 >= Bot)
				}
				else
				{
					if(CCompFunc::CompRealTol(Joint1, Bot, dTol)==0 || 
						 CCompFunc::CompRealTol(Joint1, Bot, dTol)==1) WebInsideBot_L = TRUE; //if(Joint1 >= Bot)
				}				
					
				// Web 의 바깥쪽 선
				OutWebLine1_L[0][0] = Top;        OutWebLine1_L[0][1] = 0.0;
				OutWebLine1_L[1][0] = Joint1;     OutWebLine1_L[1][1] = tf1+jh1;

				if(J2)
				{
					OutWebLine2_L[0][0] = Joint1;   OutWebLine2_L[0][1] = tf1+jh1;
					OutWebLine2_L[1][0] = Joint2;   OutWebLine2_L[1][1] = tf1+jh2;
					OutWebLine3_L[0][0] = Joint2;   OutWebLine3_L[0][1] = tf1+jh2;
					OutWebLine3_L[1][0] = Bot;      OutWebLine3_L[1][1] = tf1+H+tf2;
				}
				else
				{							
					OutWebLine3_L[0][0] = Joint1;   OutWebLine3_L[0][1] = tf1+jh1;
					OutWebLine3_L[1][0] = Bot;      OutWebLine3_L[1][1] = tf1+H+tf2;
				}
				
				if(WebInsideTop_L)
				{
					line[0][0] = Top;      line[0][1] = tf1; 
					line[1][0] = Top+B1;   line[1][1] = tf1;
					CMathFunc::mathLineLineCross2D(line, OutWebLine1_L, temp);

					OutWebLine1_L[0][0] = temp[0];
					OutWebLine1_L[0][1] = temp[1];
				}

				if(WebInsideBot_L)
				{
					line[0][0] = Bot;      line[0][1] = tf1+H; 
					line[1][0] = Bot+B3;   line[1][1] = tf1+H;
					CMathFunc::mathLineLineCross2D(line, OutWebLine3_L, temp);

					OutWebLine3_L[1][0] = temp[0];
					OutWebLine3_L[1][1] = temp[1];
				}
				
				// Web 의 안쪽 선
				InWebLine1_L[0][0] = OutWebLine1_L[0][0];    InWebLine1_L[0][1] = OutWebLine1_L[0][1];
				InWebLine1_L[1][0] = OutWebLine1_L[1][0];    InWebLine1_L[1][1] = OutWebLine1_L[1][1];
				GetInWebLine_MCell_Closed(InWebLine1_L, tw1, 0);

				if(J2)
				{
					InWebLine2_L[0][0] = OutWebLine2_L[0][0];    InWebLine2_L[0][1] = OutWebLine2_L[0][1];
					InWebLine2_L[1][0] = OutWebLine2_L[1][0];    InWebLine2_L[1][1] = OutWebLine2_L[1][1];
					GetInWebLine_MCell_Closed(InWebLine2_L, tw1, 0);
				}

				InWebLine3_L[0][0] = OutWebLine3_L[0][0];    InWebLine3_L[0][1] = OutWebLine3_L[0][1];
				InWebLine3_L[1][0] = OutWebLine3_L[1][0];    InWebLine3_L[1][1] = OutWebLine3_L[1][1];
				GetInWebLine_MCell_Closed(InWebLine3_L, tw1, 0);

				// 안쪽 교차점
				line[0][0] = Top;      line[0][1] = tf1; 
				line[1][0] = Top+B1;   line[1][1] = tf1;
				CMathFunc::mathLineLineCross2D(line, InWebLine1_L, InTop_L);
				
				if(J2)
				{
					if(!CMathFunc::mathLineLineCross2D(InWebLine1_L, InWebLine2_L, InJoint1_L))
					{
						InJoint1_L[0] = InWebLine2_L[0][0];
						InJoint1_L[1] = InWebLine2_L[0][1];
					}

					if(!CMathFunc::mathLineLineCross2D(InWebLine2_L, InWebLine3_L, InJoint2_L))
					{
						InJoint2_L[0] = InWebLine3_L[0][0];
						InJoint2_L[1] = InWebLine3_L[0][1];
					}
				}
				else
				{
					if(!CMathFunc::mathLineLineCross2D(InWebLine1_L, InWebLine3_L, InJoint1_L))
					{
						InJoint1_L[0] = InWebLine3_L[0][0];
						InJoint1_L[1] = InWebLine3_L[0][1];
					}
				}

				line[0][0] = Bot;      line[0][1] = tf1+H; 
				line[1][0] = Bot+B3;   line[1][1] = tf1+H;
				CMathFunc::mathLineLineCross2D(line, InWebLine3_L, InBot_L);
				
				// Top Flange
				pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
				tempx = Top;                   tempy = 0.0;                 pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Top+B1;                tempy = 0.0;                 pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Top+B1;                tempy = tf1;                 pPolygon->Add(SCMPoint2D(tempx, tempy));
				if(WebInsideTop_L) 
				{
					tempx = OutWebLine1_L[0][0];     tempy = OutWebLine1_L[0][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
				}
				else
				{
					tempx = InTop_L[0];              tempy = InTop_L[1];            pPolygon->Add(SCMPoint2D(tempx, tempy));
				}					
				aGeom.Add(pPolygon);
				
				// Bot Flange
				pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
				if(WebInsideBot_L) 
				{
					tempx = OutWebLine3_L[1][0];     tempy = OutWebLine3_L[1][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
				}
				else
				{
					tempx = InBot_L[0];              tempy = InBot_L[1];            pPolygon->Add(SCMPoint2D(tempx, tempy));
				}				
				tempx = Bot+B3;                tempy = tf1+H;               pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Bot+B3;                tempy = tf1+H+tf2;           pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Bot;                   tempy = tf1+H+tf2;           pPolygon->Add(SCMPoint2D(tempx, tempy));
				aGeom.Add(pPolygon);

				// Web
				pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
				tempx = OutWebLine1_L[0][0];     tempy = OutWebLine1_L[0][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = OutWebLine1_L[1][0];     tempy = OutWebLine1_L[1][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
				if(J2)
				{
					tempx = OutWebLine3_L[0][0];     tempy = OutWebLine3_L[0][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
				}
				tempx = OutWebLine3_L[1][0];     tempy = OutWebLine3_L[1][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = InBot_L[0];              tempy = InBot_L[1];            pPolygon->Add(SCMPoint2D(tempx, tempy));
				if(J2)
				{
					tempx = InJoint2_L[0];           tempy = InJoint2_L[1];         pPolygon->Add(SCMPoint2D(tempx, tempy));
				}
				tempx = InJoint1_L[0];           tempy = InJoint1_L[1];         pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = InTop_L[0];              tempy = InTop_L[1];            pPolygon->Add(SCMPoint2D(tempx, tempy));
				aGeom.Add(pPolygon);
			}
			else // Right
			{
				if(CCompFunc::CompRealTol(Br1, (Br3+jbr1), dTol)==0 || 
					 CCompFunc::CompRealTol(Br1, (Br3+jbr1), dTol)==1) WebInsideTop_R = TRUE; //if( Br1 >= (Br3+jbr1) ) 

				if(Jr2)
				{
					if(CCompFunc::CompRealTol(Br3, (Br3+jbr2), dTol)==0 || 
						 CCompFunc::CompRealTol(Br3, (Br3+jbr2), dTol)==1) WebInsideBot_R = TRUE; //if( Br3 >= (Br3+jbr2) ) 
				}
				else
				{
					if(CCompFunc::CompRealTol(Br3, (Br3+jbr1), dTol)==0 || 
						 CCompFunc::CompRealTol(Br3, (Br3+jbr1), dTol)==1) WebInsideBot_R = TRUE; //if( Br3 >= (Br3+jbr1) ) 
				}				

				// Web 의 바깥쪽 선
				OutWebLine1_R[0][0] = Br1;          OutWebLine1_R[0][1] = 0.0;
				OutWebLine1_R[1][0] = Br3+jbr1;     OutWebLine1_R[1][1] = tf1+jhr1;
				
				if(Jr2)
				{
					OutWebLine2_R[0][0] = Br3+jbr1;   OutWebLine2_R[0][1] = tf1+jhr1;
					OutWebLine2_R[1][0] = Br3+jbr2;   OutWebLine2_R[1][1] = tf1+jhr2;
					OutWebLine3_R[0][0] = Br3+jbr2;   OutWebLine3_R[0][1] = tf1+jhr2;
					OutWebLine3_R[1][0] = Br3;        OutWebLine3_R[1][1] = tf1+H+tf2;
				}
				else
				{							
					OutWebLine3_R[0][0] = Br3+jbr1;   OutWebLine3_R[0][1] = tf1+jhr1;
					OutWebLine3_R[1][0] = Br3;        OutWebLine3_R[1][1] = tf1+H+tf2;
				}

				if(WebInsideTop_R)
				{
					line[0][0] = 0.0;      line[0][1] = tf1; 
					line[1][0] = Br1;      line[1][1] = tf1;
					CMathFunc::mathLineLineCross2D(line, OutWebLine1_R, temp);

					OutWebLine1_R[0][0] = temp[0];
					OutWebLine1_R[0][1] = temp[1];
				}

				if(WebInsideBot_R)
				{
					line[0][0] = 0.0;      line[0][1] = tf1+H; 
					line[1][0] = Br3;      line[1][1] = tf1+H;
					CMathFunc::mathLineLineCross2D(line, OutWebLine3_R, temp);

					OutWebLine3_R[1][0] = temp[0];
					OutWebLine3_R[1][1] = temp[1];
				}

				// Web 의 안쪽 선
				InWebLine1_R[0][0] = OutWebLine1_R[0][0];    InWebLine1_R[0][1] = OutWebLine1_R[0][1];
				InWebLine1_R[1][0] = OutWebLine1_R[1][0];    InWebLine1_R[1][1] = OutWebLine1_R[1][1];
				GetInWebLine_MCell_Closed(InWebLine1_R, tw1, 1);

				if(Jr2)
				{
					InWebLine2_R[0][0] = OutWebLine2_R[0][0];    InWebLine2_R[0][1] = OutWebLine2_R[0][1];
					InWebLine2_R[1][0] = OutWebLine2_R[1][0];    InWebLine2_R[1][1] = OutWebLine2_R[1][1];
					GetInWebLine_MCell_Closed(InWebLine2_R, tw1, 1);
				}

				InWebLine3_R[0][0] = OutWebLine3_R[0][0];    InWebLine3_R[0][1] = OutWebLine3_R[0][1];
				InWebLine3_R[1][0] = OutWebLine3_R[1][0];    InWebLine3_R[1][1] = OutWebLine3_R[1][1];
				GetInWebLine_MCell_Closed(InWebLine3_R, tw1, 1);
				
				// 안쪽 교차점
				line[0][0] = 0.0;      line[0][1] = tf1; 
				line[1][0] = Br1;      line[1][1] = tf1;
				CMathFunc::mathLineLineCross2D(line, InWebLine1_R, InTop_R);

				if(Jr2)
				{
					if(!CMathFunc::mathLineLineCross2D(InWebLine1_R, InWebLine2_R, InJoint1_R))
					{
						InJoint1_R[0] = InWebLine2_R[0][0];
						InJoint1_R[1] = InWebLine2_R[0][1];
					}

					if(!CMathFunc::mathLineLineCross2D(InWebLine2_R, InWebLine3_R, InJoint2_R))
					{
						InJoint2_R[0] = InWebLine3_R[0][0];
						InJoint2_R[1] = InWebLine3_R[0][1];
					}
				}
				else
				{
					if(!CMathFunc::mathLineLineCross2D(InWebLine1_R, InWebLine3_R, InJoint1_R))
					{
						InJoint1_R[0] = InWebLine3_R[0][0];
						InJoint1_R[1] = InWebLine3_R[0][1];
					}
				}

				line[0][0] = 0.0;      line[0][1] = tf1+H; 
				line[1][0] = Br3;      line[1][1] = tf1+H;
				CMathFunc::mathLineLineCross2D(line, InWebLine3_R, InBot_R);

				// Top Flange
				pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
				tempx = 0.0;                   tempy = 0.0;                 pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Br1;                   tempy = 0.0;                 pPolygon->Add(SCMPoint2D(tempx, tempy));
				if(WebInsideTop_R) 
				{
					tempx = OutWebLine1_R[0][0];     tempy = OutWebLine1_R[0][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
				}
				else
				{
					tempx = InTop_R[0];              tempy = InTop_R[1];            pPolygon->Add(SCMPoint2D(tempx, tempy));
				}					
				tempx = 0.0;                   tempy = tf1;                 pPolygon->Add(SCMPoint2D(tempx, tempy));				
				aGeom.Add(pPolygon);

				// Bot Flange
				pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
				tempx = 0.0;                   tempy = tf1+H;               pPolygon->Add(SCMPoint2D(tempx, tempy));
				if(WebInsideBot_R) 
				{
					tempx = OutWebLine3_R[1][0];     tempy = OutWebLine3_R[1][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
				}
				else
				{
					tempx = InBot_R[0];              tempy = InBot_R[1];            pPolygon->Add(SCMPoint2D(tempx, tempy));
				}				
				tempx = Br3;                   tempy = tf1+H+tf2;           pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = 0.0;                   tempy = tf1+H+tf2;           pPolygon->Add(SCMPoint2D(tempx, tempy));
				aGeom.Add(pPolygon);
				
				// Web
				pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
				tempx = OutWebLine1_R[0][0];     tempy = OutWebLine1_R[0][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = OutWebLine1_R[1][0];     tempy = OutWebLine1_R[1][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
				if(Jr2)
				{
					tempx = OutWebLine3_R[0][0];     tempy = OutWebLine3_R[0][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
				}
				tempx = OutWebLine3_R[1][0];     tempy = OutWebLine3_R[1][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = InBot_R[0];              tempy = InBot_R[1];            pPolygon->Add(SCMPoint2D(tempx, tempy));
				if(Jr2)
				{
					tempx = InJoint2_R[0];           tempy = InJoint2_R[1];         pPolygon->Add(SCMPoint2D(tempx, tempy));
				}
				tempx = InJoint1_R[0];           tempy = InJoint1_R[1];         pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = InTop_R[0];              tempy = InTop_R[1];            pPolygon->Add(SCMPoint2D(tempx, tempy));
				aGeom.Add(pPolygon);
			}
		}
		else
		{
			int nCellNum = m_pDataSrc->SectBefore.nCellNum;
			int nWebNum = m_pDataSrc->SectBefore.nCellNum - 1; // 반복되는 Web 수
			int nDuplCell = nCellNum - 2;

			if(nCellNum < 0) nCellNum = 0;
			if(nWebNum  < 0) nWebNum = 0;
			if(nDuplCell < 0) nDuplCell = 0;

			double Max = 0.0;
			if(J2) Max = __max(B1, __max(jb1+B3, jb2+B3));
			else   Max = __max(B1, jb1+B3);

			Top = Max-B1;
			Bot = Max-B3;
			Joint1 = Max-jb1-B3;
			if(J2) Joint2 = Max-jb2-B3;

			if(Top    < 0.0) Top = 0.0;
			if(Bot    < 0.0) Bot = 0.0;
			if(Joint1 < 0.0) Joint1 = 0.0;
			if(Joint2 < 0.0) Joint2 = 0.0;

			double MaxTop = B1+(nDuplCell*B4)+Br1;
			double MaxBot = B3+(nDuplCell*B4)+Br3;

			if(CCompFunc::CompRealTol(Joint1, Top, dTol)==0 || 
				 CCompFunc::CompRealTol(Joint1, Top, dTol)==1) WebInsideTop_L = TRUE; //if(Joint1 >= Top)

			if(J2)
			{
				if(CCompFunc::CompRealTol(Joint2, Bot, dTol)==0 || 
					 CCompFunc::CompRealTol(Joint2, Bot, dTol)==1) WebInsideBot_L = TRUE; //if(Joint2 >= Bot)
			}
			else
			{
				if(CCompFunc::CompRealTol(Joint1, Bot, dTol)==0 || 
					 CCompFunc::CompRealTol(Joint1, Bot, dTol)==1) WebInsideBot_L = TRUE; //if(Joint1 >= Bot)
			}				

			if(CCompFunc::CompRealTol((Top+MaxTop), (Bot+MaxBot+jbr1), dTol)==0 || 
				 CCompFunc::CompRealTol((Top+MaxTop), (Bot+MaxBot+jbr1), dTol)==1) WebInsideTop_R = TRUE; //if( (Top+MaxTop) >= (Bot+MaxBot+jbr1) ) 

			if(Jr2)
			{
				if(CCompFunc::CompRealTol((Bot+MaxBot), (Bot+MaxBot+jbr2), dTol)==0 || 
					 CCompFunc::CompRealTol((Bot+MaxBot), (Bot+MaxBot+jbr2), dTol)==1) WebInsideBot_R = TRUE; //if( (Bot+MaxBot) >= (Bot+MaxBot+jbr2) ) 
			}
			else
			{
				if(CCompFunc::CompRealTol((Bot+MaxBot), (Bot+MaxBot+jbr1), dTol)==0 ||  
					 CCompFunc::CompRealTol((Bot+MaxBot), (Bot+MaxBot+jbr1), dTol)==1) WebInsideBot_R = TRUE; //if( (Bot+MaxBot) >= (Bot+MaxBot+jbr1) ) 
			}	

			// Left Web 의 바깥쪽 선
			OutWebLine1_L[0][0] = Top;        OutWebLine1_L[0][1] = 0.0;
			OutWebLine1_L[1][0] = Joint1;     OutWebLine1_L[1][1] = tf1+jh1;

			if(J2)
			{
				OutWebLine2_L[0][0] = Joint1;   OutWebLine2_L[0][1] = tf1+jh1;
				OutWebLine2_L[1][0] = Joint2;   OutWebLine2_L[1][1] = tf1+jh2;
				OutWebLine3_L[0][0] = Joint2;   OutWebLine3_L[0][1] = tf1+jh2;
				OutWebLine3_L[1][0] = Bot;      OutWebLine3_L[1][1] = tf1+H+tf2;
			}
			else
			{							
				OutWebLine3_L[0][0] = Joint1;   OutWebLine3_L[0][1] = tf1+jh1;
				OutWebLine3_L[1][0] = Bot;      OutWebLine3_L[1][1] = tf1+H+tf2;
			}

			if(WebInsideTop_L)
			{
				line[0][0] = Top;      line[0][1] = tf1; 
				line[1][0] = Top+B1;   line[1][1] = tf1;
				CMathFunc::mathLineLineCross2D(line, OutWebLine1_L, temp);

				OutWebLine1_L[0][0] = temp[0];
				OutWebLine1_L[0][1] = temp[1];
			}

			if(WebInsideBot_L)
			{
				line[0][0] = Bot;      line[0][1] = tf1+H; 
				line[1][0] = Bot+B3;   line[1][1] = tf1+H;
				CMathFunc::mathLineLineCross2D(line, OutWebLine3_L, temp);

				OutWebLine3_L[1][0] = temp[0];
				OutWebLine3_L[1][1] = temp[1];
			}

			// Left Web 의 안쪽 선
			InWebLine1_L[0][0] = OutWebLine1_L[0][0];    InWebLine1_L[0][1] = OutWebLine1_L[0][1];
			InWebLine1_L[1][0] = OutWebLine1_L[1][0];    InWebLine1_L[1][1] = OutWebLine1_L[1][1];
			GetInWebLine_MCell_Closed(InWebLine1_L, tw1, 0);

			if(J2)
			{
				InWebLine2_L[0][0] = OutWebLine2_L[0][0];    InWebLine2_L[0][1] = OutWebLine2_L[0][1];
				InWebLine2_L[1][0] = OutWebLine2_L[1][0];    InWebLine2_L[1][1] = OutWebLine2_L[1][1];
				GetInWebLine_MCell_Closed(InWebLine2_L, tw1, 0);
			}

			InWebLine3_L[0][0] = OutWebLine3_L[0][0];    InWebLine3_L[0][1] = OutWebLine3_L[0][1];
			InWebLine3_L[1][0] = OutWebLine3_L[1][0];    InWebLine3_L[1][1] = OutWebLine3_L[1][1];
			GetInWebLine_MCell_Closed(InWebLine3_L, tw1, 0);

			// 안쪽 교차점
			line[0][0] = Top;      line[0][1] = tf1; 
			line[1][0] = Top+B1;   line[1][1] = tf1;
			CMathFunc::mathLineLineCross2D(line, InWebLine1_L, InTop_L);

			if(J2)
			{
				if(!CMathFunc::mathLineLineCross2D(InWebLine1_L, InWebLine2_L, InJoint1_L))
				{
					InJoint1_L[0] = InWebLine2_L[0][0];
					InJoint1_L[1] = InWebLine2_L[0][1];
				}

				if(!CMathFunc::mathLineLineCross2D(InWebLine2_L, InWebLine3_L, InJoint2_L))
				{
					InJoint2_L[0] = InWebLine3_L[0][0];
					InJoint2_L[1] = InWebLine3_L[0][1];
				}
			}
			else
			{
				if(!CMathFunc::mathLineLineCross2D(InWebLine1_L, InWebLine3_L, InJoint1_L))
				{
					InJoint1_L[0] = InWebLine3_L[0][0];
					InJoint1_L[1] = InWebLine3_L[0][1];
				}
			}

			line[0][0] = Bot;      line[0][1] = tf1+H; 
			line[1][0] = Bot+B3;   line[1][1] = tf1+H;
			CMathFunc::mathLineLineCross2D(line, InWebLine3_L, InBot_L);

			// Right Web 의 바깥쪽 선
			OutWebLine1_R[0][0] = Top+MaxTop;          OutWebLine1_R[0][1] = 0.0;
			OutWebLine1_R[1][0] = Bot+MaxBot+jbr1;     OutWebLine1_R[1][1] = tf1+jhr1;

			if(Jr2)
			{
				OutWebLine2_R[0][0] = Bot+MaxBot+jbr1;   OutWebLine2_R[0][1] = tf1+jhr1;
				OutWebLine2_R[1][0] = Bot+MaxBot+jbr2;   OutWebLine2_R[1][1] = tf1+jhr2;
				OutWebLine3_R[0][0] = Bot+MaxBot+jbr2;   OutWebLine3_R[0][1] = tf1+jhr2;
				OutWebLine3_R[1][0] = Bot+MaxBot;        OutWebLine3_R[1][1] = tf1+H+tf2;
			}
			else
			{							
				OutWebLine3_R[0][0] = Bot+MaxBot+jbr1;   OutWebLine3_R[0][1] = tf1+jhr1;
				OutWebLine3_R[1][0] = Bot+MaxBot;        OutWebLine3_R[1][1] = tf1+H+tf2;
			}
			
			if(WebInsideTop_R)
			{
				line[0][0] = Top;             line[0][1] = tf1; 
				line[1][0] = Top+MaxTop;      line[1][1] = tf1;
				CMathFunc::mathLineLineCross2D(line, OutWebLine1_R, temp);

				OutWebLine1_R[0][0] = temp[0];
				OutWebLine1_R[0][1] = temp[1];
			}

			if(WebInsideBot_R)
			{
				line[0][0] = Bot;             line[0][1] = tf1+H; 
				line[1][0] = Bot+MaxBot;      line[1][1] = tf1+H;
				CMathFunc::mathLineLineCross2D(line, OutWebLine3_R, temp);

				OutWebLine3_R[1][0] = temp[0];
				OutWebLine3_R[1][1] = temp[1];
			}

			// Right Web 의 안쪽 선
			InWebLine1_R[0][0] = OutWebLine1_R[0][0];    InWebLine1_R[0][1] = OutWebLine1_R[0][1];
			InWebLine1_R[1][0] = OutWebLine1_R[1][0];    InWebLine1_R[1][1] = OutWebLine1_R[1][1];
			GetInWebLine_MCell_Closed(InWebLine1_R, tw1, 1);

			if(Jr2)
			{
				InWebLine2_R[0][0] = OutWebLine2_R[0][0];    InWebLine2_R[0][1] = OutWebLine2_R[0][1];
				InWebLine2_R[1][0] = OutWebLine2_R[1][0];    InWebLine2_R[1][1] = OutWebLine2_R[1][1];
				GetInWebLine_MCell_Closed(InWebLine2_R, tw1, 1);
			}

			InWebLine3_R[0][0] = OutWebLine3_R[0][0];    InWebLine3_R[0][1] = OutWebLine3_R[0][1];
			InWebLine3_R[1][0] = OutWebLine3_R[1][0];    InWebLine3_R[1][1] = OutWebLine3_R[1][1];
			GetInWebLine_MCell_Closed(InWebLine3_R, tw1, 1);
			
			// 안쪽 교차점
			line[0][0] = Top;             line[0][1] = tf1; 
			line[1][0] = Top+MaxTop;      line[1][1] = tf1;
			CMathFunc::mathLineLineCross2D(line, InWebLine1_R, InTop_R);

			if(Jr2)
			{
				if(!CMathFunc::mathLineLineCross2D(InWebLine1_R, InWebLine2_R, InJoint1_R))
				{
					InJoint1_R[0] = InWebLine2_R[0][0];
					InJoint1_R[1] = InWebLine2_R[0][1];
				}

				if(!CMathFunc::mathLineLineCross2D(InWebLine2_R, InWebLine3_R, InJoint2_R))
				{
					InJoint2_R[0] = InWebLine3_R[0][0];
					InJoint2_R[1] = InWebLine3_R[0][1];
				}
			}
			else
			{
				if(!CMathFunc::mathLineLineCross2D(InWebLine1_R, InWebLine3_R, InJoint1_R))
				{
					InJoint1_R[0] = InWebLine3_R[0][0];
					InJoint1_R[1] = InWebLine3_R[0][1];
				}
			}

			line[0][0] = Bot;             line[0][1] = tf1+H; 
			line[1][0] = Bot+MaxBot;      line[1][1] = tf1+H;
			CMathFunc::mathLineLineCross2D(line, InWebLine3_R, InBot_R);
			
			// Top Flange
			pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
			tempx = Top;                   tempy = 0.0;                 pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = Top+MaxTop;            tempy = 0.0;                 pPolygon->Add(SCMPoint2D(tempx, tempy));
			if(WebInsideTop_R) 
			{
				tempx = OutWebLine1_R[0][0];     tempy = OutWebLine1_R[0][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
			}
			else
			{
				tempx = InTop_R[0];              tempy = InTop_R[1];            pPolygon->Add(SCMPoint2D(tempx, tempy));
			}

			if(WebInsideTop_L) 
			{
				tempx = OutWebLine1_L[0][0];     tempy = OutWebLine1_L[0][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
			}
			else
			{
				tempx = InTop_L[0];              tempy = InTop_L[1];            pPolygon->Add(SCMPoint2D(tempx, tempy));
			}					
			aGeom.Add(pPolygon);

			// Bot Flange
			pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
			if(WebInsideBot_L) 
			{
				tempx = OutWebLine3_L[1][0];     tempy = OutWebLine3_L[1][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
			}
			else
			{
				tempx = InBot_L[0];              tempy = InBot_L[1];            pPolygon->Add(SCMPoint2D(tempx, tempy));
			}				
			if(WebInsideBot_R) 
			{
				tempx = OutWebLine3_R[1][0];     tempy = OutWebLine3_R[1][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
			}
			else
			{
				tempx = InBot_R[0];              tempy = InBot_R[1];            pPolygon->Add(SCMPoint2D(tempx, tempy));
			}				
			tempx = Bot+MaxBot;    tempy = tf1+H+tf2;  pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = Bot;           tempy = tf1+H+tf2;  pPolygon->Add(SCMPoint2D(tempx, tempy));
			aGeom.Add(pPolygon);

			// 반복되는 Web
			for(int i=0; i<nWebNum; i++)
			{
				pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
				tempx = Top+B1+(i*B4)-(tw2*0.5);    tempy = tf1;      pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Top+B1+(i*B4)+(tw2*0.5);    tempy = tf1;      pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Bot+B3+(i*B4)+(tw2*0.5);    tempy = tf1+H;    pPolygon->Add(SCMPoint2D(tempx, tempy));
				tempx = Bot+B3+(i*B4)-(tw2*0.5);    tempy = tf1+H;    pPolygon->Add(SCMPoint2D(tempx, tempy));
				aGeom.Add(pPolygon);
			}

			// Left Web
			pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
			tempx = OutWebLine1_L[0][0];     tempy = OutWebLine1_L[0][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = OutWebLine1_L[1][0];     tempy = OutWebLine1_L[1][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
			if(J2)
			{
				tempx = OutWebLine3_L[0][0];     tempy = OutWebLine3_L[0][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
			}
			tempx = OutWebLine3_L[1][0];     tempy = OutWebLine3_L[1][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = InBot_L[0];              tempy = InBot_L[1];            pPolygon->Add(SCMPoint2D(tempx, tempy));
			if(J2)
			{
				tempx = InJoint2_L[0];           tempy = InJoint2_L[1];         pPolygon->Add(SCMPoint2D(tempx, tempy));
			}
			tempx = InJoint1_L[0];           tempy = InJoint1_L[1];         pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = InTop_L[0];              tempy = InTop_L[1];            pPolygon->Add(SCMPoint2D(tempx, tempy));
			aGeom.Add(pPolygon);

			// Right Web
			pPolygon = new CCMPolygon(CCM_BORDER, CCM_FILL);
			tempx = OutWebLine1_R[0][0];     tempy = OutWebLine1_R[0][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = OutWebLine1_R[1][0];     tempy = OutWebLine1_R[1][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
			if(Jr2)
			{
				tempx = OutWebLine3_R[0][0];     tempy = OutWebLine3_R[0][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
			}
			tempx = OutWebLine3_R[1][0];     tempy = OutWebLine3_R[1][1];   pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = InBot_R[0];              tempy = InBot_R[1];            pPolygon->Add(SCMPoint2D(tempx, tempy));
			if(Jr2)
			{
				tempx = InJoint2_R[0];           tempy = InJoint2_R[1];         pPolygon->Add(SCMPoint2D(tempx, tempy));
			}
			tempx = InJoint1_R[0];           tempy = InJoint1_R[1];         pPolygon->Add(SCMPoint2D(tempx, tempy));
			tempx = InTop_R[0];              tempy = InTop_R[1];            pPolygon->Add(SCMPoint2D(tempx, tempy));
			aGeom.Add(pPolygon);
		}
	}
	else
	{
		ASSERT(0);
		return;
	}

	T_SECT_SOD_STIFFNER_MCELL Size; Size.Initialize();
	Size.B1     = B1;
	Size.B2     = B2;
	Size.B3     = B3;
	Size.B4     = B4;
	Size.B5     = B5;
	Size.B6     = B6;
	Size.Br1    = Br1;
	Size.Br2    = Br2;
	Size.Br3    = Br3;
	Size.Br4    = Br4;
	Size.Br5    = Br5;
	Size.H      = H;
	Size.tf1    = tf1;
	Size.tf2    = tf2;
	Size.tw1    = tw1;
	Size.twp_L  = twp_L;
	Size.twp_R  = twp_R;
	Size.tw2    = tw2;
	Size.Top    = Top;
	Size.Bot    = Bot;
	Size.Joint1 = Joint1;
	Size.Joint2 = Joint2;
	Size.rh     = rh;
	Size.rb     = rb;
	Size.rt     = rt;
	Size.rhr    = rhr;
	Size.rbr    = rbr;
	Size.rtr    = rtr;
	Size.rh1    = rh1;
	Size.rh2    = rh2;
	Size.rh3    = rh3;
	Size.rt1    = rt1;
	Size.rt2    = rt2;
	Size.rt2p   = rt2p;
	Size.rhr1   = rhr1;
	Size.rhr2   = rhr2;
	Size.rhr3   = rhr3;
	Size.rtr1   = rtr1;
	Size.rtr2   = rtr2;
	Size.rtr2p  = rtr2p;
	Size.sideB  = sideB;
	Size.sideH  = sideH;
	Size.sideBr = sideBr;
	Size.sideHr = sideHr;
	Size.jh1    = jh1;
	Size.jh2    = jh2;
	Size.jb1    = jb1;
	Size.jb2    = jb2;
	Size.jhr1   = jhr1;
	Size.jhr2   = jhr2;
	Size.jbr1   = jbr1;
	Size.jbr2   = jbr2;
	Size.J1     = J1;
	Size.J2     = J2;
	Size.Jr1    = Jr1;
	Size.Jr2    = Jr2;
	MakePictureSODStiffner_MCell(aGeom, Size);
}

void ICMGeomMaker::MakePictureSODStiffner_B(CCMGeom2DArray &aGeom,T_SECT_SOD_STIFFNER_B& Size, double refy)
{
	double tempx = 0.0;
	double tempy = 0.0;
	double theta = 0.0;
	double dy_org = 0.0;
	double dz_org = 0.0;
	double dSpacing = 0.0;

	double dS_H  = 0.0;
	double dS_B  = 0.0;
	double dS_B1 = 0.0;
	double dS_B2 = 0.0;
	double dS_t  = 0.0;
	double dS_tw = 0.0;
	double dS_tf = 0.0;
	double dS_R  = 0.0;

	//////////////////////////////////////////////////////////////////////////////////////////
	// Stiffener 
	int nStiffSize = m_pDataSrc->SectBefore.SectI.SODStiffener.aSODStiffSub.GetSize();
	int nStiffShape = m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape.GetSize();
	int nStiffPosRoof = 0;

	for(int i=0; i<nStiffSize; ++i)
	{
		dy_org = dz_org = dSpacing = 0.0;

		T_SECT_SOD_STIFF_SUB_D StiffSubD; StiffSubD.Initialize();
		StiffSubD = m_pDataSrc->SectBefore.SectI.SODStiffener.aSODStiffSub[i];

		double dDeckLength = 0.0;
		if(StiffSubD.nDeckPos == 0) // Top Flange
		{
			if     (StiffSubD.nDeckPart == 0) { dDeckLength = Size.B1; } // Deck 1
			else if(StiffSubD.nDeckPart == 1) { dDeckLength = Size.B2; } // Deck 2
			else if(StiffSubD.nDeckPart == 2) { dDeckLength = Size.B3; } // Deck 3
			else                              { ASSERT(0);             }
		}
		else if(StiffSubD.nDeckPos == 1) // Left Web
		{
			dDeckLength = Size.H;
		}
		else if(StiffSubD.nDeckPos == 2) // Right Web
		{
			dDeckLength = Size.H;
		}

		else if(StiffSubD.nDeckPos == 3) // Bot Flange
		{
			if     (StiffSubD.nDeckPart == 0) { dDeckLength = Size.B4; } // Deck 1
			else if(StiffSubD.nDeckPart == 1) { dDeckLength = Size.B5; } // Deck 2
			else if(StiffSubD.nDeckPart == 2) { dDeckLength = Size.B6; } // Deck 3
			else                              { ASSERT(0);             }
		}

		CDBDoc::GetDocPoint()->m_pAttrCtrl->ConvertSODStiffnerRefPos(StiffSubD, dDeckLength);


		for(int j=0; j<StiffSubD.nNum; ++j)
		{
			T_SECT_SOD_STIFF_DATA_D StiffDataD; StiffDataD.Initialize();
			StiffDataD = StiffSubD.aStiffData[j];
			
			int nStiffShape = 0;
			for(int k=0; k<m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape.GetSize(); ++k)
			{
				if(m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape[k].strName == StiffDataD.strShapeName)
				{
					nStiffShape = k;
					break;
				}
			}

			T_STIFF_SHAPE_D StiffShapeD; StiffShapeD.Initialize();
			StiffShapeD = m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape[nStiffShape];

			if(StiffDataD.nStiffPos==2) nStiffPosRoof = 2;
			else                        nStiffPosRoof = 1;

			for(int k=0; k<nStiffPosRoof; ++k)
			{
				BOOL bMirror = FALSE;

				CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
				pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);

				if(StiffSubD.nDeckPos == 0) // Top Flange
				{
					theta = 180.0 * CMathFunc::m_trrad;
					dSpacing += StiffDataD.dSpacing;

					if     (StiffSubD.nDeckPart == 0) { tempx = Size.Top;                 } // Deck 1
					else if(StiffSubD.nDeckPart == 1) { tempx = Size.Top+Size.B1;         } // Deck 2
					else if(StiffSubD.nDeckPart == 2) { tempx = Size.Top+Size.B1+Size.B2; } // Deck 3
					else                              {} // Nothing

					tempy = refy + Size.t1;

					///////////////////////////
					dy_org = tempx + dSpacing;
					dz_org = tempy;
					///////////////////////////
				}
				else if(StiffSubD.nDeckPos == 1) // Left Web
				{
					if(StiffDataD.nStiffPos==0 || (StiffDataD.nStiffPos==2 && k==0) ) // Left. Both
					{
						double tmp = (Size.Top + Size.B1) - (Size.Bot + Size.B4);

						if(tmp > 0.0)
						{
							theta = -atan(Size.H/tmp);
						}
						else if(tmp < 0.0)
						{
							theta = (180.0 * CMathFunc::m_trrad)-atan(Size.H/tmp);	
						}
						else
						{
							theta = (-90.0 * CMathFunc::m_trrad);
						}

						dSpacing += StiffDataD.dSpacing;

						if(theta == 0.0) tempx = 0.0;
						else             tempx = dSpacing / tan(theta);

						tempx += (Size.Top + Size.B1 - Size.twp_L);
						tempy = refy + Size.t1;
					}
					else if(StiffDataD.nStiffPos==1 || (StiffDataD.nStiffPos==2 && k==1) ) // Right, Both
					{
						bMirror = TRUE;
						double tmp = (Size.Top + Size.B1) - (Size.Bot + Size.B4);

						if(tmp > 0.0)
						{
							theta = (180.0 * CMathFunc::m_trrad)-atan(Size.H/tmp);	
						}
						else if(tmp < 0.0)
						{
							theta = -atan(Size.H/tmp);
						}
						else
						{
							theta = (90.0 * CMathFunc::m_trrad);
						}

						if(StiffDataD.nStiffPos==1) dSpacing += StiffDataD.dSpacing; // Both 일때는 Left 랑 위치 동일

						if(theta == 0.0) tempx = 0.0;
						else             tempx = dSpacing / tan(theta);

						tempx += (Size.Top + Size.B1);
						tempy = refy + Size.t1;
					}

					///////////////////////////
					dy_org = tempx;
					dz_org = tempy + dSpacing;
					///////////////////////////
				}
				else if(StiffSubD.nDeckPos == 2) // Right Web
				{
					if(StiffDataD.nStiffPos==0 || (StiffDataD.nStiffPos==2 && k==0) ) // Left. Both
					{
						double tmp = (Size.Top + Size.B1 + Size.B2) - (Size.Bot + Size.B4 + Size.B5);

						if(tmp > 0.0)
						{
							theta = -atan(Size.H/tmp);
						}
						else if(tmp < 0.0)
						{
							theta = (180.0 * CMathFunc::m_trrad)-atan(Size.H/tmp);	
						}
						else
						{
							theta = (-90.0 * CMathFunc::m_trrad);
						}

						dSpacing += StiffDataD.dSpacing;

						if(theta == 0.0) tempx = 0.0;
						else             tempx = dSpacing / tan(theta);

						tempx += (Size.Top + Size.B1 + Size.B2);
						tempy = refy + Size.t1;
					}
					else if(StiffDataD.nStiffPos==1 || (StiffDataD.nStiffPos==2 && k==1) ) // Right, Both
					{
						bMirror = TRUE;

						double tmp = (Size.Top + Size.B1 + Size.B2) - (Size.Bot + Size.B4 + Size.B5);

						if(tmp > 0.0)
						{
							theta = (180.0 * CMathFunc::m_trrad)-atan(Size.H/tmp);	
						}
						else if(tmp < 0.0)
						{
							theta = -atan(Size.H/tmp);
						}
						else
						{
							theta = (90.0 * CMathFunc::m_trrad);
						}

						if(StiffDataD.nStiffPos==1) dSpacing += StiffDataD.dSpacing; // Both 일때는 Left 랑 위치 동일

						if(theta == 0.0) tempx = 0.0;
						else             tempx = dSpacing / tan(theta);

						tempx += (Size.Top + Size.B1 + Size.B2 + Size.twp_R);
						tempy = refy + Size.t1;
					}

					///////////////////////////
					dy_org = tempx;
					dz_org = tempy + dSpacing;
					///////////////////////////
				}
				else if(StiffSubD.nDeckPos == 3) // Bot Flange
				{
					bMirror = TRUE;

					theta = 0.0;

					if     (StiffSubD.nDeckPart == 0) { tempx = Size.Bot;                 } // Deck 1
					else if(StiffSubD.nDeckPart == 1) { tempx = Size.Bot+Size.B4;         } // Deck 2
					else if(StiffSubD.nDeckPart == 2) { tempx = Size.Bot+Size.B4+Size.B5; } // Deck 3
					else                              {} // Nothing

					tempy = refy + Size.t1 + Size.H;

					dSpacing += StiffDataD.dSpacing;
					
					///////////////////////////
					dy_org = tempx + dSpacing;
					dz_org = tempy;
					///////////////////////////
				}
				else
				{
					// Nothing
				}

				if(StiffShapeD.nType == 0) // I
				{
					dS_H = StiffShapeD.dSize[0];
					dS_B = StiffShapeD.dSize[1];

					MakePictureH_SODStiffener(dS_H, dS_B, theta, dy_org, dz_org, pPoly);
					aGeom.Add(pPoly);
				}
				else if(StiffShapeD.nType == 1) // T
				{
					dS_H  = StiffShapeD.dSize[0];
					dS_B  = StiffShapeD.dSize[1];
					dS_tw = StiffShapeD.dSize[2];
					dS_tf = StiffShapeD.dSize[3];

					MakePictureT_SODStiffener(dS_H, dS_B, dS_tw, dS_tf, theta, dy_org, dz_org, pPoly);
					aGeom.Add(pPoly);	
				}
				else if(StiffShapeD.nType == 2) // U-Rib
				{
					dS_H  = StiffShapeD.dSize[0];
					dS_B1 = StiffShapeD.dSize[1];
					dS_B2 = StiffShapeD.dSize[2];
					dS_t  = StiffShapeD.dSize[3];
					dS_R  = StiffShapeD.dSize[4];

					MakePictureURib_SODStiffener(dS_H, dS_B1, dS_B2, dS_t, dS_R, theta, dy_org, dz_org, pPoly);
					aGeom.Add(pPoly);
				}
				else if(StiffShapeD.nType == 3) // L
				{
					dS_H  = StiffShapeD.dSize[0];
					dS_B  = StiffShapeD.dSize[1];
					dS_tw = StiffShapeD.dSize[2];
					dS_tf = StiffShapeD.dSize[3];

					int nDir = StiffDataD.nStiffDir;
					if(bMirror) 
					{
						nDir = (nDir==0)? 1 : 0 ;
					}

					MakePictureL_SODStiffener(dS_H, dS_B, dS_tw, dS_tf, nDir, theta, dy_org, dz_org, pPoly);
					aGeom.Add(pPoly);	
				}
			}	
		}
	}
}

void ICMGeomMaker::MakePictureSODStiffner_I(CCMGeom2DArray &aGeom, T_SECT_SOD_STIFFNER_I& Size, double refy)
{
	double tempx = 0.0;
	double tempy = 0.0;
	double theta = 0.0;
	double dy_org = 0.0;
	double dz_org = 0.0;
	double dSpacing = 0.0;

	double dS_H  = 0.0;
	double dS_B  = 0.0;
	double dS_B1 = 0.0;
	double dS_B2 = 0.0;
	double dS_t  = 0.0;
	double dS_tw = 0.0;
	double dS_tf = 0.0;
	double dS_R  = 0.0;
	int nStiffPosRoof = 0; // Left,Right,Top,Bottom:1, Both:2

	//////////////////////////////////////////////////////////////////////////////////////////
	// Stiffener 
	int nStiffSize = m_pDataSrc->SectBefore.SectI.SODStiffener.aSODStiffSub.GetSize();
	int nStiffShape = m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape.GetSize();

	for(int i=0; i<nStiffSize; ++i)
	{
		dy_org = dz_org = dSpacing = 0.0;

		T_SECT_SOD_STIFF_SUB_D StiffSubD; StiffSubD.Initialize();
		StiffSubD = m_pDataSrc->SectBefore.SectI.SODStiffener.aSODStiffSub[i];

		double dDeckLength = 0.0;
		if(StiffSubD.nDeckPos == 0) // Top Flange
		{
			if     (StiffSubD.nDeckPart == 0) { dDeckLength = Size.B1; } // Deck 1
			else if(StiffSubD.nDeckPart == 1) { dDeckLength = Size.B2; } // Deck 2
			else                              { ASSERT(0);             }
		}
		else if(StiffSubD.nDeckPos == 1) // Web
		{
			dDeckLength = Size.H;
		}
		else if(StiffSubD.nDeckPos == 2) // Bot Flange
		{
			if     (StiffSubD.nDeckPart == 0) { dDeckLength = Size.B3; } // Deck 1
			else if(StiffSubD.nDeckPart == 1) { dDeckLength = Size.B4; } // Deck 2
			else                              { ASSERT(0);             }
		}

		CDBDoc::GetDocPoint()->m_pAttrCtrl->ConvertSODStiffnerRefPos(StiffSubD, dDeckLength);
		if (m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape.GetSize() == 0) return;

		for(int j=0; j<StiffSubD.nNum; ++j)
		{
			T_SECT_SOD_STIFF_DATA_D StiffDataD; StiffDataD.Initialize();
			StiffDataD = StiffSubD.aStiffData[j];
			
			int nStiffShape = 0;
			for(int k=0; k<m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape.GetSize(); ++k)
			{
				if(m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape[k].strName == StiffDataD.strShapeName)
				{
					nStiffShape = k;
					break;
				}
			}

			T_STIFF_SHAPE_D StiffShapeD; StiffShapeD.Initialize();
			StiffShapeD = m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape[nStiffShape];

			if(StiffDataD.nStiffPos==2) nStiffPosRoof = 2;
			else                        nStiffPosRoof = 1;

			for(int k=0; k<nStiffPosRoof; ++k)
			{
				CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
				pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);

				BOOL bMirror = FALSE;
				if(StiffSubD.nDeckPos == 0) // Top Flange
				{
					theta = 180.0 * CMathFunc::m_trrad;

					if     (StiffSubD.nDeckPart == 0) { tempx = Size.Top;         } // Deck 1
					else if(StiffSubD.nDeckPart == 1) { tempx = Size.Top+Size.B1; } // Deck 2
					else                              {} // Nothing

					tempy = refy + Size.t1;

					dSpacing += StiffDataD.dSpacing;

					///////////////////////////
					dy_org = tempx + dSpacing;
					dz_org = tempy;
					///////////////////////////
				}
				else if(StiffSubD.nDeckPos == 1) // Web
				{
					if(StiffDataD.nStiffPos==0 || (StiffDataD.nStiffPos==2 && k==0) ) // Left. Both
					{
						double tmp = (Size.Top + Size.B1) - (Size.Bot + Size.B3);

						if(tmp > 0.0)
						{
							theta = -atan(Size.H/tmp);
						}
						else if(tmp < 0.0)
						{
							theta = (180.0 * CMathFunc::m_trrad)-atan(Size.H/tmp);	
						}
						else
						{
							theta = (-90.0 * CMathFunc::m_trrad);
						}

						dSpacing += StiffDataD.dSpacing;

						if(theta == 0.0) tempx = 0.0;
						else             tempx = dSpacing / tan(theta);

						tempx += (Size.Top + Size.B1 - (Size.twp*0.5));
						tempy = refy + Size.t1;
					}
					else if(StiffDataD.nStiffPos==1 || (StiffDataD.nStiffPos==2 && k==1) ) // Right, Both
					{
						bMirror = TRUE;
						double tmp = (Size.Top + Size.B1) - (Size.Bot + Size.B3);

						if(tmp > 0.0)
						{
							theta = (180.0 * CMathFunc::m_trrad)-atan(Size.H/tmp);	
						}
						else if(tmp < 0.0)
						{
							theta = -atan(Size.H/tmp);
						}
						else
						{
							theta = (90.0 * CMathFunc::m_trrad);
						}

						if(StiffDataD.nStiffPos==1) dSpacing += StiffDataD.dSpacing; // Both 일때는 Left 랑 위치 동일

						if(theta == 0.0) tempx = 0.0;
						else             tempx = dSpacing / tan(theta);

						tempx += (Size.Top + Size.B1 + (Size.twp*0.5));
						tempy = refy + Size.t1;
					}

					///////////////////////////
					dy_org = tempx;
					dz_org = tempy + dSpacing;
					///////////////////////////
				}		
				else if(StiffSubD.nDeckPos == 2) // Bot Flange
				{
					bMirror = TRUE;

					theta = 0.0;

					if     (StiffSubD.nDeckPart == 0) { tempx = Size.Bot;         } // Deck 1
					else if(StiffSubD.nDeckPart == 1) { tempx = Size.Bot+Size.B3; } // Deck 2
					else                              {} // Nothing

					tempy = refy + Size.t1 + Size.H;

					dSpacing += StiffDataD.dSpacing;

					///////////////////////////
					dy_org = tempx + dSpacing;
					dz_org = tempy;
					///////////////////////////
				}
				else
				{
					// Nothing
				}

				if(StiffShapeD.nType == 0) // I
				{
					dS_H = StiffShapeD.dSize[0];
					dS_B = StiffShapeD.dSize[1];

					MakePictureH_SODStiffener(dS_H, dS_B, theta, dy_org, dz_org, pPoly);
					aGeom.Add(pPoly);
				}
				else if(StiffShapeD.nType == 1) // T
				{
					dS_H  = StiffShapeD.dSize[0];
					dS_B  = StiffShapeD.dSize[1];
					dS_tw = StiffShapeD.dSize[2];
					dS_tf = StiffShapeD.dSize[3];

					MakePictureT_SODStiffener(dS_H, dS_B, dS_tw, dS_tf, theta, dy_org, dz_org, pPoly);
					aGeom.Add(pPoly);	
				}
				else if(StiffShapeD.nType == 2) // U-Rib
				{
					dS_H  = StiffShapeD.dSize[0];
					dS_B1 = StiffShapeD.dSize[1];
					dS_B2 = StiffShapeD.dSize[2];
					dS_t  = StiffShapeD.dSize[3];
					dS_R  = StiffShapeD.dSize[4];

					MakePictureURib_SODStiffener(dS_H, dS_B1, dS_B2, dS_t, dS_R, theta, dy_org, dz_org, pPoly);
					aGeom.Add(pPoly);
				}
				else if(StiffShapeD.nType == 3) // L
				{
					dS_H  = StiffShapeD.dSize[0];
					dS_B  = StiffShapeD.dSize[1];
					dS_tw = StiffShapeD.dSize[2];
					dS_tf = StiffShapeD.dSize[3];

					int nDir = StiffDataD.nStiffDir;
					if(bMirror) 
					{
						nDir = (nDir==0)? 1 : 0 ;
					}

					MakePictureL_SODStiffener(dS_H, dS_B, dS_tw, dS_tf, nDir, theta, dy_org, dz_org, pPoly);
					aGeom.Add(pPoly);	
				}

			}	
		}
	}
}

void ICMGeomMaker::MakePictureSODStiffner_Tub(CCMGeom2DArray &aGeom, T_SECT_SOD_STIFFNER_TUB& Size, double refy)
{
	double tempx = 0.0;
	double tempy = 0.0;
	double theta = 0.0;
	double dy_org = 0.0;
	double dz_org = 0.0;
	double dSpacing = 0.0;

	double dS_H  = 0.0;
	double dS_B  = 0.0;
	double dS_B1 = 0.0;
	double dS_B2 = 0.0;
	double dS_t  = 0.0;
	double dS_tw = 0.0;
	double dS_tf = 0.0;
	double dS_R  = 0.0;
	int nStiffPosRoof = 0; // Left,Right,Top,Bottom:1, Both:2
	
	//////////////////////////////////////////////////////////////////////////////////////////
	// Stiffener 
	int nStiffSize = m_pDataSrc->SectBefore.SectI.SODStiffener.aSODStiffSub.GetSize();
	int nStiffShape = m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape.GetSize();

	for(int i=0; i<nStiffSize; ++i)
	{
		dy_org = dz_org = dSpacing = 0.0;

		T_SECT_SOD_STIFF_SUB_D StiffSubD; StiffSubD.Initialize();
		StiffSubD = m_pDataSrc->SectBefore.SectI.SODStiffener.aSODStiffSub[i];

		double dDeckLength = 0.0;
		if(StiffSubD.nDeckPos == 1) // Left Web
		{
			dDeckLength = Size.H;
		}
		else if(StiffSubD.nDeckPos == 2) // Right Web
		{
			dDeckLength = Size.H;
		}
		else if(StiffSubD.nDeckPos == 3) // Bot Flange
		{
			if     (StiffSubD.nDeckPart == 0) { dDeckLength = Size.B4; } // Deck 1
			else if(StiffSubD.nDeckPart == 1) { dDeckLength = Size.B5; } // Deck 2
			else if(StiffSubD.nDeckPart == 2) { dDeckLength = Size.B6; } // Deck 3
			else                              { ASSERT(0);             }
		}

		CDBDoc::GetDocPoint()->m_pAttrCtrl->ConvertSODStiffnerRefPos(StiffSubD, dDeckLength);


		for(int j=0; j<StiffSubD.nNum; ++j)
		{
			T_SECT_SOD_STIFF_DATA_D StiffDataD; StiffDataD.Initialize();
			StiffDataD = StiffSubD.aStiffData[j];
			
			int nStiffShape = 0;
			for(int k=0; k<m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape.GetSize(); ++k)
			{
				if(m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape[k].strName == StiffDataD.strShapeName)
				{
					nStiffShape = k;
					break;
				}
			}

			T_STIFF_SHAPE_D StiffShapeD; StiffShapeD.Initialize();
			StiffShapeD = m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape[nStiffShape];

			if(StiffDataD.nStiffPos==2) nStiffPosRoof = 2;
			else                        nStiffPosRoof = 1;

			for(int k=0; k<nStiffPosRoof; ++k)
			{
				BOOL bMirror = FALSE;

				CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
				pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);

				if(StiffSubD.nDeckPos == 1) // Left Web
				{
					if(StiffDataD.nStiffPos==0 || (StiffDataD.nStiffPos==2 && k==0) ) // Left. Both
					{
						double tmp = (Size.Top + Size.bf1) - (Size.Bot + Size.B4);

						if(tmp > 0.0)
						{
							theta = -atan(Size.H/tmp);
						}
						else if(tmp < 0.0)
						{
							theta = (180.0 * CMathFunc::m_trrad)-atan(Size.H/tmp);	
						}
						else
						{
							theta = (-90.0 * CMathFunc::m_trrad);
						}

						dSpacing += StiffDataD.dSpacing;

						if(theta == 0.0) tempx = 0.0;
						else             tempx = dSpacing / tan(theta);

						tempx += (Size.Top + Size.bf1 - Size.twp_L);
						tempy = refy + Size.t1;
					}
					else if(StiffDataD.nStiffPos==1 || (StiffDataD.nStiffPos==2 && k==1) ) // Right, Both
					{
						bMirror = TRUE;
						double tmp = (Size.Top + Size.bf1) - (Size.Bot + Size.B4);

						if(tmp > 0.0)
						{
							theta = (180.0 * CMathFunc::m_trrad)-atan(Size.H/tmp);	
						}
						else if(tmp < 0.0)
						{
							theta = -atan(Size.H/tmp);
						}
						else
						{
							theta = (90.0 * CMathFunc::m_trrad);
						}

						if(StiffDataD.nStiffPos==1) dSpacing += StiffDataD.dSpacing; // Both 일때는 Left 랑 위치 동일

						if(theta == 0.0) tempx = 0.0;
						else             tempx = dSpacing / tan(theta);

						tempx += (Size.Top + Size.bf1);
						tempy = refy + Size.t1;
					}
					
					///////////////////////////
					dy_org = tempx;
					dz_org = tempy + dSpacing;
					///////////////////////////
				}
				else if(StiffSubD.nDeckPos == 2) // Right Web
				{
					if(StiffDataD.nStiffPos==0 || (StiffDataD.nStiffPos==2 && k==0) ) // Left. Both
					{
						double tmp = (Size.Top + Size.B1 + Size.B2 + Size.B3 - Size.bf2) - (Size.Bot + Size.B4 + Size.B5);

						if(tmp > 0.0)
						{
							theta = -atan(Size.H/tmp);
						}
						else if(tmp < 0.0)
						{
							theta = (180.0 * CMathFunc::m_trrad)-atan(Size.H/tmp);	
						}
						else
						{
							theta = (-90.0 * CMathFunc::m_trrad);
						}

						dSpacing += StiffDataD.dSpacing;

						if(theta == 0.0) tempx = 0.0;
						else             tempx = dSpacing / tan(theta);

						tempx += (Size.Top + Size.B1 + Size.B2 + Size.B3 - Size.bf2);
						tempy = refy + Size.t1;
					}
					else if(StiffDataD.nStiffPos==1 || (StiffDataD.nStiffPos==2 && k==1) ) // Right, Both
					{
						bMirror = TRUE;
						double tmp = (Size.Top + Size.B1 + Size.B2 + Size.B3 - Size.bf2) - (Size.Bot + Size.B4 + Size.B5);

						if(tmp > 0.0)
						{
							theta = (180.0 * CMathFunc::m_trrad)-atan(Size.H/tmp);	
						}
						else if(tmp < 0.0)
						{
							theta = -atan(Size.H/tmp);
						}
						else
						{
							theta = (90.0 * CMathFunc::m_trrad);
						}

						if(StiffDataD.nStiffPos==1) dSpacing += StiffDataD.dSpacing; // Both 일때는 Left 랑 위치 동일

						if(theta == 0.0) tempx = 0.0;
						else             tempx = dSpacing / tan(theta);

						tempx += (Size.Top + Size.B1 + Size.B2 + Size.B3 - Size.bf2 + Size.twp_R);
						tempy = refy + Size.t1;
					}

					///////////////////////////
					dy_org = tempx;
					dz_org = tempy + dSpacing;
					///////////////////////////
				}
				else if(StiffSubD.nDeckPos == 3) // Bot Flange 
				{
					bMirror = TRUE;
					theta = 0.0;

					if     (StiffSubD.nDeckPart == 0) { tempx = Size.Bot;                 } // Deck 1
					else if(StiffSubD.nDeckPart == 1) { tempx = Size.Bot+Size.B4;         } // Deck 2
					else if(StiffSubD.nDeckPart == 2) { tempx = Size.Bot+Size.B4+Size.B5; } // Deck 3
					else                              {} // Nothing

					tempy = refy + Size.t1 + Size.H;

					dSpacing += StiffDataD.dSpacing;

					///////////////////////////
					dy_org = tempx + dSpacing;
					dz_org = tempy;
					///////////////////////////
				}
				else
				{
					// Nothing
				}

				if(StiffShapeD.nType == 0) // I
				{
					dS_H = StiffShapeD.dSize[0];
					dS_B = StiffShapeD.dSize[1];

					MakePictureH_SODStiffener(dS_H, dS_B, theta, dy_org, dz_org, pPoly);
					aGeom.Add(pPoly);
				}
				else if(StiffShapeD.nType == 1) // T
				{
					dS_H  = StiffShapeD.dSize[0];
					dS_B  = StiffShapeD.dSize[1];
					dS_tw = StiffShapeD.dSize[2];
					dS_tf = StiffShapeD.dSize[3];

					MakePictureT_SODStiffener(dS_H, dS_B, dS_tw, dS_tf, theta, dy_org, dz_org, pPoly);
					aGeom.Add(pPoly);	
				}
				else if(StiffShapeD.nType == 2) // U-Rib
				{
					dS_H  = StiffShapeD.dSize[0];
					dS_B1 = StiffShapeD.dSize[1];
					dS_B2 = StiffShapeD.dSize[2];
					dS_t  = StiffShapeD.dSize[3];
					dS_R  = StiffShapeD.dSize[4];

					MakePictureURib_SODStiffener(dS_H, dS_B1, dS_B2, dS_t, dS_R, theta, dy_org, dz_org, pPoly);
					aGeom.Add(pPoly);
				}
				else if(StiffShapeD.nType == 3) // L
				{
					dS_H  = StiffShapeD.dSize[0];
					dS_B  = StiffShapeD.dSize[1];
					dS_tw = StiffShapeD.dSize[2];
					dS_tf = StiffShapeD.dSize[3];

					int nDir = StiffDataD.nStiffDir;
					if(bMirror) 
					{
						nDir = (nDir==0)? 1 : 0 ;
					}

					MakePictureL_SODStiffener(dS_H, dS_B, dS_tw, dS_tf, nDir, theta, dy_org, dz_org, pPoly);
					aGeom.Add(pPoly);	
				}
			}	
		}
	}
}

void ICMGeomMaker::MakePictureSODStiffner_MCell(CCMGeom2DArray &aGeom, T_SECT_SOD_STIFFNER_MCELL& Size)
{
	double dTol = 1.0e-10;

	double tempx = 0.0;
	double tempy = 0.0;
	double theta = 0.0;
	double dy_org = 0.0;
	double dz_org = 0.0;
	double dSpacing = 0.0;
	
	double dS_H  = 0.0;
	double dS_B  = 0.0;
	double dS_B1 = 0.0;
	double dS_B2 = 0.0;
	double dS_t  = 0.0;
	double dS_tw = 0.0;
	double dS_tf = 0.0;
	double dS_R  = 0.0;
	int nStiffPosRoof = 0; // Left,Right,Top,Bottom:1, Both:2

	//////////////////////////////////////////////////////////////////////////////////////////
	// Stiffener 
	int nStiffSize = m_pDataSrc->SectBefore.SectI.SODStiffener.aSODStiffSub.GetSize();
	int nStiffShape = m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape.GetSize();

	for(int i=0; i<nStiffSize; ++i)
	{
		dy_org = dz_org = dSpacing = 0.0;

		T_SECT_SOD_STIFF_SUB_D StiffSubD; StiffSubD.Initialize();
		StiffSubD = m_pDataSrc->SectBefore.SectI.SODStiffener.aSODStiffSub[i];

		if(m_pDataSrc->SectBefore.nFlangeShape == 0 ||
			 m_pDataSrc->SectBefore.nFlangeShape == 1 ||
			 m_pDataSrc->SectBefore.nFlangeShape == 2)
		{
			if(m_pDataSrc->SectBefore.nCellNum == 0) // 0 Cell 
			{
				if(m_pDataSrc->SectBefore.nCellType == 0) // Left
				{
					double dDeckLength = 0.0;
					if(StiffSubD.nDeckPos == 0) // Top Flange
					{
						if     (StiffSubD.nDeckPart == 0) { dDeckLength = Size.B1; } // Deck 1
						else if(StiffSubD.nDeckPart == 1) { dDeckLength = Size.B2; } // Deck 2
						else                              { ASSERT(0);             }
					}
					else if(StiffSubD.nDeckPos == 1) // Web
					{
						dDeckLength = Size.H;
					}
					else if(StiffSubD.nDeckPos == 2) // Bot Flange
					{
						if     (StiffSubD.nDeckPart == 0) { dDeckLength = Size.B4; } // Deck 1
						else if(StiffSubD.nDeckPart == 1) { dDeckLength = Size.B5; } // Deck 2
						else                              { ASSERT(0);             }
					}
					else if(StiffSubD.nDeckPos == 3) // Side Flange
					{
						dDeckLength = Size.sideB;
					}

					CDBDoc::GetDocPoint()->m_pAttrCtrl->ConvertSODStiffnerRefPos(StiffSubD, dDeckLength);

					for(int j=0; j<StiffSubD.nNum; ++j)
					{
						T_SECT_SOD_STIFF_DATA_D StiffDataD; StiffDataD.Initialize();
						StiffDataD = StiffSubD.aStiffData[j];

						int nStiffShape = 0;
						for(int k=0; k<m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape.GetSize(); ++k)
						{
							if(m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape[k].strName == StiffDataD.strShapeName)
							{
								nStiffShape = k;
								break;
							}
						}

						T_STIFF_SHAPE_D StiffShapeD; StiffShapeD.Initialize();
						StiffShapeD = m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape[nStiffShape];

						if(StiffDataD.nStiffPos==2) nStiffPosRoof = 2;
						else                        nStiffPosRoof = 1;
						
						for(int k=0; k<nStiffPosRoof; ++k)
						{
							BOOL bMirror = FALSE;
							CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
							pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);

							if(StiffSubD.nDeckPos == 0) // Top Flange
							{
								theta = 180.0 * CMathFunc::m_trrad;
								dSpacing += StiffDataD.dSpacing;

								if     (StiffSubD.nDeckPart == 0) { tempx = 0.0;     } // Deck 1
								else if(StiffSubD.nDeckPart == 1) { tempx = Size.B1; } // Deck 2
								else                              {} // Nothing

								tempy = Size.tf1;

								///////////////////////////
								dy_org = tempx + dSpacing;
								dz_org = tempy;
								///////////////////////////
							}
							else if(StiffSubD.nDeckPos == 1) // Web
							{
								if(StiffDataD.nStiffPos==0 || (StiffDataD.nStiffPos==2 && k==0) ) // Left. Both
								{
									double tmp = Size.B1-(Size.Bot+Size.B4);

									if(tmp > 0.0)
									{
										theta = -atan(Size.H/tmp);
									}
									else if(tmp < 0.0)
									{
										theta = (180.0 * CMathFunc::m_trrad)-atan(Size.H/tmp);	
									}
									else
									{
										theta = (-90.0 * CMathFunc::m_trrad);
									}

									dSpacing += StiffDataD.dSpacing;

									if(theta == 0.0) tempx = 0.0;
									else             tempx = dSpacing / tan(theta);

									tempx += Size.B1;
									tempy = Size.tf1;
								}
								else if(StiffDataD.nStiffPos==1 || (StiffDataD.nStiffPos==2 && k==1) ) // Right, Both
								{
									bMirror = TRUE;
									double tmp = Size.B1-(Size.Bot+Size.B4);

									if(tmp > 0.0)
									{
										theta = (180.0 * CMathFunc::m_trrad)-atan(Size.H/tmp);	
									}
									else if(tmp < 0.0)
									{
										theta = -atan(Size.H/tmp);
									}
									else
									{
										theta = (90.0 * CMathFunc::m_trrad);
									}

									if(StiffDataD.nStiffPos==1) dSpacing += StiffDataD.dSpacing; // Both 일때는 Left 랑 위치 동일

									if(theta == 0.0) tempx = 0.0;
									else             tempx = dSpacing / tan(theta);

									tempx += Size.B1+Size.twp_L;
									tempy = Size.tf1;
								}
								
								///////////////////////////
								dy_org = tempx;
								dz_org = tempy + dSpacing;
								///////////////////////////
							}
							else if(StiffSubD.nDeckPos == 2) // Bot Flange
							{
								bMirror = TRUE;
								theta = 0.0;

								if     (StiffSubD.nDeckPart == 0) { tempx = Size.Bot;         } // Deck 1
								else if(StiffSubD.nDeckPart == 1) { tempx = Size.Bot+Size.B4; } // Deck 2
								else                              {} // Nothing

								tempy = Size.tf1 + Size.H;

								dSpacing += StiffDataD.dSpacing;

								///////////////////////////
								dy_org = tempx + dSpacing;
								dz_org = tempy;
								///////////////////////////
							}
							else if(StiffSubD.nDeckPos == 3) // Side Flange
							{
								bMirror = TRUE;
								if(Size.sideB > 0.0)
								{
									theta = atan((Size.rh3-Size.rh1) / Size.sideB);
								}
								
								dSpacing += StiffDataD.dSpacing;

								tempx = Size.rb + Size.rt1;

								if(theta == 0.0) tempy = 0.0;
								else             tempy = dSpacing * tan(theta);

								tempy += Size.tf1 + Size.rh1 - Size.rt2p;

								///////////////////////////
								dy_org = tempx + dSpacing;
								dz_org = tempy;
								///////////////////////////
							}
							else 
							{
								// Nothing
							}

							if(StiffShapeD.nType == 0) // I
							{
								dS_H = StiffShapeD.dSize[0];
								dS_B = StiffShapeD.dSize[1];

								MakePictureH_SODStiffener(dS_H, dS_B, theta, dy_org, dz_org, pPoly);
								aGeom.Add(pPoly);
							}
							else if(StiffShapeD.nType == 1) // T
							{
								dS_H  = StiffShapeD.dSize[0];
								dS_B  = StiffShapeD.dSize[1];
								dS_tw = StiffShapeD.dSize[2];
								dS_tf = StiffShapeD.dSize[3];

								MakePictureT_SODStiffener(dS_H, dS_B, dS_tw, dS_tf, theta, dy_org, dz_org, pPoly);
								aGeom.Add(pPoly);	
							}
							else if(StiffShapeD.nType == 2) // U-Rib
							{
								dS_H  = StiffShapeD.dSize[0];
								dS_B1 = StiffShapeD.dSize[1];
								dS_B2 = StiffShapeD.dSize[2];
								dS_t  = StiffShapeD.dSize[3];
								dS_R  = StiffShapeD.dSize[4];

								MakePictureURib_SODStiffener(dS_H, dS_B1, dS_B2, dS_t, dS_R, theta, dy_org, dz_org, pPoly);
								aGeom.Add(pPoly);
							}
							else if(StiffShapeD.nType == 3) // L
							{
								dS_H  = StiffShapeD.dSize[0];
								dS_B  = StiffShapeD.dSize[1];
								dS_tw = StiffShapeD.dSize[2];
								dS_tf = StiffShapeD.dSize[3];

								int nDir = StiffDataD.nStiffDir;
								if(bMirror) 
								{
									nDir = (nDir==0)? 1 : 0 ;
								}

								MakePictureL_SODStiffener(dS_H, dS_B, dS_tw, dS_tf, nDir, theta, dy_org, dz_org, pPoly);
								aGeom.Add(pPoly);	
							}

						}
					}
				}
				////////////////////////////
				else // Right
				{
					double dDeckLength = 0.0;
					if(StiffSubD.nDeckPos == 0) // Top Flange
					{
						if     (StiffSubD.nDeckPart == 0) { dDeckLength = Size.Br2; } // Deck 1
						else if(StiffSubD.nDeckPart == 1) { dDeckLength = Size.Br1; } // Deck 2
						else                              { ASSERT(0);             }
					}
					else if(StiffSubD.nDeckPos == 1) // Web
					{
						dDeckLength = Size.H;
					}
					else if(StiffSubD.nDeckPos == 2) // Bot Flange
					{
						if     (StiffSubD.nDeckPart == 0) { dDeckLength = Size.Br5; } // Deck 1
						else if(StiffSubD.nDeckPart == 1) { dDeckLength = Size.Br4; } // Deck 2
						else                              { ASSERT(0);             }
					}
					else if(StiffSubD.nDeckPos == 3) // Side Flange
					{
						dDeckLength = Size.sideBr;
					}

					CDBDoc::GetDocPoint()->m_pAttrCtrl->ConvertSODStiffnerRefPos(StiffSubD, dDeckLength);

					for(int j=0; j<StiffSubD.nNum; ++j)
					{
						T_SECT_SOD_STIFF_DATA_D StiffDataD; StiffDataD.Initialize();
						StiffDataD = StiffSubD.aStiffData[j];

						int nStiffShape = 0;
						for(int k=0; k<m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape.GetSize(); ++k)
						{
							if(m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape[k].strName == StiffDataD.strShapeName)
							{
								nStiffShape = k;
								break;
							}
						}

						T_STIFF_SHAPE_D StiffShapeD; StiffShapeD.Initialize();
						StiffShapeD = m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape[nStiffShape];

						if(StiffDataD.nStiffPos==2) nStiffPosRoof = 2;
						else                        nStiffPosRoof = 1;

						for(int k=0; k<nStiffPosRoof; ++k)
						{
							BOOL bMirror = FALSE;
							CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
							pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);

							if(StiffSubD.nDeckPos == 0) // Top Flange
							{
								theta = 180.0 * CMathFunc::m_trrad;
								dSpacing += StiffDataD.dSpacing;

								if     (StiffSubD.nDeckPart == 0) { tempx = Size.Top;          } // Deck 1
								else if(StiffSubD.nDeckPart == 1) { tempx = Size.Top+Size.Br2; } // Deck 2
								else                              {} // Nothing

								tempy = Size.tf1;

								///////////////////////////
								dy_org = tempx + dSpacing;
								dz_org = tempy;
								///////////////////////////
							}
							else if(StiffSubD.nDeckPos == 1) // Web
							{
								if(StiffDataD.nStiffPos==0 || (StiffDataD.nStiffPos==2 && k==0) ) // Left. Both
								{
									double tmp = (Size.Top+Size.Br2) - Size.Br5;

									if(tmp > 0.0)
									{
										theta = -atan(Size.H/tmp);
									}
									else if(tmp < 0.0)
									{
										theta = (180.0 * CMathFunc::m_trrad)-atan(Size.H/tmp);	
									}
									else
									{
										theta = (-90.0 * CMathFunc::m_trrad);
									}

									dSpacing += StiffDataD.dSpacing;

									if(theta == 0.0) tempx = 0.0;
									else             tempx = dSpacing / tan(theta);

									tempx += Size.Top + Size.Br2 - Size.twp_R;
									tempy = Size.tf1;
								}
								else if(StiffDataD.nStiffPos==1 || (StiffDataD.nStiffPos==2 && k==1) ) // Right, Both
								{
									bMirror = TRUE;
									double tmp = (Size.Top+Size.Br2) - Size.Br5;

									if(tmp > 0.0)
									{
										theta = (180.0 * CMathFunc::m_trrad)-atan(Size.H/tmp);	
									}
									else if(tmp < 0.0)
									{
										theta = -atan(Size.H/tmp);
									}
									else
									{
										theta = (90.0 * CMathFunc::m_trrad);
									}

									if(StiffDataD.nStiffPos==1) dSpacing += StiffDataD.dSpacing; // Both 일때는 Left 랑 위치 동일

									if(theta == 0.0) tempx = 0.0;
									else             tempx = dSpacing / tan(theta);

									tempx += Size.Top + Size.Br2;
									tempy = Size.tf1;
								}

								///////////////////////////
								dy_org = tempx;
								dz_org = tempy + dSpacing;
								///////////////////////////
							}
							else if(StiffSubD.nDeckPos == 2) // Bot Flange
							{
								bMirror = TRUE;
								theta = 0.0;

								if     (StiffSubD.nDeckPart == 0) { tempx = Size.Bot;          } // Deck 1
								else if(StiffSubD.nDeckPart == 1) { tempx = Size.Bot+Size.Br5; } // Deck 2
								else                              {} // Nothing

								tempy = Size.tf1 + Size.H;

								dSpacing += StiffDataD.dSpacing;

								///////////////////////////
								dy_org = tempx + dSpacing;
								dz_org = tempy;
								///////////////////////////
							}
							else if(StiffSubD.nDeckPos == 3) // Side Flange
							{
								bMirror = TRUE;
								if(Size.sideBr > 0.0)
								{
									theta = -atan((Size.rhr3-Size.rhr1) / Size.sideBr);
								}

								dSpacing += StiffDataD.dSpacing;

								tempx = Size.Top + Size.Br2 + Size.Br1 - Size.rbr - Size.rtr1 - Size.sideBr;

								if(theta == 0.0) tempy = 0.0;
								else             tempy = dSpacing * tan(theta);

								tempy += Size.tf1 + Size.rhr3 - Size.rtr2p;

								///////////////////////////
								dy_org = tempx + dSpacing;
								dz_org = tempy;
								///////////////////////////
							}
							else 
							{
								// Nothing
							}

							if(StiffShapeD.nType == 0) // I
							{
								dS_H = StiffShapeD.dSize[0];
								dS_B = StiffShapeD.dSize[1];

								MakePictureH_SODStiffener(dS_H, dS_B, theta, dy_org, dz_org, pPoly);
								aGeom.Add(pPoly);
							}
							else if(StiffShapeD.nType == 1) // T
							{
								dS_H  = StiffShapeD.dSize[0];
								dS_B  = StiffShapeD.dSize[1];
								dS_tw = StiffShapeD.dSize[2];
								dS_tf = StiffShapeD.dSize[3];

								MakePictureT_SODStiffener(dS_H, dS_B, dS_tw, dS_tf, theta, dy_org, dz_org, pPoly);
								aGeom.Add(pPoly);	
							}
							else if(StiffShapeD.nType == 2) // U-Rib
							{
								dS_H  = StiffShapeD.dSize[0];
								dS_B1 = StiffShapeD.dSize[1];
								dS_B2 = StiffShapeD.dSize[2];
								dS_t  = StiffShapeD.dSize[3];
								dS_R  = StiffShapeD.dSize[4];

								MakePictureURib_SODStiffener(dS_H, dS_B1, dS_B2, dS_t, dS_R, theta, dy_org, dz_org, pPoly);
								aGeom.Add(pPoly);
							}
							else if(StiffShapeD.nType == 3) // L
							{
								dS_H  = StiffShapeD.dSize[0];
								dS_B  = StiffShapeD.dSize[1];
								dS_tw = StiffShapeD.dSize[2];
								dS_tf = StiffShapeD.dSize[3];

								int nDir = StiffDataD.nStiffDir;
								if(bMirror) 
								{
									nDir = (nDir==0)? 1 : 0 ;
								}

								MakePictureL_SODStiffener(dS_H, dS_B, dS_tw, dS_tf, nDir, theta, dy_org, dz_org, pPoly);
								aGeom.Add(pPoly);	
							}
						}
					}
				}
			}
			else
			{
				int nCellNum = m_pDataSrc->SectBefore.nCellNum;
				int nWebNum = m_pDataSrc->SectBefore.nCellNum - 1; // 반복되는 Web 수
				int nDuplCell = nCellNum - 2;

				if(nCellNum < 0) nCellNum = 0;
				if(nWebNum  < 0) nWebNum = 0;
				if(nDuplCell < 0) nDuplCell = 0;
					
				double dDeckLength = 0.0;
				if(StiffSubD.nDeckPos == 0) // Top Flange
				{
					if(nCellNum == 1)
					{
						if     (StiffSubD.nDeckPart == 0) { dDeckLength = Size.B1;          } // Deck 1
						else if(StiffSubD.nDeckPart == 1) { dDeckLength = Size.B2+Size.Br2; } // Deck 2
						else if(StiffSubD.nDeckPart == 2) { dDeckLength = Size.Br1;         } // Deck 3
						else                              { ASSERT(0);                      }
					}
					else
					{
						if     (StiffSubD.nDeckPart == 0)            { dDeckLength = Size.B1;  } // Deck 1
						else if(StiffSubD.nDeckPart == 1)            { dDeckLength = Size.B2;  } // Deck 2
						else if(StiffSubD.nDeckPart == (nCellNum))   { dDeckLength = Size.Br2; } // Deck n-1
						else if(StiffSubD.nDeckPart == (nCellNum+1)) { dDeckLength = Size.Br1; } // Deck n
						else                                         { dDeckLength = Size.B6;  } // 반복되는 Deck
					}					
				}
				else if(StiffSubD.nDeckPos == 1) // Web
				{
					dDeckLength = Size.H;
				}
				else if(StiffSubD.nDeckPos == 2) // Bot Flange
				{
					if(nCellNum == 1)
					{
						if     (StiffSubD.nDeckPart == 0) { dDeckLength = Size.B4;          } // Deck 1
						else if(StiffSubD.nDeckPart == 1) { dDeckLength = Size.B5+Size.Br5; } // Deck 2
						else if(StiffSubD.nDeckPart == 2) { dDeckLength = Size.Br4;         } // Deck 3
						else                              { ASSERT(0);                      }
					}
					else
					{
						if     (StiffSubD.nDeckPart == 0)            { dDeckLength = Size.B4;  } // Deck 1
						else if(StiffSubD.nDeckPart == 1)            { dDeckLength = Size.B5;  } // Deck 2
						else if(StiffSubD.nDeckPart == (nCellNum))   { dDeckLength = Size.Br5; } // Deck n-1
						else if(StiffSubD.nDeckPart == (nCellNum+1)) { dDeckLength = Size.Br4; } // Deck n
						else                                         { dDeckLength = Size.B6;  } // 반복되는 Deck
					}	
				}
				else if(StiffSubD.nDeckPos == 3) // Side Flange
				{
					if     (StiffSubD.nDeckPart == 0) { dDeckLength = Size.sideB;  } // Deck 1
					else if(StiffSubD.nDeckPart == 1) { dDeckLength = Size.sideBr; } // Deck 2
					else                              { ASSERT(0);                 }
				}

				CDBDoc::GetDocPoint()->m_pAttrCtrl->ConvertSODStiffnerRefPos(StiffSubD, dDeckLength);


				for(int j=0; j<StiffSubD.nNum; ++j)
				{
					T_SECT_SOD_STIFF_DATA_D StiffDataD; StiffDataD.Initialize();
					StiffDataD = StiffSubD.aStiffData[j];

					int nStiffShape = 0;
					for(int k=0; k<m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape.GetSize(); ++k)
					{
						if(m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape[k].strName == StiffDataD.strShapeName)
						{
							nStiffShape = k;
							break;
						}
					}

					T_STIFF_SHAPE_D StiffShapeD; StiffShapeD.Initialize();
					StiffShapeD = m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape[nStiffShape];

					if(StiffDataD.nStiffPos==2) nStiffPosRoof = 2;
					else                        nStiffPosRoof = 1;

					for(int k=0; k<nStiffPosRoof; ++k)
					{
						BOOL bMirror = FALSE;
						CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
						pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);

						if(StiffSubD.nDeckPos == 0) // Top Flange
						{
							theta = 180.0 * CMathFunc::m_trrad;
							dSpacing += StiffDataD.dSpacing;

							if(nCellNum == 1)
							{
								if     (StiffSubD.nDeckPart == 0) { tempx = 0.0;                      } // Deck 1
								else if(StiffSubD.nDeckPart == 1) { tempx = Size.B1;                  } // Deck 2
								else if(StiffSubD.nDeckPart == 2) { tempx = Size.B1+Size.B2+Size.Br2; } // Deck 3
								else                              {} // Nothing
							}
							else
							{
								if     (StiffSubD.nDeckPart == 0)            { tempx = 0.0;                                          } // Deck 1
								else if(StiffSubD.nDeckPart == 1)            { tempx = Size.B1;                                      } // Deck 2
								else if(StiffSubD.nDeckPart == (nCellNum))   { tempx = Size.B1+Size.B2+(nDuplCell*Size.B6);          } // Deck n-1
								else if(StiffSubD.nDeckPart == (nCellNum+1)) { tempx = Size.B1+Size.B2+(nDuplCell*Size.B6)+Size.Br2; } // Deck n
								else
								{
									tempx = Size.B1+Size.B2+((StiffSubD.nDeckPart-2)*Size.B6);
								}
							}	

							tempy = Size.tf1;

							///////////////////////////
							dy_org = tempx + dSpacing;
							dz_org = tempy;
							///////////////////////////
						}
						else if(StiffSubD.nDeckPos == 1) // Web
						{
							if(StiffSubD.nDeckPart == 0)
							{
								bMirror = TRUE;
								double tmp = Size.B1-(Size.Bot+Size.B4);

								if(tmp > 0.0)
								{
									theta = (180.0 * CMathFunc::m_trrad)-atan(Size.H/tmp);	
								}
								else if(tmp < 0.0)
								{
									theta = -atan(Size.H/tmp);
								}
								else
								{
									theta = (90.0 * CMathFunc::m_trrad);
								}

								dSpacing += StiffDataD.dSpacing; // Both 일때는 Left 랑 위치 동일

								if(theta == 0.0) tempx = 0.0;
								else             tempx = dSpacing / tan(theta);

								tempx += Size.B1+Size.twp_L;
								tempy = Size.tf1;
							}
							else if(StiffSubD.nDeckPart == nCellNum)
							{
								double tmp = Size.Br2 - Size.Br5;

								if(tmp > 0.0)
								{
									theta = -atan(Size.H/tmp);
								}
								else if(tmp < 0.0)
								{
									theta = (180.0 * CMathFunc::m_trrad)-atan(Size.H/tmp);	
								}
								else
								{
									theta = (-90.0 * CMathFunc::m_trrad);
								}

								dSpacing += StiffDataD.dSpacing;

								if(theta == 0.0) tempx = 0.0;
								else             tempx = dSpacing / tan(theta);

								tempx += Size.B1+Size.B2+(nDuplCell*Size.B6)+Size.Br2-Size.twp_R;
								tempy = Size.tf1;
							}
							else
							{
								if(StiffDataD.nStiffPos==0 || (StiffDataD.nStiffPos==2 && k==0) ) // Left. Both
								{
									theta = (-90.0 * CMathFunc::m_trrad);
									dSpacing += StiffDataD.dSpacing;

									if(theta == 0.0) tempx = 0.0;
									else             tempx = dSpacing / tan(theta);

									tempx += Size.B1+Size.B2+((StiffSubD.nDeckPart-1)*Size.B6)-(Size.tw2*0.5);
									tempy = Size.tf1;
								}
								else if(StiffDataD.nStiffPos==1 || (StiffDataD.nStiffPos==2 && k==1) ) // Right, Both
								{
									bMirror = TRUE;
									theta = (90.0 * CMathFunc::m_trrad);

									if(StiffDataD.nStiffPos==1) dSpacing += StiffDataD.dSpacing; // Both 일때는 Left 랑 위치 동일

									if(theta == 0.0) tempx = 0.0;
									else             tempx = dSpacing / tan(theta);

									tempx += Size.B1+Size.B2+((StiffSubD.nDeckPart-1)*Size.B6)+(Size.tw2*0.5);
									tempy = Size.tf1;
								}
							}

							///////////////////////////
							dy_org = tempx;
							dz_org = tempy + dSpacing;
							///////////////////////////
						}
						else if(StiffSubD.nDeckPos == 2) // Bot Flange
						{
							bMirror = TRUE;
							theta = 0.0;

							if(nCellNum == 1)
							{
								if     (StiffSubD.nDeckPart == 0) { tempx = Size.Bot;                          } // Deck 1
								else if(StiffSubD.nDeckPart == 1) { tempx = Size.Bot+Size.B4;                  } // Deck 2
								else if(StiffSubD.nDeckPart == 2) { tempx = Size.Bot+Size.B4+Size.B5+Size.Br5; } // Deck 3
								else                              {} // Nothing
							}
							else
							{
								if     (StiffSubD.nDeckPart == 0)            { tempx = Size.Bot;                                              } // Deck 1
								else if(StiffSubD.nDeckPart == 1)            { tempx = Size.Bot+Size.B4;                                      } // Deck 2
								else if(StiffSubD.nDeckPart == (nCellNum))   { tempx = Size.Bot+Size.B4+Size.B5+(nDuplCell*Size.B6);          } // Deck n-1
								else if(StiffSubD.nDeckPart == (nCellNum+1)) { tempx = Size.Bot+Size.B4+Size.B5+(nDuplCell*Size.B6)+Size.Br5; } // Deck n
								else
								{
									tempx = Size.Bot+Size.B4+Size.B5+((StiffSubD.nDeckPart-2)*Size.B6);
								}
							}	

							tempy = Size.tf1 + Size.H;

							dSpacing += StiffDataD.dSpacing;

							///////////////////////////
							dy_org = tempx + dSpacing;
							dz_org = tempy;
							///////////////////////////
						}
						else if(StiffSubD.nDeckPos == 3) // Side Flange
						{
							bMirror = TRUE;
							if(StiffSubD.nDeckPart == 0)
							{
								if(Size.sideB > 0.0)
								{
									theta = atan((Size.rh3-Size.rh1) / Size.sideB);
								}

								dSpacing += StiffDataD.dSpacing;

								tempx = Size.rb + Size.rt1;

								if(theta == 0.0) tempy = 0.0;
								else             tempy = dSpacing * tan(theta);

								tempy += Size.tf1 + Size.rh1 - Size.rt2p;
							}
							else
							{
								if(Size.sideBr > 0.0)
								{
									theta = -atan((Size.rhr3-Size.rhr1) / Size.sideBr);
								}

								dSpacing += StiffDataD.dSpacing;

								tempx = Size.B1+Size.B2+(nDuplCell*Size.B6)+Size.Br2+Size.Br1-Size.rbr-Size.rtr1-Size.sideBr;
								
								if(theta == 0.0) tempy = 0.0;
								else             tempy = dSpacing * tan(theta);

								tempy += Size.tf1 + Size.rhr3 - Size.rtr2p;
							}

							///////////////////////////
							dy_org = tempx + dSpacing;
							dz_org = tempy;
							///////////////////////////
						}
						else 
						{
							// Nothing
						}

						if(StiffShapeD.nType == 0) // I
						{
							dS_H = StiffShapeD.dSize[0];
							dS_B = StiffShapeD.dSize[1];

							MakePictureH_SODStiffener(dS_H, dS_B, theta, dy_org, dz_org, pPoly);
							aGeom.Add(pPoly);
						}
						else if(StiffShapeD.nType == 1) // T
						{
							dS_H  = StiffShapeD.dSize[0];
							dS_B  = StiffShapeD.dSize[1];
							dS_tw = StiffShapeD.dSize[2];
							dS_tf = StiffShapeD.dSize[3];

							MakePictureT_SODStiffener(dS_H, dS_B, dS_tw, dS_tf, theta, dy_org, dz_org, pPoly);
							aGeom.Add(pPoly);	
						}
						else if(StiffShapeD.nType == 2) // U-Rib
						{
							dS_H  = StiffShapeD.dSize[0];
							dS_B1 = StiffShapeD.dSize[1];
							dS_B2 = StiffShapeD.dSize[2];
							dS_t  = StiffShapeD.dSize[3];
							dS_R  = StiffShapeD.dSize[4];

							MakePictureURib_SODStiffener(dS_H, dS_B1, dS_B2, dS_t, dS_R, theta, dy_org, dz_org, pPoly);
							aGeom.Add(pPoly);
						}
						else if(StiffShapeD.nType == 3) // L
						{
							dS_H  = StiffShapeD.dSize[0];
							dS_B  = StiffShapeD.dSize[1];
							dS_tw = StiffShapeD.dSize[2];
							dS_tf = StiffShapeD.dSize[3];

							int nDir = StiffDataD.nStiffDir;
							if(bMirror) 
							{
								nDir = (nDir==0)? 1 : 0 ;
							}

							MakePictureL_SODStiffener(dS_H, dS_B, dS_tw, dS_tf, nDir, theta, dy_org, dz_org, pPoly);
							aGeom.Add(pPoly);	
						}
					}
				}
			}
		}
		else if(m_pDataSrc->SectBefore.nFlangeShape == 3) // Closed
		{
			if(m_pDataSrc->SectBefore.nCellNum == 0) // 0 Cell 
			{
				if(m_pDataSrc->SectBefore.nCellType == 0) // Left
				{
					double dDeckLength = 0.0;
					if(StiffSubD.nDeckPos == 0) // Top Flange
					{
						dDeckLength = Size.B1;
					}
					else if(StiffSubD.nDeckPos == 1) // Web
					{
						if(Size.J2)
						{
							if     (StiffSubD.nDeckPart == 0) { dDeckLength = sqrt( (Size.jb1-Size.B2)*(Size.jb1-Size.B2) + (Size.tf1+Size.jh1)*(Size.tf1+Size.jh1));   }
							else if(StiffSubD.nDeckPart == 1) { dDeckLength = sqrt( (Size.jb1-Size.jb2)*(Size.jb1-Size.jb2) + (Size.jh2-Size.jh1)*(Size.jh2-Size.jh1)); }
							else if(StiffSubD.nDeckPart == 2) { dDeckLength = sqrt( Size.jb2*Size.jb2 + (Size.H+Size.tf2-Size.jh2)*(Size.H+Size.tf2-Size.jh2));         }
							else                              { ASSERT(0);                                                                                              }
						}
						else
						{
							if     (StiffSubD.nDeckPart == 0) { dDeckLength = sqrt( (Size.jb1-Size.B2)*(Size.jb1-Size.B2) + (Size.tf1+Size.jh1)*(Size.tf1+Size.jh1)); }
							else if(StiffSubD.nDeckPart == 1) { dDeckLength = sqrt( Size.jb1*Size.jb1 + (Size.H+Size.tf2-Size.jh1)*(Size.H+Size.tf2-Size.jh1));       }
							else                              { ASSERT(0);                                                                                            }
						}
					}
					else if(StiffSubD.nDeckPos == 2) // Bot Flange
					{
						dDeckLength = Size.B3;
					}

					CDBDoc::GetDocPoint()->m_pAttrCtrl->ConvertSODStiffnerRefPos(StiffSubD, dDeckLength);


					for(int j=0; j<StiffSubD.nNum; ++j)
					{
						T_SECT_SOD_STIFF_DATA_D StiffDataD; StiffDataD.Initialize();
						StiffDataD = StiffSubD.aStiffData[j];

						int nStiffShape = 0;
						for(int k=0; k<m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape.GetSize(); ++k)
						{
							if(m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape[k].strName == StiffDataD.strShapeName)
							{
								nStiffShape = k;
								break;
							}
						}

						T_STIFF_SHAPE_D StiffShapeD; StiffShapeD.Initialize();
						StiffShapeD = m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape[nStiffShape];

						if(StiffDataD.nStiffPos==2) nStiffPosRoof = 2;
						else                        nStiffPosRoof = 1;

						for(int k=0; k<nStiffPosRoof; ++k)
						{
							BOOL bMirror = FALSE;
							CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
							pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);

							if(StiffSubD.nDeckPos == 0) // Top Flange
							{
								theta = 180.0 * CMathFunc::m_trrad;
								dSpacing += StiffDataD.dSpacing;

								tempx = Size.Top;
								tempy = Size.tf1;

								///////////////////////////
								dy_org = tempx + dSpacing;
								dz_org = tempy;
								///////////////////////////
							}
 							else if(StiffSubD.nDeckPos == 1) // Web
 							{
								bMirror = TRUE;
								double side_B = 0.0;
								double side_H = 0.0;

								if(Size.J2)
								{
									if(StiffSubD.nDeckPart == 0)
									{
										side_B = Size.jb1-Size.B2;
										side_H = Size.tf1+Size.jh1;
									}
									else if(StiffSubD.nDeckPart == 1)
									{
										side_B = Size.jb2-Size.jb1;
										side_H = Size.jh2-Size.jh1;
									}
									else if(StiffSubD.nDeckPart == 2)
									{
										side_B = -Size.jb2;
										side_H = Size.H-Size.jh2+Size.tf2;
									}
									else 
									{ 
										ASSERT(0); 
									}
								}
								else
								{
									if(StiffSubD.nDeckPart == 0)
									{
										side_B = Size.jb1-Size.B2;
										side_H = Size.tf1+Size.jh1;
									}
									else if(StiffSubD.nDeckPart == 1)
									{
										side_B = -Size.jb1;
										side_H = Size.H-Size.jh1+Size.tf2;
									}
									else 
									{ 
										ASSERT(0); 
									}
								}

								if(side_B > 0.0)
								{
									theta = (180.0 * CMathFunc::m_trrad)-atan(side_H/side_B);	
								}
								else if(side_B < 0.0)
								{
									theta = -atan(side_H/side_B);
								}
								else
								{
									theta = (90.0 * CMathFunc::m_trrad);
								}

								dSpacing += StiffDataD.dSpacing;

								double dRatio = 0.0;
								double InWebLine[2][2];

								if(dDeckLength > 0.0)
								{
									dRatio = dSpacing/dDeckLength;

									if(Size.J2)
									{
										if(StiffSubD.nDeckPart == 0)
										{
											InWebLine[0][0] = Size.Top;
											InWebLine[0][1] = 0.0;
											InWebLine[1][0] = Size.Top+((Size.Joint1-Size.Top)*dRatio);
											InWebLine[1][1] = (Size.tf1+Size.jh1)*dRatio;
										}
										else if(StiffSubD.nDeckPart == 1)
										{
											InWebLine[0][0] = Size.Joint1;
											InWebLine[0][1] = Size.tf1+Size.jh1;
											InWebLine[1][0] = Size.Joint1+((Size.Joint2-Size.Joint1)*dRatio);
											InWebLine[1][1] = Size.tf1+Size.jh1+(Size.jh2-Size.jh1)*dRatio;
										}
										else if(StiffSubD.nDeckPart == 2)
										{
											InWebLine[0][0] = Size.Joint2;         	               
											InWebLine[0][1] = Size.tf1+Size.jh2;
											InWebLine[1][0] = Size.Joint2+((Size.Bot-Size.Joint2)*dRatio);    
											InWebLine[1][1] = Size.tf1+Size.jh2+(Size.H-Size.jh2+Size.tf2)*dRatio;
										}
										else 
										{ 
											ASSERT(0); 
										}
									}
									else
									{
										if(StiffSubD.nDeckPart == 0)
										{
											InWebLine[0][0] = Size.Top;
											InWebLine[0][1] = 0.0;
											InWebLine[1][0] = Size.Top+((Size.Joint1-Size.Top)*dRatio);
											InWebLine[1][1] = (Size.tf1+Size.jh1)*dRatio;
										}
										else if(StiffSubD.nDeckPart == 1)
										{
											InWebLine[0][0] = Size.Joint1;
											InWebLine[0][1] = Size.tf1+Size.jh1;
											InWebLine[1][0] = Size.Joint1+((Size.Bot-Size.Joint1)*dRatio);    
											InWebLine[1][1] = Size.tf1+Size.jh1+(Size.H-Size.jh1+Size.tf2)*dRatio;
										}
										else 
										{ 
											ASSERT(0); 
										}
									}
								}
								
								GetInWebLine_MCell_Closed(InWebLine, Size.tw1, 0);

								tempx = InWebLine[1][0];
								tempy = InWebLine[1][1];

								///////////////////////////
								dy_org = tempx;
								dz_org = tempy;
								///////////////////////////
 							}
							else if(StiffSubD.nDeckPos == 2) // Bot Flange
							{
								bMirror = TRUE;
								theta = 0.0;

								tempx = Size.Bot;
								tempy = Size.tf1 + Size.H;

								dSpacing += StiffDataD.dSpacing;

								///////////////////////////
								dy_org = tempx + dSpacing;
								dz_org = tempy;
								///////////////////////////
							}
							else 
							{
								// Nothing
							}

							if(StiffShapeD.nType == 0) // I
							{
								dS_H = StiffShapeD.dSize[0];
								dS_B = StiffShapeD.dSize[1];

								MakePictureH_SODStiffener(dS_H, dS_B, theta, dy_org, dz_org, pPoly);
								aGeom.Add(pPoly);
							}
							else if(StiffShapeD.nType == 1) // T
							{
								dS_H  = StiffShapeD.dSize[0];
								dS_B  = StiffShapeD.dSize[1];
								dS_tw = StiffShapeD.dSize[2];
								dS_tf = StiffShapeD.dSize[3];

								MakePictureT_SODStiffener(dS_H, dS_B, dS_tw, dS_tf, theta, dy_org, dz_org, pPoly);
								aGeom.Add(pPoly);	
							}
							else if(StiffShapeD.nType == 2) // U-Rib
							{
								dS_H  = StiffShapeD.dSize[0];
								dS_B1 = StiffShapeD.dSize[1];
								dS_B2 = StiffShapeD.dSize[2];
								dS_t  = StiffShapeD.dSize[3];
								dS_R  = StiffShapeD.dSize[4];

								MakePictureURib_SODStiffener(dS_H, dS_B1, dS_B2, dS_t, dS_R, theta, dy_org, dz_org, pPoly);
								aGeom.Add(pPoly);
							}
							else if(StiffShapeD.nType == 3) // L
							{
								dS_H  = StiffShapeD.dSize[0];
								dS_B  = StiffShapeD.dSize[1];
								dS_tw = StiffShapeD.dSize[2];
								dS_tf = StiffShapeD.dSize[3];

								int nDir = StiffDataD.nStiffDir;
								if(bMirror) 
								{
									nDir = (nDir==0)? 1 : 0 ;
								}

								MakePictureL_SODStiffener(dS_H, dS_B, dS_tw, dS_tf, nDir, theta, dy_org, dz_org, pPoly);
								aGeom.Add(pPoly);	
							}
						}
					}
				}
				else // Right
				{
					double dDeckLength = 0.0;
					if(StiffSubD.nDeckPos == 0) // Top Flange
					{
						dDeckLength = Size.Br1;
					}
					else if(StiffSubD.nDeckPos == 1) // Web
					{
						if(Size.Jr2)
						{
							if     (StiffSubD.nDeckPart == 0) { dDeckLength = sqrt( (Size.jbr1-Size.Br2)*(Size.jbr1-Size.Br2) + (Size.tf1+Size.jhr1)*(Size.tf1+Size.jhr1));     }
							else if(StiffSubD.nDeckPart == 1) { dDeckLength = sqrt( (Size.jbr1-Size.jbr2)*(Size.jbr1-Size.jbr2) + (Size.jhr2-Size.jhr1)*(Size.jhr2-Size.jhr1)); }
							else if(StiffSubD.nDeckPart == 2) { dDeckLength = sqrt( Size.jbr2*Size.jbr2 + (Size.H+Size.tf2-Size.jhr2)*(Size.H+Size.tf2-Size.jhr2));             }
							else                              { ASSERT(0);                                                                                                      }
						}
						else
						{
							if     (StiffSubD.nDeckPart == 0) { dDeckLength = sqrt( (Size.jbr1-Size.Br2)*(Size.jbr1-Size.Br2) + (Size.tf1+Size.jhr1)*(Size.tf1+Size.jhr1)); }
							else if(StiffSubD.nDeckPart == 1) { dDeckLength = sqrt( Size.jbr1*Size.jbr1 + (Size.H+Size.tf2-Size.jhr1)*(Size.H+Size.tf2-Size.jhr1));         }
							else                              { ASSERT(0);                                                                                                  }
						}
					}
					else if(StiffSubD.nDeckPos == 2) // Bot Flange
					{
						dDeckLength = Size.Br3;
					}

					CDBDoc::GetDocPoint()->m_pAttrCtrl->ConvertSODStiffnerRefPos(StiffSubD, dDeckLength);

					for(int j=0; j<StiffSubD.nNum; ++j)
					{
						T_SECT_SOD_STIFF_DATA_D StiffDataD; StiffDataD.Initialize();
						StiffDataD = StiffSubD.aStiffData[j];

						int nStiffShape = 0;
						for(int k=0; k<m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape.GetSize(); ++k)
						{
							if(m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape[k].strName == StiffDataD.strShapeName)
							{
								nStiffShape = k;
								break;
							}
						}

						T_STIFF_SHAPE_D StiffShapeD; StiffShapeD.Initialize();
						StiffShapeD = m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape[nStiffShape];

						if(StiffDataD.nStiffPos==2) nStiffPosRoof = 2;
						else                        nStiffPosRoof = 1;

						for(int k=0; k<nStiffPosRoof; ++k)
						{
							BOOL bMirror = FALSE;
							CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
							pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);

							if(StiffSubD.nDeckPos == 0) // Top Flange
							{
								theta = 180.0 * CMathFunc::m_trrad;
								dSpacing += StiffDataD.dSpacing;

								tempx = 0.0;
								tempy = Size.tf1;

								///////////////////////////
								dy_org = tempx + dSpacing;
								dz_org = tempy;
								///////////////////////////
							}
 							else if(StiffSubD.nDeckPos == 1) // Web
 							{
 								double side_B = 0.0;
 								double side_H = 0.0;

								if(Size.Jr2)
								{
									if(StiffSubD.nDeckPart == 0)
									{
										side_B = -(Size.jbr1-Size.Br2);
										side_H = Size.tf1+Size.jhr1;
									}
									else if(StiffSubD.nDeckPart == 1)
									{
										side_B = -(Size.jbr2-Size.jbr1);
										side_H = Size.jhr2-Size.jhr1;
									}
									else if(StiffSubD.nDeckPart == 2)
									{
										side_B = Size.jbr2;
										side_H = Size.H-Size.jhr2+Size.tf2;
									}
									else 
									{ 
										ASSERT(0); 
									}
								}
								else
								{
									if(StiffSubD.nDeckPart == 0)
									{
										side_B = -(Size.jbr1-Size.Br2);
										side_H = Size.tf1+Size.jhr1;
									}
									else if(StiffSubD.nDeckPart == 1)
									{
										side_B = Size.jbr1;
										side_H = Size.H-Size.jhr1+Size.tf2;
									}
									else 
									{ 
										ASSERT(0); 
									}
								}

								if(side_B > 0.0)
								{
									theta = -atan(side_H/side_B);
								}
								else if(side_B < 0.0)
								{
									theta = (180.0 * CMathFunc::m_trrad)-atan(side_H/side_B);										
								}
								else
								{
									theta = (-90.0 * CMathFunc::m_trrad);
								}

 								dSpacing += StiffDataD.dSpacing;

								double dRatio = 0.0;
								double InWebLine[2][2]; 

								if(dDeckLength > 0.0)
								{
									dRatio = dSpacing/dDeckLength;

									if(Size.Jr2)
									{
										if(StiffSubD.nDeckPart == 0)
										{
											InWebLine[0][0] = Size.Br1;
											InWebLine[0][1] = 0.0;
											InWebLine[1][0] = Size.Br1+((Size.jbr1-Size.Br2)*dRatio);
											InWebLine[1][1] = (Size.tf1+Size.jhr1)*dRatio;
										}
										else if(StiffSubD.nDeckPart == 1)
										{
											InWebLine[0][0] = Size.Br3+Size.jbr1;
											InWebLine[0][1] = Size.tf1+Size.jhr1;
											InWebLine[1][0] = Size.Br3+Size.jbr1+((Size.jbr2-Size.jbr1)*dRatio);
											InWebLine[1][1] = Size.tf1+Size.jhr1+(Size.jhr2-Size.jhr1)*dRatio;
										}
										else if(StiffSubD.nDeckPart == 2)
										{
											InWebLine[0][0] = Size.Br3+Size.jbr2;         	               
											InWebLine[0][1] = Size.tf1+Size.jhr2;
											InWebLine[1][0] = Size.Br3+Size.jbr2-(Size.jbr2*dRatio);    
											InWebLine[1][1] = Size.tf1+Size.jhr2+(Size.H-Size.jhr2+Size.tf2)*dRatio;
										}
										else 
										{ 
											ASSERT(0); 
										}
									}
									else
									{
										if(StiffSubD.nDeckPart == 0)
										{
											InWebLine[0][0] = Size.Br1;
											InWebLine[0][1] = 0.0;
											InWebLine[1][0] = Size.Br1+((Size.jbr1-Size.Br2)*dRatio);
											InWebLine[1][1] = (Size.tf1+Size.jhr1)*dRatio;
										}
										else if(StiffSubD.nDeckPart == 1)
										{
											InWebLine[0][0] = Size.Br3+Size.jbr1;
											InWebLine[0][1] = Size.tf1+Size.jhr1;
											InWebLine[1][0] = Size.Br3+Size.jbr1-(Size.jbr1*dRatio);    
											InWebLine[1][1] = Size.tf1+Size.jhr1+(Size.H-Size.jhr1+Size.tf2)*dRatio;
										}
										else 
										{ 
											ASSERT(0); 
										}
									}
								}

								GetInWebLine_MCell_Closed(InWebLine, Size.tw1, 1);

								tempx = InWebLine[1][0];
								tempy = InWebLine[1][1];
 
 								///////////////////////////
 								dy_org = tempx;
 								dz_org = tempy;
 								///////////////////////////
 							}
							else if(StiffSubD.nDeckPos == 2) // Bot Flange
							{
								bMirror = TRUE;
								theta = 0.0;

								tempx = 0.0;
								tempy = Size.tf1 + Size.H;

								dSpacing += StiffDataD.dSpacing;

								///////////////////////////
								dy_org = tempx + dSpacing;
								dz_org = tempy;
								///////////////////////////
							}
							else 
							{
								// Nothing
							}

							if(StiffShapeD.nType == 0) // I
							{
								dS_H = StiffShapeD.dSize[0];
								dS_B = StiffShapeD.dSize[1];

								MakePictureH_SODStiffener(dS_H, dS_B, theta, dy_org, dz_org, pPoly);
								aGeom.Add(pPoly);
							}
							else if(StiffShapeD.nType == 1) // T
							{
								dS_H  = StiffShapeD.dSize[0];
								dS_B  = StiffShapeD.dSize[1];
								dS_tw = StiffShapeD.dSize[2];
								dS_tf = StiffShapeD.dSize[3];

								MakePictureT_SODStiffener(dS_H, dS_B, dS_tw, dS_tf, theta, dy_org, dz_org, pPoly);
								aGeom.Add(pPoly);	
							}
							else if(StiffShapeD.nType == 2) // U-Rib
							{
								dS_H  = StiffShapeD.dSize[0];
								dS_B1 = StiffShapeD.dSize[1];
								dS_B2 = StiffShapeD.dSize[2];
								dS_t  = StiffShapeD.dSize[3];
								dS_R  = StiffShapeD.dSize[4];

								MakePictureURib_SODStiffener(dS_H, dS_B1, dS_B2, dS_t, dS_R, theta, dy_org, dz_org, pPoly);
								aGeom.Add(pPoly);
							}
							else if(StiffShapeD.nType == 3) // L
							{
								dS_H  = StiffShapeD.dSize[0];
								dS_B  = StiffShapeD.dSize[1];
								dS_tw = StiffShapeD.dSize[2];
								dS_tf = StiffShapeD.dSize[3];

								int nDir = StiffDataD.nStiffDir;
								if(bMirror) 
								{
									nDir = (nDir==0)? 1 : 0 ;
								}

								MakePictureL_SODStiffener(dS_H, dS_B, dS_tw, dS_tf, nDir, theta, dy_org, dz_org, pPoly);
								aGeom.Add(pPoly);	
							}
						}
					}
				}
			}
			else
			{
				int nCellNum = m_pDataSrc->SectBefore.nCellNum;
				int nWebNum = m_pDataSrc->SectBefore.nCellNum - 1; // 반복되는 Web 수
				int nDuplCell = nCellNum - 2;

				if(nCellNum < 0) nCellNum = 0;
				if(nWebNum  < 0) nWebNum = 0;
				if(nDuplCell < 0) nDuplCell = 0;

				int nLeftWeb  = Size.J2?  3 : 2;
				int nRightWeb = Size.Jr2? 3 : 2;

				double dDeckLength = 0.0;
				if(StiffSubD.nDeckPos == 0) // Top Flange
				{
					if(nCellNum == 1)
					{
						dDeckLength = Size.B1+Size.Br1;
					}
					else
					{
						if     (StiffSubD.nDeckPart == 0)            { dDeckLength = Size.B1;  } // Deck 1
						else if(StiffSubD.nDeckPart == (nCellNum-1)) { dDeckLength = Size.Br1; } // Deck n
						else                                         { dDeckLength = Size.B4;  } // 반복되는 Deck
					}					
				}
				else if(StiffSubD.nDeckPos == 1) // Web
				{
					if(StiffSubD.nDeckPart < nLeftWeb)
					{
						if(Size.J2)
						{
							if     (StiffSubD.nDeckPart == 0) { dDeckLength = sqrt( (Size.jb1-Size.B2)*(Size.jb1-Size.B2) + (Size.tf1+Size.jh1)*(Size.tf1+Size.jh1));   }
							else if(StiffSubD.nDeckPart == 1) { dDeckLength = sqrt( (Size.jb1-Size.jb2)*(Size.jb1-Size.jb2) + (Size.jh2-Size.jh1)*(Size.jh2-Size.jh1)); }
							else if(StiffSubD.nDeckPart == 2) { dDeckLength = sqrt( Size.jb2*Size.jb2 + (Size.H+Size.tf2-Size.jh2)*(Size.H+Size.tf2-Size.jh2));         }
							else                              { ASSERT(0);                                                                                              }
						}
						else
						{
							if     (StiffSubD.nDeckPart == 0) { dDeckLength = sqrt( (Size.jb1-Size.B2)*(Size.jb1-Size.B2) + (Size.tf1+Size.jh1)*(Size.tf1+Size.jh1)); }
							else if(StiffSubD.nDeckPart == 1) { dDeckLength = sqrt( Size.jb1*Size.jb1 + (Size.H+Size.tf2-Size.jh1)*(Size.H+Size.tf2-Size.jh1));       }
							else                              { ASSERT(0);                                                                                            }
						}
					}
					else if(StiffSubD.nDeckPart >= (nLeftWeb+nCellNum-1))
					{
						int nDeckPart = StiffSubD.nDeckPart - nLeftWeb - (nCellNum-1);

						if(Size.Jr2)
						{
							if     (nDeckPart == 0) { dDeckLength = sqrt( (Size.jbr1-Size.Br2)*(Size.jbr1-Size.Br2) + (Size.tf1+Size.jh1)*(Size.tf1+Size.jh1));       }
							else if(nDeckPart == 1) { dDeckLength = sqrt( (Size.jbr1-Size.jbr2)*(Size.jbr1-Size.jbr2) + (Size.jhr2-Size.jhr1)*(Size.jhr2-Size.jhr1)); }
							else if(nDeckPart == 2) { dDeckLength = sqrt( Size.jbr2*Size.jbr2 + (Size.H+Size.tf2-Size.jhr2)*(Size.H+Size.tf2-Size.jhr2));             }
							else                    { ASSERT(0);                                                                                                      }
						}
						else
						{
							if     (nDeckPart == 0) { dDeckLength = sqrt( (Size.jbr1-Size.Br2)*(Size.jbr1-Size.Br2) + (Size.tf1+Size.jh1)*(Size.tf1+Size.jh1)); }
							else if(nDeckPart == 1) { dDeckLength = sqrt( Size.jbr1*Size.jbr1 + (Size.H+Size.tf2-Size.jhr1)*(Size.H+Size.tf2-Size.jhr1));       }
							else                    { ASSERT(0);                                                                                                }
						}
					}
					else
					{
						dDeckLength = Size.H;
					}
				}
				else if(StiffSubD.nDeckPos == 2) // Bot Flange
				{
					if(nCellNum == 1)
					{
						dDeckLength = Size.B3+Size.Br3;
					}
					else
					{
						if     (StiffSubD.nDeckPart == 0)            { dDeckLength = Size.B3;  } // Deck 1
						else if(StiffSubD.nDeckPart == (nCellNum-1)) { dDeckLength = Size.Br3; } // Deck n
						else                                         { dDeckLength = Size.B4;  } // 반복되는 Deck
					}	
				}

				CDBDoc::GetDocPoint()->m_pAttrCtrl->ConvertSODStiffnerRefPos(StiffSubD, dDeckLength);


				for(int j=0; j<StiffSubD.nNum; ++j)
				{
					T_SECT_SOD_STIFF_DATA_D StiffDataD; StiffDataD.Initialize();
					StiffDataD = StiffSubD.aStiffData[j];

					int nStiffShape = 0;
					for(int k=0; k<m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape.GetSize(); ++k)
					{
						if(m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape[k].strName == StiffDataD.strShapeName)
						{
							nStiffShape = k;
							break;
						}
					}

					T_STIFF_SHAPE_D StiffShapeD; StiffShapeD.Initialize();
					StiffShapeD = m_pDataSrc->SectBefore.SectI.SODStiffener.aStiffShape[nStiffShape];

					if(StiffDataD.nStiffPos==2) nStiffPosRoof = 2;
					else                        nStiffPosRoof = 1;

					for(int k=0; k<nStiffPosRoof; ++k)
					{
						BOOL bMirror = FALSE;
						CCMPolygon *pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
						pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);

						if(StiffSubD.nDeckPos == 0) // Top Flange
						{
							theta = 180.0 * CMathFunc::m_trrad;
							dSpacing += StiffDataD.dSpacing;

							if(nCellNum == 1)
							{
								tempx = Size.Top;
							}
							else
							{
								if     (StiffSubD.nDeckPart == 0)            { tempx = Size.Top;                                           } // Deck 1
								else if(StiffSubD.nDeckPart == (nCellNum-1)) { tempx = Size.Top+Size.B1+(nDuplCell*Size.B4)+Size.tw2/2;    } // Deck n
								else                                         { tempx = Size.Top+Size.B1+(StiffSubD.nDeckPart-1)*Size.B4;   } // 반복되는 Deck
							}	

							tempy = Size.tf1;

							///////////////////////////
							dy_org = tempx + dSpacing;
							dz_org = tempy;
							///////////////////////////
						}
						else if(StiffSubD.nDeckPos == 1) // Web
						{
 							if(StiffSubD.nDeckPart < nLeftWeb)
 							{
								bMirror = TRUE;
								double side_B = 0.0;
								double side_H = 0.0;
								
								if(Size.J2)
								{
									if(StiffSubD.nDeckPart == 0)
									{
										side_B = Size.jb1-Size.B2;
										side_H = Size.tf1+Size.jh1;
									}
									else if(StiffSubD.nDeckPart == 1)
									{
										side_B = Size.jb2-Size.jb1;
										side_H = Size.jh2-Size.jh1;
									}
									else if(StiffSubD.nDeckPart == 2)
									{
										side_B = -Size.jb2;
										side_H = Size.H-Size.jh2+Size.tf2;
									}
									else 
									{ 
										ASSERT(0); 
									}
								}
								else
								{
									if(StiffSubD.nDeckPart == 0)
									{
										side_B = Size.jb1-Size.B2;
										side_H = Size.tf1+Size.jh1;
									}
									else if(StiffSubD.nDeckPart == 1)
									{
										side_B = -Size.jb1;
										side_H = Size.H-Size.jh1+Size.tf2;
									}
									else 
									{ 
										ASSERT(0); 
									}
								}
							
								if(side_B > 0.0)
								{
									theta = (180.0 * CMathFunc::m_trrad)-atan(side_H/side_B);	
								}
								else if(side_B < 0.0)
								{
									theta = -atan(side_H/side_B);
								}
								else
								{
									theta = (90.0 * CMathFunc::m_trrad);
								}

								dSpacing += StiffDataD.dSpacing;

								double dRatio = 0.0;
								double InWebLine[2][2];

								if(dDeckLength > 0.0)
								{
									dRatio = dSpacing/dDeckLength;

									if(Size.J2)
									{
										if(StiffSubD.nDeckPart == 0)
										{
											InWebLine[0][0] = Size.Top;
											InWebLine[0][1] = 0.0;
											InWebLine[1][0] = Size.Top+((Size.Joint1-Size.Top)*dRatio);
											InWebLine[1][1] = (Size.tf1+Size.jh1)*dRatio;
										}
										else if(StiffSubD.nDeckPart == 1)
										{
											InWebLine[0][0] = Size.Joint1;
											InWebLine[0][1] = Size.tf1+Size.jh1;
											InWebLine[1][0] = Size.Joint1+((Size.Joint2-Size.Joint1)*dRatio);
											InWebLine[1][1] = Size.tf1+Size.jh1+(Size.jh2-Size.jh1)*dRatio;
										}
										else if(StiffSubD.nDeckPart == 2)
										{
											InWebLine[0][0] = Size.Joint2;         	               
											InWebLine[0][1] = Size.tf1+Size.jh2;
											InWebLine[1][0] = Size.Joint2+((Size.Bot-Size.Joint2)*dRatio);    
											InWebLine[1][1] = Size.tf1+Size.jh2+(Size.H-Size.jh2+Size.tf2)*dRatio;
										}
										else 
										{ 
											ASSERT(0); 
										}
									}
									else
									{
										if(StiffSubD.nDeckPart == 0)
										{
											InWebLine[0][0] = Size.Top;
											InWebLine[0][1] = 0.0;
											InWebLine[1][0] = Size.Top+((Size.Joint1-Size.Top)*dRatio);
											InWebLine[1][1] = (Size.tf1+Size.jh1)*dRatio;
										}
										else if(StiffSubD.nDeckPart == 1)
										{
											InWebLine[0][0] = Size.Joint1;
											InWebLine[0][1] = Size.tf1+Size.jh1;
											InWebLine[1][0] = Size.Joint1+((Size.Bot-Size.Joint1)*dRatio);    
											InWebLine[1][1] = Size.tf1+Size.jh1+(Size.H-Size.jh1+Size.tf2)*dRatio;
										}
										else 
										{ 
											ASSERT(0); 
										}
									}
								}

								GetInWebLine_MCell_Closed(InWebLine, Size.tw1, 0);

								tempx = InWebLine[1][0];
								tempy = InWebLine[1][1];

								///////////////////////////
								dy_org = tempx;
								dz_org = tempy;
								///////////////////////////
							}
							else if(StiffSubD.nDeckPart >= (nLeftWeb+nCellNum-1))
							{
								double side_B = 0.0;
								double side_H = 0.0;

								int nDeckPart = StiffSubD.nDeckPart - nLeftWeb - (nCellNum-1);

								if(Size.J2)
								{
									if(nDeckPart == 0)
									{
										side_B = -(Size.jbr1-Size.Br2);
										side_H = Size.tf1+Size.jhr1;
									}
									else if(nDeckPart == 1)
									{
										side_B = -(Size.jbr2-Size.jbr1);
										side_H = Size.jhr2-Size.jhr1;
									}
									else if(nDeckPart == 2)
									{
										side_B = Size.jbr2;
										side_H = Size.H-Size.jhr2+Size.tf2;
									}
									else 
									{ 
										ASSERT(0); 
									}
								}
								else
								{
									if(nDeckPart == 0)
									{
										side_B = -(Size.jbr1-Size.Br2);
										side_H = Size.tf1+Size.jhr1;
									}
									else if(nDeckPart == 1)
									{
										side_B = Size.jbr1;
										side_H = Size.H-Size.jhr1+Size.tf2;
									}
									else 
									{ 
										ASSERT(0); 
									}
								}

								if(side_B > 0.0)
								{
									theta = -atan(side_H/side_B);
								}
								else if(side_B < 0.0)
								{
									theta = (180.0 * CMathFunc::m_trrad)-atan(side_H/side_B);										
								}
								else
								{
									theta = (-90.0 * CMathFunc::m_trrad);
								}

								dSpacing += StiffDataD.dSpacing;

								double dRatio = 0.0;
								double InWebLine[2][2];

								if(dDeckLength > 0.0)
								{
									dRatio = dSpacing/dDeckLength;

									if(Size.J2)
									{
										if(nDeckPart == 0)
										{
											InWebLine[0][0] = Size.Top+Size.B1+(nDuplCell*Size.B4)+Size.Br1;
											InWebLine[0][1] = 0.0;
											InWebLine[1][0] = Size.Top+Size.B1+(nDuplCell*Size.B4)+Size.Br1+((Size.jbr1-Size.Br2)*dRatio);
											InWebLine[1][1] = (Size.tf1+Size.jhr1)*dRatio;
										}
										else if(nDeckPart == 1)
										{
											InWebLine[0][0] = Size.Bot+Size.B3+(nDuplCell*Size.B4)+Size.Br3+Size.jbr1;
											InWebLine[0][1] = Size.tf1+Size.jhr1;
											InWebLine[1][0] = Size.Bot+Size.B3+(nDuplCell*Size.B4)+Size.Br3+Size.jbr1+((Size.jbr2-Size.jbr1)*dRatio);
											InWebLine[1][1] = Size.tf1+Size.jhr1+(Size.jhr2-Size.jhr1)*dRatio;
										}
										else if(nDeckPart == 2)
										{
											InWebLine[0][0] = Size.Bot+Size.B3+(nDuplCell*Size.B4)+Size.Br3+Size.jbr2;         	               
											InWebLine[0][1] = Size.tf1+Size.jhr2;
											InWebLine[1][0] = Size.Bot+Size.B3+(nDuplCell*Size.B4)+Size.Br3+Size.jbr2-(Size.jbr2*dRatio);    
											InWebLine[1][1] = Size.tf1+Size.jhr2+(Size.H-Size.jhr2+Size.tf2)*dRatio;
										}
										else 
										{ 
											ASSERT(0); 
										}
									}
									else
									{
										if(nDeckPart == 0)
										{
											InWebLine[0][0] = Size.Top+Size.B1+(nDuplCell*Size.B4)+Size.Br1;
											InWebLine[0][1] = 0.0;
											InWebLine[1][0] = Size.Top+Size.B1+(nDuplCell*Size.B4)+Size.Br1+((Size.jbr1-Size.Br2)*dRatio);
											InWebLine[1][1] = (Size.tf1+Size.jhr1)*dRatio;
										}
										else if(nDeckPart == 1)
										{
											InWebLine[0][0] = Size.Bot+Size.B3+(nDuplCell*Size.B4)+Size.Br3+Size.jbr1;
											InWebLine[0][1] = Size.tf1+Size.jhr1;
											InWebLine[1][0] = Size.Bot+Size.B3+(nDuplCell*Size.B4)+Size.Br3+Size.jbr1-(Size.jbr1*dRatio);    
											InWebLine[1][1] = Size.tf1+Size.jhr1+(Size.H-Size.jhr1+Size.tf2)*dRatio;
										}
										else 
										{ 
											ASSERT(0); 
										}
									}
								}

								GetInWebLine_MCell_Closed(InWebLine, Size.tw1, 1);

								tempx = InWebLine[1][0];
								tempy = InWebLine[1][1];

								///////////////////////////
								dy_org = tempx;
								dz_org = tempy;
								///////////////////////////
 							}
							else
							{
								if(StiffDataD.nStiffPos==0 || (StiffDataD.nStiffPos==2 && k==0) ) // Left. Both
								{
									theta = (-90.0 * CMathFunc::m_trrad);
									dSpacing += StiffDataD.dSpacing;

									if(theta == 0.0) tempx = 0.0;
									else             tempx = dSpacing / tan(theta);

									tempx += Size.Top + Size.B1+((StiffSubD.nDeckPart-nLeftWeb)*Size.B4)-(Size.tw2*0.5);
									tempy = Size.tf1;
								}
								else if(StiffDataD.nStiffPos==1 || (StiffDataD.nStiffPos==2 && k==1) ) // Right, Both
								{
									bMirror = TRUE;
									theta = (90.0 * CMathFunc::m_trrad);

									if(StiffDataD.nStiffPos==1) dSpacing += StiffDataD.dSpacing; // Both 일때는 Left 랑 위치 동일

									if(theta == 0.0) tempx = 0.0;
									else             tempx = dSpacing / tan(theta);

									tempx += Size.Top + Size.B1+((StiffSubD.nDeckPart-nLeftWeb)*Size.B4)+(Size.tw2*0.5);
									tempy = Size.tf1;
								}

								///////////////////////////
								dy_org = tempx;
								dz_org = tempy + dSpacing;
								///////////////////////////
							}
						}
						else if(StiffSubD.nDeckPos == 2) // Bot Flange
						{
							bMirror = TRUE;
							theta = 0.0;

							if(nCellNum == 1)
							{
								tempx = Size.Bot;
							}
							else
							{
								if     (StiffSubD.nDeckPart == 0)            { tempx = Size.Bot;                                           } // Deck 1
								else if(StiffSubD.nDeckPart == (nCellNum-1)) { tempx = Size.Bot+Size.B3+(nDuplCell*Size.B4)+Size.tw2/2;    } // Deck n
								else                                         { tempx = Size.Bot+Size.B3+((StiffSubD.nDeckPart-1)*Size.B4); } // 반복되는 Deck
							}	
							
							tempy = Size.tf1 + Size.H;

							dSpacing += StiffDataD.dSpacing;

							///////////////////////////
							dy_org = tempx + dSpacing;
							dz_org = tempy;
							///////////////////////////
						}
						else 
						{
							// Nothing
						}

						if(StiffShapeD.nType == 0) // I
						{
							dS_H = StiffShapeD.dSize[0];
							dS_B = StiffShapeD.dSize[1];

							MakePictureH_SODStiffener(dS_H, dS_B, theta, dy_org, dz_org, pPoly);
							aGeom.Add(pPoly);
						}
						else if(StiffShapeD.nType == 1) // T
						{
							dS_H  = StiffShapeD.dSize[0];
							dS_B  = StiffShapeD.dSize[1];
							dS_tw = StiffShapeD.dSize[2];
							dS_tf = StiffShapeD.dSize[3];

							MakePictureT_SODStiffener(dS_H, dS_B, dS_tw, dS_tf, theta, dy_org, dz_org, pPoly);
							aGeom.Add(pPoly);	
						}
						else if(StiffShapeD.nType == 2) // U-Rib
						{
							dS_H  = StiffShapeD.dSize[0];
							dS_B1 = StiffShapeD.dSize[1];
							dS_B2 = StiffShapeD.dSize[2];
							dS_t  = StiffShapeD.dSize[3];
							dS_R  = StiffShapeD.dSize[4];

							MakePictureURib_SODStiffener(dS_H, dS_B1, dS_B2, dS_t, dS_R, theta, dy_org, dz_org, pPoly);
							aGeom.Add(pPoly);
						}
						else if(StiffShapeD.nType == 3) // L
						{
							dS_H  = StiffShapeD.dSize[0];
							dS_B  = StiffShapeD.dSize[1];
							dS_tw = StiffShapeD.dSize[2];
							dS_tf = StiffShapeD.dSize[3];

							int nDir = StiffDataD.nStiffDir;
							if(bMirror) 
							{
								nDir = (nDir==0)? 1 : 0 ;
							}

							MakePictureL_SODStiffener(dS_H, dS_B, dS_tw, dS_tf, nDir, theta, dy_org, dz_org, pPoly);
							aGeom.Add(pPoly);	
						}
					}
				}
			}
		}
		else
		{
			ASSERT(0);
			return;
		}
	}
}

void ICMGeomMaker::GetInWebLine_MCell_Closed(double InWebLine[2][2], double tw, int nWebPos)
{
	// nWebPos -> 0:Left, 1:Right
	double dx, dy;
	double ux, uy;
	dx = InWebLine[1][0]-InWebLine[0][0];
	dy = InWebLine[1][1]-InWebLine[0][1];
	CMathFunc::mathNormalize(dx, dy, ux, uy);

	if(nWebPos == 0)
	{
		InWebLine[0][0]+=uy*tw, InWebLine[0][1]-=ux*tw;
		InWebLine[1][0]+=uy*tw, InWebLine[1][1]-=ux*tw;
	}
	else
	{
		InWebLine[0][0]-=uy*tw, InWebLine[0][1]+=ux*tw;
		InWebLine[1][0]-=uy*tw, InWebLine[1][1]+=ux*tw;
	}	
}

void ICMGeomMaker::MakePictureRivet_I(CCMGeom2DArray& aGeom, T_SECT_SOD_STIFFNER_I& Size)
{
	double Top = Size.Top;
	double Bot = Size.Bot;
	double B1 = Size.B1;
	double B2 = Size.B2;
	double B3 = Size.B3;
	double B4 = Size.B4;
	double H  = Size.H;
	double t1 = Size.t1;
	double t2 = Size.t2;
	double twp = Size.twp;

	double dS_H = 0.0;
	double dS_B = 0.0;
	double dS_tw = 0.0;
	double dS_tf = 0.0;

	double tempx = 0.0;
	double tempy = 0.0;
	double dy_org = 0.0;
	double dz_org = 0.0;
	int nStiffShape = m_pDataSrc->SectBefore.SectI.RivetStiff.aStiffShape.GetSize();
	int nStiffSize  = m_pDataSrc->SectBefore.SectI.RivetStiff.aStiffPosD.GetSize();   // nStiffSize = 8
	int nRivetSize  = m_pDataSrc->SectBefore.SectI.RivetStiff.aRivetPosD.GetSize();
	BOOL bRivet     = m_pDataSrc->SectBefore.SectI.RivetStiff.bRivet;

	double dx[8], dy[8]; // Rivet Position
	// Top Flange
	tempx = Top;           tempy = 0.0;
	tempx = Top + B1 + B2; tempy = 0.0;
	tempx = Top + B1 + B2; tempy = t1;            dx[6] = tempx; dy[6] = tempy;
	tempx = Top;           tempy = t1;            dx[2] = tempx; dy[2] = tempy;

	// Bottom Flange
	tempx = Bot;           tempy = t1 + H;        dx[3] = tempx; dy[3] = tempy;
	tempx = Bot + B3 + B4; tempy = t1 + H;        dx[7] = tempx; dy[7] = tempy;
	tempx = Bot + B3 + B4; tempy = t1 + H + t2;
	tempx = Bot;           tempy = t1 + H + t2; 

	// Web
	tempx = Top + B1 - (twp / 2); tempy = t1;     dx[0] = tempx; dy[0] = tempy;
	tempx = Top + B1 + (twp / 2); tempy = t1;     dx[4] = tempx; dy[4] = tempy;
	tempx = Bot + B3 + (twp / 2); tempy = t1 + H; dx[5] = tempx; dy[5] = tempy;
	tempx = Bot + B3 - (twp / 2); tempy = t1 + H; dx[1] = tempx; dy[1] = tempy;

	std::map<int, T_SECT_RIVET_POS_D> mapUseRevet;
	std::map<int, T_SECT_RIVET_POS_D> mapUseRevetW;
	CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&> aRivetPosD;
	aRivetPosD.Copy(m_pDataSrc->SectBefore.SectI.RivetStiff.aRivetPosD);

	for (int i = 0; i < aRivetPosD.GetSize(); i++)
	{
		if (aRivetPosD[i].nComponent == RIVET_WEB)
			mapUseRevetW[aRivetPosD[i].nAngle] = aRivetPosD[i];
		else
			mapUseRevet[aRivetPosD[i].nAngle] = aRivetPosD[i];
	}

	if (m_pDataSrc->SectBefore.SectI.RivetStiff.aStiffShape.GetSize() == 0) return;

	for (int i = 0; i < nStiffSize; ++i)
	{
		int nAngle = i;

		T_SECT_STIFF_POS_D StiffPosD; StiffPosD.Initialize();
		StiffPosD = m_pDataSrc->SectBefore.SectI.RivetStiff.aStiffPosD[i];

		if(!StiffPosD.bUseStiff) continue;

		int nStiffShape = 0;
		for (int k = 0; k < m_pDataSrc->SectBefore.SectI.RivetStiff.aStiffShape.GetSize(); ++k)
		{
			if (m_pDataSrc->SectBefore.SectI.RivetStiff.aStiffShape[k].strName == StiffPosD.strShapeName)
			{
				nStiffShape = k;
				break;
			}
		}

		T_STIFF_SHAPE_D StiffShapeD; StiffShapeD.Initialize();
		StiffShapeD = m_pDataSrc->SectBefore.SectI.RivetStiff.aStiffShape[nStiffShape];

		// Angle
		dS_H  = StiffShapeD.dSize[0];
		dS_B  = StiffShapeD.dSize[1];
		dS_tw = StiffShapeD.dSize[2];
		dS_tf = StiffShapeD.dSize[3];

		//   nAngleType 
		//    0    1    2    3
		//  __           __
		//    |    |    |    |
		//    |  __|    |    |__
		//

		int nAngleType = 0;
		if      (nAngle == 0) nAngleType = 0;
		else if (nAngle == 1) nAngleType = 1;
		else if (nAngle == 2) nAngleType = 2;
		else if (nAngle == 3) nAngleType = 3;
		else if (nAngle == 4) nAngleType = 2;
		else if (nAngle == 5) nAngleType = 3;
		else if (nAngle == 6) nAngleType = 0;
		else if (nAngle == 7) nAngleType = 1;
		else { ASSERT(0); continue; }

		///////////////////////////
		dy_org = dx[nAngle];
		dz_org = dy[nAngle];
		///////////////////////////

		CCMPolygon* pPoly;// = new CCMPolygon(CCM_BORDER, CCM_FILL);
		pPoly = new CCMPolygon(CCM_BORDER, CCM_FILL);

		MakePictureAngle_Rivet(dS_H, dS_B, dS_tw, dS_tf, nAngleType, dy_org, dz_org, pPoly);
		aGeom.Add(pPoly);
	}
}

void ICMGeomMaker::MakePictureCompoStlGirderBox(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ)
{
	CCMPolygon *pPolygon1 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon2 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon3 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon4 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon5 = new CCMPolygon(CCM_BORDER, CCM_FILL);

	double SW=0.0, CTC=0.0, Bc=0.0, tc=0.0, Hh=0.0;
	double Sg=0.0, Top=0.0, Bot=0.0;
	double B1=0.0, B2=0.0, B3=0.0, B4=0.0, B5=0.0, B6=0.0;
	double H=0.0, t1=0.0, t2=0.0, tw1=0.0, tw2=0.0;
	int    GNum=0;

	// read ///////////////////////////////////////
	SW   = m_pDataSrc->SectAfter.SectI.Size[0];
	CTC  = m_pDataSrc->SectAfter.SectI.Size[1];
	GNum = m_pDataSrc->SectAfter.SectI.BuiltUpFlag;

	Bc  = m_pDataSrc->SectAfter.SectJ.Size[0];
	tc  = m_pDataSrc->SectAfter.SectJ.Size[1];
	Hh  = m_pDataSrc->SectAfter.SectJ.Size[2];

	BOOL bTapJ = (m_pDataSrc->nStype==D_SECT_TYPE_TAPERED && bJ);
	if(bTapJ)
	{
		Sg  = m_pDataSrc->SectAfter.SectJ.Size[3];
		Top = m_pDataSrc->SectAfter.SectJ.Size[4];
		Bot = m_pDataSrc->SectAfter.SectJ.Size[5];

		B1  = m_pDataSrc->CmpTapJ.Size[0];
		B2  = m_pDataSrc->CmpTapJ.Size[1];
		B3  = m_pDataSrc->CmpTapJ.Size[2];
		B4  = m_pDataSrc->CmpTapJ.Size[3];
		B5  = m_pDataSrc->CmpTapJ.Size[4];
		B6  = m_pDataSrc->CmpTapJ.Size[5];
		H   = m_pDataSrc->CmpTapJ.Size[6];
		t1  = m_pDataSrc->CmpTapJ.Size[7];
		t2  = m_pDataSrc->CmpTapJ.Size[8];
		tw1 = m_pDataSrc->CmpTapJ.Size[9];
		tw2 = m_pDataSrc->CmpTapJ.Size[10];
	}
	else
	{
		Sg  = m_pDataSrc->SectAfter.SectI.Size[3];
		Top = m_pDataSrc->SectAfter.SectI.Size[4];
		Bot = m_pDataSrc->SectAfter.SectI.Size[5];

		B1  = m_pDataSrc->SectBefore.SectI.Size[0];
		B2  = m_pDataSrc->SectBefore.SectI.Size[1];
		B3  = m_pDataSrc->SectBefore.SectI.Size[2];
		B4  = m_pDataSrc->SectBefore.SectI.Size[3];
		B5  = m_pDataSrc->SectBefore.SectI.Size[4];
		B6  = m_pDataSrc->SectBefore.SectI.Size[5];
		H   = m_pDataSrc->SectBefore.SectI.Size[6];
		t1  = m_pDataSrc->SectBefore.SectI.Size[7];
		t2  = m_pDataSrc->SectBefore.SectI.Size[8];
		tw1 = m_pDataSrc->SectBefore.SectI.Size[9];
		tw2 = m_pDataSrc->SectBefore.SectI.Size[10];
	}

	double tempx = 0.0;
	double tempy = 0.0;
	double twp_L = 0.0;
	double twp_R = 0.0;

	// Slab 
	tempx = Sg;           tempy = 0.0;       pPolygon1->Add(SCMPoint2D(tempx, tempy));
	tempx = Sg+Bc;        tempy = 0.0;       pPolygon1->Add(SCMPoint2D(tempx, tempy));
	tempx = Sg+Bc;        tempy = tc;        pPolygon1->Add(SCMPoint2D(tempx, tempy));
	tempx = Sg;           tempy = tc;        pPolygon1->Add(SCMPoint2D(tempx, tempy));
	if(bTapJ) aGeomPart2[1].Add(pPolygon1);
	else      aGeomPart2[0].Add(pPolygon1);
		
	// Top Flange
	tempx = Top;          tempy = tc+Hh;     pPolygon2->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1+B2+B3; tempy = tc+Hh;     pPolygon2->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1+B2+B3; tempy = tc+Hh+t1;  pPolygon2->Add(SCMPoint2D(tempx, tempy));
	tempx = Top;          tempy = tc+Hh+t1;  pPolygon2->Add(SCMPoint2D(tempx, tempy));
	if(bTapJ) aGeomPart1[1].Add(pPolygon2);
	else      aGeomPart1[0].Add(pPolygon2);

	// Bottom Flange
	tempx = Bot;          tempy = tc+Hh+t1+H;    pPolygon3->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B4+B5+B6; tempy = tc+Hh+t1+H;    pPolygon3->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B4+B5+B6; tempy = tc+Hh+t1+H+t2; pPolygon3->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot;          tempy = tc+Hh+t1+H+t2; pPolygon3->Add(SCMPoint2D(tempx, tempy));
	if(bTapJ) aGeomPart1[1].Add(pPolygon3);
	else      aGeomPart1[0].Add(pPolygon3);

	// Left Web
	// 1. Web 의 높이가 한변을 이루고 있는 직각 삼각형의 한변을 구한다. (Top + B1) - (Bot + B4) 
	double dSide_L = fabs((Top + B1) - (Bot + B4));

	// 2. 비례식을 이용해서 변환된 Web 의 길이를 구한다.
	if(H > 0.0)
	{
		twp_L = tw1/H * sqrt(H*H + dSide_L*dSide_L);
	}

	tempx = Top+B1-twp_L; tempy = tc+Hh+t1;   pPolygon4->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1;       tempy = tc+Hh+t1;   pPolygon4->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B4;       tempy = tc+Hh+t1+H; pPolygon4->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B4-twp_L; tempy = tc+Hh+t1+H; pPolygon4->Add(SCMPoint2D(tempx, tempy));
	if(bTapJ) aGeomPart1[1].Add(pPolygon4);
	else      aGeomPart1[0].Add(pPolygon4);

	// Right Web
	// 1. Web 의 높이가 한변을 이루고 있는 직각 삼각형의 한변을 구한다. (Top + B1 + B2) - (Bot + B4 + B5) 
	double dSide_R = fabs((Top + B1 + B2) - (Bot + B4 + B5));

	// 2. 비례식을 이용해서 변환된 Web 의 길이를 구한다.
	if(H > 0.0)
	{
		twp_R = tw2/H * sqrt(H*H + dSide_R*dSide_R);
	}

	tempx = Top+B1+B2;       tempy = tc+Hh+t1;   pPolygon5->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1+B2+twp_R; tempy = tc+Hh+t1;   pPolygon5->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B4+B5+twp_R; tempy = tc+Hh+t1+H; pPolygon5->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B4+B5;       tempy = tc+Hh+t1+H; pPolygon5->Add(SCMPoint2D(tempx, tempy));
	if(bTapJ) aGeomPart1[1].Add(pPolygon5);
	else      aGeomPart1[0].Add(pPolygon5);

	T_SECT_SOD_STIFFNER_B Size; Size.Initialize();
	Size.Top = Top;
	Size.Bot = Bot;
	Size.B1 = B1;
	Size.B2 = B2;
	Size.B3 = B3;
	Size.B4 = B4;
	Size.B5 = B5;
	Size.B6 = B6;
	Size.H = H;
	Size.t1 = t1;
	Size.t2 = t2;
	Size.twp_L = twp_L;
	Size.twp_R = twp_R;
	MakePictureSODStiffner_B(aGeom, Size, tc+Hh);
	
	aGeom.Add(pPolygon1);
	aGeom.Add(pPolygon2);
	aGeom.Add(pPolygon3);
	aGeom.Add(pPolygon4);
	aGeom.Add(pPolygon5);
}

void ICMGeomMaker::MakePictureCompoStlGirderI(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ)
{
	CCMPolygon *pPolygon1 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon2 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon3 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon4 = new CCMPolygon(CCM_BORDER, CCM_FILL);

	double SW=0.0, CTC=0.0, Bc=0.0, tc=0.0, Hh=0.0;
	double Sg=0.0, Top=0.0, Bot=0.0;
	double B1=0.0, B2=0.0, B3=0.0, B4=0.0;
	double H=0.0, t1=0.0, t2=0.0, tw=0.0;
	int    GNum=0;

	// read ///////////////////////////////////////
	SW   = m_pDataSrc->SectAfter.SectI.Size[0];
	CTC  = m_pDataSrc->SectAfter.SectI.Size[1];
	GNum = m_pDataSrc->SectAfter.SectI.BuiltUpFlag;

	Bc  = m_pDataSrc->SectAfter.SectJ.Size[0];
	tc  = m_pDataSrc->SectAfter.SectJ.Size[1];
	Hh  = m_pDataSrc->SectAfter.SectJ.Size[2];

	BOOL bTapJ = (m_pDataSrc->nStype==D_SECT_TYPE_TAPERED && bJ);
	if(bTapJ)
	{
		Sg  = m_pDataSrc->SectAfter.SectJ.Size[3];
		Top = m_pDataSrc->SectAfter.SectJ.Size[4];
		Bot = m_pDataSrc->SectAfter.SectJ.Size[5];

		B1  = m_pDataSrc->CmpTapJ.Size[0];
		B2  = m_pDataSrc->CmpTapJ.Size[1];
		B3  = m_pDataSrc->CmpTapJ.Size[2];
		B4  = m_pDataSrc->CmpTapJ.Size[3];
		H   = m_pDataSrc->CmpTapJ.Size[4];
		t1  = m_pDataSrc->CmpTapJ.Size[5];
		t2  = m_pDataSrc->CmpTapJ.Size[6];
		tw  = m_pDataSrc->CmpTapJ.Size[7];
	}
	else
	{
		Sg  = m_pDataSrc->SectAfter.SectI.Size[3];
		Top = m_pDataSrc->SectAfter.SectI.Size[4];
		Bot = m_pDataSrc->SectAfter.SectI.Size[5];

		B1  = m_pDataSrc->SectBefore.SectI.Size[0];
		B2  = m_pDataSrc->SectBefore.SectI.Size[1];
		B3  = m_pDataSrc->SectBefore.SectI.Size[2];
		B4  = m_pDataSrc->SectBefore.SectI.Size[3];
		H   = m_pDataSrc->SectBefore.SectI.Size[4];
		t1  = m_pDataSrc->SectBefore.SectI.Size[5];
		t2  = m_pDataSrc->SectBefore.SectI.Size[6];
		tw  = m_pDataSrc->SectBefore.SectI.Size[7];
	}

	double tempx = 0.0;
	double tempy = 0.0;

	// Slab 
	tempx = Sg;           tempy = 0.0;       pPolygon1->Add(SCMPoint2D(tempx, tempy));
	tempx = Sg+Bc;        tempy = 0.0;       pPolygon1->Add(SCMPoint2D(tempx, tempy));
	tempx = Sg+Bc;        tempy = tc;        pPolygon1->Add(SCMPoint2D(tempx, tempy));
	tempx = Sg;           tempy = tc;        pPolygon1->Add(SCMPoint2D(tempx, tempy));
	if(bTapJ) aGeomPart2[1].Add(pPolygon1);
	else      aGeomPart2[0].Add(pPolygon1);

	// Top Flange
	tempx = Top;       tempy = tc+Hh;     pPolygon2->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1+B2; tempy = tc+Hh;     pPolygon2->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1+B2; tempy = tc+Hh+t1;  pPolygon2->Add(SCMPoint2D(tempx, tempy));
	tempx = Top;       tempy = tc+Hh+t1;  pPolygon2->Add(SCMPoint2D(tempx, tempy));
	if(bTapJ) aGeomPart1[1].Add(pPolygon2);
	else      aGeomPart1[0].Add(pPolygon2);

	// Bottom Flange
	tempx = Bot;       tempy = tc+Hh+t1+H;    pPolygon3->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B3+B4; tempy = tc+Hh+t1+H;    pPolygon3->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B3+B4; tempy = tc+Hh+t1+H+t2; pPolygon3->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot;       tempy = tc+Hh+t1+H+t2; pPolygon3->Add(SCMPoint2D(tempx, tempy));
	if(bTapJ) aGeomPart1[1].Add(pPolygon3);
	else      aGeomPart1[0].Add(pPolygon3);

	// Web
	// 1. Web 의 높이가 한변을 이루고 있는 직각 삼각형의 한변을 구한다. (Top + B1) - (Bot + B3) 
	double twp = 0.0;
	double dSide = fabs((Top + B1) - (Bot + B3));

	// 2. 비례식을 이용해서 변환된 Web 의 길이를 구한다.
	if(H > 0.0)
	{
		twp = tw/H * sqrt(H*H + dSide*dSide);
	}

	tempx = Top+B1-(twp/2); tempy = tc+Hh+t1;   pPolygon4->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1+(twp/2); tempy = tc+Hh+t1;   pPolygon4->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B3+(twp/2); tempy = tc+Hh+t1+H; pPolygon4->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B3-(twp/2); tempy = tc+Hh+t1+H; pPolygon4->Add(SCMPoint2D(tempx, tempy));
	if(bTapJ) aGeomPart1[1].Add(pPolygon4);
	else      aGeomPart1[0].Add(pPolygon4);

	T_SECT_SOD_STIFFNER_I Size; Size.Initialize();
	Size.Top = Top;
	Size.Bot = Bot;
	Size.B1 = B1;
	Size.B2 = B2;
	Size.B3 = B3;
	Size.B4 = B4;
	Size.H = H;
	Size.t1 = t1;
	Size.t2 = t2;
	Size.twp = twp;
	MakePictureSODStiffner_I(aGeom, Size, tc+Hh);

	aGeom.Add(pPolygon1);
	aGeom.Add(pPolygon2);
	aGeom.Add(pPolygon3);
	aGeom.Add(pPolygon4);
}

void ICMGeomMaker::MakePictureCompoStlGirderTub(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ)
{
	CCMPolygon *pPolygon1 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon2 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon3 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon4 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon5 = new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pPolygon6 = new CCMPolygon(CCM_BORDER, CCM_FILL);

	double SW=0.0, CTC=0.0, Bc=0.0, tc=0.0, Hh=0.0;
	double Sg=0.0, Top=0.0, Bot=0.0;
	double B1=0.0, B2=0.0, B3=0.0, B4=0.0, B5=0.0, B6=0.0;
	double H=0.0, t1=0.0, t2=0.0, tw1=0.0, tw2=0.0, bf1=0.0, bf2=0.0;
	int    GNum=0;

	// read ///////////////////////////////////////
	SW   = m_pDataSrc->SectAfter.SectI.Size[0];
	CTC  = m_pDataSrc->SectAfter.SectI.Size[1];
	GNum = m_pDataSrc->SectAfter.SectI.BuiltUpFlag;

	Bc  = m_pDataSrc->SectAfter.SectJ.Size[0];
	tc  = m_pDataSrc->SectAfter.SectJ.Size[1];
	Hh  = m_pDataSrc->SectAfter.SectJ.Size[2];

	BOOL bTapJ = (m_pDataSrc->nStype==D_SECT_TYPE_TAPERED && bJ);
	if(bTapJ)
	{
		Sg  = m_pDataSrc->SectAfter.SectJ.Size[3];
		Top = m_pDataSrc->SectAfter.SectJ.Size[4];
		Bot = m_pDataSrc->SectAfter.SectJ.Size[5];

		B1  = m_pDataSrc->CmpTapJ.Size[0];
		B2  = m_pDataSrc->CmpTapJ.Size[1];
		B3  = m_pDataSrc->CmpTapJ.Size[2];
		B4  = m_pDataSrc->CmpTapJ.Size[3];
		B5  = m_pDataSrc->CmpTapJ.Size[4];
		B6  = m_pDataSrc->CmpTapJ.Size[5];
		H   = m_pDataSrc->CmpTapJ.Size[6];
		t1  = m_pDataSrc->CmpTapJ.Size[7];
		t2  = m_pDataSrc->CmpTapJ.Size[8];
		tw1 = m_pDataSrc->CmpTapJ.Size[9];
		tw2 = m_pDataSrc->CmpTapJ.Size[10];
		bf1 = m_pDataSrc->CmpTapJ.Size[11];
		bf2 = m_pDataSrc->CmpTapJ.Size[12];
	}
	else
	{
		Sg  = m_pDataSrc->SectAfter.SectI.Size[3];
		Top = m_pDataSrc->SectAfter.SectI.Size[4];
		Bot = m_pDataSrc->SectAfter.SectI.Size[5];

		B1  = m_pDataSrc->SectBefore.SectI.Size[0];
		B2  = m_pDataSrc->SectBefore.SectI.Size[1];
		B3  = m_pDataSrc->SectBefore.SectI.Size[2];
		B4  = m_pDataSrc->SectBefore.SectI.Size[3];
		B5  = m_pDataSrc->SectBefore.SectI.Size[4];
		B6  = m_pDataSrc->SectBefore.SectI.Size[5];
		H   = m_pDataSrc->SectBefore.SectI.Size[6];
		t1  = m_pDataSrc->SectBefore.SectI.Size[7];
		t2  = m_pDataSrc->SectBefore.SectI.Size[8];
		tw1 = m_pDataSrc->SectBefore.SectI.Size[9];
		tw2 = m_pDataSrc->SectBefore.SectI.Size[10];
		bf1 = m_pDataSrc->SectBefore.SectI.Size[11];
		bf2 = m_pDataSrc->SectBefore.SectI.Size[12];
	}

	double tempx = 0.0;
	double tempy = 0.0;
	double twp_L = 0.0;
	double twp_R = 0.0;
	
	// Slab 
	tempx = Sg;           tempy = 0.0;       pPolygon1->Add(SCMPoint2D(tempx, tempy));
	tempx = Sg+Bc;        tempy = 0.0;       pPolygon1->Add(SCMPoint2D(tempx, tempy));
	tempx = Sg+Bc;        tempy = tc;        pPolygon1->Add(SCMPoint2D(tempx, tempy));
	tempx = Sg;           tempy = tc;        pPolygon1->Add(SCMPoint2D(tempx, tempy));
	if(bTapJ) aGeomPart2[1].Add(pPolygon1);
	else      aGeomPart2[0].Add(pPolygon1);

	// Top Flange - Left
	tempx = Top;          tempy = tc+Hh;     pPolygon2->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1;       tempy = tc+Hh;     pPolygon2->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1;       tempy = tc+Hh+t1;  pPolygon2->Add(SCMPoint2D(tempx, tempy));
	tempx = Top;          tempy = tc+Hh+t1;  pPolygon2->Add(SCMPoint2D(tempx, tempy));
	if(bTapJ) aGeomPart1[1].Add(pPolygon2);
	else      aGeomPart1[0].Add(pPolygon2);

	// Top Flange - Right
	tempx = Top+B1+B2;    tempy = tc+Hh;     pPolygon3->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1+B2+B3; tempy = tc+Hh;     pPolygon3->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1+B2+B3; tempy = tc+Hh+t1;  pPolygon3->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1+B2;    tempy = tc+Hh+t1;  pPolygon3->Add(SCMPoint2D(tempx, tempy));
	if(bTapJ) aGeomPart1[1].Add(pPolygon3);
	else      aGeomPart1[0].Add(pPolygon3);

	// Bottom Flange
	tempx = Bot;          tempy = tc+Hh+t1+H;    pPolygon4->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B4+B5+B6; tempy = tc+Hh+t1+H;    pPolygon4->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B4+B5+B6; tempy = tc+Hh+t1+H+t2; pPolygon4->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot;          tempy = tc+Hh+t1+H+t2; pPolygon4->Add(SCMPoint2D(tempx, tempy));
	if(bTapJ) aGeomPart1[1].Add(pPolygon4);
	else      aGeomPart1[0].Add(pPolygon4);

	// Left Web
	// 1. Web 의 높이가 한변을 이루고 있는 직각 삼각형의 한변을 구한다. (Top + B1) - (Bot + B4) 
	double dSide_L = fabs((Top + bf1) - (Bot + B4));

	// 2. 비례식을 이용해서 변환된 Web 의 길이를 구한다.
	if(H > 0.0)
	{
		twp_L = tw1/H * sqrt(H*H + dSide_L*dSide_L);
	}

	tempx = Top+bf1-twp_L; tempy = tc+Hh+t1;   pPolygon5->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+bf1;       tempy = tc+Hh+t1;   pPolygon5->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B4;        tempy = tc+Hh+t1+H; pPolygon5->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B4-twp_L;  tempy = tc+Hh+t1+H; pPolygon5->Add(SCMPoint2D(tempx, tempy));
	if(bTapJ) aGeomPart1[1].Add(pPolygon5);
	else      aGeomPart1[0].Add(pPolygon5);

	// Right Web
	// 1. Web 의 높이가 한변을 이루고 있는 직각 삼각형의 한변을 구한다. (Top + B1 + B2) - (Bot + B4 + B5) 
	double dSide_R = fabs((Top + B1 + B2 + B3 - bf2) - (Bot + B4 + B5));

	// 2. 비례식을 이용해서 변환된 Web 의 길이를 구한다.
	if(H > 0.0)
	{
		twp_R = tw2/H * sqrt(H*H + dSide_R*dSide_R);
	}

	tempx = Top+B1+B2+B3-bf2;       tempy = tc+Hh+t1;   pPolygon6->Add(SCMPoint2D(tempx, tempy));
	tempx = Top+B1+B2+B3-bf2+twp_R; tempy = tc+Hh+t1;   pPolygon6->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B4+B5+twp_R;        tempy = tc+Hh+t1+H; pPolygon6->Add(SCMPoint2D(tempx, tempy));
	tempx = Bot+B4+B5;              tempy = tc+Hh+t1+H; pPolygon6->Add(SCMPoint2D(tempx, tempy));
	if(bTapJ) aGeomPart1[1].Add(pPolygon6);
	else      aGeomPart1[0].Add(pPolygon6);

	T_SECT_SOD_STIFFNER_TUB Size; Size.Initialize();
	Size.Top = Top;
	Size.Bot = Bot;
	Size.B1 = B1;
	Size.B2 = B2;
	Size.B3 = B3;
	Size.B4 = B4;
	Size.B5 = B5;
	Size.B6 = B6;
	Size.H = H;
	Size.t1 = t1;
	Size.t2 = t2;
	Size.twp_L = twp_L;
	Size.twp_R = twp_R;
	Size.bf1 = bf1;
	Size.bf2 = bf2;
	MakePictureSODStiffner_Tub(aGeom, Size, tc+Hh);
	
	aGeom.Add(pPolygon1);
	aGeom.Add(pPolygon2);
	aGeom.Add(pPolygon3);
	aGeom.Add(pPolygon4);
	aGeom.Add(pPolygon5);
	aGeom.Add(pPolygon6);
}

void ICMGeomMaker::SetDataAndClearAndDeleteArray(CArray<CArray<T_2COOR, T_2COOR&>*, CArray<T_2COOR, T_2COOR&>*>& array, int nColor, CCMGeom2DArray& aGeom)
{
	for (int i = 0; i < array.GetSize(); i++)
	{
		CCMPolygon* pCPolyTmp = new CCMPolygon(CCM_BORDER, nColor);
		int nCNum = array[i]->GetSize();

		for (int j = 0; j < nCNum; j++)
		{
			pCPolyTmp->Add(SCMPoint2D(array[i]->GetAt(j).x, -array[i]->GetAt(j).y));
		}
		if (pCPolyTmp->m_aVertex.GetSize())
			aGeom.Add(pCPolyTmp);
		else
			delete pCPolyTmp;
	}

	for (int i = 0; i < array.GetSize(); i++)
	{
		if (array[i])
		{
			array[i]->RemoveAll();
			delete array[i];
			array[i] = NULL;
		}
	}
	array.RemoveAll();
}

void ICMGeomMaker::MakePictureSrcDumbbell(T_SECT_D& SectData, CCMGeom2DArray& aGeom)
{

	if (SectData.SectBefore.Shape != D_SECT_SHAPE_SRC_DUMB) return;

	CArray<CArray<T_2COOR, T_2COOR&>*, CArray<T_2COOR, T_2COOR&>*> arOuterP1List;
	CArray<CArray<T_2COOR, T_2COOR&>*, CArray<T_2COOR, T_2COOR&>*> arOuterP2List;
	CArray<CArray<T_2COOR, T_2COOR&>*, CArray<T_2COOR, T_2COOR&>*> arOuterPwList;
	CArray<CArray<T_2COOR, T_2COOR&>*, CArray<T_2COOR, T_2COOR&>*> arInerP1List;
	CArray<CArray<T_2COOR, T_2COOR&>*, CArray<T_2COOR, T_2COOR&>*> arInerP2List;
	CArray<CArray<T_2COOR, T_2COOR&>*, CArray<T_2COOR, T_2COOR&>*> arInerPwList;

	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetSrcDumbbellPolygonData(SectData, 0);

	int nOutPoly = SectData.SectBefore.SectI.aOuterPolygon.GetSize();

	for (int nOuter = 0; nOuter < nOutPoly; ++nOuter)
	{
		//T_2COOR Coord;
		SCMPoint2D Point1, Point2;
		CArray<T_2COOR, T_2COOR&>* pPoly = new CArray<T_2COOR, T_2COOR&>;
		CArray<T_GSEC_VERTEX, T_GSEC_VERTEX&>* paVertex = &SectData.SectBefore.SectI.aOuterPolygon[nOuter].aVertex;
		for (int i = 0; i < paVertex->GetSize(); i++)
		{
			T_2COOR Coord;
			Coord.x = (*paVertex)[i].dX;
			Coord.y = (*paVertex)[i].dY;
			//OutLines.Add(Coord);
			pPoly->Add(Coord);
		}

		if     (nOuter == 0) arOuterP1List.Add(pPoly);
		else if(nOuter == 1) arOuterP2List.Add(pPoly);
		else if(nOuter == 2) arOuterPwList.Add(pPoly);
		else ASSERT(0);
	}

	int nInPoly = SectData.SectBefore.SectI.aInnerPolygon.GetSize();
	for (int i = 0; i < nInPoly; i++)
	{
		CArray<T_2COOR, T_2COOR&>* pPoly = new CArray<T_2COOR, T_2COOR&>;
		CArray<T_GSEC_VERTEX, T_GSEC_VERTEX&>* paVertex = &SectData.SectBefore.SectI.aInnerPolygon[i].aVertex;
		for (int j = 0; j < paVertex->GetSize(); j++)
		{
			T_2COOR Coord;
			Coord.x = (*paVertex)[j].dX;
			Coord.y = (*paVertex)[j].dY;
			pPoly->Add(Coord);
		}
		if      (i == 0) arInerP1List.Add(pPoly);
		else if (i == 1) arInerP2List.Add(pPoly);
		else if (i == 2) arInerPwList.Add(pPoly);
		else ASSERT(0);
	}

	SetDataAndClearAndDeleteArray(arOuterP1List, CCM_FILL, aGeom);  // Steel
	SetDataAndClearAndDeleteArray(arOuterP2List, CCM_FILL, aGeom);  // Steel
	SetDataAndClearAndDeleteArray(arOuterPwList, CCM_FILL, aGeom);  // Steel

	int nColor = (SectData.bInfusionChk) ? CCM_SRC_FILL : CCM_HOLE;
	SetDataAndClearAndDeleteArray(arInerP1List, CCM_SRC_FILL, aGeom);  // Src Conc
	SetDataAndClearAndDeleteArray(arInerP2List, CCM_SRC_FILL, aGeom);  // Src Conc
	SetDataAndClearAndDeleteArray(arInerPwList,       nColor, aGeom);  // HOLE
}

void ICMGeomMaker::MakePicturePSC(T_SECT_D& SectData,CCMGeom2DArray &aGeom)
{
	CArray<T_2COOR,T_2COOR&>OutLines;
	CArray<T_2COOR,T_2COOR&>Cell1;
	CArray<T_2COOR,T_2COOR&>Cell2;
	CArray<T_2COOR,T_2COOR&>Cell3;
	CArray<T_2COOR,T_2COOR&>Cell4;
	CArray<CCMLine*,CCMLine*>Lines;

	int nNumCell=0;
	int nNumLines=0;

	if(SectData.SectBefore.Shape == D_SECT_SHAPE_PSC_1CELL)
		nNumCell = 1;
	else if(SectData.SectBefore.Shape  == D_SECT_SHAPE_PSC_2CELL)
		nNumCell = 2;
	else if(SectData.SectBefore.Shape  == D_SECT_SHAPE_PSC_3CELL)
		nNumCell = 3;
	else if(SectData.SectBefore.Shape  == D_SECT_SHAPE_PSC_4CELL)
		nNumCell = 4;
	else if(SectData.SectBefore.Shape  == D_SECT_SHAPE_PSC_MID)
		nNumCell = 1;
	else if(SectData.SectBefore.Shape  == D_SECT_SHAPE_PSC_HALF)
		nNumCell = 1;
	else if(SectData.SectBefore.Shape  == D_SECT_SHAPE_PSC_T)
		nNumCell = 1;
	else if(SectData.SectBefore.Shape  == D_SECT_SHAPE_PSC_BOX)
	{
		if (SectData.SectBefore.nCellType == 0) nNumCell = 1;
		else if (SectData.SectBefore.nCellType == 1) nNumCell = 1;
		else if (SectData.SectBefore.nCellType == 2) nNumCell = 2;
	}
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_PSC_I)
		nNumCell = 0; //안쓰임 
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_PSC_NCEL2)
		nNumCell = 1; //안쓰임 
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_PSC_VALUE)
		nNumCell = 1; //안쓰임
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_ROCT) //R Octagon을 PSC같이 만든다 
		nNumCell = 0;
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_BSTF) //Box with Stiffener을 PSC같이 만든다 
		nNumCell = 0;
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_PSTF) //Pipe with Stiffener를 PSC같이 만든다 
		nNumCell = 0;
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_SRC_BSTF)
		nNumCell = 0;
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_SRC_PSTF)
		nNumCell = 0;
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_SRC_STUB)
		nNumCell = 0;
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_SRC_DUMB)
		nNumCell = 0;
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB)
		nNumCell = 1;
	else return;

	CArray<CArray<T_2COOR,T_2COOR&>*,CArray<T_2COOR,T_2COOR&>*> arOuterList;
	CArray<CArray<T_2COOR,T_2COOR&>*,CArray<T_2COOR,T_2COOR&>*> arInerCellList;

	if (SectData.SectBefore.Shape == D_SECT_SHAPE_PSC_VALUE)
	{
		// 우선 여기서 만들어 그림만 보여주고 차후 makePSCShape에서 통합한다.

		int nOutPoly = SectData.SectBefore.SectI.aOuterPolygon.GetSize();
		//if (nOutPoly != 1) return;
		//ASSERT(nOutPoly == 1);
		//T_2COOR Coord;
		// 20080624 mylee - Composite General 에서 한 파트에 Outer 2 개 있는 경우 고려
		for(int nOuter=0 ; nOuter < nOutPoly ; ++nOuter)
		{
			//T_2COOR Coord;
			SCMPoint2D Point1,Point2;
			CCMLine* ccmline;
			CArray<T_2COOR,T_2COOR&>* pPoly = new CArray<T_2COOR,T_2COOR&>;
			CArray<T_GSEC_VERTEX, T_GSEC_VERTEX&>* paVertex = &SectData.SectBefore.SectI.aOuterPolygon[nOuter/*0*/].aVertex;
			for (int i = 0; i < paVertex->GetSize(); i++)
			{
				T_2COOR Coord;
				Coord.x = (*paVertex)[i].dX;
				Coord.y = (*paVertex)[i].dY;
				OutLines.Add(Coord);
				pPoly->Add(Coord);
			}
			arOuterList.Add(pPoly);


			nNumLines=SectData.SectBefore.SectI.aLine.GetSize();
			
			if(nNumLines>0)
			{
				for(int j=0;j<nNumLines;j++)
				{
					Point1.x=OutLines[SectData.SectBefore.SectI.aLine[j].v1].x;
					Point1.y=-OutLines[SectData.SectBefore.SectI.aLine[j].v1].y;  // y좌표는 반대로
					Point2.x=OutLines[SectData.SectBefore.SectI.aLine[j].v2].x;
					Point2.y=-OutLines[SectData.SectBefore.SectI.aLine[j].v2].y;  // y좌표는 반대로
					ccmline=new CCMLine(CCM_BORDER, CCM_FILL);
					ccmline->Set(Point1,Point2);
					Lines.Add(ccmline);
				}
				OutLines.RemoveAll();
				arOuterList.RemoveAll();
			}
			else
			{
				OutLines.RemoveAll();
			}
		}

		int nInPoly = SectData.SectBefore.SectI.aInnerPolygon.GetSize();
		for (int i = 0; i < nInPoly; i++)
		{
			CArray<T_2COOR,T_2COOR&>* pPoly = new CArray<T_2COOR,T_2COOR&>;
			CArray<T_GSEC_VERTEX, T_GSEC_VERTEX&>* paVertex = &SectData.SectBefore.SectI.aInnerPolygon[i].aVertex;
			for (int j = 0; j < paVertex->GetSize(); j++)
			{
				T_2COOR Coord;
				Coord.x = (*paVertex)[j].dX;
				Coord.y = (*paVertex)[j].dY;
				pPoly->Add(Coord);
			}
			arInerCellList.Add(pPoly);
		}
		
	}
	else
	{
		CDBDoc::GetDocPoint()->makePSCShape(SectData.SectBefore.SectI.Size,
																				SectData.SectBefore,
																				nNumCell,
																				OutLines,Cell1,Cell2,Cell3,Cell4,
																				arInerCellList);
	}
	
	COLORREF nColor;
	if (SectData.SectBefore.Shape == D_SECT_SHAPE_SRC_BSTF ||
			SectData.SectBefore.Shape == D_SECT_SHAPE_SRC_PSTF  )
			nColor = CCM_SRC_FILL;
	else nColor = CCM_HOLE;

	CCMPolygon *pOPoly  =  new CCMPolygon(CCM_BORDER, CCM_FILL);
	CCMPolygon *pC1Poly =  new CCMPolygon(CCM_BORDER, nColor);
	CCMPolygon *pC2Poly =  new CCMPolygon(CCM_BORDER, CCM_HOLE);
	CCMPolygon *pC3Poly =  new CCMPolygon(CCM_BORDER, CCM_HOLE);
	CCMPolygon *pC4Poly =  new CCMPolygon(CCM_BORDER, CCM_HOLE);
		
	int nO  = OutLines.GetSize();
	int nC1 = Cell1.GetSize();
	int nC2 = Cell2.GetSize();
	int nC3 = Cell3.GetSize();
	int nC4 = Cell4.GetSize();
	for( int i = 0; i < nO  ; i++)
	{
		pOPoly->Add(SCMPoint2D(OutLines[i].x,-OutLines[i].y));
	}

	for(int  i = 0; i < nC1 ; i++)
	{
		pC1Poly->Add(SCMPoint2D(Cell1[i].x,-Cell1[i].y));
	}

	for(int i = 0; i < nC2 ; i++)
	{
		pC2Poly->Add(SCMPoint2D(Cell2[i].x,-Cell2[i].y));
	}

	for(int  i = 0; i < nC3 ; i++)
	{
		pC3Poly->Add(SCMPoint2D(Cell3[i].x,-Cell3[i].y));
	}

	for(int i = 0; i < nC4 ; i++)
	{
		pC4Poly->Add(SCMPoint2D(Cell4[i].x,-Cell4[i].y));
	}

	if(pOPoly->m_aVertex.GetSize())
		aGeom.Add(pOPoly);
	else
		delete pOPoly;
	if(pC1Poly->m_aVertex.GetSize())
		aGeom.Add(pC1Poly);
	else
		delete pC1Poly;
	
	if(pC2Poly->m_aVertex.GetSize())
		aGeom.Add(pC2Poly);
	else
		delete pC2Poly;

	if(pC3Poly->m_aVertex.GetSize())
		aGeom.Add(pC3Poly);
	else
		delete pC3Poly;


	if(pC4Poly->m_aVertex.GetSize())
		aGeom.Add(pC4Poly);
	else
		delete pC4Poly;

	for(int k=0;k<Lines.GetSize();k++)
	{
		aGeom.Add(Lines[k]);
	}

	for(int i=0; i<arOuterList.GetSize(); i++)
	{ 
		CCMPolygon *pCPolyTmp = new CCMPolygon(CCM_BORDER, CCM_FILL);
		int nCNum = arOuterList[i]->GetSize(); 

		for(int j = 0; j < nCNum ; j++)
		{
			pCPolyTmp->Add(SCMPoint2D(arOuterList[i]->GetAt(j).x,-arOuterList[i]->GetAt(j).y));
		}
		if(pCPolyTmp->m_aVertex.GetSize())
			aGeom.Add(pCPolyTmp);
		else
			delete pCPolyTmp;
	}

	for(int i=0; i<arOuterList.GetSize(); i++)
	{
		if(arOuterList[i])
		{
			arOuterList[i]->RemoveAll();
			delete arOuterList[i];
			arOuterList[i] = NULL; 
		}
	}
	arOuterList.RemoveAll(); 

	for(int i=0; i<arInerCellList.GetSize(); i++)
	{ 
		CCMPolygon *pCPolyTmp =  new CCMPolygon(CCM_BORDER, CCM_HOLE); 
		int nCNum = arInerCellList[i]->GetSize(); 
		
		for(int j = 0; j < nCNum ; j++)
		{
			pCPolyTmp->Add(SCMPoint2D(arInerCellList[i]->GetAt(j).x,-arInerCellList[i]->GetAt(j).y));
		}
		if(pCPolyTmp->m_aVertex.GetSize())
			aGeom.Add(pCPolyTmp);
		else
			delete pCPolyTmp;
	}

	for(int i=0; i<arInerCellList.GetSize(); i++)
	{
		if(arInerCellList[i])
		{
			arInerCellList[i]->RemoveAll();
			delete arInerCellList[i];
			arInerCellList[i] = NULL; 
		}
	}
	arInerCellList.RemoveAll(); 
}

/////////////////////////////////////////////////////////////////////////////
// CCMSecViewWnd

CCMSecViewWnd::CCMSecViewWnd()
{
	m_pDataSrc = 0;
	m_bAfter = FALSE;
	m_bReverse = FALSE;
	m_bCentroid = FALSE;
	m_bShearCheck = FALSE;
	m_bChangeAxis = FALSE;

	m_pDataSrcRebar = 0;
	m_bEndJ = FALSE;

	m_bComGenJ = FALSE;
}

CCMSecViewWnd::~CCMSecViewWnd()
{
}

/////////////////////////////////////////////////////////////////////////////
// CCMSecViewWnd public interface functions
BOOL CCMSecViewWnd::Init(CWnd* pParentWnd)
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
	dwStyle = GetWindowLong(m_hWnd,GWL_STYLE);
	dwStyle |=  WS_CLIPCHILDREN;
	SetWindowLong(m_hWnd,GWL_STYLE,dwStyle);

	CRect r;
	GetClientRect(r);
	m_Painter.SetDrawingArea(r);

	return TRUE;
}

void CCMSecViewWnd::SetDataSource(T_SECT_D* pDataSrc, BOOL bAfter, BOOL bChangeAxis)
{
	m_pDataSrc = pDataSrc;
	m_bAfter = bAfter;
	m_bChangeAxis = bChangeAxis;
}

void CCMSecViewWnd::SetDataSourceRebar(T_RPSC_D* pDataSrc, BOOL bEndJ)
{
	m_pDataSrcRebar = pDataSrc;
	m_bEndJ = bEndJ;
}

/////////////////////////////////////////////////////////////////////////////
// CCMSecViewWnd implementation functions

BEGIN_MESSAGE_MAP(CCMSecViewWnd, CWnd)
	//{{AFX_MSG_MAP(CCMSecViewWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSecViewWnd message handlers
BOOL CCMSecViewWnd::OnEraseBkgnd(CDC* pDC)
{
	m_Painter.EraseBkgnd(pDC);
	return CWnd::OnEraseBkgnd(pDC);
}

void CCMSecViewWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (m_pDataSrc != 0) 
	{

		CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
		pDoc->SetErrorMsg(FALSE);
		
		if (m_pDataSrc->nStype == 0) return;

		// centroid
		BOOL bCentroid;
		if (m_pDataSrc->nStype == D_SECT_TYPE_PSC || 
				m_pDataSrc->nStype == D_SECT_TYPE_COMPO_CI || 
				m_pDataSrc->nStype == D_SECT_TYPE_COMPO_CT ||
				m_pDataSrc->nStype == D_SECT_TYPE_COMPO_PC) bCentroid = m_bCentroid;
		else if (m_pDataSrc->nStype == D_SECT_TYPE_TAPERED)
		{
			if(m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_COMPO_G)
			{
				static T_SECT_D data;
				data = *m_pDataSrc;
				bCentroid = CSectUtil::CalcSectData(data, FALSE, FALSE, FALSE, FALSE, TRUE);
			}
			else if(m_pDataSrc->SectBefore.nStype == D_SECT_TYPE_USER)
			{
				static T_SECT_D data;
				data.Initialize();
				data.nStype = D_SECT_TYPE_REGULAR;
				data.SectBefore.bConsiderWarpingEffect = m_pDataSrc->SectBefore.bConsiderWarpingEffect;
				data.SectBefore.Shape = m_pDataSrc->SectBefore.Shape; 
				data.SectBefore.SectI = m_pDataSrc->SectBefore.SectI;
				bCentroid = CSectUtil::CalcSectData(data, TRUE, FALSE, FALSE, FALSE, TRUE);
			}
			else
			{
				int nShapeIndex = CSectUtil::GetShapeIndexFromNamePsc(m_pDataSrc->SectBefore.Shape);
				if (nShapeIndex != -1) bCentroid = m_bCentroid;
				else bCentroid = CSectUtil::CalcSectData(*m_pDataSrc, FALSE, FALSE, FALSE, FALSE, TRUE);
			}
		}
		else if (m_pDataSrc->nStype == D_SECT_TYPE_USER)
		{
			T_SECT_D data;
			data.Initialize();
			data.nStype = D_SECT_TYPE_REGULAR;
			data.SectBefore.bConsiderWarpingEffect = m_pDataSrc->SectBefore.bConsiderWarpingEffect;
			data.SectBefore.Shape = m_pDataSrc->SectBefore.Shape; 
			data.SectBefore.SectI = m_pDataSrc->SectBefore.SectI;
			bCentroid = CSectUtil::CalcSectData(data, TRUE, FALSE, FALSE, FALSE, TRUE);
		}
		else if (m_pDataSrc->nStype != 0) bCentroid = CSectUtil::CalcSectData(*m_pDataSrc, FALSE, FALSE, FALSE, FALSE, TRUE);

		// offset  (도심인 경우 Offset 표시 않음)
		// real offset : Centroid 또는 User Defined Offset인 경우 
		// Size의 Center, Left, Right, Top, Bottom의 위치와 틀림.
		// 도심에서의 상대 거리(Top,RIght:+, Bottom,Left:-)를 계산해서 
		// 도심위치 이용해서 출력한다.
		BOOL bOffset = TRUE;
		int nOffsetPoint = m_pDataSrc->SectBefore.nOffsetPoint;
		if (nOffsetPoint < 1 || nOffsetPoint > 9) bOffset = FALSE;    // error
		else if (nOffsetPoint == 5)
		{
			if (m_pDataSrc->SectBefore.nOffsetCenter == 0) bOffset = FALSE;  // 도심과 일치하는 경우
		}
		else bOffset = bCentroid;   // 그외 

		// 전단 검토위치 출력
		BOOL bShearCheck = FALSE;
		if (m_bShearCheck && m_pDataSrc->SectBefore.bShearCheck == TRUE)
		{
			bShearCheck = TRUE;
		} 

		// make shape & draw
		int nRebarNum;
		CCMGeom2DArray aGeom;
		m_GeomMaker.MakePicture(m_pDataSrc, aGeom, m_bAfter, m_bReverse, bCentroid, bOffset, bShearCheck, m_pDataSrcRebar, m_bEndJ, &nRebarNum, m_bComGenJ);
		m_Painter.DrawSection(&dc, aGeom, bCentroid, bOffset, bShearCheck, nRebarNum, m_bChangeAxis);

		pDoc->SetErrorMsg(TRUE);
	}
	// Do not call CWnd::OnPaint() for painting messages
}

void CCMSecViewWnd::GetPictureParam(BOOL& bCentroid,BOOL &bOffset,BOOL &bShearCheck)
{
	if (m_pDataSrc != 0) 
	{
		CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
		pDoc->SetErrorMsg(FALSE);
		
		if (m_pDataSrc->nStype == 0) return;

		// centroid
		bCentroid = FALSE;
		if (m_pDataSrc->nStype == D_SECT_TYPE_PSC || 
				m_pDataSrc->nStype == D_SECT_TYPE_COMPO_CI || 
				m_pDataSrc->nStype == D_SECT_TYPE_COMPO_CT) bCentroid = m_bCentroid;
		else if (m_pDataSrc->nStype == D_SECT_TYPE_TAPERED)
		{
			int nShapeIndex = CSectUtil::GetShapeIndexFromNamePsc(m_pDataSrc->SectBefore.Shape);
			if (nShapeIndex != -1) bCentroid = m_bCentroid;
			else bCentroid = CSectUtil::CalcSectData(*m_pDataSrc, FALSE, FALSE, FALSE, FALSE, TRUE);
		}
		else if (m_pDataSrc->nStype == D_SECT_TYPE_USER)
		{
			T_SECT_D data;
			data.Initialize();
			data.nStype = D_SECT_TYPE_REGULAR;
			data.SectBefore.bConsiderWarpingEffect = m_pDataSrc->SectBefore.bConsiderWarpingEffect;
			data.SectBefore.Shape = m_pDataSrc->SectBefore.Shape; 
			data.SectBefore.SectI = m_pDataSrc->SectBefore.SectI;
			bCentroid = CSectUtil::CalcSectData(data, TRUE, FALSE, FALSE, FALSE, TRUE);
		}
		else if (m_pDataSrc->nStype != 0) bCentroid = CSectUtil::CalcSectData(*m_pDataSrc, FALSE, FALSE, FALSE, FALSE, TRUE);

		// offset  (도심인 경우 Offset 표시 않음)
		// real offset : Centroid 또는 User Defined Offset인 경우 
		// Size의 Center, Left, Right, Top, Bottom의 위치와 틀림.
		// 도심에서의 상대 거리(Top,RIght:+, Bottom,Left:-)를 계산해서 
		// 도심위치 이용해서 출력한다.
		bOffset = TRUE;
		int nOffsetPoint = m_pDataSrc->SectBefore.nOffsetPoint;
		if (nOffsetPoint < 1 || nOffsetPoint > 9) bOffset = FALSE;    // error
		else if (nOffsetPoint == 5)
		{
			if (m_pDataSrc->SectBefore.nOffsetCenter == 0) bOffset = FALSE;  // 도심과 일치하는 경우
		}
		else bOffset = bCentroid;   // 그외 

		// 전단 검토위치 출력
		bShearCheck = FALSE;
		if (m_bShearCheck && m_pDataSrc->SectBefore.bShearCheck == TRUE)
		{
			bShearCheck = TRUE;
		} 

		pDoc->SetErrorMsg(TRUE);
	}
}

void CCMSecViewWnd::ApplyPictureParam(BOOL bCentroid,BOOL bOffset,BOOL bShearCheck,CCMGeom2DArray& aGeom)
{
	// offset point용 geometry는 뺀다.
	CCMGeom2D *pOffset, *pCentroid, *pStress[4], *pShear[3];
	CArray<CCMGeom2D*, CCMGeom2D*> apRebar;
	BOOL bExist = aGeom.GetSize() > 0;
	SCMRect2D mbr;
	if (bExist) 
	{
		if (bOffset)
		{
			pOffset = aGeom.GetAt(aGeom.GetSize()-1);
			aGeom.RemoveAt(aGeom.GetSize()-1);
		}
		if (bCentroid)
		{
			for (int i=0; i<4; i++)
			{
				pStress[i] = aGeom.GetAt(aGeom.GetSize()-1);
				aGeom.RemoveAt(aGeom.GetSize()-1);
			}
		}
		if (bCentroid)
		{
			pCentroid = aGeom.GetAt(aGeom.GetSize()-1);
			aGeom.RemoveAt(aGeom.GetSize()-1);
		}
		if (bShearCheck)
		{
			for (int i = 0; i < 3; i++)
			{
				pShear[i] = aGeom.GetAt(aGeom.GetSize()-1);
				aGeom.RemoveAt(aGeom.GetSize()-1);
			}
		}
	}
}

BOOL CCMSecViewWnd::MakeEllipse(SCMRect2D rect, CArray<SCMPoint2D,SCMPoint2D> &PosArr)
{
	double Xc, Yc, a, b;
	Xc = (rect.min.x + rect.max.x) / 2.;
	Yc = (rect.min.y + rect.max.y) / 2.;
	a  = rect.Width() / 2.;
	b  = rect.Height() / 2.;

	int i; 
	double x[65],y[65];
	double num,dt,dCos,dSin,adivb,bdiva;

	num = 64.; // number of segment !!!
	dt = 2.* M_PI / num;
	dCos = cos(dt);
	dSin = sin(dt);
	adivb = a / b ;
	bdiva = b / a ; 

	x[0] = a;
	y[0] = 0.;

	for( i = 0 ; i < num ; i++)
	{
		x[i+1] = x[i]*dCos - adivb*y[i]*dSin;
		y[i+1] = bdiva*x[i]*dSin + y[i]*dCos;
	}

	SCMPoint2D TPos;
	TPos.x = Xc + x[0];
	TPos.y = Yc + y[0];
	PosArr.Add(TPos);

	for( i = 0 ; i < num ; i++)
	{
		TPos.x = Xc + x[i+1];
		TPos.y = Yc + y[i+1];
		PosArr.Add(TPos);
	}

	return TRUE;
}

SCMPoint2D CCMSecViewWnd::GetPtOffset(CCMGeom2DArray &aGeom, BOOL bReverse)
{
	SCMPoint2D ptOffset;

	SCMRect2D mbr;
	mbr = aGeom.GetMBR();

	double centroidX, centroidY;
	if (m_pDataSrc->nStype == D_SECT_TYPE_SRC ||
				m_pDataSrc->nStype == D_SECT_TYPE_COMBINED)
	{
		centroidX = mbr.min.x + m_pDataSrc->SectBefore.Design.YBar;
		centroidY = (mbr.min.y + (mbr.Height() - m_pDataSrc->SectBefore.Design.ZBar)) * (bReverse?-1:1);
	}
	else if (m_pDataSrc->nStype == D_SECT_TYPE_COMPO_I ||
					 m_pDataSrc->nStype == D_SECT_TYPE_COMPO_B || 
					 m_pDataSrc->nStype == D_SECT_TYPE_COMPO_TUB || 	
					 m_pDataSrc->nStype == D_SECT_TYPE_COMPO_STLG_B || 	
					 m_pDataSrc->nStype == D_SECT_TYPE_COMPO_STLG_I || 	
					 m_pDataSrc->nStype == D_SECT_TYPE_COMPO_STLG_TUB || 	
					 m_pDataSrc->nStype == D_SECT_TYPE_COMPO_CI || 
					 m_pDataSrc->nStype == D_SECT_TYPE_COMPO_CT || 
					 m_pDataSrc->nStype == D_SECT_TYPE_CONSTRUCTION)
	{
		centroidX = mbr.min.x + m_pDataSrc->SectAfter.Design.YBar;
		centroidY = (mbr.min.y + (mbr.Height() - m_pDataSrc->SectAfter.Design.ZBar)) * (bReverse?-1:1);
	}
	else if(m_pDataSrc->nStype == D_SECT_TYPE_COMPO_G)
	{
		centroidX = mbr.min.x + m_pDataSrc->SectAfter.SectI.Design.YBar;
		centroidY = (mbr.min.y + (mbr.Height() - m_pDataSrc->SectAfter.SectI.Design.ZBar)) * (bReverse?-1:1);
	}
	else
	{
		centroidX = mbr.min.x + m_pDataSrc->SectBefore.SectI.Design.YBar;
		centroidY = (mbr.min.y + (mbr.Height() - m_pDataSrc->SectBefore.SectI.Design.ZBar)) * (bReverse?-1:1);
	}

	int nOffsetPoint = m_pDataSrc->SectBefore.nOffsetPoint;
	int nHorzOption = m_pDataSrc->SectBefore.nHorzOffsetOpt;
	int nVertOption = m_pDataSrc->SectBefore.nVertOffsetOpt;
	if (nOffsetPoint < 1 || nOffsetPoint > 9) nOffsetPoint = 5;
	
	double centX, centY;
	if (m_pDataSrc->SectBefore.nHorzOffsetOpt == 1) centX = centroidX; 
	else 
	{
		if (m_pDataSrc->SectBefore.nOffsetCenter == 0) centX = centroidX;
		else centX = (mbr.min.x+mbr.max.x)/2.0;
	}
	if (m_pDataSrc->SectBefore.nVertOffsetOpt == 1) centY = centroidY;
	else
	{
		if (m_pDataSrc->SectBefore.nOffsetCenter == 0) centY = centroidY;
		else centY = (mbr.min.y+mbr.max.y)/2.0;
	}

	double optionX = m_pDataSrc->SectBefore.dUserDefOffsetY[0];
	double optionY = m_pDataSrc->SectBefore.dUserDefOffsetZ[0]*(bReverse?-1:1);

	switch(nOffsetPoint)
	{
	case 1: case 4: case 7: ptOffset.x = (nHorzOption == 0) ? mbr.min.x : centX - optionX ; break;
	case 2: case 5: case 8: ptOffset.x = centX; break; 
	case 3: case 6: case 9: ptOffset.x = (nHorzOption == 0) ? mbr.max.x : centX + optionX; break;
	}
	switch(nOffsetPoint)
	{
	case 1: case 2: case 3: ptOffset.y = (nVertOption == 0) ? mbr.min.y : centY - optionY; break;
	case 4: case 5: case 6: ptOffset.y = centY; break;
	case 7: case 8: case 9: ptOffset.y = (nVertOption == 0) ? mbr.max.y : centY + optionY; break;
	}

	return ptOffset;
}

BOOL CCMSecViewWnd::ConvertSectionData(T_SECT_D* pSectD, CArray<DlgView_Object*,DlgView_Object*> & arDViewGeoms,
													CArray<double, double&> &arPos)
{
	CCMGeom2DArray aGeom;
	int nRebarNum;
	BOOL bShearCheck, bCentroid, bOffset;

	m_pDataSrc = pSectD;

	GetPictureParam(bCentroid, bOffset,bShearCheck);
	m_GeomMaker.MakePicture(m_pDataSrc, aGeom, m_bAfter, m_bReverse, bCentroid, bOffset, bShearCheck, m_pDataSrcRebar, m_bEndJ, &nRebarNum, m_bComGenJ);

	SCMPoint2D offset = GetPtOffset(aGeom, m_bReverse);
	// Offset
	double offsetX = -offset.x;	// [0]

	// High, Low
	SCMRect2D mbr;
	mbr = aGeom.GetMBR();
	double dis = mbr.Height();

//	double high = -mbr.min.y;
//	double low = -mbr.max.y;
	double high = -(mbr.min.y+dis);		// [1]
	double low = -(mbr.max.y+dis);		// [2]
	double left = -mbr.max.x;					// [3]
	double right = -mbr.min.x;				// [4]
	double offsetY = -(offset.y+dis);	// [5]

	arPos.Add(offsetX);	
	arPos.Add(high);  arPos.Add(low);  
	arPos.Add(left);  arPos.Add(right);
	arPos.Add(offsetY);
	
	ApplyPictureParam(bCentroid, bOffset,bShearCheck,aGeom);
	
	int nGeom = aGeom.GetSize();
	for(int i=0; i < nGeom; i++)
	{
		
		if(ID_Geom::Instance().GTYPE_LINE == aGeom[i]->WhoAmI())
		{
			DlgView_Object *pDVObj;
			DV_3dp Point,P1,P2;
			pDVObj = new DlgView_Object;
			
			CCMLine* pLine = (CCMLine*)aGeom[i];
		//	P1.Set(-pLine->m_pt1.x,-pLine->m_pt1.y,0);
		//	P2.Set(-pLine->m_pt2.x,-pLine->m_pt2.y,0);
			P1.Set(-pLine->m_pt1.x,-(pLine->m_pt1.y+dis),0);
			P2.Set(-pLine->m_pt2.x,-(pLine->m_pt2.y+dis),0);
			pDVObj->GetGeomData().SetLine(P1,P2);
			
			arDViewGeoms.Add(pDVObj);
		}
		else if(ID_Geom::Instance().GTYPE_POLYGON == aGeom[i]->WhoAmI())
		{
			DV_3dp Point3D;
			CArray<DV_3dp, DV_3dp&> arVerts; 
			CCMPolygon* pPoly = (CCMPolygon*)aGeom[i];
			int nVert = pPoly->m_aVertex.GetSize();
			for(int i = 0 ; i < nVert; i++)
			{
			//  Point3D.Set(-pPoly->m_aVertex[i].x,-pPoly->m_aVertex[i].y,0);
				Point3D.Set(-pPoly->m_aVertex[i].x,-(pPoly->m_aVertex[i].y+dis),0);
				arVerts.Add(Point3D);
			}
			
			DlgView_Object *pDVObj;
			pDVObj = new DlgView_Object;
			pDVObj->GetGeomData().SetPolygon(arVerts);

			if(pPoly->BrushColor == CCM_HOLE)
				pDVObj->SetInfo_Render(RGB(255,255,255), pPoly->PenColor, 0.4f);
			else 
			  pDVObj->SetInfo_Render(RGB(200, 200, 200), pPoly->PenColor, 0.4f);
			
			arDViewGeoms.Add(pDVObj);
		}
		else if(ID_Geom::Instance().GTYPE_ELLIPSE == aGeom[i]->WhoAmI())
		{
			CCMEllipse* pEllipse = (CCMEllipse*)aGeom[i];

			CArray<SCMPoint2D, SCMPoint2D> arPts;
		  MakeEllipse(pEllipse->m_Rect, arPts);

		  CArray<DV_3dp, DV_3dp&> arVerts;
			SCMPoint2D Pt2D;
			DV_3dp Point3D;
			int nPts = arPts.GetSize();
			for(i=0; i<nPts; i++)
			{
				Pt2D = arPts.GetAt(i);
			//	Point3D.Set(-Pt2D.x,-Pt2D.y,0);
				Point3D.Set(-Pt2D.x,-(Pt2D.y+dis),0);
				arVerts.Add(Point3D);
			}
			DlgView_Object *pDVObj;		
			pDVObj = new DlgView_Object;
			pDVObj->GetGeomData().SetPolygon(arVerts);
			arDViewGeoms.Add(pDVObj);
		}
	}	// end of loop

	// release connection
	m_pDataSrc = NULL;

	return TRUE;
}

#undef CCM_WHITE
#undef CCM_BLACK
#undef CCM_REDGREEN
#undef CCM_GRAY
#undef CCM_BLUE
#undef CCM_DARKBLUE
#undef CCM_YELLOW
#undef CCM_CYAN
#undef CCM_RED

#undef CCM_BORDER
#undef CCM_FILL
#undef CCM_HOLE
#undef CCM_SRC_FILL

#undef CCM_SECTI_BORDER
#undef CCM_SECTI_FILL
#undef CCM_SECTJ_BORDER
#undef CCM_SECTJ_FILL

