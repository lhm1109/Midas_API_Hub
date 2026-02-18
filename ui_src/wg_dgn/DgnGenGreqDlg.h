#if !defined(AFX_DGNGENGREQDLG_H__147352C7_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNGENGREQDLG_H__147352C7_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenGReqDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnGenGreqDlg dialog

class CDBDoc;

class CDgnGenGreqDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnGenGreqDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnGenGreqDlg)
	enum { IDD = IDD_DGN_GEN_GREQ_DLG };
	int		m_nOption;
	CComboBox m_SrcEqGrade;
	CComboBox m_ConEqGrade;
	CComboBox m_StlEqGrade;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();
	void Update_InitDataByCode();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void AdjustComboListBox(CComboBox& rComboBox);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenGreqDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CDgnGenGreqDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnGreqAr();
	afx_msg void OnDgnGreqDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENGREQDLG_H__147352C7_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
