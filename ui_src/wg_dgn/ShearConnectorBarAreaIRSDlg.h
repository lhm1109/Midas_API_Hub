#if !defined(__ShearConnectorBarAreaIRSDlg_H__)
#define __ShearConnectorBarAreaIRSDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShearConnectorAreaDlg.h : header file
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
// CShearConnectorBarAreaIRSDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CShearConnectorBarAreaIRSDlg : public CDialogMove
{
	// Construction
public:
	CShearConnectorBarAreaIRSDlg(CWnd* pParent = NULL);   // standard constructor
	~CShearConnectorBarAreaIRSDlg();   // standard deconstruction

	// Dialog Data
	//{{AFX_DATA(CShearConnectorBarAreaIRSDlg)
	enum { IDD = IDD_DGN_DESIGN_CONNECTOR_BAR_AREA_IRS_DLG };

	//MComboBox m_cmbCode;
	MComboBox m_cmbDia;
	CEditUnit m_edtLatNum;
	CEditUnit m_edtLongNum;
	CEditUnit m_edtArea;
	CTextUnit m_untArea;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShearConnectorBarAreaIRSDlg)
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
	//{{AFX_MSG(CShearConnectorBarAreaIRSDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//afx_msg void OnShearConnectorAreaCodeCmb();
	afx_msg void OnShearConnectorAreaDiaCmb();
	afx_msg void OnShearConnectorAreaLatNumEdt();
	afx_msg void OnShearConnectorAreaLongNumEdt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ShearConnectorAreaDlg_H__)
