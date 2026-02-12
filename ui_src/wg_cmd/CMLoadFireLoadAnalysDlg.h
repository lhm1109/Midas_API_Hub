#if !defined(__CMLOADFIRELOADANALYSDLG_H__)
#define __CMLOADFIRELOADANALYSDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadCaseDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMLoadFireLoadAnalysDlg dialog

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////

class __MY_EXT_CLASS__ CCMLoadFireLoadAnalysDlg : public CDialogMove
{
// Construction
public:
	CCMLoadFireLoadAnalysDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMLoadFireLoadAnalysDlg();

// Dialog Data
	//{{AFX_DATA(CCMLoadFireLoadAnalysDlg)
	enum { IDD = IDD_CMD_LOADFIRE_DLG };
	BOOL	m_bChk;
	//}}AFX_DATA


// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	CDBDoc* m_pDoc;
	void AddNewSection();
	BOOL AddElemStiffCoef();
	double GetCoef(T_SECT_K nKey, T_KEY_LIST &lstFireElem);

	// Generated message map functions
	//{{AFX_MSG(CCMLoadFireLoadAnalysDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDlgOK();
	afx_msg void OnDlgClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////
#include "HeaderPost.h"     /////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMLOADFIRELOADANALYSDLG_H__)
