// CSeisCvlJpSetInvestSidpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCvlJpSetInvestSidpDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COMBO_DIRECTION_SEISMIC			_LS(IDS_IDD_TM_SIDP_IDC_TM_SIDP_DIR_SEISMIC)
#define COMBO_DIRECTION_ELEM_COORDINATE	_LS(IDS_IDD_TM_SIDP_IDC_TM_SIDP_DIR_ELEM_COORDINATE)
#define COMBO_DIRECTION_USER_DEFINE		_LS(IDS_IDD_TM_SIDP_IDC_TM_SIDP_DIR_USER_DEFINE)

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestSidpDlg dialog


CSeisCvlJpSetInvestSidpDlg::CSeisCvlJpSetInvestSidpDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisCvlJpSetInvestSidpDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nOption = 0;
	m_nAllowDispPlasFormula = 0;
	m_nLpFormula = 0;
}

CSeisCvlJpSetInvestSidpDlg::~CSeisCvlJpSetInvestSidpDlg()
{
	m_pDoc = NULL;
}

void CSeisCvlJpSetInvestSidpDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCvlEvalGrupDlg)
	DDX_Radio(pDX, IDC_TM_SIDP_ADD_RDO, m_nOption);
	DDX_Control(pDX, IDC_TM_SIDP_SIPA_CMB, m_caseName);
	DDX_Control(pDX, IDC_TM_SIDP_SIGR_CMB, m_groupName);
	DDX_Control(pDX, IDC_TM_SIDP_COLM_GROUP_CMB, m_colmGroupName);
	DDX_Radio(pDX, IDC_TM_SIDP_ALLO_DISP_PLAS_RATE1_RDO, m_nAllowDispPlasFormula);
	DDX_Control(pDX, IDC_TM_SIDP_ALLO_DISP_PLAS_RATE_ALPHA_EDT, m_Alpha);
	DDX_Control(pDX, IDC_TM_SIDP_ALLO_DISP_PLAS_RATE_DELTA_LS_EDT, m_DeltaLs);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeisCvlJpSetInvestSidpDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSeisCvlJpSetInvestSidpDlg)
	ON_CBN_SELCHANGE(IDC_TM_SIDP_SIPA_CMB, OnChangedCaseCmb)
	ON_CBN_SELCHANGE(IDC_TM_SIDP_SIGR_CMB, OnChangedGroupCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSeisCvlJpSetInvestSidpDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	InitControl();
	InitCaseNameCtrl();
	InitGroupNameCtrl();
	UpdateColmGroupNameCtrl();
	InitAdprFormulaCtrl();
	InitDeltaLs_LengthCtrl();
	UpdateControl_Alpha();
	UpdateControl_DeltaLs();
	return TRUE;// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisCvlJpSetInvestSidpDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_UNIT:
		InitDeltaLs_LengthCtrl();
		break;
	default:
		break;
	}
}

void CSeisCvlJpSetInvestSidpDlg::OnChangedCaseCmb()
{
	UpdateControl_Alpha();
	UpdateControl_DeltaLs();
}

void CSeisCvlJpSetInvestSidpDlg::OnChangedGroupCmb()
{
	UpdateColmGroupNameCtrl();
	UpdateControl_Alpha();
	UpdateControl_DeltaLs();
}

void CSeisCvlJpSetInvestSidpDlg::Execute()
{
	UpdateData(TRUE);

	if (m_nOption == 0)
		AddOrModData();
	else
		DelData();
}

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestSidpDlg diagnostics
// 
void CSeisCvlJpSetInvestSidpDlg::InitControl()
{
	T_SIDC_D sidcD; sidcD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(sidcD);

	CButton* pBtn = NULL;
	//옵션 추가/변경 삭제 라디오 버튼
	pBtn = (CButton*)GetDlgItem(IDC_TM_SIDP_ADD_RDO);
	pBtn->SetCheck(1);
	m_nOption = 0;
	
	//허용변위 소성률
	if (sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H14)
	{
		pBtn = (CButton*)GetDlgItem(IDC_TM_SIDP_ALLO_DISP_PLAS_RATE1_RDO);
		pBtn->SetCheck(1);
		m_nAllowDispPlasFormula = 0;
		GetDlgItem(IDC_TM_SIDP_ALLO_DISP_PLAS_RATE2_RDO)->EnableWindow(FALSE);
	}
	else if (sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H24)
	{
		pBtn = (CButton*)GetDlgItem(IDC_TM_SIDP_ALLO_DISP_PLAS_RATE2_RDO);
		pBtn->SetCheck(1);
		m_nAllowDispPlasFormula = 1;
		GetDlgItem(IDC_TM_SIDP_ALLO_DISP_PLAS_RATE1_RDO)->EnableWindow(FALSE);
	}

	pBtn = (CButton*)GetDlgItem(IDC_TM_SIDP_ALLO_DISP_PLAS_RATE2_RDO);
	pBtn->EnableWindow(sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H14);

}

void CSeisCvlJpSetInvestSidpDlg::InitCaseNameCtrl()
{
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->GetCount() < 1)
		return;

	T_SIDC_D sidcD; sidcD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(sidcD);

	m_caseName.ResetContent();

	CArray<T_SIPA_K, T_SIPA_K> keys;
	m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKeyList(keys);
	for (int i = 0; i < keys.GetCount(); i++)
	{
		T_SIPA_K& key = keys[i];
		T_SIPA_D data;
		m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(key, data);

		if (sidcD.nDesignCode != data.nDesignCode)
			continue;

		m_caseName.AddString(data.strCaseName);
	}
	m_caseName.SetCurSel(0);
}

void CSeisCvlJpSetInvestSidpDlg::InitGroupNameCtrl()
{
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetCount() < 1)
		return;

	m_groupName.ResetContent();

	CArray<T_SIGR_LOWER_K, T_SIGR_LOWER_K> keys;
	m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetKeyList(keys);
	for (int i = 0; i < keys.GetCount(); i++)
	{
		T_SIGR_LOWER_K& key = keys[i];
		T_SIGR_LOWER_D data;
		m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(key, data);
// 		if (data.nMaterialType != T_SIGR_LOWER_D::kConcrete)
// 			continue;

		CString str = m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetSigrName(key);
		m_groupName.AddString(str);
	}
	m_groupName.SetCurSel(0);
}

void CSeisCvlJpSetInvestSidpDlg::UpdateColmGroupNameCtrl()
{
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetCount() < 1)
		return;

	CString strGroup;
	m_groupName.GetWindowText(strGroup);

	T_SIGR_K kGroup = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroup);

	T_SIGR_LOWER_D groupD;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kGroup, groupD) == FALSE)
		return;

	m_colmGroupName.ResetContent();

	for (int i = 0; i < groupD.COLM_D.GetCount(); i++)
	{
		CString str = groupD.COLM_D[i].strName;
		m_colmGroupName.AddString(str);
	}
	
	m_colmGroupName.SetCurSel(0);
}

void CSeisCvlJpSetInvestSidpDlg::InitAdprFormulaCtrl()
{
	T_SIDC_D sidcD; sidcD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(sidcD);

	CButton* pBtn = NULL;

	pBtn = (CButton*)GetDlgItem(IDC_TM_SIDP_ALLO_DISP_PLAS_RATE1_RDO);
	pBtn->SetCheck(sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H14);

	pBtn = (CButton*)GetDlgItem(IDC_TM_SIDP_ALLO_DISP_PLAS_RATE2_RDO);
	pBtn->EnableWindow(sidcD.nDesignCode != BrdgSE_CodeJP::JROAD5_H14);
	pBtn->SetCheck(sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H24);

	if (sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H14)
		m_nAllowDispPlasFormula = 0;
	else if (sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H24)
		m_nAllowDispPlasFormula = 1;
}

void CSeisCvlJpSetInvestSidpDlg::InitDeltaLs_LengthCtrl()
{
	T_SIDP_UNIT* pUnit = &(CUnitCtrl::m_SIDP_UNIT);

	CString strUnit;
	CUnitCtrl::GetUnitSystem(pUnit->Length, strUnit);

	CString str; str.Format(_LS(IDS_IDD_TM_SIDP_IDC_TM_SIDP_ALLO_DISP_PLAS_RATE_DELTA_LS_STC), strUnit);

	GetDlgItem(IDC_TM_SIDP_ALLO_DISP_PLAS_RATE_DELTA_LS_STC)->SetWindowText(str);
}

void CSeisCvlJpSetInvestSidpDlg::UpdateControl_Alpha()
{
	T_SIDC_D codeD; codeD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(codeD);

	CString strCase;
	m_caseName.GetWindowText(strCase);

	T_SIPA_K kSipa = m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKey(strCase);

	T_SIPA_D sipaD;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(kSipa, sipaD) == FALSE)
		return;

	CString strGroup;
	m_groupName.GetWindowText(strGroup);

	T_SIGR_K kGroup = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroup);

	T_SIGR_LOWER_D groupD;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kGroup, groupD) == FALSE)
		return;

	if (codeD.nDesignCode == BrdgSE_CodeJP::JROAD5_H14)
		UpdateControl_AlphaH14(sipaD, groupD);
	else if (codeD.nDesignCode == BrdgSE_CodeJP::JROAD5_H24)
		UpdateControl_AlphaH24(groupD);
}

void CSeisCvlJpSetInvestSidpDlg::UpdateControl_AlphaH14(const T_SIPA_D& sipaD, const T_SIGR_LOWER_D& groupD)
{
	GetDlgItem(IDC_TM_SIDP_ALLO_DISP_PLAS_RATE_ALPHA_STC)->SetWindowText(_LS(IDS_IDD_TM_SIDP_IDC_TM_SIDP_ALLO_DISP_PLAS_RATE_ALPHA_STC));

	double dAlpha = 0;
	if (sipaD.nSesmicPerformanceType == T_SIPA_D::kLevel2) {
		if(sipaD.nSeismicForceType == T_SIPA_D::kType1)
			dAlpha = 3.0;
		else if(sipaD.nSeismicForceType == T_SIPA_D::kType2)
			dAlpha = 1.5;
	}
	else if (sipaD.nSesmicPerformanceType == T_SIPA_D::kLevel3) {
		if (sipaD.nSeismicForceType == T_SIPA_D::kType1)
			dAlpha = 2.4;
		else if (sipaD.nSeismicForceType == T_SIPA_D::kType2)
			dAlpha = 1.2;
	}

	CString strDef;
	if (groupD.BEAR_D.GetCount() < 1) {
		strDef.Format(_T("%g"), dAlpha);
		m_Alpha.SetWindowText(strDef);
		return;
	}

	BOOL bSeismicIsolation = FALSE;
	for (int i = 0; i < groupD.BEAR_D.GetCount(); i++)
	{
		if (groupD.BEAR_D[i].nType == T_SIGR_LOWER_BEAR_D::kSeismicIsolation) {
			bSeismicIsolation = TRUE;
			break;
		}
	}

	if (bSeismicIsolation == TRUE)
		dAlpha *= 2;

	strDef.Format(_T("%g"), dAlpha);
	m_Alpha.SetWindowText(strDef);

	if (bSeismicIsolation == TRUE)
		GetDlgItem(IDC_TM_SIDP_ALLO_DISP_PLAS_RATE_ALPHA_STC)->SetWindowText(_LS(IDS_IDD_TM_SIDP_IDC_TM_SIDP_ALLO_DISP_PLAS_RATE_ALPHA_M_STC));
}

void CSeisCvlJpSetInvestSidpDlg::UpdateControl_AlphaH24(const T_SIGR_LOWER_D& data)
{
	GetDlgItem(IDC_TM_SIDP_ALLO_DISP_PLAS_RATE_ALPHA_STC)->SetWindowText(_LS(IDS_IDD_TM_SIDP_IDC_TM_SIDP_ALLO_DISP_PLAS_RATE_ALPHA_STC));

	double dAlpha = 1.2;

	CString strDef;
	if (data.BEAR_D.GetCount() < 1) {
		strDef.Format(_T("%g"), dAlpha);
		m_Alpha.SetWindowText(strDef);
		return;
	}

	BOOL bSeismicIsolation = FALSE;
	for (int i = 0; i < data.BEAR_D.GetCount(); i++)
	{
		if (data.BEAR_D[i].nType == T_SIGR_LOWER_BEAR_D::kSeismicIsolation) {
			bSeismicIsolation = TRUE;
			break;
		}
	}

	if (bSeismicIsolation == TRUE)
		dAlpha *= 2;

	strDef.Format(_T("%g"), dAlpha);
	m_Alpha.SetWindowText(strDef);

	if (bSeismicIsolation == TRUE)
		GetDlgItem(IDC_TM_SIDP_ALLO_DISP_PLAS_RATE_ALPHA_STC)->SetWindowText(_LS(IDS_IDD_TM_SIDP_IDC_TM_SIDP_ALLO_DISP_PLAS_RATE_ALPHA_M_STC));
}

void CSeisCvlJpSetInvestSidpDlg::UpdateControl_DeltaLs()
{
	CString strCase; CString strGroup;
	m_caseName.GetWindowText(strCase);
	m_groupName.GetWindowText(strGroup);

	T_SIPA_K kSipa = m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKey(strCase);
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroup);

	T_SIRD_D tSird;
	if (m_pDoc->m_pAttrCtrl2->GetQSird()->Get(kSipa, kSigr, tSird) == FALSE) return;

	m_DeltaLs.EnableWindow(tSird.nAlwCalcType == seis_jp::kUserInput);
}

BOOL CSeisCvlJpSetInvestSidpDlg::AddOrModData()
{
	T_SIDP_D data; CString msg;
	if (DlgToData(data, msg) == FALSE) {
		AfxMessageBox(msg);
		return FALSE;
	}

	//조사 케이스 + 조사 대상 조합으로 기존에 등록되어 있는게 있는지 체크
	BOOL bResult = FALSE;
	T_SIDP_K key = GetExistingDataKey(data);
	if (key == 0) { // Add
		key = m_pDoc->m_pAttrCtrl2->GetQSidp()->GetStartNum();
		bResult = m_pDoc->m_pDataCtrl->AddSidp(key, data);
	}
	else { //Mod
		bResult = m_pDoc->m_pDataCtrl->ModifySidp(key, data);
	}

	ASSERT(bResult);
	return bResult;
}

BOOL CSeisCvlJpSetInvestSidpDlg::DelData()
{
	T_SIDP_D delData; CString msg;
	if (DlgToData(delData, msg) == FALSE) {
		AfxMessageBox(msg);
		return FALSE;
	}

	T_SIDP_K key = 0;
	T_SIDP_D data;

	POSITION pos = m_pDoc->m_pAttrCtrl2->GetQSidp()->GetStart();
	while (pos)
	{
		m_pDoc->m_pAttrCtrl2->GetQSidp()->GetNext(pos, key, data);
		if (data.kSipa == delData.kSipa && data.kSigrLower == delData.kSigrLower) {
			return m_pDoc->m_pDataCtrl->DelSidp(key);
		}
	}

	AfxMessageBox(_LS(IDS_IDD_TM_SIDP_MSG_NOT_EXIST_EQUAL_SIPA_SIGR));
	return FALSE;
}

BOOL CSeisCvlJpSetInvestSidpDlg::DlgToData(OUT T_SIDP_D& rData, OUT CString& errMsg)
{
	CString str;
	//조사 케이스
	m_caseName.GetWindowText(str);
	rData.kSipa = m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKey(str);
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Exist(rData.kSipa) == FALSE) {
		ASSERT(FALSE);
		return FALSE;
	}
	//조사 대상
	m_groupName.GetWindowText(str);
	rData.kSigrLower = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(str);
	T_SIGR_LOWER_D tSigrLower;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(rData.kSigrLower, tSigrLower) == FALSE) {
		ASSERT(FALSE);
		return FALSE;
	}
	//기둥그룹
	m_colmGroupName.GetWindowText(str);
	rData.strNameCOLM = str;
	//허용변위 소성률 계산식
	if (m_nAllowDispPlasFormula == 0)
		rData.ADPR_D.nAllowDispPlasFormula = T_SIDP_ADPR_D::kMuA_1_Plus_Ellipsis;
	else if (m_nAllowDispPlasFormula == 1)
		rData.ADPR_D.nAllowDispPlasFormula = T_SIDP_ADPR_D::kMuA_DeltaLs_Ellipsis;
	//안전계수α
	m_Alpha.GetWindowText(str);
	if (str.IsEmpty()) { errMsg = _LS(IDS_IDD_TM_SIDP_MSG_INVALID_ALPHA); return FALSE; }
	rData.ADPR_D.dAlpha = _wtof(str);
	//δls
	m_DeltaLs.GetWindowText(str);
	if (m_DeltaLs.IsWindowEnabled() && str.IsEmpty()) { errMsg = _LS(IDS_IDD_TM_SIDP_MSG_INVALID_DELTA_LS); return FALSE; }
	rData.ADPR_D.dDeltaLs = _wtof(str);

	return TRUE;
}

T_SIDP_K CSeisCvlJpSetInvestSidpDlg::GetExistingDataKey(const T_SIDP_D& newData)
{
	CString str;
	//조사 케이스 + 조사 대상 조합으로 기존에 등록되어 있는게 있는지 체크
	T_SIDP_K key;
	T_SIDP_D data;

	POSITION pos = m_pDoc->m_pAttrCtrl2->GetQSidp()->GetStart();
	while (pos)
	{
		m_pDoc->m_pAttrCtrl2->GetQSidp()->GetNext(pos, key, data);
		if (data.kSipa == newData.kSipa && data.kSigrLower == newData.kSigrLower) {
			return key;
		}
	}

	return 0;
}