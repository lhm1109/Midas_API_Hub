#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_RUS_SP14_2018.h"
#include "SpfcSP14_2018Util.h"
#include "../wg_db/UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CETC_GDS_RUS_SP14_2018::CETC_GDS_RUS_SP14_2018(T_SPFC_D* pParamData, BOOL bModify, CWnd* pParent)
	: CMyChildDialog(CETC_GDS_RUS_SP14_2018::IDD, pParent, std::make_shared<CSpfcSP14_2018Util>())
{
	m_pParamData = pParamData;
	m_pUtil = std::dynamic_pointer_cast< CSpfcSP14_2018Util >( m_pSpfcBaseUtil );
	m_bInit = !bModify;
}

void CETC_GDS_RUS_SP14_2018::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_SOIL_CMB, m_cmbSoil);
	DDX_Control(pDX, IDC_CMD_K0_CMB, m_cmbK0);
	DDX_Control(pDX, IDC_CMD_K1_CMB, m_cmbK1);
	DDX_Control(pDX, IDC_CMD_KPSI_CMB, m_cmbKPsi);
	DDX_Control(pDX, IDC_CMD_ACCE_EDIT, m_edtAcce);
	DDX_Control(pDX, IDC_CMD_ACCE_UNIT, m_untAcce);
	DDX_Control(pDX, IDC_CMD_NLDS_CHK, m_chkNLDS);
	DDX_Control(pDX, IDC_CMD_PERIOD_EDIT, m_edtMaxT);
}

BEGIN_MESSAGE_MAP(CETC_GDS_RUS_SP14_2018, CMyChildDialog)
END_MESSAGE_MAP()

BOOL CETC_GDS_RUS_SP14_2018::OnInitDialog()
{
	CMyChildDialog::OnInitDialog();
	InitCombo();
	InitEditUnit();

	T_SPFC_RUS_SP14_2018 cData;
	m_pUtil->GetDefault(cData);
	if ( m_pParamData && m_pUtil )
	{
		if ( m_pParamData->nSpecCodeType == D_SPFC_CODE_RUS_SP14_2018 )
		{
			if ( m_pUtil->CheckData(m_pParamData->CodeParam.SP14_2018) )
			{
				cData = m_pParamData->CodeParam.SP14_2018;
			}
		}
	}
	Data2Dlg(cData);

	Data2Dlg();
	return TRUE;
}

void CETC_GDS_RUS_SP14_2018::InitEditUnit()
{
	m_edtAcce.SetUnitType(CUnitCtrl::m_SPFC_UNIT.Gravity);
	m_untAcce.SetUnitType(CUnitCtrl::m_SPFC_UNIT.Gravity);
}

void CETC_GDS_RUS_SP14_2018::GetFinalValue()
{
	T_SPFC_RUS_SP14_2018 cData;
	if ( Dlg2Data(cData) )
	{
		m_pUtil->SetSoil(cData.nSoil);
		m_pUtil->SetK0(cData.dK0);
		m_pUtil->SetK1(cData.dK1);
		m_pUtil->SetKPsi(cData.dKPsi);
		m_pUtil->SetAcce(cData.dAcce);
		m_pUtil->SetNLDS(cData.bNLDS);
		m_pUtil->SetMaxPeriod(cData.dMaxPeriod);
	}
}

void CETC_GDS_RUS_SP14_2018::InitCombo()
{
	auto L_SetCombo = [ ] (CComboBox& rCombo, const std::vector<CString>& vItem)
	{
		rCombo.ResetContent();
		for ( const CString& csItem : vItem )
		{
			rCombo.AddString(csItem);
		}
		rCombo.SetCurSel(0);
	};

	std::vector<CString> vSoil;
	m_pUtil->GetSoilData(vSoil);
	L_SetCombo(m_cmbSoil, vSoil);

	std::vector<CString> vK0;
	vK0.push_back(_T("0.80"));
	vK0.push_back(_T("1.00"));
	vK0.push_back(_T("1.10"));
	vK0.push_back(_T("1.30"));
	vK0.push_back(_T("1.50"));
	L_SetCombo(m_cmbK0, vK0);

	std::vector<CString> vK1;
	vK1.push_back(_T("0.12"));
	vK1.push_back(_T("0.15"));
	vK1.push_back(_T("0.22"));
	vK1.push_back(_T("0.25"));
	vK1.push_back(_T("0.30"));
	vK1.push_back(_T("0.35"));
	vK1.push_back(_T("0.40"));
	vK1.push_back(_T("1.00"));
	L_SetCombo(m_cmbK1, vK1);

	std::vector<CString> vKPsi;
	vKPsi.push_back(_T("1.00"));
	vKPsi.push_back(_T("1.30"));
	vKPsi.push_back(_T("1.50"));
	L_SetCombo(m_cmbKPsi, vKPsi);
}

BOOL CETC_GDS_RUS_SP14_2018::ISVALID()
{
	GetFinalValue();

	T_SPFC_RUS_SP14_2018 cData;
	cData.nSoil      = m_pUtil->GetSoil();
	cData.dK0        = m_pUtil->GetK0();
	cData.dK1        = m_pUtil->GetK1();
	cData.dKPsi      = m_pUtil->GetKPsi();
	cData.dAcce      = m_pUtil->GetAcce();
	cData.bNLDS      = m_pUtil->GetNLDS();
	cData.dMaxPeriod = m_pUtil->GetMaxPeriod();
	return m_pUtil->CheckData(cData);
}

void CETC_GDS_RUS_SP14_2018::End()
{
	CMyChildDialog::End();
}

BOOL CETC_GDS_RUS_SP14_2018::Dlg2Data()
{
	T_SPFC_RUS_SP14_2018& rData = m_pParamData->CodeParam.SP14_2018;
	rData.nSoil      = m_pUtil->GetSoil();
	rData.dK0        = m_pUtil->GetK0();
	rData.dK1        = m_pUtil->GetK1();
	rData.dKPsi      = m_pUtil->GetKPsi();
	rData.dAcce      = m_pUtil->GetAcce();
	rData.bNLDS      = m_pUtil->GetNLDS();
	rData.dMaxPeriod = m_pUtil->GetMaxPeriod();

	m_pParamData->Description = m_pUtil->GetFuncDesc(rData);
	return TRUE;
}

void CETC_GDS_RUS_SP14_2018::Data2Dlg()
{
	SetPushOverData(m_bInit);
}

void CETC_GDS_RUS_SP14_2018::SetPushOverData(BOOL bInit)
{
	if ( m_pParamData->nSpecCodeType != D_SPFC_CODE_RUS_SP14_2018 ) return;

	if ( !bInit )
	{
		Data2Dlg(m_pParamData->CodeParam.SP14_2018);
		GetFinalValue();
	}
}

void CETC_GDS_RUS_SP14_2018::Dlg2UtilData()
{
	GetFinalValue();
}

bool CETC_GDS_RUS_SP14_2018::Data2Dlg(const T_SPFC_RUS_SP14_2018& cData)
{
	auto L_SetCombo = [ ] (CComboBox& rCombo, double dValue)
	{
		CString csValue = nsGetStr(_T("%.2f"), dValue);
		rCombo.SetWindowText(csValue);
	};

	L_SetCombo(m_cmbK0, cData.dK0);
	L_SetCombo(m_cmbK1, cData.dK1);
	L_SetCombo(m_cmbKPsi, cData.dKPsi);
	m_cmbSoil.SetCurSel(cData.nSoil);
	m_edtAcce.SetEditUnit(cData.dAcce, 2);
	m_chkNLDS.SetCheck(cData.bNLDS ? BST_CHECKED : BST_UNCHECKED);
	m_edtMaxT.SetEditUnit(cData.dMaxPeriod, 2);
	return true;
}

bool CETC_GDS_RUS_SP14_2018::Dlg2Data(T_SPFC_RUS_SP14_2018& rData)
{
	auto L_GetCombo = [ ] (const CComboBox& Combo) -> double
	{
		CString csValue;
		Combo.GetWindowText(csValue);
		return _tstof(csValue);
	};

	rData.Initialize();
	rData.nSoil      = m_cmbSoil.GetCurSel();
	rData.dK0        = L_GetCombo(m_cmbK0);
	rData.dK1        = L_GetCombo(m_cmbK1);
	rData.dKPsi      = L_GetCombo(m_cmbKPsi);
	rData.dAcce      = m_edtAcce.GetEditValue();
	rData.bNLDS      = ( m_chkNLDS.GetCheck() == BST_CHECKED ) ? true : false;
	rData.dMaxPeriod = m_edtMaxT.GetEditValue();
	return true;
}