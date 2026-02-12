// CMMvhlItemStdBSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdBSDlg.h"

#include "CMMvhlItemStdBSBD3701HAPage.h"
#include "CMMvhlItemStdBSBD3701HBPage.h"
#include "CMMvhlItemStdBSBD3701HAHBPage.h"
#include "CMMvhlItemStdBSBD3701HAHBAutoPage.h"
#include "CMMvhlItemStdBSBD3701PedestrianPage.h"
#include "CMMvhlItemStdBSBS5400Page.h"
#include "CMMvhlItemStdBSCS454AllModel1Page.h"
#include "CMMvhlItemStdBSCS454AllModel2Page.h"
#include "CMMvhlItemStdBSCS458Page.h"
#include "CMMvhlItemStdBSNRRailEUDLPage.h"
#include "CMMvhlItemStdBSNRRailPage.h"
#include "CMMvhlItemStdBSNRWagonPage.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\SpecialChar.h"
#include "..\wg_base\wg_base_LocaleLib.h"

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
// CCMMvhlItemStdBSDlg dialog
#define COLCOUNT 3

CCMMvhlItemStdBSDlg::CCMMvhlItemStdBSDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemStdBSDlg::IDD, pParent)
{
	m_nRALoadingType = 0;

	m_pDoc          = 0;
	m_bModify       = FALSE;

	m_pBS3701HADlg = NULL;
	m_pBS3701HBDlg = NULL;
	m_pBS3701HAHBDlg = NULL;
	m_pBS3701HAHBAutoDlg = NULL;
	m_pBS3701PedestrianDlg = NULL;
	m_pBS5400Dlg = NULL;
	m_pBSCS454AllModel1Dlg = NULL;
	m_pBSCS454AllModel2Dlg = NULL;
	m_pBSCS458Dlg = NULL;
	m_pBSNRRailEUDLDlg = NULL;
	m_pBSNRRailDlg = NULL;
	m_pBSNRWagonDlg = NULL;
}

CCMMvhlItemStdBSDlg::~CCMMvhlItemStdBSDlg()
{
}

void CCMMvhlItemStdBSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdBSDlg)  
	DDX_Control(pDX, IDC_CMD_MVHL_CODE_CMB, m_cbxCodeName);
	DDX_Control(pDX, IDC_CMD_MVHL_TYPE_COMBO, m_wndVehicleTypeList);
	DDX_Control(pDX, IDC_CMD_MVHL_SEL_VEHICLE_CMB, m_cmbSelVihicle);
	DDX_Control(pDX, IDC_CMD_MVHL_NAME, m_edtVehicleName);
	DDX_Radio  (pDX, IDC_RA_TRAIN_RDO, m_nRALoadingType);
 	DDX_Control(pDX, IDC_CMD_MVHL_PIC_WND, m_wndPicture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvhlItemStdBSDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemStdBSDlg)
 	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
 	ON_CBN_SELCHANGE(IDC_CMD_MVHL_CODE_CMB,        OnSelchangeCmdMvhlCodeCmb)
 	ON_CBN_SELCHANGE(IDC_CMD_MVHL_TYPE_COMBO,      OnSelchangeCmdMvhlTypeCombo)
 	ON_CBN_SELCHANGE(IDC_CMD_MVHL_SEL_VEHICLE_CMB, OnCmdMvhlEuroSelVehicleCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSDlg message handlers

BOOL CCMMvhlItemStdBSDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	CreateBSLoadDlg();
	
	SetDefValAndCbxItem();

	int nLoadType, nSelVehicle;
	GetIndex(nLoadType, nSelVehicle);
	if (nLoadType < 0) return TRUE;
	// 중복된 이름이 있을 경우 Standard 를 따라가야 한다.
	if(m_Data.nStandardCode != nLoadType) nLoadType = m_Data.nStandardCode;

	SetLoadTypeList(nLoadType);
//	SetSelVehicleList();

 	Data2Dlg();

 	ChangeData(FALSE);  
 
 	if(m_bModify) GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//-------------------------------------------------------------------------
// Implementation
void CCMMvhlItemStdBSDlg::Data2Dlg()
{
	m_nStndCode = m_Data.nStandardCode;
	if(m_Data.VehicleLoadName.IsEmpty())
		m_edtVehicleName.SetWindowText(ConvVehicleNameStrRaw2Trans(m_Data.VehicleTypeName));
	else
		m_edtVehicleName.SetWindowText(ConvVehicleNameStrRaw2Trans(m_Data.VehicleLoadName));
	
	int nLoadType;
	int nSelVehicle;
	CDlgUtil::CobxSetCurSelItemData(m_cbxCodeName, m_nStndCode);
	ResetVehicleTypeCmb();
	GetIndex(nLoadType, nSelVehicle);
	m_wndVehicleTypeList.SetCurSel(nLoadType);  
	m_cmbSelVihicle.SetCurSel(nSelVehicle);

	m_nRALoadingType = m_Data.nRALoadType;

	double dRA1dW = 0.0;
	double dEUDL = 0.0;
	double dTrainSpeed = 0.0;
	int nRALoadType = 0;
	int nRailDynFacType = 0;

	if (m_bModify)
	{
		dRA1dW = m_Data.dRA1dW;
		dEUDL = m_Data.dEUDL;
		dTrainSpeed = m_Data.dTrainSpeed;
		nRALoadType = m_Data.nRALoadType;
		nRailDynFacType = m_Data.nRailDynFacType;
	}
 	
 	T_MVHL_PATCH_LOAD PatchLoadDB;
 	CVehlDB db(m_pDoc);  
	db.GetStandardVehicleLoadValue(m_Data, &PatchLoadDB);

	if (m_bModify)
	{
		m_Data.dRA1dW = dRA1dW;
		m_Data.dEUDL = dEUDL;
		m_Data.dTrainSpeed = dTrainSpeed;
		m_Data.nRALoadType = nRALoadType;
		m_Data.nRailDynFacType = nRailDynFacType;
	}

 	ChangeBitmap(m_nStndCode, nLoadType, nSelVehicle);

 	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdBSDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.Initialize();
	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);
	m_Data.bStandard = TRUE;
	m_Data.VehicleTypeName = GetVehicleTypeStr();
	m_Data.nStandardCode = m_nStndCode;
	m_Data.SelVehicle = GetSelVihicleStr();

	m_Data.nRALoadType = m_nRALoadingType;

	int nLoadType, nSelVehicle;
	CDlgUtil::CobxSetCurSelItemData(m_cbxCodeName, m_nStndCode);
	GetIndex(nLoadType, nSelVehicle);

	if (m_nStndCode == D_MVHL_BS_5400)
	{
		if (m_pBS5400Dlg)
		{
			m_pBS5400Dlg->SetDlg2Data();
		}
	}
	else if (m_nStndCode == D_MVHL_BS_BD3701)
	{
		if (nLoadType == 0)
		{
			if (m_pBS3701HADlg)
			{
				m_pBS3701HADlg->SetDlg2Data();
			}
		}
		else if (nLoadType == 1)
		{
			if (m_pBS3701HBDlg)
			{
				m_pBS3701HBDlg->SetDlg2Data();
			}
		}
		else if (nLoadType == 2)
		{
			if (m_pBS3701HAHBDlg)
			{
				m_pBS3701HAHBDlg->SetDlg2Data();
			}
		}
		else if (nLoadType == 3)
		{
			if (m_pBS3701HAHBAutoDlg)
			{
				m_pBS3701HAHBAutoDlg->SetDlg2Data();
			}
		}
		else if (nLoadType == 4)
		{
			if (m_pBS3701PedestrianDlg)
			{
				m_pBS3701PedestrianDlg->SetDlg2Data();
			}
		}
	}
	else if (m_nStndCode == D_MVHL_BS_CS454)
	{
		if (nLoadType == 0)
		{
			if (m_pBSCS454AllModel1Dlg)
			{
				m_pBSCS454AllModel1Dlg->SetDlg2Data();
			}
		}		
		else if (nLoadType == 1)
		{
			if (m_pBSCS454AllModel2Dlg)
			{
				m_pBSCS454AllModel2Dlg->SetDlg2Data();
			}
		}
	}
	else if (m_nStndCode == D_MVHL_BS_BD8611)
	{
		if (m_pBSCS458Dlg)
		{
			m_pBSCS458Dlg->SetDlg2Data();
		}
	}
	else if (m_nStndCode == D_MVHL_BS_NETWORK_RAIL_STANDARDS)
	{
		if (nLoadType == 0)
		{
			if (nSelVehicle == 0)
			{
				if (m_pBSNRRailEUDLDlg)
				{
					m_pBSNRRailEUDLDlg->SetDlg2Data();
				}
			}
			else
			{
				if (m_pBSNRRailDlg)
				{
					m_pBSNRRailDlg->SetDlg2Data();
				}				
			}
		}
		else if (nLoadType == 1)
		{
			if (m_pBSNRWagonDlg)
			{
				m_pBSNRWagonDlg->SetDlg2Data();
			}			
		}
	}	

	UpdateData(TRUE);

	return TRUE;
}

BOOL CCMMvhlItemStdBSDlg::ApplyOrOK()
{
	if(!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if(m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else          bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);
	if(!bSuccess) return FALSE;

	return TRUE;
}

void CCMMvhlItemStdBSDlg::OnOK() 
{
	if (!ApplyOrOK()) return;	

	DestroyBSLoadDlg();

	CDialogMove::OnOK();
}

void CCMMvhlItemStdBSDlg::OnCancel()
{
	DestroyBSLoadDlg();

	CDialogMove::OnCancel();
}

void CCMMvhlItemStdBSDlg::OnCmdApply() 
{	
	ApplyOrOK();
}

/////////////////////////////////////////////////////////////////////////////
// picture window <--- nowlee *^o^*
void CCMMvhlItemStdBSDlg::ChangeBitmap(int nIndex, int nLoadType, int nSelVehicle)
{
	BOOL bBigSize = FALSE;

	CString strSVG;  
	switch(nIndex)
	{
		case D_MVHL_BS_5400:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_39.svg");
			else ASSERT(0);
			break;
		case D_MVHL_BS_BD3701:
			if(nLoadType == 0)        strSVG = _T("cmd_mvhl_47.svg");
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_48.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_49.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_39.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_50.svg");
			else ASSERT(0);
			break;
		case D_MVHL_BS_CS454: 
			if     (nLoadType == 0)  {strSVG = _T("cmd_mvhl_BS_LM1.svg"); bBigSize = TRUE;}// ALL Model 1
			else if(nLoadType == 1)   strSVG = _T("cmd_mvhl_47_CS.svg");  // All Model 2 
			else ASSERT(0);
			break;
		case D_MVHL_BS_BD8611:
			if(nLoadType == 0 || nLoadType == 1)        strSVG = _T("cmd_mvhl_sv80_sv100.svg");
			else if(nLoadType == 2)   strSVG = _T("cmd_mvhl_sv150.svg");
			else if(nLoadType == 3)   strSVG = _T("cmd_mvhl_sv196.svg");
			else if(nLoadType == 4)   strSVG = _T("cmd_mvhl_sv-train.svg");
			else if(nLoadType == 5)   strSVG = _T("cmd_mvhl_SVTT.svg");
			else if(nLoadType == 6)   strSVG = _T("cmd_mvhl_sov250.svg");
			else if(nLoadType == 7)   strSVG = _T("cmd_mvhl_sov350.svg");
			else if(nLoadType == 8)   strSVG = _T("cmd_mvhl_sov450.svg");
			else if(nLoadType == 9)   strSVG = _T("cmd_mvhl_sov600.svg");
			else ASSERT(0);
			break;
		case D_MVHL_BS_NETWORK_RAIL_STANDARDS:
			if (nLoadType == 0) {
				if (nSelVehicle == 0)
				{
					strSVG = _T("cmd_mvhl_87.svg");
				}
				else {
					strSVG = _T("cmd_mvhl_267_BS_RALoad.svg");
				}
			}
			else if (nLoadType == 1)
			{
				strSVG = _T("cmd_mvhl_23_Wagon.svg");
			}
			else
			{
				ASSERT(0);
			}
			break;
		default:
			ASSERT(0);
	}

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + strSVG);
}

void CCMMvhlItemStdBSDlg::ResetVehicleTypeCmb() 
{
	int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_cbxCodeName, m_cbxCodeName.GetCurSel());
	SetLoadTypeList(nCodeType);
}

void CCMMvhlItemStdBSDlg::OnSelchangeCmdMvhlTypeCombo() 
{  
	int nIndex = m_cbxCodeName.GetCurSel();
	m_nStndCode = m_cbxCodeName.GetItemData(nIndex);

	SetSelVehicleList();
	ChangeData();  	
}

void CCMMvhlItemStdBSDlg::OnSelchangeCmdMvhlCodeCmb() 
{
	int nIndex = m_cbxCodeName.GetCurSel();
	m_nStndCode = m_cbxCodeName.GetItemData(nIndex);

	ResetVehicleTypeCmb();
	SetSelVehicleList();
	ChangeData();  
}

void CCMMvhlItemStdBSDlg::OnCmdMvhlEuroSelVehicleCmb()
{
	ChangeData();
}

BOOL CCMMvhlItemStdBSDlg::ChangeData(BOOL bInitData/*=TRUE*/)
{
	if (bInitData) 
	{
		m_Data.Initialize();

		m_Data.dAddDataaL = M_InitValueCurUnit(3.65, N, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dImpactCoef = 1.0;

		m_Data.dEUDL /= m_pDoc->m_pUnitCtrl->GetConvertFactorCurrent(CUnitCtrl::m_MVHL_UNIT.dEUDL);
		m_Data.dRA1dW /= m_pDoc->m_pUnitCtrl->GetConvertFactorCurrent(CUnitCtrl::m_MVHL_UNIT.dRA1dW);
		m_Data.dRA1dL /= m_pDoc->m_pUnitCtrl->GetConvertFactorCurrent(CUnitCtrl::m_MVHL_UNIT.dRA1dL);
		m_Data.dRA1dTail /= m_pDoc->m_pUnitCtrl->GetConvertFactorCurrent(CUnitCtrl::m_MVHL_UNIT.dRA1dTail);
	}
	m_Data.bStandard = TRUE;
	m_Data.VehicleTypeName = GetVehicleTypeStr();
	m_Data.nStandardCode = m_nStndCode;

	//SetSelVehicleList();
	m_Data.SelVehicle = GetSelVihicleStr();

	int nLoadType, nSelVehicle;
	CDlgUtil::CobxSetCurSelItemData(m_cbxCodeName, m_nStndCode);
	GetIndex(nLoadType, nSelVehicle);

 	ChangeBitmap(m_nStndCode, nLoadType, nSelVehicle);

	double dRA1dW = 0.0;
	double dEUDL = 0.0;
	double dTrainSpeed = 0.0;
	int nRALoadType = 0;
	int nRailDynFacType = 0;

	if (m_bModify && !bInitData)
	{
		dRA1dW = m_Data.dRA1dW;
		dEUDL = m_Data.dEUDL;
		dTrainSpeed = m_Data.dTrainSpeed;
		nRALoadType = m_Data.nRALoadType;
		nRailDynFacType = m_Data.nRailDynFacType;
	}

	T_MVHL_PATCH_LOAD PatchLoadDB;
	CVehlDB db(m_pDoc);
	//if(db.GetStandardVehicleLoadValue(m_Data, &PatchLoadDB)) MakeItemEx();
	db.GetStandardVehicleLoadValue(m_Data, &PatchLoadDB);

	if (m_bModify && !bInitData)
	{
		m_Data.dRA1dW = dRA1dW;
		m_Data.dEUDL = dEUDL;
		m_Data.dTrainSpeed = dTrainSpeed;
		m_Data.nRALoadType = nRALoadType;
		m_Data.nRailDynFacType = nRailDynFacType;
	}

	if (bInitData)
	{
		BOOL bUseSelVehl = FALSE;

		if (m_nStndCode == D_MVHL_BS_CS454 && nLoadType == 0) bUseSelVehl = TRUE;
		if (m_nStndCode == D_MVHL_BS_NETWORK_RAIL_STANDARDS) bUseSelVehl = TRUE;

		if (bUseSelVehl)
		{
			m_edtVehicleName.SetWindowText(ConvVehicleNameStrRaw2Trans(m_Data.SelVehicle));
		}
		else
		{
			m_edtVehicleName.SetWindowText(ConvVehicleNameStrRaw2Trans(m_Data.VehicleTypeName));
		}
	}	

	if (!m_Data.PatchLoad.bPatchLoad)
		m_Data.PatchLoad = PatchLoadDB;

	if (m_nStndCode == D_MVHL_BS_5400)
	{
		if (m_pBS5400Dlg)
		{
			m_pBS5400Dlg->SetData2Dlg();
		}
	}
	else if (m_nStndCode == D_MVHL_BS_BD3701)
	{
		if (nLoadType == 0)
		{
			if (m_pBS3701HADlg)
			{
				m_pBS3701HADlg->SetData2Dlg();
				m_pBS3701HADlg->EnableDisableControls();
			}
		}
		else if (nLoadType == 1)
		{
			if (m_pBS3701HBDlg)
			{
				m_pBS3701HBDlg->SetData2Dlg();
			}
		}
		else if (nLoadType == 2)
		{
			if (m_pBS3701HAHBDlg)
			{
				m_pBS3701HAHBDlg->SetData2Dlg();
				m_pBS3701HAHBDlg->EnableDisableControls();
			}
		}		
		else if (nLoadType == 3)
		{
			if (m_pBS3701HAHBAutoDlg)
			{
				m_pBS3701HAHBAutoDlg->SetData2Dlg();
				m_pBS3701HAHBAutoDlg->EnableDisableControls();
			}
		}
		else if (nLoadType == 4)
		{
			if (m_pBS3701PedestrianDlg)
			{
				m_pBS3701PedestrianDlg->SetData2Dlg();
			}
		}
	}
	else if (m_nStndCode == D_MVHL_BS_CS454)
	{
		if (nLoadType == 0)
		{
			if (m_pBSCS454AllModel1Dlg)
			{
				m_pBSCS454AllModel1Dlg->SetLoadandSubType(nLoadType, nSelVehicle);
				m_pBSCS454AllModel1Dlg->ChangeText();
				m_pBSCS454AllModel1Dlg->SetData2Dlg();
				m_pBSCS454AllModel1Dlg->EnableDisableControls();
			}
		}		
		else if (nLoadType == 1)
		{
			if (m_pBSCS454AllModel2Dlg)
			{
				m_pBSCS454AllModel2Dlg->SetData2Dlg();
				m_pBSCS454AllModel2Dlg->EnableDisableControls();
			}
		}
	}
	else if (m_nStndCode == D_MVHL_BS_BD8611)
	{
		if (m_pBSCS458Dlg)
		{
			m_pBSCS458Dlg->SetLoadType(nLoadType);
			m_pBSCS458Dlg->ChangeText();
			m_pBSCS458Dlg->SetData2Dlg();
			m_pBSCS458Dlg->EnableDisableControls();
		}
	}
	else if (m_nStndCode == D_MVHL_BS_NETWORK_RAIL_STANDARDS)
	{
		if (nLoadType == 0)
		{
			if (nSelVehicle == 0)
			{
				if (m_pBSNRRailEUDLDlg)
				{
					m_pBSNRRailEUDLDlg->SetData2Dlg();
					m_pBSNRRailEUDLDlg->EnableDisableControls();
				}
			}
			else
			{
				if (m_pBSNRRailDlg)
				{
					m_pBSNRRailDlg->SetLoadType(nLoadType);
					m_pBSNRRailDlg->SetData2Dlg();
					m_pBSNRRailDlg->EnableDisableControls();
				}			
			}
		}
		else if (nLoadType == 1)
		{
			if (m_pBSNRWagonDlg)
			{
				m_pBSNRWagonDlg->SetLoadType(nLoadType);
				m_pBSNRWagonDlg->SetData2Dlg();
				m_pBSNRWagonDlg->EnableDisableControls();
			}		
		}
	}

	if (m_pBS5400Dlg)
	{
		m_pBS5400Dlg->ShowWindow(m_nStndCode == D_MVHL_BS_5400);
	}	

	if (m_pBS3701HADlg)
	{
		m_pBS3701HADlg->ShowWindow(m_nStndCode == D_MVHL_BS_BD3701 && nLoadType == 0);
	}

	if (m_pBS3701HBDlg)
	{
		m_pBS3701HBDlg->ShowWindow(m_nStndCode == D_MVHL_BS_BD3701 && nLoadType == 1);
	}

	if (m_pBS3701HAHBDlg)
	{
		m_pBS3701HAHBDlg->ShowWindow(m_nStndCode == D_MVHL_BS_BD3701 && nLoadType == 2);
	}

	if (m_pBS3701HAHBAutoDlg)
	{
		m_pBS3701HAHBAutoDlg->ShowWindow(m_nStndCode == D_MVHL_BS_BD3701 && nLoadType == 3);
	}

	if (m_pBS3701PedestrianDlg)
	{
		m_pBS3701PedestrianDlg->ShowWindow(m_nStndCode == D_MVHL_BS_BD3701 && nLoadType == 4);
	}	

	if (m_pBSCS454AllModel1Dlg)
	{
		m_pBSCS454AllModel1Dlg->ShowWindow(m_nStndCode == D_MVHL_BS_CS454 && nLoadType == 0);
	}

	if (m_pBSCS454AllModel2Dlg)
	{
		m_pBSCS454AllModel2Dlg->ShowWindow(m_nStndCode == D_MVHL_BS_CS454 && nLoadType == 1);
	}

	if (m_pBSCS458Dlg)
	{
		m_pBSCS458Dlg->ShowWindow(m_nStndCode == D_MVHL_BS_BD8611);
	}

	if (m_pBSNRRailEUDLDlg)
	{
		m_pBSNRRailEUDLDlg->ShowWindow(m_nStndCode == D_MVHL_BS_NETWORK_RAIL_STANDARDS && nLoadType == 0 && nSelVehicle == 0);
	}

	if (m_pBSNRRailDlg)
	{
		m_pBSNRRailDlg->ShowWindow(m_nStndCode == D_MVHL_BS_NETWORK_RAIL_STANDARDS && nLoadType == 0 && nSelVehicle != 0);
	}

	if (m_pBSNRWagonDlg)
	{
		m_pBSNRWagonDlg->ShowWindow(m_nStndCode == D_MVHL_BS_NETWORK_RAIL_STANDARDS && nLoadType == 1);
	}	

	ResizeBSDialog();

	ShowHideControls();

	UpdateData(FALSE);

	return TRUE;
}

// nIndex - 0: KSL, 1: KSTL, 2: AASHTO, 3: CSL, 4: JTJ001-97, 5: CJJ77-98, 6: TB10002.1-99,
//          7: OTHERS, 8: AASHTO_LRFD, 9: IRC6-2000
void CCMMvhlItemStdBSDlg::SetLoadTypeList(int nIndex)
{
	if (nIndex < 0) return;

	CArray<CString, CString> aVehicleType;
	int i = 0;
	while (i < D_MAX_COUNT && CVehlDB::GetTypeListAr()[nIndex][i] != _T(""))
	{
		aVehicleType.Add(CVehlDB::GetTypeListAr()[nIndex][i++]);
	}

	if(aVehicleType.GetSize() == 0) return;

	m_wndVehicleTypeList.ResetContent();

	for(i=0; i<aVehicleType.GetSize(); i++)
	{
		const auto& strTranslation =
			ConvVehicleTypeStrRaw2Trans(aVehicleType[i]);
		m_wndVehicleTypeList.AddString(strTranslation);
	}

	CDlgUtil::CobxAdjustListBoxWidth(m_wndVehicleTypeList);
	m_wndVehicleTypeList.SetCurSel(0);
}

void CCMMvhlItemStdBSDlg::GetIndex(int& nLoadType, int& nSelVehicle)
{  
	// hslee : 이 함수 VehlDB로 빼서 다같이 써야 함...
	nLoadType = 0;
	nSelVehicle = 0;
	
	BOOL bBreakLoadType = TRUE;

	CArray<CString, CString> aVehicleType;
	for (int i=0; i < D_MAX_COUNT; i++)
	{
		if(CVehlDB::GetTypeListAr()[m_Data.nStandardCode][i] == _T("")) continue;
		aVehicleType.Add(CVehlDB::GetTypeListAr()[m_Data.nStandardCode][i]);
	}
	
	for(int i=0; i<aVehicleType.GetSize(); i++)
	{
		if(aVehicleType[i] == m_Data.VehicleTypeName)
		{
			nLoadType = i;
			bBreakLoadType = TRUE;
			break;
		}
	}

	if(m_nStndCode == D_MVHL_BS_CS454) // All Model 1
	{
		// nLoadType : 0 =_T("ALL MODEL 1"), 1= _T("ALL MODEL 2(UDL+KEL)")
		if     (m_Data.SelVehicle == _T("A-4AXLE")  ) {nSelVehicle = 0;}
		else if(m_Data.SelVehicle == _T("B-4AXLE")  ) {nSelVehicle = 1;}
		else if(m_Data.SelVehicle == _T("C-5AXLE")  ) {nSelVehicle = 2;}
		else if(m_Data.SelVehicle == _T("D-5AXLE_1")) {nSelVehicle = 3;}
		else if(m_Data.SelVehicle == _T("D-5AXLE_2")) {nSelVehicle = 4;}
		else if(m_Data.SelVehicle == _T("E-5AXLE_1")) {nSelVehicle = 5;}
		else if(m_Data.SelVehicle == _T("E-5AXLE_2")) {nSelVehicle = 6;}
		else if(m_Data.SelVehicle == _T("F-6AXLE_1")) {nSelVehicle = 7;}
		else if(m_Data.SelVehicle == _T("F-6AXLE_2")) {nSelVehicle = 8;}
		else if(m_Data.SelVehicle == _T("G-6AXLE_1")) {nSelVehicle = 9;}
		else if(m_Data.SelVehicle == _T("G-6AXLE_2")) {nSelVehicle =10;}
		else if(m_Data.SelVehicle == _T("H-5AXLE_1")) {nSelVehicle =11;}
		else if(m_Data.SelVehicle == _T("H-5AXLE_2")) {nSelVehicle =12;}
		else if(m_Data.SelVehicle == _T("I-3AXLE")  ) {nSelVehicle =13;}
		else if(m_Data.SelVehicle == _T("J-3AXLE")  ) {nSelVehicle =14;}
		else if(m_Data.SelVehicle == _T("K-3AXLE_1")) {nSelVehicle =15;}
		else if(m_Data.SelVehicle == _T("K-3AXLE_2")) {nSelVehicle =16;}
		else if(m_Data.SelVehicle == _T("L-3AXLE_1")) {nSelVehicle =17;}
		else if(m_Data.SelVehicle == _T("L-3AXLE_2")) {nSelVehicle =18;}
		else if(m_Data.SelVehicle == _T("M-2AXLE")  ) {nSelVehicle =19;}
		else if(m_Data.SelVehicle == _T("N-2AXLE")  ) {nSelVehicle =20;}
		else if(m_Data.SelVehicle == _T("O-2AXLE")  ) {nSelVehicle =21;}
		else { nSelVehicle = 0; }
	}

	if (m_nStndCode == D_MVHL_BS_NETWORK_RAIL_STANDARDS)
	{
		if (m_Data.VehicleTypeName == _T("Type RAI Loading"))
		{
		  if (m_Data.SelVehicle == _T("EUDL")) { nSelVehicle = 0; }
			else if (m_Data.SelVehicle == _T("RA1"))  { nSelVehicle =  1; }
			else if (m_Data.SelVehicle == _T("RA2"))  { nSelVehicle =  2; }
			else if (m_Data.SelVehicle == _T("RA3"))  { nSelVehicle =  3; }
			else if (m_Data.SelVehicle == _T("RA4"))  { nSelVehicle =  4; }
			else if (m_Data.SelVehicle == _T("RA5"))  { nSelVehicle =  5; }
			else if (m_Data.SelVehicle == _T("RA6"))  { nSelVehicle =  6; }
			else if (m_Data.SelVehicle == _T("RA7"))  { nSelVehicle =  7; }
			else if (m_Data.SelVehicle == _T("RA8"))  { nSelVehicle =  8; }
			else if (m_Data.SelVehicle == _T("RA9"))  { nSelVehicle =  9; }
			else if (m_Data.SelVehicle == _T("RA10")) { nSelVehicle = 10; }
			else if (m_Data.SelVehicle == _T("RA11")) { nSelVehicle = 11; }
			else if (m_Data.SelVehicle == _T("RA12")) { nSelVehicle = 12; }
			else if (m_Data.SelVehicle == _T("RA13")) { nSelVehicle = 13; }
			else if (m_Data.SelVehicle == _T("RA14")) { nSelVehicle = 14; }
			else if (m_Data.SelVehicle == _T("RA15")) { nSelVehicle = 15; }
			else { nSelVehicle = 10; }
		}
		else
		{
			if (m_Data.SelVehicle == _T("Assessment Load Wagon")) { nSelVehicle = 0; }
			else if (m_Data.SelVehicle == _T("Wagon Type D4")) { nSelVehicle = 1; }
			else { nSelVehicle = 0; }
		}
	}

	if(bBreakLoadType == FALSE)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_vehicle_load_name));
	}
}

BOOL CCMMvhlItemStdBSDlg::CreateBSLoadDlg()
{	
	m_pBS5400Dlg = new CCMMvhlItemStdBSBS5400Page(&m_Data, this);
	ASSERT(m_pBS5400Dlg);
	if (!m_pBS5400Dlg) return FALSE;

	m_pBS3701HADlg = new CCMMvhlItemStdBSBD3701HAPage(&m_Data, this);
	ASSERT(m_pBS3701HADlg);
	if (!m_pBS3701HADlg) return FALSE;

	m_pBS3701HBDlg = new CCMMvhlItemStdBSBD3701HBPage(&m_Data, this);
	ASSERT(m_pBS3701HBDlg);
	if (!m_pBS3701HBDlg) return FALSE;

	m_pBS3701HAHBDlg = new CCMMvhlItemStdBSBD3701HAHBPage(&m_Data, this);
	ASSERT(m_pBS3701HAHBDlg);
	if (!m_pBS3701HAHBDlg) return FALSE;

	m_pBS3701HAHBAutoDlg = new CCMMvhlItemStdBSBD3701HAHBAutoPage(&m_Data, this);
	ASSERT(m_pBS3701HAHBAutoDlg);
	if (!m_pBS3701HAHBAutoDlg) return FALSE;

	m_pBS3701PedestrianDlg = new CCMMvhlItemStdBSBD3701PedestrianPage(&m_Data, this);
	ASSERT(m_pBS3701PedestrianDlg);
	if (!m_pBS3701PedestrianDlg) return FALSE;	

	m_pBSCS454AllModel1Dlg = new CCMMvhlItemStdBSCS454AllModel1Page(&m_Data, this);
	ASSERT(m_pBSCS454AllModel1Dlg);
	if (!m_pBSCS454AllModel1Dlg) return FALSE;

	m_pBSCS454AllModel2Dlg = new CCMMvhlItemStdBSCS454AllModel2Page(&m_Data, this);
	ASSERT(m_pBSCS454AllModel2Dlg);
	if (!m_pBSCS454AllModel2Dlg) return FALSE;

	m_pBSCS458Dlg = new CCMMvhlItemStdBSCS458Page(&m_Data, this);
	ASSERT(m_pBSCS458Dlg);
	if (!m_pBSCS458Dlg) return FALSE;

	m_pBSNRRailEUDLDlg = new CCMMvhlItemStdBSNRRailEUDLPage(&m_Data, this);
	ASSERT(m_pBSNRRailEUDLDlg);
	if (!m_pBSNRRailEUDLDlg) return FALSE;

	m_pBSNRRailDlg = new CCMMvhlItemStdBSNRRailPage(&m_Data, this);
	ASSERT(m_pBSNRRailDlg);
	if (!m_pBSNRRailDlg) return FALSE;

	m_pBSNRWagonDlg = new CCMMvhlItemStdBSNRWagonPage(&m_Data, this);
	ASSERT(m_pBSNRWagonDlg);
	if (!m_pBSNRWagonDlg) return FALSE;

	// holder
	CWnd* cwnd = (CWnd*)GetDlgItem(IDC_CMD_DLGHOLDER);
	if (!cwnd)	return FALSE;

	CRect rc;
	cwnd->GetWindowRect(&rc);	// holder 위치.. 
	this->ScreenToClient(&rc);	// 현재 대화상자 기준좌표. 

	// create dlg
	CRect rDlg;
	
	if (m_pBS5400Dlg->GetSafeHwnd() == 0)
	{
		m_pBS5400Dlg->Create(IDD_CMD_ML_MVHL_ITEMS_BS_BS5400, this);
		m_pBS5400Dlg->GetWindowRect(rDlg);
		m_pBS5400Dlg->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rDlg.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}

	if (m_pBS3701HADlg->GetSafeHwnd() == 0)
	{
		m_pBS3701HADlg->Create(IDD_CMD_ML_MVHL_ITEMS_BS_BD3701_HA, this);
		m_pBS3701HADlg->GetWindowRect(rDlg);
		m_pBS3701HADlg->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rDlg.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}

	if (m_pBS3701HBDlg->GetSafeHwnd() == 0)
	{
		m_pBS3701HBDlg->Create(IDD_CMD_ML_MVHL_ITEMS_BS_BD3701_HB, this);
		m_pBS3701HBDlg->GetWindowRect(rDlg);
		m_pBS3701HBDlg->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rDlg.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}

	if (m_pBS3701HAHBDlg->GetSafeHwnd() == 0)
	{
		m_pBS3701HAHBDlg->Create(IDD_CMD_ML_MVHL_ITEMS_BS_BD3701_HAHB, this);
		m_pBS3701HAHBDlg->GetWindowRect(rDlg);
		m_pBS3701HAHBDlg->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rDlg.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}

	if (m_pBS3701HAHBAutoDlg->GetSafeHwnd() == 0)
	{
		m_pBS3701HAHBAutoDlg->Create(IDD_CMD_ML_MVHL_ITEMS_BS_BD3701_HAHB_AUTO, this);
		m_pBS3701HAHBAutoDlg->GetWindowRect(rDlg);
		m_pBS3701HAHBAutoDlg->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rDlg.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}

	if (m_pBS3701PedestrianDlg->GetSafeHwnd() == 0)
	{
		m_pBS3701PedestrianDlg->Create(IDD_CMD_ML_MVHL_ITEMS_BS_BD3701_PEDESTRIAN, this);
		m_pBS3701PedestrianDlg->GetWindowRect(rDlg);
		m_pBS3701PedestrianDlg->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rDlg.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}	

	if (m_pBSCS454AllModel1Dlg->GetSafeHwnd() == 0)
	{
		m_pBSCS454AllModel1Dlg->Create(IDD_CMD_ML_MVHL_ITEMS_BS_CS454_ALL_MODEL1, this);
		m_pBSCS454AllModel1Dlg->GetWindowRect(rDlg);
		m_pBSCS454AllModel1Dlg->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rDlg.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}

	if (m_pBSCS454AllModel2Dlg->GetSafeHwnd() == 0)
	{
		m_pBSCS454AllModel2Dlg->Create(IDD_CMD_ML_MVHL_ITEMS_BS_CS454_ALL_MODEL2, this);
		m_pBSCS454AllModel2Dlg->GetWindowRect(rDlg);
		m_pBSCS454AllModel2Dlg->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rDlg.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}

	if (m_pBSCS458Dlg->GetSafeHwnd() == 0)
	{
		m_pBSCS458Dlg->Create(IDD_CMD_ML_MVHL_ITEMS_BS_CS458, this);
		m_pBSCS458Dlg->GetWindowRect(rDlg);
		m_pBSCS458Dlg->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rDlg.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}

	if (m_pBSNRRailEUDLDlg->GetSafeHwnd() == 0)
	{
		m_pBSNRRailEUDLDlg->Create(IDD_CMD_ML_MVHL_ITEMS_BS_NR_RAIL_EUDL, this);
		m_pBSNRRailEUDLDlg->GetWindowRect(rDlg);
		m_pBSNRRailEUDLDlg->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rDlg.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}

	if (m_pBSNRRailDlg->GetSafeHwnd() == 0)
	{
		m_pBSNRRailDlg->Create(IDD_CMD_ML_MVHL_ITEMS_BS_NR_RAIL, this);
		m_pBSNRRailDlg->GetWindowRect(rDlg);
		m_pBSNRRailDlg->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rDlg.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}

	if (m_pBSNRWagonDlg->GetSafeHwnd() == 0)
	{
		m_pBSNRWagonDlg->Create(IDD_CMD_ML_MVHL_ITEMS_BS_NR_WAGON, this);
		m_pBSNRWagonDlg->GetWindowRect(rDlg);
		m_pBSNRWagonDlg->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rDlg.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}	

	return TRUE;
}

void CCMMvhlItemStdBSDlg::ResizeBSDialog()
{
	CRect rRef;
	CRect rToMove;
	int nDistX = 0;
	int nDistY = 0;
	CArray<UINT, UINT> aControls;

	if (m_nStndCode == D_MVHL_BS_NETWORK_RAIL_STANDARDS)
	{
		aControls.RemoveAll();
		aControls.Add(IDC_RA1_LOADING_TYPE_GRP);
		aControls.Add(IDC_RA_TRAIN_RDO);
		aControls.Add(IDC_RA_SHORT_LENGTH_RDO);
		aControls.Add(IDC_RA_MORE_CRITICAL_RDO);
		
		GetDlgItem(IDC_CMD_MVHL_SEL_VEHICLE_CMB)->GetWindowRect(rRef);
		GetDlgItem(IDC_RA1_LOADING_TYPE_GRP)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

		aControls.RemoveAll();
		aControls.Add(IDC_CMD_MVHL_PIC_WND);
		aControls.Add(IDC_CMD_DLGHOLDER);
		
		GetDlgItem(IDC_RA1_LOADING_TYPE_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_PIC_WND)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);			
	}
	else
	{
		aControls.RemoveAll();
		aControls.Add(IDC_CMD_MVHL_PIC_WND);
		aControls.Add(IDC_CMD_DLGHOLDER);

		GetDlgItem(IDC_CMD_MVHL_SEL_VEHICLE_CMB)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_PIC_WND)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	}

	// 각 Page 를 이동한 Holder 에 맞게 새로 그리고..
	GetDlgItem(IDC_CMD_DLGHOLDER)->GetWindowRect(rToMove);
	ScreenToClient(rToMove);

	int nLoadType, nSelVehicle;
	CDlgUtil::CobxSetCurSelItemData(m_cbxCodeName, m_nStndCode);
	GetIndex(nLoadType, nSelVehicle);

	if (m_nStndCode == D_MVHL_BS_5400)
	{
		m_pBS5400Dlg->GetWindowRect(rRef);
		m_pBS5400Dlg->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rRef.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
		m_pBS5400Dlg->GetWindowRect(rRef);
	}	
	else if (m_nStndCode == D_MVHL_BS_BD3701)
	{
		if (nLoadType == 0)
		{
			m_pBS3701HADlg->GetWindowRect(rRef);
			m_pBS3701HADlg->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rRef.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
			m_pBS3701HADlg->GetWindowRect(rRef);
		}
		else if (nLoadType == 1)
		{
			m_pBS3701HBDlg->GetWindowRect(rRef);
			m_pBS3701HBDlg->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rRef.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
			m_pBS3701HBDlg->GetWindowRect(rRef);
		}
		else if (nLoadType == 2)
		{
			m_pBS3701HAHBDlg->GetWindowRect(rRef);
			m_pBS3701HAHBDlg->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rRef.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
			m_pBS3701HAHBDlg->GetWindowRect(rRef);
		}
		else if (nLoadType == 3)
		{
			m_pBS3701HAHBAutoDlg->GetWindowRect(rRef);
			m_pBS3701HAHBAutoDlg->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rRef.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
			m_pBS3701HAHBAutoDlg->GetWindowRect(rRef);
		}
		else if (nLoadType == 4)
		{
			m_pBS3701PedestrianDlg->GetWindowRect(rRef);
			m_pBS3701PedestrianDlg->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rRef.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
			m_pBS3701PedestrianDlg->GetWindowRect(rRef);
		}		
	}
	else if (m_nStndCode == D_MVHL_BS_CS454)
	{
		if (nLoadType == 0)
		{
			m_pBSCS454AllModel1Dlg->GetWindowRect(rRef);
			m_pBSCS454AllModel1Dlg->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rRef.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
			m_pBSCS454AllModel1Dlg->GetWindowRect(rRef);
		}
		else if (nLoadType == 1)
		{
			m_pBSCS454AllModel2Dlg->GetWindowRect(rRef);
			m_pBSCS454AllModel2Dlg->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rRef.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
			m_pBSCS454AllModel2Dlg->GetWindowRect(rRef);
		}
	}
	else if (m_nStndCode == D_MVHL_BS_BD8611)
	{
		m_pBSCS458Dlg->GetWindowRect(rRef);
		m_pBSCS458Dlg->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rRef.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
		m_pBSCS458Dlg->GetWindowRect(rRef);
	}
	else if (m_nStndCode == D_MVHL_BS_NETWORK_RAIL_STANDARDS)
	{
		if (nLoadType == 0)
		{
			if (nSelVehicle == 0)
			{
				m_pBSNRRailEUDLDlg->GetWindowRect(rRef);
				m_pBSNRRailEUDLDlg->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rRef.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
				m_pBSNRRailEUDLDlg->GetWindowRect(rRef);
			}
			else
			{
				m_pBSNRRailDlg->GetWindowRect(rRef);
				m_pBSNRRailDlg->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rRef.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
				m_pBSNRRailDlg->GetWindowRect(rRef);
			}
		}
		else if (nLoadType == 1)
		{
			m_pBSNRWagonDlg->GetWindowRect(rRef);
			m_pBSNRWagonDlg->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rRef.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
			m_pBSNRWagonDlg->GetWindowRect(rRef);
		}
	}

	// Holder 변경 
	GetDlgItem(IDC_CMD_DLGHOLDER)->GetWindowRect(rToMove);
	rToMove.bottom = rRef.bottom;
	ScreenToClient(rToMove);
	GetDlgItem(IDC_CMD_DLGHOLDER)->MoveWindow(rToMove);

	// Vehicular Load Properties
	GetDlgItem(IDC_CMD_DLGHOLDER)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES1)->GetWindowRect(rToMove);
	rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	ScreenToClient(rToMove);
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES1)->MoveWindow(rToMove);

	// OK, CANCEL, APPLY
	CArray<UINT, UINT> aOKCancel;
	aOKCancel.Add(IDOK);
	aOKCancel.Add(IDCANCEL);
	aOKCancel.Add(IDC_CMD_APPLY);
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES1)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, aOKCancel, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

void CCMMvhlItemStdBSDlg::DestroyBSLoadDlg()
{
	if (m_pBS3701HADlg)
	{
		m_pBS3701HADlg->SendMessage(WM_DESTROY, 0, 0);
		delete m_pBS3701HADlg;
		m_pBS3701HADlg = NULL;
	}

	if (m_pBS3701HBDlg)
	{
		m_pBS3701HBDlg->SendMessage(WM_DESTROY, 0, 0);
		delete m_pBS3701HBDlg;
		m_pBS3701HBDlg = NULL;
	}

	if (m_pBS3701HAHBDlg)
	{
		m_pBS3701HAHBDlg->SendMessage(WM_DESTROY, 0, 0);
		delete m_pBS3701HAHBDlg;
		m_pBS3701HAHBDlg = NULL;
	}

	if (m_pBS3701HAHBAutoDlg)
	{
		m_pBS3701HAHBAutoDlg->SendMessage(WM_DESTROY, 0, 0);
		delete m_pBS3701HAHBAutoDlg;
		m_pBS3701HAHBAutoDlg = NULL;
	}

	if (m_pBS3701PedestrianDlg)
	{
		m_pBS3701PedestrianDlg->SendMessage(WM_DESTROY, 0, 0);
		delete m_pBS3701PedestrianDlg;
		m_pBS3701PedestrianDlg = NULL;
	}

	if (m_pBS5400Dlg)
	{
		m_pBS5400Dlg->SendMessage(WM_DESTROY, 0, 0);
		delete m_pBS5400Dlg;
		m_pBS5400Dlg = NULL;
	}	

	if (m_pBSCS454AllModel1Dlg)
	{
		m_pBSCS454AllModel1Dlg->SendMessage(WM_DESTROY, 0, 0);
		delete m_pBSCS454AllModel1Dlg;
		m_pBSCS454AllModel1Dlg = NULL;
	}

	if (m_pBSCS454AllModel2Dlg)
	{
		m_pBSCS454AllModel2Dlg->SendMessage(WM_DESTROY, 0, 0);
		delete m_pBSCS454AllModel2Dlg;
		m_pBSCS454AllModel2Dlg = NULL;
	}

	if (m_pBSCS458Dlg)
	{
		m_pBSCS458Dlg->SendMessage(WM_DESTROY, 0, 0);
		delete m_pBSCS458Dlg;
		m_pBSCS458Dlg = NULL;
	}

	if (m_pBSNRRailEUDLDlg)
	{
		m_pBSNRRailEUDLDlg->SendMessage(WM_DESTROY, 0, 0);
		delete m_pBSNRRailEUDLDlg;
		m_pBSNRRailEUDLDlg = NULL;
	}

	if (m_pBSNRRailDlg)
	{
		m_pBSNRRailDlg->SendMessage(WM_DESTROY, 0, 0);
		delete m_pBSNRRailDlg;
		m_pBSNRRailDlg = NULL;
	}

	if (m_pBSNRWagonDlg)
	{
		m_pBSNRWagonDlg->SendMessage(WM_DESTROY, 0, 0);
		delete m_pBSNRWagonDlg;
		m_pBSNRWagonDlg = NULL;
	}
}

void CCMMvhlItemStdBSDlg::ShowHideControls()
{
	int nLoadType, nSelVehicle;
	CDlgUtil::CobxSetCurSelItemData(m_cbxCodeName, m_nStndCode);
	GetIndex(nLoadType, nSelVehicle);

	CArray<UINT, UINT> aControls;

	aControls.RemoveAll();
	aControls.Add(IDC_CMD_MVHL_SEL_VEHICLE_STC);
	aControls.Add(IDC_CMD_MVHL_SEL_VEHICLE_CMB);

	BOOL bShow = FALSE;

	if (m_nStndCode == D_MVHL_BS_CS454 && nLoadType == 0) bShow = TRUE;
	if (m_nStndCode == D_MVHL_BS_NETWORK_RAIL_STANDARDS) bShow = TRUE;
	
	CDlgUtil::CtrlShowHide(this, aControls, bShow);

	aControls.RemoveAll();
	aControls.Add(IDC_RA1_LOADING_TYPE_GRP);
	aControls.Add(IDC_RA_TRAIN_RDO);
	aControls.Add(IDC_RA_SHORT_LENGTH_RDO);
	aControls.Add(IDC_RA_MORE_CRITICAL_RDO);
	
	CDlgUtil::CtrlShowHide(this, aControls, m_nStndCode == D_MVHL_BS_NETWORK_RAIL_STANDARDS && nLoadType == 0 && nSelVehicle != 0);
}

void CCMMvhlItemStdBSDlg::SetDefValAndCbxItem()
{
	CString csDefaultType;
	CString csDefaultVehi;
	int nDefaultStndCode = 1;

	int nCodeSeq[5];
	int nNumIter = 5;

	nCodeSeq[0] = D_MVHL_BS_5400;
	nCodeSeq[1] = D_MVHL_BS_BD3701;
	nCodeSeq[2] = D_MVHL_BS_CS454;
	nCodeSeq[3] = D_MVHL_BS_BD8611;
	nCodeSeq[4] = D_MVHL_BS_NETWORK_RAIL_STANDARDS;

	csDefaultType = _T("HA");//_T("HA & HB (BS5400)");
	csDefaultVehi = _T("");
	nDefaultStndCode = D_MVHL_BS_BD3701;

	CString VehicleTypeName;

	// TODO: Add extra initialization here  
	if(m_bModify)
	{
		m_nStndCode = m_Data.nStandardCode;

		int nLoadType, nSelVehicle;
		GetIndex(nLoadType, nSelVehicle);
		if (m_nStndCode != nLoadType) nLoadType = m_Data.nStandardCode;
		SetLoadTypeList(nLoadType);
		m_wndVehicleTypeList.SetCurSel(nLoadType);

		if (m_nStndCode == D_MVHL_BS_NETWORK_RAIL_STANDARDS)
		{
			VehicleTypeName = m_Data.VehicleTypeName;
		}
	}
	else
	{
		m_Data.Initialize();
		m_Data.bStandard = TRUE;    
		m_Data.VehicleTypeName = csDefaultType;    
		m_nStndCode = m_Data.nStandardCode = nDefaultStndCode;
		m_Data.SelVehicle = csDefaultVehi; 
		m_Data.dAddDataaL = M_InitValueCurUnit(3.65, N, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dImpactCoef = 1.0;

		m_Data.dEUDL /= m_pDoc->m_pUnitCtrl->GetConvertFactorCurrent(CUnitCtrl::m_MVHL_UNIT.dEUDL);
		m_Data.dRA1dW /= m_pDoc->m_pUnitCtrl->GetConvertFactorCurrent(CUnitCtrl::m_MVHL_UNIT.dRA1dW);
		m_Data.dRA1dL /= m_pDoc->m_pUnitCtrl->GetConvertFactorCurrent(CUnitCtrl::m_MVHL_UNIT.dRA1dL);
		m_Data.dRA1dTail /= m_pDoc->m_pUnitCtrl->GetConvertFactorCurrent(CUnitCtrl::m_MVHL_UNIT.dRA1dTail);
	}

	for(int i = 0; i < nNumIter; i++) 
	{
		CDlgUtil::CobxAddItem(m_cbxCodeName, CVehlDB::GetTypeNameAr()[nCodeSeq[i]], nCodeSeq[i]);
	}

	CDlgUtil::CobxAdjustListBoxWidth(m_cbxCodeName);

	if (!VehicleTypeName.IsEmpty())
	{
		SetSelVehicleList(&VehicleTypeName);
	}
	else
	{
		SetSelVehicleList();
	}
}

void CCMMvhlItemStdBSDlg::SetSelVehicleList(CString* pVehicleTypeName)
{
	UpdateData(TRUE);

	int nLoadType, nSelVehicle;
	GetIndex(nLoadType, nSelVehicle);

	m_cmbSelVihicle.ResetContent();

	if (m_nStndCode == D_MVHL_BS_NETWORK_RAIL_STANDARDS)
	{
		CString VehicleType;
		if (pVehicleTypeName != NULL)
		{
			VehicleType = *pVehicleTypeName;
		}
		else
		{
			VehicleType = GetVehicleTypeStr();
		}
		if (VehicleType == _T("Type RAI Loading"))
		{
			m_cmbSelVihicle.ResetContent();
			m_cmbSelVihicle.AddString(_T("EUDL"));
			m_cmbSelVihicle.AddString(_T("RA1"));
			m_cmbSelVihicle.AddString(_T("RA2"));
			m_cmbSelVihicle.AddString(_T("RA3"));
			m_cmbSelVihicle.AddString(_T("RA4"));
			m_cmbSelVihicle.AddString(_T("RA5"));
			m_cmbSelVihicle.AddString(_T("RA6"));
			m_cmbSelVihicle.AddString(_T("RA7"));
			m_cmbSelVihicle.AddString(_T("RA8"));
			m_cmbSelVihicle.AddString(_T("RA9"));
			m_cmbSelVihicle.AddString(_T("RA10"));
			m_cmbSelVihicle.AddString(_T("RA11"));
			m_cmbSelVihicle.AddString(_T("RA12"));
			m_cmbSelVihicle.AddString(_T("RA13"));
			m_cmbSelVihicle.AddString(_T("RA14"));
			m_cmbSelVihicle.AddString(_T("RA15"));
			m_cmbSelVihicle.SetCurSel(10);
		}
		else
		{
			m_cmbSelVihicle.ResetContent();
			CString WagonD4 = _T("Wagon Type D4");
			CString AssessmentWagon = _T("Assessment Load Wagon");
			m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(AssessmentWagon));
			m_cmbSelVihicle.AddString(ConvSelVihicleStrRaw2Trans(WagonD4));
			m_cmbSelVihicle.SetCurSel(0);
		}
	}
	else
	{
		m_cmbSelVihicle.ResetContent();
		m_cmbSelVihicle.AddString(_T("A-4AXLE"));
		m_cmbSelVihicle.AddString(_T("B-4AXLE"));
		m_cmbSelVihicle.AddString(_T("C-5AXLE"));
		m_cmbSelVihicle.AddString(_T("D-5AXLE_1"));
		m_cmbSelVihicle.AddString(_T("D-5AXLE_2"));
		m_cmbSelVihicle.AddString(_T("E-5AXLE_1"));
		m_cmbSelVihicle.AddString(_T("E-5AXLE_2"));
		m_cmbSelVihicle.AddString(_T("F-6AXLE_1"));
		m_cmbSelVihicle.AddString(_T("F-6AXLE_2"));
		m_cmbSelVihicle.AddString(_T("G-6AXLE_1"));
		m_cmbSelVihicle.AddString(_T("G-6AXLE_2"));
		m_cmbSelVihicle.AddString(_T("H-5AXLE_1"));
		m_cmbSelVihicle.AddString(_T("H-5AXLE_2"));
		m_cmbSelVihicle.AddString(_T("I-3AXLE"));
		m_cmbSelVihicle.AddString(_T("J-3AXLE"));
		m_cmbSelVihicle.AddString(_T("K-3AXLE_1"));
		m_cmbSelVihicle.AddString(_T("K-3AXLE_2"));
		m_cmbSelVihicle.AddString(_T("L-3AXLE_1"));
		m_cmbSelVihicle.AddString(_T("L-3AXLE_2"));
		m_cmbSelVihicle.AddString(_T("M-2AXLE"));
		m_cmbSelVihicle.AddString(_T("N-2AXLE"));
		m_cmbSelVihicle.AddString(_T("O-2AXLE"));

		m_cmbSelVihicle.SetCurSel(0);
	}
}


typedef std::pair<CString, CString> pair_cstring;
template <std::size_t N> using array_pair_cstring = std::array<pair_cstring, N>;

template <std::size_t N>
CString ConvStrRaw2Trans(const array_pair_cstring<N>& aPairStr,
	const CString& strRaw)
{
	for (const auto& strPair : aPairStr)
	{
		const auto& strTarget = strPair.first;
		if (strRaw == strTarget)
		{
			return strPair.second;
		}
	}

	return strRaw;
}

template <std::size_t N>
CString ConvStrTrans2Raw(const array_pair_cstring<N>& aPairStr,
	const CString& strTrans)
{
	for (const auto& strPair : aPairStr)
	{
		const auto& strTarget = strPair.second;
		if (strTrans == strTarget)
		{
			return strPair.first;
		}
	}

	return strTrans;
}

#define __static_caVehicleLoadType static const array_pair_cstring<4> caVehicleLoadType = \
{ \
    std::make_pair(_T("Type RAI Loading"),    _LS(IDS_WG_CMD_Type_RAI_Loading)), \
    std::make_pair(_T("Wagon"),               _LS(IDS_WG_CMD_Wagon)), \
    std::make_pair(_T("HA & HB(Auto)"),       _LS(IDS_WG_CMD_HA_HB_Auto)), \
    std::make_pair(_T("Pedestrian"),          _LS(IDS_WG_CMD_Pedestrian)), \
}; \

CString CCMMvhlItemStdBSDlg::ConvVehicleTypeStrRaw2Trans(const CString& strRaw) const
{
	__static_caVehicleLoadType;
	return ConvStrRaw2Trans(caVehicleLoadType, strRaw);
}

CString CCMMvhlItemStdBSDlg::ConvVehicleTypeStrTrans2Raw(const CString& strTrans) const
{
	__static_caVehicleLoadType;
	return ConvStrTrans2Raw(caVehicleLoadType, strTrans);
}

#undef __static_caVehicleLoadType


#define __static_caSelVehicleLoadType static const array_pair_cstring<2> caSelVehicleLoadType = \
{ \
    std::make_pair(_T("Assessment Load Wagon"),    _LS(IDS_WG_CMD_Assessment_Load_Wagon)), \
    std::make_pair(_T("Wagon Type D4"),            _LS(IDS_WG_CMD_Wagon_Type_D4)), \
}; \

CString CCMMvhlItemStdBSDlg::ConvSelVihicleStrRaw2Trans(const CString& strRaw) const
{
	__static_caSelVehicleLoadType;
	return ConvStrRaw2Trans(caSelVehicleLoadType, strRaw);
}

CString CCMMvhlItemStdBSDlg::ConvSelVihicleStrTrans2Raw(const CString& strTrans) const
{
	__static_caSelVehicleLoadType;
	return ConvStrTrans2Raw(caSelVehicleLoadType, strTrans);
}

#undef __static_caSelVehicleLoadType

#define __static_caVehicleLoadName static const array_pair_cstring<6> caVehicleLoadName = \
{ \
    std::make_pair(_T("Assessment Load Wagon"),    _LS(IDS_WG_CMD_Assessment_Load_Wagon)), \
    std::make_pair(_T("Wagon Type D4"),            _LS(IDS_WG_CMD_Wagon_Type_D4)), \
    std::make_pair(_T("Type RAI Loading"),         _LS(IDS_WG_CMD_Type_RAI_Loading)), \
    std::make_pair(_T("Wagon"),                    _LS(IDS_WG_CMD_Wagon)), \
    std::make_pair(_T("HA & HB(Auto)"),            _LS(IDS_WG_CMD_HA_HB_Auto)), \
    std::make_pair(_T("Pedestrian"),               _LS(IDS_WG_CMD_Pedestrian)), \
}; \

CString CCMMvhlItemStdBSDlg::ConvVehicleNameStrRaw2Trans(const CString& strRaw) const
{
	__static_caVehicleLoadName;
	return ConvStrRaw2Trans(caVehicleLoadName, strRaw);
}

CString CCMMvhlItemStdBSDlg::ConvVehicleNameStrTrans2Raw(const CString& strTrans) const
{
	__static_caVehicleLoadName;
	return ConvStrTrans2Raw(caVehicleLoadName, strTrans);
}

#undef __static_caVehicleLoadName

CString CCMMvhlItemStdBSDlg::GetVehicleTypeStr() const
{
	CString CurTransStr;
	m_wndVehicleTypeList.GetWindowText(CurTransStr);
	return ConvVehicleTypeStrTrans2Raw(CurTransStr);
}

CString CCMMvhlItemStdBSDlg::GetSelVihicleStr() const
{
	CString CurTransStr;
	m_cmbSelVihicle.GetWindowText(CurTransStr);
	return ConvSelVihicleStrTrans2Raw(CurTransStr);
}