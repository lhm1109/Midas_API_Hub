// CMLoadCombDefDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadCombDefDlg.h"
#include "CMLoadCombDefPage.h"
#include "CMLoadCombParaPage.h"
#include "CMLoadCombSpecPage.h"
#include "LoadCombDefCtrl.h"
#include "CMLoadCombDlg.h"
#include "..\wg_base\EnFileEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_DBLib.h"

#include "..\wg_dbLock\LockUtil.h"
#include "..\wg_dbLock\LockMgr.h"   // for CLockMgr
#include "..\wg_dbLock\WebLockUtil.h"


#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"
#include "..\wg_base\ServiceProvider.h"
#include "..\wg_cmd2\CMD2_ServiceDef.h"
#include "..\wg_dbLock\LockOption.h"
#include "ExportFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombDefDlg dialog
CCMLoadCombDefDlg::CCMLoadCombDefDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMLoadCombDefDlg::IDD, pParent)
{
	m_pDoc = NULL;
	m_nActivePage = 0 ;
	m_nOption = 0;
	m_pData = ILoadCombDefineMgr::Instance()->Para();
}

CCMLoadCombDefDlg::~CCMLoadCombDefDlg()
{
	
}

void CCMLoadCombDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadCombDefDlg)
	DDX_Control(pDX, IDC_CMD_TAB_CTRL     , m_wndTab);
	DDX_Control(pDX,IDC_CMD_LOADCOMB_DEF_COMB,m_cmbLcomType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMLoadCombDefDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMLoadCombDefDlg)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_DEF_ADD_BTN, OnAdd)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_DEF_REPLACE_BTN, OnReplace)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_DEF_INPUT_BTN, OnCmdBtnImport)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_DEF_OUTPUT_BTN, OnCmdBtnExport)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnDlgClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCMLoadCombDefDlg::SetActivePage(int SelectPage)
{
	m_wndTab.ShowTab(SelectPage);
	m_nActivePage = SelectPage;
}

BOOL CCMLoadCombDefDlg::Data2Dlg()
{
	if (m_wndTab.GetCurSel() == 0)
		m_pDefine->Data2Dlg();
	else if (m_wndTab.GetCurSel() == 1)
		m_pPara->Data2Dlg();
	else
		m_pSpec->Data2Dlg();
	return TRUE;
}

BOOL CCMLoadCombDefDlg::Dlg2Data()
{
	if (m_wndTab.GetCurSel() == 0)
		m_pDefine->Dlg2Data();
	else if (m_wndTab.GetCurSel() == 1)
		m_pPara->Dlg2Data();
	else
		m_pSpec->Dlg2Data();
	return TRUE;
	//m_pSpec->GetSafeHwnd()
}

BOOL CCMLoadCombDefDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	m_pDefine = new CCMLoadCombDefPage(m_pData, this);
	m_pPara = new CCMLoadCombParaPage(m_pData, this);
	m_pSpec = new CCMLoadCombSpecPage(m_pData, this);

	m_wndTab.AddTab(m_pDefine , _LS(IDS_WG_CMD__ADDD__Load_Comb_Define ), CCMLoadCombDefPage::IDD , TRUE);
	m_wndTab.AddTab(m_pPara  , _LS(IDS_WG_CMD__ADDD__Load_Comb_ParaGeneral  ), CCMLoadCombParaPage::IDD  , TRUE);
	m_wndTab.AddTab(m_pSpec , _LS(IDS_WG_CMD__ADDD__Load_Comb_SpecialSet ), CCMLoadCombSpecPage::IDD , TRUE);

	m_wndTab.ShowTab(m_nActivePage);

	InitComboType();
	UpdateData(FALSE);
	return TRUE;
}

void CCMLoadCombDefDlg::InitComboType()
{	
	m_cmbLcomType.ResetContent();	

	CStringArray LcomType;
	LcomType.Add(_LS(IDS_WG_CMD__ADDD__General));
	LcomType.Add(_LS(IDS_WG_CMD__ADDD__Steel_Design));
	LcomType.Add(_LS(IDS_WG_CMD__ADDD__Concrete_Design));
	LcomType.Add(_LS(IDS_WG_CMD__ADDD__SRC_Design));
	LcomType.Add(_LS(IDS_WG_CMD__ADDD__CFSteel_Design));
	LcomType.Add(_LS(IDS_WG_CMD__ADDD__Aluminum_Design));

	for (int i=0; i<6; i++)
	{
		int iIndex = m_cmbLcomType.AddString(LcomType[i]);
		m_cmbLcomType.SetItemData(iIndex,i);
	}

	m_cmbLcomType.SetCurSel(0);
	m_cmbLcomType.SetCheck(0);
}


BOOL CCMLoadCombDefDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CCMDlgBase::OnNotify(wParam, lParam, pResult);
}

void CCMLoadCombDefDlg::OnAdd()
{
	if (!Dlg2Data())
		return;
	CString strCode = m_pData->m_strCode;
	m_nOption = 0;
	
	CLoadCombDefCtrl LoadCombCtrl;
	for (int i=0; i<6; i++)
	{	
		if (m_cmbLcomType.GetCheck(i))
			LoadCombCtrl.m_bChkLCType[i] = TRUE;
		else
			LoadCombCtrl.m_bChkLCType[i] = FALSE;
	}
	LoadCombCtrl.Set_DefaultLoadComb(m_nOption, m_pData);
	OnCmdCombDlg();
}

void CCMLoadCombDefDlg::OnReplace()
{
	if (!Dlg2Data())
		return;
	m_nOption = 1;
	CLoadCombDefCtrl LoadCombCtrl;
	for (int i=0; i<6; i++)
	{
		if (m_cmbLcomType.GetCheck(i))
			LoadCombCtrl.m_bChkLCType[i] = TRUE;
		else
			LoadCombCtrl.m_bChkLCType[i] = FALSE;
	}
	LoadCombCtrl.Set_DefaultLoadComb(m_nOption, m_pData);
	OnCmdCombDlg();
}

void CCMLoadCombDefDlg::OnCmdCombDlg()
{
	CreateOrActivateDlg((CDBDoc*)CDBDoc::GetDocPoint(), CCMLoadCombDlg::IDD, NULL);
}

void CCMLoadCombDefDlg::OnCmdBtnImport()
{
	CString csExt = _T(".lct");
	CString csFilter = _T("LCT Files(*.lct)|*.lct|All File(*.*)|*.*||");

	CString csFileName;
	FILE* fp;
	CFileDialog fd(TRUE, csExt, NULL, OFN_FILEMUSTEXIST | OFN_LONGNAMES, csFilter, NULL);

	if(fd.DoModal()==IDOK) 
		csFileName = fd.GetPathName();
	else 
		return;
	fp = _tfopen(fd.GetPathName(), _T("r"));

	if (!ReadData(fp))
	{
		m_pData->initial();
		m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_CMD__ADDD__an_Error_occurred_while_reading_d));
		return; 
	}

	Data2Dlg();
}

BOOL CCMLoadCombDefDlg::WriteData(CString strFile)
{
	CFileStatus FStatus;
	FILE* fout;

	if (strFile == "") return FALSE;

	CFile::GetStatus(strFile, FStatus);
	if ((fout = _tfopen(strFile, _T("w"))) == NULL) return FALSE;

	CString strBland = _T(",");
	CString strAddTxt = _T("Load Combinations Define");
	_ftprintf(fout,_T("%s\n"), strAddTxt.GetString());

	_ftprintf(fout, _T("Case List:\n"));
	CArray<T_STLD_K, T_STLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	T_STLD_D StldD;
	for (int i = 0; i < arKeyList.GetCount(); i++)
	{
		StldD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetStld(arKeyList[i], StldD))
			ASSERT(FALSE);
		_ftprintf(fout, _T("%d, %s\n"), arKeyList[i], StldD.LoadCaseName.GetString());
	}
	_ftprintf(fout, _T("\n"));

	CString strSplcCaselst = _T("SPLC Case List:");
	_ftprintf(fout, _T("%s\n"), (LPCTSTR)strSplcCaselst);
	CArray<T_SPLC_K, T_SPLC_K> arSplcList;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(arSplcList);
	T_SPLC_D SplcD;
	for (int i = 0; i < arSplcList.GetCount(); i++)
	{
		SplcD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetSplc(arSplcList[i], SplcD))
			ASSERT(FALSE);
		_ftprintf(fout, _T("%d, %s\n"), arSplcList[i], SplcD.LoadCaseName.GetString());
	}
	_ftprintf(fout, _T("\n"));

	_ftprintf(fout, _T("Reference Code:\n"));
	_ftprintf(fout, _T("%s\n"), m_pData->m_strCode.GetString());
	_ftprintf(fout, _T("Initial imperfection load case considered check or not:\n"));
	_ftprintf(fout, _T("%s\n"), m_pData->m_bImperfection ? _T("YES") : _T("NO"));

	CString strLoadCase, strFactor;
	_ftprintf(fout, _T("Partial Coefficient:\n"));
	for (int i=0; i<m_pData->m_item.m_aType.GetCount(); i++)
	{
		strLoadCase += m_pData->m_item.m_aType[i] + strBland;
	}
	_ftprintf(fout, _T("%s\n"),strLoadCase.GetString());

	for (int i = 0; i < m_pData->m_item.m_aFactor.GetCount(); i++)
	{
		strFactor = _T("");
		CArrayEx<double, double> aFactor;
		aFactor = m_pData->m_item.m_aFactor[i];
		for (int k = 0; k < m_pData->m_item.m_aFactor[i].GetCount(); k++)
		{
			CString str;		
			str.Format(_T("%.2f,"), aFactor[k]);
			strFactor += str;
		}
		strFactor += _T("\n");
		_ftprintf(fout, _T("%s\n"),strFactor.GetString());
	}

	_ftprintf(fout, _T("Construction stage case: \n"));
	CString strStageCase=_T("");
	switch(m_pData->m_nStage)
	{
	case 0 : strStageCase = _T("ST"); break;
	case 1 : strStageCase = _T("CS"); break;
	case 2 : strStageCase = _T("ST+CS"); break;
	default: ASSERT(0);
		break;
	}
	_ftprintf(fout, strStageCase);
	_ftprintf(fout, _T("Consider the orthogonal result or not:\n"));
	int nCount = (int)m_pData->m_mapBiEarthquake.size();
	if (m_pData->m_bBiEarthquake && nCount >0)
	{
		CString strOrth;
		_ftprintf(fout, _T("%s, %d\n"), _T("YES"), nCount);
		for (auto itr = m_pData->m_mapBiEarthquake.begin(); itr!= m_pData->m_mapBiEarthquake.end(); itr++)
		{
			LOAD_CASE_K keyX, keyY;
			keyX = itr->first;
			keyY = itr->second;
			strOrth.Format(_T("%d, %d, %d\n"), keyX.nLoadCaseType, keyX.Key, keyY.Key);
			_ftprintf(fout, _T("%s\n"), (LPCTSTR)strOrth);
		}
	}
	else
		_ftprintf(fout, _T("%s\n"), _T("NO"));

	_ftprintf(fout, _T("Consider Wind & Seis or not:\n"));
	nCount = (int)m_pData->m_mapBC2C.size();
	if (nCount > 0)
	{
		CString strWindASeis;
		_ftprintf(fout, _T("%s, %d\n"), _T("YES"), (int)nCount);
		for (auto itr = m_pData->m_mapBC2C.begin(); itr != m_pData->m_mapBC2C.end(); itr++)
		{
			LOAD_CASE_K keyW, keyS;
			keyW = itr->first;
			keyS = itr->second;
			strWindASeis.Format(_T("%d, %d, %d\n"), keyS.nLoadCaseType, keyW.Key, keyS.Key);
			_ftprintf(fout, _T("%s\n"), (LPCTSTR)strWindASeis);
		}
	}
	else
		_ftprintf(fout, _T("%s\n"), _T("NO"));

	_ftprintf(fout, _T("Consider PartialFactors of special case or not:\n"));
	nCount = (int)m_pData->m_mapSpecialFactor.size();
	if (nCount > 0)
	{
		CString strPart;
		_ftprintf(fout, _T("%s, %d\n"), _T("YES"), nCount);
		for (auto itr : m_pData->m_mapSpecialFactor)
		{
			LOAD_CASE_K key = itr.first;
			strPart.Format(_T("%d, %d, %.2f\n"), key.Key, key.nLoadCaseType, itr.second);
			_ftprintf(fout, _T("%s\n"), (LPCTSTR)strPart);
		}
	}
	else
	{
		_ftprintf(fout, _T("%s\n"), _T("NO"));
	}

	_ftprintf(fout, _T("CombCoefficient:\n"));
	_ftprintf(fout, _T("%.2f, %.2f, %.2f\n"), m_pData->m_dCombFactor[0], m_pData->m_dCombFactor[1], m_pData->m_dCombFactor[2]);
	nCount = (int)m_pData->m_mapCombFactor.size();
	if (nCount > 0)
	{
		CString strPart;
		for (auto itr : m_pData->m_mapCombFactor)
		{
			LOAD_CASE_K key = itr.first;
			strPart.Format(_T("%d, %.2f\n"), key.Key, itr.second);
		}
	}
	_ftprintf(fout, _T("Adjustment factor of live load:\n"));
	_ftprintf(fout, _T("%g\n"), m_pData->m_dLLAdjustCombFactor);

	_ftprintf(fout, _T("Gravity load coefficient:\n"));
	_ftprintf(fout, _T("%g\n"), m_pData->m_dLLGravityFactor);
	nCount = (int)m_pData->m_mapGravityFactor.size();
	if (nCount > 0)
	{
		CString strGarv;
		for (auto itr : m_pData->m_mapGravityFactor)
		{
			LOAD_CASE_K key = itr.first;
			strGarv.Format(_T("%d, %.2f\n"), key.Key, itr.second);
			_ftprintf(fout, _T("%s\n"), (LPCTSTR)strGarv);
		}
	}
	else
		_ftprintf(fout, _T("NO\n"));

	_ftprintf(fout, _T("Live load type:\n"));
	_ftprintf(fout, _T("%d\n"), m_pData->m_nSpecitlArrange);
	_ftprintf(fout, _T("AdverseCase:\n"));
	nCount = m_pData->m_aLLUnfavorable.GetCount();
	if (nCount > 0)
	{
		for (int i = 0; i < nCount; i++)
		{
			CString strAver;
			LOAD_CASE_K_ARR arCase = m_pData->m_aLLUnfavorable[i];
			for (int k = 0; k < arCase.GetCount(); k++)
			{
				LOAD_CASE_K key = arCase[k];
				CString strCase;
				strCase.Format(_T("%d"), key.Key);
				strAver += strCase + strBland;
			}
			strAver.Format(_T("%d, %s"), i + 1, strAver);
			_ftprintf(fout, _T("%s*\n"), (LPCTSTR)strAver);
		}
	}
	else
	{
		_ftprintf(fout, _T("NO\n"));
	}

	_ftprintf(fout, _T("Mutually exclusive load:\n"));
	nCount = (int)m_pData->m_mapExclusion.size();
	if (nCount>0)
	{
		for (auto itr : m_pData->m_mapExclusion)
		{
			CString strExclusion;
			LOAD_CASE_K key = itr.first;
			LOAD_CASE_K_ARR arCase = itr.second;
			for (int i=0; i<arCase.GetCount(); i++)
			{
				CString strCase;
				strCase.Format(_T("%d"), arCase[i].Key);
				strExclusion += strCase+ strBland;
			}
			strExclusion.Format(_T("%d, %s"), key.Key, strExclusion);
			_ftprintf(fout, _T("%s*\n"), (LPCTSTR)strExclusion);
		}
	}
	else
	{
		_ftprintf(fout, _T("NO\n"));
	}

	_ftprintf(fout, _T("End\n"));
	if(fout!=NULL) fclose(fout);  

	return TRUE;
}

BOOL CCMLoadCombDefDlg::ReadData(FILE *fp)
{
	m_pData->initial();
	char		yytext[120];

	std::string csTitle = "Load Combinations Define";
	std::string csLoadCase = "Case List:";
	std::string csSplcCase = "SPLC Case List:";
	std::string csCodeName = "Reference Code:";
	std::string csPartCoef = "Partial Coefficient:";
	std::string csStagCase = "Construction stage case: ";
	std::string csOrthRlt  = "Consider the orthogonal result or not:";
	std::string csCombWE = "CombW&S:";
	std::string csPartFact = "Consider PartialFactors of special case or not:";
	std::string csCombCoef = "CombCoefficient:";
	std::string csLiveLoad = "Adjustment factor of live load:";
	std::string csGravLoad = "Gravity load coefficient:";
	std::string csAverCase = "AdverseCase";
	std::string csMutuCase = "Mutually exclusive load:";
	std::string csEnd = "End";
	std::string csText;

	lex(fp, yytext);	//Load Combinations Define
	csText = yytext;
	
	CArray<T_STLD_K, T_STLD_K> arStldKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKeyList);
	CArray<T_SPLC_K, T_SPLC_K> arSplcKeyList;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(arSplcKeyList);
	if (arStldKeyList.GetCount()<1 && arSplcKeyList.GetCount()<1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_));
		return FALSE;
	}

// load list
	lex(fp, yytext);	// CASE LIST
	std::string strLoadCaseName, strKey;
	int nkey;
	lex(fp, yytext);	//list No.1 line, key
	while (strcmp(yytext, csSplcCase.c_str()) !=0)
	{
		strKey = yytext;
		nkey = atoi(strKey.c_str());
		lex(fp, yytext);	//","
		lex(fp, yytext);
		strLoadCaseName = yytext;
		T_STLD_D StldD;
		BOOL bSame = TRUE;
		for (int i=0; i<arStldKeyList.GetCount(); i++)
		{
			StldD.Initialize();
			if (nkey == arStldKeyList[i])
			{
				m_pDoc->m_pAttrCtrl->GetStld(arStldKeyList[i], StldD);
				CStringA saLoadCaseName(StldD.LoadCaseName);
				if (strcmp(strLoadCaseName.c_str(), saLoadCaseName.GetString()) != 0)
				{
					bSame = TRUE;
					break;
				}
			}
			else
				bSame = FALSE;
		}
		//lex(fp, yytext);	// \n
		if (!bSame)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_));
			return FALSE;
		}
		lex(fp, yytext);	//the next line
	}	
//SPLC Case List:
	std::string strSplcCaseName, strSplcKey;
	int nSplckey;
	lex(fp, yytext);	//list No.1 line, key
	while (strcmp(yytext, csCodeName.c_str()) != 0)
	{
		strSplcKey = yytext;
		nSplckey = atoi(strSplcKey.c_str());
		lex(fp, yytext);	//","
		lex(fp, yytext);
		strSplcCaseName = yytext;
		T_SPLC_D SplcD;
		BOOL bSame = TRUE;
		for (int i = 0; i < arSplcKeyList.GetCount(); i++)
		{
			SplcD.Initialize();
			if (nSplckey == arSplcKeyList[i])
			{
				m_pDoc->m_pAttrCtrl->GetSplc(arSplcKeyList[i], SplcD);
				CStringA saLoadCaseName(SplcD.LoadCaseName);
				if (strcmp(strSplcCaseName.c_str(), saLoadCaseName.GetString()) != 0)
				{
					bSame = TRUE;
					break;
				}
			}
			else
				bSame = FALSE;
		}
		//lex(fp, yytext);	// \n
		if (!bSame)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_));
			return FALSE;
		}
		lex(fp, yytext);	//the next line
	}

// code	
	csText = yytext;
	lex(fp, yytext);	// code	name
	m_pData->m_strCode = yytext;
	lex(fp, yytext);	//Initial imperfection load case considered check or not:
	lex(fp, yytext);	// yes or no
	m_pData->m_bImperfection = strcmp(yytext, "YES") == 0 ? TRUE : FALSE;

	lex(fp, yytext);	// Partial Coefficient
	//Partial Coefficient
	lex(fp, yytext);	//D, L, LR...	
	std::string strAsterisk = "*";
	while(strcmp(yytext, strAsterisk.c_str()) != 0)
	{
		m_pData->m_item.m_aType.Add(yytext);
		lex(fp, yytext);	//","
		lex(fp, yytext);	//D, L, LR...	or asterisk
	}
	int i=0;
	lex(fp, yytext);	//D
	while (strcmp(yytext, csStagCase.c_str()) != 0)
	{
		CArrayEx<double, double> aFactor;
		while(strcmp(yytext, strAsterisk.c_str()) != 0)
		{
			double dVal = atof(yytext);
			aFactor.Add(dVal);
			lex(fp, yytext);	// ","
			lex(fp, yytext);	//L, LR, W...or "\n"
		}		
		m_pData->m_item.m_aFactor.Add(aFactor);
		lex(fp, yytext);	//the next line
	}

	lex(fp, yytext);	// ST
	if (strcmp(yytext, "ST") == 0)
		m_pData->m_nStage = 0;
	else if (strcmp(yytext, "CS") == 0)
		m_pData->m_nStage = 1;
	else
		m_pData->m_nStage = 2;

	lex(fp, yytext);	// Consider the orthogonal result or not:
	lex(fp, yytext);
	m_pData->m_bBiEarthquake = strcmp(yytext, "YES") == 0 ? TRUE : FALSE;
	if (m_pData->m_bBiEarthquake)
	{
		lex(fp, yytext);	// ","
		lex(fp, yytext);	//count
		int nCount = atoi(yytext);
		
		for (int i=0; i<nCount; i++)
		{
			LOAD_CASE_K keyX, keyY;
			lex(fp, yytext);
			keyX.nLoadCaseType = atoi(yytext)==1 ? D_LCOM_STATIC : D_LCOM_SPECTRUM;
			keyY.nLoadCaseType = atoi(yytext)==1 ? D_LCOM_STATIC : D_LCOM_SPECTRUM;
			lex(fp, yytext);	// ","
			lex(fp, yytext);	//key 1
			keyX.Key = atoi(yytext);
			lex(fp, yytext);	// ","		
			lex(fp, yytext);	//key 2
			keyY.Key = atoi(yytext);
			m_pData->m_mapBiEarthquake[keyX] = keyY;
		}
	}

	lex(fp, yytext);	// Consider Wind & Seis or not
	lex(fp, yytext);
	BOOL bWindASeis = strcmp(yytext, "YES") == 0 ? TRUE : FALSE;
	if (bWindASeis)
	{
		lex(fp, yytext);	// ","
		lex(fp, yytext);	//count
		int nCount = atoi(yytext);

		for (int i = 0; i < nCount; i++)
		{
			LOAD_CASE_K keyW, keyS;
			lex(fp, yytext);	//case type
			keyW.nLoadCaseType = D_LCOM_STATIC;
			keyS.nLoadCaseType = atoi(yytext) == 1 ? D_LCOM_STATIC : D_LCOM_SPECTRUM;
			lex(fp, yytext); // ","			
			lex(fp, yytext);	//key 1
			keyW.Key = atoi(yytext);
			lex(fp, yytext);	// ","		
			lex(fp, yytext);	//key 2
			keyS.Key = atoi(yytext);
			m_pData->m_mapBC2C[keyW] = keyS;
		}
	}

	lex(fp, yytext);	// Consider PartialFactors of special case or not:
	lex(fp, yytext);
	BOOL  bChkSpec = strcmp(yytext, "YES") == 0 ? TRUE : FALSE;
	if (bChkSpec)
	{
		lex(fp, yytext);	// ","
		lex(fp, yytext);	//count
		int nCount = atoi(yytext);
		lex(fp, yytext);	// key
		for (int i=0; i<nCount; i++)
		{
			LOAD_CASE_K key;
			key.Key = atoi(yytext);
			lex(fp, yytext);	// ","
			lex(fp, yytext);	// Type
			key.nLoadCaseType = atoi(yytext)==1 ? D_LCOM_STATIC : D_LCOM_SPECTRUM;
			lex(fp, yytext);	// ","
			lex(fp, yytext);	// coef
			m_pData->m_mapSpecialFactor[key] = atof(yytext);
			lex(fp, yytext);	// the next line
		}
	}
	else
		lex(fp, yytext);	// the next line

	lex(fp, yytext);	//dPhiL
	m_pData->m_dCombFactor[0] = atof(yytext);
	lex(fp, yytext);	// ","
	lex(fp, yytext);	//dPhiW
	m_pData->m_dCombFactor[1] = atof(yytext);
	lex(fp, yytext);	// ","
	lex(fp, yytext);	//dPhiT
	m_pData->m_dCombFactor[2] = atof(yytext);
	lex(fp, yytext); // no or data
	BOOL  bCombFactor = strcmp(yytext, "NO") == 0 ? FALSE : TRUE;
	if (bCombFactor)
	{
		int iComIndex = 0;
		while (strcmp(yytext, csLiveLoad.c_str()) != 0)
		{
			LOAD_CASE_K key;
			key.Key = atoi(yytext);
			key.nLoadCaseType = D_LCOM_STATIC;
			lex(fp, yytext);	// ","
			lex(fp, yytext);	//coef
			m_pData->m_mapCombFactor[key] = atof(yytext);
			lex(fp, yytext);	//the next line
			iComIndex++;
		}	
	}
	else
		lex(fp, yytext);	// the next line
	
	lex(fp, yytext);
	m_pData->m_dLLAdjustCombFactor = atof(yytext);

	lex(fp, yytext);// "Gravity load coefficient"
	lex(fp, yytext);
	m_pData->m_dLLGravityFactor = atof(yytext);
	lex(fp, yytext); // no or data
	BOOL bGravity = strcmp(yytext, "NO") == 0 ? FALSE : TRUE;
	if (bGravity)
	{
		while (strcmp(yytext, csAverCase.c_str()) != 0)
		{
			LOAD_CASE_K key;
			key.Key = atoi(yytext);
			key.nLoadCaseType = D_LCOM_STATIC;
			lex(fp, yytext);	// ","
			lex(fp, yytext);
			m_pData->m_mapGravityFactor[key] = atof(yytext);
			lex(fp, yytext);	// the next line
		}
	}
	else
		lex(fp, yytext);	// the next line

	lex(fp, yytext);	// m_pData->m_nSpecitlArrange
	m_pData->m_nSpecitlArrange = atoi(yytext);
	
	//AdverseCase
	lex(fp, yytext);	// AdverseCase
	lex(fp, yytext);	// no or list number
	BOOL bAverCase = strcmp(yytext, "NO") == 0 ? FALSE : TRUE;
	if (bAverCase)
	{
		while (strcmp(yytext, csMutuCase.c_str()) != 0)
		{
			lex(fp, yytext);	//  ","
			LOAD_CASE_K_ARR arCase;
			lex(fp, yytext);
			while (strcmp(yytext,strAsterisk.c_str()) != 0)
			{
				LOAD_CASE_K key;
				key.Key = atoi(yytext);
				key.nLoadCaseType = D_LCOM_STATIC;
				arCase.Add(key);
				lex(fp, yytext);	// ","
				lex(fp, yytext);
			}
			m_pData->m_aLLUnfavorable.Add(arCase);
			lex(fp, yytext);	// the next line
		}
	}
	else
		lex(fp, yytext);	// the next line

	//Mutually exclusive load
	lex(fp, yytext);	// no or //key1
	BOOL bMutuCase = strcmp(yytext, "NO") == 0 ? FALSE : TRUE;
	if (bMutuCase)
	{
		while (strcmp(yytext, csEnd.c_str()) != 0)
		{
			LOAD_CASE_K key1;
			key1.Key = atoi(yytext);
			lex(fp, yytext);	//","
			key1.nLoadCaseType = D_LCOM_STATIC;
			lex(fp, yytext); //key list
			LOAD_CASE_K_ARR arCase;
			while (strcmp(yytext, strAsterisk.c_str()) != 0)
			{
				LOAD_CASE_K key;
				key.Key = atoi(yytext);
				key.nLoadCaseType = D_LCOM_STATIC;
				arCase.Add(key);
				lex(fp, yytext);	// ","
				lex(fp, yytext);				
			}
			m_pData->m_mapExclusion[key1] = arCase;
			lex(fp, yytext);
		}
	}

	return TRUE;
}

int CCMLoadCombDefDlg::lex(FILE *fp, char *yytext)
{
	int		c, index;

	index	= 0;
	c		= getc(fp);

	// Discard white characters
	while ((c=='\r') || (c=='\t') || (c=='\n')) {
		c = getc(fp);
	}

	if (c == EOF ) return -1;

	// Handle(Discard) Comments
	while (c == '*') {
		c = getc(fp);
		if (c == '*') {
			c = getc(fp);
			while (c != '\n')
				c = getc(fp);
			c = getc(fp);
		}
		else {
			ungetc(c, fp);
			yytext[index++] = '*';
			yytext[index] = '\0';
			return 1;
		}
	}

	switch (c) {
	case ',':
		yytext[index++] = c;
		yytext[index] = '\0';
		break ;
	default:
		while ((c!='*') && (c!=',') && (c!='\r') && (c!='\n')) {
			yytext[index++] = c;
			c = getc(fp);
			if (c == EOF || index >=120)
				return -1;
		}
		yytext[index] = '\0';
		if ((c=='*') || (c==','))
			ungetc(c, fp);
	}

	return 1;
}

void CCMLoadCombDefDlg::OnCmdBtnExport()
{
	UpdateData(TRUE);
	if (!Dlg2Data())
		return;

	CString csFileName = CDBLib::MakeFileNameWithNewExt(_T(".lct"));
	LPCTSTR szFileName;
	if (csFileName.IsEmpty()) szFileName = NULL;
	else szFileName = csFileName;

	CString csFilter = _T("LCT Files(*.lct)|*.lct|All File(*.*)|*.*||");

	CFileDialog fd(FALSE, _T(".lct"), szFileName,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_LONGNAMES, 
		csFilter, NULL);
	if (fd.DoModal() != IDOK) return;

	csFileName = fd.GetPathName();

	if(!WriteData(csFileName)) return;

	// Launching the Text Editor;
	CString strTitle = _T("LOAD COMBINATION DEFINE"); 

	CDBLib::RunTextEditor(csFileName, strTitle);  

	//AfxMessageBox(_LS(IDS_MAIN_EMFDXF_Its_Succeed_to_Convert));
}

void CCMLoadCombDefDlg::OnDlgClose()
{
	if (!Dlg2Data())
		return;
	DestroyWindow();
}
