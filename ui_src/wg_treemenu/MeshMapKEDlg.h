#if !defined(AFX_MESHMAPKEDLG_H__C4F5F3A2_B2C9_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_MESHMAPKEDLG_H__C4F5F3A2_B2C9_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeshMapKEDlg.h : header file
//

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CMeshMapKEDlg dialog

class CMeshMapKEDlg : public CMenuBarChildDlg
{
// Construction
public:
	CMeshMapKEDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	
public:
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CMeshMapKEDlg)
	enum { IDD = IDD_TM_MESH_MAP_KE };
	//}}AFX_DATA
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeshMapKEDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc*   m_pDoc;
	
	// Generated message map functions
	//{{AFX_MSG(CMeshMapKEDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESHMAPKEDLG_H__C4F5F3A2_B2C9_11D3_92DE_0000C0B0E6B3__INCLUDED_)
