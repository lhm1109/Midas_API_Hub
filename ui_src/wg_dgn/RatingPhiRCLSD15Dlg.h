#if !defined(__RATINGPHIRCLSD15DLG_H__)
#define __RATINGPHIRCLSD15DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 //DgnConPhiDlg.h : header file

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConPhiDlg dialog

class __MY_EXT_CLASS__ CRatingPhiRCLSD15Dlg : public CDialogMove
{
	// Construction
public:
	CRatingPhiRCLSD15Dlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDgnConPhiDlg)
	enum { IDD = IDD_DGN_RATING_RC_CODE_MAT_RF_LSD_DLG };
	
	CEditUnit	m_dConPhi_U;
	CEditUnit	m_dConPhi_E;
	CEditUnit	m_dConPhi_S;
	CEditUnit	m_dRebarPhi_U;
	CEditUnit	m_dRebarPhi_E;
	CEditUnit	m_dRebarPhi_S;
	MButton		m_bAlphacc;
	CEditUnit	m_dAlphacc_CVL;

	void Dlg2Data();

public:
	void Initial_Data();
	//
	T_RPAC_D m_Data;
	CDBDoc* m_pDoc;
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConPhiDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:


	// 20081030 / Han Sang Soon / Tel:2167 (M-net:3780)
	void SetForShearWindowText(CString strCode);

	// Generated message map functions
	//{{AFX_MSG(CDgnConPhiDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDgnPhiUpdateBtn();
	afx_msg void OnCheckAlphaCCData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//
#endif // !defined(AFX_DGNCONPHIDLG_H__B1565524_E5DB_11D3_888F_0000C0F30D4D__INCLUDED_)
