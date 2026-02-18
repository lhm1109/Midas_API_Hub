#if !defined(__DGNREBARPSCMOMENT_H__)
#define __DGNREBARPSCMOMENT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRebarPscMoment.h : header file
//

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\ChildDialog.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

class CDgnRebarPscMoment_I;
class CDgnRebarPscMoment_J;
/////////////////////////////////////////////////////////////////////////////
// CDgnRebarPscMoment dialog

class CCMSecViewWnd;
class CDgnRebarPscDlg;
class CDgnRebarPscMoment : public CChildDialog
{
// Construction
public:
	CDgnRebarPscMoment(CCMSecViewWnd* pSectView, int nPart, CWnd* pParent = NULL);   // standard constructor

// interface
public:
	void SetCurData(T_RPSC_D* pData);
	BOOL GetCurData(T_RPSC_D* pData);
	void RedrawView();

// Dialog Data
	//{{AFX_DATA(CDgnRebarPscMoment)
	enum { IDD = IDD_DGN_REBAR_PSC_MOMENT };
	CDlgTabCtrl	m_ctrlTab;
	MButton	m_chkSameIJ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRebarPscMoment)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	T_RPSC_D m_Data;
	CCMSecViewWnd*  m_pSectView;
	CDgnRebarPscDlg* m_pParent;

	CDgnRebarPscMoment_I* m_pMomentI;
	CDgnRebarPscMoment_J* m_pMomentJ;

	int m_nPart;

protected:
	void LayoutChildDlg();

	void Data2Dlg();
	BOOL Dlg2Data(T_RPSC_D* pData, BOOL bSaveAlways=TRUE);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnRebarPscMoment)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnSameCheck();
	afx_msg void OnSelchangeDgnTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNREBARPSCMOMENT_H__)
