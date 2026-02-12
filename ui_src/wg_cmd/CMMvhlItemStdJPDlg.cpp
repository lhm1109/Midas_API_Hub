// CMMvhlItemStdJPDlg.cpp : implementation file 
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdJPDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"

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
// CCMMvhlItemStdJPDlg dialog

#define COLCOUNT_JP 3

CCMMvhlItemStdJPDlg::CCMMvhlItemStdJPDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemStdJPDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_bModify = FALSE;

	m_aPicture.RemoveAll();
	m_aPicture.Add(IDC_CMD_MVHL_PICTURE);

	m_aList.RemoveAll();
	m_aList.Add(IDC_CMD_MVHL_PTLIST);

	m_aNum.RemoveAll();
	m_aNum.Add(IDC_CMD_MVHL_JP_VEHICLE_NUM_STC);
	m_aNum.Add(IDC_CMD_MVHL_JP_VEHICLE_NUM_EDT);
	m_aNum.Add(IDC_CMD_MVHL_JP_VEHICLE_NUM_SPIN);

	m_aDW2.RemoveAll();
	m_aDW2.Add(IDC_CMD_MVHL_JP_DW2_STC);
	m_aDW2.Add(IDC_CMD_MVHL_JP_DW2_EDT);
	m_aDW2.Add(IDC_CMD_MVHL_JP_DW2_UNT);

// 	m_aDD2.RemoveAll();
// 	m_aDD2.Add(IDC_CMD_MVHL_JP_DD2_STC);
// 	m_aDD2.Add(IDC_CMD_MVHL_JP_DD2_EDT);
// 	m_aDD2.Add(IDC_CMD_MVHL_JP_DD2_UNT);

	m_aEtc.RemoveAll();
	m_aEtc.Add(IDOK);
	m_aEtc.Add(IDCANCEL);
	m_aEtc.Add(IDC_CMD_APPLY);

}

CCMMvhlItemStdJPDlg::~CCMMvhlItemStdJPDlg()
{
}

void CCMMvhlItemStdJPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdJPDlg)  
	DDX_Control(pDX, IDC_CMD_MVHL_CODE_CMB,           m_cmbCodeName);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_NAME_EDT,      m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_TYPE_CMB,      m_cmbVehicleType);
	DDX_Control(pDX, IDC_CMD_MVHL_PTLIST,             m_List);

	DDX_Control(pDX, IDC_CMD_MVHL_JP_VEHICLE_NUM_EDT, m_ProcessNumEdt);

	DDX_Control(pDX, IDC_CMD_MVHL_JP_VEHICLE_H_EDT, m_edtH);
	DDX_Control(pDX, IDC_CMD_MVHL_JP_VEHICLE_H_UNT, m_untH);

	DDX_Control(pDX, IDC_CMD_MVHL_JP_W_EDT,   m_edtW);
	DDX_Control(pDX, IDC_CMD_MVHL_JP_W_UNT,   m_untW);
	DDX_Control(pDX, IDC_CMD_MVHL_JP_PS_EDT,  m_edtPs);
	DDX_Control(pDX, IDC_CMD_MVHL_JP_PS_UNT,  m_untPs);
	DDX_Control(pDX, IDC_CMD_MVHL_JP_PM_EDT,  m_edtPm);
	DDX_Control(pDX, IDC_CMD_MVHL_JP_PM_UNT,  m_untPm);

	DDX_Control(pDX, IDC_CMD_MVHL_JP_DW1_EDT, m_edtW1);
	DDX_Control(pDX, IDC_CMD_MVHL_JP_DW1_UNT, m_untW1);
	DDX_Control(pDX, IDC_CMD_MVHL_JP_DD1_EDT, m_edtD1);
	DDX_Control(pDX, IDC_CMD_MVHL_JP_DD1_UNT, m_untD1);

	DDX_Control(pDX, IDC_CMD_MVHL_JP_DW2_EDT, m_edtW2);
	DDX_Control(pDX, IDC_CMD_MVHL_JP_DW2_UNT, m_untW2);
	DDX_Control(pDX, IDC_CMD_MVHL_JP_DD2_EDT, m_edtD2);
	DDX_Control(pDX, IDC_CMD_MVHL_JP_DD2_UNT, m_untD2);

	DDX_Control(pDX, IDC_CMD_MVHL_PICTURE, m_wndPicture);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvhlItemStdJPDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemStdJPDlg)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_CODE_CMB,        OnCmdMvhlCodeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_VEHI_TYPE_CMB,   OnCmdMvhlTypeCmb)
	ON_BN_CLICKED   (IDC_CMD_APPLY,                OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemStdJPDlg::InitUnit()
{
	m_edtH.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untH.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtW .SetUnitType(D_UNITSYS_BASE_UNITLOAD);
	m_untW .SetUnitType(D_UNITSYS_BASE_UNITLOAD);
	m_edtPs.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_untPs.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_edtPm.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_untPm.SetUnitType(D_UNITSYS_BASE_FORCE);

	m_edtW1.SetUnitType(D_UNITSYS_BASE_UNITLOAD);
	m_untW1.SetUnitType(D_UNITSYS_BASE_UNITLOAD);
	m_edtD1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untD1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtW2.SetUnitType(D_UNITSYS_BASE_UNITLOAD);
	m_untW2.SetUnitType(D_UNITSYS_BASE_UNITLOAD);
	m_edtD2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untD2.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMMvhlItemStdJPDlg::InitCombo()
{
	int nCodeSeq[2];
	nCodeSeq[0] = D_MVHL_JP_STD_TRAIN;
	nCodeSeq[1] = D_MVHL_JP_LOCOMOTIV;
	
	for(int i=0; i<2; i++) 
	{
		CDlgUtil::CobxAddItem(m_cmbCodeName, CVehlDB::GetTypeNameAr()[nCodeSeq[i]], nCodeSeq[i]);
	}
}

void CCMMvhlItemStdJPDlg::AlignControl()
{
 	UpdateData(TRUE);
 	
	CString strVehicleType = _T("");
	m_cmbVehicleType.GetWindowText(strVehicleType);
	
	CRect rRef;
	CRect rToMove;
	int nDistY = 0;	

	return;

	// OK, Cancel, Apply
// 	GetDlgItem(IDOK)->GetWindowRect(rToMove);
// 	nDistY = rRef.bottom - rToMove.top + 25;
// 	CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);
// 
// 	// resize self size
// 	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
// 	CRect rectLast;
// 	pWndLast->GetWindowRect(rectLast);
// 	CRect r;
// 	GetWindowRect(r);
// 	r.bottom = rectLast.bottom + 15;
// 	MoveWindow(r);
}

void CCMMvhlItemStdJPDlg::ControlsShowHide()
{
	UpdateData(TRUE);

	CString strVehicleType = _T("");
	m_cmbVehicleType.GetWindowText(strVehicleType);
 
	if (m_nStndCode == D_MVHL_JP_STD_TRAIN)
	{	
		CDlgUtil::CtrlEnableDisable(this, m_aNum, TRUE);

		CDlgUtil::CtrlShowHide(this, m_aDW2, FALSE);
		//CDlgUtil::CtrlShowHide(this, m_aDD2, FALSE);
	}
	else if (m_nStndCode == D_MVHL_JP_LOCOMOTIV)
	{
		m_ProcessNumEdt.SetValue(1);  // Num
		CDlgUtil::CtrlEnableDisable(this, m_aNum, FALSE);

		CDlgUtil::CtrlShowHide(this, m_aDW2, TRUE);
		//CDlgUtil::CtrlShowHide(this, m_aDD2, TRUE);
	}
	else
		ASSERT(0);
}

void CCMMvhlItemStdJPDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__No), _LS(IDS_WG_CMD__ADDD__Load), _LS(IDS_WG_CMD__ADDD__Spacing)};
	
	// title에 단위 추가
	CString sLdUnit,sDtUnit;
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointLoad,sLdUnit);
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointDistance,sDtUnit);
	aTitle[1] = aTitle[1] +_T("(")+sLdUnit+_T(")");
	aTitle[2] = aTitle[2] +_T("(")+sDtUnit+_T(")");
	
	int nColWidth[COLCOUNT_JP];
	
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
	for(i = 0; i < COLCOUNT_JP; i++)
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

BOOL CCMMvhlItemStdJPDlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
{
	LVITEM lvitem;
	CString str;
	
	lvitem.iItem = nPos;
	for(int i = 0; i < COLCOUNT_JP; i++)
	{
		lvitem.iSubItem=i;
		if (i == 0) str.Format(_T("%d"), nNo);
		else if (i == 1) str.Format(_T("%g"), dLoad);
		else if (i == 2)
		{
			double dDistTmp = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dDist);

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

void CCMMvhlItemStdJPDlg::MakeItemEx()
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

int CCMMvhlItemStdJPDlg::GetLoadCount()
{
	int i = 0;
	for( i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemStdJPDlg::GetDistCount()
{
	int i = 0;
	for( i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}  
	return i;
}

void CCMMvhlItemStdJPDlg::ChangeBitmap(int nIndex, int nBitmap)
{
	CString aBitmapTrain[]  = { _T("cmd_mvhl_jp_Train.svg") };  // IDB_CMD_MVHL_JP_TRAIN
	CString aBitmapLocomo[] = { _T("cmd_mvhl_jp_Loco.svg") }; // IDB_CMD_MVHL_JP_LOCO

	CString strSVG;
	int nSelBitmap = 0;
	switch (nIndex)
	{
	case D_MVHL_JP_STD_TRAIN:          strSVG = aBitmapTrain[nSelBitmap];  break;
	case D_MVHL_JP_LOCOMOTIV:          strSVG = aBitmapLocomo[nSelBitmap];  break;
	default:
		ASSERT(0); break;
	}

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + strSVG);

// 	nBitmap = 0;
// 	UINT aBitmapTrain[]    = { IDB_CMD_MVHL_JP_TRAIN };
// 	UINT aBitmapLocomo[]   = { IDB_CMD_MVHL_JP_LOCO };
// 		
// 	UINT bitmapID; 
// 	int nSelBitmap = 0;
// 	switch(nIndex)
// 	{
// 		case D_MVHL_JP_STD_TRAIN:          bitmapID = aBitmapTrain[nBitmap];  break;
// 		case D_MVHL_JP_LOCOMOTIV:          bitmapID = aBitmapLocomo[nBitmap];  break;
// 		default:
// 		{
// 			ASSERT(0);
// 			bitmapID = aBitmapTrain[nBitmap];
// 			break;
// 		}
// 	}
// 	
// 	if(m_pBitmap != 0) delete m_pBitmap;
// 	CStatic* pImage = (CStatic*)GetDlgItem(IDC_CMD_MVHL_PICTURE);
// 	m_pBitmap = new CBitmap;
// 	m_pBitmap->LoadBitmap(bitmapID);
// 	pImage->SetBitmap(HBITMAP(*m_pBitmap));
}

void CCMMvhlItemStdJPDlg::Data2Dlg()
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

	CVehlDB db(m_pDoc);
	if (db.GetStandardVehicleLoadValue(m_Data))	MakeItemEx();

	m_ProcessNumEdt.SetValue(m_Data.nNumCarriage);
	m_edtH.SetEditUnit(m_Data.dHeight);
	//m_edtD2.SetEditUnit(m_Data.dD2); 무한대로 변경

	m_edtW2.SetEditUnit(m_Data.dW2);

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdJPDlg::Dlg2Data()
{
	UpdateData(TRUE);
	
	m_Data.Initialize();  
	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);   
	m_Data.bStandard = TRUE;
	m_cmbVehicleType.GetWindowText(m_Data.VehicleTypeName);

	m_Data.nStandardCode = m_nStndCode;
	m_Data.nLoadType = 1;

  //m_Data.dW2          = m_edtW2.GetEditValue();   // DB
	m_Data.nNumCarriage = m_ProcessNumEdt.GetEditValue();
	m_Data.dHeight      = m_edtH.GetEditValue();
  //m_Data.dD2          = m_edtD2.GetEditValue();   // 무한대

	return TRUE;
}

BOOL CCMMvhlItemStdJPDlg::ApplyOrOK()
{
	if(!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if(m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else          bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);
	if(!bSuccess) return FALSE;
	
	return TRUE;
}

BOOL CCMMvhlItemStdJPDlg::ChangeData()
{
	UpdateData(TRUE);

	//m_Data.Initialize();
	m_Data.bStandard = TRUE;
	m_cmbVehicleType.GetWindowText(m_Data.VehicleTypeName);
	m_Data.nStandardCode = m_nStndCode;
	
	int sIndex;
	CDlgUtil::CobxSetCurSelItemData(m_cmbCodeName, m_nStndCode);
	GetIndex(m_Data.VehicleTypeName, &sIndex);
	m_cmbVehicleType.SetCurSel(sIndex);
	ChangeBitmap(m_nStndCode, sIndex);

// 	if (m_Data.VehicleTypeName == _T("Sidewalk"))
// 	{
// 		m_Data.dSidewalkW = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_PRESSURE, 4.5);
// 	}
// 	if (m_Data.VehicleTypeName == _T("Load System Bt"))
// 	{
// 		m_Data.dCoefficient01[0] = 1.2;
// 		m_Data.dCoefficient02[0] = 1.0;
// 	}
	Data2Dlg();

	m_edtVehicleName.SetWindowText(m_Data.VehicleTypeName);

// 	if (m_Data.VehicleTypeName == _T("Load System A"))
// 	{
// 		GetDlgItem(IDC_CMD_MVHL_FR_COEFFICIENT_STC)->SetWindowText(_LS(IDS_WG_CMD__ADDD__FR_MVHL_Coefficent_a1));
// 	}
// 	else if (m_Data.VehicleTypeName == _T("Load System Bc"))
// 	{
// 		GetDlgItem(IDC_CMD_MVHL_FR_COEFFICIENT_STC)->SetWindowText(_LS(IDS_WG_CMD__ADDD__FR_MVHL_Coefficent_bc));
// 	}
// 	else if (m_Data.VehicleTypeName == _T("Load System Bt"))
// 	{
// 		GetDlgItem(IDC_CMD_MVHL_FR_COEFFICIENT_STC)->SetWindowText(_LS(IDS_WG_CMD__ADDD__FR_MVHL_Coefficent_bt));
// 	}

	return TRUE;
}

void CCMMvhlItemStdJPDlg::SetLoadTypeList(int nIndex)
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

void CCMMvhlItemStdJPDlg::GetIndex(CString &csName, int *sitem)
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
// CCMMvhlItemStdJPDlg message handlers

BOOL CCMMvhlItemStdJPDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	if(!m_bModify)
	{
		m_Data.Initialize();
		m_Data.bStandard = TRUE;    
		m_Data.VehicleTypeName = _T("N-16");
		m_nStndCode          = D_MVHL_JP_STD_TRAIN;
		m_Data.nStandardCode = D_MVHL_JP_STD_TRAIN;

		m_Data.dD2     = M_InitValueCurUnit(250.0, KN, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dHeight = M_InitValueCurUnit(  1.5, KN, M, D_UNITSYS_BASE_LENGTH);	
	}
	GetIndex(m_Data.VehicleTypeName);
	int nIndex = m_Data.nStandardCode;
	if (nIndex < 0) return TRUE;
	
	InitUnit();
	InitCombo();
	SetHeaderTitle();
	SetLoadTypeList(nIndex);
	
	m_ProcessNumEdt.SetRange(1, SHRT_MAX);

	Data2Dlg();
	
	AlignControl();
	ControlsShowHide();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemStdJPDlg::OnOK() 
{
	if (!ApplyOrOK()) return;	
	CDialogMove::OnOK();
}

void CCMMvhlItemStdJPDlg::OnCmdMvhlCodeCmb() 
{
	int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());
	SetLoadTypeList(nCodeType);

	int nIndex = m_cmbCodeName.GetCurSel();
	m_nStndCode = m_cmbCodeName.GetItemData(nIndex);

	ChangeData();

	AlignControl();
	ControlsShowHide();
}

void CCMMvhlItemStdJPDlg::OnCmdMvhlTypeCmb() 
{
	ChangeData();  

	AlignControl();
	ControlsShowHide();
}

void CCMMvhlItemStdJPDlg::OnCmdApply() 
{	
	ApplyOrOK();
}