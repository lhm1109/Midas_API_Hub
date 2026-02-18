#if !defined(__DGNSTLMATSECTDLG_H__)
#define __DGNSTLMATSECTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlSectDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "HeaderPre.h"

#define CONTROL_TYPE_A		0
#define CONTROL_TYPE_B		1

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnStlSectDlg dialog

class __MY_EXT_CLASS__ CDgnStlSectDlg : public CDialogMove
{
// Construction
public:
	CDgnStlSectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnStlSectDlg)
	enum { IDD = IDD_DGN_STL_SECT_DLG };
	CListCtrl	m_ListCtrl;
	CComboBox	m_SectY;
	CComboBox	m_SectZ;
	
	CString m_yzList[6];
	//}}AFX_DATA

public:
	void Initial_Data();
	void Initial_ListCtrl();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlSectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;  

	// Generated message map functions
	//{{AFX_MSG(CDgnStlSectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedDgnSteelSectList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickDgnSteelSectList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeY();
	afx_msg void OnSelchangeZ();
	virtual void OnOK();
	virtual void OnCancel();  	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif 
