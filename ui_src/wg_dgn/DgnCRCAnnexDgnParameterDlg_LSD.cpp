// DgnCRCAnnexDgnParameterDlg_LSD.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "DgnCRCAnnexDgnParameterDlg_LSD.h"

#include "DgnStruct.h"
//#include "DgnPscCommon.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\DBCodeDef.h"

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
CDgnCRCAnnexDgnParameterDlg_LSD::CDgnCRCAnnexDgnParameterDlg_LSD(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnCRCAnnexDgnParameterDlg_LSD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConMatDlg)
	//}}AFX_DATA_INIT
}

void CDgnCRCAnnexDgnParameterDlg_LSD::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConMatDlg)
	DDX_Control(pDX, IDC_LSDPSC_PARTIAL_PARAMETER3,     m_dPartialFac_c_U);
	DDX_Control(pDX, IDC_LSDPSC_PARTIAL_PARAMETER5,     m_dPartialFac_s_U);
	DDX_Control(pDX, IDC_LSDPSC_PARTIAL_PARAMETER7,     m_dPartialFac_ps_U);

	DDX_Control(pDX, IDC_LSDPSC_PARTIAL_PARAMETER10,    m_dPartialFac_c_S);
	DDX_Control(pDX, IDC_LSDPSC_PARTIAL_PARAMETER12,    m_dPartialFac_s_S);
	DDX_Control(pDX, IDC_LSDPSC_PARTIAL_PARAMETER14,    m_dPartialFac_ps_S);

	//
	DDX_Control(pDX, IDC_EC2PSC_STRESSLIM_PARAMETER3,   m_dStressLim_c_k1);
	DDX_Control(pDX, IDC_EC2PSC_STRESSLIM_PARAMETER5,   m_dStressLim_c_k3);
	DDX_Control(pDX, IDC_EC2PSC_STRESSLIM_PARAMETER7,   m_dStressLim_c_k4);
	DDX_Control(pDX, IDC_EC2PSC_STRESSLIM_PARAMETER9,   m_dStressLim_c_k6);
	DDX_Control(pDX, IDC_EC2PSC_STRESSLIM_PARAMETER12,  m_dStressLim_s_k1);
	DDX_Control(pDX, IDC_EC2PSC_STRESSLIM_PARAMETER14,  m_dStressLim_s_k2);
	DDX_Control(pDX, IDC_EC2PSC_STRESSLIM_PARAMETER16,  m_dStressLim_s_k5);
	DDX_Control(pDX, IDC_EC2PSC_STRESSLIM_PARAMETER18,  m_dStressLim_s_k7);
	DDX_Control(pDX, IDC_EC2PSC_STRESSLIM_PARAMETER20,  m_dStressLim_s_k8);
	//
	DDX_Control(pDX, IDC_EC2PSC_PRINCIPAL_PARAMETER3,   m_dk_c_ps_stage);
	DDX_Control(pDX, IDC_EC2PSC_PRINCIPAL_PARAMETER5,   m_dk_t_ps_stage);
	DDX_Control(pDX, IDC_EC2PSC_PRINCIPAL_PARAMETER8,   m_dk_c_ps);
	DDX_Control(pDX, IDC_EC2PSC_PRINCIPAL_PARAMETER10,  m_dk_t_ps);
	//

	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnCRCAnnexDgnParameterDlg_LSD, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnConMatDlg)
	ON_BN_CLICKED(IDC_LSDPSC_CODE_PARAMETER3,   OnResetDgnParam_byAnnex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConMatDlg message handlers

BOOL CDgnCRCAnnexDgnParameterDlg_LSD::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	Initial_Data();

	return TRUE;
}

void CDgnCRCAnnexDgnParameterDlg_LSD::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnCRCAnnexDgnParameterDlg_LSD::OnOK() 
{
	UpdateData(TRUE);

	Dlg2Data();

	CDialogMove::OnOK();
}

void CDgnCRCAnnexDgnParameterDlg_LSD::Dlg2Data()
{                          
	m_pParamData->dPhi_c[0]  = m_dPartialFac_c_U.GetEditValue();
	m_pParamData->dPhi_s[0]  = m_dPartialFac_s_U.GetEditValue();
	m_pParamData->dPhi_ps[0] = m_dPartialFac_ps_U.GetEditValue();
	m_pParamData->dPhi_c[2]  = m_dPartialFac_c_S.GetEditValue();
	m_pParamData->dPhi_s[2]  = m_dPartialFac_s_S.GetEditValue();
	m_pParamData->dPhi_ps[2] = m_dPartialFac_ps_S.GetEditValue();

	//
	m_pParamData->dk1_sl        = m_dStressLim_c_k1.GetEditValue();
	m_pParamData->dk3_sl        = m_dStressLim_c_k3.GetEditValue();
	m_pParamData->dk4_sl        = m_dStressLim_c_k4.GetEditValue();
	m_pParamData->dk6_sl        = m_dStressLim_c_k6.GetEditValue();
	m_pParamData->dk1_ps        = m_dStressLim_s_k1.GetEditValue();
	m_pParamData->dk2_ps        = m_dStressLim_s_k2.GetEditValue();
	m_pParamData->dk5_ps        = m_dStressLim_s_k5.GetEditValue();
	m_pParamData->dk7_ps        = m_dStressLim_s_k7.GetEditValue();
	m_pParamData->dk8_ps        = m_dStressLim_s_k8.GetEditValue();
	//
	m_pParamData->dk_c_ps_stage = m_dk_c_ps_stage.GetEditValue();
	m_pParamData->dk_t_ps_stage = m_dk_t_ps_stage.GetEditValue();
	m_pParamData->dk_c_ps       = m_dk_c_ps.GetEditValue();
	m_pParamData->dk_t_ps       = m_dk_t_ps.GetEditValue();
	//

}

void CDgnCRCAnnexDgnParameterDlg_LSD::Initial_Data()
{
	// Eurocode2-2:05!!
	CString strCode = _T("");  
	strCode = CDBLib::GetPscCodeName(m_pParamData->iDgnCode);

	GetDlgItem(IDC_LSDPSC_CODE_PARAMETER0)->SetWindowText(strCode);  // DgnCode!!

	// SET UNIT!!
	m_dPartialFac_c_U.SetUnitType(D_UNITSYS_NONE);
	m_dPartialFac_c_S.SetUnitType(D_UNITSYS_NONE);
	m_dPartialFac_ps_U.SetUnitType(D_UNITSYS_NONE);
	m_dPartialFac_s_U.SetUnitType(D_UNITSYS_NONE);
	m_dPartialFac_s_S.SetUnitType(D_UNITSYS_NONE);
	m_dPartialFac_ps_S.SetUnitType(D_UNITSYS_NONE);
	//
	m_dStressLim_c_k1.SetUnitType(D_UNITSYS_NONE);
	m_dStressLim_c_k3.SetUnitType(D_UNITSYS_NONE);
	m_dStressLim_c_k4.SetUnitType(D_UNITSYS_NONE);
	m_dStressLim_c_k6.SetUnitType(D_UNITSYS_NONE);
	m_dStressLim_s_k1.SetUnitType(D_UNITSYS_NONE);
	m_dStressLim_s_k2.SetUnitType(D_UNITSYS_NONE);
	m_dStressLim_s_k5.SetUnitType(D_UNITSYS_NONE);
	m_dStressLim_s_k7.SetUnitType(D_UNITSYS_NONE);
	m_dStressLim_s_k8.SetUnitType(D_UNITSYS_NONE);
	//
	m_dk_c_ps_stage.SetUnitType(D_UNITSYS_NONE);
	m_dk_t_ps_stage.SetUnitType(D_UNITSYS_NONE);
	m_dk_c_ps.SetUnitType(D_UNITSYS_NONE);
	m_dk_t_ps.SetUnitType(D_UNITSYS_NONE);
	//


	// SET Default DATA by ANNEX!!
	T_PSCD_D ParamData; ParamData.Initialize();
	//GetDefaultDatabyAnnex(ParamData, m_nAnnex);
	ParamData.Set_AnnexData_LSD();

	m_dPartialFac_c_U.SetEditUnit( m_pParamData->dPhi_c[0]==0.0? ParamData.dPhi_c[0] : m_pParamData->dPhi_c[0]);
	m_dPartialFac_s_U.SetEditUnit( m_pParamData->dPhi_s[0]==0.0? ParamData.dPhi_s[0] : m_pParamData->dPhi_s[0]);
	m_dPartialFac_ps_U.SetEditUnit( m_pParamData->dPhi_ps[0]==0.0? ParamData.dPhi_ps[0] : m_pParamData->dPhi_ps[0]);
	m_dPartialFac_c_S.SetEditUnit( m_pParamData->dPhi_c[2]==0.0? ParamData.dPhi_c[2] : m_pParamData->dPhi_c[2]);  
	m_dPartialFac_s_S.SetEditUnit( m_pParamData->dPhi_s[2]==0.0? ParamData.dPhi_s[2] : m_pParamData->dPhi_s[2]);   
	m_dPartialFac_ps_S.SetEditUnit( m_pParamData->dPhi_ps[2]==0.0? ParamData.dPhi_ps[2] : m_pParamData->dPhi_ps[2]);   
	//
	m_dStressLim_c_k1.SetEditUnit(m_pParamData->dk1_sl==0.0? ParamData.dk1_sl : m_pParamData->dk1_sl);
	m_dStressLim_c_k3.SetEditUnit(m_pParamData->dk3_sl==0.0? ParamData.dk3_sl : m_pParamData->dk3_sl);
	m_dStressLim_c_k4.SetEditUnit(m_pParamData->dk4_sl==0.0? ParamData.dk4_sl : m_pParamData->dk4_sl);
	m_dStressLim_c_k6.SetEditUnit(m_pParamData->dk6_sl==0.0? ParamData.dk6_sl : m_pParamData->dk6_sl);
	m_dStressLim_s_k1.SetEditUnit(m_pParamData->dk1_ps==0.0? ParamData.dk1_ps : m_pParamData->dk1_ps);
	m_dStressLim_s_k2.SetEditUnit(m_pParamData->dk2_ps==0.0? ParamData.dk2_ps : m_pParamData->dk2_ps);
	m_dStressLim_s_k5.SetEditUnit(m_pParamData->dk5_ps==0.0? ParamData.dk5_ps : m_pParamData->dk5_ps);
	m_dStressLim_s_k7.SetEditUnit(m_pParamData->dk7_ps==0.0? ParamData.dk7_ps : m_pParamData->dk7_ps);
	m_dStressLim_s_k8.SetEditUnit(m_pParamData->dk8_ps==0.0? ParamData.dk8_ps : m_pParamData->dk8_ps);
	//
	m_dk_c_ps_stage.SetEditUnit(m_pParamData->dk_c_ps_stage==0.0? ParamData.dk_c_ps_stage : m_pParamData->dk_c_ps_stage);
	m_dk_t_ps_stage.SetEditUnit(m_pParamData->dk_t_ps_stage==0.0? ParamData.dk_t_ps_stage : m_pParamData->dk_t_ps_stage);
	m_dk_c_ps.SetEditUnit(m_pParamData->dk_c_ps==0.0? ParamData.dk_c_ps : m_pParamData->dk_c_ps);
	m_dk_t_ps.SetEditUnit(m_pParamData->dk_t_ps==0.0? ParamData.dk_t_ps : m_pParamData->dk_t_ps);
	//  


}

void CDgnCRCAnnexDgnParameterDlg_LSD::OnResetDgnParam_byAnnex()
{
	m_pParamData->Set_AnnexData_LSD();
	Initial_Data();
}

