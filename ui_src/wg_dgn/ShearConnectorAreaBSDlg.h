#if !defined(__ShearConnectorAreaBSDlg_H__)
#define __ShearConnectorAreaBSDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShearConnectorAreaBSDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CShearConnectorAreaBSDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CShearConnectorAreaBSDlg : public CDialogMove
{
	// Construction
public:
	CShearConnectorAreaBSDlg(CWnd* pParent = NULL);   // standard constructor
	~CShearConnectorAreaBSDlg();   // standard deconstruction

	// Dialog Data
	//{{AFX_DATA(CShearConnectorAreaBSDlg)
	enum { IDD = IDD_DGN_DESIGN_CONNECTOR_AREA_BS_DLG };

	MComboBox m_cmbDia;
	CEditUnit m_edtNum;
	CEditUnit m_edtSpacing;
	CTextUnit m_untSpacing;
	CEditUnit m_edtAe;
	CTextUnit m_untAe;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShearConnectorAreaBSDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
public:
	void InitUnit();  
	void InitCombo();

	void CalcArea();
	void GetValue(double &dArea);

	// Generated message map functions
	//{{AFX_MSG(CShearConnectorAreaBSDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnShearConnectorAreaDiaCmb();
	afx_msg void OnShearConnectorAreaNumEdt();
	afx_msg void OnShearConnectorAreSpacingEdt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ShearConnectorAreaBSDlg_H__)
