#if !defined(__CMREBARCOMPOSITECOPYDLG_H__)
#define __CMREBARCOMPOSITECOPYDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMRebarCompositeCopyDlg.h : header file
//

#include "..\wg_base\wg_base_CompFunc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMRebarCompositeCopyDlg dialog
class CDBDoc;
class CCMRebarCompositeCopyDlg : public CDialogMove
{
// Construction
public:
	CCMRebarCompositeCopyDlg(UINT key, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMRebarCompositeCopyDlg)
	enum { IDD = IDD_CMD_REBAR_COMPOSITE_COPY };
	CListBox m_lstSect;
	CListBox m_lstSelect;
	//BOOL     m_bLongi;
	BOOL     m_bLongi_i;
	BOOL     m_bLongi_j;
	//BOOL     m_bShear;
	//BOOL     m_bShear_i;
	//BOOL     m_bShear_j;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMRebarCompositeCopyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	UINT m_key;
	CDBDoc *m_pDoc;

protected:
	void SetInitSectionList();
	void SetInitSelectList();
	BOOL GetSelectData(CArray<T_POS_INT_PAIR, T_POS_INT_PAIR&>& aPosIdPair, CStringArray& aName, BOOL bAll);
	BOOL GetSectData(CArray<T_POS_INT_PAIR, T_POS_INT_PAIR&>& aPosIdPair, CStringArray& aName, BOOL bAll);
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMRebarCompositeCopyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSelectBtn();
	afx_msg void OnCmdUnselectBtn();
	afx_msg void OnCmdSelectAllBtn();
	afx_msg void OnCmdNoneBtn();
	//afx_msg void OnCmdLongitudinalCheck();
	//afx_msg void OnCmdShearCheck();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMREBARCOMPOSITECOPYDLG_H__)
