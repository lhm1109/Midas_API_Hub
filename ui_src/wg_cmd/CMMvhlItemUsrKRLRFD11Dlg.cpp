
#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrKRLRFD11Dlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrKRLRFD11Dlg dialog

#define COLCOUNT 4

CCMMvhlItemUsrKRLRFD11Dlg::CCMMvhlItemUsrKRLRFD11Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemUsrKRLRFD11Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemUsrKRLRFD11Dlg)
	m_nLoadType = 0;
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();

	m_bModify = FALSE;

	m_Data.Initialize();

	m_aTruck.RemoveAll();
	m_aTruck.Add(IDC_CMD_MVHL_CONVERT_DIST_LOAD_CHK);
	m_aTruck.Add(IDC_CMD_MVHL_PATCH_CHK);
	m_aTruck.Add(IDC_CMD_USER_VEHI_TRUCK_GRP);
	m_aTruck.Add(IDC_CMD_USER_VEHI_TRUCK_P_STC);
	m_aTruck.Add(IDC_CMD_USER_VEHI_TRUCK_P_EDT);
	m_aTruck.Add(IDC_CMD_USER_VEHI_TRUCK_D_STC);
	m_aTruck.Add(IDC_CMD_USER_VEHI_TRUCK_D_EDT);
	m_aTruck.Add(IDC_CMD_USER_VEHI_TRUCK_L_STC);
	m_aTruck.Add(IDC_CMD_USER_VEHI_TRUCK_L_EDT);
	m_aTruck.Add(IDC_CMD_LIST);
	m_aTruck.Add(IDC_CMD_BTN_ADD);
	m_aTruck.Add(IDC_CMD_BTN_INSERT);
	m_aTruck.Add(IDC_CMD_BTN_MODIFY);
	m_aTruck.Add(IDC_CMD_BTN_DELETE);

	m_aTruck.Add(IDC_CMD_USER_VEHI_LANE_GRP);
	m_aTruck.Add(IDC_CMD_USER_MVHL_L1_STC);
	m_aTruck.Add(IDC_CMD_USER_MVHL_L1_EDT);
	m_aTruck.Add(IDC_CMD_USER_MVHL_L1_UNT);
	m_aTruck.Add(IDC_CMD_USER_MVHL_W_STC);
	m_aTruck.Add(IDC_CMD_USER_MVHL_W_EDT);
	m_aTruck.Add(IDC_CMD_USER_MVHL_W_UNT);
	m_aTruck.Add(IDC_CMD_USER_MVHL_L2_STC);
	m_aTruck.Add(IDC_CMD_USER_MVHL_L2_EDT);
	m_aTruck.Add(IDC_CMD_USER_MVHL_L2_UNT);
	m_aTruck.Add(IDC_CMD_USER_MVHL_W2_STC);
	m_aTruck.Add(IDC_CMD_USER_MVHL_W2_EDT);
	m_aTruck.Add(IDC_CMD_USER_MVHL_W2_M1_STC);
	m_aTruck.Add(IDC_CMD_USER_MVHL_W2_L_EDT);
	m_aTruck.Add(IDC_CMD_USER_MVHL_W2_M2_STC);
	m_aTruck.Add(IDC_CMD_USER_MVHL_W2_EXP_EDT);
	m_aTruck.Add(IDC_CMD_USER_MVHL_W2_UNT);
	m_aTruck.Add(IDC_CMD_MVHL_DYNA_ALLOW_STC);
	m_aTruck.Add(IDC_CMD_MVHL_DYNA_ALLOW_EDT);
	m_aTruck.Add(IDC_CMD_MVHL_DYNA_ALLOW_UNT);
	m_aTruck.Add(IDC_CMD_MVHL_VEHI_1ST_MODEL_RDO);
	m_aTruck.Add(IDC_CMD_MVHL_VEHI_2ND_MODEL_RDO);
	m_aTruck.Add(IDC_CMD_MVHL_VEHI_1ST_MODEL_EDT);
	m_aTruck.Add(IDC_CMD_MVHL_VEHI_1ST_MODEL_UNT);

	m_aTrain.Add(IDC_CMD_USER_VEHI_SUB_GRP1);		
	m_aTrain.Add(IDC_CMD_MVLD_TLGRID);
	m_aTrain.Add(IDC_CMD_USER_MVHL_TRAIN_W1_STC);
	m_aTrain.Add(IDC_CMD_USER_MVHL_TRAIN_W1_EDT);
	m_aTrain.Add(IDC_CMD_USER_MVHL_TRAIN_W1_UNT);
	m_aTrain.Add(IDC_CMD_USER_MVHL_TRAIN_D1_STC);
	m_aTrain.Add(IDC_CMD_USER_MVHL_TRAIN_D1_EDT);
	m_aTrain.Add(IDC_CMD_USER_MVHL_TRAIN_D1_UNT);
	m_aTrain.Add(IDC_CMD_USER_MVHL_TRAIN_W2_STC);
	m_aTrain.Add(IDC_CMD_USER_MVHL_TRAIN_W2_EDT);
	m_aTrain.Add(IDC_CMD_USER_MVHL_TRAIN_W2_UNT);
	m_aTrain.Add(IDC_CMD_USER_MVHL_TRAIN_D2_STC);
	m_aTrain.Add(IDC_CMD_USER_MVHL_TRAIN_D2_EDT);
	m_aTrain.Add(IDC_CMD_USER_MVHL_TRAIN_D2_UNT);		

	m_aLane.Add(IDC_CMD_USER_VEHI_SUB_GRP2);
	m_aLane.Add(IDC_CMD_USER_MVHL_NUM_STC);
	m_aLane.Add(IDC_CMD_USER_MVHL_NUM_CMB);
	m_aLane.Add(IDC_CMD_USER_MVHL_DIST_STC);
	m_aLane.Add(IDC_CMD_USER_MVHL_DIST_EDT);
	m_aLane.Add(IDC_CMD_USER_MVHL_DIST_UNT);
	m_aLane.Add(IDC_CMD_USER_MVHL_LOAD_W_STC);
	m_aLane.Add(IDC_CMD_USER_MVHL_LOAD_W_EDT);
	m_aLane.Add(IDC_CMD_USER_MVHL_LOAD_W_UNT);
	m_aLane.Add(IDC_CMD_USER_MVHL_L_STC);
	m_aLane.Add(IDC_CMD_USER_MVHL_L_EDT);
	m_aLane.Add(IDC_CMD_USER_MVHL_L_UNT);

	m_aEtc.RemoveAll();
	m_aEtc.Add(IDOK);
	m_aEtc.Add(IDCANCEL);
	m_aEtc.Add(IDC_CMD_APPLY);

	m_bConvertDistLoad = FALSE;
	m_nLengthLane = 0;
}

void CCMMvhlItemUsrKRLRFD11Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Radio  (pDX, IDC_CMD_USER_VEHI_TRUCK_RDO,         m_nLoadType);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_NAME_EDT,          m_edtVehicleName);
	DDX_Check  (pDX, IDC_CMD_MVHL_CONVERT_DIST_LOAD_CHK,  m_bConvertDistLoad);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_CHK,              m_chkPatch);

	DDX_Control(pDX, IDC_CMD_USER_VEHI_TRUCK_P_EDT,		    m_edtTrkP);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_TRUCK_D_EDT,		    m_edtTrkD);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_TRUCK_L_EDT,       m_edtTrkL);
	DDX_Control(pDX, IDC_CMD_LIST,				                m_List);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_L1_EDT,            m_edtLaneL1);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_L1_UNT,            m_untLaneL1);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_W_EDT,             m_edtLaneW);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_W_UNT,             m_untLaneW);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_L2_EDT,            m_edtLaneL2);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_L2_UNT,            m_untLaneL2);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_W2_EDT,            m_edtLaneW2);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_W2_L_EDT,          m_edtLaneW2L);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_W2_EXP_EDT,        m_edtLaneW2Exp);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_W2_UNT,            m_untLaneW2);

	DDX_Control(pDX, IDC_CMD_USER_MVHL_TRAIN_W1_EDT,      m_edtTrainW1);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_TRAIN_W1_UNT,      m_untTrainW1);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_TRAIN_D1_EDT,      m_edtTrainD1);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_TRAIN_D1_UNT,      m_untTrainD1);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_TRAIN_W2_EDT,      m_edtTrainW2);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_TRAIN_W2_UNT,      m_untTrainW2);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_TRAIN_D2_EDT,      m_edtTrainD2);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_TRAIN_D2_UNT,      m_untTrainD2);
																										    
	DDX_Control(pDX, IDC_CMD_USER_MVHL_NUM_CMB,           m_cmbLaneNum);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_DIST_EDT,          m_edtLaneDist);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_DIST_UNT,          m_untLaneDist);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_LOAD_W_EDT,        m_edtLoadW);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_LOAD_W_UNT,        m_untLoadW);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_L_EDT,             m_edtLaneL);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_L_UNT,             m_untLaneL);
																										    
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_ALLOW_EDT,         m_edtDynaAllow);
	DDX_Radio  (pDX, IDC_CMD_MVHL_VEHI_1ST_MODEL_RDO,     m_nLengthLane);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_1ST_MODEL_EDT,     m_edt1stModel);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_1ST_MODEL_UNT,     m_unt1stModel);
	DDX_Control(pDX, IDC_CMD_MVLD_TLGRID, m_TrainGrid);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_PICTURE, m_wndPicture);
}

BEGIN_MESSAGE_MAP(CCMMvhlItemUsrKRLRFD11Dlg, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_USER_VEHI_TRUCK_RDO, OnCmdMvhlLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_USER_VEHI_TRAIN_RDO, OnCmdMvhlLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_USER_VEHI_LANE_RDO,  OnCmdMvhlLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_CONVERT_DIST_LOAD_CHK,     OnCmdMvhlConvertDistLoadChk)
	ON_CBN_SELCHANGE(IDC_CMD_USER_MVHL_NUM_CMB, OnCmdMlMvhlLaneNumCmb)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD,    OnCmdMvhlAddBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_INSERT, OnCmdMvhlInsBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdMvhlModBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdMvhlDelBtn)
	ON_BN_CLICKED(IDC_CMD_APPLY,      OnCmdMvhlApplyBtn)
	ON_EN_CHANGE(IDC_CMD_USER_MVHL_L1_EDT, OnCmdMvhlChangeL1Edt)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST, OnItemchangedCmdList)
	ON_BN_CLICKED(IDC_CMD_MVHL_VEHI_1ST_MODEL_RDO, OnCmdMvhlDgnLengthLaneRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_VEHI_2ND_MODEL_RDO, OnCmdMvhlDgnLengthLaneRdo)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void CCMMvhlItemUsrKRLRFD11Dlg::InitUnit()
{
	m_edtLaneL1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untLaneL1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtLaneW.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untLaneW.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_edtLaneL2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untLaneL2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtLaneW2.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_edtLaneW2L.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtLaneW2Exp.SetUnitType(D_UNITSYS_NONE);
	m_untLaneW2.SetUnitType(D_UNITSYS_BASE_UNITFORCE);

	m_edtTrainW1.SetUnitType(D_UNITSYS_BASE_UNITLOAD);
	m_untTrainW1.SetUnitType(D_UNITSYS_BASE_UNITLOAD);
	m_edtTrainD1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTrainD1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtTrainW2.SetUnitType(D_UNITSYS_BASE_UNITLOAD);
	m_untTrainW2.SetUnitType(D_UNITSYS_BASE_UNITLOAD);
	m_edtTrainD2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTrainD2.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtLaneDist.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untLaneDist.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtLoadW.SetUnitType(D_UNITSYS_BASE_UNITLOAD);
	m_untLoadW.SetUnitType(D_UNITSYS_BASE_UNITLOAD);
	m_edtLaneL.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untLaneL.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edt1stModel.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unt1stModel.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMMvhlItemUsrKRLRFD11Dlg::InitCombo()
{
	m_cmbLaneNum.ResetContent();

	m_cmbLaneNum.AddString(_T("1"));
	m_cmbLaneNum.AddString(_T("2"));
	m_cmbLaneNum.AddString(_T("3"));
	m_cmbLaneNum.AddString(_T("4"));
	m_cmbLaneNum.AddString(_T("5"));
	m_cmbLaneNum.AddString(_T("6"));
	m_cmbLaneNum.AddString(_T("7"));
	m_cmbLaneNum.AddString(_T("8"));
	m_cmbLaneNum.AddString(_T("9"));
	m_cmbLaneNum.AddString(_T("10"));

	m_cmbLaneNum.SetCurSel(0);
}

void CCMMvhlItemUsrKRLRFD11Dlg::AlignControl()
{
	UpdateData(TRUE);

	CRect rRef;
	CRect rToMove;
	int nDistX = 0;
	int nDistY = 0;

	GetDlgItem(IDC_CMD_MVHL_CONVERT_DIST_LOAD_CHK)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_USER_VEHI_SUB_GRP1 )->GetWindowRect(rToMove);

	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top  - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aTrain, nDistX, nDistY);

	GetDlgItem(IDC_CMD_MVHL_CONVERT_DIST_LOAD_CHK)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_USER_VEHI_SUB_GRP2 )->GetWindowRect(rToMove);

	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top  - rToMove.top;		
	CDlgUtil::CtrlMoveDistXY(this, m_aLane, nDistX, nDistY);

	// Group Box 크기
	CRect rectTarget;
	CRect rect;
	CRect rectShort;

	GetDlgItem(IDC_CMD_USER_VEHI_GRP)->GetWindowRect(rectShort);

	GetDlgItem(IDC_CMD_USER_VEHI_NAME_EDT)->GetWindowRect(rectTarget);

	if  (m_nLoadType == 0) GetDlgItem(IDC_CMD_USER_VEHI_LANE_GRP)->GetWindowRect(rect);
	else                   GetDlgItem(IDC_CMD_USER_VEHI_SUB_GRP1)->GetWindowRect(rect);
	nDistY = rect.bottom - rectTarget.top + globalUtils.ScaleByDPI(30);

	rectShort.bottom = rectShort.top + nDistY;

	ScreenToClient(rectShort);
	GetDlgItem(IDC_CMD_USER_VEHI_GRP)->MoveWindow(rectShort);
	
	GetDlgItem(IDC_CMD_USER_VEHI_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

void CCMMvhlItemUsrKRLRFD11Dlg::ControlsShowHide()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlShowHide(this, m_aTruck, m_nLoadType==0);
	CDlgUtil::CtrlShowHide(this, m_aTrain, m_nLoadType==1);
	CDlgUtil::CtrlShowHide(this, m_aLane,  m_nLoadType==2);
}

void CCMMvhlItemUsrKRLRFD11Dlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__No), _LS(IDS_WG_CMD__ADDD__Load), _LS(IDS_WG_CMD__ADDD__Spacing), _LS(IDS_WG_CMD__ADDD__Length) };

	// title에 단위 추가
	CString sLdUnit,sDtUnit;
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointLoad,sLdUnit);
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointDistance,sDtUnit);
	aTitle[1] = aTitle[1] +_T("(")+sLdUnit+_T(")");
	aTitle[2] = aTitle[2] +_T("(")+sDtUnit+_T(")");
	aTitle[3] = aTitle[3] +_T("(")+sDtUnit+_T(")");

	int nColWidth[COLCOUNT];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 34; nColWidth[1] = 83; nColWidth[2] = 83; nColWidth[3] = 83;

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

void CCMMvhlItemUsrKRLRFD11Dlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();

	if (nLoadCount == 0) return;
	int i = 0;
	for (i = 0; i < nDistCount; i++)
		InsertItem(i, i+1, m_Data.dPointLoad[i], m_Data.dPointDistance[i], m_Data.dPointDistance2[i]);

	if (nLoadCount > nDistCount)
		InsertItem(i, i+1, m_Data.dPointLoad[i], 0.0, m_Data.dPointDistance2[i]);
}

BOOL CCMMvhlItemUsrKRLRFD11Dlg::InsertItem(int nPos, int nNo, double dLoad, double dDist, double dLength)
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
		else if (i == 3) str.Format(_T("%g"), dLength);
		else str = _LS(IDS_WG_CMD__ADDD__Error);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if(i == 0) m_List.InsertItem(&lvitem);
		else       m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CCMMvhlItemUsrKRLRFD11Dlg::DeleteItem(int nPos)
{
	if (nPos != -1) m_List.DeleteItem(nPos);
	return TRUE;
}

BOOL CCMMvhlItemUsrKRLRFD11Dlg::ModifyItem(int nPos, int nNo, double dLoad, double dDist, double dLength)
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
			else if (i == 3) str.Format(_T("%g"), dLength);
			else str = _LS(IDS_WG_CMD__ADDD__Error);
			m_List.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

void CCMMvhlItemUsrKRLRFD11Dlg::ChangeBitMap()
{	
	CString aBitmapID[]   = {_T("cmd_mvhl_206.svg"), _T("cmd_mvhc2.svg"), _T("cmd_mvhc8-kr.svg"), _T("cmd_mvhc9-kr.svg"), _T("cmd_mvhl_205.svg")};

	UpdateData(TRUE);

	int nBitmap = 0;
	int nLaneCombo = m_cmbLaneNum.GetCurSel();

	if(m_nLoadType == 0) 
	{
		if(m_bConvertDistLoad) nBitmap = 4;
		else                   nBitmap = 0;
	}		
	else if(m_nLoadType == 1) nBitmap = 1;
	else                     
	{
		if(nLaneCombo == 0) nBitmap = 2;
		else				nBitmap = 3;
	}

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + aBitmapID[nBitmap]);
}

void CCMMvhlItemUsrKRLRFD11Dlg::Data2Dlg()
{
	m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);

	m_nLoadType = 0;

	if     (m_Data.nLoadType==8) m_nLoadType = 0;
	else if(m_Data.nLoadType==1) m_nLoadType = 1;
	else                         m_nLoadType = 2;

	m_chkPatch.SetCheck(FALSE);
	if(m_nLoadType==0)
	{
		m_bConvertDistLoad = m_Data.bConvertDistLoad;
		m_chkPatch.SetCheck(m_Data.PatchLoad.bPatchLoad);

		m_edtLaneL1.SetEditUnit(m_Data.dLoadedLength);
		m_edtLaneW.SetEditUnit(m_Data.dW1);
		m_edtLaneL2.SetEditUnit(m_Data.dLoadedLength); // L1 과 동일하게 Setting
		m_edtLaneW2.SetEditUnit(m_Data.dW2);
		m_edtLaneW2L.SetEditUnit(m_Data.dLoadedLength); // L1 과 동일하게 Setting
		m_edtLaneW2Exp.SetEditUnit(m_Data.dExp);	

		m_edtDynaAllow.SetEditUnit(m_Data.dDynLoadAllowance);
		m_nLengthLane = m_Data.nLengthLane;
		m_edt1stModel.SetEditUnit(m_Data.dLengthLaneUser);
	}
	else if(m_nLoadType==1)
	{
		m_TrainGrid.MakeItemEx();

		m_edtTrainW1.SetEditUnit(m_Data.dW1);
		m_edtTrainD1.SetEditUnit(m_Data.dD1);
		m_edtTrainW2.SetEditUnit(m_Data.dW2);
		m_edtTrainD2.SetEditUnit(m_Data.dD2);
	}
	else
	{
		m_cmbLaneNum.SetCurSel(m_Data.nUniformLoadNum);
		m_edtLaneDist.SetEditUnit(m_Data.dUniformLoadDistance);
		m_edtLoadW.SetEditUnit(m_Data.dUniformLoadW);
		m_edtLaneL.SetEditUnit(m_Data.dUniformLoadLengthL);
	}

	MakeItemEx();

	UpdateData(FALSE);
}

BOOL CCMMvhlItemUsrKRLRFD11Dlg::Dlg2Data()
{	
	UpdateData(TRUE);  
	m_Data.bStandard = FALSE;  
	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);	
	m_Data.PatchLoad.Initialize();
	if(m_nLoadType==0)
	{
		m_Data.nLoadType = 8;

		m_Data.bConvertDistLoad = m_bConvertDistLoad;
		m_Data.dLoadedLength = m_edtLaneL1.GetEditValue();
		m_Data.dW1 = m_edtLaneW.GetEditValue();
		m_Data.dW2 = m_edtLaneW2.GetEditValue();
		m_Data.dExp = m_edtLaneW2Exp.GetEditValue();

		m_Data.dDynLoadAllowance = m_edtDynaAllow.GetEditValue();
		m_Data.nLengthLane = m_nLengthLane;
		m_Data.dLengthLaneUser = m_edt1stModel.GetEditValue();

		m_Data.PatchLoad.bPatchLoad = m_chkPatch.GetCheck();
		if(m_Data.PatchLoad.bPatchLoad)
		{
			// CIVIL-9473 참고
			int nLoadCount = GetLoadCount();
			for (int i = 0; i < nLoadCount; i++)
			{
				// kN
				double dLoad = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_FORCE, m_Data.dPointLoad[i]);

				// mm^2
				T_MVHL_PATCH_ITEM Patch;
				double dArea = 12500.0 / 9.0 * dLoad/2.0;
				double dL = sqrt(dArea / 2.5);
				double dW = dL * 2.5;
				Patch.dWidth  = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, dW);
				Patch.dLength = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, dL);
				Patch.nPatchType = i+1;
				m_Data.PatchLoad.aPatchLoad.Add(Patch);
			}
		}
	}
	else if(m_nLoadType==1)
	{
		m_Data.nLoadType = 1;
		m_Data.nTrainType = 0;

		if (!m_TrainGrid.OnTerminate(D_TB_SAVE_ALWAYS)) return FALSE;

		m_Data.dW1 = m_edtTrainW1.GetEditValue();
		m_Data.dD1 = m_edtTrainD1.GetEditValue();
		m_Data.dW2 = m_edtTrainW2.GetEditValue();
		m_Data.dD2 = m_edtTrainD2.GetEditValue();

		m_Data.dDynLoadAllowance = 0.0;
		m_Data.nLengthLane = 0;
		m_Data.dLengthLaneUser = 0.0;
	}
	else
	{
		m_Data.nLoadType = 6;

		m_Data.nUniformLoadNum = m_cmbLaneNum.GetCurSel();
		m_Data.dUniformLoadDistance = m_edtLaneDist.GetEditValue();
		m_Data.dUniformLoadW = m_edtLoadW.GetEditValue();
		m_Data.dUniformLoadLengthL = m_edtLaneL.GetEditValue();

		m_Data.dDynLoadAllowance = 0.0;
		m_Data.nLengthLane = 0;
		m_Data.dLengthLaneUser = 0.0;
	}

	return TRUE;
}

int CCMMvhlItemUsrKRLRFD11Dlg::GetLoadCount()
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemUsrKRLRFD11Dlg::GetDistCount()
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}
	return i;
}

BOOL CCMMvhlItemUsrKRLRFD11Dlg::GetLoadAndDistValue(double &dLoad, double &dDist, double &dLength)
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

	if (!CFormulaEdit::GetEditValue(&m_edtTrkL, dLength))
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

BOOL CCMMvhlItemUsrKRLRFD11Dlg::ApplyOrOK()
{
	BOOL bSuccess = FALSE;

	if(!Dlg2Data()) return FALSE;

	if (m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else					 bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);

	if (!bSuccess) return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrKRLRFD11Dlg message handlers

BOOL CCMMvhlItemUsrKRLRFD11Dlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	if(!m_bModify)
	{
		m_Data.nLoadType = 8; // Truck Load 의 Load Type이 8번
		m_Data.dLoadedLength = M_InitValueCurUnit(60.0, KN, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dW1 = M_InitValueCurUnit(12.7, KN, M, D_UNITSYS_BASE_UNITFORCE);
		m_Data.dW2 = M_InitValueCurUnit(12.7, KN, M, D_UNITSYS_BASE_UNITFORCE);
		m_Data.dExp = 0.1;
		m_Data.dDynLoadAllowance = 25.0;
		m_Data.dLengthLaneUser = 0.;
	}

	// grid 초기화 
	m_TrainGrid.Initialize(&m_Data);

	// always show vertical scrollbar
	m_TrainGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	InitUnit();
	InitCombo();
	SetHeaderTitle();

	Data2Dlg();

	UpdateData(FALSE);

	AlignControl();
	ControlsShowHide();

	ChangeBitMap();

	OnCmdMvhlLoadTypeRdo();
	OnCmdMvhlConvertDistLoadChk();
	OnCmdMlMvhlLaneNumCmb();
	EnableDisableCtrl();

	if(m_bModify)	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemUsrKRLRFD11Dlg::OnOK() 
{
	if (!ApplyOrOK()) return;

	CDialogMove::OnOK();
}

void CCMMvhlItemUsrKRLRFD11Dlg::OnCmdMvhlLoadTypeRdo()
{
	UpdateData(TRUE);

	ChangeBitMap();
	AlignControl();
	ControlsShowHide();
} 

void CCMMvhlItemUsrKRLRFD11Dlg::OnCmdMvhlConvertDistLoadChk()
{
	UpdateData(TRUE);

	ChangeBitMap();

	GetDlgItem(IDC_CMD_USER_VEHI_TRUCK_L_STC)->EnableWindow(m_bConvertDistLoad);
	GetDlgItem(IDC_CMD_USER_VEHI_TRUCK_L_EDT)->EnableWindow(m_bConvertDistLoad);
}

void CCMMvhlItemUsrKRLRFD11Dlg::OnCmdMlMvhlLaneNumCmb()
{
	UpdateData(TRUE);

	int nIndex = m_cmbLaneNum.GetCurSel();

	if(nIndex == 0) // Load Num = 1
	{
		GetDlgItem(IDC_CMD_USER_MVHL_DIST_STC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_USER_MVHL_DIST_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_USER_MVHL_DIST_UNT)->EnableWindow(FALSE);
	}
	else // Load Num = 2~10
	{
		GetDlgItem(IDC_CMD_USER_MVHL_DIST_STC)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_USER_MVHL_DIST_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_USER_MVHL_DIST_UNT)->EnableWindow(TRUE);
	}

	ChangeBitMap();
}

void CCMMvhlItemUsrKRLRFD11Dlg::OnCmdMvhlAddBtn() 
{
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();

	double dLoad, dDist, dLength;
	if (!GetLoadAndDistValue(dLoad, dDist, dLength)) return;

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
		m_Data.dPointDistance2[nLoadCount] = dLength;
	}
	else
	{
		m_Data.dPointLoad[nLoadCount] = dLoad;
		m_Data.dPointDistance[nLoadCount] = dDist;
		m_Data.dPointDistance2[nLoadCount] = dLength;
	}

	int no = nLoadCount+1;
	InsertItem(nLoadCount, no, dLoad, dDist, dLength);

	int nCount = m_List.GetItemCount();
	m_List.SetItemState(nCount-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUsrKRLRFD11Dlg::OnCmdMvhlInsBtn() 
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

	double dLoad, dDist, dLength;
	if (!GetLoadAndDistValue(dLoad, dDist,dLength)) return;

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
		m_Data.dPointDistance2[i] = m_Data.dPointDistance2[i-1];
	}

	// 데이타 삽입한다.
	m_Data.dPointLoad[iItem] = dLoad;
	m_Data.dPointDistance[iItem] = dDist;
	m_Data.dPointDistance2[iItem] = dLength;

	// List에 Item 삽입
	InsertItem(iItem, iItem+1, dLoad, dDist, dLength);

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

void CCMMvhlItemUsrKRLRFD11Dlg::OnCmdMvhlModBtn() 
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

	double dLoad, dDist, dLength;
	if (!GetLoadAndDistValue(dLoad, dDist, dLength)) return;

	if (iItem != nLoadCount-1 && dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}

	m_Data.dPointLoad[iItem] = dLoad;
	m_Data.dPointDistance[iItem] = dDist;
	m_Data.dPointDistance2[iItem] = dLength;

	ModifyItem(iItem, iItem+1, dLoad, dDist, dLength);
}

void CCMMvhlItemUsrKRLRFD11Dlg::OnCmdMvhlDelBtn() 
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
		m_Data.dPointDistance2[i] = m_Data.dPointDistance2[i+1];
	}
	m_Data.dPointLoad[i] = 0.0;
	m_Data.dPointDistance[i] = 0.0;
	m_Data.dPointDistance2[i] = 0.0;

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

void CCMMvhlItemUsrKRLRFD11Dlg::OnCmdMvhlApplyBtn() 
{
	ApplyOrOK();
}

void CCMMvhlItemUsrKRLRFD11Dlg::OnCmdMvhlChangeL1Edt()
{
	UpdateData(TRUE);

	double dL1 = m_edtLaneL1.GetEditValue();
	m_edtLaneL2.SetEditUnit(dL1);
	m_edtLaneW2L.SetEditUnit(dL1);
}

void CCMMvhlItemUsrKRLRFD11Dlg::OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
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
		csVal.Format(_T("%g"), m_Data.dPointDistance2[iItem]);
		m_edtTrkL.SetWindowText(csVal);
	}
	else
	{
		csVal.Format(_T("%g"), m_Data.dPointLoad[iItem]);
		m_edtTrkP.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.dPointDistance[iItem]);
		m_edtTrkD.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.dPointDistance2[iItem]);
		m_edtTrkL.SetWindowText(csVal);
	}

	*pResult = 0;
}

void CCMMvhlItemUsrKRLRFD11Dlg::OnCmdMvhlDgnLengthLaneRdo() 
{
	UpdateData(TRUE);
	EnableDisableCtrl();
}

void CCMMvhlItemUsrKRLRFD11Dlg::EnableDisableCtrl() 
{
	CArray<UINT, UINT> a1St;
	a1St.Add(IDC_CMD_MVHL_VEHI_1ST_MODEL_EDT);
	a1St.Add(IDC_CMD_MVHL_VEHI_1ST_MODEL_UNT);
	CDlgUtil::CtrlEnableDisable(this, a1St, m_nLengthLane==0);
}