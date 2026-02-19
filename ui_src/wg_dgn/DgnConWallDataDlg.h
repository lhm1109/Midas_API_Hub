#if !defined(AFX_DGNCONWALLDATADLG_H__FA9C33E1_3ACB_11D4_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNCONWALLDATADLG_H__FA9C33E1_3ACB_11D4_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConWallDataDlg.h : header file
//
#include "..\MIT_frx\MillustViewer.h"

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_TextUnit.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConWallDataDlg dialog

class CDgnConWallDataDlg : public CDialogMove
{
// Construction
public:
	CDgnConWallDataDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConWallDataDlg)
	enum { IDD = IDD_DGN_CON_WDATA_DLG };
	CTextUnit	m_Dist3Unit;
	CTextUnit	m_Dist2Unit;
	CTextUnit	m_Dist1Unit;
	CTextUnit	m_HSpaceUnit;
	int		m_nMethod;
	BOOL	m_bOutOfPlane;
	double	m_Dist1;
	double	m_Dist2;
	double	m_Dist3;
	CString	m_strWVSpace;
	double	m_WHSpace;
	BOOL m_bSpaceLimit450;

	mit::frx::MillustViewer m_wndWallEnd1;
	mit::frx::MillustViewer m_wndWallEnd2;
	mit::frx::MillustViewer m_wndWallEnd3;
	//}}AFX_DATA

public:
	int m_SpinNo;
	T_DCRB_D m_Dcrb;

public:
	BOOL ErrorCheck();
	CString Adding_ReBarSpace(CString* strSpace);

	void SetDcrb(T_DCRB_D& rDcrb);
	T_DCRB_D GetDcrb();
	void Initial_Data();
	void Initial_Units();
	void Initial_ExistData();
	void Cutting_ReBarSize(CString strSize,CArray<CString,CString>& VRebarSpace);

	// Add by ZINU.('01.7.7). Not used End Rebar if WSD.
	void Enable_EndBarEdit();
	void EnableCtrls();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConWallDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConWallDataDlg)
	afx_msg void OnDgnRcWmethod1();
	afx_msg void OnDgnRcWmethod2();
	afx_msg void OnDgnRcWmethod3();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDgnRcWspacesel();
	afx_msg void OnDgnRcWmethod4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONWALLDATADLG_H__FA9C33E1_3ACB_11D4_888F_0000C0F30D4D__INCLUDED_)
