#include "stdafx.h"
#include "wg_cmd.h"
#include "SpfcListDlg.h"
#include "SpfcItemDlg.h"
#include "SpfcEnvelopeDlg.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_CompFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSpfcListDlg::CSpfcListDlg(CWnd* pParent)
	: CDialogMove(CSpfcListDlg::IDD, pParent)
{
}

void CSpfcListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPFC_LIST, m_List);
}

#define COLCOUNT 2

void CSpfcListDlg::SetHeaderTitle(BOOL bInit)
{
	if ( bInit )
	{
		DWORD dwStyle;
		dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
		dwStyle |= LVS_EX_FULLROWSELECT;
		ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);
	}

	// Calculate width for each column
	int nColWidth[COLCOUNT] = { 0, };
	nColWidth[0] = 160;
	nColWidth[1] = 68;

	// Set Title
	CString aTitle[] = { _LS(IDS_CMD_SPFC_spectrum_name), _LS(IDS_CMD_SPFC_type) };
	for ( int i = 0; i < COLCOUNT; i++ )
	{
		CString title = aTitle[i];
		// 제일 처음 시작일 때면 Column 추가
		// 아니면 Column Title만 변경
		LV_COLUMN lvcolumn;
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		if ( bInit )
		{
			m_List.InsertColumn(i, &lvcolumn);
		}
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CSpfcListDlg::MakeItemEx(BOOL bUnitChanged)
{
	CWaitCursor Cursor;

	if ( !bUnitChanged )
	{
		m_List.DeleteAllItems();
	}

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountSpfc();
	if ( nItemCount == 0 ) return;

	int* KeyBuf = new int[nItemCount];
	int nCount = 0;
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartSpfc();
	while ( pos != NULL )
	{
		T_SPFC_K Key = 0;
		T_SPFC_D Data;
		m_pDoc->m_pAttrCtrl->GetNextSpfc(pos, Key, Data);
		KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nItemCount, sizeof(T_SPFC_K), CCompFunc::UINTAsc);

	for ( nCount = 0; nCount < nItemCount; nCount++ )
	{
		T_SPFC_K Key = KeyBuf[nCount];
		T_SPFC_D Data;
		m_pDoc->m_pAttrCtrl->GetSpfc(Key, Data);
		for ( int i = 0; i < COLCOUNT; i++ )
		{
			LV_ITEM lvitem;
			lvitem.iItem = nCount;
			lvitem.iSubItem = i;
			CString str = DataToStr(i, Key, Data);
			lvitem.pszText=str.GetBuffer(0);
			lvitem.mask=LVIF_TEXT;
			if ( !bUnitChanged )
			{
				if ( i == 0 )
				{
					lvitem.mask |= LVIF_PARAM;
					lvitem.lParam = ( LPARAM ) Key;
					m_List.InsertItem(&lvitem);
				}
				else
				{
					m_List.SetItem(&lvitem);
				}
			}
			else
			{
				m_List.SetItemText(lvitem.iItem, lvitem.iSubItem, lvitem.pszText);
			}
			str.ReleaseBuffer();
		}
	}
	delete[] KeyBuf;
}

void CSpfcListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if ( nCount == 0 ) return;
	ASSERT(nCount == 1);

	T_SPFC_K Key, KeyBak;
	T_SPFC_D Data, DataBak;
	BOOL bMFD = FALSE;

	POSITION pos = pViewBuff->GetStartBuffer();
	while ( pos != nullptr )
	{
		T_UDRD_BUFFER buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch ( nCmd )
		{
		case UR_SPFC_ADD:
			{
				pViewBuff->GetSpfc(nKey, Key, Data);
				InsertItem(Key, Data);
				SetEnbleEnvelope();
			}
			break;
		case UR_SPFC_DEL:
			{
				pViewBuff->GetSpfc(nKey, Key, Data);
				DeleteItem(Key, Data);
				SetEnbleEnvelope();
			}
			break;
		case UR_SPFC_MFD:
			{
				pViewBuff->GetSpfc(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
			}
			break;
		case UR_SPFC_MFS:
			{
				ASSERT(bMFD);
				pViewBuff->GetSpfc(nKey, Key, Data);
				ModifyItem(Key, KeyBak, DataBak);
				bMFD = FALSE;
			}
			break;
		default:
			break;
		}
	} // end of while
}

void CSpfcListDlg::UnitChanged()
{
	// unit 변경과 관련 없다.
	return;
}

BOOL CSpfcListDlg::InsertItem(T_SPFC_K Key, T_SPFC_D &Data)
{
	int nItem = m_List.GetItemCount();
	for ( int i = 0; i < COLCOUNT; i++ )
	{
		LVITEM lvitem;
		lvitem.iItem = nItem;
		lvitem.iSubItem=i;

		CString str = DataToStr(i, Key, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if ( i == 0 )
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = ( LPARAM ) Key;
			nItem = m_List.InsertItem(&lvitem);
		}
		else
		{
			m_List.SetItem(&lvitem);
		}
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CSpfcListDlg::DeleteItem(T_SPFC_K Key, T_SPFC_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = ( LPARAM ) Key;
	int nItem = m_List.FindItem(&FindInfo);
	if ( nItem != -1 )
	{
		m_List.DeleteItem(nItem);
	}
	return TRUE;
}

BOOL CSpfcListDlg::ModifyItem(T_SPFC_K KeyOld, T_SPFC_K Key, T_SPFC_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags  = LVFI_PARAM;
	FindInfo.lParam = ( LPARAM ) Key;

	int nItem = m_List.FindItem(&FindInfo);
	if ( nItem != -1 )
	{
		for ( int i = 0; i < COLCOUNT; i++ )
		{
			CString str = DataToStr(i, Key, Data);
			m_List.SetItemText(nItem, i, str);
		}
		// key 재 설정
		LVITEM lvitem;
		lvitem.iItem    = nItem;
		lvitem.iSubItem = 0;
		lvitem.mask     = LVIF_PARAM;
		lvitem.lParam   = ( LPARAM ) Key;
		m_List.SetItem(&lvitem);
	}
	return TRUE;
}

CString CSpfcListDlg::DataToStr(int i, T_SPFC_K Key, T_SPFC_D &Data)
{
	CString csType[] =
	{
		_LS(IDS_WG_CMD__ADD2__Norm),
		_LS(IDS_WG_CMD__ADD2__Acc),
		_LS(IDS_WG_CMD__ADD2__Vel),
		_LS(IDS_WG_CMD__ADD2__Disp)
	};
	ASSERT(Data.nSpecFuncType >= 1 && Data.nSpecFuncType <= 4);

	switch ( i )
	{
	case  0: return Data.SpecFuncName;
	case  1: return csType[Data.nSpecFuncType - 1];
	default: return _LS(IDS_WG_CMD__ADDD__Error);
	}
}

BEGIN_MESSAGE_MAP(CSpfcListDlg, CDialogMove)
	ON_BN_CLICKED(IDC_SPFC_BTN_ADD, OnSpfcBtnAdd)
	ON_BN_CLICKED(IDC_SPFC_BTN_MODIFY, OnSpfcBtnModify)
	ON_BN_CLICKED(IDC_SPFC_BTN_ENVELOPE, OnSpfcBtnEnvelope)
	ON_BN_CLICKED(IDC_SPFC_BTN_DELETE, OnSpfcBtnDelete)
	ON_BN_CLICKED(IDC_SPFC_BTN_CLOSE, OnSpfcBtnClose)
	ON_NOTIFY(NM_DBLCLK, IDC_SPFC_LIST, OnDblclkSpfcList)
END_MESSAGE_MAP()

BOOL CSpfcListDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();
	SetHeaderTitle(TRUE);
	MakeItemEx();
	SetEnbleEnvelope();
	return TRUE;
}

void CSpfcListDlg::OnSpfcBtnAdd()
{
	CSpfcItemDlg dlg;
	dlg.SetSeismicEvaluation(m_bSeismicEvaluation);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();
}

void CSpfcListDlg::OnSpfcBtnModify()
{
	int iItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if ( iItem == -1 )
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_spectrum_function__nC));
		return;
	}

	T_SPFC_K Key = ( T_SPFC_K ) m_List.GetItemData(iItem);
	T_SPFC_D Data;
	if ( !m_pDoc->m_pAttrCtrl->GetSpfc(Key, Data) )
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Spectrum_function_key__d_does_not), Key);
		AfxMessageBox(msg);
		return;
	}

	CSpfcItemDlg dlg;
	dlg.SetParamData(Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CSpfcListDlg::OnSpfcBtnDelete()
{
	int iItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if ( iItem == -1 )
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_spectrum_function__nC));
		return;
	}
	CString csName = m_List.GetItemText(iItem, 0);
	if ( !m_pDoc->m_pDataCtrl->DelSpfc(csName) ) return;

	int nCount = m_List.GetItemCount();
	if ( nCount <= iItem ) iItem -= 1;
	if ( iItem >= 0 )
	{
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CSpfcListDlg::OnDblclkSpfcList(NMHDR* pNMHDR, LRESULT* pResult)
{
	int iItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if ( iItem == -1 ) return;

	T_SPFC_K Key = ( T_SPFC_K ) m_List.GetItemData(iItem);
	T_SPFC_D Data;
	if ( !m_pDoc->m_pAttrCtrl->GetSpfc(Key, Data) ) return;

	CString csOldName = Data.SpecFuncName;
	CSpfcItemDlg dlg;
	dlg.SetParamData(Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);

	*pResult = 0;
}

void CSpfcListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if ( hWnd == nullptr || !IsWindow(hWnd) ) return;

	switch ( lHint )
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	case D_UPDATE_BUFFER_AFTER:
		{
			UpdateBuffer();
		}
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_UNIT:
		{
			UnitChanged();
		}
		break;
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
	case D_UPDATE_EXE_START:
	case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(0);
		break;
	}
}

void CSpfcListDlg::OnSpfcBtnClose()
{
	CDialogMove::OnOK();
}

void CSpfcListDlg::SetEnbleEnvelope()
{
	int nSpfc = m_pDoc->m_pAttrCtrl->GetCountSpfc();
	bool bEnable = ( nSpfc > 1 ) ? true : false;
	GetDlgItem(IDC_SPFC_BTN_ENVELOPE)->EnableWindow(bEnable);
}

void CSpfcListDlg::OnSpfcBtnEnvelope()
{
	CSpfcEnvelopeDlg EnvDlg;
	EnvDlg.SetInitPos(D_INIT_POS_RT);
	if ( IDOK == EnvDlg.DoModal() )
	{
	}
}