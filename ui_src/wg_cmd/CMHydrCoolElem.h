#if !defined(AFX_CMHYDRCOOLELEM_H__ACDCCD0D_16DE_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMHYDRCOOLELEM_H__ACDCCD0D_16DE_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMHydrCoolElem.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CCMHydrCoolElem dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMHydrCoolElem : public CDialogMove
{
// Construction
public:
	CCMHydrCoolElem(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMHydrCoolElem)
	enum { IDD = IDD_CMD_HYDR_COOL_LIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMHydrCoolElem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMHydrCoolElem)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnClose();
	afx_msg void OnCmdBtnAdd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMHYDRCOOLELEM_H__ACDCCD0D_16DE_11D5_880F_00010263A1F4__INCLUDED_)
