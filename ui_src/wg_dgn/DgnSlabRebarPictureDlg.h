#if !defined(AFX_DGNSLABREBARPICTUREDLG_H__)
#define AFX_DGNSLABREBARPICTUREDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSlabRebarPictureDlg.h : header file
//
#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_ModelessMoveDlg.h"
#include "..\MIT_frx\MillustViewer.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSlabRebarPictureDlg dialog
using namespace mit::frx;
class CDgnSlabRebarPictureDlg : public CModelessMoveDlg
{
// Construction
public:
	CDgnSlabRebarPictureDlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnSlabRebarPictureDlg();

// Dialog Data
	//{{AFX_DATA(CDgnSlabRebarPictureDlg)
	enum { IDD = IDD_DGN_CON_SLAB_REBAR_PIC_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSlabRebarPictureDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnSlabRebarPictureDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	MillustViewer m_wndPicture;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSLABREBARPICTUREDLG_H__)
