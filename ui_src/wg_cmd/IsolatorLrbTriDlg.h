#if !defined(AFX_ISOLATORLRBTRIDLG_H__C180C21C_8C04_44EB_85EF_2A35FF5AEEC4__INCLUDED_)
#define AFX_ISOLATORLRBTRIDLG_H__C180C21C_8C04_44EB_85EF_2A35FF5AEEC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IsolatorLrbTriDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\mit_frx\MComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CIsolatorLrbTriDlg dialog

class CIsolatorLrbTriDlg : public CDialogMove
{
// Construction
public:
	CIsolatorLrbTriDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIsolatorLrbTriDlg)
	enum { IDD = IDD_IEHP_ISOLATOR_LRBTRI };
	CTextUnit	m_wndHUnit;
	CTextUnit	m_wndAreaUnit;
	mit::frx::MComboBox	m_wndSWCmb;
	CEditUnit	m_wndRminEdit;
	CEditUnit	m_wndHEdit;
	CEditUnit	m_wndAreaEdit;
	int		m_nRminRdo;
	//}}AFX_DATA

public:
	T_IEHP_D m_Data;
	void SetIehpPropData(T_IEHP_D iehpD, T_IEHP_LRBTRI data, int nModel, int nDof);
	bool SyncDialogData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIsolatorLrbTriDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nMdlType;
	int m_nDof;
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL CheckData(BOOL bMsg = TRUE);
	BOOL CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszName, LPCTSTR lpszVar, BOOL bMsg);
	CString GetSwName(int nSw);
	// Generated message map functions
	//{{AFX_MSG(CIsolatorLrbTriDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISOLATORLRBTRIDLG_H__C180C21C_8C04_44EB_85EF_2A35FF5AEEC4__INCLUDED_)
