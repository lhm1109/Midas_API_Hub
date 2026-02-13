// RatingParamPSCLSD15Annex.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "RatingParamPSCLSD15Annex.h"

#include "DgnStruct.h"
//#include "DgnPscCommon.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\wg_base\wg_base_LocaleLib.h"

#include "..\wg_base\MSExcel.h"
#include "..\wg_base\XLParser.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConMatDlg dialog
CRatingParamPSCLSDAnnex::CRatingParamPSCLSDAnnex(CWnd* pParent /*=NULL*/)
	: CDialogMove(CRatingParamPSCLSDAnnex::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();

}

void CRatingParamPSCLSDAnnex::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConMatDlg)
	DDX_Control(pDX, IDC_RATING_LSDPSC_PARTIAL_EDT1,    m_dPartialFac_c_U);
	DDX_Control(pDX, IDC_RATING_LSDPSC_PARTIAL_EDT2,    m_dPartialFac_s_U);
	DDX_Control(pDX, IDC_RATING_LSDPSC_PARTIAL_EDT3,    m_dPartialFac_ps_U);

	DDX_Control(pDX, IDC_RATING_LSDPSC_PARTIAL_EDT4,    m_dPartialFac_c_S);
	DDX_Control(pDX, IDC_RATING_LSDPSC_PARTIAL_EDT5,    m_dPartialFac_s_S);
	DDX_Control(pDX, IDC_RATING_LSDPSC_PARTIAL_EDT6,    m_dPartialFac_ps_S);

	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CRatingParamPSCLSDAnnex, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnConMatDlg)
	ON_BN_CLICKED(IDC_RATING_LSDPSC_CODE_PARAMETER1,   OnResetDgnParam_byAnnex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConMatDlg message handlers

BOOL CRatingParamPSCLSDAnnex::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	Initial_Data();

	return TRUE;
}

void CRatingParamPSCLSDAnnex::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CRatingParamPSCLSDAnnex::OnOK() 
{
	UpdateData(TRUE);

	Dlg2Data();

	if(!m_pDoc->m_pDataCtrl->AddRakr(m_Data))
		return;

	CDialogMove::OnOK();		

}

void CRatingParamPSCLSDAnnex::Dlg2Data()
{              
	m_Data.dPhi_c[0]  = m_dPartialFac_c_U.GetEditValue();
	m_Data.dPhi_s[0]  = m_dPartialFac_s_U.GetEditValue();
	m_Data.dPhi_ps[0] = m_dPartialFac_ps_U.GetEditValue();
	m_Data.dPhi_c[2]  = m_dPartialFac_c_S.GetEditValue();
	m_Data.dPhi_s[2]  = m_dPartialFac_s_S.GetEditValue();
	m_Data.dPhi_ps[2] = m_dPartialFac_ps_S.GetEditValue();

}



void CRatingParamPSCLSDAnnex::Initial_Data()
{

	// SET UNIT!!
	m_dPartialFac_c_U.SetUnitType(D_UNITSYS_NONE);
	m_dPartialFac_c_S.SetUnitType(D_UNITSYS_NONE);
	m_dPartialFac_ps_U.SetUnitType(D_UNITSYS_NONE);
	m_dPartialFac_s_U.SetUnitType(D_UNITSYS_NONE);
	m_dPartialFac_s_S.SetUnitType(D_UNITSYS_NONE);
	m_dPartialFac_ps_S.SetUnitType(D_UNITSYS_NONE);

	// SET Default DATA by ANNEX!!
	T_RAKR_D Data; Data.Initialize();
	//GetDefaultDatabyAnnex(ParamData, m_nAnnex);
	Data.Set_PSC_Rating_AnnexData_LSD();

	m_dPartialFac_c_U.SetEditUnit (m_Data.dPhi_c[0]==0.0?  Data.dPhi_c[0]		: m_Data.dPhi_c[0]);
	m_dPartialFac_s_U.SetEditUnit (m_Data.dPhi_s[0]==0.0?  Data.dPhi_s[0]		: m_Data.dPhi_s[0]);
	m_dPartialFac_ps_U.SetEditUnit(m_Data.dPhi_ps[0]==0.0? Data.dPhi_ps[0]	: m_Data.dPhi_ps[0]);
	m_dPartialFac_c_S.SetEditUnit (m_Data.dPhi_c[2]==0.0?  Data.dPhi_c[2]		: m_Data.dPhi_c[2]);  
	m_dPartialFac_s_S.SetEditUnit (m_Data.dPhi_s[2]==0.0?  Data.dPhi_s[2]		: m_Data.dPhi_s[2]);   
	m_dPartialFac_ps_S.SetEditUnit(m_Data.dPhi_ps[2]==0.0? Data.dPhi_ps[2]	: m_Data.dPhi_ps[2]);   

}

void CRatingParamPSCLSDAnnex::OnResetDgnParam_byAnnex()
{
	m_Data.Set_PSC_Rating_AnnexData_LSD();
	Initial_Data();
}

