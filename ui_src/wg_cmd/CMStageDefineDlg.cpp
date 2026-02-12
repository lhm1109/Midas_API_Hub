// CMStageDefineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageDefineDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"


#include "CMStageUDefStepDlg.h"

using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMStageDefineDlg dialog

#define CDialog CInternationalDlg

CCMStageDefineDlg::CCMStageDefineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMStageDefineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMStageDefineDlg)
	m_bTStep = TRUE;
	//m_strDuration = _T("0");
	m_strStageName = _T("");
	m_strSuffix = _T("");
	m_nStepOption = 1;   // Only user defined ....
	m_bSaveStage = FALSE;
	m_bSaveStep = FALSE;
	m_strDays = _T("");
	//}}AFX_DATA_INIT
}


void CCMStageDefineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStageDefineDlg)
	DDX_Control(pDX, IDC_CMD_DAY_LIST, m_wndDaysList);
	DDX_Control(pDX, IDC_CMD_NUMBER_OF_TIME_EDIT, m_wndTStep);
	//DDX_Text(pDX, IDC_CMD_DURATION_EDIT, m_strDuration);
	DDX_Control(pDX, IDC_CMD_DURATION_EDIT, m_wndNumberOfTime);
	DDX_Text(pDX, IDC_CMD_NAME_EDIT, m_strStageName);
	DDX_Text(pDX, IDC_CMD_SUFFIX_EDIT, m_strSuffix);
	DDX_Check(pDX, IDC_CMD_STAGE_CHECK, m_bSaveStage);
	DDX_Check(pDX, IDC_CMD_STEP_CHECK, m_bSaveStep);
	DDX_Text(pDX, IDC_CMD_DAYS_EDIT, m_strDays);
	//}}AFX_DATA_MAP
}


void CCMStageDefineDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMStageDefineDlg::UpdateBuffer()
{
	/*
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsBngrCommand = FALSE;	
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		 
		case(UR_BNGR_ADD):
		case(UR_BNGR_DEL):
		case(UR_BNGR_MFD): 
		case(UR_BNGR_MFS):
					bIsBngrCommand = TRUE;
				break;
		default:
			break;
		}
	} 
	if(bIsBngrCommand) 
		UpdateBngrList();
	*/
}

BEGIN_MESSAGE_MAP(CCMStageDefineDlg, CDialog)
	//{{AFX_MSG_MAP(CCMStageDefineDlg)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOk)
	ON_BN_CLICKED(IDC_CMD_CANCEL, OnCmdCancel)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_STEP_SPIN, OnDeltaposTstepSpin)
	ON_BN_CLICKED(IDC_CMD_STAGE_CHECK, OnCmdStageCheck)
	ON_BN_CLICKED(IDC_CMD_ADD_BUTTON, OnCmdAddButton)
	ON_BN_CLICKED(IDC_CMD_DEL_BUTTON, OnCmdDelButton)
	ON_BN_CLICKED(IDC_CMD_GEN_STEP_BUTTON, OnCmdGenStepButton)
	ON_BN_CLICKED(IDC_CMD_ASTEP_CLEAR_BUTTON, OnCmdAstepClearButton)
	ON_BN_CLICKED(IDC_CMD_ASTEP_MOD_BUTTON, OnCmdAstepModButton)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_DUR_SPIN, OnDeltaposCmdDurSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMStageDefineDlg message handlers

BOOL CCMStageDefineDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_wndTStep.SetRange(0, SHRT_MAX);
	m_wndTStep.SetValue(0);
	m_wndTStep.SetInteger(TRUE);
	m_wndNumberOfTime.SetRange(0, SHRT_MAX);
	m_wndNumberOfTime.SetValue(0);
	m_wndNumberOfTime.SetInteger(TRUE);

	StepOptionMan();
	SaveOptionMan();
	SetListHeader();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMStageDefineDlg::SetListHeader()
{
	CStringArray HTitles;
	CArray<float, float> HRatio;
	HTitles.Add(_LS(IDS_CMD0417__Step)); HTitles.Add(_LS(IDS_CMD0417__Day));
	HRatio.Add(0.5f)   ; HRatio.Add(0.5f);
	CDlgUtil::_SetListCtrlHeader(&m_wndDaysList,HTitles,&HRatio,NULL);
}

BOOL ___bIsError = FALSE;

void CCMStageDefineDlg::OnCmdApply() 
{
	___bIsError = FALSE;
	UpdateData(TRUE);
	if(m_strSuffix.IsEmpty())
	{
		T_STAG_D StagD;
		if (!GetStagData(StagD))
		{
			___bIsError = TRUE;
			return;
		}
		if (!CDBDoc::GetDocPoint()->m_pDataCtrl->AddStag(StagD))
		{
			___bIsError = TRUE;
			return;
		}
	}
	else
	{
		if (!GenBysuffixButton())
		{ 
			___bIsError = TRUE;
			return;
		}
	}
}

void CCMStageDefineDlg::OnCmdOk() 
{
	OnCmdApply();
	if(!___bIsError)
		OnOK();
}

void CCMStageDefineDlg::OnCmdCancel() 
{
	OnCancel();
}



LRESULT CCMStageDefineDlg::OnNcHitTest(CPoint point) 
{
	UINT nHitTest = CDialog::OnNcHitTest( point );
	return (nHitTest == HTCLIENT) ? HTCAPTION : nHitTest;
}

BOOL CCMStageDefineDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(nHitTest == HTCAPTION )
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL)); 
		return TRUE;
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);	
}

BOOL CCMStageDefineDlg::GetStagData(T_STAG_D& rData)
{
	
	rData.Initialize();
	rData.StageName = m_strStageName;

	BOOL bError = FALSE;

	//if (!CStrParser::GetFloatNumber(m_strDuration, rData.dDuration)) bError = TRUE;
	rData.dDuration = m_wndNumberOfTime.GetEditValue();
	/*
	if(m_nStepOption == 0)
	{
		if (!CStrParser::GetINumber(m_strNumberOfTime, rData.nTimeStepNum)) bError = TRUE;
	}
	else
	{
	}
	*/
	rData.aUserDefStep.Copy(m_UserDefSteps);

	//rData.nStepOption = m_nStepOption + 1 ;
	
	rData.bSaveResultStage = m_bSaveStage;
	rData.bSaveResultStep  = m_bSaveStep ;
	
	if (bError) AfxMessageBox(_LS(IDS_CMD_INPUT_VALUE_ERROR));
	return !bError;
}

BOOL CCMStageDefineDlg::GenBysuffixButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CArray<int,int> SuffixList;
	CMap<int,int,int,int> SuffixMap;
	SuffixMap.InitHashTable(500);
	CStringArray    StageNameArray;
	int TempInt;
	T_STAG_D StagD;
	CArray<T_STAG_D,T_STAG_D&> StagDs;

	if (!GetStagData(StagD)) return FALSE;
	if(CStrParser::ParsingListByTo(m_strSuffix,SuffixList))
	{
		CString TempStr;
		int nSuffix = SuffixList.GetSize();
		for(int i = 0 ; i < nSuffix ; i++)
		{
			if(!SuffixMap.Lookup(i,TempInt))
			{
				SuffixMap.SetAt(i,i);
				TempStr.Format(_T("%d"),SuffixList[i]);
				TempStr = m_strStageName + TempStr;
				StagD.StageName = TempStr;
				StagDs.Add(StagD);
			}
		}
		return CDBDoc::GetDocPoint()->m_pDataCtrl->AddStag(StagDs);
	}
	else AfxMessageBox(_LS(IDS_CMD_BY_TO_SYNTAX_ERROR));
	return FALSE;
}


void CCMStageDefineDlg::OnDeltaposTstepSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	CString csDiv;
	int nDiv;

	if (!GetNumOfDiv(&m_wndTStep, nDiv)) { *pResult = 0; return; }
	
	nDiv -= pNMUpDown->iDelta;
	
	if (nDiv < 1) nDiv = 1;
	
	m_wndTStep.SetValue(nDiv);
	
	*pResult = 0;
}

BOOL CCMStageDefineDlg::GetNumOfDiv(CEdit* pEdit, int &nDiv)
{
	CString csDiv;
	pEdit->GetWindowText(csDiv);
	return CStrParser::GetINumber(csDiv, nDiv);
}

void CCMStageDefineDlg::StepOptionMan()
{
	/*
	CArray<UINT, UINT> Ctrls1;
	CArray<UINT, UINT> Ctrls2;

	Ctrls1.Add(IDC_CMD_NUMBER_OF_TIME_EDIT);
	Ctrls1.Add(IDC_TSTEP_SPIN);
	Ctrls2.Add(IDC_CMD_USERDEF_BUTTON);

	if(m_nStepOption == 0)
	{
		CDlgUtil::CtrlEnableDisable(this, Ctrls1, TRUE);
		CDlgUtil::CtrlEnableDisable(this, Ctrls2, FALSE);
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this, Ctrls1, FALSE);
		CDlgUtil::CtrlEnableDisable(this, Ctrls2, TRUE);
	}
	*/
}

/*
void CCMStageDefineDlg::OnStepOption() 
{
	UpdateData(TRUE);
	StepOptionMan();
}
*/

void CCMStageDefineDlg::SaveOptionMan()
{
	CArray<UINT, UINT> Ctrls;
	Ctrls.Add(IDC_CMD_STEP_CHECK);
	if(m_bSaveStage)
	{
		CDlgUtil::CtrlEnableDisable(this, Ctrls, TRUE);
	}
	else
	{
		m_bSaveStep = FALSE;
		CDlgUtil::CtrlEnableDisable(this, Ctrls, FALSE);
		UpdateData(FALSE);
	}
}

void CCMStageDefineDlg::OnCmdStageCheck() 
{
	UpdateData(TRUE);	
	SaveOptionMan();
}

/*
void CCMStageDefineDlg::OnCmdUserdefButton() 
{
	CCMStageUDefStepDlg Dlg;
	Dlg.SetSteps(&m_UserDefSteps);
	Dlg.DoModal();
}
*/

void CCMStageDefineDlg::OnCmdAddButton() 
{
	UpdateData(TRUE);

	CArray<int, int> Days;
	CMap<int, int,int, int> DayMap;
	int   Day;

	int Duration = m_wndNumberOfTime.GetEditValueInt();
	//if(!CStrParser::GetINumber(m_strDuration,Duration)) return;
	if(CStrParser::GetNodeList(m_strDays,Days))
	{
		if(Days.GetSize())
		{
			int nItem = m_wndDaysList.GetItemCount();
			for( int i = 0 ; i < nItem ; i++)
			{
				Day = (int)m_wndDaysList.GetItemData(i);
				DayMap.SetAt(Day,Day);
			}

			nItem = Days.GetSize();
			for(int i = 0 ; i < nItem ; i++)
			{
				if(Days[i] != 0)
				{
					if(Duration != 0)
					{
						if(Duration > Days[i]) 
						{
							DayMap.SetAt(Days[i], Days[i]);
						}
					}
					else if(Duration == 0)
						DayMap.SetAt(Days[i], Days[i]);
				}
			}

			Days.RemoveAll();

			POSITION Pos = DayMap.GetStartPosition();
			int Key, Data;
			while(Pos)
			{
				DayMap.GetNextAssoc(Pos,Key, Data);
				Days.Add(Key);
			}

			qsort(Days.GetData(),Days.GetSize(),sizeof(int),CCompFunc::intAsc);

			m_wndDaysList.DeleteAllItems();

			nItem = Days.GetSize();

			CStringArray Contents;
			CString      TStr;

			for(int i = 0 ; i < nItem ; i++)
			{
				Contents.RemoveAll();
				TStr.Format(_T("%d"), i+1);
				Contents.Add(TStr);
				TStr.Format(_T("%d"), Days[i]);
				Contents.Add(TStr);
				CDlgUtil::SetListItem(&m_wndDaysList,i,Contents,Days[i]);
			}
		}
		SaveData();
	}
	else
	{
		// Parameter Error!!!
	}
}

void CCMStageDefineDlg::OnCmdGenStepButton() 
{
	UpdateData();

	CArray<int, int>Days   ;  
	CArray<int, int>LogDays;
	CMap<int, int,int, int> DayMap;
	DayMap.InitHashTable(50);
 //m_UserDefSteps.Add(nTemp);

	int nItems = m_wndDaysList.GetItemCount();
	int Day;
	for(int i = 0; i < nItems; i++)
	{
		Day = (int)m_wndDaysList.GetItemData(i);
		DayMap.SetAt(Day,Day);
	}

	// Log Scale Day생성....
	int nCount,nDuration,nDay;
	if(m_wndNumberOfTime.GetEditValue(nCount))
	{
		if(m_wndNumberOfTime.GetEditValue(nDuration))
		//if(CStrParser::GetINumber(m_strDuration,nDuration))
		{
			if (nCount > 0)
			{
				double p = log10(nDuration)/double(nCount+1);
				double d, t;
				for (int i = 0; i < nCount; i++)
				{
					d = pow(10.0, p*(i+1));
					t = floor(d);
					if (d - t >= 0.5) d = ceil(d);
					nDay = (int)d;

					DayMap.SetAt(nDay,nDay);
				}
			}
		}
		else
		{
			return;
		}
	}
	else
	{ 
		return;
	}
	 
		
	POSITION Pos = DayMap.GetStartPosition();
	int Key,Data;
	while(Pos)
	{
		DayMap.GetNextAssoc(Pos,Key, Data);
		Days.Add(Key);
	}

	if(Days.GetSize())
	{
		qsort(Days.GetData(),Days.GetSize(),sizeof(int),CCompFunc::intAsc);

		m_wndDaysList.DeleteAllItems();

		int nItem = Days.GetSize();

		CStringArray Contents;
		CString      TStr;

		for(int  i = 0 ; i < nItem ; i++)
		{
			if(Days[i] != 0 && Days[i] < nDuration)
			{
				Contents.RemoveAll();
				TStr.Format(_T("%d"), i+1);
				Contents.Add(TStr);
				TStr.Format(_T("%d"), Days[i]);
				Contents.Add(TStr);
				CDlgUtil::SetListItem(&m_wndDaysList,i,Contents,Days[i]);
			}
		}
	}
	SaveData();
}


void CCMStageDefineDlg::OnCmdDelButton() 
{
	DeleteCurSelItems();
	SaveData();
}

void CCMStageDefineDlg::DeleteCurSelItems()
{
	CMap<int, int, int, int> ItemMap;
	ItemMap.InitHashTable(50);
	int nItems = m_wndDaysList.GetItemCount();
	if(!nItems) return;
	int nTemp;
	for(int i = 0; i < nItems; i++)
	{
		nTemp = m_wndDaysList.GetItemData(i);
		ItemMap.SetAt(nTemp,nTemp);
	}

	CArray<int, int> SelItems;
	CDlgUtil::GetListItemByMask(&m_wndDaysList,SelItems,LVIS_SELECTED);

	nItems = SelItems.GetSize();
	if(nItems == 0) return;
	int nT;
	for(int  i = 0; i < nItems ;i ++)
	{
		nTemp = m_wndDaysList.GetItemData(SelItems[i]);
		ItemMap.RemoveKey(nTemp);
	}

	m_wndDaysList.DeleteAllItems();

	POSITION Pos = ItemMap.GetStartPosition();
	int nI = 0 ;
	CStringArray Contents;
	CString TStr;
	SelItems.RemoveAll();
	while(Pos)
	{
		ItemMap.GetNextAssoc(Pos,nTemp,nT);
		SelItems.Add(nTemp);
	}
	qsort(SelItems.GetData(),SelItems.GetSize(),sizeof(int),CCompFunc::intAsc);

	nItems = SelItems.GetSize();
	for(int i = 0; i < nItems; i++)
	{
		Contents.RemoveAll();
		TStr.Format(_T("%d"),i+1);
		Contents.Add(TStr);
		TStr.Format(_T("%d"),SelItems[i]);
		Contents.Add(TStr);
		CDlgUtil::SetListItem(&m_wndDaysList,i,Contents,SelItems[i]);
	}
}

void CCMStageDefineDlg::SaveData()
{
	m_UserDefSteps.RemoveAll();
	int nI = m_wndDaysList.GetItemCount();
	int nTemp;
	for( int i = 0 ; i < nI ; i++)
	{
		nTemp = m_wndDaysList.GetItemData(i);
		m_UserDefSteps.Add(nTemp);
	}
}

/*
struct T_STAG_STEP_INFO_BASE
{
	int nDay;
	int nStepType;  // 0:Time Step, 1:하중에 의한 Step
	CArray<T_LDGR_K, T_LDGR_K> aLdgrKey;
	T_STAG_STEP_INFO_BASE() {}
	T_STAG_STEP_INFO_BASE(const T_STAG_STEP_INFO_BASE& src) { *this = src; }
	T_STAG_STEP_INFO_BASE& operator=(const T_STAG_STEP_INFO_BASE& src)
	{
		nDay = src.nDay;
		nStepType = src.nStepType;
		aLdgrKey.Copy(src.aLdgrKey);
		return *this;
	}
	void Initialize()
	{
		nDay = 0;
		nStepType = 0;
		aLdgrKey.RemoveAll();
	}
};

struct T_STAG_STEP_INFO
{
	T_STAG_K StagKey;
	CArray<T_STAG_STEP_INFO_BASE, T_STAG_STEP_INFO_BASE&> aInfo;
	T_STAG_STEP_INFO(){}
	T_STAG_STEP_INFO(const T_STAG_STEP_INFO& src) { *this = src; }
	T_STAG_STEP_INFO& operator=(const T_STAG_STEP_INFO& src)
	{
		StagKey = src.StagKey;
		aInfo.Copy(src.aInfo);
		return *this;
	}
	
	void Initialize()
	{
		StagKey = 0;
		aInfo.RemoveAll();
	}
};

BOOL CAttrCtrl::GetStagStepInfo(T_STAG_K Key, T_STAG_STEP_INFO& rStepInfo)
{
	T_STAG_D Stag;
	if (!GetStag(Key, Stag)) return FALSE;

	rStepInfo.Initialize();
	T_STAG_STEP_INFO_BASE sinfo;
	CMap<int, int, T_STAG_STEP_INFO_BASE, T_STAG_STEP_INFO_BASE&> map;

	// time step
	if (Stag.nStepOption == 1)  // Auto Generate
	{
		int nCount = Stag.nTimeStepNum;
		if (nCount > 0)
		{
			sinfo.Initialize();
			sinfo.nDay = 0;
			sinfo.nStepType = 0;
			map.SetAt(sinfo.nDay, sinfo);
			double p = log10((double)Stag.nDuration)/double(nCount);
			double d, t;
			for (int i = 1; i < nCount; i++)
			{
				d = pow(10.0, p*i);
				t = floor(d);
				if (d - t >= 0.5) d = ceil(d);
				sinfo.nDay = (int)d;
				map.SetAt(sinfo.nDay, sinfo);
			}
		}
	}
	else  // User Define
	{
		int nCount = Stag.aUserDefStep.GetSize();
		sinfo.Initialize();
		sinfo.nStepType = 0;
		for (int i = 0; i < nCount; i++)
		{
			sinfo.nDay = Stag.aUserDefStep[i];
			map.SetAt(sinfo.nDay, sinfo);
		}
	}
	sinfo.nDay = Stag.nDuration;
	sinfo.nStepType = 0;
	map.SetAt(sinfo.nDay, sinfo);

	// load step
	int nLoad = Stag.aActiveLoad.GetSize();
	for (int i = 0; i < nLoad; i++)
	{
		sinfo.nDay = Stag.aActiveLoad[i].nRetardation;
		if (!map.Lookup(sinfo.nDay, sinfo)) 
		{
			sinfo.nStepType = 1;
			sinfo.aLdgrKey.RemoveAll();
		}
		sinfo.aLdgrKey.Add(Stag.aActiveLoad[i].LdgrKey);
		map.SetAt(sinfo.nDay, sinfo);
	}

	int nStep = map.GetCount();
	CArray<int, int> aDay;
	aDay.SetSize(nStep);
	POSITION pos = map.GetStartPosition();
	i = 0;
	while (pos)
	{
		map.GetNextAssoc(pos, aDay[i++], sinfo);
	}
	qsort(aDay.GetData(), nStep, sizeof(int), CCompFunc::intAsc);
	rStepInfo.aInfo.SetSize(nStep);
	rStepInfo.StagKey = Key;
	for (i = 0; i < nStep; i++)
	{
		map.Lookup(aDay[i], rStepInfo.aInfo[i]);
	}

	return TRUE;
}
*/


void CCMStageDefineDlg::OnCmdAstepClearButton() 
{
	m_wndDaysList.DeleteAllItems();
	SaveData();
}

void CCMStageDefineDlg::OnCmdAstepModButton() 
{
	UpdateData(TRUE);
 
	CArray<int,int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndDaysList,arRet,LVIS_SELECTED);
	if(arRet.GetSize() > 1) return;

	CArray<int,int> Days;
	if(!CStrParser::GetNodeList(m_strDays,Days)) return;
	if(Days.GetSize() > 1) return;

	int Duration = m_wndNumberOfTime.GetEditValueInt();
	//if(!CStrParser::GetINumber(m_strDuration,Duration)) return;

	if(Days[0] > Duration) return;

	CStringArray Contents;
	CString StrTemp;

	CMap<int, int ,int,int> DayMap;
	
	int nDay = m_wndDaysList.GetItemCount();
	
	for( int i = 0 ; i < nDay ; i++)
	{
		DayMap.SetAt((int)m_wndDaysList.GetItemData(i),(int)m_wndDaysList.GetItemData(i));
	}
	
	int TDay;
	if(DayMap.Lookup(Days[0],TDay)) return;
	
	m_wndDaysList.DeleteItem(arRet[0]);
	SaveData();
	
	OnCmdAddButton();
}

void CCMStageDefineDlg::OnDeltaposCmdDurSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDlgUtil::SetSpinValue(this,IDC_CMD_DURATION_EDIT,0,-1,pNMHDR,pResult);
}
