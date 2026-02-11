// CMMvhlItemUsrJPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrJPDlg.h"
#include "CMMvhlItemUsrPermitDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrJPDlg dialog

#define COLCOUNT 3

CCMMvhlItemUsrJPDlg::CCMMvhlItemUsrJPDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemUsrJPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemUsrJPDlg)
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();

	m_bModify = FALSE;

	m_nTruckType = 0;

	m_Data.Initialize();
	m_Data.nNumCarriage = 1;

	//
	m_aNum.RemoveAll();
	m_aNum.Add(IDC_CMD_MVHL_JP_VEHICLE_NUM_STC);
	m_aNum.Add(IDC_CMD_MVHL_JP_VEHICLE_NUM_EDT);
	m_aNum.Add(IDC_CMD_MVHL_JP_VEHICLE_NUM_SPIN);

	m_aDD1.RemoveAll();
	m_aDD1.Add(IDC_CMD_USER_MVHL_D1_STC);
	m_aDD1.Add(IDC_CMD_USER_MVHL_D1_EDT);
	m_aDD1.Add(IDC_CMD_USER_MVHL_D1_UNT);

// 	m_aDD2.RemoveAll();
// 	m_aDD2.Add(IDC_CMD_USER_MVHL_D2_STC);
// 	m_aDD2.Add(IDC_CMD_USER_MVHL_D2_EDT);
// 	m_aDD2.Add(IDC_CMD_USER_MVHL_D2_UNT);

	m_aDW1.RemoveAll();
	m_aDW1.Add(IDC_CMD_USER_MVHL_W1_STC);
	m_aDW1.Add(IDC_CMD_USER_MVHL_W1_EDT);
	m_aDW1.Add(IDC_CMD_USER_MVHL_W1_UNT);
}

void CCMMvhlItemUsrJPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemUsrJPDlg)  
	DDX_Radio  (pDX, IDC_CMD_USER_TRK_RDO,            m_nTruckType);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_NAME_EDT,      m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_TYPE_CMB,      m_cmbVehicleType);
	DDX_Control(pDX, IDC_CMD_LIST,				      m_List);
	DDX_Control(pDX, IDC_CMD_MVHL_JP_VEHICLE_NUM_EDT, m_ProcessNumEdt);
	DDX_Control(pDX, IDC_CMD_MVHL_JP_VEHICLE_H_EDT,   m_edtH);
	DDX_Control(pDX, IDC_CMD_MVHL_JP_VEHICLE_H_UNT,   m_untH);

	DDX_Control(pDX, IDC_CMD_USER_VEHI_TRUCK_P_EDT,	  m_edtTrkP);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_TRUCK_D_EDT,	  m_edtTrkD);
													  
	DDX_Control(pDX, IDC_CMD_USER_MVHL_W1_EDT,        m_edtLaneW1);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_W1_UNT,        m_untLaneW1);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_D1_EDT,        m_edtLaneD1);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_D1_UNT,        m_untLaneD1);	
	DDX_Control(pDX, IDC_CMD_USER_MVHL_D2_EDT,        m_edtLaneD2);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_D2_UNT,        m_untLaneD2);	

	DDX_Control(pDX, IDC_CMD_USER_VEHI_PICTURE, m_wndPicture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvhlItemUsrJPDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemUsrJPDlg)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_VEHI_TYPE_CMB, OnCmdMvhlTypeCmb)

	ON_BN_CLICKED(IDC_CMD_USER_TRK_RDO,          OnCmdMvhlLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_USER_TRN_RDO,          OnCmdMvhlLoadTypeRdo)

	ON_BN_CLICKED(IDC_CMD_BTN_ADD,               OnCmdMvhlAddBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_INSERT,            OnCmdMvhlInsBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY,            OnCmdMvhlModBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE,            OnCmdMvhlDelBtn)

	ON_BN_CLICKED(IDC_CMD_APPLY,                 OnCmdMvhlApplyBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST,     OnItemchangedCmdList)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemUsrJPDlg::InitUnit()
{
	m_edtLaneW1.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untLaneW1.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_edtLaneD1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untLaneD1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtLaneD2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untLaneD2.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMMvhlItemUsrJPDlg::InitCombo()
{
	m_cmbVehicleType.ResetContent();

	if (m_nTruckType == 0)
	{
		CDlgUtil::CobxAddItem(m_cmbVehicleType, _LS(IDS_CMD_MVHL_JP_USER_Shinkansen)  , 0);  // 신칸센 하중
		CDlgUtil::CobxAddItem(m_cmbVehicleType, _LS(IDS_CMD_MVHL_JP_USER_Train_engine), 1);  // 전철/내연 동력차 하중
		CDlgUtil::CobxSetCurSelItemData(m_cmbVehicleType, 0);
	}
	else if (m_nTruckType == 1)
	{
		CDlgUtil::CobxAddItem(m_cmbVehicleType, _LS(IDS_CMD_MVHL_JP_USER_Locomotive),   2);  // 기관차 하중
		CDlgUtil::CobxSetCurSelItemData(m_cmbVehicleType, 2);
	}
	else
		ASSERT(0);
};

void CCMMvhlItemUsrJPDlg::AlignControl()
{
// 	CRect rRef;
// 	CRect rToMove;
// 	int nDistX = 0;	
// 	int nDistY = 0;	
}

void CCMMvhlItemUsrJPDlg::ControlsShowHide()
{
	UpdateData(TRUE);
}

void CCMMvhlItemUsrJPDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	if (m_nTruckType == 1)
	{
		m_ProcessNumEdt.SetValue(1); // Num
		CDlgUtil::CtrlEnableDisable(this, m_aNum, FALSE);
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this, m_aNum, TRUE);
	}

	CDlgUtil::CtrlEnableDisable(this, m_aDD1, m_nTruckType == 1);
//	CDlgUtil::CtrlEnableDisable(this, m_aDD2, m_nTruckType == 1);
	CDlgUtil::CtrlEnableDisable(this, m_aDW1, m_nTruckType == 1);

}
void CCMMvhlItemUsrJPDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__No), _LS(IDS_WG_CMD__ADDD__Load), _LS(IDS_WG_CMD__ADDD__Spacing)};
	
	// title에 단위 추가
	CString sLdUnit,sDtUnit;
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointLoad,sLdUnit);
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointDistance,sDtUnit);
	aTitle[1] = aTitle[1] +_T("(")+sLdUnit+_T(")");
	aTitle[2] = aTitle[2] +_T("(")+sDtUnit+_T(")");
	
	int nColWidth[COLCOUNT];
	
	CString title;
	int i;
	LV_COLUMN lvcolumn;
	
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);
	
	// Calculate width for each column
	nColWidth[0] = 28; nColWidth[1] = 70; nColWidth[2] = 80;
	
	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;
		
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvhlItemUsrJPDlg::MakeItemEx()
{
	m_List.DeleteAllItems();
	
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	
	if (nLoadCount == 0) return;
	int i = 0;
	for (i = 0; i < nDistCount; i++)
		InsertItem(i, i+1, m_Data.dPointLoad[i], m_Data.dPointDistance[i]);
	
	if (nLoadCount > nDistCount)
		InsertItem(i, i+1, m_Data.dPointLoad[i], 0.0);
}

BOOL CCMMvhlItemUsrJPDlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
{
	LVITEM lvitem;
	CString str;
	
	lvitem.iItem = nPos;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		if (i == 0) str.Format(_T("%d"), nNo);
		else if (i == 1) str.Format(_T("%g"), dLoad);
		else if (i == 2)
		{
			if (dDist == 0.0) str = _LS(IDS_WG_CMD__ADDD__end);
			else str.Format(_T("%g"), dDist);
		}
		else str = _LS(IDS_WG_CMD__ADDD__Error);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;
		
		if(i == 0) m_List.InsertItem(&lvitem);
		else       m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	
	return TRUE;
}

BOOL CCMMvhlItemUsrJPDlg::DeleteItem(int nPos)
{
	if (nPos != -1) m_List.DeleteItem(nPos);
	return TRUE;
}

BOOL CCMMvhlItemUsrJPDlg::ModifyItem(int nPos, int nNo, double dLoad, double dDist)
{
	int nItem = nPos;
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			if (i == 0) str.Format(_T("%d"), nNo);
			else if (i == 1) str.Format(_T("%g"), dLoad);
			else if (i == 2)
			{
				if (dDist == 0.0) str = _LS(IDS_WG_CMD__ADDD__end);
				else str.Format(_T("%g"), dDist);
			}
			else str = _LS(IDS_WG_CMD__ADDD__Error);
			m_List.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}


void CCMMvhlItemUsrJPDlg::ChangeData()
{
	UpdateData(FALSE);

	ChangeBitMap();
	//AlignControl();
	ControlsShowHide();
	EnableDisableControls();

}

void CCMMvhlItemUsrJPDlg::ChangeBitMap()
{
	CString aBitmapTrain[] = { _T("cmd_mvhl_jp_Train_User.svg") };  // IDB_CMD_MVHL_JP_TRAIN_USER
	CString aBitmapLocomo[] = { _T("cmd_mvhl_jp_Loco_User.svg") }; // IDB_CMD_MVHL_JP_LOCO_USER

	CString strSVG;
	int nBitmap = 0;

	if     (m_nTruckType == 0)  strSVG = aBitmapTrain[nBitmap];
	else if(m_nTruckType == 1)  strSVG = aBitmapLocomo[nBitmap];
	else ASSERT(0);

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + strSVG);

// 	UpdateData(TRUE);
// 
// 	UINT bitmapID = IDB_CMD_MVHL_JP_TRAIN_USER;
// 
// 	if     (m_nLoadType == 0) bitmapID = IDB_CMD_MVHL_JP_TRAIN_USER;
// 	else if(m_nLoadType == 1) bitmapID = IDB_CMD_MVHL_JP_LOCO_USER;
// 	else ASSERT(0);
// 	
// 	if(m_pBitmap != 0) delete m_pBitmap;
// 	CStatic* pImage = (CStatic*)GetDlgItem(IDC_CMD_USER_VEHI_PICTURE);
// 	m_pBitmap = new CBitmap;
// 	m_pBitmap->LoadBitmap(bitmapID);
// 	pImage->SetBitmap(HBITMAP(*m_pBitmap));

}

//m_nTruckType      : 0=wheel axle load  1:EA load
void CCMMvhlItemUsrJPDlg::Data2Dlg()
{
	m_nTruckType = m_Data.nTruckType;
	InitCombo();

	m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);

	CDlgUtil::CobxSetCurSelItemData(m_cmbVehicleType, m_Data.nTrainType);

	m_ProcessNumEdt.SetValue(m_Data.nNumCarriage);
	m_edtH     .SetEditUnit(m_Data.dHeight);
	m_edtLaneW1.SetEditUnit(m_Data.dW1);
	m_edtLaneD1.SetEditUnit(m_Data.dD1);
  //m_edtLaneD2.SetEditUnit(m_Data.dD2);
	
	MakeItemEx();
	
	UpdateData(FALSE);
}

//m_nTruckType       0=wheel axle load  1:EA load
BOOL CCMMvhlItemUsrJPDlg::Dlg2Data()
{	
	UpdateData(TRUE);  
	m_Data.bStandard = FALSE;    

	m_Data.nLoadType = 1; // JP Rail
	m_Data.nTruckType = m_nTruckType;

	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);	

	m_Data.nTrainType = CDlgUtil::CobxGetCurSelItemData(m_cmbVehicleType, m_cmbVehicleType.GetCurSel());

	m_Data.nNumCarriage = m_ProcessNumEdt.GetEditValue();
	m_Data.dHeight      = m_edtH.GetEditValue();
	m_Data.dW1          = m_edtLaneW1.GetEditValue();
	m_Data.dD1          = m_edtLaneD1.GetEditValue();
  //m_Data.dD2          = m_edtLaneD2.GetEditValue(); 무한대로 변경
  

	int i = 0;
	int ip = GetLoadCount();
	int id = GetDistCount();
	if (m_Data.nTruckType == 1)
	{
		if (id != ip - 1)
		{
			AfxMessageBox(_LS(IDS_CMD_MVHL_JP_LAST_SPACING));
			return FALSE;
		}
	}

	return TRUE;
}

int CCMMvhlItemUsrJPDlg::GetLoadCount()
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemUsrJPDlg::GetDistCount()
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}
	return i;
}

BOOL CCMMvhlItemUsrJPDlg::GetLoadAndDistValue(double &dLoad, double &dDist)
{
	if (!CFormulaEdit::GetEditValue(&m_edtTrkP, dLoad))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_load_value_));
		return FALSE;
	}
	if (!CFormulaEdit::GetEditValue(&m_edtTrkD, dDist))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_distance_value_));
		return FALSE;
	}
	
	if (dLoad == 0.0) // Load 0은 허용 안함
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Load_value_must_not_be_ze));
		return FALSE;
	}
	
	return TRUE;
}

BOOL CCMMvhlItemUsrJPDlg::ApplyOrOK()
{
	BOOL bSuccess = FALSE;
	
	if (!Dlg2Data()) return FALSE;

	if (m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else					 bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);

	if (!bSuccess) return FALSE;
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrJPDlg message handlers

BOOL CCMMvhlItemUsrJPDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitUnit();
	//InitCombo(); Data2Dlg 안으로 이동
	SetHeaderTitle();
		
	m_ProcessNumEdt.SetRange(1, SHRT_MAX);

	Data2Dlg();

	AlignControl();
	ControlsShowHide();
	EnableDisableControls();

	ChangeBitMap();

	if (m_Data.VehicleLoadName.IsEmpty())
	{
		CString strVehicleType = _T("");
		m_cmbVehicleType.GetWindowText(strVehicleType);
		m_edtVehicleName.SetWindowText(strVehicleType);
	}

	if(m_bModify)	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemUsrJPDlg::OnOK() 
{
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CCMMvhlItemUsrJPDlg::OnCmdMvhlTypeCmb()
{
	CString strVehicleType = _T("");
	m_cmbVehicleType.GetWindowText(strVehicleType);

	m_edtVehicleName.SetWindowText(strVehicleType);
}

void CCMMvhlItemUsrJPDlg::OnCmdMvhlLoadTypeRdo() 
{
	UpdateData(TRUE);

	InitCombo();
	ChangeBitMap();
	//AlignControl();
	OnCmdMvhlTypeCmb();
	ControlsShowHide();
	EnableDisableControls();
}

void CCMMvhlItemUsrJPDlg::OnCmdMvhlAddBtn() 
{
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	
	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;
	
	if (nLoadCount >= D_MVHL_NUMLOAD) // MAX 갯수 초과시
	{
		CString csErr;
		csErr.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_exceeded_l), D_MVHL_NUMLOAD);
		AfxMessageBox(csErr);
		return;
	}
	if (nLoadCount > nDistCount) // Last 입력된 경우 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Delete_last_item_and_try_));
		return;
	}
	
	if (dDist == 0.0) // Last
	{
		m_Data.dPointLoad[nLoadCount] = dLoad;
	}
	else
	{
		m_Data.dPointLoad[nLoadCount] = dLoad;
		m_Data.dPointDistance[nLoadCount] = dDist;
	}
	
	int no = nLoadCount+1;
	InsertItem(nLoadCount, no, dLoad, dDist);
	
	int nCount = m_List.GetItemCount();
	m_List.SetItemState(nCount-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUsrJPDlg::OnCmdMvhlInsBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_inser));
		return;
	}
	
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	
	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;
	
	if (nLoadCount >= D_MVHL_NUMLOAD) // MAX 갯수 초과시
	{
		CString csErr;
		csErr.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_exceeded_l), D_MVHL_NUMLOAD);
		AfxMessageBox(csErr);
		return;
	}
	
	if (dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}
	
	// 데이타 이동하기
	for (int i = D_MVHL_NUMLOAD-1; i > iItem; i--)
	{
		m_Data.dPointLoad[i] = m_Data.dPointLoad[i-1];
		m_Data.dPointDistance[i] = m_Data.dPointDistance[i-1];
	}
	
	// 데이타 삽입한다.
	m_Data.dPointLoad[iItem] = dLoad;
	m_Data.dPointDistance[iItem] = dDist;
	
	// List에 Item 삽입
	InsertItem(iItem, iItem+1, dLoad, dDist);
	
	// List 번호 변경
	int nCount = m_List.GetItemCount();
	CString csVal;
	for (int i = iItem+1; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_List.SetItemText(i, 0, csVal);
	}
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUsrJPDlg::OnCmdMvhlModBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_modif));
		return;
	}
	
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	
	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;
	
	if (iItem != nLoadCount-1 && dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}
	
	m_Data.dPointLoad[iItem] = dLoad;
	m_Data.dPointDistance[iItem] = dDist;
	
	ModifyItem(iItem, iItem+1, dLoad, dDist);
}

void CCMMvhlItemUsrJPDlg::OnCmdMvhlDelBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}
	
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	int i = iItem;
	for (i = iItem; i < D_MVHL_NUMLOAD-1; i++)
	{
		m_Data.dPointLoad[i] = m_Data.dPointLoad[i+1];
		m_Data.dPointDistance[i] = m_Data.dPointDistance[i+1];
	}
	m_Data.dPointLoad[i] = 0.0;
	m_Data.dPointDistance[i] = 0.0;
	
	m_List.DeleteItem(iItem);
	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	
	// List 번호 변경
	CString csVal;
	for (int i = iItem; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_List.SetItemText(i, 0, csVal);
	}
}


void CCMMvhlItemUsrJPDlg::OnCmdMvhlApplyBtn() 
{
	ApplyOrOK();
}

void CCMMvhlItemUsrJPDlg::OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	CString csVal;
	if (iItem == nLoadCount-1 && nLoadCount != nDistCount)  // fixed, last
	{
		csVal.Format(_T("%g"), m_Data.dPointLoad[iItem]);
		m_edtTrkP.SetWindowText(csVal);
		m_edtTrkD.SetWindowText(_T(""));
	}
	else
	{
		csVal.Format(_T("%g"), m_Data.dPointLoad[iItem]);
		m_edtTrkP.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.dPointDistance[iItem]);
		m_edtTrkD.SetWindowText(csVal);
	}
	
	*pResult = 0;
}
