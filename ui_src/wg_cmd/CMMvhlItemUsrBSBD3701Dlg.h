#if !defined(AFX_CMMVHLITEMUSRBSBD3701DLG_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
#define AFX_CMMVHLITEMUSRBSBD3701DLG_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemUsrBSBD3701Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrBSBD3701Dlg dialog
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_db\TextUnit.h"
#include "..\wg_db\EditUnit.h"

struct T_MVHL_D;
class CCMMvhlItemUsrDlg;
class CCMMvhlItemUsrBSBD3701Dlg : public CInternationalDlg
{
// Construction
public:
	CCMMvhlItemUsrBSBD3701Dlg(T_MVHL_D* pData, CWnd* pParent = NULL);   // standard constructor

public:
	T_MVHL_D* m_pData;

// Dialog Data
	//{{AFX_DATA(CCMMvhlItemUsrBSBD3701Dlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMU_BS_BD3701 };
	CEditUnit m_edtNumUnit;
// 	double    m_dBSW1;
//  double    m_dBSW2;
//  double    m_dBSW3;
//  double    m_dBSL1;
//  double    m_dBSL21;
//  double    m_dBSL22;
//  double    m_dBSL3;
//  double    m_dBSPa;
//  double    m_dBSPb;
// 	double    m_dBSD1;
// 	double    m_dBSD2;
// 	double    m_dBSD;
//  double    m_dBSD3;
// 	double    m_dBSD4;
// 	double    m_dBSD5;
// 	double    m_dBSD6;
	CEditUnit m_edtBSW1;
	CEditUnit m_edtBSW2;
	CEditUnit m_edtBSW3;
	CEditUnit m_edtBSL1;
	CEditUnit m_edtBSL21;
	CEditUnit m_edtBSL22;
	CEditUnit m_edtBSL3;
	CEditUnit m_edtBSPa;
	CEditUnit m_edtBSPb;
	CEditUnit m_edtBSD1;
	CEditUnit m_edtBSD2;
	CEditUnit m_edtBSD;
	CEditUnit m_edtBSD3;
	CEditUnit m_edtBSD4;
	CEditUnit m_edtBSD5;
	CEditUnit m_edtBSD6;
//   CString   m_strBSW1_exp;
//   CString   m_strBSW2_exp;
	CEditUnit m_edtBSW1_exp;
	CEditUnit m_edtBSW2_exp;
	CEditUnit m_edtaL;
	CTextUnit	m_untBSW1;
	CTextUnit	m_untBSW2;
	CTextUnit	m_untBSW3;
	CTextUnit	m_untBSL1;
	CTextUnit	m_untBSL2;
	CTextUnit	m_untBSL3;
	CTextUnit	m_untBSPa;
	CTextUnit	m_untBSPb;
	CTextUnit	m_untBSD1;
	CTextUnit	m_untBSD2;
	CTextUnit	m_untBSD;
	CTextUnit	m_untBSD3;
	CTextUnit	m_untBSD4;
	CTextUnit	m_untBSD5;
	CTextUnit m_untBSD6;
	CTextUnit m_untaL;
	int       m_nLaneFact;
	CEditUnit m_edtLaneFact1;
	CEditUnit m_edtLaneFact2;
	CEditUnit m_edtLaneFact3;
	CEditUnit m_edtLaneFact4;
	
	CComboBox m_cbxCategory;
	CComboBox m_cbxLoadLevel;

	mit::frx::MButton   m_chkAddData;

	CFoldGroupBox_Cross	m_chkHALaneFactor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemUsrBSBD3701Dlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	BOOL SetDlg2Data();

protected:
	void InitUnit();
	void InitCbx();
	void SetEnable();
	

protected:
	double GetExpValue(double a, double b, double l);

// Implementation
protected:
	CCMMvhlItemUsrDlg* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemUsrBSBD3701Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeLoadLen1Edit();
	afx_msg void OnChangeLoadLen2Edit();
	afx_msg void OnChangeLoadDistEdit();
	afx_msg void OnChangeLaneFactorRdo();
	afx_msg void OnChkAddData();
	afx_msg void OnHALaneFactor();

	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
		//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMUSRBSBD3701DLG_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
