// CMMvhlItemTr.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemTr.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MvhlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemTr dialog


CCMMvhlItemTr::CCMMvhlItemTr(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemTr::IDD, pParent)
{
	m_pDoc		= 0;
	m_bModify = FALSE;

	//{{AFX_DATA_INIT(CCMMvhlItemTr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInit = TRUE;
}


void CCMMvhlItemTr::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemTr)
	//DDX_Control(pDX, IDC_CMD_NUM_LANES_SPIN, m_spnNum);
	DDX_Control(pDX, IDC_CMD_WIDTH_UNIT, m_trWidthUnit);
	DDX_Control(pDX, IDC_CMD_WHEEL_LOAD_UNIT, m_trWheelLoadUnit);
	DDX_Control(pDX, IDC_CMD_MIN_DIST_UNIT, m_trMinDistUnit);
	DDX_Control(pDX, IDC_CMD_LOCATION_UNIT, m_trLocationUnit);
	DDX_Control(pDX, IDC_CMD_EDGE_DIST_UNIT, m_trEdgeDistUnit);
	DDX_Control(pDX, IDC_CMD_DIST_WIDTH_UNIT, m_trDistWidthUnit);
	DDX_Control(pDX, IDC_CMD_LONG_WIDTH_UNIT, m_trLongWidthUnit); // MNET:XXXX 20071005 mylee
	DDX_Control(pDX, IDC_CMD_DIST_WHEELS_UNIT, m_trDistWheelsUnit);
	DDX_Control(pDX, IDC_CMD_EDGE_DIST_TEXT, m_trEdgeDist);
	DDX_Control(pDX, IDC_CMD_DIST_WHEELS_TEXT, m_trDistWheels);
	DDX_Control(pDX, IDC_CMD_DIST_WIDTH_TEXT, m_trDistWidth);
	DDX_Control(pDX, IDC_CMD_LONG_WIDTH_TEXT, m_trLongWidth); // MNET:XXXX 20071005 mylee
	DDX_Control(pDX, IDC_CMD_LOCATION_TEXT, m_trLocation);
	DDX_Control(pDX, IDC_CMD_MIN_DIST_TEXT, m_trMinDist);
	DDX_Control(pDX, IDC_CMD_MVHL_NAME, m_trMvhlName);
	DDX_Control(pDX, IDC_CMD_NUM_LANES_TEXT, m_trNumLanes);
	DDX_Control(pDX, IDC_CMD_WHEEL_LOAD_TEXT, m_trWheelLoad);
	DDX_Control(pDX, IDC_CMD_WIDTH_TEXT, m_trWidth);
	DDX_Control(pDX, IDC_CMD_MEDIAN_CHK, m_chkMedianStrip);
	DDX_Control(pDX, IDC_CMD_NUM_LEFT_LANES_EDT, m_edtLeftLaneNum);
	//DDX_Control(pDX, IDC_CMD_NUM_LEFT_LANES_SPN, m_spnLeftLaneNum);
	DDX_Control(pDX, IDC_CMD_NUM_RIGHT_LANES_EDT, m_edtRightLaneNum);
	DDX_Control(pDX, IDC_CMD_MVHL_PIC_WND, m_wndPicture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvhlItemTr, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemTr)
	ON_EN_CHANGE(IDC_CMD_NUM_LEFT_LANES_EDT, OnChangeCmdNumLeftLanesEdt)
	ON_EN_CHANGE(IDC_CMD_NUM_LANES_TEXT, OnChangeCmdNumLanesText)
	ON_BN_CLICKED(IDC_CMD_MEDIAN_CHK, OnMedianStripCheck)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemTr message handlers

BOOL CCMMvhlItemTr::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	m_bInit = FALSE;
	
	if(!m_bModify)
	{
		m_Data.Initialize();

		CString csLengUnit;
		m_pDoc->m_pUnitCtrl->GetUnitSystemLength(csLengUnit);
		if(csLengUnit==D_UNITSYS_LENGTH_NAME_IN || csLengUnit==D_UNITSYS_LENGTH_NAME_FT)
		{
			m_Data.dLongW = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(
				D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 0.3048);
		}
		else
		{
			m_Data.dLongW = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(
				D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 1.);
		}

	}
	m_trNumLanes.SetRange(1,1000);
	m_trNumLanes.SetValue(m_Data.nNum);
	m_trNumLanes.SetInteger(TRUE);

	m_edtLeftLaneNum.SetRange(1, 1000);
	m_edtLeftLaneNum.SetValue(m_Data.nLeftLaneNum);
	m_edtLeftLaneNum.SetInteger(TRUE);

	m_trWidthUnit.SetUnitType(CUnitCtrl::m_MVHLtr_UNIT.dMw);
	m_trWheelLoadUnit.SetUnitType(CUnitCtrl::m_MVHLtr_UNIT.dP);
	m_trMinDistUnit.SetUnitType(CUnitCtrl::m_MVHLtr_UNIT.dDv);
	m_trLocationUnit.SetUnitType(CUnitCtrl::m_MVHLtr_UNIT.dMl);
	m_trEdgeDistUnit.SetUnitType(CUnitCtrl::m_MVHLtr_UNIT.dDe);
	m_trDistWidthUnit.SetUnitType(CUnitCtrl::m_MVHLtr_UNIT.dW);
	m_trLongWidthUnit.SetUnitType(CUnitCtrl::m_MVHLtr_UNIT.dW);
	m_trDistWheelsUnit.SetUnitType(CUnitCtrl::m_MVHLtr_UNIT.dDw);
	m_edtRightLaneNum.SetUnitType(0);

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_mvhl_46.svg"));

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CCMMvhlItemTr::Data2Dlg()
{
	m_chkMedianStrip.SetCheck(m_Data.bMedianStrip);
	OnMedianStripCheck();

	m_trWidth.SetEditUnit(m_Data.dMw);
	m_trWheelLoad.SetEditUnit(m_Data.dP);
	m_trMinDist.SetEditUnit(m_Data.dDv);
	m_trLocation.SetEditUnit(m_Data.dMl);
	m_trEdgeDist.SetEditUnit(m_Data.dDe);
	m_trDistWidth.SetEditUnit(m_Data.dW);
	m_trLongWidth.SetEditUnit(m_Data.dLongW);
	m_trDistWheels.SetEditUnit(m_Data.dDw);
	m_trMvhlName.SetWindowText(m_Data.VehicleLoadName);

	m_trNumLanes.SetValue(m_Data.nNum);
	m_edtLeftLaneNum.SetValue(m_Data.nLeftLaneNum);
	m_edtRightLaneNum.SetEditUnit(m_Data.nNum-m_Data.nLeftLaneNum);
	 
}
BOOL CCMMvhlItemTr::Dlg2Data()
{
	m_Data.Initialize();  
	m_trMvhlName.GetWindowText(m_Data.VehicleLoadName);
	
	m_Data.nNum=m_trNumLanes.GetEditValueInt();
	m_Data.bMedianStrip=m_chkMedianStrip.GetCheck();
	m_Data.nLeftLaneNum = m_edtLeftLaneNum.GetEditValueInt();
	m_Data.dMw=m_trWidth.GetEditValue();
	m_Data.dP=m_trWheelLoad.GetEditValue();
	m_Data.dDv=m_trMinDist.GetEditValue();
	m_Data.dMl=m_trLocation.GetEditValue();
	m_Data.dDe=m_trEdgeDist.GetEditValue();
	m_Data.dW=m_trDistWidth.GetEditValue();
	m_Data.dLongW=m_trLongWidth.GetEditValue();
	m_Data.dDw=m_trDistWheels.GetEditValue();

	//CVehlDB db(m_pDoc);
	//if(!db.GetStandardVehicleLoadValue(m_Data))  return FALSE;  
	
	return TRUE;
}

BOOL CCMMvhlItemTr::ApplyOrOK()
{
	if(!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if(m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhltr(m_csOldName, m_Data);
	else          bSuccess = m_pDoc->m_pDataCtrl->AddMvhltr(m_Data);
	if(!bSuccess) return FALSE;

	return TRUE;
}

void CCMMvhlItemTr::OnOK() 
{
	// TODO: Add extra validation here
	
	 if (!ApplyOrOK()) return;	
	CDialogMove::OnOK();
}

void CCMMvhlItemTr::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}

void CCMMvhlItemTr::OnCmdApply() 
{	
	ApplyOrOK();
}

void CCMMvhlItemTr::OnChangeCmdNumLeftLanesEdt() 
{
	if(m_bInit) return;
	BOOL bMedianStrip = m_chkMedianStrip.GetCheck();
	if(!bMedianStrip) return;

	int nNum = m_trNumLanes.GetEditValueInt();
	int nLeftNum = m_edtLeftLaneNum.GetEditValueInt();
	int nRightNum = nNum-nLeftNum;
	m_edtRightLaneNum.SetEditUnit(nRightNum);
}

void CCMMvhlItemTr::OnChangeCmdNumLanesText() 
{
	if(m_bInit) return;
	BOOL bMedianStrip = m_chkMedianStrip.GetCheck();
	if(!bMedianStrip) return;

	int nNum = m_trNumLanes.GetEditValueInt();
	int nLeftNum = m_edtLeftLaneNum.GetEditValueInt();
	int nRightNum = nNum-nLeftNum;
	m_edtRightLaneNum.SetEditUnit(nRightNum);
}

void CCMMvhlItemTr::OnMedianStripCheck() 
{
	BOOL bMedianStrip = m_chkMedianStrip.GetCheck();
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_MEDIAN_STRIP, bMedianStrip, FALSE);
	GetDlgItem(IDC_CMD_MEDIAN_CHK)->EnableWindow(TRUE);
	if(!bMedianStrip) return;

	int nNum = m_trNumLanes.GetEditValueInt();
	int nLeftNum = m_edtLeftLaneNum.GetEditValueInt();
	int nRightNum = nNum-nLeftNum;
	m_edtRightLaneNum.SetEditUnit(nRightNum);
}
