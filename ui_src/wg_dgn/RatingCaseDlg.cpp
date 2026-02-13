// RatingCaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "RatingCaseDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\CheckDialogOpen.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"

#include "RatingLiveFactorDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRatingCaseDlg dialog


CRatingCaseDlg::CRatingCaseDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CRatingCaseDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();

	//{{AFX_DATA_INIT(CRatingCaseDlg)
	m_PrimaryFactor = 1.0;
	m_AdjacentFactor = 1.0;
	m_iLimitState = -1;
	m_iEvaluation = 0;
	m_strName = _T("");
	m_strDescription = _T("");
	//}}AFX_DATA_INIT

	m_pMainGrid = new CRatingCaseMainGrid;
	m_pSubGrid = new CRatingCaseSubGrid;
}

CRatingCaseDlg::~CRatingCaseDlg()
{
	if(m_pMainGrid != NULL)
	{
		delete m_pMainGrid;
		m_pMainGrid = NULL;
	}
	if(m_pSubGrid != NULL)
	{
		delete m_pSubGrid;
		m_pSubGrid = NULL;
	}  
}


void CRatingCaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRatingCaseDlg)
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_LIST, m_List);
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_ADJACENT_COMBO, m_AdjacentCombo);
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_PRIMARY_COMBO, m_PrimaryCombo);
	DDX_Text(pDX, IDC_DGN_DEFINE_RATING_CASE_PRIMARY_FACTOR_EDIT, m_PrimaryFactor);
	DDX_Text(pDX, IDC_DGN_DEFINE_RATING_CASE_ADJACENT_FACTOR_EDIT, m_AdjacentFactor);
	DDX_Radio(pDX, IDC_DGN_DEFINE_RATING_CASE_SERVICE_RADIO, m_iLimitState);
	DDX_Radio(pDX, IDC_DGN_DEFINE_RATING_EVALU_DGN_RDO,      m_iEvaluation);
	DDX_Radio(pDX, IDC_DGN_DEFINE_RATING19_EVALU_DGN_RDO,      m_iEvaluation);
	DDX_Text(pDX, IDC_DGN_DEFINE_RATING_CASE_NAME_EDIT, m_strName);
	DDX_Text(pDX, IDC_DGN_DEFINE_RATING_CASE_DESCRIPTION_EDIT, m_strDescription);
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_SERVICE_RADIO, m_Ctrl_LimitState);

	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_MAIN_GRID, *m_pMainGrid);
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_SUB_GRID, *m_pSubGrid);
	//}}AFX_DATA_MAP
}

BOOL CRatingCaseDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CCheckDialogOpen::SetSoftwareRendering();

	// TODO: Add extra initialization here
		
	//m_pMainGrid->SetLcomType(m_nLcomType);
	//m_pSubGrid->SetLcomType(m_nLcomType);
	
	T_UNIT_SYSTEM UnitSystem;

	SetListCtrlHeader();

	InitBlrcKey();

	InitCurrentData();
	
	//m_pDoc->m_pUnitCtrl->GetUnitSystemLcom(UnitSystem);
	m_pMainGrid->Initialize(m_CurrentData.RateLoad, this);
	m_pSubGrid->Initialize(m_pDoc);
	
	m_pMainGrid->m_pSubGrid = m_pSubGrid;
	//m_pSubGrid->m_pMainGrid = m_pMainGrid;

	//InitLcomTypeCombo();
	m_pMainGrid->LoadDlgData();
	m_pSubGrid->SetData(m_CurrentData.RateLoad[0]);
	
	InitCtrl();

	InitMoveLoadCombo();

	LoadDlgMoveLoad();
	LoadDlgLimitState();
	LoadDlgEvaluation();
	LoadDlgNameDescription();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CRatingCaseDlg::TestDataInitialize()
{
	m_CurrentData.iLimitState = 0;
	m_CurrentData.RatingCaseName = _T("TestCase1");
	m_CurrentData.Description = _T("Test...Test...Test...Test");

	m_CurrentData.PrimaryVeh.AnalType = D_LCOM_MOVING;
	m_CurrentData.PrimaryVeh.LoadCaseKey = 0;
	m_CurrentData.PrimaryVeh.Factor = 1.5;

	m_CurrentData.AdjacentVeh.AnalType = D_LCOM_MOVING;
	m_CurrentData.AdjacentVeh.LoadCaseKey = 0;
	m_CurrentData.AdjacentVeh.Factor = 2.5;

	T_RTLD_D data;

	data.LoadType = D_RCTYPE_DC;
	data.FactorType = 0;
	data.dFactor[0] = 1.0;
	data.dFactor[1] = 1.0;	
	m_CurrentData.RateLoad[0] = data;
	
	data.LoadType = D_RCTYPE_DW;
	data.FactorType = 0;
	data.dFactor[0] = 1.0;
	data.dFactor[1] = 1.0;	
	m_CurrentData.RateLoad[1] = data;

	data.LoadType = D_RCTYPE_TEMPERATURE;
	data.FactorType = 1;
	data.dFactor[0] = 1.0;
	data.dFactor[1] = 1.0;	
	m_CurrentData.RateLoad[2] = data;

	data.LoadType = D_RCTYPE_SECONDARY;
	data.FactorType = 1;
	data.dFactor[0] = 1.0;
	data.dFactor[1] = 1.0;	
	m_CurrentData.RateLoad[3] = data;

	data.LoadType = D_RCTYPE_PERMANENT;
	data.FactorType = 1;
	data.dFactor[0] = 1.0;
	data.dFactor[1] = 1.0;	
	m_CurrentData.RateLoad[4] = data;

	data.LoadType = D_RCTYPE_USER;
	data.FactorType = 1;
	data.dFactor[0] = 1.0;
	data.dFactor[1] = 1.0;	
	m_CurrentData.RateLoad[5] = data;

	data.LoadType = D_RCTYPE_TG;
	data.FactorType = 1;
	data.dFactor[0] = 1.0;
	data.dFactor[1] = 1.0;	
	m_CurrentData.RateLoad[6] = data;
}

void CRatingCaseDlg::InitCurrentData()
{
	CArray<T_BLRC_K, T_BLRC_K> arBlrcKey;

	m_pDoc->m_pAttrCtrl->GetBlrcKeyList(arBlrcKey);

	if(arBlrcKey.GetSize() > 0)
	{
		for(int i=0 ; i<arBlrcKey.GetSize() ; i++)
		{
			InsertListItem(arBlrcKey[i]);
		}

		m_pDoc->m_pAttrCtrl->GetBlrc(arBlrcKey[0], m_CurrentData);
	}
	else
		m_CurrentData.Initialize();
}

void CRatingCaseDlg::InitBlrcKey()
{
	CArray<T_BLRC_K, T_BLRC_K> arBlrcKey;

	m_pDoc->m_pAttrCtrl->GetBlrcKeyList(arBlrcKey);

	if(arBlrcKey.GetSize() > 0)
	{
		int temp = 0;
		for(int i=0 ; i<arBlrcKey.GetSize() ; i++)
		{
			if(temp < arBlrcKey[i])
				temp = arBlrcKey[i];
		}
		Blrckey = temp+1;
	}
	else
	{
		Blrckey = 1;
	}
}

void CRatingCaseDlg::InitCtrl()
{
	CRect rRef;
	CRect rToMove;
	int nDistX, nDistY;
	CArray<UINT, UINT> aControls, aControls19;

	aControls.RemoveAll();
	aControls.Add(IDC_DGN_DEFINE_RATING_EVALU_DGN_RDO);
	aControls.Add(IDC_DGN_DEFINE_RATING_EVALU_LEGAL_RDO);

	aControls19.RemoveAll();
	aControls19.Add(IDC_DGN_DEFINE_RATING19_EVALU_DGN_RDO);
	aControls19.Add(IDC_DGN_DEFINE_RATING19_EVALU_LEGAL_RDO);
	aControls19.Add(IDC_DGN_DEFINE_RATING19_EVALU_PERMIT_RDO);

	T_RACD_D RacdD; RacdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRacd(RacdD);
	if(RacdD.nRatingCode !=AASHTO_LRFD12_PSC_RATING && RacdD.nRatingCode !=AASHTO_LRFD19_PSC_RATING)
	{
		//For LRFR12 Design Load / Legal Load / Permit Load
		GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_GRP)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_DGN_RDO)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_LEGAL_RDO)->EnableWindow(FALSE);

		CDlgUtil::CtrlEnableDisable(this, aControls, FALSE);
		CDlgUtil::CtrlEnableDisable(this, aControls19, FALSE);
	}
	else
	{
		if(RacdD.nRatingCode ==AASHTO_LRFD12_PSC_RATING)
		{
			CDlgUtil::CtrlShowHide(this, aControls,   TRUE);
			CDlgUtil::CtrlShowHide(this, aControls19, FALSE);
		}
		else if(RacdD.nRatingCode ==AASHTO_LRFD19_PSC_RATING)
		{
			CDlgUtil::CtrlShowHide(this, aControls,   FALSE);
			CDlgUtil::CtrlShowHide(this, aControls19, TRUE);

			GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_DGN_RDO)->GetWindowRect(rRef);
			GetDlgItem(IDC_DGN_DEFINE_RATING19_EVALU_DGN_RDO)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistXY(this, aControls19, nDistX, nDistY);
		}
	}
}

void CRatingCaseDlg::InitMoveLoadCombo()
{
	m_PrimaryCombo.ResetContent();
	m_AdjacentCombo.ResetContent();
	m_arMoveLoadKey.RemoveAll();

	CString item;
	int nRangeStart;

	int nNameLen = 0, nDescLen = 0;
	CSize nTempLen;
	CDC* pDC = GetDC();
	ASSERT(pDC);  

	//D_LCOM_MOVING		  4 : 이동 하중 조건
	T_MVCD_D DataMvcd;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	switch (DataMvcd.nCodeType)
	{
	case D_MOVE_CODE_NONE:
		break;
	case D_MOVE_CODE_AASHTO_STAN:
	case D_MOVE_CODE_AASHTO_LRFD:
	case D_MOVE_CODE_TAIWAN:
	case D_MOVE_CODE_KOREA:
	case D_MOVE_CODE_CANADA:
	case D_MOVE_CODE_PENDOT:
	case D_MOVE_CODE_EURO_BS:
	case D_MOVE_CODE_RUSSIA:
	case D_MOVE_CODE_KOREA_LRFD_2011:
	case D_MOVE_CODE_AUSTRALIA:
	case D_MOVE_CODE_POLAND:
	case D_MOVE_CODE_SOUTH_AFRICA:
	case D_MOVE_CODE_NEWZEALAND:
	case D_MOVE_CODE_BRAZIL:
		{
			T_MVLD_K MvldKey;
			T_MVLD_D MvldData;
			nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
			POSITION pos = m_pDoc->m_pAttrCtrl->GetStartMvld();
			while(pos != NULL)
			{
	      m_pDoc->m_pAttrCtrl->GetNextMvld(pos,MvldKey,MvldData);
				item.Format(_T("%s(MV)"), MvldData.LoadCaseName);
				m_PrimaryCombo.AddString(item);
				m_AdjacentCombo.AddString(item);
				m_arMoveLoadKey.Add(MvldKey);
			}
		}
		break;
	case D_MOVE_CODE_CHINA:
		{
			T_MVLDch_K MvldchKey;
			T_MVLDch_D MvldchData;
			nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
			POSITION pos = m_pDoc->m_pAttrCtrl->GetStartMvldch();
			while(pos != NULL)
			{
	      m_pDoc->m_pAttrCtrl->GetNextMvldch(pos,MvldchKey,MvldchData);
				item.Format(_T("%s(MV)"), MvldchData.LoadCaseName);
				m_PrimaryCombo.AddString(item);
				m_AdjacentCombo.AddString(item);
				m_arMoveLoadKey.Add(MvldchKey);
			}
		}
		break;
	case D_MOVE_CODE_JAPAN:
		{
			// japan version
			T_MVLDjp_K MvldjpKey;
			T_MVLDjp_D MvldjpData;
			nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
			POSITION pos = m_pDoc->m_pAttrCtrl->GetStartMvldjp();
			while(pos != NULL)
			{
	      m_pDoc->m_pAttrCtrl->GetNextMvldjp(pos,MvldjpKey,MvldjpData);
				item.Format(_T("%s(MV)"), MvldjpData.LoadCaseName);
				m_PrimaryCombo.AddString(item);
				m_AdjacentCombo.AddString(item);
				m_arMoveLoadKey.Add(MvldjpKey);
				nTempLen = pDC->GetTextExtent(MvldjpData.LoadCaseName+_T("(MV)"));
				if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
				nTempLen = pDC->GetTextExtent(MvldjpData.Description);
				if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
			}      
		}
		break;
	case D_MOVE_CODE_INDIA:
		{
			T_MVLDid_K MvldidKey;
			T_MVLDid_D MvldidData;
			nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
			POSITION pos = m_pDoc->m_pAttrCtrl->GetStartMvldid();
			while(pos != NULL)
			{
	      m_pDoc->m_pAttrCtrl->GetNextMvldid(pos,MvldidKey,MvldidData);
				item.Format(_T("%s(MV)"), MvldidData.LoadCaseName);
				m_PrimaryCombo.AddString(item);
				m_AdjacentCombo.AddString(item);
				m_arMoveLoadKey.Add(MvldidKey);
			}
		}
		break;
	case D_MOVE_CODE_BS:
		{
			T_MVLDbs_K MvldbsKey;
			T_MVLDbs_D MvldbsData;
			nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
			POSITION pos = m_pDoc->m_pAttrCtrl->GetStartMvldbs();
			while(pos != NULL)
			{
	      m_pDoc->m_pAttrCtrl->GetNextMvldbs(pos,MvldbsKey,MvldbsData);
				item.Format(_T("%s(MV)"), MvldbsData.LoadCaseName);
				m_PrimaryCombo.AddString(item);
				m_AdjacentCombo.AddString(item); 
				m_arMoveLoadKey.Add(MvldbsKey);
			}
		}
		break;
	case D_MOVE_CODE_FRANCE:
		{
			T_MVLDfr_K MvldfrKey;
			T_MVLDfr_D MvldfrData;
			nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
			POSITION pos = m_pDoc->m_pAttrCtrl->GetStartMvldfr();
			while(pos != NULL)
			{
				m_pDoc->m_pAttrCtrl->GetNextMvldfr(pos, MvldfrKey, MvldfrData);
				item.Format(_T("%s(MV)"), MvldfrData.LoadCaseName);
				m_PrimaryCombo.AddString(item);
				m_AdjacentCombo.AddString(item); 
				m_arMoveLoadKey.Add(MvldfrKey);
			}
		}
		break;
	case D_MOVE_CODE_TRANS:
		{
			T_MVLDtr_K MvldKey;
			T_MVLDtr_D MvldData;
			nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
			POSITION pos = m_pDoc->m_pAttrCtrl->GetStartMvldtr();
			while(pos != NULL)
			{
	      m_pDoc->m_pAttrCtrl->GetNextMvldtr(pos,MvldKey,MvldData);
				item.Format(_T("%s(MV)"), MvldData.LoadCaseName);
				m_PrimaryCombo.AddString(item);
				m_AdjacentCombo.AddString(item);
				m_arMoveLoadKey.Add(MvldKey);
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	} 

	// Sort arMoveLoad Key.
	//qsort(m_arMoveLoadKey.GetData(), m_arMoveLoadKey.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);
	m_PrimaryCombo.SetCurSel(0);
	m_AdjacentCombo.SetCurSel(0);
}

void CRatingCaseDlg::LoadDlgMoveLoad()
{
	m_PrimaryCombo.SetCurSel(GetMoveLoadKeyIndex(m_CurrentData.PrimaryVeh.LoadCaseKey));
	m_PrimaryFactor = m_CurrentData.PrimaryVeh.Factor;

	m_AdjacentCombo.SetCurSel(GetMoveLoadKeyIndex(m_CurrentData.AdjacentVeh.LoadCaseKey));
	m_AdjacentFactor = m_CurrentData.AdjacentVeh.Factor;

	UpdateData(FALSE);
}

int CRatingCaseDlg::GetMoveLoadKeyIndex(T_MVLD_K MvKey)
{
	for(int i=0 ; i<m_arMoveLoadKey.GetSize() ; i++)
	{
		if(m_arMoveLoadKey[i] == MvKey)
			return i;
	}

	return 0;
}

void CRatingCaseDlg::SaveDlgMoveLoad()
{
	UpdateData(TRUE);

	m_CurrentData.PrimaryVeh.AnalType = D_LCOM_MOVING;
	if(m_arMoveLoadKey.GetSize() > 0)
		m_CurrentData.PrimaryVeh.LoadCaseKey = m_arMoveLoadKey[m_PrimaryCombo.GetCurSel()];
	m_CurrentData.PrimaryVeh.Factor = m_PrimaryFactor;

	m_CurrentData.AdjacentVeh.AnalType = D_LCOM_MOVING;
	if(m_arMoveLoadKey.GetSize() > 0)
		m_CurrentData.AdjacentVeh.LoadCaseKey = m_arMoveLoadKey[m_AdjacentCombo.GetCurSel()];
	m_CurrentData.AdjacentVeh.Factor = m_AdjacentFactor;
}

void CRatingCaseDlg::LoadDlgLimitState()
{
	m_iLimitState = m_CurrentData.iLimitState;

	UpdateData(FALSE);
}

void CRatingCaseDlg::LoadDlgEvaluation()
{
	T_RACD_D RacdD; RacdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRacd(RacdD);

	if(RacdD.nRatingCode !=AASHTO_LRFD19_PSC_RATING && m_CurrentData.iEvaluation==2)
	{
		m_iEvaluation = 1;
	}
	else
	{
		m_iEvaluation = m_CurrentData.iEvaluation;
	}

	UpdateData(FALSE);
}

void CRatingCaseDlg::SaveDlgLimitState()
{
	UpdateData(TRUE);

	m_CurrentData.iLimitState = m_iLimitState;
}

void CRatingCaseDlg::SaveDlgEvaluation()
{
	UpdateData(TRUE);

	m_CurrentData.iEvaluation = m_iEvaluation;
}


void CRatingCaseDlg::LoadDlgNameDescription()
{
	m_strName = m_CurrentData.RatingCaseName;
	m_strDescription = m_CurrentData.Description;

	UpdateData(FALSE);
}

void CRatingCaseDlg::SaveDlgNameDescription()
{
	UpdateData(TRUE);

	m_CurrentData.RatingCaseName = m_strName;
	m_CurrentData.Description = m_strDescription;
}


BEGIN_MESSAGE_MAP(CRatingCaseDlg, CDialogMove)
	//{{AFX_MSG_MAP(CRatingCaseDlg)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_ADD_BTN, OnDgnDefineRatingCaseAddBtn)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_DELETE_BTN, OnDgnDefineRatingCaseDeleteBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_DEFINE_RATING_CASE_LIST, OnOnItemchangedDgnRatingCaseList)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_MODIFY_BTN, OnDgnDefineRatingCaseModifyBtn)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_CLOSE, OnDgnDefineRatingCaseClose)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_SERVICE_RADIO, OnDgnDefineRatingCaseServiceRadio_Clicked)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_STRENGTH_RADIO, OnDgnDefineRatingCaseServiceRadio_Clicked)
	//ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_EVALU_DGN_RDO, OnDgnDefineRatingCaseServiceRadio_Clicked)
	//ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_EVALU_LEGAL_RDO, OnDgnDefineRatingCaseServiceRadio_Clicked)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_LIVE_FACTORS_BTN, OnDgnDefineRatingCaseLiveFactorsBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRatingCaseDlg message handlers

void CRatingCaseDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}

void CRatingCaseDlg::OnDgnDefineRatingCaseAddBtn() 
{
	// TODO: Add your control notification handler code here
	SaveDlgMoveLoad();
	SaveDlgLimitState();
	SaveDlgEvaluation();
	SaveDlgNameDescription();

	m_pMainGrid->SaveCurrentGrid();
	for(int i=0 ; i<7 ; i++)
	{
		m_CurrentData.RateLoad[i] = m_pMainGrid->RateLoad[i];
	}

	if(RatingCaseNameCheck(m_CurrentData.RatingCaseName))
	{
		AfxMessageBox(_LS(IDS_DGN_RATING_ERROR_MESSAGE_SAMENAME), MB_OK);
		return;
	}

	if(m_pDoc->m_pDataCtrl->AddBlrc(Blrckey, m_CurrentData))
	{
		InsertListItem(Blrckey);	
		Blrckey++;
	}
}

void CRatingCaseDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_DGN_RATING_FACTOR_NAME), _LS(IDS_DGN_RATING_LIMITE_STATE), _LS(IDS_DGN_RATING_DESCRIPTION)};  // _T("Index"), _T("Element List")
	int nColWidth[] = {70, 70, 120};
	int nAlign[] = {LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT};
	int nColNum = sizeof(nColWidth)/sizeof(int);
	CString title;  
	LV_COLUMN lvcolumn;
	
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	for(int i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = nAlign[i];

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

int CRatingCaseDlg::FindInsertionPos(T_BLRC_K nID)
{
	int nCount = m_List.GetItemCount();
	for (int i=0; i < nCount; i++)
	{
        T_BLRC_K key = m_List.GetItemData(i);
		if (nID < key) { return i; }
	}
	return nCount;
}


BOOL CRatingCaseDlg::InsertListItem(T_BLRC_K Key)
{
	//DB에서 Data를 가져온다

	T_BLRC_D Data;
	
	m_pDoc->m_pAttrCtrl->GetBlrc(Key, Data);
	//Data = m_CurrentData;
	
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i=0; i < 3; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr(i, Key, Data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if (i == 0) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_List.InsertItem(&lvitem);
		}
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	m_List.SetItemData(nItem, (DWORD)Key);
	return TRUE;
}

CString CRatingCaseDlg::DataToStr(int i, T_BLRC_K Key, T_BLRC_D &Data)
{  
	CString str;

	if(i == 0) 
		str = Data.RatingCaseName;
	else if(i == 1)
	{
		if(Data.iLimitState == 0)
			str = _LS(IDS_DGN_RATING_SERVICE);
		else
			str = _LS(IDS_DGN_RATING_STRENGTH);
	}
	else if(i == 2)
		str = Data.Description;
	else ASSERT(0);

	return str;
}

BOOL CRatingCaseDlg::DeleteListItem(T_BLRC_K key)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)key;

	int nItem = m_List.FindItem(&FindInfo);
	if(nItem==-1) return TRUE;

	m_List.DeleteItem(nItem);

	return TRUE;
}

BOOL CRatingCaseDlg::ModifyListItem(T_BLRC_K key)
{
	T_BLRC_D Data;

	Data = m_CurrentData;

	if(!m_pDoc->m_pDataCtrl->ModifyBlrc(key, key, Data))
		return FALSE;

	LVFINDINFO FindInfo;
	FindInfo.flags  = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)key;

	int nItem = m_List.FindItem(&FindInfo);
	if(nItem == -1) return TRUE;

	for(int i=0; i<3; i++)
	{
		CString str = DataToStr(i, key, Data);
		m_List.SetItemText(nItem, i, str);
	}

	LVITEM lvitem;
	lvitem.iItem    = nItem;
	lvitem.iSubItem = 0;
	lvitem.mask     = LVIF_PARAM;
	lvitem.lParam   = (LPARAM)key;
	m_List.SetItem(&lvitem);

	return TRUE;
}


void CRatingCaseDlg::OnDgnDefineRatingCaseDeleteBtn() 
{
	// TODO: Add your control notification handler code here
	CArray<int, int> aSelItem;
	CArray<T_BLRC_K, T_BLRC_K> arKey;
	aSelItem.RemoveAll();
	arKey.RemoveAll();
	
	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;

	for(int i = 0; i < nSize; i++)
	{
		DWORD(key) = m_List.GetItemData(aSelItem[i]);
		arKey.Add(key);	
	}
	for(int i=0 ; i<arKey.GetSize() ; i++)
	{
		//DB에서도 지워야함
		if(m_pDoc->m_pDataCtrl->DelBlrc(arKey[i]))
			DeleteListItem(arKey[i]);
	}
}

void CRatingCaseDlg::OnOnItemchangedDgnRatingCaseList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIndex = pNMListView->iItem; 
	ChangeItem(nIndex);
	*pResult = 0;
}

void CRatingCaseDlg::ChangeItem(int nIndex)
{
	// get data
	m_BlrcKey = 0;
		
	if (nIndex != -1)
	{
	  m_BlrcKey = m_List.GetItemData(nIndex);
		//DB에서 데이터 가져옴
		m_pDoc->m_pAttrCtrl->GetBlrc(m_BlrcKey, m_CurrentData);

		//TestDataInitialize();//임시
	}

	m_pMainGrid->SetRtldData(m_CurrentData.RateLoad);
	
	LoadDlgMoveLoad();
	LoadDlgLimitState();
	LoadDlgEvaluation();
	LoadDlgNameDescription();
}

void CRatingCaseDlg::OnDgnDefineRatingCaseModifyBtn() 
{
	// TODO: Add your control notification handler code here
	SaveDlgMoveLoad();
	SaveDlgLimitState();
	SaveDlgEvaluation();
	SaveDlgNameDescription();

	m_pMainGrid->SaveCurrentGrid();
	for(int i=0 ; i<7 ; i++)
	{
		m_CurrentData.RateLoad[i] = m_pMainGrid->RateLoad[i];
	}

	CArray<int, int> aSelItem;
	CArray<T_BLRC_K, T_BLRC_K> arKey;
	aSelItem.RemoveAll();
	arKey.RemoveAll();
	
	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;

	for(int i=0 ; i < nSize ; i++)
	{
		DWORD(key) = m_List.GetItemData(aSelItem[i]);
		arKey.Add(key);	
	}

	//선택한것 중에서 가장 처음것에만 Modify한다
	ModifyListItem(arKey[0]);
	//m_pDoc->m_pDataCtrl->ModifyBlrc(arKey[0], arKey[0], m_CurrentData);
}

void CRatingCaseDlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
{
	int  nItem = -1;
	UINT uSelectedCount = pList->GetSelectedCount();
	aSelItem.SetSize(uSelectedCount);
	for(UINT i = 0; i < uSelectedCount; i++)
	{
		nItem = pList->GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		aSelItem[i] = nItem;
	}
}



void CRatingCaseDlg::OnDgnDefineRatingCaseClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}

BOOL CRatingCaseDlg::RatingCaseNameCheck(CString strName) 
{
	BOOL bSame = FALSE;

	CArray<T_BLRC_K, T_BLRC_K> arBlrcKey;
	m_pDoc->m_pAttrCtrl->GetBlrcKeyList(arBlrcKey);
	
	for(int i=0 ; i<arBlrcKey.GetSize() ; i++)
	{
		T_BLRC_D BlrcData;
		if(m_pDoc->m_pAttrCtrl->GetBlrc(arBlrcKey[i], BlrcData))
		{
			if(BlrcData.RatingCaseName == strName)
			{
				bSame = TRUE;
				return bSame;
			}
		}
	}

	return bSame;
}

// Add by sshan MNET:XXXX  '20060818 
// Radio 버튼을 Strength, Service로 선택함에 따라 Factor의 기본값을 변경 시켜줌
void CRatingCaseDlg::OnDgnDefineRatingCaseServiceRadio_Clicked() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_iLimitState == 0)
	{
		m_CurrentData.RateLoad[0].dFactor[0] = 1.0;
		m_CurrentData.RateLoad[0].dFactor[1] = 1.0;

		m_CurrentData.RateLoad[1].dFactor[0] = 1.0;
		m_CurrentData.RateLoad[1].dFactor[1] = 1.0;

		m_CurrentData.RateLoad[2].dFactor[0] = 1.0;
		m_CurrentData.RateLoad[3].dFactor[0] = 1.0;
		m_CurrentData.RateLoad[4].dFactor[0] = 1.0;
		m_CurrentData.RateLoad[5].dFactor[0] = 1.0;
		m_CurrentData.RateLoad[6].dFactor[0] = 1.0;
	}
	else
	{
		m_CurrentData.RateLoad[0].dFactor[0] = 1.25;
		m_CurrentData.RateLoad[0].dFactor[1] = 0.9;

		m_CurrentData.RateLoad[1].dFactor[0] = 1.50;
		m_CurrentData.RateLoad[1].dFactor[1] = 0.9;

		m_CurrentData.RateLoad[2].dFactor[0] = 1.0;
		m_CurrentData.RateLoad[3].dFactor[0] = 1.0;
		m_CurrentData.RateLoad[4].dFactor[0] = 1.25;
		m_CurrentData.RateLoad[5].dFactor[0] = 1.25;
		m_CurrentData.RateLoad[6].dFactor[0] = 1.0;
	}

	m_pMainGrid->SetRtldData(m_CurrentData.RateLoad);	
}

void CRatingCaseDlg::OnDgnDefineRatingCaseLiveFactorsBtn() 
{
	// TODO: Add your control notification handler code here
	RatingLiveFactorDlg dlg;

	dlg.m_iLimitState = m_Ctrl_LimitState.GetCheck() ? 0 : 1;
	dlg.m_nRatingType = 1;
	
	if(dlg.DoModal() == IDOK)
	{
		m_PrimaryFactor = dlg.m_dPrimary;
		m_AdjacentFactor = dlg.m_dAdjacent;
		
		UpdateData(FALSE);
	}
	
}
