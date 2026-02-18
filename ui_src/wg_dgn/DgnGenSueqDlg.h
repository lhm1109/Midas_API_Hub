#if !defined(AFX_DGNGENSUEQDLG_H__01BFEEE8_2420_4E09_8FAB_FB09D14475E9__INCLUDED_)
#define AFX_DGNGENSUEQDLG_H__01BFEEE8_2420_4E09_8FAB_FB09D14475E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenSueqDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnGenSueqDlg dialog

class CDgnGenSueqDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnGenSueqDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }
// Dialog Data
	//{{AFX_DATA(CDgnGenSueqDlg)
	enum { IDD = IDD_DGN_GEN_SUEQ_DLG };
	double	m_dEqLcaseN;
	double	m_dEqLcaseM;
	double	m_dEqLcaseV;
	double	m_dEqLcomN;
	double	m_dEqLcomM;
	double	m_dEqLcomV;
	int		m_nOption;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();
	BOOL ErrorCheck();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenSueqDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnGenSueqDlg)
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnClose();
	afx_msg void OnDGNSUEQARRadio();
	afx_msg void OnDGNSUEQDELRadio();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENSUEQDLG_H__01BFEEE8_2420_4E09_8FAB_FB09D14475E9__INCLUDED_)
