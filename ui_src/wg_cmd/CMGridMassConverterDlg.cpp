// CMGridMassConverterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGridMassConverterDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_main\wg_mainRes2.h"

using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMGridMassConverterDlg dialog


CCMGridMassConverterDlg::CCMGridMassConverterDlg(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMGridMassConverterDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMGridMassConverterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bWorkTab = FALSE;
	m_WTLoadCaseNum = -1;
}


void CCMGridMassConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGridMassConverterDlg)
	DDX_Control(pDX, IDC_CMD_GLTM_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_GLTM_MGDR_CMB, m_wndMgdrCmb);
	DDX_Control(pDX, IDC_CMD_GLTM_GILC_CMB, m_wndGilcCmb);
	DDX_Control(pDX, IDC_CMD_GLTM_SCALE_FACTOR_EDIT, m_wndScaleFactor);
	DDX_Control(pDX, IDC_CMD_GLTM_GRAVITY_UNIT, m_wndGravityUnit);
	DDX_Control(pDX, IDC_CMD_GLTM_GRAVITY_EDIT, m_wndGravityEdit);
	DDX_Control(pDX, IDC_CMD_GLTM_FISH_BONE_CHK, m_wndFishBoneChk);
	DDX_Control(pDX, IDC_CMD_GLTM_DIR_Z_CHK, m_wndDirZChk);
	DDX_Control(pDX, IDC_CMD_GLTM_DIR_Y_CHK, m_wndDirYChk);
	DDX_Control(pDX, IDC_CMD_GLTM_DIR_X_CHK, m_wndDirXChk);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGridMassConverterDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMGridMassConverterDlg)
	ON_BN_CLICKED(IDC_CMD_GLTM_GILC_BTN, OnCmdGilcBtn)
	ON_BN_CLICKED(IDC_CMD_GLTM_FISH_BONE_CHK, OnCmdGltmFishBoneChk)
	ON_BN_CLICKED(IDC_CMD_GLTM_ADD_BTN, OnCmdGltmAddBtn)
	ON_BN_CLICKED(IDC_CMD_GLTM_MOD_BTN, OnCmdGltmModBtn)
	ON_BN_CLICKED(IDC_CMD_GLTM_DEL_BTN, OnCmdGltmDelBtn)
	ON_BN_CLICKED(IDC_CMD_GLTM_OK_BTN, OnCmdGltmOkBtn)
	ON_BN_CLICKED(IDC_CMD_GLTM_CANCEL_BTN, OnCmdGltmCancelBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_GLTM_LIST, OnChangedCurrentItem)
	ON_BN_CLICKED(IDC_CMD_GLTM_REMOVE_BTN, OnCmdGltmRemoveBtn)
	ON_BN_CLICKED(IDC_CMD_GLTM_CALC_MASS, OnCmdGltmCalcMass)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGridMassConverterDlg message handlers
BOOL CCMGridMassConverterDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();
	GetDlgItem(IDC_CMD_GLTM_CLOSE_BTN)->ShowWindow(FALSE);
	
	m_pDoc = CDBDoc::GetDocPoint();
	
	if(!m_pDoc->m_pAttrCtrl->GetGltm(m_Data))
	{    
//    GetDlgItem(IDC_CMD_GLTM_REMOVE_BTN)->EnableWindow(FALSE);
		m_Data.Initialize();
		m_Data.dGravity = m_pDoc->m_pInitCtrl->DefaultGravity();
	}

	m_wndScaleFactor.SetUnitType(0);  
	m_wndGravityUnit.SetUnitType(CUnitCtrl::m_GLTM_UNIT.dGravity);

	m_wndGilcCmb.SetLoadType(D_SELECTLOAD_GRIDMODEL);
	
	// Dead Load Type 이외는 제외함
	m_wndGilcCmb.RemoveGilcByType(2, FALSE);
	m_wndGilcCmb.RemoveGilcByType(3, FALSE);
	m_wndGilcCmb.RemoveGilcByType(4, TRUE);

	SetListCtrlHeader();

	// Set Value
	Data2Dlg();
	CtrlManager();

	if(m_bWorkTab) SetCurrentLC();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGridMassConverterDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__LoadCase), _LS(IDS_WG_CMD__ADDD__ScaleFactor)};
	int nColWidth[] = {110, 52};
	int nColNum = 2;
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < nColNum; i++)
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

BOOL CCMGridMassConverterDlg::GetSelectedItem(int &nIndex)
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMGridMassConverterDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_Data.nNumGltmCase;
	if(nItemCount == 0) return;

	for (int nCount = 0; nCount < nItemCount; nCount++)
	{
		m_List.InsertItem(nCount, _T(""));
		SetItem(nCount, m_Data.GltmCase[nCount]);
	}
	m_List.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

CString CCMGridMassConverterDlg::DataToStr(int i, T_GLTM_BASE &data)
{
	CString value;
	if (i == 0) 
	{
		T_GILC_K key = data.LoadCaseKey;
		T_GILC_D data;
		m_pDoc->m_pAttrCtrl->GetGilc(key, data);
		value = data.LoadCaseName;
	}
	else if (i == 1) value.Format(_T("%g"), data.dblFactor);
	return value;
}

void CCMGridMassConverterDlg::SetItem(int nIndex, T_GLTM_BASE &data)
{
	LVITEM lvitem;
	CString str;
	for(int i = 0; i < 2; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		str = DataToStr(i, data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_List.SetItem(&lvitem);
	  str.ReleaseBuffer();
	}
}

BOOL CCMGridMassConverterDlg::ValidItem(T_GLTM_BASE& data)
{
	if (data.LoadCaseKey <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_));
		return FALSE;
	}

	if (data.dblFactor <= 0.0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_scale_factor_));
		return FALSE;
	}
		
	return TRUE;
}

BOOL CCMGridMassConverterDlg::Dlg2Item(T_GLTM_BASE &data)
{
	UINT nLoadCaseType;
	if (!m_wndGilcCmb.GetSelectedLoad(nLoadCaseType, data.LoadCaseKey))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Loadcase_was_not_selected_));
		return FALSE;
	}
	CString csScaleFactor;
	m_wndScaleFactor.GetWindowText(csScaleFactor);
	if(!CStrParser::GetFloatNumber(csScaleFactor, data.dblFactor)) return FALSE;

	return TRUE;
}

BOOL CCMGridMassConverterDlg::Item2Dlg(T_GLTM_BASE &data)
{
	m_wndGilcCmb.ChangeSelect(D_LOADCASE_GRIDMODEL, data.LoadCaseKey);
	CString csScaleFactor;
	csScaleFactor.Format(_T("%g"), data.dblFactor);
	m_wndScaleFactor.SetWindowText(csScaleFactor);

	return TRUE;
}

void CCMGridMassConverterDlg::Data2Dlg()
{  
	m_wndDirXChk.SetCheck(m_Data.bMassDirX);
	m_wndDirYChk.SetCheck(m_Data.bMassDirY);
	m_wndDirZChk.SetCheck(m_Data.bMassDirZ);
	m_wndFishBoneChk.SetCheck(m_Data.bFishBoneModel);
	m_wndGravityEdit.SetEditUnit(m_Data.dGravity);
	m_wndScaleFactor.SetWindowText(_T("1"));

	BOOL bModify = FALSE;
	if(m_pDoc->m_pAttrCtrl->ExistGltm()) bModify = TRUE;

	int nSelect = 0;
	T_MGDR_D MgdrData;
	CArray<T_MGDR_K, T_MGDR_K> aMgdrKey;
	aMgdrKey.RemoveAll();  
	m_pDoc->m_pAttrCtrl->GetMgdrKeyList(aMgdrKey);
	m_wndMgdrCmb.ResetContent();
	for(int i = 0; i < aMgdrKey.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetMgdr(aMgdrKey[i], MgdrData)) continue;
		m_wndMgdrCmb.AddString(MgdrData.Name);
		if(m_Data.MgdrKey == aMgdrKey[i]) nSelect = i;    
	}  
	m_wndMgdrCmb.SetCurSel(nSelect);

	CString str;
	CSize   sz;
	int     dx=0;
	CDC* pDC = GetDC();
	for(int i = 0; i < m_wndMgdrCmb.GetCount(); i++)
	{
		m_wndMgdrCmb.GetLBText(i, str);
		sz = pDC->GetTextExtent(str);    
		if(sz.cx > dx) dx = sz.cx;
	}  
	m_wndMgdrCmb.SetDroppedWidth(dx);
	
	dx = 0;
	for(int i = 0; i < m_wndGilcCmb.GetCount(); i++)
	{
		m_wndGilcCmb.GetLBText(i, str);
		sz = pDC->GetTextExtent(str);    
		if(sz.cx > dx) dx = sz.cx;
	}  
	m_wndGilcCmb.SetDroppedWidth(dx);

	ReleaseDC(pDC);

	MakeItemEx();
}

void CCMGridMassConverterDlg::Dlg2Data()
{   
	m_Data.bMassDirX = m_wndDirXChk.GetCheck() ? 1 : 0;
	m_Data.bMassDirY = m_wndDirYChk.GetCheck() ? 1 : 0;
	m_Data.bMassDirZ = m_wndDirZChk.GetCheck() ? 1 : 0;
	m_Data.bFishBoneModel = m_wndFishBoneChk.GetCheck() ? 1 : 0;
	m_Data.dGravity = m_wndGravityEdit.GetEditValue();
	CString strMgdrName = _T("");
	int nSelect = m_wndMgdrCmb.GetCurSel();
	if(nSelect >= 0)
	{
		m_wndMgdrCmb.GetLBText(nSelect, strMgdrName);
		m_Data.MgdrKey = m_pDoc->m_pAttrCtrl->GetMgdrKey(strMgdrName);
	}
	else m_Data.MgdrKey = 0;

	for(int i = m_Data.nNumGltmCase; i < D_GLTM_NUMGLTM; i++)
		m_Data.GltmCase[i].Initialize();
}

void CCMGridMassConverterDlg::OnCmdGilcBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_GANAL_LOAD,0));	
}

void CCMGridMassConverterDlg::OnCmdGltmFishBoneChk() 
{
	CtrlManager();
}

void CCMGridMassConverterDlg::CtrlManager()
{
	BOOL bExistGmas = FALSE;
	if(m_pDoc->m_pAttrCtrl->GetCountGmas() > 0) bExistGmas = TRUE;
	GetDlgItem(IDC_CMD_GLTM_REMOVE_BTN)->EnableWindow(bExistGmas);
	
	BOOL bCheck = m_wndFishBoneChk.GetCheck();
	m_wndMgdrCmb.EnableWindow(bCheck);	
}

void CCMGridMassConverterDlg::OnCmdGltmAddBtn() 
{
	T_GLTM_BASE data;
	
	if(m_Data.nNumGltmCase >= D_GLTM_NUMGLTM) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Can_t_add_loadcase_any_more_));
		return;
	}
	if(!Dlg2Item(data)) return;
	if(!ValidItem(data)) return;
	for(int i = 0; i < m_Data.nNumGltmCase; i++)
	{
		if(m_Data.GltmCase[i].LoadCaseKey == data.LoadCaseKey)
		{
			CString csLoadCase;
			m_wndGilcCmb.GetWindowText(csLoadCase);
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), csLoadCase);
			AfxMessageBox(msg);
			return;
		}
	}
	
	m_Data.GltmCase[m_Data.nNumGltmCase] = data;
	m_List.InsertItem(m_Data.nNumGltmCase, _T(""));
	SetItem(m_Data.nNumGltmCase, data);
	m_List.SetItemState(m_Data.nNumGltmCase++, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMGridMassConverterDlg::OnCmdGltmModBtn() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if(!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	T_GLTM_BASE data;
	if(!Dlg2Item(data)) return;
	if(!ValidItem(data)) return;
	for(int i = 0; i < m_Data.nNumGltmCase; i++)
	{
		if(i == nIndex) continue;  // 변경 대상은 비교 제외
		if(m_Data.GltmCase[i].LoadCaseKey == data.LoadCaseKey)
		{
			CString csLoadCase;
			m_wndGilcCmb.GetWindowText(csLoadCase);
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), csLoadCase);
			AfxMessageBox(msg);
			return;
		}
	}
	
	m_Data.GltmCase[nIndex] = data;
	SetItem(nIndex, data);
	m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMGridMassConverterDlg::OnCmdGltmDelBtn() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if(!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}
	m_List.DeleteItem(nIndex);
	for(int i = nIndex; i < m_Data.nNumGltmCase-1; i++)
		m_Data.GltmCase[i] = m_Data.GltmCase[i+1];
	m_Data.nNumGltmCase--;
	if(m_Data.nNumGltmCase > 0)
	{
		if(nIndex >= m_Data.nNumGltmCase) nIndex--;
		m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}	
}

void CCMGridMassConverterDlg::OnCmdGltmRemoveBtn() 
{
	if(DelAllGmasData()) CtrlManager();
}

BOOL CCMGridMassConverterDlg::DelAllGmasData()
{
	CArray<T_GMAS_K, T_GMAS_K> aGmasKey;
	m_pDoc->m_pAttrCtrl->GetGmasKeyList(aGmasKey);
	if(aGmasKey.GetSize() != 0)
	{
	  if(!m_pDoc->m_pDataCtrl->DelGmas(aGmasKey)) return FALSE;
	}
	return TRUE;
}

void CCMGridMassConverterDlg::ResetData()
{
	m_Data.Initialize();
	m_Data.dGravity = m_pDoc->m_pInitCtrl->DefaultGravity();
	Data2Dlg();
	CtrlManager();    
}

void CCMGridMassConverterDlg::OnCmdGltmOkBtn() 
{
	Dlg2Data();
	if(m_pDoc->m_pDataCtrl->AddGltm(m_Data))
	{
		CtrlManager();
	}	
}

void CCMGridMassConverterDlg::OnCmdGltmCancelBtn() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);		
}

void CCMGridMassConverterDlg::OnTmClose()
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
}

void CCMGridMassConverterDlg::OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	

	int nIndex;
	T_GLTM_BASE data;
	if(!GetSelectedItem(nIndex))
	{
		data.Initialize();
		data.dblFactor = 1.0;
	}
	else data = m_Data.GltmCase[nIndex];

	// 값을 설정한다.
	Item2Dlg(data);

	*pResult = 0;
}

void CCMGridMassConverterDlg::SetCurrentLC()
{
	m_bWorkTab = FALSE;
	T_GLTM_BASE data;

	data = m_Data.GltmCase[m_WTLoadCaseNum];
	m_List.SetItemState(m_WTLoadCaseNum, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	Item2Dlg(data);
}

void CCMGridMassConverterDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

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

void CCMGridMassConverterDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_GILC_K Key, KeyBak;
	T_GILC_D Data, DataBak; 

	int i;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_GILC_ADD):
				// do nothing
				break;
		case(UR_GILC_DEL):
				// 현재 loadcase list에서 지워진 것 삭제한다.
				pViewBuff->GetGilc(nKey, Key, Data);
				for (i = m_Data.nNumGltmCase-1; i >= 0; i--)
				{
					if (m_Data.GltmCase[i].LoadCaseKey == Key)
					{
						m_List.DeleteItem(i);
						for (int j = i; j < m_Data.nNumGltmCase-1; j++)
							m_Data.GltmCase[j] = m_Data.GltmCase[j+1];
						m_Data.nNumGltmCase--;
					}
				}
				break;
		case(UR_GILC_MFD):
				// 여기서는 저장만 하고 UR_GILC_MFS에서 처리한다.
				pViewBuff->GetGilc(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_GILC_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetGilc(nKey, Key, Data);
				for(i = 0; i < m_Data.nNumGltmCase; i++)
				{
					if(m_Data.GltmCase[i].LoadCaseKey == Key)
					{
						if(DataBak.nLoadCaseType != 0 && DataBak.nLoadCaseType != 1) // Dead Load Type이 아닌 경우
						{              
							m_List.DeleteItem(i);
							for(int j = i; j < m_Data.nNumGltmCase-1; j++)
								m_Data.GltmCase[j] = m_Data.GltmCase[j+1];
							m_Data.nNumGltmCase--;              
						}
						else
						{
							m_Data.GltmCase[i].LoadCaseKey = KeyBak;
							SetItem(i, m_Data.GltmCase[i]);
						}
					}
				}
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while

	// GMAS 의 변화를 반영한다.
	BOOL bUpdate = FALSE;
	pos = pViewBuff->GetStartBuffer();
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_GMAS_ADD):      
			bUpdate = TRUE;
			break;
		case(UR_GMAS_DEL):  
			bUpdate = TRUE;
			break;
		}
	}
	if(bUpdate)
	{    
		CtrlManager();
	}
}

void CCMGridMassConverterDlg::OnCmdGltmCalcMass() 
{
	if(!CheckData()) return;
	//pig if(!DelAllGmasData()) return;
	//pig if(m_pDoc->m_pAttrCtrl->MakeGridMassData(m_Data)) CtrlManager();  
	m_pDoc->RunGridMassConverter(m_Data);
	CtrlManager();
}

BOOL CCMGridMassConverterDlg::CheckData()
{
	Dlg2Data();
	CString msg;
	if(m_Data.nNumGltmCase == 0) 
	{     
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Load_case_is_not_selected));
		return FALSE;
	}
	for(int i = 0; i < m_Data.nNumGltmCase; i++)
	{
		if(m_Data.GltmCase[i].dblFactor <= 0.) 
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_scale_factor_));
			return FALSE;
		}      
	}
	if(!m_Data.bMassDirX && !m_Data.bMassDirY && !m_Data.bMassDirZ)
	{
		AfxMessageBox(_LS(IDS_CMD_GLTM_MASS_DIRECTION));
		return FALSE;
	}
	if(m_Data.dGravity <= 0.) 
	{
		AfxMessageBox(_LS(IDS_CMD_GLTM_GRAVITY));
		return FALSE;
	}
	if(m_Data.bFishBoneModel)
	{
		if(!m_pDoc->m_pAttrCtrl->ExistMgdr(m_Data.MgdrKey)) 
		{
			AfxMessageBox(_LS(IDS_WG_CMD_MAIN_GIRDER_NO_SELECTED_MAIN_GIRDER));
			return FALSE;
		}
	}
	return TRUE;
} 