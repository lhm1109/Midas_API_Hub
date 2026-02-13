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
#include "ExposureClassTabDlg.h"
#include "ExposureClassTabJDlg.h"

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"

class CDBDoc;

// PSC
#define NUM_PSC_EC2    18
#define NUM_PSC_LSD12  10
#define NUM_PSC_RUS     5
#define NUM_PSC_IRC112  4
#define NUM_PSC_AS5100  6
#define NUM_PSC_IRS	    3
#define NUM_PSC_BS	    4
#define NUM_R_PSC_ABNT 4
// Rating
#define NUM_R_PSC_CS454 4

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CExposureClassTabDlg;
class CExposureClassTabJDlg;

class __MY_EXT_CLASS__ CExposureClassDlg : public CMenuBarChildDlg
{
public:
	CExposureClassDlg(CWnd* pParent = NULL, int nDgnCategory = 0);

// Dialog Data
	//{{AFX_DATA(CExposureClassDlg)
	enum { IDD = IDD_DGN_DESIGN_EXPOSURE_CLASS_DLG };

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
	//{{AFX_VIRTUAL(CExposureClassDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
		
public:
	void InitTab();
	void SetSubDlg();

	void Data2Dlg();
	void Dlg2Data();

	void HideAndMoveControlVBemByDB();

	CDBDoc* m_pDoc;

	T_PESC_D m_Data;

	int m_nCurTab;
	int m_nDgnCategory; // 0:PSC, 1:Rating PSC
	int m_iDgnCode;

	CExposureClassTabDlg* m_pSubDlg_I;
	CExposureClassTabJDlg* m_pSubDlg_J;

// Implementation
	
	// Generated message map functions
	//{{AFX_MSG(CExposureClassDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnExposureClassOptionRdo();
	afx_msg void OnDgnExposureClassBothChk();
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
