#if !defined(AFX_BNGRCHANGEDLG_H__F213ECDF_FEB0_4798_AC50_4EBFA1E11D72__INCLUDED_)
#define AFX_BNGRCHANGEDLG_H__F213ECDF_FEB0_4798_AC50_4EBFA1E11D72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BngrChangeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBngrChangeDlg dialog
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\mit_frx\MCheckListBox.h"

class CBngrChangeDlg : public CMenuBarChildDlg
{
// Construction
public:
	CBngrChangeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	void SetBndrTypeList();
	unsigned int GetSourceGroupKey();
	unsigned int GetTargetGroupKey();
	void OptionCtrlMan();
// Dialog Data
	//{{AFX_DATA(CBngrChangeDlg)
	enum { IDD = IDD_TM_BNDR_GROUP_CHANGE };
	CCobxBngr m_wndTarGroupComb;
	CCobxBngr	m_wndSrcGroupComb;
	mit::frx::MCheckListBox m_wndBndrTypeList;
	int		m_nOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBngrChangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void DataExchange(CArray<int, int>& aItemChecked, int nOperation);

	// Generated message map functions
	//{{AFX_MSG(CBngrChangeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDefineGroupButton();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnTmOptionChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNGRCHANGEDLG_H__F213ECDF_FEB0_4798_AC50_4EBFA1E11D72__INCLUDED_)
