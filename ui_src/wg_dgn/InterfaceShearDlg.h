// InterfaceShearDlg.h: interface for the CInterfaceShearDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERFACESHEARDLG_H__)
#define AFX_INTERFACESHEARDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "DgnTabCtrl.h"

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"

class CDBDoc;


/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CInterfaceShearTabDlg;

class __MY_EXT_CLASS__ CInterfaceShearDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CInterfaceShearDlg(CWnd* pParent = NULL);

	// Dialog Data
	enum { IDD = IDD_DGN_DESIGN_INTERFACE_SHEAR_DLG };

	int   m_nOption;
	BOOL	m_bBothIJ;
	CDlgTabCtrl	m_ctrlTab;
	//}}AFX_DATA


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void Initial_SelectItem();

	virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();	

	
	void InitTab();
	void SetSubDlg();

	void Data2Dlg();
	void Dlg2Data();


	CDBDoc* m_pDoc;

	T_PSCS_D m_Data;

	int m_nCurTab;
	int m_iDgnCode;

	CInterfaceShearTabDlg* m_pSubDlg_I;
	CInterfaceShearTabDlg* m_pSubDlg_J;


	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnConnectorClassOptionRdo();
	afx_msg void OnDgnConnectorClassBothChk();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPOSURECLASSDLG_H__)
