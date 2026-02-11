// PsllItemKISTEC2019.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "PsllItemKISTEC2019.h"
#include "SeisKBC2005PeriodDlg.h"
#include "SeisKDS4117PeriodDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPsllItemKISTEC2019 dialog


CPsllItemKISTEC2019::CPsllItemKISTEC2019(CWnd* pParent /*=NULL*/)
	: CChildDialog(CPsllItemKISTEC2019::IDD, pParent)
{
}


void CPsllItemKISTEC2019::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPsllItemKISTEC2019)
	DDX_Control(pDX, IDC_CMD_ZONE_CMB,        m_cmbSeisZone);
	DDX_Control(pDX, IDC_CMD_ZONE_FACTOR_CMB, m_cmbSeisZoneFactor);
	DDX_Control(pDX, IDC_CMD_SITE_CMB,        m_cmbSiteClass);
	DDX_Control(pDX, IDC_CMD_CU_EDIT,       m_editCu);

	DDX_Control(pDX, IDC_CMD_FA_EDIT,       m_editFa);
	DDX_Control(pDX, IDC_CMD_FV_EDIT,       m_editFv);
	DDX_Control(pDX, IDC_CMD_SDS_EDIT,      m_editSds);
	DDX_Control(pDX, IDC_CMD_SD1_EDIT,      m_editSd1);

	DDX_Control(pDX, IDC_CMD_TX_APPR_EDIT,  m_editApprX);
	DDX_Control(pDX, IDC_CMD_TY_APPR_EDIT,  m_editApprY);

	DDX_Control(pDX, IDC_CMD_PARAM_C,  m_editParamC);
	DDX_Control(pDX, IDC_CMD_PARAM_J,  m_editParamJ);
	DDX_Control(pDX, IDC_CMD_PARAM_X,  m_editParamX);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPsllItemKISTEC2019, CChildDialog)
	//{{AFX_MSG_MAP(CPsllItemKISTEC2019)
	ON_CBN_SELCHANGE(IDC_CMD_ZONE_CMB,        OnSelchangeCmdZoneCmb)
	ON_CBN_SELCHANGE(IDC_CMD_ZONE_FACTOR_CMB, OnSelchangeCmdZoneFactorCmb)
	ON_CBN_SELCHANGE(IDC_CMD_SITE_CMB,        OnSelchangeCmdSiteCmb)
	ON_BN_CLICKED(IDC_CMD_PERIOD_BTN, OnCmdPeriodBtn)
	ON_CBN_EDITCHANGE(IDC_CMD_ZONE_FACTOR_CMB, OnChangeCmdZoneFactor)

	ON_EN_CHANGE(IDC_CMD_FA_EDIT,      OnChangeCmdFaEdit)
	ON_EN_CHANGE(IDC_CMD_FV_EDIT,      OnChangeCmdFvEdit)
	ON_EN_CHANGE(IDC_CMD_SD1_EDIT,     OnChangeCmdSd1Edit)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPsllItemKISTEC2019 message handlers

BOOL CPsllItemKISTEC2019::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_editApprX.SetUnitType(D_UNITSYS_NONE);
	m_editApprY.SetUnitType(D_UNITSYS_NONE);

	GetDlgItem(IDC_WG_CMD_STATIC3)->SetWindowText(_LS(IDS_CMD_EPA));

	InitZoneCombo();
	InitZoneFactorCombo();
	InitSiteCombo();

	if(!m_bModify)
	{
		m_Data.Initialize();
		InitDefaultData();
	}

	CString strTemp;
	strTemp.Format(_T("%g"), m_Data.dZoneFactor);
	m_cmbSeisZoneFactor.SetWindowText(strTemp);

	InitDefaultPeriodData();
	Data2Dlg();	

	if(!m_bModify)
		SetFaFvSdsSd1();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPsllItemKISTEC2019::DestroyWindow() 
{
	return CChildDialog::DestroyWindow();
}

void CPsllItemKISTEC2019::SetData2Dlg(T_PSLL_KISTEC2019& data, BOOL bModify, int nMethod)
{
	m_Data = data;
	m_bModify = bModify;
	m_nMethod = nMethod;
}
// 
BOOL CPsllItemKISTEC2019::SetDlg2Data(T_PSLL_KISTEC2019& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CPsllItemKISTEC2019::InitDefaultData()
{
	CDBDoc* pDoc  = (CDBDoc*) CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;	
	
	T_UNIT_INDEX CurIndex, CodeIndex;
	pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CodeIndex = CurIndex;
	CodeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M; // M 기준
	pUnitCtrl->SetUnitIndexCurrent(CodeIndex);

	int nStorCount = pDoc->m_pAttrCtrl->GetCountStor() - 1;
	if(nStorCount <= 0)
	{
		m_dC = 1.0;
	}
	else
	{
		T_SEME_D data;
		pDoc->m_pAttrCtrl2->GetSeme(data);
		int nStructureType = data.data.eval.nResistSystem;
		double dTable4_2_1[3][5] = { {0.0, 1.3, 1.1, 1.0, 1.0},		{0.0, 1.4, 1.2, 1.1, 1.0},		{0.0, 1.0, 1.0, 1.0, 1.0} };

		ASSERT(nStorCount >= 0);

		if(nStorCount>=4) m_Data.dC = m_dC = dTable4_2_1[nStructureType][4];
		else m_Data.dC = m_dC = dTable4_2_1[nStructureType][nStorCount];
	}
	// <Remember> Data를 받고나서는 User Unit기준
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);
}

void CPsllItemKISTEC2019::InitDefaultPeriodData()
{
	CDBDoc* pDoc  = (CDBDoc*) CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;	
	T_STOR_D StorD, BaseStorD;

	CCurUnitSaver SaveUnit(TRUE); // {}밖으로 빠져 나갈때 단위계 자동 복구
	T_UNIT_INDEX CurIndex, CodeIndex;
	pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CodeIndex = CurIndex;
	CodeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M; // M 기준
	pUnitCtrl->SetUnitIndexCurrent(CodeIndex);
	
	double dHn = 0.0;
	double dBaseLevel = pDoc->m_pAttrCtrl->GetBaseLevel();

	CArray<T_STOR_K,T_STOR_K> aStorK;
	pAttrCtrl->GetStorKeyList(aStorK);
	int nStorNum = aStorK.GetSize();
	if (nStorNum>0) pAttrCtrl->GetStor(aStorK[nStorNum-1], StorD);
	else StorD.Initialize();

	dHn = StorD.dStoryLevel - dBaseLevel;		
	if (!pAttrCtrl->GetCountStor())	dHn = 0;

	m_nNX  = m_nNY = nStorNum - 1;
	m_dHnX = m_dHnY = dHn;
	m_dAcX = m_dAcY = 0.0;
	m_nMtdX = m_nMtdY = 0;
}

void CPsllItemKISTEC2019::SetComboValue()
{
	int nSeismicZone = m_cmbSeisZone.GetCurSel();
	int nSiteClass   = m_cmbSiteClass.GetCurSel();

	if(nSeismicZone == 0)
	{
		m_cmbSeisZoneFactor.SetWindowText(_T("0.22"));
	}
	else if(nSeismicZone == 1)
	{
		m_cmbSeisZoneFactor.SetWindowText(_T("0.14"));
	}
	
	SetFaFvSdsSd1();
}

void CPsllItemKISTEC2019::SetFaFvSdsSd1()
{
	UpdateData();
	
	CString str; 

	m_cmbSeisZoneFactor.GetWindowText(str);
	double dZoneFactor  = _tstof(str);
	int    nSiteClass   = m_cmbSiteClass.GetCurSel();

	double dFa=0., dFv=0., dSds=0., dSd1=0., dCu=0.;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	
	if(nSiteClass >= 0 && nSiteClass <= 4)
	{
		if(!pDoc->m_pAttrCtrl->CalcKDS2019Fa(dZoneFactor, nSiteClass, dFa)) return;
		if(!pDoc->m_pAttrCtrl->CalcKDS2019Fv(dZoneFactor, nSiteClass, dFv)) return;
		if(!pDoc->m_pAttrCtrl->CalcKBC2009Sds(dZoneFactor, dFa, dSds)) return;
		if(!pDoc->m_pAttrCtrl->CalcKBC2009Sd1(dZoneFactor, dFv, dSd1)) return;
	}

	if(!pDoc->m_pAttrCtrl->CalcKBC2009Cu(dSd1, dCu)) return;
	
	m_editFa.SetEditUnit(dFa, 5);
	m_editFv.SetEditUnit(dFv, 5);
	m_editSds.SetEditUnit(dSds, 5);
	m_editSd1.SetEditUnit(dSd1, 5);
}

void CPsllItemKISTEC2019::SetKey(T_PSLL_K key)
{
	m_Key = key;
}

void CPsllItemKISTEC2019::OnSelchangeCmdSiteCmb() 
{
	SetFaFvSdsSd1();
}

void CPsllItemKISTEC2019::OnSelchangeCmdZoneCmb() 
{
	int nSiteClass = m_cmbSiteClass.GetCurSel();
	if(nSiteClass==5) return;

	SetComboValue();
	SetFaFvSdsSd1();
}

void CPsllItemKISTEC2019::OnSelchangeCmdZoneFactorCmb() 
{
	int nSiteClass = m_cmbSiteClass.GetCurSel();
	if(nSiteClass==5) return;

	// Edit Box 값을 변경후에 Combo 로 값을 변경하면 바로 Update 안되는 문제...

	CString csValue;

	int nTmp = m_cmbSeisZoneFactor.GetCurSel();
	if (nTmp == 0)
	{
		csValue.Format(_T("%g"), 0.14);
		m_cmbSeisZoneFactor.SetWindowText(csValue);
	}
	else
	{
		csValue.Format(_T("%g"), 0.22);
		m_cmbSeisZoneFactor.SetWindowText(csValue);
	}

	SetFaFvSdsSd1();	
}

void CPsllItemKISTEC2019::OnChangeCmdZoneFactor() 
{
	int nSiteClass = m_cmbSiteClass.GetCurSel();
	if(nSiteClass==5) return;

	SetFaFvSdsSd1();
}

void CPsllItemKISTEC2019::OnChangeCmdFaEdit() 
{
	int nSiteClass = m_cmbSiteClass.GetCurSel();
	if(nSiteClass==5) return;

	CString str;

	m_cmbSeisZoneFactor.GetWindowText(str);
	double dZoneFactor  = _tstof(str);
	double dFa = m_editFa.GetEditValue();
	double dSds;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(!pDoc->m_pAttrCtrl->CalcKBC2009Sds(dZoneFactor, dFa, dSds)) return;

	m_editSds.SetEditUnit(dSds, 5);
}

void CPsllItemKISTEC2019::OnChangeCmdFvEdit() 
{
	int nSiteClass = m_cmbSiteClass.GetCurSel();
	if(nSiteClass==5) return;

	CString str; 

	m_cmbSeisZoneFactor.GetWindowText(str);
	double dZoneFactor  = _tstof(str);
	double dFv = m_editFv.GetEditValue();
	double dSd1, dCu;  
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(!pDoc->m_pAttrCtrl->CalcKBC2009Sd1(dZoneFactor, dFv, dSd1)) return;
	if(!pDoc->m_pAttrCtrl->CalcKBC2009Cu(dSd1, dCu)) return;

	m_editCu.SetEditUnit(dCu, 5);
	m_editSd1.SetEditUnit(dSd1, 5);
}

void CPsllItemKISTEC2019::OnChangeCmdSd1Edit() 
{ 
	double dSd1 = m_editSd1.GetEditValue();
	double dCu = 0.0;  

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(!pDoc->m_pAttrCtrl->CalcKBC2009Cu(dSd1, dCu)) return;

	m_editCu.SetEditUnit(dCu, 5);
}

void CPsllItemKISTEC2019::OnCmdPeriodBtn() 
{
	CString strMethod = CDBLib::GetSeisEvalCodeNameByMethod(m_nMethod);
	CString strTitle = _T("");
	strTitle.Format(_T("%s Period Calculator"), strMethod);
	
	CSeisKDS4117PeriodDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.SetTitleText(strTitle);

	dlg.m_nMethodX = m_nMtdX;
	dlg.m_nMethodY = m_nMtdY;
	dlg.m_dHnX     = m_dHnX;
	dlg.m_dHnY     = m_dHnY;
	dlg.m_nNX      = m_nNX;
	dlg.m_nNY      = m_nNY;

	if (dlg.DoModal() == IDOK)
	{
		CString cx,cy;
		cx.Format(_T("%-10.4g"), dlg.m_dPeriodX);
		cy.Format(_T("%-10.4g"), dlg.m_dPeriodY);
		m_editApprX.SetEditUnit(_tstof(cx));
		m_editApprY.SetEditUnit(_tstof(cy));

		m_nMtdX = dlg.m_nMethodX;
		m_nMtdY = dlg.m_nMethodY;
		m_dHnX  = dlg.m_dHnX    ;
		m_dHnY  = dlg.m_dHnY    ;
		m_nNX   = dlg.m_nNX     ;
		m_nNY   = dlg.m_nNY     ;
	}
}

void CPsllItemKISTEC2019::InitZoneCombo()
{
	CString aItem[2] = {_T("1"), _T("2")};

	m_cmbSeisZone.ResetContent();
	
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		CDlgUtil::CobxAddItem(m_cmbSeisZone, aItem[i], i);

	CDlgUtil::CobxSetCurSelItemData(m_cmbSeisZone,0);
}

void CPsllItemKISTEC2019::InitZoneFactorCombo()
{
	CString aItem[2] = {_T("0.14"), _T("0.22")};

	m_cmbSeisZoneFactor.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		CDlgUtil::CobxAddItem(m_cmbSeisZoneFactor, aItem[i], i);
}

void CPsllItemKISTEC2019::InitSiteCombo()
{
	CString aItem[6] = {_T("S1"), _T("S2"), _T("S3"), _T("S4"), _T("S5"), _T("S6")};

	m_cmbSiteClass.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		CDlgUtil::CobxAddItem(m_cmbSiteClass, aItem[i], i);

	CDlgUtil::CobxSetCurSelItemData(m_cmbSiteClass,0);
}

void CPsllItemKISTEC2019::Data2Dlg()
{
	CString csValue;
	m_cmbSeisZone.SetCurSel(m_Data.nSeismicZone);
	m_cmbSiteClass.SetCurSel(m_Data.nSiteClass);

	csValue.Format(_T("%g"), m_Data.dZoneFactor);
	m_cmbSeisZoneFactor.SetWindowText(csValue);	

	m_editCu.SetEditUnit(m_Data.dPeriodCoef , 5);
	
	m_editFa.SetEditUnit(m_Data.dFa, 5);
	m_editFv.SetEditUnit(m_Data.dFv, 5);
	m_editSds.SetEditUnit(m_Data.dSds, 5);
	m_editSd1.SetEditUnit(m_Data.dSd1, 5);

	m_editParamC.SetEditUnit(m_Data.dC,2);
	m_editParamJ.SetEditUnit(m_Data.dJ,2);
	m_editParamX.SetEditUnit(m_Data.dX,2);
	m_editApprX.SetEditUnit(m_Data.dPeriodApprX);
	m_editApprY.SetEditUnit(m_Data.dPeriodApprY);

	UpdateData(FALSE);
}

BOOL CPsllItemKISTEC2019::Dlg2Data()
{
	UpdateData();

	CString str; 

	m_Data.nSeismicZone  = m_cmbSeisZone.GetCurSel();
	m_cmbSeisZoneFactor.GetWindowText(str);
	m_Data.dZoneFactor	= _tstof(str);
	m_Data.nSiteClass	= m_cmbSiteClass.GetCurSel();
	m_Data.dPeriodCoef	= m_editCu.GetEditValue();
	m_Data.dFa			= m_editFa.GetEditValue();
	m_Data.dFv			= m_editFv.GetEditValue();
	m_Data.dSds			= m_editSds.GetEditValue();
	m_Data.dSd1			= m_editSd1.GetEditValue();
	m_Data.dC			= m_editParamC.GetEditValue();
	m_Data.dJ			= m_editParamJ.GetEditValue();
	m_Data.dX			= m_editParamX.GetEditValue();
	m_Data.dPeriodApprX	= m_editApprX.GetEditValue();
	m_Data.dPeriodApprY	= m_editApprY.GetEditValue();
	return TRUE;
}

// Cu 미고려 시 1.0으로 설정
void CPsllItemKISTEC2019::OnNotConsCuChk()
{
	UpdateData();
	SetFaFvSdsSd1();
}
