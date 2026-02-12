#if !defined(AFX_NLLINKPRTVISCDLG_H__E167798B_942B_4584_B534_105E1ABE278F__INCLUDED_)
#define AFX_NLLINKPRTVISCDLG_H__E167798B_942B_4584_B534_105E1ABE278F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NLLinkPrtViscDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtViscDlg dialog

class CNLLinkPrtViscDlg : public CDialogMove
{
// Construction
public:
	CNLLinkPrtViscDlg(CWnd* pParent = NULL);   // standard constructor
	~CNLLinkPrtViscDlg();
// Dialog Data
	//{{AFX_DATA(CNLLinkPrtViscDlg)
	enum { IDD = IDD_ETC_NLLINK_PRT_VISC };
	CTextUnit	m_wndStifBracUnit;
	CTextUnit	m_wndStifDampUnit;
	CTextUnit	m_wndDampingUnit;
	CTextUnit m_wndRefVelocityUnit;	
	CEditUnit	m_wndStifBrac;
	CEditUnit	m_wndStifDamp;
	CEditUnit	m_wndDamping;  
	CEditUnit	m_wndDampExpo;
	CEditUnit	m_wndRefVelocity;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNLLinkPrtViscDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	T_NLLP_D m_Data;
	int m_nDofType;  

protected:
	CArray<UINT, UINT> m_aCtrlDamperType;

	void  ShowData(int nFlag);
	int   ShowHideCtrl();
	void  ChangeImage(int nItem);
	MillustViewer m_pBitmap01;
	MillustViewer m_pBitmap02;
	// Generated message map functions
	//{{AFX_MSG(CNLLinkPrtViscDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDamperType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NLLINKPRTVISCDLG_H__E167798B_942B_4584_B534_105E1ABE278F__INCLUDED_)
