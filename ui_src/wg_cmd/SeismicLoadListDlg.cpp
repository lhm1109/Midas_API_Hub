// SeismicLoadListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeismicLoadListDlg.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "SeismicLoadProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeismicLoadListDlg dialog
#define CDialog CDialogMove


CSeismicLoadListDlg::CSeismicLoadListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSeismicLoadListDlg::IDD, pParent)
{
}


void CSeismicLoadListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_LIST, m_wndSeismicLoadList);
}


BEGIN_MESSAGE_MAP(CSeismicLoadListDlg, CDialog)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnCancel)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkList)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeismicLoadListDlg message handlers
BOOL CSeismicLoadListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	SetListHeader();
	MakeListData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeismicLoadListDlg::SetListHeader()
{
	CStringArray HTitles;
	CArray<float, float> HRatio;

	HTitles.Add(_LS(IDS_WG_CMD__ADDD__Name));
	HTitles.Add(_LS(IDS_WG_CMD__ADD2__Code_Name));
	HTitles.Add(_LS(IDS_WG_CMD__ADDD__Method));
	HRatio.Add(0.2f);
	HRatio.Add(0.4f);
	HRatio.Add(0.4f);

	CDlgUtil::_SetListCtrlHeader(&m_wndSeismicLoadList, HTitles, &HRatio, NULL);
}


void CSeismicLoadListDlg::MakeListData()
{
	int Index = 0;

	CStringArray Contents;
	Contents.SetSize(3);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	m_wndSeismicLoadList.DeleteAllItems();

	T_POSL_D PoslD;
	CArray<T_POSL_K, T_POSL_K> caKeyList;
	pDoc->m_pAttrCtrl->GetPoslKeyList(caKeyList);
	int nSize = caKeyList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (!pDoc->m_pAttrCtrl->GetPosl(caKeyList[i], PoslD))
		{
			ASSERT(0);
			continue;
		}
		Contents[0] = PoslD.strName;
		Contents[1] = pDoc->m_pAttrCtrl->GetPoslCodeName(PoslD.nCodeType);
#if defined(_CIVIL)
		Contents[2] = _LS(IDS_DB_EP_SEISMIC_LOAD_RESPONSE); 
		CDlgUtil::SetListItem(&m_wndSeismicLoadList, Index++, Contents, caKeyList[i]);
#else
		Contents[2] = pDoc->m_pAttrCtrl->GetPoslMethodName(PoslD.KDS2019.nEPSEMethod);
		CDlgUtil::SetListItem(&m_wndSeismicLoadList, Index++, Contents, caKeyList[i]);
#endif
	}
}

void CSeismicLoadListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
void CSeismicLoadListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsPoslCommand = FALSE;

	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_POSL_ADD):
		case(UR_POSL_DEL):
		case(UR_POSL_MFD):
		case(UR_POSL_MFS):
			bIsPoslCommand = TRUE;
			break;
		default:
			break;
		}
	}

	if (bIsPoslCommand)
	{
		MakeListData();
	}
}


void CSeismicLoadListDlg::OnCmdAdd()
{
	CSeismicLoadProperty dlg;
	dlg.DoModal();
}

void CSeismicLoadListDlg::OnCmdDelete()
{
	CArray<DWORD, DWORD> arDwdKeys;

	if (CDlgUtil::GetSelectedListItemData(&m_wndSeismicLoadList, arDwdKeys))
	{
		CArray<T_POSL_K, T_POSL_K> arPoslKeys;

		int i;
		int nSize = arDwdKeys.GetSize();

		arPoslKeys.SetSize(nSize);
		for (i = 0; i < nSize; i++)
		{
			arPoslKeys[i] = (T_POSL_K)arDwdKeys[i];
		}

		if (nSize > 0)
			CDBDoc::GetDocPoint()->m_pDataCtrl->DelPosl(arPoslKeys);
	}
}

void CSeismicLoadListDlg::OnCmdModify()
{
	T_POSL_K PoslK;
	T_POSL_D PoslD;
	CArray<int, int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndSeismicLoadList, arRet, LVIS_SELECTED);
	if (arRet.GetSize() == 1)
	{
		CSeismicLoadProperty dlg;
		PoslK = (T_POSL_K)m_wndSeismicLoadList.GetItemData(arRet[0]);
		if (!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetPosl(PoslK, PoslD)) ASSERT(0);
 		dlg.SetParamData(PoslK, PoslD);
		dlg.DoModal();
	}
}

void CSeismicLoadListDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnCmdModify();
	*pResult = 0;
}

