#if !defined(__DGNCFPHIDLG_H__)
#define __DGNCFPHIDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnCfPhiDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "HeaderPre.h"

const int iDgn_CFS_FPHIID = 12;
const int iDgn_CFS_MPHIID1 = 7;
const int iDgn_CFS_MPHIID2 = 5;
const int iDgn_CFS_MPHIID3 = 3;
const int iDgn_CFS_MPHIID4 = 3;// Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnCfPhiDlg dialog

class __MY_EXT_CLASS__ CDgnCfPhiDlg : public CDialogMove
{
// Construction
public:
	CDgnCfPhiDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnCfPhiDlg)
	enum { IDD = IDD_DGN_CF_PHI_DLG };
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
	double	m_M4Rm;

	CString m_strTitle;
	CString	m_strCode;
	int     m_nNationalAnnex;
	//}}AFX_DATA
	int m_iFPHIID[iDgn_CFS_FPHIID];
	int m_iMPHIID1[iDgn_CFS_MPHIID1];
	int m_iMPHIID2[iDgn_CFS_MPHIID2];
	int m_iMPHIID3[iDgn_CFS_MPHIID3];
	int m_iMPHIID4[iDgn_CFS_MPHIID4];

public:
	void Initial_DefaultData();
	void Initial_ExistData();
	BOOL Set_DefaultByCode(CString strCode);
	void Update_InitDataByCode(CString strCodeName);
	CRect MoveCtrl(int iKind, int* pID);
	void InitDlgCtrls();
	int GetDataStatus(CString strCode);
	void Set_SubCodeTitle();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnCfPhiDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnCfPhiDlg)
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

#endif // !defined(AFX_DGNCFPHIDLG_H__B1565524_E5DB_11D3_888F_0000C0F30D4D__INCLUDED_)
