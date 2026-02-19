#if !defined(__DGNCRACKSECTANALCTRLDLG_H__)
#define __DGNCRACKSECTANALCTRLDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnCrackSectAnalCtrlDlg.h : header file
//
#include "..\MIT_frx\MButton.h"

#include "..\wg_base\DialogMove.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnCrackSectAnalCtrlDlg dialog

#include "HeaderPre.h"
class CDBDoc;
class __MY_EXT_CLASS__ CDgnCrackSectAnalCtrlDlg : public CDialogMove
{
// Construction
public:
	CDgnCrackSectAnalCtrlDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnCrackSectAnalCtrlDlg)
	enum { IDD = IDD_DGN_CRACK_SECT_ANAL_CTRL_DLG };
	CTextUnit	m_unitExTolerance;
	CEditUnit	m_edtExTolerance;
	CEditUnit	m_edtExLoadCase;	
	CEditUnit	m_edtCreep;
	mit::frx::MButton     m_chkCreep;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnCrackSectAnalCtrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitDialogData();

protected:
	CDBDoc* m_pDoc;
	
	// Generated message map functions
	//{{AFX_MSG(CDgnCrackSectAnalCtrlDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDeltaposExlcaseSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickCreep2Chk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNCRACKSECTANALCTRLDLG_H__)
