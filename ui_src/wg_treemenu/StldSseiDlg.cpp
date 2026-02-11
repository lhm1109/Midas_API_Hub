// StldSseiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldSseiDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_EditData.h"

#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_StrParser.h"
//#include "..\wg_base\I_GENStateDefine.h"

#include "..\wg_cmd\wg_cmdAll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldSseiDlg dialog


CStldSseiDlg::CStldSseiDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldSseiDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldSseiDlg)
	m_bCheckUSE = FALSE;
	m_bAdditional = FALSE;
	m_bSseiNodal = TRUE;
	m_bSseiBeam = TRUE;
	m_nDirectionMtd = 0;
	//}}AFX_DATA_INIT
}
void CStldSseiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldSseiDlg)
	DDX_Control(pDX, IDC_TM_KH_EDT, m_editKh);
	DDX_Check(pDX, IDC_TM_CHK_USE, m_bCheckUSE);
	DDX_Text(pDX, IDC_TM_DESC_EDT, m_strDescription);
	DDX_Radio(pDX, IDC_TM_SEI_RDO, m_nDirectionMtd);
	DDX_Control(pDX, IDC_TM_1ST_EDT, m_wndT1Edit);
	DDX_Control(pDX, IDC_TM_2ST_EDT, m_wndT2Edit);
	DDX_Control(pDX, IDC_TM_ANGLE_EDT, m_wndT3Edit);
	DDX_Control(pDX, IDC_TM_1ST_UNIT, m_wndT1Unit);
	DDX_Control(pDX, IDC_TM_2ST_UNIT, m_wndT2Unit);
	DDX_Control(pDX, IDC_TM_CBO_LC, m_LoadCase);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_USE, m_wndGrupCobx);
	DDX_Control(pDX, IDC_TM_EDT_EL, m_editSelectElem);
	DDX_Control(pDX, IDC_TM_LIST, m_List);
	DDX_Check(pDX, IDC_TM_CHK_ADDITIONAL, m_bAdditional);
	DDX_Check(pDX, IDC_TM_SSEI_NODAL, m_bSseiNodal);
	DDX_Check(pDX, IDC_TM_SSEI_BEAM, m_bSseiBeam);
	DDX_Control(pDX, IDC_TM_CBO_LC2, m_LoadCase2);
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CStldSseiDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldSseiDlg)
	ON_BN_CLICKED(IDC_TM_BTN_LC, OnTmBtnLc)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	ON_BN_CLICKED(IDC_TM_ADD, OnTmAdd)
	ON_BN_CLICKED(IDC_TM_MODIFY, OnTmModify)
	ON_BN_CLICKED(IDC_TM_DELETE2, OnTmDelete)
	ON_BN_CLICKED(IDC_TM_CHK_USE, OnTmChkUse)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_SEI_RDO, OnTmSeiRdo)
	ON_CBN_SELCHANGE(IDC_TM_LDGR_CBO_USE, OnSelchangeTmGrupCboUse)
	ON_EN_UPDATE(IDC_TM_2ST_EDT, OnUpdateTm2stEdt)
	ON_EN_CHANGE(IDC_TM_2ST_EDT, OnChangeTm2stEdt)
	ON_BN_CLICKED(IDC_TM_SEI_RDO2, OnTmSeiRdo)
	ON_BN_CLICKED(IDC_TM_CHK_ADDITIONAL, OnTmChkAdditional)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_LIST, OnItemchangedTmList)
	//}}AFX_MSG_MAP
 // ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER, OnMouseEditEnter)
 // ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
END_MESSAGE_MAP()

void CStldSseiDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_WG_TREEMENU_Load_Case), _LS(IDS_WG_TREEMENU_Angle), _LSX(Kh)};
	int nColWidth[] = {80,70,60};
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < sizeof(nColWidth)/sizeof(int); i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CStldSseiDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountSsei();
	if(nItemCount == 0) return;

	POSITION pos;
	int nCount;

	T_SSEI_K Key;
	T_SSEI_D Data;

	int* KeyBuf=new int[nItemCount];
	nCount = 0;
	pos = m_pDoc->m_pAttrCtrl->GetStartSsei();
	while(pos !=NULL)
	{
		m_pDoc->m_pAttrCtrl->GetNextSsei(pos, Key, Data);
		KeyBuf[nCount++] = Key;
	}
	
	qsort(KeyBuf, nItemCount, sizeof(T_SSEI_K), CCompFunc::UINTAsc);
	
	for(nCount = 0 ; nCount < nItemCount ; nCount++)
	{
		Key=KeyBuf[nCount];
		m_pDoc->m_pAttrCtrl->GetSsei(Key,Data);
		m_List.InsertItem(nCount, _T(""));
		SetItem(nCount, Key, Data);
	}
	delete []KeyBuf;
}

CString CStldSseiDlg::DataToStr(int i, T_SSEI_K key, T_SSEI_D &data)
{
	CString value;
	T_STLD_D stldData;

	m_pDoc->m_pAttrCtrl->GetStld(key, stldData);
	if (i == 0) 
	{
		value.Format(_T("%s"), stldData.LoadCaseName);
	}
	else if (i == 1) value.Format(_T("%g"), data.dAngle);
	else if (i == 2) value.Format(_T("%g"), data.dKh);
	return value;
}

BOOL CStldSseiDlg::GetSelectedItem(T_SSEI_K &key)
{
	
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	CString sLoadCaseName = m_List.GetItemText(iItem, 0);
	key = (T_SSEI_K)m_pDoc->m_pAttrCtrl->GetStldKey(sLoadCaseName);

	return TRUE;
}

void CStldSseiDlg::SetItem(int nIndex, T_SSEI_K key, T_SSEI_D &data)
{
	LVITEM lvitem;
	CString str;

	for (int i = 0; i < 3; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		str = DataToStr(i, key, data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

	  m_List.SetItem(&lvitem);
		m_List.SetItemData(nIndex,(DWORD)key);
		str.ReleaseBuffer();
	}
}

BOOL CStldSseiDlg::ValidItem(T_SSEI_K key, T_SSEI_D& data)
{
	if (key <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Invalid_loadcase_));
		return FALSE;
	}

	return TRUE;
}
/*

	CString Description;
	BOOL    bUseGrup;
	UINT    GrupKey;
	CArray<T_ELEM_K, T_ELEM_K> arKeyElem;
	int     nDirectionMtd;                   // 0=Points, 1=Angle
	double  dPoint1[3];  
	double  dPoint2[3];  
	double  dAngle;
	double  dKh;

		Description   = src.Description;
		bUseGrup      = src.bUseGrup;
		GrupKey       = src.GrupKey;
		arKeyElem.Copy(src.arKeyElem);
		nDirectionMtd = src.nDirectionMtd;    
		for(int i = 0; i < 3; i++)
		{
			dPoint1[i]  = src.dPoint1[i];
			dPoint2[i]  = src.dPoint2[i];      
		}
		dAngle        = src.dAngle;
		dKh           = src.dKh;
*/

BOOL CStldSseiDlg::Dlg2Data(T_SSEI_K &key, T_SSEI_D &data) 
{
	UpdateData(TRUE);
	data.Initialize();
	
	UINT loadType;
	if (!m_LoadCase.GetSelectedLoad(loadType, key))
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Loadcase_is_not_selected_));
		return FALSE;
	}
	
	data.dKh = m_editKh.GetEditValue();
	data.Description.Format(_T("%s"), m_strDescription);
	data.bUseGrup = m_bCheckUSE;
	if (m_bCheckUSE)
		m_wndGrupCobx.GetSelectedGrup(data.GrupKey);
	else
		m_pDoc->m_pViewCtrl->GetAllSelectedElem(data.arKeyElem);

	data.nDirectionMtd = m_nDirectionMtd;
	if (m_nDirectionMtd == 1)
	{
		data.dAngle = m_wndT3Edit.GetEditValue();
	}
	else
	{
		m_wndT1Edit.GetCoordData(&data.dPoint1[0], &data.dPoint1[1], &data.dPoint1[2]);
		m_wndT2Edit.GetCoordData(&data.dPoint2[0], &data.dPoint2[1], &data.dPoint2[2]);
	}  

	data.bAdditionDeadLoad = m_bAdditional;
	if (m_bAdditional)
	{
		data.bNodalLoad = m_bSseiNodal;
		data.bBeamLoad = m_bSseiBeam;
		int nSel = m_LoadCase2.GetCurSel();
		data.StldKey = m_LoadCase2.GetItemData(nSel);
	}
	return TRUE;
}

BOOL CStldSseiDlg::Data2Dlg(T_SSEI_K key, T_SSEI_D &data)
{
	CString strElem;
	CNumericOptimizer optimizer;

	m_LoadCase.ChangeSelect(D_LOADCASE_STATIC, key);
	m_bCheckUSE = data.bUseGrup;
	m_strDescription = data.Description;
	if (m_bCheckUSE)
	{
		m_wndGrupCobx.ChangeSelect(data.GrupKey);
	}
	else
	{
		strElem = optimizer.OptimizeUnsortedArray(data.arKeyElem);
		m_editSelectElem.SetWindowText(strElem);
		m_editSelectElem.SelectByStr(strElem);
	}
	
	m_nDirectionMtd = data.nDirectionMtd; // 0=Points, 1=Angle
	
	if(m_nDirectionMtd == 0)
	{
		CString str;
		str.Format(_T("%5.1f, %5.1f, %5.1f"), data.dPoint1[0], data.dPoint1[1], data.dPoint1[2]);
		m_wndT1Edit.SetWindowText(str);

		str.Format(_T("%5.1f, %5.1f, %5.1f"), data.dPoint2[0], data.dPoint2[1], data.dPoint2[2]);
		m_wndT2Edit.SetWindowText(str);

		double dAngle;
		if(!m_pDoc->m_pEditData->GetSseiAngle(data.dPoint1, data.dPoint2, dAngle)) return FALSE;
		m_wndT3Edit.SetEditUnit(dAngle);
	}
	else
		m_wndT3Edit.SetEditUnit(data.dAngle);
	
	m_editKh.SetEditUnit(data.dKh);

	m_bAdditional = data.bAdditionDeadLoad;
	m_bSseiNodal = data.bNodalLoad;
	m_bSseiBeam = data.bBeamLoad;
	if (m_bAdditional)
	{
		int nSize = m_LoadCase2.GetCount();
		for (int i=0; i<nSize; i++)
		{
			if (m_LoadCase2.GetItemData(i) == data.StldKey)
			{
				m_LoadCase2.SetCurSel(i);
				break;
			}
		}
	}

	UpdateData(FALSE);
	return TRUE;
}

void CStldSseiDlg::UpdateBuffer()
{  
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bUpdate = FALSE;
	BOOL bUpdateLC = FALSE;

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case UR_SSEI_ADD: case UR_SSEI_DEL:
			bUpdate = TRUE;
		case UR_STLD_ADD: case UR_STLD_DEL:
			bUpdateLC = TRUE;
		default:
			break;
		}
	}

	if (bUpdate)
		MakeItemEx();
	if (bUpdateLC)
		OnMakeStldCmb();
}
void CStldSseiDlg::OnTmBtnLc() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldSseiDlg::OnTmDefineLdgrBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_GROUP,0));	
}
void CStldSseiDlg::OnTmAdd() 
{
	T_SSEI_K key=1;
	T_SSEI_D data;
	data.Initialize();

	if (!Dlg2Data(key, data)) 
	{
		return;
	}
	if (!ValidItem(key, data)) return;

	if (m_pDoc->m_pDataCtrl->AddSsei(key,data))
	{
		int nIndex = m_pDoc->m_pAttrCtrl->GetCountSsei();
		m_List.SetItemState(nIndex-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CStldSseiDlg::OnTmModify() 
{
	T_SSEI_K key=1, keyOld;
	T_SSEI_D data;
	data.Initialize();

	// Get Old Key
	if (!GetSelectedItem(keyOld))
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_No_selected_item_in_the_list__nChoo));
		return;
	}
	int nIndex = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	ASSERT(nIndex != -1);

	// Get new key and data
	if (!Dlg2Data(key, data)) return;
	if (!ValidItem(key, data)) return;
 
	// BOOL ModifySsei(T_SSEI_K KeyOld, T_SSEI_K KeyNew, T_SSEI_D& rDataNew);
	if (m_pDoc->m_pDataCtrl->ModifySsei(keyOld, key, data))
		m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CStldSseiDlg::OnTmDelete() 
{
	T_SSEI_K key;

	if (!GetSelectedItem(key))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_system_temperat));
		return;
	}
	int nIndex = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	ASSERT(nIndex != -1);
	if (m_pDoc->m_pDataCtrl->DelSsei(key))
	{
		if (nIndex >= m_pDoc->m_pAttrCtrl->GetCountSsei()) nIndex--;
		m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CStldSseiDlg message handlers
BOOL CStldSseiDlg::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();

	/**
	T_SSEI_K key;
	T_SSEI_D data;
	data.Initialize();
	**/

	m_wndT1Unit.SetUnitType(0); 
	//m_wndT1Edit.SetUnitType(CUnitCtrl::m_SSEI_UNIT.dPoint);
	m_wndT2Unit.SetUnitType(0);
	//m_wndT2Edit.SetUnitType(CUnitCtrl::m_SSEI_UNIT.dPoint);
	//m_wndT3Unit.SetUnitType(CUnitCtrl::m_SSEI_UNIT.dblSysTemp);
	//m_wndT3Edit.SetUnitType(CUnitCtrl::m_SSEI_UNIT.dblSysTemp);
	//---> static으로 바꾸자
	
	//m_editSelectElem.Connect(SC_ID_ELEM, &m_editSelectElem);
	
	m_wndT3Edit.SetUnitType(0);

	/*MouseEdit 속성 만들기*/
	m_wndT1Edit.SetAttUcsPos();
	m_wndT1Edit.SetModeToUse(MOUSEEDIT_USE_SETP2_FIRST);
	m_wndT2Edit.SetAttUcsPos();
	m_wndT2Edit.SetModeToUse(MOUSEEDIT_USE_SETP2_SECOND);
	m_wndT2Edit.SetLButtonDownNotifyWindow(this);
	m_wndT1Edit.SetNextLink(&m_wndT2Edit);
	//m_wndT2Edit.SetEnterNotifyWindow(this);
	
	//m_wndT2Edit.SetNextLink(&m_wndT1Edit);

	SetListCtrlHeader();  
	MakeItemEx();

	//Data2Dlg(key,data);
	m_nDirectionMtd = 0;

	UpdateData(FALSE);
	OnTmChkUse();
	OnTmSeiRdo();  
	OnTmChkAdditional();
	OnMakeStldCmb();
	return TRUE;
}

void CStldSseiDlg::OnMakeStldCmb()
{
	m_LoadCase2.ResetContent();

	CArray<T_STLD_K, T_STLD_K> rKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(rKeyList);

	T_STLD_D StldD;
	for (int i=0; i<rKeyList.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetStld(rKeyList[i], StldD);
		m_LoadCase2.AddString(StldD.LoadCaseName);
		m_LoadCase2.SetItemData(i, rKeyList[i]);
	}
	if (rKeyList.GetSize() > 0)
		m_LoadCase2.SetCurSel(0);
}

BOOL CStldSseiDlg::ExternalInit(UINT key)
{
	if (key == 0) return FALSE;

	T_SSEI_D data;
	if (!m_pDoc->m_pAttrCtrl->GetSsei(key, data))
		return FALSE;

	int nItemNum = m_List.GetItemCount();
	int i = 0;
	for (i=0; i<nItemNum; i++)
		if (key == m_List.GetItemData(i)) break;
	if (i == nItemNum) return FALSE;

	m_List.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);

	return TRUE;
}

void CStldSseiDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;
	int nIndex;
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
		nIndex = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
		MakeItemEx(); // Unit Change시 MakeItemEx를 한다.
		if (nIndex >= 0)
			m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
		break;
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
		MakeItemEx();
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

void CStldSseiDlg::OnTmClose() 
{
	CloseDlg();
}

void CStldSseiDlg::OnTmChkUse() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_TM_LDGR_CBO_USE)->EnableWindow(m_bCheckUSE);
	GetDlgItem(IDC_TM_DEFINE_LDGR_BTN)->EnableWindow(m_bCheckUSE);

	m_editSelectElem.SetReadOnly(m_bCheckUSE);
//  m_editSelectElem.EnableWindow(!m_bCheckUSE);
	OnSelchangeTmGrupCboUse();

	
	if(m_bCheckUSE)
		m_editSelectElem.Disconnect();
	else
		m_editSelectElem.Connect(SC_ID_ELEM, &m_editSelectElem);
}

void CStldSseiDlg::OnTmSeiRdo() 
{
	UpdateData(TRUE);
	m_wndT1Edit.EnableWindow(m_nDirectionMtd == 0);
	m_wndT2Edit.EnableWindow(m_nDirectionMtd == 0);
	m_wndT3Edit.SetReadOnly(m_nDirectionMtd != 1);
	
	if (m_nDirectionMtd == 1)
	{
		CMouseEdit::ImNotAssocWindowST();
		m_wndT1Edit.SetModeToUse(MOUSEEDIT_USE_NOTHING);
		m_wndT2Edit.SetModeToUse(MOUSEEDIT_USE_NOTHING);
	}
	else
	{
		m_wndT1Edit.SetModeToUse(MOUSEEDIT_USE_SETP2_FIRST);
		m_wndT2Edit.SetModeToUse(MOUSEEDIT_USE_SETP2_SECOND);
	}
}

void CStldSseiDlg::OnSelchangeTmGrupCboUse() 
{
	T_GRUP_K rKey; //T_GRUP_D rData;
	CArray<T_ELEM_K, T_ELEM_K> arKeyElem;

	CString str;
	CNumericOptimizer optimizer;
	UpdateData(TRUE); 

	if(m_bCheckUSE)
	{    
		m_wndGrupCobx.GetSelectedGrup(rKey); // 그룹 박스에서 선택 된 것의 키 정보를 얻어 온다.
	
		m_pDoc->m_pEditData->GetSseiElemList(rKey, arKeyElem); //Element 키 정보를 얻어온다.

		str = optimizer.OptimizeUnsortedArray(arKeyElem); // optimise data
	
		m_editSelectElem.SetWindowText(str); // set editbox
	}
}

void CStldSseiDlg::OnUpdateTm2stEdt() 
{	
	GetAngle();
}

void CStldSseiDlg::OnChangeTm2stEdt() 
{
	GetAngle();
}

void CStldSseiDlg::GetAngle()
{
	/*
	double dAngle= 0;
	double uPoint1[3], uPoint2[3], wPoint1[3], wPoint2[3];

	if (!CFormulaEdit::GetPosition(&m_wndT1Edit, uPoint1[0], uPoint1[1], uPoint1[2]))
	{
		AfxMessageBox(_T("Error"));
	}
	m_wndT1Edit.GetWcsPos(uPoint1[0], uPoint1[1], uPoint1[2], wPoint1[0], wPoint1[1], wPoint1[2]);

	if (!CFormulaEdit::GetPosition(&m_wndT2Edit, uPoint2[0], uPoint2[1], uPoint2[2]))
	{
		AfxMessageBox(_T("Error"));
	}
	m_wndT2Edit.GetWcsPos(uPoint2[0], uPoint2[1], uPoint2[2], wPoint2[0], wPoint2[1], wPoint2[2]);


	if(!m_pDoc->m_pEditData->GetSseiAngle(wPoint1, wPoint2, dAngle)) return;

	for(int i = 0; i < 3; i++)
	{
		m_dPoint1[i] = wPoint1[i];
		m_dPoint2[i] = wPoint2[i];
	}
	
	m_wndT3Edit.SetEditUnit(dAngle);
	*/

	double dPoint1[3], dPoint2[3];
	double dAngle = 0.0;

	m_wndT1Edit.GetCoordData(&dPoint1[0], &dPoint1[1], &dPoint1[2]);
	m_wndT2Edit.GetCoordData(&dPoint2[0], &dPoint2[1], &dPoint2[2]);
	m_pDoc->m_pEditData->GetSseiAngle(dPoint1, dPoint2, dAngle);
	m_wndT3Edit.SetEditUnit(dAngle);
}

BOOL CStldSseiDlg::GetSelectedData(int &nIndex)
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CStldSseiDlg::OnItemchangedTmList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	T_SSEI_K key;
	T_SSEI_D data;
	
	int nIndex;

	if (!GetSelectedData(nIndex))
	{
		data.Initialize();
	}
	else 
	{
		key = m_List.GetItemData(nIndex);
		m_pDoc->m_pAttrCtrl->GetSsei(key, data);
	
		Data2Dlg(key,data);
	
		OnTmChkUse();
		OnTmSeiRdo();
		OnSelchangeTmGrupCboUse();
		OnTmChkAdditional();
	}

	*pResult = 0;
}

LRESULT CStldSseiDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
//  m_wndT2Edit.AddNodeKey(int NodeKey)
	return 0L;
}

void CStldSseiDlg::OnTmChkAdditional()
{
	UpdateData();
	GetDlgItem(IDC_TM_CBO_LC2)->EnableWindow(m_bAdditional);
	GetDlgItem(IDC_TM_SSEI_NODAL)->EnableWindow(m_bAdditional);
	GetDlgItem(IDC_TM_SSEI_BEAM)->EnableWindow(m_bAdditional);
}
