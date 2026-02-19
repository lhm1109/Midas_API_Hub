// DgnSeisBearingEvalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisBearingEvalDlg.h"

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
// CDgnSeisBearingEvalDlg dialog

CDgnSeisBearingEvalDlg::CDgnSeisBearingEvalDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisBearingEvalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisBearingEvalDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//m_nBearing = 0;

	m_pDoc = CDBDoc::GetDocPoint();
}

void CDgnSeisBearingEvalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisBearingEvalDlg)
 	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_NAME_EDT,         m_edtName);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_GROUP_CMB,        m_cmbEvalGrup);

	//
// 	DDX_Radio  (pDX, IDC_DGN_SEIS_BEARING_EVAL_BEARING_RDO1,     m_nBearing);
// 	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_STL_BEARING_CMB,  m_cmbStlBearing);
// 	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_DS_EDT,           m_edtds);
// 	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_DS_UNT,           m_untds);
// 	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_CAPA_L_EDT,       m_edtCapaL);
// 	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_CAPA_L_UNT,       m_untCapaL);
// 	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_CAPA_T_EDT,       m_edtCapaT);
// 	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_CAPA_T_UNT,       m_untCapaT);
// 	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_DISP_L_EDT,       m_edtDispL);
// 	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_DISP_L_UNT,       m_untDispL);
// 	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_DISP_T_EDT,       m_edtDispT);
// 	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_DISP_T_UNT,       m_untDispT);
	//

	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_2W_FIX_EDT,       m_edt2wFix);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_2W_MOV_EDT,       m_edt2wMov);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_1W_L_MOV_EDT,     m_edt1wLMov);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_1W_T_MOV_EDT,     m_edt1wTMov);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_LST,              m_List);
	//}}AFX_DATA_MAP
}

void CDgnSeisBearingEvalDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnSeisBearingEvalDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount==0) return; 
	ASSERT(nCount==1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_BREV_K Key,  KeyBack;
	T_BREV_D Data, DataBack;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_BREV_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetBrev(nKey, Key, Data);
			InsertItem(Key, Data);
			break;
		case(UR_BREV_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			pViewBuff->GetBrev(nKey, Key, Data);
			DeleteItem(Key, Data);
			break;
		case(UR_BREV_MFD):
			// 여기서는 저장만 하고 MFS에서 처리한다.
			pViewBuff->GetBrev(nKey, Key, Data);
			KeyBack  = Key;
			DataBack = Data;
			bMFD = TRUE;
			break;
		case(UR_BREV_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bMFD);
			pViewBuff->GetBrev(nKey, Key, Data);
			ModifyItem(Key, KeyBack, DataBack);
			bMFD = FALSE;
			break;
		default:
			break;
		}
	}
}

void CDgnSeisBearingEvalDlg::InitUnit()
{
// 	m_edtds.SetUnitType(CUnitCtrl::m_BREV_UNIT.dds);
// 	m_untds.SetUnitType(CUnitCtrl::m_BREV_UNIT.dds);
// 	m_edtCapaL.SetUnitType(CUnitCtrl::m_BREV_UNIT.dCapaL);
// 	m_untCapaL.SetUnitType(CUnitCtrl::m_BREV_UNIT.dCapaL);
// 	m_edtCapaT.SetUnitType(CUnitCtrl::m_BREV_UNIT.dCapaT);
// 	m_untCapaT.SetUnitType(CUnitCtrl::m_BREV_UNIT.dCapaT);
// 	m_edtDispL.SetUnitType(CUnitCtrl::m_BREV_UNIT.dDispL);
// 	m_untDispL.SetUnitType(CUnitCtrl::m_BREV_UNIT.dDispL);
// 	m_edtDispT.SetUnitType(CUnitCtrl::m_BREV_UNIT.dDispT);
// 	m_untDispT.SetUnitType(CUnitCtrl::m_BREV_UNIT.dDispT);

	m_edt2wFix.SetUnitType(D_UNITSYS_NONE);
	m_edt2wMov.SetUnitType(D_UNITSYS_NONE);
	m_edt1wLMov.SetUnitType(D_UNITSYS_NONE);
	m_edt1wTMov.SetUnitType(D_UNITSYS_NONE);
}

void CDgnSeisBearingEvalDlg::InitCombo()
{
	// Evaluation Group
	m_cmbEvalGrup.ResetContent();
	
	T_EVGP_D EvgpD; EvgpD.Initialize();
	CArray<T_EVGP_K, T_EVGP_K> aEvgpK;
	m_pDoc->m_pAttrCtrl2->GetEvgpKeyList(aEvgpK);

	for(int i=0; i<aEvgpK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetEvgp(aEvgpK[i], EvgpD)) continue;
		m_cmbEvalGrup.SetItemData(m_cmbEvalGrup.AddString(EvgpD.strName), aEvgpK[i]);
	}

	m_cmbEvalGrup.SetCurSel(0);

// 	// 강재 받침 
// 	m_cmbStlBearing.ResetContent();
// 	m_cmbStlBearing.SetItemData(m_cmbStlBearing.AddString(_LS(IDS_DGN_SEIS_BEARING_EVAL_STL_BEARING_TYPE1)), 0);
// 	m_cmbStlBearing.SetItemData(m_cmbStlBearing.AddString(_LS(IDS_DGN_SEIS_BEARING_EVAL_STL_BEARING_TYPE2)), 1);
// 	m_cmbStlBearing.SetItemData(m_cmbStlBearing.AddString(_LS(IDS_DGN_SEIS_BEARING_EVAL_STL_BEARING_TYPE3)), 2);
// 	m_cmbStlBearing.SetCurSel(0);
}

void CDgnSeisBearingEvalDlg::EnableDisableControls(int nItem)
{
	UpdateData(TRUE);
	if(nItem == -1) return;
	T_BREV_K BrevK = m_List.GetItemData(nItem);
	
	if (!CheckData(BrevK))
	{
		GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_MOD_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DEL_BTN)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_MOD_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DEL_BTN)->EnableWindow(TRUE);
	}

// 	int nStlBearing = m_cmbStlBearing.GetItemData(m_cmbStlBearing.GetCurSel());
// 
// 	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_STL_BEARING_CMB)->EnableWindow(m_nBearing == 0);
// 
// 	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DS_STC)->EnableWindow(m_nBearing == 0 && nStlBearing == 2);
// 	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DS_EDT)->EnableWindow(m_nBearing == 0 && nStlBearing == 2);
// 	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DS_UNT)->EnableWindow(m_nBearing == 0 && nStlBearing == 2);
// 
// 	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DISP_GRP)->EnableWindow(m_nBearing == 1);
// 	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DISP_L_STC)->EnableWindow(m_nBearing == 1);
// 	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DISP_L_EDT)->EnableWindow(m_nBearing == 1);
// 	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DISP_L_UNT)->EnableWindow(m_nBearing == 1);
// 	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DISP_T_STC)->EnableWindow(m_nBearing == 1);
// 	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DISP_T_EDT)->EnableWindow(m_nBearing == 1);
// 	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DISP_T_UNT)->EnableWindow(m_nBearing == 1);
}

void CDgnSeisBearingEvalDlg::Data2Dlg(T_BREV_D &Data)
{
	m_edtName.SetWindowText(Data.strName);
	
	CArray<T_EVGP_K, T_EVGP_K> aEvgpK;
	m_pDoc->m_pAttrCtrl2->GetEvgpKeyList(aEvgpK);

	for(int i=0; i<aEvgpK.GetSize(); i++)
	{
		if(aEvgpK[i] == Data.EvgpK) 
		{ 
			m_cmbEvalGrup.SetCurSel(i); 
			break; 
		}
	}
// 	//
// 	m_nBearing = Data.nBearing;
// 	m_cmbStlBearing.SetCurSel(Data.nStlBearing);
// 	m_edtds.SetEditUnit(Data.dds);
// 	m_edtCapaL.SetEditUnit(Data.dCapaL);
// 	m_edtCapaT.SetEditUnit(Data.dCapaT);
// 	m_edtDispL.SetEditUnit(Data.dDispL);
// 	m_edtDispT.SetEditUnit(Data.dDispT);
// 	//
	m_edt2wFix.SetEditUnit(Data.n2wFixed);
	m_edt2wMov.SetEditUnit(Data.n2wMovable);
	m_edt1wLMov.SetEditUnit(Data.n1wLMovable);
	m_edt1wTMov.SetEditUnit(Data.n1wTMovable);
	
	UpdateData(FALSE);
}

BOOL CDgnSeisBearingEvalDlg::Dlg2Data(T_BREV_D &Data)
{
	UpdateData(TRUE);

	Data.Initialize();
	
	m_edtName.GetWindowText(Data.strName);
	Data.EvgpK = m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel());
	//
// 	Data.nBearing = m_nBearing;
// 	Data.nStlBearing = m_cmbStlBearing.GetItemData(m_cmbStlBearing.GetCurSel());	
// 	Data.dds = m_edtds.GetEditValue();
// 	Data.dCapaL = m_edtCapaL.GetEditValue();
// 	Data.dCapaT = m_edtCapaT.GetEditValue();
// 	Data.dDispL = m_edtDispL.GetEditValue();
// 	Data.dDispT = m_edtDispT.GetEditValue();
	//
	Data.n2wFixed = m_edt2wFix.GetEditValue();
	Data.n2wMovable = m_edt2wMov.GetEditValue();
	Data.n1wLMovable = m_edt1wLMov.GetEditValue();
	Data.n1wTMovable = m_edt1wTMov.GetEditValue();

	int nTotaldlg = Data.n2wFixed + Data.n2wMovable + Data.n1wLMovable + Data.n1wTMovable;
	//////////////////////////////////////////////////////////////////////////
	// Check
	T_EVGP_D EvgpD; EvgpD.Initialize();
	if (!m_pDoc->m_pAttrCtrl2->GetEvgp(Data.EvgpK, EvgpD)) EvgpD.Initialize();

	int n2wFixed = 0;
	int n2wMovable = 0;
	int n1wLMovable = 0;
	int n1wTMovable = 0;

	m_pDoc->m_pAttrCtrl2->GetBearingNum(Data.EvgpK, 0, n2wFixed, n2wMovable, n1wLMovable, n1wTMovable);
	int nTotal = n2wFixed + n2wMovable + n1wLMovable + n1wTMovable;
	

	if(nTotaldlg != nTotal)
	{
		CString msg;
		msg.Format(_LS(IDS_DGN_SEIS_BEARING_EVAL_NUM_ERR), nTotaldlg, nTotal);
		AfxMessageBox(msg);
		return FALSE;
	}

	return TRUE;
}

void CDgnSeisBearingEvalDlg::UpdateName()
{
	CString strName;
	m_cmbEvalGrup.GetWindowText(strName);
	
	m_edtName.SetWindowText(strName);

	UpdateData(FALSE);
}

void CDgnSeisBearingEvalDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_DGN_SEIS_BEARING_EVAL_NAME), _LS(IDS_DGN_SEIS_BEARING_EVAL_EVGP), _T("양방향 고정단"), _T("양방향 가동단"), _T("교축 가동단"), _T("교직 가동단")};
 	int nColWidth[] = {65, 120, 100,100,100,100};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < 6; i++)
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

void CDgnSeisBearingEvalDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl2->GetCountBrev();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_BREV_K Key;
		T_BREV_D Data;

		int* KeyBuf=new int[nItemCount];
		nCount = 0;
		pos = m_pDoc->m_pAttrCtrl2->GetStartBrev();
		while(pos !=NULL)
		{
			m_pDoc->m_pAttrCtrl2->GetNextBrev(pos, Key, Data);
			KeyBuf[nCount++] = Key;
		}

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl2->GetBrev(Key,Data);
			InsertItem(Key, Data);
		}
		delete []KeyBuf;
	}
}

int CDgnSeisBearingEvalDlg::FindInsertionPos(int nID)
{
	int nCount = m_List.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
        T_BREV_K key = m_List.GetItemData(i);
		if (nID < key) { return i;}
	}
	return nCount;    
}

BOOL CDgnSeisBearingEvalDlg::InsertItem(T_BREV_K Key, T_BREV_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i = 0; i < 6; i++)
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

BOOL CDgnSeisBearingEvalDlg::DeleteItem(T_BREV_K Key, T_BREV_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CDgnSeisBearingEvalDlg::CheckData(T_BREV_K Key)
{
	CArray<T_ANEV_K, T_ANEV_K> rKeyList; rKeyList.RemoveAll();

	m_pDoc->m_pAttrCtrl2->GetAnevKeyList(rKeyList);

	T_ANEV_D rData;
	CArray<T_ELEM_K, T_ELEM_K> aBrprK;
	for (int m = 0; m < rKeyList.GetSize(); m++)
	{
		m_pDoc->m_pAttrCtrl2->GetAnev(rKeyList[m], rData);

		if (Key == rData.BrevK) return FALSE;

	}
	return TRUE;
}


BOOL CDgnSeisBearingEvalDlg::ModifyItem(T_BREV_K KeyOld, T_BREV_K Key, T_BREV_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < 6; i++)
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

CString CDgnSeisBearingEvalDlg::DataToStr(int i, T_BREV_K Key, T_BREV_D &Data)
{
	CString str;

	if(i==0) str = Data.strName;
	else if(i==1)
	{
		T_EVGP_D EvgpD; EvgpD.Initialize();
		if(!m_pDoc->m_pAttrCtrl2->GetEvgp(Data.EvgpK, EvgpD)) EvgpD.Initialize();
		str = EvgpD.strName;
	}
	else if(i==2) str.Format(_T("%d"), Data.n2wFixed);
	else if(i==3) str.Format(_T("%d"), Data.n2wMovable);
	else if(i==4) str.Format(_T("%d"), Data.n1wLMovable);
	else if(i==5) str.Format(_T("%d"), Data.n1wTMovable);
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisBearingEvalDlg implementation functions
BEGIN_MESSAGE_MAP(CDgnSeisBearingEvalDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisBearingEvalDlg)
	ON_CBN_SELCHANGE(IDC_DGN_SEIS_BEARING_EVAL_GROUP_CMB,          OnSeisBearingEvalEvgpCmb)

// 	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_EVAL_BEARING_RDO1,          OnSeisBearingEvalBearingRdo)	
// 	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_EVAL_BEARING_RDO2,          OnSeisBearingEvalBearingRdo)	
// 	ON_CBN_SELCHANGE(IDC_DGN_SEIS_BEARING_EVAL_STL_BEARING_CMB,    OnSeisBearingEvalStlBearingCmb)

	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_EVAL_ADD_BTN,               OnSeisBearingEvalAddBtn)	
	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_EVAL_MOD_BTN,               OnSeisBearingEvalModBtn)	
	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_EVAL_DEL_BTN,               OnSeisBearingEvalDelBtn)	
	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_EVAL_CLOSE_BTN,             OnSeisBearingEvalCloseBtn)	
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_SEIS_BEARING_EVAL_LST,      OnSeisBearingEvalDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisBearingEvalDlg message handlers

BOOL CDgnSeisBearingEvalDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitUnit();
	InitCombo();
	
	SetHeaderTitle();
	MakeItemEx();
	
	EnableDisableControls(-1);

	OnSeisBearingEvalEvgpCmb();
			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisBearingEvalDlg::OnSeisBearingEvalEvgpCmb()
{
	UpdateName();

	UpdateData(TRUE);

	T_EVGP_K EvgpK;
	EvgpK = m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel());

	int n2wFixed = 0;
	int n2wMovable = 0;
	int n1wLMovable = 0;
	int n1wTMovable = 0;

	m_pDoc->m_pAttrCtrl2->GetBearingNum(EvgpK, 0, n2wFixed, n2wMovable, n1wLMovable, n1wTMovable);

	m_edt2wFix.SetEditUnit(n2wFixed);
	m_edt2wMov.SetEditUnit(n2wMovable);
	m_edt1wLMov.SetEditUnit(n1wLMovable);
	m_edt1wTMov.SetEditUnit(n1wTMovable);

	UpdateData(FALSE);
}

// void CDgnSeisBearingEvalDlg::OnSeisBearingEvalBearingRdo()
// {
// 	EnableDisableControls();
// }
// 
// void CDgnSeisBearingEvalDlg::OnSeisBearingEvalStlBearingCmb()
// {
// 	EnableDisableControls();
// }

void CDgnSeisBearingEvalDlg::OnSeisBearingEvalAddBtn()
{
	T_BREV_D BrevD;
	if (!Dlg2Data(BrevD)) return;

	if(!m_pDoc->m_pDataCtrl->AddBrev(BrevD))
	{
		ASSERT(0);
		return;
	}
}

void CDgnSeisBearingEvalDlg::OnSeisBearingEvalModBtn()
{
	int nItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_SEIS_BEARING_EVAL_NO_SEL));
		return;
	}

	T_BREV_K BrevK;
	BrevK = (T_BREV_K)m_List.GetItemData(nItem);

	T_BREV_D BrevD;
	if (!Dlg2Data(BrevD)) return;

	if (!CheckData(BrevK))
	{
		T_BREV_D BrevD;
		m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD);
		CString msg;
		msg.Format(_LS(IDS_DGN_SEIS_ANCHOR_EVAL_BEARING_EVAL_USED), BrevD.strName);  //_T("Error : Bearing Evaluation(Name %s) is Used in Anchor Evaluation.")
		AfxMessageBox(msg);
		return;
	}

	if(!m_pDoc->m_pDataCtrl->ModifyBrev(BrevK, BrevD))
	{
		ASSERT(0);
		return;
	}
}

void CDgnSeisBearingEvalDlg::OnSeisBearingEvalDelBtn()
{
	int nItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_SEIS_BEARING_EVAL_NO_SEL));
		return;
	}

	T_BREV_K BrevK;
	BrevK = (T_BREV_K)m_List.GetItemData(nItem);

	if (!CheckData(BrevK))
	{
		T_BREV_D BrevD;
		m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD);
		CString msg;
		msg.Format(_LS(IDS_DGN_SEIS_ANCHOR_EVAL_BEARING_EVAL_USED), BrevD.strName);  //_T("Error : Bearing Evaluation(Name %s) is Used in Anchor Evaluation.")
		AfxMessageBox(msg);
		return;
	}

	if(!m_pDoc->m_pDataCtrl->DelBrev(BrevK))
	{
		ASSERT(0);
		return;
	}
}

void CDgnSeisBearingEvalDlg::OnSeisBearingEvalCloseBtn()
{
	CDialogMove::OnCancel();
}

void CDgnSeisBearingEvalDlg::OnSeisBearingEvalDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	T_BREV_D BrevD;
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
		T_BREV_K BrevK = m_List.GetItemData(nItem);
		m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD);
	}

	Data2Dlg(BrevD);

	EnableDisableControls(nItem);
	
	*pResult = 0;
}