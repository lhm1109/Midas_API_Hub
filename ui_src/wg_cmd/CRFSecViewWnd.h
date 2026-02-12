#pragma once

#include "..\wg_db\wg_db_DBDoc.h"
#include "CMGeometry.h"
#include "CMSecViewWnd.h"

//Reinforce Sect

class DlgView_Object;
class CRFSecViewWnd : public CWnd
{
// Construction
public:
	CRFSecViewWnd();
	virtual ~CRFSecViewWnd();

// Attributes
public:
	T_SECT_D* m_pDataSrc;
	T_RFST_D* m_pReinforceSect;
	//ICMPainter    m_Painter;  // Geometry Painting Interface
	ICMGeomMaker  m_GeomMaker;  // Geometry Making Interface
	BOOL  m_bAfter;   // construction section : flag for before or after
	BOOL	m_bReverse;
	BOOL  m_bCentroid;
	BOOL  m_bShearCheck;
	BOOL  m_bChangeAxis;

	T_RPSC_D* m_pDataSrcRebar;
	BOOL      m_bEndJ;
	// MNET:XXXX-CJJEONG-20090317 (Tel : 2124) 
	BOOL      m_bComGenJ;

// Operations
public:
	BOOL Init(CWnd* pParentWnd);
	void SetDataSource(T_SECT_D* pOriginSect, T_RFST_D* pReinforceSect, BOOL bAfter = FALSE, BOOL bChangeAxis = FALSE);
public:
private:
	bool EQ(double d1, double d2, double dTol = 1.0e-7);
	void EraseBkgnd(CDC* pDC);
	void GetRegularData(double& H, double& B1, double& tw, double& tf1, double& B2, double& tf2);
	void MakeReinforcementHBeam(CCMGeom2DArray& aGeom);
	void MakePicture(T_SECT_D* pDataSrc, CCMGeom2DArray& aGeom);
private:
	//Draw
	void DrawSection(CDC* pDC, CCMGeom2DArray& aGeom, BOOL bCentroid, BOOL bOffset, BOOL bShearCheck, int nRebarNum, BOOL bChangeAxis);
	void CalcScaleOrgOff(CRect& rect, CCMGeom2DArray& aGeom, double& scale, CPoint& org, CPoint& offset);
	void DrawAxis(CDC* pDC, CPoint& org, BOOL bChangeAxis);

	//Data
	CRect   m_DrawArea;
private:
	int RoundUp(double dVal);
protected:
	//{{AFX_MSG(CCMSecViewWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
