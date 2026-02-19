#if !defined(__CONCSECTIONCREATETORSIONBEAMREBAR_H__)
#define __CONCSECTIONCREATETORSIONBEAMREBAR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConcSectionCreateRebar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConcSectionCreateTorsionBeamRebar dialog

#include "..\wg_base\ChildDialog.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\MIT_frx\MComboBox.h"

#include "ConcSectionBeamRebarViewDlg.h"

#include "DgnStruct.h"

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////
using namespace mit::frx;
class __MY_EXT_CLASS__ CConcSectionCreateTorsionBeamRebar : public CChildDialog
{
// Construction
public:
	CConcSectionCreateTorsionBeamRebar(CWnd* pParent = NULL, int nCurTab = 0);   // standard constructor
	virtual ~CConcSectionCreateTorsionBeamRebar();
	
// Dialog Data
	//{{AFX_DATA(CConcSectionCreateTorsionBeamRebar)
	enum { IDD = IDD_DGN_CON_CREATE_BEAM_REBAR_TORSION };
	
	BOOL      m_bTorsionalBar;
	MComboBox m_cmbStirrupSize;
	CEditUnit m_edtStirrupSpacing;
	CTextUnit m_untStirrupSpacing;
	BOOL      m_bBundled;
	CEditUnit m_edtBundledNum;	
	MComboBox m_cmbLongiSize;
	CEditUnit m_edtLongiNum;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConcSectionCreateTorsionBeamRebar)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL


// Implementation
public:
	T_RBGC_TORBR m_TorbrData;
	int m_nCurTab;

	void EnableDisableControls(BOOL bSameIMJ);
	void Data2Dlg();
	BOOL Dlg2Data();	

	void SetInitData(T_RBGC_TORBR TorsionRebarData);
	void SaveDlgData();	

protected:
	CConcSectionTorsionBeamRebar* m_pParent;
	CDBDoc* m_pDoc;

	void InitCombo();
	void InitUnit();
	
	int GetComboRebarIndex(CString strRebarname);

protected:
	// Generated message map functions
	//{{AFX_MSG(CConcSectionCreateTorsionBeamRebar)
	afx_msg void OnClickTorsionalBarBtn();
	afx_msg void OnClickBundledBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////
#include "HeaderPost.h"     /////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CONCSECTIONCREATETORSIONBEAMREBAR_H__)
