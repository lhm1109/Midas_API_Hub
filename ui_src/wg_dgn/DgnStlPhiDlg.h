#if !defined(__DGNSTLPHIDLG_H__)
#define __DGNSTLPHIDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlPhiDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "HeaderPre.h"

const int iDgn_STL_FPHIID = 12;
const int iDgn_STL_MPHIID1 = 7;
const int iDgn_STL_MPHIID2 = 5;
const int iDgn_STL_MPHIID3 = 3;
const int iDgn_STL_MPHIID4 = 3;// Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)
const int iDgn_STL_MPHIID5 = 11;
const int iDgn_STL_MPHIID5_EDT = 7;
const int iDgn_STL_MPHIID6 = 9;

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnStlPhiDlg dialog

class __MY_EXT_CLASS__ CDgnStlPhiDlg : public CDialogMove
{
// Construction
public:
	CDgnStlPhiDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnStlPhiDlg)
	enum { IDD = IDD_DGN_STL_PHI_DLG };
	double	m_FPhiT1;
	double	m_FPhiT2;
	double	m_FPhiC;
	double	m_FPhiB;
	double	m_FPhiS;

	double	m_MRm0;
	double	m_MRm1;
	double	m_MRm2;
	double	m_M2Rm1;
	double	m_M2Rm2;
	double	m_M3Phi;
	double	m_M4Rm;// Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)
	double    m_M5Rm[iDgn_STL_MPHIID5_EDT];

	CString m_strTitle;
	CString	m_strCode;
	int     m_nNationalAnnex;
	//}}AFX_DATA
	int m_iFPHIID[iDgn_STL_FPHIID];
	int m_iMPHIID1[iDgn_STL_MPHIID1];
	int m_iMPHIID2[iDgn_STL_MPHIID2];
	int m_iMPHIID3[iDgn_STL_MPHIID3];
	int m_iMPHIID4[iDgn_STL_MPHIID4];
	int m_iMPHIID5[iDgn_STL_MPHIID5];
	int m_iMPHIID6[iDgn_STL_MPHIID6];

	double	m_M6_Phi1; // Reliability coefficient in time-resistance calculation
	double	m_M6_Phi2; // Reliability coefficient of responsibility
	double	m_M6_Phi3; // Working conditions factor (strength)
	double	m_M6_Phi4; // Operating conditions coefficient (stability)

public:
	void Initial_DefaultData();
	void Initial_ExistData();
	BOOL Set_DefaultByCode(CString strCode);
	// Add by ZINU.('01.10.24). If WSD, Disable.
	void Update_InitDataByCode(CString strCodeName);
	CRect MoveCtrl(int iKind, int* pID);
	void InitDlgCtrls();
	int GetDataStatus(CString strCode);
	void Set_SubCodeTitle();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlPhiDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnStlPhiDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDgnPhiUpdateBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSTLPHIDLG_H__B1565524_E5DB_11D3_888F_0000C0F30D4D__INCLUDED_)
