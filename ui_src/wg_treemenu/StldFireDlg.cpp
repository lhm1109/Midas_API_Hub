// StldFireDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldFireDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\PostCtrl.h"

#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"
#include "..\wg_base\DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CMenuBarChildDlg
/////////////////////////////////////////////////////////////////////////////
// CStldFireDlg dialog


CStldFireDlg::CStldFireDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStldFireDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldFireDlg)
	m_nOption = 0;
	m_nCalcWay = 0;
	m_strCurLoadName = _T("");
	//}}AFX_DATA_INIT
	m_aDeleteCtrl.Add(IDC_TM_T1_TEMP_EDIT);
}


void CStldFireDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldFireDlg)
	//DDX_Control(pDX, IDC_TM_CBO_LC, m_LoadCase);
	DDX_Radio(pDX, IDC_TM_OPT_ADD, m_nOption);
	DDX_Radio(pDX, IDC_TM_FIRE_RADIO_CODE, m_nCalcWay);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_wndLdgrCobx);
	DDX_Control(pDX, IDC_TM_FIRE_CBO_FLUS, m_wndFlueFun);
	DDX_Control(pDX, IDC_TM_FIRE_CBO_USER, m_wndTimeTempFun);
	DDX_Control(pDX, IDC_TM_CBO_LC, m_LoadCase);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStldFireDlg, CDialog)
	//{{AFX_MSG_MAP(CStldFireDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_BTN_LC, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_PROJ_CONF_BTN, OnTmProjConfBtn)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_FIRE_RADIO_CODE, OnChangeCalcWay)
	ON_BN_CLICKED(IDC_TM_FIRE_RADIO_USER, OnChangeCalcWay)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_TM_FIRE_BTN_FLUS, &CStldFireDlg::OnFlueFunList)
	ON_BN_CLICKED(IDC_TM_FIRE_BTN_USER, &CStldFireDlg::OnSttfFunList)
	ON_CBN_SELCHANGE(IDC_TM_CBO_LC, &CStldFireDlg::OnSelLoadCase)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldFireDlg message handlers
BOOL CStldFireDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();
	
	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	// TODO: Add extra initialization here
	InitLoadCase();

	// Set Default Values
	m_nOption = 0;  // Default : Add

	//Flue Fun && Time-Temp Fun
	CArray<CString,CString&> aFlueName;
	CArray<CString,CString&> aTimeTempName;
	m_pDoc->m_pAttrCtrl->GetFlueList(aFlueName);
	m_pDoc->m_pAttrCtrl->GetSttfList(aTimeTempName);
	for(int i = 0;i < aFlueName.GetSize();i++)
		m_wndFlueFun.SetItemData(m_wndFlueFun.AddString(aFlueName[i]),m_pDoc->m_pAttrCtrl->GetFlueKey(aFlueName[i]));
	if(!aFlueName.IsEmpty())
		m_wndFlueFun.SetCurSel(0);
	for(int i = 0;i < aTimeTempName.GetSize();i++)
		m_wndTimeTempFun.SetItemData(m_wndTimeTempFun.AddString(aTimeTempName[i]),m_pDoc->m_pAttrCtrl->GetSttfKey(aTimeTempName[i]));
	if(!aTimeTempName.IsEmpty())
		m_wndTimeTempFun.SetCurSel(0);
	
	((CComboBox*)GetDlgItem(IDC_TM_FIRE_CBO_CODE))->AddString(_T("GB51249-2017"));
	((CComboBox*)GetDlgItem(IDC_TM_FIRE_CBO_CODE))->SetCurSel(0);
	m_wndLdgrCobx.EnableWindow(FALSE);
	// Enable/Disable/Show/Hide/Align controls
	OnChangeCalcWay();
	UpdateData(FALSE);
	return TRUE;
}

void CStldFireDlg::OnTmExecute() 
{
	// TODO: Add your control notification handler code here
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);
	if (aSelKey.GetSize() == 0) 
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
		return;
	}
	m_LoadCase.GetWindowText(m_strCurLoadName);
	if(m_strCurLoadName == FIRE_LOAD_CASE_NAME && !m_pDoc->m_pAttrCtrl->ExistStld(FIRE_LOAD_CASE_NAME))
	{
		T_STLD_D stlD;
		stlD.LoadCaseName = FIRE_LOAD_CASE_NAME;
		stlD.LoadCaseType = _T("T");
		m_pDoc->m_pDataCtrl->AddStld(stlD);
	}
	T_ELEM_D ElemD;
	T_MATL_D MatlD;
	T_LDGR_K LdgrK;
	m_wndLdgrCobx.GetSelectedLdgr(LdgrK);
	BOOL bSuccess;
	if (m_nOption == 0)
	{
		//FireD.GroupKey = LdgrK;
		//FireD.bCode = !m_nCalcWay;
		for(int i = aSelKey.GetSize()-1;i >= 0;i--)
		{
			//Filt Matl
			if(!m_pDoc->m_pAttrCtrl->GetElem(aSelKey[i],ElemD) || !m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat,MatlD) ||
				!(MatlD.Type == _T("S") || MatlD.Type == _T("U")))
			{
				aSelKey.RemoveAt(i);
				continue;
			}
		}
		if (aSelKey.GetSize() == 0) 
		{
			AfxMessageBox(_LS(IDS_TM_VSEC_NOT_FOUND_ELEMENTS));
			return;
		}
		if(!m_nCalcWay)
		{
			bSuccess = AddFireTempByGB51249_2017(aSelKey);
		}
		else
		{
			bSuccess = AddFireTempByUser(aSelKey);
		}
	}
	else
	{
		bSuccess = m_pDoc->m_pDataCtrl->DelEtmp(aSelKey,m_pDoc->m_pAttrCtrl->GetStldKey(m_strCurLoadName));
	}

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	
}

BOOL CStldFireDlg::AddFireTempByGB51249_2017(T_ELEM_K_LIST& aKey)
{
	CString strTemp;
	m_wndFlueFun.GetWindowText(strTemp);
	if(!m_pDoc->m_pAttrCtrl->ExistFlue(strTemp))
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___No_selected_flue_));
		return FALSE;
	}
	T_FLUE_K FlueK = m_pDoc->m_pAttrCtrl->GetFlueKey(strTemp);
	T_ELEM_D ElemD;
	T_MATL_D MatlD;
	T_ETMP_D EtmpD;

	//bSuccess = m_pDoc->m_pDataCtrl->AddFire(aSelKey, data, (m_nOption == 1));
	T_FIPA_D FipaD;
	T_FSSF_D FssfD;
	
	
	double dAlpha = 0.0;
	double dSigma = 5.67*pow(10,-8);
	CArray<T_ELEM_K, T_ELEM_K> aElemK;
	CArray<T_ETMP_D, T_ETMP_D&> aData;
	BOOL bAdd = FALSE;
	for(int i = 0;i < aKey.GetSize();i++)
	{
		VERIFY(m_pDoc->m_pAttrCtrl->GetElem(aKey[i],ElemD));
		VERIFY(m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat,MatlD));
		EtmpD.LoadCaseKey = m_pDoc->m_pAttrCtrl->GetStldKey(m_strCurLoadName);
		m_wndLdgrCobx.GetSelectedLdgr(EtmpD.GroupKey);
		if(!m_pDoc->m_pAttrCtrl->GetFipa(aKey[i],FipaD))
		{
			m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_DB_Error___fire_no_fipa_),aKey[i]);
			continue;
		}
		FipaD.dThickness /= 1000;
		if(!m_pDoc->m_pAttrCtrl->GetFssf(aKey[i],FssfD))
		{
			m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_DB_Error___fire_no_fssf_),aKey[i]);
			continue;
		}
		if(FssfD.dFactor < 1e-7)
		{
			ASSERT(FALSE);
			continue;
		}
		m_pDoc->m_pUnitCtrl->ConvertUnitMatlIn(MatlD);
		MatlD.dSpecificHeat *= 10;
		if(fabs(MatlD.Data1.Analysis.MassDensity) < 1e-7)
		{
			m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_DB_Error___fire_matl_no_density_),aKey[i]);
			continue;
		}
		if(fabs(MatlD.dSpecificHeat) < 1e-7)
		{
			m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_DB_Error___fire_matl_no_sheat_),aKey[i]);
			continue;
		}
		bAdd = TRUE;
		double dTs = 20.;
		double dAddTs = 0.0;
		int nStep = 5;
		int nCurTime = 0;
		double dTg = m_pDoc->m_pAttrCtrl->GetFlueTemp(nStep,FlueK);
		int nTimeCount = FipaD.dfireResTime*3600;
		while(nCurTime < nTimeCount)
		{
			nCurTime += nStep;//Step = 5 Second
			dTg = m_pDoc->m_pAttrCtrl->GetFlueTemp(nCurTime,FlueK);
			if(FipaD.firetype == T_FIPA_D::NO_FIRECONTAIN)
			{
				dAlpha = FipaD.dRadiance * dSigma * (pow(dTg + 273.,4) - pow(dTs + 273.,4)) / (dTg - dTs) + 25;
			}
			else if(FipaD.firetype == T_FIPA_D::NO_LIGHT_FIRECONTAIN)
			{
				dAlpha = FipaD.dConductivity / FipaD.dThickness / (1 + FipaD.dDensity * FipaD.dSHeat * FipaD.dThickness * FssfD.dFactor /
					2 / MatlD.Data1.Analysis.MassDensity / MatlD.dSpecificHeat);
			}
			else if(FipaD.firetype == T_FIPA_D::LIGHT_EXPANSION)
			{
				dAlpha = 1 / FipaD.dResistance;
			}
			else
			{
				dAlpha = FipaD.dConductivity / FipaD.dThickness;
			}
			dAddTs = dAlpha / MatlD.Data1.Analysis.MassDensity / MatlD.dSpecificHeat * FssfD.dFactor * (dTg - dTs) * nStep;
			dTs += dAddTs;
		}
		EtmpD.dblElemTemp= dTs;
		aElemK.Add(aKey[i]);
		aData.Add(EtmpD);
	}
	if(bAdd)
		return m_pDoc->m_pDataCtrl->AddEtmpByFire(aElemK,aData);
	return FALSE;
}

BOOL CStldFireDlg::AddFireTempByUser(T_ELEM_K_LIST& aKey)
{
	CString strTemp;
	m_wndTimeTempFun.GetWindowText(strTemp);
	if(!m_pDoc->m_pAttrCtrl->ExistSttf(strTemp))
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___No_selected_sttf_));
		return FALSE;
	}
	T_ETMP_D EtmpD;
	EtmpD.LoadCaseKey = m_pDoc->m_pAttrCtrl->GetStldKey(m_strCurLoadName);
	m_wndLdgrCobx.GetSelectedLdgr(EtmpD.GroupKey);
	CArray<T_ETMP_D, T_ETMP_D&> aData;
	aData.SetSize(aKey.GetSize());

	int nTime = 0.0;
	T_FIPA_D FipaD;
	for(int i = 0;i < aData.GetSize();i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetFipa(aKey[i],FipaD))
		{
			m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_DB_Error___fire_no_fipa_),aKey[i]);
			continue;
		}
		nTime = FipaD.dfireResTime*3600;
		EtmpD.dblElemTemp = m_pDoc->m_pAttrCtrl->GetSttfTemp(nTime,strTemp);
		aData[i] = EtmpD;
	}
	return  m_pDoc->m_pDataCtrl->AddEtmpByFire(aKey, aData);
}

void CStldFireDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CStldFireDlg::OnClickLoadCaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));	
}

void CStldFireDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	switch(LOWORD(pMsg->wParam))
	{
	case IDC_TM_OPT_ADD: m_nOption = 0; break;
	case IDC_TM_OPT_DELETE: m_nOption = 1; break;
	}

	if (m_nOption == 1)  // Delete
	{
	  CtrlEnableDisable(m_aDeleteCtrl, FALSE);
		this->m_wndLdgrCobx.ShowHideAll(TRUE);
	}
	else 
	{
		CtrlEnableDisable(m_aDeleteCtrl, TRUE);
		this->m_wndLdgrCobx.ShowHideAll(FALSE);
	}
}

void CStldFireDlg::OnChangeCalcWay()
{
	UpdateData(TRUE);
	CArray<UINT,UINT> aCtrlTimeTempID,aCtrlCode;
	aCtrlTimeTempID.Add(IDC_TM_FIRE_CBO_USER);
	aCtrlTimeTempID.Add(IDC_TM_FIRE_BTN_USER);
	aCtrlCode.Add(IDC_TM_FIRE_CBO_CODE);
	aCtrlCode.Add(IDC_TM_FIRE_CBO_FLUS);//
	aCtrlCode.Add(IDC_TM_FIRE_BTN_FLUS);//
	CDlgUtil::CtrlEnableDisable(this,aCtrlCode,!m_nCalcWay);
	CDlgUtil::CtrlEnableDisable(this,aCtrlTimeTempID,m_nCalcWay);

}

void CStldFireDlg::OnTmProjConfBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_CONFIG,0));
}

void CStldFireDlg::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);		
}

void CStldFireDlg::OnFlueFunList()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_LOAD_FLUE_GAS_TEMP,0));	
}

void CStldFireDlg::OnSttfFunList()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_LOAD_Fire_Fun_Time_Temp,0));	
}

void CStldFireDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CStldFireDlg::UpdateBuffer()
{
	m_wndFlueFun.ResetContent();
	m_wndTimeTempFun.ResetContent();
	CArray<CString,CString&> aFlueName;
	CArray<CString,CString&> aTimeTempName;
	m_pDoc->m_pAttrCtrl->GetFlueList(aFlueName);
	m_pDoc->m_pAttrCtrl->GetSttfList(aTimeTempName);
	for(int i = 0;i < aFlueName.GetSize();i++)
		m_wndFlueFun.SetItemData(m_wndFlueFun.AddString(aFlueName[i]),m_pDoc->m_pAttrCtrl->GetFlueKey(aFlueName[i]));
	for(int i = 0;i < aTimeTempName.GetSize();i++)
		m_wndTimeTempFun.SetItemData(m_wndTimeTempFun.AddString(aTimeTempName[i]),m_pDoc->m_pAttrCtrl->GetSttfKey(aTimeTempName[i]));
	if(!aFlueName.IsEmpty())
		m_wndFlueFun.SetCurSel(0);
	if(!aTimeTempName.IsEmpty())
		m_wndTimeTempFun.SetCurSel(0);
	InitLoadCase();
}

void CStldFireDlg::InitLoadCase()
{
	int nCurIndex = m_LoadCase.GetCurSel();
	if(nCurIndex == -1)
		nCurIndex = 0;
	m_LoadCase.ResetContent();
	CArray<T_STLD_K, T_STLD_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(KeyList);
	int nLoad=KeyList.GetSize();
	BOOL bExist;
	T_STLD_D DataStld;
	int nIndex = 0;
	for(int i=0; i<nLoad; i++)
	{
		if (m_pDoc->IsPostMode())
			bExist = m_pDoc->m_pPostCtrl->GetStld(KeyList[i], DataStld);
		else
			bExist = m_pDoc->m_pAttrCtrl->GetStld(KeyList[i], DataStld);
		if (bExist)
		{
			if(DataStld.LoadCaseType == _T("T"))
				m_LoadCase.SetItemData(m_LoadCase.AddString(DataStld.LoadCaseName),KeyList[i]);
		}
	}
	if(!m_pDoc->m_pAttrCtrl->ExistStld(FIRE_LOAD_CASE_NAME))
	{
		nIndex = m_LoadCase.AddString(FIRE_LOAD_CASE_NAME);
		m_LoadCase.SetItemData(nIndex,0);
	}
	m_LoadCase.SetCurSel(nCurIndex);
}

void CStldFireDlg::OnSelLoadCase()
{
	CString strTemp;
	m_LoadCase.GetWindowText(strTemp);
	BOOL bFireLoad = strTemp == FIRE_LOAD_CASE_NAME;
	m_wndLdgrCobx.EnableWindow(!bFireLoad);
}
