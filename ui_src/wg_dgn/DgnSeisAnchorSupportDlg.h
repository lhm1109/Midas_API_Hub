#if !defined(__DGNSEISANCHORSUPPORTDLG_H__)
#define __DGNSEISANCHORSUPPORTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorSupportDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

#include "DgnSeisAnchorSupportGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorSupportDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnSeisAnchorSupportDlg : public CDialogMove
{
// Construction
public:
	CDgnSeisAnchorSupportDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorSupportDlg)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_SUPPORT_DLG };
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchorSupportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Data2Dlg();
	BOOL Dlg2Data();

	CDgnSeisAnchorSupportGrid* m_pGrid;

public:
	T_ANEV_REG_SUPP_TYPE_D m_Data;

protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchorSupportDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnSeisAnchorSupportOKBtn();
	afx_msg void OnDgnSeisAnchorSupportCancelBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	MillustViewer m_wndPicture;
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISANCHORSUPPORTDLG_H__)
