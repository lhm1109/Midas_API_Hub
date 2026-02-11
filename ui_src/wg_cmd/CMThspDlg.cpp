// CMThspDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThspDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_StrParser.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_base\wg_base_CompFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMThspDlg dialog
#define CDialog CCMChildBarBase

CCMThspDlg::CCMThspDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMThspDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMThspDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMThspDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThspDlg)
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR_EDIT, m_wndScaleFactor);
	DDX_Control(pDX, IDC_CMD_ARRIVAL_TIME_EDIT, m_wndArrivalTime);
	DDX_Control(pDX, IDC_CMD_FUNC_LIST, m_wndFuncList);
	DDX_Control(pDX, IDC_CMD_FUNC_NAME_COMBO, m_FuncNameCombo);
	DDX_Control(pDX, IDC_CMD_SPA_LOADCASE_COMBO, m_wndSTLCCombo);
	DDX_Control(pDX, IDC_CMD_LOADCASE_COMBO, m_wndTHLCCombo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThspDlg, CDialog)
	//{{AFX_MSG_MAP(CCMThspDlg)
	ON_BN_CLICKED(IDC_CMD_LOADCASE_BTN, OnCmdLoadcaseBtn)
	ON_BN_CLICKED(IDC_CMD_FUNC_NAME_BTN, OnCmdFuncNameBtn)
	ON_BN_CLICKED(IDC_CMD_LOADCASE_BTN2, OnCmdSTLoadcaseBtn)
	ON_CBN_SELCHANGE(IDC_CMD_FUNC_NAME_COMBO, OnSelchangeCmdFuncNameCombo)
	ON_BN_CLICKED(IDC_CMD_ADD_BUTTON, OnCmdAddButton)
	ON_BN_CLICKED(IDC_CMD_MODIFY_BUTTON, OnCmdModifyButton)
	ON_BN_CLICKED(IDC_CMD_DELETE_BUTTON, OnCmdDeleteButton)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_FUNC_LIST, OnItemchangedCmdFuncList)
	ON_NOTIFY(NM_CLICK, IDC_CMD_FUNC_LIST, OnClickCmdFuncList)
	//ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThspDlg message handlers

void CCMThspDlg::OnCmdLoadcaseBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_HISTORY_LOADCASE, 0));
}

void CCMThspDlg::OnCmdFuncNameBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_TIME_TFUNCTION, 0));
}

BOOL CCMThspDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_wndSTLCCombo.SetIgnoreStldCSLoad(TRUE);
	m_wndSTLCCombo.SetLoadType(D_SELECTLOAD_STATIC , TRUE, TRUE);	
	m_wndTHLCCombo.SetIgnoreInitialLoad(TRUE);
	m_wndTHLCCombo.SetLoadType(D_SELECTLOAD_HISTORY, TRUE, TRUE);
	m_FuncNameCombo.ChangeType(FALSE, FALSE, FALSE, FALSE, TRUE,FALSE);
	
	MakeListHeader();
	OnSelchangeCmdFuncNameCombo(); 

	MakeThslList();

	m_wndScaleFactor.SetWindowText(_T("1"));
	m_wndArrivalTime.SetWindowText(_T("0"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThspDlg::OnCmdSTLoadcaseBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CCMThspDlg::MakeListHeader()
{
	CStringArray HTitles;
	
	HTitles.Add(_LS(IDS_CMD_0524Case));  
	HTitles.Add(_LS(IDS_CMD_0524Load));
	HTitles.Add(_LS(IDS_CMD_0524Function));

	CArray<float, float> HRatio;
	HRatio.Add(0.3f);
	HRatio.Add(0.3f);
	HRatio.Add(0.4f);

	CDlgUtil::_SetListCtrlHeader(&m_wndFuncList,HTitles,&HRatio,NULL);
}


void CCMThspDlg::MakeThslList()
{
	m_wndFuncList.DeleteAllItems();
	CStringArray Contents;
	DWORD        ItemData;
	
	T_THSL_K ThslK;
	T_THSL_D ThslD;
	T_THIS_D ThisD;
	T_STLD_D StldD;
	T_THFC_D ThfcD;
/* 
	int Index = 0;
	POSITION Pos = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStartThsl();
	while(Pos)
	{
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetNextThsl(Pos,ThslK,ThslD);
		
		//ThisD.

		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetThis(ThslD.nThisKey,ThisD)&&
			 CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStld(ThslD.nStldKey,StldD)&&
			 CDBDoc::GetDocPoint()->m_pAttrCtrl->GetThfc(ThslD.nThisFuncKey,ThfcD))

		{
			Contents.RemoveAll();
			Contents.Add(ThisD.LoadCaseName);
			Contents.Add(StldD.LoadCaseName);
			Contents.Add(ThfcD.ThisFuncName);
			ItemData = (DWORD)ThslK;
			CDlgUtil::SetListItem(&m_wndFuncList,Index,Contents,ItemData);
			Index++;
		}
	}
*/
	CArray<UINT, UINT> aThslK; aThslK.RemoveAll();
	POSITION Pos = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStartThsl();
	while(Pos)
	{
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetNextThsl(Pos,ThslK,ThslD);
		aThslK.Add(ThslK);
	}
	qsort(aThslK.GetData(), aThslK.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);
	for(int i=0; i<aThslK.GetSize(); i++)
	{
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetThsl(aThslK[i], ThslD);
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetThis(ThslD.nThisKey,ThisD)&&
			 CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStld(ThslD.nStldKey,StldD)&&
			 CDBDoc::GetDocPoint()->m_pAttrCtrl->GetThfc(ThslD.nThisFuncKey,ThfcD))

		{
			Contents.RemoveAll();
			Contents.Add(ThisD.LoadCaseName);
			Contents.Add(StldD.LoadCaseName);
			Contents.Add(ThfcD.ThisFuncName);
			ItemData = (DWORD)aThslK[i];
			CDlgUtil::SetListItem(&m_wndFuncList,i,Contents,ItemData);
		}
	}
}


void CCMThspDlg::OnCmdAddButton() 
{
	T_THSL_D ThslD;
	T_THSL_K ThslK;
	
	GetParameter(&ThslD,&ThslK);
	//if(CDBDoc::GetDocPoint()->m_pDataCtrl->AddThsl(ThslK,ThslD))
	ThslD.nThisKey = ThslK;
	if(CDBDoc::GetDocPoint()->m_pDataCtrl->AddThsl(ThslD))
	{
		//MakeThslList();
	}
}

void CCMThspDlg::OnCmdModifyButton() 
{
	T_THSL_D ThslD;
	T_THSL_K ThslK;

	CArray<DWORD, DWORD> arRet;
	if(CDlgUtil::GetSelectedListItemData(&this->m_wndFuncList,arRet))
	{
		if(arRet.GetSize() != 1) return;
		GetParameter(&ThslD,&ThslK);
		ThslD.nThisKey = ThslK;
		CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyThsl((T_THSL_K)arRet[0],(T_THSL_K)arRet[0],ThslD);	
	}
}

void CCMThspDlg::OnCmdDeleteButton() 
{
	CArray<DWORD, DWORD> arRet;
	if(CDlgUtil::GetSelectedListItemData(&m_wndFuncList,arRet))
	{
		CArray<T_THSL_K,T_THSL_K> arThslK;
		int nRet = arRet.GetSize();
		if(nRet)
		{
			for(int i = 0; i < nRet; i++)
				arThslK.Add((T_THSL_K)arRet[i]);
			CDBDoc::GetDocPoint()->m_pDataCtrl->DelThsl(arThslK);
		}
	}
}

void CCMThspDlg::GetParameter(void* pThslD, void * pThslK)
{
	unsigned int nSTLCType, nSTLCKey;
	unsigned int nTHLCType, nTHLCKey;
	T_THFC_K ThfcK;
	
	m_wndTHLCCombo.GetSelectedLoad(nTHLCType,nTHLCKey,NULL);
	m_wndSTLCCombo.GetSelectedLoad(nSTLCType,nSTLCKey,NULL);
	
	m_FuncNameCombo.GetSelectedThfc(ThfcK) ;
	
	CString strAT,strSF;
	double ATime,SFactor;

	m_wndArrivalTime.GetWindowText(strAT);
	m_wndScaleFactor.GetWindowText(strSF);

	if(!CStrParser::GetFloatNumber(strAT,ATime  )) 
		ATime   = 0;
	if(!CStrParser::GetFloatNumber(strSF,SFactor))
		SFactor = 0;

	*((unsigned int*)pThslK) = nTHLCKey;

	T_THSL_D *prThslD;
		
	prThslD = (T_THSL_D*) pThslD;
	
	prThslD->dArrivalTime = ATime;
	prThslD->dFactor      = SFactor;
	prThslD->nThisFuncKey = ThfcK;
	prThslD->nStldKey     = nSTLCKey;
}

void CCMThspDlg::SetParameter(void* pThslD, void * pThslK)
{
	T_THSL_D * prThslD = (T_THSL_D*) pThslD;
	
	//m_wndTHLCCombo.ChangeSelect(D_LOADCASE_HISTORY,*((T_THIS_K*)pThslK));
	m_wndTHLCCombo.ChangeSelect(D_LOADCASE_HISTORY,prThslD->nThisKey); /* JJB 2003/12/26 */
	m_wndSTLCCombo.ChangeSelect(D_LOADCASE_STATIC  ,prThslD->nStldKey);
	m_FuncNameCombo.ChangeSelect(prThslD->nThisFuncKey);
	CString StrTemp;
	StrTemp.Format(_T("%g"),prThslD->dArrivalTime);
	m_wndArrivalTime.SetWindowText(StrTemp);
	StrTemp.Format(_T("%g"),prThslD->dFactor);
	m_wndScaleFactor.SetWindowText(StrTemp);

	OnSelchangeCmdFuncNameCombo(); 
}

BOOL CCMThspDlg::GetSelectedData(int &nIndex)
{
	int iItem = m_wndFuncList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}


void CCMThspDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		//UpdateBuffer();
		//MakeItemEx();
		MakeThslList();
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

void CCMThspDlg::UpdateBuffer()
{
	UR_THSL_ADD;
	UR_THSL_DEL;
	
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	//T_THSL_K Key;
	//T_THSL_D Data;

	BOOL bThslCmd = FALSE;
	BOOL bThfcCmd = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_THFC_ADD):
		case(UR_THFC_DEL):
			bThfcCmd = TRUE;
			break;
		case(UR_THSL_ADD):
			{
				bThslCmd = TRUE;
			}
			break;
		case(UR_THSL_DEL):
			{
				bThslCmd = TRUE;
			}
			break;
		default:
			break;
		}
	} // end of while
	
	if(bThslCmd)
		MakeThslList();

	if(bThfcCmd)
		OnSelchangeCmdFuncNameCombo();
}

void CCMThspDlg::OnSelchangeCmdFuncNameCombo() 
{
	/*
	T_THFC_K ThfcK;
	T_THFC_D ThfcD;
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_CMD_LOADTYPE_STATIC);

	if(m_FuncNameCombo.GetSelectedThfc(ThfcK))
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetThfc(ThfcK,ThfcD))
		{
			if(pWnd)
			{
				//int nThisDataType;    // =1 
				
				switch(ThfcD.nThisDataType)
				{
				case 1:   
					//Normalized Acceleration =1 
					pWnd->SetWindowText(_LS(IDS_CMD_THSPDLG_Normalized_Acceleration));
					break;
				case 2: //Acceleration =2 
					pWnd->SetWindowText(_LS(IDS_CMD_THSPDLG_Acceleration));
					break; 
				case 3:   
					//Force  = 3 
					pWnd->SetWindowText(_LS(IDS_CMD_THSPDLG_Force));
					break;
				case 4:   
					//Moment = 4
					pWnd->SetWindowText(_LS(IDS_CMD_THSPDLG_Moment));
					break;
				case 5:   
					//Normal = 5
					pWnd->SetWindowText(_LS(IDS_CMD_THSPDLG_Normal));
					break;
				}
				//ThfcD.nThisDataType
				
				//if(ThfcD.nThisFuncType == 1)
				//  pWnd->SetWindowText(_LS(IDS_CMD_0507_Time_Function));
				//else 
				//  pWnd->SetWindowText(_LS(IDS_CMD_0507_Sinusoidal_Function));
				

				return;
			}
		}
	}

	if(pWnd)
		pWnd->SetWindowText(_T(""));
	*/
}


void CCMThspDlg::OnItemchangedCmdFuncList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int nIndex;
	if(this->GetSelectedData(nIndex))
	{
		T_THSL_K ThslK;
		T_THSL_D ThslD;
		ThslK = (T_THSL_K) m_wndFuncList.GetItemData(nIndex);
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetThsl(ThslK,ThslD))
		{
			SetParameter(&ThslD,&ThslK);
		}
	}
	
	*pResult = 0;
}

void CCMThspDlg::OnClickCmdFuncList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nIndex;
	if(this->GetSelectedData(nIndex))
	{
		T_THSL_K ThslK;
		T_THSL_D ThslD;
		ThslK = (T_THSL_K) m_wndFuncList.GetItemData(nIndex);
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetThsl(ThslK,ThslD))
		{
			SetParameter(&ThslD,&ThslK);
		}
	}
	
	*pResult = 0;
}

// void CCMThspDlg::OnCmdClose() 
// {
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);		
// }

BOOL CCMThspDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_THSL_D ThslData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	pDoc->m_pAttrCtrl->GetThsl(key, ThslData);

	SetParameter(&ThslData, &key);

	return TRUE;
}
