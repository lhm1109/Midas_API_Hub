// ShearConnectorDlg.h: interface for the CShearConnectorDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAERCONNECTORDLG_H__)
#define AFX_SHAERCONNECTORDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "DgnTabCtrl.h"
#include "ShearConnectorTabIDlg.h"
#include "ShearConnectorTabJDlg.h"
#include "ShearConnectorTabI_IRS_Dlg.h"
#include "ShearConnectorTabJ_IRS_Dlg.h"
#include "ShearConnectorTabI_BS_Dlg.h"
#include "ShearConnectorTabJ_BS_Dlg.h"
#include "ShearConnectorTabI_TMH_Dlg.h"
#include "ShearConnectorTabJ_TMH_Dlg.h"

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"

class CDBDoc;


/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class __MY_EXT_CLASS__ CShearConnectorDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CShearConnectorDlg(CWnd* pParent = NULL);
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CShearConnectorDlg)
	enum { IDD = IDD_DGN_DESIGN_CONNECTOR_CLASS_DLG };

	int   m_nOption;
	int		m_nElemType;
	BOOL	m_bBothIJ;
	CDlgTabCtrl	m_ctrlTab;
	//}}AFX_DATA

protected:
	CArray<UINT, UINT> m_aElemTypeCtrl;
	CArray<UINT, UINT> m_aMoveCtrl;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShearConnectorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
		
public:

	virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();	

	void InitTab();
	void SetSubDlg();

	void Data2Dlg();
	void Dlg2Data();

	void HideAndMoveControlVBemByDB();

	CDBDoc* m_pDoc;

	T_PSCN_D m_Data;
	

	int m_nCurTab;
	int m_iDgnCode;

	CShearConnectorTabIDlg* m_pSubDlg_I;
	CShearConnectorTabJDlg* m_pSubDlg_J;
	CShearConnectorTabI_IRS_Dlg* m_pSubDlg_IRS_I;
	CShearConnectorTabJ_IRS_Dlg* m_pSubDlg_IRS_J;
	CShearConnectorTabI_BS_Dlg* m_pSubDlg_BS_I;
	CShearConnectorTabJ_BS_Dlg* m_pSubDlg_BS_J;
	CShearConnectorTabI_TMH_Dlg* m_pSubDlg_TMH_I;
	CShearConnectorTabJ_TMH_Dlg* m_pSubDlg_TMH_J;

// Implementation
	
	// Generated message map functions
	//{{AFX_MSG(CShearConnectorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnConnectorClassOptionRdo();
	afx_msg void OnDgnConnectorClassBothChk();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnPSCElemTypeRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPOSURECLASSDLG_H__)
