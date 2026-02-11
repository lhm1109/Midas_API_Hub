// CMSdviDisp.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSdviDlg.h"
#include "CMSdviGrid.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\IGenSDCDBManager.h"
#include "..\wg_db\MathFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SDVI_DAMP_SINGLE 0
#define SDVI_DAMP_KELVIN 1
#define SDVI_DAMP_MAXWEL 2

#define SDVI_DASH_LINEAR 0
#define SDVI_DASH_ELBILI 1
#define SDVI_DASH_EXFUNC 2

#define SDVI_DOF_DX 0
#define SDVI_DOF_DY 1
#define SDVI_DOF_DZ 2 
#define SDVI_DOF_RX 3
#define SDVI_DOF_RY 4
#define SDVI_DOF_RZ 5

/////////////////////////////////////////////////////////////////////////////
// CCMSdviDlg dialog

CCMSdviDlg::CCMSdviDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSdviDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMSdviDlg)
	m_bModify = FALSE;
	m_key = 0;
	m_data.Initialize();
	m_data.CompProp[0].bDof=TRUE;

	m_Name = _T("");
	m_Desc = _T("");
	m_nInputMethod = 0;
	m_nDamperType  = SDVI_DAMP_SINGLE;
	m_nDashpotType = SDVI_DASH_LINEAR;
	m_nInputType   = 0;
	m_nInputType_Exfn=0;
	m_nDOF          = SDVI_DOF_DX;

	m_pIGenSDCDB=CIGenSDCDBManager::GetInstance();
	m_bOnInit = TRUE;

	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDVP_CE_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDVP_P1_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDVP_C1_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDVP_A1_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDVP_KB_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDVP_EXFN_PY_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDVP_EXFN_VY_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDVP_EXFN_DE_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDVP_EXFN_DC_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDVP_EXFN_CE_EDT);

	m_aCtrlID_DBValue_Disabled.Add(IDC_ETC_SDVP_DAMP_SINGLE_RDO);
	m_aCtrlID_DBValue_Disabled.Add(IDC_ETC_SDVP_DAMP_KELVIN_RDO);
	m_aCtrlID_DBValue_Disabled.Add(IDC_ETC_SDVP_DAMP_MAXWEL_RDO);
	m_aCtrlID_DBValue_Disabled.Add(IDC_ETC_SDVP_DASH_LELA_RDO);
	m_aCtrlID_DBValue_Disabled.Add(IDC_ETC_SDVP_DASH_EBIL_RDO);
	m_aCtrlID_DBValue_Disabled.Add(IDC_ETC_SDVP_DASH_EXFN_RDO);
	m_aCtrlID_DBValue_Disabled.Add(IDC_ETC_SDVP_INTER_A12_RDO);
	m_aCtrlID_DBValue_Disabled.Add(IDC_ETC_SDVP_INTER_C12_RDO);
	m_aCtrlID_DBValue_Disabled.Add(IDC_ETC_SDVP_INTER_EXFN_P1_RDO);
	m_aCtrlID_DBValue_Disabled.Add(IDC_ETC_SDVP_INTER_EXFN_P2_RDO);

	m_aCtrlID_ShowElastic.Add(IDC_ETC_SDVP_INTER_A12_RDO);
	m_aCtrlID_ShowElastic.Add(IDC_ETC_SDVP_INTER_C12_RDO);
	m_aCtrlID_ShowElastic.Add(IDC_ETC_SDVP_CE_STC);
	m_aCtrlID_ShowElastic.Add(IDC_ETC_SDVP_CE_EDT);
	m_aCtrlID_ShowElastic.Add(IDC_ETC_SDVP_CE_UNT);
	m_aCtrlID_ShowElastic.Add(IDC_ETC_SDVP_P1_STC);
	m_aCtrlID_ShowElastic.Add(IDC_ETC_SDVP_P1_EDT);
	m_aCtrlID_ShowElastic.Add(IDC_ETC_SDVP_P1_UNT);
	m_aCtrlID_ShowElastic.Add(IDC_ETC_SDVP_V1_STC);
	m_aCtrlID_ShowElastic.Add(IDC_ETC_SDVP_V1_EDT);
	m_aCtrlID_ShowElastic.Add(IDC_ETC_SDVP_V1_UNT);
	m_aCtrlID_ShowElastic.Add(IDC_ETC_SDVP_C1_STC);
	m_aCtrlID_ShowElastic.Add(IDC_ETC_SDVP_C1_EDT);
	m_aCtrlID_ShowElastic.Add(IDC_ETC_SDVP_C1_UNT);
	m_aCtrlID_ShowElastic.Add(IDC_ETC_SDVP_A1_STC);
	m_aCtrlID_ShowElastic.Add(IDC_ETC_SDVP_A1_EDT);
	m_aCtrlID_ShowElastic.Add(IDC_ETC_SDVP_A1_UNT);

	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_INTER_EXFN_P1_RDO);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_INTER_EXFN_P1_STC);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_INTER_EXFN_P2_RDO);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_INTER_EXFN_P2_STC);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_EXFN_PY_STC);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_EXFN_PY_EDT);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_EXFN_PY_UNT);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_EXFN_VY_STC);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_EXFN_VY_EDT);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_EXFN_VY_UNT);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_EXFN_DE_STC);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_EXFN_DE_EDT);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_EXFN_DE_UNT);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_EXFN_DC_STC);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_EXFN_DC_EDT);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_EXFN_DC_UNT);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_EXFN_CE_STC);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_EXFN_CE_EDT);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_EXFN_CE_UNT);
	m_aCtrlID_ShowExpFunc.Add(IDC_ETC_SDVP_EXFN_CE2_STC);
	//}}AFX_DATA_INIT
}

CCMSdviDlg::~CCMSdviDlg()
{
}

void CCMSdviDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSdviDlg)
	DDX_Control(pDX, IDC_ETC_SDVP_DEVTYPE_CMB, m_cmbDeviceType);
	DDX_Control(pDX, IDC_ETC_SDVP_COMPANY_CMB, m_cmbCompany);
	DDX_Control(pDX, IDC_ETC_SDVP_PRONAME_CMB, m_cmbProductName);
	DDX_Control(pDX, IDC_ETC_SDVP_TYPENUM_CMB, m_cmbTypeNumber);
	DDX_Text   (pDX, IDC_CMD_PROPERTY_NAME   , m_Name);
	DDX_Text   (pDX, IDC_CMD_DESCRIPTION     , m_Desc);
	DDX_Radio  (pDX, IDC_ETC_SDVP_USER_RDO       , m_nInputMethod);
	DDX_Radio  (pDX, IDC_ETC_SDVP_DAMP_SINGLE_RDO, m_nDamperType );
	DDX_Radio  (pDX, IDC_ETC_SDVP_DASH_LELA_RDO  , m_nDashpotType);
	DDX_Radio  (pDX, IDC_ETC_SDVP_INTER_A12_RDO  , m_nInputType  );
	DDX_Radio  (pDX, IDC_ETC_SDVP_DOF_DX_RDO  , m_nDOF);
	DDX_Control(pDX, IDC_ETC_SDVP_CE_EDT, m_edtCe);
	DDX_Control(pDX, IDC_ETC_SDVP_P1_EDT, m_edtP1);
	DDX_Control(pDX, IDC_ETC_SDVP_V1_EDT, m_edtV1);
	DDX_Control(pDX, IDC_ETC_SDVP_C1_EDT, m_edtC1);
	DDX_Control(pDX, IDC_ETC_SDVP_A1_EDT, m_edtA1);
	DDX_Control(pDX, IDC_ETC_SDVP_KB_EDT, m_edtKb);
	DDX_Radio  (pDX, IDC_ETC_SDVP_INTER_EXFN_P1_RDO, m_nInputType_Exfn  );
	DDX_Control(pDX, IDC_ETC_SDVP_EXFN_PY_EDT, m_edtPy_Exfn);
	DDX_Control(pDX, IDC_ETC_SDVP_EXFN_VY_EDT, m_edtVy_Exfn);
	DDX_Control(pDX, IDC_ETC_SDVP_EXFN_DE_EDT, m_edtDe_Exfn);
	DDX_Control(pDX, IDC_ETC_SDVP_EXFN_DC_EDT, m_edtDc_Exfn);
	DDX_Control(pDX, IDC_ETC_SDVP_EXFN_CE_EDT, m_edtCe_Exfn);

	DDX_Control(pDX, IDC_ETC_SDVI_DAMPER_STC, m_ImgDamper);
	DDX_Control(pDX, IDC_ETC_SDVI_DASHPOT_STC, m_ImgDashpot);
	DDX_Control(pDX, IDC_ETC_SDVP_INTER_EXFN_P1_STC, m_ImgExfnP1);
	DDX_Control(pDX, IDC_ETC_SDVP_INTER_EXFN_P2_STC, m_ImgExfnP2);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMSdviDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSdviDlg)
	ON_BN_CLICKED(IDCANCEL, OnCmdCancel)
	ON_BN_CLICKED(IDOK    , OnCmdOK)
	ON_BN_CLICKED(IDC_CMD_APPLY , OnCmdApply)
	ON_BN_CLICKED(IDC_ETC_SDVP_USER_RDO       , OnCmdInputMethodRdo)
	ON_BN_CLICKED(IDC_ETC_SDVP_IMDB_RDO       , OnCmdInputMethodRdo)
	ON_BN_CLICKED(IDC_ETC_SDVP_DAMP_SINGLE_RDO, OnCmdDamperTypeRdo)
	ON_BN_CLICKED(IDC_ETC_SDVP_DAMP_KELVIN_RDO, OnCmdDamperTypeRdo)
	ON_BN_CLICKED(IDC_ETC_SDVP_DAMP_MAXWEL_RDO, OnCmdDamperTypeRdo)
	ON_BN_CLICKED(IDC_ETC_SDVP_DASH_LELA_RDO  , OnCmdDashopotTypeRdo)
	ON_BN_CLICKED(IDC_ETC_SDVP_DASH_EBIL_RDO  , OnCmdDashopotTypeRdo)
	ON_BN_CLICKED(IDC_ETC_SDVP_DASH_EXFN_RDO  , OnCmdDashopotTypeRdo)
	ON_BN_CLICKED(IDC_ETC_SDVP_INTER_A12_RDO  , OnCmdInputTypeRdo)
	ON_BN_CLICKED(IDC_ETC_SDVP_INTER_C12_RDO  , OnCmdInputTypeRdo)
	ON_BN_CLICKED(IDC_ETC_SDVP_INTER_EXFN_P1_RDO, OnCmdInputTypeRdo_Exfn)
	ON_BN_CLICKED(IDC_ETC_SDVP_INTER_EXFN_P2_RDO, OnCmdInputTypeRdo_Exfn)
	ON_BN_CLICKED(IDC_ETC_SDVP_DOF_DX_RDO , OnCmdDOFRdo)
	ON_BN_CLICKED(IDC_ETC_SDVP_DOF_DY_RDO , OnCmdDOFRdo)
	ON_BN_CLICKED(IDC_ETC_SDVP_DOF_DZ_RDO , OnCmdDOFRdo)
	ON_BN_CLICKED(IDC_ETC_SDVP_DOF_RX_RDO , OnCmdDOFRdo)
	ON_BN_CLICKED(IDC_ETC_SDVP_DOF_RY_RDO , OnCmdDOFRdo)
	ON_BN_CLICKED(IDC_ETC_SDVP_DOF_RZ_RDO , OnCmdDOFRdo)
	ON_EN_CHANGE(IDC_ETC_SDVP_P1_EDT, SetAutoCalcV1)
	ON_EN_CHANGE(IDC_ETC_SDVP_CE_EDT, OnCmdCeEdtChanged)
	ON_EN_CHANGE(IDC_ETC_SDVP_A1_EDT, SetAutoCalcC1a1)
	ON_EN_CHANGE(IDC_ETC_SDVP_C1_EDT, SetAutoCalcC1a1)
	ON_EN_CHANGE(IDC_ETC_SDVP_CE_EDT, OnCmdCeEdtChanged)  
	ON_EN_CHANGE(IDC_ETC_SDVP_EXFN_PY_EDT, SetAutoCalcExfnVal)
	ON_EN_CHANGE(IDC_ETC_SDVP_EXFN_VY_EDT, SetAutoCalcExfnVal)
	ON_EN_CHANGE(IDC_ETC_SDVP_EXFN_DE_EDT, OnCmdExfnDEEdtChanged)
	ON_EN_CHANGE(IDC_ETC_SDVP_EXFN_DC_EDT, SetAutoCalcExfnVal)
	ON_CBN_SELCHANGE(IDC_ETC_SDVP_DEVTYPE_CMB, OnSelChangeDeviceType )
	ON_CBN_SELCHANGE(IDC_ETC_SDVP_COMPANY_CMB, OnSelChangeCompany    )
	ON_CBN_SELCHANGE(IDC_ETC_SDVP_PRONAME_CMB, OnSelChangeProductName)
	ON_CBN_SELCHANGE(IDC_ETC_SDVP_TYPENUM_CMB, OnSelChangeTypeNumber )
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCMSdviDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitCtrl();
	Data2Dlg();
	EnableDisableCtrls();
	((CEdit*)GetDlgItem(IDC_ETC_SDVP_V1_EDT))->SetReadOnly(TRUE);

	SetDamperBmpImage();
	SetDashpotBmpImage();
	UpdateUnitText();
	if(m_data.nInputMethod==1)
	{
		UpdateDeviceType();
	}

	GetDlgItem(IDC_ETC_SDVP_A1_UNT)->ShowWindow(SW_HIDE);

	m_bOnInit=FALSE;
	if(!m_bModify)
	{
		SetAutoCalcV1();
		SetAutoCalcC1a1();
		SetAutoCalcExfnVal();
	}

    InitCtrlTexts();

	return FALSE;
}

BOOL CCMSdviDlg::OnExecute()
{
	UpdateData(TRUE);

	if (!Dlg2Data())
		return FALSE;

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	if (m_bModify)
	{
		if (!pDoc->m_pDataCtrl->ModifySdvi(m_key, m_key, m_data))
			return FALSE;
	}
	else
	{
		if (!pDoc->m_pDataCtrl->AddSdvi(m_data))
			return FALSE;
	}
	return TRUE;
}

void CCMSdviDlg::OnCmdCancel()
{
	CDialogMove::OnCancel();
}

void CCMSdviDlg::OnCmdOK()
{
	if(!OnExecute())
	{
		return;
	}

	CDialogMove::OnOK();
}

void CCMSdviDlg::OnCmdApply()
{
	if (OnExecute())
	{
	}
}

void CCMSdviDlg::DataInit()
{
	m_data.Initialize();
}

void CCMSdviDlg::Data2Dlg()
{
	m_Name = m_data.strName;
	m_Desc = m_data.strDesc;
	m_nInputMethod = m_data.nInputMethod;
	m_nDamperType = m_data.nDamperType;
	m_nDashpotType = m_data.nDashpotType;
	m_nInputType = m_data.nInputType;
	m_nInputType_Exfn = m_data.nInputType_Exfn;
	m_nDOF       = m_data.GetActiveDOF();
	m_bCe_Exfn = m_data.CompProp[m_nDOF].bExfn_Ce;

	CString str;
	str.Format(_T("%g"), m_data.CompProp[m_nDOF].Ce);     m_edtCe.SetWindowText(str);
	str.Format(_T("%g"), m_data.CompProp[m_nDOF].P1);     m_edtP1.SetWindowText(str);
	str.Format(_T("%g"), m_data.CompProp[m_nDOF].C1);     m_edtC1.SetWindowText(str);
	str.Format(_T("%g"), m_data.CompProp[m_nDOF].Alpha1); m_edtA1.SetWindowText(str);
	str.Format(_T("%g"), m_data.CompProp[m_nDOF].K0);     m_edtKb.SetWindowText(str);
	
	str.Format(_T("%g"), m_data.CompProp[m_nDOF].Exfn_Py); m_edtPy_Exfn.SetWindowText(str);
	str.Format(_T("%g"), m_data.CompProp[m_nDOF].Exfn_Vy); m_edtVy_Exfn.SetWindowText(str);
	str.Format(_T("%g"), m_data.CompProp[m_nDOF].Exfn_DE); m_edtDe_Exfn.SetWindowText(str);
	str.Format(_T("%g"), m_data.CompProp[m_nDOF].Exfn_DC); m_edtDc_Exfn.SetWindowText(str);
	str.Format(_T("%g"), m_data.CompProp[m_nDOF].Exfn_Ce); m_edtCe_Exfn.SetWindowText(str);

	UpdateData(FALSE);
}

BOOL CCMSdviDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_data.Initialize();
	m_data.strName = m_Name;
	m_data.strDesc = m_Desc;
	if(m_nInputMethod==1)
	{
		m_cmbDeviceType.GetLBText(m_cmbDeviceType.GetCurSel(), m_data.strDeviceType);
		m_cmbCompany.GetLBText(m_cmbCompany.GetCurSel(), m_data.strCompany);
		m_cmbProductName.GetLBText(m_cmbProductName.GetCurSel(), m_data.strProductName);
		m_cmbTypeNumber.GetLBText(m_cmbTypeNumber.GetCurSel(), m_data.strTypeNumber);
	}  
	m_data.nInputMethod = m_nInputMethod;
	m_data.nDamperType  = m_nDamperType;
	m_data.nDashpotType = m_nDashpotType;
	m_data.nInputType   = m_nInputType;
	m_data.nInputType_Exfn = m_nInputType_Exfn;

	m_data.CompProp[m_nDOF].bDof   = TRUE;

	CString str;
	m_edtCe.GetWindowText(str); m_data.CompProp[m_nDOF].Ce     = _ttof(str);
	m_edtP1.GetWindowText(str); m_data.CompProp[m_nDOF].P1     = _ttof(str);
	m_edtC1.GetWindowText(str); m_data.CompProp[m_nDOF].C1     = _ttof(str);
	m_edtA1.GetWindowText(str); m_data.CompProp[m_nDOF].Alpha1 = _ttof(str);
	m_edtKb.GetWindowText(str); m_data.CompProp[m_nDOF].K0     = _ttof(str);

	m_edtPy_Exfn.GetWindowText(str); m_data.CompProp[m_nDOF].Exfn_Py = _ttof(str);
	m_edtVy_Exfn.GetWindowText(str); m_data.CompProp[m_nDOF].Exfn_Vy = _ttof(str);
	m_edtDe_Exfn.GetWindowText(str); m_data.CompProp[m_nDOF].Exfn_DE = _ttof(str);
	m_edtDc_Exfn.GetWindowText(str); m_data.CompProp[m_nDOF].Exfn_DC = _ttof(str);
	m_edtCe_Exfn.GetWindowText(str); m_data.CompProp[m_nDOF].Exfn_Ce = _ttof(str);
	m_data.CompProp[m_nDOF].bExfn_Ce = m_bCe_Exfn;

	return TRUE;
}

void CCMSdviDlg::EnableDisableCtrls()
{
	UpdateData(TRUE);

	BOOL bDBDevice=m_nInputMethod==1;
	if(!bDBDevice)
	{
		for(int i=0; i<m_aCtrlID_DBValue_ReadOnly.GetSize(); i++)
		{
			((CEdit*)GetDlgItem(m_aCtrlID_DBValue_ReadOnly[i]))->SetReadOnly(FALSE);
		}
		for(int i=0; i<m_aCtrlID_DBValue_Disabled.GetSize(); i++)
		{
			GetDlgItem(m_aCtrlID_DBValue_Disabled[i])->EnableWindow(TRUE);
		}
	}

	CDlgUtil::CtrlShowHide(this, m_aCtrlID_ShowElastic, m_nDashpotType==0 || m_nDashpotType==1);
	CDlgUtil::CtrlShowHide(this, m_aCtrlID_ShowExpFunc, m_nDashpotType==2);

	GetDlgItem(IDC_ETC_SDVP_A1_UNT)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_ETC_SDVP_DEVTYPE_STC)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDVP_DEVTYPE_CMB)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDVP_COMPANY_STC)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDVP_COMPANY_CMB)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDVP_PRONAME_STC)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDVP_PRONAME_CMB)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDVP_TYPENUM_STC)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDVP_TYPENUM_CMB)->EnableWindow(m_nInputMethod==1);

	GetDlgItem(IDC_ETC_SDVP_DOF_RX_RDO)->EnableWindow(m_nInputMethod!=1);
	GetDlgItem(IDC_ETC_SDVP_DOF_RY_RDO)->EnableWindow(m_nInputMethod!=1);
	GetDlgItem(IDC_ETC_SDVP_DOF_RZ_RDO)->EnableWindow(m_nInputMethod!=1);

	GetDlgItem(IDC_ETC_SDVP_INTER_A12_RDO)->EnableWindow(m_nDashpotType==1);
	GetDlgItem(IDC_ETC_SDVP_INTER_C12_RDO)->EnableWindow(m_nDashpotType==1);
		
	GetDlgItem(IDC_ETC_SDVP_P1_EDT)->EnableWindow(m_nDashpotType==1); 
	GetDlgItem(IDC_ETC_SDVP_V1_EDT)->EnableWindow(m_nDashpotType==1); 
	GetDlgItem(IDC_ETC_SDVP_C1_EDT)->EnableWindow(m_nDashpotType==1); 
	GetDlgItem(IDC_ETC_SDVP_A1_EDT)->EnableWindow(m_nDashpotType==1); 

	GetDlgItem(IDC_ETC_SDVP_DASH_EXFN_RDO)->EnableWindow(m_nDamperType==SDVI_DAMP_SINGLE); 
	
	if(m_nDashpotType==1)
	{
		if(m_nInputType==0)
		{
			((CEdit*)GetDlgItem(IDC_ETC_SDVP_A1_EDT))->SetReadOnly(FALSE);
			((CEdit*)GetDlgItem(IDC_ETC_SDVP_C1_EDT))->SetReadOnly(TRUE);
		}
		else if(m_nInputType==1)
		{
			((CEdit*)GetDlgItem(IDC_ETC_SDVP_A1_EDT))->SetReadOnly(TRUE);
			((CEdit*)GetDlgItem(IDC_ETC_SDVP_C1_EDT))->SetReadOnly(FALSE);
		}
		else ASSERT(0);
	}
	else if(m_nDashpotType==2)
	{
		((CEdit*)GetDlgItem(IDC_ETC_SDVP_EXFN_PY_EDT))->SetReadOnly(m_nInputType_Exfn==1); 
		((CEdit*)GetDlgItem(IDC_ETC_SDVP_EXFN_VY_EDT))->SetReadOnly(m_nInputType_Exfn==1); 
		((CEdit*)GetDlgItem(IDC_ETC_SDVP_EXFN_DC_EDT))->SetReadOnly(m_nInputType_Exfn==0); 
	}

	GetDlgItem(IDC_ETC_SDVP_KB_EDT)->EnableWindow(m_nDamperType!=0); 

	if(bDBDevice)
	{
		for(int i=0; i<m_aCtrlID_DBValue_ReadOnly.GetSize(); i++)
		{
			((CEdit*)GetDlgItem(m_aCtrlID_DBValue_ReadOnly[i]))->SetReadOnly(TRUE);
		}
		for(int i=0; i<m_aCtrlID_DBValue_Disabled.GetSize(); i++)
		{
			GetDlgItem(m_aCtrlID_DBValue_Disabled[i])->EnableWindow(FALSE);
		}
	}
}

void CCMSdviDlg::OnCmdInputMethodRdo()
{
	UpdateData(TRUE);
	if(m_nInputMethod==1) UpdateDeviceType();
	EnableDisableCtrls();
	UpdateProperties();
}
void CCMSdviDlg::OnCmdDamperTypeRdo()
{
	UpdateData(TRUE);
	if(m_nDamperType!=SDVI_DAMP_SINGLE && m_nDashpotType==SDVI_DASH_EXFUNC)
	{
		m_nDashpotType=SDVI_DASH_LINEAR;
		UpdateData(FALSE);
	}

	EnableDisableCtrls();
	SetDamperBmpImage();
	SetDashpotBmpImage();
	UpdateProperties();
}
void CCMSdviDlg::OnCmdDashopotTypeRdo()
{
	EnableDisableCtrls();
	SetDashpotBmpImage();
	UpdateProperties();
}
void CCMSdviDlg::OnCmdInputTypeRdo()
{
	m_edtA1.SetWindowText(_T("0"));
	m_edtC1.SetWindowText(_T("0"));

	EnableDisableCtrls();
	UpdateProperties();
}
void CCMSdviDlg::UpdateProperties()
{
	UpdateData(TRUE);
	SetAutoCalcC1a1();
	SetAutoCalcExfnVal();
}

void CCMSdviDlg::SetDamperBmpImage()
{
	UpdateData(TRUE);
	
	//UINT aImgPath[3] = { IDB_ETC_SDVI_SINGLE, IDB_ETC_SDVI_KELVIN, IDB_ETC_SDVI_MAXWEL };
	CString aImgPath[3] = { _T("SVG\\illustration\\Dialog\\Sdvi_DASHPOT.svg"),
		_T("SVG\\illustration\\Dialog\\Sdvi_VOIGT.svg"), 
		_T("SVG\\illustration\\Dialog\\Sdvi_MAXWELL.svg") };

	m_ImgDamper.SetImage(aImgPath[m_nDamperType]);
}

void CCMSdviDlg::SetDashpotBmpImage()
{
	UpdateData(TRUE);
	
	//UINT aBitmapID[3] = { IDB_ETC_SDVI_LELA, IDB_ETC_SDVI_EBIL, IDB_ETC_SDVI_EXFN };

	CString aImgPath[3] = { _T("SVG\\illustration\\Dialog\\Sdvi_OIL_Linear.svg"),
		_T("SVG\\illustration\\Dialog\\Sdvi_OIL_Bilinear.svg"),
		_T("SVG\\illustration\\Dialog\\Sdvi_OIL_ExpFunc.svg") };

	m_ImgDashpot.SetImage(aImgPath[m_nDashpotType]);
}

void CCMSdviDlg::OnCmdDOFRdo()
{
	UpdateData(TRUE);
	UpdateUnitText();
	SetAutoCalcV1();
}

void CCMSdviDlg::UpdateUnitText()
{
	UpdateData(TRUE);

	T_SDVI_UNIT* pUnit = &(CUnitCtrl::m_SDVI_UNIT);
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CString strTitle,strUnit[6],strExfnProp[5];

	if(m_nDOF<=SDVI_DOF_DZ) // Dx,Dy,Dz
	{
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Ce_Dxyz, strUnit[0]); 
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->P1_Dxyz, strUnit[1]); 
		pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_VELOCITY, strUnit[2]); 
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->C1_Dxyz, strUnit[3]); 
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->K0_Dxyz, strUnit[5]); 
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Exfn_Py_Dxyz  , strExfnProp[0]); 
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Exfn_Vy_Dxyz  , strExfnProp[1]); 
		pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_NONE, strExfnProp[2]); 
		strExfnProp[3] = CUnitCtrl::GetSdviExfnCUnitStr(0); 
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Exfn_Ce_Dxyz  , strExfnProp[4]); 
	}
	else // Rx,Ry,Rz
	{
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Ce_Rxyz, strUnit[0]);
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->P1_Rxyz, strUnit[1]);
		pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_FREQUENCY, strUnit[2]); 
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->C1_Rxyz, strUnit[3]);
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->K0_Rxyz, strUnit[5]);
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Exfn_Py_Rxyz  , strExfnProp[0]); 
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Exfn_Vy_Rxyz  , strExfnProp[1]); 
		pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_NONE, strExfnProp[2]); 
		strExfnProp[3] = CUnitCtrl::GetSdviExfnCUnitStr(1); 
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Exfn_Ce_Rxyz  , strExfnProp[4]); 
	}

	GetDlgItem(IDC_ETC_SDVP_CE_UNT)->SetWindowText(strUnit[0]);
	GetDlgItem(IDC_ETC_SDVP_P1_UNT)->SetWindowText(strUnit[1]);
	GetDlgItem(IDC_ETC_SDVP_V1_UNT)->SetWindowText(strUnit[2]);
	GetDlgItem(IDC_ETC_SDVP_C1_UNT)->SetWindowText(strUnit[3]);
	GetDlgItem(IDC_ETC_SDVP_KB_UNT)->SetWindowText(strUnit[5]);
	GetDlgItem(IDC_ETC_SDVP_EXFN_PY_UNT)->SetWindowText(strExfnProp[0]);
	GetDlgItem(IDC_ETC_SDVP_EXFN_VY_UNT)->SetWindowText(strExfnProp[1]);
	GetDlgItem(IDC_ETC_SDVP_EXFN_DE_UNT)->SetWindowText(strExfnProp[2]);
	GetDlgItem(IDC_ETC_SDVP_EXFN_DC_UNT)->SetWindowText(strExfnProp[3]);
	GetDlgItem(IDC_ETC_SDVP_EXFN_CE_UNT)->SetWindowText(strExfnProp[4]);
}

void CCMSdviDlg::SetAutoCalcV1()
{
	if(m_bOnInit) return;
	
	CString str;
	m_edtP1.GetWindowText(str);
	double P1 = _ttof(str);
	m_edtCe.GetWindowText(str);
	double Ce = _ttof(str);
	double V1 = Ce!=0.? P1/Ce : 0.;
	str.Format(_T("%g"),V1);
	m_edtV1.SetWindowText(str);
}

void CCMSdviDlg::SetAutoCalcC1a1()
{  
	if(m_bOnInit) return;

	// Input type : a1 -> C1=a1*Ce
	if(m_nInputType==0)
	{
		CString str;
		m_edtA1.GetWindowText(str);
		double a1 = _ttof(str);
		m_edtCe.GetWindowText(str);
		double Ce = _ttof(str);
		double C1 = a1*Ce;
		str.Format(_T("%g"),C1);
		m_edtC1.SetWindowText(str);
	}
	// Input type : C1 -> a1=C1/Ce
	else
	{
		CString str;
		m_edtC1.GetWindowText(str);
		double C1 = _ttof(str);
		m_edtCe.GetWindowText(str);
		double Ce = _ttof(str);
		double a1 = Ce!=0.? C1/Ce : 0.;
		str.Format(_T("%g"),a1);
		m_edtA1.SetWindowText(str);
	}
}

void CCMSdviDlg::OnCmdCeEdtChanged()
{  
	SetAutoCalcV1();
	SetAutoCalcC1a1();
}

void CCMSdviDlg::OnSelChangeDeviceType()
{  
	UpdateData(TRUE);
	UpdateCompany();
}
void CCMSdviDlg::OnSelChangeCompany    ()
{
	UpdateData(TRUE);
	UpdateProductName();
}
void CCMSdviDlg::OnSelChangeProductName()
{
	UpdateData(TRUE);
	UpdateTypeNumber();
}
void CCMSdviDlg::OnSelChangeTypeNumber ()
{
	UpdateData(TRUE);
	int nIdx = m_cmbTypeNumber.GetCurSel();
	if(nIdx<0) return;
	CString strTypeNum;
	m_cmbTypeNumber.GetLBText(m_cmbTypeNumber.GetCurSel(), strTypeNum);
	m_pIGenSDCDB->GetSdviFromDB(strTypeNum,m_data);
	m_data.strName = m_Name;
	m_data.strDesc = m_Desc;
	Data2Dlg();
	EnableDisableCtrls();
}

int CCMSdviDlg::GetDeviceTypeInt(CString strDeviceType)
{
	if(m_data.strDeviceType.IsEmpty()) return IGENSDC_DMP_OILDAMP;

			 if(m_data.strDeviceType==_LS(IDS_DB_IGEN_SDC_DMP_OILDAMP)) return IGENSDC_DMP_OILDAMP;
	else if(m_data.strDeviceType==_LS(IDS_DB_IGEN_SDC_ISL_OILDAMP)) return IGENSDC_ISL_OILDAMP;
	else if(m_data.strDeviceType==_LS(IDS_DB_IGEN_SDC_DMP_VSCDAMP)) return IGENSDC_DMP_VSCDAMP;
	else if(m_data.strDeviceType==_LS(IDS_DB_IGEN_SDC_ISL_VSCDAMP)) return IGENSDC_ISL_VSCDAMP;

	ASSERT(0);
	return IGENSDC_DMP_OILDAMP;
}

void CCMSdviDlg::InitCtrlTexts()
{
    GetDlgItem(IDC_ETC_SDVP_DASHPROP_GRP)->SetWindowText(_LS(IDS_WG_CMD_DASHPOT_PROPERTIES));
    GetDlgItem(IDC_ETC_SDVP_CE_STC)->SetWindowText(_LS(IDS_WG_CMD_INIT_DAMP_COEFF));
    GetDlgItem(IDC_ETC_SDVP_P1_STC)->SetWindowText(_LS(IDS_WG_CMD_RELIEF_DAMP_FORCE));
    GetDlgItem(IDC_ETC_SDVP_V1_STC)->SetWindowText(_LS(IDS_WG_CMD_RELIEF_VELOCITY));
    GetDlgItem(IDC_ETC_SDVP_C1_STC)->SetWindowText(_LS(IDS_WG_CMD_SECOND_DAMP_COEFF));
    GetDlgItem(IDC_ETC_SDVP_A1_STC)->SetWindowText(_LS(IDS_WG_CMD_DAMP_REDUCTION_FACTOR));
    GetDlgItem(IDC_ETC_SDVP_SPRGPROP_GRP)->SetWindowText(_LS(IDS_WG_CMD_SPRING_PROPERTIES));
    GetDlgItem(IDC_ETC_SDVP_KB_STC)->SetWindowText(_LS(IDS_WG_CMD_SPRING_STIFFNESS));
}

void CCMSdviDlg::UpdateDeviceType()
{
	// 일본 버전에만 점성 댐퍼 탑재
	BOOL bUseVscDamp = FALSE;
#if defined(_JP)
	bUseVscDamp = TRUE;
#endif
	if(CTestEnvMgr::GetTestEnvST(_LSX(UseSeisCtrlDevice)) == _LSX(yes)) bUseVscDamp = TRUE;

	m_cmbDeviceType.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbDeviceType, _LS(IDS_DB_IGEN_SDC_DMP_OILDAMP), IGENSDC_DMP_OILDAMP);
	if(bUseVscDamp) CDlgUtil::CobxAddItem(m_cmbDeviceType, _LS(IDS_DB_IGEN_SDC_DMP_VSCDAMP), IGENSDC_DMP_VSCDAMP);
	CDlgUtil::CobxAddItem(m_cmbDeviceType, _LS(IDS_DB_IGEN_SDC_ISL_OILDAMP), IGENSDC_ISL_OILDAMP);
	if(bUseVscDamp) CDlgUtil::CobxAddItem(m_cmbDeviceType, _LS(IDS_DB_IGEN_SDC_ISL_VSCDAMP), IGENSDC_ISL_VSCDAMP);

	CDlgUtil::CobxSetCurSelItemData(m_cmbDeviceType, GetDeviceTypeInt(m_data.strDeviceType));
	UpdateCompany();
}
void CCMSdviDlg::UpdateCompany()
{
	int nDBType = CDlgUtil::CobxGetCurSelItemData(m_cmbDeviceType, m_cmbDeviceType.GetCurSel());
	CStringArray aDBCompany;
	m_pIGenSDCDB->GetCompanyList(nDBType,aDBCompany);
	m_cmbCompany.ResetContent();
	for(int i=0; i<aDBCompany.GetSize(); i++)
	{
		CDlgUtil::CobxAddItem(m_cmbCompany, aDBCompany[i], i);
	}
	
	int nCmbIdx = m_cmbCompany.FindString(0, m_data.strCompany);
	if(nCmbIdx==-1) m_cmbCompany.SetCurSel(0);
	else            m_cmbCompany.SetCurSel(nCmbIdx);
	UpdateProductName();
}

void CCMSdviDlg::UpdateProductName()
{
	int nDBType = CDlgUtil::CobxGetCurSelItemData(m_cmbDeviceType, m_cmbDeviceType.GetCurSel());
	int nIdxCompany= m_cmbCompany.GetCurSel();
	if(nIdxCompany<0) { ASSERT(0); return; } // 회사가 없다니
	
	CString strCompany;
	m_cmbCompany.GetLBText(nIdxCompany, strCompany);
	
	CStringArray aDBPrdName;
	m_pIGenSDCDB->GetPrdNameList(nDBType,strCompany,aDBPrdName);
	m_cmbProductName.ResetContent();
	for(int i=0; i<aDBPrdName.GetSize(); i++)
	{
		CDlgUtil::CobxAddItem(m_cmbProductName, aDBPrdName[i], i);
	}

	int nCmbIdx = m_cmbProductName.FindString(0, m_data.strProductName);
	if(nCmbIdx==-1) m_cmbProductName.SetCurSel(0);
	else            m_cmbProductName.SetCurSel(nCmbIdx);
	UpdateTypeNumber();
}

void CCMSdviDlg::UpdateTypeNumber()
{
	int nDBType = CDlgUtil::CobxGetCurSelItemData(m_cmbDeviceType, m_cmbDeviceType.GetCurSel());
	int nIdxCompany= m_cmbCompany.GetCurSel();
	int nIdxPrdName= m_cmbProductName.GetCurSel();
	if(nIdxCompany<0 || nIdxPrdName<0) { ASSERT(0); return; } 

	CString strCompany, strPrdName;
	m_cmbCompany.GetLBText(nIdxCompany, strCompany);
	m_cmbProductName.GetLBText(nIdxPrdName, strPrdName);

	CStringArray aDBTypeNum;
	m_pIGenSDCDB->GetTypeNumListAndMakeData(nDBType,strCompany,strPrdName,aDBTypeNum);
	m_cmbTypeNumber.ResetContent();
	for(int i=0; i<aDBTypeNum.GetSize(); i++)
	{
		CDlgUtil::CobxAddItem(m_cmbTypeNumber, aDBTypeNum[i], i);
	}
	int nCmbIdx = m_cmbTypeNumber.FindString(0, m_data.strTypeNumber);
	if(nCmbIdx==-1) m_cmbTypeNumber.SetCurSel(0);
	else            m_cmbTypeNumber.SetCurSel(nCmbIdx);
	if(!m_bOnInit) OnSelChangeTypeNumber();
}

void CCMSdviDlg::InitCtrl()
{
	CRect rTo, rMove;
	int nDistX=0, nDistY=0;
	GetDlgItem(IDC_WG_CMD_STATIC8)->GetWindowRect(rTo);
	GetDlgItem(IDC_ETC_SDVP_INTER_EXFN_INPUT_GRP)->GetWindowRect(rMove);
	nDistX = rTo.left-rMove.left;
	nDistY = rTo.top-rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aCtrlID_ShowExpFunc, nDistX, nDistY);

	m_ImgExfnP1.SetImage(_T("SVG\\illustration\\Dialog\\Sdvi_Exfn_Input1.svg"));
	m_ImgExfnP2.SetImage(_T("SVG\\illustration\\Dialog\\Sdvi_Exfn_Input2.svg"));
}

void CCMSdviDlg::OnCmdInputTypeRdo_Exfn()
{
	EnableDisableCtrls();
	UpdateProperties();
}

void CCMSdviDlg::SetAutoCalcExfnVal()
{
	if(m_bOnInit || m_nDashpotType!=2) return;

	CString strPy, strVy, strA, strC;
	m_edtVy_Exfn.GetWindowText(strVy);
	m_edtDe_Exfn.GetWindowText(strA);
	m_edtPy_Exfn.GetWindowText(strPy);
	m_edtDc_Exfn.GetWindowText(strC);
	double dVy=_ttof(strVy);
	double dA =_ttof(strA);
	double dPy=_ttof(strPy);
	double dC =_ttof(strC);

	const double dTol = 1.e-5; // 대화상자에서 5자리까지 표기
	if (m_nInputType_Exfn==0)
	{
		double dC_calc = dPy * pow((1/dVy), dA);
		if (fabs(dC-dC_calc)>dTol)
		{
			strC.Format(_T("%g"), dC_calc);
			m_edtDc_Exfn.SetWindowText(strC);
		}

		if (!m_bCe_Exfn)
			m_edtCe_Exfn.SetWindowText(strC);
	}
	else
	{
		double dPy_calc = dC / pow(1/dVy, dA);
		if (fabs(dPy-dPy_calc)>dTol)
		{
			strPy.Format(_T("%g"), dPy_calc);
			m_edtPy_Exfn.SetWindowText(strPy);
		}

		double dVal_1m = M_InitValueCurUnit(1., KN, M, D_UNITSYS_BASE_LENGTH);
		if (fabs(dVy-dVal_1m)>dTol)
		{
			strVy.Format(_T("%g"), dVal_1m);
			m_edtVy_Exfn.SetWindowText(strVy);
		}

		if (!m_bCe_Exfn)
			m_edtCe_Exfn.SetWindowText(strC);
	}
}

void CCMSdviDlg::OnCmdExfnDEEdtChanged()
{  
	SetAutoCalcExfnVal();
	CString strA;
	m_edtDe_Exfn.GetWindowText(strA);
	double dA = _ttof(strA);

	const double dBenchmark = 1;
	CString strCe2Stc;
	if(dA>dBenchmark)
	{
		strCe2Stc=_LS(IDS_SDVI_EXFN_CE2_STC_1);
	}
	else if(fabs(dA-dBenchmark)<CMathFunc::m_NormalZero)
	{
		strCe2Stc=_LS(IDS_SDVI_EXFN_CE2_STC_2);
	}
	else
	{
		strCe2Stc=_LS(IDS_SDVI_EXFN_CE2_STC_3);
	}
	GetDlgItem(IDC_ETC_SDVP_EXFN_CE2_STC)->SetWindowText(strCe2Stc);
}