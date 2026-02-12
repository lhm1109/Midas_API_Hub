// CMWindPressureVelocityDefDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "CMWindPressureFunctionDefDlg.h"
#include "CMWindPressureFunctionAddModDlg.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureFunctionDefDlg dialog
#define CDialog CChildDialogMove


CCMWindPressureFunctionDefDlg::CCMWindPressureFunctionDefDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMWindPressureFunctionDefDlg::IDD, pParent)
{
}


void CCMWindPressureFunctionDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_DEF_WIND_FUNCTION_LIST, m_wndFunctionList);
}


BEGIN_MESSAGE_MAP(CCMWindPressureFunctionDefDlg, CDialog)
	ON_BN_CLICKED(IDC_CMD_DEF_WIND_FUNCTION_ADD,         OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_DEF_WIND_FUNCTION_MODIFY,      OnCmdModify)
	ON_BN_CLICKED(IDC_CMD_DEF_WIND_FUNCTION_DEL,         OnCmdDelete)
	ON_BN_CLICKED(IDC_CMD_DEF_WIND_FUNCTION_CLOSE,       OnCancel)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_DEF_WIND_FUNCTION_LIST, OnDblclkCmdTendonTypeList)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureFunctionDefDlg message handlers
BOOL CCMWindPressureFunctionDefDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetListHeader();
	MakeListData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMWindPressureFunctionDefDlg::SetListHeader()
{
	CStringArray HTitles;
	CArray<float, float> HRatio;
	
	HTitles.Add(_LS(IDS_CMD_WINDP_FUNCTION_DEF_NAME));
	HTitles.Add(_LS(IDS_CMD_MVLD_description));
	HTitles.Add(_LS(IDS_CMD_WINDP_FUNCTION_DEF_COORD));
	HRatio.Add(0.35f);
	HRatio.Add(0.35f);
	HRatio.Add(0.3f);

	CDlgUtil::_SetListCtrlHeader(&m_wndFunctionList, HTitles, &HRatio, NULL);
}


void CCMWindPressureFunctionDefDlg::MakeListData()
{
	int Index = 0;

	CStringArray Contents;
	Contents.SetSize(3);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	m_wndFunctionList.DeleteAllItems();

	T_WDFC_D WdfcD;
	CArray<T_WDFC_K, T_WDFC_K> caKeyList;
	pDoc->m_pAttrCtrl2->GetWdfcKeyList(caKeyList);
	int i, nSize;
	nSize = caKeyList.GetSize();
	for(i = 0 ; i < nSize; i++)
	{
		if(!pDoc->m_pAttrCtrl2->GetWdfc(caKeyList[i], WdfcD))
	{
		ASSERT(0);
		continue;
	}
		Contents[0] = WdfcD.strName;
		Contents[1] = WdfcD.strDescription;
		if(WdfcD.nCoordType == 1)      Contents[2] = _LS(IDS_CMD_WINDP_FUNCTION_DEF_COORD_REC);
		else if(WdfcD.nCoordType == 2) Contents[2] = _LS(IDS_CMD_WINDP_FUNCTION_DEF_COORD_CYL);

		CDlgUtil::SetListItem(&m_wndFunctionList, Index++, Contents, caKeyList[i]);
	}
}

void CCMWindPressureFunctionDefDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
void CCMWindPressureFunctionDefDlg::UpdateBuffer()
{  
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsWdfcCommand = FALSE;	
	
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_WDFC_ADD):
		case(UR_WDFC_DEL):
		case(UR_WDFC_MFD): 
		case(UR_WDFC_MFS):
		case(UR_WDFC_LT_ADD):
		case(UR_WDFC_LT_DEL):
				bIsWdfcCommand = TRUE;     
				break;
		default:
			break;
		}
	} 

	if(bIsWdfcCommand)
	{
		MakeListData();
	}
}


void CCMWindPressureFunctionDefDlg::OnCmdAdd() 
{
	CCMWindPressureFunctionAddModDlg dlg;
	dlg.DoModal();
}

void CCMWindPressureFunctionDefDlg::OnCmdDelete() 
{
	CArray<DWORD,DWORD> arDwdKeys;
	
	if(CDlgUtil::GetSelectedListItemData(&m_wndFunctionList, arDwdKeys))
	{
		CArray<T_WDFC_K,T_WDFC_K> arWdfcKeys;

		int i;
		int nSize = arDwdKeys.GetSize();

		arWdfcKeys.SetSize(nSize);
		for(i = 0 ; i < nSize; i++)
		{
			arWdfcKeys[i] = (T_WDFC_K)arDwdKeys[i];
		}

		if(nSize > 0)
			CDBDoc::GetDocPoint()->m_pDataCtrl->DelWdfc(arWdfcKeys);
	}
}

void CCMWindPressureFunctionDefDlg::OnCmdModify() 
{
	T_WDFC_K WdfcK;
	CArray<int,int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndFunctionList,arRet,LVIS_SELECTED);
	if(arRet.GetSize() == 1)
	{
		CCMWindPressureFunctionAddModDlg dlg;
		WdfcK = (T_WDFC_K)m_wndFunctionList.GetItemData(arRet[0]);
		dlg.SetWdfcKey(WdfcK);
		dlg.DoModal();
	}
}

void CCMWindPressureFunctionDefDlg::OnDblclkCmdTendonTypeList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCmdModify();
	*pResult = 0;
}

