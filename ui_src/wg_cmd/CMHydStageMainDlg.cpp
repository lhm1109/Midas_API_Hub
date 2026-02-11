// CMHydStageMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"

#include "CMHydStageMainDlg.h"
#include "CMHydStageElemDlg.h"
#include "CMHydStageBndrDlg.h"
#include "CMHydStageLoadDlg.h"
#include "CMHydStageInfoDlg.h"
#include "CMHydStageItemBase.h"
#include "LCUTIL.h"  

using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CChildDialogMove
/////////////////////////////////////////////////////////////////////////////
// CCMHydStageMainDlg dialog


CCMHydStageMainDlg::CCMHydStageMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMHydStageMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMHydStageMainDlg)
	m_strTimes = _T("");
	m_strStageName = _T("");
	//}}AFX_DATA_INIT
	m_nAddOption = 0;
	m_pStageData = new T_HSTG_D;
	m_CurStageK = 0;
}


void CCMHydStageMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMHydStageMainDlg)
	DDX_Control(pDX, IDC_CMD_DAY_LIST, m_wndTimeList);
	DDX_Control(pDX, IDC_CMD_TAB, m_wndTab);
	DDX_Control(pDX, IDC_CMD_INIT_TEMP_EDIT, m_wndInitTempEdit);
	DDX_Control(pDX, IDC_CMD_DURATION_EDIT, m_edtDuration);
	DDX_Control(pDX, IDC_CMD_TIME_STEP_EDIT, m_edtTStep);
	DDX_Text(pDX, IDC_CMD_DAYS_EDIT, m_strTimes);
	DDX_Text(pDX, IDC_CMD_NAME_EDIT, m_strStageName);
	DDX_Control(pDX, IDC_CMD_INIT_TEMP_CHK, m_bInitTemperature);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMHydStageMainDlg, CDialog)
	//{{AFX_MSG_MAP(CCMHydStageMainDlg)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_TIME_STEP_SPIN, OnDeltaposCmdTimeStepSpin)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_DURATION_SPIN, OnDeltaposCmdDurationSpin)
	ON_BN_CLICKED(IDC_CMD_OK_BUTTON, OnCmdOkButton)
	ON_BN_CLICKED(IDC_CMD_CANCEL_BUTTON, OnCmdCancelButton)
	ON_BN_CLICKED(IDC_CMD_APPLY_BUTTON, OnCmdApplyButton)
	ON_BN_CLICKED(IDC_CMD_ADD_BUTTON, OnCmdAddButton)
	ON_BN_CLICKED(IDC_CMD_DEL_BUTTON, OnCmdDelButton)
	ON_BN_CLICKED(IDC_CMD_GEN_STEP_BUTTON, OnCmdGenStepButton)
	ON_BN_CLICKED(IDC_CMD_ASTEP_CLEAR_BUTTON, OnCmdClearButton)
	ON_BN_CLICKED(IDC_CMD_ASTEP_MOD_BUTTON, OnCmdModifyButton)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TAB, OnSelchangeCmdTab)
	ON_BN_CLICKED(IDC_STAGE_INFO_BUTTON, OnStageInfoButton)
	ON_BN_CLICKED(IDC_CMD_INIT_TEMP_CHK, OnCmdInitTemperature)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMHydStageMainDlg message handlers

CCMHydStageMainDlg::~CCMHydStageMainDlg()
{
	delete m_pStageData;
}

BOOL CCMHydStageMainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_edtDuration.SetRange(0, SHRT_MAX);
	m_edtDuration.SetValue(0);
	m_edtDuration.SetUnitType(D_UNITSYS_BASE_TIME);

	m_edtTStep.SetRange(0, SHRT_MAX);
	m_edtTStep.SetValue(1);
	m_edtTStep.SetInteger(TRUE);

	SetChildDlgs();

	if(m_CurStageK == 0)
	{
		m_pStageData->Initialize();
	}
	else
	{
		if(m_nAddOption == 0)
		 CDBDoc::GetDocPoint()->m_pAttrCtrl->GetHstg(m_CurStageK, *m_pStageData);
		else
			m_pStageData->Initialize();
	}

	UpdateCurStageData();
	
	UpdateData(FALSE);

	//SetCtrlFocus()
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMHydStageMainDlg::UpdateCurStageData()
{
	m_strStageName = m_pStageData->StageName;
	
	m_bInitTemperature.SetCheck(m_pStageData->bInitTemperature);
	m_wndInitTempEdit.SetEditUnit(m_pStageData->dInitTemperature);
	//m_strDuration.Format(_T("%.5g"),m_pStageData->d
	
	CLCUtil::ClearAllListItemDbl(&m_wndTimeList);

	int nItem = m_pStageData->aStep.GetSize();

	CStringArray Contents;
	CString      TStr;

	for(int i = 0 ; i < nItem ; i++)
	{
		Contents.RemoveAll();
		TStr.Format(_T("%d"), i+1);
		Contents.Add(TStr);
		TStr.Format(_T("%g"), m_pStageData->aStep[i]);
		Contents.Add(TStr);
		CLCUtil::SetListItemDbl(&m_wndTimeList,i,Contents,m_pStageData->aStep[i]);
	}
		
	CCMHydStageItemBase* pChild = (CCMHydStageItemBase*) m_wndTab.GetTab(m_wndTab.GetCurSel());
	if(pChild)
	{
		pChild->SetCurStageData(m_pStageData);
		pChild->ResetParam();
	}
	EnableDisableCtrl();
}

BOOL CCMHydStageMainDlg::SetChildDlgs()
{
	CStringArray HTitles;
	CArray<float, float> HRatio;
	HTitles.Add(_LS(IDS_CMD0417__Step)); HTitles.Add(_LS(IDS_CMD0417__Time_hr_));
	HRatio.Add(0.5f)   ; HRatio.Add(0.5f);
	CDlgUtil::_SetListCtrlHeader(&m_wndTimeList,HTitles,&HRatio,NULL);
	
	CCMHydStageItemBase* pDlg;

	CWnd* pPlaceHolder = GetDlgItem(IDC_CMD_PLACEHOLDER);
	
	m_wndTab.SetPlaceHolder(pPlaceHolder);
	
	pDlg = new CCMHydStageElemDlg();
	m_wndTab.AddTab(pDlg, _LS(IDS_CMD0417__Element) , CCMHydStageElemDlg::IDD  , TRUE);
	pDlg = new CCMHydStageBndrDlg();
	m_wndTab.AddTab(pDlg, _LS(IDS_CMD0417__Boundary), CCMHydStageBndrDlg::IDD  , TRUE);
	pDlg = new CCMHydStageLoadDlg();
	m_wndTab.AddTab(pDlg, _LS(IDS_CMD0417__Load), CCMHydStageLoadDlg::IDD  , TRUE);
	
	m_wndTab.ShowTab(0);

	return TRUE;
}

void CCMHydStageMainDlg::SetCurrentStage(T_HSTG_K CurStageKey,int nOption/* = 0*/) // Default Add(0)
{
	m_CurStageK    = CurStageKey ;
	m_nAddOption   = nOption     ;
}

void CCMHydStageMainDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMHydStageMainDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsStagCommand = FALSE;	
	BOOL bIsGrupCommand = FALSE;
	BOOL bIsBngrCommand = FALSE;

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_HSTG_ADD):
		case(UR_HSTG_DEL):
				bIsStagCommand = TRUE;
				break;
		case(UR_GRUP_ADD):
		case(UR_GRUP_DEL):
		case(UR_GRUP_MFD):
		case(UR_GRUP_MFS):
				bIsGrupCommand = TRUE;
				break;
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

	if(bIsStagCommand)
	{
		T_HSTG_K __StageKey = m_CurStageK;
			
		if(__StageKey)
		{
			if(bIsStagCommand)
			{
				if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetHstg(__StageKey, *m_pStageData))
					OnCancel();
				CCMHydStageItemBase * pChild = (CCMHydStageItemBase*) m_wndTab.GetTab(m_wndTab.GetCurSel());
				if(pChild)
					pChild->SetCurStageData(m_pStageData);
			}
			else 
			{
				
				int nWhat = m_wndTab.GetCurSel();
				if((bIsGrupCommand && nWhat == 0) ||
					 (bIsBngrCommand && nWhat == 1) )
					 
				{
					CDBDoc::GetDocPoint()->m_pAttrCtrl->GetHstg(__StageKey, * m_pStageData);
					CCMHydStageItemBase * pChild = (CCMHydStageItemBase*) m_wndTab.GetTab(m_wndTab.GetCurSel());
					if(pChild)
						pChild->SetCurStageData(m_pStageData);
				}
				
			}
		}
	}
	else
	{
		
		int nWhat = m_wndTab.GetCurSel();
		if((bIsGrupCommand && nWhat == 0) ||
			 (bIsBngrCommand && nWhat == 1)  )
		{
			
			CCMHydStageItemBase * pChild = (CCMHydStageItemBase*) m_wndTab.GetTab(m_wndTab.GetCurSel());
			if(pChild)
				pChild->SetCurStageData(m_pStageData);
		}
	}
}

/*
Conn1.CreateInstance( __uuidof( Connection ) ); // Instantiating connection object
	  Conn1->ConnectionString = bstrSQLServerConnect; // giving the sqlconnection
	  Conn1->Open( bstrEmpty, bstrEmpty, bstrEmpty ); // open the connection object
	  Cmd1.CreateInstance( __uuidof( Command ) ); // creating command object
	  Cmd1->ActiveConnection = Conn1;             // giving the connection handle
	  Cmd1->CommandText      = _bstr_t( bstrProc ); // passing the stored procedue
	  Cmd1->CommandType      = adCmdStoredProc;     // type
	  Cmd1->Parameters->Refresh();                 // passing string value as argument to stored procedure
	  Cmd1->Parameters->Item[ _variant_t( (long) 1 ) ]->Value = _variant_t( (LPCTSTR)m_sfilename );
	  Rs1 = Cmd1->Execute( &vtEmpty, &vtEmpty2, adCmdUnknown ); // executing the stored procedure and storing the recordset value
	  bvalid = true;
	  Final  = Rs1->Fields->GetItem( _variant_t( 0L ) )->Value; // getting the first column value of the result row
	  strTmp.Format( _T("%s"), CrackStrVariant( Final) ); // to see the value
	  // put your code to see all column 
*/

void CCMHydStageMainDlg::OnDeltaposCmdTimeStepSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDlgUtil::SetSpinValue(this,IDC_CMD_TIME_STEP_EDIT,0,-1,pNMHDR,pResult);
}

void CCMHydStageMainDlg::OnDeltaposCmdDurationSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDlgUtil::SetFloatSpinValue(this,IDC_CMD_DURATION_EDIT,_T("0."),_T("UDM"),1,pNMHDR,pResult);
}

void CCMHydStageMainDlg::SaveAdditionalStep()
{
	m_pStageData->aStep.RemoveAll();
		
	int nI = m_wndTimeList.GetItemCount();
	double dTime;
	for( int i = 0 ; i < nI ; i++)
	{
		dTime =*(double*)m_wndTimeList.GetItemData(i);
		m_pStageData->aStep.Add(dTime);
	}
	
	CCMHydStageItemBase * pChild = (CCMHydStageItemBase*) m_wndTab.GetTab(m_wndTab.GetCurSel());
	if(pChild)
		pChild->SetCurStageData(m_pStageData);  
}

void CCMHydStageMainDlg::OnCmdAddButton() 
{
	// TODO: Add your control notification handler code here
	//this->m_wndTimeList;

	UpdateData(TRUE);
	
	CMap<double,double,double,double> TimeMap;
	TimeMap.InitHashTable(50);
	double   dTime;
	CArray<double,double> Times;
	double Duration;
	if(!m_edtDuration.GetEditValue(Duration)) return;
	if(CStrParser::GetUEDistance(m_strTimes,Times))
	{
		if(Times.GetSize())
		{
			int nItem = m_wndTimeList.GetItemCount();
			for( int i = 0 ; i < nItem ; i++)
			{
				dTime = *(double*)m_wndTimeList.GetItemData(i);
				TimeMap.SetAt(dTime,dTime);
			}

			nItem = Times.GetSize();
			for(int i = 0 ; i < nItem ; i++)
			{
				if(Times[i] != 0)
				{
					/*
					if(Duration != 0)
					{
						if(Duration > Times[i]) 
						{
							TimeMap.SetAt(Times[i], Times[i]);
						}
					}
					else if(Duration == 0)
					*/
					{
						TimeMap.SetAt(Times[i], Times[i]);
					}
				}
			}

			CArray<double , double> RTimes;

			POSITION Pos = TimeMap.GetStartPosition();
			double dKey;
			double dData;
			while(Pos)
			{
				TimeMap.GetNextAssoc(Pos,dKey,dData);
				RTimes.Add(dData);
			}

			qsort(RTimes.GetData(),RTimes.GetSize(),sizeof(double),CCompFunc::doubleAsc);
			
			CLCUtil::ClearAllListItemDbl(&m_wndTimeList);

			nItem = RTimes.GetSize();

			CStringArray Contents;
			CString      TStr;

			for(int  i = 0 ; i < nItem ; i++)
			{
				Contents.RemoveAll();
				TStr.Format(_T("%d"), i+1);
				Contents.Add(TStr);
				TStr.Format(_T("%.8g"), RTimes[i]);
				Contents.Add(TStr);
				CLCUtil::SetListItemDbl(&m_wndTimeList,i,Contents,RTimes[i]);
			}
		}
		SaveAdditionalStep();
	}
	else
	{
		// Parameter Error!!!
	}
	
}

void CCMHydStageMainDlg::OnCmdModifyButton() 
{
	UpdateData(TRUE);
 
	CArray<int,int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndTimeList,arRet,LVIS_SELECTED);
	
	if(arRet.GetSize() == 1 )
	{
		CArray<double,double> Days;
		if(!CStrParser::GetUEDistance(m_strTimes,Days)) return;
		if(Days.GetSize() > 1) return;

		CMap<double , double,double,double> DayMap;
		DayMap.InitHashTable(50);
		
		int nDay = m_wndTimeList.GetItemCount();
		double  dDay;
		for( int i = 0 ; i < nDay ; i++)
		{ 
			dDay = *((double*)m_wndTimeList.GetItemData(i));
			DayMap.SetAt(dDay,dDay);
		}
		
		double rDay;
		if(DayMap.Lookup(Days[0],rDay)) return;
		
		//m_wndDayList.DeleteItem(arRet[0]);
		CLCUtil::DeleteListItemDbl(&m_wndTimeList,arRet[0]);
		SaveAdditionalStep();
		OnCmdAddButton() ;
	}
}

void CCMHydStageMainDlg::OnCmdDelButton() 
{
	CMap  <double , double,double , double> ItemMap;
	CArray<double , double> ArrDay ;
	ItemMap.InitHashTable(50);

	int nItems = m_wndTimeList.GetItemCount();
	if(!nItems) return;
	double dDay;
	
	for(int i = 0; i < nItems; i++)
	{
		dDay = *(double*)m_wndTimeList.GetItemData(i);
		ItemMap.SetAt(dDay,dDay);
	}

	CArray<int, int> SelItems;
	CDlgUtil::GetListItemByMask(&m_wndTimeList,SelItems,LVIS_SELECTED);

	nItems = SelItems.GetSize();
	
	if(nItems == 0) return;
	
	
	for(int  i = 0; i < nItems ;i ++)
	{
		dDay = *(double *)m_wndTimeList.GetItemData(SelItems[i]);
		ItemMap.RemoveKey(dDay);
	}

	CLCUtil::ClearAllListItemDbl(&m_wndTimeList);
		
	POSITION Pos = ItemMap.GetStartPosition();
	
	double rKey;
	double rData;
	
	CArray<double , double> ArrRDay;
	while(Pos)
	{
		ItemMap.GetNextAssoc(Pos,rKey,rData);
		ArrRDay.Add(rData);
	}
	qsort(ArrRDay.GetData(),ArrRDay.GetSize(),sizeof(double),CCompFunc::doubleAsc);

	nItems = ArrRDay.GetSize();
	CString TStr;
	CStringArray Contents;
	for(int  i = 0; i < nItems; i++)
	{
		Contents.RemoveAll();
		TStr.Format(_T("%d"),i+1);
		Contents.Add(TStr);
		TStr.Format(_T("%.8g"),ArrRDay[i]);
		Contents.Add(TStr);
		CLCUtil::SetListItemDbl(&m_wndTimeList,i,Contents,ArrRDay[i]);
	}
	SaveAdditionalStep();
	
}

void CCMHydStageMainDlg::OnCmdClearButton() 
{
	// TODO: Add your control notification handler code here
	CLCUtil::ClearAllListItemDbl(&m_wndTimeList); 
	SaveAdditionalStep();
}

void CCMHydStageMainDlg::OnCmdGenStepButton() 
{
	UpdateData();
	CMap<double,double,double,double> TimeMap;
	TimeMap.InitHashTable(50);
	
	int nItems = m_wndTimeList.GetItemCount();
	double dTime;
	for(int i = 0; i < nItems; i++)
	{
		dTime = *(double*)m_wndTimeList.GetItemData(i);
		TimeMap.SetAt(dTime,dTime);
	}

	// Log Scale Day생성....
	int nCount;
	double dDuration;
	if(m_edtTStep.GetEditValue(nCount))
	{
		if(m_edtDuration.GetEditValue(dDuration))
		{
			if (nCount > 0)
			{
				double p = log10(dDuration)/double(nCount+1);
				double d, t;
				for (int i = 0; i < nCount; i++)
				{
					d = pow(10.0, p*(i+1));
					t = floor(d);
					if (d - t >= 0.5) d = ceil(d);
					dTime = (int)d;
					TimeMap.SetAt(dTime,dTime);
				}
				// Duration은 항상 넣는다.
				if (dTime != dDuration) TimeMap.SetAt(dDuration, dDuration);
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
		
	CArray<double, double> RTimes;
	POSITION Pos = TimeMap.GetStartPosition();
	double rKey;
	double rData;
	while(Pos)
	{
		TimeMap.GetNextAssoc(Pos,rKey, rData);
		RTimes.Add(rData);
	}

	if(RTimes.GetSize())
	{
		qsort(RTimes.GetData(),RTimes.GetSize(),sizeof(double),CCompFunc::doubleAsc);

		//m_wndDayList.DeleteAllItems();
		CLCUtil::ClearAllListItemDbl(&m_wndTimeList); 

		int nItem = RTimes.GetSize();

		CStringArray Contents;
		CString      TStr;
		
		int Index = 0;
		for(int i = 0 ; i < nItem ; i++)
		{
			if(RTimes[i] != 0 && RTimes[i] <= dDuration)
			{
				Contents.RemoveAll();
				TStr.Format(_T("%d"), i+1);
				Contents.Add(TStr);
				TStr.Format(_T("%.8g"), RTimes[i]);
				Contents.Add(TStr);
				//CDlgUtil::SetListItem(&m_wndDayList,i,Contents,Days[i]);
				CLCUtil::SetListItemDbl(&m_wndTimeList,Index,Contents,RTimes[i]);
				Index++;
			}
		}
	}
	SaveAdditionalStep();
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

BOOL t__bIsError;
void CCMHydStageMainDlg::OnCmdApplyButton() 
{
	UpdateData(TRUE);

	t__bIsError = FALSE;

	if(m_strStageName.IsEmpty())
	{
		t__bIsError = TRUE;
		goto Error;
	}
	m_pStageData->StageName = this->m_strStageName;  
	m_pStageData->bInitTemperature = m_bInitTemperature.GetCheck();
	m_pStageData->dInitTemperature = m_wndInitTempEdit.GetEditValue();
	SaveAdditionalStep();

Error:
	if(!t__bIsError)
	{
		if(this->m_nAddOption  == 0)
		{
			if(this->m_CurStageK == 0) // ADD
			{
				t__bIsError = !CDBDoc::GetDocPoint()->m_pDataCtrl->AddHstg(*m_pStageData);

				if(!t__bIsError)
				{
					m_pStageData->aActiveBndr.RemoveAll();
					m_pStageData->aActiveElem.RemoveAll();
					m_pStageData->aDeactiveBndr.RemoveAll();

					UpdateCurStageData();
				}
			}
			else  // Modify
			{
				t__bIsError =  !CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyHstg(m_CurStageK,*m_pStageData);
			}
		}
		else if( this->m_nAddOption == 1) // Insert Next 
		{
			ASSERT(m_CurStageK);
			t__bIsError = !CDBDoc::GetDocPoint()->m_pDataCtrl->InsertHstg(m_CurStageK,TRUE,*m_pStageData);
			if(!t__bIsError)
			{
				m_pStageData->aActiveBndr.RemoveAll();
				m_pStageData->aActiveElem.RemoveAll();
				m_pStageData->aDeactiveBndr.RemoveAll();
				UpdateCurStageData();
			}
		}
		else if( this->m_nAddOption == 2) // Insert Prev
		{
			ASSERT(m_CurStageK);
			t__bIsError = !CDBDoc::GetDocPoint()->m_pDataCtrl->InsertHstg(m_CurStageK,FALSE,*m_pStageData);
			if(!t__bIsError)
			{
				m_pStageData->aActiveBndr.RemoveAll();
				m_pStageData->aActiveElem.RemoveAll();
				m_pStageData->aDeactiveBndr.RemoveAll();
				UpdateCurStageData();
			}
		}
		else
			ASSERT(FALSE);
	}
	else
	{
		MessageBox(_LS(IDS_CMD0417__Parameter_Error__),_LS(IDS_CMD0417__Error));
	}
}

void CCMHydStageMainDlg::OnCmdOkButton() 
{
	OnCmdApplyButton();
	if(!t__bIsError)
		OnOK();
}

void CCMHydStageMainDlg::OnCmdCancelButton() 
{
	OnCancel();
}


void CCMHydStageMainDlg::OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CCMHydStageItemBase* pChild = (CCMHydStageItemBase*) m_wndTab.GetTab(m_wndTab.GetCurSel());
	if(pChild)
		pChild->SetCurStageData(m_pStageData);	
	*pResult = 0;
}

void CCMHydStageMainDlg::OnStageInfoButton() 
{
	CCMHydStageInfoDlg Dlg;

	CArray<T_HSTG_K, T_HSTG_K> StagArray;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetHstgKeyList(StagArray);
	
	int nCurStagIndex;

	if(this->m_nAddOption  == 0)
	{
		if(this->m_CurStageK == 0) // ADD
		{
			nCurStagIndex = StagArray.GetSize()-1;
		}
		else  // Modify
		{
			nCurStagIndex= -1;
			int nSize = StagArray.GetSize();
			for(int i = 0;i < nSize; i++)
			{
				nCurStagIndex++; 
				if(m_CurStageK == StagArray[i]) break;
			}
		}
		Dlg.SetCurStageData(nCurStagIndex,*m_pStageData);
	}
	else if( this->m_nAddOption == 1) // Insert Next 
	{
		ASSERT(m_CurStageK);
		nCurStagIndex= -1;
		int nSize = StagArray.GetSize();
		if(nSize > 0)
		{
			for(int i = 0;i < nSize; i++)
			{
				nCurStagIndex++; 
				if(m_CurStageK == StagArray[i]) break;
			}
		}
		else
		{
			nCurStagIndex = 0;
		}

		Dlg.SetCurStageData(nCurStagIndex,*m_pStageData);
	}
	else if( this->m_nAddOption == 2) // Insert Prev
	{
		ASSERT(m_CurStageK);
		nCurStagIndex= -1;
		int nSize = StagArray.GetSize();
		if(nSize > 0)
		{
			for(int i = 0;i < nSize; i++)
			{
				nCurStagIndex++; 
				if(m_CurStageK == StagArray[i]) break;
			}
		}
		else
		{
			nCurStagIndex= 0;
		}
		Dlg.SetCurStageData(nCurStagIndex,*m_pStageData);
	}
	Dlg.DoModal();
}

void CCMHydStageMainDlg::OnCmdInitTemperature()
{
	EnableDisableCtrl();
}

void CCMHydStageMainDlg::EnableDisableCtrl()
{
	if(m_bInitTemperature.GetCheck()) GetDlgItem(IDC_CMD_INIT_TEMP_EDIT)->EnableWindow(TRUE);
	else                              GetDlgItem(IDC_CMD_INIT_TEMP_EDIT)->EnableWindow(FALSE);
}

BOOL CCMHydStageMainDlg::DestroyWindow() 
{
	CLCUtil::ClearAllListItemDbl(&m_wndTimeList);
	return CDialog::DestroyWindow();
}

