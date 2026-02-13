#if !defined(__DGNCONPHIDLG_H__)
#define __DGNCONPHIDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConPhiDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"

#include "HeaderPre.h"

const int iDgn_FPHIID = 12;
const int iDgn_MPHIID1 = 7;
const int iDgn_MPHIID2 = 7;
const int iDgn_MPHIID3 = 5;
const int iDgn_MPHIID4 = 5;
const int iDgn_MPHIID5 = 11;
const int iDgn_MPHIID6 = 3; // Coded by Seungjun ('20070706) MNet:No.2864.
const int iDgn_MPHIID7 = 10; // Coded by sshan ('20090602) MNet:No..
const int iDgn_MPHIID8 = 15;
const int iDgn_MPHIID9 = 5; 
const int iDgn_MPHIID10 =2;  // csa-s6-14  // Seismic check
const int iDgn_MPHIID11 =12;  // IRS CBC 1997
const int iDgn_MPHIID12 = 15; // russian : SP_63_2018

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConPhiDlg dialog

class __MY_EXT_CLASS__ CDgnConPhiDlg : public CDialogMove
{
// Construction
public:
	CDgnConPhiDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConPhiDlg)
	enum { IDD = IDD_DGN_CON_PHI_DLG };
	double	m_FPhiB;
	double	m_FPhiC1;
	double	m_FPhiC2;
	double	m_FPhiS;
	double	m_FPhiT;

	double	m_MPhiC;
	double	m_MPhiS;
	double	m_MPhiM;
	double	m_MPhiC2;
	double	m_MPhiS2;
	double	m_MPhiV2;
	double	m_MPhiC3;
	double	m_MPhiS3;
	double	m_MPhiC4;
	double	m_MPhiS4;
	double	m_MFundPhiC5;
	double	m_MAcciPhiC5;
	double	m_MFundPhiS5;
	double	m_MAcciPhiS5;
	//
	double	m_MAlphacc;

	CString m_strTitle;
	CString	m_strCode;
	CString	m_strCode2;

	double	m_07PhiT;
	double	m_07PhiC1;
	double	m_07PhiC2;
	double	m_07PhiV;	
	//
	double	m_MFundPhiC8;
	double	m_MAcciPhiC8;
	double	m_MFundPhiS8;
	double	m_MAcciPhiS8;
	double	m_MSLSPhiC8;
	double	m_MSLSPhiS8;

    double	m_MAlphacc_CVL;
    double	m_MAlphacc4Flex_CVL;
    double	m_MAlphacc4Shear_CVL;

	double	m_IRS_compr11;
	double	m_IRS_shr11;
	double	m_IRS_sls11;
	double	m_IRS_gamma_s11;
	double	m_IRS_alpha_cc11;

	BOOL m_bSeisChk;  // CSA-S6-14

	double	m_rus_sp_63_2018_g1;
	double	m_rus_sp_63_2018_g2;
	double	m_rus_sp_63_2018_g3;
	double	m_rus_sp_63_2018_g4;
	double	m_rus_sp_63_2018_g5;
	double	m_rus_sp_63_2018_gs;

	//}}AFX_DATA
	int m_iFPHIID[iDgn_FPHIID];
	int m_iMPHIID1[iDgn_MPHIID1];
	int m_iMPHIID2[iDgn_MPHIID2];
	int m_iMPHIID3[iDgn_MPHIID3];
	int m_iMPHIID4[iDgn_MPHIID4];
	int m_iMPHIID5[iDgn_MPHIID5];
	int m_iMPHIID6[iDgn_MPHIID6]; // Coded by Seungjun ('20070706) MNet:No.2864.
	int m_iMPHIID7[iDgn_MPHIID7]; // Coded by sshan ('20090602) MNet:No.. 
	int m_iMPHIID8[iDgn_MPHIID8]; //(2009.06.15) Add by Unsang :: Only EC2-2:05, ColmDgn.
	int m_iMPHIID9[iDgn_MPHIID9]; //(2009.06.15) Add by Unsang :: Only EC2-2:05, ColmDgn.
	int m_iMPHIID10[iDgn_MPHIID10]; //CSA_S6_14
	int m_iMPHIID11[iDgn_MPHIID11]; //IRS CBC 1997
	int m_iMPHIID12[iDgn_MPHIID12]; //SP_63_2018

public:
	void Initial_DefaultData();
	void Initial_ExistData();
	BOOL Set_DefaultByCode(CString strCode);
	// Add by ZINU.('01.10.24). If WSD, Disable.
	void Update_InitDataByCode(CString strCodeName);
	CRect MoveCtrl(int iKind, int* pID, CRect Rect0);
	void InitDlgCtrls();
	int GetDataStatus(CString strCode);

	//
	void SetDataIRC112(int iStatus, double* dPhi, double& dAlpha_cc);
	CString m_strNationalAnnex;

    void GetDataFromDlg(T_DCON_D& rData);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConPhiDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// 20081030 / Han Sang Soon / Tel:2167 (M-net:3780)
	void SetForShearWindowText(CString strCode);

	// Generated message map functions
	//{{AFX_MSG(CDgnConPhiDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDgnPhiUpdateBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    bool HasAlphacc4ShearCtrl(int nAnnex);
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONPHIDLG_H__B1565524_E5DB_11D3_888F_0000C0F30D4D__INCLUDED_)
