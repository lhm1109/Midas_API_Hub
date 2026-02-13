#if !defined(AFX_SHEARCONNECTORTABIIRSDLG_H__)
#define AFX_SHEARCONNECTORTABIIRSDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShearConnectorTabIDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShearConnectorTabI_IRS_Dlg dialog
#include "..\wg_base\ChildDialog.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CShearConnectorDlg;

class __MY_EXT_CLASS__ CShearConnectorTabI_IRS_Dlg : public CChildDialog
{
	// Construction
public:
	CShearConnectorTabI_IRS_Dlg(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CShearConnectorTabI_IRS_Dlg)
	enum { IDD = IDD_DGN_DESIGN_CONNECTOR_CLASS_TAB_IRS_DLG1 };
	
	CEditUnit m_edtSL;
	CTextUnit m_untSL;
	CEditUnit m_edtASL;
	CTextUnit m_untASL;
	CEditUnit m_edtAst;
	CTextUnit m_untAst;

	CEditUnit m_edtAe;
	CTextUnit m_untAe;
	CEditUnit m_edtFck;
	CTextUnit m_untFck;
	CEditUnit m_edtFy;
	CTextUnit m_untFy;
	//}}AFX_DATA
	
	
	// Overrides
	//{{AFX_VIRTUAL(CShearConnectorTabI_IRS_Dlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CArray<UINT, UINT> m_aFace;
	
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
	//{{AFX_MSG(CShearConnectorTabI_IRS_Dlg)
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnShearConnectAwBtn();
	afx_msg void OnShearConnectFyBtn();
	afx_msg void OnShearConnectFckBtn();
	afx_msg void OnShearConnectAeBtn();
	afx_msg void OnShearConnectorBarAreaEdtAst();
	afx_msg void OnShearConnectorBarAreaEdtASL();

	//afx_msg void OnUserInputChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEARCONNECTORTABIDLG_H__)
