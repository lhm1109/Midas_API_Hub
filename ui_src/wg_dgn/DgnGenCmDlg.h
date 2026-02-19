#if !defined(AFX_DGNGENCMDLG_H__147352C3_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNGENCMDLG_H__147352C3_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenCmDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnGenCmDlg dialog

class CDgnGenCmDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CDgnGenCmDlg(CWnd* pParent = NULL); // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDgnGenCmDlg)
	enum { IDD = IDD_DGN_GEN_CM_DLG };

	int m_nOption;
	double m_Cmy;
	double m_Cmz;
	BOOL m_bAutoCm;
	int	m_nElemType;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();
	BOOL ErrorCheck();
	// Add by ZINU.('01.1.3).
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenCmDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	//}}AFX_VIRTUAL

	void AlignControl();
	void ShowHideControls();

	// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnGenCmDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnCmAutocm();
	afx_msg void OnDgnCmAr();
	afx_msg void OnDgnCmDel();
	afx_msg void OnDgnCPGElemTypeRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CArray<UINT, UINT> m_aPositionCtrl, m_aElemTypeCtrl;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENCMDLG_H__147352C3_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
