// CMMvldItemFRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemFRDlg.h"
#include "CMMvldSubItemFRDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_TestEnvMgr.h"
#include "..\wg_base\SpecialChar.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\VehlDef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemFRDlg dialog

CCMMvldItemFRDlg::CCMMvldItemFRDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldItemFRDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvldItemFRDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();

	m_bModify = FALSE;
	m_bSurface = FALSE;
	m_nPreLoadModel = 0;
	
	m_aLoadModelType.RemoveAll();
	m_aLoadModelType.Add(IDC_CMD_LM1_RDO);
	m_aLoadModelType.Add(IDC_CMD_LM2_RDO);
	m_aLoadModelType.Add(IDC_CMD_LM3_RDO);
	m_aLoadModelType.Add(IDC_CMD_LM4_RDO);
	m_aLoadModelType.Add(IDC_CMD_LM5_RDO);
}

void CCMMvldItemFRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemFRDlg)
	DDX_Control(pDX, IDC_CMD_LOADCASE_NAME, m_wndLoadCase);
	DDX_Control(pDX, IDC_CMD_DESCRIPTION  , m_wndDesc);
	DDX_Control(pDX, IDC_CMD_LIST         , m_List);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvhcListDlg implementation functions
BOOL CCMMvldItemFRDlg::OnApplySubDlg(BOOL bModify, T_MVLDfr_BASE& data, int nPos)
{
	if(m_nPreLoadModel != m_Data.nLoadModel) 
	{
		m_Data.arSubLoadData.RemoveAll();
		m_List.DeleteAllItems();
	}	
	
	if (!bModify)
	{
		int nCount = m_Data.arSubLoadData.GetSize();
		m_Data.arSubLoadData.Add(data);
		InsertItem(nCount, data);
	}
	else
	{
		m_Data.arSubLoadData.SetAt(nPos, data);
		ModifyItem(nPos, data);
	}
	return TRUE;
}

void CCMMvldItemFRDlg::Data2Dlg()
{
	m_wndLoadCase.SetWindowText(m_Data.LoadCaseName);
	m_wndDesc.    SetWindowText(m_Data.Description);

	CDlgUtil::CtrlRadioSetCheck(this, m_aLoadModelType, m_Data.nLoadModel);

	MakeItemEx();

	UpdateData(FALSE);
}

BOOL CCMMvldItemFRDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_wndLoadCase.GetWindowText(m_Data.LoadCaseName);
	if(m_pDoc->IsSymbol(m_Data.LoadCaseName)) { AfxMessageBox(_LS(IDS_CMD_MVLD_ERROR_CASE_NAME)); return FALSE; }
	m_wndDesc.GetWindowText(m_Data.Description);

	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadModelType, m_Data.nLoadModel);
	
	return TRUE;
}

void CCMMvldItemFRDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_WG_CMD_FR_Vehicle),  _LS(IDS_WG_CMD__ADDD__Lane0), _LS(IDS_WG_CMD_FR_Sidewalk), _LS(IDS_WG_CMD__ADDD__Lane0) };
	int nColWidth[COLCOUNT] = { 70, 70, 70, 70};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	///////////////////////////////////////////////////////////////////////////////////
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);
	///////////////////////////////////////////////////////////////////////////////////

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvldItemFRDlg::MakeItemEx()
{
	m_List.DeleteAllItems();
	
	int nItemCount = m_Data.arSubLoadData.GetSize();
	if(nItemCount == 0) return;

	CString str;
	int nCount;

	if(nItemCount)
	{
		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
			InsertItem(nCount, m_Data.arSubLoadData[nCount]);
		}
	}
}

BOOL CCMMvldItemFRDlg::InsertItem(int nIndex, T_MVLDfr_BASE &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem;

	lvitem.iItem = nIndex;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr(i, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) 
		{
			nItem = m_List.InsertItem(&lvitem);
		}
		else 
		{
			m_List.SetItem(&lvitem);
		}

		str.ReleaseBuffer();
	}
	
	return TRUE;
}

BOOL CCMMvldItemFRDlg::DeleteItem(int nIndex, T_MVLDfr_BASE &Data)
{
	Data; // Not Used;
	if (nIndex >= 0)
	{
		m_List.DeleteItem(nIndex);	
	}
	return TRUE;
}

BOOL CCMMvldItemFRDlg::ModifyItem(int nIndex, T_MVLDfr_BASE &Data)
{
	int nItem = nIndex;
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, Data);
			m_List.SetItemText(nItem, i, str);	
		}
	}
	return TRUE;
}

CString CCMMvldItemFRDlg::DataToStr(int i, T_MVLDfr_BASE &Data)
{
	CString str;

	T_MVHC_D mvhc;
	T_MVHL_D mvhl;
	if (i == 0)
	{
		m_pDoc->m_pAttrCtrl->GetMvhl(Data.VehicleKey, mvhl);
		if (mvhl.VehicleLoadName.IsEmpty())
			str = _LS(IDS_WG_CMD_NONE);
		else
			str = _T("VL:") + mvhl.VehicleLoadName;
	}
	else if (i == 1)
	{
		if (Data.aSelectedLanes.GetSize() > 0)
		{
			if (Data.aSelectedLanes[0] == 0) str = _T("-");
			else
			{
				str = GetLaneName(Data.aSelectedLanes[0]);
			}
		}
		else str = _T("-");
	}
	else if (i == 2)
	{
		m_pDoc->m_pAttrCtrl->GetMvhl(Data.SidewalKey, mvhl);
		if (mvhl.VehicleLoadName.IsEmpty())
			str = _LS(IDS_WG_CMD_NONE);
		else
			str = _T("VL:") + mvhl.VehicleLoadName;

	}
	else if (i == 3)
	{
		if (Data.aSidewalkLanes.GetSize() > 0)
		{
			if (Data.aSidewalkLanes[0] == 0)
			{
				str = _T("-");
			}
			else
			{
				for(int nl = 0; nl< Data.aSidewalkLanes.GetSize(); nl++)
				{
					CString strTemp;
					if(nl == Data.aSidewalkLanes.GetSize()-1)
					{
						strTemp = GetLaneName(Data.aSidewalkLanes[nl]);
					}
					else
					{
						strTemp = GetLaneName(Data.aSidewalkLanes[nl]) + _T(",");
					}
					str += strTemp;
				}			
			}
		}
		else str = _T("-");
	}
	else
		ASSERT(0);

	return str;
}

CString CCMMvldItemFRDlg::GetLaneName(UINT nLaneKey)
{
	if (m_bSurface)
	{
		T_SLANfr_D slan;
		m_pDoc->m_pAttrCtrl->GetSlanfr(nLaneKey, slan);
		return slan.LaneName;
	}
	else
	{
		T_LLANfr_D llan;
		m_pDoc->m_pAttrCtrl->GetLlanfr(nLaneKey, llan);
		return llan.LineLaneName;
	}
	return _LS(IDS_WG_CMD__ADDD__Error);
}


BOOL CCMMvldItemFRDlg::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;

	BOOL bSuccess;
	if (m_bModify)
		bSuccess = m_pDoc->m_pDataCtrl->ModifyMvldfr(m_csOldName, m_Data);
	else
		bSuccess = m_pDoc->m_pDataCtrl->AddMvldfr(m_Data);
	if (!bSuccess) return FALSE;

	return TRUE;
}


BEGIN_MESSAGE_MAP(CCMMvldItemFRDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldItemFRDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY      , OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD    , OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY , OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE , OnCmdBtnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)

	ON_BN_CLICKED(IDC_CMD_LM1_RDO, OnCmdLoadModelRdo)
	ON_BN_CLICKED(IDC_CMD_LM2_RDO, OnCmdLoadModelRdo)
	ON_BN_CLICKED(IDC_CMD_LM3_RDO, OnCmdLoadModelRdo)
	ON_BN_CLICKED(IDC_CMD_LM4_RDO, OnCmdLoadModelRdo)
	ON_BN_CLICKED(IDC_CMD_LM5_RDO, OnCmdLoadModelRdo)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemFRDlg message handlers

BOOL CCMMvldItemFRDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();

	if (m_pDoc->m_pAttrCtrl->GetCountSlanfr() > 0)
	{
		m_bSurface = TRUE;
	}
	else
	{
		m_bSurface = FALSE;
	}

	if(!m_bModify)
	{
		m_Data.Initialize();
	}

	SetHeaderTitle();
	Data2Dlg();

	m_nPreLoadModel = m_Data.nLoadModel;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvldItemFRDlg::OnOK() 
{
	if (!ApplyOrOK()) return;
	CDialogMove::OnOK();
}

void CCMMvldItemFRDlg::OnCmdApply() 
{
	ApplyOrOK();	
}

void CCMMvldItemFRDlg::OnCmdBtnAdd() 
{
	UpdateData(TRUE);
	
	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadModelType, m_Data.nLoadModel);

	CCMMvldSubItemFRDlg dlg(this);
	dlg.m_nLoadModel = m_Data.nLoadModel;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_nPreLoadModel = m_Data.nLoadModel;
}

void CCMMvldItemFRDlg::OnCmdBtnModify() 
{
	UpdateData(TRUE);

	int iItem = -1;
	iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);

	if(iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_sub_load_case));
		return;
	}
 
	CCMMvldSubItemFRDlg dlg(this);
	dlg.m_nLoadModel = m_Data.nLoadModel;	
	dlg.SetParamData(m_Data.arSubLoadData[iItem], iItem);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_nPreLoadModel = m_Data.nLoadModel;
}

void CCMMvldItemFRDlg::OnCmdBtnDelete() 
{
	int iItem = -1;
	iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);

	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_sub_load_case));
		return;
	}
	m_Data.arSubLoadData.RemoveAt(iItem);

	m_List.DeleteItem(iItem); 

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
	{
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CCMMvldItemFRDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCmdBtnModify();	
	*pResult = 0;
}

void CCMMvldItemFRDlg::OnCmdLoadModelRdo()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadModelType, m_Data.nLoadModel);

	if(m_nPreLoadModel != m_Data.nLoadModel) 
	{
		m_Data.arSubLoadData.RemoveAll();
		m_List.DeleteAllItems();
	}	
}

