#if !defined(AFX_CMLOADPNLDAREADLG_H__47B80CBE_BD3F_4E55_93EF_46EE78F8928C__INCLUDED_)
#define AFX_CMLOADPNLDAREADLG_H__47B80CBE_BD3F_4E55_93EF_46EE78F8928C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadPNLDAreaDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMLoadPNLDAreaDlg dialog
#include "..\wg_base\wg_base_InternationalDlg.h"
#include "..\wg_base\MouseEdit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

class CCMLoadPNLDAreaDlg : public CInternationalDlg
{
	T_PNLD_D *m_pData;

// Construction
public:
	CCMLoadPNLDAreaDlg(T_PNLD_D *pData,CWnd* pParent = NULL);   // standard constructor
	
// data exchagne
public:
	void	SetData2Dlg(void);
	BOOL	SetDlg2Data(void);
	void	AfterPressUniChkAndPtRd(BOOL uni,int which);
	void	InitUnit(void);

// Dialog Data
	//{{AFX_DATA(CCMLoadPNLDAreaDlg)
	enum { IDD = IDD_CMD_PNLD_PAGE_AREALD };
	CEditUnit	m_edX4Y4Ld;
	CEditUnit	m_edX3Y3Ld;
	CEditUnit	m_edX2Y2Ld;
	CEditUnit	m_edX1Y1Ld;
	CMouseEdit	m_edX4Y4;
	CMouseEdit	m_edX3Y3;
	CMouseEdit	m_edX2Y2;
	CMouseEdit	m_edX1Y1;
	BOOL	m_bUniChk;
	int		m_nPt34;
	//}}AFX_DATA
	CString			m_sXYEd[4];
	CTextUnit		m_stXYUt[4];
	//CString			m_sXYLdEd[4];
	CTextUnit		m_stXYLdUt[4];
	CButton			m_chkUniform;

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadPNLDAreaDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMLoadPNLDAreaDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdPlldAlUnichk();
	afx_msg void OnCmdPlldAlPt3();
	afx_msg void OnCmdPlldAlPt4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADPNLDAREADLG_H__47B80CBE_BD3F_4E55_93EF_46EE78F8928C__INCLUDED_)

