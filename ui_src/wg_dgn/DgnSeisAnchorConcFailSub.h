#if !defined(__DGNSEISANCHORCONCFAILSUB_H__)
#define __DGNSEISANCHORCONCFAILSUB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorConcFailSub.h : header file
//

#include "..\wg_base\wg_base_ChildDialog.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "DgnSeisAnchorEvalEditBtn.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorConcFailSub dialog

class CDgnSeisAnchorConcFailSub : public CChildDialog
{
// Construction
public:
	CDgnSeisAnchorConcFailSub(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorConcFailSub)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_CONC_FAIL_SUB };

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
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchorConcFailSub)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetSubData(int nDir, T_ANEV_D& Data);
	void GetSubData(T_ANEV_D& Data);

	void EnableDisableControls();
	void SetViewData();

	void Data2Dlg();
	BOOL Dlg2Data();

	void MakeEmfFile(CString strPathForEmf);

protected:
	T_ANEV_CONC_FAIL_D* GetMyConcFail();
	void InitUnit();
	void InitView();	

	int  m_nDir; // 0:교축방향, 1:교축직각방향
	T_ANEV_D m_Data;

	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchorConcFailSub)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISANCHORCONCFAILSUB_H__)
