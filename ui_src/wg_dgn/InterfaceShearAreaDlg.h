#if !defined(__InterfaceShearAreaDlg_H__)
#define __InterfaceShearAreaDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InterfaceShearAreaDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

//#include "..\wg_base\wg_base_DlgTabCtrl.h"

//#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CInterfaceShearAreaDlg dialog
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CInterfaceShearAreaDlg : public CDialogMove
{
	// Construction
public:
	CInterfaceShearAreaDlg(CWnd* pParent = NULL);   // standard constructor
	~CInterfaceShearAreaDlg();   // standard deconstruction

	// Dialog Data
	//{{AFX_DATA(CInterfaceShearAreaDlg)
	enum { IDD = IDD_DGN_DESIGN_INTERFACE_SHEAR_AREA_DLG };

	//MComboBox m_cmbCode;
	MComboBox m_cmbDia;
	CEditUnit m_edtLatNum;
	CEditUnit m_edtSpacing;
	CTextUnit m_untSpacing;
	CEditUnit m_edtArea;
	CTextUnit m_untArea;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInterfaceShearAreaDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
public:
	void InitUnit();  
	void InitCombo();
	//void InitDiaCombo();
	void CalcArea();
	void GetValue(double &dArea);


	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CInterfaceShearAreaDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//afx_msg void OnInterfaceShearAreaCodeCmb();
	afx_msg void OnInterfaceShearAreaDiaCmb();
	afx_msg void OnInterfaceShearAreaLatNumEdt();
	afx_msg void OnInterfaceShearAreaSpacingEdt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__InterfaceShearAreaDlg_H__)
