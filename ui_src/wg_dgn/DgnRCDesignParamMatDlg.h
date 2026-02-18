#if !defined(AFX_DGNRCDESIGNPARAMMATDLG_H__B035925A_3084_4F4F_A822_5AE9CE81F83E__INCLUDED_)
#define AFX_DGNRCDESIGNPARAMMATDLG_H__B035925A_3084_4F4F_A822_5AE9CE81F83E__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRCDesignParamMatDlg.h : header file
//
#include "..\wg_base\ChildDialog.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_TextUnit.h"

const int CONST_DGN_iGRIDCOLM = 5;

class CDBDoc;
class DgnRCDesignParamBaseDlg;

#include "HeaderPre.h"      
/////////////////////////////////////////////////////////////////////////////
// DgnRCDesignParamMatDlg dialog

class __MY_EXT_CLASS__ DgnRCDesignParamMatDlg : public CChildDialog
{
// Construction
public:
	DgnRCDesignParamMatDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DgnRCDesignParamMatDlg)
	enum { IDD = IDD_DGN_RC_DESIGN_PARAM_MAT };
		// NOTE: the ClassWizard will add data members here
	CComboBox	m_RebarCode;
	CComboBox	m_SubBarName;
	CComboBox	m_MainBarName;
	CComboBox	m_Code;
	CComboBox	m_MatName;
	CTextUnit	m_FyUnit;
	CTextUnit	m_FysUnit;
	CTextUnit	m_FcUnit;
	CListCtrl	m_ListCtrl;
	double	m_Fc;
	CString	m_MatName2;
	CString	m_SubBarName2;
	CString	m_MainBarName2;
	double	m_Fy;
	double	m_Fys;
	//}}AFX_DATA

public:
	BOOL ErrorCheck();
	void Init_GridData();
	void Init_Unit();
	void Init_MatlCode();
	void Init_RebarCombo(CString strRebarCode, T_MATD_D mData);
	void Init_MatlNameCombo(CString strCode, CString strNa);
	void ChangeDlgItemInfo();
	void EnableEditBox(int nCheck);
	void EnableRebarEditBox(int nCheck);
	void Write_ListBox(int iIndex, CString strID, CString strNa, CString strFc, CString strMbar,CString strSbar);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DgnRCDesignParamMatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	DgnRCDesignParamBaseDlg* m_pParent;
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(DgnRCDesignParamMatDlg)
		// NOTE: the ClassWizard will add member functions here

	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClickDgnConcmatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedDgnConcmatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDgnConcMat();
	afx_msg void OnSelchangeDgnConcmatCode();
	afx_msg void OnSelchangeDgnConcMainname();
	afx_msg void OnSelchangeDgnConcSubname();
	afx_msg void OnDgnConcmatModify();
	afx_msg void OnSelchangeDgnConcRebarcode();

	//}}AFX_MSG
public:
	void SaveOrUpdate();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////
#include "HeaderPost.h"     /////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNRCDESIGNPARAMMATDLG_H__B035925A_3084_4F4F_A822_5AE9CE81F83E__INCLUDED_)
