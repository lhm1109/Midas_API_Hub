// DconChildDialog.cpp : implementation file
//

#include "stdafx.h"

#include "..\wg_db\DBCodeDef.h"

#include "DconChildDialog.h"
#include "DgnConCodeNewDlg.h"

#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDconChildDialog dialog

CDconChildDialog::CDconChildDialog(UINT resID,CDgnConCodeNewDlg* pParent /*=NULL*/, T_DCON_D* pData, BOOL bExistData/*=FALSE*/)
	: CChildDialog(resID, pParent)
{
	//{{AFX_DATA_INIT(CDconChildDialog)
	CChildDialog::resID = resID;
	m_pDconParent = pParent;
	m_pData = pData;
	m_bExistData = bExistData;
	if(bExistData && m_pData != NULL)
	{
		m_bSpecialEQ = pData->bSpecialSeismic;

		int iStatus = GetDataStatus(pData->DesignCode);
		m_iColmMethod	= (iStatus==1 || iStatus==9               ? pData->nClass : 0);
		m_iSafeLevel  = (iStatus==2                             ? pData->nClass : 0);
		m_iEqGrade    = (iStatus==3                             ? pData->nClass : 0);
		m_iPmCurve    = (iStatus==4                             ? pData->nClass : 0);
		m_iEqCategory	= (iStatus==6 || iStatus==7 || iStatus==8 || iStatus==19 ? pData->nClass : 0);
		m_iSlabType   = pData->iSlabType;
		m_iFrameType  = (iStatus==10 || iStatus==18 || iStatus==20 ? pData->nClass : 0);
		m_iDuctility  = (iStatus==17                               ? pData->nClass : 0);
	}
	else
	{
		m_bSpecialEQ = FALSE;

		m_iColmMethod	= 0;
		m_iSafeLevel	= 0;
		m_iEqGrade		= 0;
		m_iPmCurve		= 0;
		m_iEqCategory = 0;
		m_iSlabType   = 0;
		m_iFrameType  = 0;
		m_iDuctility	= 0;
	}
	m_bIsCreated = FALSE;

    m_pDgnDataCtrl = new CDgnDataCtrl;

}
CDconChildDialog::~CDconChildDialog()
{
    _SAFE_DELETE(m_pDgnDataCtrl);
}

void CDconChildDialog::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDconChildDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDconChildDialog, CChildDialog)
	//{{AFX_MSG_MAP(CDconChildDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDconChildDialog message handlers
void CDconChildDialog::ResizeChildDlg()
{
	if(m_pDconParent==nullptr) return;
	 m_pDconParent->ResizeDialog(this);
}

void CDconChildDialog::SetCodeType(int nCodeType)
{
	//m_nCodeType = nCodeType;
}
int CDconChildDialog::GetDataStatus(const CString& strCode)
{
	int iStatus=0;
	BOOL bKoreanUSD = FALSE;
	
    if ( strCode == CONCODE_AIJ_WSD99 ) { iStatus = (m_bSpecialEQ ? 9 : 1); }
    else if ( strCode==CONCODE_GB50010_02 ) { iStatus = (m_bSpecialEQ ? 3 : 2); }
    else if ( strCode==CONCODE_GB50010_10 ) { iStatus = (m_bSpecialEQ ? 3 : 2); }
    else if ( bKoreanUSD )                  { iStatus = 4; }
    else if ( strCode==CONCODE_JTJ023_85 )  { iStatus = 5; }
    else if ( strCode==CONCODE_AASHTO_LRFD02 ) { iStatus = (m_bSpecialEQ ? 6 : 0); }
    else if ( strCode==CONCODE_AASHTO_LRFD07 ) { iStatus = (m_bSpecialEQ ? 19: 0); }
    else if ( strCode==CONCODE_AASHTO_LRFD12 ) { iStatus = (m_bSpecialEQ ? 19: 0); }
    else if ( strCode==CONCODE_AASHTO_LRFD16 ) { iStatus = (m_bSpecialEQ ? 19: 0); }
    else if ( strCode==CONCODE_AASHTO_LRFD17 ) { iStatus = (m_bSpecialEQ ? 19: 0); }
    else if ( strCode==CONCODE_AASHTO_LRFD20 ) { iStatus = (m_bSpecialEQ ? 19: 0); }
	else if ( strCode==CONCODE_AASHTO_LRFD24 ) { iStatus = (m_bSpecialEQ ? 19: 0); }
    else if ( strCode==CONCODE_CSA_S6_00 )     { iStatus = (m_bSpecialEQ ? 7 : 0); }
    else if ( strCode==CONCODE_CSA_S6_14 )     { iStatus = (m_bSpecialEQ ? 7 : 0); }
	else if ( strCode==CONCODE_CSA_S6_19 )     { iStatus = (m_bSpecialEQ ? 7 : 0); }
    else if ( strCode==CONCODE_AASHTO_LFD96 )  { iStatus = (m_bSpecialEQ ? 8 : 0); }
    else if ( strCode==CONCODE_BS8110_97 )     { iStatus = -1; } // Non-seismic.
    else if ( strCode==CONCODE_EC2 )           { iStatus = -1; }	// Non-seismic.
    else if ( strCode==CONCODE_EC2_04 )        { iStatus = /*17;*/(m_bSpecialEQ ? 17 : 0); }	// Coded by sshan ('20080108)
    else if ( strCode==CONCODE_IRC21_2000 )    { iStatus = -1; }	// Non-seismic.
    else if ( strCode==CONCODE_IRC112_2011 )   { iStatus = -1; } // Non-seismic.
    else if ( strCode==CONCODE_IRC112_2020 )   { iStatus = -1; } // Non-seismic.
    else if ( strCode==CONCODE_ACI318_89 )     { iStatus = (m_bSpecialEQ ? 10 : 0); }
    else if ( strCode==CONCODE_ACI318_95 )     { iStatus = (m_bSpecialEQ ? 10 : 0); }
    else if ( strCode==CONCODE_ACI318_99 )     { iStatus = (m_bSpecialEQ ? 10 : 0); }
    else if ( strCode==CONCODE_ACI318_02 )     { iStatus = (m_bSpecialEQ ? 10 : 0); }
    else if ( strCode==CONCODE_ACI318_05 )     { iStatus = (m_bSpecialEQ ? 10 : 0); } 
    else if ( strCode==CONCODE_ACI318_08 )     { iStatus = (m_bSpecialEQ ? 20 : 0); } // Add by GAY. PMS:4450. ('12.03.12)
    else if ( strCode==CONCODE_ACI318_11 )     { iStatus = (m_bSpecialEQ ? 20 : 0); }// Add by GAY. PMS:4450. ('12.03.12)
    else if ( strCode==CONCODE_ACI318_14 )     { iStatus = (m_bSpecialEQ ? 20 : 0); }
    else if ( strCode==CONCODE_ACI318M_14)     { iStatus = (m_bSpecialEQ ? 20 : 0); }
    else if ( strCode==CONCODE_ACI318_19 )     { iStatus = (m_bSpecialEQ ? 20 : 0); }
    else if ( strCode==CONCODE_ACI318M_19)     { iStatus = (m_bSpecialEQ ? 20 : 0); }
	else if ( strCode==CONCODE_ACI318_25 )     { iStatus = (m_bSpecialEQ ? 20 : 0); }
    else if ( strCode==CONCODE_ACI318M_25)     { iStatus = (m_bSpecialEQ ? 20 : 0); }
    else if ( strCode==CONCODE_NSR_10 )        { iStatus = (m_bSpecialEQ ? 20 : 0); }// by GAY. PMS:5009. ('15.04.03).
    else if ( strCode==CONCODE_NSCP_2015 )     { iStatus = (m_bSpecialEQ ? 20 : 0); }
	else if ( strCode==CONCODE_NTC_DCEC_2017 ) { iStatus = (m_bSpecialEQ ? 20 : 0); }
	else if ( strCode==CONCODE_NTC_DCEC_2023 ) { iStatus = (m_bSpecialEQ ? 20 : 0); }
    else if ( strCode==CONCODE_AIK_USD94 )     { iStatus = (m_bSpecialEQ ? 11 : 0); }
    else if ( strCode==CONCODE_KSCE_USD96 )    { iStatus = (m_bSpecialEQ ? 12 : 0); }
    else if ( strCode==CONCODE_KSCE_USD05 )    { iStatus = -1; }	// Non-seismic.
    else if ( strCode==CONCODE_KSCE_RAIL_USD04 ) { iStatus = -1; }	// Non-seismic.
    else if ( strCode==CONCODE_KCI_USD99 )     { iStatus = (m_bSpecialEQ ? 13 : 0); }
    else if ( strCode==CONCODE_KCI_USD03 )     { iStatus = (m_bSpecialEQ ? 13 : 0); }
    else if ( strCode==CONCODE_KCI_USD07 || strCode==CONCODE_KCI_USD12 ||
        strCode == CONCODE_KDS_41_30_2018 || strCode == CONCODE_KDS_41_20_2022 )
    {
#if defined(_CIVIL)
        iStatus = -1;	// Non-seismic.
#else
        iStatus = (m_bSpecialEQ ? 18 : 0);
#endif
    }
    else if ( strCode==CONCODE_CSA_A23_3_94 ) { iStatus = (m_bSpecialEQ ? 14 : 0); }
    else if ( strCode==CONCODE_TWN_USD92 )    { iStatus = (m_bSpecialEQ ? 15 : 0); }
    else if ( strCode==CONCODE_TWN_USD100 )   { iStatus = (m_bSpecialEQ ? 15 : 0); }
    else if ( strCode==CONCODE_TWN_USD112 )   { iStatus = (m_bSpecialEQ ? 15 : 0); }
    else if ( strCode==CONCODE_IS456_2000 )   { iStatus = (m_bSpecialEQ ? 16 : 0); }
    else if ( strCode==CONCODE_EC2_2_05 )     { iStatus = -1; }	// Non-seismic.
    else if ( strCode==CONCODE_SNIP_2_05_03_84 )  { iStatus = -1; }	// Non-seismic.
    else if ( strCode==CONCODE_SP_35_13330_2011 ) { iStatus = -1; }	// Non-seismic.
    else if ( strCode==CONCODE_SNIP_2_05_03_84_MKS )  { iStatus = -1; }	// Non-seismic.
    else if ( strCode==CONCODE_SP_35_13330_2011_MKS ) { iStatus = -1; }// Non-seismic.
    else if ( strCode==CONCODE_IS456_2000 )           { iStatus = -1; } // Non-seismic
    else { iStatus = 0; }

#if defined(_IGEN)
	if ( strCode == CONCODE_AIJ_WSD99 )
	{
		iStatus = 0;
	}
#endif

	return iStatus;
}

void CDconChildDialog::SetDesignCode(const CString& strDgnCode)
{
	ChangeDesignCode();
}

void CDconChildDialog::UpdateChildData()
{
	m_bSpecialEQ = m_pData->bSpecialSeismic;
	int iStatus = GetDataStatus(m_pData->DesignCode);

	if(iStatus==1 || iStatus==9) m_pData->nClass = m_iColmMethod;
	else if(iStatus==2)	         m_pData->nClass = m_iSafeLevel ;
	else if(iStatus==3)	         m_pData->nClass = m_iEqGrade   ;
	else if(iStatus==4)	         m_pData->nClass = m_iPmCurve   ;
	else if(iStatus==6)	         m_pData->nClass = m_iEqCategory;
	else if(iStatus==7)	         m_pData->nClass = m_iEqCategory;
	else if(iStatus==8)	         m_pData->nClass = m_iEqCategory;
	else if(iStatus==10)         m_pData->nClass = m_iFrameType ;
	else if(iStatus==17)         m_pData->nClass = m_iDuctility ;
	else if(iStatus==18)         m_pData->nClass = m_iFrameType ;
	else if(iStatus==19)	     m_pData->nClass = m_iEqCategory;
	else if(iStatus==20)         m_pData->nClass = m_iFrameType ;
	else						 m_pData->nClass = 0            ;
}

BOOL CDconChildDialog::Dlg2Data()
{
	// Get Special Seismic and Uni Axis.
	m_pData->bSpecialSeismic = m_bSpecialEQ;

	int iStatus = GetDataStatus(m_pData->DesignCode);

	if(iStatus==1 || iStatus==9)  m_pData->nClass = m_iColmMethod;
	else if(iStatus==2)	          m_pData->nClass = m_iSafeLevel ;
	else if(iStatus==3)	          m_pData->nClass = m_iEqGrade   ;
	else if(iStatus==4)	          m_pData->nClass = m_iPmCurve   ;
	else if(iStatus==6)	          m_pData->nClass = m_iEqCategory;
	else if(iStatus==7)	          m_pData->nClass = m_iEqCategory;
	else if(iStatus==8)	          m_pData->nClass = m_iEqCategory;
	else if(iStatus==10)          m_pData->nClass = m_iFrameType ;
	else if(iStatus==17)          m_pData->nClass = m_iDuctility ;//Eurocode2:04
	else if(iStatus==18)          m_pData->nClass = m_iFrameType ;//KCI-USD07
	else if(iStatus==19)	      m_pData->nClass = m_iEqCategory;
	else if(iStatus==20)          m_pData->nClass = m_iFrameType ; // Modify by GAY. PMS:4450. ('12.05.14). ACI318-08, 11.
	else						  m_pData->nClass = 0            ;

	return TRUE;
}