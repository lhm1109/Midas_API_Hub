#if !defined(__NLLINKPRTTFRICDLG_H__)
#define __NLLINKPRTTFRICDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NLLinkPrtTfricDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtTfricDlg dialog

class CNLLinkPrtTfricDlg : public CDialogMove
{
// Construction
public:
	CNLLinkPrtTfricDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNLLinkPrtTfricDlg)
	enum { IDD = IDD_ETC_NLLINK_PRT_TFRIC };

	// 0:Outer Top, 1:Outer Bottom, 2:Inner Top, 3:Inner Bottom
	BOOL      m_bSymOut;
	CEditUnit m_edtStif[4];
	CEditUnit m_edtFrcs[4];
	CEditUnit m_edtFrcf[4];
	CEditUnit m_edtRate[4];
	CEditUnit m_edtRadi[4];
	CEditUnit m_edtStpd[4];
	CEditUnit m_edtHin;
	CEditUnit m_edtHout;
	CTextUnit m_untStif;
	CTextUnit m_untRate;
	CTextUnit m_untRadi;
	CTextUnit m_untStpd;
	CTextUnit m_untHin;
	CTextUnit m_untHout;

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNLLinkPrtTfricDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	T_NLLP_D m_Data;
	int m_nDofType;  

protected:
	MillustViewer m_wndPicture;
	MillustViewer m_wndPicture2;

	void ShowData(int nFlag);
	// Generated message map functions
	//{{AFX_MSG(CNLLinkPrtTfricDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	BOOL DataCheck();

	afx_msg void OnSymmetricOutChk();
	afx_msg void OnOTValueChangeEdt();
	afx_msg void OnITValueChangeEdt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__NLLINKPRTTFRICDLG_H__)
