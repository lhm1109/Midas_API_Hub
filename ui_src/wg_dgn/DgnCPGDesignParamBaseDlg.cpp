// CDgnCPGDesignParamBaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCPGDesignParamBaseDlg.h"
#include "DgnCPGDesignParamEC2Dlg.h" 
#include "DgnCPGDesignParamLSD12Dlg.h"
#include "DgnCPGDesignParamAASHTO07Dlg.h"
#include "DgnCPGDesignParamSNiPDlg.h"
#include "DgnCPGDesignParamIRC22Dlg.h"
#include "DgnCPGDesignParamCSA14Dlg.h"
#include "DgnCPGDesignParamIRC22_15_Dlg.h"
#include "DgnCPGDesignParamAS5100Dlg.h"
#include "NationalAnnexTool.h"

#include "..\wg_base\TestEnvMgr.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\DBCodeDef.h"

#include "..\wg_base\CheckDialogOpen.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_main\wg_main.h"


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamBaseDlg dialog
CDgnCPGDesignParamBaseDlg::CDgnCPGDesignParamBaseDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnCPGDesignParamBaseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCPGDesignParamBaseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	m_pEC2Dlg = 0;
	m_pLSD12Dlg = 0;
	m_pLSDKDSDlg = 0;
	m_pAASHTO07Dlg = 0;
	m_pAASHTO12Dlg = 0;
	m_pAASHTO16Dlg = 0;
	m_pAASHTO17Dlg = 0;
	m_pAASHTO20Dlg = 0;
	m_pSNiP20503_84 = 0;
	m_pSP13330_2011 = 0;
	m_nCurrPage = 0;
	m_strCurCode = _T("");
	m_pIRC22Dlg = 0;
	m_pCSA14Dlg = 0;
	m_pCSA19Dlg = 0;
	m_pIRC22_15_Dlg = 0;
	m_pAS5100_6_17Dlg = 0;
}
CDgnCPGDesignParamBaseDlg::~CDgnCPGDesignParamBaseDlg()
{
//	_SAFE_DELETE(m_pEC2Dlg);
//	_SAFE_DELETE(m_pLSD12Dlg);
//	_SAFE_DELETE(m_pLSDKDSDlg);
//	_SAFE_DELETE(m_pAASHTO07Dlg);
//	_SAFE_DELETE(m_pAASHTO12Dlg);
//	_SAFE_DELETE(m_pAASHTO16Dlg);
//	_SAFE_DELETE(m_pAASHTO17Dlg);
//	_SAFE_DELETE(m_pAASHTO20Dlg);
//	_SAFE_DELETE(m_pSNiP20503_84);
//	_SAFE_DELETE(m_pSP13330_2011);
//	_SAFE_DELETE(m_pIRC22Dlg);
//	_SAFE_DELETE(m_pCSA14Dlg);
//	_SAFE_DELETE(m_pCSA19Dlg);
//	_SAFE_DELETE(m_pIRC22_15_Dlg);
//	_SAFE_DELETE(m_pAS5100_6_17Dlg);
}

void CDgnCPGDesignParamBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCPGDesignParamBaseDlg)
	DDX_Control(pDX, IDC_DGN_CPG_PARAM_CODE_COMBO, m_DesignCode);
	DDX_Control(pDX, IDC_DGN_CPG_NA_COMBO, m_SubCode);
	DDX_Control(pDX, IDC_CRC_DGN_ANNEX_AASHTO_CMB, m_DesignAnnexAASHTO);
	DDX_Control(pDX, IDC_DGN_CPG_PARAM_CODE_UPDATE_BTN, m_btnUpdateParam);

	// NOTE: the ClassWizard will add DDX and DDV calls here
//}}AFX_DATA_MAP	
}

BEGIN_MESSAGE_MAP(CDgnCPGDesignParamBaseDlg, CDialogMove)
	ON_BN_CLICKED(ID_RC_CLOSE, OnClose)
	ON_EN_CHANGE(IDC_DGN_CPG_PARAM_CODE_COMBO, OnSelChangeDgnCombo)
	ON_CBN_SELCHANGE(IDC_DGN_CPG_PARAM_CODE_COMBO, OnSelChangeDgnCombo)
	ON_CBN_SELCHANGE(IDC_CRC_DGN_ANNEX_AASHTO_CMB, OnSelchangeDgnCtrldataDesignAnnexAashto)
	ON_BN_CLICKED(IDC_DGN_CPG_PARAM_CODE_UPDATE_BTN, OnClickedBtnUpdateParam)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamBaseDlg message handlers

void CDgnCPGDesignParamBaseDlg::OnClose()
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
	//  DestroyWindow();
}

void CDgnCPGDesignParamBaseDlg::OnSelChangeDgnCombo()
{
	m_nCurrPage = m_DesignCode.GetCurSel();
	m_DesignCode.GetLBText(m_nCurrPage, m_strCurCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(m_strCurCode);
#endif
	int nDgnCode = CDBLib::GetCsgCodeNo(m_strCurCode);

	CString strSubCode = _T("");
	m_SubCode.GetLBText(m_SubCode.GetCurSel(), strSubCode);
	int nSubCode = CNationalAnnexTool::ConvertNationalAnnex(strSubCode);

	T_CPGD_D CurrentData;

	CurrentData = m_MainData;
	if (nDgnCode == CurrentData.iDgnCode)
	{
		if (m_pDoc->m_pAttrCtrl->ExistCpgd())
		{
			m_pDoc->m_pAttrCtrl->GetCpgd(m_MainData);
		}
		else
		{
			m_pDoc->m_pAttrCtrl2->GetDgnCpgd(m_MainData);
			CDgnCodeCtrl::GetDefaultCpgdFactor(m_MainData.iDgnCode, m_MainData.nSubCode, m_MainData);
		}
	}
	else
		CDgnCodeCtrl::GetDefaultCpgdFactor(nDgnCode, nSubCode, m_MainData);

	m_DlgPageMgr.ShowTab(m_nCurrPage);

	InitCtrl();

	ResizeDialog();
}

void CDgnCPGDesignParamBaseDlg::OnSelchangeDgnCtrldataDesignAnnexAashto()
{
	// 특별한 동작 없음.
}


int CDgnCPGDesignParamBaseDlg::GetCurSelDgnCode()
{
	CString strCurCode;
	m_DesignCode.GetLBText(m_DesignCode.GetCurSel(), strCurCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCurCode);
#endif
	return CDBLib::GetCsgCodeNo(strCurCode);
}

void CDgnCPGDesignParamBaseDlg::OnClickedBtnUpdateParam()
{
	m_nCurrPage = m_DesignCode.GetCurSel();
	m_DesignCode.GetLBText(m_nCurrPage, m_strCurCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(m_strCurCode);
#endif
	int nDgnCode = CDBLib::GetCsgCodeNo(m_strCurCode);

	CString strSubCode = _T("");
	m_SubCode.GetLBText(m_SubCode.GetCurSel(), strSubCode);
	int nSubCode = CNationalAnnexTool::ConvertNationalAnnex(strSubCode);

	CDgnCodeCtrl::GetDefaultCpgdFactor(nDgnCode, nSubCode, m_MainData);
	switch (nDgnCode)
	{
	case KSCE_LSD15_CSG:
		{
			m_pLSD12Dlg->SetData(&m_MainData);
			m_pLSD12Dlg->Data2Dlg();
		}
		break;
	case KDS_24_14_31_2018_CSG:
		{
			m_pLSDKDSDlg->SetData(&m_MainData);
			m_pLSDKDSDlg->Data2Dlg();
		}
		break;
	case AASHTO_LRFD07_CSG:
		{
			m_pAASHTO07Dlg->SetData(&m_MainData);
			m_pAASHTO07Dlg->Data2Dlg();
		}
		break;
	case AASHTO_LRFD12_CSG:
		{
			m_pAASHTO12Dlg->SetData(&m_MainData);
			m_pAASHTO12Dlg->Data2Dlg();
		}
		break;
	case AASHTO_LRFD16_CSG:
		{
			m_pAASHTO16Dlg->SetData(&m_MainData);
			m_pAASHTO16Dlg->Data2Dlg();
		}
		break;
	case AASHTO_LRFD17_CSG:
		{
			m_pAASHTO17Dlg->SetData(&m_MainData);
			m_pAASHTO17Dlg->Data2Dlg();
		}
		break;
	case AASHTO_LRFD20_CSG:
		{
			m_pAASHTO20Dlg->SetData(&m_MainData);
			m_pAASHTO20Dlg->Data2Dlg();
		}
		break;
	case EUROCODE4_2_04_CSG:
		{
			m_pEC2Dlg->SetData(&m_MainData);
			m_pEC2Dlg->Data2Dlg();
		}
		break;
	case SNiP_20503_84_CSG:
		{
			m_pSNiP20503_84->SetData(&m_MainData);
			m_pSNiP20503_84->Data2Dlg();
		}
		break;
	case SP_35_13330_11_CSG:
		{
			m_pSP13330_2011->SetData(&m_MainData);
			m_pSP13330_2011->Data2Dlg();
		}
		break;
	case IRC22_08_CSG:
		{
			m_pIRC22Dlg->SetData(&m_MainData);
			m_pIRC22Dlg->Data2Dlg();
		}
		break;
	case CSA_S6_14_CSG:
		{
			m_pCSA14Dlg->SetData(&m_MainData);
			m_pCSA14Dlg->Data2Dlg();
		}
		break;
	case CSA_S6_19_CSG:
		{
			m_pCSA19Dlg->SetData(&m_MainData);
			m_pCSA19Dlg->Data2Dlg();
		}
		break;
	case IRC22_15_CSG:
		{
			m_pIRC22_15_Dlg->SetData(&m_MainData);
			m_pIRC22_15_Dlg->SetDefaultData();
			m_pIRC22_15_Dlg->Data2Dlg();
		}
		break;
	case AS5100_6_17_CSG:
		{
			m_pAS5100_6_17Dlg->SetData(&m_MainData);
			m_pAS5100_6_17Dlg->Data2Dlg();
		}
		break;
	default:		
		{
			ASSERT(0);
		}
		break;
	}
}

void CDgnCPGDesignParamBaseDlg::AddCpgCodeParamDlg(CString &strCode)
{
	if (strCode.CompareNoCase(_T("EN 1994-2")) == 0)
	{
		m_DlgPageMgr.AddTab(m_pEC2Dlg, _T(""), CDgnCPGDesignParamEC2Dlg::IDD, TRUE);
	}
    else if ( strCode.CompareNoCase(_T("AASHTO-LRFD20")) == 0 )
    {
        m_DlgPageMgr.AddTab(m_pAASHTO20Dlg, _T(""), CDgnCPGDesignParamAASHTO07Dlg::IDD, TRUE);
    }
	else if (strCode.CompareNoCase(_T("AASHTO-LRFD17")) == 0)
	{
		m_DlgPageMgr.AddTab(m_pAASHTO17Dlg, _T(""), CDgnCPGDesignParamAASHTO07Dlg::IDD, TRUE);
	}
	else if (strCode.CompareNoCase(_T("AASHTO-LRFD16")) == 0)
	{
		m_DlgPageMgr.AddTab(m_pAASHTO16Dlg, _T(""), CDgnCPGDesignParamAASHTO07Dlg::IDD, TRUE);
	}
	else if (strCode.CompareNoCase(_T("AASHTO-LRFD12")) == 0)
	{
		m_DlgPageMgr.AddTab(m_pAASHTO12Dlg, _T(""), CDgnCPGDesignParamAASHTO07Dlg::IDD, TRUE);
	}
	else if (strCode.CompareNoCase(_T("AASHTO-LRFD07")) == 0)
	{
		m_DlgPageMgr.AddTab(m_pAASHTO07Dlg, _T(""), CDgnCPGDesignParamAASHTO07Dlg::IDD, TRUE);
	}
	else if (strCode.CompareNoCase(_T("KSCE-LSD15")) == 0)
	{
		m_DlgPageMgr.AddTab(m_pLSD12Dlg, _T(""), CDgnCPGDesignParamLSD12Dlg::IDD, TRUE);
	}
	else if (strCode.CompareNoCase(CSGCODE_KDS_24_14_31_2018) == 0)
	{
		m_DlgPageMgr.AddTab(m_pLSDKDSDlg, _T(""), CDgnCPGDesignParamLSD12Dlg::IDD, TRUE);
	}
	else if (strCode.CompareNoCase(CSGCODE_SNIP_2_05_03_84) == 0)
	{
		m_DlgPageMgr.AddTab(m_pSNiP20503_84, _T(""), CDgnCPGDesignParamSNiPDlg::IDD, TRUE);
	}
	else if (strCode.CompareNoCase(_T("IRC:22-2008")) == 0)
	{
		m_DlgPageMgr.AddTab(m_pIRC22Dlg, _T(""), CDgnCPGDesignParamIRC22Dlg::IDD, TRUE);
	}
	else if (strCode.CompareNoCase(CSGCODE_SP_35_13330_2011) == 0)
	{
		m_DlgPageMgr.AddTab(m_pSP13330_2011, _T(""), CDgnCPGDesignParamSNiPDlg::IDD, TRUE);
	}
	else if (strCode.CompareNoCase(CSGCODE_CSA_S6_14) == 0)
	{
		m_DlgPageMgr.AddTab(m_pCSA14Dlg, _T(""), CDgnCPGDesignParamCSA14Dlg::IDD, TRUE);
	}
	else if (strCode.CompareNoCase(CSGCODE_CSA_S6_19) == 0)
	{
		m_DlgPageMgr.AddTab(m_pCSA19Dlg, _T(""), CDgnCPGDesignParamCSA14Dlg::IDD, TRUE);
	}
	else if (strCode.CompareNoCase(_T("IRC:22-2015")) == 0)
	{
		m_DlgPageMgr.AddTab(m_pIRC22_15_Dlg, _T(""), CDgnCPGDesignParamIRC22_15_Dlg::IDD, TRUE);
	}
	else if ( strCode.CompareNoCase(CSGCODE_AS5100_6_2017) == 0 )
	{
		m_DlgPageMgr.AddTab(m_pAS5100_6_17Dlg, _T(""), CDgnCPGDesignParamAS5100Dlg::IDD, TRUE);
	}
	else
		ASSERT(0);
}


void CDgnCPGDesignParamBaseDlg::OnOK()
{
	// TODO: Add your control notification handler code here
	Dlg2Data();

	if (!m_pDoc->m_pDataCtrl->AddCpgd(m_MainData))
		return;

	CDialogMove::OnOK();
}

BOOL CDgnCPGDesignParamBaseDlg::OnInitDialog()
{
	MInitCombo initCombo;

	CDialogMove::OnInitDialog();

	CCheckDialogOpen::SetSoftwareRendering();

	m_DesignCode.ResetContent();

	CString strCsgCodeNa;
	CStringArray arCsgCodeNa;
	CDBLib::GetCsgCodeNameList(arCsgCodeNa);

	for (int i = 0; i < arCsgCodeNa.GetSize(); ++i)
	{
		strCsgCodeNa = arCsgCodeNa[i];

#if defined(_RUS)
		CInitCtrl::ConvertEngRusDesignCode(strCsgCodeNa);
#endif
		m_DesignCode.AddString(strCsgCodeNa);
	}
	CDlgUtil::CobxAdjustListBoxWidth(m_DesignCode);

	auto vString = CNationalAnnexTool::GetCpgNationalAnnex();
	for (int i = 0; i < vString.size(); ++i)
	{
		m_SubCode.SetItemData(m_SubCode.AddString(vString[i]), CNationalAnnexTool::ConvertNationalAnnex(vString[i]));
	}
	//
	m_DesignAnnexAASHTO.ResetContent();
	vString = CNationalAnnexTool::GetRcAASHTOAmendment();
	for (int i = 0; i < vString.size(); ++i)
	{
		CDlgUtil::CobxAddItem(m_DesignAnnexAASHTO, vString[i], CNationalAnnexTool::ConvertAASHTOAmendment(vString[i]));
	}
	CDlgUtil::CobxSetCurSelItemData(m_DesignAnnexAASHTO, 0/*Amend_Recommended*/);

	m_MainData.Initialize();
	if (m_pDoc->m_pAttrCtrl->ExistCpgd())
	{
		m_pDoc->m_pAttrCtrl->GetCpgd(m_MainData);
	}
	else
	{
		T_PREFERENCE& Pref = m_pDoc->GetPreference();
		CString strCsgCode = Pref.DgnCode.PGCode;
		m_MainData.iDgnCode = CDBLib::GetCsgCodeNo(strCsgCode);
		CDgnCodeCtrl::GetDefaultCpgdFactor(m_MainData.iDgnCode, m_MainData.nSubCode, m_MainData);
	}

	int Index = 0;
	CString strCodeName = CDBLib::GetCsgCodeName(m_MainData.iDgnCode);

#if defined(_RUS)
	CInitCtrl::ConvertEngRusDesignCode(strCodeName);
#endif

	if (strCodeName != _T(""))	Index = m_DesignCode.FindStringExact(-1, strCodeName);
	m_DesignCode.SetCurSel(Index);
	m_nCurrPage = Index;
	m_DesignCode.GetLBText(m_nCurrPage, m_strCurCode);

#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(m_strCurCode);
#endif

	if (CDBLib::GetCsgCodeNo(m_strCurCode) == AASHTO_LRFD17_CSG)
	{
		CDlgUtil::CobxSetCurSelItemData(m_DesignAnnexAASHTO, m_MainData.nSubCode);
	}
	else
	{
		m_SubCode.SetCurSel(m_SubCode.FindStringExact(-1, CNationalAnnexTool::ConvertNationalAnnex(m_MainData.nSubCode)));
	}


	m_pEC2Dlg = new CDgnCPGDesignParamEC2Dlg(this);
	m_pEC2Dlg->SetData(&m_MainData);
	m_pLSD12Dlg = new CDgnCPGDesignParamLSD12Dlg(this);
	m_pLSD12Dlg->SetData(&m_MainData);
	m_pLSDKDSDlg = new CDgnCPGDesignParamLSD12Dlg(this);
	m_pLSDKDSDlg->SetData(&m_MainData);
	m_pAASHTO07Dlg = new CDgnCPGDesignParamAASHTO07Dlg(this);
	m_pAASHTO07Dlg->SetData(&m_MainData);
	m_pAASHTO12Dlg = new CDgnCPGDesignParamAASHTO07Dlg(this);
	m_pAASHTO12Dlg->SetData(&m_MainData);
	m_pAASHTO16Dlg = new CDgnCPGDesignParamAASHTO07Dlg(this);
	m_pAASHTO16Dlg->SetData(&m_MainData);
	m_pAASHTO17Dlg = new CDgnCPGDesignParamAASHTO07Dlg(this);
	m_pAASHTO17Dlg->SetData(&m_MainData);
	m_pAASHTO20Dlg = new CDgnCPGDesignParamAASHTO07Dlg(this);
	m_pAASHTO20Dlg->SetData(&m_MainData);
	m_pSNiP20503_84 = new CDgnCPGDesignParamSNiPDlg(this);
	m_pSNiP20503_84->SetData(&m_MainData);
	m_pSP13330_2011 = new CDgnCPGDesignParamSNiPDlg(this);
	m_pSP13330_2011->SetData(&m_MainData);
	m_pIRC22Dlg = new CDgnCPGDesignParamIRC22Dlg(this);
	m_pIRC22Dlg->SetData(&m_MainData);
	m_pCSA14Dlg = new CDgnCPGDesignParamCSA14Dlg(this);
	m_pCSA14Dlg->SetData(&m_MainData);
	m_pCSA19Dlg = new CDgnCPGDesignParamCSA14Dlg(this);
	m_pCSA19Dlg->SetData(&m_MainData);
	m_pIRC22_15_Dlg = new CDgnCPGDesignParamIRC22_15_Dlg(this);
	m_pIRC22_15_Dlg->SetData(&m_MainData);
	m_pAS5100_6_17Dlg = new CDgnCPGDesignParamAS5100Dlg(this);
	m_pAS5100_6_17Dlg->SetData(&m_MainData);

	m_DlgPageMgr.SetPlaceHolder(this, IDC_BASE_PLACE_HOLDER);

	for (int i = 0; i < arCsgCodeNa.GetSize(); ++i)
	{
		AddCpgCodeParamDlg(arCsgCodeNa[i]);
	}

	m_DlgPageMgr.ShowTab(m_nCurrPage, TRUE);

	InitCtrl();

	ResizeDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnCPGDesignParamBaseDlg::Data2Dlg()
{
	m_nCurrPage = m_DesignCode.GetCurSel();
	m_DesignCode.GetLBText(m_nCurrPage, m_strCurCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(m_strCurCode);
#endif
	int nDgnCode = CDBLib::GetCsgCodeNo(m_strCurCode);

	switch (nDgnCode)
	{
	case KSCE_LSD15_CSG: 
		m_pLSD12Dlg->Data2Dlg(); 
		break;
	case KDS_24_14_31_2018_CSG:
		m_pLSDKDSDlg->Data2Dlg();
		break;
	case AASHTO_LRFD07_CSG:
		m_pAASHTO07Dlg->Data2Dlg();
		break;
	case AASHTO_LRFD12_CSG:
		m_pAASHTO12Dlg->Data2Dlg();
		break;
	case AASHTO_LRFD16_CSG:
		m_pAASHTO16Dlg->Data2Dlg();
		break;
	case AASHTO_LRFD17_CSG:
		m_pAASHTO17Dlg->Data2Dlg();
		break;
	case AASHTO_LRFD20_CSG:
		m_pAASHTO20Dlg->Data2Dlg();
		break;
	case EUROCODE4_2_04_CSG:
		m_pEC2Dlg->Data2Dlg();
		break;
	case SNiP_20503_84_CSG:
		m_pSNiP20503_84->Data2Dlg();
		break;
	case SP_35_13330_11_CSG:
		m_pSP13330_2011->Data2Dlg();
		break;
	case IRC22_08_CSG:
		m_pIRC22Dlg->Data2Dlg();
		break;
	case CSA_S6_14_CSG:
		m_pCSA14Dlg->Data2Dlg();
		break;
	case CSA_S6_19_CSG:
		m_pCSA19Dlg->Data2Dlg();
		break;
	case IRC22_15_CSG:
		m_pIRC22_15_Dlg->Data2Dlg();
		break;
	case AS5100_6_17_CSG:
		m_pAS5100_6_17Dlg->Data2Dlg();
		break;
	default:
		ASSERT(0);
		break;
	}	
}

BOOL CDgnCPGDesignParamBaseDlg::Dlg2Data()
{
	m_nCurrPage = m_DesignCode.GetCurSel();
	m_DesignCode.GetLBText(m_nCurrPage, m_strCurCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(m_strCurCode);
#endif
	m_MainData.iDgnCode = CDBLib::GetCsgCodeNo(m_strCurCode);

	if (CDBLib::GetCsgCodeNo(m_strCurCode) == AASHTO_LRFD17_CSG)
	{
		m_MainData.nSubCode = CDlgUtil::CobxGetCurSelItemData(m_DesignAnnexAASHTO, m_DesignAnnexAASHTO.GetCurSel());
	}
	else
	{
		CString strSubCode = _T("");
		m_SubCode.GetLBText(m_SubCode.GetCurSel(), strSubCode);
		m_MainData.nSubCode = CNationalAnnexTool::ConvertNationalAnnex(strSubCode);
	}

	switch (m_MainData.iDgnCode)
	{
	case KSCE_LSD15_CSG:
		m_pLSD12Dlg->Dlg2Data();
		break;
	case KDS_24_14_31_2018_CSG:
		m_pLSDKDSDlg->Dlg2Data();
		break;
	case AASHTO_LRFD07_CSG:
		m_pAASHTO07Dlg->Dlg2Data();
		break;
	case AASHTO_LRFD12_CSG:
		m_pAASHTO12Dlg->Dlg2Data();
		break;
	case AASHTO_LRFD16_CSG:
		m_pAASHTO16Dlg->Dlg2Data();
		break;
	case AASHTO_LRFD17_CSG:
		m_pAASHTO17Dlg->Dlg2Data();
		break;
	case AASHTO_LRFD20_CSG:
		m_pAASHTO20Dlg->Dlg2Data();
		break;
	case EUROCODE4_2_04_CSG:
		m_pEC2Dlg->Dlg2Data();
		break;
	case SNiP_20503_84_CSG:
		m_pSNiP20503_84->Dlg2Data();
		break;
	case SP_35_13330_11_CSG:
		m_pSP13330_2011->Dlg2Data();
		break;
	case IRC22_08_CSG:
		m_pIRC22Dlg->Dlg2Data();
		break;
	case CSA_S6_14_CSG:
		m_pCSA14Dlg->Dlg2Data();
		break;
	case CSA_S6_19_CSG:
		m_pCSA19Dlg->Dlg2Data();
		break;
	case IRC22_15_CSG:
		m_pIRC22_15_Dlg->Dlg2Data();
		break;
	case AS5100_6_17_CSG:
		m_pAS5100_6_17Dlg->Dlg2Data();
		break;
	default:
		ASSERT(0);
		break;
	}
	return TRUE;
}

void CDgnCPGDesignParamBaseDlg::InitCtrl()
{
	if (CDBLib::GetCsgCodeNo(m_strCurCode) == EUROCODE4_2_04_CSG)
	{
		GetDlgItem(IDC_DGN_CPG_NA_STATIC)->ShowWindow(TRUE);
		GetDlgItem(IDC_DGN_CPG_NA_COMBO)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_NA_STATIC)->ShowWindow(FALSE);
		GetDlgItem(IDC_DGN_CPG_NA_COMBO)->ShowWindow(FALSE);
	}

	CArray<UINT, UINT> aAnnexAASHTOCtrls;
	aAnnexAASHTOCtrls.RemoveAll();
	aAnnexAASHTOCtrls.Add(IDC_CRC_DGN_ANNEX_AASHTO_STC);
	aAnnexAASHTOCtrls.Add(IDC_CRC_DGN_ANNEX_AASHTO_CMB);
	CDlgUtil::CtrlShowHide(this, aAnnexAASHTOCtrls, FALSE);

	BOOL bShowAmendent = FALSE;
	if(CDBLib::GetCsgCodeNo(m_strCurCode) == AASHTO_LRFD17_CSG) bShowAmendent = TRUE;

	if (bShowAmendent)
	{
		CDlgUtil::CtrlShowHide(this, aAnnexAASHTOCtrls, bShowAmendent);

		CRect rTarget, rSource;
		int nDistX, nDistY;
		GetDlgItem(IDC_DGN_CPG_NA_COMBO)->GetWindowRect(rSource);
		GetDlgItem(IDC_CRC_DGN_ANNEX_AASHTO_CMB)->GetWindowRect(rTarget);
		nDistX = rSource.left - rTarget.left;
		nDistY = rSource.top - rTarget.top;
		CDlgUtil::CtrlMoveDistXY(this, aAnnexAASHTOCtrls, nDistX, nDistY);
	}
}

void CDgnCPGDesignParamBaseDlg::ResizeDialog()
{
	//test
	CRect rRef;
	CRect rToMove;
	int nDistY = 0;

	CRect rectHolder, rectDlgChild;
 	m_DlgPageMgr.GetCurTabRec(m_nCurrPage, rectHolder, rectDlgChild);

	// Holder 변경 
	//GetDlgItem(IDC_BASE_PLACE_HOLDER)->GetWindowRect(rToMove);
	//rToMove.bottom = rToMove.top + (rectDlgChild.bottom- rectDlgChild.top);
	//ScreenToClient(rToMove);
	//GetDlgItem(IDC_BASE_PLACE_HOLDER)->MoveWindow(rToMove);

	rectHolder.bottom = rectHolder.top + (rectDlgChild.bottom - rectDlgChild.top);
	GetDlgItem(IDC_BASE_PLACE_HOLDER)->MoveWindow(rectHolder);

	// OK, CanCel 이동
	CArray<UINT, UINT> aCtrl;
	aCtrl.Add(IDOK);
	aCtrl.Add(IDCANCEL);

	GetDlgItem(IDC_BASE_PLACE_HOLDER)->GetWindowRect(rRef);
	GetDlgItem(IDCANCEL)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);

	//Dlg Resize
	UINT nLastCtrlID = IDCANCEL;
	CWnd* pWndLast = GetDlgItem(nLastCtrlID);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);

	MoveWindow(r);

	//윈도우 사이즈 조절후 하위에 있는 컨트롤들이 제대로 그려지지 않아서 아래와 같이 명시적 갱신	
	Invalidate();
	UpdateWindow();

	GetDlgItem(IDOK)->Invalidate();
	GetDlgItem(IDOK)->UpdateWindow();
	GetDlgItem(IDCANCEL)->Invalidate();
	GetDlgItem(IDCANCEL)->UpdateWindow();

	CBCGPWindowDC dc(this);
	DrawBottomButtonArea(&dc, TRUE);
}

