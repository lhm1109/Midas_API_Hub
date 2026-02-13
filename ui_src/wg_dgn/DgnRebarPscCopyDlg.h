#if !defined(AFX_DGNREBARPSCCOPYDLG_H__B76ED01B_956D_46CD_99BF_BABA87F13D2F__INCLUDED_)
#define AFX_DGNREBARPSCCOPYDLG_H__B76ED01B_956D_46CD_99BF_BABA87F13D2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRebarPscCopyDlg.h : header file
//

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarPscCopyDlg dialog
class CDBDoc;
class CDgnRebarPscCopyDlg : public CDialogMove
{
// Construction
public:
	CDgnRebarPscCopyDlg(UINT key, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnRebarPscCopyDlg)
	enum { IDD = IDD_DGN_REBAR_PSC_COPY };
	CListBox m_lstSect;
	CListBox m_lstSelect;
	BOOL     m_bLongi;
	BOOL     m_bLongi_i;
	BOOL     m_bLongi_j;
	BOOL     m_bShear;
	BOOL     m_bShear_i;
	BOOL     m_bShear_j;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRebarPscCopyDlg)
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
	//{{AFX_MSG(CDgnRebarPscCopyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnSelectBtn();
	afx_msg void OnDgnUnselectBtn();
	afx_msg void OnDgnSelectAllBtn();
	afx_msg void OnDgnNoneBtn();
	afx_msg void OnDgnLongitudinalCheck();
	afx_msg void OnDgnShearCheck();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNREBARPSCCOPYDLG_H__B76ED01B_956D_46CD_99BF_BABA87F13D2F__INCLUDED_)
