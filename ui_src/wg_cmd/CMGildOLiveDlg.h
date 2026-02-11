#if !defined(__CMGILDOLIVEDLG_H__)
#define __CMGILDOLIVEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGildDeadDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMGildOLiveDlg dialog
class CDBDoc;
class CCMGildBaseGrid;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMGildOLiveDlg : public CDialogMove
{
// Construction
public:
	CCMGildOLiveDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMGildOLiveDlg();

	void SetLoadCase(T_GILD_K LoadCaseKey);

// Dialog Data
	//{{AFX_DATA(CCMGildOLiveDlg)
	enum { IDD = IDD_CMD_GRID_GILD_OLIVE };
	CEdit	m_editName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGildOLiveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CDBDoc* m_pDoc;
	CCMGildBaseGrid* m_wndGrid;

	BOOL m_bModify;
	T_GILD_K m_Key;
	T_GILD_D m_Data;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMGildOLiveDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMGILDOLIVEDLG_H__)
