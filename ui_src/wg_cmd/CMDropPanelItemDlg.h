#if !defined(AFX_CMDropPanelItemDlg_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMDropPanelItemDlg_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMDropPanelItemDlg.h : header file
//

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MillustViewer.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMDropPanelItemDlg dialog
using namespace mit::frx;

class CCMDropPanelItemDlg : public CDialogMove
{
// Construction
public:
	CCMDropPanelItemDlg(CWnd* pParent = NULL);   // standard constructor

public:
	CDBDoc* m_pDoc;
	BOOL		m_bModify;

protected:
	T_DPAN_K m_Key;
	T_DPAN_D m_Data;
	CArray<UINT , UINT> m_aCtrlLocation;

	BOOL Dlg2Data();
	BOOL Data2Dlg();

public:
	void	SetData(T_DPAN_K Key, T_DPAN_D &Data);
	BOOL	OnApplyPublic();

// Dialog Data
	//{{AFX_DATA(CCMDropPanelItemDlg)
	enum { IDD = IDD_CMD_DROP_PANEL_ITEM_DLG };
	MEdit     m_edtName;
	MEdit     m_edtDesc;
	CEditUnit m_edtB1;
	CTextUnit m_untB1;
	CEditUnit m_edtB2;
	CTextUnit m_untB2;
	CEditUnit m_edtH1;
	CTextUnit m_untH1;
	CEditUnit m_edtH2;
	CTextUnit m_untH2;
	CEditUnit m_edtAngle;
	CTextUnit m_untAngle;
	CEditUnit m_edtThik;
	CTextUnit m_untThik;

	MButton   m_chkAng;
	MButton	  m_chkThick;

	mit::frx::MillustViewer m_wndDropPanel;
	
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMDropPanelItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitUnit();
	BOOL AddToDB();

	// Generated message map functions
	//{{AFX_MSG(CCMDropPanelItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDropPanelItemDlg_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_)
