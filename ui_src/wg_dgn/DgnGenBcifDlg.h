#if !defined(AFX_DGNGENBCIFDLG_H__147352C8_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNGENBCIFDLG_H__147352C8_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;
class CDBDoc;

class CDgnGenBCifDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnGenBCifDlg(CWnd* pParent = NULL);   // standard constructor
	
public:


public:
	void Initial_Data();
	void InitCtrl();
	void Initial_SelectItem();
	// Add by ZINU.('01.1.3).
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();

// Dialog Data
	//{{AFX_DATA(CDgnGenBCifDlg)
	enum { IDD = IDD_DGN_GEN_BCIF_DLG };
	int		m_nOption;
	
	MComboBox m_cmbAlphaY;
	MComboBox m_cmbAlphaZ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenBCifDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnGenBCifDlg)
	virtual void Execute();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnBcifnifyAr();
	afx_msg void OnDgnBcifnifyDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENBCIFDLG_H__147352C8_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
