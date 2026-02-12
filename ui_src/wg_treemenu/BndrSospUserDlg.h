#if !defined(AFX_BNDRSOSPUSERDLG_H__7A86753E_B530_4D04_BB3A_F0D70673D5E9__INCLUDED_)
#define AFX_BNDRSOSPUSERDLG_H__7A86753E_B530_4D04_BB3A_F0D70673D5E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrSospUserDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBndrSospUserDlg dialog

#include "..\wg_base\wg_base_InternationalDlg.h"
#include "..\wg_base\MouseEdit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\DlgChild.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

struct SpringUser {
	int nNonType;
	int ElType;
	int nFace;
	double dWidth;
	double dStiff;
	int nSpringDirect;
	double dVx;
	double dVy;
	double dVz;
	BOOL bDist;
	int nGradientDirect;
	double dReference;
	double dGradientStiff;
};

class CBndrSospUserDlg : public CDlgChild
{
// Construction
public:
	CBndrSospUserDlg(SpringUser* pData, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBndrSospUserDlg)
	enum { IDD = IDD_TM_BNDR_SOSP_USER };
	CTextUnit m_wndWidthUnit;
	CTextUnit m_unitGradientStiff;
	CTextUnit m_unitStiff;
	CTextUnit m_unitReference;
	CMouseEdit m_edtReference;
	CEditUnit m_edtGradientStiff;
	CEditUnit m_wndWidth;
	CEditUnit m_edtStiff;
	MComboBox	m_CobxElemType;
	MComboBox	m_cboElemType;
	MComboBox m_cboGra;  
	MComboBox m_cboDirect;  
	MButton m_chkDist;
	CMouseEdit m_edtVector;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrSospUserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitSubCtrl();
	BOOL GetParameter();
	void CheckMode(BOOL bCheck);
	BOOL GetPosition(CString& strPos,double& Px ,double& Py ,double& Pz);
	void InitDirectionCombo();
	//void AlignControl();
protected:
	SpringUser* m_pData;
	 

	// Generated message map functions
	//{{AFX_MSG(CBndrSospUserDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTmElemTypeCombo();
	afx_msg void OnSelchangeTmDirectCombo();
	afx_msg void OnSelchangeTmBndrSospTypeSolidCbo();
	afx_msg void OnTmBndrSospDistChk();
	afx_msg void OnChangeGradDir();
	afx_msg void OnChangeType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDRSOSPUSERDLG_H__7A86753E_B530_4D04_BB3A_F0D70673D5E9__INCLUDED_)
