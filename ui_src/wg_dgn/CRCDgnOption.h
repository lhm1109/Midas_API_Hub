// CRCDgnOption.h: interface for the CCRCDgnOption class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRCDGNOPTION_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_CRCDGNOPTION_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"


#include "CRCDesignOptTabIDlg.h"
#include "CRCDesignOptTabJDlg.h"

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "DgnTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"

class CDBDoc;

class CCRCDesignOptTabIDlg;
class CCRCDesignOptTabJDlg;


class CCRCDgnOption : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CCRCDgnOption(CWnd* pParent = NULL);
	virtual void Execute();

// Dialog Data
	//{{AFX_DATA(CCRCDgnOption)
	enum { IDD = IDD_DGN_CRC_DESIGN_OPTION };

	int	 m_nOption;
	int	 m_nMOption;
	int	 m_nSOption;
	int  m_nTOption;
	int  m_nFOption;
	int  m_nCOption;
	int  m_nClass;
	int  m_nElemType;
	BOOL m_bTypeC;

	//
	int   m_iDgnCode;
	//}}AFX_DATA
	BOOL	m_bBothIJ;
	CDlgTabCtrl	m_Tab;
	CButton m_Ctrl_BothIJ;
	//}}AFX_DATA

	T_CPUL_TYPE m_I_Data;
	T_CPUL_TYPE m_J_Data;

	BOOL m_bJTabEnable;

	CArray<UINT, UINT> m_aElemTypeCtrl;
	CArray<UINT, UINT> m_aMoveCtrl;

public:
	void InitArray();
	void Initial_SelectItem();
	void Initial_Data();
	void AlignControls();
	void EnableDisableControls();
	void ShowHideControls();
	void ResizeDialog();
	void HideAndMoveControlVBemByDB();
	void UpdateBuffer();

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCRCDgnOption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSelchangeTabIJ(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	CArray<UINT, UINT> m_aRUS;
	CArray<UINT, UINT> m_aBS;
	CArray<UINT, UINT> m_aTMH;

	// Generated message map functions
	//{{AFX_MSG(CCRCDgnOption)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnCRCPoscAr();
	afx_msg void OnDgnCRCPoscDel();
	afx_msg void OnDgnClassRdo();
	afx_msg void OnDgnPSCElemTypeRdo();
	//}}AFX_MSG

	BOOL Dlg2Data();

	T_CPUL_D m_Data;

	CCRCDesignOptTabIDlg* m_pSubDlg_I;
	CCRCDesignOptTabJDlg* m_pSubDlg_J;

	// Generated message map functions
	//{{AFX_MSG(CCPGUnbraLengDlg)
	//virtual BOOL OnInitDialogIJ();
	afx_msg void OnDgnCloseIJ();
	afx_msg void OnDgnExecuteIJ();
	afx_msg void OnDgnCPGLoadAppAddIJ();
	afx_msg void OnDgnCPGLoadAppDelIJ();
	afx_msg void OnBothIJChk();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRCDGNOPTION_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
