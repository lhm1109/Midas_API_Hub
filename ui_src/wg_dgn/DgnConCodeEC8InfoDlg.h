#if !defined(__DGNCONCODEEC8INFODLG_H__)
#define __DGNCONCODEEC8INFODLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConCodeEC8InfoDlg.h : header file
//

#include "..\mit_frx\MComboBox.h"

#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "HeaderPre.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeEC8InfoDlg dialog

class __MY_EXT_CLASS__ CDgnConCodeEC8InfoDlg : public CDialogMove
{
// Construction
public:
	CDgnConCodeEC8InfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConCodeEC8InfoDlg)
	enum { IDD = IDD_DGN_CON_CODE_EC8_INFO_DLG };
	mit::frx::MComboBox	m_comboFrameType;
	CEditUnit m_editAua1;
	mit::frx::MComboBox	m_comboFunction;
	CEditUnit m_editSf;
	CEditUnit m_editTb;
	CEditUnit m_editTc;
	CEditUnit m_editTd;
	CEditUnit m_editAgR;
	CEditUnit m_editI;
	CEditUnit m_editVdr;
	CEditUnit m_editUserInputQ;
	CEditUnit m_editUserInputQO;
	CEditUnit m_editT1_X;
	CEditUnit m_editT1_Y;
	//}}AFX_DATA

public:
	BOOL ErrorCheck();

	void InitUnit();
	void InitFrameCombo();
	void InitFunctionCombo();

	void SetDconData(T_DCON_D* pDconData);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConCodeEC8InfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	T_DCON_D* m_pDconData;

	CArray<T_SPFC_K, T_SPFC_K> m_aOKSpfcK;
	CArray<UINT,UINT> m_aBehaviorFactRdo;
	CArray<UINT,UINT> m_aBehaviorT1Rdo;
	CArray<UINT,UINT> m_aT1User;

	void Data2Dlg();
	BOOL Dlg2Data();

	// Generated message map functions
	//{{AFX_MSG(CDgnConCodeEC8InfoDlg)
	virtual BOOL OnInitDialog();	
	afx_msg void OnSelchangeFumctionCombo();
	afx_msg void OnSelchangeFrameTypeCombo();
	afx_msg void OnBehaviorFactorRdo();
	afx_msg void OnBehaviorT1Rdo();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNCONCODEEC8INFODLG_H__)
