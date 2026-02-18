#if !defined(__ShearConnectorFckIRSDlg_H__)
#define __ShearConnectorFckIRSDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShearConnectorFyDlg.h : header file
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
// CShearConnectorFckIRSDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CShearConnectorFckIRSDlg : public CDialogMove
{
	// Construction
public:
	CShearConnectorFckIRSDlg(CWnd* pParent = NULL);   // standard constructor
	~CShearConnectorFckIRSDlg();   // standard deconstruction

	// Dialog Data
	//{{AFX_DATA(CShearConnectorFckIRSDlg)
	enum { IDD = IDD_DGN_DESIGN_CONNECTOR_FCK_IRS_DLG};

	MComboBox m_cmbCode;
	MComboBox m_cmbDia;
	//CEditUnit m_edtLatNum;
	//CEditUnit m_edtLongNum;
	CEditUnit m_edtFy;
	CTextUnit m_untFy;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShearConnectorFckIRSDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
public:
	void InitUnit();  
	void InitCombo();
	void GetFy();
	void GetValue(double &dFy);


	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CShearConnectorFckIRSDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnShearConnectorFyCodeCmb();
	afx_msg void OnShearConnectorFyDiaCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ShearConnectorFyDlg_H__)
