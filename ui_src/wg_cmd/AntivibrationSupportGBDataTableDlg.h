#if !defined(__ANTIVIBRATIONSUPPORTGBDATATABLEDLG_H__)
#define __ANTIVIBRATIONSUPPORTGBDATATABLEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NLLinkPrtListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "AntivibrationSupportGBDataGridWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CAntivibrationSupportGBDataTableDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CAntivibrationSupportGBDataTableDlg : public CDialogMove
{
// Construction
public:
	CAntivibrationSupportGBDataTableDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAntivibrationSupportGBDataTableDlg();

	CAntivibrationSupportGBDataGridWnd	m_wndGrid;
	CEdit m_Classify;
	int m_nType;
//----------------------------------------------------------------------
// Overrides
//----------------------------------------------------------------------
public:
	//void OnLButtonDblClkOnGrid(T_ASGB_D &data);

	void SetParamData(int nPropType, int nCode, int nRow, CString strClassify);
	BOOL GetParamData(T_ASGB_D &ParamData, int &nCode, BOOL &bAdd, BOOL &bCancel);
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
	T_ASGB_D m_Data;
	BOOL m_bOK;
	BOOL m_bSave;
	BOOL m_bCancel;
// Dialog Data
	//{{AFX_DATA(CAntivibrationSupportGBDataTableDlg)
	enum {IDD = IDD_ETC_ANTISUPPORT_GB_DATA_LIST};
 	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAntivibrationSupportGBDataTableDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAntivibrationSupportGBDataTableDlg)
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

#endif // !defined(__ANTIVIBRATIONSUPPORTGBDATATABLEDLG_H__)
