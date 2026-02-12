#if !defined(AFX_ISOLATORDMPH30RBRDLG_H__139BCB41_1086_4603_9E75_059EC642B39E__INCLUDED_)
#define AFX_ISOLATORDMPH30RBRDLG_H__139BCB41_1086_4603_9E75_059EC642B39E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IsolatorDmpRbrDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\mit_frx\MComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CIsolatorDmpRbrDlg dialog

class CIsolatorDmpH30RbrDlg : public CDialogMove
{
	// Construction
public:
	CIsolatorDmpH30RbrDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
		//{{AFX_DATA(CIsolatorDmpRbrDlg)
	enum { IDD = IDD_IEHP_ISOLATOR_DMPRBR_H30 };
	CTextUnit	m_wndHUnit;
	CTextUnit	m_wndAreaUnit;
	mit::frx::MComboBox	m_wndSWCmb;
	CEditUnit	m_wndHEdit;
	CEditUnit	m_wndAreaEdit;
	CEditUnit	m_wndCbEdit;
	//}}AFX_DATA

public:
	T_IEHP_D m_Data;
	void SetIehpPropData(T_IEHP_D iehpD, T_IEHP_DMPRH30 data, int nModel, int nDof);
	bool SyncDialogData();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CIsolatorDmpRbrDlg)
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
	//{{AFX_MSG(CIsolatorDmpRbrDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISOLATORDMPRBRDLG_H__139BCB41_1086_4603_9E75_059EC642B39E__INCLUDED_)
