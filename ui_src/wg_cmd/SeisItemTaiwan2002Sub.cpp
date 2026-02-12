#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemTaiwan2002Sub.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_editData.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CSeisItemTaiwan2002Sub::GetSeismicZoneName(int nCode)
{
	if (nCode < 0 || nCode > 2) { ASSERT(0); return _T(""); }

	CString aItem[3] = { _T("General Zone"), _T("Near Fault Zone"), _T("Taipei Basin") };
	return aItem[nCode];
}

CString CSeisItemTaiwan2002Sub::GetSoilTypeName(int nCode)
{
	if (nCode < 0 || nCode > 3) { ASSERT(0); return _T(""); }

	CString aItem[4] = { _T("Type 1"),_T("Type 2"),_T("Type 3"),_T("User Input") };
	return aItem[nCode];
}

CSeisItemTaiwan2002Sub::CSeisItemTaiwan2002Sub(CWnd* pParent)
	: CDialogMove(CSeisItemTaiwan2002Sub::IDD, pParent)
{
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_SDS_CMB);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_SD1_CMB);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_SMS_CMB);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_SM1_CMB);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_GEN_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_01_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_02_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_0D_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_0M_STC);

	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_NV_EDT);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_NA_EDT);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_NA_STC);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_NV_STC);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_NSF_STC);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_NSFD_STC);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_NSFM_STC);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_NV_EDT2);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_NA_EDT2);

	m_aCtrlMethod_SMF.Add(IDC_CMD_TAIWAN02_SOIL_CMB);
	m_aCtrlMethod_SMF.Add(IDC_CMD_TAIWAN02_FMV_EDT);
	m_aCtrlMethod_SMF.Add(IDC_CMD_TAIWAN02_FMA_EDT);
	m_aCtrlMethod_SMF.Add(IDC_CMD_TAIWAN02_FDV_EDT);
	m_aCtrlMethod_SMF.Add(IDC_CMD_TAIWAN02_FDA_EDT);
	m_aCtrlMethod_SMF.Add(IDC_CMD_TAIWAN02_SMF_STC);
	m_aCtrlMethod_SMF.Add(IDC_CMD_TAIWAN02_STYPE_STC);
	m_aCtrlMethod_SMF.Add(IDC_CMD_TAIWAN02_SMF01_STC);
	m_aCtrlMethod_SMF.Add(IDC_CMD_TAIWAN02_SMF02_STC);
	m_aCtrlMethod_SMF.Add(IDC_CMD_TAIWAN02_SMFD_STC);
	m_aCtrlMethod_SMF.Add(IDC_CMD_TAIWAN02_SMFM_STC);

	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_SUBZONE_CMB);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_SDS_T_EDT);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_SMS_T_EDT);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_TD0_EDT);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_TM0_EDT);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_SUBZONE_STC);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_2S_STC);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_TP_STC);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_2D_STC);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_2M_STC);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_T_STC);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_2D_STC2);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_2M_STC2);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_TP_STC2);
}

void CSeisItemTaiwan2002Sub::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_ZONE_EDT, m_edtSeisZone);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_SDS_CMB, m_cbxSds);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_SD1_CMB, m_cbxSd1);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_SMS_CMB, m_cbxSms);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_SM1_CMB, m_cbxSm1);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_NV_EDT, m_edtNdv);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_NA_EDT, m_edtNda);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_NV_EDT2, m_edtNmv);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_NA_EDT2, m_edtNma);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_SOIL_CMB, m_cbxSoilType);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_FMV_EDT, m_edtFmv);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_FMA_EDT, m_edtFma);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_FDV_EDT, m_edtFdv);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_FDA_EDT, m_edtFda);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_SUBZONE_CMB, m_cbxSubZone_taipai);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_SDS_T_EDT, m_edtSds_T);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_SMS_T_EDT, m_edtSms_T);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_TM0_EDT, m_edtTm0);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_TD0_EDT, m_edtTd0);
}

BEGIN_MESSAGE_MAP(CSeisItemTaiwan2002Sub, CDialogMove)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_SOIL_CMB, OnSelchangeCmdTaiwan02SoilTypeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_SUBZONE_CMB, OnSelchangeCmdTaiwan02SubzoneCmb)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_SDS_CMB, OnSelchangeCmdTaiwan02SpecAccelCmb)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_SD1_CMB, OnSelchangeCmdTaiwan02SpecAccelCmb)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_SMS_CMB, OnSelchangeCmdTaiwan02SpecAccelCmb)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_SM1_CMB, OnSelchangeCmdTaiwan02SpecAccelCmb)
	ON_CBN_EDITCHANGE(IDC_CMD_TAIWAN02_SD1_CMB, OnEditchangeCmdTaiwan02SpecAccelCmb)
	ON_CBN_EDITCHANGE(IDC_CMD_TAIWAN02_SDS_CMB, OnEditchangeCmdTaiwan02SpecAccelCmb)
	ON_CBN_EDITCHANGE(IDC_CMD_TAIWAN02_SMS_CMB, OnEditchangeCmdTaiwan02SpecAccelCmb)
	ON_CBN_EDITCHANGE(IDC_CMD_TAIWAN02_SM1_CMB, OnEditchangeCmdTaiwan02SpecAccelCmb)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_NV_EDT, OnEditchangeCmdTaiwan02NSFactorEdt)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_NA_EDT, OnEditchangeCmdTaiwan02NSFactorEdt)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_NV_EDT2, OnEditchangeCmdTaiwan02NSFactorEdt)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_NA_EDT2, OnEditchangeCmdTaiwan02NSFactorEdt)
END_MESSAGE_MAP()

void CSeisItemTaiwan2002Sub::AlignCtrl()
{
	auto l_MoveOKCancel = [this](int nID)
	{
		CRect rRef, rMove;
		GetDlgItem(nID)->GetWindowRect(rRef);
		GetDlgItem(IDOK)->GetWindowRect(rMove);
		int nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(16);

		CArray<UINT, UINT> aCtrlOKs; aCtrlOKs.SetSize(2);
		aCtrlOKs[0] = IDOK;
		aCtrlOKs[1] = IDCANCEL;
		CDlgUtil::CtrlMoveDistY(this, aCtrlOKs, nDistY);
	};

	CRect rRef, rMove;
	GetDlgItem(IDC_CMD_TAIWAN02_GEN_STC)->GetWindowRect(rRef);
	switch(m_nSeisZone)
	{
		case 0:	// General Zone
		{
			// Align Site Magnify Factor
			GetDlgItem(IDC_CMD_TAIWAN02_SMF_STC)->GetWindowRect(rMove);
			int nDistY = rRef.bottom-rMove.top + globalUtils.ScaleByDPI(4);
			CDlgUtil::CtrlMoveDistY(this, m_aCtrlMethod_SMF, nDistY);
			l_MoveOKCancel(IDC_CMD_TAIWAN02_SMF_STC);
		}
		break;
		case 1: // Near fault zone
		{
			// Align Near Source Factor
			GetDlgItem(IDC_CMD_TAIWAN02_NSF_STC)->GetWindowRect(rMove);
			int nDistY = rRef.bottom-rMove.top + globalUtils.ScaleByDPI(4);
			CDlgUtil::CtrlMoveDistY(this, m_aCtrlMethod1, nDistY);
			l_MoveOKCancel(IDC_CMD_TAIWAN02_SMF_STC);
		}
		break;
		case 2:	// Taipei Basin
		{
			// Align Taipei
			GetDlgItem(IDC_CMD_TAIWAN02_SUBZONE_STC)->GetWindowRect(rMove);
			int nDistY = rRef.top-rMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aCtrlMethod2, nDistY);
			l_MoveOKCancel(IDC_CMD_TAIWAN02_TP_STC2);
		}
		break;
		default: ASSERT(0); break;
	}

	// Resize Dialog
	GetDlgItem(IDOK)->GetWindowRect(rRef);
	GetWindowRect(rMove);
	rMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(rMove);
}

BOOL CSeisItemTaiwan2002Sub::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	m_edtNdv.SetUnitType(D_UNITSYS_NONE);
	m_edtNda.SetUnitType(D_UNITSYS_NONE);
	m_edtNmv.SetUnitType(D_UNITSYS_NONE);
	m_edtNma.SetUnitType(D_UNITSYS_NONE);
	m_edtFdv.SetUnitType(D_UNITSYS_NONE);
	m_edtFda.SetUnitType(D_UNITSYS_NONE);
	m_edtFmv.SetUnitType(D_UNITSYS_NONE);
	m_edtFma.SetUnitType(D_UNITSYS_NONE);

	AlignCtrl();

	CDlgUtil::CtrlShowHide(this, m_aCtrlMethod0, m_nSeisZone != 2);
	CDlgUtil::CtrlShowHide(this, m_aCtrlMethod1, m_nSeisZone == 1);
	CDlgUtil::CtrlShowHide(this, m_aCtrlMethod_SMF, m_nSeisZone != 2);
	CDlgUtil::CtrlShowHide(this, m_aCtrlMethod2, m_nSeisZone == 2);

	m_edtSeisZone.SetReadOnly();
	m_edtSeisZone.SetWindowText(GetSeismicZoneName(m_nSeisZone));

	InitSpecAccel();
	InitSoilType();
	InitTaipai();

	Data2Dlg();
	CtrlManager();
	return TRUE;
}

void CSeisItemTaiwan2002Sub::OnOk()
{
	if (!Dlg2Data()) return;
	CDialogMove::OnOK();
}

void CSeisItemTaiwan2002Sub::OnCancel()
{
	CDialogMove::OnCancel();
}

void CSeisItemTaiwan2002Sub::OnSelchangeCmdTaiwan02SpecAccelCmb()
{
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CMD_TAIWAN02_SDS_CMB, CBN_EDITCHANGE), (LPARAM)m_cbxSds.GetSafeHwnd());
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CMD_TAIWAN02_SD1_CMB, CBN_EDITCHANGE), (LPARAM)m_cbxSd1.GetSafeHwnd());
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CMD_TAIWAN02_SMS_CMB, CBN_EDITCHANGE), (LPARAM)m_cbxSms.GetSafeHwnd());
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CMD_TAIWAN02_SM1_CMB, CBN_EDITCHANGE), (LPARAM)m_cbxSm1.GetSafeHwnd());
}

void CSeisItemTaiwan2002Sub::OnEditchangeCmdTaiwan02SpecAccelCmb()
{
	SetSiteMagnifyFactor();
}

void CSeisItemTaiwan2002Sub::OnSelchangeCmdTaiwan02SoilTypeCmb()
{
	CtrlManager();
	SetSiteMagnifyFactor();
}

void CSeisItemTaiwan2002Sub::OnSelchangeCmdTaiwan02SubzoneCmb()
{
	CtrlManager();
	SetChangeTaipei();
}

void CSeisItemTaiwan2002Sub::OnEditchangeCmdTaiwan02NSFactorEdt()
{
	SetSiteMagnifyFactor();
}

void CSeisItemTaiwan2002Sub::Data2Dlg()
{
	CString csVal;

	csVal.Format(_T("%g"), m_pData->dSds);
	m_cbxSds.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_pData->dSd1);
	m_cbxSd1.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_pData->dSms);
	m_cbxSms.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_pData->dSm1);
	m_cbxSm1.SetWindowText(csVal);

	m_edtNdv.SetEditUnit(m_pData->dNdv);
	m_edtNda.SetEditUnit(m_pData->dNda);
	m_edtNmv.SetEditUnit(m_pData->dNmv);
	m_edtNma.SetEditUnit(m_pData->dNma);
	m_cbxSoilType.SetCurSel(m_pData->nSoilType);

	m_edtFmv.SetEditUnit(m_pData->dFmv);
	m_edtFma.SetEditUnit(m_pData->dFma);
	m_edtFdv.SetEditUnit(m_pData->dFdv);
	m_edtFda.SetEditUnit(m_pData->dFda);

	m_cbxSubZone_taipai.SetCurSel(m_pData->nSubZone);
	m_edtSds_T.SetEditUnit(m_pData->dSds_t);
	m_edtSms_T.SetEditUnit(m_pData->dSms_t);
	m_edtTm0.SetEditUnit(m_pData->dTm0);
	m_edtTd0.SetEditUnit(m_pData->dTd0);
}

BOOL CSeisItemTaiwan2002Sub::Dlg2Data()
{
	//  Main Dlg OK 전(DB에 넣기 전)에 에러체크 수행
	double dSds, dSd1, dSms, dSm1;
	CFormulaEdit::GetEditValue(&m_cbxSds, dSds);
	CFormulaEdit::GetEditValue(&m_cbxSd1, dSd1);
	CFormulaEdit::GetEditValue(&m_cbxSms, dSms);
	CFormulaEdit::GetEditValue(&m_cbxSm1, dSm1);

	double dNdv, dNda, dNmv, dNma;
	CFormulaEdit::GetEditValue(&m_edtNdv, dNdv);
	CFormulaEdit::GetEditValue(&m_edtNda, dNda);
	CFormulaEdit::GetEditValue(&m_edtNmv, dNmv);
	CFormulaEdit::GetEditValue(&m_edtNma, dNma);

	double dFdv, dFda, dFmv, dFma;
	CFormulaEdit::GetEditValue(&m_edtFdv, dFdv);
	CFormulaEdit::GetEditValue(&m_edtFda, dFda);
	CFormulaEdit::GetEditValue(&m_edtFmv, dFmv);
	CFormulaEdit::GetEditValue(&m_edtFma, dFma);

	double dSds_t, dSms_t, dTm0, dTd0;
	CFormulaEdit::GetEditValue(&m_edtSds_T, dSds_t);
	CFormulaEdit::GetEditValue(&m_edtSms_T, dSms_t);
	CFormulaEdit::GetEditValue(&m_edtTm0, dTm0);
	CFormulaEdit::GetEditValue(&m_edtTd0, dTd0);

	BOOL bError = FALSE;
	if (dSds <= 0.0 || dSd1 <= 0.0 || dSms <= 0.0 || dSm1 <= 0.0)
	{
		bError = TRUE; AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__TAIWAN02_SpecAccel));
	}
	if (dNdv < 1.0 || dNda < 1.0 || dNmv < 1.0 || dNma < 1.0)
	{
		bError = TRUE; AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__TAIWAN02_NSF));
	}
	if (dFdv < 0.0 || dFda < 0.0 || dFmv < 0.0 || dFma < 0.0)
	{
		bError = TRUE; AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__TAIWAN02_SMF));
	}

	//Taipei
	if (dSds_t <= 0.0 || dSms_t <= 0.0)
	{
		bError = TRUE; AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__TAIWAN02_SpecAccel));
	}
	if (dTm0 < 0.0 || dTd0 < 0.0)
	{
		bError = TRUE; AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__TAIWAN02_TransPrd));
	}

	if (bError) return FALSE;

	m_pData->dSds = dSds;
	m_pData->dSd1 = dSd1;
	m_pData->dSms = dSms;
	m_pData->dSm1 = dSm1;

	m_pData->dNdv = dNdv;
	m_pData->dNda = dNda;
	m_pData->dNmv = dNmv;
	m_pData->dNma = dNma;

	m_pData->nSoilType = m_cbxSoilType.GetItemData(m_cbxSoilType.GetCurSel());
	m_pData->dFmv = dFmv;
	m_pData->dFma = dFma;
	m_pData->dFdv = dFdv;
	m_pData->dFda = dFda;

	m_pData->nSubZone = m_cbxSubZone_taipai.GetItemData(m_cbxSubZone_taipai.GetCurSel());
	m_pData->dSds_t = dSds_t;
	m_pData->dSms_t = dSms_t;
	m_pData->dTm0 = dTm0;
	m_pData->dTd0 = dTd0;
	return TRUE;
}

void CSeisItemTaiwan2002Sub::InitSpecAccel()
{
	m_cbxSds.ResetContent();
	m_cbxSds.AddString(_T("0.5"));
	m_cbxSds.AddString(_T("0.6"));
	m_cbxSds.AddString(_T("0.7"));
	m_cbxSds.AddString(_T("0.8"));
	m_cbxSds.SetCurSel(0);

	m_cbxSd1.ResetContent();
	m_cbxSd1.AddString(_T("0.30"));
	m_cbxSd1.AddString(_T("0.35"));
	m_cbxSd1.AddString(_T("0.40"));
	m_cbxSd1.AddString(_T("0.45"));
	m_cbxSd1.SetCurSel(0);

	m_cbxSms.ResetContent();
	m_cbxSms.AddString(_T("0.7"));
	m_cbxSms.AddString(_T("0.8"));
	m_cbxSms.AddString(_T("0.9"));
	m_cbxSms.AddString(_T("1.0"));
	m_cbxSms.SetCurSel(0);

	m_cbxSm1.ResetContent();
	m_cbxSm1.AddString(_T("0.40"));
	m_cbxSm1.AddString(_T("0.45"));
	m_cbxSm1.AddString(_T("0.50"));
	m_cbxSm1.AddString(_T("0.55"));
	m_cbxSm1.SetCurSel(0);

	m_edtNda.SetWindowText(_T("1.0"));        // User Input only
	m_edtNdv.SetWindowText(_T("1.0"));
	m_edtNma.SetWindowText(_T("1.0"));
	m_edtNmv.SetWindowText(_T("1.0"));
}

void CSeisItemTaiwan2002Sub::InitSoilType()
{
	for (int i = 0; i < 4; i++)
	{
		CDlgUtil::CobxAddItem(m_cbxSoilType, GetSoilTypeName(i), i);
	}
}

void CSeisItemTaiwan2002Sub::InitTaipai()
{
	CString aItem[5] = { _T("Taipei Basin I"),_T("Taipei Basin II"),_T("Taipei Basin III"),_T("Taipei Basin IV"),_T("User Input") };
	for (int i = 0; i < 5; i++)
	{
		CDlgUtil::CobxAddItem(m_cbxSubZone_taipai, aItem[i], i);
	}

	m_edtSds_T.SetEditUnit(0.6);
	m_edtSms_T.SetEditUnit(0.8);
	m_edtTd0.SetEditUnit(1.6);
	m_edtTm0.SetEditUnit(1.6);
}

void CSeisItemTaiwan2002Sub::SetSiteMagnifyFactor()
{
	int nSoilType = m_cbxSoilType.GetItemData(m_cbxSoilType.GetCurSel());

	double dFda, dFdv, dFma, dFmv;
	if (nSoilType != 3) // user type 아닐 경우
	{
		CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
		if (m_nSeisZone == 0 || m_nSeisZone == 1)
		{
			double dSds, dSd1, dSms, dSm1;
			CFormulaEdit::GetEditValue(&m_cbxSds, dSds);
			CFormulaEdit::GetEditValue(&m_cbxSd1, dSd1);
			CFormulaEdit::GetEditValue(&m_cbxSms, dSms);
			CFormulaEdit::GetEditValue(&m_cbxSm1, dSm1);

			double dNda, dNdv, dNma, dNmv;
			CFormulaEdit::GetEditValue(&m_edtNda, dNda);
			CFormulaEdit::GetEditValue(&m_edtNdv, dNdv);
			CFormulaEdit::GetEditValue(&m_edtNma, dNma);
			CFormulaEdit::GetEditValue(&m_edtNmv, dNmv);

			if (!pDoc->m_pAttrCtrl->CalcTaiwan02SiteMagnifyFactor(TAIWAN_E_2006, m_nSeisZone, nSoilType,
				dSds, dSd1, dSms, dSm1, dNda, dNdv, dNma, dNmv,
				dFda, dFdv, dFma, dFmv)) return;
			// NearFaultZone의 V* 산정시 Na=Nv=1.0 일때 Fa_1,Fv_1은 LateralLoad.cpp에서 계산
			// Fa_1,Fv_1은 출력하지 않음.

			m_edtFda.SetEditUnit(dFda);
			m_edtFdv.SetEditUnit(dFdv);
			m_edtFma.SetEditUnit(dFma);
			m_edtFmv.SetEditUnit(dFmv);
		}
	}
	//User type 경우에는 Dlg2Data에서 처리
}

void CSeisItemTaiwan2002Sub::SetChangeTaipei()
{
	m_edtSds_T.SetEditUnit(0.6);
	m_edtSms_T.SetEditUnit(0.8);

	int nSubZone = m_cbxSubZone_taipai.GetItemData(m_cbxSubZone_taipai.GetCurSel());
	switch (nSubZone)
	{
	case  0: { m_edtTd0.SetEditUnit(1.6);  m_edtTm0.SetEditUnit(1.6); } break;
	case  1: { m_edtTd0.SetEditUnit(1.3);  m_edtTm0.SetEditUnit(1.3); } break;
	case  2: { m_edtTd0.SetEditUnit(1.05); m_edtTm0.SetEditUnit(1.05); } break;
	case  3: { m_edtTd0.SetEditUnit(0.85); m_edtTm0.SetEditUnit(0.85); } break;
	default: { m_edtTd0.SetWindowText(_T("1.0")); m_edtTm0.SetWindowText(_T("1.0")); } break; // User Input
	}
}

void CSeisItemTaiwan2002Sub::CtrlManager()
{
	//Site Magnify Factor
	int nSoilType = m_cbxSoilType.GetItemData(m_cbxSoilType.GetCurSel());
	if (nSoilType != 3)
	{
		m_edtFmv.SetReadOnly();
		m_edtFma.SetReadOnly();
		m_edtFdv.SetReadOnly();
		m_edtFda.SetReadOnly();
	}
	else
	{
		m_edtFmv.SetReadOnly(0);
		m_edtFma.SetReadOnly(0);
		m_edtFdv.SetReadOnly(0);
		m_edtFda.SetReadOnly(0);
	}

	// Taipei Basin only
	int nSubZone = m_cbxSubZone_taipai.GetItemData(m_cbxSubZone_taipai.GetCurSel());
	if (nSubZone != 4)
	{
		m_edtSds_T.SetReadOnly();
		m_edtSms_T.SetReadOnly();
		m_edtTm0.SetReadOnly();
		m_edtTd0.SetReadOnly();
	}
	else
	{
		m_edtSds_T.SetReadOnly(0);
		m_edtSms_T.SetReadOnly(0);
		m_edtTm0.SetReadOnly(0);
		m_edtTd0.SetReadOnly(0);
	}
}