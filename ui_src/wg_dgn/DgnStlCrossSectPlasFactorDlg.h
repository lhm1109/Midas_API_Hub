#pragma once

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnStlCrossSectPlasFactorDlg dialog

class CDBDoc;

class CDgnStlCrossSectPlasFactorDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnStlCrossSectPlasFactorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnStlCrossSectPlasFactorDlg)
	enum {		IDD = IDD_DGN_STL_CROSSSECT_PLAC_FACT_DLG	};
	int		m_nOption;
	double	m_dGammaY;
	double	m_dGammaZ;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();
	BOOL ErrorCheck();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();
	void SectPropertyTest();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlCrossSectPlasFactorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CDgnStlCrossSectPlasFactorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnSpdfAr();
	afx_msg void OnDgnSpdfDel();
	afx_msg void OnDgnSpdfSel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

