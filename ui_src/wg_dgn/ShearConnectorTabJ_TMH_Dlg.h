#if !defined(AFX_SHEARCONNECTORTABJ_TMH_DLG_H__)
#define AFX_SHEARCONNECTORTABJ_TMH_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShearConnectorTabJ_TMH_Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShearConnectorTabJ_TMH_Dlg dialog
#include "..\wg_base\ChildDialog.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CShearConnectorDlg;

class __MY_EXT_CLASS__ CShearConnectorTabJ_TMH_Dlg : public CChildDialog
{
	// Construction
public:
	CShearConnectorTabJ_TMH_Dlg(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CShearConnectorTabJ_TMH_Dlg)
	enum { IDD = IDD_DGN_DESIGN_CONNECTOR_CLASS_TAB_TMH_DLG };
	
	int       m_nPlane;
	CEditUnit m_edtLs;
	CTextUnit m_untLs;
	CEditUnit m_edtAe;
	CTextUnit m_untAe;
	CEditUnit m_edtFy;
	CTextUnit m_untFy;
	//}}AFX_DATA	
	
	// Overrides
	//{{AFX_VIRTUAL(CShearConnectorTabJ_TMH_Dlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData(BOOL bBothIJ, int nOption);
	void GetData(T_PSCN_D &PescD);

	void InitUnit();
	void ControlsEnableDisable();
	
	void Data2Dlg();
	void Dlg2Data();
	
	CShearConnectorDlg* m_pParent;
	
	T_PSCN_D m_Data;
	
	BOOL m_bBothIJ;
	int  m_nOption;
	
	// Generated message map functions
	//{{AFX_MSG(CShearConnectorTabJ_TMH_Dlg)
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnShearConnectAeBtn();
	afx_msg void OnShearConnectFyBtn();
	//afx_msg void OnUserInputChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEARCONNECTORTABJ_TMH_DLG_H__)
