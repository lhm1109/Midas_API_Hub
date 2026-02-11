#if !defined(__CMLOADMLLCDLG_H__)
#define			 __CMLOADMLLCDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadMLLCDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\mit_frx\MCheckListBox.h"

/////////////////////////////////////////////////////////////////////////////
// CCMLoadMLLCDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMLoadMLLCDlg : public CDialogMove , public CDBUpdateConnector
{
// Construction
public:
	CCMLoadMLLCDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMLoadMLLCDlg();
// Dialog Data
	//{{AFX_DATA(CCMLoadMLLCDlg)
	enum { IDD = IDD_CMD_LOAD_MLLC_DLG };
	CComboBox	m_cboLoadCase;
	CComboBox	m_cboLoadComb;
	mit::frx::MCheckListBox m_listLoad;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadMLLCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Implementation
protected:
	void DataExchange(T_LOAD_ATTR& LoadAttr, int nOperation);
	CDBDoc* m_pDoc;

	void ModifyLoadData(BOOL bCheckLoad);
	void InitLoadData();	
	void InitLoadCaseData();
	void InitLoadCombData();
	void InitMLLCData();
	BOOL MakeLctbData(T_LCOM_D& LcomData, T_STLD_K StldKey, T_LCTB_D& LctbData);
	T_LCOM_K GetLoadCombKey(CString& strLoadComb, int& nLoadType);
	CString GetLoadCombName(int nCombType, CString strLoadCombName);
	BOOL m_bCheckUpdate;


	BOOL m_bCheckLoad;  //True for LoadCase , FALSE for LoadCombination
	T_STLD_K m_StldKeyOld;
	T_LCOM_K m_LcomKeyOld;
	int m_nOldCombType;
	// Generated message map functions
	//{{AFX_MSG(CCMLoadMLLCDlg)
	afx_msg void OnCmdMllcCombBtn();
	afx_msg void OnCmdMllcCaseBtn();
	afx_msg void OnCmdMllcAcheckBtn();
	afx_msg void OnCmdMllcUcheckBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdMllcCancelBtn();
	afx_msg void OnCmdMllcOkBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADMLLCDLG_H__BB93AF01_7A74_11D4_9AAF_0000C0B9C58C__INCLUDED_)
