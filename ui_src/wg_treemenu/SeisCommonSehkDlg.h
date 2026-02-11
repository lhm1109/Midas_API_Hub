#if !defined(__SeisCommonSehkDlg_H__)
#define      __SeisCommonSehkDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisCommonSehkDlg.h : header file

/////////////////////////////////////////////////////////////////////////////
// CSeisCommonSehkDlg dialog
#include "..\wg_base\wg_base_ControlEx.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
class CSeisCommonSehkDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CSeisCommonSehkDlg(CWnd* pParent = NULL);   // standard constructor
	~CSeisCommonSehkDlg();

public:
	virtual void Execute();
	  
	// Dialog Data
	//{{AFX_DATA(CSeisCommonSehkDlg)
	enum { IDD = IDD_TM_SEISCOMMON_SEHK };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCommonSehkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

protected:
	CDBDoc* m_pDoc;
	int m_nAddDel; // 0:Add/Replace, 1:Delete
	BOOL m_bUseHook;

	MComboBox m_cmbCcpt;
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSeisCommonSehkDlg)
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	void SetDescriptionByCode(int nMethod);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SeisCommonSehkDlg_H__E3A50C03_6049_11D3_8899_0000C0A2329D__INCLUDED_)
