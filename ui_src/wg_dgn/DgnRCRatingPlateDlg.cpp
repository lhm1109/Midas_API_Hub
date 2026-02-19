// DgnRCRatingPlateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRCRatingPlateDlg.h"
#include "DgnRCRatingPlateRebarDlg.h"

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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnRCRatingPlateDlg dialog
CDgnRCRatingPlateDlg::CDgnRCRatingPlateDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnRCRatingPlateDlg::IDD, pParent)
{
	m_nOption = 0;
	m_nLocalUCS = 0;
	m_nElemAvg = 0;
	m_bAvgNodal = FALSE;
	m_nXYDir = 0;
 
	m_pDoc = CDBDoc::GetDocPoint();
	m_Data.Initialize();
}

CDgnRCRatingPlateDlg::~CDgnRCRatingPlateDlg()
{
}

void CDgnRCRatingPlateDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRCRatingPlateDlg)
	DDX_Radio  (pDX, IDC_DGN_RATING_RC_PLATE_MEMB_OPT_AR_RDO, m_nOption);
	DDX_Control(pDX, IDC_DGN_RATING_RC_PLATE_NAME_EDT,        m_edtName);
	DDX_Radio  (pDX, IDC_DGN_RATING_RC_PLATE_LOCAL_RDO,       m_nLocalUCS);
	DDX_Control(pDX, IDC_DGN_RATING_RC_PLATE_UCS_CMB,         m_cmbUCS);
	DDX_Radio  (pDX, IDC_DGN_RATING_RC_PLATE_ELEM_RDO,        m_nElemAvg);
	DDX_Check  (pDX, IDC_DGN_RATING_RC_PLATE_AVG_NODAL_CHK,   m_bAvgNodal);
	DDX_Radio  (pDX, IDC_DGN_RATING_RC_PLATE_X_RDO,           m_nXYDir);
	DDX_Control(pDX, IDC_DGN_RATING_RC_PLATE_LIST,            m_List);
	//}}AFX_DATA_MAP
}


void CDgnRCRatingPlateDlg::InitCombo()
{
	m_cmbUCS.ResetContent();

	CArray<CString,CString&> arNUCSNames;
	m_pDoc->m_pAttrCtrl->GetNucsList(arNUCSNames);

	for(int i=0; i<arNUCSNames.GetSize(); i++)
	{
		m_cmbUCS.AddString(arNUCSNames[i]);
	}
	m_cmbUCS.SetCurSel(0);  
}

void CDgnRCRatingPlateDlg::Data2Dlg()
{
	m_edtName.SetWindowText(m_Data.strName);
	m_nLocalUCS = m_Data.iAxisType;

	T_NUCS_D USCData; USCData.Initialize();
	m_pDoc->m_pAttrCtrl->GetNucs(m_Data.ucsKey, USCData);
	int Index = m_cmbUCS.FindStringExact(-1, USCData.UcsName);
	if(Index!=-1)	m_cmbUCS.SetCurSel(Index);
	else          m_cmbUCS.SetCurSel(0);

	if(m_cmbUCS.GetCount() == 0) m_nLocalUCS = 0;
	
	m_nElemAvg = m_Data.iAnaType;
	m_bAvgNodal = m_Data.bActiveOnly;
	m_nXYDir   = m_Data.iDir;

	m_pDoc->m_pViewCtrl->SelectElem(NULL, m_Data.arElemK, FALSE, TRUE);

	UpdateData(FALSE);
}

BOOL CDgnRCRatingPlateDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_edtName.GetWindowText(m_Data.strName);

	CArray<T_ELEM_K, T_ELEM_K> arElemK;	
	m_pDoc->m_pViewCtrl->GetAllSelectedElem(arElemK);
	qsort(arElemK.GetData(), arElemK.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);
	m_Data.arElemK.Copy(arElemK);

	m_Data.nPlateMemb = 0;
	m_Data.iAxisType = m_nLocalUCS;

	CString strTemp;
	m_cmbUCS.GetWindowText(strTemp);
	m_Data.ucsKey = m_pDoc->m_pAttrCtrl->GetNucsKey(strTemp);	

	m_Data.iAnaType    = m_nElemAvg;
	m_Data.bActiveOnly = m_bAvgNodal;
	m_Data.iDir        = m_nXYDir;	

	return TRUE;
}

void CDgnRCRatingPlateDlg::ControlsEnableDisable()
{
	UpdateData(TRUE);

	BOOL bEnable = FALSE;

	T_RCDC_D DgnCodeD; DgnCodeD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRcdc(DgnCodeD);

	if(DgnCodeD.nRatingCode == KSCE_USD10_RC_RATING || DgnCodeD.nRatingCode == KSCE_RAIL_USD11_RC_RATING || DgnCodeD.nRatingCode == KSCE_LSD15_RC_RATING) 
	{
		if(m_nOption == 0) bEnable = TRUE;  // Add/Replace
		else               bEnable = FALSE; // Delete
	}

	CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_RATING_RC_PLATE_MEMB_GRP, bEnable, TRUE);

	GetDlgItem(IDC_DGN_RATING_RC_PLATE_UCS_RDO)->EnableWindow(bEnable && m_cmbUCS.GetCount()!=0);
	GetDlgItem(IDC_DGN_RATING_RC_PLATE_UCS_CMB)->EnableWindow(bEnable && m_nLocalUCS == 1 && m_cmbUCS.GetCount()!=0);
	GetDlgItem(IDC_DGN_RATING_RC_PLATE_AVG_NODAL_CHK)->EnableWindow(bEnable && m_nElemAvg==1);
}

void CDgnRCRatingPlateDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_DGN_LIST_ID), _LS(IDS_DGN_LIST_NAME), _LS(IDS_DGN_CHK_ELEM_LIST)};
	int nColWidth[] = {30, 70, 90};
	int nAlign[] = {LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_LEFT};
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


void CDgnRCRatingPlateDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl2->GetCountRrpl();
	if(nItemCount == 0) return;

	POSITION pos;
	int nIndex;  

	T_RRPL_K Key;
	T_RRPL_D Data;
	CString strID, strMembName, strElemList;

	int* KeyBuf=new int[nItemCount];
	int nCount = 0;
	pos = m_pDoc->m_pAttrCtrl2->GetStartRrpl();
	while(pos !=NULL)
	{
		m_pDoc->m_pAttrCtrl2->GetNextRrpl(pos, Key, Data);		  
		KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nItemCount, sizeof(T_ELEM_K), CCompFunc::UINTAsc);

	for(nCount = 0 ; nCount < nItemCount ; nCount++)
	{
		Key = KeyBuf[nCount];

		m_pDoc->m_pAttrCtrl2->GetRrpl(Key, Data);

		strID.Format(_T("%d"), Key);
		nIndex = m_List.InsertItem(nCount, strID);      

		strMembName.Format(_T("%s"), Data.strName);
		m_List.SetItemText(nIndex, 1, strMembName);		

		CString csItem;
		strElemList = _T("");
        int i = 0;
		for(i = 0; i < Data.arElemK.GetSize() - 1; i++)
		{
			csItem.Format(_T("%d"), Data.arElemK[i]);
			strElemList += csItem + _T(", ");
		}
		if(Data.arElemK.GetSize() > 0)
		{
			csItem.Format(_T("%d"), Data.arElemK[i]);
			strElemList += csItem;
		}
		m_List.SetItemText(nIndex, 2, strElemList);		
		m_List.SetItemData(nIndex, (DWORD)Key);
	}
	delete []KeyBuf;
}

void CDgnRCRatingPlateDlg::InsertItem(T_RRPL_K Key)
{
	T_RRPL_D Data; Data.Initialize();
	if (!m_pDoc->m_pAttrCtrl2->GetRrpl(Key, Data)) return;

	int nSize = m_List.GetItemCount();
		
	CString strID;
	CString strMembName;     

	strID.Format(_T("%d"), Key);
	int nIndex = m_List.InsertItem(nSize, strID);      

	strMembName.Format(_T("%s"), Data.strName);
	m_List.SetItemText(nIndex, 1, strMembName);

	CString csItem;
	CString strElemList = _T("");

    int i = 0;
	for(i = 0; i < Data.arElemK.GetSize() - 1; i++)
	{
		csItem.Format(_T("%d"), Data.arElemK[i]);
		strElemList += csItem + _T(", ");
	}
	if(Data.arElemK.GetSize() > 0)
	{
		csItem.Format(_T("%d"), Data.arElemK[i]);
		strElemList += csItem;
	}
	m_List.SetItemText(nIndex, 2, strElemList);
	m_List.SetItemData(nIndex, (DWORD)Key);
}

void CDgnRCRatingPlateDlg::DeleteItem(T_RRPL_K Key)
{
	int nSize = m_List.GetItemCount();
    int nCount = 0;
	for(nCount = 0; nCount < nSize; nCount++)
	{
		if(m_List.GetItemData(nCount) == Key) break;
	}  
	if(nCount == nSize) return;
	else m_List.DeleteItem(nCount);
}

void CDgnRCRatingPlateDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			m_pDoc->m_pUnitCtrl->ConvertUnitRrplPrevious(m_Data);
			break;
		case D_UPDATE_SEL_ADD: 
		case D_UPDATE_SEL_DEL:
			break;
		default: ASSERT(TRUE);
	}
}

void CDgnRCRatingPlateDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_RRPL_K Key;
	T_RRPL_D Data;

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_RRPL_ADD):
			{
				pViewBuff->GetRrpl(nKey, Key, Data);
				InsertItem(Key);
			}
			break;
		case(UR_RRPL_DEL):
			{
				pViewBuff->GetRrpl(nKey, Key, Data);
				DeleteItem(Key);
			}
			break;
		case(UR_DCON_ADD): // Design Code 변경
		case(UR_DCON_DEL):	
			{
				ControlsEnableDisable();
				break;
			}
		case(UR_NUCS_ADD):
		case(UR_NUCS_DEL):
			InitCombo();
			break;
		default:
			break;
		}
	} 

	MakeItemEx();
}


BEGIN_MESSAGE_MAP(CDgnRCRatingPlateDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnRCRatingPlateDlg) 
	ON_BN_CLICKED(IDC_DGN_RATING_RC_PLATE_MEMB_OPT_AR_RDO,   OnDgnOptionRdo)
	ON_BN_CLICKED(IDC_DGN_RATING_RC_PLATE_MEMB_OPT_DEL_RDO,  OnDgnOptionRdo)
	ON_BN_CLICKED(IDC_DGN_RATING_RC_PLATE_LOCAL_RDO,         OnDgnLocalUCSRdo)
	ON_BN_CLICKED(IDC_DGN_RATING_RC_PLATE_UCS_RDO,           OnDgnLocalUCSRdo)
	ON_BN_CLICKED(IDC_DGN_RATING_RC_PLATE_ELEM_RDO,          OnDgnRadioElemAvg)
	ON_BN_CLICKED(IDC_DGN_RATING_RC_PLATE_AVG_RDO,           OnDgnRadioElemAvg)
	ON_BN_CLICKED(IDC_DGN_RATING_RC_PLATE_REBAR_BTN,         OnDgnPlateRebarInfo)
// 	ON_BN_CLICKED(IDC_DGN_RATING_RC_PLATE_APPLY,             OnDgnApply)
// 	ON_BN_CLICKED(IDC_DGN_RATING_RC_PLATE_CLOSE,             OnDgnClose)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_RATING_RC_PLATE_LIST, OnItemchanged)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRCRatingPlateDlg message handlers
BOOL CDgnRCRatingPlateDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	InitCombo();
	SetListCtrlHeader();
	MakeItemEx();

	ControlsEnableDisable();

	GetDlgItem(IDC_DGN_RATING_RC_PLATE_APPLY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RATING_RC_PLATE_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnRCRatingPlateDlg::OnDgnOptionRdo() 
{
	ControlsEnableDisable();
}

void CDgnRCRatingPlateDlg::OnDgnLocalUCSRdo() 
{
	ControlsEnableDisable();
}

void CDgnRCRatingPlateDlg::OnDgnRadioElemAvg() 
{
	ControlsEnableDisable();
}

void CDgnRCRatingPlateDlg::OnDgnPlateRebarInfo()
{
	// 두께 정보
// 	T_ELEM_D eData;
// 	eData.Initialize();
// 
// 	if(m_Data.arElemK.GetSize()==0)
// 	{
// 		CArray<T_ELEM_K, T_ELEM_K> arElemK;	
// 		m_pDoc->m_pViewCtrl->GetAllSelectedElem(arElemK);
// 		qsort(arElemK.GetData(), arElemK.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);
// 		if(arElemK.GetSize()==0)
// 		{
// 			AfxMessageBox(_LS(IDS_DGN_RRPL_NO_SEL_ELEM));
// 			return;
// 		}
// 
// 		T_ELEM_D ElemD; ElemD.Initialize();
// 		for(int i=0; i<arElemK.GetSize(); i++)
// 		{
// 			if(!m_pDoc->m_pAttrCtrl->GetElem(arElemK[i], ElemD)) 
// 			{
// 				AfxMessageBox(_LS(IDS_DGN_RRPL_ELEM_ERROR));
// 				return;
// 			}
// 			if(!m_pDoc->m_pAttrCtrl->IsPlate(ElemD.eltyp))
// 			{
// 				AfxMessageBox(_LS(IDS_DGN_RRPL_PLATE_ELEM_ERROR));
// 				return;
// 			}
// 		}
// 
// 		m_Data.arElemK.Copy(arElemK);
// 	}
// 
// 	if(!m_pDoc->m_pAttrCtrl->GetElem(m_Data.arElemK.GetAt(0), eData))	ASSERT(0); // 첫번째 요소만 확인.
// 	int iThik = eData.elpro;
// 
// 
// 	// 철근정보 대화창
// 	CDgnRCRatingPlateRebarDlg dlg;
// 	dlg.SetData(iThik, m_Data.Rebar);

	CDgnRCRatingPlateRebarDlg dlg;
	dlg.SetData(0, m_Data.Rebar);

	if(dlg.DoModal()==IDOK)
	{
		T_RRPL_SUB_D Rebar; Rebar.Initialize();
		dlg.GetData(Rebar);
		m_Data.Rebar = Rebar;
	}
}

void CDgnRCRatingPlateDlg::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
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

	T_RRPL_D Data; Data.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->GetRrpl(Key, Data)) return;
	
	m_Data = Data;
	
	Data2Dlg();
	ControlsEnableDisable();
	*pResult = 0;
}

void CDgnRCRatingPlateDlg::Execute()
{
	UpdateData(TRUE);	

	if(!Dlg2Data()) return;

	if(m_Data.strName.IsEmpty()) 
	{
		AfxMessageBox(_LS(IDS_DGN_GEN_MEMB_PLATE_NO_NAME_ERROR));
		return;
	}

	if(m_nOption == 0) // Add/Replace
	{
		if(m_pDoc->m_pAttrCtrl2->ExistRrpl(m_Data.strName))
		{
			m_pDoc->m_pDataCtrl->ModifyRrpl(m_Data.strName, m_Data);
		}
		else
		{
			m_pDoc->m_pDataCtrl->AddRrpl(m_Data);
		}
	}
	else // Delete
	{
		m_pDoc->m_pDataCtrl->DelRrpl(m_Data.strName);
	}

	MakeItemEx();

	if(m_pDoc->m_pViewCtrl->SelectedNodeCount()) m_pDoc->m_pViewCtrl->UnselectAll(NULL);	
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

//void CDgnRCRatingPlateDlg::OnDgnClose() 
//{
//	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
//}