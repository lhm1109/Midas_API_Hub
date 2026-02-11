#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemTaiwan2022.h"
#include "SeisItemTaiwan2022Sub.h"
#include "SeisTaiwan86PeriodDlg.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "SeisItemDlgNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSeisItemTaiwan2022::CSeisItemTaiwan2022(CWnd* pParent)
	: CChildDialog(CSeisItemTaiwan2022::IDD, pParent)
{
	m_pParent = (CSeisItemDlgNew*)pParent;

	m_aCtrlStruct.Add(IDC_CMD_RDO_STRUCT_GEN);
	m_aCtrlStruct.Add(IDC_CMD_RDO_STRUCT_MIS);

	m_aCtrlPeriod.Add(IDC_CMD_PERIOD_RDO);
	m_aCtrlPeriod.Add(IDC_CMD_PERIOD_RDO2);

	m_aIFactorName.RemoveAll();
	m_aIFactorData.RemoveAll();
	m_aIFactorName.Add(_T("1.00"));
	m_aIFactorData.Add(1.00);
	m_aIFactorName.Add(_T("1.25"));
	m_aIFactorData.Add(1.25);
	m_aIFactorName.Add(_T("1.50"));
	m_aIFactorData.Add(1.50);

	m_aAFactorName.RemoveAll();
	m_aAFactorData.RemoveAll();
	m_aAFactorName.Add(_T("1.0"));
	m_aAFactorData.Add(1.0);
	m_aAFactorName.Add(_T("1.2"));
	m_aAFactorData.Add(1.2);
	m_aAFactorName.Add(_T("1.5"));
	m_aAFactorData.Add(1.5);

	m_aRFactorName.RemoveAll();
	m_aRFactorData.RemoveAll();
	m_aRFactorName.Add(_T("2.0"));
	m_aRFactorData.Add(2.0);
	m_aRFactorName.Add(_T("2.4"));
	m_aRFactorData.Add(2.4);
	m_aRFactorName.Add(_T("2.8"));
	m_aRFactorData.Add(2.8);
	m_aRFactorName.Add(_T("3.0"));
	m_aRFactorData.Add(3.0);
	m_aRFactorName.Add(_T("3.2"));
	m_aRFactorData.Add(3.2);
	m_aRFactorName.Add(_T("3.6"));
	m_aRFactorData.Add(3.6);
	m_aRFactorName.Add(_T("4.0"));
	m_aRFactorData.Add(4.0);
	m_aRFactorName.Add(_T("4.2"));
	m_aRFactorData.Add(4.2);
	m_aRFactorName.Add(_T("4.8"));
	m_aRFactorData.Add(4.8);

	m_bInit = FALSE;
	m_Data.Initialize();
}

void CSeisItemTaiwan2022::SetData2Dlg(T_SEIS_TAIWAN22& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CSeisItemTaiwan2022::SetDlg2Data(T_SEIS_TAIWAN22& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CSeisItemTaiwan2022::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_TY_FINAL_EDIT, m_editFinalY);
	DDX_Control(pDX, IDC_CMD_TY_APPR_EDIT, m_editApprY);
	DDX_Control(pDX, IDC_CMD_TY_ANAL_EDIT, m_editAnalY);
	DDX_Control(pDX, IDC_CMD_TX_FINAL_EDIT, m_editFinalX);
	DDX_Control(pDX, IDC_CMD_TX_APPR_EDIT, m_editApprX);
	DDX_Control(pDX, IDC_CMD_TX_ANAL_EDIT, m_editAnalX);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_ZONE_CMB, m_cbxZone);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_ALPHA_CMB, m_cbxAlpha);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_IMPORTANCE_CMB, m_cbxIFactor);
	DDX_Control(pDX, IDC_CMD_RY_CMB, m_cbxRY);
	DDX_Control(pDX, IDC_CMD_RX_CMB, m_cbxRX);
	DDX_Control(pDX, IDC_WG_CMD_STATIC0, m_grpFold);
}

BEGIN_MESSAGE_MAP(CSeisItemTaiwan2022, CChildDialog)
	ON_BN_CLICKED(IDC_CMD_PERIOD_BTN, OnBtnPeriod)
	ON_BN_CLICKED(IDC_CMD_PERIOD_RDO, OnRdoPeriod)
	ON_BN_CLICKED(IDC_CMD_PERIOD_RDO2, OnRdoPeriod)
	ON_EN_CHANGE(IDC_CMD_TX_ANAL_EDIT, OnChangePeriod)
	ON_EN_CHANGE(IDC_CMD_TY_ANAL_EDIT, OnChangePeriod)
	ON_EN_CHANGE(IDC_CMD_TX_APPR_EDIT, OnChangePeriod)
	ON_EN_CHANGE(IDC_CMD_TY_APPR_EDIT, OnChangePeriod)
	ON_BN_CLICKED(IDC_CMD_SEIS_TAIWAN02_SUB_BTN, OnBtnSeisSub)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_ZONE_CMB, OnSelChangeZone)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

BOOL CSeisItemTaiwan2022::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	m_editAnalX.SetUnitType(D_UNITSYS_NONE);
	m_editAnalY.SetUnitType(D_UNITSYS_NONE);
	m_editApprX.SetUnitType(D_UNITSYS_NONE);
	m_editApprY.SetUnitType(D_UNITSYS_NONE);
	m_editFinalX.SetUnitType(D_UNITSYS_NONE);
	m_editFinalY.SetUnitType(D_UNITSYS_NONE);
	m_editFinalX.SetReadOnly(TRUE);
	m_editFinalY.SetReadOnly(TRUE);

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	InitZoneCombo();
	InitIFactorCombo();
	InitAlphaCombo();
	InitRxRyCombo();
	SetFoldGroupCtrls();

	if (m_bModify)
	{
		Data2Dlg();
	}
	else
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlStruct, 0);
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlPeriod, 0);
		m_editAnalX.SetEditUnitAuto(0.0);
		m_editAnalY.SetEditUnitAuto(0.0);
		m_editApprX.SetEditUnitAuto(0.0);
		m_editApprY.SetEditUnitAuto(0.0);
		m_editFinalX.SetEditUnitAuto(0.0);
		m_editFinalY.SetEditUnitAuto(0.0);
	}

	OnRdoPeriod();
	return TRUE;
}
void CSeisItemTaiwan2022::SetFoldGroupCtrls()
{
	// After AlignControls
	m_grpFold.InitControl(this, CSeisItemTaiwan2022::IDD, IDC_WG_CMD_STATIC0, TRUE, TRUE);
	m_grpFold.SetFoldState(FALSE); // open
}

BOOL CSeisItemTaiwan2022::DestroyWindow()
{
	//Dlg2Data();
	if (m_pParent != NULL)
	{
		if (!m_pParent->IsSaveFoldedInfo())
		{
			m_grpFold.SetFoldState(FALSE, FALSE);
		}
	}

	return CChildDialog::DestroyWindow();
}

void CSeisItemTaiwan2022::OnBtnPeriod()
{
	CSeisTaiwan86PeriodDlg dlg(EN_DLG_MODE_TAIWAN_2022, false);
	dlg.SetInitPos(D_INIT_POS_RT);

	dlg.m_nMethodX = m_nMtdXTaiwan02;
	dlg.m_nMethodY = m_nMtdYTaiwan02;
	dlg.m_dHnX = m_dHnXTaiwan02;
	dlg.m_dHnY = m_dHnYTaiwan02;
	dlg.m_dAcX = m_dAcXTaiwan02;
	dlg.m_dAcY = m_dAcYTaiwan02;

	if (dlg.DoModal() == IDOK)
	{
		m_editApprX.SetEditUnitAuto(dlg.m_dPeriodX);
		m_editApprY.SetEditUnitAuto(dlg.m_dPeriodY);

		m_nMtdXTaiwan02 = dlg.m_nMethodX;
		m_nMtdYTaiwan02 = dlg.m_nMethodY;
		m_dHnXTaiwan02 = dlg.m_dHnX;
		m_dHnYTaiwan02 = dlg.m_dHnY;
		m_dAcXTaiwan02 = dlg.m_dAcX;
		m_dAcYTaiwan02 = dlg.m_dAcY;
	}
}

void CSeisItemTaiwan2022::OnRdoPeriod()
{
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlPeriod, nCheck);
	m_editAnalX.EnableWindow(nCheck == 0);
	m_editAnalY.EnableWindow(nCheck == 0);
	OnChangePeriod();
}

void CSeisItemTaiwan2022::SetFinalPeriod()
{
	double dSD1 = 0.0;
	m_Data.nSeismicZone = m_cbxZone.GetCurSel();
	switch (m_Data.nSeismicZone)
	{
	case 0: dSD1 = m_Data.dSd1 * m_Data.dFdv;
		break;
	case 1: dSD1 = m_Data.dSd1 * m_Data.dFdv * m_Data.dNdv;
		break;
	case 2: dSD1 = m_Data.dSds_t * m_Data.dTd0;
		break;
	default: ASSERT(0);
	}
	double dTXAnal = m_editAnalX.GetEditValue();
	double dTXAppr = m_editApprX.GetEditValue();
	double dTYAnal = m_editAnalY.GetEditValue();
	double dTYAppr = m_editApprY.GetEditValue();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlPeriod, nCheck);

	m_Data.dPeriodFinalX = pDoc->m_pAttrCtrl->CalcFundamentalPeriod2(TAIWAN_E_2022, dSD1, nCheck, dTXAnal, dTXAppr);
	m_Data.dPeriodFinalY = pDoc->m_pAttrCtrl->CalcFundamentalPeriod2(TAIWAN_E_2022, dSD1, nCheck, dTYAnal, dTYAppr);
}

void CSeisItemTaiwan2022::OnChangePeriod()
{
	SetFinalPeriod();
	m_editFinalX.SetEditUnitAuto(m_Data.dPeriodFinalX);
	m_editFinalY.SetEditUnitAuto(m_Data.dPeriodFinalY);
}

void CSeisItemTaiwan2022::InitDefaultData()
{
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	// 주의 !
	// Seismic Load 에서는 m 단위로 변환하여 받아온다.
	// Wind Load 에서는 현재 단위로 받아온다.
	//////////////////////////////////////////////
	T_UNIT_INDEX CurIndex, CodeIndex;
	pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CodeIndex = CurIndex;
	CodeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
	pUnitCtrl->SetUnitIndexCurrent(CodeIndex);

	CArray<T_STOR_K, T_STOR_K> aStorK;
	pAttrCtrl->GetStorKeyList(aStorK);
	int nStorNum = aStorK.GetSize();

	T_STOR_D StorD;
	StorD.Initialize();
	if (nStorNum > 0)
	{
		pAttrCtrl->GetStor(aStorK[nStorNum - 1], StorD);
	}

	double dHn = 0.0;
	if (pAttrCtrl->GetCountStor())
	{
		double dBaseLevel = pAttrCtrl->GetBaseLevel(FALSE);
		dHn = StorD.dStoryLevel - dBaseLevel;
	}

	m_dHnXTaiwan02 = m_dHnYTaiwan02 = dHn;
	m_dAcXTaiwan02 = m_dAcYTaiwan02 = 0.0;
	m_nMtdXTaiwan02 = m_nMtdYTaiwan02 = 0;

	pUnitCtrl->SetUnitIndexCurrent(CurIndex);
}

void CSeisItemTaiwan2022::InitZoneCombo()
{
	m_cbxZone.ResetContent();
	for (int i = 0; i < 3; i++)
	{
		m_cbxZone.AddString(CSeisItemTaiwan2022Sub::GetSeismicZoneName(i));
	}
	m_cbxZone.SetCurSel(0);
}

void CSeisItemTaiwan2022::InitIFactorCombo()
{
	m_cbxIFactor.ResetContent();
	for (int i = 0; i < m_aIFactorName.GetSize(); i++)
	{
		m_cbxIFactor.AddString(m_aIFactorName[i]);
	}
	m_cbxIFactor.SetCurSel(0);
}

void CSeisItemTaiwan2022::InitAlphaCombo()
{
	m_cbxAlpha.ResetContent();
	for (int i = 0; i < m_aAFactorName.GetSize(); i++)
	{
		m_cbxAlpha.AddString(m_aAFactorName[i]);
	}
	m_cbxAlpha.SetCurSel(0);
}

void CSeisItemTaiwan2022::InitRxRyCombo()
{
	m_cbxRX.ResetContent();
	m_cbxRY.ResetContent();
	for (int i = 0; i < m_aRFactorName.GetSize(); i++)
	{
		m_cbxRX.AddString(m_aRFactorName[i]);
		m_cbxRY.AddString(m_aRFactorName[i]);
	}
	m_cbxRX.SetCurSel(0);
	m_cbxRY.SetCurSel(0);
}

void CSeisItemTaiwan2022::Data2Dlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlStruct, m_Data.nStructType);

	m_cbxZone.SetCurSel(m_Data.nSeismicZone);

	CDlgUtil::CobxSetCurSelItemData(m_cbxIFactor, m_Data.dImportanceFactor, m_aIFactorData);
	CDlgUtil::CobxSetCurSelItemData(m_cbxAlpha, m_Data.dSeisMagnifyFactor, m_aAFactorData);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlPeriod, m_Data.nPeriodMethod);
	m_editAnalX.SetEditUnitAuto(m_Data.dPeriodAnalX);
	m_editAnalY.SetEditUnitAuto(m_Data.dPeriodAnalY);
	m_editApprX.SetEditUnitAuto(m_Data.dPeriodApprX);
	m_editApprY.SetEditUnitAuto(m_Data.dPeriodApprY);
	m_editFinalX.SetEditUnitAuto(m_Data.dPeriodFinalX);
	m_editFinalY.SetEditUnitAuto(m_Data.dPeriodFinalY);

	CDlgUtil::CobxSetCurSelItemData(m_cbxRX, m_Data.dResponseModifyFactorX, m_aRFactorData);
	CDlgUtil::CobxSetCurSelItemData(m_cbxRY, m_Data.dResponseModifyFactorY, m_aRFactorData);
}

BOOL CSeisItemTaiwan2022::Dlg2Data()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlStruct, m_Data.nStructType);

	m_Data.nSeismicZone = m_cbxZone.GetCurSel();

	m_Data.dImportanceFactor = CDlgUtil::GetComboData(m_cbxIFactor);
	m_Data.dSeisMagnifyFactor = CDlgUtil::GetComboData(m_cbxAlpha);

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlPeriod, m_Data.nPeriodMethod);
	m_Data.dPeriodAnalX = m_editAnalX.GetEditValue();
	m_Data.dPeriodAnalY = m_editAnalY.GetEditValue();
	m_Data.dPeriodApprX = m_editApprX.GetEditValue();
	m_Data.dPeriodApprY = m_editApprY.GetEditValue();
	SetFinalPeriod();

	m_Data.dResponseModifyFactorX = CDlgUtil::GetComboData(m_cbxRX);
	m_Data.dResponseModifyFactorY = CDlgUtil::GetComboData(m_cbxRY);
	return TRUE;
}

void CSeisItemTaiwan2022::OnBtnSeisSub()
{
	CSeisItemTaiwan2022Sub dlg;
	dlg.SetInitPos(D_INIT_POS_RT);

	if (!m_bInit)
	{
		m_bInit = TRUE;
	}

	dlg.SetModifyData(&m_Data);
	dlg.SetSeismicZone(m_cbxZone.GetCurSel());
	if (dlg.DoModal() == IDOK)
	{
		OnChangePeriod();
	}
}

void CSeisItemTaiwan2022::OnSelChangeZone()
{
	m_Data.nSeismicZone = m_cbxZone.GetCurSel();
	OnChangePeriod();
}

LRESULT CSeisItemTaiwan2022::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}
