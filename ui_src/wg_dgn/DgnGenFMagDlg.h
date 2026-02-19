#if !defined(AFX_DGNGENFMAGDLG_H__147352C8_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNGENFMAGDLG_H__147352C8_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenFMagDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnGenFMagDlg dialog

class CDgnGenFMagDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnGenFMagDlg(CWnd* pParent = NULL);   // standard constructor

public:
	void Initial_Data();
	void Initial_SelectItem();
	BOOL ErrorCheck();
	// Add by ZINU.('01.1.3).
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();

// Dialog Data
	//{{AFX_DATA(CDgnGenFMagDlg)
	enum { IDD = IDD_DGN_GEN_FMAG_DLG };
	int		m_nOption;
	double	m_B1y;
	double	m_B1z;
	double	m_B2y;
	double	m_B2z;
	int	m_nElemType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenFMagDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnGenFMagDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnFmagnifyAr();
	afx_msg void OnDgnFmagnifyDel();
	afx_msg void OnDgnCPGElemTypeRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CArray<UINT, UINT> m_aPositionCtrl, m_aElemTypeCtrl;

	void AlignControl();
	void ShowHideControls();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENFMAGDLG_H__147352C8_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
