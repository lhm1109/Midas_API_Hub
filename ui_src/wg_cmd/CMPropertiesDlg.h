#if !defined(__CMPROPERTIESDLG_H__)
#define __CMPROPERTIESDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// CMPropertiesDlg.h : header file
//

#include "CMDlgBase.h"
//#include "..\wg_db\wg_db_DlgBase.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCMPropertiesDlg dialog


class CDBDoc;
class CCMMaterialPage;
class CCMSectionPage;
class CCMThicknessPage;
// class CXPropertySheet;

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////

class __MY_EXT_CLASS__ CCMPropertiesDlg : public CCMDlgBase
{
// Construction
public:
	CCMPropertiesDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMPropertiesDlg();

// Attributes
public:
//   CXPropertySheet*  m_pSheet;
	CCMMaterialPage*  m_pMaterial;
	CCMSectionPage*   m_pSection;
	CCMThicknessPage* m_pThickness;

	int m_nActivePage;
// 	CString m_csTip;

// Dialog Data
	//{{AFX_DATA(CCMPropertiesDlg)
	enum { IDD = IDD_CMD_PROPERTIES_DLG };
	CDlgTabCtrl	m_wndTab;
// 	MEdit	m_editTip;
	//}}AFX_DATA

// Operations
public:
	void SetActivePage(int SelectPage);
// 	void SetShowTip(CString& csTip);

// Overrides
public:
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMPropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMPropertiesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDlgClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////
#include "HeaderPost.h"      ////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMPROPERTIESDLG_H__)
