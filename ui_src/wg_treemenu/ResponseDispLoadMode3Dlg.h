
#pragma once

/////////////////////////////////////////////////////////////////////////////
// ResponseDispLoadMode3Dlg dialog
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\wg_base\DlgChild.h"
#include "..\wg_db\SelectLC.h"
#include "..\wg_db\CobxLdgr.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

class ResponseDispLoadMode3Dlg : public CMenuBarChildDlg
{
// Construction
public:
	ResponseDispLoadMode3Dlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(ResponseDispLoadMode3Dlg)
	enum { IDD = IDD_TM_RESPONS_DISP_LOAD_MODE3 };
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ResponseDispLoadMode3Dlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	// Generated message map functions
	//{{AFX_MSG(ResponseDispLoadMode3Dlg)
	virtual BOOL OnInitDialog();
	virtual void Execute() override { return OnBnClickedTmExecute(); }
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
		afx_msg void OnBnClickedTmExecute();
		afx_msg void OnBnClickedTmClose();
		afx_msg void OnBnClickedTmDefineLdgrBtn();
		afx_msg void OnBnClickedTmBfBtnLc();
public:
		BOOL GetPara(double dPara[]);
		BOOL CalForce(const T_KEY &ElemK,  double dPara[],  double dForce[], double dLen=1.0);
		double GetGroudZ();
		BOOL Delete(T_KEY_LIST&lstSlabs, T_KEY_LIST&lstBeams, T_KEY stldK, T_KEY groupK);
		double GetBeamWidth();
private:
		CSelectLC	m_LoadCase;
		CCobxLdgr	m_wndLdgrCobx;
		CTextUnit m_UnitGroudZ  ;
		CTextUnit m_UnitDisp    ;
		CTextUnit m_UnitModulus ;
		CEditUnit m_EditGroudZ  ;
		CEditUnit m_EditDisp    ;
		CEditUnit m_EditModulus ;
		MComboBox m_CobxDir;
		CDBDoc *m_pDoc;

};
