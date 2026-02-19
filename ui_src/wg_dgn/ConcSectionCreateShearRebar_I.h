// (060123) sshan Create

#if !defined(AFX_CONCSECTIONCREATESHEARREBAR_I_H__0E038880_1B43_4058_B4ED_8DE2CE7BE12F__INCLUDED_)
#define AFX_CONCSECTIONCREATESHEARREBAR_I_H__0E038880_1B43_4058_B4ED_8DE2CE7BE12F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConcSectionCreateRebar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConcSectionCreateShearRebar_I dialog


#include "..\wg_base\ChildDialog.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\MIT_frx\MComboBox.h"

#include "oncSectionRebarViewDlg.h"

#include "DgnStruct.h"

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////

//class ConcSectionRebarViewDlg;
using namespace mit::frx;
class __MY_EXT_CLASS__ CConcSectionCreateShearRebar_I : public CChildDialog
{

// Construction
public:
	CConcSectionCreateShearRebar_I(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConcSectionCreateShearRebar_I();
	
	virtual void UpdateBuffer();

	void SetCreationType();
// Dialog Data
	//{{AFX_DATA(CConcSectionCreateShearRebar_I)
	enum { IDD = IDD_DGN_CON_CREATE_REBAR_SHEAR_I };
	MComboBox	m_CobxShearRebarDia;
	CString	m_strShearRebarDia;
	CEditUnit	m_dShearRebarXStep;
	CTextUnit	m_wndLengthUnit;	
	CFormulaEditSpin m_edtShearRebarYNum;
	CFormulaEditSpin m_edtShearRebarZNum;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConcSectionCreateShearRebar_I)
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
	//{{AFX_MSG(CConcSectionCreateShearRebar_I)
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

#endif // !defined(AFX_CONCSECTIONCREATESHEARREBAR_I_H__0E038880_1B43_4058_B4ED_8DE2CE7BE12F__INCLUDED_)
