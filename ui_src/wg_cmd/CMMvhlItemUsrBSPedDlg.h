#if !defined(AFX_CMMVHLITEMUSRBSPEDDLG_H__)
#define AFX_CMMVHLITEMUSRBSPEDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemUsrBSPedDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrBSPedDlg dialog
#include "..\wg_db\TextUnit.h"
#include "..\wg_db\EditUnit.h"

#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

struct T_MVHL_D;
class CCMMvhlItemUsrBSPedDlg : public CInternationalDlg
{
// Construction
public:
	CCMMvhlItemUsrBSPedDlg(T_MVHL_D* pData, CWnd* pParent = NULL);   // standard constructor

public:
	T_MVHL_D* m_pData;

// Dialog Data
	//{{AFX_DATA(CCMMvhlItemUsrBSPedDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMU_PED };
	CEditUnit m_edtPedW1;
	CTextUnit m_untPedW1;
	MEdit     m_edtPedW2;
	CTextUnit m_untPedW2;
	CEditUnit m_edtPedL1;
	CTextUnit m_untPedL1;
	MEdit     m_edtPedL2;
	CTextUnit m_untPedL2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemUsrBSPedDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	BOOL SetDlg2Data();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemUsrBSPedDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangePedW1Edit();
	afx_msg void OnChangePedL1Edit();
		//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMUSRBSPEDDLG_H__)
