#if !defined(AFX_DGNGENCMDLG_CH_H__INCLUDED_)
#define AFX_DGNGENCMDLG_CH_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenCmDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnGenCmDlg_CH dialog

class CDgnGenCmDlg_CH : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnGenCmDlg_CH(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnGenCmDlg_CH)
	enum { IDD = IDD_DGN_GEN_CM_CH_DLG };
	int		m_nOption;
	double	m_Cmy;
	double	m_Cmz;
	double  m_Cty;
	double  m_Ctz;
	BOOL    m_bFormulaCmy;
	BOOL    m_bFormulaCmz;
	BOOL    m_bFormulaCty;
	BOOL    m_bFormulaCtz;
	CComboBox m_wndCombxCmy;
	CComboBox m_wndCombxCmz;
	CComboBox m_wndCombxCty;
	CComboBox m_wndCombxCtz;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();
	void InitialFormulaCombx();
	BOOL ErrorCheck();
	// Add by ZINU.('01.1.3).
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenCmDlg_CH)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnGenCmDlg_CH)
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnCmAr();
	afx_msg void OnDgnCmDel();
	afx_msg void OnDgnCmAutoCmy();
	afx_msg void OnDgnCmAutoCmz();
	afx_msg void OnDgnCmAutoCty();
	afx_msg void OnDgnCmAutoCtz();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENCMDLG_H__147352C3_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
