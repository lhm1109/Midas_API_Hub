#if !defined(AFX_CDgnCPGDesignParamBaseDlg_H__A5FA86BC_EA80_42CC_B555_B733E29842FF__INCLUDED_)
#define AFX_CDgnCPGDesignParamBaseDlg_H__A5FA86BC_EA80_42CC_B555_B733E29842FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDgnCPGDesignParamBaseDlg.h : header file
//
#include "DgnDlgBase.h"
#include "DgnCodeCtrl.h"

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\DlgReplaceMgr.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

//#include "DgnTabCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamBaseDlg dialog
// add by maxiao 2006.08.08  RC Design input params main dialog
enum{
#if defined (_US)
	ENUM_AASHTO_12,
	ENUM_AASHTO_07,
	ENUM_EN_1994,
	ENUM_CSA_S6_14,
	ENUM_KSCE_LSD12,
	ENUM_SNIP_2050384,
	ENUM_SP35_13330_2011,
 	ENUM_IRC_22_2008,
	ENUM_IRC_22_2015,

#elif defined(_RUS)
	ENUM_SNIP_2050384,
	ENUM_SP35_13330_2011,
	ENUM_EN_1994,
	ENUM_AASHTO_12,
	ENUM_AASHTO_07,
	ENUM_IRC_22_2008,
	ENUM_KSCE_LSD12,
	ENUM_CSA_S6_14,
	ENUM_IRC_22_2015,
#elif defined(_JP)
	ENUM_EN_1994,
	ENUM_AASHTO_12,
	ENUM_AASHTO_07,
	ENUM_KSCE_LSD12,
	ENUM_SNIP_2050384,
	ENUM_SP35_13330_2011,
	ENUM_CSA_S6_14,
	ENUM_IRC_22_2008,
	ENUM_IRC_22_2015,
#elif defined(_CH)
	ENUM_AASHTO_12,
	ENUM_AASHTO_07,
	ENUM_EN_1994,
	ENUM_KSCE_LSD12,
	ENUM_SNIP_2050384,
	ENUM_SP35_13330_2011,
	ENUM_CSA_S6_14,
	ENUM_IRC_22_2008,
	ENUM_IRC_22_2015,
#elif defined(_ORG)
	ENUM_KSCE_LSD12,
	ENUM_AASHTO_07,
	ENUM_AASHTO_12,
	ENUM_EN_1994,
	ENUM_SNIP_2050384,
	ENUM_SP35_13330_2011,
	ENUM_CSA_S6_14,
	ENUM_IRC_22_2008,
	ENUM_IRC_22_2015,
#else
	ENUM_EN_1994,
	ENUM_AASHTO_12,
	ENUM_AASHTO_07,
	ENUM_KSCE_LSD12,
	ENUM_SNIP_2050384,
	ENUM_IRC_22_2008,
	ENUM_SP35_13330_2011,
	ENUM_CSA_S6_14,
	ENUM_IRC_22_2015,

#endif
};

class CDgnCPGDesignParamEC2Dlg;
class CDgnCPGDesignParamLSD12Dlg;
class CDgnCPGDesignParamAASHTO07Dlg;
class CDgnCPGDesignParamSNiPDlg;
class CDgnCPGDesignParamIRC22Dlg;
class CDgnCPGDesignParamCSA14Dlg;
class CDgnCPGDesignParamIRC22_15_Dlg;
class CDgnCPGDesignParamAS5100Dlg;
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnCPGDesignParamBaseDlg :public CDialogMove//: public CDgnDlgBase, public CDBUpdateConnector//public CDialog
{
// Construction
public:
	CDgnCPGDesignParamBaseDlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnCPGDesignParamBaseDlg();   // standard Destructor

// Dialog Data
	//{{AFX_DATA(CDgnCPGDesignParamBaseDlg)
	enum { IDD = IDD_DGN_CPG_PARAM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnCPGDesignParamBaseDlg)
	public:
		int GetCurSelDgnCode();
		MComboBox m_DesignCode;
		MComboBox m_SubCode;
		MComboBox m_DesignAnnexAASHTO;
		MButton   m_btnUpdateParam;

	//virtual BOOL DestroyWindow();//add by maxiao
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SaveOrUpdate();
	void Data2Dlg();
	BOOL Dlg2Data();

    void InitCtrl();
	void ResizeDialog();

	CString m_strCurCode;

protected:  
	int m_nCurrPage;
	CDBDoc* m_pDoc;
	T_CPGD_D m_MainData;
	CArray<CRect, CRect> m_aRect;

	CDlgReplaceMgr m_DlgPageMgr;
	CDgnCPGDesignParamEC2Dlg* m_pEC2Dlg;
 	CDgnCPGDesignParamLSD12Dlg* m_pLSD12Dlg;
 	CDgnCPGDesignParamLSD12Dlg* m_pLSDKDSDlg;
	CDgnCPGDesignParamAASHTO07Dlg* m_pAASHTO07Dlg;
	CDgnCPGDesignParamAASHTO07Dlg* m_pAASHTO12Dlg;
	CDgnCPGDesignParamAASHTO07Dlg* m_pAASHTO16Dlg;
    CDgnCPGDesignParamAASHTO07Dlg* m_pAASHTO17Dlg;
	CDgnCPGDesignParamAASHTO07Dlg* m_pAASHTO20Dlg;
	CDgnCPGDesignParamSNiPDlg *m_pSNiP20503_84;
	CDgnCPGDesignParamSNiPDlg *m_pSP13330_2011;
	CDgnCPGDesignParamIRC22Dlg* m_pIRC22Dlg;
	CDgnCPGDesignParamCSA14Dlg* m_pCSA14Dlg;
	CDgnCPGDesignParamCSA14Dlg* m_pCSA19Dlg;
	CDgnCPGDesignParamIRC22_15_Dlg* m_pIRC22_15_Dlg;
	CDgnCPGDesignParamAS5100Dlg* m_pAS5100_6_17Dlg;

	// Generated message map functions
	//{{AFX_MSG(CDgnCPGDesignParamBaseDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnClose();
	afx_msg void OnSelChangeDgnCombo();	
	afx_msg void OnSelchangeDgnCtrldataDesignAnnexAashto();
	afx_msg void OnClickedBtnUpdateParam();
	//}}AFX_MSG

	//int GetDgnCode(const int &EnumCode);
	void AddCpgCodeParamDlg(CString &strCode);
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDgnCPGDesignParamBaseDlg_H__A5FA86BC_EA80_42CC_B555_B733E29842FF__INCLUDED_)
