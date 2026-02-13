// DgnLateralCapaResultGrid.h: interface for the CDgnLateralCapaResultGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNLATERALCAPARESULTGRID_H__A8F911D1_3000_451F_95D9_FA9D95722624__INCLUDED_)
#define AFX_DGNLATERALCAPARESULTGRID_H__A8F911D1_3000_451F_95D9_FA9D95722624__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "..\wg_db\wg_db_UnitCtrl.h"
#include "DgnSeismicDesignStruct.h"

#define D_LCR_COL_SEL       1
#define D_LCR_COL_NAME      2
#define D_LCR_COL_ELEM      3
#define D_LCR_COL_EQTYPE    4
#define D_LCR_COL_DIR       5
#define D_LCR_COL_PC        6
#define D_LCR_COL_PU        7
#define D_LCR_COL_PS        8
#define D_LCR_COL_PS0       9
#define D_LCR_COL_DECISION  10
#define D_LCR_COL_PA        11
#define D_LCR_COL_KHCW      12
#define D_LCR_COL_DECISION2 13
#define D_LCR_COL_DELTA_RA  14
#define D_LCR_COL_DELTA_R   15
#define D_LCR_COL_DECISION3 16
#define D_LCR_COL_NUM       16

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnLateralCapaResultGrid : public CGXGridWnd
{
	// Construction
public:
	CDgnLateralCapaResultGrid();
	
	// Attributes
public:
	
	//_DGN_RESERVE_LATERAL_CAPA_RES* m_pResData;
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnLateralCapaResultGrid)
	//}}AFX_VIRTUAL
	
	////////////// CODE BY ZINU
	
	virtual BOOL OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);
	virtual BOOL IsHiddenByIntend(int nCol);
	virtual BOOL CopyAll();	
	
	////////////// CODE BY ZINU
	
	// Implementation
public:
	virtual ~CDgnLateralCapaResultGrid();	
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CDgnLateralCapaResultGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"


#endif // !defined(AFX_DGNLATERALCAPARESULTGRID_H__A8F911D1_3000_451F_95D9_FA9D95722624__INCLUDED_)
