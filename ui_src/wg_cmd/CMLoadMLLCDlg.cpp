// CMLoadMLLCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadMLLCDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_main\wg_mainRes2.h"

#include "CMLoadCombDlg.h"
#include "CMLoadCaseDlg.h"

#define D_CB    _ULS(CB)
#define D_CBS   _ULS(CBS)
#define D_CBC   _ULS(CBC)
#define D_CBF   _ULS(CBF)
#define D_CBR   _ULS(CBR)

#define D_CB2    _ULS('CB: ')   //공백 또는 , 가 있는 경우 '로 둘러 싼다.
#define D_CBS2   _ULS('CBS: ')  
#define D_CBC2   _ULS('CBC: ')
#define D_CBF2   _ULS('CBF: ')
#define D_CBR2   _ULS('CBR: ')

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadMLLCDlg dialog

CCMLoadMLLCDlg::CCMLoadMLLCDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMLoadMLLCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLoadMLLCDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_bCheckLoad = TRUE;
	m_bCheckUpdate = TRUE;
}


void CCMLoadMLLCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadMLLCDlg)
	DDX_Control(pDX, IDC_CMD_MLLC_CASE_CBO, m_cboLoadCase);
	DDX_Control(pDX, IDC_CMD_MLLC_COMB_CBO, m_cboLoadComb);
	DDX_Control(pDX, IDC_CMD_LIST_LOAD, m_listLoad);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMLoadMLLCDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMLoadMLLCDlg)
	ON_BN_CLICKED(IDC_CMD_MLLC_COMB_BTN, OnCmdMllcCombBtn)
	ON_BN_CLICKED(IDC_CMD_MLLC_CASE_BTN, OnCmdMllcCaseBtn)
	ON_BN_CLICKED(IDC_CMD_MLLC_ACHECK_BTN, OnCmdMllcAcheckBtn)
	ON_BN_CLICKED(IDC_CMD_MLLC_UCHECK_BTN, OnCmdMllcUcheckBtn)
	ON_BN_CLICKED(IDC_CMD_MLLC_CANCEL_BTN, OnCmdMllcCancelBtn)
	ON_BN_CLICKED(IDC_CMD_MLLC_OK_BTN, OnCmdMllcOkBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLoadMLLCDlg message handlers

BOOL CCMLoadMLLCDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	//((CButton*)GetDlgItem(IDC_CMD_MLLC_FLOOR_CHK))->EnableWindow(FALSE); //기능이 안될것 같아서 임시로 Disable 시킴.

	InitLoadData();	

	m_listLoad.SetColumnWidth(180);
	T_LOAD_ATTR LoadAttr;
	LoadAttr.Initialize();
	DataExchange(LoadAttr, 0);
	OnCmdMllcAcheckBtn();
	((CButton*)GetDlgItem(IDC_CMD_MLLC_DELETE_CHECK))->SetCheck(TRUE);

//  CWnd* pWnd = GetDlgItem(IDC_CMD_MLLC_PLANLOAD_CHK);
//  if(pWnd)
//    pWnd->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CCMLoadMLLCDlg::~CCMLoadMLLCDlg()
{
}

void CCMLoadMLLCDlg::InitLoadData()
{
	CArray<T_STLD_K,T_STLD_K> arStldKey;
	CArray<T_LCOM_K,T_LCOM_K> arLcomKey;
	T_STLD_D StldData;
	T_LCOM_D LcomData;
	int i;

	// Load Case
	m_cboLoadCase.ResetContent();

	m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKey);

	if(arStldKey.GetSize())
		for(i=0; i<arStldKey.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetStld(arStldKey.GetAt(i), StldData);
			m_cboLoadCase.AddString(StldData.LoadCaseName);
		}


	// Load Combination
	m_cboLoadComb.ResetContent();

	int j;
	CStringArray arLoadComb;
	arLoadComb.Add(D_CB2);    // for General
	arLoadComb.Add(D_CBS2);    // for Steel Design
	arLoadComb.Add(D_CBC2);    // for Concrete Design
	arLoadComb.Add(D_CBF2);    // for Footing Design
	arLoadComb.Add(D_CBR2);    // for SRC Design
	BOOL bCheckLcom=FALSE;

	for(i=1; i<=5; i++)
	{
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(i, arLcomKey);	

		if(arLcomKey.GetSize())
			bCheckLcom = TRUE;
		else
			continue;

		for(j=0; j<arLcomKey.GetSize(); j++)
		{
			m_pDoc->m_pAttrCtrl->GetLcom(i, arLcomKey.GetAt(j), LcomData);
			m_cboLoadComb.AddString(arLoadComb.GetAt(i-1) + LcomData.LoadCombName);
		}
		arLcomKey.RemoveAll();
	}

	// 데이타가 존재한다면 화면을 갱신하고 아니면 디폴트값으로.
	InitMLLCData();
}

void CCMLoadMLLCDlg::InitMLLCData()  //DB에 있는 데이타 화면에 표시.
{
	;
}

void CCMLoadMLLCDlg::OnCmdMllcCombBtn() 
{
	m_bCheckLoad = FALSE;

	m_LcomKeyOld = 0;
	m_nOldCombType = 0;
	CString strOldLoadComb;
	m_cboLoadComb.GetWindowText(strOldLoadComb);
	if(strOldLoadComb != _T(""))
		m_LcomKeyOld = m_pDoc->m_pAttrCtrl->GetLcomKey(GetLoadCombKey(strOldLoadComb, m_nOldCombType), strOldLoadComb.Right(strOldLoadComb.GetLength()-4));

	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_COMBINATION,0));
}

void CCMLoadMLLCDlg::OnCmdMllcCaseBtn() 
{
	m_bCheckLoad = TRUE;

	m_StldKeyOld = 0;
	CString strTempLCaseName;
	m_cboLoadCase.GetWindowText(strTempLCaseName);
	if(strTempLCaseName != _T(""))
		m_StldKeyOld = m_pDoc->m_pAttrCtrl->GetStldKey(strTempLCaseName);

	m_LcomKeyOld = 0;
	m_nOldCombType = 0;
	CString strOldLoadComb;
	m_cboLoadComb.GetWindowText(strOldLoadComb);
	if(strOldLoadComb != _T(""))
		m_LcomKeyOld = m_pDoc->m_pAttrCtrl->GetLcomKey(GetLoadCombKey(strOldLoadComb, m_nOldCombType), strOldLoadComb.Right(strOldLoadComb.GetLength()-4));

	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CCMLoadMLLCDlg::OnCmdMllcAcheckBtn() 
{
	int nCount;
	nCount = m_listLoad.GetCount();
	for (int i = 0; i < nCount; i++)
		m_listLoad.SetCheck(i, 1);
}

void CCMLoadMLLCDlg::OnCmdMllcUcheckBtn() 
{
	int nCount;
	nCount = m_listLoad.GetCount();
	for (int i = 0; i < nCount; i++)
		m_listLoad.SetCheck(i, 0);
}

void CCMLoadMLLCDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		ModifyLoadData(m_bCheckLoad);
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		ModifyLoadData(m_bCheckLoad);
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMLoadMLLCDlg::ModifyLoadData(BOOL bCheckLoad)
{
	if(m_bCheckUpdate)
	{
		if(bCheckLoad)
		{
			InitLoadCaseData();
			InitLoadCombData();
		}
		else
			InitLoadCombData();
	}
}

void CCMLoadMLLCDlg::InitLoadCaseData()
{
	m_cboLoadCase.ResetContent();

	T_STLD_D StldData;
	CArray<T_STLD_K,T_STLD_K> arStldKey;
	
	if(!m_pDoc->m_pAttrCtrl->GetCountStld())
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Load_Case_was_not_defined));  
		return;
	}

	m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKey);

	for(int i=0; i<arStldKey.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetStld(arStldKey.GetAt(i), StldData);
		m_cboLoadCase.AddString(StldData.LoadCaseName);
	}

	if(m_StldKeyOld && m_pDoc->m_pAttrCtrl->ExistStld(m_StldKeyOld))
	{
		m_pDoc->m_pAttrCtrl->GetStld(m_StldKeyOld, StldData);
		if(CB_ERR==m_cboLoadCase.SelectString(-1, StldData.LoadCaseName))
			m_cboLoadCase.SetWindowText(_T(""));
	}
	else
	{
		m_cboLoadCase.SetWindowText(_T(""));
	}
}

void CCMLoadMLLCDlg::InitLoadCombData()
{
	m_cboLoadComb.ResetContent();
	
	CArray<T_LCOM_K,T_LCOM_K> arLcomKey;
	T_LCOM_D LcomData;

	int j;
	CStringArray arLoadComb;
	arLoadComb.Add(D_CB2);    // for General
	arLoadComb.Add(D_CBS2);    // for Steel Design
	arLoadComb.Add(D_CBC2);    // for Concrete Design
	arLoadComb.Add(D_CBF2);    // for Footing Design
	arLoadComb.Add(D_CBR2);    // for SRC Design
	BOOL bCheckLcom=FALSE;

	for(int i=1; i<=5; i++)
	{
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(i, arLcomKey);	

		if(arLcomKey.GetSize())
			bCheckLcom = TRUE;
		else
			continue;

		for(j=0; j<arLcomKey.GetSize(); j++)
		{
			m_pDoc->m_pAttrCtrl->GetLcom(i, arLcomKey.GetAt(j), LcomData);
			m_cboLoadComb.AddString(arLoadComb.GetAt(i-1) + LcomData.LoadCombName);
		}
		arLcomKey.RemoveAll();
	}
	
	if(bCheckLcom && m_nOldCombType && m_pDoc->m_pAttrCtrl->ExistLcom(m_nOldCombType, m_LcomKeyOld))
	{
		m_pDoc->m_pAttrCtrl->GetLcom(m_nOldCombType, m_LcomKeyOld, LcomData);

		if(CB_ERR==m_cboLoadComb.SelectString(-1, GetLoadCombName(m_nOldCombType, LcomData.LoadCombName)))
			m_cboLoadComb.SetWindowText(_T(""));
	}
	else
	{
		m_cboLoadComb.SetWindowText(_T(""));
	}
}

T_LCOM_K CCMLoadMLLCDlg::GetLoadCombKey(CString& strLoadComb, int& nLoadType)
{
	CString strTemp = strLoadComb.Left(3);
	CString strTempLcom = strLoadComb.Right(strLoadComb.GetLength() - 4);
	strTempLcom.TrimLeft();

	if(strTemp == _ULS(CB:)) 
	{
		nLoadType = 1;
		return m_pDoc->m_pAttrCtrl->GetLcomKey(nLoadType, strTempLcom);
	}
	else if(strTemp == _ULS(CBS))
	{
		nLoadType = 2;
		return m_pDoc->m_pAttrCtrl->GetLcomKey(nLoadType, strTempLcom);
	}
	else if(strTemp == _ULS(CBC))
	{
		nLoadType = 3;
		return m_pDoc->m_pAttrCtrl->GetLcomKey(nLoadType, strTempLcom);
	}
	else if(strTemp == _ULS(CBF))
	{
		nLoadType = 4;
		return m_pDoc->m_pAttrCtrl->GetLcomKey(nLoadType, strTempLcom);
	}
	else if(strTemp == _ULS(CBR))
	{
		nLoadType = 5;
		return m_pDoc->m_pAttrCtrl->GetLcomKey(nLoadType, strTempLcom);
	}
	else
	{
		nLoadType = 0;
		return 	0;
	}
}

CString CCMLoadMLLCDlg::GetLoadCombName(int nCombType, CString strLoadCombName)
{
	CString strTempCombName;
	switch(nCombType)
	{
		case 1:
			strTempCombName = D_CB2+strLoadCombName;
			return strTempCombName;
			break;
		case 2:
			strTempCombName = D_CBS2+strLoadCombName;
			return strTempCombName;
			break;
		case 3:
			strTempCombName = D_CBC2+strLoadCombName;
			return strTempCombName;
			break;
		case 4:
			strTempCombName = D_CBF2+strLoadCombName;
			return strTempCombName;
			break;
		case 5:
			strTempCombName = D_CBR2+strLoadCombName;
			return strTempCombName;
			break;
		default:
			return _T("");
			break;
	}
}

void CCMLoadMLLCDlg::OnCmdMllcCancelBtn() 
{
	OnCancel();	
}

void CCMLoadMLLCDlg::OnCmdMllcOkBtn() 
{
	T_LOAD_ATTR LoadAttrData;
	T_STLD_K    StldKey;
	T_LCOM_D    LcomData;
	T_LCOM_K    LcomKey;
	int nLoadType;

	LcomData.Initialize();
	
	CString strTempLcom;
	m_cboLoadComb.GetWindowText(strTempLcom);

	if(strTempLcom == _T(""))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_specified_load_combina));
		return;
	}

	LcomKey = GetLoadCombKey(strTempLcom,nLoadType);
	m_pDoc->m_pAttrCtrl->GetLcom(nLoadType, LcomKey, LcomData);     // Load Combination

	CString strTempStld;
	m_cboLoadCase.GetWindowText(strTempStld);
	if(strTempStld == _T(""))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_specified_load_case_));
		return;
	}
	StldKey = m_pDoc->m_pAttrCtrl->GetStldKey(strTempStld);

	DataExchange(LoadAttrData, 1);

	BOOL bDeleteLoad=FALSE;
	bDeleteLoad = BST_CHECKED == this->IsDlgButtonChecked(IDC_CMD_MLLC_DELETE_CHECK);

	m_bCheckUpdate = FALSE;
	T_LCTB_D DataLctb;
	if(!MakeLctbData(LcomData, StldKey, DataLctb)) return;
	if(m_pDoc->m_pDataCtrl->MakeLoadsByCombination(LcomData, StldKey, LoadAttrData, DataLctb, bDeleteLoad))
	{		
		m_cboLoadCase.SetCurSel(-1);
		m_cboLoadComb.SetCurSel(-1);
		m_bCheckUpdate = TRUE;    
	}
	else
		m_bCheckUpdate = TRUE;
}


BOOL CCMLoadMLLCDlg::MakeLctbData(T_LCOM_D& LcomData, T_STLD_K StldKey, T_LCTB_D& DataLctb)
{  
	T_STLD_D DataStld;
	T_LCTB_BASE LctbBase;

	DataLctb.Initialize();  
	if(!m_pDoc->m_pAttrCtrl->GetStld(StldKey, DataStld)) return FALSE;
	DataLctb.StldKey = StldKey;
	DataLctb.strLctbName = DataStld.LoadCaseName;
	DataLctb.Description = DataStld.Description;

	T_LCOM_D_UL LcomUlData;
	LcomUlData.Initialize();
	if(!m_pDoc->m_pAttrCtrl->NormalizeLcom(LcomData, LcomUlData)) return FALSE;
	if(LcomUlData.Combination.GetSize() == 0) return FALSE;	
	for(int i = 0; i < LcomUlData.Combination.GetSize(); i++) 
	{
		LctbBase.Initialize();
		if(!LcomUlData.Combination[i].LoadCaseKey) break;  // 키가 없으면 그 이후는 skip
		LctbBase.LoadCaseKey = LcomUlData.Combination[i].LoadCaseKey;
		LctbBase.iAnalType = LcomUlData.Combination[i].AnalType;
		LctbBase.dFactor = LcomUlData.Combination[i].Factor;
		DataLctb.aLctbBase.Add(LctbBase);    
	}
	return TRUE;
}

/*
BOOL CCMLoadMLLCDlg::MakeLctbData(T_LCOM_D& LcomData, T_STLD_K StldKey)
{
	T_LCTB_D DataLctb;  
	T_STLD_D DataStld;
	T_LCTB_BASE LctbBase;

	DataLctb.Initialize();  
	if(!m_pDoc->m_pAttrCtrl->GetStld(StldKey, DataStld)) return FALSE;
	DataLctb.StldKey = StldKey;
	DataLctb.strLctbName = DataStld.LoadCaseName;
	DataLctb.Description = DataStld.Description;

	T_LCOM_D_UL LcomUlData;
	LcomUlData.Initialize();
	if(!m_pDoc->m_pAttrCtrl->NormalizeLcom(LcomData, LcomUlData)) return FALSE;
	if(LcomUlData.Combination.GetSize() == 0) return FALSE;	
	for(int i = 0; i < LcomUlData.Combination.GetSize(); i++) 
	{
		LctbBase.Initialize();
		if(!LcomUlData.Combination[i].LoadCaseKey) break;  // 키가 없으면 그 이후는 skip
		LctbBase.LoadCaseKey = LcomUlData.Combination[i].LoadCaseKey;
		LctbBase.iAnalType = LcomUlData.Combination[i].AnalType;
		LctbBase.dFactor = LcomUlData.Combination[i].Factor;
		DataLctb.aLctbBase.Add(LctbBase);    
	}
	POSITION pos=m_pDoc->m_pAttrCtrl->GetStartLctb();
	T_LCTB_K key;
	T_LCTB_D data;
	BOOL bModify = FALSE;
	while(pos != NULL)
	{
		m_pDoc->m_pAttrCtrl->GetNextLctb(pos, key, data);
		if(data.strLctbName == DataLctb.strLctbName) 
		{
			DataLctb.LoadContributionId = data.LoadContributionId;
			bModify = TRUE;
			break;
		}
	}
	if(bModify)
	{
		if(!m_pDoc->m_pDataCtrl->ModifyLctb(data.strLctbName, DataLctb)) return FALSE;
	}    
	else
	{
		if(!m_pDoc->m_pDataCtrl->AddLctb(DataLctb)) return FALSE;
	}
	return TRUE;
}
*/
void CCMLoadMLLCDlg::DataExchange(T_LOAD_ATTR& LoadAttr, int nOperation)
{
	struct T_LOAD_ATTR_ITEM
	{
		CString sID;
		int* pVar;
		int nMask;
	};
	T_LOAD_ATTR_ITEM aItemLoad[] = {
		{_LS(IDS_CMD_LOAD_ATTR_BODF), &LoadAttr.bCopyBodf, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_CMD_LOAD_ATTR_NBOF), &LoadAttr.bCopyNbof, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_CMD_LOAD_ATTR_CNLD), &LoadAttr.bCopyCnld, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_CMD_LOAD_ATTR_SDSP), &LoadAttr.bCopySdsp, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_CMD_LOAD_ATTR_BMLD), &LoadAttr.bCopyBmld, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_CMD_LOAD_ATTR_FBLD), &LoadAttr.bCopyFbld, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_CMD_LOAD_ATTR_FMLD), &LoadAttr.bCopyFmld, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_CMD_LOAD_ATTR_PRES), &LoadAttr.bCopyPres, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_CMD_LOAD_ATTR_PNLD), &LoadAttr.bCopyPnld, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_CMD_LOAD_ATTR_STMP), &LoadAttr.bCopyStmp, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_CMD_LOAD_ATTR_NTMP), &LoadAttr.bCopyNtmp, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_CMD_LOAD_ATTR_ETMP), &LoadAttr.bCopyEtmp, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_CMD_LOAD_ATTR_GTMP), &LoadAttr.bCopyGtmp, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_CMD_LOAD_ATTR_BTMP), &LoadAttr.bCopyBtmp, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_CMD_LOAD_ATTR_PRST), &LoadAttr.bCopyPrst, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_CMD_LOAD_ATTR_PTNS), &LoadAttr.bCopyPtns, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_CMD_LOAD_ATTR_TDPL), &LoadAttr.bCopyTdpl, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_CMD_LOAD_ATTR_WIND), &LoadAttr.bCopyWind, D_PROD_GEN},
		{_LS(IDS_CMD_LOAD_ATTR_SEIS), &LoadAttr.bCopySeis, D_PROD_GEN},
	};

	int nMask = 0;
#if defined(_MGEN)
	nMask = D_PROD_GEN;
#elif defined(_CIVIL)
	nMask = D_PROD_CVL;
#elif defined(_TOWER)
	nMask = D_PROD_TOW;
#else
	#error _T("Unknown Product Definition!")
#endif

	int i, nCount, ix=0;
	nCount = sizeof(aItemLoad)/sizeof(T_LOAD_ATTR_ITEM);
	for (i = 0; i < nCount; i++)
	{
		if ((nMask & aItemLoad[i].nMask) == 0) continue;
		if (nOperation == 0)
		{
			ix = m_listLoad.AddString(aItemLoad[i].sID);
			m_listLoad.SetCheck(ix, *aItemLoad[i].pVar);
		}
		else
		{
			*aItemLoad[i].pVar = m_listLoad.GetCheck(ix++);
		}
	}
}