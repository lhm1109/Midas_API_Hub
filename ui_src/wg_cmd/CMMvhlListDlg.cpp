// CMMvhlListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlListDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"


#include "CMMvhlItemStdDlg.h"
#include "CMMvhlItemStdEuroBSDlg.h"
#include "CMMvhlItemStdEuroBSNewDlg.h"
#include "CMMvhlItemStdRusDlg.h"
#include "CMMvhlItemStdKRLRFD11Dlg.h"
#include "CMMvhlItemStdAustraliaDlg.h"
#include "CMMvhlItemStdPolandDlg.h"
#include "CMMvhlItemStdSouthAfricaDlg.h"
#include "CMMvhlItemStdIndiaDlg.h"
#include "CMMvhlItemUsrDlg.h"
#include "CMMvhlItemUsrRusDlg.h"
#include "CMMvhlItemUsrKRLRFD11Dlg.h"
#include "CMMvhlItemUsrAustraliaDlg.h"
#include "CMMvhlItemUsrPolandDlg.h"
#include "CMMvhlItemUserEuroBSDlg.h"
#include "CMMvhlItemUserEuroBSNewDlg.h"
#include "CMMvhlItemUsrSouthAfricaDlg.h"
#include "CMMvhlItemUsrIndiaDlg.h"
#include "CMMvhlItemUsrJPDlg.h"
#include "CMMvhlItemStdBSDlg.h"
#include "CMMvhlItemStdFRDlg.h"
#include "CMMvhlItemStdJPDlg.h"
#include "CMMvhlItemStdNewZealandDlg.h"
#include "CMMvhlItemUsrNewZealandDlg.h"
#include "CMMvhlItemStdBrazilDlg.h"
#include "CMMvhlItemUsrBrazilDlg.h"
#include "..\wg_base\TestEnvMgr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlListDlg dialog


CCMMvhlListDlg::CCMMvhlListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMMvhlListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlListDlg)
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	//}}AFX_DATA_MAP
}

#define COLCOUNT 2
/////////////////////////////////////////////////////////////////////////////
// CCMMvhlListDlg implementation functions
void CCMMvhlListDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_CMD_MVHL_vehicle_name), _LS(IDS_CMD_MVHL_type)};
//18japan  CString aTitle[] = {_T("Vehicle Name"), _LS(IDS_WG_CMD__ADDD__Type)};
	int nColWidth[COLCOUNT];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 120; nColWidth[1] = 90;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvhlListDlg::MakeItemEx(BOOL bUnitChanged)
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	CArray<T_MVHL_K, T_MVHL_K> aMvhlKey;
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aMvhlKey);

	int nItemCount = aMvhlKey.GetSize();
	if(nItemCount == 0) return;

	int nCount;

	if(nItemCount)
	{
		T_MVHL_K Key;
		T_MVHL_D Data;

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=aMvhlKey[nCount];
			m_pDoc->m_pAttrCtrl->GetMvhl(Key,Data);
			InsertItem(Key, Data);
		}
	}
}

void CCMMvhlListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_MVHL_K Key, KeyBak;
	T_MVHL_D Data,DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_MVHL_ADD):
			{
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetMvhl(nKey, Key, Data);
				InsertItem(Key, Data);
			}
			break;
		case(UR_MVHL_DEL):
			{
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetMvhl(nKey, Key, Data);
				DeleteItem(Key, Data);
			}
			break;
		case(UR_MVHL_MFD):
			{
				// 여기서는 저장만 하고 UR_STLD_MFS에서 처리한다.
				pViewBuff->GetMvhl(nKey, KeyBak, DataBak);
				bMFD = TRUE;
			}
			break;
		case(UR_MVHL_MFS):  // Static Load Case의 Name 변경 처리
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			{
				ASSERT(bMFD);
				pViewBuff->GetMvhl(nKey, Key, Data);
				DeleteItem(Key, Data);
				InsertItem(KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
			}
			break;
		default:
			break;
		}
	} // end of while
}

int CCMMvhlListDlg::FindInsertionPos(T_MVHL_K Key, T_MVHL_D& Data)
{
	CArray<T_MVHL_K, T_MVHL_K> aKeyList;
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aKeyList);
	int nSize = aKeyList.GetSize();
	int nIx = 0;
	int nItemCount = m_List.GetItemCount();
	for (int i = 0; i < nSize; i++)
	{
		if (nIx >= nItemCount) break;
		if (m_List.GetItemData(nIx) == aKeyList[i]) nIx++;
		else if (Key == aKeyList[i]) break;
	}
	
	return nIx;
}

BOOL CCMMvhlListDlg::InsertItem(T_MVHL_K Key, T_MVHL_D &Data)
{
	CString str;
	int nItem = FindInsertionPos(Key, Data);
	m_List.InsertItem(nItem, _T(""));
	for(int i = 0; i < COLCOUNT; i++)
	{
		str = DataToStr(i, Key, Data);
		m_List.SetItemText(nItem, i, str);
	}
	m_List.SetItemData(nItem, Key);

	return TRUE;
}

BOOL CCMMvhlListDlg::DeleteItem(T_MVHL_K Key, T_MVHL_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CCMMvhlListDlg::ModifyItem(T_MVHL_K KeyOld, T_MVHL_K Key, T_MVHL_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, Key, Data);
			m_List.SetItemText(nItem, i, str);
		}
		// key 재 설정 
		m_List.SetItemData(nItem, Key);
	}
	return TRUE;
}

CString CCMMvhlListDlg::DataToStr(int i, T_MVHL_K Key, T_MVHL_D &Data)
{
	CString str;

	if(i==0) str = Data.VehicleLoadName;
	else if(i==1) str = Data.bStandard ? _LS(IDS_WG_CMD__ADDD__Standard) : _LS(IDS_WG_CMD__ADDD__User_Designed);
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

void CCMMvhlListDlg::ModifyVehicle(T_MVHL_D& Data)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_MVCD_D MvcdD;
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) MvcdD.Initialize();

	CDialogMove* pDlg = nullptr;

	if (Data.bStandard)
	{
		if (MvcdD.nCodeType == D_MOVE_CODE_EURO_BS)
		{
			pDlg = new CCMMvhlItemStdEuroBSNewDlg;
			((CCMMvhlItemStdEuroBSNewDlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_RUSSIA)
		{
			pDlg = new CCMMvhlItemStdRusDlg;
			((CCMMvhlItemStdRusDlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_KOREA_LRFD_2011)
		{
			pDlg = new CCMMvhlItemStdKRLRFD11Dlg;
			((CCMMvhlItemStdKRLRFD11Dlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_AUSTRALIA)
		{
			pDlg = new CCMMvhlItemStdAustraliaDlg;
			((CCMMvhlItemStdAustraliaDlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_POLAND)
		{
			pDlg = new CCMMvhlItemStdPolandDlg;
			((CCMMvhlItemStdPolandDlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_SOUTH_AFRICA)
		{
			pDlg = new CCMMvhlItemStdSouthAfricaDlg;
			((CCMMvhlItemStdSouthAfricaDlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_INDIA)
		{
			pDlg = new CCMMvhlItemStdIndiaDlg;
			((CCMMvhlItemStdIndiaDlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_BS)
		{
			pDlg = new CCMMvhlItemStdBSDlg;
			((CCMMvhlItemStdBSDlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_FRANCE)
		{
			pDlg = new CCMMvhlItemStdFRDlg;
			((CCMMvhlItemStdFRDlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_JAPAN_RAIL)
		{
			pDlg = new CCMMvhlItemStdJPDlg;
			((CCMMvhlItemStdJPDlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_NEWZEALAND)
		{
			pDlg = new CCMMvhlItemStdNewZealandDlg;
			((CCMMvhlItemStdNewZealandDlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_BRAZIL)
		{
			pDlg = new CCMMvhlItemStdBrazilDlg;
			((CCMMvhlItemStdBrazilDlg*)pDlg)->SetParamData(Data);
		}
		else
		{
			pDlg = new CCMMvhlItemStdDlg;
			((CCMMvhlItemStdDlg*)pDlg)->SetParamData(Data);
		}
	}
	else
	{
		if (MvcdD.nCodeType == D_MOVE_CODE_EURO_BS)
		{
			pDlg = new CCMMvhlItemUserEuroBSNewDlg;
			((CCMMvhlItemUserEuroBSNewDlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_RUSSIA)
		{
			pDlg = new CCMMvhlItemUsrRusDlg;
			((CCMMvhlItemUsrRusDlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_KOREA_LRFD_2011)
		{
			pDlg = new CCMMvhlItemUsrKRLRFD11Dlg;
			((CCMMvhlItemUsrKRLRFD11Dlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_AUSTRALIA)
		{
			pDlg = new CCMMvhlItemUsrAustraliaDlg;
			((CCMMvhlItemUsrAustraliaDlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_POLAND)
		{
			pDlg = new CCMMvhlItemUsrPolandDlg;
			((CCMMvhlItemUsrPolandDlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_SOUTH_AFRICA)
		{
			pDlg = new CCMMvhlItemUsrSouthAfricaDlg;
			((CCMMvhlItemUsrSouthAfricaDlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_INDIA)
		{
			pDlg = new CCMMvhlItemUsrIndiaDlg;
			((CCMMvhlItemUsrIndiaDlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_JAPAN_RAIL)
		{
			pDlg = new CCMMvhlItemUsrJPDlg;
			((CCMMvhlItemUsrJPDlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_NEWZEALAND)
		{
			pDlg = new CMMvhlItemUsrNewZealandDlg;
			((CMMvhlItemUsrNewZealandDlg*)pDlg)->SetParamData(Data);
		}
		else if (MvcdD.nCodeType == D_MOVE_CODE_BRAZIL)
		{
			pDlg = new CMMvhlItemUsrBrazilDlg;
			((CMMvhlItemUsrBrazilDlg*)pDlg)->SetParamData(Data);
		}
		else
		{
			pDlg = new CCMMvhlItemUsrDlg;
			((CCMMvhlItemUsrDlg*)pDlg)->SetParamData(Data);
		}
	}

	if (pDlg == nullptr)
	{
		ASSERT(0);
		return;
	}

	pDlg->SetInitPos(D_INIT_POS_RT);
	pDlg->DoModal();

	delete pDlg;
}

void CCMMvhlListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

BEGIN_MESSAGE_MAP(CCMMvhlListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlListDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD2, OnCmdBtnAddUser)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnCmdBtnClose)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlListDlg message handlers

BOOL CCMMvhlListDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();

	SetHeaderTitle();
	MakeItemEx();

// US : 2004.02.06 고영현 차장님 지시사항 : Gen US인 경우 Standard Vehicle Load 사용하지 못하도록
// BY KYM
// MQC 11966 : 문의들어옴 > 순전히 사업상의 이유입니다.Civil을 안사고 Gen으로 교량을 설계하는 것을 방지하기 위해서입니다.
	int nMovigType = CProduct::GetMovingType();
#if defined(_MGEN)
	if(nMovigType == D_PRODUCT_MOVING_US || nMovigType == D_PRODUCT_MOVING_RUS)   
		GetDlgItem(IDC_CMD_BTN_ADD)->EnableWindow(FALSE);
#endif

	T_MVCD_D MvcdD;
	if (!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) MvcdD.Initialize();
	if (MvcdD.nCodeType == D_MOVE_CODE_FRANCE)
	{
		GetDlgItem(IDC_CMD_BTN_ADD2)->EnableWindow(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlListDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_MVCD_D MvcdD;
	if(!pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) MvcdD.Initialize();

	CDialogMove* pDlg = nullptr;
	if(MvcdD.nCodeType==D_MOVE_CODE_EURO_BS)
	{
		pDlg = new CCMMvhlItemStdEuroBSNewDlg;
	}
	else if(MvcdD.nCodeType==D_MOVE_CODE_RUSSIA)
	{
		pDlg = new CCMMvhlItemStdRusDlg;
	}
	else if(MvcdD.nCodeType==D_MOVE_CODE_KOREA_LRFD_2011)
	{
		pDlg = new CCMMvhlItemStdKRLRFD11Dlg;
	}
	else if(MvcdD.nCodeType==D_MOVE_CODE_AUSTRALIA)
	{
		pDlg = new CCMMvhlItemStdAustraliaDlg;
	}
	else if(MvcdD.nCodeType==D_MOVE_CODE_POLAND)
	{
		pDlg = new CCMMvhlItemStdPolandDlg;
	}
	else if(MvcdD.nCodeType==D_MOVE_CODE_SOUTH_AFRICA)
	{
		pDlg = new CCMMvhlItemStdSouthAfricaDlg;
	}
	else if(MvcdD.nCodeType==D_MOVE_CODE_INDIA)
	{
		pDlg = new CCMMvhlItemStdIndiaDlg;
	}
	else if(MvcdD.nCodeType==D_MOVE_CODE_BS)
	{
		pDlg = new CCMMvhlItemStdBSDlg;
	}
	else if (MvcdD.nCodeType == D_MOVE_CODE_FRANCE)
	{
		pDlg = new CCMMvhlItemStdFRDlg;
	}
	else if (MvcdD.nCodeType == D_MOVE_CODE_JAPAN_RAIL)
	{
		pDlg = new CCMMvhlItemStdJPDlg;
	}
	else if (MvcdD.nCodeType == D_MOVE_CODE_NEWZEALAND)
	{
		pDlg = new CCMMvhlItemStdNewZealandDlg;
	}
	else if (MvcdD.nCodeType == D_MOVE_CODE_BRAZIL)
	{
		pDlg = new CCMMvhlItemStdBrazilDlg;
	}
	else
	{
		pDlg = new CCMMvhlItemStdDlg;
	}

	if (pDlg == nullptr)
	{
		ASSERT(0);
		return;
	}

	pDlg->SetInitPos(D_INIT_POS_RT);
	pDlg->DoModal();
	delete pDlg;
}

void CCMMvhlListDlg::OnCmdBtnAddUser() 
{
	// TODO: Add your control notification handler code here

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_MVCD_D MvcdD;
	if(!pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) MvcdD.Initialize();

	CDialogMove* pDlg = nullptr;
	if(MvcdD.nCodeType==D_MOVE_CODE_EURO_BS)
	{
		pDlg = new CCMMvhlItemUserEuroBSNewDlg;
	}
	else if(MvcdD.nCodeType==D_MOVE_CODE_RUSSIA)
	{
		pDlg = new CCMMvhlItemUsrRusDlg;
	}
	else if(MvcdD.nCodeType==D_MOVE_CODE_KOREA_LRFD_2011)
	{
		pDlg = new CCMMvhlItemUsrKRLRFD11Dlg;
	}
	else if(MvcdD.nCodeType==D_MOVE_CODE_AUSTRALIA)
	{
		pDlg = new CCMMvhlItemUsrAustraliaDlg;
	}
	else if(MvcdD.nCodeType==D_MOVE_CODE_POLAND)
	{
		pDlg = new CCMMvhlItemUsrPolandDlg;
	}
	else if(MvcdD.nCodeType==D_MOVE_CODE_SOUTH_AFRICA)
	{
		pDlg = new CCMMvhlItemUsrSouthAfricaDlg;
	}
	else if(MvcdD.nCodeType==D_MOVE_CODE_INDIA)
	{
		pDlg = new CCMMvhlItemUsrIndiaDlg;
	}
	else if (MvcdD.nCodeType == D_MOVE_CODE_JAPAN_RAIL)
	{
		pDlg = new CCMMvhlItemUsrJPDlg;
	}
	else if (MvcdD.nCodeType == D_MOVE_CODE_NEWZEALAND)
	{
		pDlg = new CMMvhlItemUsrNewZealandDlg;
	}
	else if (MvcdD.nCodeType == D_MOVE_CODE_BRAZIL)
	{
		pDlg = new CMMvhlItemUsrBrazilDlg;
	}
	else
	{
		pDlg = new CCMMvhlItemUsrDlg;
	}

	if (pDlg == nullptr)
	{
		ASSERT(0);
		return;
	}

	pDlg->SetInitPos(D_INIT_POS_RT);
	pDlg->DoModal();

	delete pDlg;
}

void CCMMvhlListDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	T_MVHL_K Key;
	T_MVHL_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_vehicle_load_));
		return;
	}
 
	Key = (T_MVHL_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetMvhl(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Vehicle_load_key__d__does), Key);
		AfxMessageBox(msg);
		return;
	}

	ModifyVehicle(Data);

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);		
}

void CCMMvhlListDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_vehicle_load_));
		return;
	}
	CString csName = m_List.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelMvhl(csName)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMMvhlListDlg::OnCmdBtnClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}

void CCMMvhlListDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	T_MVHL_K Key;
	T_MVHL_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return;  // no message, just return
 
	Key = (T_MVHL_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetMvhl(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Vehicle_load_key__d__does), Key);
		AfxMessageBox(msg);
		return;
	}

	ModifyVehicle(Data);

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);		
}
