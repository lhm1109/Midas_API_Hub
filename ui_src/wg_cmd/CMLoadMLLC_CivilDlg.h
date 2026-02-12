#if !defined(AFX_CMLOADMLLC_CIVILDLG_H__C16C7F9D_6496_472E_9AA8_B3A82F18E39F__INCLUDED_)
#define AFX_CMLOADMLLC_CIVILDLG_H__C16C7F9D_6496_472E_9AA8_B3A82F18E39F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadMLLC_CivilDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\mit_frx\MEdit.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MCheckListBox.h"
/////////////////////////////////////////////////////////////////////////////
// CCMLoadMLLC_CivilDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMLoadMLLC_CivilDlg : public CDialogMove , public CDBUpdateConnector
{
// Construction
public:
	CCMLoadMLLC_CivilDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMLoadMLLC_CivilDlg();
// Dialog Data
	//{{AFX_DATA(CCMLoadMLLC_CivilDlg)
	enum { IDD = IDD_CMD_LOAD_MLLC_DLG_CIVIL };
	mit::frx::MComboBox	m_cmbPosition;
	mit::frx::MEdit	m_edtPrefix;
	CListBox	m_lstSelected;
	CListBox	m_lstDefined;
	mit::frx::MCheckListBox m_listLoad;
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadMLLC_CivilDlg)
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
	BOOL CheckCondition(int nIndex);
	BOOL MakeLctbData(T_LCOM_D& LcomData, T_STLD_K StldKey, T_LCTB_D& LctbData);
	T_LCOM_K GetLoadCombKey(CString& strLoadComb, int& nLoadType);
	CString GetLoadCombName(int nCombType, CString strLoadCombName);
	BOOL m_bCheckUpdate;
	CArray<BOOL,BOOL> m_aSelFlag;
	CArray<T_LCOM_K,T_LCOM_K> m_arLcomKey;
	CArray<int,int> m_aComType;


	BOOL m_bCheckLoad;  //True for LoadCase , FALSE for LoadCombination
	T_STLD_K m_StldKeyOld;
	T_LCOM_K m_LcomKeyOld;
	int m_nOldCombType;
	// Generated message map functions
	//{{AFX_MSG(CCMLoadMLLC_CivilDlg)
	afx_msg void OnCmdMllcAcheckBtn();
	afx_msg void OnCmdMllcUcheckBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdMllcCancelBtn();
	afx_msg void OnCmdMllcOkBtn();
	afx_msg void OnCmdMllcSelectBtn();
	afx_msg void OnCmdMllcUnselectBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADMLLC_CIVILDLG_H__C16C7F9D_6496_472E_9AA8_B3A82F18E39F__INCLUDED_)
