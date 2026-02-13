#if !defined(__DgnCRCDgnParameter_TMH7_H__)
#define __DgnCRCDgnParameter_TMH7_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnCRCDgnParameter_TMH7.h : header file
//

#include "..\wg_cmd\MyChildDialog.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnCRCDgnParameter_TMH7 dialog
#include "HeaderPre.h"

class CDgnCRCAnnexDgnParameterDlg;

class __MY_EXT_CLASS__ CDgnCRCDgnParameter_TMH7 : public CMyChildDialog
{
// Construction
public:
	CDgnCRCDgnParameter_TMH7(T_PSCD_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	~CDgnCRCDgnParameter_TMH7(); 
 
// Dialog Data
	//{{AFX_DATA(CDgnCRCDgnParameter_TMH7)
	enum { IDD = IDD_DGN_CRC_DESIGN_PARAMETER_TMH7 };

	BOOL      m_bUser;
	CEditUnit m_edtComp;
	CTextUnit m_untComp;
	CEditUnit m_edtTens;
	CTextUnit m_untTens;
	CEditUnit m_edtCSComp;
	CTextUnit m_untCSComp;
	CEditUnit m_edtCSTens;
	CTextUnit m_untCSTens;
	BOOL      m_TMH7LS1;
	BOOL      m_TMH7LS2;
	BOOL      m_TMH7LS3;
	BOOL      m_TMH7LS4;
	BOOL      m_TMH7LS5;
	BOOL      m_bULS1;
	BOOL      m_bULS2;
	BOOL      m_bULS3;
	//}}AFX_DATA

public:
	T_PSCD_D* m_pParamData;

	CDBDoc* m_pDoc;

	void InitUnit();
	void EnableDisableControls();

	void Data2Dlg();
	BOOL Dlg2Data();

protected:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnCRCDgnParameter_TMH7)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnCRCDgnParameter_TMH7)
	virtual BOOL OnInitDialog();
	afx_msg void OnCRCDgnParamBSUserChk();
	afx_msg void OnCRCDgnParamBSUserBtn();
	afx_msg void OnCRCDgnParamBSSelAllBtn();
	afx_msg void OnCRCDgnParamBSUnSelAllBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnCRCDgnParameter_TMH7_H__)





















