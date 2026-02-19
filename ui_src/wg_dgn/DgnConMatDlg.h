#if !defined(__DGNCONMATDLG_H__)
#define __DGNCONMATDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConMatDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MComboBox.h"

#include "HeaderPre.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConMatDlg dialog

class __MY_EXT_CLASS__ CDgnConMatDlg : public CDialogMove
{
// Construction
public:
	CDgnConMatDlg(BOOL bPscDgn=FALSE, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConMatDlg)
	enum { IDD = IDD_DGN_CON_MAT_DLG };
	mit::frx::MComboBox	m_RebarCode;
	mit::frx::MComboBox	m_SubBarName;
	mit::frx::MComboBox	m_MainBarName;
	mit::frx::MComboBox	m_Code;
	mit::frx::MComboBox	m_MatName;
	CTextUnit	m_FyUnit;
	CTextUnit	m_FysUnit;
	CTextUnit	m_FcUnit;
	CTextUnit	m_TransferUnit;
	BOOL      m_bChkTransfer;
	BOOL      m_bLambda;
	BOOL      m_bedtLambda =TRUE;
	CEditUnit m_edtLambda;
	CEditUnit m_edtTransfer;
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

	void AlignControl();
	void Init_GridData();
	void Init_Unit();
	void Init_MatlCode();
	void Init_RebarCombo(CString strRebarCode, T_MATD_D mData);
	void Init_MatlNameCombo(CString strCode, CString strNa);
	void ChangeDlgItemInfo();
	void EnableEditBox(int nCheck);
	void EnableRebarEditBox(int nCheck);
	void Write_ListBox(int iIndex, CString strID, CString strNa, CString strFc, CString strChk, CString strLambda, CString strMbar,CString strSbar);
	void ControlsEnableDisable();

	// Coded by Seungjun MNet:No.2393 ('20061110)
	void Init_Items();

private:
	BOOL IsEnableLambdaCode_Civil();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConMatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CString m_strCurrentItem;
	BOOL m_bTransfer;

	// Generated message map functions
	//{{AFX_MSG(CDgnConMatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickDgnConcmatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedDgnConcmatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDgnConcMat();
	afx_msg void OnSelchangeDgnConcmatCode();
	afx_msg void OnSelchangeDgnConcMainname();
	afx_msg void OnSelchangeDgnConcSubname();
	afx_msg void OnClickLambdaChk();
	afx_msg void OnClickTransferChk();
	afx_msg void OnDgnConcmatModify();
	afx_msg void OnDgnConcmatClose();
	afx_msg void OnSelchangeDgnConcRebarcode();
	afx_msg void OnDgnConcElasticity();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONMATDLG_H__4B768343_DD95_11D3_888F_0000C0F30D4D__INCLUDED_)
