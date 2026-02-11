#if !defined(AFX_MASSNMASDLG_H__D2C7A801_B45B_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_MASSNMASDLG_H__D2C7A801_B45B_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MassNmasDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MillustViewer.h"

/////////////////////////////////////////////////////////////////////////////
// CMassNmasDlg dialog

class CMassNmasDlg : public CMenuBarChildDlg
{
// Construction
public:
	CMassNmasDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CMassNmasDlg)
	enum { IDD = IDD_TM_MASS_NMAS };
	int		m_nOption;
	//}}AFX_DATA
	CEditUnit m_Value[6];
	CTextUnit m_Unit[6];
	mit::frx::MComboBox m_AngleCobx[3];
	mit::frx::MillustViewer m_wndPicture;
	mit::frx::MillustViewer m_wndPicture2;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMassNmasDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	void EnableValueEdit(BOOL bEnable);
	void AlignCtrl();
	CArray<UINT, UINT> m_aCtrlRotAngle;
	CArray<UINT, UINT> m_aCtrlOkApply;
	// Generated message map functions
	//{{AFX_MSG(CMassNmasDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeOption();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MASSNMASDLG_H__D2C7A801_B45B_11D3_92DE_0000C0B0E6B3__INCLUDED_)
