// DgnConPhiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_CON_DCON_SRED
#include "wg_dgn.h"
#include "DgnConPhiDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_MsgDll.h"

#include "DgnDataCtrl.h"
#include "NationalAnnexTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace dgn::def;

CDgnConPhiDlg::CDgnConPhiDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConPhiDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConPhiDlg)
	m_FPhiB = 0.0;
	m_FPhiC1 = 0.0;
	m_FPhiC2 = 0.0;
	m_FPhiS = 0.0;
	m_FPhiT = 0.0;

	m_MPhiC = 0.0;
	m_MPhiS = 0.0;
	m_MPhiM = 0.0;
	m_MPhiC2 = 0.0;
	m_MPhiS2 = 0.0;
	m_MPhiV2 = 0.0;
	m_MPhiC3 = 0.0;
	m_MPhiS3 = 0.0;
	m_MPhiC4 = 0.0;
	m_MPhiS4 = 0.0;

	m_MFundPhiC5 = 0.0;
	m_MAcciPhiC5 = 0.0;
	m_MFundPhiS5 = 0.0;
	m_MAcciPhiS5 = 0.0;
	m_MAlphacc = 0.0;

	//
	m_MFundPhiC8 = 0.0;
	m_MAcciPhiC8 = 0.0;
	m_MFundPhiS8 = 0.0;
	m_MAcciPhiS8 = 0.0;
	m_MSLSPhiC8 = 0.0;
	m_MSLSPhiS8 = 0.0;

	m_MAlphacc_CVL = 0.0;
	m_MAlphacc4Flex_CVL = 0.0;
	m_MAlphacc4Shear_CVL = 0.0;

	m_strTitle = _T("");
	m_strCode = _T("");
	m_strCode2 = _T("");

	m_07PhiT = 0.0;
	m_07PhiC1 = 0.0;
	m_07PhiC2 = 0.0;
	m_07PhiV = 0.0;

	//

	m_IRS_compr11 = 0.0;
	m_IRS_shr11 = 0.0;
	m_IRS_sls11 = 0.0;
	m_IRS_gamma_s11 = 0.0;
	m_IRS_alpha_cc11 = 0.0;
	m_strNationalAnnex = _T("");

	m_rus_sp_63_2018_g1 = 0.0;
	m_rus_sp_63_2018_g2 = 0.0;
	m_rus_sp_63_2018_g3 = 0.0;
	m_rus_sp_63_2018_g4 = 0.0;
	m_rus_sp_63_2018_g5 = 0.0;
	m_rus_sp_63_2018_gs = 0.0;

	m_bSeisChk = FALSE;  // CSA-S6-14
	//}}AFX_DATA_INIT
}

void CDgnConPhiDlg::GetDataFromDlg(T_DCON_D& rData)
{
    int iStatus = GetDataStatus(m_strCode);

    switch ( iStatus )
    {
    case 1:
        {
            rData.dPhi[0] = m_FPhiB;
            rData.dPhi[1] = m_FPhiT;
            rData.dPhi[2] = m_FPhiC1;
            rData.dPhi[3] = m_FPhiC2;
            rData.dPhi[4] = m_FPhiS;
        }
        break;
    case 2:
        {
            rData.dPhi[0] = m_MPhiC;
            rData.dPhi[1] = m_MPhiS;
            rData.dPhi[2] = m_MPhiM;
        }
        break;
    case 3:
        {
            rData.dPhi[0] = m_MPhiC2;
            rData.dPhi[1] = m_MPhiS2;
            rData.dPhi[2] = m_MPhiV2;
        }
        break;
    case 4:
        {
            rData.dPhi[0] = m_MPhiC3;
            rData.dPhi[1] = m_MPhiS3;
        }
        break;
    case 5:
        {
            rData.dPhi[0] = m_MPhiC4;
            rData.dPhi[1] = m_MPhiS4;
        }
        break;
    case 6:
        {
            rData.dPhi[0] = m_MFundPhiC5;
            rData.dPhi[1] = m_MAcciPhiC5;
            rData.dPhi[2] = m_MFundPhiS5;
            rData.dPhi[3] = m_MAcciPhiS5;
        }
        break;
        // Coded by Seungjun ('20070706) MNet:No.2864.
    case 7:
        {
            rData.dPhi[0] = m_MFundPhiC5;
            rData.dPhi[1] = m_MAcciPhiC5;
            rData.dPhi[2] = m_MFundPhiS5;
            rData.dPhi[3] = m_MAcciPhiS5;
            rData.dAlphacc = m_MAlphacc;
        }
        break;
    case 8:
        {
            rData.dPhi[0] = m_07PhiT;
            rData.dPhi[1] = m_07PhiT;
            rData.dPhi[2] = m_07PhiC1;
            rData.dPhi[3] = m_07PhiC2;
            rData.dPhi[4] = m_07PhiV;
        }
        break;
    case 9:
        {
			rData.dPhi[0] = m_MFundPhiC8;
			rData.dPhi[1] = m_MAcciPhiC8;
			rData.dPhi[2] = m_MFundPhiS8;
			rData.dPhi[3] = m_MAcciPhiS8;
			rData.dPhi[4] = m_MSLSPhiC8;
			rData.dPhi[5] = m_MSLSPhiS8;

			const auto nAnnex = CNationalAnnexTool::ConvertNationalAnnex(m_strNationalAnnex);
			const auto bAlphacc4Shear = HasAlphacc4ShearCtrl(nAnnex);

			if (!bAlphacc4Shear)
				rData.dAlphacc = m_MAlphacc_CVL;
			else
				rData.dAlphacc = m_MAlphacc4Flex_CVL;

			rData.dAlphacc4Shear = m_MAlphacc4Shear_CVL;
        }
        break;
    case 10:
        {

            rData.dPhi[0] = m_MPhiC;
            rData.dPhi[1] = m_MPhiS;
            rData.dPhi[2] = m_MPhiM;
            rData.bSpecialSeismic=m_bSeisChk;
            if ( m_bSeisChk && rData.nClass != 2 && rData.nClass != 3 )
            {
                rData.nClass=2; // Default Zone 3   	 //  0 1 2 3  // Zones
            }
        }
        break;

    case 11:
        {
            rData.dPhi[0] = m_IRS_compr11;
            rData.dPhi[1] = m_IRS_shr11;
            rData.dPhi[2] = m_IRS_sls11;
            rData.dPhi[3] = m_IRS_gamma_s11;
            rData.dPhi[4] = m_IRS_alpha_cc11;
        }
        break;
	case 12:
	{
		rData.dPhi[0] = m_rus_sp_63_2018_g1;
		rData.dPhi[1] = m_rus_sp_63_2018_g2;
		rData.dPhi[2] = m_rus_sp_63_2018_g3;
		rData.dPhi[3] = m_rus_sp_63_2018_g4;
		rData.dPhi[4] = m_rus_sp_63_2018_g5;
		rData.dPhi[5] = m_rus_sp_63_2018_gs;
		rData.dAlphacc = m_MAlphacc;
	}
	break;
    default:
        ASSERT(0);
        break;
    }

    return;
}

void CDgnConPhiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConPhiDlg)
	DDX_Text(pDX, IDC_DGN_CON_F_PHIB, m_FPhiB);
	DDX_Text(pDX, IDC_DGN_CON_F_PHIC1, m_FPhiC1);
	DDX_Text(pDX, IDC_DGN_CON_F_PHIC2, m_FPhiC2);
	DDX_Text(pDX, IDC_DGN_CON_F_PHIS, m_FPhiS);
	DDX_Text(pDX, IDC_DGN_CON_F_PHIT, m_FPhiT);
	DDX_Text(pDX, IDC_DGN_CON_M_PHIC, m_MPhiC);
	DDX_Text(pDX, IDC_DGN_CON_M_PHIS, m_MPhiS);
	DDX_Text(pDX, IDC_DGN_CON_M_PHIM, m_MPhiM);
	DDX_Text(pDX, IDC_DGN_CON_M_PHIC2, m_MPhiC2);
	DDX_Text(pDX, IDC_DGN_CON_M_PHIS2, m_MPhiS2);
	DDX_Text(pDX, IDC_DGN_CON_M_PHIV2, m_MPhiV2);
	DDX_Text(pDX, IDC_DGN_CON_M_PHIC3, m_MPhiC3);
	DDX_Text(pDX, IDC_DGN_CON_M_PHIS3, m_MPhiS3);
	DDX_Text(pDX, IDC_DGN_CON_M_PHIC4, m_MPhiC4);
	DDX_Text(pDX, IDC_DGN_CON_M_PHIS4, m_MPhiS4);
	DDX_Text(pDX, IDC_DGN_CON_M_FUND_PHIC5, m_MFundPhiC5);
	DDX_Text(pDX, IDC_DGN_CON_M_ACCI_PHIC5, m_MAcciPhiC5);
	DDX_Text(pDX, IDC_DGN_CON_M_FUND_PHIS5, m_MFundPhiS5);
	DDX_Text(pDX, IDC_DGN_CON_M_ACCI_PHIS5, m_MAcciPhiS5);
	//
	DDX_Text(pDX, IDC_DGN_CON_M_ALPHACC, m_MAlphacc);
	DDX_Text(pDX, IDC_DGN_STATIC_RCCODE, m_strCode);
	DDX_Text(pDX, IDC_DGN_STATIC_RCCODE2, m_strCode2);

	DDX_Text(pDX, IDC_DGN_CON_07_PHIT, m_07PhiT);
	DDX_Text(pDX, IDC_DGN_CON_07_PHIC1, m_07PhiC1);
	DDX_Text(pDX, IDC_DGN_CON_07_PHIC2, m_07PhiC2);
	DDX_Text(pDX, IDC_DGN_CON_07_PHIV, m_07PhiV);
	//
	DDX_Text(pDX, IDC_DGN_CON_M_FUND_PHIC8, m_MFundPhiC8);
	DDX_Text(pDX, IDC_DGN_CON_M_ACCI_PHIC8, m_MAcciPhiC8);
	DDX_Text(pDX, IDC_DGN_CON_M_FUND_PHIS8, m_MFundPhiS8);
	DDX_Text(pDX, IDC_DGN_CON_M_ACCI_PHIS8, m_MAcciPhiS8);
	DDX_Text(pDX, IDC_DGN_CON_M_SLS_PHIC8, m_MSLSPhiC8);
	DDX_Text(pDX, IDC_DGN_CON_M_SLS_PHIS8, m_MSLSPhiS8);
	DDX_Text(pDX, IDC_DGN_CON_M_CVL_ALPHACC, m_MAlphacc_CVL);
	DDX_Text(pDX, IDC_DGN_CON_M_CVL_ALPHACC4FLEX, m_MAlphacc4Flex_CVL);
	DDX_Text(pDX, IDC_DGN_CON_M_CVL_ALPHACC4SHEAR, m_MAlphacc4Shear_CVL);

	DDX_Text(pDX, IDC_DGN_CON_M_FUND_PHIC11, m_IRS_compr11);
	DDX_Text(pDX, IDC_DGN_CON_M_ACCI_PHIC11, m_IRS_shr11);
	DDX_Text(pDX, IDC_DGN_CON_M_SLS_PHIC11, m_IRS_sls11);
	DDX_Text(pDX, IDC_DGN_CON_M_FUND_PHIS11, m_IRS_gamma_s11);
	DDX_Text(pDX, IDC_DGN_CON_M_ACCI_PHIS11, m_IRS_alpha_cc11);

	DDX_Text(pDX, IDC_DGN_CON_PHIC_G1_EDITTEXT12, m_rus_sp_63_2018_g1);
	DDX_Text(pDX, IDC_DGN_CON_PHIC_G2_EDITTEXT12, m_rus_sp_63_2018_g2);
	DDX_Text(pDX, IDC_DGN_CON_PHIC_G3_EDITTEXT12, m_rus_sp_63_2018_g3);
	DDX_Text(pDX, IDC_DGN_CON_PHIC_G4_EDITTEXT12, m_rus_sp_63_2018_g4);
	DDX_Text(pDX, IDC_DGN_CON_PHIC_G5_EDITTEXT12, m_rus_sp_63_2018_g5);
	DDX_Text(pDX, IDC_DGN_CON_PHIS_GS_EDITTEXT12, m_rus_sp_63_2018_gs);

	DDX_Check(pDX, IDC_DGN_CON_M_SEIS_CHECK, m_bSeisChk);
	//}}AFX_DATA_MAP

	// Strength Reduction Factors 
	m_iFPHIID[0] = IDC_DGN_CON_F_FRAME;
	m_iFPHIID[1] = IDC_DGN_CON_F_PHIT_STR;
	m_iFPHIID[2] = IDC_DGN_CON_F_PHIB_STR;
	m_iFPHIID[3] = IDC_DGN_CON_F_PHIC_STR;
	m_iFPHIID[4] = IDC_DGN_CON_F_PHIC1_STR;
	m_iFPHIID[5] = IDC_DGN_CON_F_PHIC2_STR;
	m_iFPHIID[6] = IDC_DGN_CON_F_PHIS_STR;
	m_iFPHIID[7] = IDC_DGN_CON_F_PHIB;
	m_iFPHIID[8] = IDC_DGN_CON_F_PHIT;
	m_iFPHIID[9] = IDC_DGN_CON_F_PHIC1;
	m_iFPHIID[10] = IDC_DGN_CON_F_PHIC2;
	m_iFPHIID[11] = IDC_DGN_CON_F_PHIS;

	//Resistance Factors
	m_iMPHIID1[0] = IDC_DGN_CON_M_FRAME;
	m_iMPHIID1[1] = IDC_DGN_CON_M_PHIC_STR;
	m_iMPHIID1[2] = IDC_DGN_CON_M_PHIS_STR;
	m_iMPHIID1[3] = IDC_DGN_CON_M_PHIM_STR;
	m_iMPHIID1[4] = IDC_DGN_CON_M_PHIC;
	m_iMPHIID1[5] = IDC_DGN_CON_M_PHIS;
	m_iMPHIID1[6] = IDC_DGN_CON_M_PHIM;

	//Partial Safety Factor for Strength of  Materials (Gamma_m)
	m_iMPHIID2[0] = IDC_DGN_CON_M_FRAME2;
	m_iMPHIID2[1] = IDC_DGN_CON_M_PHIC_STR2;
	m_iMPHIID2[2] = IDC_DGN_CON_M_PHIS_STR2;
	m_iMPHIID2[3] = IDC_DGN_CON_M_PHIV_STR2;	
	m_iMPHIID2[4] = IDC_DGN_CON_M_PHIC2;
	m_iMPHIID2[5] = IDC_DGN_CON_M_PHIS2;
	m_iMPHIID2[6] = IDC_DGN_CON_M_PHIV2;

	//Safety Factor for Capacity of  Materials
	m_iMPHIID3[0] = IDC_DGN_CON_M_FRAME3;
	m_iMPHIID3[1] = IDC_DGN_CON_M_PHIC_STR3;
	m_iMPHIID3[2] = IDC_DGN_CON_M_PHIS_STR3;	
	m_iMPHIID3[3] = IDC_DGN_CON_M_PHIC3;
	m_iMPHIID3[4] = IDC_DGN_CON_M_PHIS3;

	//Partial Safety Factor for Material Strength (Gamma_m)
	m_iMPHIID4[0] = IDC_DGN_CON_M_FRAME4;
	m_iMPHIID4[1] = IDC_DGN_CON_M_PHIC_STR4;
	m_iMPHIID4[2] = IDC_DGN_CON_M_PHIS_STR4;	
	m_iMPHIID4[3] = IDC_DGN_CON_M_PHIC4;
	m_iMPHIID4[4] = IDC_DGN_CON_M_PHIS4;

	//Partial Safety Factors for Material Properties
	m_iMPHIID5[0] = IDC_DGN_CON_M_FRAME5;
	m_iMPHIID5[1] = IDC_DGN_CON_M_PHIC_STR5;
	m_iMPHIID5[2] = IDC_DGN_CON_M_FUND_PHIC_STR5;
	m_iMPHIID5[3] = IDC_DGN_CON_M_ACCI_PHIC_STR5;
	m_iMPHIID5[4] = IDC_DGN_CON_M_PHIS_STR5;
	m_iMPHIID5[5] = IDC_DGN_CON_M_FUND_PHIS_STR5;
	m_iMPHIID5[6] = IDC_DGN_CON_M_ACCI_PHIS_STR5;
	m_iMPHIID5[7] = IDC_DGN_CON_M_FUND_PHIC5;
	m_iMPHIID5[8] = IDC_DGN_CON_M_ACCI_PHIC5;
	m_iMPHIID5[9] = IDC_DGN_CON_M_FUND_PHIS5;
	m_iMPHIID5[10] = IDC_DGN_CON_M_ACCI_PHIS5;

	// Coded by Seungjun ('20070706) MNet:No.2864.
	// The coefficient for long term effects in Eurocode2:04.
	m_iMPHIID6[0] = IDC_DGN_CON_M_FRAME6;
	m_iMPHIID6[1] = IDC_DGN_CON_M_ALPHACC_STR;	
	m_iMPHIID6[2] = IDC_DGN_CON_M_ALPHACC;

	// Coded by sshan ('20090602) MNet:No..
	// Strength Reduction Factors
	m_iMPHIID7[0] = IDC_DGN_CON_M_FRAME7;
	m_iMPHIID7[1] = IDC_DGN_CON_07_PHIT_STR;
	m_iMPHIID7[2] = IDC_DGN_CON_07_PHIC_STR;
	m_iMPHIID7[3] = IDC_DGN_CON_07_PHIC1_STR;
	m_iMPHIID7[4] = IDC_DGN_CON_07_PHIC2_STR;
	m_iMPHIID7[5] = IDC_DGN_CON_07_PHIV_STR;
	m_iMPHIID7[6] = IDC_DGN_CON_07_PHIT;
	m_iMPHIID7[7] = IDC_DGN_CON_07_PHIC1;
	m_iMPHIID7[8] = IDC_DGN_CON_07_PHIC2;
	m_iMPHIID7[9] = IDC_DGN_CON_07_PHIV;	

	//(2009.06.15) Add by Unsang :: Only EC2-2:05, ColmDgn!
	// Partial Safety Factor in EC2-2:05 by National Annex!!
	m_iMPHIID8[0] = IDC_DGN_CON_M_FRAME8;
	m_iMPHIID8[1] = IDC_DGN_CON_M_PHIC_STR8;
	m_iMPHIID8[2] = IDC_DGN_CON_M_FUND_PHIC_STR8;
	m_iMPHIID8[3] = IDC_DGN_CON_M_ACCI_PHIC_STR8;
	m_iMPHIID8[4] = IDC_DGN_CON_M_PHIS_STR8;
	m_iMPHIID8[5] = IDC_DGN_CON_M_FUND_PHIS_STR8;
	m_iMPHIID8[6] = IDC_DGN_CON_M_ACCI_PHIS_STR8;
	m_iMPHIID8[7] = IDC_DGN_CON_M_FUND_PHIC8;
	m_iMPHIID8[8] = IDC_DGN_CON_M_ACCI_PHIC8;
	m_iMPHIID8[9] = IDC_DGN_CON_M_FUND_PHIS8;
	m_iMPHIID8[10] = IDC_DGN_CON_M_ACCI_PHIS8;
	m_iMPHIID8[11] = IDC_DGN_CON_M_SLS_PHIC_STR8;
	m_iMPHIID8[12] = IDC_DGN_CON_M_SLS_PHIS_STR8;
	m_iMPHIID8[13] = IDC_DGN_CON_M_SLS_PHIC8;
	m_iMPHIID8[14] = IDC_DGN_CON_M_SLS_PHIS8;

	// The Coefficient for Long-term Effects in EC2-2:05 by National Annex!!
	m_iMPHIID9[0] = IDC_DGN_CON_M_FRAME9;
	m_iMPHIID9[1] = IDC_DGN_CON_M_CVL_ALPHACC_STR;	
	m_iMPHIID9[2] = IDC_DGN_CON_M_CVL_ALPHACC4FLEX;
	m_iMPHIID9[3] = IDC_DGN_CON_M_CVL_ALPHACC4SHEAR_STR;	
	m_iMPHIID9[4] = IDC_DGN_CON_M_CVL_ALPHACC4SHEAR;

	// Resistance Factors  // CSA-S6-14
	m_iMPHIID10[0] = IDC_DGN_CON_M_FRAME10;
	m_iMPHIID10[1] = IDC_DGN_CON_M_SEIS_CHECK;

	//(2019.11.05) Add by Pinakin :: IRS CBC 1997 RC Beam column design	
	m_iMPHIID11[0] = IDC_DGN_CON_M_FRAME11;
	m_iMPHIID11[1] = IDC_DGN_CON_M_PHIC_STR11;
	m_iMPHIID11[2] = IDC_DGN_CON_M_FUND_PHIC_STR11;
	m_iMPHIID11[3] = IDC_DGN_CON_M_ACCI_PHIC_STR11;
	m_iMPHIID11[4] = IDC_DGN_CON_M_SLS_PHIC_STR11;
	m_iMPHIID11[5] = IDC_DGN_CON_M_PHIS_STR11;
	m_iMPHIID11[6] = IDC_DGN_CON_M_FUND_PHIS_STR11;
	m_iMPHIID11[7] = IDC_DGN_CON_M_FUND_PHIC11;
	m_iMPHIID11[8] = IDC_DGN_CON_M_ACCI_PHIC11;
	m_iMPHIID11[9] = IDC_DGN_CON_M_SLS_PHIC11;
	m_iMPHIID11[10] = IDC_DGN_CON_M_FUND_PHIS11;
	m_iMPHIID11[11] = IDC_DGN_CON_M_ACCI_PHIS11;

	m_iMPHIID12[0] = IDC_DGN_CON_M_FRAME12;
	m_iMPHIID12[1] = IDC_DGN_CON_M_PHIC_STR12;
	m_iMPHIID12[2] = IDC_DGN_CON_PHIC_G1_STR12;
	m_iMPHIID12[3] = IDC_DGN_CON_PHIC_G2_STR12;
	m_iMPHIID12[4] = IDC_DGN_CON_PHIC_G3_STR12;
	m_iMPHIID12[5] = IDC_DGN_CON_PHIC_G4_STR12;
	m_iMPHIID12[6] = IDC_DGN_CON_PHIC_G5_STR12;
	m_iMPHIID12[7] = IDC_DGN_CON_PHIC_G1_EDITTEXT12;
	m_iMPHIID12[8] = IDC_DGN_CON_PHIC_G2_EDITTEXT12;
	m_iMPHIID12[9] = IDC_DGN_CON_PHIC_G3_EDITTEXT12;
	m_iMPHIID12[10] = IDC_DGN_CON_PHIC_G4_EDITTEXT12;
	m_iMPHIID12[11] = IDC_DGN_CON_PHIC_G5_EDITTEXT12;
	m_iMPHIID12[12] = IDC_DGN_CON_PHIS_STR12;
	m_iMPHIID12[13] = IDC_DGN_CON_PHIS_GS_STR12;
	m_iMPHIID12[14] = IDC_DGN_CON_PHIS_GS_EDITTEXT12;
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnConPhiDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnConPhiDlg)
	ON_BN_CLICKED(IDC_DGN_PHI_UPDATE_BTN, OnDgnPhiUpdateBtn)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConPhiDlg message handlers

BOOL CDgnConPhiDlg::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	// TODO: Add extra initialization here
	if (m_pDoc->m_pAttrCtrl->ExistDcon())  Initial_ExistData();
	else                                  Initial_DefaultData();
	// Add by ZINU.('01.10.24). If WSD, Disable.

	Update_InitDataByCode(m_strCode);

	return TRUE;
}

BOOL CDgnConPhiDlg::Set_DefaultByCode(CString strCode)
{
	if (strCode == _T("")) { return FALSE; }
	CDgnDataCtrl DataCtrl;
	double dPhi[6] = { 1.0, };
	if (!DataCtrl.Get_DgnConPhiDef(strCode, dPhi))	ASSERT(0);

	int iStatus = GetDataStatus(m_strCode);

	switch (iStatus)
	{
	case 1:
	{
		m_FPhiB = dPhi[0];
		m_FPhiT = dPhi[1];
		m_FPhiC1 = dPhi[2];
		m_FPhiC2 = dPhi[3];
		m_FPhiS = dPhi[4];
	}
	break;
	case 2:
	{
		m_MPhiC = dPhi[0];
		m_MPhiS = dPhi[1];
		m_MPhiM = dPhi[2];
	}
	break;
	case 3:
	{
		m_MPhiC2 = dPhi[0];
		m_MPhiS2 = dPhi[1];
		m_MPhiV2 = dPhi[2];
	}
	break;
	case 4:
	{
		m_MPhiC3 = dPhi[0];
		m_MPhiS3 = dPhi[1];
	}
	break;
	case 5:
	{
		m_MPhiC4 = dPhi[0];
		m_MPhiS4 = dPhi[1];
	}
	break;
	case 6:
	{
		m_MFundPhiC5 = dPhi[0];
		m_MAcciPhiC5 = dPhi[1];
		m_MFundPhiS5 = dPhi[2];
		m_MAcciPhiS5 = dPhi[3];
	}
	break;
	case 7:
	{
		T_DCON_D rData;
		rData.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetDcon(rData))
		{
			T_PREFERENCE rPref;
			rPref.Initialize();
			m_pDoc->m_pInitCtrl->GetPreference(rPref);

			rData.DesignCode = rPref.DgnCode.ConcCode;
			rData.nNationalAnnex = rPref.DgnCode.nConcNationalAnnex;
		}
		m_strNationalAnnex = CNationalAnnexTool::ConvertNationalAnnex(rData.nNationalAnnex);
		if(rData.nNationalAnnex != enNationalAnnex::Recommended)
		{
			m_strCode2.Format(_T("%s[%s]"), m_strCode, CNationalAnnexTool::Get_Alpha_3_code(rData.nNationalAnnex));
		}
		else
		{
			m_strCode2 = m_strCode;
		}

		m_MFundPhiC5 = dPhi[0];
		m_MAcciPhiC5 = dPhi[1];
		m_MFundPhiS5 = dPhi[2];
		m_MAcciPhiS5 = dPhi[3];
		m_MAlphacc = [](int nAnnex) -> double
		{
			// EN 1992-1-1:2004 3.1.6 (1) αcc for design compressive strength
			switch (nAnnex)
			{
				case dgn::def::enNationalAnnex::Belgium:
				case dgn::def::enNationalAnnex::Finland:
				case dgn::def::enNationalAnnex::Germany:
				case dgn::def::enNationalAnnex::Greece:
				case dgn::def::enNationalAnnex::Italy:
				case dgn::def::enNationalAnnex::Luxembourg:
				case dgn::def::enNationalAnnex::Norway:
				case dgn::def::enNationalAnnex::Singapore:
					return 0.85;
				default: return 1.0; // recommended
			}
		}(rData.nNationalAnnex);
	}
	break;
	case 8:
	{
		m_07PhiT = dPhi[1];
		m_07PhiC1 = dPhi[2];
		m_07PhiC2 = dPhi[3];
		m_07PhiV = dPhi[4];
	}
	break;
	case 9:
	{
		//TEST// National Annex고려..
		double dAlpha_cc = 0.0;
		double dAlphacc4Shear = 0.0;
		T_DCON_D rData;
		rData.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetDcon(rData))
		{
			T_PREFERENCE rPref;
			rPref.Initialize();
			m_pDoc->m_pInitCtrl->GetPreference(rPref);

			rData.DesignCode = rPref.DgnCode.ConcCode;
			rData.nNationalAnnex = rPref.DgnCode.nConcNationalAnnex;
		}
		if (m_strCode == _T("IRC:112-2011"))
		{
			SetDataIRC112(iStatus, dPhi, dAlpha_cc);
			m_strCode2 = m_strCode;
		}
		else if (m_strCode == _T("IRC:112-2020"))
		{
			SetDataIRC112(iStatus, dPhi, dAlpha_cc);
			m_strCode2 = m_strCode;
		}
		else
		{
			m_pDoc->m_pDgnDataCtrl->SetDatabyNationalAnnex(rData.nNationalAnnex, dPhi, dAlpha_cc, dAlphacc4Shear);
			m_strNationalAnnex = CNationalAnnexTool::ConvertNationalAnnex(rData.nNationalAnnex);
			if(rData.nNationalAnnex != enNationalAnnex::Recommended)
			{
				m_strCode2.Format(_T("%s[%s]"), m_strCode, CNationalAnnexTool::Get_Alpha_3_code(rData.nNationalAnnex));
			}
			else
			{
				m_strCode2 = m_strCode;
			}
		}

		m_MFundPhiC8 = dPhi[0];
		m_MAcciPhiC8 = dPhi[1];
		m_MFundPhiS8 = dPhi[2];
		m_MAcciPhiS8 = dPhi[3];
		m_MSLSPhiC8 = dPhi[4]; // SLS Fund.
		m_MSLSPhiS8 = dPhi[5]; // SLS Acci.
		m_MAlphacc_CVL = dAlpha_cc;
		m_MAlphacc4Flex_CVL = dAlpha_cc;
		m_MAlphacc4Shear_CVL = dAlphacc4Shear;
	}
	break;
	case 10:
	{
		m_MPhiC = dPhi[0];
		m_MPhiS = dPhi[1];
		m_MPhiM = dPhi[2];
	}
	break;
	case 11:
	{

		m_IRS_compr11 = dPhi[0];
		m_IRS_shr11 = dPhi[1];
		m_IRS_sls11 = dPhi[2];
		m_IRS_gamma_s11 = dPhi[3];
		m_IRS_alpha_cc11 = dPhi[4];
	}
	break;
	case 12:
	{
		m_rus_sp_63_2018_g1 = dPhi[0];
		m_rus_sp_63_2018_g2 = dPhi[1];
		m_rus_sp_63_2018_g3 = dPhi[2];
		m_rus_sp_63_2018_g4 = dPhi[3];
		m_rus_sp_63_2018_g5 = dPhi[4];
		m_rus_sp_63_2018_gs = dPhi[5];
		m_MAlphacc = 1.0;
	}
	break;
	default:
		ASSERT(0);
		break;
	}
	return TRUE;
}

void CDgnConPhiDlg::Initial_DefaultData()
{
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);
	m_strCode = rPref.DgnCode.ConcCode;

	m_strNationalAnnex = _T("");
	if (m_strCode == _T("Eurocode2:04") || m_strCode == _T("Eurocode2-2:05"))
		m_strNationalAnnex = CNationalAnnexTool::ConvertNationalAnnex(rPref.DgnCode.nConcNationalAnnex);

	if (!Set_DefaultByCode(m_strCode))	ASSERT(0);
	UpdateData(FALSE);

	InitDlgCtrls();
}

void CDgnConPhiDlg::Initial_ExistData()
{
	T_DCON_D rData;
	rData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDcon(rData);
	if (rData.DesignCode == _T(""))
	{
		T_PREFERENCE rPref;
		rPref.Initialize();
		m_pDoc->m_pInitCtrl->GetPreference(rPref);
		m_strCode = rPref.DgnCode.ConcCode;

		m_strNationalAnnex = _T("");
		if (m_strCode == _T("Eurocode2:04") || m_strCode == _T("Eurocode2-2:05"))
			m_strNationalAnnex = CNationalAnnexTool::ConvertNationalAnnex(rPref.DgnCode.nConcNationalAnnex);

		if (!Set_DefaultByCode(m_strCode))	ASSERT(0);
	}
	else
	{
		m_strCode = rData.DesignCode;

		CDgnDataCtrl DataCtrl;
		double dPhi[6];
		if (!DataCtrl.Get_DgnConPhiDef(m_strCode, dPhi))	ASSERT(0);
		int iStatus = GetDataStatus(m_strCode);

		switch (iStatus)
		{
		case 1:
		{
			m_FPhiB = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
			m_FPhiT = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
			m_FPhiC1 = (rData.dPhi[2] == 0.0 ? dPhi[2] : rData.dPhi[2]);
			m_FPhiC2 = (rData.dPhi[3] == 0.0 ? dPhi[3] : rData.dPhi[3]);
			m_FPhiS = (rData.dPhi[4] == 0.0 ? dPhi[4] : rData.dPhi[4]);
		}
		break;
		case 2:
		{
			m_MPhiC = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
			m_MPhiS = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
			m_MPhiM = (rData.dPhi[2] == 0.0 ? dPhi[2] : rData.dPhi[2]);
		}
		break;

		case 3:
		{
			m_MPhiC2 = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
			m_MPhiS2 = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
			m_MPhiV2 = (rData.dPhi[2] == 0.0 ? dPhi[2] : rData.dPhi[2]);
		}
		break;
		case 4:
		{
			m_MPhiC3 = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
			m_MPhiS3 = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
		}
		break;
		case 5:
		{
			m_MPhiC4 = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
			m_MPhiS4 = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
		}
		break;
		case 6:
		{
			m_MFundPhiC5 = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
			m_MAcciPhiC5 = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
			m_MFundPhiS5 = (rData.dPhi[2] == 0.0 ? dPhi[2] : rData.dPhi[2]);
			m_MAcciPhiS5 = (rData.dPhi[3] == 0.0 ? dPhi[3] : rData.dPhi[3]);
		}
		break;
		// Coded by Seungjun ('20070706) MNet:No.2864.
		case 7:
		{
			m_strNationalAnnex = CNationalAnnexTool::ConvertNationalAnnex(rData.nNationalAnnex);
			if(rData.nNationalAnnex != enNationalAnnex::Recommended)
			{
				m_strCode2.Format(_T("%s[%s]"), m_strCode, CNationalAnnexTool::Get_Alpha_3_code(rData.nNationalAnnex));
			}
			else
			{
				m_strCode2 = m_strCode;
			}

			m_MFundPhiC5 = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
			m_MAcciPhiC5 = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
			m_MFundPhiS5 = (rData.dPhi[2] == 0.0 ? dPhi[2] : rData.dPhi[2]);
			m_MAcciPhiS5 = (rData.dPhi[3] == 0.0 ? dPhi[3] : rData.dPhi[3]);
			if (rData.dAlphacc == 0.0)
				m_MAlphacc = 1.0;
			else
				m_MAlphacc = rData.dAlphacc;

		}
		break;
		case 8:
		{
			m_07PhiT = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
			m_07PhiC1 = (rData.dPhi[2] == 0.0 ? dPhi[2] : rData.dPhi[2]);
			m_07PhiC2 = (rData.dPhi[3] == 0.0 ? dPhi[3] : rData.dPhi[3]);
			m_07PhiV = (rData.dPhi[4] == 0.0 ? dPhi[4] : rData.dPhi[4]);
		}
		break;
		case 9:
		{
			//TEST// National Annex고려..
			double dAlpha_cc = 0.0;
			double dAlphacc4Shear = 0.0;
			if (m_strCode == _T("IRC:112-2011"))
			{
				SetDataIRC112(iStatus, dPhi, dAlpha_cc);
				m_strCode2 = m_strCode;
				rData.dAlphacc = dAlpha_cc;
			}
			else if (m_strCode == _T("IRC:112-2020"))
			{
				SetDataIRC112(iStatus, dPhi, dAlpha_cc);
				m_strCode2 = m_strCode;
				rData.dAlphacc = dAlpha_cc;
			}
			else
			{
				m_pDoc->m_pDgnDataCtrl->SetDatabyNationalAnnex(rData.nNationalAnnex, dPhi, dAlpha_cc, dAlphacc4Shear);
				m_strNationalAnnex = CNationalAnnexTool::ConvertNationalAnnex(rData.nNationalAnnex);
				if(rData.nNationalAnnex != enNationalAnnex::Recommended)
				{
					m_strCode2.Format(_T("%s[%s]"), m_strCode, CNationalAnnexTool::Get_Alpha_3_code(rData.nNationalAnnex));
				}
				else
				{
					m_strCode2 = m_strCode;
				}
			}


			m_MFundPhiC8 = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
			m_MAcciPhiC8 = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
			m_MFundPhiS8 = (rData.dPhi[2] == 0.0 ? dPhi[2] : rData.dPhi[2]);
			m_MAcciPhiS8 = (rData.dPhi[3] == 0.0 ? dPhi[3] : rData.dPhi[3]);
			m_MSLSPhiC8 = (rData.dPhi[4] == 0.0 ? dPhi[4] : rData.dPhi[4]);
			m_MSLSPhiS8 = (rData.dPhi[5] == 0.0 ? dPhi[5] : rData.dPhi[5]);

			const auto nAnnex = CNationalAnnexTool::ConvertNationalAnnex(m_strNationalAnnex);
			const auto bAlphacc4Shear = HasAlphacc4ShearCtrl(nAnnex);

			if (rData.dAlphacc == 0.0)
			{
				if (bAlphacc4Shear)
					m_MAlphacc4Flex_CVL = dAlpha_cc;
				else
					m_MAlphacc_CVL = dAlpha_cc;
			}
			else
			{
				if (bAlphacc4Shear)
					m_MAlphacc4Flex_CVL = rData.dAlphacc;
				else
					m_MAlphacc_CVL = rData.dAlphacc;
			}

			if (rData.dAlphacc4Shear == 0.0)
				m_MAlphacc4Shear_CVL = dAlphacc4Shear; // 제한값오류 시 Defualt값 SET함.
			else
				m_MAlphacc4Shear_CVL = rData.dAlphacc4Shear;

			if (m_strCode == _T("IRC:112-2011") || m_strCode == _T("IRC:112-2020"))
			{
				m_MAlphacc_CVL = dAlpha_cc;
				rData.dAlphacc = dAlpha_cc;
			}
		}
		break;
		case 10:
		{
			m_MPhiC = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
			m_MPhiS = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
			m_MPhiM = (rData.dPhi[2] == 0.0 ? dPhi[2] : rData.dPhi[2]);

			// int Zone= rData.nClass;  //  0 1 2 3  // Zone: 1 2 3 4
			if (rData.nClass == 2 || rData.nClass == 3)
			{
				m_bSeisChk = rData.bSpecialSeismic;  // GUI DEFAULT CSA-S6-14
			}
		}
		break;
		case 11:
		{
			m_IRS_compr11 = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
			m_IRS_shr11 = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
			m_IRS_sls11 = (rData.dPhi[2] == 0.0 ? dPhi[2] : rData.dPhi[2]);
			m_IRS_gamma_s11 = (rData.dPhi[3] == 0.0 ? dPhi[3] : rData.dPhi[3]);
			m_IRS_alpha_cc11 = (rData.dPhi[4] == 0.0 ? dPhi[4] : rData.dPhi[4]);
		}
		break;
		case 12:
		{
			m_strNationalAnnex = CNationalAnnexTool::ConvertNationalAnnex(rData.nNationalAnnex);
			if(rData.nNationalAnnex != enNationalAnnex::Recommended)
			{
				m_strCode2.Format(_T("%s[%s]"), m_strCode, CNationalAnnexTool::Get_Alpha_3_code(rData.nNationalAnnex));
			}
			else
			{
				m_strCode2 = m_strCode;
			}

			m_rus_sp_63_2018_g1 = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
			m_rus_sp_63_2018_g2 = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
			m_rus_sp_63_2018_g3 = (rData.dPhi[2] == 0.0 ? dPhi[2] : rData.dPhi[2]);
			m_rus_sp_63_2018_g4 = (rData.dPhi[3] == 0.0 ? dPhi[3] : rData.dPhi[3]);
			m_rus_sp_63_2018_g5 = (rData.dPhi[4] == 0.0 ? dPhi[4] : rData.dPhi[4]);
			m_rus_sp_63_2018_gs = (rData.dPhi[5] == 0.0 ? dPhi[5] : rData.dPhi[5]);
			if(rData.dAlphacc == 0.0)
				m_MAlphacc = 1.0;
			else
				m_MAlphacc = rData.dAlphacc;
		}
		break;
		default:
			ASSERT(0);
			break;
		}
	}
	UpdateData(FALSE);

	InitDlgCtrls();
}

void CDgnConPhiDlg::SetDataIRC112(int iStatus, double* dPhi, double& dAlpha_cc)
{
	if (iStatus != 9) return;  // Only EC2-2:05 & IRC112



	dPhi[0] = 1.50; // Fund.Conc
	dPhi[1] = 1.20; // Acci.Conc
	dPhi[2] = 1.15; // Fund.Rbar
	dPhi[3] = 1.00; // Acci.Rbar
	dPhi[4] = 1.00; // SLS. Conc
	dPhi[5] = 1.00; // SLS. Rbar

	dAlpha_cc = 0.67;

}

void CDgnConPhiDlg::Update_InitDataByCode(CString strCodeName)
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleCon(ID_DGN_CON_DCON_SRED, m_strTitle, bShow, m_pDoc->IsPostMode());
	SetWindowText(m_strTitle);

	if ( strCodeName == CONCODE_BS5400_90 )
	{
		m_strTitle += _T("(ULS)");
	}
	if (strCodeName == CONCODE_TMH07_89)
	{
		m_strTitle += _T("(ULS)");
	}
	if(strCodeName == CONCODE_SP_63_13330_2018)
	{
		GetDlgItem(IDC_DGN_CON_M_ALPHACC_STR)->SetWindowText(_LS(IDS_DGN_CONC_PHI_SP63_RELATIVE_HUMIDITY));
	}
	int iStatus = GetDataStatus(m_strCode);

	// EC2:04 / EC2-2:05에서 DgnCode+NationalAnnex를 출력함.
	if (iStatus == 7 || iStatus == 9)
	{
		GetDlgItem(IDC_DGN_STATIC_RCCODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_RCCODE2)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_DGN_STATIC_RCCODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_RCCODE2)->ShowWindow(SW_HIDE);
	}

	if (iStatus == 1)			GetDlgItem(IDC_DGN_CON_F_FRAME)->SetWindowText(m_strTitle);
	else if (iStatus == 2)	GetDlgItem(IDC_DGN_CON_M_FRAME)->SetWindowText(m_strTitle);
	else if (iStatus == 3)	GetDlgItem(IDC_DGN_CON_M_FRAME2)->SetWindowText(m_strTitle);
	else if (iStatus == 4)	GetDlgItem(IDC_DGN_CON_M_FRAME3)->SetWindowText(m_strTitle);
	else if (iStatus == 5)	GetDlgItem(IDC_DGN_CON_M_FRAME4)->SetWindowText(m_strTitle);
	else if (iStatus == 6)	GetDlgItem(IDC_DGN_CON_M_FRAME5)->SetWindowText(m_strTitle);
	else if (iStatus == 7)	GetDlgItem(IDC_DGN_CON_M_FRAME5)->SetWindowText(m_strTitle);  // Coded by Seungjun ('20070706) MNet:No.2864. // MQC:7381-JWKWON-20140311
	else if (iStatus == 8)	GetDlgItem(IDC_DGN_CON_M_FRAME7)->SetWindowText(m_strTitle);  // Coded by sshan ('20090602) MNet:No..
	else if (iStatus == 9)	GetDlgItem(IDC_DGN_CON_M_FRAME8)->SetWindowText(m_strTitle);  // Coded by sshan ('20090602) MNet:No..
	else if (iStatus == 10)	GetDlgItem(IDC_DGN_CON_M_FRAME)->SetWindowText(m_strTitle); // CSA-S6-10
	else if (iStatus == 11)	GetDlgItem(IDC_DGN_CON_M_FRAME11)->SetWindowText(m_strTitle); //IRS CBC 1997
	else if (iStatus == 12)	GetDlgItem(IDC_DGN_CON_M_FRAME12)->SetWindowText(m_strTitle);
	else	ASSERT(0);

	if (bShow)
	{
		GetDlgItem(IDC_DGN_CON_F_PHIB)->EnableWindow(TRUE);
#if defined(_CIVIL)
		GetDlgItem(IDC_DGN_CON_F_PHIC1)->EnableWindow(FALSE);
#else
		GetDlgItem(IDC_DGN_CON_F_PHIC1)->EnableWindow(TRUE);
#endif
		GetDlgItem(IDC_DGN_CON_F_PHIC2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_F_PHIS)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_F_PHIT)->EnableWindow(TRUE);

		GetDlgItem(IDC_DGN_CON_M_PHIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_PHIS)->EnableWindow(TRUE);
		if (strCodeName == _T("CSA-A23.3-94"))
			GetDlgItem(IDC_DGN_CON_M_PHIM)->EnableWindow(TRUE);
		else
			GetDlgItem(IDC_DGN_CON_M_PHIM)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_PHIC2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_PHIS2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_PHIV2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_PHIC3)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_PHIS3)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_PHIC4)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_PHIS4)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_FUND_PHIC5)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_ACCI_PHIC5)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_FUND_PHIS5)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_ACCI_PHIS5)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_ALPHACC)->EnableWindow(TRUE);  // Coded by Seungjun ('20070706) MNet:No.2864.

		GetDlgItem(IDC_DGN_CON_07_PHIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_07_PHIC1)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_07_PHIC2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_07_PHIV)->EnableWindow(TRUE);

		GetDlgItem(IDC_DGN_CON_M_FUND_PHIC8)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_ACCI_PHIC8)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_FUND_PHIS8)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_ACCI_PHIS8)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_SLS_PHIC8)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_SLS_PHIS8)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_CVL_ALPHACC4FLEX)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_M_CVL_ALPHACC4SHEAR)->EnableWindow(TRUE);
		if (strCodeName == _T("IRC:112-2011") || strCodeName == _T("IRC:112-2020"))
			GetDlgItem(IDC_DGN_CON_M_CVL_ALPHACC)->EnableWindow(FALSE);
		else
			GetDlgItem(IDC_DGN_CON_M_CVL_ALPHACC)->EnableWindow(TRUE);  //(2009.06.15) Add by Unsang :: Only EC2-2:05, ColmDgn
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_F_PHIB)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_F_PHIC1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_F_PHIC2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_F_PHIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_F_PHIT)->EnableWindow(FALSE);

		GetDlgItem(IDC_DGN_CON_M_PHIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_PHIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_PHIM)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_PHIC2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_PHIS2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_PHIV2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_PHIC3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_PHIS3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_PHIC4)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_PHIS4)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_FUND_PHIC5)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_ACCI_PHIC5)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_FUND_PHIS5)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_ACCI_PHIS5)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_ALPHACC)->EnableWindow(FALSE);  // Coded by Seungjun ('20070706) MNet:No.2864.

		GetDlgItem(IDC_DGN_CON_07_PHIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_07_PHIC1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_07_PHIC2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_07_PHIV)->EnableWindow(FALSE);

		GetDlgItem(IDC_DGN_CON_M_FUND_PHIC8)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_ACCI_PHIC8)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_FUND_PHIS8)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_ACCI_PHIS8)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_SLS_PHIC8)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_SLS_PHIS8)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_M_CVL_ALPHACC)->EnableWindow(FALSE);  //(2009.06.15) Add by Unsang :: Only EC2-2:05, ColmDgn
		GetDlgItem(IDC_DGN_CON_M_CVL_ALPHACC4FLEX)->EnableWindow(FALSE);  //(2009.06.15) Add by Unsang :: Only EC2-2:05, ColmDgn
		GetDlgItem(IDC_DGN_CON_M_CVL_ALPHACC4SHEAR)->EnableWindow(FALSE);  //(2009.06.15) Add by Unsang :: Only EC2-2:05, ColmDgn
	}

	SetForShearWindowText(strCodeName);
}

void CDgnConPhiDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CDgnConPhiDlg::OnOK()
{
	UpdateData(TRUE);

	T_DCON_D rData;
	rData.Initialize();
	if (m_pDoc->m_pAttrCtrl->ExistDcon())	m_pDoc->m_pAttrCtrl->GetDcon(rData);
	else
	{
		T_PREFERENCE rPref;
		rPref.Initialize();
		m_pDoc->m_pInitCtrl->GetPreference(rPref);

		rData.DesignCode = m_strCode;
		rData.nNationalAnnex = rPref.DgnCode.nConcNationalAnnex;
	}

	GetDataFromDlg(rData);

	// Save RC Control Data at DB.
	BOOL bCheck = m_pDoc->m_pDataCtrl->AddDcon(rData);
	if (bCheck)  CDialogMove::OnOK();
}

void CDgnConPhiDlg::OnDgnPhiUpdateBtn()
{
	Set_DefaultByCode(m_strCode);
	UpdateData(FALSE);
}

void CDgnConPhiDlg::InitDlgCtrls()
{
	// TODO: Add your control notification handler code here
	// Change by ZINU.('02.7.8). For Dynamic Size.
	UpdateData(TRUE);
	GetDlgItem(IDC_DGN_REFPOS)->ShowWindow(SW_HIDE);
	int i = 0;
	for (i = 0; i < iDgn_FPHIID; i++)	GetDlgItem(m_iFPHIID[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_MPHIID1; i++)	GetDlgItem(m_iMPHIID1[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_MPHIID2; i++)	GetDlgItem(m_iMPHIID2[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_MPHIID3; i++)	GetDlgItem(m_iMPHIID3[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_MPHIID4; i++)	GetDlgItem(m_iMPHIID4[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_MPHIID5; i++)	GetDlgItem(m_iMPHIID5[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_MPHIID6; i++)	GetDlgItem(m_iMPHIID6[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_MPHIID7; i++)	GetDlgItem(m_iMPHIID7[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_MPHIID8; i++)	GetDlgItem(m_iMPHIID8[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_MPHIID9; i++)	GetDlgItem(m_iMPHIID9[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_MPHIID10; i++)	GetDlgItem(m_iMPHIID10[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_MPHIID11; i++)	GetDlgItem(m_iMPHIID11[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_MPHIID12; i++)	GetDlgItem(m_iMPHIID12[i])->ShowWindow(SW_HIDE);

	CRect RectDlg, RectOk, RectCn, RectOrg;
	GetWindowRect(&RectDlg);
	GetDlgItem(IDOK)->GetWindowRect(&RectOk);
	GetDlgItem(IDCANCEL)->GetWindowRect(&RectCn);

	int iStatus = GetDataStatus(m_strCode);

	CRect RectRef;
	GetDlgItem(IDC_DGN_REFPOS)->GetWindowRect(&RectRef);
	switch (iStatus)
	{
	case 1:
		RectOrg = MoveCtrl(1, m_iFPHIID, RectRef);
		break;
	case 2:
		RectOrg = MoveCtrl(2, m_iMPHIID1, RectRef);
		break;
	case 3:
		RectOrg = MoveCtrl(3, m_iMPHIID2, RectRef);
		break;
	case 4:
		RectOrg = MoveCtrl(4, m_iMPHIID3, RectRef);
		break;
	case 5:
		RectOrg = MoveCtrl(5, m_iMPHIID4, RectRef);
		break;
	case 6:
		RectOrg = MoveCtrl(6, m_iMPHIID5, RectRef);
		break;
	case 7:
	{
		RectOrg = MoveCtrl(6, m_iMPHIID5, RectRef);
		RectRef.top = RectOrg.bottom + (RectOrg.left - RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom - RectOrg.top);
		RectRef.left = RectOrg.left;
		RectRef.right = RectOrg.right;
		RectOrg = MoveCtrl(7, m_iMPHIID6, RectRef);
	}
	break;
	case 8:
		RectOrg = MoveCtrl(8, m_iMPHIID7, RectRef);
		break;
	case 9:
	{
		RectOrg = MoveCtrl(9, m_iMPHIID8, RectRef);
		RectRef.top = RectOrg.bottom + (RectOrg.left - RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom - RectOrg.top);
		RectRef.left = RectOrg.left;
		RectRef.right = RectOrg.right;
		const auto nAnnex = CNationalAnnexTool::ConvertNationalAnnex(m_strNationalAnnex);
		const auto bAlphacc4Shear = HasAlphacc4ShearCtrl(nAnnex);
		if (bAlphacc4Shear)
		{
			RectOrg = MoveCtrl(10, m_iMPHIID9, RectRef);
		}
		else
		{
			m_iMPHIID6[3] = IDC_DGN_CON_M_CVL_ALPHACC;
			RectOrg = MoveCtrl(7, m_iMPHIID6, RectRef);
		}
	}
	break; // added break
	case 10:
	{
		RectOrg = MoveCtrl(2, m_iMPHIID1, RectRef);
		RectRef.top = RectOrg.bottom + (RectOrg.left - RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom - RectOrg.top);
		RectRef.left = RectOrg.left;
		RectRef.right = RectOrg.right;
		RectOrg = MoveCtrl(11, m_iMPHIID10, RectRef);
	}
	break;
	case 11:
	{
		RectOrg = MoveCtrl(12, m_iMPHIID11, RectRef);
	}
	break;
	case 12:
	{
		RectOrg = MoveCtrl(13, m_iMPHIID12, RectRef);
		RectRef.top = RectOrg.bottom + (RectOrg.left - RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom - RectOrg.top);
		RectRef.left = RectOrg.left;
		RectRef.right = RectOrg.right;
		RectOrg = MoveCtrl(7, m_iMPHIID6, RectRef);
	}
	break;
	}

	CRect rDlg;
	rDlg.left = RectDlg.left;
	rDlg.top = RectDlg.top;
	rDlg.right = RectDlg.right;
	rDlg.bottom = RectOrg.bottom + globalUtils.ScaleByDPI(23) + RectOk.Height();

	CRect rOk;
	rOk.left = RectOk.left;
	rOk.right = RectOk.right;
	rOk.top = rDlg.bottom - globalUtils.ScaleByDPI(10) - RectOk.Height();
	rOk.bottom = rOk.top + RectOk.Height();
	CRect rCn;
	rCn.left = RectCn.left;
	rCn.right = RectCn.right;
	rCn.top = rOk.top;
	rCn.bottom = rOk.bottom;

	ScreenToClient(rDlg);
	ScreenToClient(rOk);
	ScreenToClient(rCn);
	SetWindowPos(NULL, rDlg.left, rDlg.top, rDlg.Width(), rDlg.Height(), SWP_NOMOVE | SWP_NOZORDER);
	GetDlgItem(IDOK)->SetWindowPos(NULL, rOk.left, rOk.top, rOk.Width(), rOk.Height(), SWP_NOSIZE);
	GetDlgItem(IDCANCEL)->SetWindowPos(NULL, rCn.left, rCn.top, rCn.Width(), rCn.Height(), SWP_NOSIZE);
}

CRect CDgnConPhiDlg::MoveCtrl(int iKind, int* pID, CRect Rect0)
{
	CRect RectOrg;
	int iArSize = 0;
	if (iKind == 1)		iArSize = iDgn_FPHIID;
	else if (iKind == 2)	iArSize = iDgn_MPHIID1;
	else if (iKind == 3)	iArSize = iDgn_MPHIID2;
	else if (iKind == 4)	iArSize = iDgn_MPHIID3;
	else if (iKind == 5)	iArSize = iDgn_MPHIID4;
	else if (iKind == 6)	iArSize = iDgn_MPHIID5;
	else if (iKind == 7)	iArSize = iDgn_MPHIID6;
	else if (iKind == 8)	iArSize = iDgn_MPHIID7;
	else if (iKind == 9)	iArSize = iDgn_MPHIID8;
	else if (iKind == 10)  iArSize = iDgn_MPHIID9;
	else if (iKind == 11)  iArSize = iDgn_MPHIID10;
	else if (iKind == 12)  iArSize = iDgn_MPHIID11;
	else if (iKind == 13)  iArSize = iDgn_MPHIID12;
	else	ASSERT(0);

	CRect* pRectCng = new CRect[iArSize];

	//CRect Rect0, Rect1;
	//GetDlgItem(m_iFPHIID[0])->GetWindowRect(&Rect0);
	CRect Rect1;
	for (int i = 0; i < iArSize; i++)
	{
		GetDlgItem(pID[i])->ShowWindow(SW_SHOW);
		GetDlgItem(pID[i])->GetWindowRect(&(pRectCng[i]));
		if (i == 0)	Rect1 = pRectCng[i];
		pRectCng[i].top -= Rect1.top - Rect0.top;
		pRectCng[i].bottom -= Rect1.top - Rect0.top;
	}
	RectOrg = pRectCng[0];
	for (int i = 0; i < iArSize; i++)
	{
		ScreenToClient(pRectCng[i]);
		GetDlgItem(pID[i])->SetWindowPos(NULL, pRectCng[i].left, pRectCng[i].top, pRectCng[i].Width(), pRectCng[i].Height(), SWP_NOSIZE | SWP_NOZORDER);
	}
	delete[] pRectCng;

	return RectOrg;
}

int CDgnConPhiDlg::GetDataStatus(CString strCode)
{
	if (strCode == CONCODE_CSA_A23_3_94) return 2;
	if (strCode == CONCODE_CSA_S6_00) return 2;
	if (strCode == CONCODE_CSA_S6_14) return 10;
	if (strCode == CONCODE_CSA_S6_19) return 10;
	if (strCode == CONCODE_BS8110_97) return 3;
	if (strCode == CONCODE_GB50010_02) return 4;
	if (strCode == CONCODE_JTJ023_85) return 4;
	if (strCode == CONCODE_GB50010_10) return 4;  // Modify by GAY. MQC:5045. 
	if (strCode == CONCODE_IS456_2000) return 5;
	if (strCode == CONCODE_EC2) return 6;
	if (strCode == CONCODE_EC2_04) return 7;  // Coded by Seungjun ('20070613) MNet:No.2313.
	if (strCode == CONCODE_KCI_USD07) return 8;  // Coded by sshan ('090602) MNet:No..
	if (strCode == CONCODE_KCI_USD12) return 8;
	if (strCode == CONCODE_KDS_41_30_2018) return 8;
	if (strCode == CONCODE_KDS_41_20_2022) return 8;
	if (strCode == CONCODE_ACI318_08) return 8;
	if (strCode == CONCODE_ACI318_11) return 8;
	if (strCode == CONCODE_ACI318_14) return 8;
	if (strCode == CONCODE_ACI318M_14) return 8;
	if (strCode == CONCODE_ACI318_19) return 8;
	if (strCode == CONCODE_ACI318M_19) return 8;
	if (strCode == CONCODE_ACI318_25) return 8;
	if (strCode == CONCODE_ACI318M_25) return 8;
	if (strCode == CONCODE_NSR_10) return 8;  // Add by GAY. PMS:4450. ('12.03.22). Phi Type : Tension Control, Compression Control(Spiral,Tie), Shear and Torsion.	
	if (strCode == CONCODE_NSCP_2015) return 8;
	if (strCode == CONCODE_NTC_DCEC_2017) return 8;
	if (strCode == CONCODE_NTC_DCEC_2023) return 8;
	if (strCode == CONCODE_EC2_2_05) return 9;
	if (strCode == CONCODE_BS5400_90) return 3;
	if (strCode == CONCODE_TMH07_89) return 3;
	if (strCode == CONCODE_IRC112_2011) return 9;  //(2009.06.12) Add by Unsang // Only Civil!
	if (strCode == CONCODE_IRC112_2020) return 9;  //(2009.06.12) Add by Unsang // Only Civil!
	if (strCode == CONCODE_AASHTO_LRFD07) return 8;
	if (strCode == CONCODE_AASHTO_LRFD12) return 8;
	if (strCode == CONCODE_AASHTO_LRFD16) return 8;
	if (strCode == CONCODE_AASHTO_LRFD17) return 8;
	if (strCode == CONCODE_AASHTO_LRFD20) return 8;
	if (strCode == CONCODE_AASHTO_LRFD24) return 8;
	if (strCode == CONCODE_AREMA_2023) return 8;
	if (strCode == CONCODE_IRS)		  return 11;
	else if(strCode == CONCODE_SP_63_13330_2018)	return 12;

	return 1;
}

void CDgnConPhiDlg::SetForShearWindowText(CString strCode)
{
	if (strCode == CONCODE_TWN_USD92 || strCode == CONCODE_TWN_USD100 || strCode == CONCODE_TWN_USD112)
		GetDlgItem(IDC_DGN_CON_F_PHIS_STR)->SetWindowText(_LS(IDS_DGN_STR_PHI_FOR_SHEAR_TORSION));
	else
		GetDlgItem(IDC_DGN_CON_F_PHIS_STR)->SetWindowText(_LS(IDS_DGN_STR_PHI_FOR_SHEAR));
}

bool CDgnConPhiDlg::HasAlphacc4ShearCtrl(int nAnnex)
{
	return (nAnnex == Luxembourg || nAnnex == UnitedKingdom);
}