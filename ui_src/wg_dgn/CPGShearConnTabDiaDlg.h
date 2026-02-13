#if !defined(__CPGShearConnTabDiaDlg_H__)
#define __CPGShearConnTabDiaDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPGShearConnTabDiaDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

//#include "..\wg_base\wg_base_DlgTabCtrl.h"

//#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCPGShearConnTabDiaDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCPGShearConnTabDiaDlg : public CDialogMove
{
	// Construction
public:
	CCPGShearConnTabDiaDlg(CWnd* pParent = NULL);   // standard constructor
	~CCPGShearConnTabDiaDlg();   // standard deconstruction

	// Dialog Data
	//{{AFX_DATA(CCPGShearConnTabDiaDlg)
	enum { IDD = IDD_DGN_CPG_SHEAR_CONN_TAB_DIA_DLG };

	//MComboBox m_cmbCode;
	MComboBox m_cmbDia;
	CEditUnit m_edtDia;
	CTextUnit m_untDia;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGShearConnTabDiaDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
public:
	void InitUnit();  
	void InitCombo();
	//void InitDiaCombo();
	void GetDia();
	void GetValue(double &dDia);


	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CCPGShearConnTabDiaDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnShearConnectorDiaCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CPGShearConnTabDiaDlg_H__)
