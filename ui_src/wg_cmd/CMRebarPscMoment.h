#if !defined(__CMREBARPSCMOMENT_H__)
#define __CMREBARPSCMOMENT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMRebarPscMoment.h : header file
//
//#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

class CCMRebarPscMoment_I;
class CCMRebarPscMoment_J;
/////////////////////////////////////////////////////////////////////////////
// CCMRebarPscMoment dialog

class CCMSecViewWnd;
class CCMRebarPscDlg;
class CCMRebarPscMoment : public CChildDialog
{
// Construction
public:
	CCMRebarPscMoment(CCMSecViewWnd* pSectView, CWnd* pParent = NULL);   // standard constructor

// interface
public:
	void SetCurData(T_RPSC_D* pData);
	BOOL GetCurData(T_RPSC_D* pData);
	void RedrawView();

// Dialog Data
	//{{AFX_DATA(CCMRebarPscMoment)
	enum { IDD = IDD_CMD_REBAR_PSC_MOMENT };
	CDlgTabCtrl	m_ctrlTab;
	CButton	m_chkSameIJ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMRebarPscMoment)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	T_RPSC_D m_Data;
	CCMSecViewWnd*  m_pSectView;
	CCMRebarPscDlg* m_pParent;

	CCMRebarPscMoment_I* m_pMomentI;
	CCMRebarPscMoment_J* m_pMomentJ;

protected:
	void LayoutChildDlg();

	void Data2Dlg();
	BOOL Dlg2Data(T_RPSC_D* pData, BOOL bSaveAlways=TRUE);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMRebarPscMoment)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSameCheck();
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMREBARPSCMOMENT_H__)
