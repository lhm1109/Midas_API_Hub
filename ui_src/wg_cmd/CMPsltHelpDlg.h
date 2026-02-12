#if !defined(__CMPRESTYPEHELPDLG_H__)
#define __CMPRESTYPEHELPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMPsltHelpDlg.h : header file
//
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMPsltHelpDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMPsltHelpDlg : public CDialogMove, public CDBUpdateConnector
{
	// Construction
public:
	CCMPsltHelpDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMPsltHelpDlg();
	// Dialog Data
	//{{AFX_DATA(CCMPsltHelpDlg)
	enum { IDD = IDD_CMD_PRES_TYPE_HELP_DLG };

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMPsltHelpDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation

	// Generated message map functions
	//{{AFX_MSG(CCMPsltHelpDlg)
	virtual BOOL OnInitDialog();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetElemType(int nElemType);

protected:
	int m_nElemType; 
	MillustViewer m_wndPicture;
	void SetBitmap();
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMPRESTYPEHELPDLG_H__)
