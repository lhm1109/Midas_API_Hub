// ETC_GDS_RUS_SP2016.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_RUS_SP2016.h"

#include "SpfcSP2016Util.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_RUS_SP2016 dialog


CETC_GDS_RUS_SP2016::CETC_GDS_RUS_SP2016(T_SPFC_D* pParamData, BOOL bModify, CWnd* pParent /*=NULL*/)
    : CMyChildDialog(CETC_GDS_RUS_SP2016::IDD, pParent, std::make_shared<CSpfcSP2016Util>())
{
    //{{AFX_DATA_INIT(CETC_GDS_RUS_SP2016)
    //}}AFX_DATA_INIT
    //m_strFuncName=_T("SP 268.1325800.2016");
    m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcSP2016Util>(m_pSpfcBaseUtil);
	m_bInit = !bModify;
}

void CETC_GDS_RUS_SP2016::DoDataExchange(CDataExchange* pDX)
{
    CMyChildDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CETC_GDS_RUS_SP2016)
    DDX_Control(pDX, IDC_CMD_REGION_CMB, m_cmbRegion);
    DDX_Control(pDX, IDC_CMD_SOIL_CMB,   m_cmbSoil);

	DDX_Control(pDX, IDC_CMD_K1_EDIT,   m_editK1);
	DDX_Control(pDX, IDC_CMD_K2_EDIT,   m_editK2);
	DDX_Control(pDX, IDC_CMD_K3_EDIT,   m_editK3);
	DDX_Control(pDX, IDC_CMD_K4_EDIT,   m_editK4);
	DDX_Control(pDX, IDC_CMD_KPSI_EDIT, m_editKPsi);

	DDX_Control(pDX, IDC_CMD_PERIOD_EDIT, m_editMaxPeriod);

    //DDX_Control(pDX, IDC_CMD_TS_UNT,           m_untTs);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_RUS_SP2016, CMyChildDialog)
    //{{AFX_MSG_MAP(CETC_GDS_RUS_SP2016)
    ON_CBN_SELCHANGE(IDC_CMD_REGION_CMB, OnSelchangeCmdZoneCmb)
    ON_CBN_SELCHANGE(IDC_CMD_SOIL_CMB, OnSelchangeCmdSiteCmb)
    
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_RUS_SP2016 message handlers

BOOL CETC_GDS_RUS_SP2016::OnInitDialog() 
{
    CMyChildDialog::OnInitDialog();
  
    InitZoneCombo();
    InitSiteCombo();

	m_editK1.SetEditUnit(0.5);
	m_editK2.SetEditUnit(1.0);
	m_editK3.SetEditUnit(1.0);
	m_editK4.SetEditUnit(1.0);
	m_editKPsi.SetEditUnit(1.0);

	m_editMaxPeriod.SetEditUnit(PRD_ED);

	Data2Dlg();
   
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_RUS_SP2016::GetFinalValue()
{
	m_pSpfcUtil->SetRegion(m_cmbRegion.GetCurSel());
	m_pSpfcUtil->SetSoil(m_cmbSoil.GetCurSel());
	m_pSpfcUtil->SetK1(m_editK1.GetEditValue());
	m_pSpfcUtil->SetK2(m_editK2.GetEditValue());
	m_pSpfcUtil->SetK3(m_editK3.GetEditValue());
	m_pSpfcUtil->SetK4(m_editK4.GetEditValue());
	m_pSpfcUtil->SetKPsi(m_editKPsi.GetEditValue());
	m_dMaxPeriod = m_editMaxPeriod.GetEditValue();

	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
}

void CETC_GDS_RUS_SP2016::OnSelchangeCmdZoneCmb()
{

}

void CETC_GDS_RUS_SP2016::OnSelchangeCmdSiteCmb() 
{
    
}

void CETC_GDS_RUS_SP2016::InitZoneCombo()
{
    CString aItem[4] = {_T("7"), _T("8"), _T("9") , _T("10") };

	m_cmbRegion.ResetContent();
    for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbRegion.AddString(aItem[i]);
	m_cmbRegion.SetCurSel(0);
}

void CETC_GDS_RUS_SP2016::InitSiteCombo()
{
	CString aItem[3] = { _T("I"), _T("II"), _T("III") };

	m_cmbSoil.ResetContent();
    for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSoil.AddString(aItem[i]);
	m_cmbSoil.SetCurSel(0);
}

// CETC_DESIGN_SPECT에서 ISVALID()를 호출한 후 End()를 호출한다.
BOOL CETC_GDS_RUS_SP2016::ISVALID()
{ 	
	GetFinalValue();

	CString strMsg;
    if(m_pSpfcUtil->GetK1() <= 0.)
    {    
        strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("K1"));
        AfxMessageBox(strMsg);
        return FALSE;
    }
	if (m_pSpfcUtil->GetK2() <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("K2"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if (m_pSpfcUtil->GetK3() <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("K3"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if (m_pSpfcUtil->GetK4() <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("K4"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if (m_pSpfcUtil->GetKPsi() <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("KPsi"));
		AfxMessageBox(strMsg);
		return FALSE;
	}

    if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
    return TRUE;  
}

void CETC_GDS_RUS_SP2016::End()
{
    CMyChildDialog::End();
}

BOOL CETC_GDS_RUS_SP2016::Dlg2Data()
{
	m_pParamData->CodeParam.SP2016.nRegion = m_pSpfcUtil->GetRegion();
	m_pParamData->CodeParam.SP2016.nSoil   = m_pSpfcUtil->GetSoil();

	m_pParamData->CodeParam.SP2016.dK1 = m_pSpfcUtil->GetK1();
	m_pParamData->CodeParam.SP2016.dK2 = m_pSpfcUtil->GetK2();
	m_pParamData->CodeParam.SP2016.dK3 = m_pSpfcUtil->GetK3();
	m_pParamData->CodeParam.SP2016.dK4 = m_pSpfcUtil->GetK4();
	m_pParamData->CodeParam.SP2016.dKPsi = m_pSpfcUtil->GetKPsi();
	m_pParamData->CodeParam.SP2016.dMaxPeriod = m_dMaxPeriod;

	CString strRegion[] = { _T("7"), _T("8"), _T("9") , _T("10") };
    CString strSoil[]={_T("S1"),_T("S2"),_T("S3"),_T("S4"),_T("S5"),_T("S6")};
    m_pParamData->Description.Format(_LS(IDS_WG_CMD__ADD_SP2016_PROFILE), strRegion[m_pSpfcUtil->GetRegion()], strSoil[m_pSpfcUtil->GetSoil()], m_pSpfcUtil->GetK1(), m_pSpfcUtil->GetK2(), m_pSpfcUtil->GetK3(), m_pSpfcUtil->GetK4(), m_pSpfcUtil->GetKPsi());

    return TRUE;
}

void CETC_GDS_RUS_SP2016::Data2Dlg()
{
    SetPushOverData(m_bInit);
}

void CETC_GDS_RUS_SP2016::SetPushOverData(BOOL bInit/*=TRUE*/)
{
    if(m_pParamData->nSpecCodeType != D_SPFC_CODE_RUS_SP2016) return;

    if(!bInit)
    {
		m_cmbRegion.SetCurSel(m_pParamData->CodeParam.SP2016.nRegion);
		m_cmbSoil.SetCurSel(m_pParamData->CodeParam.SP2016.nSoil);
		m_editK1.SetEditUnit(m_pParamData->CodeParam.SP2016.dK1);
		m_editK2.SetEditUnit(m_pParamData->CodeParam.SP2016.dK2);
		m_editK3.SetEditUnit(m_pParamData->CodeParam.SP2016.dK3);
		m_editK4.SetEditUnit(m_pParamData->CodeParam.SP2016.dK4);
		m_editKPsi.SetEditUnit(m_pParamData->CodeParam.SP2016.dKPsi);
		m_editMaxPeriod.SetEditUnit(m_pParamData->CodeParam.SP2016.dMaxPeriod);
		
		GetFinalValue();
    }

}

void CETC_GDS_RUS_SP2016::Dlg2UtilData()
{
	GetFinalValue();
}
