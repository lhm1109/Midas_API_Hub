// ExposureClassDlg.h: interface for the CExposureClassDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPOSURECLASSDLG_H__)
#define AFX_EXPOSURECLASSDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "DgnTabCtrl.h"
#include "ExposureClassTabDlg_RC.h"
#include "ExposureClassTabJDlg_RC.h"

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"

class CDBDoc;

#define NUM_AS5100    6

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CExposureClassTabDlg_RC;
class CExposureClassTabJDlg_RC;

class __MY_EXT_CLASS__ CExposureClassDlg_RC : public CMenuBarChildDlg
{
public:
	CExposureClassDlg_RC(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CExposureClassDlg)
	enum { IDD = IDD_DGN_DESIGN_EXPOSURE_CLASS_DLG };

	int   m_nOption;
	BOOL	m_bBothIJ;
	CDlgTabCtrl	m_ctrlTab;
	//}}AFX_DATA

protected:
	CArray<UINT, UINT> m_aElemTypeCtrl;
	CArray<UINT, UINT> m_aMoveCtrl;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExposureClassDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
		
public:
	void InitTab();
	void SetSubDlg();

	void Data2Dlg();
	void Dlg2Data();

	void HideAndMoveControlForRC();

	CDBDoc* m_pDoc;

	T_RESC_D m_Data;

	int m_nCurTab;
	int m_iDgnCode;

	CExposureClassTabDlg_RC* m_pSubDlg_I;
	CExposureClassTabJDlg_RC* m_pSubDlg_J;

// Implementation
	
	// Generated message map functions
	//{{AFX_MSG(CExposureClassDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnExposureClassOptionRdo();
	afx_msg void OnDgnExposureClassBothChk();
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
