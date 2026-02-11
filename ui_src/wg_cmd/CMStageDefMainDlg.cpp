// CMStageDefMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageDefMainDlg.h"
#include "CMStageDefineDlg.h"
#include "CMStageItemBndrDlg.h"
#include "CMStageItemElemDlg.h"
#include "CMStageItemLoadDlg.h"
#include "CMStageItemTendonDlg.h"
#include "CMStageUDefStepDlg.h"
#include "CMStageInfoDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
//#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "LCUTIL.h"  

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define StrMapFormat _T("%.8g")
/////////////////////////////////////////////////////////////////////////////
// CCMStageDefMainDlg dialog
#define CDialog CChildDialogMove


CCMStageDefMainDlg::CCMStageDefMainDlg(CWnd* pParent /*=NULL*/)
	: CChildDialogMove(CCMStageDefMainDlg::IDD, pParent)
{
	//m_wndGrid = new CCMStageGrid;
	//{{AFX_DATA_INIT(CCMStageDefMainDlg)
	m_strStageName = _T("");
	m_strDuration  = _T("0");
	m_strTStep     = _T("0");
	m_bSaveStage = FALSE;
	m_bSaveStep = FALSE;
	m_nStepOption = 1;
	m_strDays = _T("0");
	//}}AFX_DATA_INIT
	m_pStageData = new T_STAG_D;
	m_CurStageK  = 0;
	m_nAddOption = 0; // (0) Add (1) Insert Next  (2) Insert Prev  (3) Modify
	m_PrevDay;
	m_bIncreStep = FALSE;
	m_wndTStep.SetRange(0, SHRT_MAX);
}

CCMStageDefMainDlg::~CCMStageDefMainDlg()
{
	//if (m_wndGrid) delete m_wndGrid;
	delete m_pStageData;
}

void CCMStageDefMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStageDefMainDlg)
	DDX_Control(pDX, IDC_CMD_DAY_LIST, m_wndDayList);
	DDX_Control(pDX, IDC_CMD_TIME_STEP_EDIT, m_wndTStep);
	//DDX_Control(pDX, IDC_CMD_TIME_STEP_SPIN, m_wndTStepSpin);
	DDX_Control(pDX, IDC_CMD_TAB, m_wndTabCtrl);
	DDX_Text(pDX, IDC_CMD_NAME_EDIT, m_strStageName);
	DDX_Text(pDX, IDC_CMD_DURATION_EDIT, m_strDuration);
	DDX_Text(pDX, IDC_CMD_TIME_STEP_EDIT, m_strTStep);
	DDX_Check(pDX, IDC_CMD_STAGE_CHECK, m_bSaveStage);
	DDX_Check(pDX, IDC_CMD_STEP_CHECK, m_bSaveStep);
	DDX_Text(pDX, IDC_CMD_DAYS_EDIT, m_strDays);
	DDX_Control(pDX, IDC_CMD_STAGE_CMB, m_cmbStage);
	DDX_Check(pDX, IDC_STAGE_INCRE_STEP_MATL_NONLINEAR_ANAL_CHK, m_bIncreStep);
	DDX_Control(pDX, IDC_STAGE_INCRE_STEP_MATL_NONLINEAR_ANAL_EDT, m_edtIncreStep);
	//}}AFX_DATA_MAP
}

#define CDialog CChildDialogMove
BEGIN_MESSAGE_MAP(CCMStageDefMainDlg, CDialog)
#undef CDialog 
	//{{AFX_MSG_MAP(CCMStageDefMainDlg)
	ON_WM_PAINT()
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TAB, OnSelchangeCmdTab)
	ON_BN_CLICKED(IDC_CMD_APPLY_BUTTON, OnCmdApplyButton)
	ON_BN_CLICKED(IDC_CMD_CANCEL_BUTTON, OnCmdCancelButton)
	ON_BN_CLICKED(IDC_CMD_OK_BUTTON, OnCmdOkButton)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_TIME_STEP_SPIN, OnDeltaposCmdTimeStepSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_DURATION_SPIN, OnDeltaposCmdDurationSpin)
	ON_BN_CLICKED(IDC_CMD_STAGE_CHECK, OnCmdStageCheck)
	ON_BN_CLICKED(IDC_STAGE_INFO_BUTTON, OnStageInfoButton)
	ON_BN_CLICKED(IDC_CMD_GEN_STEP_BUTTON, OnCmdGenStepButton)
	ON_BN_CLICKED(IDC_CMD_ADD_BUTTON, OnCmdAddButton)
	ON_BN_CLICKED(IDC_CMD_DEL_BUTTON, OnCmdDelButton)
	ON_BN_CLICKED(IDC_CMD_ASTEP_CLEAR_BUTTON, OnCmdAstepClearButton)
	ON_BN_CLICKED(IDC_CMD_ASTEP_MOD_BUTTON, OnCmdAstepModButton)
	ON_CBN_SELCHANGE(IDC_CMD_STAGE_CMB, OnSelchangeCmdStageCmb)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_STAGE_SPN, OnDeltaposCmdStageSpn)
	ON_BN_CLICKED(IDC_STAGE_INCRE_STEP_MATL_NONLINEAR_ANAL_CHK, OnIncreStepChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMStageDefMainDlg message handlers
void CCMStageDefMainDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CCMStageDefMainDlg::UpdateBuffer()
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
	BOOL bIsLdgrCommand = FALSE;
	BOOL bMFD = FALSE;

	T_STAG_K Key,  KeyBak;
	T_STAG_D Data, DataBak;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_STAG_ADD):
		case(UR_STAG_DEL):
		case(UR_STAG_MFD): 
		case(UR_STAG_MFS):
			if(nCmd==UR_STAG_MFD)
			{
				pViewBuff->GetStag(nKey, KeyBak, DataBak);
				bMFD = TRUE;
			}
			else if(nCmd==UR_STAG_MFS)
			{
				pViewBuff->GetStag(nKey, Key, Data);
				if(bMFD && m_nAddOption==3 && DataBak.StageName.CompareNoCase(Data.StageName)!=0)
					ResetStagCombo();
				bMFD = FALSE;
			}
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
		case(UR_LDGR_ADD):
		case(UR_LDGR_DEL):
		case(UR_LDGR_MFD):
		case(UR_LDGR_MFS):
				bIsLdgrCommand = TRUE;
				break;
		default:
			break;
		}
	} 

	if(bIsStagCommand)
	{
		T_STAG_K __StageKey = m_CurStageK;
			
		if(__StageKey)
		{
			if(bIsStagCommand)
			{
				CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStag(__StageKey, * m_pStageData);
				CCMStageItemBase * pChild = (CCMStageItemBase*) m_wndTabCtrl.GetTab(m_wndTabCtrl.GetCurSel());
				if(pChild)
				 pChild->SetCurStageData(m_pStageData);
			}
			else 
			{
				
				int nWhat = m_wndTabCtrl.GetCurSel();
				if((bIsGrupCommand && nWhat == 0) ||
					 (bIsBngrCommand && nWhat == 1) ||
					 (bIsLdgrCommand && nWhat == 2)  )
				{
					CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStag(__StageKey, * m_pStageData);
					CCMStageItemBase * pChild = (CCMStageItemBase*) m_wndTabCtrl.GetTab(m_wndTabCtrl.GetCurSel());
					if(pChild)
						pChild->SetCurStageData(m_pStageData);
				}
				
			}
		}
	}
	else
	{
		
		//T_STAG_K __StageKey = m_CurStageK;
		int nWhat = m_wndTabCtrl.GetCurSel();
		if((bIsGrupCommand && nWhat == 0) ||
			 (bIsBngrCommand && nWhat == 1) ||
			 (bIsLdgrCommand && nWhat == 2)  )
		{
			//CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStag(__StageKey, * m_pStageData);
			CCMStageItemBase * pChild = (CCMStageItemBase*) m_wndTabCtrl.GetTab(m_wndTabCtrl.GetCurSel());
			if(pChild)
				pChild->SetCurStageData(m_pStageData);
		}
	}
}

void CCMStageDefMainDlg::SetCurrentStage(T_STAG_K CurStageKey,double PrevDay,int nOption /*= 0*/)
{
	m_CurStageK  = CurStageKey;
	m_nAddOption = nOption;
	m_PrevDay    = PrevDay;
}


void CCMStageDefMainDlg::ResetStagCombo()
{
	m_cmbStage.ResetContent();

	if(m_nAddOption!=3) return;

	int nCurSel = 0;
	CArray<CString, CString&> aStagName;

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetStagList(aStagName);

	for(int i=0; i<aStagName.GetSize(); ++i)
	{
		CString strName = aStagName.GetAt(i);
		T_STAG_K StagK = pDoc->m_pAttrCtrl->GetStagKey(strName);
		m_cmbStage.SetItemData(m_cmbStage.AddString(strName), StagK);
		if(m_CurStageK==StagK) nCurSel = i;
	}
	m_cmbStage.SetCurSel(nCurSel);
}

BOOL CCMStageDefMainDlg::OnInitDialog() 
{
	CChildDialogMove::OnInitDialog();
	
	SetChildDlgs();

	m_cmbStage.ResetContent();

	ResetStagCombo();

	GetDlgItem(IDC_CMD_STAGE_STC)->EnableWindow(m_nAddOption==3); // modify
	GetDlgItem(IDC_CMD_STAGE_CMB)->EnableWindow(m_nAddOption==3); // modify
	//GetDlgItem(IDC_CMD_STAGE_SPN)->EnableWindow(m_nAddOption==3); // modify

	m_edtIncreStep.SetUnitType(D_UNITSYS_NONE);

	if(m_CurStageK==0)
	{
		m_pStageData->Initialize();
	}
	else
	{
		if(m_nAddOption==0 || m_nAddOption==3)
		{
			CDBDoc *pDoc = CDBDoc::GetDocPoint();
			
			if(!pDoc->m_pAttrCtrl->GetStag(m_CurStageK,* m_pStageData))
				m_pStageData->Initialize();
		}
		else
		{
			m_pStageData->Initialize();
		}
	}

	UpdateCurStageData();
	
	UpdateData(FALSE);

	CtrlMan();
	OnIncreStepChk();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMStageDefMainDlg::UpdateCurStageData()
{
	m_strStageName = m_pStageData->StageName;
	m_strDuration.Format(_T("%.5g"),m_pStageData->dDuration   );
	//m_strTStep   .Format(_T("%d"),m_pStageData->nTimeStepNum);

	m_bSaveStage  = m_pStageData->bSaveResultStage    ;
	m_bSaveStep   = m_pStageData->bSaveResultStep     ;

	m_bIncreStep = m_pStageData->bIncreStep;
	m_edtIncreStep.SetEditUnit(m_pStageData->nIncreStep);

	//m_nStepOption = m_pStageData->nStepOption       -1;
	m_nStepOption = 1;

	if(m_nStepOption == 1)
	{
		//m_wndDayList.DeleteAllItems();
		CLCUtil::ClearAllListItemDbl(&m_wndDayList);

		int nItem = m_pStageData->aUserDefStep.GetSize();

		CStringArray Contents;
		CString      TStr;

		for(int i = 0 ; i < nItem ; i++)
		{
			Contents.RemoveAll();
			TStr.Format(_T("%d"), i+1);
			Contents.Add(TStr);
			TStr.Format(_T("%g"), m_pStageData->aUserDefStep[i]);
			Contents.Add(TStr);
			CLCUtil::SetListItemDbl(&m_wndDayList,i,Contents,m_pStageData->aUserDefStep[i]);
		}
	}
	
	StepOptionMan();
	SaveResultMan();
		
	CCMStageItemBase* pChild = (CCMStageItemBase*) m_wndTabCtrl.GetTab(m_wndTabCtrl.GetCurSel());
	if(pChild)
	{
		pChild->SetCurStageData(m_pStageData);
		pChild->ResetParam();
	}
}

void CCMStageDefMainDlg::StepOptionMan()
{
	/*
	CArray<UINT, UINT> Ctrls1;
	CArray<UINT, UINT> Ctrls2;
	Ctrls1.Add(IDC_CMD_TIME_STEP_EDIT);
	Ctrls1.Add(IDC_CMD_TIME_STEP_SPIN);
	Ctrls2.Add(IDC_CMD_USERDEF_BUTTON);
	if(m_nStepOption == 0)
	{
		CDlgUtil::CtrlEnableDisable(this,Ctrls1,TRUE);
		CDlgUtil::CtrlEnableDisable(this,Ctrls2,FALSE);
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this,Ctrls1,FALSE);
		CDlgUtil::CtrlEnableDisable(this,Ctrls2,TRUE);
	}
	*/
}

BOOL CCMStageDefMainDlg::SetChildDlgs()
{
	CStringArray HTitles;
	CArray<float, float> HRatio;
	HTitles.Add(_LS(IDS_CMD0417__Step)); HTitles.Add(_LS(IDS_CMD0417__Day));
	HRatio.Add(0.5f)   ; HRatio.Add(0.5f);
	CDlgUtil::_SetListCtrlHeader(&m_wndDayList,HTitles,&HRatio,NULL);
	
	CCMStageItemBase* pDlg;

	CWnd* pPlaceHolder = GetDlgItem(IDC_CMD_PLACEHOLDER);
	
	m_wndTabCtrl.SetPlaceHolder(pPlaceHolder);
	
	pDlg = new CCMStageItemElemDlg();
	m_wndTabCtrl.AddTab(pDlg, _LS(IDS_CMD0417__Element) , CCMStageItemElemDlg::IDD  , TRUE);
	pDlg = new CCMStageItemBndrDlg();
	m_wndTabCtrl.AddTab(pDlg, _LS(IDS_CMD0417__Boundary), CCMStageItemBndrDlg::IDD  , TRUE);
	pDlg = new CCMStageItemLoadDlg();
	m_wndTabCtrl.AddTab(pDlg, _LS(IDS_CMD0417__Load)    , CCMStageItemLoadDlg::IDD  , TRUE);
	/*
	pDlg = new CCMStageItemTendonDlg();
	m_wndTabCtrl.AddTab(pDlg, _T("Tendon")  , CCMStageItemTendonDlg::IDD, TRUE);
	*/

	m_wndTabCtrl.ShowTab(0);
	
	return TRUE;
}

/*
void CCMStageDefMainDlg::OnCmdGenerateButton() 
{
	CCMStageDefineDlg Dlg;
	Dlg.DoModal();
}
*/

// Add, Modify, Del시 에러 메세지가 떴을 때 이동하면 화면이 
// 다시 그려지도록 하기 위해 LockUpdate를 끄고 다시 그린 후
// LockUpdate를 원상복구한다.
void CCMStageDefMainDlg::OnPaint() 
{
	
	CPaintDC dc(this); // device context for painting
	/*
	// TODO: Add your message handler code here
	BOOL bOldLock = m_wndGrid->LockUpdate(FALSE);
	m_wndGrid->Redraw();
	m_wndGrid->LockUpdate(bOldLock);
	*/
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CCMStageDefMainDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//m_wndGrid->OnTerminate(D_TB_SAVE_ALWAYS);
	CLCUtil::ClearAllListItemDbl(&m_wndDayList);
	return CChildDialogMove::DestroyWindow();
}

void CCMStageDefMainDlg::OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CCMStageItemBase* pChild = (CCMStageItemBase*) m_wndTabCtrl.GetTab(m_wndTabCtrl.GetCurSel());
	if(pChild)
		pChild->SetCurStageData(m_pStageData);

	/*
	int nCurSel = m_wndTabCtrl.GetCurSel();

	CArray<UINT,UINT> Ctrls;
	Ctrls.Add(IDC_CMD_DAYS_EDIT);
	Ctrls.Add(IDC_CMD_ADD_BUTTON);
	Ctrls.Add(IDC_CMD_DEL_BUTTON);
	Ctrls.Add(IDC_CMD_STATIC15);
	Ctrls.Add(IDC_CMD_STATIC19);
	Ctrls.Add(IDC_CMD_STATIC20);
	Ctrls.Add(IDC_CMD_DAY_LIST);
	Ctrls.Add(IDC_CMD_TIME_STEP_EDIT);
	Ctrls.Add(IDC_CMD_GEN_STEP_BUTTON);
	Ctrls.Add(IDC_CMD_TIME_STEP_SPIN);

	if(nCurSel == 2) // Load Group...
	{
		CDlgUtil::CtrlEnableDisable(this,Ctrls,TRUE);  
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this,Ctrls,FALSE);
	}
	*/
	
	*pResult = 0;
}

BOOL __bIsError;
void CCMStageDefMainDlg::OnCmdApplyButton() 
{
	UpdateData(TRUE);

	__bIsError = FALSE;

	if(m_strStageName.IsEmpty())
	{
		__bIsError = TRUE;
		goto Error;
	}
	m_pStageData->StageName = m_strStageName;
	
	double  nDur;
	if(!CStrParser::GetFloatNumber(m_strDuration,nDur))
	{
		__bIsError = TRUE;
		goto Error;
	}
	m_pStageData->dDuration = nDur;

	if(m_nStepOption == 0)
	{
		int nTStep;
		if(!CStrParser::GetINumber(m_strTStep,nTStep))
		{
			__bIsError = TRUE;
			goto Error;
		}
		//m_pStageData->nTimeStepNum = nTStep;
	}
	/*
	else
	{
		
	}
	*/
	
	//m_pStageData->nStepOption = m_nStepOption + 1  ;
	m_pStageData->bSaveResultStage = m_bSaveStage;
	m_pStageData->bSaveResultStep  = m_bSaveStep;
	
	m_pStageData->bIncreStep = m_bIncreStep;
	m_pStageData->nIncreStep = m_edtIncreStep.GetEditValue();

	SaveAdditionalStep();

Error:
	if(!__bIsError)
	{
		if(m_nAddOption==0 || m_nAddOption==3)
		{
			if(m_CurStageK==0) // ADD
			{
				__bIsError = !CDBDoc::GetDocPoint()->m_pDataCtrl->AddStag(*m_pStageData);

				if(!__bIsError)
				{
					m_pStageData->aActiveBndr.RemoveAll();
					m_pStageData->aActiveElem.RemoveAll();
					m_pStageData->aActiveLoad.RemoveAll();
					m_pStageData->aDeactiveBndr.RemoveAll();
					m_pStageData->aDeactiveElem.RemoveAll();
					m_pStageData->aDeactiveLoad.RemoveAll();
					UpdateCurStageData();
				}
			}
			else  // Modify
			{
				__bIsError =  !CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyStag(m_CurStageK,*m_pStageData);
			}
		}
		else if( m_nAddOption==1) // Insert Next 
		{
			ASSERT(m_CurStageK);
			__bIsError = !CDBDoc::GetDocPoint()->m_pDataCtrl->InsertStag(m_CurStageK,TRUE,*m_pStageData);
			if(!__bIsError)
			{
				m_pStageData->aActiveBndr  .RemoveAll();
				m_pStageData->aActiveElem  .RemoveAll();
				m_pStageData->aActiveLoad  .RemoveAll();
				m_pStageData->aDeactiveBndr.RemoveAll();
				m_pStageData->aDeactiveElem.RemoveAll();
				m_pStageData->aDeactiveLoad.RemoveAll();
				UpdateCurStageData();
			}
		}
		else if(m_nAddOption==2) // Insert Prev
		{
			ASSERT(m_CurStageK);
			__bIsError = !CDBDoc::GetDocPoint()->m_pDataCtrl->InsertStag(m_CurStageK,FALSE,*m_pStageData);
			if(!__bIsError)
			{
				m_pStageData->aActiveBndr  .RemoveAll();
				m_pStageData->aActiveElem  .RemoveAll();
				m_pStageData->aActiveLoad  .RemoveAll();
				m_pStageData->aDeactiveBndr.RemoveAll();
				m_pStageData->aDeactiveElem.RemoveAll();
				m_pStageData->aDeactiveLoad.RemoveAll();
				UpdateCurStageData();
			}
		}
		else
			ASSERT(FALSE);

		// 메세지 출력
		CDBDoc::GetDocPoint()->m_pAttrCtrl->CheckStageSelfWeightData();

		CDBDoc::GetDocPoint()->m_pAttrCtrl->CheckStageTdplData();
	}
	else
	{
		MessageBox(_LS(IDS_CMD0417__Parameter_Error__),_LS(IDS_CMD0417__Error));
	}
}

void CCMStageDefMainDlg::OnStageInfoButton() 
{
	CCMStageInfoDlg Dlg;

	CArray<T_STAG_K, T_STAG_K> StagArray;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStagKeyList(StagArray);
	
	int nCurStagIndex;

	if(m_nAddOption==0 || m_nAddOption==3)
	{
		if(m_CurStageK==0) // ADD
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
				if(m_CurStageK==StagArray[i]) break;
			}
		}
		Dlg.SetCurStageData(nCurStagIndex,*m_pStageData);
	}
	else if(m_nAddOption==1) // Insert Next 
	{
		ASSERT(m_CurStageK);
		nCurStagIndex= -1;
		int nSize = StagArray.GetSize();
		if(nSize > 0)
		{
			for(int i = 0;i < nSize; i++)
			{
				nCurStagIndex++; 
				if(m_CurStageK==StagArray[i]) break;
			}
		}
		else
		{
			nCurStagIndex = 0;
		}

		Dlg.SetCurStageData(nCurStagIndex,*m_pStageData);
	}
	else if(m_nAddOption==2) // Insert Prev
	{
		ASSERT(m_CurStageK);
		nCurStagIndex= -1;
		int nSize = StagArray.GetSize();
		if(nSize > 0)
		{
			for(int i = 0;i < nSize; i++)
			{
				nCurStagIndex++; 
				if(m_CurStageK==StagArray[i]) break;
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


void CCMStageDefMainDlg::OnCmdCancelButton() 
{
	OnCancel();
}


void CCMStageDefMainDlg::OnCmdOkButton() 
{
	OnCmdApplyButton(); 
	if(! __bIsError)
		OnOK();
}

void CCMStageDefMainDlg::OnDeltaposCmdTimeStepSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDlgUtil::SetSpinValue(this,IDC_CMD_TIME_STEP_EDIT,0,-1,pNMHDR,pResult);
}

BOOL CCMStageDefMainDlg::GetNumOfDiv(CEdit* pEdit, int &nDiv)
{
	CString csDiv;
	pEdit->GetWindowText(csDiv);
	return CStrParser::GetINumber(csDiv, nDiv);
}

void CCMStageDefMainDlg::OnDeltaposCmdDurationSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//CDlgUtil::SetSpinValue(this,IDC_CMD_DURATION_EDIT,0,-1,pNMHDR,pResult);
	CDlgUtil::SetFloatSpinValue(this,IDC_CMD_DURATION_EDIT,_T("0"),_LS(IDS_CMD0417__UDF),1.0,pNMHDR,pResult);
}

void CCMStageDefMainDlg::OnStepOption() 
{
	UpdateData(TRUE);
	StepOptionMan();
}

void CCMStageDefMainDlg::SaveResultMan()
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

void CCMStageDefMainDlg::OnCmdStageCheck() 
{
	UpdateData(TRUE);
	SaveResultMan();
}

void CCMStageDefMainDlg::OnCmdUserdefButton() 
{
	/* 사용되지 않음 !!!
	// TODO: Add your control notification handler code here
	CCMStageUDefStepDlg Dlg;
	Dlg.SetSteps(&(m_pStageData->aUserDefStep));
	Dlg.DoModal();
	*/
}

void CCMStageDefMainDlg::SaveAdditionalStep()
{
	m_pStageData->aUserDefStep.RemoveAll();
	
	int nI = m_wndDayList.GetItemCount();
	double dDay;
	for( int i = 0 ; i < nI ; i++)
	{
		dDay =*(double*)m_wndDayList.GetItemData(i);
		m_pStageData->aUserDefStep.Add(dDay);
	}

	CCMStageItemBase * pChild = (CCMStageItemBase*) m_wndTabCtrl.GetTab(m_wndTabCtrl.GetCurSel());
	if(pChild)
		pChild->SetCurStageData(m_pStageData);
}

void CCMStageDefMainDlg::OnCmdGenStepButton() 
{
	UpdateData();
	CMap<double,double,double,double> DayMap;
	DayMap.InitHashTable(50);
	
	// MQC:2288-JHYUN-20081117
	CLCUtil::ClearAllListItemDbl(&m_wndDayList);

//   int nItems = m_wndDayList.GetItemCount();
//   double Day;
//   for(int i = 0; i < nItems; i++)
//   {
//     Day = *(double*)m_wndDayList.GetItemData(i);
//     DayMap.SetAt(Day,Day);
//   }

	m_PrevDay = 0.0;

	// Log Scale Day생성....
	int nCount;
	double dDuration,dDay;
	if(CStrParser::GetINumber(m_strTStep,nCount))
	{
		if(CStrParser::GetFloatNumber(m_strDuration,dDuration))
		{
			//dDuration += m_PrevDay;
			if (nCount > 0)
			{
				double PrevLog = 0;
				double p;
				if(m_PrevDay == 0.0)
					p = log10(dDuration)/double(nCount+1);
				else
				{
					PrevLog = log10(m_PrevDay);
					p = (log10(dDuration+m_PrevDay)- PrevLog)/double(nCount+1);
				}

				double d, t;
				for (int i = 0; i < nCount; i++)
				{
					d = pow(10.0,PrevLog + p*(i+1)) ;
					
					t = floor(d);
					if (d - t >= 0.5) d = ceil(d);
					
					dDay = (int)(d - m_PrevDay);
					DayMap.SetAt(dDay,dDay);
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
		
	CArray<double, double> RDays;
	POSITION Pos = DayMap.GetStartPosition();
	double rKey;
	double rData;
	while(Pos)
	{
		DayMap.GetNextAssoc(Pos,rKey, rData);
		RDays.Add(rData);
	}

	if(RDays.GetSize())
	{
		qsort(RDays.GetData(),RDays.GetSize(),sizeof(double),CCompFunc::doubleAsc);

		//m_wndDayList.DeleteAllItems();
		CLCUtil::ClearAllListItemDbl(&m_wndDayList);

		int nItem = RDays.GetSize();

		CStringArray Contents;
		CString      TStr;

		int Index = 0;
		for(int i = 0 ; i < nItem ; i++)
		{
			if(RDays[i] != 0 && RDays[i] < dDuration)
			{
				Contents.RemoveAll();
				TStr.Format(_T("%d"), i+1);
				Contents.Add(TStr);
				TStr.Format(StrMapFormat, RDays[i]);
				Contents.Add(TStr);
				//CDlgUtil::SetListItem(&m_wndDayList,i,Contents,Days[i]);
				CLCUtil::SetListItemDbl(&m_wndDayList,Index,Contents,RDays[i]);
				Index++;
			}
		}
	}

	SaveAdditionalStep();
}

void CCMStageDefMainDlg::OnCmdAddButton() 
{
	UpdateData(TRUE);
	
	CMap<double,double,double,double> DayMap;
	DayMap.InitHashTable(50);
	double   Day;
	CArray<double,double> Days;
	double Duration;
	if(!CStrParser::GetFloatNumber(m_strDuration,Duration)) return;
	if(CStrParser::GetUEDistance(m_strDays,Days))
	{
		if(Days.GetSize())
		{
			int nItem = m_wndDayList.GetItemCount();
			for( int i = 0 ; i < nItem ; i++)
			{
				Day = *(double*)m_wndDayList.GetItemData(i);
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
					{
						DayMap.SetAt(Days[i], Days[i]);
					}
				}
			}

			CArray<double , double> RDays;

			POSITION Pos = DayMap.GetStartPosition();
			double dKey;
			double dData;
			while(Pos)
			{
				DayMap.GetNextAssoc(Pos,dKey,dData);
				RDays.Add(dData);
			}

			qsort(RDays.GetData(),RDays.GetSize(),sizeof(double),CCompFunc::doubleAsc);
			
			CLCUtil::ClearAllListItemDbl(&m_wndDayList);

			nItem = RDays.GetSize();

			CStringArray Contents;
			CString      TStr;

			for(int  i = 0 ; i < nItem ; i++)
			{
				Contents.RemoveAll();
				TStr.Format(_T("%d"), i+1);
				Contents.Add(TStr);
				TStr.Format(StrMapFormat, RDays[i]);
				Contents.Add(TStr);
				CLCUtil::SetListItemDbl(&m_wndDayList,i,Contents,RDays[i]);
			}
		}
		SaveAdditionalStep();
	}
	else
	{
		// Parameter Error!!!
	}
}

void CCMStageDefMainDlg::OnCmdDelButton() 
{
	CMap  <double , double,double , double> ItemMap;
	CArray<double , double> ArrDay ;
	ItemMap.InitHashTable(50);

	int nItems = m_wndDayList.GetItemCount();
	if(!nItems) return;
	double dDay;
	
	for(int i = 0; i < nItems; i++)
	{
		dDay = *(double*)m_wndDayList.GetItemData(i);
		ItemMap.SetAt(dDay,dDay);
	}

	CArray<int, int> SelItems;
	CDlgUtil::GetListItemByMask(&m_wndDayList,SelItems,LVIS_SELECTED);

	nItems = SelItems.GetSize();
	
	if(nItems == 0) return;
	
	
	for(int  i = 0; i < nItems ;i ++)
	{
		dDay = *(double *)m_wndDayList.GetItemData(SelItems[i]);
		ItemMap.RemoveKey(dDay);
	}

	CLCUtil::ClearAllListItemDbl(&m_wndDayList);
	
	
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
		CLCUtil::SetListItemDbl(&m_wndDayList,i,Contents,ArrRDay[i]);
	}
	SaveAdditionalStep();
}

void CCMStageDefMainDlg::OnCmdAstepClearButton() 
{
	CLCUtil::ClearAllListItemDbl(&m_wndDayList);
	SaveAdditionalStep();
}

void CCMStageDefMainDlg::OnCmdAstepModButton() 
{
	UpdateData(TRUE);
 
	CArray<int,int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndDayList,arRet,LVIS_SELECTED);
	if(arRet.GetSize() != 1) return;

	CArray<double,double> Days;
	if(!CStrParser::GetUEDistance(m_strDays,Days)) return;
	if(Days.GetSize()  != 1) return;

	CMap<double , double,double,double> DayMap;
	DayMap.InitHashTable(50);
	
	int nDay = m_wndDayList.GetItemCount();
	double  dDay;
	for( int i = 0 ; i < nDay ; i++)
	{ 
		dDay = *((double*)m_wndDayList.GetItemData(i));
		DayMap.SetAt(dDay,dDay);
	}
	
	double rDay;
	if(DayMap.Lookup(Days[0],rDay)) return;
	
	//m_wndDayList.DeleteItem(arRet[0]);
	CLCUtil::DeleteListItemDbl(&m_wndDayList,arRet[0]);
	SaveAdditionalStep();
	OnCmdAddButton() ;
}

void CCMStageDefMainDlg::CtrlMan()
{
	if(!CCMStageItemBase::IsCurStageBase())
	{
		CArray<UINT,UINT> Ctrls;
		Ctrls.Add(IDC_CMD_OK_BUTTON    );
		//Ctrls.Add(IDC_CMD_CANCEL_BUTTON);
		Ctrls.Add(IDC_CMD_APPLY_BUTTON );

		CWnd * pAWnd, *pCWnd;

		pAWnd = GetDlgItem(IDC_CMD_APPLY_BUTTON);
		pCWnd = GetDlgItem(IDC_CMD_CANCEL_BUTTON);

		ASSERT(pAWnd); ASSERT(pCWnd);
		CRect ARect, CARect;

		pAWnd->GetWindowRect(&ARect);
		pCWnd->GetWindowRect(&CARect);

		int nDX = ARect.left - CARect.left;

		CDlgUtil::CtrlShowHide(this,Ctrls,FALSE);

		Ctrls.RemoveAll();
		Ctrls.Add(IDC_CMD_CANCEL_BUTTON);
		CDlgUtil::CtrlMoveDistX(this,Ctrls,nDX);
		pCWnd->SetWindowText(_LS(IDS_CMD0417__Close));
	}
}



////////////////////////////////////////////////////////////////////////////////////////////
// List Control 내용 채우기..... Multi Column List Control의 내용 설정 
// Single Column에도 동일하게 적용된다. 
BOOL CLCUtil::ClearAllListItemDbl(CListCtrl* pList)
{
	int nItem = pList->GetItemCount();
	double *pDbl;
	for(int i = 0; i< nItem ; i++)
	{
		pDbl = (double *)pList->GetItemData(i);
		ASSERT(pDbl);
		delete pDbl;
	}
	return pList->DeleteAllItems();
}

BOOL CLCUtil::DeleteListItemDbl(CListCtrl *pList, int nIndex)
{
	double *pDbl;
	pDbl = (double *)pList->GetItemData(nIndex);
	ASSERT(pDbl);
	delete pDbl;
	return pList->DeleteItem(nIndex);
}

BOOL CLCUtil::SetListItemDbl(CListCtrl *pList, int nIndex , CStringArray &Contents,double ItemData)
{
	int nColumn;
	
	if(pList->GetHeaderCtrl())
		nColumn = pList->GetHeaderCtrl()->GetItemCount();
	else
		nColumn = 1;

	int nItem = pList->GetItemCount();

	LVITEM lvitem;
	CString str;
	lvitem.iItem=nIndex;
	
	ASSERT(Contents.GetSize() <= nColumn);

	int nIter = Contents.GetSize();


	
	for(int i = 0; i < nColumn ; i++)
	{
		lvitem.iSubItem = i;
		str = Contents[i];
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;
		
		if(nIndex < nItem && nIndex >= 0 )
		{
			pList->SetItem(&lvitem);
		}
		else
		{
			if(i == 0)
			 pList->InsertItem(&lvitem);
			else
			 pList->SetItem(&lvitem);
		}
		

	  str.ReleaseBuffer();
	}

	double* pDbl =  new double;

	*pDbl = ItemData;
	
	pList->SetItemData(nIndex, (LONG_PTR)pDbl);

	return TRUE;
}


BOOL CLCUtil::GetListItemDbl(CListCtrl *pList, int nIndex, CStringArray &Contents,double& ItemData)
{
	Contents.RemoveAll();
	int nColumn;
	if(pList->GetHeaderCtrl())
		nColumn = pList->GetHeaderCtrl()->GetItemCount();
	else
		nColumn = 1;

	LVITEM lvitem;
	CString str;
	TCHAR Buffer[256];
	lvitem.iItem=nIndex;
	
	ASSERT(Contents.GetSize() <= nColumn);

	int nIter = Contents.GetSize();
	
	for(int i = 0; i < nColumn ; i++)
	{
		lvitem.iSubItem = i;
		lvitem.pszText  = Buffer;
		lvitem.cchTextMax  = 255;
		lvitem.mask = LVIF_TEXT;
		pList->GetItem(&lvitem);
		str = lvitem.pszText;
		Contents.Add(str);
	  str.ReleaseBuffer();
	}

	double * pDbl;
	pDbl = (double*)pList->GetItemData(nIndex);

	ItemData = *pDbl;

	return TRUE;
}

BOOL CLCUtil::GetSelectedListItemDataDbl(CListCtrl *pList, CArray<double, double> &arRet)
{
	//ASSERT(pmyListCtrl->GetItemState(n, LVIS_SELECTED) == LVIS_SELECTED);
	arRet.RemoveAll();
	ASSERT(pList);
	int nItem = pList->GetItemCount();
	double *pItemData;

	for( int i = 0 ; i < nItem ; i++)
	{
		if(pList->GetItemState(i,LVIS_SELECTED) == LVIS_SELECTED)
		{
			pItemData = (double*)pList->GetItemData(i);
			arRet.Add(*pItemData);
		}
	}
	return TRUE;
}


void CCMStageDefMainDlg::OnSelchangeCmdStageCmb() 
{
	m_CurStageK = m_cmbStage.GetItemData(m_cmbStage.GetCurSel());
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetStag(m_CurStageK, *m_pStageData);

	UpdateCurStageData();
	
	UpdateData(FALSE);

	CtrlMan();
	OnIncreStepChk();
}

void CCMStageDefMainDlg::OnDeltaposCmdStageSpn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	int nCount = m_cmbStage.GetCount();
	int nSel   = m_cmbStage.GetCurSel();
	nSel -= pNMUpDown->iDelta;
	if     (nSel< 0)      nSel = 0;
	else if(nSel>=nCount) nSel = nCount-1;

	m_cmbStage.SetCurSel(nSel);

	OnSelchangeCmdStageCmb();

	*pResult = 0;
}

void CCMStageDefMainDlg::OnIncreStepChk() 
{
	UpdateData(TRUE);

	GetDlgItem(IDC_STAGE_INCRE_STEP_MATL_NONLINEAR_ANAL_EDT)->EnableWindow(m_bIncreStep);
}