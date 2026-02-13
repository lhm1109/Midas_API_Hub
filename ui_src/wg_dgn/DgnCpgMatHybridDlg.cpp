// DgnCpgMatHybridDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCpgMatHybridDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_db\wg_db_EditData.h"

#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCpgMatHybridDlg dialog


CDgnCpgMatHybridDlg::CDgnCpgMatHybridDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnCpgMatHybridDlg::IDD, pParent)
{
	m_pData = NULL;
	m_strCode.Empty();
	m_strMatlName.Empty();
	//{{AFX_DATA_INIT(CDgnCpgMatHybridDlg)
	//}}AFX_DATA_INIT
}


void CDgnCpgMatHybridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCpgMatHybridDlg)
	DDX_Control(pDX, IDC_DGN_TOP_FUUNIT,  m_FuUnit_top);
	DDX_Control(pDX, IDC_DGN_TOP_FY1UNIT, m_Fy1Unit_top);
	DDX_Control(pDX, IDC_DGN_TOP_FY2UNIT, m_Fy2Unit_top);
	DDX_Control(pDX, IDC_DGN_TOP_FY3UNIT, m_Fy3Unit_top);
	DDX_Control(pDX, IDC_DGN_TOP_FY4UNIT, m_Fy4Unit_top);
	DDX_Control(pDX, IDC_DGN_TOP_FY5UNIT, m_Fy5Unit_top);  
	DDX_Control(pDX, IDC_DGN_TOP_FY6UNIT, m_Fy6Unit_top);  
	DDX_Control(pDX, IDC_DGN_TOP_ESUNIT, m_EsUnit_top);
	DDX_Control(pDX, IDC_DGN_TOP_STEELMAT, m_sMatName_top);
	DDX_Text(pDX, IDC_DGN_TOP_STEELMAT2, m_sMatName2_top);
	DDX_Text(pDX, IDC_DGN_TOP_ES, m_Es_top);
	DDX_Text(pDX, IDC_DGN_TOP_FU, m_Fu_top);
	DDX_Text(pDX, IDC_DGN_TOP_FY1, m_Fy1_top);
	DDX_Text(pDX, IDC_DGN_TOP_FY2, m_Fy2_top);
	DDX_Text(pDX, IDC_DGN_TOP_FY3, m_Fy3_top);
	DDX_Text(pDX, IDC_DGN_TOP_FY4, m_Fy4_top);
	DDX_Text(pDX, IDC_DGN_TOP_FY5, m_Fy5_top); 
	DDX_Text(pDX, IDC_DGN_TOP_FY6, m_Fy6_top); 

	DDX_Control(pDX, IDC_DGN_BOT_FUUNIT,  m_FuUnit_bot);
	DDX_Control(pDX, IDC_DGN_BOT_FY1UNIT, m_Fy1Unit_bot);
	DDX_Control(pDX, IDC_DGN_BOT_FY2UNIT, m_Fy2Unit_bot);
	DDX_Control(pDX, IDC_DGN_BOT_FY3UNIT, m_Fy3Unit_bot);
	DDX_Control(pDX, IDC_DGN_BOT_FY4UNIT, m_Fy4Unit_bot);
	DDX_Control(pDX, IDC_DGN_BOT_FY5UNIT, m_Fy5Unit_bot);  
	DDX_Control(pDX, IDC_DGN_BOT_FY6UNIT, m_Fy6Unit_bot);  
	DDX_Control(pDX, IDC_DGN_BOT_ESUNIT, m_EsUnit_bot);
	DDX_Control(pDX, IDC_DGN_BOT_STEELMAT, m_sMatName_bot);
	DDX_Text(pDX, IDC_DGN_BOT_STEELMAT2, m_sMatName2_bot);
	DDX_Text(pDX, IDC_DGN_BOT_ES, m_Es_bot);
	DDX_Text(pDX, IDC_DGN_BOT_FU, m_Fu_bot);
	DDX_Text(pDX, IDC_DGN_BOT_FY1, m_Fy1_bot);
	DDX_Text(pDX, IDC_DGN_BOT_FY2, m_Fy2_bot);
	DDX_Text(pDX, IDC_DGN_BOT_FY3, m_Fy3_bot);
	DDX_Text(pDX, IDC_DGN_BOT_FY4, m_Fy4_bot);
	DDX_Text(pDX, IDC_DGN_BOT_FY5, m_Fy5_bot); 
	DDX_Text(pDX, IDC_DGN_BOT_FY6, m_Fy6_bot); 

	DDX_Control(pDX, IDC_DGN_WEB_FUUNIT,  m_FuUnit_web);
	DDX_Control(pDX, IDC_DGN_WEB_FY1UNIT, m_Fy1Unit_web);
	DDX_Control(pDX, IDC_DGN_WEB_FY2UNIT, m_Fy2Unit_web);
	DDX_Control(pDX, IDC_DGN_WEB_FY3UNIT, m_Fy3Unit_web);
	DDX_Control(pDX, IDC_DGN_WEB_FY4UNIT, m_Fy4Unit_web);
	DDX_Control(pDX, IDC_DGN_WEB_FY5UNIT, m_Fy5Unit_web);  
	DDX_Control(pDX, IDC_DGN_WEB_FY6UNIT, m_Fy6Unit_web);  
	DDX_Control(pDX, IDC_DGN_WEB_ESUNIT, m_EsUnit_web);
	DDX_Control(pDX, IDC_DGN_WEB_STEELMAT, m_sMatName_web);
	DDX_Text(pDX, IDC_DGN_WEB_STEELMAT2, m_sMatName2_web);
	DDX_Text(pDX, IDC_DGN_WEB_ES, m_Es_web);
	DDX_Text(pDX, IDC_DGN_WEB_FU, m_Fu_web);
	DDX_Text(pDX, IDC_DGN_WEB_FY1, m_Fy1_web);
	DDX_Text(pDX, IDC_DGN_WEB_FY2, m_Fy2_web);
	DDX_Text(pDX, IDC_DGN_WEB_FY3, m_Fy3_web);
	DDX_Text(pDX, IDC_DGN_WEB_FY4, m_Fy4_web);
	DDX_Text(pDX, IDC_DGN_WEB_FY5, m_Fy5_web); 
	DDX_Text(pDX, IDC_DGN_WEB_FY6, m_Fy6_web); 
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnCpgMatHybridDlg, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnCpgMatHybridDlg)
	ON_CBN_SELCHANGE(IDC_DGN_TOP_STEELMAT, OnSelchangeTopGrade)
	ON_CBN_SELCHANGE(IDC_DGN_BOT_STEELMAT, OnSelchangeBotGrade)
	ON_CBN_SELCHANGE(IDC_DGN_WEB_STEELMAT, OnSelchangeWebGrade)
	ON_BN_CLICKED(IDC_DGN_SRCMAT_MODIFY, OnOK)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCpgMatHybridDlg message handlers

BOOL CDgnCpgMatHybridDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	// TODO: Add extra initialization here
	Initial_MaterialCombo();
	Initial_Unit();
	UpdateData(FALSE);

	return TRUE;
}

void CDgnCpgMatHybridDlg::Initial_Unit()
{
	m_EsUnit_top.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_FuUnit_top.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy1Unit_top.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy2Unit_top.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy3Unit_top.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy4Unit_top.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy5Unit_top.SetUnitType(D_UNITSYS_BASE_STRESS);  
	m_Fy6Unit_top.SetUnitType(D_UNITSYS_BASE_STRESS);  

	m_EsUnit_bot.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_FuUnit_bot.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy1Unit_bot.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy2Unit_bot.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy3Unit_bot.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy4Unit_bot.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy5Unit_bot.SetUnitType(D_UNITSYS_BASE_STRESS);  
	m_Fy6Unit_bot.SetUnitType(D_UNITSYS_BASE_STRESS);  

	m_EsUnit_web.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_FuUnit_web.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy1Unit_web.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy2Unit_web.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy3Unit_web.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy4Unit_web.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_Fy5Unit_web.SetUnitType(D_UNITSYS_BASE_STRESS);  
	m_Fy6Unit_web.SetUnitType(D_UNITSYS_BASE_STRESS);  
}

void CDgnCpgMatHybridDlg::SetData(const int &nMatlKey, T_MATD_ALL *pData, const CString &strCode, const CString &strMatl)
{
	m_nMatlKey = nMatlKey;
	m_pData    = pData;
	m_strCode  = strCode;
	m_strMatlName = strMatl;
}

void CDgnCpgMatHybridDlg::Data2Dlg()
{

}

BOOL CDgnCpgMatHybridDlg::Dlg2Data()
{
	UpdateData(TRUE);

	CString strSName = _T("");
	if (m_strCode == _T("None"))
	{
		m_pData->DesignTopFlange.strMatlName = m_sMatName2_top;
		m_pData->DesignBotFlange.strMatlName = m_sMatName2_bot;
		m_pData->DesignWeb.strMatlName       = m_sMatName2_web;
	}
	else
	{
		//Top
		int Index = m_sMatName_top.GetCurSel();
		if(Index != -1)
		{
			m_sMatName_top.GetLBText(Index, strSName);
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(m_strCode, strSName);
#endif
			m_pData->DesignTopFlange.strMatlName = strSName;
		}
		else
		{
			ASSERT(0);
			return FALSE;
		}

		//Bot
		Index = m_sMatName_bot.GetCurSel();
		if(Index != -1)
		{
			m_sMatName_bot.GetLBText(Index, strSName);
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(m_strCode, strSName);
#endif
			m_pData->DesignBotFlange.strMatlName = strSName;
		}
		else
		{
			ASSERT(0);
			return FALSE;
		}

		//Web
		Index = m_sMatName_web.GetCurSel();
		if(Index != -1)
		{
			m_sMatName_web.GetLBText(Index, strSName);
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(m_strCode, strSName);
#endif
			m_pData->DesignWeb.strMatlName = strSName;
		}
		else
		{
			ASSERT(0);
			return FALSE;
		}
	}

	//Top Flange
	m_pData->AnalysisTopFlange.Elast = m_Es_top;
	m_pData->DesignTopFlange.S_Fu = m_Fu_top;
	m_pData->DesignTopFlange.S_Fy1 = m_Fy1_top;
	m_pData->DesignTopFlange.S_Fy2 = m_Fy2_top;
	m_pData->DesignTopFlange.S_Fy3 = m_Fy3_top;
	m_pData->DesignTopFlange.S_Fy4 = m_Fy4_top;
	m_pData->DesignTopFlange.S_Fy5 = m_Fy5_top;
	m_pData->DesignTopFlange.S_Fy6 = m_Fy6_top;

	//Bot Flange
	m_pData->AnalysisBotFlange.Elast = m_Es_bot;
	m_pData->DesignBotFlange.S_Fu = m_Fu_bot;
	m_pData->DesignBotFlange.S_Fy1 = m_Fy1_bot;
	m_pData->DesignBotFlange.S_Fy2 = m_Fy2_bot;
	m_pData->DesignBotFlange.S_Fy3 = m_Fy3_bot;
	m_pData->DesignBotFlange.S_Fy4 = m_Fy4_bot;
	m_pData->DesignBotFlange.S_Fy5 = m_Fy5_bot;
	m_pData->DesignBotFlange.S_Fy6 = m_Fy6_bot;

	//WEb
	m_pData->AnalysisWeb.Elast = m_Es_web;
	m_pData->DesignWeb.S_Fu  = m_Fu_web;
	m_pData->DesignWeb.S_Fy1 = m_Fy1_web;
	m_pData->DesignWeb.S_Fy2 = m_Fy2_web;
	m_pData->DesignWeb.S_Fy3 = m_Fy3_web;
	m_pData->DesignWeb.S_Fy4 = m_Fy4_web;
	m_pData->DesignWeb.S_Fy5 = m_Fy5_web;
	m_pData->DesignWeb.S_Fy6 = m_Fy6_web;

	return TRUE;
}

void CDgnCpgMatHybridDlg::Initial_MaterialCombo()
{
	Initial_SteelMatNameTop(m_strCode, m_pData->DesignTopFlange.strMatlName);
	Initial_SteelMatNameBot(m_strCode, m_pData->DesignBotFlange.strMatlName);
	Initial_SteelMatNameWeb(m_strCode, m_pData->DesignWeb.strMatlName);
}

void CDgnCpgMatHybridDlg::SetSteelStrengthTop(CString strCode,CString strName)
{
	T_MATL_STEEL sData;
	BOOL bCheck = m_pDoc->m_pMatlDB->GetSteelData(strCode,strName,sData);
	if(bCheck)
	{
		m_Es_top  = sData.Elast;
		m_Fu_top  = sData.S_Fu;
		m_Fy1_top = sData.S_Fy1;
		m_Fy2_top = sData.S_Fy2;
		m_Fy3_top = sData.S_Fy3;
		m_Fy4_top = sData.S_Fy4;
		m_Fy5_top = sData.S_Fy5;  // add by Seungjun (`06.05.30).
		m_Fy6_top = sData.S_Fy6;  // add by Seungjun (`06.05.30).
	}
	else
	{
		T_MATD_D mData; mData.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetMatlDesign(m_nMatlKey, mData);

		if(bCheck)
		{
			m_Es_top  = mData.Data1.AnalysisTopFlange.Elast;
			m_Fu_top  = mData.Data1.DesignTopFlange.S_Fu;
			m_Fy1_top = mData.Data1.DesignTopFlange.S_Fy1;
			m_Fy2_top = mData.Data1.DesignTopFlange.S_Fy2;
			m_Fy3_top = mData.Data1.DesignTopFlange.S_Fy3;
			m_Fy4_top = mData.Data1.DesignTopFlange.S_Fy4;
			m_Fy5_top = mData.Data1.DesignTopFlange.S_Fy5;  // add by Seungjun (`06.05.30).
			m_Fy6_top = mData.Data1.DesignTopFlange.S_Fy6;  // add by Seungjun (`06.05.30).
		}
		else
		{
			m_Es_top  = 0.0;
			m_Fu_top  = 0.0;
			m_Fy1_top = 0.0;
			m_Fy2_top = 0.0;
			m_Fy3_top = 0.0;
			m_Fy4_top = 0.0;
			m_Fy5_top = 0.0;  // add by Seungjun (`06.05.30).
			m_Fy6_top = 0.0;  // add by Seungjun (`06.05.30).
		}
	}
}

void CDgnCpgMatHybridDlg::SetSteelStrengthBot(CString strCode,CString strName)
{
	T_MATL_STEEL sData;
	BOOL bCheck = m_pDoc->m_pMatlDB->GetSteelData(strCode,strName,sData);
	if(bCheck)
	{
		m_Es_bot  = sData.Elast;
		m_Fu_bot  = sData.S_Fu;
		m_Fy1_bot = sData.S_Fy1;
		m_Fy2_bot = sData.S_Fy2;
		m_Fy3_bot = sData.S_Fy3;
		m_Fy4_bot = sData.S_Fy4;
		m_Fy5_bot = sData.S_Fy5;  // add by Seungjun (`06.05.30).
		m_Fy6_bot = sData.S_Fy6;  // add by Seungjun (`06.05.30).
	}
	else
	{
		T_MATD_D mData; mData.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetMatlDesign(m_nMatlKey, mData);

		if(bCheck)
		{
			m_Es_bot  = mData.Data1.AnalysisBotFlange.Elast;
			m_Fu_bot  = mData.Data1.DesignBotFlange.S_Fu;
			m_Fy1_bot = mData.Data1.DesignBotFlange.S_Fy1;
			m_Fy2_bot = mData.Data1.DesignBotFlange.S_Fy2;
			m_Fy3_bot = mData.Data1.DesignBotFlange.S_Fy3;
			m_Fy4_bot = mData.Data1.DesignBotFlange.S_Fy4;
			m_Fy5_bot = mData.Data1.DesignBotFlange.S_Fy5;  // add by Seungjun (`06.05.30).
			m_Fy6_bot = mData.Data1.DesignBotFlange.S_Fy6;  // add by Seungjun (`06.05.30).
		}
		else
		{
			m_Es_bot  = 0.0;
			m_Fu_bot  = 0.0;
			m_Fy1_bot = 0.0;
			m_Fy2_bot = 0.0;
			m_Fy3_bot = 0.0;
			m_Fy4_bot = 0.0;
			m_Fy5_bot = 0.0;  // add by Seungjun (`06.05.30).
			m_Fy6_bot = 0.0;  // add by Seungjun (`06.05.30).
		}
	}
}

void CDgnCpgMatHybridDlg::SetSteelStrengthWeb(CString strCode,CString strName)
{
	T_MATL_STEEL sData;
	BOOL bCheck = m_pDoc->m_pMatlDB->GetSteelData(strCode,strName,sData);
	if(bCheck)
	{
		m_Es_web  = sData.Elast;
		m_Fu_web  = sData.S_Fu;
		m_Fy1_web = sData.S_Fy1;
		m_Fy2_web = sData.S_Fy2;
		m_Fy3_web = sData.S_Fy3;
		m_Fy4_web = sData.S_Fy4;
		m_Fy5_web = sData.S_Fy5;  // add by Seungjun (`06.05.30).
		m_Fy6_web = sData.S_Fy6;  // add by Seungjun (`06.05.30).
	}
	else
	{
		T_MATD_D mData; mData.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetMatlDesign(m_nMatlKey, mData);

		if(bCheck)
		{
			m_Es_web  = mData.Data1.AnalysisWeb.Elast;
			m_Fu_web  = mData.Data1.DesignWeb.S_Fu;
			m_Fy1_web = mData.Data1.DesignWeb.S_Fy1;
			m_Fy2_web = mData.Data1.DesignWeb.S_Fy2;
			m_Fy3_web = mData.Data1.DesignWeb.S_Fy3;
			m_Fy4_web = mData.Data1.DesignWeb.S_Fy4;
			m_Fy5_web = mData.Data1.DesignWeb.S_Fy5;  // add by Seungjun (`06.05.30).
			m_Fy6_web = mData.Data1.DesignWeb.S_Fy6;  // add by Seungjun (`06.05.30).
		}
		else
		{
			m_Es_web  = 0.0;
			m_Fu_web  = 0.0;
			m_Fy1_web = 0.0;
			m_Fy2_web = 0.0;
			m_Fy3_web = 0.0;
			m_Fy4_web = 0.0;
			m_Fy5_web = 0.0;  // add by Seungjun (`06.05.30).
			m_Fy6_web = 0.0;  // add by Seungjun (`06.05.30).
		}
	}
}

void CDgnCpgMatHybridDlg::Initial_SteelMatNameTop(CString sCodeName,CString sCodeMatlName)
{
	if (sCodeMatlName.IsEmpty() == TRUE) { sCodeMatlName = m_strMatlName; }
	if(m_sMatName_top.GetCount() > 0)	m_sMatName_top.ResetContent();

	CArray <CString, CString&> NameList;
	m_pDoc->m_pMatlDB->GetSteelNameList(sCodeName, NameList);
	for (int i = 0; i < NameList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(sCodeName, NameList[i]);
#endif
		m_sMatName_top.AddString(NameList.GetAt(i));
	}

	CString MatlName;
	MatlName = sCodeMatlName;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(sCodeName, MatlName);
#endif
	int Index = m_sMatName_top.FindStringExact(-1, MatlName);
	if(Index==-1)	{ m_sMatName2_top = sCodeMatlName; }
	else					m_sMatName_top.SetCurSel(Index);

	ShowSteelMatNameTop(m_strCode);
	SetSteelStrengthTop(sCodeName,sCodeMatlName);
}

void CDgnCpgMatHybridDlg::Initial_SteelMatNameBot(CString sCodeName,CString sCodeMatlName)
{
	if (sCodeMatlName.IsEmpty() == TRUE) { sCodeMatlName = m_strMatlName; }
	if(m_sMatName_bot.GetCount() > 0)	m_sMatName_bot.ResetContent();

	CArray <CString, CString&> NameList;
	m_pDoc->m_pMatlDB->GetSteelNameList(sCodeName, NameList);
	for (int i = 0; i < NameList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(sCodeName, NameList[i]);
#endif
		m_sMatName_bot.AddString(NameList.GetAt(i));
	}

	CString MatlName;
	MatlName = sCodeMatlName;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(sCodeName, MatlName);
#endif

	int Index = m_sMatName_bot.FindStringExact(-1, MatlName);
	if(Index==-1)	{ m_sMatName2_bot = sCodeMatlName; }
	else					m_sMatName_bot.SetCurSel(Index);

	ShowSteelMatNameBot(m_strCode);
	SetSteelStrengthBot(sCodeName,sCodeMatlName);
}

void CDgnCpgMatHybridDlg::Initial_SteelMatNameWeb(CString sCodeName,CString sCodeMatlName)
{
	if (sCodeMatlName.IsEmpty() == TRUE) { sCodeMatlName = m_strMatlName; }
	if(m_sMatName_web.GetCount() > 0)	m_sMatName_web.ResetContent();

	CArray <CString, CString&> NameList;
	m_pDoc->m_pMatlDB->GetSteelNameList(sCodeName, NameList);
	for (int i = 0; i < NameList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(sCodeName, NameList[i]);
#endif
		m_sMatName_web.AddString(NameList.GetAt(i));
	}

	CString MatlName;
	MatlName = sCodeMatlName;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(sCodeName, MatlName);
#endif

	int Index = m_sMatName_web.FindStringExact(-1, MatlName);
	if(Index==-1)	{ m_sMatName2_web = sCodeMatlName; }
	else					m_sMatName_web.SetCurSel(Index);

	ShowSteelMatNameWeb(m_strCode);
	SetSteelStrengthWeb(sCodeName,sCodeMatlName);
}

void CDgnCpgMatHybridDlg::OnSelchangeTopGrade() 
{
	UpdateData(TRUE);
	int Index = m_sMatName_top.GetCurSel();
	CString strName=_T("");
	m_sMatName_top.GetLBText(Index,strName);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(m_strCode, strName);
#endif
	SetSteelStrengthTop(m_strCode, strName);
	UpdateData(FALSE);
}

void CDgnCpgMatHybridDlg::OnSelchangeBotGrade() 
{
	UpdateData(TRUE);
	int Index = m_sMatName_bot.GetCurSel();
	CString strName=_T("");
	m_sMatName_bot.GetLBText(Index,strName);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(m_strCode, strName);
#endif

	SetSteelStrengthBot(m_strCode, strName);
	UpdateData(FALSE);
}

void CDgnCpgMatHybridDlg::OnSelchangeWebGrade() 
{
	UpdateData(TRUE);
	int Index = m_sMatName_web.GetCurSel();
	CString strName=_T("");
	m_sMatName_web.GetLBText(Index,strName);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(m_strCode, strName);
#endif

	SetSteelStrengthWeb(m_strCode, strName);
	UpdateData(FALSE);
}


void CDgnCpgMatHybridDlg::ShowSteelMatNameTop(CString &strSCode)
{
	if (strSCode.IsEmpty()==TRUE) return;
	if(strSCode==_T("") || strSCode==_T("None"))
	{
		GetDlgItem(IDC_DGN_TOP_STEELMAT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_TOP_STEELMAT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TOPMAT_SNAME)->SetWindowText(_LS(IDS_DGN_TEXT_NAME));

		GetDlgItem(IDC_DGN_TOP_ES)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_TOP_FU)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_TOP_FY1)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_TOP_FY2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_TOP_FY3)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_TOP_FY4)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_TOP_FY5)->EnableWindow(TRUE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_TOP_FY6)->EnableWindow(TRUE);  // add by Seungjun (`06.05.30).
	}
	else
	{
		GetDlgItem(IDC_DGN_TOP_STEELMAT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_STEELMAT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TOPMAT_SNAME)->SetWindowText(_LS(IDS_DGN_TEXT_DB_NAME));

		GetDlgItem(IDC_DGN_TOP_ES)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_TOP_FU)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_TOP_FY1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_TOP_FY2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_TOP_FY3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_TOP_FY4)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_TOP_FY5)->EnableWindow(FALSE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_TOP_FY6)->EnableWindow(FALSE);  // add by Seungjun (`06.05.30).
	}
	// Change by ZINU.('02.9.17).
	int iChkKind = GetChkKind(strSCode);
	if(iChkKind==1)
	{
		GetDlgItem(IDC_DGN_TOP_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_TOP_FY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_TOP_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_TOP_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_TOP_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).

		GetDlgItem(IDC_STATIC_TOPMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TOPMAT_FY2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TOPMAT_FY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TOPMAT_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TOPMAT_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_TOPMAT_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_TOPMAT_FY1)->SetWindowText(_T("Fy    "));
		GetDlgItem(IDC_DGN_TOP_FY2UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_TOP_FY3UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_TOP_FY4UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_TOP_FY5UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_TOP_FY6UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
	}
	else if(iChkKind==2) 
	{
		GetDlgItem(IDC_DGN_TOP_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_TOP_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_TOP_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_TOP_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).

		GetDlgItem(IDC_STATIC_TOPMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TOPMAT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TOPMAT_FY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TOPMAT_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TOPMAT_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_TOPMAT_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_TOPMAT_FY1)->SetWindowText(_T("Fy1   "));
		GetDlgItem(IDC_DGN_TOP_FY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY3UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_TOP_FY4UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_TOP_FY5UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_TOP_FY6UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
	}
	else if(iChkKind==3)
	{
		GetDlgItem(IDC_DGN_TOP_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_TOP_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_TOP_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).

		GetDlgItem(IDC_STATIC_TOPMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TOPMAT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TOPMAT_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TOPMAT_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TOPMAT_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_TOPMAT_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_TOPMAT_FY1)->SetWindowText(_T("Fy1   "));
		GetDlgItem(IDC_DGN_TOP_FY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY3UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY4UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_TOP_FY5UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_TOP_FY6UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
	}
	else if(iChkKind==4)
	{
		GetDlgItem(IDC_DGN_TOP_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_TOP_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).

		GetDlgItem(IDC_STATIC_TOPMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TOPMAT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TOPMAT_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TOPMAT_FY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TOPMAT_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_TOPMAT_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_TOPMAT_FY1)->SetWindowText(_T("Fy1   "));
		GetDlgItem(IDC_DGN_TOP_FY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY3UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY4UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY5UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_TOP_FY6UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
	}
	else if(iChkKind==5)  // add by Seungjun (`06.05.30).
	{
		GetDlgItem(IDC_DGN_TOP_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY6)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_TOPMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TOPMAT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TOPMAT_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TOPMAT_FY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TOPMAT_FY5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TOPMAT_FY6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TOPMAT_FY1)->SetWindowText(_T("Fy1   "));
		GetDlgItem(IDC_DGN_TOP_FY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY3UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY4UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY5UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TOP_FY6UNIT)->ShowWindow(SW_SHOW);
	}
}

void CDgnCpgMatHybridDlg::ShowSteelMatNameBot(CString &strSCode)
{
	if (strSCode.IsEmpty()==TRUE) return;
	if(strSCode==_T("") || strSCode==_T("None"))
	{
		GetDlgItem(IDC_DGN_BOT_STEELMAT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BOT_STEELMAT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BOTMAT_SNAME)->SetWindowText(_LS(IDS_DGN_TEXT_NAME));

		GetDlgItem(IDC_DGN_BOT_ES)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_BOT_FU)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_BOT_FY1)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_BOT_FY2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_BOT_FY3)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_BOT_FY4)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_BOT_FY5)->EnableWindow(TRUE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_BOT_FY6)->EnableWindow(TRUE);  // add by Seungjun (`06.05.30).
	}
	else
	{
		GetDlgItem(IDC_DGN_BOT_STEELMAT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_STEELMAT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_BOTMAT_SNAME)->SetWindowText(_LS(IDS_DGN_TEXT_DB_NAME));

		GetDlgItem(IDC_DGN_BOT_ES)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_BOT_FU)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_BOT_FY1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_BOT_FY2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_BOT_FY3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_BOT_FY4)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_BOT_FY5)->EnableWindow(FALSE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_BOT_FY6)->EnableWindow(FALSE);  // add by Seungjun (`06.05.30).
	}
	// Change by ZINU.('02.9.17).
	int iChkKind = GetChkKind(strSCode);
	if(iChkKind==1)
	{
		GetDlgItem(IDC_DGN_BOT_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BOT_FY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BOT_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BOT_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_BOT_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).

		GetDlgItem(IDC_STATIC_BOTMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BOTMAT_FY2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_BOTMAT_FY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_BOTMAT_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_BOTMAT_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_BOTMAT_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_BOTMAT_FY1)->SetWindowText(_T("Fy    "));
		GetDlgItem(IDC_DGN_BOT_FY2UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BOT_FY3UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BOT_FY4UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BOT_FY5UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_BOT_FY6UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
	}
	else if(iChkKind==2) 
	{
		GetDlgItem(IDC_DGN_BOT_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BOT_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BOT_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_BOT_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).

		GetDlgItem(IDC_STATIC_BOTMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BOTMAT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BOTMAT_FY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_BOTMAT_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_BOTMAT_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_BOTMAT_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_BOTMAT_FY1)->SetWindowText(_T("Fy1   "));
		GetDlgItem(IDC_DGN_BOT_FY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY3UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BOT_FY4UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BOT_FY5UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_BOT_FY6UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
	}
	else if(iChkKind==3)
	{
		GetDlgItem(IDC_DGN_BOT_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BOT_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_BOT_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).

		GetDlgItem(IDC_STATIC_BOTMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BOTMAT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BOTMAT_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BOTMAT_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_BOTMAT_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_BOTMAT_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_BOTMAT_FY1)->SetWindowText(_T("Fy1   "));
		GetDlgItem(IDC_DGN_BOT_FY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY3UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY4UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BOT_FY5UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_BOT_FY6UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
	}
	else if(iChkKind==4)
	{
		GetDlgItem(IDC_DGN_BOT_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_BOT_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).

		GetDlgItem(IDC_STATIC_BOTMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BOTMAT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BOTMAT_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BOTMAT_FY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BOTMAT_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_BOTMAT_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_BOTMAT_FY1)->SetWindowText(_T("Fy1   "));
		GetDlgItem(IDC_DGN_BOT_FY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY3UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY4UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY5UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_BOT_FY6UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
	}
	else if(iChkKind==5)  // add by Seungjun (`06.05.30).
	{
		GetDlgItem(IDC_DGN_BOT_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY6)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_BOTMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BOTMAT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BOTMAT_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BOTMAT_FY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BOTMAT_FY5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BOTMAT_FY6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BOTMAT_FY1)->SetWindowText(_T("Fy1   "));
		GetDlgItem(IDC_DGN_BOT_FY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY3UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY4UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY5UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOT_FY6UNIT)->ShowWindow(SW_SHOW);
	}
}

void CDgnCpgMatHybridDlg::ShowSteelMatNameWeb(CString &strSCode)
{
	if (strSCode.IsEmpty()==TRUE) return;
	if(strSCode==_T("") || strSCode==_T("None"))
	{
		GetDlgItem(IDC_DGN_WEB_STEELMAT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WEB_STEELMAT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_WEBMAT_SNAME)->SetWindowText(_LS(IDS_DGN_TEXT_NAME));

		GetDlgItem(IDC_DGN_WEB_ES)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WEB_FU)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WEB_FY1)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WEB_FY2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WEB_FY3)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WEB_FY4)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WEB_FY5)->EnableWindow(TRUE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_WEB_FY6)->EnableWindow(TRUE);  // add by Seungjun (`06.05.30).
	}
	else
	{
		GetDlgItem(IDC_DGN_WEB_STEELMAT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_STEELMAT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_WEBMAT_SNAME)->SetWindowText(_LS(IDS_DGN_TEXT_DB_NAME));

		GetDlgItem(IDC_DGN_WEB_ES)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WEB_FU)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WEB_FY1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WEB_FY2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WEB_FY3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WEB_FY4)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WEB_FY5)->EnableWindow(FALSE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_WEB_FY6)->EnableWindow(FALSE);  // add by Seungjun (`06.05.30).
	}
	// Change by ZINU.('02.9.17).
	int iChkKind = GetChkKind(strSCode);
	if(iChkKind==1)
	{
		GetDlgItem(IDC_DGN_WEB_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WEB_FY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WEB_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WEB_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_WEB_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).

		GetDlgItem(IDC_STATIC_WEBMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_WEBMAT_FY2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_WEBMAT_FY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_WEBMAT_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_WEBMAT_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_WEBMAT_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_WEBMAT_FY1)->SetWindowText(_T("Fy    "));
		GetDlgItem(IDC_DGN_WEB_FY2UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WEB_FY3UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WEB_FY4UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WEB_FY5UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_WEB_FY6UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
	}
	else if(iChkKind==2) 
	{
		GetDlgItem(IDC_DGN_WEB_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WEB_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WEB_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_WEB_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).

		GetDlgItem(IDC_STATIC_WEBMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_WEBMAT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_WEBMAT_FY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_WEBMAT_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_WEBMAT_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_WEBMAT_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_WEBMAT_FY1)->SetWindowText(_T("Fy1   "));
		GetDlgItem(IDC_DGN_WEB_FY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY3UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WEB_FY4UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WEB_FY5UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_WEB_FY6UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
	}
	else if(iChkKind==3)
	{
		GetDlgItem(IDC_DGN_WEB_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WEB_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_WEB_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).

		GetDlgItem(IDC_STATIC_WEBMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_WEBMAT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_WEBMAT_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_WEBMAT_FY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_WEBMAT_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_WEBMAT_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_WEBMAT_FY1)->SetWindowText(_T("Fy1   "));
		GetDlgItem(IDC_DGN_WEB_FY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY3UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY4UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WEB_FY5UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_WEB_FY6UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
	}
	else if(iChkKind==4)
	{
		GetDlgItem(IDC_DGN_WEB_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_WEB_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).

		GetDlgItem(IDC_STATIC_WEBMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_WEBMAT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_WEBMAT_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_WEBMAT_FY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_WEBMAT_FY5)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_WEBMAT_FY6)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_STATIC_WEBMAT_FY1)->SetWindowText(_T("Fy1   "));
		GetDlgItem(IDC_DGN_WEB_FY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY3UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY4UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY5UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
		GetDlgItem(IDC_DGN_WEB_FY6UNIT)->ShowWindow(SW_HIDE);  // add by Seungjun (`06.05.30).
	}
	else if(iChkKind==5)  // add by Seungjun (`06.05.30).
	{
		GetDlgItem(IDC_DGN_WEB_ES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY6)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_WEBMAT_FY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_WEBMAT_FY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_WEBMAT_FY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_WEBMAT_FY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_WEBMAT_FY5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_WEBMAT_FY6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_WEBMAT_FY1)->SetWindowText(_T("Fy1   "));
		GetDlgItem(IDC_DGN_WEB_FY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY3UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY4UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY5UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WEB_FY6UNIT)->ShowWindow(SW_SHOW);
	}
}

int CDgnCpgMatHybridDlg::GetChkKind(CString strMatlCode)
{
	CDgnDataCtrl DataCtrl;
	return DataCtrl.GetChkKindStlMatl(strMatlCode);
}

void CDgnCpgMatHybridDlg::OnOK() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data();		
	CDialogMove::OnOK();		
}

