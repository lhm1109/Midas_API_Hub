// CMMvhlItemStdSouthAfricaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdSouthAfricaDlg.h"

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
// CCMMvhlItemStdSouthAfricaDlg dialog

CCMMvhlItemStdSouthAfricaDlg::CCMMvhlItemStdSouthAfricaDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemStdSouthAfricaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdSouthAfricaDlg)	
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	m_Data.Initialize();
	m_nStndCode = 0;
	m_bModify = FALSE;

	m_bIncreLength = FALSE;
	m_bOpposite = FALSE;

	m_aCtrl_NA.RemoveAll();
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_W1_STC);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_W2_STC);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_PA_STC);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_INCRE_LENGTH_CHK);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_INCRE_LENGTH_EDT);
	m_aCtrl_NA.Add(IDC_CMD_MVHL_NA_INCRE_LENGTH_UNT);

	m_aCtrl_NB.RemoveAll();
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_PB_STC);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_PB_EDT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_PB_UNT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_NUM_STC);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_NUM_CMB);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D_STC);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D_EDT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D_UNT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D1_STC);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D1_EDT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D1_UNT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D2_STC);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D2_EDT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D2_UNT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D3_STC);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D3_EDT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D3_UNT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D4_STC);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D4_EDT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D4_UNT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D5_STC);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D5_EDT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D5_UNT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D6_STC);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D6_EDT);
	m_aCtrl_NB.Add(IDC_CMD_MVHL_NB_D6_UNT);

	m_aCtrl_NC.RemoveAll();
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_W_STC);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_W_EDT);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_W_UNT);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_GRP);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_A_STC);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_A_GRID);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_B_STC);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_B_GRID);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_C_STC);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_L_C_GRID);
	m_aCtrl_NC.Add(IDC_CMD_MVHL_NC_OPPOSITE_CHK);

	m_aCtrl_OKCancel.Add(IDOK);
	m_aCtrl_OKCancel.Add(IDCANCEL);
	m_aCtrl_OKCancel.Add(IDC_CMD_APPLY);
}

CCMMvhlItemStdSouthAfricaDlg::~CCMMvhlItemStdSouthAfricaDlg()
{
}

void CCMMvhlItemStdSouthAfricaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdSouthAfricaDlg)  
	DDX_Control(pDX, IDC_CMD_MVHL_CODE_CMB,             m_cmbCodeName);
	DDX_Control(pDX, IDC_CMD_MVHL_NAME_EDT,             m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_MVHL_TYPE_CMB,             m_cmbVehicleType);
	DDX_Check  (pDX, IDC_CMD_MVHL_NA_INCRE_LENGTH_CHK,  m_bIncreLength);
	DDX_Control(pDX, IDC_CMD_MVHL_NA_INCRE_LENGTH_EDT,  m_edtIncreLength);
	DDX_Control(pDX, IDC_CMD_MVHL_NA_INCRE_LENGTH_UNT,  m_untIncreLength);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_PB_EDT,            m_edtPb);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_PB_UNT,            m_untPb);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_NUM_CMB,           m_cmbNumUnit);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D_EDT,             m_edtd);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D_UNT,             m_untd);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D1_EDT,            m_edtD1);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D1_UNT,            m_untD1);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D2_EDT,            m_edtD2);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D2_UNT,            m_untD2);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D3_EDT,            m_edtD3);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D3_UNT,            m_untD3);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D4_EDT,            m_edtD4);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D4_UNT,            m_untD4);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D5_EDT,            m_edtD5);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D5_UNT,            m_untD5);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D6_EDT,            m_edtD6);
	DDX_Control(pDX, IDC_CMD_MVHL_NB_D6_UNT,            m_untD6);
	DDX_Control(pDX, IDC_CMD_MVHL_NC_W_EDT,             m_edtW);
	DDX_Control(pDX, IDC_CMD_MVHL_NC_W_UNT,             m_untW);
	DDX_Check  (pDX, IDC_CMD_MVHL_NC_OPPOSITE_CHK,      m_bOpposite);
	DDX_Control(pDX, IDC_CMD_MVHL_NC_L_A_GRID, m_NCGrid[0]);
	DDX_Control(pDX, IDC_CMD_MVHL_NC_L_B_GRID, m_NCGrid[1]);
	DDX_Control(pDX, IDC_CMD_MVHL_NC_L_C_GRID, m_NCGrid[2]);
	DDX_Control(pDX, IDC_CMD_MVHL_PIC_WND, m_wndPicture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvhlItemStdSouthAfricaDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemStdSouthAfricaDlg)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_TYPE_CMB,           OnCmdMvhlTypeCmb)
	ON_BN_CLICKED(IDC_CMD_MVHL_NA_INCRE_LENGTH_CHK,   OnCmdMvhlIncreLengthChk)
	ON_BN_CLICKED(IDC_CMD_APPLY,				              OnCmdMvhlApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemStdSouthAfricaDlg::InitUnit()
{
	m_edtVehicleName.SetUnitType(D_UNITSYS_NONE);

	m_edtIncreLength.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untIncreLength.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtPb.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_untPb.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_edtd .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untd .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtD1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untD1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtD2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untD2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtD3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untD3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtD4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untD4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtD5.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untD5.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtD6.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untD6.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtW.SetUnitType(D_UNITSYS_BASE_PRESSURE);
	m_untW.SetUnitType(D_UNITSYS_BASE_PRESSURE);
}

void CCMMvhlItemStdSouthAfricaDlg::InitCombo()
{
	m_cmbCodeName.ResetContent();
	m_cmbVehicleType.ResetContent();
	m_cmbNumUnit.ResetContent();

	int nCodeSeq = D_MVHL_SOUTH_AFRICA_TMH7_1981;
	CDlgUtil::CobxAddItem(m_cmbCodeName, CVehlDB::GetTypeNameAr()[nCodeSeq], nCodeSeq);

	int i = 0;
	while (i < D_MAX_COUNT && CVehlDB::GetTypeListAr()[nCodeSeq][i] != _T(""))
	{
		m_cmbVehicleType.AddString(CVehlDB::GetTypeListAr()[nCodeSeq][i++]);
	}
	m_cmbVehicleType.SetCurSel(0); 

	m_cmbNumUnit.AddString(_T("24"));
	m_cmbNumUnit.AddString(_T("36"));	
	m_cmbNumUnit.SetCurSel(1);
}

void CCMMvhlItemStdSouthAfricaDlg::InitControls()
{
	m_NCGrid[0].InitGrid();
	m_NCGrid[1].InitGrid();
	m_NCGrid[2].InitGrid();
}

void CCMMvhlItemStdSouthAfricaDlg::AlignControl()
{
	UpdateData(TRUE);

	int nIndex = m_cmbVehicleType.GetCurSel();

	CRect rRef;
	CRect rToMove;
	CRect rectGrp;
	int nDistY = 0;

	GetDlgItem(IDC_CMD_MVHL_NA_W1_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_NB_PB_STC)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_NB, nDistY);

	GetDlgItem(IDC_CMD_MVHL_NA_W1_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_NC_W_STC )->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_NC, nDistY);

	// Group Box 크기
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->GetWindowRect(rectGrp);

	if(nIndex == 0 || nIndex == 1)
	{
		GetDlgItem(IDC_CMD_MVHL_NB_D4_EDT)->GetWindowRect(rRef);
	}
	else
	{
		GetDlgItem(IDC_CMD_MVHL_NC_OPPOSITE_CHK)->GetWindowRect(rRef);
	}

	rectGrp.bottom = rRef.bottom + globalUtils.ScaleByDPI(16);
	ScreenToClient(rectGrp);
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->MoveWindow(rectGrp);

	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_OKCancel, nDistY);

	CRect r;
	GetWindowRect(r);
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rToMove);
	r.bottom = rToMove.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);

	RedrawWindow();
}

void CCMMvhlItemStdSouthAfricaDlg::ControlsShowHide()
{
	UpdateData(TRUE);

	int nIndex = m_cmbVehicleType.GetCurSel();

	CDlgUtil::CtrlShowHide(this, m_aCtrl_NA, nIndex==0);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_NB, nIndex==1);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_NC, nIndex==2);
}

void CCMMvhlItemStdSouthAfricaDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_MVHL_NA_INCRE_LENGTH_EDT)->EnableWindow(m_bIncreLength);
	GetDlgItem(IDC_CMD_MVHL_NA_INCRE_LENGTH_UNT)->EnableWindow(m_bIncreLength);
}

void CCMMvhlItemStdSouthAfricaDlg::ChangeBitmap(int nBitmap)
{
	CString aBitmapID[] = { _T("cmd_mvhl_199.svg"), _T("cmd_mvhl_200.svg"), _T("cmd_mvhl_201.svg")};

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + aBitmapID[nBitmap]);
}

void CCMMvhlItemStdSouthAfricaDlg::Data2Dlg()
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
	ChangeBitmap(sIndex);

	CVehlDB db(m_pDoc);  
	db.GetStandardVehicleLoadValue(m_Data);
	
	m_bIncreLength = m_Data.bIncreLength;
	m_edtIncreLength.SetEditUnit(m_Data.dIncreLength);
	m_edtPb.SetEditUnit(m_Data.dPb);

	if(m_Data.dUnitNum == 24.0) m_cmbNumUnit.SetCurSel(0);
	else                        m_cmbNumUnit.SetCurSel(1);

	m_edtd .SetEditUnit(m_Data.dDelta);
	m_edtD1.SetEditUnit(m_Data.dD1);
	m_edtD2.SetEditUnit(m_Data.dD2);	
	m_edtD3.SetEditUnit(m_Data.dD2 + m_Data.dDelta);
	m_edtD4.SetEditUnit(m_Data.dD2 + m_Data.dDelta * 2.);
	m_edtD5.SetEditUnit(m_Data.dD2 + m_Data.dDelta * 3.);
	m_edtD6.SetEditUnit(m_Data.dD2 + m_Data.dDelta * 4.);
	
	m_edtW.SetEditUnit(m_Data.dPressureLoad);	

	m_NCGrid[0].SetDataSource(&m_Data, 0, 1);
	m_NCGrid[1].SetDataSource(&m_Data, 1, 1);
	m_NCGrid[2].SetDataSource(&m_Data, 2, 1);

	m_NCGrid[0].SetEnable(FALSE);
	m_NCGrid[1].SetEnable(FALSE);
	m_NCGrid[2].SetEnable(FALSE);

	m_bOpposite = m_Data.bOpposite;

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdSouthAfricaDlg::Dlg2Data()
{
	UpdateData(TRUE);
	
	m_Data.Initialize();  
	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);   
	m_Data.bStandard = TRUE;
	m_cmbVehicleType.GetWindowText(m_Data.VehicleTypeName);
	
	m_Data.nStandardCode = m_nStndCode;

	CVehlDB db(m_pDoc);
	if(!db.GetStandardVehicleLoadValue(m_Data))  return FALSE;  

	m_Data.bIncreLength = m_bIncreLength;
	m_Data.dIncreLength = m_edtIncreLength.GetEditValue();

	if(m_cmbNumUnit.GetCurSel()==0) m_Data.dUnitNum = 24.0;
	else                            m_Data.dUnitNum = 36.0;

	m_Data.nNumLoad3[0] = 4;
	m_Data.nNumLoad3[1] = 3;
	m_Data.nNumLoad3[2] = 6;

	m_Data.bOpposite = m_bOpposite;

	return TRUE;
}

BOOL CCMMvhlItemStdSouthAfricaDlg::ApplyOrOK()
{
	if(!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if(m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else          bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);
	if(!bSuccess) return FALSE;
	
	return TRUE;
}

void CCMMvhlItemStdSouthAfricaDlg::GetIndex(CString &csName, int *sitem)
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
// CCMMvhlItemStdSouthAfricaDlg message handlers

BOOL CCMMvhlItemStdSouthAfricaDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	if(!m_bModify)
	{
		m_Data.Initialize();
		m_Data.bStandard = TRUE;    
		m_Data.VehicleTypeName = _T("NA");
		m_nStndCode          = D_MVHL_SOUTH_AFRICA_TMH7_1981;
		m_Data.nStandardCode = D_MVHL_SOUTH_AFRICA_TMH7_1981;
		m_Data.dIncreLength  = M_InitValueCurUnit( 1.0, KN, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dUnitNum      = 36.0;
		m_Data.nNumLoad3[0] = 4;
		m_Data.nNumLoad3[1] = 3;
		m_Data.nNumLoad3[2] = 6;
	}

	InitUnit();
	InitCombo();
	InitControls();
		
	Data2Dlg();

	AlignControl();
	ControlsShowHide();
	EnableDisableControls();
	
	if(m_bModify)	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemStdSouthAfricaDlg::OnOK() 
{
	if (!ApplyOrOK()) return;	
	CDialogMove::OnOK();
}

void CCMMvhlItemStdSouthAfricaDlg::OnCmdMvhlTypeCmb() 
{
	Dlg2Data();
	AlignControl();
	ControlsShowHide();
	EnableDisableControls();

	// VehlDB 에서 셋팅해줄수 없는 값을 초기값으로 셋팅
	m_Data.VehicleLoadName.Empty();
	m_Data.dIncreLength  = M_InitValueCurUnit( 1.0, KN, M, D_UNITSYS_BASE_LENGTH);
	m_Data.dUnitNum = 36.0;

	Data2Dlg();
}

void CCMMvhlItemStdSouthAfricaDlg::OnCmdMvhlIncreLengthChk() 
{
	EnableDisableControls();
}

void CCMMvhlItemStdSouthAfricaDlg::OnCmdMvhlApply() 
{	
	ApplyOrOK();
}