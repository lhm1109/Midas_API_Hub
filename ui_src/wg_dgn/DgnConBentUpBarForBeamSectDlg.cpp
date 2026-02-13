// DgnConBentUpBarForBeamSectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBentUpBarForBeamSectDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\CheckDialogOpen.h"
#include "..\wg_base\NumericOptimizer.h"
#include "..\wg_base\I_GENModelBase.h"
#include "..\wg_base\TreeMenuBarBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\ViewBuff.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"
#include "..\wg_db\EditData.h"
#include "..\wg_db\UndoCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBentUpBarForBeamSectDlg dialog
CDgnConBentUpBarForBeamSectDlg::CDgnConBentUpBarForBeamSectDlg(CWnd* pParent )
	: CDgnDlgBase(CDgnConBentUpBarForBeamSectDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	//m_Data.Initialize();

	m_pGridBentUpInfo = new CDgnConBentUpInfoGrid(this);

	//{{AFX_DATA_INIT(CDgnConBentUpBarForBeamSectDlg)	
	//}}AFX_DATA_INIT
}


CDgnConBentUpBarForBeamSectDlg::~CDgnConBentUpBarForBeamSectDlg()
{
	if(m_pGridBentUpInfo)
	{
		delete m_pGridBentUpInfo;
	}
}


void CDgnConBentUpBarForBeamSectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBentUpBarForBeamSectDlg)
	
	//DDX_Control(pDX, IDC_DGN_METHOD_CHECK3, m_chkDgnOutput3);

	DDX_Control(pDX, IDC_BENT_UP_BAR_LST, m_BurbList);
	DDX_Control(pDX, IDC_ASSIGN_ELEM_EDT, m_edtEList); 
	DDX_Control(pDX, IDC_NAME_EDT, m_edtName); 	
	DDX_Control(pDX, IDC_WG_DGN_STATIC0, m_wndPicture);

	DDX_Control(pDX, IDC_BENT_UP_BAR_INFO_GRD, *m_pGridBentUpInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConBentUpBarForBeamSectDlg, CDgnDlgBase)
	//{{AFX_MSG_MAP(CDgnConBentUpBarForBeamSectDlg)
	ON_NOTIFY(NM_CLICK, IDC_BENT_UP_BAR_LST, OnClkListItem)		
	ON_BN_CLICKED(IDC_CLOSE_BTN, OnOk)
	ON_BN_CLICKED(IDC_ADD_BTN, OnAdd)
	ON_BN_CLICKED(IDC_DEL_BTN, OnDel)
	ON_BN_CLICKED(IDC_MOD_BTN, OnMod)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CDgnConBentUpBarForBeamSectDlg::OnInitDialog() 
{
	CDgnDlgBase::OnInitDialog();
	
	CCheckDialogOpen::SetSoftwareRendering();
	
	SetControl();
	
// 	if(m_pDoc->m_pAttrCtrl2->ExistRakr())
// 	{
// 		m_pDoc->m_pAttrCtrl2->GetRakr(m_Data);
// 	}	
	RefreshListByDB();

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\BentUpBarGuide.svg"));
	m_wndPicture.SetIgnoreLargeView(TRUE);

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConBentUpBarForBeamSectDlg::SetControl()
{
	//m_editLiveLoad.SetUnitType(D_UNITSYS_NONE);	

	CNumericOptimizer NumOptimizer;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  	
	m_edtEList.Connect(SC_ID_ELEM, &m_edtEList);	
	pIGM->GetSelectedElemKeyListForDgn(m_Data.aElemList);  // m_edtEList에 대화상자열기전에 선택한 Elem이 세팅되어 있게 하기 위한 코드 (m_Data.aElemList에 내용을 채워 넣어 놓으면 Data2Dlg에서 m_edtEList의 Text를 세팅함)

	m_pGridBentUpInfo->Initialize();

	m_aXAxisDirRdo.RemoveAll();
	m_aXAxisDirRdo.Add(IDC_X_AXIS_DIR_I_TO_J_RDO);
	m_aXAxisDirRdo.Add(IDC_X_AXIS_DIR_J_TO_I_RDO);

	// set list
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_BurbList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_BurbList.GetSafeHwnd(), dwStyle);
	CString aTitle[4] = {_T(""), _LS(IDS_DGN_BENT_UP_BAR_NAME), _LS(IDS_DGN_DIRECTION), _LS(IDS_DGN_ELEM_LIST)};
	int		  aColWidth[4] = {20, 145, 75, 146};
	LV_COLUMN lvcolumn;
	CString title;
	for(int i = 0; i < 4; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(aColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_BurbList.InsertColumn(i,&lvcolumn);
		m_BurbList.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CDgnConBentUpBarForBeamSectDlg::Data2DlgGrid()
{
	m_pGridBentUpInfo->SetData(m_Data.aBurbSub);
}

void CDgnConBentUpBarForBeamSectDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
	case D_UPDATE_UNIT:
		m_pGridBentUpInfo->UpdateUnit();
		break;
	case D_UPDATE_SEL_ADD:    case D_UPDATE_SEL_DEL:
		
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	default:
		break;
	}
}

void CDgnConBentUpBarForBeamSectDlg::RefreshListByDB()
{
	CArray<T_BURB_K, T_BURB_K> aBurbK;
	m_pDoc->m_pAttrCtrl2->GetBurbKeyList(aBurbK);
	
	CArray<T_BURB_D, T_BURB_D&> aBurbDInList;
	m_aBurbKInList.RemoveAll();
	T_BURB_D EachBurbD;
	// ElemList가 비어있는걸 제일 위로
	for(int i = 0; i < aBurbK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl2->GetBurb(aBurbK[i], EachBurbD);
		if(EachBurbD.aElemList.GetSize() == 0)
		{
			m_aBurbKInList.Add(aBurbK[i]);
			aBurbDInList.Add(EachBurbD);
		}		
	}
	// ElemList가 있는걸 그 아래로
	for(int i = 0; i < aBurbK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl2->GetBurb(aBurbK[i], EachBurbD);
		if(EachBurbD.aElemList.GetSize() != 0)
		{
			m_aBurbKInList.Add(aBurbK[i]);
			aBurbDInList.Add(EachBurbD);
		}		
	}
 
	// ListCtrl에 내용 채우기
	CNumericOptimizer NumOptimizer;
	m_BurbList.DeleteAllItems();
	CString szTemp;
	for(int i = 0; i < aBurbDInList.GetSize(); i++)
	{
		LVITEM lvitem;
		
		// No. 1
		m_BurbList.InsertItem(i, _T(""));
		
		lvitem.iItem = i;
		lvitem.iSubItem = 0;		
		szTemp.Format(_T("%d"), i + 1);
		lvitem.pszText = szTemp.GetBuffer(0);
		lvitem.mask = LVIF_TEXT ;
		m_BurbList.SetItem(&lvitem);
		
		lvitem.iItem = i;
		lvitem.iSubItem = 1;
		szTemp = aBurbDInList[i].strName;
		lvitem.pszText = szTemp.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;
		m_BurbList.SetItem(&lvitem);
		
		lvitem.iItem = i;
		lvitem.iSubItem = 2;
		szTemp = aBurbDInList[i].nDir == 0 ?  _T("I -> J") : _T("J -> I");
		lvitem.pszText = szTemp.GetBuffer(0);
		lvitem.mask = LVIF_TEXT ;
		m_BurbList.SetItem(&lvitem);
		
		lvitem.iItem = i;
		lvitem.iSubItem = 3;
		szTemp = NumOptimizer.OptimizeSortedArray(aBurbDInList[i].aElemList);
		lvitem.pszText = szTemp.IsEmpty() ? _T("-") : szTemp.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;
		m_BurbList.SetItem(&lvitem);
	}
}

void CDgnConBentUpBarForBeamSectDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount == 0) return; 
	ASSERT(nCount == 1);
	
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	
	BOOL bChange = FALSE;
	T_REBW_K RebwK;   RebwK.keymap = 0;
	T_REBW_D RebwD;
	BOOL bChangeStor = FALSE;
	
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_BURB_ADD):
		case(UR_BURB_DEL):
			RefreshListByDB();
			break;
		default:						break;
		}
	}
}

void CDgnConBentUpBarForBeamSectDlg::Data2Dlg()
{
	m_edtName.SetWindowText(m_Data.strName);

	CNumericOptimizer NumOptimizer;
	m_edtEList.SetWindowText(NumOptimizer.OptimizeSortedArray(m_Data.aElemList));	
	SelElemByEdtEList();

	CDlgUtil::CtrlRadioSetCheck(this, m_aXAxisDirRdo, m_Data.nDir);

	Data2DlgGrid();	
}

void CDgnConBentUpBarForBeamSectDlg::Dlg2Data()
{
	m_edtName.GetWindowText(m_Data.strName);

	SelElemByEdtEList();
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(m_Data.aElemList);

	CDlgUtil::CtrlRadioGetCheck(this, m_aXAxisDirRdo, m_Data.nDir);

	Dlg2DataGrid();
}

void CDgnConBentUpBarForBeamSectDlg::Dlg2DataGrid()
{
	m_Data.aBurbSub.RemoveAll();
	m_pGridBentUpInfo->GetData(m_Data.aBurbSub);
}

/////////////////////////////////////////////////////////////////////////////
// CDgnConBentUpBarForBeamSectDlg message handlers
void CDgnConBentUpBarForBeamSectDlg::OnOk() 
{
	//Dlg2Data();
	
// 	if(!m_pDoc->m_pDataCtrl->AddRakr(m_Data))
// 		return;

	DestroyWindow();
}

BOOL CDgnConBentUpBarForBeamSectDlg::DestroyWindow() 
{
	CCheckDialogOpen::ResetSoftwareRendering();
	
	return CDgnDlgBase::DestroyWindow();
}

void CDgnConBentUpBarForBeamSectDlg::OnClkListItem( NMHDR* pNMHDR, LRESULT* pResult )
{
	CArray<int, int> aSelIdx;
	CDlgUtil::GetSelectedListItemIndex(&m_BurbList, aSelIdx);
	
	if(aSelIdx.GetSize() == 0) return;
	
	ASSERT(m_aBurbKInList.GetSize() > aSelIdx[0]);  // 여기서 ASSERT 걸리면 m_aBurbKInList와 m_BurbList의 내용이 다른 상황
	
	m_pDoc->m_pAttrCtrl2->GetBurb(m_aBurbKInList[aSelIdx[0]], m_Data);
	
	Data2Dlg();
}

BOOL CDgnConBentUpBarForBeamSectDlg::EndEdit(BOOL bEnd)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	
	if(bEnd && pDoc->m_pDataCtrl->CheckModifiedModel())
	{
		BOOL bUpdateDefault = FALSE;
		pDoc->m_pUndoCtrl->CloseEditDB(bUpdateDefault, FALSE, 0);
		return TRUE;
	}
	else
	{
		pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}
	return FALSE;
}


void CDgnConBentUpBarForBeamSectDlg::OnAdd()
{
	Dlg2Data();
	
	if(!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DGN_DATACTRL_ADD_BURB)), CMDTYPE_FREE)) return;

	if(!m_pDoc->m_pEditData->AddBurb(m_Data))
	{
		EndEdit(FALSE);
		return;
	}
	EndEdit(TRUE);

	CDlgUtil::UnselectAllListItems(&m_BurbList);
	// ListCtrl갱신은 OnUpdate()를 통해 됨
}

void CDgnConBentUpBarForBeamSectDlg::OnDel()
{
	CArray<int, int> aSelIdx;
	CDlgUtil::GetSelectedListItemIndex(&m_BurbList, aSelIdx);

	if(aSelIdx.GetSize() != 1)
	{
		AfxMessageBox(_LS(IDS_DGN__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	ASSERT(m_aBurbKInList.GetSize() > aSelIdx[0]);  // 여기서 ASSERT 걸리면 m_aBurbKInList와 m_BurbList의 내용이 다른 상황
	
	if(!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DGN_DATACTRL_DEL_BURB)), CMDTYPE_FREE)) return;

	if(!m_pDoc->m_pEditData->DelBurb(m_aBurbKInList[aSelIdx[0]]))
	{
		EndEdit(FALSE);
		return;
	}
	EndEdit(TRUE);

	CDlgUtil::UnselectAllListItems(&m_BurbList);
	// ListCtrl갱신은 OnUpdate()를 통해 됨
}

void CDgnConBentUpBarForBeamSectDlg::OnMod()
{
	CArray<int, int> aSelIdx;
	CDlgUtil::GetSelectedListItemIndex(&m_BurbList, aSelIdx);
	
	if(aSelIdx.GetSize() != 1)
	{
		AfxMessageBox(_LS(IDS_DGN__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}
	
	ASSERT(m_aBurbKInList.GetSize() > aSelIdx[0]);  // 여기서 ASSERT 걸리면 m_aBurbKInList와 m_BurbList의 내용이 다른 상황
	
	Dlg2Data();

	if(!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DGN_DATACTRL_MOD_BURB)), CMDTYPE_FREE)) return;
	if(!m_pDoc->m_pEditData->DelBurb(m_aBurbKInList[aSelIdx[0]]))
	{
		EndEdit(FALSE);
		return;
	}
	if(!m_pDoc->m_pEditData->AddBurb(m_Data))
	{
		EndEdit(FALSE);
		return;
	}
	EndEdit(TRUE);

	CDlgUtil::UnselectAllListItems(&m_BurbList);
	// ListCtrl갱신은 OnUpdate()를 통해 됨
}

void CDgnConBentUpBarForBeamSectDlg::SelElemByEdtEList()
{
	// text preserve
	CString strPreviousText = _T("");
	m_edtEList.GetWindowText(strPreviousText);
	
	// model select
	CString strBlank = _T("");
	m_edtEList.SelectByStr(strBlank);
	m_edtEList.SelectByStr(strPreviousText);
}