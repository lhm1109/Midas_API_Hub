#if !defined(AFX_CMGRIDCOPYLOADLINEDLG_H__0E2E95A8_A354_4638_999A_F97ED7DF7220__INCLUDED_)
#define AFX_CMGRIDCOPYLOADLINEDLG_H__0E2E95A8_A354_4638_999A_F97ED7DF7220__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGridCopyLoadLineDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_GridInfo.h"
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMGridCopyLoadLineDlg dialog
#include "HeaderPre.h"
class CCMGridCopyLoadLineDlg : public CDialogMove
{
// Construction
public:
	CCMGridCopyLoadLineDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMGridCopyLoadLineDlg)
	enum { IDD = IDD_CMD_GRID_COPY_LOADLINE };
	CTextUnit	m_wndDistanceUnit;
	CTextUnit	m_wndDistanceGlobalUnit;
	CTextUnit	m_wndDistanceRefUnit;
	CEditUnit	m_wndDistance;
	CEditUnit	m_wndDistanceRef;
	CEditUnit	m_wndRatio;
	CEdit     m_wndName;
	MComboBox m_cmbCopyFrom;
	MComboBox m_cmbCopyFrom2;
	MComboBox m_cmbCopyFromRef1;
	MComboBox m_cmbCopyFromRef2;
	MComboBox m_cmbCopyType;
	int		m_nRefType;
	CEdit     m_strDXDY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGridCopyLoadLineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT, UINT> m_arGlobal;
	CArray<UINT, UINT> m_arNormal;
	CArray<UINT, UINT> m_arReference;

	void InitCombo();
	BOOL Execute();
	BOOL ExecuteGlobal();
	BOOL ExecuteNormal();
	BOOL ExecuteReference();
	void InitDlg();
	void AlignCtrl();
	BOOL GetTransData(CArray<T_NODE_D, T_NODE_D>& src, CArray<T_NODE_D, T_NODE_D>& target, double dDistance);
	void GetTransCoord(T_NODE_D node, double Direction[3], double dDistance, T_NODE_D& target);
	void GetDirVector(T_NODE_D v1, T_NODE_D v2, double dirVector[3]);
	void NewellsMethod(CArray<T_NODE_D, T_NODE_D>&src, double normal[3]);
	void GetNodeCoord(BOOL bMgdr, UINT key, CArray<T_NODE_D, T_NODE_D>& coord);
	BOOL IsSameNode(T_NODE_D& node1, T_NODE_D& node2);
	CString CreateGirderName();

	// Generated message map functions
	//{{AFX_MSG(CCMGridCopyLoadLineDlg)
	afx_msg void OnCmdOk();
	afx_msg void OnCmdApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeGridCopyType();
	afx_msg void OnCmdRefOpt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMGRIDCOPYLOADLINEDLG_H__0E2E95A8_A354_4638_999A_F97ED7DF7220__INCLUDED_)
