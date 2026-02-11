// CMMvhlItemStdBrazilDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdBrazilDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TestEnvMgr.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MvhlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBrazilDlg dialog

#define COLCOUNT 3

CCMMvhlItemStdBrazilDlg::CCMMvhlItemStdBrazilDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemStdBrazilDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdBrazilDlg)	

	//}}AFX_DATA_INIT  
	m_pDoc = CDBDoc::GetDocPoint();
	m_bModify = FALSE;

	m_aPicture.RemoveAll();
	m_aPicture.Add(IDC_CMD_MVHL_PICTURE);

	m_aList.RemoveAll();
	m_aList.Add(IDC_CMD_MVHL_PTLIST);

	m_aRoadWay.RemoveAll();
	m_aRoadWay.Add(IDC_CMD_MVHL_W_STC);
	m_aRoadWay.Add(IDC_CMD_MVHL_W_EDT);
	m_aRoadWay.Add(IDC_CMD_MVHL_W_UNT);
	m_aRoadWay.Add(IDC_CMD_MVHL_D1_STC);
	m_aRoadWay.Add(IDC_CMD_MVHL_D1_EDT);
	m_aRoadWay.Add(IDC_CMD_MVHL_D1_UNT);
	m_aRoadWay.Add(IDC_CMD_MVHL_D2_STC);
	m_aRoadWay.Add(IDC_CMD_MVHL_D2_EDT);
	m_aRoadWay.Add(IDC_CMD_MVHL_D2_UNT);

	m_aTrain.RemoveAll();
	m_aTrain.Add(IDC_CMD_MVHL_W1_STC);
	m_aTrain.Add(IDC_CMD_MVHL_W1_EDT);
	m_aTrain.Add(IDC_CMD_MVHL_W1_UNT);
	m_aTrain.Add(IDC_CMD_MVHL_W2_STC);
	m_aTrain.Add(IDC_CMD_MVHL_W2_EDT);
	m_aTrain.Add(IDC_CMD_MVHL_W2_UNT);
	m_aTrain.Add(IDC_CMD_MVHL_D1_STC);
	m_aTrain.Add(IDC_CMD_MVHL_D1_EDT);
	m_aTrain.Add(IDC_CMD_MVHL_D1_UNT);
	m_aTrain.Add(IDC_CMD_MVHL_D2_STC);
	m_aTrain.Add(IDC_CMD_MVHL_D2_EDT);
	m_aTrain.Add(IDC_CMD_MVHL_D2_UNT);

	m_aDist.RemoveAll();
	m_aDist.Add(IDC_CMD_MVHL_D1_STC);
	m_aDist.Add(IDC_CMD_MVHL_D1_EDT);
	m_aDist.Add(IDC_CMD_MVHL_D1_UNT);
	m_aDist.Add(IDC_CMD_MVHL_D2_STC);
	m_aDist.Add(IDC_CMD_MVHL_D2_EDT);
	m_aDist.Add(IDC_CMD_MVHL_D2_UNT);

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

CCMMvhlItemStdBrazilDlg::~CCMMvhlItemStdBrazilDlg()
{
}

void CCMMvhlItemStdBrazilDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdBrazilDlg)  
	DDX_Control(pDX, IDC_CMD_MVHL_CODE_CMB, m_cmbCodeName);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_NAME_EDT, m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_TYPE_CMB, m_cmbVehicleType);

	DDX_Control(pDX, IDC_CMD_MVHL_W_EDT, m_edtW);
	DDX_Control(pDX, IDC_CMD_MVHL_W_UNT, m_untW);

	DDX_Control(pDX, IDC_CMD_MVHL_W1_EDT, m_edtW1);
	DDX_Control(pDX, IDC_CMD_MVHL_W1_UNT, m_untW1);

	DDX_Control(pDX, IDC_CMD_MVHL_W2_EDT, m_edtW2);
	DDX_Control(pDX, IDC_CMD_MVHL_W2_UNT, m_untW2);

	DDX_Control(pDX, IDC_CMD_MVHL_D1_EDT, m_edtD1);
	DDX_Control(pDX, IDC_CMD_MVHL_D1_UNT, m_untD1);

	DDX_Control(pDX, IDC_CMD_MVHL_D2_EDT, m_edtD2);
	DDX_Control(pDX, IDC_CMD_MVHL_D2_UNT, m_untD2);

	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_CHK, m_chkPatch);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_W_EDT, m_edtPatchW);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_W_UNT, m_untPatchW);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_L_EDT, m_edtPatchL);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_L_UNT, m_untPatchL);

	DDX_Control(pDX, IDC_CMD_MVHL_PTLIST, m_List);
	DDX_Control(pDX, IDC_CMD_MVHL_PICTURE, m_wndPicture);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvhlItemStdBrazilDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemStdBrazilDlg)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_CODE_CMB, OnCmdMvhlCodeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_VEHI_TYPE_CMB, OnCmdMvhlTypeCmb)
	ON_BN_CLICKED(IDC_CMD_MVHL_PATCH_CHK, OnChkPatch)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemStdBrazilDlg::InitUnit()
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

void CCMMvhlItemStdBrazilDlg::InitCombo()
{
	int nCodeSeq[3];
	nCodeSeq[0] = D_MVHL_NBR7188_ROADWAY;
	nCodeSeq[1] = D_MVHL_NBR7188_FOOTWAY;
	nCodeSeq[2] = D_MVHL_NBR7189_RAILWAY;

	for (int i = 0; i < 3; i++)
	{
		CDlgUtil::CobxAddItem(m_cmbCodeName, CVehlDB::GetTypeNameAr()[nCodeSeq[i]], nCodeSeq[i]);
	}
}

void CCMMvhlItemStdBrazilDlg::AlignControl()
{
	UpdateData(TRUE);

	CString strVehicleType = _T("");
	m_cmbVehicleType.GetWindowText(strVehicleType);

	BOOL bTrain = (m_nStndCode == D_MVHL_NBR7189_RAILWAY);

	CRect rRef;
	CRect rToMove;
	GetDlgItem(IDC_CMD_MVHL_W_EDT)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_W1_EDT)->GetWindowRect(rToMove);

	int nDistX = rRef.left - rToMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aTrain, nDistX);

	int nDistY;

	if (bTrain)
	{
		GetDlgItem(IDC_CMD_MVHL_W2_EDT)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_D1_EDT)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);;
		CDlgUtil::CtrlMoveDistY(this, m_aDist, nDistY);
	}
	else
	{
		GetDlgItem(IDC_CMD_MVHL_W_EDT)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_D1_EDT)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);;
		CDlgUtil::CtrlMoveDistY(this, m_aDist, nDistY);
	}

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

	Invalidate();
}

void CCMMvhlItemStdBrazilDlg::ControlsShowHide()
{
	UpdateData(TRUE);

	CString strVehicleType = _T("");
	m_cmbVehicleType.GetWindowText(strVehicleType);

	BOOL bTrain = (m_nStndCode == D_MVHL_NBR7189_RAILWAY);
	BOOL bUDL = (m_nStndCode == D_MVHL_NBR7188_FOOTWAY);

	GetDlgItem(IDC_CMD_MVHL_W_STC)->ShowWindow((!bTrain)? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_CMD_MVHL_W_EDT)->ShowWindow((!bTrain)? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_CMD_MVHL_W_UNT)->ShowWindow((!bTrain)? SW_SHOW : SW_HIDE);

	GetDlgItem(IDC_CMD_MVHL_W1_STC)->ShowWindow((bTrain) ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_CMD_MVHL_W1_EDT)->ShowWindow((bTrain) ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_CMD_MVHL_W1_UNT)->ShowWindow((bTrain) ? SW_SHOW : SW_HIDE);

	GetDlgItem(IDC_CMD_MVHL_W2_STC)->ShowWindow((bTrain) ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_CMD_MVHL_W2_EDT)->ShowWindow((bTrain) ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_CMD_MVHL_W2_UNT)->ShowWindow((bTrain) ? SW_SHOW : SW_HIDE);

	GetDlgItem(IDC_CMD_MVHL_D1_STC)->ShowWindow((!bUDL) ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_CMD_MVHL_D1_EDT)->ShowWindow((!bUDL) ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_CMD_MVHL_D1_UNT)->ShowWindow((!bUDL) ? SW_SHOW : SW_HIDE);

	GetDlgItem(IDC_CMD_MVHL_D2_STC)->ShowWindow((!bUDL) ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_CMD_MVHL_D2_EDT)->ShowWindow((!bUDL) ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_CMD_MVHL_D2_UNT)->ShowWindow((!bUDL) ? SW_SHOW : SW_HIDE);


	BOOL bPatch = (m_nStndCode == D_MVHL_NBR7188_ROADWAY || m_nStndCode == D_MVHL_NBR7189_RAILWAY);
	CDlgUtil::CtrlShowHide(this, m_aCtrlPatch, bPatch);
}

void CCMMvhlItemStdBrazilDlg::SetHeaderTitle()
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
	nColWidth[0] = 28; nColWidth[1] = 70; nColWidth[2] = 90;

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

BOOL CCMMvhlItemStdBrazilDlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
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
			double dDistTmp = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dDist);

			// 			if      (dDistTmp ==  0.0) str = _LS(IDS_WG_CMD__ADDD__end);
			// 			else if (dDistTmp == -1.0) str = _LS(IDS_WG_CMD_INFINITE);
			// 			else                       str.Format(_T("%g"), dDist);

			double dTol = 1.e-10;
			if (fabs(dDistTmp) < dTol) str = _LS(IDS_WG_CMD__ADDD__end);
			else if (fabs(dDistTmp + 1.0) < dTol) str = _LS(IDS_WG_CMD_INFINITE);
			else                                  str.Format(_T("%g"), dDist);
		}
		else str = _LS(IDS_WG_CMD__ADDD__Error);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		if (i == 0)
			m_List.InsertItem(&lvitem);
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

void CCMMvhlItemStdBrazilDlg::MakeItemEx()
{
	m_List.DeleteAllItems();
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();

	if (nLoadCount > 0)
	{
		int i = 0;
		for (i = 0; i < nDistCount; i++)
			InsertItem(i, i + 1, m_Data.dPointLoad[i], m_Data.dPointDistance[i]);

		if (nLoadCount > nDistCount)
			InsertItem(i, i + 1, m_Data.dPointLoad[i], 0.0);
	}

	UpdateData(FALSE);
}

int CCMMvhlItemStdBrazilDlg::GetLoadCount()
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemStdBrazilDlg::GetDistCount()
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}
	return i;
}

void CCMMvhlItemStdBrazilDlg::ChangeBitmap(int nIndex, int nBitmap)
{

	CString aBitmapNBR7188Road[] = { _T("cmd_mvhc_NBR7188.svg"),_T("cmd_mvhc_NBR7188.svg") };
	CString aBitmapNBR7188Foot[] = { _T("cmd_mvhl_55.svg"),_T("cmd_mvhl_55.svg") };
	CString aBitmapNBR7189Rail[] = { _T("cmd_mvhc_NBR7189.svg"), _T("cmd_mvhc_NBR7189.svg"), _T("cmd_mvhc_NBR7189.svg"), _T("cmd_mvhc_NBR7189.svg") };

	CString strSVG;
	switch (nIndex)
	{
	case D_MVHL_NBR7188_ROADWAY:
		strSVG = aBitmapNBR7188Road[nBitmap];
		break;
	case D_MVHL_NBR7188_FOOTWAY:
		strSVG = aBitmapNBR7188Foot[nBitmap];
		break;
	case D_MVHL_NBR7189_RAILWAY:
		strSVG = aBitmapNBR7189Rail[nBitmap];
		break;
	default:
		ASSERT(0);
	}

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + strSVG);
}

void CCMMvhlItemStdBrazilDlg::Data2Dlg()
{
	m_nStndCode = m_Data.nStandardCode;
	if (m_Data.VehicleLoadName.IsEmpty())
		m_edtVehicleName.SetWindowText(m_Data.VehicleTypeName);
	else
		m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);

	int sIndex;
	CDlgUtil::CobxSetCurSelItemData(m_cmbCodeName, m_nStndCode);
	GetIndex(m_Data.VehicleTypeName, &sIndex);
	m_cmbVehicleType.SetCurSel(sIndex);
	ChangeBitmap(m_nStndCode, sIndex);

	BOOL bTrain = (m_nStndCode == D_MVHL_NBR7189_RAILWAY);
	BOOL bUDL = (m_nStndCode == D_MVHL_NBR7188_FOOTWAY);
	BOOL bPatch = (m_nStndCode == D_MVHL_NBR7188_ROADWAY || bTrain);

	T_MVHL_PATCH_LOAD PatchLoadDB; PatchLoadDB.Initialize();
	T_MVHL_PATCH_LOAD* pPatch = NULL;

	if (bPatch)
	{
		if (m_bModify)
		{
			if (m_Data.PatchLoad.aPatchLoad.GetSize() != 0)
			{
				PatchLoadDB = m_Data.PatchLoad;
			}
			else
			{
				pPatch = &PatchLoadDB;
			}
		}
		else
		{
			pPatch = &PatchLoadDB;
		}
	}
	else
	{
		m_Data.PatchLoad.Initialize();
	}

	CVehlDB db(m_pDoc);
	if (db.GetStandardVehicleLoadValue(m_Data, pPatch))	MakeItemEx();

	if (bPatch)
	{
		m_Data.PatchLoad = PatchLoadDB;
	}

	m_edtW.SetEditUnit(m_Data.dPressureLoad);

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

	OnChkPatch();

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdBrazilDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.Initialize();
	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);
	m_Data.bStandard = TRUE;
	m_cmbVehicleType.GetWindowText(m_Data.VehicleTypeName);

	m_Data.nStandardCode = m_nStndCode;

	CVehlDB db(m_pDoc);
	T_MVHL_PATCH_LOAD PatchLoadDB;
	if (!db.GetStandardVehicleLoadValue(m_Data, &PatchLoadDB))  return FALSE;

	T_MVHL_PATCH_ITEM Patch;
	Patch.dWidth = m_edtPatchW.GetEditValue();
	Patch.dLength = m_edtPatchL.GetEditValue();
	m_Data.PatchLoad.aPatchLoad.RemoveAll();
	m_Data.PatchLoad.aPatchLoad.Add(Patch);
	m_Data.PatchLoad.bPatchLoad = m_chkPatch.GetCheck();

	return TRUE;
}

BOOL CCMMvhlItemStdBrazilDlg::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if (m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else          bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);
	if (!bSuccess) return FALSE;

	return TRUE;
}

BOOL CCMMvhlItemStdBrazilDlg::ChangeData()
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
	ChangeBitmap(m_nStndCode, sIndex);


	BOOL bTrain = (m_nStndCode == D_MVHL_NBR7189_RAILWAY);
	BOOL bUDL = (m_nStndCode == D_MVHL_NBR7188_FOOTWAY);
	BOOL bPatch = (m_nStndCode == D_MVHL_NBR7188_ROADWAY);

	T_MVHL_PATCH_LOAD PatchLoadDB; PatchLoadDB.Initialize();
	T_MVHL_PATCH_LOAD* pPatch = NULL;

	if (bPatch) pPatch = &PatchLoadDB;
	CVehlDB db(m_pDoc);
	if (db.GetStandardVehicleLoadValue(m_Data, pPatch)) MakeItemEx();

	if (bPatch)
	{
		m_Data.PatchLoad = PatchLoadDB;
	}

	Data2Dlg();

	m_edtVehicleName.SetWindowText(m_Data.VehicleTypeName);

	return TRUE;
}

void CCMMvhlItemStdBrazilDlg::SetLoadTypeList(int nIndex)
{
	if (nIndex < 0) return;

	m_cmbVehicleType.ResetContent();
	int i = 0;
	while (i < D_MAX_COUNT && CVehlDB::GetTypeListAr()[nIndex][i] != _T(""))
	{
		CString strVehicleType = CVehlDB::GetTypeListAr()[nIndex][i++];
		m_cmbVehicleType.AddString(strVehicleType);
	}
	m_cmbVehicleType.SetCurSel(0);
}

void CCMMvhlItemStdBrazilDlg::GetIndex(CString& csName, int* sitem)
{
	for (int i = 0; i < D_MAX_COUNT; i++)
	{
		if (CVehlDB::GetTypeListAr()[m_Data.nStandardCode][i] == csName)
		{
			if (sitem) *sitem = i;
			return;
		}
	}

	ASSERT(0);
	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_vehicle_load_name));
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBrazilDlg message handlers

BOOL CCMMvhlItemStdBrazilDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	if (!m_bModify)
	{
		m_Data.Initialize();
		m_Data.bStandard = TRUE;
		m_Data.VehicleTypeName = _T("TB-450");
		m_nStndCode = D_MVHL_NBR7188_ROADWAY;
		m_Data.nStandardCode = D_MVHL_NBR7188_ROADWAY;
	}
	GetIndex(m_Data.VehicleTypeName);
	int nIndex = m_Data.nStandardCode;
	if (nIndex < 0) return TRUE;

	InitUnit();
	InitCombo();
	SetHeaderTitle();
	SetLoadTypeList(nIndex);

	Data2Dlg();

	AlignControl();
	ControlsShowHide();

	//	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_mvhl_128.svg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemStdBrazilDlg::OnOK()
{
	if (!ApplyOrOK()) return;
	CDialogMove::OnOK();
}

void CCMMvhlItemStdBrazilDlg::OnCmdMvhlCodeCmb()
{
	int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());
	SetLoadTypeList(nCodeType);

	int nIndex = m_cmbCodeName.GetCurSel();
	m_nStndCode = m_cmbCodeName.GetItemData(nIndex);

	ChangeData();

	AlignControl();
	ControlsShowHide();
}

void CCMMvhlItemStdBrazilDlg::OnCmdMvhlTypeCmb()
{
	ChangeData();

	AlignControl();
	ControlsShowHide();
}

void CCMMvhlItemStdBrazilDlg::OnCmdApply()
{
	ApplyOrOK();
}

void CCMMvhlItemStdBrazilDlg::OnChkPatch()
{
	BOOL bPatchLoad;

	if (m_Data.nStandardCode == D_MVHL_NBR7188_FOOTWAY)
	{
		bPatchLoad = FALSE;
	}
	else
	{
		bPatchLoad = m_chkPatch.GetCheck();
	}

	GetDlgItem(IDC_CMD_MVHL_PATCH_W_STC)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_W_EDT)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_W_UNT)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_STC)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_EDT)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_UNT)->EnableWindow(bPatchLoad);
}

