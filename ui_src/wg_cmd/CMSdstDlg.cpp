// CMSdstDisp.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSdstDlg.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\IGenSDCDBManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSdstDlg dialog

CCMSdstDlg::CCMSdstDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSdstDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMSdstDlg)
	m_bModify = FALSE;
	m_key = 0;
	m_data.Initialize();

	m_Name = _T("");
	m_Desc = _T("");
	m_nInputMethod = 0;
	m_pIGenSDCDB=CIGenSDCDBManager::GetInstance();
	m_bOnInit = TRUE;

	m_aCtrlID_DBValue_ReadOnly_Brace.Add(IDC_ETC_SDST_A1_EDT);
	m_aCtrlID_DBValue_ReadOnly_Brace.Add(IDC_ETC_SDST_SR_EDT);
	m_aCtrlID_DBValue_ReadOnly_Brace.Add(IDC_ETC_SDST_SF_EDT);
	m_aCtrlID_DBValue_ReadOnly_Brace.Add(IDC_ETC_SDST_BETA_EDT);
	m_aCtrlID_DBValue_Disabled_Brace.Add(IDC_ETC_SDST_HMD_CMB);

	m_aCtrlID_DBValue_ReadOnly_Colum.Add(IDC_ETC_SDST_P1_EDT);
	m_aCtrlID_DBValue_ReadOnly_Colum.Add(IDC_ETC_SDST_A1_EDT);
	m_aCtrlID_DBValue_ReadOnly_Colum.Add(IDC_ETC_SDST_A2_EDT);
	m_aCtrlID_DBValue_ReadOnly_Colum.Add(IDC_ETC_SDST_SF_EDT);
	m_aCtrlID_DBValue_ReadOnly_Colum.Add(IDC_ETC_SDST_SR_EDT);
	m_aCtrlID_DBValue_ReadOnly_Colum.Add(IDC_ETC_SDST_BETA_EDT);
	m_aCtrlID_DBValue_Disabled_Colum.Add(IDC_ETC_SDST_HMD_CMB);
	m_aCtrlID_DBValue_Disabled_Colum.Add(IDC_ETC_SDST_KB_EDT); // Kb는 Disable시킴
	//}}AFX_DATA_INIT
}

CCMSdstDlg::~CCMSdstDlg()
{
}

void CCMSdstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSdstDlg)
	DDX_Control(pDX, IDC_ETC_SDST_DEVTYPE_CMB, m_cmbDeviceType);
	DDX_Control(pDX, IDC_ETC_SDST_COMPANY_CMB, m_cmbCompany);
	DDX_Control(pDX, IDC_ETC_SDST_PRONAME_CMB, m_cmbProductName);
	DDX_Control(pDX, IDC_ETC_SDST_TYPENUM_CMB, m_cmbTypeNumber);
	DDX_Control(pDX, IDC_ETC_SDST_HMD_CMB    , m_cmbHysModel);
	DDX_Text   (pDX, IDC_ETC_SDST_NAME_EDT   , m_Name);
	DDX_Text   (pDX, IDC_ETC_SDST_DESC_EDT   , m_Desc);
	DDX_Radio  (pDX, IDC_ETC_SDST_USER_RDO   , m_nInputMethod);
	DDX_Radio  (pDX, IDC_ETC_SDST_DX_RDO     , m_nDOF);
	DDX_Control(pDX, IDC_ETC_SDST_K0_EDT     , m_edtK0   );
	DDX_Control(pDX, IDC_ETC_SDST_K0_UNT     , m_untK0   );
	DDX_Control(pDX, IDC_ETC_SDST_P1_EDT     , m_edtP1   );
	DDX_Control(pDX, IDC_ETC_SDST_P1_UNT     , m_untP1   );
	DDX_Control(pDX, IDC_ETC_SDST_KB_EDT     , m_edtKb   );
	DDX_Control(pDX, IDC_ETC_SDST_KB_UNT     , m_untKb   );
	DDX_Control(pDX, IDC_ETC_SDST_A1_EDT     , m_edtA1   );
	DDX_Control(pDX, IDC_ETC_SDST_A2_EDT     , m_edtA2   );
	DDX_Control(pDX, IDC_ETC_SDST_SF_EDT     , m_edtTheta);
	DDX_Control(pDX, IDC_ETC_SDST_SR_EDT     , m_edtGamma);
	DDX_Control(pDX, IDC_ETC_SDST_BETA_EDT   , m_edtBeta );
	DDX_Control(pDX, IDC_ETC_SDST_BMP, m_ImgViewer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMSdstDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSdstDlg)
	ON_BN_CLICKED(IDOK                  , OnCmdOK)
	ON_BN_CLICKED(IDC_CMD_APPLY         , OnCmdApply)
	ON_BN_CLICKED(IDC_ETC_SDST_USER_RDO    , OnCmdInputMethodRdo)
	ON_BN_CLICKED(IDC_ETC_SDST_IMDB_RDO    , OnCmdInputMethodRdo)
	ON_CBN_SELCHANGE(IDC_ETC_SDST_HMD_CMB  , OnHysModelCmb)
	ON_CBN_SELCHANGE(IDC_ETC_SDST_DEVTYPE_CMB, OnSelChangeDeviceType )
	ON_CBN_SELCHANGE(IDC_ETC_SDST_COMPANY_CMB, OnSelChangeCompany    )
	ON_CBN_SELCHANGE(IDC_ETC_SDST_PRONAME_CMB, OnSelChangeProductName)
	ON_CBN_SELCHANGE(IDC_ETC_SDST_TYPENUM_CMB, OnSelChangeTypeNumber )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCMSdstDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitCtrl();
	Data2Dlg();
	if(m_data.nInputMethod==1)
	{
		UpdateDeviceType();
	}
	UpdateCtrls();
	m_bOnInit=FALSE;

	return FALSE;
}

BOOL CCMSdstDlg::OnExecute()
{
	UpdateData(TRUE);

	if (!Dlg2Data())
		return FALSE;

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	if (m_bModify)
	{
		if (!pDoc->m_pDataCtrl->ModifySdst(m_key, m_key, m_data))
			return FALSE;
	}
	else
	{
		if (!pDoc->m_pDataCtrl->AddSdst(m_data))
			return FALSE;
	}
	return TRUE;
}

void CCMSdstDlg::OnCmdOK()
{
	if(!OnExecute())
	{
		return;
	}

	CDialogMove::OnOK();
}

void CCMSdstDlg::OnCmdApply()
{
	OnExecute();
}

void CCMSdstDlg::Data2Dlg()
{
	m_Name = m_data.strName;
	m_Desc = m_data.strDesc;
	m_nInputMethod = m_data.nInputMethod;  
	m_cmbHysModel.SetCurSel(m_data.GetHysIdx());
	m_edtK0   .SetEditUnit(m_data.dK0);
	m_edtP1   .SetEditUnit(m_data.dP1);
	m_edtA1   .SetEditUnit(m_data.dAlpha1);
	m_edtA2   .SetEditUnit(m_data.dAlpha2);
	m_edtBeta .SetEditUnit(m_data.dBeta );
	m_edtTheta.SetEditUnit(m_data.dTheta);
	m_edtGamma.SetEditUnit(m_data.dGamma);
	m_edtKb   .SetEditUnit(m_data.dKb   );
	m_nDOF = m_data.nDir-1;
 
	UpdateData(FALSE);
}

BOOL CCMSdstDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_data.Initialize();
	m_data.strName = m_Name;
	m_data.strDesc = m_Desc;
	m_data.nInputMethod = m_nInputMethod;
	if(m_nInputMethod==1)
	{
		m_cmbDeviceType.GetLBText(m_cmbDeviceType.GetCurSel(), m_data.strDeviceType);
		m_cmbCompany.GetLBText(m_cmbCompany.GetCurSel(), m_data.strCompany);
		m_cmbProductName.GetLBText(m_cmbProductName.GetCurSel(), m_data.strProductName);
		m_cmbTypeNumber.GetLBText(m_cmbTypeNumber.GetCurSel(), m_data.strTypeNumber);
	} 
	m_data.nHysModel = CDlgUtil::CobxGetCurSelItemData(m_cmbHysModel, m_cmbHysModel.GetCurSel());
	m_data.dK0     = m_edtK0   .GetEditValue();
	m_data.dP1     = m_edtP1   .GetEditValue();
	m_data.dAlpha1 = m_edtA1   .GetEditValue();
	m_data.dAlpha2 = m_edtA2   .GetEditValue();
	m_data.dBeta   = m_edtBeta .GetEditValue();
	m_data.dTheta  = m_edtTheta.GetEditValue();
	m_data.dGamma  = m_edtGamma.GetEditValue();
	m_data.dKb     = m_edtKb   .GetEditValue();
	m_data.nDir    = m_nDOF+1;
	
	return TRUE;
}

void CCMSdstDlg::OnHysModelCmb()
{
	UpdateCtrls();
}

void CCMSdstDlg::SetBmpImage()
{
	UpdateData(TRUE);
	
	//UINT aBitmapID[] = { IDB_ETC_SDST_BL2, IDB_ETC_SDST_LY2, IDB_ETC_SDST_LY3, IDB_ETC_SDST_IK2 };
	CString aImgPath[] = {
		_T("SVG\\illustration\\Dialog\\Sdst_BL2.svg"),
		_T("SVG\\illustration\\Dialog\\Sdst_LY2.svg"),
		_T("SVG\\illustration\\Dialog\\Sdst_LY3.svg"),
		_T("SVG\\illustration\\Dialog\\Sdst_IK2.svg")
	};
	
	m_ImgViewer.SetImage(aImgPath[m_cmbHysModel.GetCurSel()]);
}

void CCMSdstDlg::InitCtrl()
{
	CRect rRef;
	CRect rToMove;

	GetDlgItem(IDC_ETC_SDST_SF_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_ETC_SDST_BETA_STC)->GetWindowRect(rToMove);
	int nDistX = rRef.left - rToMove.left;
	int nDistY = rRef.top - rToMove.top;
	
	CArray<UINT,UINT> aCtrl;
	aCtrl.Add(IDC_ETC_SDST_BETA_STC);
	aCtrl.Add(IDC_ETC_SDST_BETA_STC2);
	aCtrl.Add(IDC_ETC_SDST_BETA_EDT);
	CDlgUtil::CtrlMoveDistXY(this, aCtrl, nDistX, nDistY);

	m_edtK0.SetUnitType(CUnitCtrl::m_SDST_UNIT.dK0);
	m_untK0.SetUnitType(CUnitCtrl::m_SDST_UNIT.dK0);
	m_edtP1.SetUnitType(CUnitCtrl::m_SDST_UNIT.dP1);
	m_untP1.SetUnitType(CUnitCtrl::m_SDST_UNIT.dP1);
	m_edtKb.SetUnitType(CUnitCtrl::m_SDST_UNIT.dKb);
	m_untKb.SetUnitType(CUnitCtrl::m_SDST_UNIT.dKb);

	CDlgUtil::CobxAddItem(m_cmbHysModel, _LS(IDS_ETC_SDST_HYS_BL2), D_SDST_HYS_BL2);
	CDlgUtil::CobxAddItem(m_cmbHysModel, _LS(IDS_ETC_SDST_HYS_LY2), D_SDST_HYS_LY2);
	CDlgUtil::CobxAddItem(m_cmbHysModel, _LS(IDS_ETC_SDST_HYS_LY3), D_SDST_HYS_LY3);
	CDlgUtil::CobxAddItem(m_cmbHysModel, _LS(IDS_ETC_SDST_HYS_IK2), D_SDST_HYS_IK2);
}

void CCMSdstDlg::UpdateCtrls()
{
	UpdateData(TRUE);

	BOOL bDBDevice=m_nInputMethod==1;
	if(!bDBDevice)
	{
		for(int i=0; i<m_aCtrlID_DBValue_ReadOnly_Brace.GetSize(); i++)
		{
			((CEdit*)GetDlgItem(m_aCtrlID_DBValue_ReadOnly_Brace[i]))->SetReadOnly(FALSE);
		}
		for(int i=0; i<m_aCtrlID_DBValue_Disabled_Brace.GetSize(); i++)
		{
			GetDlgItem(m_aCtrlID_DBValue_Disabled_Brace[i])->EnableWindow(TRUE);
		}
		for(int i=0; i<m_aCtrlID_DBValue_ReadOnly_Colum.GetSize(); i++)
		{
			((CEdit*)GetDlgItem(m_aCtrlID_DBValue_ReadOnly_Colum[i]))->SetReadOnly(FALSE);
		}
		for(int i=0; i<m_aCtrlID_DBValue_Disabled_Colum.GetSize(); i++)
		{
			GetDlgItem(m_aCtrlID_DBValue_Disabled_Colum[i])->EnableWindow(TRUE);
		}
	}

	GetDlgItem(IDC_ETC_SDST_DEVTYPE_STC)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDST_DEVTYPE_CMB)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDST_COMPANY_STC)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDST_COMPANY_CMB)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDST_PRONAME_STC)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDST_PRONAME_CMB)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDST_TYPENUM_STC)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDST_TYPENUM_CMB)->EnableWindow(m_nInputMethod==1);

	int nHysModel = CDlgUtil::CobxGetCurSelItemData(m_cmbHysModel, m_cmbHysModel.GetCurSel());

	GetDlgItem(IDC_ETC_SDST_A2_STC)->ShowWindow(nHysModel==D_SDST_HYS_LY2 || nHysModel==D_SDST_HYS_LY3);
	GetDlgItem(IDC_ETC_SDST_A2_EDT)->ShowWindow(nHysModel==D_SDST_HYS_LY2 || nHysModel==D_SDST_HYS_LY3);

	GetDlgItem(IDC_ETC_SDST_BETA_STC )->ShowWindow(nHysModel==D_SDST_HYS_BL2);
	GetDlgItem(IDC_ETC_SDST_BETA_STC2)->ShowWindow(nHysModel==D_SDST_HYS_BL2);
	GetDlgItem(IDC_ETC_SDST_BETA_EDT )->ShowWindow(nHysModel==D_SDST_HYS_BL2);

	GetDlgItem(IDC_ETC_SDST_SF_STC )->ShowWindow(nHysModel==D_SDST_HYS_LY2 || nHysModel==D_SDST_HYS_LY3);
	GetDlgItem(IDC_ETC_SDST_SF_STC2)->ShowWindow(nHysModel==D_SDST_HYS_LY2 || nHysModel==D_SDST_HYS_LY3);
	GetDlgItem(IDC_ETC_SDST_SF_EDT )->ShowWindow(nHysModel==D_SDST_HYS_LY2 || nHysModel==D_SDST_HYS_LY3);

	GetDlgItem(IDC_ETC_SDST_SR_STC )->ShowWindow(nHysModel==D_SDST_HYS_LY3 || nHysModel==D_SDST_HYS_IK2);
	GetDlgItem(IDC_ETC_SDST_SR_EDT )->ShowWindow(nHysModel==D_SDST_HYS_LY3 || nHysModel==D_SDST_HYS_IK2);

	// Gamma : Stiffness Ratio ( 0 < γ < 1 )
	CRect rRef;
	CRect rToMove;
	GetDlgItem(IDC_ETC_SDST_SF_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_ETC_SDST_SR_STC)->GetWindowRect(rToMove);
	int nDistX = rRef.left - rToMove.left;
	int nDistY = rRef.top - rToMove.top;
	if(nHysModel==D_SDST_HYS_LY3) nDistY+=globalUtils.ScaleByDPI(40);
	CArray<UINT,UINT> aCtrl_SR;
	aCtrl_SR.Add(IDC_ETC_SDST_SR_STC);
	aCtrl_SR.Add(IDC_ETC_SDST_SR_EDT);
	CDlgUtil::CtrlMoveDistXY(this, aCtrl_SR, nDistX, nDistY);
			 if(nHysModel==D_SDST_HYS_LY3) GetDlgItem(IDC_ETC_SDST_SR_STC)->SetWindowText(_LS(IDS_ETC_SDST_SR_LY3_TXT));
	else if(nHysModel==D_SDST_HYS_IK2) GetDlgItem(IDC_ETC_SDST_SR_STC)->SetWindowText(_LS(IDS_ETC_SDST_SR_IK2_TXT));

	SetBmpImage();

	if(bDBDevice)
	{
		int nDBType = m_cmbDeviceType.GetCurSel()==0 ? IGENSDC_DMP_STLBRAC:IGENSDC_DMP_STLSTUD;
		if(nDBType==IGENSDC_DMP_STLBRAC)
		{
			((CEdit*)GetDlgItem(IDC_ETC_SDST_P1_EDT))->SetReadOnly(FALSE);
			for(int i=0; i<m_aCtrlID_DBValue_ReadOnly_Brace.GetSize(); i++)
			{
				((CEdit*)GetDlgItem(m_aCtrlID_DBValue_ReadOnly_Brace[i]))->SetReadOnly(TRUE);
			}
			for(int i=0; i<m_aCtrlID_DBValue_Disabled_Brace.GetSize(); i++)
			{
				GetDlgItem(m_aCtrlID_DBValue_Disabled_Brace[i])->EnableWindow(FALSE);
			}
		}
		else if(nDBType==IGENSDC_DMP_STLSTUD)
		{
			for(int i=0; i<m_aCtrlID_DBValue_ReadOnly_Colum.GetSize(); i++)
			{
				((CEdit*)GetDlgItem(m_aCtrlID_DBValue_ReadOnly_Colum[i]))->SetReadOnly(TRUE);
			}
			for(int i=0; i<m_aCtrlID_DBValue_Disabled_Colum.GetSize(); i++)
			{
				GetDlgItem(m_aCtrlID_DBValue_Disabled_Colum[i])->EnableWindow(FALSE);
			}
		}
		else
		{
			ASSERT(0);
		}
	}
}

void CCMSdstDlg::OnCmdInputMethodRdo()
{
	UpdateData(TRUE);
	if(m_nInputMethod==1) UpdateDeviceType();
	UpdateCtrls();
}

void CCMSdstDlg::OnSelChangeDeviceType()
{  
	UpdateData(TRUE);
	UpdateCompany();
}
void CCMSdstDlg::OnSelChangeCompany    ()
{
	UpdateData(TRUE);
	UpdateProductName();
}
void CCMSdstDlg::OnSelChangeProductName()
{
	UpdateData(TRUE);
	UpdateTypeNumber();
}
void CCMSdstDlg::OnSelChangeTypeNumber ()
{
	UpdateData(TRUE);
	int nIdx = m_cmbTypeNumber.GetCurSel();
	if(nIdx<0) return;
	CString strTypeNum;
	m_cmbTypeNumber.GetLBText(m_cmbTypeNumber.GetCurSel(), strTypeNum);
	m_pIGenSDCDB->GetSdstFromDB(strTypeNum,m_data);
	m_data.strName = m_Name;
	m_data.strDesc = m_Desc;
	Data2Dlg();
	UpdateCtrls();
}


void CCMSdstDlg::UpdateDeviceType()
{
	m_cmbDeviceType.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbDeviceType, _LS(IDS_DB_IGEN_SDC_BRACE), 0);
	CDlgUtil::CobxAddItem(m_cmbDeviceType, _LS(IDS_DB_IGEN_SDC_STUD), 1);
	int nIdx=0;
	if(m_data.strDeviceType==_LS(IDS_DB_IGEN_SDC_BRACE)) nIdx=0;
	else if(m_data.strDeviceType==_LS(IDS_DB_IGEN_SDC_STUD)) nIdx=1;
	m_cmbDeviceType.SetCurSel(nIdx);

	UpdateCompany();
}
void CCMSdstDlg::UpdateCompany()
{
	int nDBType = m_cmbDeviceType.GetCurSel()==0 ? IGENSDC_DMP_STLBRAC:IGENSDC_DMP_STLSTUD;
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

void CCMSdstDlg::UpdateProductName()
{
	int nDBType = m_cmbDeviceType.GetCurSel()==0 ? IGENSDC_DMP_STLBRAC:IGENSDC_DMP_STLSTUD;
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

void CCMSdstDlg::UpdateTypeNumber()
{
	int nDBType = m_cmbDeviceType.GetCurSel()==0 ? IGENSDC_DMP_STLBRAC:IGENSDC_DMP_STLSTUD;
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