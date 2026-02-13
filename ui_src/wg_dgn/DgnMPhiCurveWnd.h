// DgnMPhiCurveWnd.h: interface for the CDgnMPhiCurveWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNMPHICURVEWND_H__71FF1A61_7C19_44F2_A882_D7B5AB1AF2DB__INCLUDED_)
#define AFX_DGNMPHICURVEWND_H__71FF1A61_7C19_44F2_A882_D7B5AB1AF2DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\DB_ST_DT.h"
#include "..\dgnengine\src\DgnCalcBase\DgnCalcBase_MPhi_Struct.h"
#include "DgnMomentCurvatureDesign.h"

class CDgnMPhiCurveWnd : public CWnd    
{
public:
	CDgnMPhiCurveWnd();
	virtual ~CDgnMPhiCurveWnd();

	BOOL Initial(CWnd* pParentWnd);
	void Set_SectPointer(T_MPCC_D* pMpccD) { m_nDrawType = 0;  m_pMpccD = pMpccD; }
	void Set_ResultPointer(DGN_CALC_MPHI_RESULT* pResultD) { m_nDrawType = 1;  m_pResultD = pResultD; }
	void Set_CurveType(int nType) { m_nCurveType = nType; }
	void OnPaint_Wnd();
protected:
	int m_nDrawType; // 0:Section Draw  1:Chart Draw
	T_MPCC_D* m_pMpccD;
	DGN_CALC_MPHI_RESULT* m_pResultD;
	int m_nCurveType;
	CDgnMomentCurvatureDesign m_MPhiDesign;
	
	//{{AFX_MSG(CUmdSectionDrawWnd)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DGNMPHICURVEWND_H__71FF1A61_7C19_44F2_A882_D7B5AB1AF2DB__INCLUDED_)
