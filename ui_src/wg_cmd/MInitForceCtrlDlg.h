#if !defined(AFX_MINITFORCECTRLDLG_H__5A9EBFE1_CF7A_11D4_880F_00010263A1F4__INCLUDED_)
#define AFX_MINITFORCECTRLDLG_H__5A9EBFE1_CF7A_11D4_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MInitForceCtrlDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CMInitForceCtrlDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CMInitForceCtrlDlg : public CDialogMove
{
// Construction
public:
	CMInitForceCtrlDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMInitForceCtrlDlg)
	enum { IDD = IDD_CMD_INIT_FORCE };
	BOOL    	m_bAddInit;
	CSelectLC	m_cmbInitLC;
	BOOL      m_bComb;
	CSelectLC	m_cmbCombLC;
	MEdit     m_edtCombF;
	CListCtrl m_lstComb;
	BOOL      m_bConsiderGeomStiff;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMInitForceCtrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CDBDoc* m_pDoc;
	T_EFCT_D m_Data;

	void SetListCtrlHeader();
	BOOL GetSelectedItem(int &nIndex);
	void MakeItemEx();
	CString DataToStr(int i, T_EFCT_BASE &data);
	void SetItem(int nIndex, T_EFCT_BASE &data);
	BOOL ValidItem(T_EFCT_BASE& data);
	BOOL Dlg2Item(T_EFCT_BASE &data);
	BOOL Item2Dlg(T_EFCT_BASE &data);

	void EnableDisableCtrl();
	void ChangeData();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMInitForceCtrlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdInitForceBtn();
	virtual void OnOK();
	afx_msg void OnCmdInitForceChk();
	afx_msg void OnCmdInitAddBtn();
	afx_msg void OnCmdInitModBtn();
	afx_msg void OnCmdInitDelBtn();
	afx_msg void OnCmdInitCombChk();
	afx_msg void OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINITFORCECTRLDLG_H__5A9EBFE1_CF7A_11D4_880F_00010263A1F4__INCLUDED_)
