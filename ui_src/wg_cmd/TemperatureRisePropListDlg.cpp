// CMTimeDefMatTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "TemperatureRisePropListDlg.h"
#include "TemperatureRisePropItemDlg.h"  

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTemperatureRisePropListDlg dialog
#define CDialog CDialogMove

CTemperatureRisePropListDlg::CTemperatureRisePropListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTemperatureRisePropListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTemperatureRisePropListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}

void CTemperatureRisePropListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTemperatureRisePropListDlg)
	DDX_Control(pDX, IDC_CMD_TEMPERATURE_RISE_PROP_LIST, m_wndList);
	//}}AFX_DATA_MAP
}

void CTemperatureRisePropListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

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
		//    UnitChanged();
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

void CTemperatureRisePropListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_MHTP_K Key, KeyBak;
	T_MHTP_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_MHTP_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetMhtp(nKey, Key, Data);
			InsertItem(Key, Data);
			break;
		case(UR_MHTP_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			pViewBuff->GetMhtp(nKey, Key, Data);
			DeleteItem(Key, Data);
			break;
		case(UR_MHTP_MFD):
			// 여기서는 저장만 하고 UR_TDME_MFS에서 처리한다.
			pViewBuff->GetMhtp(nKey, Key, Data);
			KeyBak = Key;
			DataBak = Data;
			bMFD = TRUE;
			break;
		case(UR_MHTP_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bMFD);
			pViewBuff->GetMhtp(nKey, Key, Data);
			ModifyItem(Key, KeyBak, DataBak);
			// 플래그 수정
			bMFD = FALSE;
			break;
		default:
			break;
		}
	} // end of while
}

BOOL CTemperatureRisePropListDlg::InsertItem(T_MHTP_K Key, T_MHTP_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_wndList.GetItemCount();
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iItem = nItem;
		lvitem.iSubItem=i;
		str = DataToStr(i, Key, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_wndList.InsertItem(&lvitem);
		}
		else m_wndList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	return TRUE;
}

CString CTemperatureRisePropListDlg::DataToStr(int i, T_MHTP_K Key, T_MHTP_D &Data)
{
	CString str;
	CString csFuncType[] = { _LS(IDS_CMD_HMTP_code), _LS(IDS_CMD_HMTP_user) };

	ASSERT(Data.nMatlType >= 1 && Data.nMatlType <= 2);

	if(i==0) str = Data.strMatlName;
	else if(i==1) str = csFuncType[Data.nMatlType-1];
	else str = _LS(IDS_CMD_HMTP_error);

	return str;
}

BOOL CTemperatureRisePropListDlg::DeleteItem(T_MHTP_K Key, T_MHTP_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_wndList.FindItem(&FindInfo);
	if (nItem != -1) m_wndList.DeleteItem(nItem);
	return TRUE;
}

BOOL CTemperatureRisePropListDlg::ModifyItem(T_MHTP_K KeyOld, T_MHTP_K Key, T_MHTP_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_wndList.FindItem(&FindInfo);
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, Key, Data);
			m_wndList.SetItemText(nItem, i, str);
		}
		// key 재 설정 
		LVITEM lvitem;
		lvitem.iItem = nItem;
		lvitem.iSubItem=0;
		lvitem.mask = LVIF_PARAM;
		lvitem.lParam = (LPARAM)Key;
		m_wndList.SetItem(&lvitem);
	}
	return TRUE;
}

BEGIN_MESSAGE_MAP(CTemperatureRisePropListDlg, CDialog)
	//{{AFX_MSG_MAP(CTemperatureRisePropListDlg)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	ON_BN_CLICKED(IDC_CMD_ADD, OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_MODIFY, OnCmdModify)
	ON_BN_CLICKED(IDC_CMD_DELETE, OnCmdDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_TEMPERATURE_RISE_PROP_LIST, OnDblclkCmdList)
	ON_BN_CLICKED(IDC_CMD_COPY, OnCmdCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTemperatureRisePropListDlg message handlers
BOOL CTemperatureRisePropListDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialog::OnInitDialog();
	
	SetHeaderTitle(TRUE);
	MakeItemEx();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTemperatureRisePropListDlg::SetHeaderTitle(BOOL bInit)
{
	CString aTitle[] = { _LS(IDS_CMD_HMTP_name), _LS(IDS_CMD_HMTP_type) };
	//18japan  CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Name), _LS(IDS_WG_CMD__ADDD__Type)};
	int nColWidth[COLCOUNT];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	if (bInit)
	{
		DWORD dwStyle;
		dwStyle = ListView_GetExtendedListViewStyle(m_wndList.GetSafeHwnd());
		dwStyle |= LVS_EX_FULLROWSELECT; 
		ListView_SetExtendedListViewStyle(m_wndList.GetSafeHwnd(), dwStyle);
	}

	// Calculate width for each column
	nColWidth[0] = 100;
	nColWidth[1] = 100;

	// Set Title
	for(i=0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		if (bInit)
			m_wndList.InsertColumn(i,&lvcolumn);
		m_wndList.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CTemperatureRisePropListDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_wndList.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountMhtp();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;
	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_MHTP_K Key;
		T_MHTP_D Data;

		int* KeyBuf=new int[nItemCount];
		nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartMhtp();
		while(pos !=NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextMhtp(pos, Key, Data);
			KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_MHTP_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			Key = KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetMhtp(Key, Data);
			for(int i = 0; i < COLCOUNT; i++)
			{
				lvitem.iItem=nCount;
				lvitem.iSubItem=i;
				str = DataToStr(i, Key, Data);
				lvitem.pszText=str.GetBuffer(0);
				lvitem.mask=LVIF_TEXT ;
				if( i == 0 )
				{
					lvitem.mask |= LVIF_PARAM;
					lvitem.lParam = (LPARAM)Key;
					m_wndList.InsertItem(&lvitem);
				}
				else
					m_wndList.SetItem(&lvitem);
				str.ReleaseBuffer();
			}
		}
		delete []KeyBuf;
	}
}

void CTemperatureRisePropListDlg::OnCmdClose() 
{
	CDialogMove::OnOK();
}

void CTemperatureRisePropListDlg::OnCmdAdd() 
{
	// TODO: Add your control notification handler code here
	CTemperatureRisePropItemDlg Dlg;
	Dlg.DoModal();
	
}

void CTemperatureRisePropListDlg::OnCmdDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_wndList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_Material_type__nChoos));
		return;
	}
	CString csName = m_wndList.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelMhtp(csName)) return;

	int nCount = m_wndList.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_wndList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CTemperatureRisePropListDlg::OnCmdModify() 
{
	// TODO: Add your control notification handler code here
	T_MHTP_K Key;
	T_MHTP_D Data;

	int iItem = m_wndList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_material_type__nChoos));
		return;
	}

	Key = (T_MHTP_K)m_wndList.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetMhtp(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Material_type_key__d_does_not_exi), Key);
		AfxMessageBox(msg);
		return;
	}

	CTemperatureRisePropItemDlg dlg;
	dlg.SetParamData(Data);
	//  dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_wndList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CTemperatureRisePropListDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCmdModify(); 
	*pResult = 0;
}

void CTemperatureRisePropListDlg::OnCmdCopy() 
{
	int iItem = m_wndList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_Material_type__nChoos));
		return;
	}
	CString csName = m_wndList.GetItemText(iItem, 0);

	CString _tcscpyName = GetCopyName(csName);

	T_MHTP_D MhtpSrc, MhtpCpy;
	T_MHTP_K Key;

	Key = (T_MHTP_K)m_wndList.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetMhtp(Key, MhtpSrc)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Material_type_key__d_does_not_exi), Key);
		AfxMessageBox(msg);
		return;
	}

	MhtpCpy = MhtpSrc;
	MhtpCpy.strMatlName = _tcscpyName;

	m_pDoc->m_pDataCtrl->AddMhtp(MhtpCpy);
}

CString CTemperatureRisePropListDlg::GetCopyName(CString strOrigin)
{
	CString str = _T("");
	int nIndex = strOrigin.ReverseFind('_');
	
	if (nIndex+1 == strOrigin.GetLength())
	{
		str = strOrigin + _T("1");
	}
	else if (nIndex == -1)
	{
		str = strOrigin + _T("_1");
	}
	else
	{

		BOOL bNumeric = TRUE;
		char cha;
		for (int i=nIndex+1; i<strOrigin.GetLength(); i++)
		{
			cha = strOrigin.GetAt(i);
			if (cha < '0' || cha > '9')
			{
				bNumeric = FALSE;
				break;
			}
		}

		if (bNumeric)
		{
			int nNum = _ttoi(strOrigin.Right(strOrigin.GetLength() - nIndex - 1));
			nNum++;
			CString tmp;
			tmp.Format(_T("_%d"), nNum);
			str = strOrigin.Left(nIndex) + tmp;
		}
		else
		{
			str = strOrigin + _T("_1");
		}
	}

	nIndex = str.ReverseFind('_');
	CString strHead = str.Left(nIndex+1);
	CString strTail;
	int nNum;

	while(m_pDoc->m_pAttrCtrl->ExistTdmt(str))
	{
		nNum = _ttoi(str.Right(str.GetLength() - nIndex - 1));
		nNum++;
		strTail.Format(_T("%d"), nNum);
		str = strHead + strTail;
	}

	return str;
}
