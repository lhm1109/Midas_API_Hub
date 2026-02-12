#if !defined(AFX_CMLOADPNLDLINEDLG_H__EA13D091_55AB_469D_AEFE_9DF3F7005805__INCLUDED_)
#define AFX_CMLOADPNLDLINEDLG_H__EA13D091_55AB_469D_AEFE_9DF3F7005805__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadPNLDLineDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMLoadPNLDLineDlg dialog
#include "..\wg_base\wg_base_InternationalDlg.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_Cobxpnld.h"
#include "..\wg_db\wg_db_CobxLdgr.h"
#include "..\wg_base\MouseEdit.h"

class CCMLoadPNLDLineDlg : public CInternationalDlg
{
	T_PNLD_D *m_pData;

// Construction
public:
	CCMLoadPNLDLineDlg(T_PNLD_D *pData,CWnd* pParent = NULL);   // standard constructor

// data exchagne
public:
	void	SetData2Dlg(void);
	BOOL	SetDlg2Data(void);
	void	AfterPressUniChk(BOOL uni);
	void	InitUnit(void);
	void  ShowEdit(BOOL fg);


// Dialog Data
	//{{AFX_DATA(CCMLoadPNLDLineDlg)
	enum { IDD = IDD_CMD_PNLD_PAGE_LINELD };
	CTextUnit m_stX2Y2MomUt;
	CTextUnit m_stX1Y1MomUt;
	CEditUnit	m_edX2Y2Mom;
	CEditUnit	m_edX1Y1Mom;
	CEditUnit	m_edX2Y2Ld;
	CEditUnit	m_edX1Y1Ld;
	CMouseEdit	m_edX1Y1;
	CMouseEdit	m_edX2Y2;
	CTextUnit	m_stX1Y1Ut;
	CTextUnit	m_stX2Y2Ut;
	CTextUnit	m_stX2Y2LdUt;
	CTextUnit	m_stX1Y1LdUt;
	//CString	m_sX2Y2LdEd;
	//CString	m_sX1Y1LdEd;
	CString	m_sX2Y2Ed;
	CString	m_sX1Y1Ed;
	BOOL	m_bUniChk;
	int		m_nTypeRD;
	//CString	m_sX1Y1MomEd;
	//CString	m_sX2Y2MomEd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadPNLDLineDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMLoadPNLDLineDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdPlldLlUnichk();
	afx_msg void OnCmdPlldLlFrcrd();
	afx_msg void OnCmdPlldLlMmtrd();
	//}}AFX_MSG
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADPNLDLINEDLG_H__EA13D091_55AB_469D_AEFE_9DF3F7005805__INCLUDED_)
