#include "stdafx.h"
#include "wg_dgn.h"
#include "SteelKRDesignParamDlg.h"

#include "DgnDataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_MembCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\DesignResult.h"
#include "..\wg_dbLock\LockCtrl.h"

#include "..\wg_base\wg_base_Product.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\CheckDialogOpen.h"

#include "DgnConMatDlg.h"
#include "..\wg_base\wg_base_TreeMenuMode.h"
#include "..\wg_base\TreeMenuBarBase.h"

#include "..\wg_acs\Acs_ServiceDef.h"
#include "..\wg_base\ServiceProvider.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSteelKRDesignParamDlg dialog


CSteelKRDesignParamDlg::CSteelKRDesignParamDlg(CWnd* pParent /*=NULL*/)
	: CDgnDlgBase(CSteelKRDesignParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSteelKRDesignParamDlg)
	m_SelectElemString = _T("");		
	//}}AFX_DATA_INIT
	
	m_pDoc = CDBDoc::GetDocPoint();
	m_arStreLcomKey.InitHashTable(101);	
}

CSteelKRDesignParamDlg::~CSteelKRDesignParamDlg()
{	
	m_arSelElemK.RemoveAll();
	m_arLcomKey.RemoveAll();
	m_arStreLcomKey.RemoveAll();
}


void CSteelKRDesignParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSteelKRDesignParamDlg)  
//IDC_RAO_NUMBER
	DDX_Control(pDX, IDC_DGN_CODE_STA, m_DesignCodeStatic);  
	DDX_Control(pDX, IDC_DGN_STL_LIST_LCOM_STRENGTH, m_ltbxLcomStre);
	DDX_Text(pDX, IDC_DGN_ELEMENT_EDIT, m_SelectElemString);
	DDX_Control(pDX, IDC_DGN_ELEMENT_EDIT, m_editSelectElem);
	DDX_Control(pDX, IDC_DGN_ETC_EDIT, m_editEtc);
	DDX_Control(pDX, IDC_DGN_CHECK_METHOD1_RADIO, m_radioCheckMethod1);
	DDX_Control(pDX, IDC_DGN_CHECK_METHOD2_RADIO, m_radioCheckMethod2);
	DDX_Control(pDX, IDC_DGN_CHECK_METHOD3_RADIO, m_radioCheckMethod3);
	DDX_Control(pDX, IDC_DGN_BIAXIAL_CHECK, m_chkBiaxial);
	DDX_Control(pDX, IDC_DGN_Y_RADIO, m_radioY);
	DDX_Control(pDX, IDC_DGN_Z_RADIO, m_radioZ);
	DDX_Control(pDX, IDC_DGN_YZ_RADIO, m_radioYZ);
	DDX_Control(pDX, IDC_DGN_I_CHECK, m_chkIend);
	DDX_Control(pDX, IDC_DGN_J_CHECK, m_chkJend);
	DDX_Control(pDX, IDC_DGN_LIST, m_List);
	//}}AFX_DATA_MAP
}

#define CDialog CDgnDlgBase
BEGIN_MESSAGE_MAP(CSteelKRDesignParamDlg, CDgnDlgBase)
#undef CDialog
	//{{AFX_MSG_MAP(CSteelKRDesignParamDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_LIST, OnOnItemchangedList)	
	ON_BN_CLICKED(IDC_DGN_BTN_ADD, OnAddBtnClick)
	ON_BN_CLICKED(IDC_DGN_BTN_MODIFY, OnModifyBtnClick)
	ON_BN_CLICKED(IDC_DGN_BTN_DELETE, OnDeleteBtnClick)
	ON_BN_CLICKED(IDC_DGN_CHECK_METHOD1_RADIO, OnCheckMethod)
	ON_BN_CLICKED(IDC_DGN_CHECK_METHOD2_RADIO, OnCheckMethod)
	ON_BN_CLICKED(IDC_DGN_CHECK_METHOD3_RADIO, OnCheckMethod)
	ON_BN_CLICKED(IDC_DGN_UNBRACED_LENGTH_BTN, OnUnbracedLengthClick)
	ON_BN_CLICKED(IDC_DGN_EFFECTIVE_LENGTH_BTN, OnEffectLengthClick)
	ON_BN_CLICKED(IDC_DGN_EFFECT_WIDTH_BTN, OnEffectWidthClick)
	ON_BN_CLICKED(IDC_DGN_APP_BTN, OnApplyClick)
	ON_BN_CLICKED(IDC_DGN_CLOSE_BTN, OnCloseClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSteelKRDesignParamDlg message handlers

BOOL CSteelKRDesignParamDlg::OnInitDialog() 
{
	CDgnDlgBase::OnInitDialog();
	
	CCheckDialogOpen::SetSoftwareRendering();

	m_editSelectElem.Connect(SC_ID_ELEM, &m_editSelectElem);

	CString strCode = CDBLib::GetStlCodeName();
	CString strDesignCode;
	strDesignCode.Format( _T("%s%s"), _LS(IDS_DGN_DESIGN_CODE_), strCode );
	m_DesignCodeStatic.SetWindowText(strDesignCode);

	SetListCtrlHeader();

	InitialLcomDataForDgn();
	T_STDP_D data;
	data.Initialize();
	Data2Dlg_StdpCotrol(data);

	Initial_ListData();

	//해석되어 있지 않으면 계산버튼 비활성화
	if(!m_pDoc->IsPostMode())
		GetDlgItem(IDC_DGN_APP_BTN)->EnableWindow(false);
	  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSteelKRDesignParamDlg::Data2Dlg_StdpCotrol(const T_STDP_D& data) 
{
	m_editEtc.SetWindowText(data.strRemark);//비고

	if(data.iChkMethod == 0)//단면검토
	{
		m_radioCheckMethod1.SetCheck(TRUE);
		m_radioCheckMethod2.SetCheck(FALSE);
		m_radioCheckMethod3.SetCheck(FALSE);
	}
	else if(data.iChkMethod == 1)
	{
		m_radioCheckMethod1.SetCheck(FALSE);
		m_radioCheckMethod2.SetCheck(TRUE);
		m_radioCheckMethod3.SetCheck(FALSE);
	}
	else if(data.iChkMethod == 2)
	{
		m_radioCheckMethod1.SetCheck(FALSE);
		m_radioCheckMethod2.SetCheck(FALSE);
		m_radioCheckMethod3.SetCheck(TRUE);
	}

	if(data.iChkAxial == 0)//검토 주축 선택
	{
		m_radioY.SetCheck(TRUE);
		m_radioZ.SetCheck(FALSE);
		m_radioYZ.SetCheck(FALSE);
	}
	else if(data.iChkAxial == 1)
	{
		m_radioY.SetCheck(FALSE);
		m_radioZ.SetCheck(TRUE);
		m_radioYZ.SetCheck(FALSE);
	}
	else if(data.iChkAxial == 2)
	{
		m_radioY.SetCheck(FALSE);
		m_radioZ.SetCheck(FALSE);
		m_radioYZ.SetCheck(TRUE);
	}

	m_chkIend.SetCheck(data.bPosi[0]);//I-End
	m_chkJend.SetCheck(data.bPosi[1]);//J-End

	OnCheckMethod();
}

T_STDP_D CSteelKRDesignParamDlg::Dlg2Data_StdpCotrol() 
{
	T_STDP_D data;
	data.Initialize();

	m_editEtc.GetWindowText(data.strRemark);//비고

	if(m_radioCheckMethod1.GetCheck())//단면검토
		data.iChkMethod = 0;
	else if(m_radioCheckMethod2.GetCheck())
		data.iChkMethod = 1;
	else if(m_radioCheckMethod3.GetCheck())
		data.iChkMethod = 2;
	else
		data.iChkMethod = 0;

	if(m_radioY.GetCheck())//검토 주축 선택
		data.iChkAxial = 0;
	else if(m_radioZ.GetCheck())
		data.iChkAxial = 1;
	else if(m_radioYZ.GetCheck())
		data.iChkAxial = 2;
	else
		data.iChkAxial = 0;

	data.bPosi[0] = m_chkIend.GetCheck();//I-End
	data.bPosi[1] = m_chkJend.GetCheck();//J-End

	return data;
}

void CSteelKRDesignParamDlg::InitialLcomDataForDgn()
{ 
	// Save Total Load Combination Name by Serial Number(No LcomKey).
	T_STLC_D StlcData;
	
	if(m_pDoc->m_pAttrCtrl->ExistStlc())//DB에 데이터가 있다면 DB에서 데이터를 가져와서 세팅
	{
		m_pDoc->m_pAttrCtrl->GetStlc(StlcData);
		int i;
		m_arStreLcomKey.RemoveAll();
		int nStlcDataSize = StlcData.StreLcom.GetSize();
		for(i=0 ; i<nStlcDataSize ; i++)
		{
			m_arStreLcomKey.SetAt(StlcData.StreLcom[i], StlcData.StreLcom[i]);
		}		
	}

	int iLcomSerNum=0;
	int i=0, j=0;
	int iLcomCount=0;
	
	int nID;
	int nIndex;  

	T_LCOM_D LcomData;
	T_LCOM_K ExistKey;

	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_STEEL, m_arLcomKey);

	int nLcomKeySize = m_arLcomKey.GetSize();
	for(i=0; i<nLcomKeySize; i++)
	{		
		LcomData.Initialize();
		m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, m_arLcomKey[i], LcomData);
		// Modify by GAY. MNET:2903. ('07.08.24). Add Type의 하중조합만 Combo Box에 Setting.
		if(LcomData.LoadCombType!=0) continue;
		if(LcomData.nActive==0)      continue;

		if(LcomData.nActive==1)//Strenge
		{
			CString strLcomName = LcomData.LoadCombName;    
			nID = i; //i+1;    
			bool bExist = false; //해당 LoadCase가 DB에 존재하면 Check True

			nIndex = m_ltbxLcomStre.AddString(strLcomName);
			m_ltbxLcomStre.SetItemData(nIndex, DWORD(nID));
			
			if(m_pDoc->m_pAttrCtrl->ExistStlc())//DB에 저장된것이 있다면 저장된 값으로 체크
			{
				if(m_arStreLcomKey.Lookup(m_arLcomKey[i], ExistKey))
					bExist = true;
				else
					bExist = false;
			}
			else//DB저장된 데이터가 없다면
			{
				bExist = true;
			}
			m_ltbxLcomStre.SetCheck(nIndex, bExist);
		}		
	}  
}

//LoadCase 체크유무 정보를 저장한다
void CSteelKRDesignParamDlg::Dlg2Data_LoadCase() 
{
	CString sName;
	DWORD nID;

	m_arStreLcomKey.RemoveAll();
	
	int nCount = m_ltbxLcomStre.GetCount();

	T_LCOM_K LcomK;
	for (int k = 0; k < nCount ; k++)
	{
		if(m_ltbxLcomStre.GetCheck(k) == 1)
		{
			nID = m_ltbxLcomStre.GetItemData(k);
			LcomK = m_arLcomKey.GetAt(int(nID));
			m_arStreLcomKey.SetAt(LcomK,LcomK);
		}
	}	
}

void CSteelKRDesignParamDlg::SetSelectElemString() 
{
	m_SelectElemString.Empty();

	//List에서 선택된 m_SelectElemString을 가져오는 소스
	CArray<int, int> arSelItem;
	arSelItem.RemoveAll();
	GetSelectedItemList(&m_List, arSelItem);

	CString TempString;
	int arSelItemSize = arSelItem.GetSize();
	for(int i=0 ; i<arSelItemSize ; i++)
	{
		if(m_SelectElemString.GetLength()==0)
			m_SelectElemString.Format(_T("%d"), arSelItem[i]);
		else
		{
			TempString.Format(_T(" %d"), arSelItem[i]);
			m_SelectElemString += TempString;
		}
	}

	UpdateData(FALSE);
	m_editSelectElem.SelectByStr(m_SelectElemString);	
}

BOOL CSteelKRDesignParamDlg::DestroyWindow() 
{
	CCheckDialogOpen::ResetSoftwareRendering();

	return CDgnDlgBase::DestroyWindow();
}


void CSteelKRDesignParamDlg::Initial_ListData()
{
	m_List.DeleteAllItems();

	CArray<T_STDP_K, T_STDP_K> arStdpKey;
	m_pDoc->m_pAttrCtrl->GetStdpKeyList(arStdpKey);

	int stdpKeySize = arStdpKey.GetSize();
	for(int i=0 ; i<stdpKeySize ; i++)
	{
		InsertListItem(arStdpKey[i]);
	}
}

void CSteelKRDesignParamDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_DGN_STEEL_ELEM), _LS(IDS_DGN_STEEL_SECT_CHECK), _LS(IDS_DGN_STEEL_MAIN_CHECK), _LS(IDS_DGN_STEEL_CHECK_POSITION), _LS(IDS_DGN_STEEL_NOTE)};  
	// _T("Elem"), _T("단면검토"), _T("검토주축"), _T("검토위치"), _T("비고")
	int nColWidth[] = {55, 90, 70, 70, 50};
	int nAlign[] = {LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT};
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

int CSteelKRDesignParamDlg::FindInsertionPos(T_STDP_K nID)
{
	int nCount = m_List.GetItemCount();
	for (int i=0; i < nCount; i++)
	{
        T_STDP_K key = m_List.GetItemData(i);
		if (nID < key) { return i;}
	}
	return nCount;
}

BOOL CSteelKRDesignParamDlg::InsertListItem(T_STDP_K Key)
{
	LVITEM lvitem;
	CString str = _T("");
	int nItem = m_List.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i=0; i < 5; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr(i, Key);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if (i == 0) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_List.InsertItem(&lvitem);
		}
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	m_List.SetItemData(nItem, (DWORD)Key);
	return TRUE;
}

CString CSteelKRDesignParamDlg::DataToStr(int i, T_STDP_K Key)
{  
	CString str = _T("-");

	T_STDP_D StdpData;
	//DB에서 데이타를 가져옴
	m_pDoc->m_pAttrCtrl->GetStdp(Key, StdpData);

	if(i == 0) 
		str.Format(_T("%d"), Key);
	else if(i == 1)
	{
		if(StdpData.iChkMethod == 0)
			str = _LS(IDS_DGN_STEEL_CURVE_CHECK);
		else if(StdpData.iChkMethod == 1)
			str = _LS(IDS_DGN_STEEL_LANG_CHECK);
		else if(StdpData.iChkMethod == 2)
			str = _LS(IDS_DGN_STEEL_CURVE_LANG_CHECK);
		else
			ASSERT(0);
	}
	else if(i == 2)
	{
		if(StdpData.iChkMethod == 1)
			str = _T("-");
		else
		{
			if(StdpData.iChkAxial == 0)
				str = _T("y");
			else if(StdpData.iChkAxial == 1)
				str = _T("z");
			else if(StdpData.iChkAxial == 2)
				str = _T("y&z");
			else
				ASSERT(0);
		}
	}
	else if(i == 3)
	{
		if(StdpData.bPosi[0])
			str = _T("I");
		if(StdpData.bPosi[1])
		{
			if(str == _T("-"))
				str = _T("J");
			else
				str += _T(", J");
		}
	}
	else if(i == 4)
	{
		str = StdpData.strRemark;
	}
	else ASSERT(0);

	return str;
}

BOOL CSteelKRDesignParamDlg::DeleteListItem(T_STDP_K key)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)key;

	int nItem = m_List.FindItem(&FindInfo);
	if(nItem==-1) return TRUE;

	m_List.DeleteItem(nItem);

	return TRUE;
}

BOOL CSteelKRDesignParamDlg::ModifyListItem(T_STDP_K key)
{
	LVFINDINFO FindInfo;
	FindInfo.flags  = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)key;

	int nItem = m_List.FindItem(&FindInfo);
	if(nItem == -1) return TRUE;

	for(int i=0; i<5; i++)
	{
		CString str = DataToStr(i, key);
		m_List.SetItemText(nItem, i, str);
	}

	LVITEM lvitem;
	lvitem.iItem    = nItem;
	lvitem.iSubItem = 0;
	lvitem.mask     = LVIF_PARAM;
	lvitem.lParam   = (LPARAM)key;
	m_List.SetItem(&lvitem);

	return TRUE;
}


void CSteelKRDesignParamDlg::OnOnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIndex = pNMListView->iItem; 
	ChangeItem(nIndex);
	*pResult = 0;
}

void CSteelKRDesignParamDlg::ChangeItem(int nIndex)
{ 
	if (nIndex != -1)
	{
		T_STDP_K key;
		T_STDP_D data;
	  key = m_List.GetItemData(nIndex);

	  //DB에서 데이터 가져옴
		m_pDoc->m_pAttrCtrl->GetStdp(key, data);

		Data2Dlg_StdpCotrol(data);

		CArray<int, int> arSelItem;
		CArray<int, int> arSelElem;
		arSelItem.RemoveAll();
		arSelElem.RemoveAll();
		GetSelectedItemList(&m_List, arSelItem);

		int selItemSize = arSelItem.GetSize();
		for(int i=0 ; i<selItemSize ; i++)
		{
			arSelElem.Add(m_List.GetItemData(arSelItem[i]));
		}

		CString	strSelectElem;
		strSelectElem.Empty();
		CString TempString;
		int selElemSize = arSelElem.GetSize();
		for(int i=0 ; i<selElemSize ; i++)
		{
			if(strSelectElem.GetLength()==0)
				strSelectElem.Format(_T("%d"), arSelElem[i]);
			else
			{
				TempString.Format(_T(" %d"), arSelElem[i]);
				strSelectElem += TempString;
			}
		}

		m_SelectElemString = strSelectElem;
		UpdateData(FALSE);

		if(strSelectElem.GetLength() > 0)
			m_editSelectElem.SelectByStr(strSelectElem);
	}
}

void CSteelKRDesignParamDlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
{
	int  nItem = -1;
	UINT uSelectedCount = pList->GetSelectedCount();
	aSelItem.SetSize(uSelectedCount);
	for(UINT i = 0; i < uSelectedCount; i++)
	{
		nItem = pList->GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		aSelItem[i] = nItem;
	}
}

BOOL CSteelKRDesignParamDlg::DataCheck(const T_STDP_D& data) 
{
	CString ErrorMessanges = _T("");

	if(!data.bPosi[0] && !data.bPosi[1])
		ErrorMessanges = _LS(IDS_DGN_MUST_ONE_OVER_PART_NUMBER);//_T("Part Number를 한가지 이상 선택하셔야 합니다.")
	
	if(!ErrorMessanges.GetLength()==0)
	{
		AfxMessageBox(ErrorMessanges, MB_OK);
		return FALSE;
	}

	return TRUE;
}


void CSteelKRDesignParamDlg::OnAddBtnClick()
{
	T_STDP_D data = Dlg2Data_StdpCotrol();

	if(!DataCheck(data))
		return;	

	CArray<T_ELEM_K, T_ELEM_K> arElemK;
	CArray<T_ELEM_K, T_ELEM_K> BadMemberKeyList;
	CArray<T_STDP_K, T_STDP_K> GoodMemberKeyList;

	m_pDoc->m_pViewCtrl->GetAllSelectedElem(arElemK);

	CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();

	T_ELEM_K    Key;
	int arElemKSize = arElemK.GetSize();
	for(int i=0; i<arElemKSize; i++)
	{
		Key = arElemK[i];

		BOOL bReverseDir;
		T_ELEM_K ElemShowKey;
		CArray<T_ELEM_K, T_ELEM_K> aElemList;
		aElemList.RemoveAll();
		pMembCtrl->GetElemListByIncludeElem(Key, bReverseDir, ElemShowKey, aElemList);

		if(aElemList.GetSize() > 0)
		{
			if(Key != ElemShowKey)
				BadMemberKeyList.Add(Key);
			else
				GoodMemberKeyList.Add(Key);
		}
	}

	m_pDoc->m_pDataCtrl->AddStdp(GoodMemberKeyList, data);

	if(BadMemberKeyList.GetSize()>0)
	{
		CString Message = _T("");
		CString strElem = _T("");
		int BadMemberKeyListSize = BadMemberKeyList.GetSize();
		for(int i=0 ; i<BadMemberKeyListSize ; i++)
		{
			strElem.Format(_T("%d"), BadMemberKeyList[i]);
			if(Message == _T(""))
				Message = strElem;
			else
				Message = Message + _T(", ") + strElem;
		}
		Message = Message + _LS(IDS_DGN_NOT_INPUT_DATA_NOT_MEMBER);//_T("번\n요소는 Member의 대표요소가 아니므로 입력할 수 없습니다.")
		AfxMessageBox(Message, MB_OK);
	}	
}

void CSteelKRDesignParamDlg::OnModifyBtnClick()
{
	T_STDP_D data = Dlg2Data_StdpCotrol();

	if(!DataCheck(data))
		return;	

	CArray<T_ELEM_K, T_ELEM_K> arElemK;
	CArray<T_STDP_K, T_STDP_K> ExistKeyList;
	CArray<T_STDP_D, T_STDP_D&> rData;
	
	m_pDoc->m_pViewCtrl->GetAllSelectedElem(arElemK);

	T_ELEM_K    Key;
	int arElemKSize = arElemK.GetSize();
	for(int i=0; i<arElemKSize; i++)
	{
		Key = arElemK[i];

		if(m_pDoc->m_pAttrCtrl->ExistStdp(Key))
		{
			ExistKeyList.Add(Key);
			rData.Add(data);
		}
	}

	if(m_pDoc->m_pDataCtrl->ModifyStdp(ExistKeyList, ExistKeyList, rData))
	{
		// Add Jaeoh.(07.12.24)
		BOOL bExistResFile = m_pDoc->m_pPostCtrl->GetDesignResult()->IsSteelDesignResultEnable();
		if(bExistResFile)
		{      
			T_DSTL_D DstlD;
			DstlD.Initialize();
			m_pDoc->m_pAttrCtrl->GetDstl(DstlD);
			int iSelDgnLcom = DstlD.iSelDgnLcom;
			for(int i=0; i<ExistKeyList.GetSize(); i++)
			{
				T_ELEM_K ElemK = ExistKeyList[i];
				//
				int iLcomK=0; // Max는 하중조합 0
				BOOL bMaxMoving=FALSE;
				int iCompType=0;
				T_STDR_D StdrD;
				StdrD.Initialize();
				if(iSelDgnLcom==0)
				{
					// Read
					m_pDoc->m_pPostCtrl->GetDesignResult()->ReadSteelDesignConcurrentResultForKsceAsd05(ElemK, iLcomK, StdrD);
					StdrD.StdrBase[0].bCheck = StdrD.StdrBase[1].bCheck = FALSE; // bCheck만 false로 만든다.
					//Write
					m_pDoc->m_pPostCtrl->GetDesignResult()->WriteSteelDesignConcurrentResultForKsceAsd05(ElemK, iLcomK, StdrD);
				}
				else if(iSelDgnLcom==1)
				{
					m_pDoc->m_pPostCtrl->GetDesignResult()->ReadSteelDesignMaxMinResultForKsceAsd05(ElemK, iLcomK, StdrD);
					StdrD.StdrBase[0].bCheck = StdrD.StdrBase[1].bCheck = FALSE; // bCheck만 false로 만든다.
					m_pDoc->m_pPostCtrl->GetDesignResult()->WriteSteelDesignMaxMinResultForKsceAsd05(ElemK, iLcomK, StdrD);          
				}
				else if(iSelDgnLcom==2)
				{
					m_pDoc->m_pPostCtrl->GetDesignResult()->ReadSteelDesignResultForKsceAsd05(ElemK, iLcomK, bMaxMoving, iCompType, StdrD);
					StdrD.StdrBase[0].bCheck = StdrD.StdrBase[1].bCheck = FALSE; // bCheck만 false로 만든다.
					m_pDoc->m_pPostCtrl->GetDesignResult()->WriteSteelDesignResultForKsceAsd05(ElemK, iLcomK, bMaxMoving, iCompType, StdrD);          
				}
				else ASSERT(0);
			}  
		}
	}  
}

void CSteelKRDesignParamDlg::OnDeleteBtnClick()
{
	T_STDP_D data = Dlg2Data_StdpCotrol();

	if(!DataCheck(data))
		return;	

	CArray<T_ELEM_K, T_ELEM_K> arElemK;
	CArray<T_STDP_K, T_STDP_K> ExistKeyList;
	
	m_pDoc->m_pViewCtrl->GetAllSelectedElem(arElemK);

	T_ELEM_K    Key;
	int arElemKSize = arElemK.GetSize();
	for(int i=0; i<arElemKSize; i++)
	{
		Key = arElemK[i];

		if(m_pDoc->m_pAttrCtrl->ExistStdp(Key))
			ExistKeyList.Add(Key);
	}

	m_pDoc->m_pDataCtrl->DelStdp(ExistKeyList);	
}

void CSteelKRDesignParamDlg::OnCheckMethod()
{
	if(m_radioCheckMethod1.GetCheck())
	{
		//m_chkBiaxial.EnableWindow(TRUE);
		m_radioY.EnableWindow(TRUE);
		m_radioZ.EnableWindow(TRUE);
		m_radioYZ.EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_GROUP5)->EnableWindow(TRUE);
	}
	else if(m_radioCheckMethod2.GetCheck())
	{
		//m_chkBiaxial.EnableWindow(FALSE);
		m_radioY.EnableWindow(FALSE);
		m_radioZ.EnableWindow(FALSE);
		m_radioYZ.EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_GROUP5)->EnableWindow(FALSE);
	}
	else if(m_radioCheckMethod3.GetCheck())
	{
		//m_chkBiaxial.EnableWindow(FALSE);
		m_radioY.EnableWindow(TRUE);
		m_radioZ.EnableWindow(TRUE);
		m_radioYZ.EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_GROUP5)->EnableWindow(TRUE);
	}
	else 
		ASSERT(0);
}

void CSteelKRDesignParamDlg::OnUnbracedLengthClick()
{
	CTreeMenuBarBase::SetModeST(__TMMODE_DGN_GEN_LENG__);
}

void CSteelKRDesignParamDlg::OnEffectLengthClick()
{
	CTreeMenuBarBase::SetModeST(__TMMODE_DGN_GEN_KFAC__);
}

void CSteelKRDesignParamDlg::OnEffectWidthClick()
{
	CTreeMenuBarBase::SetModeST(__TMMODE_DGN_GEN_EWSD__);
}

void CSteelKRDesignParamDlg::OnApplyClick()
{
	// Add, Jaeoh. [11/4/2008]
	if(!m_pDoc->AllowCommand(D_OPTN_ID_MODS_SERVICE)) 
	{
		AfxMessageBox(_LS(IDS_DGN_MODS_Expriation_Message), MB_OK);
		return;    
	}

	CString sName;
	DWORD nID;

	m_arStreLcomKey.RemoveAll();

	int nCount = m_ltbxLcomStre.GetCount();
		
	for (int k = 0; k < nCount ; k++)
	{
		if(m_ltbxLcomStre.GetCheck(k) == 1)
		{
			nID = m_ltbxLcomStre.GetItemData(k);
			T_LCOM_K LcomK = m_arLcomKey.GetAt(int(nID));
			m_arStreLcomKey.SetAt(LcomK,LcomK);
		}
	}

	if(m_arStreLcomKey.IsEmpty())
	{
		AfxMessageBox(_LS(IDS_DGN_LOADCASE_SELETE), MB_OK);
		return;
	}
	
	// Change, Jaeoh(07.12.24)
	T_STLC_D StlcDB;
	StlcDB.Initialize();
	m_pDoc->m_pAttrCtrl->GetStlc(StlcDB);
	int iSaveLcomNum = StlcDB.StreLcom.GetSize();

	T_STLC_D StlcD;
	StlcD.Initialize();
	T_LCOM_K LcomK, GetLcomK;
	POSITION Pos = m_arStreLcomKey.GetStartPosition();
	while(Pos)
	{
		m_arStreLcomKey.GetNextAssoc(Pos,LcomK,GetLcomK);
		StlcD.StreLcom.Add(GetLcomK);
	}  
	int iCurrLcomNum = StlcD.StreLcom.GetSize();
	
	if(iSaveLcomNum==0)
	{
		m_pDoc->m_pDataCtrl->AddStlc(StlcD); 
	}
	else if(iSaveLcomNum == iCurrLcomNum)
	{
		qsort((void*)StlcDB.StreLcom.GetData(), iSaveLcomNum, sizeof(UINT), CCompFunc::UINTAsc);
		qsort((void*)StlcD.StreLcom.GetData(), iCurrLcomNum, sizeof(UINT), CCompFunc::UINTAsc);
		for(int i=0; i<iCurrLcomNum; i++)
		{
			if(StlcDB.StreLcom[i] != StlcD.StreLcom[i])
			{
				m_pDoc->m_pDataCtrl->AddStlc(StlcD); 
				break;
			}
		}    
	}
	else
	{
		if(!m_pDoc->m_pDataCtrl->AddStlc(StlcD)) 
			return;
	}

	// Add, Jaeoh(07.08.27)
	T_DSTL_D DstlD;
	DstlD.Initialize();
	m_pDoc->m_pDgnDataCtrl->Get_DgnStlDstl(DstlD);

	if(DstlD.DesignCode != _T("KSCE-ASD05"))
	{
		CString strMessage;
		strMessage.Format(_T("'%s' 는 설계할 수 없습니다."),DstlD.DesignCode);
		AfxMessageBox(strMessage,MB_OK); //_LS()
		return;
	}

	// 사용자가 기준을 선택 하지 않았을 경우. 
	//int iStlDgnExecute = 2; // KSCE-ASD05 기준 
	//CDBLib::SetStlDgnExecute(iStlDgnExecute);

	CServiceHeadOffice* pHeadOffice = CServiceHeadOffice::Instance();
	pHeadOffice->ReqService(_ULS(acs), EN_ACSSA_CIVIL_STL_CALC_DLG, NULL);

	// 설계 끝난후 임의값..
	//iStlDgnExecute = 999; // 임의값. 
	//CDBLib::SetStlDgnExecute(iStlDgnExecute);
}

void CSteelKRDesignParamDlg::OnCloseClick()
{
	DestroyWindow();
}

void CSteelKRDesignParamDlg::ChangeLoadCombination()
{
	//리스트 삭제
	m_ltbxLcomStre.ResetContent();
	InitialLcomDataForDgn();
}


void CSteelKRDesignParamDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch(lHint)
	{  
	case D_UPDATE_SEL_PRE:
		EnableCalcBtn(false);
		break;
	case D_UPDATE_SEL_POST:
		EnableCalcBtn(true);
		break;
	case D_UPDATE_DGN_FORCE_CHANGED: //Load Conbination이 변경되었을 경우 해당 입력창을 닫아줌.
		{
			//DestroyWindow();
			break;
		}
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_PREF_CHANGED:
		{
			CString strCodeName = CDBLib::GetStlCodeName();
			if(strCodeName!=_T("KSCE-ASD05"))
				DestroyWindow();    
			break;
		}

	default:
		//ASSERT(FALSE);
		break;
	}
}

void CSteelKRDesignParamDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bSpfcChanged = FALSE;
	BOOL bStorChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		switch(nCmd)
		{
		case(UR_MEMB_ADD)://MEMB변경시 창을 닫는다.
		case(UR_MEMB_DEL): 
		case(UR_MEMB_MFD): 
		case(UR_MEMB_MFS):
		case(UR_GRUP_ADD)://GRUP변경시 창을 닫는다.
		case(UR_GRUP_DEL): 
		case(UR_GRUP_MFD): 
		case(UR_GRUP_MFS):		
		case(UR_MBTP_ADD)://Modify MemberType
		case(UR_MBTP_DEL):
		case(UR_DSTL_DEL):
			{
				DestroyWindow();
				return;
			}
		case(UR_STDP_ADD):
		case(UR_STDP_DEL):
			Initial_ListData();
			break;
		case(UR_LCST_ADD)://하중조합 변경시 반영
		case(UR_LCST_DEL): 
		case(UR_LCST_MFD): 
		case(UR_LCST_MFS):
			ChangeLoadCombination();
			break;
		default:
		  break;
		}
	}
}

void CSteelKRDesignParamDlg::EnableCalcBtn(bool Enable) 
{
	GetDlgItem(IDC_DGN_APP_BTN)->EnableWindow(Enable);
}
