// (060123) sshan Create

#if !defined(AFX_CONCSECTIONCREATESHEARREBAR_J_H__0E038880_1B43_4058_B4ED_8DE2CE7BE12F__INCLUDED_)
#define AFX_CONCSECTIONCREATESHEARREBAR_J_H__0E038880_1B43_4058_B4ED_8DE2CE7BE12F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConcSectionCreateShearRebar_J.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConcSectionCreateShearRebar_J dialog


#include "..\wg_base\ChildDialog.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MEdit.h"

#include "oncSectionRebarViewDlg.h"

#include "DgnStruct.h"

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////

//class ConcSectionRebarViewDlg;
class __MY_EXT_CLASS__ CConcSectionCreateShearRebar_J : public CChildDialog
{

// Construction
public:
	CConcSectionCreateShearRebar_J(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConcSectionCreateShearRebar_J();
	
	virtual void UpdateBuffer();

	void SetCreationType();
// Dialog Data
	//{{AFX_DATA(CConcSectionCreateShearRebar_J)
	enum { IDD = IDD_DGN_CON_CREATE_REBAR_SHEAR_J };
	MComboBox	m_CobxShearRebarDia;
	CString	m_strShearRebarDia;
	CEditUnit	m_dShearRebarXStep;	
	CFormulaEditSpin m_edtShearRebarYNum;
	CFormulaEditSpin m_edtShearRebarZNum;
	CTextUnit	m_wndLengthUnit;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConcSectionCreateShearRebar_J)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL


// Implementation

public:
	T_REBT_SHBR m_ShbrData;

	void SetInitData(T_REBT_SHBR ShearRebarData);

	void SaveDlgData();

	void AllControlEnableWindow(BOOL bEnable);

protected:

	CConcSectionShearRebar* m_pParent;
	CDBDoc* m_pDoc;

	void SetInitUnit();
	BOOL Dlg2Data();
	BOOL Data2Dlg();

	REBAR_KEY RebarKey;
	
	int addDelMod;

	int GetComboRebarIndex(CString strRebarname);

protected:
	// Generated message map functions
	//{{AFX_MSG(CConcSectionCreateShearRebar_J)
	afx_msg void OnUpdateUI(CCmdUI*pCmdUI);
	afx_msg void OnEditchangeDgnRebardbComboShear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////
#include "HeaderPost.h"     /////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONCSECTIONCREATESHEARREBAR_J_H__0E038880_1B43_4058_B4ED_8DE2CE7BE12F__INCLUDED_)
