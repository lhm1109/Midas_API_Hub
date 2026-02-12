// CMWindPressureVelocityDefDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "CMWindPressureVelocityDefDlg.h"
#include "CMWindPressureVelocityAddMod.h"
#include "CMWindPressureBaseMgr.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureVelocityDefDlg dialog
#define CDialog CChildDialogMove


CCMWindPressureVelocityDefDlg::CCMWindPressureVelocityDefDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMWindPressureVelocityDefDlg::IDD, pParent)
{
}


void CCMWindPressureVelocityDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_DEF_WIND_VELOCITY_PRESSURE_LIST, m_wndVelocityList);
}


BEGIN_MESSAGE_MAP(CCMWindPressureVelocityDefDlg, CDialog)
	ON_BN_CLICKED(IDC_CMD_DEF_WIND_VELOCITY_PRESSURE_ADD,         OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_DEF_WIND_VELOCITY_PRESSURE_MODIFY,      OnCmdModify)
	ON_BN_CLICKED(IDC_CMD_DEF_WIND_VELOCITY_PRESSURE_DEL,         OnCmdDelete)
	ON_BN_CLICKED(IDC_CMD_DEF_WIND_VELOCITY_PRESSURE_CLOSE,       OnCancel)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_DEF_WIND_VELOCITY_PRESSURE_LIST, OnDblclkCmdTendonTypeList)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureVelocityDefDlg message handlers
BOOL CCMWindPressureVelocityDefDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetListHeader();
	MakeListData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMWindPressureVelocityDefDlg::SetListHeader()
{
	CStringArray HTitles;
	CArray<float, float> HRatio;

	HTitles.Add(_LS(IDS_CMD_WINDP_VELOCITY_DEF_PRESNAME));
	HTitles.Add(_LS(IDS_CMD_WINDP_VELOCITY_DEF_CODENAME));
	HRatio.Add(0.5f);
	HRatio.Add(0.5f);

	CDlgUtil::_SetListCtrlHeader(&m_wndVelocityList, HTitles, &HRatio, NULL);
}


void CCMWindPressureVelocityDefDlg::MakeListData()
{
	CArray<T_WVEP_K, T_WVEP_K> WvepKeys;
	CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetWvepKeyList(WvepKeys);

	int nSize = WvepKeys.GetSize();
	int Index = 0;

	CStringArray Contents;
	CString TStr;
	T_WVEP_D WvepD;

	m_wndVelocityList.DeleteAllItems();
	for(int i = 0; i < nSize; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetWvep(WvepKeys[i], WvepD))
		{
			Contents.RemoveAll();
			Contents.Add(WvepD.StrName);
			Contents.Add(CCMWindPressureBaseMgr::GetCodeStrByDBCode(WvepD.nCodeType));
			CDlgUtil::SetListItem(&m_wndVelocityList,Index++,Contents,WvepKeys[i]);
		}
	}
}

void CCMWindPressureVelocityDefDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
		MakeListData();
		break;
	case D_UPDATE_BUFFER_BEFORE:
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
	case D_UPDATE_EXE_START:
	case D_UPDATE_EXE_END:
	default:
		break;
	}
 
}
void CCMWindPressureVelocityDefDlg::UpdateBuffer()
{  
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsWvepCommand = FALSE;	
	
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_WVEP_ADD):
		case(UR_WVEP_DEL):
		case(UR_WVEP_MFD): 
		case(UR_WVEP_MFS):
		case(UR_WVEP_LT_ADD):
		case(UR_WVEP_LT_DEL):
				bIsWvepCommand = TRUE;     
				break;
		default:
			break;
		}
	} 

	if(bIsWvepCommand)
	{
		MakeListData();
	}
}


void CCMWindPressureVelocityDefDlg::OnCmdAdd() 
{
	CCMWindPressureVelocityAddMod dlg;
	dlg.DoModal();
}

void CCMWindPressureVelocityDefDlg::OnCmdDelete() 
{
	CArray<DWORD,DWORD> arDwdKeys;
	
	if(CDlgUtil::GetSelectedListItemData(&m_wndVelocityList, arDwdKeys))
	{
		CArray<T_WVEP_K,T_WVEP_K> arWvepKeys;

		int i;
		int nSize = arDwdKeys.GetSize();

		arWvepKeys.SetSize(nSize);
		for(i = 0 ; i < nSize; i++)
		{
			arWvepKeys[i] = (T_WVEP_K)arDwdKeys[i];
		}

		if(nSize > 0)
			CDBDoc::GetDocPoint()->m_pDataCtrl->DelWvep(arWvepKeys);
	}
}

void CCMWindPressureVelocityDefDlg::OnCmdModify() 
{
	T_WVEP_K WvepK;
	CArray<int,int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndVelocityList,arRet,LVIS_SELECTED);
	if(arRet.GetSize() == 1)
	{
		CCMWindPressureVelocityAddMod dlg;
		WvepK = (T_WVEP_K)m_wndVelocityList.GetItemData(arRet[0]);
		dlg.SetWvepKey(WvepK);
		dlg.DoModal();
	}
}

void CCMWindPressureVelocityDefDlg::OnDblclkCmdTendonTypeList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCmdModify();
	*pResult = 0;
}

