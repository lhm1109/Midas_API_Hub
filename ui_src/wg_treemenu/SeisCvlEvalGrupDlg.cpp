// SeisCvlEvalGrupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCvlEvalGrupDlg.h"

#include "..\wg_base\wg_base_NumericOptimizer.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define SETFOCUS_COLOR RGB( 192, 220, 192)

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlEvalGrupDlg dialog


CSeisCvlEvalGrupDlg::CSeisCvlEvalGrupDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisCvlEvalGrupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisCvlEvalGrupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nOption = 0;
	m_nType = 0;
	m_bPierCap = FALSE;
	m_bLink = TRUE;
	m_nAbutPos = 0;
	m_nLink = 0;

	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();

	m_aPier.RemoveAll();
	m_aAbut.RemoveAll();
	m_aAbut2.RemoveAll();
}


void CSeisCvlEvalGrupDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCvlEvalGrupDlg)
	DDX_Radio  (pDX, IDC_TM_SEIS_CVL_ADD_RDO,          m_nOption);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_NAME_EDT,         m_edtName);
	DDX_Radio  (pDX, IDC_TM_SEIS_CVL_PIER_RDO,         m_nType);
	DDX_Check  (pDX, IDC_TM_SEIS_CVL_PIERCAP_CHK,      m_bPierCap);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_PIERCAP_ELEM_EDT, m_edtPierCap);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_COLUMN_NAME_EDT,  m_edtColmName);	
	DDX_Control(pDX, IDC_TM_SEIS_CVL_COLUMN_ELEM_EDT,  m_edtColumn);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_COLUMN_LST,       m_lstColumn);
	DDX_Check  (pDX, IDC_TM_SEIS_CVL_LINK_CHK,         m_bLink);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_LINK_NODE_EDT,    m_edtLink);
	DDX_Radio  (pDX, IDC_TM_SEIS_CVL_ABUT_START_RDO,   m_nAbutPos);
	DDX_Radio  (pDX, IDC_TM_SEIS_CVL_ABUT_LINK_RDO,    m_nLink);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_ABUT_NODE_EDT,    m_edtAbutNode);	
	DDX_Control(pDX, IDC_TM_SEIS_CVL_LST,              m_lstEvalGrup);
	//}}AFX_DATA_MAP
}

void CSeisCvlEvalGrupDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CSeisCvlEvalGrupDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount==0) return; 
	ASSERT(nCount==1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_EVGP_K Key,  KeyBack;
	T_EVGP_D Data, DataBack;

	BOOL bEvgp = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_EVGP_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetEvgp(nKey, Key, Data);
			InsertItem(Key, Data);
			bEvgp = TRUE;
			break;
		case(UR_EVGP_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			pViewBuff->GetEvgp(nKey, Key, Data);
			DeleteItem(Key, Data);
			bEvgp = TRUE;
			break;
		case(UR_EVGP_MFD):
			// 여기서는 저장만 하고 MFS에서 처리한다.
			pViewBuff->GetEvgp(nKey, Key, Data);
			KeyBack  = Key;
			DataBack = Data;
			bEvgp = TRUE;
			break;
		case(UR_EVGP_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bEvgp);
			pViewBuff->GetEvgp(nKey, Key, Data);
			ModifyItem(Key, KeyBack, DataBack);
			bEvgp = TRUE;
			break;
		default:
			break;
		}
	}

	if(bEvgp) ResetControls();
}

void CSeisCvlEvalGrupDlg::InitControl()
{
	m_edtColumn.Connect(SC_ID_ELEM, &m_edtColumn);
	//m_edtLink.Connect(SC_ID_NODE, &m_edtLink);

	m_aPier.RemoveAll();
	m_aPier.Add(IDC_TM_SEIS_CVL_PIERCAP_CHK);
	m_aPier.Add(IDC_TM_SEIS_CVL_PIERCAP_GRP);
	m_aPier.Add(IDC_TM_SEIS_CVL_PIERCAP_ELEM_STC);
	m_aPier.Add(IDC_TM_SEIS_CVL_PIERCAP_ELEM_EDT);
	m_aPier.Add(IDC_TM_SEIS_CVL_COLUMN_GRP);
	m_aPier.Add(IDC_TM_SEIS_CVL_COLUMN_NAME_STC);
	m_aPier.Add(IDC_TM_SEIS_CVL_COLUMN_NAME_EDT);
	m_aPier.Add(IDC_TM_SEIS_CVL_COLUMN_ELEM_STC);
	m_aPier.Add(IDC_TM_SEIS_CVL_COLUMN_ELEM_EDT);
	m_aPier.Add(IDC_TM_SEIS_CVL_COLUMN_ADD_BTN);
	m_aPier.Add(IDC_TM_SEIS_CVL_COLUMN_MOD_BTN);
	m_aPier.Add(IDC_TM_SEIS_CVL_COLUMN_DEL_BTN);
	m_aPier.Add(IDC_TM_SEIS_CVL_COLUMN_LST);
	m_aPier.Add(IDC_TM_SEIS_CVL_LINK_GRP);
	m_aPier.Add(IDC_TM_SEIS_CVL_LINK_CHK);
	m_aPier.Add(IDC_TM_SEIS_CVL_LINK_NODE_STC);
	m_aPier.Add(IDC_TM_SEIS_CVL_LINK_NODE_EDT);

	m_aAbut.RemoveAll();
	m_aAbut.Add(IDC_TM_SEIS_CVL_ABUT_GRP);
	m_aAbut.Add(IDC_TM_SEIS_CVL_ABUT_LINK_RDO);
	m_aAbut.Add(IDC_TM_SEIS_CVL_ABUT_SUPP_RDO);
	m_aAbut.Add(IDC_TM_SEIS_CVL_ABUT_NODE_STC);
	m_aAbut.Add(IDC_TM_SEIS_CVL_ABUT_NODE_EDT);

	m_aAbut2.RemoveAll();
	m_aAbut2.Add(IDC_TM_SEIS_CVL_ABUT_START_RDO);
	m_aAbut2.Add(IDC_TM_SEIS_CVL_ABUT_END_RDO);
}

void CSeisCvlEvalGrupDlg::AlignControl()
{
	UpdateData(TRUE);

	CRect rRef;
	CRect rToMove;
	CRect rectGrp;
	int nDistX = 0;
	int nDistY = 0;

// 	BOOL bTest = CTestEnvMgr::IsTestEnvST(_LSX(CivilSeismicTest), _LSX(yes));
// 	if(bTest)
	{
		if (m_nType == 1) // Abutment
		{
			CRect rRef01;
			GetDlgItem(IDC_TM_SEIS_CVL_PIER_RDO)->GetWindowRect(rRef01);
			GetDlgItem(IDC_TM_SEIS_CVL_PIERCAP_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_TM_SEIS_CVL_ABUT_START_RDO)->GetWindowRect(rToMove);
			nDistX = rRef01.left - rToMove.left;
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistXY(this, m_aAbut2, nDistX, nDistY);

			GetDlgItem(IDC_TM_SEIS_CVL_PIER_RDO)->GetWindowRect(rRef);
			GetDlgItem(IDC_TM_SEIS_CVL_PIERCAP_GRP)->GetWindowRect(rToMove);

			int nDistY_Abut = rRef.top - rToMove.top;

			GetDlgItem(IDC_TM_SEIS_CVL_PIERCAP_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_TM_SEIS_CVL_ABUT_GRP)->GetWindowRect(rToMove);

			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.top - rToMove.top - nDistY_Abut;

			CDlgUtil::CtrlMoveDistXY(this, m_aAbut, nDistX, nDistY);

		}
		else
		{
			GetDlgItem(IDC_TM_SEIS_CVL_PIERCAP_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_TM_SEIS_CVL_ABUT_GRP)->GetWindowRect(rToMove);

			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.top - rToMove.top;

			CDlgUtil::CtrlMoveDistXY(this, m_aAbut, nDistX, nDistY);
		}
	}
// 	else
// 	{
// 	GetDlgItem(IDC_TM_SEIS_CVL_PIERCAP_GRP)->GetWindowRect(rRef);
// 	GetDlgItem(IDC_TM_SEIS_CVL_ABUT_GRP)->GetWindowRect(rToMove);
// 
// 	nDistX = rRef.left - rToMove.left;
// 	nDistY = rRef.top - rToMove.top;
// 
// 	CDlgUtil::CtrlMoveDistXY(this, m_aAbut, nDistX, nDistY);
// 	}

	// Group Box 크기
	GetDlgItem(IDC_TM_SEIS_CVL_GROUP_GRP)->GetWindowRect(rectGrp);

	if(m_nType == 0) GetDlgItem(IDC_TM_SEIS_CVL_LINK_GRP)->GetWindowRect(rRef);
	else             GetDlgItem(IDC_TM_SEIS_CVL_ABUT_GRP)->GetWindowRect(rRef);
	
	rectGrp.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	ScreenToClient(rectGrp);
	GetDlgItem(IDC_TM_SEIS_CVL_GROUP_GRP)->MoveWindow(rectGrp);

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	aControls.Add(IDC_TM_SEIS_CVL_LST);
	//aControls.Add(IDC_TM_SEIS_CVL_APPLY_BTN);
	//aControls.Add(IDC_TM_SEIS_CVL_CLOSE_BTN);
	
	GetDlgItem(IDC_TM_SEIS_CVL_GROUP_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_SEIS_CVL_LST)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	
	RedrawWindow();
}

void CSeisCvlEvalGrupDlg::ControlsShowHide()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlShowHide(this, m_aPier, m_nType == 0);
	CDlgUtil::CtrlShowHide(this, m_aAbut, m_nType == 1);
	CDlgUtil::CtrlShowHide(this, m_aAbut2, m_nType == 1);
}

void CSeisCvlEvalGrupDlg::EnableDisableControls()
{
	UpdateData(TRUE);
	
	GetDlgItem(IDC_TM_SEIS_CVL_NAME_STC)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_NAME_EDT)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_GROUP_GRP)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_PIER_RDO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_ABUT_RDO)->EnableWindow(m_nOption == 0);

	GetDlgItem(IDC_TM_SEIS_CVL_PIERCAP_CHK)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_PIERCAP_GRP)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_PIERCAP_ELEM_STC)->EnableWindow(m_nOption == 0 && m_bPierCap);
	GetDlgItem(IDC_TM_SEIS_CVL_PIERCAP_ELEM_EDT)->EnableWindow(m_nOption == 0 && m_bPierCap);
	GetDlgItem(IDC_TM_SEIS_CVL_COLUMN_GRP)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_COLUMN_NAME_STC)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_COLUMN_NAME_EDT)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_COLUMN_ELEM_STC)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_COLUMN_ELEM_EDT)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_COLUMN_ADD_BTN)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_COLUMN_MOD_BTN)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_COLUMN_DEL_BTN)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_COLUMN_LST)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_LINK_GRP)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_LINK_CHK)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_LINK_NODE_STC)->EnableWindow(m_nOption == 0 && m_bLink);
	GetDlgItem(IDC_TM_SEIS_CVL_LINK_NODE_EDT)->EnableWindow(m_nOption == 0 && m_bLink);

	GetDlgItem(IDC_TM_SEIS_CVL_ABUT_GRP)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_ABUT_LINK_RDO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_ABUT_SUPP_RDO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_ABUT_NODE_STC)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_ABUT_NODE_EDT)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_ABUT_START_RDO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_TM_SEIS_CVL_ABUT_END_RDO  )->EnableWindow(m_nOption == 0);
	
}

void CSeisCvlEvalGrupDlg::Dlg2Data()
{
	UpdateData(TRUE);
	
	CString str;

	m_edtName.GetWindowText(str);
	m_Data.strName = str;
	m_Data.nType = m_nType;
	m_Data.bPierCap = m_bPierCap;
	
	m_edtPierCap.GetWindowText(str);
	if(m_bPierCap)
	{
		m_edtPierCap.GetWindowText(str);
		GetNodeList(str, m_Data.aPierCap);
	}
	else
	{
		m_Data.aPierCap.RemoveAll();
	}  
	
	m_Data.bLink = m_bLink;
	if(m_bLink)
	{
		m_edtLink.GetWindowText(str);
		GetNodeList(str, m_Data.aLink);
	}
	else
	{
		m_Data.aLink.RemoveAll();
	}

	m_Data.nAbutPos = m_nAbutPos;
	m_Data.nLink = m_nLink;
	m_edtAbutNode.GetWindowText(str);

	if(m_nType == 1)
	{
		if(m_nLink == 0) GetNodeList(str, m_Data.aLink);
		else             GetNodeList(str, m_Data.aSupport);	
	}
}

void CSeisCvlEvalGrupDlg::Data2Dlg()
{
	m_edtName.SetWindowText(m_Data.strName);
	m_nType = m_Data.nType;
	m_bPierCap = m_Data.bPierCap;

	CNumericOptimizer opt;
	CString str(_T(""));
	str = opt.OptimizeUnsortedArray(m_Data.aPierCap);
	m_edtPierCap.SetWindowText(str);
	
	m_bLink = m_Data.bLink;

	str = opt.OptimizeUnsortedArray(m_Data.aLink);
	m_edtLink.SetWindowText(str);

	m_nAbutPos = m_Data.nAbutPos;
	m_nLink = m_Data.nLink;
	if(m_nLink == 0) str = opt.OptimizeUnsortedArray(m_Data.aLink);
	else             str = opt.OptimizeUnsortedArray(m_Data.aSupport);	
	m_edtAbutNode.SetWindowText(str);

	UpdateData(FALSE);
}

void CSeisCvlEvalGrupDlg::SetHeaderTitle_Column()
{
	CString aTitle[] = { _LS(IDS_WG_TREEMENU_EVGP_NAME), _LS(IDS_WG_TREEMENU_EVGP_ELEM) };
	int nColWidth[] = {60, 95};

	CString title;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_lstColumn.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_lstColumn.GetSafeHwnd(), dwStyle);

	for(int i=0; i<2; i++)
	{
		CString strTitle = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = strTitle.GetBuffer(0);
		m_lstColumn.InsertColumn(i+1, &lvcolumn);
		strTitle.ReleaseBuffer();
	}
}

void CSeisCvlEvalGrupDlg::MakeItemEx_Column(T_EVGP_D& Data)
{
	m_lstColumn.DeleteAllItems();
	
	for(int i=0; i<Data.aEvgpColm.GetSize(); i++)
	{
		InsertItem_Column(i, Data.aEvgpColm[i]);
	}
}

BOOL CSeisCvlEvalGrupDlg::InsertItem_Column(int nIndex, T_EVGP_COLM_D& EvgpColm)
{
	LVITEM lvitem;
	CString str;
	int nItem;

	lvitem.iItem = nIndex;
	for(int i = 0; i < 2; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr_Column(i, EvgpColm);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) 
		{
			nItem = m_lstColumn.InsertItem(&lvitem);
		}
		else m_lstColumn.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CSeisCvlEvalGrupDlg::ModifyItem_Column(int nIndex, T_EVGP_COLM_D& EvgpColm)
{
	int nItem = nIndex;
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < 2; i++)
		{
			str = DataToStr_Column(i, EvgpColm);
			m_lstColumn.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

CString CSeisCvlEvalGrupDlg::DataToStr_Column(int i, T_EVGP_COLM_D &EvgpColm)
{
	CNumericOptimizer opt;

	CString str = _T("");

	switch(i)
	{
	case 0:
		str = EvgpColm.strName;
		break;
	case 1:
		str = opt.OptimizeUnsortedArray(EvgpColm.aColumn);
		break;
	default: ASSERT(0); break;
	}

	return str;
}


void CSeisCvlEvalGrupDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_WG_TREEMENU_EVGP_NAME), _LS(IDS_WG_TREEMENU_EVGP_TYPE), _LS(IDS_WG_TREEMENU_EVGP_PIER_CAP), 
		                 _LS(IDS_WG_TREEMENU_EVGP_COLUMN), _LS(IDS_WG_TREEMENU_EVGP_LINK), _LS(IDS_WG_TREEMENU_EVGP_SUPPORT) };
	int nColWidth[] = {50, 70, 70, 70, 70, 70};

	CString title;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_lstEvalGrup.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_lstEvalGrup.GetSafeHwnd(), dwStyle);

	for(int i=0; i<6; i++)
	{
		CString strTitle = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = strTitle.GetBuffer(0);
		m_lstEvalGrup.InsertColumn(i+1, &lvcolumn);
		strTitle.ReleaseBuffer();
	}
}

void CSeisCvlEvalGrupDlg::MakeItemEx()
{
	m_lstEvalGrup.DeleteAllItems();

	CArray<T_EVGP_K, T_EVGP_K> aEvgpK;
	m_pDoc->m_pAttrCtrl2->GetEvgpKeyList(aEvgpK);

	for(int i=0; i<aEvgpK.GetSize(); i++)
	{
		T_EVGP_D EvgpD; EvgpD.Initialize();
		if(!m_pDoc->m_pAttrCtrl2->GetEvgp(aEvgpK[i], EvgpD)) continue;
		InsertItem(aEvgpK[i], EvgpD);
	}
}

BOOL CSeisCvlEvalGrupDlg::InsertItem(T_EVGP_K Key, T_EVGP_D& Data)
{
	int nItem = m_lstEvalGrup.GetItemCount();
	for(int i=0; i<6; i++)
	{
		LVITEM lvitem;
		lvitem.iItem    = nItem;
		lvitem.iSubItem = i;
		CString str     = DataToStr(i, Key, Data);
		lvitem.pszText  = str.GetBuffer(0);
		lvitem.mask     = LVIF_TEXT;

		if(i==0)
		{
			lvitem.mask   |= LVIF_PARAM;
			lvitem.lParam  = (LPARAM)Key;
			nItem          = m_lstEvalGrup.InsertItem(&lvitem);
		}
		else m_lstEvalGrup.SetItem(&lvitem);

		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CSeisCvlEvalGrupDlg::ModifyItem(T_EVGP_K KeyOld, T_EVGP_K Key, T_EVGP_D& Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags  = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;

	int nItem = m_lstEvalGrup.FindItem(&FindInfo);
	if(nItem == -1) return TRUE;

	for(int i=0; i<6; i++)
	{
		CString str = DataToStr(i, Key, Data);
		m_lstEvalGrup.SetItemText(nItem, i, str);
	}

	LVITEM lvitem;
	lvitem.iItem    = nItem;
	lvitem.iSubItem = 0;
	lvitem.mask     = LVIF_PARAM;
	lvitem.lParam   = (LPARAM)Key;
	m_lstEvalGrup.SetItem(&lvitem);

	return TRUE;
}

BOOL CSeisCvlEvalGrupDlg::DeleteItem(T_EVGP_K Key, T_EVGP_D& Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;

	int nItem = m_lstEvalGrup.FindItem(&FindInfo);
	if(nItem==-1) return TRUE;

	m_lstEvalGrup.DeleteItem(nItem);

	return TRUE;
}

CString CSeisCvlEvalGrupDlg::DataToStr(int i, T_EVGP_K Key, T_EVGP_D &Data)
{
	CNumericOptimizer opt;

	CString str = _T("");

	switch(i)
	{
	case 0:
		str = Data.strName;
		break;
	case 1:
		if(Data.nType == 0) str = _LS(IDS_WG_TREEMENU_EVGP_TYPE_PIER);
		else                str = _LS(IDS_WG_TREEMENU_EVGP_TYPE_ABUT);   
		break;
	case 2:
		if(Data.nType == 0)
		{
			if(Data.bPierCap && Data.aPierCap.GetSize() > 0) str = _T("O");
			else                                             str = _T("X");
		}
		else
		{
			str = _T("X");
		}		
		break;
	case 3:
		if(Data.nType == 0)
		{
			if(Data.aEvgpColm.GetSize() > 0) str = _T("O");
			else                             str = _T("X");
		}
		else
		{
			str = _T("X");
		}
		break;
	case 4:
		if(Data.nType == 0)
		{
			if(Data.bLink && Data.aLink.GetSize() > 0) str = _T("O");
			else                                       str = _T("X");
		}
		else
		{
			if(Data.nLink == 0 && Data.aLink.GetSize() > 0) str = _T("O");
			else                                           	str = _T("X");
		}
		break;
	case 5:
		if(Data.nType == 1)
		{
			if(Data.nLink == 1 && Data.aSupport.GetSize() > 0) str = _T("O");
			else                                               str = _T("X");
		}
		else
		{
			str = _T("X");
		}
		break;
	default: ASSERT(0); break;
	}

	return str;
}

BOOL CSeisCvlEvalGrupDlg::IsEvalGrupElem(CMouseEdit* pMEdit, T_ELEM_K ElemK)
{
	return TRUE;
}

void CSeisCvlEvalGrupDlg::RedrawSelectEditColor() 
{
	m_edtPierCap.Invalidate(FALSE);
	m_edtColumn.Invalidate(FALSE);
}

void CSeisCvlEvalGrupDlg::RemoveNodeSelect() 
{
	CString strPreviousText = _T("");
	m_edtLink.GetWindowText(strPreviousText);
	m_edtLink.Connect(SC_ID_NODE, &m_edtLink);

	CString strBlank = _T("");
	m_edtLink.SelectByStr(strBlank);

	m_edtLink.SetWindowText(strPreviousText);
	m_edtLink.Disconnect();

	m_edtAbutNode.GetWindowText(strPreviousText);
	m_edtAbutNode.Connect(SC_ID_NODE, &m_edtAbutNode);

	m_edtAbutNode.SelectByStr(strBlank);

	m_edtAbutNode.SetWindowText(strPreviousText);
	m_edtAbutNode.Disconnect();
}

void CSeisCvlEvalGrupDlg::RemoveElemSelect() 
{
	CString strPreviousText = _T("");
	m_edtColumn.GetWindowText(strPreviousText);
	m_edtColumn.Connect(SC_ID_ELEM, &m_edtColumn);

	CString strBlank = _T("");
	m_edtColumn.SelectByStr(strBlank);

	m_edtColumn.SetWindowText(strPreviousText);
	m_edtColumn.Disconnect();
}

void CSeisCvlEvalGrupDlg::ResetControls()
{
	m_edtName.SetWindowText(_T(""));
	m_bPierCap = FALSE;
	m_edtPierCap.SetWindowText(_T(""));

	m_lstColumn.DeleteAllItems();
	ResetColmColntrols();
	
	m_bLink = FALSE;
	m_edtLink.SetWindowText(_T(""));
	m_edtAbutNode.SetWindowText(_T(""));
}

void CSeisCvlEvalGrupDlg::ResetColmColntrols()
{
	m_edtColmName.SetWindowText(_T(""));
	m_edtColumn.SetWindowText(_T(""));
}

BOOL CSeisCvlEvalGrupDlg::SeperateColumnData(T_EVGP_COLM_D &EvgpColm, CArray<T_EVGP_COLM_D, T_EVGP_COLM_D>& aEvgpColm)
{	
	aEvgpColm.RemoveAll();

	if(EvgpColm.strName.IsEmpty())
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_EVGP_COLM_NAME_ERROR));
		return FALSE;
	}

	if(EvgpColm.aColumn.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_EVGP_COLM_ELEM_ERROR));
		return FALSE;
	}

	// 1. 우선 모두 연결된 요소만 선택했는지 Check

	if(m_pDoc->m_pAttrCtrl->IsConnectedElem(EvgpColm.aColumn)) 
	{
		aEvgpColm.Add(EvgpColm);
		return TRUE;
	}

	// 2. 모든 요소가 연결된 요소가 아닌경우 분리 하는 코드

	int nSizeElem = EvgpColm.aColumn.GetSize();

	CArray<T_ELEM_D, T_ELEM_D&> aElemD;
	aElemD.SetSize(nSizeElem);

	for(int i=0; i<nSizeElem; i++)
	{
		m_pDoc->m_pAttrCtrl->GetElem(EvgpColm.aColumn[i], aElemD[i]);
	}

	for(int i=0; i<nSizeElem; i++)
	{
		if(!m_pDoc->m_pAttrCtrl->IsBeam(aElemD[i].eltyp))
		{
			AfxMessageBox(_LS(IDS_WG_TREEMENU_EVGP_COLM_ELEM_BEAM_ERROR));
			return FALSE;
		}
	}
	
	CMap<T_ELEM_K, T_ELEM_K, BOOL, BOOL> mapUsedElem;
	CMap<T_NODE_K, T_NODE_K, T_ELEM_K, T_ELEM_K> mapStNodeKToElemK;
	CMap<T_NODE_K, T_NODE_K, T_ELEM_K, T_ELEM_K> mapEnNodeKToElemK;
	mapStNodeKToElemK.InitHashTable(500);
	mapEnNodeKToElemK.InitHashTable(500);

	T_ELEM_K ElemK;

	for(int i=0; i<nSizeElem; i++)
	{
		if(mapStNodeKToElemK.Lookup(aElemD[i].elnod[0], ElemK))
		{
			AfxMessageBox(_LS(IDS_WG_TREEMENU_EVGP_COLM_ELEM_SEPERATE_ERROR));
			return FALSE;
		}

		if(mapEnNodeKToElemK.Lookup(aElemD[i].elnod[1], ElemK))
		{
			AfxMessageBox(_LS(IDS_WG_TREEMENU_EVGP_COLM_ELEM_SEPERATE_ERROR));
			return FALSE;
		}

		mapUsedElem.SetAt(EvgpColm.aColumn[i], FALSE);
		mapStNodeKToElemK.SetAt(aElemD[i].elnod[0], EvgpColm.aColumn[i]);
		mapEnNodeKToElemK.SetAt(aElemD[i].elnod[1], EvgpColm.aColumn[i]);
	}

	T_NODE_K NodeKNext;
	T_ELEM_K ElemKFind;
	T_ELEM_D ElemDFind;

	int nNameIndex = 1;
	BOOL bUsed = FALSE;
	CArray<T_ELEM_K, T_ELEM_K> aConnectedElem;

	CString strName;
	T_EVGP_COLM_D Data;

	for(int i=0; i<nSizeElem; i++)
	{
		bUsed = FALSE;
		mapUsedElem.Lookup(EvgpColm.aColumn[i], bUsed);
		if(bUsed) continue;

		mapUsedElem.SetAt(EvgpColm.aColumn[i], TRUE);
		
		aConnectedElem.RemoveAll();
		aConnectedElem.Add(EvgpColm.aColumn[i]);

		// 정방향 연결된 요소 찾기
		NodeKNext = aElemD[i].elnod[1];
			
		while(NodeKNext != NULL)
		{
			if(mapStNodeKToElemK.Lookup(NodeKNext, ElemKFind))
			{
				mapUsedElem.Lookup(ElemKFind, bUsed);
				if(bUsed) break;

				mapUsedElem.SetAt(ElemKFind, TRUE);

				aConnectedElem.Add(ElemKFind);
				m_pDoc->m_pAttrCtrl->GetElem(ElemKFind, ElemDFind);
				NodeKNext = ElemDFind.elnod[1];
			}
			else
			{
				break;
			}
		}

		// 역방향 연결된 요소 찾기
		NodeKNext = aElemD[i].elnod[0];

		while(NodeKNext != NULL)
		{
			if(mapEnNodeKToElemK.Lookup(NodeKNext, ElemKFind))
			{
				mapUsedElem.Lookup(ElemKFind, bUsed);
				if(bUsed) break;

				mapUsedElem.SetAt(ElemKFind, TRUE);

				aConnectedElem.Add(ElemKFind);
				m_pDoc->m_pAttrCtrl->GetElem(ElemKFind, ElemDFind);
				NodeKNext = ElemDFind.elnod[0];
			}
			else
			{
				break;
			}
		}


		strName.Format(_T("%s_%d"), EvgpColm.strName, nNameIndex);
		nNameIndex++;

		Data.Initialize();
		Data.strName = strName;
		Data.aColumn.Copy(aConnectedElem);

		aEvgpColm.Add(Data);
	}

	return TRUE;
}

BOOL CSeisCvlEvalGrupDlg::CheckColumnData(T_EVGP_COLM_D &EvgpColm)
{
	if(EvgpColm.strName.IsEmpty())
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_EVGP_COLM_NAME_ERROR));
		return FALSE;
	}

	if(EvgpColm.aColumn.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_EVGP_COLM_ELEM_ERROR));
		return FALSE;
	}

	if(!m_pDoc->m_pAttrCtrl->IsConnectedElem(EvgpColm.aColumn)) 
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_EVGP_COLM_ELEM_CONNECT_ERROR));
		return FALSE;
	}

	return TRUE;
}

BEGIN_MESSAGE_MAP(CSeisCvlEvalGrupDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSeisCvlEvalGrupDlg)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_ADD_RDO,           OnTmEvalGrupOptionRdo)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_DEL_RDO,           OnTmEvalGrupOptionRdo)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_PIER_RDO,          OnTmEvalGrupTypeRdo)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_ABUT_RDO,          OnTmEvalGrupTypeRdo)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_PIERCAP_CHK,       OnTmEvalGrupPierCapChk)
	ON_EN_SETFOCUS(IDC_TM_SEIS_CVL_PIERCAP_ELEM_EDT, OnTmEvalGrupSetfocusPierCapEdt)
	ON_EN_CHANGE(IDC_TM_SEIS_CVL_COLUMN_NAME_EDT,    OnTmEvalGrupColumnNameEdt)
	ON_EN_SETFOCUS(IDC_TM_SEIS_CVL_COLUMN_ELEM_EDT,  OnTmEvalGrupSetfocusColumnEdt)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_COLUMN_ADD_BTN,    OnTmEvalGrupColumnAddBtn)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_COLUMN_MOD_BTN,    OnTmEvalGrupColumnModBtn)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_COLUMN_DEL_BTN,    OnTmEvalGrupColumnDelBtn)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_LINK_CHK,          OnTmEvalGrupLinkChk)
	ON_EN_SETFOCUS(IDC_TM_SEIS_CVL_LINK_NODE_EDT,    OnTmEvalGrupSetfocusLinkEdt)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_ABUT_LINK_RDO,     OnTmEvalGrupLinkTypeRdo)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_ABUT_SUPP_RDO,     OnTmEvalGrupLinkTypeRdo)
	ON_EN_SETFOCUS(IDC_TM_SEIS_CVL_ABUT_NODE_EDT,    OnTmEvalGrupSetfocusAbutNodeEdt)
	//ON_BN_CLICKED(IDC_TM_SEIS_CVL_APPLY_BTN,         OnTmEvalGrupApplyBtn)
	//ON_BN_CLICKED(IDC_TM_SEIS_CVL_CLOSE_BTN,         OnTmEvalGrupCloseBtn)  
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_SEIS_CVL_COLUMN_LST, OnTmEvalGrupColumnItemChanged)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_SEIS_CVL_LST,  OnTmEvalGrupItemChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlEvalGrupDlg message handlers

BOOL CSeisCvlEvalGrupDlg::OnInitDialog() 
{
 	CMenuBarChildDlg::OnInitDialog();

	InitControl();

	SetHeaderTitle_Column();
	SetHeaderTitle();
	MakeItemEx();

	AlignControl();
	ControlsShowHide();
	EnableDisableControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisCvlEvalGrupDlg::OnTmEvalGrupOptionRdo()
{
	EnableDisableControls();
}

void CSeisCvlEvalGrupDlg::OnTmEvalGrupTypeRdo()
{
	AlignControl();
	ControlsShowHide();
	EnableDisableControls();
}

void CSeisCvlEvalGrupDlg::OnTmEvalGrupPierCapChk()
{
	EnableDisableControls();

	if(m_bPierCap)
	{
		OnTmEvalGrupSetfocusPierCapEdt(); 
	}
	else
	{
		OnTmEvalGrupSetfocusColumnEdt(); 
	}

	RedrawSelectEditColor();
}

void CSeisCvlEvalGrupDlg::OnTmEvalGrupSetfocusPierCapEdt() 
{
	m_edtColumn.Disconnect();
	m_edtLink.Disconnect();
	m_edtAbutNode.Disconnect();

	// text preserve
	CString strPreviousText = _T("");
	m_edtPierCap.GetWindowText(strPreviousText);
	m_edtPierCap.Connect(SC_ID_ELEM, &m_edtPierCap);
	m_edtPierCap.SetWindowText(strPreviousText);

	// model select
	CString strBlank = _T("");
	m_edtPierCap.SelectByStr(strBlank);
	m_edtPierCap.SelectByStr(strPreviousText);

	RemoveNodeSelect();
	RedrawSelectEditColor();
}

void CSeisCvlEvalGrupDlg::OnTmEvalGrupColumnNameEdt()
{
	OnTmEvalGrupSetfocusColumnEdt();
}

void CSeisCvlEvalGrupDlg::OnTmEvalGrupSetfocusColumnEdt() 
{
	m_edtPierCap.Disconnect();
	m_edtLink.Disconnect();
	m_edtAbutNode.Disconnect();

	// text preserve
	CString strPreviousText = _T("");
	m_edtColumn.GetWindowText(strPreviousText);
	m_edtColumn.Connect(SC_ID_ELEM, &m_edtColumn);
	m_edtColumn.SetWindowText(strPreviousText);

	// model select
	CString strBlank = _T("");
	m_edtColumn.SelectByStr(strBlank);
	m_edtColumn.SelectByStr(strPreviousText);

	RemoveNodeSelect();
	RedrawSelectEditColor();
}

void CSeisCvlEvalGrupDlg::OnTmEvalGrupColumnAddBtn() 
{
	UpdateData(TRUE);

	T_EVGP_COLM_D Data; Data.Initialize();

	CString str;
	m_edtColmName.GetWindowText(str);
	Data.strName = str;

	m_edtColumn.GetWindowText(str);
	GetNodeList(str, Data.aColumn);

	CArray<T_EVGP_COLM_D, T_EVGP_COLM_D> aData;
	if(!SeperateColumnData(Data, aData)) return;

	for(int i=0; i<aData.GetSize(); i++)
	{
		if(!CheckColumnData(aData[i])) return;
	}

	int nCount = 0;
	for(int i=0; i<aData.GetSize(); i++)
	{
		m_Data.aEvgpColm.Add(aData[i]);
		nCount = m_Data.aEvgpColm.GetSize();
		InsertItem_Column(nCount-1, aData[i]);
	}	

	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
	ResetColmColntrols();
}

void CSeisCvlEvalGrupDlg::OnTmEvalGrupColumnModBtn() 
{
	UpdateData(TRUE);

	int iItem = m_lstColumn.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_ERROR_NO_SELECTED_SEIS_EVAL_COLM));
		return;
	}
	
	T_EVGP_COLM_D Data; Data.Initialize();

	CString str;
	m_edtColmName.GetWindowText(str);
	Data.strName = str;

	m_edtColumn.GetWindowText(str);
	GetNodeList(str, Data.aColumn);

	if(!CheckColumnData(Data)) return;

	m_Data.aEvgpColm.SetAt(iItem, Data);
	ModifyItem_Column(iItem, Data);

	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
	ResetColmColntrols();
}

void CSeisCvlEvalGrupDlg::OnTmEvalGrupColumnDelBtn() 
{
	int iItem = m_lstColumn.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_ERROR_NO_SELECTED_SEIS_EVAL_COLM));
		return;
	}

	m_Data.aEvgpColm.RemoveAt(iItem);
	m_lstColumn.DeleteItem(iItem);

	int nCount = m_lstColumn.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_lstColumn.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	

	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
}

void CSeisCvlEvalGrupDlg::OnTmEvalGrupLinkChk()
{
	EnableDisableControls();

	if(m_bLink)
	{
		OnTmEvalGrupSetfocusLinkEdt(); 
	}
	else
	{
		OnTmEvalGrupSetfocusColumnEdt(); 
	}

	RedrawSelectEditColor();
}

void CSeisCvlEvalGrupDlg::OnTmEvalGrupSetfocusLinkEdt() 
{
	m_edtPierCap.Disconnect();
	m_edtColumn.Disconnect();
	m_edtAbutNode.Disconnect();

	// text preserve
	CString strPreviousText = _T("");
	m_edtLink.GetWindowText(strPreviousText);
	m_edtLink.Connect(SC_ID_NODE, &m_edtLink);
	m_edtLink.SetWindowText(strPreviousText);

	// model select
	CString strBlank = _T("");
	m_edtLink.SelectByStr(strBlank);
	m_edtLink.SelectByStr(strPreviousText);

	RemoveElemSelect();
}

void CSeisCvlEvalGrupDlg::OnTmEvalGrupLinkTypeRdo() 
{
	EnableDisableControls();
}

void CSeisCvlEvalGrupDlg::OnTmEvalGrupSetfocusAbutNodeEdt() 
{
	m_edtPierCap.Disconnect();
	m_edtColumn.Disconnect();
	m_edtLink.Disconnect();

	// text preserve
	CString strPreviousText = _T("");
	m_edtAbutNode.GetWindowText(strPreviousText);
	m_edtAbutNode.Connect(SC_ID_NODE, &m_edtAbutNode);
	m_edtAbutNode.SetWindowText(strPreviousText);

	// model select
	CString strBlank = _T("");
	m_edtAbutNode.SelectByStr(strBlank);
	m_edtAbutNode.SelectByStr(strPreviousText);

	RemoveElemSelect();
}

void CSeisCvlEvalGrupDlg::Execute()
{
	UpdateData(TRUE);

	Dlg2Data();

	BOOL bModify = FALSE;
	int nItem = m_lstEvalGrup.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(nItem != -1) bModify = TRUE;

	T_EVGP_K EvgpK;
	if(bModify) EvgpK = (T_EVGP_K)m_lstEvalGrup.GetItemData(nItem);
		
	if(m_nOption == 0)
	{
		BOOL bSuccess = FALSE;
		if(bModify) 
		{
			if(!m_pDoc->m_pAttrCtrl2->ExistEvgp(EvgpK)) return;

			bSuccess = m_pDoc->m_pDataCtrl->ModifyEvgp(EvgpK, m_Data);
		}
		else
		{
			bSuccess = m_pDoc->m_pDataCtrl->AddEvgp(m_Data);
		}		

		if(bSuccess) 
		{
			m_Data.Initialize();

			ResetControls();
			m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
		}
	}
	else
	{
		if(nItem == -1) return;

		if(!m_pDoc->m_pAttrCtrl2->ExistEvgp(EvgpK)) return;

		BOOL bSuccess = FALSE;
		bSuccess = m_pDoc->m_pDataCtrl->DelEvgp(EvgpK);

		if(bSuccess) 
		{
			m_Data.Initialize();

			ResetControls();
			m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
		}
	}
}

// void CSeisCvlEvalGrupDlg::OnTmEvalGrupCloseBtn()
// {
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
// }

void CSeisCvlEvalGrupDlg::OnTmEvalGrupColumnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	T_EVGP_COLM_D EvgpColm;
	int nItem = m_lstColumn.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(nItem==-1)
	{
		ResetColmColntrols();

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
		if(nItem >= m_Data.aEvgpColm.GetSize()) return;
		EvgpColm = m_Data.aEvgpColm[nItem];
	}

	m_edtColmName.SetWindowText(EvgpColm.strName);

	CNumericOptimizer opt;
	CString str(_T(""));
	str = opt.OptimizeUnsortedArray(EvgpColm.aColumn);
	m_edtColumn.SetWindowText(str);

	UpdateData(FALSE);

	OnTmEvalGrupSetfocusColumnEdt();
			
	*pResult = 0;
}

void CSeisCvlEvalGrupDlg::OnTmEvalGrupItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	T_EVGP_D EvgpD;
	int nItem = m_lstEvalGrup.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(nItem==-1)
	{
		m_Data.Initialize();
		ResetControls();

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
		T_EVGP_K EvgpK = m_lstEvalGrup.GetItemData(nItem);
		m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, m_Data);
	}

	Data2Dlg();
	MakeItemEx_Column(m_Data);

	AlignControl();
	ControlsShowHide();
	EnableDisableControls();

	OnTmEvalGrupSetfocusColumnEdt();

	*pResult = 0;
}