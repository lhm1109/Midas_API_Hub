#if !defined(AFX_SHEARCONNECTORTABIDLG_H__)
#define AFX_SHEARCONNECTORTABIDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShearConnectorTabIDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShearConnectorTabIDlg dialog
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

class __MY_EXT_CLASS__ CShearConnectorTabIDlg : public CChildDialog
{
	// Construction
public:
	CShearConnectorTabIDlg(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CShearConnectorTabIDlg)
	enum { IDD = IDD_DGN_DESIGN_CONNECTOR_CLASS_TAB_DLG };
	
	CEditUnit m_edtAngle;
	CTextUnit m_untAngle;
	CEditUnit m_edtAw;
	CTextUnit m_untAw;
	CEditUnit m_edtFy;
	CTextUnit m_untFy;
	//}}AFX_DATA
	
	
	// Overrides
	//{{AFX_VIRTUAL(CShearConnectorTabIDlg)
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
	//{{AFX_MSG(CShearConnectorTabIDlg)
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnShearConnectAwBtn();
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

#endif // !defined(AFX_SHEARCONNECTORTABIDLG_H__)
