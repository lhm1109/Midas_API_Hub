#if !defined(__DAMPERGBDATATABLEDLG_H__)
#define __DAMPERGBDATATABLEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NLLinkPrtListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "DamperGBDataGridWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CDamperGBDataTableDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDamperGBDataTableDlg : public CDialogMove
{
// Construction
public:
	CDamperGBDataTableDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDamperGBDataTableDlg();

	CDamperGBDataGridWnd	m_wndGrid;
	CEdit m_Classify;
	int m_nType;
//----------------------------------------------------------------------
// Overrides
//----------------------------------------------------------------------
public:
	void OnLButtonDblClkOnGrid(T_DMGB_D &data);

	void SetParamData(int nPropType, int nCode, int nCategory, int nRow, CString strClassify);
	BOOL GetParamData(T_DMGB_D &ParamData, int& nCode, BOOL& bAdd, BOOL &bCancel);
//----------------------------------------------------------------------
// Implementation
protected:
	BOOL CreateFolder();
protected:
	CDBDoc* m_pDoc;
	BOOL bInitSelect;
	CString strInitName;
	int m_nPropType;
	int m_nCode;
	int m_nRow;
	int m_nCategory;
	T_DMGB_D m_Data;
	BOOL m_bOK;
	BOOL m_bSave;
	BOOL m_bCancel;
// Dialog Data
	//{{AFX_DATA(CDamperGBDataTableDlg)
	enum {IDD = IDD_ETC_DAMPER_GB_DATA_LIST};
 	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDamperGBDataTableDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDamperGBDataTableDlg)
	afx_msg void OnChkDBorUserRatio();
	afx_msg void OnCmdBtnSave();
 	afx_msg void OnCmdBtnOK();
	afx_msg void OnCmdBtnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DAMPERGBDATATABLEDLG_H__)
