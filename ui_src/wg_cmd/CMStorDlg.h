#if !defined(__CMSTORDLG_H__)
#define __CMSTORDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStorDlg.h : header file
//

#include "CMDlgBase.h"
//#include "..\wg_db\wg_db_DlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

class CCMStorGrid;
class CCMStorWindSeisGrid;
class CCMStorGrid_IGen;
/////////////////////////////////////////////////////////////////////////////
// CCMStorDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMStorDlg : public CCMDlgBase
{
// Construction
public:
	CCMStorDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMStorDlg();
	double m_dTopLevel;
// Dialog Data
	//{{AFX_DATA(CCMStorDlg)
	enum { IDD = IDD_ETC_STOR_DLG };
	CTextUnit	m_wndBaseLevelUnit;
	CEditUnit	m_wndBaseLevelEdit;
	CBCGPButton m_chkLineGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStorDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	CDBDoc* m_pDoc;
	CGXTabWnd m_Tab;
	CCMStorGrid *m_pStory;
	CCMStorWindSeisGrid *m_pWind;
	CCMStorWindSeisGrid *m_pSeismic;
	CCMStorGrid_IGen *m_pStory_IGen;  

	CButton m_chkDefineStoryRegion;
	
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMStorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnEtcStorAutoGen();
	afx_msg void OnBnClickedDefineModule();
	afx_msg void OnBnClickedAddStory();
	afx_msg void OnBnClickedCalcAccidentalEccn();
	afx_msg void OnClose();
	afx_msg void OnEtcStorBlBtn();
	afx_msg void OnBnClickedCheckForLineGrid();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSTORDLG_H__)
