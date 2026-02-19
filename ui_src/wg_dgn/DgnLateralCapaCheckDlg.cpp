// DgnLateralCapaCheckDlg.cpp: implementation of the CDgnLateralCapaCheckDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnLateralCapaCheckDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "DgnToolMPhi_SeismicDesign.h"
#include "DgnLateralCapaResultDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnLateralCapaCheckDlg::CDgnLateralCapaCheckDlg(CWnd* pParent /*=NULL*/)
	: CDgnDlgBase(CDgnLateralCapaCheckDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnLateralCapaCheckDlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	
	m_SelectElemString = _T("");	
	m_nCurSelectedKey = 0;
	m_bDataUpDate = TRUE;
	T_RLCO_D RlcoD;
	if(!m_pDoc->m_pAttrCtrl->GetRlco(RlcoD)) m_pDoc->m_pAttrCtrl->GetDefaultRlco(RlcoD);
	m_nInitialForce = RlcoD.nInitialForce;

}


void CDgnLateralCapaCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenKFacDlg)	
	DDX_Text(pDX,    IDC_RLCC_ELEMLIST_EDT, m_SelectElemString);
	DDX_Control(pDX, IDC_RLCC_ELEMLIST_EDT, m_editSelectElem);
	DDX_Control(pDX, IDC_RLCC_TYPE_COMB,    m_TypeComb);
	DDX_Control(pDX, IDC_RLCC_NAME_EDT,     m_NameEdit);
	DDX_Control(pDX, IDC_RLCC_MY_CHK,       m_MyChkBox);
	DDX_Control(pDX, IDC_RLCC_MZ_CHK,       m_MzChkBox);
	DDX_Control(pDX, IDC_RLCC_WU_EDT,       m_WuEdit);
	DDX_Control(pDX, IDC_RLCC_WU_UNIT,      m_WuUnit);
	DDX_Control(pDX, IDC_RLCC_WP_EDT,       m_WpEdit);
	DDX_Control(pDX, IDC_RLCC_WP_UNIT,      m_WpUnit);
	DDX_Control(pDX, IDC_RLCC_TY_EDT,       m_TyEdit);
	DDX_Control(pDX, IDC_RLCC_TZ_EDT,       m_TzEdit);
	DDX_Control(pDX, IDC_RLCC_T_UNIT,       m_TUnit);
	DDX_Control(pDX, IDC_RLCC_H_EDT,        m_HEdit);
	DDX_Control(pDX, IDC_RLCC_H_UNIT,       m_HUnit);
	DDX_Control(pDX, IDC_RLCC_M0Y_EDT,      m_M0yEdit);
	DDX_Control(pDX, IDC_RLCC_M0Z_EDT,      m_M0zEdit);
	DDX_Control(pDX, IDC_RLCC_M0_UNIT,      m_M0Unit);
	DDX_Control(pDX, IDC_RLCC_P0_EDT,       m_P0Edit);
	DDX_Control(pDX, IDC_RLCC_P0_UNIT,      m_P0Unit);	
	DDX_Control(pDX, IDC_DGN_RLCC_LIST,     m_RlccList);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnLateralCapaCheckDlg, CDgnDlgBase)
	//{{AFX_MSG_MAP(CDgnLateralCapaCheckDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE,       OnDgnClose)
	ON_BN_CLICKED(IDC_RLCC_MY_CHK,     OnMyMzChk)
	ON_BN_CLICKED(IDC_RLCC_MZ_CHK,     OnMyMzChk)
	ON_BN_CLICKED(IDC_DGN_ADD_BTN,     OnAddBtn)
	ON_BN_CLICKED(IDC_DGN_MODIFY_BTN,  OnModifyBtn)
	ON_BN_CLICKED(IDC_DGN_DEL_BTN,     OnDelBtn)
	ON_BN_CLICKED(IDC_RLCC_CALC_BTN,   OnDgnExecute)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_RLCC_LIST, OnItemchangedRlccList)
	ON_NOTIFY(NM_CLICK, IDC_DGN_RLCC_LIST, OnClickRlccList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CDgnLateralCapaCheckDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CDgnLateralCapaCheckDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();	

	BOOL bRlcc = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_RLCO_ADD):// 보유수령내력 Option정보 변경
		//case(UR_RLCO_DEL): // 변경시 제거후 추가 되므로 2중호출을 피하기 위해서
			{
				DestroyWindow();
				break;
			}		
		case(UR_RLCC_ADD):
		case(UR_RLCC_DEL):
				bRlcc = TRUE;
				break;
		default:
			break;
		}
	} // end of while

	if(bRlcc) UpdateRlccList();
}

BOOL CDgnLateralCapaCheckDlg::OnInitDialog() 
{
	CDgnDlgBase::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;
	
	InitCtrl(); 
	Initial_Data();
	m_editSelectElem.Connect(SC_ID_ELEM, &m_editSelectElem); 

	UpdateRlccList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnLateralCapaCheckDlg::OnMyMzChk() 
{	
	BOOL bMyChk = m_MyChkBox.GetCheck();
	BOOL bMzChk = m_MzChkBox.GetCheck();

	GetDlgItem(IDC_RLCC_TY_STC)->EnableWindow(bMyChk);
	GetDlgItem(IDC_RLCC_TY_EDT)->EnableWindow(bMyChk);	
	GetDlgItem(IDC_RLCC_TZ_STC)->EnableWindow(bMzChk);
	GetDlgItem(IDC_RLCC_TZ_EDT)->EnableWindow(bMzChk);
	
	if(m_nInitialForce == 2)
	{	
		GetDlgItem(IDC_RLCC_M0Y_STC)->EnableWindow(bMyChk);
		GetDlgItem(IDC_RLCC_M0Y_EDT)->EnableWindow(bMyChk);	
		GetDlgItem(IDC_RLCC_M0Z_STC)->EnableWindow(bMzChk);
		GetDlgItem(IDC_RLCC_M0Z_EDT)->EnableWindow(bMzChk);
	}
}

void CDgnLateralCapaCheckDlg::OnAddBtn() 
{	
	if(!Dlg2Data()) return;

	T_RLCC_K RlccK = m_pDoc->m_pAttrCtrl->GetStartNumRlcc();
	m_bDataUpDate = FALSE;
	if(!m_pDoc->m_pDataCtrl->AddRlcc(RlccK, m_Data)) return;	
	UpdateRlccList();
	m_bDataUpDate = TRUE;
	m_nCurSelectedKey = RlccK;
	Data2Dlg();
	return;
}


void CDgnLateralCapaCheckDlg::OnModifyBtn() 
{	
	if(m_nCurSelectedKey <= 0) { OnAddBtn();  return; }

	if(!Dlg2Data()) return;

	T_RLCC_K TempK = (UINT)(m_nCurSelectedKey);	
	m_bDataUpDate = FALSE;
	if(!m_pDoc->m_pDataCtrl->ModifyRlcc(TempK, TempK, m_Data)) return;
	UpdateRlccList();
	m_bDataUpDate = TRUE;
	Data2Dlg();
	return;
}

void CDgnLateralCapaCheckDlg::OnDelBtn() 
{
	int nSize = m_RlccList.GetItemCount();
	int nItem = 0;
	for(int i=0 ; i<nSize ; i++)
	{
		if(m_nCurSelectedKey == m_RlccList.GetItemData(i))
		{ nItem = i;  break; }
	}
	
	m_bDataUpDate = FALSE;
	if(!m_pDoc->m_pDataCtrl->DelRlcc(m_nCurSelectedKey)) return ;	
	UpdateRlccList();
	m_bDataUpDate = TRUE;
	if(nSize <= 1) m_nCurSelectedKey = 0;
	else           m_nCurSelectedKey = m_RlccList.GetItemData(max(0, min(nSize-2, nItem)));
	Data2Dlg();
}


void CDgnLateralCapaCheckDlg::OnDgnClose() 
{	
	// TODO: Add extra cleanup here
	DestroyWindow();
}

void CDgnLateralCapaCheckDlg::OnDgnExecute() 
{	
	GClearHistory();
	CArray<UINT, UINT> keyList;
	m_pDoc->m_pAttrCtrl->GetRlccKeyList(keyList);
	CDgnToolMPhi_SeismicDesign SeismicCalc;
	_DGN_RESERVE_LATERAL_CAPA_RES m_ResData;
	if(!SeismicCalc.Calc_RLCC(keyList, m_ResData)) 
	{ 
		GSaveHistoryFormat(_LS(IDS_DGN_RLC_CALC_FAIL_Err));  
		return; 
	}
	
	CDgnLateralCapaResultDlg dlg;
	dlg.SetResData(m_ResData);
	dlg.DoModal();
}

void CDgnLateralCapaCheckDlg::OnItemchangedRlccList(NMHDR* pNMHDR, LRESULT* pResult)
{		
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	if(m_bDataUpDate)
	{	
		int nItem = pNMListView->iItem;
		UINT TempK = (UINT)(m_RlccList.GetItemData(nItem));
		if(TempK>0) m_nCurSelectedKey = TempK;

		if(!Data2Dlg()) return;		
	}

	*pResult = 0;
}
void CDgnLateralCapaCheckDlg::OnClickRlccList(NMHDR* pNMHDR, LRESULT* pResult)
{	
	OnItemchangedRlccList(pNMHDR,pResult);
	*pResult = 0;
}

void CDgnLateralCapaCheckDlg::InitCtrl()
{		
	SetHeaderTitle();
	m_WuEdit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_WuUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_WpEdit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_WpUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_TyEdit.SetUnitType(D_UNITSYS_BASE_TIME);
	m_TzEdit.SetUnitType(D_UNITSYS_BASE_TIME);
	m_TUnit.SetUnitType(D_UNITSYS_BASE_TIME);
	m_HEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_HUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_M0yEdit.SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_M0zEdit.SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_M0Unit.SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_P0Edit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_P0Unit.SetUnitType(D_UNITSYS_BASE_FORCE);

	m_TypeComb.ResetContent();
	m_TypeComb.AddString(_LS(IDS_DGN_RLC_RCSINGLEPIER));


	BOOL bUserInitForce = (m_nInitialForce == 2);
	
	GetDlgItem(IDC_RLCC_INITFORCE_GROUP)->EnableWindow(bUserInitForce);
	GetDlgItem(IDC_RLCC_M0_STC)->EnableWindow(bUserInitForce);
	GetDlgItem(IDC_RLCC_M0Y_STC)->EnableWindow(bUserInitForce);
	GetDlgItem(IDC_RLCC_M0Z_STC)->EnableWindow(bUserInitForce);
	GetDlgItem(IDC_RLCC_M0Y_EDT)->EnableWindow(bUserInitForce);		
	GetDlgItem(IDC_RLCC_M0Z_EDT)->EnableWindow(bUserInitForce);
	GetDlgItem(IDC_RLCC_M0_UNIT)->EnableWindow(bUserInitForce);
	GetDlgItem(IDC_RLCC_P0_STC)->EnableWindow(bUserInitForce);
	GetDlgItem(IDC_RLCC_P0_EDT)->EnableWindow(bUserInitForce);
	GetDlgItem(IDC_RLCC_P0_UNIT)->EnableWindow(bUserInitForce);
}

void CDgnLateralCapaCheckDlg::Initial_Data()
{
	m_nCurSelectedKey = 0;
	Data2Dlg();	
}

BOOL CDgnLateralCapaCheckDlg::Data2Dlg()
{	
	if(!m_pDoc->m_pAttrCtrl->GetRlcc(m_nCurSelectedKey, m_Data)) m_Data.Initialize();

	m_TypeComb.SelectString(-1, GetTypeName(m_Data.nType));
	m_NameEdit.SetWindowText(m_Data.strName);
	m_SelectElemString = GetStrElemList(m_Data.aElemList, TRUE);// Elem추가 제거시 먼저 실행될수 있으므로 아직 Elem가 생성되지 않은 상태에서는 미선택으로 처리함
	m_MyChkBox.SetCheck(m_Data.bTraverse);
	m_MzChkBox.SetCheck(m_Data.bLongitude);
	m_WuEdit.SetEditUnit(m_Data.dWu);
	m_WpEdit.SetEditUnit(m_Data.dWp);
	m_TyEdit.SetEditUnit(m_Data.dNaturalFrequency[0]);
	m_TzEdit.SetEditUnit(m_Data.dNaturalFrequency[1]);
	m_HEdit.SetEditUnit(m_Data.dActHeight);
	m_M0yEdit.SetEditUnit(m_Data.dInitMoment[0]);
	m_M0zEdit.SetEditUnit(m_Data.dInitMoment[1]);
	m_P0Edit.SetEditUnit(m_Data.dInitAxisForce);	

	UpdateData(FALSE);

	OnMyMzChk();

	m_editSelectElem.SelectByStr(m_SelectElemString);
	
	return TRUE;
}
BOOL CDgnLateralCapaCheckDlg::Dlg2Data()
{
	UpdateData();
	if(!m_pDoc->m_pAttrCtrl->GetRlcc(m_nCurSelectedKey, m_Data)) m_Data.Initialize();

	CString strType;
	m_TypeComb.GetWindowText(strType);
	m_Data.nType = GetTypeID(strType);
	m_NameEdit.GetWindowText(m_Data.strName);
	m_pDoc->m_pViewCtrl->GetAllSelectedElem(m_Data.aElemList);
	m_Data.bTraverse  = m_MyChkBox.GetCheck();
	m_Data.bLongitude = m_MzChkBox.GetCheck();
	m_Data.dWu = m_WuEdit.GetEditValue();
	m_Data.dWp = m_WpEdit.GetEditValue();
	m_Data.dNaturalFrequency[0] = m_TyEdit.GetEditValue();
	m_Data.dNaturalFrequency[1] = m_TzEdit.GetEditValue();
	m_Data.dActHeight = m_HEdit.GetEditValue();
	m_Data.dInitMoment[0] = m_M0yEdit.GetEditValue();
	m_Data.dInitMoment[1] = m_M0zEdit.GetEditValue();
	m_Data.dInitAxisForce = m_P0Edit.GetEditValue();	
	
	return TRUE;
}

void CDgnLateralCapaCheckDlg::SetHeaderTitle()
{
	
	CStringArray aTitle;
	CArray<int, int> aWidth;

#define M_ADD_LIST_INFO(TITLE, WIDTH) aTitle.Add(TITLE); aWidth.Add(WIDTH)
	M_ADD_LIST_INFO(_LS(IDS_DGN_LIST_NAME),      90);  //_T("Name")   
	M_ADD_LIST_INFO(_LS(IDS_DGN_LIST_TYPE),      90);  //_T("Type")
	M_ADD_LIST_INFO(_LS(IDS_DGN_CHK_ELEM_LIST), 145);  //_T("Element List") 
#undef M_ADD_LIST_INFO

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_RlccList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_RlccList.GetSafeHwnd(), dwStyle);
	
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
		m_RlccList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}



void CDgnLateralCapaCheckDlg::UpdateRlccList()
{
	T_RLCC_K RlccKey;
	T_RLCC_D RlccData;
	LVITEM lvitem;
	CString str;

	CArray<UINT, UINT> keyList;
	m_pDoc->m_pAttrCtrl->GetRlccKeyList(keyList);
	int nSize = keyList.GetSize();
	int nItem = m_RlccList.GetItemCount();
	if(nSize != nItem) 
	{		
		if(nSize < nItem)
		{			
			for(int i=nItem-1 ; i>= nSize ; i--)
			{ m_RlccList.DeleteItem(i); }
		}
		nItem = m_RlccList.GetItemCount();
	}
	
	for(int i=0 ; i<nSize ; i++)
	{	
		RlccKey = keyList.GetAt(i);
		if(!m_pDoc->m_pAttrCtrl->GetRlcc(RlccKey, RlccData)) { ASSERT(0); RlccData.Initialize(); }
		
		lvitem.iItem = i;
		for(int j=0; j < 3; j++)
		{
			lvitem.iSubItem = j;			
			if     (j==0) str.Format(_T("%s"), RlccData.strName);
			else if(j==1) str = GetTypeName(RlccData.nType);
			else if(j==2) str = GetStrElemList(RlccData.aElemList);

			lvitem.pszText = str.GetBuffer(0);
			lvitem.mask=LVIF_TEXT;

			if (j == 0) 
			{
				lvitem.mask |= LVIF_PARAM;
				lvitem.lParam = (LPARAM)RlccKey;
				if(i >= nItem) m_RlccList.InsertItem(&lvitem);
				else           m_RlccList.SetItem(&lvitem);
			}
			else m_RlccList.SetItem(&lvitem);
			str.ReleaseBuffer();
		}
		m_RlccList.SetItemData(i, (DWORD)RlccKey);
	}
}

CString CDgnLateralCapaCheckDlg::GetStrElemList(CArray<T_ELEM_K, T_ELEM_K>& KeyList, BOOL bExistChk)
{
	int nSize = KeyList.GetSize();
	if(bExistChk)
	{
		for(int i=0 ; i<nSize ; i++)
		{ if(!m_pDoc->m_pAttrCtrl->ExistElem(KeyList[i])) return _T(""); }
	}

	long *aNum = new long[nSize];
	for (int i = 0; i < nSize; i++)
		aNum[i] = (long)(KeyList[i]);
	qsort((void*)aNum, nSize, sizeof(long), CNumericOptimizer::comparei);

	// Optimize한다.
	CNumericOptimizer optimizer;
	CString sOptimizedString = optimizer.Optimize(aNum, nSize);
	
	delete[] aNum;
	
	return sOptimizedString;
}

CString CDgnLateralCapaCheckDlg::GetTypeName(int nType)
{
	if(nType == 0) return _LS(IDS_DGN_RLC_RCSINGLEPIER);
	ASSERT(0);
	return _T("None");
}
int CDgnLateralCapaCheckDlg::GetTypeID(CString strName)
{
	if(strName == _LS(IDS_DGN_RLC_RCSINGLEPIER)) return 0;
	ASSERT(0);
	return 0;	
}


