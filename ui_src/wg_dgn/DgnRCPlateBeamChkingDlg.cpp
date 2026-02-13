// DgnRCPlateBeamChkingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRCPlateBeamChkingDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\TreeMenuMode.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\ViewCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "DgnRCPlateBeamRebarDlg.h"

#include "..\wg_main\wg_main.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnRCPlateBeamChkingDlg dialog
CDgnRCPlateBeamChkingDlg::CDgnRCPlateBeamChkingDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnRCPlateBeamChkingDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_Data.Initialize();
}

CDgnRCPlateBeamChkingDlg::~CDgnRCPlateBeamChkingDlg()
{
}

void CDgnRCPlateBeamChkingDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRCPlateBeamChkingDlg)		
	DDX_Control(pDX, IDC_DGN_AVG_NODAL_ACT_CHK,   m_chkAvgNodal);	
	DDX_Control(pDX, IDC_DGN_LST,            m_List);
	DDX_Control(pDX, IDC_DGN_SUBDOMAIN_CBX,  m_cbxSbdo);
	DDX_Control(pDX, IDC_DGN_REBAR_CBX,      m_cbxRebar);
	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnRCPlateBeamChkingDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnRCPlateBeamChkingDlg) 	      
	ON_BN_CLICKED(IDC_DGN_ELEM_RDO,          OnDgnRadioElemAvg)
	ON_BN_CLICKED(IDC_DGN_AVG_NODAL_RDO,     OnDgnRadioElemAvg)
	ON_BN_CLICKED(IDC_DGN_ADD_BTN,           OnAdd)
	ON_BN_CLICKED(IDC_DGN_MOD_BTN,           OnMod)
	ON_BN_CLICKED(IDC_DGN_DEL_BTN,           OnDel)
	ON_BN_CLICKED(IDC_DGN_CLOSE_BTN,         OnClose)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_LST,  OnItemchanged)	

	ON_BN_CLICKED(IDC_DGN_SUBDOMAIN_BTN,     OnSbdoBtn)
	ON_BN_CLICKED(IDC_DGN_REBAR_BTN,         OnRebarBtn)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRCPlateBeamChkingDlg message handlers
BOOL CDgnRCPlateBeamChkingDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	SetCtrl();
	MakeItemEx();
	
	ControlsEnableDisable();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnRCPlateBeamChkingDlg::SetCtrl()
{
	m_aRdoElemAvg.RemoveAll();
	m_aRdoElemAvg.Add(IDC_DGN_ELEM_RDO);
	m_aRdoElemAvg.Add(IDC_DGN_AVG_NODAL_RDO);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoElemAvg, 0); 

	m_aRdoXYDir.RemoveAll();
	m_aRdoXYDir.Add(IDC_DGN_DIR1_RDO);
	m_aRdoXYDir.Add(IDC_DGN_DIR2_RDO);  
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoXYDir, 0); 

	InitCombo();
	SetListCtrlHeader();
}

void CDgnRCPlateBeamChkingDlg::InitCombo()
{
	InitSbdoCombo(FALSE);
	InitRipbCombo(FALSE);
}

void CDgnRCPlateBeamChkingDlg::InitSbdoCombo(BOOL bKeepSel)
{
	CArray<T_SBDO_K, T_SBDO_K> aSbdoK;
	m_pDoc->m_pAttrCtrl->GetSbdoKeyList(aSbdoK);

	T_SBDO_K SbdoK = NULL;
	if(bKeepSel)
	{
		SbdoK = CDlgUtil::CobxGetCurSelItemData(m_cbxSbdo, m_cbxSbdo.GetCurSel());
	}

	m_cbxSbdo.ResetContent();
	for (int i = 0; i < aSbdoK.GetSize(); i++)
	{
		T_SBDO_D SbdoD;
		if(!m_pDoc->m_pAttrCtrl->GetSbdo(aSbdoK[i], SbdoD)) { ASSERT(0); SbdoD.Initialize(); }
		if(SbdoD.nMembTypeCivil == 1)
		{
			CDlgUtil::CobxAddItem(m_cbxSbdo, SbdoD.strName, aSbdoK[i]);
		}    
	}
	
	if(bKeepSel)
	{
		CDlgUtil::CobxSetCurSelItemData(m_cbxSbdo, SbdoK);
	}

	if(m_cbxSbdo.GetCurSel() == -1)
	{
		m_cbxSbdo.SetCurSel(0);
	}  
}

void CDgnRCPlateBeamChkingDlg::InitRipbCombo(BOOL bKeepSel)
{
	CArray<T_RIPB_K, T_RIPB_K> aRipbK;
	m_pDoc->m_pAttrCtrl2->GetRipbKeyList(aRipbK);

	T_RIPB_K RipbK = NULL;
	if(bKeepSel)
	{
		RipbK = CDlgUtil::CobxGetCurSelItemData(m_cbxRebar, m_cbxRebar.GetCurSel());
	}

	m_cbxRebar.ResetContent();
	for (int i = 0; i < aRipbK.GetSize(); i++)
	{
		T_RIPB_D RipbD;
		if(!m_pDoc->m_pAttrCtrl2->GetRipb(aRipbK[i], RipbD)) { ASSERT(0); RipbD.Initialize(); }
		CDlgUtil::CobxAddItem(m_cbxRebar, RipbD.strName, aRipbK[i]);    
	}
	m_cbxRebar.SetCurSel(0);

	if(bKeepSel)
	{
		CDlgUtil::CobxSetCurSelItemData(m_cbxRebar, RipbK);
	}

	if(m_cbxRebar.GetCurSel() == -1)
	{
		m_cbxRebar.SetCurSel(0);
	}
}

void CDgnRCPlateBeamChkingDlg::Data2Dlg()
{	
	CDlgUtil::CobxSetCurSelItemData( m_cbxSbdo, m_Data.SbdoK);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoElemAvg, m_Data.nOpt); 
	m_chkAvgNodal.SetCheck(m_Data.bAvgNdActOnly);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoXYDir, m_Data.nOptDir); 
	CDlgUtil::CobxSetCurSelItemData( m_cbxRebar, m_Data.RipbK);  

	CArray<T_ELEM_K, T_ELEM_K> aElemK;
	m_pDoc->m_pAttrCtrl->GetElemListSbdoK(m_Data.SbdoK, aElemK);
	m_pDoc->m_pViewCtrl->SelectElem(NULL, aElemK, FALSE, TRUE);

	UpdateData(FALSE);
}

BOOL CDgnRCPlateBeamChkingDlg::Dlg2Data()
{
	UpdateData(TRUE);
		
	m_Data.SbdoK = CDlgUtil::CobxGetCurSelItemData(m_cbxSbdo, m_cbxSbdo.GetCurSel());
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoElemAvg, m_Data.nOpt); 
	m_Data.bAvgNdActOnly = m_chkAvgNodal.GetCheck();
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoXYDir, m_Data.nOptDir);
	m_Data.RipbK = CDlgUtil::CobxGetCurSelItemData(m_cbxRebar, m_cbxRebar.GetCurSel());

	return TRUE;
}

void CDgnRCPlateBeamChkingDlg::ControlsEnableDisable()
{
	UpdateData(TRUE);
		
	//int nElemAvg;
	GetDlgItem(IDC_DGN_AVG_NODAL_ACT_CHK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE_BTN)->ShowWindow(SW_HIDE);
}

void CDgnRCPlateBeamChkingDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_DGN_LIST_ID), _LS(IDS_DGN_LIST_SUB_DOMINA), _LS(IDS_DGN_LIST_DIR), _LS(IDS_DGN_LIST_REBAR)};
	int nColWidth[] = {25, 85, 30, 64};
	int nAlign[] = {LVCFMT_RIGHT, LVCFMT_LEFT, LVCFMT_RIGHT, LVCFMT_LEFT};
	int nColNum = sizeof(nColWidth)/sizeof(int);
	CString title;  
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	for(int i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = nAlign[i];

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}


void CDgnRCPlateBeamChkingDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl2->GetCountPbdc();
	if(nItemCount == 0) return;

	POSITION pos;
	int nIndex;  

	T_PBDC_K Key;
	T_PBDC_D Data;
	CString strTemp;

	int* KeyBuf=new int[nItemCount];
	int nCount = 0;
	pos = m_pDoc->m_pAttrCtrl2->GetStartPbdc();
	while(pos !=NULL)
	{
		m_pDoc->m_pAttrCtrl2->GetNextPbdc(pos, Key, Data);		  
		KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nItemCount, sizeof(T_ELEM_K), CCompFunc::UINTAsc);

	T_SBDO_D SbdoD;
	T_RIPB_D RipbD;
	for(nCount = 0 ; nCount < nItemCount ; nCount++)
	{
		Key = KeyBuf[nCount];

		m_pDoc->m_pAttrCtrl2->GetPbdc(Key, Data);

		strTemp.Format(_T("%d"), Key);
		nIndex = m_List.InsertItem(nCount, strTemp);      
				
		if(!m_pDoc->m_pAttrCtrl->GetSbdo(Data.SbdoK, SbdoD)) { ASSERT(0); SbdoD.Initialize(); }		
		m_List.SetItemText(nIndex, 1, SbdoD.strName);		

		strTemp.Format(_T("%d"), Data.nOptDir + 1);
		m_List.SetItemText(nIndex, 2, strTemp);

		if(!m_pDoc->m_pAttrCtrl2->GetRipb(Data.RipbK, RipbD)) { ASSERT(0); RipbD.Initialize(); }		    
		m_List.SetItemText(nIndex, 3, RipbD.strName);

		m_List.SetItemData(nIndex, (DWORD)Key);
	}
	delete []KeyBuf;
}

void CDgnRCPlateBeamChkingDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			m_pDoc->m_pUnitCtrl->ConvertUnitPbdcPrevious(m_Data);
			break;
		case D_UPDATE_SEL_ADD: 
		case D_UPDATE_SEL_DEL:
			break;
		default: ASSERT(TRUE);
	}
}

void CDgnRCPlateBeamChkingDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_PBDC_ADD):
		case(UR_PBDC_DEL):
		case(UR_PBDC_MFD):
		case(UR_PBDC_MFS):
			{
				MakeItemEx();
			}
			break;		
		case(UR_SBDO_ADD):
		case(UR_SBDO_DEL):
			{
				InitSbdoCombo(TRUE);
			}
			break;
		case(UR_RIPB_ADD):
		case(UR_RIPB_DEL):
		case(UR_RIPB_MFD):
		case(UR_RIPB_MFS):
			{
				InitRipbCombo(TRUE);
			}
			break;
		default:
			break;
		}
	} 
}

void CDgnRCPlateBeamChkingDlg::OnDgnRadioElemAvg() 
{
	ControlsEnableDisable();
}

void CDgnRCPlateBeamChkingDlg::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	int nItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1)
	{
		*pResult = 1;
		return;
	}
	DWORD(Key) = m_List.GetItemData(nItem);    

	T_PBDC_D Data; Data.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->GetPbdc(Key, Data)) return;
	
	m_Data = Data;
	
	Data2Dlg();
	ControlsEnableDisable();
	*pResult = 0;
}

void CDgnRCPlateBeamChkingDlg::OnMod() 
{
	UpdateData(TRUE);	

	if(!Dlg2Data()) return;
	
	CArray<DWORD, DWORD> aSelKey;
	CDlgUtil::GetSelectedListItemData(&m_List, aSelKey);  
	if(aSelKey.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_DGN_RC_PLATE_NO_SEL_ITEM));
		return ;
	}
	if(aSelKey.GetSize() != 1)
	{
		AfxMessageBox(_LS(IDS_DGN_RC_PLATE_SEL_ITEM_ONE));
		return ;
	}
	T_PBDC_K PbdcK = aSelKey[0];

	m_pDoc->m_pDataCtrl->ModifyPbdc(PbdcK, PbdcK, m_Data);
	if(m_pDoc->m_pViewCtrl->SelectedNodeCount()) m_pDoc->m_pViewCtrl->UnselectAll(NULL);	
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CDgnRCPlateBeamChkingDlg::OnDel() 
{
	UpdateData(TRUE);	

	if(!Dlg2Data()) return;
	
	CArray<DWORD, DWORD> aSelKey;
	CDlgUtil::GetSelectedListItemData(&m_List, aSelKey);  

	if(aSelKey.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_DGN_RC_PLATE_NO_SEL_ITEM));
		return ;
	}
	ASSERT(aSelKey.GetSize() == 1);
	T_PBDC_K PbdcK = aSelKey[0];

	m_pDoc->m_pDataCtrl->DelPbdc(PbdcK);

	if(m_pDoc->m_pViewCtrl->SelectedNodeCount()) m_pDoc->m_pViewCtrl->UnselectAll(NULL);	
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CDgnRCPlateBeamChkingDlg::OnAdd() 
{
	UpdateData(TRUE);	

	if(!Dlg2Data()) return;
		
	m_pDoc->m_pDataCtrl->AddPbdc(m_Data);
	
	if(m_pDoc->m_pViewCtrl->SelectedNodeCount()) m_pDoc->m_pViewCtrl->UnselectAll(NULL);	
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CDgnRCPlateBeamChkingDlg::OnClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnRCPlateBeamChkingDlg::OnSbdoBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_SUBDOMAIN_DEFINE,0),0); // SubDomain 대화상자를 띄울려면 다양한 더러운 방법중 하나를 선택해야 하는데... wg_main 리소스 include를 택하겠다...
}

void CDgnRCPlateBeamChkingDlg::OnRebarBtn()
{
	CDgnRCPlateBeamRebarDlg dlg;
	dlg.DoModal();
}
