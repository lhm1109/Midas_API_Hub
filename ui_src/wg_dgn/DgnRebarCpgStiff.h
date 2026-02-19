#if !defined(__DGNREBARCPGSTIFF_H__)
#define __DGNREBARCPGSTIFF_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRebarCpgStiff.h : header file
//

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\ChildDialog.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

class CDgnRebarCpgStiff_I;
class CDgnRebarCpgStiff_J;
/////////////////////////////////////////////////////////////////////////////
// CDgnRebarCpgStiff dialog

class CCMSecViewWnd;
class CDgnRebarCpgStiffMain;
class CDgnRebarCpgStiff : public CChildDialog
{
// Construction
public:
	CDgnRebarCpgStiff(CCMSecViewWnd* pSectView, CWnd* pParent = NULL);   // standard constructor

// interface
public:
	void SetCurData(T_CLRS_D* pData);
	BOOL GetCurData(T_CLRS_D* pData);
	void RedrawView();

// Dialog Data
	//{{AFX_DATA(CDgnRebarCpgStiff)
	enum { IDD = IDD_DGN_REBAR_CPG_STIFF };
	CDlgTabCtrl	m_ctrlTab;
	MButton	m_chkSameIJ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRebarCpgStiff)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	T_CLRS_D m_Data;
	CCMSecViewWnd*  m_pSectView;
	CDgnRebarCpgStiffMain* m_pParent;

	CDgnRebarCpgStiff_I* m_pStiffI;
	CDgnRebarCpgStiff_J* m_pStiffJ;

protected:
	void LayoutChildDlg();

	void Data2Dlg();
	BOOL Dlg2Data(T_CLRS_D* pData, BOOL bSaveAlways=TRUE);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnRebarCpgStiff)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnSameCheck();
	afx_msg void OnSelchangeDgnTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNREBARCPGSTIFF_H__)
