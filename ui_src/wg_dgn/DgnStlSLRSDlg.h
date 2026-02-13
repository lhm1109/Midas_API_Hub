#if !defined(AFX_DGNSTLSLRSDLG_H__147352C2_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNSTLSLRSDLG_H__147352C2_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"

using namespace mit::frx;
class CDBDoc;
class CDgnStlSLRSDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnStlSLRSDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnStlSLRSDlg)
	enum { IDD = IDD_DGN_STL_SLRS_DLG };

	MComboBox m_cmbStructureType;
	MButton m_chkOption;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlSLRSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnStlSLRSDlg)
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeDgnStructureType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSTLSLRSDLG_H__147352C2_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
