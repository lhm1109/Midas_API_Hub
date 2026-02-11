#include "stdafx.h"
#include "wg_cmd.h"

#include "CMTendonTypeDlg.h"
#include "CMTendonTypeAddDlg.h"

#include "..\wg_base\DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMTendonTypeDlg dialog
CCMTendonTypeDlg::CCMTendonTypeDlg(CWnd* pParent /*=NULL*/)
: CChildDialogMove(CCMTendonTypeDlg::IDD, pParent)
{
}

void CCMTendonTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_TENDON_TYPE_LIST, m_wndTendonTypes);
}

BEGIN_MESSAGE_MAP(CCMTendonTypeDlg, CChildDialogMove)
	ON_BN_CLICKED(IDC_CMD_ADD, OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_DELETE1, OnCmdDelete)
	ON_BN_CLICKED(IDC_CMD_MODIFY1, OnCmdModify)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_TENDON_TYPE_LIST, OnDblclkCmdTendonTypeList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTendonTypeDlg message handlers
BOOL CCMTendonTypeDlg::OnInitDialog() 
{
	CChildDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetListHeader();
	MakeListData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMTendonTypeDlg::SetListHeader()
{
	CStringArray HTitles;
	CArray<float, float> HRatio;
	
	HTitles.Add(_LS(IDS_CMD_TDNT_LIST_Name));
	HTitles.Add(_LS(IDS_CMD_TDNT_LIST_Type));
	HRatio.Add(0.5f);
	HRatio.Add(0.5f);

	CDlgUtil::_SetListCtrlHeader(&m_wndTendonTypes,HTitles,&HRatio,NULL);
}


void CCMTendonTypeDlg::MakeListData()
{
	CArray<T_TDNT_K, T_TDNT_K> TdntKeys;
	T_TDNT_D TdntD;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdntKeyList(TdntKeys);

	int nTdnt = TdntKeys.GetSize();
	int Index = 0;
	CStringArray Contents;
	CString TStr;
	m_wndTendonTypes.DeleteAllItems();
	for(int i = 0; i < nTdnt; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdnt(TdntKeys[i],TdntD))
		{
			Contents.RemoveAll();
			Contents.Add(TdntD.TendonTypeName);
			if(TdntD.nTendonType == 1)
			{
				TStr.Format(_LS(IDS_CMD_TDNT_LIST_Internal));
			}
			else if(TdntD.nTendonType == 2)
			{
				TStr.Format(_LS(IDS_CMD_TDNT_LIST_External));
			}
			else
			{
				ASSERT(FALSE);
			}

			Contents.Add(TStr);
			CDlgUtil::SetListItem(&m_wndTendonTypes,Index,Contents,TdntKeys[i]);
			Index ++;
		}
	}
}

void CCMTendonTypeDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		MakeListData();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
		MakeListData();
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
void CCMTendonTypeDlg::UpdateBuffer()
{
	
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsTdntCommand = FALSE;	
	
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_TDNT_ADD):
		case(UR_TDNT_DEL):
		case(UR_TDNT_MFD): 
		case(UR_TDNT_MFS):
				bIsTdntCommand = TRUE;     
				break;
		default:
			break;
		}
	} 

	if(bIsTdntCommand)
	{
		MakeListData();
	}
}


void CCMTendonTypeDlg::OnCmdAdd() 
{
	// TODO: Add your control notification handler code here
	CCMTendonTypeAddDlg Dlg;
	Dlg.SetTDNTD(0);
	Dlg.DoModal();
}

void CCMTendonTypeDlg::OnCmdDelete() 
{
	CArray<int,int> arRet;
	CArray<CString,CString&>TdntNames;
	CStringArray Contents;
	DWORD TdntK;
	
	CDlgUtil::GetListItemByMask(&m_wndTendonTypes,arRet,LVIS_SELECTED);

	int nRet = arRet.GetSize();
	if(nRet)
	{
		for(int i = 0 ;i < nRet; i++)
		{
			Contents.RemoveAll();
			if(CDlgUtil::GetListItem(&m_wndTendonTypes,arRet[i],Contents,TdntK))
			{
				TdntNames.Add(Contents[0]);
			}
		}
	}

	if(TdntNames.GetSize())
		CDBDoc::GetDocPoint()->m_pDataCtrl->DelTdnt(TdntNames);
}

void CCMTendonTypeDlg::OnCmdModify() 
{
	T_TDNT_K TdntK;
	CArray<int,int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndTendonTypes,arRet,LVIS_SELECTED);
	if(arRet.GetSize() == 1)
	{
		CCMTendonTypeAddDlg Dlg;
		TdntK = (T_TDNT_K)m_wndTendonTypes.GetItemData(arRet[0]);
		Dlg.SetTDNTD(TdntK);
		Dlg.DoModal();
	}
	else
	{
		AfxMessageBox(_LS(IDS_CMD_TDNT_LIST_Error___No_Selected_Tendon_Type__));
	}
}

void CCMTendonTypeDlg::OnDblclkCmdTendonTypeList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCmdModify(); 
	*pResult = 0;
}

