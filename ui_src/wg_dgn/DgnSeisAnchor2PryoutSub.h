#if !defined(__DgnSeisAnchor2PryoutSub_H__)
#define __DgnSeisAnchor2PryoutSub_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchor2PryoutSub.h : header file
//

#include "..\wg_base\wg_base_ChildDialog.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "DgnSeisAnchorEvalEditBtn.h"
#include "DgnSeisAnchor2PryoutSubGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchor2PryoutSub dialog

class CDgnSeisAnchor2PryoutSub : public CChildDialog
{
// Construction
public:
	CDgnSeisAnchor2PryoutSub(CWnd* pParent = NULL, BOOL bPrint=FALSE);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchor2PryoutSub)
	enum { IDD = IDD_DGN_SEIS_ANCHOR2_PRYOUT_SUB };

	CEditUnit m_edtNum;
	CEditUnit m_edtANc;
	CTextUnit m_untANc;
	CEditUnit m_edtANco;
	CTextUnit m_untANco;
	CEditUnit m_edtHef;
	CTextUnit m_untHef;
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
	//{{AFX_VIRTUAL(CDgnSeisAnchor2PryoutSub)
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

	T_ANEV_PRYOUT_D* GetMyPryout();
	void Data2Dlg();
	BOOL Dlg2Data(BOOL bOKBtn=FALSE);

	void MakeEmfFile(CString strPathForEmf);

protected:
	void InitView();	
	void InitUnit();
	
	int  m_nDir; // 0:교축방향, 1:교축직각방향
	T_ANEV_D m_Data;
	int m_nRatTypeFailPrySub[4];
	BOOL m_bPrintCFsub;

	CDgnSeisAnchor2PryoutSubGrid* m_pGrid;
	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchor2PryoutSub)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisAnchor2PryoutSub_H__)
