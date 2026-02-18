#if !defined(AFX_DGNCONPSLCOM_H__DD827AA1_F434_4E79_82F0_371D9FB7EEB6__INCLUDED_)
#define AFX_DGNCONPSLCOM_H__DD827AA1_F434_4E79_82F0_371D9FB7EEB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConPSLCOM.h : header file
//

#include "../MIT_frx/MComboBox.h"

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConPSLCOM dialog

class __MY_EXT_CLASS__ CDgnConPSLCOM : public CDialogMove
{
// Construction
public:
	CDgnConPSLCOM(CWnd* pParent = NULL);   // standard constructor

	void InitLComData();
	void EnableEditBox(BOOL nCheck);
	BOOL ErrorCheck();
// Dialog Data
	//{{AFX_DATA(CDgnConPSLCOM)
	enum { IDD = IDD_DGN_CON_PSLCOM };
	mit::frx::MComboBox	m_TranLcb;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConPSLCOM)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(DgnConPScMatDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDgnLoadCombiBtn();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONPSLCOM_H__DD827AA1_F434_4E79_82F0_371D9FB7EEB6__INCLUDED_)
