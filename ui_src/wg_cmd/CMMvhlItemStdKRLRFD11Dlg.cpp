
#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdKRLRFD11Dlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MvhlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdKRLRFD11Dlg dialog

#define COLCOUNT 3

CCMMvhlItemStdKRLRFD11Dlg::CCMMvhlItemStdKRLRFD11Dlg(CWnd* pParent /*=NULL*/)
: CDialogMove(CCMMvhlItemStdKRLRFD11Dlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_bModify = FALSE;
	
	m_aDgnLaneType.RemoveAll();
	m_aDgnLaneType.Add(IDC_CMD_MVHL_VEHI_DGN75_RDO);
	m_aDgnLaneType.Add(IDC_CMD_MVHL_VEHI_DGN25_RDO);

	m_aLengthLane.RemoveAll();
	m_aLengthLane.Add(IDC_CMD_MVHL_CODE_LENGTH_GRP);
	m_aLengthLane.Add(IDC_CMD_MVHL_VEHI_1ST_MODEL_RDO);
	m_aLengthLane.Add(IDC_CMD_MVHL_VEHI_1ST_MODEL_EDT);
	m_aLengthLane.Add(IDC_CMD_MVHL_VEHI_1ST_MODEL_UNT);
	m_aLengthLane.Add(IDC_CMD_MVHL_VEHI_2ND_MODEL_RDO);
	
	m_aEtc.RemoveAll();
	m_aEtc.Add(IDC_CMD_MVHL_PICTURE);
	m_aEtc.Add(IDC_CMD_MVHL_PTLIST);
	m_aEtc.Add(IDOK);
	m_aEtc.Add(IDCANCEL);
	m_aEtc.Add(IDC_CMD_APPLY);

	m_aCtrlPatch.RemoveAll();
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_CHK);

	m_bConvertDistLoad = FALSE;
	m_nDgnLaneType = 0;
	m_nLengthLane = 0;
}

CCMMvhlItemStdKRLRFD11Dlg::~CCMMvhlItemStdKRLRFD11Dlg()
{
}

void CCMMvhlItemStdKRLRFD11Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_MVHL_CODE_CMB,               m_cmbCodeName);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_NAME_EDT,          m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_TYPE_CMB,          m_cmbVehicleType);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_ALLOW_EDT,         m_edtDynaAllow);
	DDX_Check  (pDX, IDC_CMD_MVHL_CONVERT_DIST_LOAD_CHK,  m_bConvertDistLoad);
	DDX_Radio  (pDX, IDC_CMD_MVHL_VEHI_DGN75_RDO,         m_nDgnLaneType);
	DDX_Radio  (pDX, IDC_CMD_MVHL_VEHI_1ST_MODEL_RDO,     m_nLengthLane);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_1ST_MODEL_EDT,     m_edt1stModel);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_1ST_MODEL_UNT,     m_unt1stModel);
	DDX_Control(pDX, IDC_CMD_MVHL_PTLIST,                 m_List);	
	DDX_Control(pDX, IDC_CMD_MVHL_CONVERT_DIST_LOAD_GRID, m_Grid);
	DDX_Control(pDX, IDC_CMD_MVHL_PICTURE, m_wndPicture);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_CHK  , m_chkPatch);

}

BEGIN_MESSAGE_MAP(CCMMvhlItemStdKRLRFD11Dlg, CDialogMove)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_CODE_CMB,                  OnCmdMvhlCodeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_VEHI_TYPE_CMB,             OnCmdMvhlTypeCmb)
	ON_BN_CLICKED   (IDC_CMD_MVHL_CONVERT_DIST_LOAD_CHK,     OnCmdMvhlConvertDistLoadChk)
	ON_BN_CLICKED   (IDC_CMD_MVHL_VEHI_DGN75_RDO,            OnCmdMvhlDgnLaneTypeRdo)
	ON_BN_CLICKED   (IDC_CMD_MVHL_VEHI_DGN25_RDO,            OnCmdMvhlDgnLaneTypeRdo)
	ON_BN_CLICKED   (IDC_CMD_MVHL_VEHI_1ST_MODEL_RDO,        OnCmdMvhlDgnLengthLaneRdo)
	ON_BN_CLICKED   (IDC_CMD_MVHL_VEHI_2ND_MODEL_RDO,        OnCmdMvhlDgnLengthLaneRdo)
	ON_BN_CLICKED   (IDC_CMD_MVHL_PATCH_CHK,                 OnChkPatch)
	ON_BN_CLICKED   (IDC_CMD_APPLY,                          OnCmdApply)
END_MESSAGE_MAP()


void CCMMvhlItemStdKRLRFD11Dlg::InitCombo()
{
	int nCodeSeq = D_MVHL_KSCE_LSD15;
	CDlgUtil::CobxAddItem(m_cmbCodeName, CVehlDB::GetTypeNameAr()[nCodeSeq], nCodeSeq);
}

void CCMMvhlItemStdKRLRFD11Dlg::InitControls()
{
	m_Grid.InitGrid();
}

void CCMMvhlItemStdKRLRFD11Dlg::AlignControl()
{
	UpdateData(TRUE);

	CString strVehicleType = _T("");
	m_cmbVehicleType.GetWindowText(strVehicleType);

	CRect rRef1, rRef2;
	CRect rToMove;
	int nDistY = 0;	

	GetDlgItem(IDC_CMD_MVHL_VEHI_DGN75_RDO)->GetWindowRect(rRef1);
	GetDlgItem(IDC_CMD_MVHL_CODE_LENGTH_GRP)->GetWindowRect(rRef2);
	GetDlgItem(IDC_CMD_MVHL_PICTURE)->GetWindowRect(rToMove);

	if(strVehicleType == _T("KL-510TRK") || strVehicleType == _T("KL-510FTG"))
	{
		nDistY = rRef1.top - rToMove.top;
	}
	else
	{
		nDistY = rRef2.bottom + 10 - rToMove.top;
	}

	CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlPatch, nDistY);
	
	CArray<UINT, UINT> aControls; 
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_MVHL_CONVERT_DIST_LOAD_GRID);

	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef1);
	GetDlgItem(IDC_CMD_MVHL_CONVERT_DIST_LOAD_GRID)->GetWindowRect(rToMove);
	nDistY = rRef1.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// Group Box 크기
	CRect rectTarget;
	CRect rect;
	CRect rectShort;

	GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->GetWindowRect(rectShort);

	GetDlgItem(IDC_CMD_MVHL_VEHI_NAME_EDT)->GetWindowRect(rectTarget);
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rect);
	nDistY = rect.bottom - rectTarget.top + globalUtils.ScaleByDPI(30);

	rectShort.bottom = rectShort.top + nDistY;

	ScreenToClient(rectShort);
	GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->MoveWindow(rectShort);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

void CCMMvhlItemStdKRLRFD11Dlg::ControlsShowHide()
{
	UpdateData(TRUE);
	
	CString strVehicleType = _T("");
	m_cmbVehicleType.GetWindowText(strVehicleType);

	if(strVehicleType == _T("KL-510TRK") || strVehicleType == _T("KL-510FTG"))
	{
		CDlgUtil::CtrlShowHide(this, m_aDgnLaneType, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aLengthLane, FALSE);

		CDlgUtil::CtrlShowHide(this, m_aCtrlPatch, !m_bConvertDistLoad);
	}
	else 
	{
		CDlgUtil::CtrlShowHide(this, m_aDgnLaneType, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aLengthLane, TRUE);

		CDlgUtil::CtrlShowHide(this, m_aCtrlPatch, !m_bConvertDistLoad);
	}

	CArray<UINT, UINT> aControls; 
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_MVHL_CONVERT_DIST_LOAD_GRID);
	CDlgUtil::CtrlShowHide(this, aControls, m_bConvertDistLoad);

	aControls.RemoveAll();
	aControls.Add(IDC_CMD_MVHL_PTLIST);
	CDlgUtil::CtrlShowHide(this, aControls, !m_bConvertDistLoad);
}

void CCMMvhlItemStdKRLRFD11Dlg::SetHeaderTitle()
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
	nColWidth[0] = 28; nColWidth[1] = 70; nColWidth[2] = 90;
	
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

BOOL CCMMvhlItemStdKRLRFD11Dlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
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
		
		if(i == 0)
			m_List.InsertItem(&lvitem);
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	
	return TRUE;
}

void CCMMvhlItemStdKRLRFD11Dlg::MakeItemEx()
{
	m_List.DeleteAllItems();    
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	
	if (nLoadCount > 0)
	{
		int i = 0;
		for (i = 0; i < nDistCount; i++)
			InsertItem(i, i+1, m_Data.dPointLoad[i], m_Data.dPointDistance[i]);
		
		if (nLoadCount > nDistCount)
			InsertItem(i, i+1, m_Data.dPointLoad[i], 0.0);
	}
	
	UpdateData(FALSE);
}

int CCMMvhlItemStdKRLRFD11Dlg::GetLoadCount()
{
	int i = 0;
	for(i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemStdKRLRFD11Dlg::GetDistCount()
{
	int i = 0;
	for(i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}  
	return i;
}

void CCMMvhlItemStdKRLRFD11Dlg::ChangeBitmap(int nBitmap)
{
	CString aBitmapID1[] = { _T("cmd_mvhl_118.svg"), _T("cmd_mvhl_118.svg"), _T("cmd_mvhl_120.svg"), _T("cmd_mvhl_197.svg") };
	CString aBitmapID2[] = { _T("cmd_mvhl_202.svg"), _T("cmd_mvhl_202.svg"), _T("cmd_mvhl_203.svg"), _T("cmd_mvhl_204.svg") };

	CString strSVG;  
	if(m_bConvertDistLoad) strSVG = aBitmapID2[nBitmap];
	else                   strSVG = aBitmapID1[nBitmap];

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + strSVG);
}

void CCMMvhlItemStdKRLRFD11Dlg::Data2Dlg()
{
	m_nStndCode = m_Data.nStandardCode;
	if(m_Data.VehicleLoadName.IsEmpty())
		m_edtVehicleName.SetWindowText(m_Data.VehicleTypeName);
	else
		m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);
	
	int sIndex;
	CDlgUtil::CobxSetCurSelItemData(m_cmbCodeName, m_nStndCode);
	GetIndex(m_Data.VehicleTypeName, &sIndex);
	m_cmbVehicleType.SetCurSel(sIndex);
		
	m_bConvertDistLoad = m_Data.bConvertDistLoad;
	m_edtDynaAllow.SetEditUnit(m_Data.dDynLoadAllowance);
	m_nDgnLaneType = m_Data.nLaneLoadType;

	m_nLengthLane = m_Data.nLengthLane;
	m_edt1stModel.SetEditUnit(m_Data.dLengthLaneUser);

	int nBitmap = sIndex;
	if(sIndex == 2 && m_Data.nLengthLane == 1)
	{
		nBitmap = 3;
	}

	ChangeBitmap(nBitmap);
		
	T_MVHL_PATCH_LOAD PatchLoadDB;
	CVehlDB db(m_pDoc);  
	if (db.GetStandardVehicleLoadValue(m_Data, &PatchLoadDB))	MakeItemEx();

	//////////////////////////////////////////////////////////////////////////
	if (!m_bModify) // new
	{
		m_Data.PatchLoad = PatchLoadDB;
	}
	else
	{
		if (m_Data.PatchLoad.aPatchLoad.GetSize() == 0)
		{
			m_Data.PatchLoad = PatchLoadDB;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	if (m_Data.PatchLoad.aPatchLoad.GetSize() > 0)
	{
		m_chkPatch.SetCheck(m_Data.PatchLoad.bPatchLoad);
	}
	OnChkPatch();

	m_Grid.SetDataSource(&m_Data);

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdKRLRFD11Dlg::Dlg2Data()
{
	UpdateData(TRUE);
	
	m_Data.Initialize();  
	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);   
	m_Data.bStandard = TRUE;
	m_cmbVehicleType.GetWindowText(m_Data.VehicleTypeName);

	m_Data.nStandardCode = m_nStndCode;

	m_Data.bConvertDistLoad = m_bConvertDistLoad;
	m_Data.dDynLoadAllowance = m_edtDynaAllow.GetEditValue();
	m_Data.nLaneLoadType = m_nDgnLaneType;

	m_Data.nLengthLane = m_nLengthLane;
	m_Data.dLengthLaneUser = m_edt1stModel.GetEditValue();

	T_MVHL_PATCH_LOAD PatchLoadDB;
	CVehlDB db(m_pDoc);
	if(!db.GetStandardVehicleLoadValue(m_Data, &PatchLoadDB))  return FALSE;

	m_Data.PatchLoad = PatchLoadDB;
	m_Data.PatchLoad.bPatchLoad = m_chkPatch.GetCheck();

	m_Grid.SaveData();
	
	return TRUE;
}

BOOL CCMMvhlItemStdKRLRFD11Dlg::ApplyOrOK()
{
	if(!Dlg2Data()) return FALSE;

	BOOL bSuccess;
	if(m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else          bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);
	if(!bSuccess) return FALSE;
	
	return TRUE;
}

BOOL CCMMvhlItemStdKRLRFD11Dlg::ChangeData()
{
	UpdateData(TRUE);

	m_Data.Initialize();
	m_Data.bStandard = TRUE;
	m_cmbVehicleType.GetWindowText(m_Data.VehicleTypeName);
	m_Data.nStandardCode = m_nStndCode;
	
	int sIndex;
	CDlgUtil::CobxSetCurSelItemData(m_cmbCodeName, m_nStndCode);
	GetIndex(m_Data.VehicleTypeName, &sIndex);
	m_cmbVehicleType.SetCurSel(sIndex);

	m_Data.dDynLoadAllowance = m_edtDynaAllow.GetEditValue();
	m_Data.bConvertDistLoad = m_bConvertDistLoad;
	m_Data.nLaneLoadType = m_nDgnLaneType;

	m_Data.nLengthLane = m_nLengthLane;
	m_Data.dLengthLaneUser = m_edt1stModel.GetEditValue();

	int nBitmap = sIndex;
	if(sIndex == 2 && m_nLengthLane == 1)
	{
		nBitmap = 3;
	}

	ChangeBitmap(nBitmap);
	
	T_MVHL_PATCH_LOAD PatchLoadDB;
	CVehlDB db(m_pDoc);
	if(db.GetStandardVehicleLoadValue(m_Data, &PatchLoadDB)) MakeItemEx();

	if (!m_Data.PatchLoad.bPatchLoad)
		m_Data.PatchLoad = PatchLoadDB;

	m_Grid.SaveData();

	if(m_Data.VehicleTypeName == _T("KL-510FTG")) m_Data.dDynLoadAllowance = 15.0;
	else                                      m_Data.dDynLoadAllowance = 25.0;

	Data2Dlg();

	m_edtVehicleName.SetWindowText(m_Data.VehicleTypeName);
	
	return TRUE;
}

void CCMMvhlItemStdKRLRFD11Dlg::SetLoadTypeList(int nIndex)
{
	if (nIndex < 0) return;
	
	m_cmbVehicleType.ResetContent();
	int i = 0;
	while (i < D_MAX_COUNT && CVehlDB::GetTypeListAr()[nIndex][i] != _T(""))
	{
		m_cmbVehicleType.AddString(CVehlDB::GetTypeListAr()[nIndex][i++]);
	}
	m_cmbVehicleType.SetCurSel(0);
}

void CCMMvhlItemStdKRLRFD11Dlg::GetIndex(CString &csName, int *sitem)
{  
	for (int i = 0; i < D_MAX_COUNT; i++)
	{
		if(CVehlDB::GetTypeListAr()[m_Data.nStandardCode][i] == csName)
		{
			if(sitem) *sitem = i;
			return ;
		}
	}
	
	ASSERT(0);
	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_vehicle_load_name));
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdKRLRFD11Dlg message handlers

BOOL CCMMvhlItemStdKRLRFD11Dlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	if(!m_bModify)
	{
		m_Data.Initialize();
		m_Data.bStandard = TRUE;    
		m_Data.VehicleTypeName = _T("KL-510TRK");
		m_nStndCode = D_MVHL_KSCE_LSD15;
		m_Data.nStandardCode = m_nStndCode;
		m_Data.dDynLoadAllowance = 25.0;
		m_Data.dLengthLaneUser = 0.;
	}

	GetIndex(m_Data.VehicleTypeName);
	int nIndex = m_Data.nStandardCode;
	if (nIndex < 0) return TRUE;
	
	m_edt1stModel.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unt1stModel.SetUnitType(D_UNITSYS_BASE_LENGTH);

	InitCombo();
	InitControls();
	SetHeaderTitle();
	SetLoadTypeList(nIndex);
	
	Data2Dlg();
	
	AlignControl();
	ControlsShowHide();
	EnableDisableCtrl();

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_mvhl_118.svg"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemStdKRLRFD11Dlg::OnOK() 
{
	if (!ApplyOrOK()) return;	
	CDialogMove::OnOK();
}

void CCMMvhlItemStdKRLRFD11Dlg::OnCmdMvhlCodeCmb() 
{
	int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());
	SetLoadTypeList(nCodeType);

	int nIndex = m_cmbCodeName.GetCurSel();
	m_nStndCode = m_cmbCodeName.GetItemData(nIndex);

	ChangeData();

	AlignControl();
	ControlsShowHide();
}

void CCMMvhlItemStdKRLRFD11Dlg::OnCmdMvhlTypeCmb() 
{
	ChangeData();  

	AlignControl();
	ControlsShowHide();
}

void CCMMvhlItemStdKRLRFD11Dlg::OnCmdMvhlConvertDistLoadChk() 
{
	ChangeData();
	ControlsShowHide();
}

void CCMMvhlItemStdKRLRFD11Dlg::OnCmdMvhlDgnLaneTypeRdo() 
{
	ChangeData();  
}

void CCMMvhlItemStdKRLRFD11Dlg::OnCmdMvhlDgnLengthLaneRdo() 
{
	UpdateData(TRUE);

	EnableDisableCtrl();
	ChangeData();  
}

void CCMMvhlItemStdKRLRFD11Dlg::EnableDisableCtrl() 
{
	CArray<UINT, UINT> a1St;
	a1St.Add(IDC_CMD_MVHL_VEHI_1ST_MODEL_EDT);
	a1St.Add(IDC_CMD_MVHL_VEHI_1ST_MODEL_UNT);
	CDlgUtil::CtrlEnableDisable(this, a1St, m_nLengthLane==0);
}

void CCMMvhlItemStdKRLRFD11Dlg::OnChkPatch()
{
	BOOL bPatchLoad = m_chkPatch.GetCheck();
}

void CCMMvhlItemStdKRLRFD11Dlg::OnCmdApply() 
{	
	ApplyOrOK();
}