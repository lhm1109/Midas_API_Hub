// CMLoadMLLC_CivilDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadMLLC_CivilDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_main\wg_mainRes2.h"

#include "CMLoadCombDlg.h"
#include "CMLoadCaseDlg.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\ClassStld.h"

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
// CCMLoadMLLC_CivilDlg dialog

CCMLoadMLLC_CivilDlg::CCMLoadMLLC_CivilDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMLoadMLLC_CivilDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLoadMLLC_CivilDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_bCheckLoad = TRUE;
	m_bCheckUpdate = TRUE;
}


void CCMLoadMLLC_CivilDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadMLLC_CivilDlg)
	DDX_Control(pDX, IDC_CMD_MLLC_POSITION_CMB, m_cmbPosition);
	DDX_Control(pDX, IDC_CMD_MLLC_PREFIX_EDT, m_edtPrefix);
	DDX_Control(pDX, IDC_CMD_LIST_SELECTED, m_lstSelected);
	DDX_Control(pDX, IDC_CMD_LIST_DEFINED, m_lstDefined);
	DDX_Control(pDX, IDC_CMD_LIST_LOAD, m_listLoad);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMLoadMLLC_CivilDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMLoadMLLC_CivilDlg)
	ON_BN_CLICKED(IDC_CMD_MLLC_ACHECK_BTN, OnCmdMllcAcheckBtn)
	ON_BN_CLICKED(IDC_CMD_MLLC_UCHECK_BTN, OnCmdMllcUcheckBtn)
	ON_BN_CLICKED(IDC_CMD_MLLC_CANCEL_BTN, OnCmdMllcCancelBtn)
	ON_BN_CLICKED(IDC_CMD_MLLC_OK_BTN, OnCmdMllcOkBtn)
	ON_BN_CLICKED(IDC_CMD_MLLC_SELECT_BTN, OnCmdMllcSelectBtn)
	ON_BN_CLICKED(IDC_CMD_MLLC_UNSELECT_BTN, OnCmdMllcUnselectBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLoadMLLC_CivilDlg message handlers

BOOL CCMLoadMLLC_CivilDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	//((CButton*)GetDlgItem(IDC_CMD_MLLC_FLOOR_CHK))->EnableWindow(FALSE); //기능이 안될것 같아서 임시로 Disable 시킴.

	InitLoadData();	

#ifdef _RUS
	m_listLoad.SetHorizontalExtent(1000);
#else
	m_listLoad.SetColumnWidth(180);
#endif

	T_LOAD_ATTR LoadAttr;
	LoadAttr.Initialize();
	DataExchange(LoadAttr, 0);
	OnCmdMllcAcheckBtn();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

CCMLoadMLLC_CivilDlg::~CCMLoadMLLC_CivilDlg()
{
}

void CCMLoadMLLC_CivilDlg::InitLoadData()
{

	CArray<T_LCOM_K,T_LCOM_K> arLcomKey;
	T_STLD_D StldData;
	T_LCOM_D LcomData;
	int i;
	m_lstDefined.ResetContent();
	m_lstSelected.ResetContent();
	m_arLcomKey.RemoveAll();
	m_aComType.RemoveAll();

	int j;
	int nIndex;
	CStringArray arLoadComb;
	arLoadComb.Add(D_CB2);    // for General
	arLoadComb.Add(D_CBS2);    // for Steel Design
	arLoadComb.Add(D_CBC2);    // for Concrete Design
	arLoadComb.Add(D_CBF2);    // for Footing Design
	arLoadComb.Add(D_CBR2);    // for SRC Design
	BOOL bCheckLcom=FALSE;

	int nAllSize=0;

	for(i=1; i<=5; i++)
	{
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(i, arLcomKey);
		int tempSize=arLcomKey.GetSize();

		nAllSize+=tempSize;

		for(j=0; j<tempSize; j++)
		{
			CString tempName;
			m_arLcomKey.Add(arLcomKey.GetAt(j));
			m_pDoc->m_pAttrCtrl->GetLcom(i, arLcomKey.GetAt(j), LcomData);
			tempName=arLoadComb.GetAt(i-1) + LcomData.LoadCombName;
			nIndex=m_lstDefined.AddString(tempName);
			m_lstDefined.SetItemData(nIndex, m_arLcomKey.GetSize()-1);
			m_aComType.Add(i);
		}
		arLcomKey.RemoveAll();
	}
	m_aSelFlag.SetSize(nAllSize);
	for(i=0;i<nAllSize;i++)
	{
		m_aSelFlag[i]=FALSE;
	}
	m_cmbPosition.ResetContent();

#if defined(_CIVIL)
	m_cmbPosition.AddString(_LS(IDS_WG_CMD__ADDD__Steel_Design));
	m_cmbPosition.AddString(_LS(IDS_WG_CMD__ADDD__Concrete_Design));
	m_cmbPosition.AddString(_LS(IDS_WG_CMD__ADDD__SRC_Design));
#else
	m_cmbPosition.AddString(_LS(IDS_WG_CMD__ADDD__Steel_Design));
	m_cmbPosition.AddString(_LS(IDS_WG_CMD__ADDD__Concrete_Design));
	m_cmbPosition.AddString(_LS(IDS_WG_CMD__ADDD__Footing_Design));
	m_cmbPosition.AddString(_LS(IDS_WG_CMD__ADDD__SRC_Design));
#endif

	CDlgUtil::CobxAdjustListBoxWidth(m_cmbPosition);

	m_edtPrefix.SetWindowText(_T("N"));
	m_cmbPosition.SelectString(0,_LS(IDS_WG_CMD__ADDD__Concrete_Design));


	// 데이타가 존재한다면 화면을 갱신하고 아니면 디폴트값으로.
	InitMLLCData();
}

void CCMLoadMLLC_CivilDlg::InitMLLCData()  //DB에 있는 데이타 화면에 표시.
{
	;
}

void CCMLoadMLLC_CivilDlg::OnCmdMllcAcheckBtn() 
{
	int nCount;
	nCount = m_listLoad.GetCount();
	for (int i = 0; i < nCount; i++)
		m_listLoad.SetCheck(i, 1);
}

void CCMLoadMLLC_CivilDlg::OnCmdMllcUcheckBtn() 
{
	int nCount;
	nCount = m_listLoad.GetCount();
	for (int i = 0; i < nCount; i++)
		m_listLoad.SetCheck(i, 0);
}

void CCMLoadMLLC_CivilDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CCMLoadMLLC_CivilDlg::ModifyLoadData(BOOL bCheckLoad)
{
	if(m_bCheckUpdate)
	{

		InitLoadCombData();

	}
}
void CCMLoadMLLC_CivilDlg::InitLoadCombData()
{
	//	m_cboLoadComb.ResetContent();
	CArray<T_LCOM_K,T_LCOM_K> arLcomKeyTemp;
	CArray<int,int> aComTypeTemp;
	CArray<T_LCOM_K,T_LCOM_K> arLcomKey;
	T_STLD_D StldData;
	T_LCOM_D LcomData;

	int i;
	m_lstDefined.ResetContent();
	m_arLcomKey.RemoveAll();
	m_aComType.RemoveAll();


	int j;
	int nIndex;
	CStringArray arLoadComb;
	arLoadComb.Add(D_CB2);    // for General
	arLoadComb.Add(D_CBS2);    // for Steel Design
	arLoadComb.Add(D_CBC2);    // for Concrete Design
	arLoadComb.Add(D_CBF2);    // for Footing Design
	arLoadComb.Add(D_CBR2);    // for SRC Design
	BOOL bCheckLcom=FALSE;

	int nAllSize=0;

	for(i=1; i<=5; i++)
	{
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(i, arLcomKey);
		int tempSize=arLcomKey.GetSize();

		nAllSize+=tempSize;

		for(j=0; j<tempSize; j++)
		{
			CString tempName;
			m_arLcomKey.Add(arLcomKey.GetAt(j));
			m_pDoc->m_pAttrCtrl->GetLcom(i, arLcomKey.GetAt(j), LcomData);
			tempName=arLoadComb.GetAt(i-1) + LcomData.LoadCombName;
			nIndex=m_lstDefined.AddString(tempName);
			m_lstDefined.SetItemData(nIndex, m_arLcomKey.GetSize()-1);
			m_aComType.Add(i);
		}
		arLcomKey.RemoveAll();
	}
	m_aSelFlag.SetSize(nAllSize);
	for(i=0;i<nAllSize;i++)
	{
		m_aSelFlag[i]=FALSE;
	}
	/*
	int nSelected=m_lstSelected.GetCount();
	for(i=0;i<nSelected;i++)
	{
	int nItemData=m_lstSelected.GetItemData(i);
	int nComType=m_aComType.GetAt(nItemData);
	m_aSelFlag.SetAt(nItemData,TRUE);
	CString tempName;
	m_pDoc->m_pAttrCtrl->GetLcom(m_aComType.GetAt(nItemData), m_arLcomKey.GetAt(nItemData), LcomData);
	tempName=arLoadComb.GetAt(m_aComType[i]-1) + LcomData.LoadCombName;
	int nDeleteIndex=m_lstDefined.FindString(0,tempName);

	if(nDeleteIndex!=LB_ERR)
	m_lstDefined.DeleteString(nDeleteIndex);
	}
	*/
	m_lstSelected.ResetContent();


}

T_LCOM_K CCMLoadMLLC_CivilDlg::GetLoadCombKey(CString& strLoadComb, int& nLoadType)
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

CString CCMLoadMLLC_CivilDlg::GetLoadCombName(int nCombType, CString strLoadCombName)
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

void CCMLoadMLLC_CivilDlg::OnCmdMllcCancelBtn() 
{
	OnCancel();	
}

void CCMLoadMLLC_CivilDlg::OnCmdMllcOkBtn() 
{

	m_bCheckUpdate = TRUE;

	// Data
	CString csPrefix;
	m_edtPrefix.GetWindowText(csPrefix);
	CArray<int, int> aLcomType;
	CArray<T_LCOM_K, T_LCOM_K> aLcomK;
	T_LOAD_ATTR LoadAttrData; 
	int nLcomTypeNew; // D_LCOMTYPE_????

#if defined(_CIVIL)
	if(m_cmbPosition.GetCurSel()==2)
		nLcomTypeNew=m_cmbPosition.GetCurSel()+3;
	else
		nLcomTypeNew=m_cmbPosition.GetCurSel()+2;
#else
	nLcomTypeNew=m_cmbPosition.GetCurSel()+2;
#endif

	int nSelected=m_lstSelected.GetCount();
	for(int i=0;i<nSelected;i++)
	{
		int nItemData=m_lstSelected.GetItemData(i);
		aLcomK.Add(m_arLcomKey.GetAt(nItemData));
		aLcomType.Add(m_aComType.GetAt(nItemData));
	}

	// Dlg2Data
	DataExchange(LoadAttrData, 1);

	CClassStld* pStldC = M_GetClass2(Stld);
	pStldC->MakeLoadsByCombination(csPrefix, aLcomType, aLcomK, LoadAttrData, nLcomTypeNew);
}


BOOL CCMLoadMLLC_CivilDlg::MakeLctbData(T_LCOM_D& LcomData, T_STLD_K StldKey, T_LCTB_D& DataLctb)
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


void CCMLoadMLLC_CivilDlg::DataExchange(T_LOAD_ATTR& LoadAttr, int nOperation)
{
	struct T_LOAD_ATTR_ITEM
	{
		CString str;
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
			ix = m_listLoad.AddString(aItemLoad[i].str);
			m_listLoad.SetCheck(ix, *aItemLoad[i].pVar);
		}
		else
		{
			*aItemLoad[i].pVar = m_listLoad.GetCheck(ix++);
		}
	}
}

void CCMLoadMLLC_CivilDlg::OnCmdMllcSelectBtn() 
{
	// TODO: Add your control notification handler code here
	CStringArray arLoadComb;
	arLoadComb.Add(D_CB2);    // for General
	arLoadComb.Add(D_CBS2);    // for Steel Design
	arLoadComb.Add(D_CBC2);    // for Concrete Design
	arLoadComb.Add(D_CBF2);    // for Footing Design
	arLoadComb.Add(D_CBR2);    // for SRC Design
	int nIndex;
	T_LCOM_D LcomData;
	int nMax = this->m_lstDefined.GetCount();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstDefined.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{

		nIndex = m_lstDefined.GetItemData(aIndex[i]);
		if(CheckCondition(nIndex)==FALSE)
			continue;
		m_aSelFlag[nIndex] = TRUE;
		m_lstDefined.DeleteString(aIndex[i]);
	}

	m_lstSelected.ResetContent();
	int nAllSize = m_aSelFlag.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		if (m_aSelFlag[i]) 
		{
			CString tempName;
			m_pDoc->m_pAttrCtrl->GetLcom(m_aComType.GetAt(i), m_arLcomKey.GetAt(i), LcomData);
			tempName=arLoadComb.GetAt(m_aComType[i]-1) + LcomData.LoadCombName;
			nIndex=m_lstSelected.AddString(tempName);
			m_lstSelected.SetItemData(nIndex,i);
		}
	}

}

void CCMLoadMLLC_CivilDlg::OnCmdMllcUnselectBtn() 
{
	// TODO: Add your control notification handler code here
	CStringArray arLoadComb;
	arLoadComb.Add(D_CB2);    // for General
	arLoadComb.Add(D_CBS2);    // for Steel Design
	arLoadComb.Add(D_CBC2);    // for Concrete Design
	arLoadComb.Add(D_CBF2);    // for Footing Design
	arLoadComb.Add(D_CBR2);    // for SRC Design
	int nIndex;
	int nMax = m_aSelFlag.GetSize();
	T_LCOM_D LcomData;
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstSelected.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_lstSelected.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = FALSE;
		m_lstSelected.DeleteString(aIndex[i]);
	}

	m_lstDefined.ResetContent();
	int nAllSize = m_aSelFlag.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		if (!m_aSelFlag[i]) 
		{
			CString tempName;
			m_pDoc->m_pAttrCtrl->GetLcom(m_aComType.GetAt(i), m_arLcomKey.GetAt(i), LcomData);
			tempName=arLoadComb.GetAt(m_aComType[i]-1) + LcomData.LoadCombName;
			nIndex = m_lstDefined.AddString(tempName);
			m_lstDefined.SetItemData(nIndex, i);
		}
	}

}
BOOL CCMLoadMLLC_CivilDlg::CheckCondition(int nIndex)
{
	T_LCOM_D LcomData;
	m_pDoc->m_pAttrCtrl->GetLcom(m_aComType.GetAt(nIndex), m_arLcomKey.GetAt(nIndex), LcomData);

	T_LCOM_D_UL rNormLcom;
	if(!(m_pDoc->m_pAttrCtrl->NormalizeLcom(LcomData, rNormLcom, TRUE)))
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_CREATE_LOAD_COM_UNABLE), LcomData.LoadCombName);
		return FALSE;
	}
	else
	{
		int nSize=rNormLcom.Combination.GetSize();
		int bReturn=TRUE;
		for(int i=0;i<nSize;i++)
		{
			if(rNormLcom.Combination[i].AnalType!=D_LCOM_STATIC)
			{
				GSaveHistoryFormatNF(_LS(IDS_CMD_CREATE_LOAD_COM_UNABLE), LcomData.LoadCombName);
				bReturn=FALSE;
			}
		}
		return bReturn;
	}

}
