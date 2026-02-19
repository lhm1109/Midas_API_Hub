#pragma once

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnStlBendModulusRtoDlg dialog

class CDBDoc;

class CDgnStlBendModulusRtoDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnStlBendModulusRtoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnStlBendModulusRtoDlg)
	enum { IDD = IDD_DGN_STL_BEND_MODULUS_DLG };
	int		m_nOption;
	double	m_dC;
	double	m_dBendModulusY;
	double	m_dBendModulusZ;
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
	//{{AFX_VIRTUAL(CDgnStlBendModulusRtoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CDgnStlBendModulusRtoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnBendModulusRtoAr();
	afx_msg void OnDgnBendModulusRtoDel();
	afx_msg void OnDgnBendModulusRtoSel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

