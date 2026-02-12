// CMMvhlItemUsrBrazilDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrBrazilDlg.h"
#include "CMMvhlItemUsrPermitDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_db\VehlDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMMvhlItemUsrBrazilDlg dialog

#define COLCOUNT 3

CMMvhlItemUsrBrazilDlg::CMMvhlItemUsrBrazilDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CMMvhlItemUsrBrazilDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_bModify = FALSE;

	m_nLoadType = 0;

	m_Data.Initialize();

	m_bPermitLoadCall = FALSE;

	m_aTrain.RemoveAll();
	m_aTrain.Add(IDC_CMD_USER_MVHL_W1_STC);
	m_aTrain.Add(IDC_CMD_USER_MVHL_W1_EDT);
	m_aTrain.Add(IDC_CMD_USER_MVHL_W1_UNT);
	m_aTrain.Add(IDC_CMD_USER_MVHL_W2_STC);
	m_aTrain.Add(IDC_CMD_USER_MVHL_W2_EDT);
	m_aTrain.Add(IDC_CMD_USER_MVHL_W2_UNT);
	m_aTrain.Add(IDC_CMD_USER_MVHL_D1_STC);
	m_aTrain.Add(IDC_CMD_USER_MVHL_D1_EDT);
	m_aTrain.Add(IDC_CMD_USER_MVHL_D1_UNT);
	m_aTrain.Add(IDC_CMD_USER_MVHL_D2_STC);
	m_aTrain.Add(IDC_CMD_USER_MVHL_D2_EDT);
	m_aTrain.Add(IDC_CMD_USER_MVHL_D2_UNT);

	m_aDist.RemoveAll();
	m_aDist.Add(IDC_CMD_USER_MVHL_D1_STC);
	m_aDist.Add(IDC_CMD_USER_MVHL_D1_EDT);
	m_aDist.Add(IDC_CMD_USER_MVHL_D1_UNT);
	m_aDist.Add(IDC_CMD_USER_MVHL_D2_STC);
	m_aDist.Add(IDC_CMD_USER_MVHL_D2_EDT);
	m_aDist.Add(IDC_CMD_USER_MVHL_D2_UNT);

	m_aRoadWay.RemoveAll();
	m_aRoadWay.Add(IDC_CMD_USER_MVHL_W_STC);
	m_aRoadWay.Add(IDC_CMD_USER_MVHL_W_EDT);
	m_aRoadWay.Add(IDC_CMD_USER_MVHL_W_UNT);
	m_aRoadWay.Add(IDC_CMD_USER_MVHL_D1_STC);
	m_aRoadWay.Add(IDC_CMD_USER_MVHL_D1_EDT);
	m_aRoadWay.Add(IDC_CMD_USER_MVHL_D1_UNT);
	m_aRoadWay.Add(IDC_CMD_USER_MVHL_D2_STC);
	m_aRoadWay.Add(IDC_CMD_USER_MVHL_D2_EDT);
	m_aRoadWay.Add(IDC_CMD_USER_MVHL_D2_UNT);

	m_aEtc.RemoveAll();
	m_aEtc.Add(IDOK);
	m_aEtc.Add(IDCANCEL);
	m_aEtc.Add(IDC_CMD_APPLY);

	m_aCtrlPatch.RemoveAll();
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_CHK);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_STC);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_EDT);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_UNT);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_STC);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_EDT);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_UNT);
}

void CMMvhlItemUsrBrazilDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMMvhlItemUsrBrazilDlg)  
	DDX_Radio(pDX, IDC_CMD_USER_TRK_RDO, m_nLoadType);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_NAME_EDT, m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_TRUCK_P_EDT, m_edtTrkP);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_TRUCK_D_EDT, m_edtTrkD);
	DDX_Control(pDX, IDC_CMD_LIST, m_List);

	DDX_Control(pDX, IDC_CMD_USER_MVHL_W_EDT, m_edtW);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_W_UNT, m_untW);

	DDX_Control(pDX, IDC_CMD_USER_MVHL_W1_EDT, m_edtW1);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_W1_UNT, m_untW1);

	DDX_Control(pDX, IDC_CMD_USER_MVHL_W2_EDT, m_edtW2);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_W2_UNT, m_untW2);

	DDX_Control(pDX, IDC_CMD_USER_MVHL_D1_EDT, m_edtD1);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_D1_UNT, m_untD1);

	DDX_Control(pDX, IDC_CMD_USER_MVHL_D2_EDT, m_edtD2);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_D2_UNT, m_untD2);

	DDX_Control(pDX, IDC_CMD_USER_VEHI_PICTURE, m_wndPicture);

	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_CHK, m_chkPatch);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_W_EDT, m_edtPatchW);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_W_UNT, m_untPatchW);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_L_EDT, m_edtPatchL);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_L_UNT, m_untPatchL);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMMvhlItemUsrBrazilDlg, CDialogMove)
	//{{AFX_MSG_MAP(CMMvhlItemUsrBrazilDlg)
	ON_BN_CLICKED(IDC_CMD_USER_TRK_RDO, OnCmdMvhlLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_USER_VEHI_TRAIN_RDO, OnCmdMvhlLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdMvhlAddBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_INSERT, OnCmdMvhlInsBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdMvhlModBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdMvhlDelBtn)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdMvhlApplyBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST, OnItemchangedCmdList)
	ON_BN_CLICKED(IDC_CMD_MVHL_PATCH_CHK, OnChkPatch)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CMMvhlItemUsrBrazilDlg::InitUnit()
{

	m_edtW.SetUnitType(D_UNITSYS_BASE_PRESSURE);
	m_untW.SetUnitType(D_UNITSYS_BASE_PRESSURE);

	m_edtW1.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untW1.SetUnitType(D_UNITSYS_BASE_UNITFORCE);

	m_edtW2.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untW2.SetUnitType(D_UNITSYS_BASE_UNITFORCE);

	m_edtD1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untD1.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtD2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untD2.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtPatchW.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untPatchW.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtPatchL.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untPatchL.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CMMvhlItemUsrBrazilDlg::ControlsShowHide()
{
	UpdateData(TRUE);

	CArray<UINT, UINT> aControls; aControls.RemoveAll();

	if (m_nLoadType == 0) // Truck
	{
		CDlgUtil::CtrlShowHide(this, m_aRoadWay, TRUE);
		aControls.Add(IDC_CMD_USER_MVHL_W1_STC);
		aControls.Add(IDC_CMD_USER_MVHL_W1_EDT);
		aControls.Add(IDC_CMD_USER_MVHL_W1_UNT);
		aControls.Add(IDC_CMD_USER_MVHL_W2_STC);
		aControls.Add(IDC_CMD_USER_MVHL_W2_EDT);
		aControls.Add(IDC_CMD_USER_MVHL_W2_UNT);

		CDlgUtil::CtrlShowHide(this, m_aCtrlPatch, TRUE);
	}
	else // Train
	{
		CDlgUtil::CtrlShowHide(this, m_aTrain, TRUE);
		aControls.Add(IDC_CMD_USER_MVHL_W_STC);
		aControls.Add(IDC_CMD_USER_MVHL_W_EDT);
		aControls.Add(IDC_CMD_USER_MVHL_W_UNT);

		CDlgUtil::CtrlShowHide(this, m_aCtrlPatch, TRUE);
	}

	CDlgUtil::CtrlShowHide(this, aControls, FALSE);
}

void CMMvhlItemUsrBrazilDlg::AlignControl()
{
	UpdateData(TRUE);

	CRect rRef;
	CRect rToMove;
	GetDlgItem(IDC_CMD_USER_MVHL_W_EDT)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_USER_MVHL_W1_EDT)->GetWindowRect(rToMove);

	int nDistX = rRef.left - rToMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aTrain, nDistX);

	int nDistY;

	if (m_nLoadType == 0) // Truck
	{
		GetDlgItem(IDC_CMD_USER_MVHL_W_EDT)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_USER_MVHL_D1_EDT)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aDist, nDistY);

		GetDlgItem(IDC_CMD_MVHL_PATCH_L_EDT)->GetWindowRect(rRef);
	}
	else // Train
	{
		GetDlgItem(IDC_CMD_USER_MVHL_W2_EDT)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_USER_MVHL_D1_EDT)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aDist, nDistY);

		GetDlgItem(IDC_CMD_MVHL_PATCH_L_EDT)->GetWindowRect(rRef);
	}


	GetDlgItem(IDC_CMD_USER_VEHI_GRP)->GetWindowRect(rToMove);
	rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	ScreenToClient(rToMove);
	GetDlgItem(IDC_CMD_USER_VEHI_GRP)->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rToMove.Height(), SWP_SHOWWINDOW);


	GetDlgItem(IDC_CMD_USER_VEHI_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);

	// resize self size
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rRef);
	GetWindowRect(rToMove);
	rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(rToMove);

	Invalidate();
}

void CMMvhlItemUsrBrazilDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_WG_CMD__ADDD__No), _LS(IDS_WG_CMD__ADDD__Load), _LS(IDS_WG_CMD__ADDD__Spacing) };

	// title에 단위 추가
	CString sLdUnit, sDtUnit;
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointLoad, sLdUnit);
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointDistance, sDtUnit);
	aTitle[1] = aTitle[1] + _T("(") + sLdUnit + _T(")");
	aTitle[2] = aTitle[2] + _T("(") + sDtUnit + _T(")");

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
	for (i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CMMvhlItemUsrBrazilDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();

	if (nLoadCount == 0) return;
	int i = 0;
	for (i = 0; i < nDistCount; i++)
		InsertItem(i, i + 1, m_Data.dPointLoad[i], m_Data.dPointDistance[i]);

	if (nLoadCount > nDistCount)
		InsertItem(i, i + 1, m_Data.dPointLoad[i], 0.0);
}

BOOL CMMvhlItemUsrBrazilDlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
{
	LVITEM lvitem;
	CString str;

	lvitem.iItem = nPos;
	for (int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem = i;
		if (i == 0) str.Format(_T("%d"), nNo);
		else if (i == 1) str.Format(_T("%g"), dLoad);
		else if (i == 2)
		{
			if (dDist == 0.0) str = _LS(IDS_WG_CMD__ADDD__end);
			else str.Format(_T("%g"), dDist);
		}
		else str = _LS(IDS_WG_CMD__ADDD__Error);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		if (i == 0) m_List.InsertItem(&lvitem);
		else       m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CMMvhlItemUsrBrazilDlg::DeleteItem(int nPos)
{
	if (nPos != -1) m_List.DeleteItem(nPos);
	return TRUE;
}

BOOL CMMvhlItemUsrBrazilDlg::ModifyItem(int nPos, int nNo, double dLoad, double dDist)
{
	int nItem = nPos;
	if (nItem != -1)
	{
		CString str;
		for (int i = 0; i < COLCOUNT; i++)
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


void CMMvhlItemUsrBrazilDlg::ChangeData()
{
	UpdateData(TRUE);

	ChangeBitMap();
	AlignControl();
	ControlsShowHide();
}

void CMMvhlItemUsrBrazilDlg::ChangeBitMap()
{
	UpdateData(TRUE);

	CString strSVG = _T("cmd_mvhc_NBR7188.svg");
	if (m_nLoadType == 0)
	{
		strSVG = _T("cmd_mvhc_NBR7188.svg");
	}
	else
	{
		strSVG = _T("cmd_mvhc_NBR7189.svg");
	}

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + strSVG);
}

void CMMvhlItemUsrBrazilDlg::Data2Dlg()
{
	m_nLoadType = m_Data.nLoadType;
	m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);

	if (m_nLoadType == 0) // Truck
	{
		m_edtW.SetEditUnit(m_Data.dPressureLoad);
		m_edtD1.SetEditUnit(m_Data.dTrainD1);
		m_edtD2.SetEditUnit(m_Data.dTrainD2);

		if (m_Data.PatchLoad.aPatchLoad.GetSize() > 0)
		{
			m_chkPatch.SetCheck(m_Data.PatchLoad.bPatchLoad);
			m_edtPatchW.SetEditUnit(m_Data.PatchLoad.aPatchLoad[0].dWidth);
			m_edtPatchL.SetEditUnit(m_Data.PatchLoad.aPatchLoad[0].dLength);
		}
		else
		{
			m_edtPatchW.SetEditUnit(0.);
			m_edtPatchL.SetEditUnit(0.);
		}
	}
	else // Train
	{
		m_edtW1.SetEditUnit(m_Data.dTrainW1);
		m_edtW2.SetEditUnit(m_Data.dTrainW2);
		m_edtD1.SetEditUnit(m_Data.dTrainD1);
		m_edtD2.SetEditUnit(m_Data.dTrainD2);

		if (m_Data.PatchLoad.aPatchLoad.GetSize() > 0)
		{
			m_chkPatch.SetCheck(m_Data.PatchLoad.bPatchLoad);
			m_edtPatchW.SetEditUnit(m_Data.PatchLoad.aPatchLoad[0].dWidth);
			m_edtPatchL.SetEditUnit(m_Data.PatchLoad.aPatchLoad[0].dLength);
		}
		else
		{
			m_edtPatchW.SetEditUnit(0.);
			m_edtPatchL.SetEditUnit(0.);
		}
	}

	OnChkPatch();

	MakeItemEx();

	UpdateData(FALSE);
}

void CMMvhlItemUsrBrazilDlg::OnChkPatch()
{
	BOOL bPatchLoad=FALSE;

	bPatchLoad = m_chkPatch.GetCheck();

	GetDlgItem(IDC_CMD_MVHL_PATCH_W_STC)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_W_EDT)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_W_UNT)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_STC)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_EDT)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_UNT)->EnableWindow(bPatchLoad);
}

//m_Data.nLoadType : 0=truck  1:Train
BOOL CMMvhlItemUsrBrazilDlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_Data.bStandard = FALSE;

	m_Data.nLoadType = m_nLoadType;

	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);

	if (m_nLoadType == 0) // Truck
	{
		m_Data.nStandardCode = D_MVHL_NBR7188_ROADWAY;
		m_Data.dPressureLoad = m_edtW.GetEditValue();
		m_Data.dTrainD1 = m_edtD1.GetEditValue();
		m_Data.dTrainD2 = m_edtD2.GetEditValue();

		m_Data.PatchLoad.bPatchLoad = m_chkPatch.GetCheck();
		if (m_Data.PatchLoad.bPatchLoad)
		{
			T_MVHL_PATCH_ITEM Patch;
			Patch.dWidth = m_edtPatchW.GetEditValue();
			Patch.dLength = m_edtPatchL.GetEditValue();
			m_Data.PatchLoad.aPatchLoad.RemoveAll();
			m_Data.PatchLoad.aPatchLoad.Add(Patch);
		}
	}
	else // Train
	{
		m_Data.nStandardCode = D_MVHL_NBR7189_RAILWAY;
		m_Data.dTrainW1 = m_edtW1.GetEditValue();
		m_Data.dTrainW2 = m_edtW2.GetEditValue();
		m_Data.dTrainD1 = m_edtD1.GetEditValue();
		m_Data.dTrainD2 = m_edtD2.GetEditValue();

		m_Data.PatchLoad.bPatchLoad = m_chkPatch.GetCheck();
		if (m_Data.PatchLoad.bPatchLoad)
		{
			T_MVHL_PATCH_ITEM Patch;
			Patch.dWidth = m_edtPatchW.GetEditValue();
			Patch.dLength = m_edtPatchL.GetEditValue();
			m_Data.PatchLoad.aPatchLoad.RemoveAll();
			m_Data.PatchLoad.aPatchLoad.Add(Patch);
		}
	}

	return TRUE;
}

int CMMvhlItemUsrBrazilDlg::GetLoadCount()
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CMMvhlItemUsrBrazilDlg::GetDistCount()
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}
	return i;
}

BOOL CMMvhlItemUsrBrazilDlg::GetLoadAndDistValue(double& dLoad, double& dDist)
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

BOOL CMMvhlItemUsrBrazilDlg::ApplyOrOK()
{
	BOOL bSuccess = FALSE;

	if (!Dlg2Data()) return FALSE;

	if (m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else					 bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);

	if (!bSuccess) return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMMvhlItemUsrBrazilDlg message handlers

BOOL CMMvhlItemUsrBrazilDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitUnit();
	SetHeaderTitle();

	Data2Dlg();

	OnCmdMvhlLoadTypeRdo();

	if (m_bModify)	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMMvhlItemUsrBrazilDlg::OnOK()
{
	if (!ApplyOrOK()) return;

	CDialogMove::OnOK();
}

void CMMvhlItemUsrBrazilDlg::OnCmdMvhlLoadTypeRdo()
{
	ChangeData();
}

void CMMvhlItemUsrBrazilDlg::OnCmdMvhlAddBtn()
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

	int no = nLoadCount + 1;
	InsertItem(nLoadCount, no, dLoad, dDist);

	int nCount = m_List.GetItemCount();
	m_List.SetItemState(nCount - 1, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CMMvhlItemUsrBrazilDlg::OnCmdMvhlInsBtn()
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
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
	for (int i = D_MVHL_NUMLOAD - 1; i > iItem; i--)
	{
		m_Data.dPointLoad[i] = m_Data.dPointLoad[i - 1];
		m_Data.dPointDistance[i] = m_Data.dPointDistance[i - 1];
	}

	// 데이타 삽입한다.
	m_Data.dPointLoad[iItem] = dLoad;
	m_Data.dPointDistance[iItem] = dDist;

	// List에 Item 삽입
	InsertItem(iItem, iItem + 1, dLoad, dDist);

	// List 번호 변경
	int nCount = m_List.GetItemCount();
	CString csVal;
	for (int i = iItem + 1; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i + 1);
		m_List.SetItemText(i, 0, csVal);
	}
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CMMvhlItemUsrBrazilDlg::OnCmdMvhlModBtn()
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_modif));
		return;
	}

	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();

	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;

	if (iItem != nLoadCount - 1 && dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}

	m_Data.dPointLoad[iItem] = dLoad;
	m_Data.dPointDistance[iItem] = dDist;

	ModifyItem(iItem, iItem + 1, dLoad, dDist);
}

void CMMvhlItemUsrBrazilDlg::OnCmdMvhlDelBtn()
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}

	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	int i = iItem;
	for (i = iItem; i < D_MVHL_NUMLOAD - 1; i++)
	{
		m_Data.dPointLoad[i] = m_Data.dPointLoad[i + 1];
		m_Data.dPointDistance[i] = m_Data.dPointDistance[i + 1];
	}
	m_Data.dPointLoad[i] = 0.0;
	m_Data.dPointDistance[i] = 0.0;

	m_List.DeleteItem(iItem);
	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0)
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);

	// List 번호 변경
	CString csVal;
	for (int i = iItem; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i + 1);
		m_List.SetItemText(i, 0, csVal);
	}
}

void CMMvhlItemUsrBrazilDlg::OnCmdMvhlApplyBtn()
{
	ApplyOrOK();
}

void CMMvhlItemUsrBrazilDlg::OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	CString csVal;
	if (iItem == nLoadCount - 1 && nLoadCount != nDistCount)  // fixed, last
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