#if !defined(AFX_DGNGENKTYPEDLG_H__2138E5E1_D0A3_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNGENKTYPEDLG_H__2138E5E1_D0A3_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenKTypeDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CDgnGenKTypeDlg dialog

class CDgnGenKTypeDlg : public CDialogMove
{
// Construction
public:
	CDgnGenKTypeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnGenKTypeDlg)
	enum { IDD = IDD_DGN_GEN_KTYP_DLG };
	int		m_nKMode;
	//}}AFX_DATA

public:
	void Initial_Data();

public:
	double m_K;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenKTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnGenKTypeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnKinputClose();
	afx_msg void OnDgnKinputOk();
	afx_msg void OnDgnKinput065();
	afx_msg void OnDgnKinput08();
	afx_msg void OnDgnKinput10();
	afx_msg void OnDgnKinput12();
	afx_msg void OnDgnKinput20();
	afx_msg void OnDgnKinput21();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	MillustViewer m_wndPicture;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENKTYPEDLG_H__2138E5E1_D0A3_11D3_888F_0000C0F30D4D__INCLUDED_)
