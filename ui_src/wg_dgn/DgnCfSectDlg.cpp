// DgnCfSectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCfSectDlg.h"

#include "DgnDataCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"

#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLCOUNT 5

/////////////////////////////////////////////////////////////////////////////
// CDgnCfSectDlg dialog


CDgnCfSectDlg::CDgnCfSectDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnCfSectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCfSectDlg)
	m_Key = NULL;
	m_Data.Initialize();
	//}}AFX_DATA_INIT
}


void CDgnCfSectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCfSectDlg)
	DDX_Control(pDX, IDC_DGN_SECT_LIST, m_List);
	DDX_Control(pDX, IDC_DGN_SECT_COATING_THIK_EDT, m_edtCoatingThik);
	DDX_Control(pDX, IDC_DGN_SECT_COATING_THIK_UNIT, m_wndCoatingThik);
	//}}AFX_DATA_MAP

	UINT aFirstID[][10] = {
		{ IDC_DGN_SECT_ID_TITLE1, IDC_DGN_SECT_ID_TITLE2, IDC_DGN_SECT_ID_TITLE3,
			IDC_DGN_SECT_ID_TITLE4, IDC_DGN_SECT_ID_TITLE5, IDC_DGN_SECT_ID_TITLE6,
			IDC_DGN_SECT_ID_TITLE7, IDC_DGN_SECT_ID_TITLE8, IDC_DGN_SECT_ID_TITLE9, 
			IDC_DGN_SECT_ID_TITLE10 },
		{ IDC_DGN_SECT_ID_VALUE1, IDC_DGN_SECT_ID_VALUE2, IDC_DGN_SECT_ID_VALUE3,
			IDC_DGN_SECT_ID_VALUE4, IDC_DGN_SECT_ID_VALUE5, IDC_DGN_SECT_ID_VALUE6,
			IDC_DGN_SECT_ID_VALUE7, IDC_DGN_SECT_ID_VALUE8, IDC_DGN_SECT_ID_VALUE9,
			IDC_DGN_SECT_ID_VALUE10 },
		{ IDC_DGN_SECT_ID_UNIT1,  IDC_DGN_SECT_ID_UNIT2,  IDC_DGN_SECT_ID_UNIT3,
			IDC_DGN_SECT_ID_UNIT4,  IDC_DGN_SECT_ID_UNIT5,  IDC_DGN_SECT_ID_UNIT6,
			IDC_DGN_SECT_ID_UNIT7,  IDC_DGN_SECT_ID_UNIT8,  IDC_DGN_SECT_ID_UNIT9,
			IDC_DGN_SECT_ID_UNIT10 },
	};
	for (int i = 0; i < 10; i++)
	{
		DDX_Control(pDX, aFirstID[0][i], m_wndFirstTitle[i]);
		DDX_Control(pDX, aFirstID[1][i], m_wndFirstSize[i]);
		DDX_Control(pDX, aFirstID[2][i], m_wndFirstUnit[i]);
	}
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnCfSectDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnCfSectDlg)
	ON_NOTIFY(NM_CLICK, IDC_DGN_SECT_LIST, OnClickList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_SECT_LIST, OnItemChanged)
	ON_BN_CLICKED(IDC_DGN_SECT_MODIFY, OnModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCfSectDlg message handlers

BOOL CDgnCfSectDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	CWnd* pWnd = GetDlgItem(IDC_DGN_SECT_PREVIEW);
	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);
	m_wndSecView.SetCentroidFlag(TRUE);

	// Unit control
	for (int i = 0; i < 10; i++)
	{
		m_wndFirstSize[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_wndFirstUnit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}

	m_edtCoatingThik.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndCoatingThik.SetUnitType(D_UNITSYS_BASE_LENGTH);

	Initial_ListCtrl();
	Initial_Data();
	Initial_Unit();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnCfSectDlg::Initial_Unit()
{
}

void CDgnCfSectDlg::Initial_ListCtrl()
{
	CString strCoating = _T("");
	CString strUnit;
	m_pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_LENGTH, strUnit);
	strCoating.Format(_T("%s(%s)"), _LS(IDS_CMD_SECTION_COATING), strUnit);

	CString aTitle[] = { _LS(IDS_CMD_SECTION_id), _LS(IDS_CMD_SECTION_name),_LS(IDS_CMD_SECTION_type), _LS(IDS_CMD_SECTION_shape), strCoating };
		
	int nColWidth[COLCOUNT];
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 45; nColWidth[1] = 130; nColWidth[2] = 45; nColWidth[3] = 50; nColWidth[4] = 100;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		if (i == 0) lvcolumn.fmt = LVCFMT_RIGHT;
		else lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CDgnCfSectDlg::Initial_Data()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	LV_ITEM lvitem;
	POSITION pos;
	CString str;
	 
	T_SECT_K Key;
	T_SECT_D Data;

	int nItemCount=0;
	std::vector<T_SECT_K> aSectK; aSectK.clear();
	CString strCfsCodeName = CDBLib::GetCfsCodeName();
	if(strCfsCodeName==_T("Eurocode3-1-3:06"))
	{
		pos = m_pDoc->m_pAttrCtrl->GetStartSectDesign();
		while(pos !=NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextSectDesign(pos, Key, Data);
			if(Data.SectBefore.Shape != D_SECT_SHAPE_REG_CC) continue;
			aSectK.push_back(Key);
			nItemCount++;
		}
		qsort(aSectK.data(), nItemCount, sizeof(T_SECT_K), CCompFunc::UINTAsc);
	}

	if(nItemCount == 0)
	{
		int nShapeIndex=10;
		CStringArray aSizeTitle;
		CSectUtil::GetSizeTitleFromShapeIndexReg(nShapeIndex, aSizeTitle);

		double Size[D_SECT_SIZE_NUM_MAX] = { 0., };
		Data2Dlg_Size(aSizeTitle, Size, FALSE);

		((CButton*)GetDlgItem(IDC_DGN_SECT_FORMING1_RDO))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_DGN_SECT_FORMING2_RDO))->SetCheck(FALSE);

		return;
	}

	int ix = 0;
	for(std::vector<T_SECT_K>::iterator IterPos=aSectK.begin(); IterPos!=aSectK.end(); ++IterPos)
	{
		Key = *IterPos;
		if(!m_pDoc->m_pAttrCtrl->GetSectDesign(Key, Data)) { ASSERT(0); continue; }

		for(int i = 0; i < COLCOUNT; i++)
		{
			lvitem.iItem=ix;
			lvitem.iSubItem=i;
			str = DataToStr(i, Key, Data);
			lvitem.pszText=str.GetBuffer(0);
			lvitem.mask=LVIF_TEXT ;
			if( i == 0 )
				m_List.InsertItem(&lvitem);
			else
				m_List.SetItem(&lvitem);
			str.ReleaseBuffer();
		}
		ix++;
	}

	if(ix>0) m_List.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CDgnCfSectDlg::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	/// OnItemChanged(pNMHDR,pResult);
	*pResult = 0;
}

void CDgnCfSectDlg::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if ((pNMListView->uChanged & LVIF_STATE) 
		&& (pNMListView->uNewState & LVIS_SELECTED))
	{
		int nItem = pNMListView->iItem;
		CString strID = m_List.GetItemText(nItem,0);
		m_Key = _ttoi(strID);

		// TODO: Add your control notification handler code here
		if (m_Key == NULL) 
			m_Data.Initialize();
		else
		{
			//if (!m_pDoc->m_pAttrCtrl->GetSect(m_Key, m_Data)) return;
			if (!m_pDoc->m_pAttrCtrl->GetSectDesign(m_Key, m_Data)) return;

		}
		m_wndSecView.SetDataSource(&m_Data);
		m_wndSecView.Invalidate();

		SetFirstSectData();
		m_edtCoatingThik.SetEditUnit(m_Data.dCoatingThik);
		if(m_Data.nFormingType == 0)
		{
			((CButton*)GetDlgItem(IDC_DGN_SECT_FORMING1_RDO))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_DGN_SECT_FORMING2_RDO))->SetCheck(FALSE);
		}
		else if(m_Data.nFormingType == 1)
		{
			((CButton*)GetDlgItem(IDC_DGN_SECT_FORMING1_RDO))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_DGN_SECT_FORMING2_RDO))->SetCheck(TRUE);
		}
	}

	*pResult = 0;
}

void CDgnCfSectDlg::EnableEditBox(BOOL bCheck)
{
}

void CDgnCfSectDlg::OnModify() 
{
	int nItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_MATL),MB_OK);
		return;
	}

	CString strName=_T(""), strMatName=_T("");
	CString strID = m_List.GetItemText(nItem, 0);
	m_Key = _ttoi(strID);
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(m_Key, m_Data)) return;

	UpdateData(TRUE);

	BOOL bCheck=TRUE;
	CString strMsg = _LS(IDS_DGN_CHK_TITLE_ERROR);

	double dCoatingThik = m_edtCoatingThik.GetEditValue();
	if(dCoatingThik < 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_SECT_COATING_THIK_ERROR);}
	if(dCoatingThik >= m_Data.SectBefore.SectI.Size[2]) {bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_SECT_COATING_THIK_ERROR2);} // [MQC 5108-70] Coating Thickness에는 tw 보다 크거나 같은 값은 입력이 안되도록 입력제한 처리 : JWKWON-2016-02-17

	if(!bCheck)	AfxMessageBox(strMsg, MB_OK);

	if(bCheck)
	{
		BOOL bDiffCoatingThik = FALSE;
		if(m_Data.dCoatingThik != dCoatingThik) bDiffCoatingThik = TRUE;
		m_Data.dCoatingThik = dCoatingThik;
		
		int nSelType = 0;
		if(((CButton*)GetDlgItem(IDC_DGN_SECT_FORMING1_RDO))->GetCheck())
			nSelType = 0;
		else if(((CButton*)GetDlgItem(IDC_DGN_SECT_FORMING2_RDO))->GetCheck())
			nSelType = 1;
		BOOL bDiffType = FALSE;
		if(m_Data.nFormingType != nSelType) bDiffType = TRUE;
		m_Data.nFormingType = nSelType;

//     if(bDiffCoatingThik || bDiffType) //[MQC 5108-85] modify만 눌러도 설계결과를 지운다는 메시지 나오도록 함(다른것들과의 통일성 때문) : JWKWON-2015-02-15
//     {
			CArray<T_SECT_K, T_SECT_K> aKey;
			CArray<T_SECT_D, T_SECT_D&> aData;
			aKey.Add(m_Key);
			aData.Add(m_Data);
			//BOOL bCheck = m_pDoc->m_pDataCtrl->AddSect(aKey, aData, TRUE);
			BOOL bCheck = m_pDoc->m_pDataCtrl->AddSecd(aKey, aData, CMDTYPE_REMOVE_CFSTEEL);

			if(bDiffCoatingThik)
			{
				int nCol = 4;
				LV_ITEM lvitem;
				lvitem.iItem=nItem;
				lvitem.iSubItem=nCol;
				CString str = DataToStr(nCol, m_Key, m_Data);
				lvitem.pszText=str.GetBuffer(0);
				lvitem.mask=LVIF_TEXT ;
				m_List.SetItem(&lvitem);
				str.ReleaseBuffer();
			}
//     }
	}
}

void CDgnCfSectDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

CString CDgnCfSectDlg::GetDesignCodeName()
{	
	CString strDesingCode = _T("");
	if(m_pDoc->m_pAttrCtrl->ExistDcfs())
	{
		T_DCFS_D rData;
		rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetDcfs(rData);
		strDesingCode = rData.DesignCode;			
	}
	else
	{
		T_PREFERENCE rPref;
		rPref.Initialize();
		m_pDoc->m_pInitCtrl->GetPreference(rPref);
		strDesingCode = rPref.DgnCode.CFSteelCode;				
	}

	return strDesingCode;
}

CString CDgnCfSectDlg::DataToStr(int i, T_SECT_K Key, T_SECT_D &Data)
{
	CString str;

	if(i==0) str.Format(_T("%6d"),Key);
	else if(i==1) str = Data.SName;
	else if(i==2) 
	{
		switch(Data.nStype)
		{
		case D_SECT_TYPE_REGULAR: 
			{
				if (Data.SectBefore.SectI.DBName == _T("")) str = _LS(IDS_WG_CMD__ADD2__User);
				else str =  _LS(IDS_WG_CMD__ADD2__DB);
			}
			break;
		case D_SECT_TYPE_USER:           str = _LS(IDS_WG_CMD__ADDD__Value);       break;
		case D_SECT_TYPE_SRC:            str = _LS(IDS_WG_CMD__ADDD_SECT_SRC);     break;
		case D_SECT_TYPE_COMBINED:       str = _LS(IDS_WG_CMD__ADDD__Combined);    break;
		case D_SECT_TYPE_TAPERED:        str = _LS(IDS_WG_CMD__ADDD__Tapered);     break;
		case D_SECT_TYPE_CONSTRUCTION:   str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_HIBEAM:         str = _LS(IDS_WG_CMD__ADDD__Hi_Beam);     break;
		case D_SECT_TYPE_COLDFORMED:     str = _LS(IDS_WG_CMD__ADDD__Cold_Formed); break;
		case D_SECT_TYPE_COMPO_B:        str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_I:        str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_TUB:      str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_STLG_B:   str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_STLG_I:   str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_STLG_TUB: str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_CI:       str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_CT:       str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_PC:       str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_G:        str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_PSC:            str = _LS(IDS_WG_CMD__ADDD__PSC);         break;
		case D_SECT_TYPE_STLG_B:				 str = _LS(IDS_WG_CMD__ADDD__STLGIRDER);   break;
		case D_SECT_TYPE_STLG_I:         str = _LS(IDS_WG_CMD__ADDD__STLGIRDER);   break;
		case D_SECT_TYPE_STLG_MCELL:     str = _LS(IDS_WG_CMD__ADDD__STLGIRDER);   break;
		}
	}
	else if(i==3) 
	{
		str = Data.SectBefore.Shape;
		CSectUtil::CustomizeShapeNameByLanguage(Data.nStype, str, FALSE);
	}
	else if(i==4)
	{
		str.Format(_T("%g"), Data.dCoatingThik);
	}
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

void CDgnCfSectDlg::SetFirstSectData()
{
	int nShapeIndex;
	CStringArray aSizeTitle;
	nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0) return;

	CSectUtil::GetSizeTitleFromShapeIndexReg(nShapeIndex, aSizeTitle);

	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	BOOL bEnable = (pSect->DBName == _T("")) ? TRUE : FALSE;
	Data2Dlg_Size(aSizeTitle, pSect->Size, bEnable);
}

void CDgnCfSectDlg::Data2Dlg_Size(CStringArray& aSizeTitle, double* pSize, BOOL bEnable)
{
	CString csValue;
	int nSizeNum = aSizeTitle.GetSize();
	for (int i=0; i<nSizeNum; i++)
	{
		m_wndFirstTitle[i].ShowWindow(SW_SHOW);
		m_wndFirstSize[i].ShowWindow(SW_SHOW);
		m_wndFirstUnit[i].ShowWindow(SW_SHOW);
		m_wndFirstTitle[i].SetWindowText(aSizeTitle[i]);
		csValue.Format(_T("%g"), pSize[i]);
		m_wndFirstSize[i].SetWindowText(csValue);
		m_wndFirstSize[i].EnableWindow(bEnable);
	}
	for (int i=nSizeNum; i<10; i++)
	{
		m_wndFirstTitle[i].ShowWindow(SW_HIDE);
		m_wndFirstSize[i].ShowWindow(SW_HIDE);
		m_wndFirstUnit[i].ShowWindow(SW_HIDE);
	}
}