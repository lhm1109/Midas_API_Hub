// DgnCRCAnnexDgnParameter_ABNT_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "DgnCRCAnnexDgnParameter_ABNT_Dlg.h"

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
#include "NationalAnnexTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace dgn::def;

/////////////////////////////////////////////////////////////////////////////
// CDgnConMatDlg dialog
CDgnCRCAnnexDgnParameterABNTDlg::CDgnCRCAnnexDgnParameterABNTDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnCRCAnnexDgnParameterABNTDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConMatDlg)
	//}}AFX_DATA_INIT
}

void CDgnCRCAnnexDgnParameterABNTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConMatDlg)
	DDX_Control(pDX, IDC_DGN_GAMMA_C_NORMAL_EDT, m_Gamma_c_N_NBR);
	DDX_Control(pDX, IDC_DGN_GAMMA_C_SPECIAL_EDT, m_Gamma_c_SC_NBR);
	DDX_Control(pDX, IDC_DGN_GAMMA_C_EXCEPTIONAL_EDT, m_Gamma_c_E_NBR);
	DDX_Control(pDX, IDC_DGN_GAMMA_S_NORMAL_EDT, m_Gamma_s_N_NBR);
	DDX_Control(pDX, IDC_DGN_GAMMA_S_SPECIAL_EDT, m_Gamma_s_SC_NBR);
	DDX_Control(pDX, IDC_DGN_GAMMA_S_EXCEPTIONAL_EDT, m_Gamma_s_E_NBR);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnCRCAnnexDgnParameterABNTDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnConMatDlg)
	ON_BN_CLICKED(IDC_DGN_UPDATE_BY_CODE_BTN, OnResetDgnParam_byAnnex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConMatDlg message handlers

BOOL CDgnCRCAnnexDgnParameterABNTDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	Initial_Data();

	return TRUE;
}

void CDgnCRCAnnexDgnParameterABNTDlg::OnCancel()
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnCRCAnnexDgnParameterABNTDlg::OnOK()
{
	UpdateData(TRUE);

	Dlg2Data();

	CDialogMove::OnOK();
}

void CDgnCRCAnnexDgnParameterABNTDlg::Dlg2Data()
{
	m_pParamData->dGamma_c_N_NBR = m_Gamma_c_N_NBR.GetEditValue();
	m_pParamData->dGamma_c_SC_NBR = m_Gamma_c_SC_NBR.GetEditValue();
	m_pParamData->dGamma_c_E_NBR = m_Gamma_c_E_NBR.GetEditValue();
	m_pParamData->dGamma_s_N_NBR = m_Gamma_s_N_NBR.GetEditValue();
	m_pParamData->dGamma_s_SC_NBR = m_Gamma_s_SC_NBR.GetEditValue();
	m_pParamData->dGamma_s_E_NBR = m_Gamma_s_E_NBR.GetEditValue();
}

void CDgnCRCAnnexDgnParameterABNTDlg::Initial_Data()
{
	// SET UNIT!!
	m_Gamma_c_N_NBR.SetUnitType(D_UNITSYS_NONE);
	m_Gamma_c_SC_NBR.SetUnitType(D_UNITSYS_NONE);
	m_Gamma_c_E_NBR.SetUnitType(D_UNITSYS_NONE);
	m_Gamma_s_N_NBR.SetUnitType(D_UNITSYS_NONE);
	m_Gamma_s_SC_NBR.SetUnitType(D_UNITSYS_NONE);
	m_Gamma_s_E_NBR.SetUnitType(D_UNITSYS_NONE);

	// SET DATA!!

	// SET Default DATA by ANNEX!!
	T_PSCD_D ParamData; ParamData.Initialize();
	ParamData.Set_ABNT_NBR_Data();
	m_Gamma_c_N_NBR.SetEditUnit(m_pParamData->dGamma_c_N_NBR == 0.0 ? ParamData.dGamma_c_N_NBR : m_pParamData->dGamma_c_N_NBR);
	m_Gamma_c_SC_NBR.SetEditUnit(m_pParamData->dGamma_c_SC_NBR == 0.0 ? ParamData.dGamma_c_SC_NBR : m_pParamData->dGamma_c_SC_NBR);
	m_Gamma_c_E_NBR.SetEditUnit(m_pParamData->dGamma_c_E_NBR == 0.0 ? ParamData.dGamma_c_E_NBR : m_pParamData->dGamma_c_E_NBR);
	m_Gamma_s_N_NBR.SetEditUnit(m_pParamData->dGamma_s_N_NBR == 0.0 ? ParamData.dGamma_s_N_NBR : m_pParamData->dGamma_s_N_NBR);
	m_Gamma_s_SC_NBR.SetEditUnit(m_pParamData->dGamma_s_SC_NBR == 0.0 ? ParamData.dGamma_s_SC_NBR : m_pParamData->dGamma_s_SC_NBR);
	m_Gamma_s_E_NBR.SetEditUnit(m_pParamData->dGamma_s_E_NBR == 0.0 ? ParamData.dGamma_s_E_NBR : m_pParamData->dGamma_s_E_NBR);
}

void CDgnCRCAnnexDgnParameterABNTDlg::OnResetDgnParam_byAnnex()
{
	m_pParamData->Set_ABNT_NBR_Data();
	Initial_Data();
}