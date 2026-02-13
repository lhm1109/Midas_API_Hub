#if !defined(__DgnSeisAnchor2ConcFailSub_H__)
#define __DgnSeisAnchor2ConcFailSub_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchor2ConcFailSub.h : header file
//

#include "..\wg_base\wg_base_ChildDialog.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "DgnSeisAnchorEvalEditBtn.h"
#include "DgnSeisAnchor2ConcFailSubGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchor2ConcFailSub dialog

class CDgnSeisAnchor2ConcFailSub : public CChildDialog
{
// Construction
public:
	CDgnSeisAnchor2ConcFailSub(CWnd* pParent = NULL, BOOL bPrint=FALSE);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchor2ConcFailSub)
	enum { IDD = IDD_DGN_SEIS_ANCHOR2_CONC_FAIL_SUB };

	CEditUnit m_edtNum;
	CEditUnit m_edtAVc;
	CTextUnit m_untAVc;
	CEditUnit m_edtAVco;
	CTextUnit m_untAVco;
	CEditUnit m_edtHa;
	CTextUnit m_untHa;
	CEditUnit m_edtCa1;
	CTextUnit m_untCa1;
	CEditUnit m_edtCa1_q;
	CTextUnit m_untCa1_q;
	CEditUnit m_edtCa2;
	CTextUnit m_untCa2;
	CEditUnit m_edtCa2_q;
	CTextUnit m_untCa2_q;
	CDgnSeisAnchorEvalEditBtn m_View;
	CDgnSeisAnchorEvalEditBtn m_View2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchor2ConcFailSub)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetSubData(int nDir, T_ANEV_D& Data, int nRatType[4]);
	void GetSubData(T_ANEV_D& Data);

	void EnableDisableControls();
	void SetViewData();

	void Data2Dlg();
	BOOL Dlg2Data(BOOL bOKBtn=FALSE);

	void MakeEmfFile(CString strPathForEmf);

protected:
	T_ANEV_CONC_FAIL_D* GetMyConcFail();
	void InitUnit();
	void InitView();	

	int  m_nDir; // 0:교축방향, 1:교축직각방향
	T_ANEV_D m_Data;
	int m_nRatTypeFailConcSub[4];
	BOOL m_bPrintCFsub;

	CDgnSeisAnchor2ConcFailSubGrid* m_pGrid;
	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchor2ConcFailSub)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisAnchor2ConcFailSub_H__)
