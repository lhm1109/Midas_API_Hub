#if !defined(AFX_CMMVHLITEMUSRBSDLG_H__606F8FB0_00D7_4D25_A986_136362255103__INCLUDED_)
#define AFX_CMMVHLITEMUSRBSDLG_H__606F8FB0_00D7_4D25_A986_136362255103__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemUsrBSDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrBSDlg dialog
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\TextUnit.h"
#include "..\wg_db\EditUnit.h"

class CCMMvhlItemUsrBSDlg : public CInternationalDlg
{
// Construction
public:
	CCMMvhlItemUsrBSDlg(T_MVHL_D* pData, CWnd* pParent = NULL);   // standard constructor

public:
	T_MVHL_D* m_pData;
// Dialog Data
	//{{AFX_DATA(CCMMvhlItemUsrBSDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMU_BS };
	CEditUnit m_edtNumUnit;
//  double	m_dBSW1;
//  double	m_dBSW2;
//  double	m_dBSW3;
//  double	m_dBSL1;
//  double	m_dBSL2;
//  double	m_dBSPa;
//  double	m_dBSPb;
// 	double	m_dBSD1;
// 	double	m_dBSD2;
// 	double	m_dBSD;
//  double  m_dBSD3;
// 	double	m_dBSD4;
// 	double	m_dBSD5;
// 	double	m_dBSD6;
	CEditUnit	m_edtBSW1;
	CEditUnit	m_edtBSW2;
	CEditUnit	m_edtBSW3;
	CEditUnit	m_edtBSL1;
	CEditUnit	m_edtBSL2;
	CEditUnit	m_edtBSPa;
	CEditUnit	m_edtBSPb;
	CEditUnit	m_edtBSD1;
	CEditUnit	m_edtBSD2;
	CEditUnit	m_edtBSD;
	CEditUnit m_edtBSD3;
	CEditUnit	m_edtBSD4;
	CEditUnit	m_edtBSD5;
	CEditUnit	m_edtBSD6;
	CString 	m_strBSW2_exp;
	CTextUnit	m_untBSW1;
	CTextUnit	m_untBSW2;
	CTextUnit	m_untBSW3;
	CTextUnit	m_untBSL1;
	CTextUnit	m_untBSL2;
	CTextUnit	m_untBSPa;
	CTextUnit	m_untBSPb;
	CTextUnit	m_untBSD1;
	CTextUnit	m_untBSD2;
	CTextUnit	m_untBSD;
	CTextUnit	m_untBSD3;
	CTextUnit	m_untBSD4;
	CTextUnit	m_untBSD5;
	CTextUnit	m_untBSD6;
	//}}AFX_DATA

public:
	void SetData2Dlg();
	BOOL SetDlg2Data();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemUsrBSDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	double GetExpValue(double a, double b, double l);


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemUsrBSDlg)
	virtual BOOL OnInitDialog();

	afx_msg void OnChangeUniLoadEdit();
	afx_msg void OnChangeLoadLenEdit();
	afx_msg void OnChangeLoadDistEdit();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMUSRBSDLG_H__606F8FB0_00D7_4D25_A986_136362255103__INCLUDED_)
