#if !defined(__NODERENUMBERDLG_H__)
#define      __NODERENUMBERDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NodeRenumberDlg.h : header file
//
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MillustViewer.h"
#include "..\MIT_frx\MComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CNodeRenumberDlg dialog

class CNodeRenumberDlg : public CMenuBarChildDlg
{
// Construction
public:
	CNodeRenumberDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
// Dialog Data
	//{{AFX_DATA(CNodeRenumberDlg)
	enum { IDD = IDD_TM_NODE_RENUMBER };
	mit::frx::MComboBox m_SortMethodCmb;
	mit::frx::MComboBox m_RefAxisCmb;    
	mit::frx::MComboBox m_PolAxisCmb;
	mit::frx::MComboBox m_CynOrderCmb1;
	mit::frx::MComboBox m_CynOrderCmb2;
	mit::frx::MComboBox m_CynOrderCmb3;
	mit::frx::MComboBox	m_CarOrderCmb1;
	mit::frx::MComboBox	m_CarOrderCmb2;
	mit::frx::MComboBox	m_CarOrderCmb3;
	CMouseEdit m_wndRot1stPoint;
	CMouseEdit m_wndRot2ndPoint;
	CMouseEdit m_wndPol1stPoint;
	CMouseEdit m_wndPol2ndPoint;
	CTextUnit m_wnd1stPointUnit;
	CTextUnit m_wnd2ndPointUnit;
	CTextUnit m_wnd1stPointUnit2;
	CTextUnit m_wnd2ndPointUnit2;
	int		m_NumTarget;
	int		m_SelType;
	CString	m_tmStartNode;
	CString	m_tmStartElement;
	int		m_nRotAxis;
	int		m_nPolAxis;
	mit::frx::MillustViewer m_wndPicture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNodeRenumberDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	void SetInitCombString();
	void AlignControl();  
	void SetEditControl();
	void ExecuteCartesian();
	void ExecuteCylindrical();
	BOOL CheckOrthogonal();
	void SetFrameSize();
	BOOL GetRot1stPoint(double &Px, double &Py, double &Pz);
	BOOL GetRot2ndPoint(double &Px, double &Py, double &Pz);
	BOOL GetPol1stPoint(double &Px, double &Py, double &Pz);
	BOOL GetPol2ndPoint(double &Px, double &Py, double &Pz);
	int  GetOrderOpt();

	T_NODE_K StartNode;
	T_ELEM_K StartElem;  
	double m_org[3], m_rot[3], m_pol[3];
	CArray<UINT, UINT> m_aCtrlCartesian, m_aCtrlCylinder;
	CArray<UINT, UINT> m_aCtrlRefAxisGlobal, m_aCtrlRefAxisTwoPnt;
	CArray<UINT, UINT> m_aCtrlPolAxisGlobal, m_aCtrlPolAxisTwoPnt;
	CArray<UINT, UINT> m_aCtrlButton;
	CString	m_szCmb[6];
	// Generated message map functions
	//{{AFX_MSG(CNodeRenumberDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCynAxisOrder();
	afx_msg void OnSelchangeCynOrder1st();
	afx_msg void OnSelchangeCynOrder2nd();
	afx_msg void OnSelchangeCarOrder1st();
	afx_msg void OnSelchangeCarOrder2nd();	
	afx_msg void OnTmRenumTarget();	
	afx_msg void OnSelchangeSortMethodCmb();
	afx_msg void OnTmCylinderAxisRdo();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODERENUMBERDLG_H__6B0FD341_3C4B_11D4_B00F_0000C0B9C58C__INCLUDED_)
