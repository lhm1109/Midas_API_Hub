// CMRebarCompositeDlg.cpp : implementation file
//
// 2004. 03. 09   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMRebarCompositeDlg.h"

#include "CMRebarCompositeMoment.h"
//#include "CMRebarPscShear.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"

#include "CMRebarCompositeCopyDlg.h"
#include "CMRebarCompositeStiffDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMRebarCompositeDlg dialog


CCMRebarCompositeDlg::CCMRebarCompositeDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMRebarCompositeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMRebarCompositeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_Key = 0;
	m_Data.Initialize();

	m_pDoc = 0;
	m_pDoc = CDBDoc::GetDocPoint();

	m_bModify = FALSE;
	m_bEndI = TRUE;

	m_pMomentDlg = 0;
	//m_pShearDlg = 0;
}

void CCMRebarCompositeDlg::SetParamData(T_RPSC_K RpscK)
{
	m_Key = RpscK;
	m_bModify = TRUE;
}

void CCMRebarCompositeDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMRebarCompositeDlg)
	DDX_Control(pDX, IDC_CMD_DELETE, m_btnDel);
	DDX_Control(pDX, IDOK, m_btnAdd);
	DDX_Control(pDX, IDC_CMD_SECT_LIST, m_wndList);
	DDX_Control(pDX, IDC_CMD_TAB, m_ctrlTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMRebarCompositeDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCMRebarCompositeDlg)
	ON_BN_CLICKED(IDC_CMD_DELETE, OnCmdDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_SECT_LIST, OnItemchangedCmdSectList)
	ON_NOTIFY(NM_CLICK, IDC_CMD_SECT_LIST, OnClickCmdSectList)
	ON_BN_CLICKED(IDC_CMD_COPY_BTN, OnClickCopyBtn)
	ON_BN_CLICKED(IDC_CMD_STIFFNESS_BTN, OnClickStiffnessBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMRebarCompositeDlg message handlers

BOOL CCMRebarCompositeDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	// preview control
	CWnd* pWnd = GetDlgItem(IDC_CMD_SP_ID_PREVIEW);
	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);

	this->LayoutChildDlg();

	SetHeaderTitle();
	MakeItemEx();

	m_btnAdd.EnableWindow(FALSE);
	m_btnDel.EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_STIFFNESS_BTN)->EnableWindow(FALSE);

	if (m_bModify)
	{
		int nItemCount = m_wndList.GetItemCount();
		int ix = 0;
		for (ix = 0; ix < nItemCount; ix++)
			if (m_Key == m_wndList.GetItemData(ix)) break;
		if (ix != nItemCount)
		{
			m_wndList.SetItemState(ix, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
			m_btnAdd.EnableWindow(TRUE);
			m_btnDel.EnableWindow(TRUE);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// add/replace
void CCMRebarCompositeDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	// get data
	if (!m_pMomentDlg->GetCurData(&m_Data))
	{
		AfxMessageBox(_LS(IDS_CMD_RPSC_ERR_MAIN_REBAR));
		return;
	}
	/*
	if (!m_pShearDlg->GetCurData(&m_Data))
	{
		AfxMessageBox(_LS(IDS_CMD_RPSC_ERR_SHEAR_REBAR));
		return;
	}*/

	CArray<T_RPSC_K, T_RPSC_K>  aKey;
	CArray<T_RPSC_D, T_RPSC_D&> aData;
	int nItem = -1;

	int nSelCount = m_wndList.GetSelectedCount();
	aKey.SetSize(nSelCount);
	aData.SetSize(nSelCount);
	for (UINT i=0; i < nSelCount; i++)
	{
		nItem = m_wndList.GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		aKey[i] = m_wndList.GetItemData(nItem);
		aData[i] = m_Data;
	}
	if (!m_pDoc->m_pDataCtrl->AddRpsc(aKey, aData))
		return;

	int nIndex = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	MakeItemEx();
	
	ChangeItem(nIndex);
	m_wndList.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);

	//CChildDialog::OnOK();
}

// close
void CCMRebarCompositeDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	((CDialog*)m_pParent)->EndDialog(IDCANCEL); // need change! added by mylee
	//CChildDialog::OnCancel();
}

// delete
void CCMRebarCompositeDlg::OnCmdDelete() 
{
	// TODO: Add your control notification handler code here
	T_RPSC_K key;
	CArray<T_RPSC_K, T_RPSC_K> aKey;
	int nItem = -1;

	int nSelCount = m_wndList.GetSelectedCount();
	for (UINT i=0; i < nSelCount; i++)
	{
		nItem = m_wndList.GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		key = m_wndList.GetItemData(nItem);
		if (m_pDoc->m_pAttrCtrl->ExistRpsc(key)) aKey.Add(key);
	}

	if (!m_pDoc->m_pDataCtrl->DelRpsc(aKey)) return;
	
	int nIndex = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	MakeItemEx();
	
	ChangeItem(nIndex);
	m_wndList.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMRebarCompositeDlg::OnClickCmdSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	m_btnAdd.EnableWindow(nIndex != -1);
	m_btnDel.EnableWindow(nIndex != -1);

	*pResult = 0;
}

void CCMRebarCompositeDlg::OnItemchangedCmdSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	/*
	if ((pNMListView->uNewState & LVIS_SELECTED) == 0)
	{
		nIndex = -1;
		//pResult = 0;
		//return;
	}
	else 
	*/
	
	//int nIndex = pNMListView->iItem;  
	int nIndex = m_wndList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED); // changed by mylee
	ChangeItem(nIndex);

	*pResult = 0;
}

void CCMRebarCompositeDlg::ChangeItem(int nIndex)
{
	// enable/disable
	m_btnAdd.EnableWindow(nIndex != -1);
	m_btnDel.EnableWindow(nIndex != -1);

	// get data
	m_Key = 0;
	m_Data.Initialize();
	m_SectD.Initialize();
	m_SectViewD.Initialize();
	if (nIndex != -1)
	{
	  m_Key = m_wndList.GetItemData(nIndex);
		if (!m_pDoc->m_pAttrCtrl->GetRpsc(m_Key, m_Data))
			m_Data.Initialize();
		m_pDoc->m_pAttrCtrl->GetSect(m_Key, m_SectD);
	}
	else
	{
		m_Data.Initialize();
	}

	// enable/disable Grid part2 select
	m_pMomentDlg->SetPart1Enabled(m_SectD.nStype == D_SECT_TYPE_COMPO_CI || 
																m_SectD.nStype == D_SECT_TYPE_COMPO_CT ||
																m_SectD.nStype == D_SECT_TYPE_COMPO_PC);

	// enable/disable Stiffness button
	BOOL bCompoBoxITub = (m_SectD.nStype == D_SECT_TYPE_COMPO_B      || m_SectD.nStype == D_SECT_TYPE_COMPO_I      || m_SectD.nStype == D_SECT_TYPE_COMPO_TUB || 
		                    m_SectD.nStype == D_SECT_TYPE_COMPO_STLG_B || m_SectD.nStype == D_SECT_TYPE_COMPO_STLG_I || m_SectD.nStype == D_SECT_TYPE_COMPO_STLG_TUB);
	BOOL bCompoTapBoxITub = (m_SectD.nStype == D_SECT_TYPE_TAPERED && 
													(m_SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_B      || m_SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_I      || m_SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB ||
													 m_SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B || m_SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I || m_SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB));
	GetDlgItem(IDC_CMD_STIFFNESS_BTN)->EnableWindow(bCompoBoxITub || bCompoTapBoxITub);

	// display data
	m_pMomentDlg->SetCurData(&m_Data);

	// update view
	if (bCompoTapBoxITub) // MNET:XXXX 20070306 mylee
	{
		CSectUtil::GetSectFromTapComposite(m_SectD, m_SectViewD, m_bEndI);
	}
	else if (m_SectD.nStype == D_SECT_TYPE_TAPERED)
	{
		CSectUtil::GetSectFromTapPsc(m_SectD, m_SectViewD, m_bEndI);
	}
	else m_SectViewD = m_SectD;
	m_wndSecView.SetDataSource(&m_SectViewD);
	m_wndSecView.SetCentroidFlag(TRUE);
	m_wndSecView.Invalidate();

}


void CCMRebarCompositeDlg::UpdateView(BOOL bEndI)
{
	m_bEndI = bEndI; // 나중에 리스트에서 바꿀때 I, J 선택해서 보여주기 위함

	BOOL bCompoTapBoxITub = (m_SectD.nStype == D_SECT_TYPE_TAPERED && 
													 (m_SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_B      || m_SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_I      || m_SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB ||
													  m_SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B || m_SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I || m_SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB)); 

	if (m_SectD.nStype != D_SECT_TYPE_TAPERED) return;
	m_SectViewD.Initialize();
	if (bCompoTapBoxITub) 
		CSectUtil::GetSectFromTapComposite(m_SectD, m_SectViewD, bEndI); // MNET:XXXX 20070306 mylee
	else               
		CSectUtil::GetSectFromTapPsc(m_SectD, m_SectViewD, bEndI);

	m_wndSecView.SetDataSource(&m_SectViewD);
	m_wndSecView.Invalidate();
}


/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMRebarCompositeDlg::LayoutChildDlg()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_CMD_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	m_pMomentDlg = new CCMRebarCompositeMoment(&m_wndSecView, this);
	m_ctrlTab.AddTab(m_pMomentDlg, _LS(IDS_CMD_RPSC_MAIN_REBAR), CCMRebarCompositeMoment::IDD, TRUE);

	//m_pShearDlg = new CCMRebarPscShear(this);
	//m_ctrlTab.AddTab(m_pShearDlg, _LS(IDS_CMD_RPSC_SHEAR_REBAR), CCMRebarPscShear::IDD, TRUE);

	m_ctrlTab.ShowTab(0);
}

void CCMRebarCompositeDlg::SetHeaderTitle()
{
	CStringArray aTitle;
	CArray<int, int> aWidth;

#define M_ADD_LIST_INFO(TITLE, WIDTH) aTitle.Add(TITLE); aWidth.Add(WIDTH)
	M_ADD_LIST_INFO(_LS(IDS_CMD_RPSC_ID),           30);
	M_ADD_LIST_INFO(_LS(IDS_CMD_RPSC_NAME),         90);
	M_ADD_LIST_INFO(_LS(IDS_CMD_RPSC_PART1),        45);
	M_ADD_LIST_INFO(_LS(IDS_CMD_RPSC_PART2),        45);
	M_ADD_LIST_INFO(_LS(IDS_CMD_RPSC_CRACK),        45);
#undef M_ADD_LIST_INFO

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_wndList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_wndList.GetSafeHwnd(), dwStyle);

	// Set Title
	int nNum = aTitle.GetSize();
	for (i=0; i < nNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(aWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_wndList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMRebarCompositeDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_wndList.DeleteAllItems();

	T_SECT_K Key;
	T_SECT_D Data;

	CArray<T_SECT_K, T_SECT_K> aKeyBuf;
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartSect();
	while (pos !=NULL)
	{
	  m_pDoc->m_pAttrCtrl->GetNextSect(pos, Key, Data);
		BOOL bCompoBoxITub = (Data.nStype == D_SECT_TYPE_COMPO_B      || Data.nStype == D_SECT_TYPE_COMPO_I      || Data.nStype == D_SECT_TYPE_COMPO_TUB ||
			                    Data.nStype == D_SECT_TYPE_COMPO_STLG_B || Data.nStype == D_SECT_TYPE_COMPO_STLG_I || Data.nStype == D_SECT_TYPE_COMPO_STLG_TUB);
		BOOL bCompoTapBoxITub = (Data.nStype == D_SECT_TYPE_TAPERED && 
														(Data.SectBefore.nStype == D_SECT_TYPE_COMPO_B      || Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I      || Data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB ||
		                         Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B || Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I || Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB));
		if ( !(bCompoBoxITub || bCompoTapBoxITub) ) continue; // MNET:XXXX 20070306 mylee
		/*
		if (Data.nStype != D_SECT_TYPE_COMPO_B && Data.nStype != D_SECT_TYPE_COMPO_I &&
				Data.nStype != D_SECT_TYPE_COMPO_CI && Data.nStype != D_SECT_TYPE_COMPO_CT) continue;
		*/
		
		aKeyBuf.Add(Key);
		/*if (Data.SectBefore.Shape == D_SECT_SHAPE_PSC_1CELL ||
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_2CELL ||
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_3CELL ||
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_4CELL ||
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_MID ||
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_HALF ||
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_T ||
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_BOX ||
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_I ||
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_NCEL2 || 
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB) 
		{
			aKeyBuf.Add(Key);
		}
		else if (Data.SectBefore.Shape == D_SECT_SHAPE_PSC_VALUE)
		{
			if (Data.SectBefore.SectI.aOuterPolygon.GetSize() > 0)  // Import한 경우만
				aKeyBuf.Add(Key);
		}*/
	}
	int nItemCount = aKeyBuf.GetSize();
	if (nItemCount == 0) return;

	for (int nCount = 0 ; nCount < nItemCount ; nCount++)
	{
	  Key=aKeyBuf[nCount];
		m_pDoc->m_pAttrCtrl->GetSect(Key,Data);
		InsertItem(Key, Data);
	}
}

BOOL CCMRebarCompositeDlg::InsertItem(T_SECT_K Key, T_SECT_D &Data)
{
	T_RPSC_D RData;
	if (!m_pDoc->m_pAttrCtrl->GetRpsc(Key, RData))
		RData.Initialize();

	LVITEM lvitem;
	CString str;
	int nItem = m_wndList.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i=0; i < 5; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr(i, Key, Data, RData);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if (i == 0) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_wndList.InsertItem(&lvitem);
		}
		else m_wndList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	m_wndList.SetItemData(nItem, (DWORD)Key);
	return TRUE;
}

int CCMRebarCompositeDlg::FindInsertionPos(T_SECT_K nID)
{
	T_SECT_K key;

	int nCount = m_wndList.GetItemCount();
	int i = 0;
	for (i=0; i < nCount; i++)
	{
		key = m_wndList.GetItemData(i);
		if (nID < key) break;
	}
	return i;
}

CString CCMRebarCompositeDlg::DataToStr(int i, T_SECT_K Key, T_SECT_D &Data, T_RPSC_D &RData)
{
	CString str;
	if (i == 0) str.Format(_T("%d"), Key);
	else if (i == 1) str = Data.SName;
	else if (i == 2) // part 1
	{
		str = _T("X");
		for (int i=0 ; i<RData.Mbar[0].GetSize() ; ++i)
			if(RData.Mbar[0][i].nPart == 1) { str = _T("O"); break; }
		for (i=0 ; i<RData.Mbar[1].GetSize() ; ++i)
			if(RData.Mbar[1][i].nPart == 1) { str = _T("O"); break; }
	}
	else if (i == 3) // part 2
	{
		str = _T("X");
		for (int i=0 ; i<RData.Mbar[0].GetSize() ; ++i)
			if(RData.Mbar[0][i].nPart == 2) { str = _T("O"); break; }
		for (i=0 ; i<RData.Mbar[1].GetSize() ; ++i)
			if(RData.Mbar[1][i].nPart == 2) { str = _T("O"); break; }
	}
	else if (i == 4) // crack
	{
		str = (RData.bCracked==TRUE? _T("O"):_T("X"));
	}
	else ASSERT(0);

	return str;
}

void CCMRebarCompositeDlg::OnClickCopyBtn()
{
	int nItem = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	T_RPSC_K RpscK = m_wndList.GetItemData(nItem);

	CCMRebarCompositeCopyDlg dlg(RpscK);
	dlg.SetInitPos(D_INIT_POS_RT);
	if(dlg.DoModal()==IDOK)
	{
		MakeItemEx();
	}
}

void CCMRebarCompositeDlg::OnClickStiffnessBtn()
{
	int nItem = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	T_RPSC_K RpscK = m_wndList.GetItemData(nItem);

	T_SECT_D Data;
	if(!m_pDoc->m_pAttrCtrl->GetSect(RpscK, Data)) return;

	BOOL bCompoBoxITub = (Data.nStype == D_SECT_TYPE_COMPO_B      || Data.nStype == D_SECT_TYPE_COMPO_I      || Data.nStype == D_SECT_TYPE_COMPO_TUB ||
		                    Data.nStype == D_SECT_TYPE_COMPO_STLG_B || Data.nStype == D_SECT_TYPE_COMPO_STLG_I || Data.nStype == D_SECT_TYPE_COMPO_STLG_TUB);
	BOOL bCompoTapBoxITub = (Data.nStype == D_SECT_TYPE_TAPERED && 
													(Data.SectBefore.nStype == D_SECT_TYPE_COMPO_B      || Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I      || Data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB ||
													 Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B || Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I || Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB));
	if ( !(bCompoBoxITub || bCompoTapBoxITub) ) return; 
		//  Data.nStype != D_SECT_TYPE_COMPO_CI && Data.nStype != D_SECT_TYPE_COMPO_CT) continue;

	/*
	CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS&> aData;
	CStringArray aTitle;

	aData.Add(SectData.SectBefore.SectI.Stiffness);
	CString strTest1 = _T("Test1");
	aTitle.Add(strTest);
	*/

	T_SECT_SECTBASE_D SectBase[2];
	BOOL bConsiderPost = FALSE; 
	CSectUtil::GetCompoSectCR4SectK(RpscK, bConsiderPost, SectBase, TRUE);
	
	CArray<T_SECT_SECTBASE_D, T_SECT_SECTBASE_D&> aData;
	CStringArray aTitle;

	aData.Add(SectBase[0]);
	aData.Add(SectBase[1]);
	CString strTest1 = _LS(IDS_WG_CMD__ADDD__Value_I_);//_T("Value(I)");
	CString strTest2 = _LS(IDS_WG_CMD__ADDD__Value_J_);//_T("Value(J)");
	aTitle.Add(strTest1);
	aTitle.Add(strTest2);

	CCMRebarCompositeStiffDlg dlg(m_pDoc, &aData, &aTitle, TRUE, this);
	dlg.DoModal();
}
