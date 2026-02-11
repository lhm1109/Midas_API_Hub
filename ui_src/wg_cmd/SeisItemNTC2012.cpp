// SeisItemNTC2012.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemNTC2012.h"
#include "SeisEuroCalcDlg.h"
#include "SeisItemDlgNew.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_LateralLoad.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisItemNTC2012 dialog


CSeisItemNTC2012::CSeisItemNTC2012(BOOL bNTC2018Mode /*= FALSE*/, CWnd* pParent /*=NULL*/)
	: CChildDialog(CSeisItemNTC2012::IDD, pParent)
{
	m_bNTC2018Mode = bNTC2018Mode;
	m_bInit = FALSE;
	m_Data.Initialize();
	m_pParent = (CSeisItemDlgNew*)pParent;
}

void CSeisItemNTC2012::SetData2Dlg(T_SEIS_NTC2008& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CSeisItemNTC2012::SetDlg2Data(T_SEIS_NTC2008& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CSeisItemNTC2012::Data2Dlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRadioSpecType, m_Data.nSpecType);
	m_GroundTypeCmb.SetCurSel(m_Data.nGroundType);
	m_wndS.SetEditUnit(m_Data.dS);
	m_wndTb.SetEditUnit(m_Data.dTb);
	m_wndTc.SetEditUnit(m_Data.dTc);
	m_wndTd.SetEditUnit(m_Data.dTd);
	m_wndAg.SetEditUnit(m_Data.dAg);
	m_wndQ.SetEditUnit(m_Data.dQ);
	m_wndAmpF.SetEditUnit(m_Data.dAmpF);
	m_wndTcStar.SetEditUnit(m_Data.dTcStar);
	m_editPeriodX.SetEditUnit(m_Data.dPeriod_X);
	m_editPeriodY.SetEditUnit(m_Data.dPeriod_Y);
}

BOOL CSeisItemNTC2012::Dlg2Data()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioSpecType, m_Data.nSpecType);
	m_Data.nGroundType = m_GroundTypeCmb.GetCurSel();
	m_Data.dS = m_wndS.GetEditValue();
	m_Data.dTb = m_wndTb.GetEditValue();
	m_Data.dTc = m_wndTc.GetEditValue();
	m_Data.dTd = m_wndTd.GetEditValue();
	m_Data.dAg = m_wndAg.GetEditValue();
	m_Data.dQ = m_wndQ.GetEditValue();
	m_Data.dAmpF = m_wndAmpF.GetEditValue();
	m_Data.dTcStar = m_wndTcStar.GetEditValue();
	m_Data.dPeriod_X = m_editPeriodX.GetEditValue();
	m_Data.dPeriod_Y = m_editPeriodY.GetEditValue();

	if (m_Data.dPeriod_X < 0 || m_Data.dPeriod_Y < 0)
	{
		CString strError = _LS(IDS_CMD_HSFC_error); // Error
		strError += _T("! "); // Error!
		AfxMessageBox(strError + _LS(IDS_WG_CMD__ADD2__Period));
		return FALSE;
	}
	return TRUE;
}

void CSeisItemNTC2012::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2008_TD_EDIT, m_wndTd);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2008_TC_EDIT, m_wndTc);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2008_TB_EDIT, m_wndTb);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2008_S_EDIT, m_wndS);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2008_GROUND_CMB, m_GroundTypeCmb);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2008_AG_EDIT, m_wndAg);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2008_BEHAVIOR_EDIT, m_wndQ);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2008_AMP_EDIT, m_wndAmpF);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2008_TCSTAR_EDIT, m_wndTcStar);
	DDX_Radio(pDX, IDC_ETC_GDS_NTC2008_PARATYPE_RDO, m_nParaTypeRdo);
	DDX_Control(pDX, IDC_CMD_NTC_PERIOD_Y_EDIT, m_editPeriodY);
	DDX_Control(pDX, IDC_CMD_NTC_PERIOD_X_EDIT, m_editPeriodX);
	DDX_Control(pDX, IDC_CMD_NTC_FRAME1, m_grpFold);
}


BEGIN_MESSAGE_MAP(CSeisItemNTC2012, CChildDialog)
	ON_BN_CLICKED(IDC_CMD_NTC_BTN_CALC_PERIOD, OnCmdNTCBtnCalcPeriod)
	ON_BN_CLICKED(IDC_ETC_GDS_NTC2008_PARATYPE_RDO, OnNTC2012ParatypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_NTC2008_PARATYPE_RDO2, OnNTC2012ParatypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_NTC2008_PARATYPE_RDO3, OnNTC2012ParatypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_NTC2008_PARATYPE_RDO4, OnNTC2012ParatypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_NTC2008_PARATYPE_RDO5, OnNTC2012ParatypeRdo)
	ON_CBN_SELCHANGE(IDC_ETC_GDS_NTC2008_GROUND_CMB, OnSelchangeNTC2012GroundCmb)
	ON_EN_KILLFOCUS(IDC_ETC_GDS_NTC2008_AG_EDIT, OnKillfocusEdit)
	ON_EN_KILLFOCUS(IDC_ETC_GDS_NTC2008_AMP_EDIT, OnKillfocusEdit)
	ON_EN_KILLFOCUS(IDC_ETC_GDS_NTC2008_TCSTAR_EDIT, OnKillfocusEdit)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisItemNTC2012 message handlers

BOOL CSeisItemNTC2012::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	m_aCtrlRadioSpecType.RemoveAll();
	m_aCtrlRadioSpecType.Add(IDC_ETC_GDS_NTC2008_PARATYPE_RDO);
	m_aCtrlRadioSpecType.Add(IDC_ETC_GDS_NTC2008_PARATYPE_RDO2);
	m_aCtrlRadioSpecType.Add(IDC_ETC_GDS_NTC2008_PARATYPE_RDO3);
	m_aCtrlRadioSpecType.Add(IDC_ETC_GDS_NTC2008_PARATYPE_RDO4);
	m_aCtrlRadioSpecType.Add(IDC_ETC_GDS_NTC2008_PARATYPE_RDO5);

	m_aCtrlParaType.RemoveAll();
	m_aCtrlParaType.Add(IDC_ETC_GDS_NTC2008_PARATYPE_RDO);
	m_aCtrlParaType.Add(IDC_ETC_GDS_NTC2008_PARATYPE_RDO2);
	m_aCtrlParaType.Add(IDC_ETC_GDS_NTC2008_PARATYPE_RDO3);
	m_aCtrlParaType.Add(IDC_ETC_GDS_NTC2008_PARATYPE_RDO4);

	InitGroundTypeCombo();
	SetFoldGroupCtrls();

	if (m_bNTC2018Mode)
		SetDlgItemText(IDC_WG_CMD_STATIC5, _LS(IDS_CMD_BEHAVIOR_FACTOR));

	Data2Dlg();
	CtrlManager();
	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisItemNTC2012::InitGroundTypeCombo()
{
	m_GroundTypeCmb.ResetContent();
	m_GroundTypeCmb.AddString(_T("A"));
	m_GroundTypeCmb.AddString(_T("B"));
	m_GroundTypeCmb.AddString(_T("C"));
	m_GroundTypeCmb.AddString(_T("D"));
	m_GroundTypeCmb.AddString(_T("E"));
	if (m_bNTC2018Mode == FALSE)
	{
		m_GroundTypeCmb.AddString(_T("S1"));
		m_GroundTypeCmb.AddString(_T("S2"));
	}
}

void CSeisItemNTC2012::SetFoldGroupCtrls()
{
	// After AlignControls
	m_grpFold.InitControl(this, CSeisItemNTC2012::IDD, IDC_CMD_NTC_FRAME1, TRUE, TRUE);
	m_grpFold.SetFoldState(FALSE); // open
}

BOOL CSeisItemNTC2012::DestroyWindow()
{
	if (m_pParent != NULL)
	{
		if (!m_pParent->IsSaveFoldedInfo())
		{
			m_grpFold.SetFoldState(FALSE, FALSE);
		}
	}

	return CChildDialog::DestroyWindow();

	// TODO: Add your message handler code here
}

void CSeisItemNTC2012::InitDefaultData()
{
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;
	double dHn = 0.0;
	T_STOR_D StorD, BaseStorD;

	// 주의 !
	// Seismic Load 에서는 m 단위로 변환하여 받아온다.
	// Wind Load 에서는 현재 단위로 받아온다.
	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	T_UNIT_INDEX CurIndex, CodeIndex;
	pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CodeIndex = CurIndex;
	CodeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M; // M 기준
	pUnitCtrl->SetUnitIndexCurrent(CodeIndex);

	CArray<T_STOR_K,T_STOR_K> aStorK;
	pAttrCtrl->GetStorKeyList(aStorK); //Story Level로 정렬된 리스트
	int nStorNum = aStorK.GetSize();
	if (nStorNum > 0) pAttrCtrl->GetStor(aStorK[nStorNum - 1], StorD);
	else StorD.Initialize();

	double dBaseLevel = pAttrCtrl->GetBaseLevel(FALSE);
	dHn = StorD.dStoryLevel - dBaseLevel;
	if (!pAttrCtrl->GetCountStor()) dHn = 0;

	m_dHX = dHn;
	m_dHY = dHn;
	m_dAcX = 0.0;
	m_dAcY = 0.0;
	m_nDX = 0.0;
	m_nDY = 0.0;
	m_nMtdX = 0;
	m_nMtdY = 0;

	// <Remember> Data를 받고나서는 User Unit기준
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);
}

void CSeisItemNTC2012::OnCmdNTCBtnCalcPeriod()
{
	// TODO: Add your control notification handler code here
	CSeisEuroCalcDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT); // child dialog의 위치 지정	
	// 현재 dialog의 변수를 child dialog의 변수에 넘겨줌		
	dlg.m_dXHedit = m_dHX;
	dlg.m_dYHedit = m_dHY;
	dlg.m_dXAcedit = m_dAcX;
	dlg.m_dYAcedit = m_dAcY;
	dlg.m_dXDedit = m_nDX;
	dlg.m_dYDedit = m_nDY;
	dlg.m_nXMethod = m_nMtdX;
	dlg.m_nYMethod = m_nMtdY;

	double dTc = m_wndTc.GetEditValue();

	if (dlg.DoModal() == IDOK)
	{
		double dP_x, dP_y;
		dP_x = dlg.m_dPeriod_x;
		dP_y = dlg.m_dPeriod_y;

		if (dP_x > 4 * dTc || dP_x > 2.0 || dP_y > 4 * dTc || dP_y > 2.0) // 기본진동수의 제한사항 Eurocode EC8 과 동일
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Restriction_of_fundamental_period_violated_));
		}

		CString cx, cy;
		cx.Format(_T("%-10.4g"), dlg.m_dPeriod_x);
		cy.Format(_T("%-10.4g"), dlg.m_dPeriod_y);
		m_editPeriodX.SetEditUnit(_tstof(cx));
		m_editPeriodY.SetEditUnit(_tstof(cy));
		// child dialog의 변수를 현재 dialog의 변수가 받음		
		m_dHX = dlg.m_dXHedit;
		m_dHY = dlg.m_dYHedit;
		m_dAcX = dlg.m_dXAcedit;
		m_dAcY = dlg.m_dYAcedit;
		m_nDX = dlg.m_dXDedit;
		m_nDY = dlg.m_dYDedit;
		m_nMtdX = dlg.m_nXMethod;
		m_nMtdY = dlg.m_nYMethod;
	}
}


void CSeisItemNTC2012::CtrlManager()
{
	UpdateData(TRUE);

	BOOL bSpecialGroundType = FALSE;
	BOOL bUserDefined = FALSE;
	BOOL bAutomatic = TRUE;

	int nGroundType = m_GroundTypeCmb.GetCurSel();

	if (nGroundType == 5 || nGroundType == 6) bSpecialGroundType = TRUE; // S1, S2

	// Special Ground Type(S1, S2)인 경우 Spectra Type은 User Defined로 강제 설정
	if (bSpecialGroundType) m_nParaTypeRdo = 4;
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlParaType, !bSpecialGroundType);

	if (m_nParaTypeRdo == 4) bUserDefined = TRUE;

	if (bSpecialGroundType || bUserDefined) bAutomatic = FALSE;

	// Soil Factor(S)는 Horizontal Spectrum인 경우 : Ground Type이 S1, S2 이거나, Spectrum Parameter Type이 User Defined인 경우 사용자 입력만 가능
	// Tb, Tc, Td는 Ground Type이 S1, S2 이거나, Spectrum Parameter Type이 User Defined인 경우 사용자 입력만 가능
	m_wndS.SetReadOnly(!bSpecialGroundType && !bUserDefined);
	m_wndTb.SetReadOnly(!bSpecialGroundType && !bUserDefined);
	m_wndTc.SetReadOnly(!bSpecialGroundType && !bUserDefined);
	m_wndTd.SetReadOnly(!bSpecialGroundType && !bUserDefined);

	if (bAutomatic)
	{
		double dAg, dQ, dAmpF, dTcStar, dS, dTb, dTc, dTd;

		dAg = m_wndAg.GetEditValue();
		dQ = m_wndQ.GetEditValue();
		dAmpF = m_wndAmpF.GetEditValue();
		dTcStar = m_wndTcStar.GetEditValue();

		CLateralLoad load;
		if (m_bNTC2018Mode)
		{
			if (fabs(dTcStar) > 1.0e-10)
			{
				load.Get_NTC2018_Spectrum_Parameter(nGroundType, m_nParaTypeRdo, dAg, dQ, dAmpF, dTcStar, dS, dTb, dTc,
				                                    dTd);
			}
		}
		else
		{
			load.Get_NTC2008_Spectrum_Parameter(nGroundType, m_nParaTypeRdo, dAg, dAmpF, dTcStar, dS, dTb, dTc, dTd);
		}

		m_wndS.SetEditUnit(dS, 2);
		m_wndTb.SetEditUnit(dTb, 2);
		m_wndTc.SetEditUnit(dTc, 2);
		m_wndTd.SetEditUnit(dTd, 2);
	}

	UpdateData(FALSE);
}

void CSeisItemNTC2012::OnNTC2012ParatypeRdo()
{
	CtrlManager();
}

void CSeisItemNTC2012::OnSelchangeNTC2012GroundCmb()
{
	CtrlManager();
}

void CSeisItemNTC2012::OnKillfocusEdit()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRadioSpecType, m_nParaTypeRdo);
	if (m_nParaTypeRdo != 4)
		CtrlManager();
}

LRESULT CSeisItemNTC2012::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}
