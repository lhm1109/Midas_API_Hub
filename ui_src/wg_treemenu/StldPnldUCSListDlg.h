#if !defined(AFX_STLDPNLDUCSLISTDLG_H__B41C24B2_6881_4BDC_8075_87F11F84EFE0__INCLUDED_)
#define AFX_STLDPNLDUCSLISTDLG_H__B41C24B2_6881_4BDC_8075_87F11F84EFE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldPnldUCSListDlg.h : header file
//

#include "..\wg_base\wg_base_InternationalDlg.h"

class CStldPnldDlg;
/////////////////////////////////////////////////////////////////////////////
// CStldPnldUCSListDlg dialog
#include "afxtempl.h"

class CStldPnldUCSListDlg : public CInternationalDlg
{
	CStldPnldDlg *m_pParent;
	int		m_nX,m_nY;
	int		m_nMode;		// 0:UCS, 1:Named Plane
	BOOL	m_bDestroyed;
	CArray<CString, CString&> arList;
// Construction
public:
	CStldPnldUCSListDlg(CWnd* pParent = NULL);   // standard constructor

	void	SetDlgData(int mode,int x,int y){ m_nMode = mode; m_nX=x; m_nY=y; }
	void	SetList(CArray<CString, CString&> &list){ arList.RemoveAll(); arList.Copy(list); }

// Dialog Data
	//{{AFX_DATA(CStldPnldUCSListDlg)
	enum { IDD = IDD_TM_UCS_NAMED_PLANE_LIST_DLG };
	CListBox	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldPnldUCSListDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStldPnldUCSListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnTmUcsListSelBtn();
	afx_msg void OnTmUcsListCancelBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDPNLDUCSLISTDLG_H__B41C24B2_6881_4BDC_8075_87F11F84EFE0__INCLUDED_)
