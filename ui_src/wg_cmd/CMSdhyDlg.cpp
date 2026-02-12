// CMSdhyDisp.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSdhyDlg.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\IGenSDCDBManager.h"
#include "..\wg_base\wg_base_TestEnvMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSdhyDlg dialog

CCMSdhyDlg::CCMSdhyDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSdhyDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMSdhyDlg)
	m_bModify = FALSE;
	m_key = 0;
	m_data.Initialize();

	m_Name = _T("");
	m_Desc = _T("");
	m_nInputMethod = 0;
	m_pIGenSDCDB=CIGenSDCDBManager::GetInstance();
	m_bOnInit = TRUE;

	m_aCtrlID_DBValue_Disabled.Add(IDC_ETC_SDHY_HMD_CMB);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDHY_K0_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDHY_P1_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDHY_P2_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDHY_A1_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDHY_A2_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDHY_BETA_EDT);

	//}}AFX_DATA_INIT
}

CCMSdhyDlg::~CCMSdhyDlg()
{
}

void CCMSdhyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSdhyDlg)
	DDX_Control(pDX, IDC_ETC_SDHY_COMPANY_CMB, m_cmbCompany);
	DDX_Control(pDX, IDC_ETC_SDHY_PRONAME_CMB, m_cmbProductName);
	DDX_Control(pDX, IDC_ETC_SDHY_TYPENUM_CMB, m_cmbTypeNumber);
	DDX_Control(pDX, IDC_ETC_SDHY_HMD_CMB    , m_cmbHysModel);
	DDX_Control(pDX, IDC_ETC_SDHY_NSS_CMB    , m_cmbNSS);
	DDX_Text   (pDX, IDC_ETC_SDHY_NAME_EDT   , m_Name);
	DDX_Text   (pDX, IDC_ETC_SDHY_DESC_EDT   , m_Desc);
	DDX_Radio  (pDX, IDC_ETC_SDHY_USER_RDO   , m_nInputMethod);
	DDX_Control(pDX, IDC_ETC_SDHY_K0_EDT     , m_edtK0    );
	DDX_Control(pDX, IDC_ETC_SDHY_K0_UNT     , m_untK0    );
	DDX_Control(pDX, IDC_ETC_SDHY_P1_EDT     , m_edtP1    );
	DDX_Control(pDX, IDC_ETC_SDHY_P1_UNT     , m_untP1    );
	DDX_Control(pDX, IDC_ETC_SDHY_P2_EDT     , m_edtP2    );
	DDX_Control(pDX, IDC_ETC_SDHY_P2_UNT     , m_untP2    );
	DDX_Control(pDX, IDC_ETC_SDHY_A1_EDT     , m_edtA1    );
	DDX_Control(pDX, IDC_ETC_SDHY_A2_EDT     , m_edtA2    );
	DDX_Control(pDX, IDC_ETC_SDHY_PHI_EDT    , m_edtPhi   );
	DDX_Control(pDX, IDC_ETC_SDHY_LMBD_EDT   , m_edtLambda);
	DDX_Control(pDX, IDC_ETC_SDHY_BETA_EDT   , m_edtBeta  );
	DDX_Control(pDX, IDC_ETC_SDHY_THT_EDT    , m_edtTheta );
	DDX_Control(pDX, IDC_ETC_SDHY_BMP, m_ImgViewer_Curve);
	DDX_Control(pDX, IDC_ETC_SDHY_BMP2, m_ImgViewer_MSS);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMSdhyDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSdhyDlg)
	ON_BN_CLICKED(IDOK                  , OnCmdOK)
	ON_BN_CLICKED(IDC_CMD_APPLY         , OnCmdApply)
	ON_BN_CLICKED(IDC_ETC_SDHY_USER_RDO       , OnCmdInputMethodRdo)
	ON_BN_CLICKED(IDC_ETC_SDHY_IMDB_RDO       , OnCmdInputMethodRdo)
	ON_CBN_SELCHANGE(IDC_ETC_SDHY_COMPANY_CMB , OnSelChangeCompany    )
	ON_CBN_SELCHANGE(IDC_ETC_SDHY_PRONAME_CMB , OnSelChangeProductName)
	ON_CBN_SELCHANGE(IDC_ETC_SDHY_TYPENUM_CMB , OnSelChangeTypeNumber )
	ON_CBN_SELCHANGE(IDC_ETC_SDHY_HMD_CMB  , OnHysModelCmb)
	ON_CBN_SELCHANGE(IDC_ETC_SDHY_NSS_CMB  , OnNSSCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCMSdhyDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitCtrl();
	Data2Dlg();
	UpdateCtrls();
	if(m_data.nInputMethod==1)
	{
		UpdateCompany();
	}
	m_bOnInit=FALSE;

	return FALSE;
}

BOOL CCMSdhyDlg::OnExecute()
{
	UpdateData(TRUE);

	if (!Dlg2Data())
		return FALSE;

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	if (m_bModify)
	{
		if (!pDoc->m_pDataCtrl->ModifySdhy(m_key, m_key, m_data))
			return FALSE;
	}
	else
	{
		if (!pDoc->m_pDataCtrl->AddSdhy(m_data))
			return FALSE;
	}
	return TRUE;
}

void CCMSdhyDlg::OnCmdOK()
{
	if(!OnExecute())
	{
		return;
	}

	CDialogMove::OnOK();
}

void CCMSdhyDlg::OnCmdApply()
{
	OnExecute();
}

void CCMSdhyDlg::Data2Dlg()
{
	m_Name = m_data.strName;
	m_Desc = m_data.strDesc;
	m_nInputMethod = m_data.nInputMethod;  
	m_cmbHysModel.SetCurSel(m_data.GetHysIdx());
	m_edtK0    .SetEditUnit(m_data.dK0);
	m_edtP1    .SetEditUnit(m_data.dP1);
	m_edtP2    .SetEditUnit(m_data.dP2);
	m_edtA1    .SetEditUnit(m_data.dAlpha1);
	m_edtA2    .SetEditUnit(m_data.dAlpha2);
	m_edtBeta  .SetEditUnit(m_data.dBeta );
	m_edtLambda.SetEditUnit(m_data.dLambda);
	m_edtPhi   .SetEditUnit(m_data.dPhi);

	int nNSSIdx=0;
	switch(m_data.nNumMSS)
	{
	case  2: nNSSIdx=0; break;
	case  4: nNSSIdx=1; break;
	case  6: nNSSIdx=2; break;
	case  8: nNSSIdx=3; break;
	case 10: nNSSIdx=4; break;
	case 12: nNSSIdx=5; break;
	case 18: nNSSIdx=6; break;
	case 24: nNSSIdx=7; break;
	default: nNSSIdx=0; break;
	}
	m_cmbNSS.SetCurSel(nNSSIdx);

	UpdateData(FALSE);
}

BOOL CCMSdhyDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_data.Initialize();
	m_data.strName = m_Name;
	m_data.strDesc = m_Desc;
	m_data.nInputMethod= m_nInputMethod;
	if(m_data.nInputMethod==1)
	{
		m_cmbCompany.GetLBText(m_cmbCompany.GetCurSel(), m_data.strCompany);
		m_cmbProductName.GetLBText(m_cmbProductName.GetCurSel(), m_data.strProductName);
		m_cmbTypeNumber.GetLBText(m_cmbTypeNumber.GetCurSel(), m_data.strTypeNumber);
	}  
	m_data.nHysModel = CDlgUtil::CobxGetCurSelItemData(m_cmbHysModel, m_cmbHysModel.GetCurSel());
	m_data.dK0     = m_edtK0    .GetEditValue();
	m_data.dP1     = m_edtP1    .GetEditValue();
	m_data.dP2     = m_edtP2    .GetEditValue();
	m_data.dAlpha1 = m_edtA1    .GetEditValue();
	m_data.dAlpha2 = m_edtA2    .GetEditValue();
	m_data.dBeta   = m_edtBeta  .GetEditValue();
	m_data.dLambda = m_edtLambda.GetEditValue();
	m_data.dPhi    = m_edtPhi   .GetEditValue();
	m_data.nNumMSS = CDlgUtil::CobxGetCurSelItemData(m_cmbNSS, m_cmbNSS.GetCurSel());

	return TRUE;
}

void CCMSdhyDlg::OnHysModelCmb()
{
	UpdateCtrls();
}
void CCMSdhyDlg::OnNSSCmb()
{
	UpdateCtrls();
}

void CCMSdhyDlg::SetBmpImage()
{
	//UINT aBitmapID_curve[3] = { IDB_ETC_SDHY_BL2, IDB_ETC_SDHY_TL3, IDB_ETC_SDHY_MRO };
	CString aImgPath_curve[3] = {
		_T("SVG\\illustration\\Dialog\\Sdhy_BL2.svg"),
		_T("SVG\\illustration\\Dialog\\Sdhy_TL3.svg"),
		_T("SVG\\illustration\\Dialog\\Sdhy_MRO.svg"),
	};
	m_ImgViewer_Curve.SetImage(aImgPath_curve[m_cmbHysModel.GetCurSel()]);

	m_ImgViewer_MSS.SetImage(_T("SVG\\illustration\\Dialog\\Sdhy_MSS.svg"));
}

void CCMSdhyDlg::InitCtrl()
{
	CRect rRef;
	CRect rToMove;

	GetDlgItem(IDC_ETC_SDHY_PHI_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_ETC_SDHY_BETA_STC)->GetWindowRect(rToMove);
	int nDistX = rRef.left - rToMove.left;
	int nDistY = rRef.top - rToMove.top;

	CArray<UINT,UINT> aCtrl;
	aCtrl.Add(IDC_ETC_SDHY_BETA_STC);
	aCtrl.Add(IDC_ETC_SDHY_BETA_STC2);
	aCtrl.Add(IDC_ETC_SDHY_BETA_EDT);
	CDlgUtil::CtrlMoveDistXY(this, aCtrl, nDistX, nDistY);

	m_edtK0.SetUnitType(CUnitCtrl::m_SDHY_UNIT.dK0);
	m_untK0.SetUnitType(CUnitCtrl::m_SDHY_UNIT.dK0);
	m_edtP1.SetUnitType(CUnitCtrl::m_SDHY_UNIT.dP1);
	m_edtP2.SetUnitType(CUnitCtrl::m_SDHY_UNIT.dP2);
	m_untP1.SetUnitType(CUnitCtrl::m_SDHY_UNIT.dP1);
	m_untP2.SetUnitType(CUnitCtrl::m_SDHY_UNIT.dP2);

	CDlgUtil::CobxAddItem(m_cmbHysModel, _LS(IDS_ETC_SDHY_HYS_DBM), D_SDHY_HYS_DEGRBL);
	CDlgUtil::CobxAddItem(m_cmbHysModel, _LS(IDS_ETC_SDHY_HYS_NTM), D_SDHY_HYS_NORTRI);
	
	// PMS 5253-21 : Hysteretic Isolator(MSS)의 “Modified Ramberg-Osgood Model” 비활성합니다. 
	if(CTestEnvMgr::IsTestEnvST(_T("SeisCtrlDeviceLater"), _T("yes")))
		CDlgUtil::CobxAddItem(m_cmbHysModel, _LS(IDS_ETC_SDHY_HYS_MRO), D_SDHY_HYS_MODIRO);

	CDlgUtil::CobxAddItem(m_cmbNSS, _T(" 2"),  2);
	CDlgUtil::CobxAddItem(m_cmbNSS, _T(" 4"),  4);
	CDlgUtil::CobxAddItem(m_cmbNSS, _T(" 6"),  6);
	CDlgUtil::CobxAddItem(m_cmbNSS, _T(" 8"),  8);
	CDlgUtil::CobxAddItem(m_cmbNSS, _T("10"), 10);
	CDlgUtil::CobxAddItem(m_cmbNSS, _T("12"), 12);
	CDlgUtil::CobxAddItem(m_cmbNSS, _T("18"), 18);
	CDlgUtil::CobxAddItem(m_cmbNSS, _T("24"), 24);
	m_edtTheta.SetReadOnly(TRUE);
}

void CCMSdhyDlg::UpdateCtrls()
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

	GetDlgItem(IDC_ETC_SDHY_COMPANY_STC)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDHY_COMPANY_CMB)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDHY_PRONAME_STC)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDHY_PRONAME_CMB)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDHY_TYPENUM_STC)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDHY_TYPENUM_CMB)->EnableWindow(m_nInputMethod==1);

	double dTheta = 180. / CDlgUtil::CobxGetCurSelItemData(m_cmbNSS, m_cmbNSS.GetCurSel());
	m_edtTheta.SetEditUnit(dTheta);

	int nHysModel = CDlgUtil::CobxGetCurSelItemData(m_cmbHysModel, m_cmbHysModel.GetCurSel());

	GetDlgItem(IDC_ETC_SDHY_A2_STC)->ShowWindow(nHysModel==D_SDHY_HYS_NORTRI || nHysModel==D_SDHY_HYS_MODIRO);
	GetDlgItem(IDC_ETC_SDHY_A2_EDT)->ShowWindow(nHysModel==D_SDHY_HYS_NORTRI || nHysModel==D_SDHY_HYS_MODIRO);
	GetDlgItem(IDC_ETC_SDHY_P2_STC)->ShowWindow(nHysModel==D_SDHY_HYS_NORTRI || nHysModel==D_SDHY_HYS_MODIRO);
	GetDlgItem(IDC_ETC_SDHY_P2_EDT)->ShowWindow(nHysModel==D_SDHY_HYS_NORTRI || nHysModel==D_SDHY_HYS_MODIRO);
	GetDlgItem(IDC_ETC_SDHY_P2_UNT)->ShowWindow(nHysModel==D_SDHY_HYS_NORTRI || nHysModel==D_SDHY_HYS_MODIRO);

	GetDlgItem(IDC_ETC_SDHY_BETA_STC )->ShowWindow(nHysModel==D_SDHY_HYS_DEGRBL);
	GetDlgItem(IDC_ETC_SDHY_BETA_STC2)->ShowWindow(nHysModel==D_SDHY_HYS_DEGRBL);
	GetDlgItem(IDC_ETC_SDHY_BETA_EDT )->ShowWindow(nHysModel==D_SDHY_HYS_DEGRBL);

	GetDlgItem(IDC_ETC_SDHY_PHI_STC  )->ShowWindow(nHysModel==D_SDHY_HYS_MODIRO);
	GetDlgItem(IDC_ETC_SDHY_PHI_STC2 )->ShowWindow(nHysModel==D_SDHY_HYS_MODIRO);
	GetDlgItem(IDC_ETC_SDHY_PHI_EDT  )->ShowWindow(nHysModel==D_SDHY_HYS_MODIRO);
	GetDlgItem(IDC_ETC_SDHY_LMBD_STC )->ShowWindow(nHysModel==D_SDHY_HYS_MODIRO);
	GetDlgItem(IDC_ETC_SDHY_LMBD_STC2)->ShowWindow(nHysModel==D_SDHY_HYS_MODIRO);
	GetDlgItem(IDC_ETC_SDHY_LMBD_EDT )->ShowWindow(nHysModel==D_SDHY_HYS_MODIRO);

	GetDlgItem(IDC_ETC_SDHY_HPARAM_GRP )->ShowWindow(nHysModel==D_SDHY_HYS_MODIRO || nHysModel==D_SDHY_HYS_DEGRBL);  

	SetBmpImage();

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

void CCMSdhyDlg::OnCmdInputMethodRdo()
{
	UpdateData(TRUE);
	if(m_nInputMethod==1) UpdateCompany();
	UpdateCtrls();
}

void CCMSdhyDlg::OnSelChangeCompany    () 
{
	UpdateData(TRUE);
	UpdateProductName();
}
void CCMSdhyDlg::OnSelChangeProductName() 
{
	UpdateData(TRUE);
	UpdateTypeNumber();
}
void CCMSdhyDlg::OnSelChangeTypeNumber () 
{
	UpdateData(TRUE);
	int nIdx = m_cmbTypeNumber.GetCurSel();
	if(nIdx<0) { ASSERT(0); return; }
	CString strTypeNum;
	m_cmbTypeNumber.GetLBText(m_cmbTypeNumber.GetCurSel(), strTypeNum);
	m_pIGenSDCDB->GetSdhyFromDB(strTypeNum,m_data);
	m_data.strName = m_Name;
	m_data.strDesc = m_Desc;
	Data2Dlg();
	UpdateCtrls();
}
void CCMSdhyDlg::UpdateCompany()          
{
	CStringArray aDBCompany;
	m_pIGenSDCDB->GetCompanyList(IGENSDC_ISL_HYSTRS2,aDBCompany);
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
void CCMSdhyDlg::UpdateProductName()      
{
	int nIdxCompany= m_cmbCompany.GetCurSel();
	if(nIdxCompany<0) { ASSERT(0); return; } // 회사가 없다니

	CString strCompany;
	m_cmbCompany.GetLBText(nIdxCompany, strCompany);

	CStringArray aDBPrdName;
	m_pIGenSDCDB->GetPrdNameList(IGENSDC_ISL_HYSTRS2,strCompany,aDBPrdName);
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
void CCMSdhyDlg::UpdateTypeNumber()      
{
	int nIdxCompany= m_cmbCompany.GetCurSel();
	int nIdxPrdName= m_cmbProductName.GetCurSel();
	if(nIdxCompany<0 || nIdxPrdName<0) { ASSERT(0); return; } 

	CString strCompany, strPrdName;
	m_cmbCompany.GetLBText(nIdxCompany, strCompany);
	m_cmbProductName.GetLBText(nIdxPrdName, strPrdName);

	CStringArray aDBTypeNum;
	m_pIGenSDCDB->GetTypeNumListAndMakeData(IGENSDC_ISL_HYSTRS2,strCompany,strPrdName,aDBTypeNum);
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
