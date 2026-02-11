#if !defined(__NLLINKPRTDFRICDLG_H__)
#define __NLLINKPRTDFRICDLG_H__

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
// CNLLinkPrtDfricDlg dialog

class CNLLinkPrtDfricDlg : public CDialogMove
{
// Construction
public:
	CNLLinkPrtDfricDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNLLinkPrtDfricDlg)
	enum { IDD = IDD_ETC_NLLINK_PRT_DFRIC };

	// 0:Outer Top, 1:Outer Bottom, 2:Inner Top, 3:Inner Bottom
	CEditUnit m_edtArea;
	CEditUnit m_edtRefe;
	CEditUnit m_edtStif[2];
	CEditUnit m_edtUref[2];
	CEditUnit m_edtRate[2];
	CEditUnit m_edtRadi[2];
	CEditUnit m_edtStpd[2];
	CEditUnit m_edtCouv[2];
	CEditUnit m_edtCoup[2];
	CEditUnit m_edtCoap[2];
	CEditUnit m_edtCohp[2];
	CTextUnit m_untArea;
	CTextUnit m_untRefe;
	CTextUnit m_untStif;
	CTextUnit m_untRate;
	CTextUnit m_untRadi;
	CTextUnit m_untStpd;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNLLinkPrtDfricDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	T_NLLP_D m_Data;
	int m_nDofType;  

protected:
	void ShowData(int nFlag);
	// Generated message map functions
	//{{AFX_MSG(CNLLinkPrtDfricDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	MillustViewer m_wndPicture;
	MillustViewer m_wndPicture2;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__NLLINKPRTDFRICDLG_H__)
