#if !defined(AFX_ISOLATORLRBBILDLG_H__0447C033_E190_48FF_B629_16538194A179__INCLUDED_)
#define AFX_ISOLATORLRBBILDLG_H__0447C033_E190_48FF_B629_16538194A179__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IsolatorLrbBilDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CIsolatorLrbBilDlg dialog

class CIsolatorLrbBilDlg : public CDialogMove
{
// Construction
public:
	CIsolatorLrbBilDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIsolatorLrbBilDlg)
	enum { IDD = IDD_IEHP_ISOLATOR_LRBBIL };
	CEditUnit	m_wndRminEdit;
	CTextUnit	m_wndHUnit;
	CEditUnit	m_wndAlfEdit;
	CEditUnit	m_wndHEdit;
	CTextUnit	m_wndQd50Unit;
	CEditUnit	m_wndQd50Edit;
	//}}AFX_DATA
public:
	T_IEHP_D m_Data;
	void SetIehpPropData(T_IEHP_D iehpD, T_IEHP_LRBBIL data, int nModel, int nDof);
	bool SyncDialogData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIsolatorLrbBilDlg)
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

	// Generated message map functions
	//{{AFX_MSG(CIsolatorLrbBilDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISOLATORLRBBILDLG_H__0447C033_E190_48FF_B629_16538194A179__INCLUDED_)
