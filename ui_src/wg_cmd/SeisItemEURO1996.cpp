// SeisItemEURO1996.cpp : implementation file
//
// 2003. 12. 29   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemEURO1996.h"
#include "SeisEuroCalcDlg.h"
#include "SeisItemDlgNew.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_base\wg_base_ControlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisItemEURO1996 dialog


CSeisItemEURO1996::CSeisItemEURO1996(CWnd* pParent /*=NULL*/)
	: CChildDialog(CSeisItemEURO1996::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisItemEURO1996)
	//}}AFX_DATA_INIT
	m_bInit = FALSE;
	m_Data.Initialize();
	m_pParent = (CSeisItemDlgNew*)pParent;
}

void CSeisItemEURO1996::SetData2Dlg(T_SEIS_EURO1996& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CSeisItemEURO1996::SetDlg2Data(T_SEIS_EURO1996& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CSeisItemEURO1996::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisItemEURO1996)
	DDX_Control(pDX, IDC_CMD_EURO_SC_COMBO, m_cobxSc);
	DDX_Control(pDX, IDC_CMD_EURO_Q0_COMBO, m_cobxQo);
	DDX_Control(pDX, IDC_CMD_EURO_KD_COMBO, m_cobxKd);
	DDX_Control(pDX, IDC_CMD_EURO_KR_COMBO, m_cobxKr);
	DDX_Control(pDX, IDC_CMD_EURO_KW_EDIT, m_editKw);
	DDX_Control(pDX, IDC_CMD_EURO_ALPHA_EDIT, m_editAlpha);
	DDX_Control(pDX, IDC_CMD_EURO_PERIOD_Y_EDIT, m_editPeriodY);
	DDX_Control(pDX, IDC_CMD_EURO_PERIOD_X_EDIT, m_editPeriodX);
	DDX_Control(pDX, IDC_CMD_EURO_FRAME1, m_grpFold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisItemEURO1996, CChildDialog)
	//{{AFX_MSG_MAP(CSeisItemEURO1996)
	ON_BN_CLICKED(IDC_CMD_EURO_BTN_CALC_PERIOD, OnCmdEuroBtnCalcPeriod)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisItemEURO1996 message handlers

BOOL CSeisItemEURO1996::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_editKw.SetUnitType(D_UNITSYS_NONE);
	m_editAlpha.SetUnitType(D_UNITSYS_NONE);
	m_editPeriodY.SetUnitType(D_UNITSYS_NONE);
	m_editPeriodX.SetUnitType(D_UNITSYS_NONE);

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	InitScCombo();
	InitQoCombo();
	InitKdCombo();
	InitKrCombo();

	SetFoldGroupCtrls();

	Data2Dlg();

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisItemEURO1996::SetFoldGroupCtrls()
{
	// After AlignControls
	m_grpFold.InitControl(this, CSeisItemEURO1996::IDD, IDC_CMD_EURO_FRAME1, TRUE, TRUE);
	m_grpFold.SetFoldState(FALSE); // open
}

BOOL CSeisItemEURO1996::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
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

void CSeisItemEURO1996::OnCmdEuroBtnCalcPeriod()
{
	// TODO: Add your control notification handler code here
	CSeisEuroCalcDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT); // child dialog의 위치 지정	
	// 현재 dialog의 변수를 child dialog의 변수에 넘겨줌		
	dlg.m_dXHedit = m_dHXEURO1996;
	dlg.m_dYHedit = m_dHYEURO1996;
	dlg.m_dXAcedit = m_dAcXEURO1996;
	dlg.m_dYAcedit = m_dAcYEURO1996;
	dlg.m_dXDedit = m_nDXEURO1996;
	dlg.m_dYDedit = m_nDYEURO1996;
	dlg.m_nXMethod = m_nMtdXEURO1996;
	dlg.m_nYMethod = m_nMtdYEURO1996;

	double dTc;
	int nSClass = m_cobxSc.GetCurSel() + 1;
	switch (nSClass)
	{
	case 1: dTc = 0.40;
		break;
	case 2: dTc = 0.60;
		break;
	case 3: dTc = 0.80;
		break;
	}

	if (dlg.DoModal() == IDOK)
	{
		double dP_x, dP_y;
		dP_x = dlg.m_dPeriod_x;
		dP_y = dlg.m_dPeriod_y;

		if (dP_x > 4 * dTc || dP_x > 2.0 || dP_y > 4 * dTc || dP_y > 2.0) // 기본진동수의 제한사항
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Restriction_of_fundamental_period_violated_));
		}

		CString cx, cy;
		cx.Format(_T("%-10.4g"), dlg.m_dPeriod_x);
		cy.Format(_T("%-10.4g"), dlg.m_dPeriod_y);
		m_editPeriodX.SetEditUnit(_tstof(cx));
		m_editPeriodY.SetEditUnit(_tstof(cy));

		// child dialog의 변수를 현재 dialog의 변수가 받음		
		m_dHXEURO1996 = dlg.m_dXHedit;
		m_dHYEURO1996 = dlg.m_dYHedit;
		m_dAcXEURO1996 = dlg.m_dXAcedit;
		m_dAcYEURO1996 = dlg.m_dYAcedit;
		m_nDXEURO1996 = dlg.m_dXDedit;
		m_nDYEURO1996 = dlg.m_dYDedit;
		m_nMtdXEURO1996 = dlg.m_nXMethod;
		m_nMtdYEURO1996 = dlg.m_nYMethod;
	}
}


/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CSeisItemEURO1996::InitDefaultData()
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

	m_dHXEURO1996 = m_dHYEURO1996 = dHn;
	m_dAcXEURO1996 = 0.0;
	m_dAcYEURO1996 = 0.0;
	m_nDXEURO1996 = 0.0;
	m_nDYEURO1996 = 0.0;
	m_nMtdXEURO1996 = 0;
	m_nMtdYEURO1996 = 0;

	// <Remember> Data를 받고나서는 User Unit기준
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);
}

void CSeisItemEURO1996::InitScCombo()
{
	m_cobxSc.ResetContent();
	m_cobxSc.AddString(_T("A (1.0)"));
	m_cobxSc.AddString(_T("B (1.0)"));
	m_cobxSc.AddString(_T("C (0.9)"));
	m_cobxSc.SetCurSel(0);
}

void CSeisItemEURO1996::InitQoCombo()
{
	m_cobxQo.ResetContent();
	m_cobxQo.AddString(_T("5.0"));
	m_cobxQo.AddString(_T("4.5"));
	m_cobxQo.AddString(_T("4.0"));
	m_cobxQo.AddString(_T("3.5"));
	m_cobxQo.AddString(_T("2.0"));
	m_cobxQo.SetCurSel(0);
}

void CSeisItemEURO1996::InitKdCombo()
{
	m_cobxKd.ResetContent();
	m_cobxKd.AddString(_LS(IDS_CMD0417__High_1_00_));
	m_cobxKd.AddString(_LS(IDS_CMD0417__Medium_0_75_));
	m_cobxKd.AddString(_LS(IDS_CMD0417__Low_0_50_));
	m_cobxKd.SetCurSel(0);
}

void CSeisItemEURO1996::InitKrCombo()
{
	m_cobxKr.ResetContent();
	m_cobxKr.AddString(_LS(IDS_CMD0417__Regular_1_00_));
	m_cobxKr.AddString(_LS(IDS_CMD0417__Irregular_0_80_));
	m_cobxKr.SetCurSel(0);
}

void CSeisItemEURO1996::Data2Dlg()
{
	m_cobxSc.SetCurSel(m_Data.nSoilClass);
	m_cobxQo.SetCurSel(m_Data.nBasicBehavior);
	m_cobxKd.SetCurSel(m_Data.nDuctility);
	m_cobxKr.SetCurSel(m_Data.nRegularity);
	m_editKw.SetEditUnit(m_Data.dFailureModeFactor);
	m_editAlpha.SetEditUnit(m_Data.dAlphaFactor);
	m_editPeriodX.SetEditUnit(m_Data.dPeriod_X);
	m_editPeriodY.SetEditUnit(m_Data.dPeriod_Y);
}

BOOL CSeisItemEURO1996::Dlg2Data()
{
	m_Data.nSoilClass = m_cobxSc.GetCurSel();
	m_Data.nBasicBehavior = m_cobxQo.GetCurSel();
	m_Data.nDuctility = m_cobxKd.GetCurSel();
	m_Data.nRegularity = m_cobxKr.GetCurSel();
	m_Data.dFailureModeFactor = m_editKw.GetEditValue();
	m_Data.dAlphaFactor = m_editAlpha.GetEditValue();
	m_Data.dPeriod_X = m_editPeriodX.GetEditValue();
	m_Data.dPeriod_Y = m_editPeriodY.GetEditValue();

	if (m_Data.dPeriod_X < 0 || m_Data.dPeriod_Y < 0)
	{
		CString strError = _LS(IDS_CMD_HSFC_error); // Error
		strError += _T("! "); // Error!
		AfxMessageBox(strError + _LS(IDS_WG_CMD__ADD2__Period));
		return FALSE;
	}

	/*
	m_Data.nSoilClass = m_cobxSc.GetCurSel();
	m_Data.nBasicBehavior = m_cobxQo.GetCurSel();
	m_Data.nDuctility = m_cobxKd.GetCurSel();
	m_Data.nRegularity = m_cobxKr.GetCurSel();
	m_Data.dFailureModeFactor = m_editKw.GetEditValue();
	m_Data.dAlphaFactor = m_editAlpha.GetEditValue();
	m_Data.dPeriod_X = m_editPeriodX.GetEditValue();
	m_Data.dPeriod_Y = m_editPeriodY.GetEditValue();
	*/
	return TRUE;
}

LRESULT CSeisItemEURO1996::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}
