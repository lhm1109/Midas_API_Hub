#if !defined(__CMREBARCOMPOSITEMOMENT_H__)
#define __CMREBARCOMPOSITEMOMENT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMRebarCompositeMoment.h : header file
//
//#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

class CCMRebarCompositeMoment_I;
class CCMRebarCompositeMoment_J;
/////////////////////////////////////////////////////////////////////////////
// CCMRebarCompositeMoment dialog

class CCMSecViewWnd;
class CCMRebarCompositeDlg;
class CCMRebarCompositeMoment : public CChildDialog
{
// Construction
public:
	CCMRebarCompositeMoment(CCMSecViewWnd* pSectView, CWnd* pParent = NULL);   // standard constructor

// interface
public:
	void SetCurData(T_RPSC_D* pData);
	BOOL GetCurData(T_RPSC_D* pData);
	void RedrawView();
	void CrackButtonCheck();
	void SetPart1Enabled(BOOL bEnable);
	BOOL IsPart1Enabled();

// Dialog Data
	//{{AFX_DATA(CCMRebarCompositeMoment)
	enum { IDD = IDD_CMD_REBAR_COMPOSITE_MOMENT };
	CDlgTabCtrl	m_ctrlTab;
	CButton	m_chkSameIJ;
	CButton	m_chkCrack;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMRebarCompositeMoment)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	T_RPSC_D m_Data;
	BOOL m_bPart1Enabled;
	CCMSecViewWnd*  m_pSectView;
	CCMRebarCompositeDlg* m_pParent;

	CCMRebarCompositeMoment_I* m_pMomentI;
	CCMRebarCompositeMoment_J* m_pMomentJ;

protected:
	void LayoutChildDlg();

	void Data2Dlg();
	BOOL Dlg2Data(T_RPSC_D* pData, BOOL bSaveAlways=TRUE);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMRebarCompositeMoment)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSameCheck();
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMREBARCOMPOSITEMOMENT_H__)
