// CSeisCvlJpSetInvestSietDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCvlJpSetInvestSietDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COMBO_CONCURVITY_EACH		_LS(IDS_IDD_TM_SIET_CONCURVITY_EACH)
#define COMBO_CONCURVITY_LINE		_LS(IDS_IDD_TM_SIET_CONCURVITY_LINE)
#define COMBO_CONCURVITY_FOOT_LOW	_LS(IDS_IDD_TM_SIET_CONCURVITY_FOOT_LOW)

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestSietDlg dialog


CSeisCvlJpSetInvestSietDlg::CSeisCvlJpSetInvestSietDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisCvlJpSetInvestSietDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nOption = 0;
	m_nOutType_Br = 0;
	m_bPierBase_Sf = 0;
	m_bFootingLower_Sf = 0;
	m_nOutType_Sf = 0;
}

CSeisCvlJpSetInvestSietDlg::~CSeisCvlJpSetInvestSietDlg()
{
	m_pDoc = NULL;
}

void CSeisCvlJpSetInvestSietDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCvlEvalGrupDlg)
	DDX_Radio(pDX, IDC_TM_SIET_ADD_RDO, m_nOption);
	DDX_Control(pDX, IDC_TM_SIET_SIPA_CMB, m_caseName);
	DDX_Control(pDX, IDC_TM_SIET_SIGR_CMB, m_groupName);
	DDX_Radio(pDX, IDC_TM_SIET_OUT_TYPE_BEAR_MINMAX_RDO, m_nOutType_Br);
	DDX_Control(pDX, IDC_TM_SIET_BEAR_CMB, m_concurvity_Br);
	DDX_Check(pDX, IDC_TM_SIET_POS_SF_PIER_BASE_CHK, m_bPierBase_Sf);
	DDX_Check(pDX, IDC_TM_SIET_POS_SF_FOOT_LOW_CHK, m_bFootingLower_Sf);
	DDX_Radio(pDX, IDC_TM_SIET_OUT_TYPE_MEMB_MINMAX_RDO, m_nOutType_Sf);
	DDX_Control(pDX, IDC_TM_SIET_MEMB_CMB, m_concurvity_Mb);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeisCvlJpSetInvestSietDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSeisCvlJpSetInvestSietDlg)
	ON_BN_CLICKED(IDC_TM_SIET_POS_SF_PIER_BASE_CHK, OnChangedMemberChk)
	ON_BN_CLICKED(IDC_TM_SIET_POS_SF_FOOT_LOW_CHK,  OnChangedMemberChk)
	ON_BN_CLICKED(IDC_TM_SIET_OUT_TYPE_BEAR_MINMAX_RDO, OnChangedConcurvityBrRdo)
	ON_BN_CLICKED(IDC_TM_SIET_OUT_TYPE_BEAR_CONC_RDO,	OnChangedConcurvityBrRdo)
	ON_BN_CLICKED(IDC_TM_SIET_OUT_TYPE_MEMB_MINMAX_RDO, OnChangedConcurvityMbRdo)
	ON_BN_CLICKED(IDC_TM_SIET_OUT_TYPE_MEMB_CONC_RDO,	OnChangedConcurvityMbRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSeisCvlJpSetInvestSietDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	InitControl();
	InitCaseNameCtrl();
	InitGroupNameCtrl();
	InitConcurvityBrCtrl();
	InitConcurvityMbCtrl();
	OnChangedConcurvityBrRdo();
	OnChangedConcurvityMbRdo();

	return TRUE;// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisCvlJpSetInvestSietDlg::OnChangedMemberChk()
{
	InitConcurvityMbCtrl();
}

void CSeisCvlJpSetInvestSietDlg::OnChangedConcurvityBrRdo()
{
	CButton* pBtn = NULL;
	pBtn = (CButton*)GetDlgItem(IDC_TM_SIET_OUT_TYPE_BEAR_CONC_RDO);

	GetDlgItem(IDC_TM_SIET_BEAR_CMB)->EnableWindow(pBtn->GetCheck());
}

void CSeisCvlJpSetInvestSietDlg::OnChangedConcurvityMbRdo()
{
	CButton* pBtn = NULL;
	pBtn = (CButton*)GetDlgItem(IDC_TM_SIET_OUT_TYPE_MEMB_CONC_RDO);

	GetDlgItem(IDC_TM_SIET_MEMB_CMB)->EnableWindow(pBtn->GetCheck());
}

void CSeisCvlJpSetInvestSietDlg::Execute()
{
	UpdateData(TRUE);

	if (m_nOption == 0)
		AddOrModData();
	else
		DelData();
}

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetTargetUserDlg diagnostics
// 
void CSeisCvlJpSetInvestSietDlg::InitControl()
{
	CButton* pBtn = NULL;
	//옵션 추가/변경 삭제 라디오 버튼
	pBtn = (CButton*)GetDlgItem(IDC_TM_SIET_ADD_RDO);
	pBtn->SetCheck(1);
	m_nOption = 0;
	//지승결과 산출방법 라디오 버튼
	pBtn = (CButton*)GetDlgItem(IDC_TM_SIET_OUT_TYPE1_RDO);
	pBtn->SetCheck(1);
	m_nOutType_Br = 0;
	//동시성 결과 라디오 버튼
	pBtn = (CButton*)GetDlgItem(IDC_TM_SIET_RESULT_TYPE_SF1_RDO);
	pBtn->SetCheck(1);
	m_nOutType_Sf = 0;
}

void CSeisCvlJpSetInvestSietDlg::InitCaseNameCtrl()
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

void CSeisCvlJpSetInvestSietDlg::InitGroupNameCtrl()
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
		CString str = m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetSigrName(key);
		m_groupName.AddString(str);
	}
	m_groupName.SetCurSel(0);
}

void CSeisCvlJpSetInvestSietDlg::InitConcurvityBrCtrl()
{
	m_concurvity_Br.ResetContent();
	m_concurvity_Br.AddString(COMBO_CONCURVITY_EACH);
	m_concurvity_Br.AddString(COMBO_CONCURVITY_LINE);
	m_concurvity_Br.SetCurSel(0);
}

void CSeisCvlJpSetInvestSietDlg::InitConcurvityMbCtrl()
{
	CButton* pBtn = NULL;
	pBtn = (CButton*)GetDlgItem(IDC_TM_SIET_POS_SF_PIER_BASE_CHK);
	m_bPierBase_Sf = pBtn->GetCheck();

	pBtn = (CButton*)GetDlgItem(IDC_TM_SIET_POS_SF_FOOT_LOW_CHK);
	m_bFootingLower_Sf = pBtn->GetCheck();

	m_concurvity_Mb.ResetContent();

	if (m_bPierBase_Sf == TRUE && m_bFootingLower_Sf == TRUE) {
		m_concurvity_Mb.AddString(COMBO_CONCURVITY_EACH);
		m_concurvity_Mb.AddString(COMBO_CONCURVITY_FOOT_LOW);
	}
	else {
		m_concurvity_Mb.AddString(COMBO_CONCURVITY_EACH);
	}
	
	m_concurvity_Mb.SetCurSel(0);
}

BOOL CSeisCvlJpSetInvestSietDlg::AddOrModData()
{
	T_SIET_D data;
	DlgToData(data);

	//조사 케이스 + 조사 대상 조합으로 기존에 등록되어 있는게 있는지 체크
	BOOL bResult = FALSE;
	T_SIET_K key = GetExistingDataKey(data);
	if (key == 0) { // Add
		key = m_pDoc->m_pAttrCtrl2->GetQSiet()->GetStartNum();
		bResult = m_pDoc->m_pDataCtrl->AddSiet(key, data);
	}
	else { //Mod
		bResult = m_pDoc->m_pDataCtrl->ModifySiet(key, data);
	}

	ASSERT(bResult);
	return bResult;
}

BOOL CSeisCvlJpSetInvestSietDlg::DelData()
{
	T_SIET_D delData;
	DlgToData(delData);

	T_SIET_K key = 0;
	T_SIET_D data;

	POSITION pos = m_pDoc->m_pAttrCtrl2->GetQSiet()->GetStart();
	while (pos)
	{
		m_pDoc->m_pAttrCtrl2->GetQSiet()->GetNext(pos, key, data);
		if (data.kSipa == delData.kSipa && data.kSigrLower == delData.kSigrLower) {
			return m_pDoc->m_pDataCtrl->DelSiet(key);
		}
	}

	AfxMessageBox(_LS(IDS_IDD_TM_SIET_MSG_NOT_EXIST_EQUAL_SIPA_SIGRLOWER));
	return FALSE;
}

void CSeisCvlJpSetInvestSietDlg::DlgToData(OUT T_SIET_D& rData)
{
	CString str;
	//조사 케이스
	m_caseName.GetWindowText(str);
	rData.kSipa = m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKey(str);
	//조사 대상
	m_groupName.GetWindowText(str);
	T_KEY key = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(str);
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Exist(key) == FALSE) {
		ASSERT(FALSE);
		return;
	}
	rData.kSigrLower = key;
	//지승결과 산출방법
	if (m_nOutType_Br == 0) {
		rData.nOutTypeBearing = T_SIET_D::kMinMax_Br;
	}
	else {
		m_concurvity_Br.GetWindowText(str);
		if(str == COMBO_CONCURVITY_EACH)
			rData.nOutTypeBearing = T_SIET_D::kConcurvity_Each_Br;
		else if (str == COMBO_CONCURVITY_LINE)
			rData.nOutTypeBearing = T_SIET_D::kConcurvity_Line_Br;
	}
		
	//단면력 산출 위치
	rData.bPierBase_Sf = m_bPierBase_Sf;
	rData.bFootingLower_Sf = m_bFootingLower_Sf;
	//동시성 결과
	if (m_nOutType_Sf == 0) {
		rData.nOutTypeMember = T_SIET_D::kMinMax_Mb;
	}
	else {
		m_concurvity_Mb.GetWindowText(str);
		if (str == COMBO_CONCURVITY_EACH)
			rData.nOutTypeMember = T_SIET_D::kConcurvity_Each_Mb;
		else if (str == COMBO_CONCURVITY_FOOT_LOW)
			rData.nOutTypeMember = T_SIET_D::kConcurvity_FootingLow;
	}
}

T_SIET_K CSeisCvlJpSetInvestSietDlg::GetExistingDataKey(const T_SIET_D& newData)
{
	CString str;
	//조사 케이스 + 조사 대상 조합으로 기존에 등록되어 있는게 있는지 체크
	T_SIET_K key;
	T_SIET_D data;

	POSITION pos = m_pDoc->m_pAttrCtrl2->GetQSiet()->GetStart();
	while (pos)
	{
		m_pDoc->m_pAttrCtrl2->GetQSiet()->GetNext(pos, key, data);
		if (data.kSipa == newData.kSipa && data.kSigrLower == newData.kSigrLower) {
			return key;
		}
	}

	return 0;
}