// SoilPropListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SoilPropListDlg.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "SoilPropAddModDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSoilPropListDlg dialog
#define CDialog CDialogMove


CSoilPropListDlg::CSoilPropListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoilPropListDlg::IDD, pParent)
{
}


void CSoilPropListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_LIST, m_wndSoilList);
}


BEGIN_MESSAGE_MAP(CSoilPropListDlg, CDialog)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_COPY  , OnCmdCopy)  
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnCancel)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkList)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoilPropListDlg message handlers
BOOL CSoilPropListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	SetListHeader();
	MakeListData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CSoilPropListDlg::SetListHeader()
{
	CStringArray HTitles;
	CArray<float, float> HRatio;

	HTitles.Add(_LS(IDS_CMD_LOAD_SOIL_PROP_NAME));
	HTitles.Add(_LS(IDS_CMD_LOAD_SOIL_PROP_DESC));
	HRatio.Add(0.3f);
	HRatio.Add(0.7f);

	CDlgUtil::_SetListCtrlHeader(&m_wndSoilList, HTitles, &HRatio, NULL);
}


void CSoilPropListDlg::MakeListData()
{
	int Index = 0;

	CStringArray Contents;
	Contents.SetSize(2);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	m_wndSoilList.DeleteAllItems();

	T_POSP_D PospD;
	CArray<T_POSP_K, T_POSP_K> caKeyList;
	pDoc->m_pAttrCtrl->GetPospKeyList(caKeyList);
	int i, nSize;
	nSize = caKeyList.GetSize();
	for (i = 0; i < nSize; i++)
	{
		if (!pDoc->m_pAttrCtrl->GetPosp(caKeyList[i], PospD))
		{
			ASSERT(0);
			continue;
		}
		Contents[0] = PospD.strName;
		Contents[1] = PospD.strDescription;

		CDlgUtil::SetListItem(&m_wndSoilList, Index++, Contents, caKeyList[i]);
	}
}

void CSoilPropListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{

	switch (lHint)
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
void CSoilPropListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsPospCommand = FALSE;

	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_POSP_ADD):
		case(UR_POSP_DEL):
		case(UR_POSP_MFD):
		case(UR_POSP_MFS):
			bIsPospCommand = TRUE;
			break;
		default:
			break;
		}
	}

	if (bIsPospCommand)
	{
		MakeListData();
	}
}


void CSoilPropListDlg::OnCmdAdd()
{
	CSoilPropAddModDlg dlg;
	dlg.DoModal();
}

void CSoilPropListDlg::OnCmdDelete()
{
	CArray<DWORD, DWORD> arDwdKeys;

	if (CDlgUtil::GetSelectedListItemData(&m_wndSoilList, arDwdKeys))
	{
		CArray<T_POSP_K, T_POSP_K> arPospKeys;

		int i;
		int nSize = arDwdKeys.GetSize();

		arPospKeys.SetSize(nSize);
		for (i = 0; i < nSize; i++)
		{
			arPospKeys[i] = (T_POSP_K)arDwdKeys[i];
		}

		if (nSize > 0)
			CDBDoc::GetDocPoint()->m_pDataCtrl->DelPosp(arPospKeys);
	}
}

void CSoilPropListDlg::OnCmdModify()
{
	T_POSP_K PospK;
	T_POSP_D PospD;
	CArray<int, int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndSoilList, arRet, LVIS_SELECTED);
	if (arRet.GetSize() == 1)
	{
		CSoilPropAddModDlg dlg;
		PospK = (T_POSP_K)m_wndSoilList.GetItemData(arRet[0]);
		if (!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetPosp(PospK, PospD)) ASSERT(0);
		dlg.SetParamData(PospK, PospD);
		dlg.DoModal();
	}
}

void CSoilPropListDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnCmdModify();
	*pResult = 0;
}

void CSoilPropListDlg::OnCmdCopy()
{
	T_POSP_K PospK;
	CArray<int, int> arRet;
	if (!CDlgUtil::GetListItemByMask(&m_wndSoilList, arRet, LVIS_SELECTED))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_POSP__nChoose_one));
		return;
	}
	
	if (arRet.GetSize() == 1)
	{
		PospK = (T_POSP_K)m_wndSoilList.GetItemData(arRet[0]);
		CDBDoc::GetDocPoint()->m_pDataCtrl->CopyPosp(PospK);
	}
}