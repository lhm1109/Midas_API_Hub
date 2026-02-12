// CMStageConsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageConsDlg.h"
#include "CMStageDefMainDlg.h"
#include "CMStageDefineDlg.h"
#include "CMStageItemBase.h"
#include "CMStageAdditionalConsDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_base\wg_base_TestEnvMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CChildDialogMove
/////////////////////////////////////////////////////////////////////////////
// CCMStageConsDlg dialog


CCMStageConsDlg::CCMStageConsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMStageConsDlg::IDD, pParent)
{
	EnableLayout(TRUE);
	//{{AFX_DATA_INIT(CCMStageConsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMStageConsDlg::LayoutCtrl(void)
{
	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
	if (pLayout)
	{
		pLayout->AddAnchor(IDC_CMD_ADD, CBCGPStaticLayout::XMoveType::e_MoveTypeHorz, CBCGPStaticLayout::XSizeType::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_CMD_ADD_PREV, CBCGPStaticLayout::XMoveType::e_MoveTypeHorz, CBCGPStaticLayout::XSizeType::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_CMD_ADD_NEXT, CBCGPStaticLayout::XMoveType::e_MoveTypeHorz, CBCGPStaticLayout::XSizeType::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_CMD_GENERATE, CBCGPStaticLayout::XMoveType::e_MoveTypeHorz, CBCGPStaticLayout::XSizeType::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_CMD_MODIFY1, CBCGPStaticLayout::XMoveType::e_MoveTypeHorz, CBCGPStaticLayout::XSizeType::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_CMD_DELETE1, CBCGPStaticLayout::XMoveType::e_MoveTypeHorz, CBCGPStaticLayout::XSizeType::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_CMD_SHOW, CBCGPStaticLayout::XMoveType::e_MoveTypeHorz, CBCGPStaticLayout::XSizeType::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_STAGE_LIST, CBCGPStaticLayout::XMoveType::e_MoveTypeNone, CBCGPStaticLayout::XSizeType::e_SizeTypeBoth);
		pLayout->AddAnchor(IDC_CMD_CLOSE, CBCGPStaticLayout::XMoveType::e_MoveTypeBoth, CBCGPStaticLayout::XSizeType::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_CMD_ADD_ANAL, CBCGPStaticLayout::XMoveType::e_MoveTypeHorz, CBCGPStaticLayout::XSizeType::e_SizeTypeNone);
	}
}

void CCMStageConsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStageConsDlg)
	DDX_Control(pDX, IDC_STAGE_LIST, m_wndStageList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMStageConsDlg, CDialog)
	//{{AFX_MSG_MAP(CCMStageConsDlg)
	ON_BN_CLICKED(IDC_CMD_ADD, OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	ON_BN_CLICKED(IDC_CMD_DELETE1, OnCmdDelete1)
	ON_BN_CLICKED(IDC_CMD_GENERATE, OnCmdGenerate)
	ON_BN_CLICKED(IDC_CMD_MODIFY1, OnCmdModify1)
	ON_BN_CLICKED(IDC_CMD_ADD_NEXT, OnCmdAddNext)
	ON_BN_CLICKED(IDC_CMD_ADD_PREV, OnCmdAddPrev)
	ON_NOTIFY(NM_DBLCLK, IDC_STAGE_LIST, OnDblclkStageList)
	ON_BN_CLICKED(IDC_CMD_SHOW, OnCmdShow)
	ON_BN_CLICKED(IDC_CMD_ADD_ANAL, OnCmdAddAnalysis)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMStageConsDlg message handlers

void CCMStageConsDlg::CtrlMan()
{
	if(!CCMStageItemBase::IsCurStageBase())
	{
		CWnd * pModify = GetDlgItem(IDC_CMD_MODIFY1);
		//CWnd * pModify = GetDlgItem(IDC_CMD_ADD );
		CWnd * pShow   = GetDlgItem(IDC_CMD_SHOW);
		ASSERT(pModify);
		ASSERT(pShow);
		CRect MRect,SRect;
		int DY;
		pModify->GetWindowRect(&MRect);
		pShow  ->GetWindowRect(&SRect);

		DY = MRect.top - SRect.top;
		
		CArray<UINT,UINT> Ctrls;
		Ctrls.Add(IDC_CMD_ADD     );
		Ctrls.Add(IDC_CMD_ADD_PREV);
		Ctrls.Add(IDC_CMD_ADD_NEXT);
		Ctrls.Add(IDC_CMD_GENERATE);
		Ctrls.Add(IDC_CMD_MODIFY1 ); 
		Ctrls.Add(IDC_CMD_DELETE1 ); 
		CDlgUtil::CtrlEnableDisable(this,Ctrls,FALSE);
		//CDlgUtil::CtrlShowHide(this,Ctrls,FALSE);

		/*
		Ctrls.RemoveAll();
		Ctrls.Add(IDC_CMD_MODIFY1);
		CDlgUtil::CtrlShowHide(this,Ctrls,FALSE);
		*/

		Ctrls.RemoveAll();
		Ctrls.Add(IDC_CMD_SHOW);
		CDlgUtil::CtrlShowHide(this,Ctrls,TRUE);
		
		CDlgUtil::CtrlMoveDistY(this,Ctrls,DY);
	}
	else
	{
		CArray<UINT,UINT> Ctrls;
		Ctrls.RemoveAll();
		Ctrls.Add(IDC_CMD_SHOW);
		CDlgUtil::CtrlShowHide(this,Ctrls,FALSE);
	}

	CArray<UINT, UINT> Ctrls;
	Ctrls.RemoveAll();
	Ctrls.Add(IDC_CMD_ADD_ANAL);
	CDlgUtil::CtrlShowHide(this,Ctrls,FALSE);
#ifdef _MEC
	if (CTestEnvMgr::GetTestEnvST(_T("CSBUCK")) == _T("yes"))
	{
		CArray<UINT, UINT> Ctrls;
		Ctrls.RemoveAll();
		Ctrls.Add(IDC_CMD_ADD_ANAL);
		CDlgUtil::CtrlShowHide(this, Ctrls, TRUE);
	}
#endif
}

BOOL CCMStageConsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	LayoutCtrl();
/*
	m_wndDown.SubclassDlgItem(IDC_CMD_DOWN_BUTTON,this);
	m_wndDown.SetIcon(IDI_DOWN_ICON);
	
	m_wndUp.SubclassDlgItem(IDC_CMD_UP_BUTTON,this);
	m_wndUp.SetIcon(IDI_UP_ICON);
	
	m_wndBottommost.SubclassDlgItem(IDC_CMD_BOTTOMMOST_BUTTON,this);
	m_wndBottommost.SetIcon(IDI_BOTTOMMOST_ICON);
	
	m_wndTopMost.SubclassDlgItem(IDC_CMD_TOPMOST_BUTTON,this);
	m_wndTopMost.SetIcon(IDI_TOPMOST_ICON);
*/
	
	// TODO: Add extra initialization here
	CStringArray HTitles;
	CArray<float, float> HRatio;

	HTitles.Add(_LS(IDS_CMD0417__Name));HTitles.Add(_LS(IDS_CMD0417__Duration));HTitles.Add(_LS(IDS_CMD0417__Date));
	HTitles.Add(_LS(IDS_CMD0417__Step)); HTitles.Add(_LS(IDS_CMD0417__Result));
	HRatio.Add(0.2f); HRatio.Add(0.2f); HRatio.Add(0.2f); HRatio.Add(0.2f);
	HRatio.Add(0.2f);
	
	CDlgUtil::_SetListCtrlHeader(&m_wndStageList,HTitles,&HRatio,NULL);

	UpdateStageList();

	/*
	CArray<int,int>arIndex;
	arIndex.Add(0);
	CDlgUtil::SetListItemByMask(&m_wndStageList,arIndex,LVIS_SELECTED);
	*/
	CtrlMan();


	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMStageConsDlg::UpdateStageList()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	m_DurationList.RemoveAll()     ;
	m_DurationList.Add(0);
	// Hydration stage일 경우 Stage 없음
	if (!pDoc->m_pAttrCtrl->ExistConstStag()) 
	{
		m_wndStageList.DeleteAllItems();
		return;
	}

	CArray<T_STAG_K, T_STAG_K> rKeyList;
	T_STAG_D StageD;
	pDoc->m_pAttrCtrl->GetStagKeyList(rKeyList);
	int nKeys = rKeyList.GetSize();
	CStringArray Contents;

	m_wndStageList.DeleteAllItems();
	
	CString TStr;

	double  DurSum = 0;
	
	
	for(int i = 0; i < nKeys; i++)
	{
		Contents.RemoveAll();
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStag(rKeyList[i],StageD))
		{
			Contents.Add(StageD.StageName);
			TStr.Format(_T("%.5g"),StageD.dDuration);
			Contents.Add(TStr);
			DurSum += StageD.dDuration;
			m_DurationList.Add(DurSum);
			TStr.Format(_T("%.5g"),DurSum);
			Contents.Add(TStr);
			
			/*
			if(StageD.nStepOption == 1)
				TStr.Format(_T("%d"),StageD.nTimeStepNum);
			else
			*/
			TStr.Format(_T("%d"),StageD.aUserDefStep.GetSize());
			Contents.Add(TStr);

			TStr.Empty();
			if(StageD.bSaveResultStage)
			{
				TStr = _LS(IDS_CMD0417__Stage);
				if(StageD.bSaveResultStep)
				{
					TStr += _LS(IDS_CMD0417___Step);
				}
			}
			else
			{
				if(StageD.bSaveResultStep)
				{
					TStr = _LS(IDS_CMD0417__Step);
				}
			}

			Contents.Add(TStr);
			
			CDlgUtil::SetListItem(&m_wndStageList,i,Contents,(DWORD)rKeyList[i]);
		}
	}
}

void CCMStageConsDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
		UpdateStageList();
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

void CCMStageConsDlg::UpdateBuffer()
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
		this->UpdateStageList();
	}
}

void CCMStageConsDlg::OnCmdClose() 
{
	OnOK();
}

void CCMStageConsDlg::OnCmdDelete1() 
{
	CArray<DWORD,DWORD> arRet;
	CArray<T_STAG_K,T_STAG_K> arStagKeys;
	CDlgUtil::GetSelectedListItemData(&m_wndStageList,arRet);
	int nRet = arRet.GetSize();
	for(int i = 0 ; i < nRet ; i ++)
	{
		arStagKeys.Add((T_STAG_K)arRet[i]); 
	}
	if(nRet)
		CDBDoc::GetDocPoint()->m_pDataCtrl->DelStag(arStagKeys);
}

void CCMStageConsDlg::OnCmdGenerate() 
{
	CCMStageDefineDlg Dlg;
	Dlg.DoModal();
	
	/*
	CArray<int,int>arIndex;
	arIndex.Add(0);
	CDlgUtil::SetListItemByMask(&m_wndStageList,arIndex,LVIS_SELECTED);
	*/
}

void CCMStageConsDlg::OnCmdAdd() 
{
	/*
	int nStage = m_wndStageList.GetItemCount();
	double PrevDay;
	if(nStage)
	{
		T_STAG_D StagD;
		m_wndStageList.GetItemText(
		T_STAG_K PrevStage = (T_STAG_K) m_wndStageList.GetItemData(nStage-1);
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStag(PrevStage,StagD))
		{
			//StagD.
		}
	}
	*/

	T_STAG_K StageK = 0;
	CCMStageDefMainDlg Dlg;
	double CurDur = m_DurationList.GetAt(m_wndStageList.GetItemCount());
	Dlg.SetCurrentStage(StageK,CurDur,0);
	Dlg.DoModal();
}


void CCMStageConsDlg::OnCmdAddNext() 
{
	CArray<int, int>arRet ;
	CDlgUtil::GetListItemByMask(&m_wndStageList,arRet,LVIS_FOCUSED);
	CCMStageDefMainDlg Dlg;
	
	if(arRet.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Not_exist_selected_item));
		return;
	}

	T_STAG_K StageK = (T_STAG_K) m_wndStageList.GetItemData(arRet[0]);
	double CurDura = m_DurationList[arRet[0]];
	Dlg.SetCurrentStage(StageK,CurDura,1);
	Dlg.DoModal();
}

void CCMStageConsDlg::OnCmdAddPrev() 
{
	
	CArray<int, int>arRet;
	CDlgUtil::GetListItemByMask(&m_wndStageList,arRet,LVIS_FOCUSED);
	CCMStageDefMainDlg Dlg;
	
	if(arRet.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Not_exist_selected_item));
		return;
	}

	T_STAG_K StageK = (T_STAG_K) m_wndStageList.GetItemData(arRet[0]);

	double CurDur;
	if(arRet[0] < 1)
		CurDur = 0;
	else
		CurDur = m_DurationList[arRet[0] - 1];
	
	Dlg.SetCurrentStage(StageK,CurDur,2);
	Dlg.DoModal();
}


void CCMStageConsDlg::OnCmdModify1() 
{
	CArray<int,int>arRet;
	BOOL bCheck = CDlgUtil::GetListItemByMask(&m_wndStageList,arRet,LVIS_SELECTED);
	if(bCheck == FALSE && arRet.GetSize() == 0 && m_wndStageList.GetItemCount() != 0)
	{
		arRet.Add(0); //리스트 박스에 아이템이 존재 하는데 선택 하지 않았을 경우, 첫번째 아이템 자동 선택.
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_STAG_K StageK=0;
	T_STAG_D StageD;
	int nSize = arRet.GetSize();
	if(nSize==0)
	{
		CArray<T_STAG_K, T_STAG_K> aStagK;
		pDoc->m_pAttrCtrl->GetStagKeyList(aStagK);
		if(aStagK.GetSize()>0) StageK = aStagK[0];
	}
	else if(nSize>0)
	{
		StageK = (T_STAG_K)m_wndStageList.GetItemData(arRet[0]);
	}

	pDoc->m_pAttrCtrl->GetStag(StageK, StageD);
	
	if(StageK==0 || !pDoc->m_pAttrCtrl->IsConstStag(StageK, StageD))
	{
		AfxMessageBox(_LS(IDS_CMD0417__Error___No_Selected_Stage__nChoose_one));
		return;
	}
	CArray<T_STAG_K,T_STAG_K> StagKeyList;
	pDoc->m_pAttrCtrl->GetStagKeyList(StagKeyList);
	int nDurationIdx = -1;
	for(int i=0; i<StagKeyList.GetSize(); i++)
	{
		if(StageK == StagKeyList[i])
		{
			nDurationIdx = i;
			break;
		}
	}

	CCMStageDefMainDlg Dlg;
	double CurDur = this->m_DurationList[nDurationIdx+1];
	Dlg.SetCurrentStage(StageK, CurDur, 3);
	Dlg.DoModal();

	m_wndStageList.SetItemState(arRet[0], LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}


void CCMStageConsDlg::OnDblclkStageList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//lpnmitem = (LPNMITEMACTIVATE) lParam;

	//if(CCMStageItemBase::IsCurStageBase())
	//{
		CArray<int,int> arRet;
		CDlgUtil::GetListItemByMask(&m_wndStageList,arRet,LVIS_FOCUSED);
		
		if(arRet.GetSize())
		{
			OnCmdModify1();
		}
	//}
	
	*pResult = 0;
}

void CCMStageConsDlg::OnCmdShow() 
{
	// TODO: Add your control notification handler code here
	OnCmdModify1();
}

void CCMStageConsDlg::OnCmdAddAnalysis()
{
	CCMStageAdditionalConsDlg Dlg;
	Dlg.DoModal();
}