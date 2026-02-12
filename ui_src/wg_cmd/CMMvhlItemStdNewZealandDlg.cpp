// CMMvhlItemStdNewZealandDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdNewZealandDlg.h"
#include "CMMvhlItemStdPatchLoadInfoNZDlg.h"

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
// CCMMvhlItemStdNewZealandDlg dialog

#define COLCOUNT 3

CCMMvhlItemStdNewZealandDlg::CCMMvhlItemStdNewZealandDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemStdNewZealandDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdNewZealandDlg)	

	//}}AFX_DATA_INIT  
	m_pDoc = CDBDoc::GetDocPoint();
	m_bModify = FALSE;

	m_aPicture.RemoveAll();
	m_aPicture.Add(IDC_CMD_MVHL_PICTURE);
	
	m_aList.RemoveAll();
	m_aList.Add(IDC_CMD_MVHL_PTLIST);

	m_aW.RemoveAll();
	m_aW.Add(IDC_CMD_MVHL_W_STC);
	m_aW.Add(IDC_CMD_MVHL_W_EDT);
	m_aW.Add(IDC_CMD_MVHL_W_UNT);

	m_aEtc.RemoveAll();
	m_aEtc.Add(IDOK);
	m_aEtc.Add(IDCANCEL);
	m_aEtc.Add(IDC_CMD_APPLY);

	m_aCtrlPatch.RemoveAll();
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_CHK);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_BTN);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_STC);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_EDT);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_UNT);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_STC);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_EDT);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_UNT);
}

CCMMvhlItemStdNewZealandDlg::~CCMMvhlItemStdNewZealandDlg()
{
}

void CCMMvhlItemStdNewZealandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdNewZealandDlg)  
	DDX_Control(pDX, IDC_CMD_MVHL_CODE_CMB,               m_cmbCodeName);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_NAME_EDT,          m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_TYPE_CMB,          m_cmbVehicleType);

	DDX_Control(pDX, IDC_CMD_MVHL_W_EDT,       m_edtW);
	DDX_Control(pDX, IDC_CMD_MVHL_W_UNT,       m_untW);

	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_CHK, m_chkPatch);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_W_EDT, m_edtPatchW);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_W_UNT, m_untPatchW);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_L_EDT, m_edtPatchL);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_L_UNT, m_untPatchL);

	DDX_Control(pDX, IDC_CMD_MVHL_PTLIST, m_List);
	DDX_Control(pDX, IDC_CMD_MVHL_PICTURE, m_wndPicture);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvhlItemStdNewZealandDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemStdNewZealandDlg)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_CODE_CMB,        OnCmdMvhlCodeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_VEHI_TYPE_CMB,   OnCmdMvhlTypeCmb)
	ON_BN_CLICKED   (IDC_CMD_MVHL_PATCH_CHK,       OnChkPatch)
	ON_BN_CLICKED   (IDC_CMD_APPLY,                OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_MVHL_PATCH_BTN, OnPatchBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemStdNewZealandDlg::InitUnit()
{
	m_edtW.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untW.SetUnitType(D_UNITSYS_BASE_UNITFORCE);

	m_edtPatchW.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untPatchW.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtPatchL.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untPatchL.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMMvhlItemStdNewZealandDlg::InitCombo()
{
	int nCodeSeq[6];
	nCodeSeq[0] = D_MVHL_TRAFFIC_LOAD_NZ;
	nCodeSeq[1] = D_MVHL_GENERAL_ACCESS_NZ;
	nCodeSeq[2] = D_MVHL_50MAX_NZ;
	nCodeSeq[3] = D_MVHL_HPMV_NZ;
	nCodeSeq[4] = D_MVHL_DECK_EVALUATION;
	nCodeSeq[5] = D_MVHL_FATIGUE_NZ;
	
	for(int i=0; i<6; i++) 
	{
		CDlgUtil::CobxAddItem(m_cmbCodeName, CVehlDB::GetTypeNameAr()[nCodeSeq[i]], nCodeSeq[i]);
	}
}

void CCMMvhlItemStdNewZealandDlg::AlignControl()
{
	UpdateData(TRUE);
	
	CString strVehicleType = _T("");
	m_cmbVehicleType.GetWindowText(strVehicleType);
	
	// Group Box 크기
	CRect rectTarget;
	CRect rect;
	CRect rectShort;
	
	CRect rRef;
	CRect rToMove;

	if (m_nStndCode == D_MVHL_FATIGUE_NZ)
	{
		GetDlgItem(IDC_CMD_MVHL_W_STC)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_PATCH_CHK)->GetWindowRect(rToMove);
		int nDistX = rRef.left - rToMove.left;
		int nDistY = rRef.top - rToMove.top;
		CArray<UINT, UINT> m_aPatchCheck;
		m_aPatchCheck.Add(IDC_CMD_MVHL_PATCH_CHK);
		CDlgUtil::CtrlMoveDistXY(this, m_aPatchCheck, nDistX, nDistY);
	}
	else
	{
		GetDlgItem(IDC_CMD_MVHL_PATCH_W_STC)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_PATCH_CHK)->GetWindowRect(rToMove);
		int nDistX = rRef.left - rToMove.left;
		int nDistY = rRef.top - rToMove.bottom - globalUtils.ScaleByDPI(8);;
		CArray<UINT, UINT> m_aPatchCheck;
		m_aPatchCheck.Add(IDC_CMD_MVHL_PATCH_CHK);
		CDlgUtil::CtrlMoveDistXY(this, m_aPatchCheck, nDistX, nDistY);
	}

	GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->GetWindowRect(rectShort);

	GetDlgItem(IDC_CMD_MVHL_VEHI_NAME_EDT)->GetWindowRect(rectTarget);
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rect);
	int nDistY = rect.bottom - rectTarget.top + globalUtils.ScaleByDPI(30);

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

void CCMMvhlItemStdNewZealandDlg::ControlsShowHide()
{
	UpdateData(TRUE);
	
	CString strVehicleType = _T("");
	m_cmbVehicleType.GetWindowText(strVehicleType);

	BOOL bUDL   = (strVehicleType == _T("HN (normal) loading") || strVehicleType == _T("HO (overload) loading (a)") || strVehicleType == _T("HO (overload) loading (b)"));
	BOOL bPatch = (bUDL || m_nStndCode == D_MVHL_DECK_EVALUATION || m_nStndCode == D_MVHL_FATIGUE_NZ);

	CDlgUtil::CtrlShowHide(this, m_aW,         bUDL);
	CDlgUtil::CtrlShowHide(this, m_aCtrlPatch, bPatch);

	if (strVehicleType == _T("HO (overload) loading (a)") || strVehicleType == _T("Rating 2/8-Tyred Oscilating"))
	{
		GetDlgItem(IDC_CMD_MVHL_PATCH_CHK)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CMD_MVHL_PATCH_CHK)->EnableWindow(bPatch);
	}

	if (m_nStndCode == D_MVHL_FATIGUE_NZ)
	{
		CArray<UINT, UINT> aCtrlPatch;
		aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_STC);
		aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_EDT);
		aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_UNT);
		aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_STC);
		aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_EDT);
		aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_UNT);
		CDlgUtil::CtrlShowHide(this, aCtrlPatch, FALSE);

	}

	BOOL bBtn = (m_nStndCode == D_MVHL_FATIGUE_NZ);
	GetDlgItem(IDC_CMD_MVHL_PATCH_BTN)->ShowWindow(bBtn);
}

void CCMMvhlItemStdNewZealandDlg::SetHeaderTitle()
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

BOOL CCMMvhlItemStdNewZealandDlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
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
			double dDistTmp = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dDist);

// 			if      (dDistTmp ==  0.0) str = _LS(IDS_WG_CMD__ADDD__end);
// 			else if (dDistTmp == -1.0) str = _LS(IDS_WG_CMD_INFINITE);
// 			else                       str.Format(_T("%g"), dDist);

			double dTol = 1.e-10;
			if      (fabs(dDistTmp)       < dTol) str = _LS(IDS_WG_CMD__ADDD__end);
			else if (fabs(dDistTmp + 1.0) < dTol) str = _LS(IDS_WG_CMD_INFINITE);
			else                                  str.Format(_T("%g"), dDist);
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

void CCMMvhlItemStdNewZealandDlg::MakeItemEx()
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

int CCMMvhlItemStdNewZealandDlg::GetLoadCount()
{
	int i = 0;
	for( i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemStdNewZealandDlg::GetDistCount()
{
	int i = 0;
	for( i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}  
	return i;
}

void CCMMvhlItemStdNewZealandDlg::ChangeBitmap(int nIndex, int nBitmap)
{

	CString aBitmapTraffic[] = { _T("cmd_mvhl_138.svg"),_T("cmd_mvhl_138.svg"),_T("cmd_mvhl_138.svg") };
	CString aBitmapGA[] = { _T("cmd_mvhl_279.svg"), _T("cmd_mvhl_280.svg"), _T("cmd_mvhl_281.svg"), _T("cmd_mvhl_282.svg") };
	CString aBitmap500Max[] = { _T("cmd_mvhl_283.svg"),_T("cmd_mvhl_284.svg"), };
	CString aBitmapHPMV[] = { _T("cmd_mvhl_276.svg"), _T("cmd_mvhl_277.svg"),_T("cmd_mvhl_278.svg") };
	CString aBitmapFTG[] = { _T("cmd_mvhl_274.svg"), _T("cmd_mvhl_275.svg"),_T("cmd_mvhl_234.svg") };
	CString aBitmapDeck[] = { _T("cmd_mvhl_129.svg"), _T("cmd_mvhl_129.svg"), _T("cmd_mvhl_234.svg"),
_T("cmd_mvhl_129.svg"), _T("cmd_mvhl_129.svg"), _T("cmd_mvhl_234.svg"), _T("cmd_mvhl_235.svg"),
_T("cmd_mvhl_129.svg"), _T("cmd_mvhl_129.svg"), _T("cmd_mvhl_234.svg"), _T("cmd_mvhl_235.svg") };

	CString strSVG;
	switch(nIndex)
	{
		case D_MVHL_TRAFFIC_LOAD_NZ:
			strSVG = aBitmapTraffic[nBitmap];
			break;
		case D_MVHL_GENERAL_ACCESS_NZ:
			strSVG = aBitmapGA[nBitmap];
			break;
		case D_MVHL_50MAX_NZ:
			strSVG = aBitmap500Max[nBitmap];
			break;
		case D_MVHL_HPMV_NZ:
			strSVG = aBitmapHPMV[nBitmap];
			break;
		case D_MVHL_DECK_EVALUATION:
			strSVG = aBitmapDeck[nBitmap];
			break;
		case D_MVHL_FATIGUE_NZ:
			strSVG = aBitmapFTG[nBitmap];
			break;
		default:
			ASSERT(0);
	}

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + strSVG);
}

void CCMMvhlItemStdNewZealandDlg::Data2Dlg()
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
	ChangeBitmap(m_nStndCode, sIndex);

	T_MVHL_PATCH_LOAD PatchLoadDB; PatchLoadDB.Initialize();
	T_MVHL_PATCH_LOAD* pPatch = NULL;

	BOOL bPatch = (m_Data.VehicleTypeName == _T("HN (normal) loading") || 
		m_Data.VehicleTypeName == _T("HO (overload) loading (a)") || 
		m_Data.VehicleTypeName == _T("HO (overload) loading (b)") ||
		m_nStndCode == D_MVHL_DECK_EVALUATION ||
		m_nStndCode == D_MVHL_FATIGUE_NZ);
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

	m_edtW.SetEditUnit(m_Data.dUniformLoad);

	if (m_Data.PatchLoad.aPatchLoad.GetSize() > 0)
	{
		m_chkPatch.SetCheck(m_Data.PatchLoad.bPatchLoad);
		if (m_nStndCode != D_MVHL_FATIGUE_NZ)
		{
			m_edtPatchW.SetEditUnit(m_Data.PatchLoad.aPatchLoad[0].dWidth);
			m_edtPatchL.SetEditUnit(m_Data.PatchLoad.aPatchLoad[0].dLength);
		}
	}
	else
	{
		m_edtPatchW.SetEditUnit(0.);
		m_edtPatchW.SetEditUnit(0.);
	}

	OnChkPatch();

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdNewZealandDlg::Dlg2Data()
{
	UpdateData(TRUE);
	
	T_MVHL_PATCH_LOAD PatchLoadFT;
  PatchLoadFT.Initialize();

	if (m_nStndCode == D_MVHL_FATIGUE_NZ)
	{
		PatchLoadFT = m_Data.PatchLoad;
	}

	m_Data.Initialize();  
	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);   
	m_Data.bStandard = TRUE;
	m_cmbVehicleType.GetWindowText(m_Data.VehicleTypeName);

	m_Data.nStandardCode = m_nStndCode;

	CVehlDB db(m_pDoc);
	T_MVHL_PATCH_LOAD PatchLoadDB;
	if (!db.GetStandardVehicleLoadValue(m_Data, &PatchLoadDB))  return FALSE;

	if (m_nStndCode == D_MVHL_FATIGUE_NZ)
	{
		m_Data.PatchLoad = PatchLoadFT;
	}

	m_Data.PatchLoad.bPatchLoad = m_chkPatch.GetCheck();

	if (m_nStndCode != D_MVHL_FATIGUE_NZ)
	{
		T_MVHL_PATCH_ITEM Patch;
		Patch.dWidth = m_edtPatchW.GetEditValue();
		Patch.dLength = m_edtPatchL.GetEditValue();
		m_Data.PatchLoad.aPatchLoad.RemoveAll();
		m_Data.PatchLoad.aPatchLoad.Add(Patch);
	}

	return TRUE;
}

BOOL CCMMvhlItemStdNewZealandDlg::ApplyOrOK()
{
	if(!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if(m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else          bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);
	if(!bSuccess) return FALSE;
	
	return TRUE;
}

BOOL CCMMvhlItemStdNewZealandDlg::ChangeData()
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

	T_MVHL_PATCH_LOAD PatchLoadDB; PatchLoadDB.Initialize();
	T_MVHL_PATCH_LOAD* pPatch = NULL;
	BOOL bPatch = (m_Data.VehicleTypeName == _T("HN (normal) loading") || 
		m_Data.VehicleTypeName == _T("HO (overload) loading (a)") || 
		m_Data.VehicleTypeName == _T("HO (overload) loading (b)") || 
		m_nStndCode == D_MVHL_DECK_EVALUATION ||
		m_nStndCode == D_MVHL_FATIGUE_NZ);

	if (bPatch) pPatch = &PatchLoadDB;
	CVehlDB db(m_pDoc);
	if(db.GetStandardVehicleLoadValue(m_Data, pPatch)) MakeItemEx();

	if (bPatch)
	{
		m_Data.PatchLoad = PatchLoadDB;
	}

	Data2Dlg();

	m_edtVehicleName.SetWindowText(m_Data.VehicleTypeName);
	
	return TRUE;
}

void CCMMvhlItemStdNewZealandDlg::SetLoadTypeList(int nIndex)
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

void CCMMvhlItemStdNewZealandDlg::GetIndex(CString& csName, int* sitem)
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
// CCMMvhlItemStdNewZealandDlg message handlers

BOOL CCMMvhlItemStdNewZealandDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	if (!m_bModify)
	{
		m_Data.Initialize();
		m_Data.bStandard = TRUE;
		m_Data.VehicleTypeName = _T("HN (normal) loading");
		m_nStndCode = D_MVHL_TRAFFIC_LOAD_NZ;
		m_Data.nStandardCode = D_MVHL_TRAFFIC_LOAD_NZ;
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

void CCMMvhlItemStdNewZealandDlg::OnOK()
{
	if (!ApplyOrOK()) return;
	CDialogMove::OnOK();
}

void CCMMvhlItemStdNewZealandDlg::OnCmdMvhlCodeCmb()
{
	int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());
	SetLoadTypeList(nCodeType);

	int nIndex = m_cmbCodeName.GetCurSel();
	m_nStndCode = m_cmbCodeName.GetItemData(nIndex);

	ChangeData();

	AlignControl();
	ControlsShowHide();
}

void CCMMvhlItemStdNewZealandDlg::OnCmdMvhlTypeCmb()
{
	ChangeData();

	AlignControl();
	ControlsShowHide();
}

void CCMMvhlItemStdNewZealandDlg::OnCmdApply()
{
	ApplyOrOK();
}

void CCMMvhlItemStdNewZealandDlg::OnPatchBtn()
{
	if (m_Data.PatchLoad.aPatchLoad.GetSize() == 0)
	{
		CVehlDB db(m_pDoc);
		T_MVHL_PATCH_LOAD PatchLoadDB;
		if (!db.GetStandardVehicleLoadValue(m_Data, &PatchLoadDB))
		{
			ASSERT(0);
		}
    m_Data.PatchLoad = PatchLoadDB;
	}

	CCMMvhlItemStdPatchLoadInfoNZDlg Dlg;
	Dlg.SetInitPos(D_INIT_POS_RT);
	Dlg.SetData(m_Data);

	if (Dlg.DoModal() == IDOK)
	{
		m_Data.PatchLoad = Dlg.m_Data.PatchLoad;
	}
}

void CCMMvhlItemStdNewZealandDlg::OnChkPatch()
{
	BOOL bPatchLoad;

	if (m_Data.VehicleTypeName == _T("HO (overload) loading (a)") || m_Data.VehicleTypeName == _T("Rating 2/8-Tyred Oscilating"))
	{
		bPatchLoad = FALSE;
	}
	else
	{
		bPatchLoad = m_chkPatch.GetCheck();
	}

	if (m_nStndCode != D_MVHL_FATIGUE_NZ)
	{
		GetDlgItem(IDC_CMD_MVHL_PATCH_W_STC)->EnableWindow(bPatchLoad);
		GetDlgItem(IDC_CMD_MVHL_PATCH_W_EDT)->EnableWindow(bPatchLoad);
		GetDlgItem(IDC_CMD_MVHL_PATCH_W_UNT)->EnableWindow(bPatchLoad);
		GetDlgItem(IDC_CMD_MVHL_PATCH_L_STC)->EnableWindow(bPatchLoad);
		GetDlgItem(IDC_CMD_MVHL_PATCH_L_EDT)->EnableWindow(bPatchLoad);
		GetDlgItem(IDC_CMD_MVHL_PATCH_L_UNT)->EnableWindow(bPatchLoad);
	}
	else
		GetDlgItem(IDC_CMD_MVHL_PATCH_BTN)->EnableWindow(bPatchLoad);
}

