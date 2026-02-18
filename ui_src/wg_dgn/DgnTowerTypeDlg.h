#if !defined(AFX_DGNTOWERTYPEDLG_H__427472FB_FC7E_48FC_8BCE_8054A7F0B026__INCLUDED_)
#define AFX_DGNTOWERTYPEDLG_H__427472FB_FC7E_48FC_8BCE_8054A7F0B026__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnTowerTypeDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnTowerTypeDlg dialog

class CDgnTowerTypeDlg : public CMenuBarChildDlg
{
// Construction
public:
	CDgnTowerTypeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnTowerTypeDlg)
	enum { IDD = IDD_DGN_TWR_MTYP_DLG };
	int		m_nOption;
	int		m_nElemType;
	//}}AFX_DATA

public:
	BOOL ErrorCheck();
	void Initial_SelectItem();
	void Initial_Data();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnTowerTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnTowerTypeDlg)
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnTypeAr();
	afx_msg void OnDgnTypeDel();
	afx_msg void OnDgnTypeYYZZVV();
	afx_msg void OnDgnTypeYYZZ();
	afx_msg void OnDgnTypeVV();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNTOWERTYPEDLG_H__427472FB_FC7E_48FC_8BCE_8054A7F0B026__INCLUDED_)
