// DgnStlCalConditionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlCalConditionDlg.h" 

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlCalConditionDlg dialog

CDgnStlCalConditionDlg::CDgnStlCalConditionDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnStlCalConditionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlCalConditionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bLBuckl = TRUE;
	m_bBendSt = TRUE;
	m_bGusset = TRUE;
	m_nBendSt = 0;
	m_nGusset = 0;

	m_pDoc = CDBDoc::GetDocPoint();
}

void CDgnStlCalConditionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlCalConditionDlg)
	DDX_Control(pDX, IDC_DGN_STL_STDC_SECT_CMB,     m_cmbSection);
    DDX_Check(pDX, IDC_DGN_STL_STDC_LBUCKLING_CHK,  m_bLBuckl);
	DDX_Check(pDX, IDC_DGN_STL_STDC_BENDSTRESS_CHK, m_bBendSt);
	DDX_Check(pDX, IDC_DGN_STL_STDC_GUSSET_CHK,     m_bGusset);
	DDX_Radio(pDX, IDC_DGN_STL_STDC_BENDSTRESS_BOX_RDO, m_nBendSt);
	DDX_Radio(pDX, IDC_DGN_STL_STDC_GUSSET_LX_RDO,      m_nGusset);

	DDX_Control(pDX, IDC_DGN_STL_STDC_LST,              m_List);
	//}}AFX_DATA_MAP
}

void CDgnStlCalConditionDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

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
	case D_UPDATE_SEL_ADD: 
	case D_UPDATE_SEL_DEL:
		break;
	default: ASSERT(TRUE);
	}
}

void CDgnStlCalConditionDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount==0) return; 
	ASSERT(nCount==1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_STDC_K Key,  KeyBack;
	T_STDC_D Data, DataBack;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_STDC_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetStdc(nKey, Key, Data);
			InsertItem(Key, Data);
			break;
		case(UR_STDC_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			pViewBuff->GetStdc(nKey, Key, Data);
			DeleteItem(Key, Data);
			break;
		case(UR_STDC_MFD):
			// 여기서는 저장만 하고 MFS에서 처리한다.
			pViewBuff->GetStdc(nKey, Key, Data);
			KeyBack  = Key;
			DataBack = Data;
			bMFD = TRUE;
			break;
		case(UR_STDC_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bMFD);
			pViewBuff->GetStdc(nKey, Key, Data);
			ModifyItem(Key, KeyBack, DataBack);
			bMFD = FALSE;
			break;
		default:
			break;
		}
	}
}

void CDgnStlCalConditionDlg::InitCombo()
{
	// Evaluation Group
	m_cmbSection.ResetContent();
	
	T_SECT_D SectD; SectD.Initialize();
	CArray<T_SECT_K, T_SECT_K> aSectK;
	m_pDoc->m_pAttrCtrl->GetSectKeyList(aSectK);

	CString tStr;
	for(int i=0; i<aSectK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetSect(aSectK[i], SectD)) continue;

		tStr.Format(_T("%d : %s"), aSectK[i], SectD.SName);

		m_cmbSection.SetItemData(m_cmbSection.AddString(tStr), aSectK[i]);
	}

	m_cmbSection.SetCurSel(0);

}

void CDgnStlCalConditionDlg::Data2Dlg(T_STDC_K StdcK, T_STDC_D &Data)
{
	CArray<T_SECT_K, T_SECT_K> aSectK;
	m_pDoc->m_pAttrCtrl->GetSectKeyList(aSectK);

	for (int i = 0; i < aSectK.GetSize(); i++)
	{
		if (aSectK[i] == StdcK)
		{
			m_cmbSection.SetCurSel(i);
			break;
		}
	}

	m_bLBuckl = Data.bUpperLimitLB;
	m_bBendSt = Data.bUpperLimitBC;
	m_bGusset = Data.bGussetPlane;

	m_nBendSt = Data.nUpperLimitBC;
	m_nGusset = Data.nGussetPlane;

	UpdateData(FALSE);

	OnBendingStressChk();
	OnGussetAxisChk();
}

BOOL CDgnStlCalConditionDlg::Dlg2Data(T_STDC_K& StdcK, T_STDC_D &Data)
{
	UpdateData(TRUE);

	Data.Initialize();
	
	StdcK = m_cmbSection.GetItemData(m_cmbSection.GetCurSel());

	Data.bUpperLimitLB = m_bLBuckl;
	Data.bUpperLimitBC = m_bBendSt;
	Data.bGussetPlane  = m_bGusset;

	Data.nUpperLimitBC = m_nBendSt;
	Data.nGussetPlane  = m_nGusset;

	return TRUE;
}

void CDgnStlCalConditionDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_DGN_STDC_SECT), _LS(IDS_DGN_STDC_LB), _LS(IDS_DGN_STDC_BS), _LS(IDS_DGN_STDC_AX) };
 	int nColWidth[] = {100,80,80,80};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < 4; i++)
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

void CDgnStlCalConditionDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountStdc();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_STDC_K Key;
		T_STDC_D Data;

		int* KeyBuf=new int[nItemCount];
		nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartStdc();
		while(pos !=NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextStdc(pos, Key, Data);
			KeyBuf[nCount++] = Key;
		}

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetStdc(Key,Data);
			InsertItem(Key, Data);
		}
		delete []KeyBuf;
	}
}

int CDgnStlCalConditionDlg::FindInsertionPos(int nID)
{
	int nCount = m_List.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
        T_STDC_K key = m_List.GetItemData(i);
		if (nID < key) { return i;}
	}
	return nCount;    
}

BOOL CDgnStlCalConditionDlg::InsertItem(T_STDC_K Key, T_STDC_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i = 0; i < 4; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr(i, Key, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_List.InsertItem(&lvitem);
		}
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CDgnStlCalConditionDlg::DeleteItem(T_STDC_K Key, T_STDC_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CDgnStlCalConditionDlg::ModifyItem(T_STDC_K KeyOld, T_STDC_K Key, T_STDC_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < 4; i++)
		{
			str = DataToStr(i, Key, Data);
			m_List.SetItemText(nItem, i, str);
		}
		// key 재 설정 
		LVITEM lvitem;
		lvitem.iItem = nItem;
		lvitem.iSubItem=0;
		lvitem.mask = LVIF_PARAM;
		lvitem.lParam = (LPARAM)Key;
		m_List.SetItem(&lvitem);
	}
	return TRUE;
}

CString CDgnStlCalConditionDlg::DataToStr(int i, T_STDC_K Key, T_STDC_D &Data)
{
	CString str;

	if(i==0)
	{
		T_SECT_K SectK = Key;
		T_SECT_D SectD; SectD.Initialize();

		CString tStr;
		if (!m_pDoc->m_pAttrCtrl->GetSect(SectK, SectD)) ASSERT(0);
 		tStr.Format(_T("%d : %s"), SectK, SectD.SName);

		str = tStr;
	}
	else if(i==1)
	{
		if (!Data.bUpperLimitLB) str = _T("-");
		else  str = _LS(IDS_DGN_STDC_YES);
	}
	else if (i == 2)
	{
		if (!Data.bUpperLimitBC) str = _T("-");
		else
		{
			if (Data.nUpperLimitBC==0) 
				str = _LS(IDS_DGN_STDC_BOX);
			else
				str = _LS(IDS_DGN_STDC_PI);
		}

	}
	else if (i == 3)
	{
		if (!Data.bGussetPlane) str = _T("-");
		else
		{
			if (Data.nGussetPlane == 0)
				str = _LS(IDS_DGN_STDC_LX);
			else
				str = _LS(IDS_DGN_STDC_LY);
		}
	}
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

/////////////////////////////////////////////////////////////////////////////
// CDgnStlCalConditionDlg implementation functions
BEGIN_MESSAGE_MAP(CDgnStlCalConditionDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnStlCalConditionDlg)
	ON_CBN_SELCHANGE(IDC_DGN_STL_STDC_SECT_CMB, OnCalcSectCmb)

 	ON_BN_CLICKED(IDC_DGN_STL_STDC_BENDSTRESS_CHK, OnBendingStressChk) 
	ON_BN_CLICKED(IDC_DGN_STL_STDC_GUSSET_CHK    , OnGussetAxisChk)

	ON_BN_CLICKED(IDC_DGN_STL_STDC_ADD_BTN,               OnCalcConditionAddBtn)
	ON_BN_CLICKED(IDC_DGN_STL_STDC_MOD_BTN,               OnCalcConditionModBtn)
	ON_BN_CLICKED(IDC_DGN_STL_STDC_DEL_BTN,               OnCalcConditionDelBtn)
	ON_BN_CLICKED(IDC_DGN_STL_STDC_CLOSE_BTN,             OnCalcConditionCloseBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_STL_STDC_LST,      OnCalcConditionDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlCalConditionDlg message handlers

BOOL CDgnStlCalConditionDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitCombo();
	
	SetHeaderTitle();
	MakeItemEx();
	
	OnBendingStressChk();
	OnGussetAxisChk();

	//OnCalcSectCmb();
			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlCalConditionDlg::OnCalcSectCmb()
{
	UpdateData(TRUE);

// 	T_EVGP_K EvgpK;
// 	EvgpK = m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel());

	int n2wFixed = 0;
	int n2wMovable = 0;
	int n1wLMovable = 0;
	int n1wTMovable = 0;

	UpdateData(FALSE);
}

void CDgnStlCalConditionDlg::OnBendingStressChk()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_STL_STDC_BENDSTRESS_BOX_RDO)->EnableWindow(m_bBendSt);
	GetDlgItem(IDC_DGN_STL_STDC_BENDSTRESS_PI_RDO)->EnableWindow(m_bBendSt);
	if (!m_bBendSt) 
		m_nBendSt = 0;
}
void CDgnStlCalConditionDlg::OnGussetAxisChk()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_STL_STDC_GUSSET_LX_RDO)->EnableWindow(m_bGusset);
	GetDlgItem(IDC_DGN_STL_STDC_GUSSET_LY_RDO)->EnableWindow(m_bGusset);
	if (!m_bGusset)
		m_nGusset = 0;
}

void CDgnStlCalConditionDlg::OnCalcConditionAddBtn()
{
	T_STDC_K StdcK;
	T_STDC_D StdcD;
	if (!Dlg2Data(StdcK, StdcD)) return;

	if(!m_pDoc->m_pDataCtrl->AddStdc(StdcK, StdcD))
	{
		ASSERT(0);
		return;
	}
}

void CDgnStlCalConditionDlg::OnCalcConditionModBtn()
{
	int nItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_STDC_NO_SEL));
		return;
	}

	T_STDC_K StdcK2;
	StdcK2 = (T_STDC_K)m_List.GetItemData(nItem);

	T_STDC_K StdcK;
	T_STDC_D StdcD;
	if (!Dlg2Data(StdcK, StdcD)) return;

	if(!m_pDoc->m_pDataCtrl->ModifyStdc(StdcK, StdcK, StdcD))
	{
		ASSERT(0);
		return;
	}
}

void CDgnStlCalConditionDlg::OnCalcConditionDelBtn()
{
	int nItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_STDC_NO_SEL));
		return;
	}

	T_STDC_K StdcK;
	StdcK = (T_STDC_K)m_List.GetItemData(nItem);

	if(!m_pDoc->m_pDataCtrl->DelStdc(StdcK))
	{
		ASSERT(0);
		return;
	}
}

void CDgnStlCalConditionDlg::OnCalcConditionCloseBtn()
{
	CDialogMove::OnCancel();
}

void CDgnStlCalConditionDlg::OnCalcConditionDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	T_STDC_D StdcD;
	T_STDC_K StdcK;
	int nItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(nItem==-1)
	{
		*pResult = 0;
		return;
	}
	else
	{
		if(pNMListView->uNewState==0 || pNMListView->uNewState==1)
		{
			*pResult = 0;
			return;
		}

		nItem = pNMListView->iItem;
		StdcK = m_List.GetItemData(nItem);
		m_pDoc->m_pAttrCtrl->GetStdc(StdcK, StdcD);
	}

	Data2Dlg(StdcK, StdcD);

	*pResult = 0;
}