#if !defined(__DgnCRCDgnParameter_BS_H__)
#define __DgnCRCDgnParameter_BS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnCRCDgnParameter_BS.h : header file
//

#include "..\wg_cmd\MyChildDialog.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnCRCDgnParameter_BS dialog
#include "HeaderPre.h"

class CDgnCRCAnnexDgnParameterDlg;

class __MY_EXT_CLASS__ CDgnCRCDgnParameter_BS : public CMyChildDialog
{
// Construction
public:
	CDgnCRCDgnParameter_BS(T_PSCD_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	~CDgnCRCDgnParameter_BS(); 
 
// Dialog Data
	//{{AFX_DATA(CDgnCRCDgnParameter_BS)
	enum { IDD = IDD_DGN_CRC_DESIGN_PARAMETER_BS };

	BOOL      m_bUser;
	CEditUnit m_edtComp;
	CTextUnit m_untComp;
	CEditUnit m_edtTens;
	CTextUnit m_untTens;
	CEditUnit m_edtCSComp;
	CTextUnit m_untCSComp;
	CEditUnit m_edtCSTens;
	CTextUnit m_untCSTens;
	BOOL      m_bSLS1;
	BOOL      m_bSLS2;
	BOOL      m_bSLS3;
	BOOL      m_bSLS4;
	BOOL      m_bSLS5;
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
	//{{AFX_VIRTUAL(CDgnCRCDgnParameter_BS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnCRCDgnParameter_BS)
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

#endif // !defined(__DgnCRCDgnParameter_BS_H__)





















