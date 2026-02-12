#if !defined(AFX_CMELSPLSPARAMDLG_H__5E50DDE7_2309_4AC3_8DCA_2C1AA070D247__INCLUDED_)
#define AFX_CMELSPLSPARAMDLG_H__5E50DDE7_2309_4AC3_8DCA_2C1AA070D247__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMElsPlsParamDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsParamDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMElsPlsParamDlg : public CDialogMove
{
// Construction
public:
	CCMElsPlsParamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMElsPlsParamDlg)
	enum { IDD = IDD_CMD_PROP_ELS_PLS_PARAM };
	CString	m_strMu;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMElsPlsParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	double m_dCValue[17];
	double m_dMu;
	void   SetMicroParamData(T_EPMT_D data) { m_Data = data; }

// Implementation
protected:
	CDBDoc* m_pDoc;
	T_EPMT_D m_Data;
	// Generated message map functions
	//{{AFX_MSG(CCMElsPlsParamDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMELSPLSPARAMDLG_H__5E50DDE7_2309_4AC3_8DCA_2C1AA070D247__INCLUDED_)
