// CMSdisDisp.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSdisDlg.h"
#include "CMSdisVerDlg.h"
#include "CMSdisHorLRBDlg.h"
#include "CMSdisHorNRBDlg.h"
#include "CMSdisVerSLDDlg.h"
#include "CMSdisHorSLDDlg.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
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
// CCMSdisDlg dialog

CCMSdisDlg::CCMSdisDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSdisDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMSdisDlg)
	m_bModify = FALSE;
	m_key = 0;
	CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetInitializedSdisInCurUnit(m_data);

	m_Name = _T("");
	m_Desc = _T("");
	m_nDevType = 0;
	m_nInputMethod = 0;
	m_nHysProp = 0;
	m_pIGenSDCDB=CIGenSDCDBManager::GetInstance();
	m_bOnInit = TRUE;

	m_aCtrls_LRBNRBProp.Add(IDC_ETC_SDIS_AR_STC);
	m_aCtrls_LRBNRBProp.Add(IDC_ETC_SDIS_AR_EDT);
	m_aCtrls_LRBNRBProp.Add(IDC_ETC_SDIS_AR_UNT);
	m_aCtrls_LRBNRBProp.Add(IDC_ETC_SDIS_TR_STC);
	m_aCtrls_LRBNRBProp.Add(IDC_ETC_SDIS_TR_EDT);
	m_aCtrls_LRBNRBProp.Add(IDC_ETC_SDIS_TR_UNT);
	m_aCtrls_SLDProp.Add(IDC_ETC_SDIS_AS_STC);
	m_aCtrls_SLDProp.Add(IDC_ETC_SDIS_AS_EDT);
	m_aCtrls_SLDProp.Add(IDC_ETC_SDIS_AS_UNT);
	m_aCtrls_SLDProp.Add(IDC_ETC_SDIS_QD_STC);
	m_aCtrls_SLDProp.Add(IDC_ETC_SDIS_QD_CMB);
	m_aCtrls_SLDProp.Add(IDC_ETC_SDIS_PI_STC);
	m_aCtrls_SLDProp.Add(IDC_ETC_SDIS_PI_EDT);
	m_aCtrls_SLDProp.Add(IDC_ETC_SDIS_PI_UNT);

	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDIS_TAUK_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDIS_TAUQ_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDIS_AR_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDIS_TR_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDIS_AS_EDT);
	//}}AFX_DATA_INIT
}

CCMSdisDlg::~CCMSdisDlg()
{
}

void CCMSdisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSdisDlg)
	DDX_Control(pDX, IDC_ETC_SDIS_COMPANY_CMB, m_cmbCompany);
	DDX_Control(pDX, IDC_ETC_SDIS_PRONAME_CMB, m_cmbProductName);
	DDX_Control(pDX, IDC_ETC_SDIS_TYPENUM_CMB, m_cmbTypeNumber);
	DDX_Control(pDX, IDC_ETC_SDIS_NSS_CMB    , m_cmbNSS);
	DDX_Text   (pDX, IDC_ETC_SDIS_NAME_EDT   , m_Name);
	DDX_Text   (pDX, IDC_ETC_SDIS_DESC_EDT   , m_Desc);
	DDX_Radio  (pDX, IDC_ETC_SDIS_LRB_RDO    , m_nDevType);
	DDX_Radio  (pDX, IDC_ETC_SDIS_USER_RDO   , m_nInputMethod);
	DDX_Radio  (pDX, IDC_ETC_SDIS_MBM_RDO    , m_nHysProp);
	DDX_Control(pDX, IDC_ETC_SDIS_AR_EDT     , m_edtAr   );
	DDX_Control(pDX, IDC_ETC_SDIS_AR_UNT     , m_untAr   );
	DDX_Control(pDX, IDC_ETC_SDIS_TR_EDT     , m_edtTr   );
	DDX_Control(pDX, IDC_ETC_SDIS_TR_UNT     , m_untTr   );
	DDX_Control(pDX, IDC_ETC_SDIS_KE_EDT     , m_edtKe   );
	DDX_Control(pDX, IDC_ETC_SDIS_KE_UNT     , m_untKe   );
	DDX_Control(pDX, IDC_ETC_SDIS_TAUK_EDT    , m_edtTauk );
	DDX_Control(pDX, IDC_ETC_SDIS_TAUQ_EDT   , m_edtTauq );
	DDX_Control(pDX, IDC_ETC_SDIS_AS_EDT   , m_edtAs);
	DDX_Control(pDX, IDC_ETC_SDIS_AS_UNT   , m_untAs);
	DDX_Control(pDX, IDC_ETC_SDIS_PI_EDT   , m_edtPi);
	DDX_Control(pDX, IDC_ETC_SDIS_PI_UNT   , m_untPi);
	DDX_Control(pDX, IDC_ETC_SDIS_QD_CMB   , m_cmbQd);
	DDX_Control(pDX, IDC_ETC_SDIS_THT_EDT    , m_edtTheta );
	DDX_Control(pDX, IDC_ETC_SDIS_MSS_STC, m_ImgViewer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMSdisDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSdisDlg)
	ON_BN_CLICKED(IDOK                  , OnCmdOK)
	ON_BN_CLICKED(IDC_CMD_APPLY         , OnCmdApply)
	ON_BN_CLICKED(IDC_ETC_SDIS_USER_RDO       , OnCmdInputMethodRdo)
	ON_BN_CLICKED(IDC_ETC_SDIS_IMDB_RDO       , OnCmdInputMethodRdo)
	ON_CBN_SELCHANGE(IDC_ETC_SDIS_NSS_CMB, OnNSSCmb)
	ON_BN_CLICKED(IDC_ETC_SDIS_LRB_RDO , OnDevTypeRdo)
	ON_BN_CLICKED(IDC_ETC_SDIS_NRB_RDO , OnDevTypeRdo)
	ON_BN_CLICKED(IDC_ETC_SDIS_HDRI_RDO, OnDevTypeRdo)
	ON_BN_CLICKED(IDC_ETC_SDIS_SB_RDO  , OnDevTypeRdo)
	ON_BN_CLICKED(IDC_ETC_SDIS_MBM_RDO , OnHysModelRdo)
	ON_BN_CLICKED(IDC_ETC_SDIS_MHDM_RDO, OnHysModelRdo)
	ON_BN_CLICKED(IDC_ETC_SDIS_HDP_BTN , OnHDPBtn)
	ON_BN_CLICKED(IDC_ETC_SDIS_VER_BTN , OnVDPBtn)
	ON_CBN_SELCHANGE(IDC_ETC_SDIS_COMPANY_CMB, OnSelChangeCompany    )
	ON_CBN_SELCHANGE(IDC_ETC_SDIS_PRONAME_CMB, OnSelChangeProductName)
	ON_CBN_SELCHANGE(IDC_ETC_SDIS_TYPENUM_CMB, OnSelChangeTypeNumber )
	ON_CBN_SELCHANGE(IDC_ETC_SDIS_QD_CMB, UpdateCtrls)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCMSdisDlg::OnInitDialog()
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

BOOL CCMSdisDlg::OnExecute()
{
	UpdateData(TRUE);

	if (!Dlg2Data())
		return FALSE;

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	if (m_bModify)
	{
		if (!pDoc->m_pDataCtrl->ModifySdis(m_key, m_key, m_data))
			return FALSE;
	}
	else
	{
		if (!pDoc->m_pDataCtrl->AddSdis(m_data))
			return FALSE;
	}
	return TRUE;
}

void CCMSdisDlg::OnCmdOK()
{
	if(!OnExecute())
	{
		return;
	}

	CDialogMove::OnOK();
}

void CCMSdisDlg::OnCmdApply()
{
	OnExecute();
}

void CCMSdisDlg::Data2Dlg()
{
	m_Name = m_data.strName;
	m_Desc = m_data.strDesc;
	m_nDevType = m_data.GetDvtIdx();;
	m_nInputMethod = m_data.nInputMethod;
	m_nHysProp = m_data.nHysModel-1;
	m_edtAr  .SetEditUnit(m_data.dAr  );
	m_edtTr  .SetEditUnit(m_data.dTr  );
	m_edtKe  .SetEditUnit(m_data.dKe  );
	m_edtTauk.SetEditUnit(m_data.dTauk);
	m_edtTauq.SetEditUnit(m_data.dTauq);
	m_edtAs.SetEditUnit(m_data.dAs);
	m_edtPi.SetEditUnit(m_data.dPi);
	CDlgUtil::CobxSetCurSelItemData(m_cmbQd, m_data.nQd);

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

BOOL CCMSdisDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_data.strName = m_Name;
	m_data.strDesc = m_Desc;
	m_data.SetIdxDvt(m_nDevType);
	if(m_nInputMethod==1)
	{
		m_cmbCompany.GetLBText(m_cmbCompany.GetCurSel(), m_data.strCompany);
		m_cmbProductName.GetLBText(m_cmbProductName.GetCurSel(), m_data.strProductName);
		m_cmbTypeNumber.GetLBText(m_cmbTypeNumber.GetCurSel(), m_data.strTypeNumber);
	} 
	m_data.nInputMethod = m_nInputMethod;
	m_data.nHysModel = m_nHysProp+1;
	m_data.dAr   = m_edtAr   .GetEditValue();
	m_data.dTr   = m_edtTr   .GetEditValue();
	m_data.dKe   = m_edtKe   .GetEditValue();
	m_data.dTauk = m_edtTauk .GetEditValue();
	m_data.dTauq = m_edtTauq .GetEditValue();
	m_data.nNumMSS = CDlgUtil::CobxGetCurSelItemData(m_cmbNSS, m_cmbNSS.GetCurSel());
	m_data.dAs   = m_edtAs.GetEditValue();
	m_data.dPi   = m_edtPi.GetEditValue();
	m_data.nQd = CDlgUtil::CobxGetCurSelItemData(m_cmbQd, m_cmbQd.GetCurSel());

	return TRUE;
}

void CCMSdisDlg::OnNSSCmb()
{
	UpdateCtrls();
}

void CCMSdisDlg::OnDevTypeRdo()
{
	UpdateData(TRUE);
	if(m_nInputMethod==1) UpdateCompany();
	UpdateCtrls();
}

void CCMSdisDlg::OnHysModelRdo()
{
	UpdateCtrls();
}

void CCMSdisDlg::OnHDPBtn()
{
	UpdateData(TRUE);

	int nDBType = GetDeviceTypeFromDevTypeRdo(m_nDevType);
	if(nDBType==D_SDIS_DEV_LRB)
	{
		CCMSdisHorLRBDlg dlg;
		dlg.SetParamData(m_key, m_data);
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.SetDBData(m_nInputMethod==1);
		if(dlg.DoModal()==IDOK)
		{
			m_data.dK0 = dlg.m_data.dK0;
			m_data.dK2 = dlg.m_data.dK2;
			m_data.dQd = dlg.m_data.dQd;
		}
	}
	else if(nDBType==D_SDIS_DEV_NRB)
	{
		CCMSdisHorNRBDlg dlg;
		dlg.SetParamData(m_key, m_data);
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.SetDBData(m_nInputMethod==1);
		if(dlg.DoModal()==IDOK)
		{
			m_data.dKh     = dlg.m_data.dKh    ;
		}
	}
	else if(nDBType==D_SDIS_DEV_SLD)
	{
		CCMSdisHorSLDDlg dlg;
		dlg.SetParamData(m_key, m_data);
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.SetDBData(m_nInputMethod==1);
		if(dlg.DoModal()==IDOK)
		{
			m_data.dK0  = dlg.m_data.dK0  ;
			m_data.dMu0 = dlg.m_data.dMu0 ;
		}
	}
	else ASSERT(0);
}

void CCMSdisDlg::OnVDPBtn()
{
	UpdateData(TRUE);

	int nDBType = GetDeviceTypeFromDevTypeRdo(m_nDevType);
	if(nDBType==D_SDIS_DEV_LRB || nDBType==D_SDIS_DEV_NRB)
	{
		CCMSdisVerDlg dlg;
		dlg.SetParamData(m_key, m_data, m_nDevType);
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.SetDBData(m_nInputMethod==1);
		if(dlg.DoModal()==IDOK)
		{
			m_data.bConsNonl = dlg.m_data.bConsNonl;
			m_data.dKv       = dlg.m_data.dKv      ;
			m_data.dBeta     = dlg.m_data.dBeta    ;
			m_data.dAlpha    = dlg.m_data.dAlpha   ;
			m_data.dSigmav   = dlg.m_data.dSigmav  ;
		}
	}
	else if(nDBType==D_SDIS_DEV_SLD)
	{
		CCMSdisVerSLDDlg dlg;
		dlg.SetParamData(m_key, m_data, m_nDevType);
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.SetDBData(m_nInputMethod==1);
		if(dlg.DoModal()==IDOK)
		{
			m_data.dKv = dlg.m_data.dKv;
		}
	}
}

void CCMSdisDlg::SetBmpImage()
{
	UpdateData(TRUE);
	
	//UINT aBitmapID = IDB_ETC_SDIS_MSS;
	CString csImgPath = _T("SVG\\illustration\\Dialog\\Sdis_MSS.svg");

	m_ImgViewer.SetImage(csImgPath);
}

void CCMSdisDlg::InitCtrl()
{
	GetDlgItem(IDC_ETC_SDIS_HDRI_RDO)->ShowWindow(SW_HIDE); // 언젠가 추가될 예정
	m_edtTheta.SetReadOnly(TRUE);

	m_edtAr   .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dAr   );
	m_untAr   .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dAr   );
	m_edtTr   .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dTr   );
	m_untTr   .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dTr   );
	m_edtKe   .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dKe   );
	m_untKe   .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dKe   );
	m_edtTauk .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dTauk );
	m_edtTauq .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dTauq );
	m_edtAs   .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dAs   );
	m_untAs   .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dAs   );
	m_edtPi   .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dPi   );
	m_untPi   .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dPi   );

	CDlgUtil::CobxAddItem(m_cmbQd, _LS(IDS_CMD_SDIS_QD_1),  1);
	CDlgUtil::CobxAddItem(m_cmbQd, _LS(IDS_CMD_SDIS_QD_2),  2);

	// PMS 5253-22 : Sliding Bearings _T("Qd") 항목, _T("1")번만 활성화 하고, _T("2~6") 비활성합니다.
	if(CTestEnvMgr::IsTestEnvST(_T("SeisCtrlDeviceLater"), _T("yes")))
	{
		CDlgUtil::CobxAddItem(m_cmbQd, _LS(IDS_CMD_SDIS_QD_3),  3);
		CDlgUtil::CobxAddItem(m_cmbQd, _LS(IDS_CMD_SDIS_QD_4),  4);
		CDlgUtil::CobxAddItem(m_cmbQd, _LS(IDS_CMD_SDIS_QD_5),  5);
		CDlgUtil::CobxAddItem(m_cmbQd, _LS(IDS_CMD_SDIS_QD_6),  6);
	}
	
	CDlgUtil::CobxAddItem(m_cmbNSS, _T(" 2"),  2);
	CDlgUtil::CobxAddItem(m_cmbNSS, _T(" 4"),  4);
	CDlgUtil::CobxAddItem(m_cmbNSS, _T(" 6"),  6);
	CDlgUtil::CobxAddItem(m_cmbNSS, _T(" 8"),  8);
	CDlgUtil::CobxAddItem(m_cmbNSS, _T("10"), 10);
	CDlgUtil::CobxAddItem(m_cmbNSS, _T("12"), 12);
	CDlgUtil::CobxAddItem(m_cmbNSS, _T("18"), 18);
	CDlgUtil::CobxAddItem(m_cmbNSS, _T("24"), 24);

	CRect rRef;
	CRect rToMove;
	GetDlgItem(IDC_ETC_SDIS_AR_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_ETC_SDIS_AS_STC)->GetWindowRect(rToMove);
	int nDistX = rRef.left - rToMove.left;
	int nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aCtrls_SLDProp, nDistX, nDistY);

	SetBmpImage();
}

void CCMSdisDlg::UpdateCtrls()
{
	UpdateData(TRUE);

	BOOL bDBDevice=m_nInputMethod==1;
	if(!bDBDevice)
	{
		for(int i=0; i<m_aCtrlID_DBValue_ReadOnly.GetSize(); i++)
		{
			((CEdit*)GetDlgItem(m_aCtrlID_DBValue_ReadOnly[i]))->SetReadOnly(FALSE);
		}
	}

	GetDlgItem(IDC_ETC_SDIS_COMPANY_STC)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDIS_COMPANY_CMB)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDIS_PRONAME_STC)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDIS_PRONAME_CMB)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDIS_TYPENUM_STC)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDIS_TYPENUM_CMB)->EnableWindow(m_nInputMethod==1);

	double dTheta = 180. / CDlgUtil::CobxGetCurSelItemData(m_cmbNSS, m_cmbNSS.GetCurSel());
	m_edtTheta.SetEditUnit(dTheta);

	CString strIProp;
	switch(m_nDevType)
	{
	case 0: 
		GetDlgItem(IDC_ETC_SDIS_IPROP_GRP  )->SetWindowText(_LS(IDS_ETC_SDIS_LRB_IPROP));
		GetDlgItem(IDC_ETC_SDIS_APARAM_STC1)->SetWindowText(_LS(IDS_SDVP_ISOLATOR_APARAM_STC1_LRB));
		GetDlgItem(IDC_ETC_SDIS_APARAM_STC2)->SetWindowText(_LS(IDS_SDVP_ISOLATOR_APARAM_STC2_LRB));
		GetDlgItem(IDC_ETC_SDIS_APARAM_STC3)->SetWindowText(_LS(IDS_SDVP_ISOLATOR_APARAM_STC3_LRB));    
		break;
	case 1: 
		GetDlgItem(IDC_ETC_SDIS_IPROP_GRP  )->SetWindowText(_LS(IDS_ETC_SDIS_NRB_IPROP));
		GetDlgItem(IDC_ETC_SDIS_APARAM_STC1)->SetWindowText(_LS(IDS_SDVP_ISOLATOR_APARAM_STC1_NRB));
		GetDlgItem(IDC_ETC_SDIS_APARAM_STC2)->SetWindowText(_LS(IDS_SDVP_ISOLATOR_APARAM_STC2_NRB));
		GetDlgItem(IDC_ETC_SDIS_APARAM_STC3)->SetWindowText(_LS(IDS_SDVP_ISOLATOR_APARAM_STC3_NRB));  
		break;
	case 3:
		GetDlgItem(IDC_ETC_SDIS_IPROP_GRP  )->SetWindowText(_LS(IDS_ETC_SDIS_SLD_IPROP));
		GetDlgItem(IDC_ETC_SDIS_APARAM_STC1)->SetWindowText(_LS(IDS_SDVP_ISOLATOR_APARAM_STC1_LRB));
		GetDlgItem(IDC_ETC_SDIS_APARAM_STC2)->SetWindowText(_LS(IDS_SDVP_ISOLATOR_APARAM_STC2_LRB));
		GetDlgItem(IDC_ETC_SDIS_APARAM_STC3)->SetWindowText(_LS(IDS_SDVP_ISOLATOR_APARAM_STC3_LRB));  
		break;
	default: 
		ASSERT(0); 
		GetDlgItem(IDC_ETC_SDIS_IPROP_GRP  )->SetWindowText(_LS(IDS_ETC_SDIS_LRB_IPROP));
		GetDlgItem(IDC_ETC_SDIS_APARAM_STC1)->SetWindowText(_LS(IDS_SDVP_ISOLATOR_APARAM_STC1_LRB));
		GetDlgItem(IDC_ETC_SDIS_APARAM_STC2)->SetWindowText(_LS(IDS_SDVP_ISOLATOR_APARAM_STC2_LRB));
		GetDlgItem(IDC_ETC_SDIS_APARAM_STC3)->SetWindowText(_LS(IDS_SDVP_ISOLATOR_APARAM_STC3_LRB));  
		break;
	}
	
	GetDlgItem(IDC_ETC_SDIS_TAUQ_STC )->EnableWindow(m_nDevType!=1);
	GetDlgItem(IDC_ETC_SDIS_TAUQ_EDT )->EnableWindow(m_nDevType!=1);  
	GetDlgItem(IDC_ETC_SDIS_HPROP_GRP)->EnableWindow(m_nDevType==0);
	GetDlgItem(IDC_ETC_SDIS_MBM_RDO  )->EnableWindow(m_nDevType==0);

	CDlgUtil::CtrlShowHide(this, m_aCtrls_LRBNRBProp, m_nDevType==0||m_nDevType==1);
	CDlgUtil::CtrlShowHide(this, m_aCtrls_SLDProp, m_nDevType==3);

	GetDlgItem(IDC_ETC_SDIS_MHDM_RDO )->EnableWindow(m_nDevType==0);
	GetDlgItem(IDC_ETC_SDIS_KE_STC   )->EnableWindow(m_nDevType==0 && m_nHysProp==1);
	GetDlgItem(IDC_ETC_SDIS_KE_EDT   )->EnableWindow(m_nDevType==0 && m_nHysProp==1);
	GetDlgItem(IDC_ETC_SDIS_KE_UNT   )->EnableWindow(m_nDevType==0 && m_nHysProp==1);

	// 161226 : Qd가 1번이 아닐 때는 비활성화
	int nQd = CDlgUtil::CobxGetCurSelItemData(m_cmbQd, m_cmbQd.GetCurSel());
	GetDlgItem(IDC_ETC_SDIS_PI_STC)->EnableWindow(nQd<2);
	GetDlgItem(IDC_ETC_SDIS_PI_EDT)->EnableWindow(nQd<2);
	GetDlgItem(IDC_ETC_SDIS_PI_UNT)->EnableWindow(nQd<2);
	if(nQd>1)
	{
		m_edtPi.SetEditUnit(0.0);
	}

	if(bDBDevice)
	{
		for(int i=0; i<m_aCtrlID_DBValue_ReadOnly.GetSize(); i++)
		{
			((CEdit*)GetDlgItem(m_aCtrlID_DBValue_ReadOnly[i]))->SetReadOnly(TRUE);
		}
	}
}

void CCMSdisDlg::OnCmdInputMethodRdo()
{
	UpdateData(TRUE);
	if(m_nInputMethod==1) UpdateCompany();
	UpdateCtrls();
}

void CCMSdisDlg::OnSelChangeCompany    ()
{
	UpdateData(TRUE);
	UpdateProductName();
}
void CCMSdisDlg::OnSelChangeProductName()
{
	UpdateData(TRUE);
	UpdateTypeNumber();
}
void CCMSdisDlg::OnSelChangeTypeNumber ()
{
	UpdateData(TRUE);
	int nIdx = m_cmbTypeNumber.GetCurSel();
	if(nIdx<0) return;
	CString strTypeNum;
	m_cmbTypeNumber.GetLBText(m_cmbTypeNumber.GetCurSel(), strTypeNum);
	m_pIGenSDCDB->GetSdisFromDB(strTypeNum,m_data);
	m_data.strName = m_Name;
	m_data.strDesc = m_Desc;
	Data2Dlg();
	UpdateCtrls();
}

void CCMSdisDlg::UpdateCompany()
{
	int nDBType = GetDBTypeFromDevTypeRdo(m_nDevType);
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

void CCMSdisDlg::UpdateProductName()
{
	int nDBType = GetDBTypeFromDevTypeRdo(m_nDevType);
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

void CCMSdisDlg::UpdateTypeNumber()
{
	int nDBType = GetDBTypeFromDevTypeRdo(m_nDevType);
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

int CCMSdisDlg::GetDBTypeFromDevTypeRdo(int nDevType)
{
	switch(nDevType)
	{
	case 0: return IGENSDC_ISL_LEADRUB;
	case 1: return IGENSDC_ISL_NATURUB;
	//case 2: return D_SDIS_DEV_HDR;
	case 3: return IGENSDC_ISL_SLDBEAR;
	}
	ASSERT(0);
	return D_SDIS_DEV_LRB;
}

int CCMSdisDlg::GetDeviceTypeFromDevTypeRdo(int nDevType)
{
	switch(nDevType)
	{
	case 0: return D_SDIS_DEV_LRB;
	case 1: return D_SDIS_DEV_NRB;
	case 2: return D_SDIS_DEV_HDR;
	case 3: return D_SDIS_DEV_SLD;
	}
	ASSERT(0);
	return D_SDIS_DEV_LRB;
}