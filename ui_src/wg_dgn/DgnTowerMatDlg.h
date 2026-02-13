#if !defined(__DGNTOWERMATDLG_H__)
#define __DGNTOWERMATDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnTowerMatDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "HeaderPre.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnTowerMatDlg dialog

class __MY_EXT_CLASS__ CDgnTowerMatDlg : public CDialogMove
{
// Construction
public:
	CDgnTowerMatDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnTowerMatDlg)
	enum { IDD = IDD_DGN_TWR_MAT_DLG };
	CTextUnit	m_Fy4Unit;
	CTextUnit	m_Fy3Unit;
	CTextUnit	m_Fy2Unit;
	CTextUnit	m_Fy1Unit;
	CTextUnit	m_FuUnit;
	CTextUnit	m_EsUnit;
	CComboBox	m_Code;
	CComboBox	m_MatName;
	CListCtrl	m_ListCtrl;
	CString	m_MatName2;
	double	m_Ps;
	double	m_Es;
	double	m_Fu;
	double	m_Fy1;
	double	m_Fy2;
	double	m_Fy3;
	double	m_Fy4;
	CTextUnit	m_BoltFy1Unit;
	CTextUnit	m_BoltFuUnit;
	CComboBox	m_BoltCode;
	CComboBox	m_BoltMatName;
	CString	m_BoltMatName2;
	double	m_BoltFu;
	double	m_BoltFy1;

	//}}AFX_DATA

public:
	void EnableEditBox(BOOL bCheck);
	void EnableBoltEditBox(BOOL bCheck);
	void ShowEditBox(int nCheck);
	void Initial_StlMatlCombo(CString strCode);
	void Initial_BoltMatlCombo(CString strCode);
	void Initial_Data();
	void Initial_Unit();
	void Initial_ListCtrl();
	void Write_ListCtrlBox(int Index, CString strID, CString strStlNa, CString strFu,
												 CString strFy1, CString strBoltNa, CString strBoltFu, CString strBoltFy1);
	CString Get_WeightUnit();
	CString Get_StressUnit();
	int GetChkKind(CString strMatlCode);

	BOOL ErrorCheck();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnTowerMatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnTowerMatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedDgnSteelmatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDgnSteelmatCode();
	afx_msg void OnSelchangeDgnBoltmatCode();
	afx_msg void OnDgnSteelmatModify();
	afx_msg void OnSelchangeDgnSteelMat();
	afx_msg void OnSelchangeDgnBoltMat();
	afx_msg void OnClickDgnSteelmatList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif // !defined(__DGNTOWERMATDLG_H__)
