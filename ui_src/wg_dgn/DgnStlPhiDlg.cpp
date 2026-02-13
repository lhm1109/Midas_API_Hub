// DgnStlPhiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlPhiDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_CON_DCON_SRED
#include "DgnDataCtrl.h"
#include "NationalAnnexTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlPhiDlg dialog


CDgnStlPhiDlg::CDgnStlPhiDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnStlPhiDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlPhiDlg)
	m_FPhiT1=0.0;
	m_FPhiT2=0.0;
	m_FPhiC=0.0;
	m_FPhiB=0.0;
	m_FPhiS=0.0;

	m_MRm0=0.0;
	m_MRm1=0.0;
	m_MRm2=0.0;
	m_M2Rm1=0.0;
	m_M2Rm2=0.0;
	m_M3Phi=0.0;
	m_M4Rm=0.0;// Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)

	m_M6_Phi1 = 0.0;
	m_M6_Phi2 = 0.0;
	m_M6_Phi3 = 0.0;
	m_M6_Phi4 = 0.0;

	for(int i = 0; i < iDgn_STL_MPHIID5_EDT; ++i){m_M5Rm[i] = 0.0;}

	m_strTitle = _T("");
	m_strCode = _T("");
	//}}AFX_DATA_INIT
}

void CDgnStlPhiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlPhiDlg)
	DDX_Text(pDX, IDC_DGN_STL_F_PHIT1, m_FPhiT1);
	DDX_Text(pDX, IDC_DGN_STL_F_PHIT2, m_FPhiT2);
	DDX_Text(pDX, IDC_DGN_STL_F_PHIC, m_FPhiC);
	DDX_Text(pDX, IDC_DGN_STL_F_PHIB, m_FPhiB);
	DDX_Text(pDX, IDC_DGN_STL_F_PHIS, m_FPhiS);

	DDX_Text(pDX, IDC_DGN_STL_M_RM0, m_MRm0);
	DDX_Text(pDX, IDC_DGN_STL_M_RM1, m_MRm1);
	DDX_Text(pDX, IDC_DGN_STL_M_RM2, m_MRm2);
	DDX_Text(pDX, IDC_DGN_STL_M2_RM1, m_M2Rm1);
	DDX_Text(pDX, IDC_DGN_STL_M2_RM2, m_M2Rm2);
	DDX_Text(pDX, IDC_DGN_STL_M3_PHI, m_M3Phi);
	DDX_Text(pDX, IDC_DGN_STL_M4_RM,  m_M4Rm);// Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)

	DDX_Text(pDX, IDC_DGN_STL_M5_FLX_EDT,	m_M5Rm[0]);
	DDX_Text(pDX, IDC_DGN_STL_M5_SHR_EDT,	m_M5Rm[1]);
	DDX_Text(pDX, IDC_DGN_STL_M5_COM_EDT,	m_M5Rm[2]);
	DDX_Text(pDX, IDC_DGN_STL_M5_TENR_EDT,  m_M5Rm[3]);
	DDX_Text(pDX, IDC_DGN_STL_M5_TENY_EDT,  m_M5Rm[4]);

	DDX_Text(pDX, IDC_DGN_STL_M6_EDT1, m_M6_Phi1);
	DDX_Text(pDX, IDC_DGN_STL_M6_EDT2, m_M6_Phi2);
	DDX_Text(pDX, IDC_DGN_STL_M6_EDT3, m_M6_Phi3);
	DDX_Text(pDX, IDC_DGN_STL_M6_EDT4, m_M6_Phi4);

	//DDX_Text(pDX, IDC_DGN_STATIC_RCCODE,  m_strCode);
	//}}AFX_DATA_MAP

	// Strength Reduction Factors 
	m_iFPHIID[0] = IDC_DGN_STL_F_FRAME;
	m_iFPHIID[1] = IDC_DGN_STL_F_PHIT_STR;
	m_iFPHIID[2] = IDC_DGN_STL_F_PHIT1_STR;
	m_iFPHIID[3] = IDC_DGN_STL_F_PHIT2_STR;
	m_iFPHIID[4] = IDC_DGN_STL_F_PHIC_STR;
	m_iFPHIID[5] = IDC_DGN_STL_F_PHIB_STR;
	m_iFPHIID[6] = IDC_DGN_STL_F_PHIS_STR;	
	m_iFPHIID[7] = IDC_DGN_STL_F_PHIT1;
	m_iFPHIID[8] = IDC_DGN_STL_F_PHIT2;
	m_iFPHIID[9] = IDC_DGN_STL_F_PHIC;
	m_iFPHIID[10] = IDC_DGN_STL_F_PHIB;
	m_iFPHIID[11] = IDC_DGN_STL_F_PHIS;

	//Partial Safety Factors
	m_iMPHIID1[0] = IDC_DGN_STL_M_FRAME;
	m_iMPHIID1[1] = IDC_DGN_STL_M_RM0_STR;
	m_iMPHIID1[2] = IDC_DGN_STL_M_RM1_STR;
	m_iMPHIID1[3] = IDC_DGN_STL_M_RM2_STR;	
	m_iMPHIID1[4] = IDC_DGN_STL_M_RM0;
	m_iMPHIID1[5] = IDC_DGN_STL_M_RM1;
	m_iMPHIID1[6] = IDC_DGN_STL_M_RM2;

	//Partial Safety Factors
	m_iMPHIID2[0] = IDC_DGN_STL_M2_FRAME;
	m_iMPHIID2[1] = IDC_DGN_STL_M2_RM1_STR;
	m_iMPHIID2[2] = IDC_DGN_STL_M2_RM2_STR;	
	m_iMPHIID2[3] = IDC_DGN_STL_M2_RM1;
	m_iMPHIID2[4] = IDC_DGN_STL_M2_RM2;

	//Safety Factor for Capacity of  Materials
	m_iMPHIID3[0] = IDC_DGN_STL_M3_FRAME;
	m_iMPHIID3[1] = IDC_DGN_STL_M3_PHI_STR;	
	m_iMPHIID3[2] = IDC_DGN_STL_M3_PHI;

	// Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)
	//Partial Safety Factors
	m_iMPHIID4[0] = IDC_DGN_STL_M4_FRAME;
	m_iMPHIID4[1] = IDC_DGN_STL_M4_RM_STR;	
	m_iMPHIID4[2] = IDC_DGN_STL_M4_RM;

	// Strength Reduction Factor KSCE LSD 15
	m_iMPHIID5[0]		= IDC_DGN_STL_M5_FRAME;
	m_iMPHIID5[1]		= IDC_DGN_STL_M5_FLX_STC;
	m_iMPHIID5[2]		= IDC_DGN_STL_M5_SHR_STC;
	m_iMPHIID5[3]		= IDC_DGN_STL_M5_COM_STC;
	m_iMPHIID5[4]		= IDC_DGN_STL_M5_TENR_STC;
	m_iMPHIID5[5]		= IDC_DGN_STL_M5_TENY_STC;
	m_iMPHIID5[6]		= IDC_DGN_STL_M5_FLX_EDT;
	m_iMPHIID5[7]		= IDC_DGN_STL_M5_SHR_EDT;
	m_iMPHIID5[8]	    = IDC_DGN_STL_M5_COM_EDT;
	m_iMPHIID5[9]	    = IDC_DGN_STL_M5_TENR_EDT;
	m_iMPHIID5[10]    = IDC_DGN_STL_M5_TENY_EDT;

	// Partial Safety Factors SP 16.13330.2017
	m_iMPHIID6[0] = IDC_DGN_STL_M6_FRAME;
	m_iMPHIID6[1] = IDC_DGN_STL_M6_STC1;
	m_iMPHIID6[2] = IDC_DGN_STL_M6_STC2;
	m_iMPHIID6[3] = IDC_DGN_STL_M6_STC3;
	m_iMPHIID6[4] = IDC_DGN_STL_M6_STC4;
	m_iMPHIID6[5] = IDC_DGN_STL_M6_EDT1;
	m_iMPHIID6[6] = IDC_DGN_STL_M6_EDT2;
	m_iMPHIID6[7] = IDC_DGN_STL_M6_EDT3;
	m_iMPHIID6[8] = IDC_DGN_STL_M6_EDT4;
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnStlPhiDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnStlPhiDlg)
	ON_BN_CLICKED(IDC_DGN_PHI_UPDATE_BTN, OnDgnPhiUpdateBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlPhiDlg message handlers

BOOL CDgnStlPhiDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	// TODO: Add extra initialization here
	if(m_pDoc->m_pAttrCtrl->ExistDstl())  Initial_ExistData();
	else                                  Initial_DefaultData();
	// Add by ZINU.('01.10.24). If WSD, Disable.
	Update_InitDataByCode(m_strCode);
	
	return TRUE;
}

BOOL CDgnStlPhiDlg::Set_DefaultByCode(CString strCode)
{
	if(strCode==_T(""))	return FALSE;
	CDgnDataCtrl DataCtrl;
	double dPhi[7];
	if(!DataCtrl.Get_DgnStlPhiDef(strCode, dPhi, m_nNationalAnnex))	ASSERT(0);

	int iStatus = GetDataStatus(m_strCode);
	switch(iStatus)
	{
	case 1:
	  {
		  m_FPhiT1  = dPhi[0];
		  m_FPhiT2  = dPhi[1];
		  m_FPhiC   = dPhi[2];
		  m_FPhiB   = dPhi[3];
		  m_FPhiS   = dPhi[4];
		  break;
	  }
	case 2:
	  {
		  m_MRm0 = dPhi[0];
		  m_MRm1 = dPhi[1];
		  m_MRm2 = dPhi[2];
		  break;
	  }
	case 3:
	  {
		  m_M2Rm1 = dPhi[0];
		  m_M2Rm2 = dPhi[1];
		  break;
	  }
	case 4:
	  {
		  m_M3Phi = dPhi[0];
		  break;
	  }
	case 5: // Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)
	  {
		 m_M4Rm = dPhi[0];
		  break;
	  }
	case 6:
	  {
		  m_M5Rm[0] = dPhi[0];
		  m_M5Rm[1] = dPhi[1];
		  m_M5Rm[2] = dPhi[2];
		  m_M5Rm[3] = dPhi[3];
		  m_M5Rm[4] = dPhi[4];
		  m_M5Rm[5] = dPhi[5];
		  m_M5Rm[6] = dPhi[6];
		  break;
	  }
	case 7:
		{
			m_M6_Phi1 = dPhi[0];
			m_M6_Phi2 = dPhi[1];
			m_M6_Phi3 = dPhi[2];
			m_M6_Phi4 = dPhi[3];
			break;
		}
	default:
	  {ASSERT(0); break;}
	}

	return TRUE;
}

void CDgnStlPhiDlg::Initial_DefaultData()
{
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);
	m_strCode = rPref.DgnCode.SteelCode;
	m_nNationalAnnex = rPref.DgnCode.nStlNationalAnnex;
	if(!Set_DefaultByCode(m_strCode))	ASSERT(0);
	UpdateData(FALSE);

	InitDlgCtrls();
}

void CDgnStlPhiDlg::Initial_ExistData()
{
	T_DSTL_D rData;
	rData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDstl(rData);
	if(rData.DesignCode==_T(""))
	{
		T_PREFERENCE rPref;
		rPref.Initialize();
		m_pDoc->m_pInitCtrl->GetPreference(rPref);
		m_strCode = rPref.DgnCode.SteelCode;
		if(!Set_DefaultByCode(m_strCode))	ASSERT(0);
	}
	else
	{
        m_strCode = rData.DesignCode;
        m_nNationalAnnex = rData.nNationalAnnex;
        CDgnDataCtrl DataCtrl;
        double dPhi[7];
        if ( !DataCtrl.Get_DgnStlPhiDef(m_strCode, dPhi, m_nNationalAnnex) )	ASSERT(0);
        int iStatus = GetDataStatus(m_strCode);
        if ( iStatus==1 )
        {
            m_FPhiT1  = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
            m_FPhiT2  = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
            m_FPhiC   = (rData.dPhi[2] == 0.0 ? dPhi[2] : rData.dPhi[2]);
            m_FPhiB   = (rData.dPhi[3] == 0.0 ? dPhi[3] : rData.dPhi[3]);
            m_FPhiS   = (rData.dPhi[4] == 0.0 ? dPhi[4] : rData.dPhi[4]);
        }
        else if ( iStatus==2 )
        {
            m_MRm0 = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
            m_MRm1 = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
            m_MRm2 = (rData.dPhi[2] == 0.0 ? dPhi[2] : rData.dPhi[2]);

        }
        else if ( iStatus==3 )
        {
            m_M2Rm1 = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
            m_M2Rm2 = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
        }
        else if ( iStatus==4 )
        {
            m_M3Phi = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
        }
        else if ( iStatus==5 )	// Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)
        {
            m_M4Rm = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
        }
        else if ( iStatus==6 )
        {
            for ( int i = 0; i < 5; ++i )
            {
                m_M5Rm[i] = (rData.dPhi[i] == 0.0 ? dPhi[i] : rData.dPhi[i]);
            }
        }
		else if ( iStatus == 7 )
		{
			m_M6_Phi1 = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
			m_M6_Phi2 = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
			m_M6_Phi3 = (rData.dPhi[2] == 0.0 ? dPhi[2] : rData.dPhi[2]);
			m_M6_Phi4 = (rData.dPhi[3] == 0.0 ? dPhi[3] : rData.dPhi[3]);
		}
        else	ASSERT(0);
	}
	UpdateData(FALSE);

	InitDlgCtrls();
}

void CDgnStlPhiDlg::Update_InitDataByCode(CString strCodeName)
{
	CString strTitle=_T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleStl(ID_DGN_STL_DSTL_SRED, m_strTitle, bShow);

	SetWindowText(m_strTitle);

	int iStatus = GetDataStatus(m_strCode);
	if(iStatus==1)			GetDlgItem(IDC_DGN_STL_F_FRAME)->SetWindowText(m_strTitle);
	else if(iStatus==2)	GetDlgItem(IDC_DGN_STL_M_FRAME)->SetWindowText(m_strTitle);
	else if(iStatus==3)	GetDlgItem(IDC_DGN_STL_M2_FRAME)->SetWindowText(m_strTitle);
	else if(iStatus==4)	GetDlgItem(IDC_DGN_STL_M3_FRAME)->SetWindowText(m_strTitle);
	else if(iStatus==5)	GetDlgItem(IDC_DGN_STL_M4_FRAME)->SetWindowText(m_strTitle);// Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)
	else if(iStatus==6)	GetDlgItem(IDC_DGN_STL_M5_FRAME)->SetWindowText(m_strTitle);
	else if(iStatus==7)	GetDlgItem(IDC_DGN_STL_M6_FRAME)->SetWindowText(m_strTitle);
	else	ASSERT(0);

	if(bShow)
	{
		GetDlgItem(IDC_DGN_STL_F_PHIT1 )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_F_PHIT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_F_PHIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_F_PHIB)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_F_PHIS )->EnableWindow(TRUE);

		GetDlgItem(IDC_DGN_STL_M_RM0 )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_M_RM1 )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_M_RM2 )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_M2_RM1 )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_M2_RM2 )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_M3_PHI )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_M4_RM )->EnableWindow(TRUE);// Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)

		GetDlgItem(IDC_DGN_STL_M5_FLX_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_M5_SHR_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_M5_COM_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_M5_TENR_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_M5_TENY_EDT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_STL_F_PHIT1 )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_F_PHIT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_F_PHIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_F_PHIB)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_F_PHIS )->EnableWindow(FALSE);

		GetDlgItem(IDC_DGN_STL_M_RM0 )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_M_RM1 )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_M_RM2 )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_M2_RM1 )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_M2_RM2 )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_M3_PHI )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_M4_RM )->EnableWindow(FALSE);// Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)
		
		GetDlgItem(IDC_DGN_STL_M5_FLX_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_M5_SHR_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_M5_COM_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_M5_TENR_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_M5_TENY_EDT)->EnableWindow(FALSE);
	}

	if(m_strCode==_T("IS:800-2007") || m_strCode == _T("IRC:24-2010"))
	{
		GetDlgItem(IDC_DGN_STL_M2_RM1_STR)->SetWindowText(_LS(IDS_DGN_STL_PHI_IS_RM1));
		GetDlgItem(IDC_DGN_STL_M2_RM2_STR)->SetWindowText(_LS(IDS_DGN_STL_PHI_IS_RM2));
	}
	else if(m_strCode == _T("AISC(13th)-ASD05"))
	{
		GetDlgItem(IDC_DGN_STL_F_PHIT1_STR)->SetWindowText(_LS(IDS_DGN_STL_OMEGA_T1));
		GetDlgItem(IDC_DGN_STL_F_PHIT2_STR)->SetWindowText(_LS(IDS_DGN_STL_OMEGA_T2));
		GetDlgItem(IDC_DGN_STL_F_PHIC_STR)->SetWindowText(_LS(IDS_DGN_STL_OMEGA_C));
		GetDlgItem(IDC_DGN_STL_F_PHIB_STR)->SetWindowText(_LS(IDS_DGN_STL_OMEGA_B));
		GetDlgItem(IDC_DGN_STL_F_PHIS_STR)->SetWindowText(_LS(IDS_DGN_STL_OMEGA_V));
	}
	else
	{
		GetDlgItem(IDC_DGN_STL_M2_RM1_STR)->SetWindowText(_LS(IDS_DGN_STL_PHI_EC_RM1));
		GetDlgItem(IDC_DGN_STL_M2_RM2_STR)->SetWindowText(_LS(IDS_DGN_STL_PHI_EC_RM2));
	}

	if(m_strCode == _T("Eurocode3:05"))
	{
			GetDlgItem(IDC_DGN_STL_M_RM0_STR)->SetWindowText(_LS(IDS_DGN_STL_EC3_05_RM0));
			GetDlgItem(IDC_DGN_STL_M_RM1_STR)->SetWindowText(_LS(IDS_DGN_STL_EC3_05_RM1));
			GetDlgItem(IDC_DGN_STL_M_RM2_STR)->SetWindowText(_LS(IDS_DGN_STL_EC3_05_RM2));  
	}
	else
	{
			GetDlgItem(IDC_DGN_STL_M_RM0_STR)->SetWindowText(_LS(IDS_DGN_STL_EC3_93_RM0));
			GetDlgItem(IDC_DGN_STL_M_RM1_STR)->SetWindowText(_LS(IDS_DGN_STL_EC3_93_RM1));
			GetDlgItem(IDC_DGN_STL_M_RM2_STR)->SetWindowText(_LS(IDS_DGN_STL_EC3_93_RM2));
	}

	Set_SubCodeTitle();
}

void CDgnStlPhiDlg::Set_SubCodeTitle()
{
	BOOL bShowSubCode = (m_strCode == _T("Eurocode3:05") || m_strCode == _T("Eurocode3-2:05")) ? TRUE : FALSE;
	
	CString strCodeName = m_strCode;
	if (bShowSubCode)
	{
		CString strSubCode = CNationalAnnexTool::Get_Alpha_3_code(m_nNationalAnnex);
		if(!strSubCode.IsEmpty())
		{
			strCodeName.Format(_T("%s[%s]"), strCodeName, strSubCode);
		}
	}
	GetDlgItem(IDC_DGN_STATIC_RCCODE)->SetWindowText(strCodeName);
}

void CDgnStlPhiDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CDgnStlPhiDlg::OnOK() 
{
	UpdateData(TRUE);

	T_DSTL_D rData;
	rData.Initialize();
	if(m_pDoc->m_pAttrCtrl->ExistDstl())	m_pDoc->m_pAttrCtrl->GetDstl(rData);
	else
	{
		rData.DesignCode = m_strCode;
		rData.nNationalAnnex = m_nNationalAnnex;
	}

	int iStatus = GetDataStatus(m_strCode);
	if(iStatus==1)			
	{
	  rData.dPhi[0] = m_FPhiT1;
	  rData.dPhi[1] = m_FPhiT2;
	  rData.dPhi[2] = m_FPhiC;
	  rData.dPhi[3] = m_FPhiB;
	  rData.dPhi[4] = m_FPhiS;
	}
	else if(iStatus==2)	
	{
	  rData.dPhi[0] = m_MRm0;
	  rData.dPhi[1] = m_MRm1;
	  rData.dPhi[2] = m_MRm2;
	}
	else if(iStatus==3)	
	{
	  rData.dPhi[0] = m_M2Rm1;
	  rData.dPhi[1] = m_M2Rm2;
	}
	else if(iStatus==4)	
	{
	  rData.dPhi[0] = m_M3Phi;
	}
	else if(iStatus==5)	// Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)
	{
	  rData.dPhi[0] = m_M4Rm;
	}
	else if(iStatus==6)	
	{
	  rData.dPhi[0] = m_M5Rm[0];
	  rData.dPhi[1] = m_M5Rm[1];
	  rData.dPhi[2] = m_M5Rm[2];
	  rData.dPhi[3] = m_M5Rm[3];
	  rData.dPhi[4] = m_M5Rm[4];
	}
	else if(iStatus==7)
	{
		rData.dPhi[0] = m_M6_Phi1;
		rData.dPhi[1] = m_M6_Phi2;
		rData.dPhi[2] = m_M6_Phi3;
		rData.dPhi[3] = m_M6_Phi4;
	}
	else	ASSERT(0);

	// Save Steel Control Data at DB.
	BOOL bCheck = m_pDoc->m_pDataCtrl->AddDstl(rData);
	if(bCheck)  CDialogMove::OnOK();
}

void CDgnStlPhiDlg::OnDgnPhiUpdateBtn() 
{
	Set_DefaultByCode(m_strCode);
	UpdateData(FALSE);	
	Set_SubCodeTitle();
}

void CDgnStlPhiDlg::InitDlgCtrls() 
{
	// TODO: Add your control notification handler code here
	// Change by ZINU.('02.7.8). For Dynamic Size.
	UpdateData(TRUE);
	int i=0;
	for(i=0; i<iDgn_STL_FPHIID; i++)	GetDlgItem(m_iFPHIID[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_STL_MPHIID1; i++)	GetDlgItem(m_iMPHIID1[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_STL_MPHIID2; i++)	GetDlgItem(m_iMPHIID2[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_STL_MPHIID3; i++)	GetDlgItem(m_iMPHIID3[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_STL_MPHIID4; i++)	GetDlgItem(m_iMPHIID4[i])->ShowWindow(SW_HIDE);// Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)
	for(i=0; i<iDgn_STL_MPHIID5; i++)	GetDlgItem(m_iMPHIID5[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_STL_MPHIID6; i++)	GetDlgItem(m_iMPHIID6[i])->ShowWindow(SW_HIDE);

	CRect RectDlg, RectOk, RectCn, RectOrg;
	GetWindowRect(&RectDlg);
	GetDlgItem(IDOK    )->GetWindowRect(&RectOk);
	GetDlgItem(IDCANCEL)->GetWindowRect(&RectCn);

	int iStatus = GetDataStatus(m_strCode);
	switch (iStatus)
	{
	case 1:	  {RectOrg = MoveCtrl(iStatus,m_iFPHIID); break;}
	case 2:	  {RectOrg = MoveCtrl(iStatus,m_iMPHIID1); break;}
	case 3:	  {RectOrg = MoveCtrl(iStatus,m_iMPHIID2); break;}
	case 4:	  {RectOrg = MoveCtrl(iStatus,m_iMPHIID3); break;}
	case 5:	  {RectOrg = MoveCtrl(iStatus,m_iMPHIID4); break;} // Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)
	case 6:	  {RectOrg = MoveCtrl(iStatus,m_iMPHIID5); break;}
	case 7:	  {RectOrg = MoveCtrl(iStatus,m_iMPHIID6); break;}
	default:		{ASSERT(0);	  break;}
	}

	CRect rDlg;
	rDlg.left		= RectDlg.left;
	rDlg.top		= RectDlg.top;
	rDlg.right		= RectDlg.right;
	rDlg.bottom		= RectOrg.bottom + RectOk.Height() + globalUtils.ScaleByDPI(24); //(int)(2.5*RectOk.Height());

	CRect rOk;
	rOk.left		= RectOk.left;
	rOk.right		= RectOk.right;
	rOk.top			= rDlg.bottom - RectOk.Height() - globalUtils.ScaleByDPI(8); // (int)(1.75*RectOk.Height());
	rOk.bottom		= rOk.top + RectOk.Height();

	CRect rCn;
	rCn.left		= RectCn.left;
	rCn.right		= RectCn.right;
	rCn.top			= rOk.top;
	rCn.bottom		= rOk.bottom;
	
	ScreenToClient(rDlg);
	ScreenToClient(rOk);
	ScreenToClient(rCn);
	SetWindowPos(NULL, rDlg.left, rDlg.top, rDlg.Width(), rDlg.Height(), SWP_NOMOVE | SWP_NOZORDER);
	GetDlgItem(IDOK    )->SetWindowPos(NULL, rOk.left, rOk.top, rOk.Width(), rOk.Height(), SWP_NOSIZE);
	GetDlgItem(IDCANCEL)->SetWindowPos(NULL, rCn.left, rCn.top, rCn.Width(), rCn.Height(), SWP_NOSIZE);

	Set_SubCodeTitle();
}

CRect CDgnStlPhiDlg::MoveCtrl(int iKind, int* pID)
{
	CRect RectOrg;
	int iArSize=0;
	switch (iKind)
	{
	case 1:{iArSize = iDgn_STL_FPHIID;	break;}
	case 2:{iArSize = iDgn_STL_MPHIID1;	break;}
	case 3:{iArSize = iDgn_STL_MPHIID2;	break;}
	case 4:{iArSize = iDgn_STL_MPHIID3;	break;}
	case 5:{iArSize = iDgn_STL_MPHIID4;	break;} // Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)
	case 6:{iArSize = iDgn_STL_MPHIID5; break;}
	case 7:{iArSize = iDgn_STL_MPHIID6; break;}
	default:{ASSERT(0);break;}
	}

	CRect* pRectCng = new CRect[iArSize];

	CRect Rect0, Rect1;
	GetDlgItem(m_iFPHIID[0])->GetWindowRect(&Rect0);
	for(int i=0; i<iArSize; i++)
	{
		GetDlgItem(pID[i])->ShowWindow(SW_SHOW);
		GetDlgItem(pID[i])->GetWindowRect(&(pRectCng[i]));
		if(i==0)	Rect1 = pRectCng[i];
		pRectCng[i].top		 -= Rect1.top - Rect0.top;
		pRectCng[i].bottom -= Rect1.top - Rect0.top;
	}
	RectOrg = pRectCng[0];
	for(int i=0; i<iArSize; i++)
	{
		ScreenToClient(pRectCng[i]);
		GetDlgItem(pID[i])->SetWindowPos(NULL, pRectCng[i].left, pRectCng[i].top, pRectCng[i].Width(), pRectCng[i].Height(), SWP_NOSIZE | SWP_NOZORDER);
	}
	delete[] pRectCng;

	return RectOrg;
}

int CDgnStlPhiDlg::GetDataStatus(CString strCode)
{
	int iStatus=0;

	if(strCode==_T("Eurocode3"))	        {iStatus = 2;}
	else if(strCode==_T("Eurocode3:05"))	{iStatus = 2;}
	else if(strCode==_T("Eurocode3-2:05"))	{iStatus = 2;}
	else if(strCode==_T("BS5950-90"))		{iStatus = 3;}
	else if(strCode==_T("IS:800-2007"))		{iStatus = 3;}
	else if(strCode==_T("IS:800-2007"))		{iStatus = 3;}
	else if(strCode==_T("CSA-S16-01"))		{iStatus = 4;}
	else if(strCode==_T("CSA-S6-14"))		{iStatus = 4;}
	else if(strCode==_T("BS5950-2K"))		{iStatus = 5;}  // Coded by Seungjun MNET:No.2086 BS5950-2K ('20060623)
	else if(strCode==_T("KSCE-LSD15"))		{iStatus = 6;}
	else if(strCode==_T("IRC:24-2010"))		{iStatus = 3;}
	else if(strCode== STLCODE_SP_16_13330_2017){iStatus = 7;}
	else									{iStatus = 1;}
	
	return iStatus;
}
