#if !defined(AFX_STLDFIREDLG_H__5640AF59_F464_4647_AB5B_6342980F4E34__INCLUDED_)
#define AFX_STLDFIREDLG_H__5640AF59_F464_4647_AB5B_6342980F4E34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldFireDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
//#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditTemp.h"
#include "..\wg_db\wg_db_CobxLdgr.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CStldFireDlg dialog

class CStldFireDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CStldFireDlg(CWnd* pParent = NULL);   // standard constructor

	virtual void Execute() { OnTmExecute(); }
// Dialog Data
	//{{AFX_DATA(CStldFireDlg)
	enum { IDD = IDD_TM_STLD_FIRE_FIRE };
private:
	//CSelectLC	m_LoadCase;
	int		m_nOption;
	int		m_nCalcWay;
	CCobxLdgr m_wndLdgrCobx;
	MComboBox m_wndFlueFun;
	MComboBox m_wndTimeTempFun;
	//}}AFX_DATA
	BOOL AddFireTempByGB51249_2017(T_ELEM_K_LIST& aKey);
	BOOL AddFireTempByUser(T_ELEM_K_LIST& aKey);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldFireDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aDeleteCtrl;
	virtual BOOL OnInitDialog();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

	void InitLoadCase();

	MComboBox	m_LoadCase;
	CString		m_strCurLoadName;
	// Generated message map functions
	//{{AFX_MSG(CStldFireDlg)
	
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnChangeOption();
	afx_msg void OnChangeCalcWay();
	afx_msg void OnTmProjConfBtn();
	afx_msg void OnTmDefineLdgrBtn();
	afx_msg void OnFlueFunList();
	afx_msg void OnSttfFunList();
	afx_msg void OnSelLoadCase();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDFIREDLG_H__5640AF59_F464_4647_AB5B_6342980F4E34__INCLUDED_)
