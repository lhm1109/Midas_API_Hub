#if !defined(__DgnSodMatDlg_H__)
#define __DgnSodMatDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSodMatDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

#include "HeaderPre.h"

using namespace mit::frx;
class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnSodMatDlg dialog

class __MY_EXT_CLASS__ CDgnSodMatDlg : public CDialogMove
{
// Construction
public:
	CDgnSodMatDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnSodMatDlg)
	enum { IDD = IDD_DGN_SOD_MAT_DLG };
	CTextUnit	m_Fy10Unit;
	CTextUnit	m_Fy9Unit;
	CTextUnit	m_Fy8Unit;
	CTextUnit	m_Fy7Unit;
	CTextUnit	m_Fy6Unit;
	CTextUnit	m_Fy5Unit;
	CTextUnit	m_Fy4Unit;
	CTextUnit	m_Fy3Unit;
	CTextUnit	m_Fy2Unit;
	CTextUnit	m_Fy1Unit;
	CTextUnit	m_FuUnit;
	CTextUnit	m_EsUnit;
	MComboBox	m_Code;
	MComboBox	m_MatName;
	CListCtrl	m_ListCtrl;
	CString	m_MatName2;
	double	m_Ps;
	double	m_Es;
	double	m_Fu;
	double	m_Fy1;
	double	m_Fy2;
	double	m_Fy3;
	double	m_Fy4;
	double	m_Fy5;
	double	m_Fy6;
	double	m_Fy7;
	double	m_Fy8;
	double	m_Fy9;
	double	m_Fy10;
	double  m_Gamma_m;

	//}}AFX_DATA

public:
	void EnableEditBox(BOOL bCheck);
	void ShowEditBox(int nCheck);
	void Initial_MaterialCombo(CString strCode, CString strName);
	void Initial_Data();
	void Initial_Unit();
	void Initial_ListCtrl();
	void Write_ListCtrlBox(int Index, CString strID, CString strName, CString strEs, CString strFu, CString strFy1, CString strFy2, CString strFy3, CString strFy4, CString strFy5, CString strFy6, CString strFy7, CString strFy8, CString strFy9, CString strFy10);

	CString Get_WeightUnit();
	CString Get_StressUnit();
	int GetChkKind(CString strMatlCode);

	BOOL ErrorCheck();

	void ControlMan(BOOL bInit);

// 	int GetControlType();
	CString GetDesignCodeName();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSodMatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;  
	CArray<UINT, UINT> m_arCtrlCType;
	int m_nOldItem;

	double Change_Flen(int iLenID);

	// Generated message map functions
	//{{AFX_MSG(CDgnSodMatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedDgnSteelmatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDgnSteelmatCode();
	afx_msg void OnSelchangeDgnSteelMat();
	afx_msg void OnDgnSteelmatModify();
	afx_msg void OnClickDgnSteelmatList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
//  afx_msg void OnSelchangeDgnSteelKSCE05Name();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif 
