#if !defined(AFX_DAMPERGBITEMDLG_H__0B2FF36B_20AE_4875_9C45_A0E31BC43CEB__INCLUDED_)
#define AFX_DAMPERGBITEMDLG_H__0B2FF36B_20AE_4875_9C45_A0E31BC43CEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NLLinkPrtItemDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\DamperGBDBMgr.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_PlaceHolder.h"

class CDamperGBItem_BRB;
class CDamperGBItem_MYD;
class CDamperGBItem_FD;
class CDamperGBItem_VFD;
class CDamperGBItem_HDRD;
class CDamperGBItem_VED;
/////////////////////////////////////////////////////////////////////////////
// CDamperGBItemDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDamperGBItemDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CDamperGBItemDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDamperGBItemDlg();
	void SetParamData(T_DMGB_K Key, T_DMGB_D &ParamData) 
	{ m_Key = m_OldKey = Key, m_Data = ParamData; m_bModify = TRUE; }


protected:
// Dialog Data
	//{{AFX_DATA(CDamperGBItemDlg)
	enum {IDD = IDD_ETC_DAMPER_GB_PROPERTY_DLG};

	CEdit	m_wndName;
	CEdit	m_wndDescription;
	int		m_nPropType;
	int		m_nInputMethod;
	CComboBox	m_wndCode;
	CComboBox	m_wndCategory;
	CComboBox	m_wndInputType;
	CPlaceHolder	m_wndPlaceHolder;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDamperGBItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL ApplyOrOK();
	
	void InitInputCodeCmb();
	void InitInputCategoryCmb();
	void InitInputTypeCmb();
	void InitUserInputTypeCmb();
	void EnableDisableItemCtrls();
	void LayoutChildDlg(CChildDialog* pChildDlg);

	// Generated message map functions
	//{{AFX_MSG(CDamperGBItemDlg)
	afx_msg void OnCmdRdferDatabaseBtn();
	afx_msg void OnCmdPropTypeRdo();
	afx_msg void OnCmdInputMethodRdo();
	afx_msg void OnSelChangeInputCodeCmb();
	afx_msg void OnSelChangeInputCategoryCmb();
	afx_msg void OnSelChangeInputTypeCmb();
	afx_msg void OnCmdOK();
	afx_msg void OnCmdCancel();
	afx_msg void OnCmdApply();

	virtual BOOL OnInitDialog();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CDBDoc*		m_pDoc;
	CDlgUtil* m_pDlgUtil;
	CDamperGBItem_BRB *m_pBRB;
	CDamperGBItem_MYD *m_pMYD;
	CDamperGBItem_FD *m_pFD;
	CDamperGBItem_VFD *m_pVFD;
	CDamperGBItem_HDRD *m_pHDRD;
	CDamperGBItem_VED *m_pVED;
	T_DMGB_K	m_Key, m_OldKey;
	T_DMGB_D	m_Data;
	T_DMGB_CODE m_Code;
	T_DMGB_CODE m_UserD;
	int		m_nType;
	BOOL	m_bModify;
	BOOL	m_bOnInit;
	BOOL    m_bInit_VFD;
	int		m_nCode;
	int		m_nCategory;
	int		m_nInputType;
	BOOL    m_bChangeData;
	int     m_nInputCount;
	BOOL    m_bAddUser;
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAMPERGBITEMDLG_H__0B2FF36B_20AE_4875_9C45_A0E31BC43CEB__INCLUDED_)
