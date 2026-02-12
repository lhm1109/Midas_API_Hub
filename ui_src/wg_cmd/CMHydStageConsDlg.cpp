// CMHydStageConsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMHydStageConsDlg.h"
#include "CMHydStageMainDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"


	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMHydStageConsDlg dialog
#define CDialog CChildDialogMove 



CCMHydStageConsDlg::CCMHydStageConsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMHydStageConsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMHydStageConsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMHydStageConsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMHydStageConsDlg)
	DDX_Control(pDX, IDC_STAGE_LIST, m_StageList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMHydStageConsDlg, CDialog)
	//{{AFX_MSG_MAP(CCMHydStageConsDlg)
	ON_BN_CLICKED(IDC_CMD_ADD, OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_ADD_NEXT, OnCmdAddNext)
	ON_BN_CLICKED(IDC_CMD_ADD_PREV, OnCmdAddPrev)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	ON_BN_CLICKED(IDC_CMD_DELETE1, OnCmdDelete1)
	ON_BN_CLICKED(IDC_CMD_MODIFY1, OnCmdModify1)
	ON_NOTIFY(NM_DBLCLK, IDC_STAGE_LIST, OnDblclkStageList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMHydStageConsDlg message handlers

BOOL CCMHydStageConsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	MakeListHeader();
	UpdateHydStageList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMHydStageConsDlg::MakeListHeader()
{
	CStringArray HTitles;
	CArray<float, float> HRatio;
	HTitles.Add(_LS(IDS_CMD0417__Stage));  HRatio.Add(1.0f);
	CDlgUtil::_SetListCtrlHeader(&m_StageList,HTitles,&HRatio,NULL);
}

void CCMHydStageConsDlg::UpdateHydStageList()
{
	CArray<T_HSTG_K, T_HSTG_K> HstgKeys;
	T_HSTG_D HstgD;

	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetHstgKeyList(HstgKeys);

	int nHstg = HstgKeys.GetSize();
	
	m_StageList.DeleteAllItems();

	CStringArray Contents;
	CString StrTemp;

	for(int i = 0 ; i < nHstg ; i ++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetHstg(HstgKeys[i],HstgD))
		{
			Contents.RemoveAll();
			Contents.Add(HstgD.StageName);
			CDlgUtil::SetListItem(&m_StageList,i,Contents,(DWORD)HstgKeys[i]);
		}
	}
}

void CCMHydStageConsDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
		UpdateHydStageList();
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

void CCMHydStageConsDlg::UpdateBuffer()
{
	
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsHstgCommand = FALSE;	
	
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_HSTG_ADD):
		case(UR_HSTG_DEL):
		case(UR_HSTG_MFD): 
		case(UR_HSTG_MFS):
				bIsHstgCommand = TRUE;
				break;
		default:
			break;
		}
	} 

	if(bIsHstgCommand)
	{
		UpdateHydStageList();
	}
}

void CCMHydStageConsDlg::OnCmdAdd() 
{
	T_HSTG_K HstgK = 0;
	CCMHydStageMainDlg Dlg;
	Dlg.SetCurrentStage(HstgK,0); // Default Add(0)
	Dlg.DoModal();
}

void CCMHydStageConsDlg::OnCmdAddNext() 
{
	CArray<int, int>arRet; 
	CDlgUtil::GetListItemByMask(&m_StageList,arRet,LVIS_FOCUSED);
	CCMHydStageMainDlg Dlg;
	
	if(arRet.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Not_exist_selected_item));
		return;
	}

	T_HSTG_K StageK = (T_HSTG_K) m_StageList.GetItemData(arRet[0]);
	Dlg.SetCurrentStage(StageK,1);
	Dlg.DoModal();
	
}

void CCMHydStageConsDlg::OnCmdAddPrev() 
{
	CArray<int, int>arRet;
	CDlgUtil::GetListItemByMask(&m_StageList,arRet,LVIS_FOCUSED);
	CCMHydStageMainDlg Dlg;
	
	if(arRet.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Not_exist_selected_item));
		return;
	}

	T_HSTG_K StageK = (T_HSTG_K) m_StageList.GetItemData(arRet[0]);
	Dlg.SetCurrentStage(StageK,2);
	Dlg.DoModal();

}

void CCMHydStageConsDlg::OnCmdDelete1() 
{
	CArray<DWORD,DWORD> arRet;
	CArray<T_HSTG_K,T_HSTG_K> arStagKeys;
	CDlgUtil::GetSelectedListItemData(&m_StageList,arRet);
	int nRet = arRet.GetSize();
	for(int i = 0 ; i < nRet ; i ++)
	{
		arStagKeys.Add((T_HSTG_K)arRet[i]); 
	}
	
	if(nRet)
		CDBDoc::GetDocPoint()->m_pDataCtrl->DelHstg(arStagKeys);
		
}

void CCMHydStageConsDlg::OnCmdModify1() 
{
	CArray<int,int>arRet;
	CDlgUtil::GetListItemByMask(&m_StageList,arRet,LVIS_FOCUSED);
	
	T_HSTG_K StageK;
	if(arRet.GetSize() == 1)
	{
		StageK = (T_HSTG_K)m_StageList.GetItemData(arRet[0]);
		CCMHydStageMainDlg Dlg;
		Dlg.SetCurrentStage(StageK,0);
		Dlg.DoModal();
	}
	else
	{
		AfxMessageBox(_LS(IDS_CMD0417__Error___No_Selected_Stage__nChoose_one));
	}
}



void CCMHydStageConsDlg::OnCmdClose() 
{
	OnOK();
}


void CCMHydStageConsDlg::OnDblclkStageList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCmdModify1();
	*pResult = 0;
}
