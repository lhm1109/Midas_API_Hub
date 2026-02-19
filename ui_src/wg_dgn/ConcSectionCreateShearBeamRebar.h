// (060123) sshan Create

#if !defined(__CONCSECTIONCREATESHEARBEAMREBAR_H__)
#define __CONCSECTIONCREATESHEARBEAMREBAR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConcSectionCreateRebar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConcSectionCreateShearBeamRebar dialog

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
class __MY_EXT_CLASS__ CConcSectionCreateShearBeamRebar : public CChildDialog
{
// Construction
public:
	CConcSectionCreateShearBeamRebar(CWnd* pParent = NULL, int nCurTab = 0);   // standard constructor
	virtual ~CConcSectionCreateShearBeamRebar();
	
// Dialog Data
	//{{AFX_DATA(CConcSectionCreateShearBeamRebar)
	enum { IDD = IDD_DGN_CON_CREATE_BEAM_REBAR_SHEAR };
	
	MComboBox m_cmbSize;
	CEditUnit m_edtSpacing;
	CTextUnit m_untSpacing;
	CEditUnit m_edtNum;
	CEditUnit m_edtAngle;
	CTextUnit m_untAngle;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConcSectionCreateShearBeamRebar)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL


// Implementation

public:
	T_RBGC_SHBR m_ShbrData;
	int m_nCurTab;

	void EnableDisableControls(BOOL bSameIMJ);
	void Data2Dlg();
	BOOL Dlg2Data();	

	void SetInitData(T_RBGC_SHBR ShearRebarData);
	void SaveDlgData();	

protected:
	CConcSectionShearBeamRebar* m_pParent;
	CDBDoc* m_pDoc;

	void InitCombo();
	void InitUnit();
	
	int GetComboRebarIndex(CString strRebarname);

protected:
	// Generated message map functions
	//{{AFX_MSG(CConcSectionCreateShearBeamRebar)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////
#include "HeaderPost.h"     /////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CONCSECTIONCREATESHEARBEAMREBAR_H__)
