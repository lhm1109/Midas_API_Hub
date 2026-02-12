// IehpPropStlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "IehpPropStlDlg.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIehpPropStlDlg dialog

CIehpPropStlDlg::CIehpPropStlDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CIehpPropStlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIehpPropStlDlg)
	m_str1Plus = _T("");
	m_str1Mnus = _T("");
	m_nUserAuto = 1;
	m_nBuckling = 0;
	m_nStiffType = 1;
	m_bAssignProperties = FALSE;
	//}}AFX_DATA_INIT

	m_nSectType = -1;
	m_bDisableAutoCalc = FALSE;    // Auto 계산을 할 수 없는 경우

	m_aAssignHinge_Disabled.Add(IDC_IEHP_METHOD_USER_RDO);
	m_aAssignHinge_Disabled.Add(IDC_IEHP_METHOD_AUTO_RDO);
	m_aAssignHinge_Disabled.Add(IDC_IEHP_BUCKLING_NONE_RDO);
	m_aAssignHinge_Disabled.Add(IDC_IEHP_BUCKLING_RDO);
	m_aAssignHinge_Disabled.Add(IDC_IEHP_INIT_STIFF_USER_RDO);
	m_aAssignHinge_Disabled.Add(IDC_IEHP_INIT_STIFF_ELASTIC_RDO);
	m_aAssignHinge_Disabled.Add(IDC_DEFORMATION_INDEX);

	m_aAssignHinge_ReadOnly.Add(IDC_IEHP_YIELD_FY_EDT);
	m_aAssignHinge_ReadOnly.Add(IDC_IEHP_YIELD_FCR_EDT);
	m_aAssignHinge_ReadOnly.Add(IDC_IEHP_HYSTERESIS_LAMBDA_EDT);
	m_aAssignHinge_ReadOnly.Add(IDC_IEHP_HYSTERESIS_ALPHA_EDT);
	m_aAssignHinge_ReadOnly.Add(IDC_IEHP_HYSTERESIS_BETA_EDT);
	m_aAssignHinge_ReadOnly.Add(IDC_IEHP_INIT_STIFF_USER_EDT);
}

CIehpPropStlDlg::~CIehpPropStlDlg()
{

}

void CIehpPropStlDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K IehgKey, T_IEHP_BUCKL data,T_IEHP_BUCKL subdata, int nFg1, int nFg2, int nSectType/*=1*/, BOOL bAssignProperties/*FALSE*/)
{
	m_Data = iehpD;
	m_nDof = nFg2; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	m_Data.AllProp.PROP[nD].BUCKL = data;
	m_Data.AllSubProp.PROP[nD].BUCKL = subdata;

	m_nModelType = nFg1;
	m_nIntrType = iehpD.nInteractionType;
	m_nHingeType = iehpD.nHingeType;
	m_nDefinition = iehpD.nDefinition;
	m_nSectType = nSectType;
	m_bAssignProperties = bAssignProperties;
	m_ElemK = IehgKey.key.objK;
}

void CIehpPropStlDlg::SetControlStateData(int nOptType)
{
	m_nUserAuto = nOptType;
}

void CIehpPropStlDlg::ChangeBitmap()
{
	m_wndPicture.SetIgnoreLargeView(TRUE);

	if(m_nBuckling==0)
	{
		m_wndPicture.SetImage(_T("SVG\\illustration\\Dialog\\Steel_Buckling_None.svg"));

	}
	else
	{
		m_wndPicture.SetImage(_T("SVG\\illustration\\Dialog\\Steel_Buckling_Deteriorated.svg"));
	}
}

bool CIehpPropStlDlg::SyncDialogData()
{
	if (!Dlg2Data()) return false;

	return true;
}

void CIehpPropStlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIehpPropStlDlg)
	DDX_Radio(pDX, IDC_IEHP_METHOD_USER_RDO     , m_nUserAuto);
	DDX_Radio(pDX, IDC_IEHP_BUCKLING_NONE_RDO   , m_nBuckling);
	DDX_Radio(pDX, IDC_IEHP_INIT_STIFF_USER_RDO , m_nStiffType);
	DDX_Control(pDX, IDC_IEHP_YIELD_FY_EDT      , m_edtCrkPlus);
	DDX_Control(pDX, IDC_IEHP_YIELD_FCR_EDT     , m_edtCrkMnus);
	DDX_Control(pDX, IDC_IEHP_YIELD_FY_UNT      , m_untCrkPlus);
	DDX_Control(pDX, IDC_IEHP_YIELD_FCR_UNT     , m_untCrkMnus);
	DDX_Control(pDX, IDC_IEHP_INIT_STIFF_USER_EDT , m_edtStiffUser);
	DDX_Control(pDX, IDC_IEHP_INIT_STIFF_USER_UNIT, m_untStiffUser);

	DDX_Control(pDX, IDC_IEHP_HYSTERESIS_LAMBDA_EDT , m_edtSlednerRatio);
	DDX_Control(pDX, IDC_IEHP_HYSTERESIS_LAMBDA_EFF_EDT , m_edtSlednerEffRatio);
	DDX_Control(pDX, IDC_IEHP_HYSTERESIS_LAMBDA_EFF_K_EDT , m_edtSlednerEffKRatio);
	DDX_Control(pDX, IDC_IEHP_HYSTERESIS_LAMBDA_EFF_K_CHK, m_chkSlednerEffKRatio);
	DDX_Text(pDX, IDC_IEHP_HYSTERESIS_ALPHA_EDT   , m_str1Plus);
	DDX_Text(pDX, IDC_IEHP_HYSTERESIS_BETA_EDT    , m_str1Mnus);
	DDX_Control(pDX, IDC_CMD_IEHP_STL_BUCK_BMP, m_wndPicture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIehpPropStlDlg, CDialogMove)
	//{{AFX_MSG_MAP(CIehpPropStlDlg)
	ON_BN_CLICKED(IDC_IEHP_METHOD_USER_RDO            , OnUserAutoRdo)
	ON_BN_CLICKED(IDC_IEHP_METHOD_AUTO_RDO            , OnUserAutoRdo)
	ON_BN_CLICKED(IDC_IEHP_BUCKLING_NONE_RDO          , OnBuckStrengthRdo)
	ON_BN_CLICKED(IDC_IEHP_BUCKLING_RDO               , OnBuckStrengthRdo)
	ON_BN_CLICKED(IDC_IEHP_HYSTERESIS_LAMBDA_EFF_K_CHK, OnCmdLambdaEffKChk)
	ON_BN_CLICKED(IDC_IEHP_INIT_STIFF_USER_RDO        , OnStiffTypeRdo)
	ON_BN_CLICKED(IDC_IEHP_INIT_STIFF_ELASTIC_RDO     , OnStiffTypeRdo)
	ON_EN_CHANGE(IDC_IEHP_HYSTERESIS_LAMBDA_EDT       , OnChangeLambda)
	ON_EN_CHANGE(IDC_IEHP_HYSTERESIS_LAMBDA_EFF_K_EDT , OnChangeLambdaEffKEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIehpPropStlDlg message handlers

BOOL CIehpPropStlDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CDialogMove::OnInitDialog();

	////////////////////////////////////////////////////
	// Auto 계산할 수 없는 경우에 비활성화.
	m_bDisableAutoCalc = FALSE;
	if (m_Data.nHingeType == 2) m_bDisableAutoCalc = TRUE;  // nHingeType       : 0=Lumped, 1=Distributed, 2=Spring, 3=Truss, 4=Wall(CRB)
	if (m_Data.nInteractionType != 0)                       // nInteractionType : 0=None, 1=P-M, 2=P-M-M
	{
		int nDof = m_nDof;
		if (nDof > 5) nDof = m_nDof-3;
		if (nDof == 0 || nDof == 4 || nDof == 5) m_bDisableAutoCalc = TRUE;
	}
	if (m_Data.nHingeType == D_IEHP_HTYP_POINTSPRING)
	{
		m_bDisableAutoCalc = TRUE;
	}
	////////////////////////////////////////////////////

	if(m_bAssignProperties == TRUE && !m_bDisableAutoCalc) // Auto
	{
		if(m_ElemK < 1)	
		{
			ASSERT(0);
			return FALSE;		//Assign 시에는 해당 키값이 1이상이 무조건 들어와야함
		}
		m_pDoc->m_pAttrCtrl->CalcIehpPropValue(m_Data, m_ElemK, m_Data.AllProp.PROP, TRUE);
	}

	SetUnitAndTitle();
	Data2Dlg();
	CtrlManager();
	ChangeBitmap();

	return TRUE;  // return TRUE unless you set the focus to a control
								// EXCEPTION: OCX Property Pages should return FALSE
}

void CIehpPropStlDlg::OnOK() 
{
	if(!Dlg2Data()) return;

	CDialogMove::OnOK();
}

void CIehpPropStlDlg::OnUserAutoRdo() 
{
	UpdateData(TRUE);
	m_chkSlednerEffKRatio.SetCheck(m_nUserAuto==0);
	CtrlManager();
}

void CIehpPropStlDlg::OnBuckStrengthRdo() 
{
	UpdateData(TRUE);
	CtrlManager();
	ChangeBitmap();
}

void CIehpPropStlDlg::OnCmdLambdaEffKChk()
{
	UpdateData(TRUE);
	
	BOOL bCheck = m_chkSlednerEffKRatio.GetCheck();
	m_edtSlednerEffKRatio.SetReadOnly(bCheck==FALSE);

	UpdateData(FALSE);
}

void CIehpPropStlDlg::OnStiffTypeRdo() 
{
	UpdateData(TRUE);
	CtrlManager();
}

void CIehpPropStlDlg::OnChangeLambda()
{
	UpdateLambdaEff();
}

void CIehpPropStlDlg::OnChangeLambdaEffKEdit()
{
	UpdateLambdaEff();
}

void CIehpPropStlDlg::UpdateLambdaEff()
{
	double dSledner = m_edtSlednerRatio.GetEditValue();
	double dSlednerEffK = m_edtSlednerEffKRatio.GetEditValue();
	m_edtSlednerEffRatio.SetEditUnit(dSledner*dSlednerEffK);
}

void CIehpPropStlDlg::CtrlManager()
{
	UpdateData(TRUE);

	if(!m_bAssignProperties)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aAssignHinge_Disabled, TRUE);
	}

	// Lumped && User(Item Dlg) : User만 Enable
	// Lumped && Auto(Item Dlg) : My, Mz 인 경우만 전체 Enable
	// Lumped가 아닌 경우 : 전체 Disable
	int nDof = m_nDof;
	if(nDof > 5) nDof = nDof - 3;  // Interaction Type이 None이 아닌 경우
	int nCtrlType;   // Initial Stiffness의 Ctrl 설정 타입(A(1) ~ G(7)) : 7가지 존재
	int nModifiedStiffType, nModifiedUserAuto, nModifiedAlphaDelta;  

	int nPAlphaDelta=0;
	if(!m_pDoc->m_pEditData->CheckIehpForcedSet(nDof, m_nStiffType+3, nPAlphaDelta, 
		m_nUserAuto, m_nHingeType, m_nIntrType, 1, 
		nModifiedUserAuto, nModifiedAlphaDelta, nCtrlType, nModifiedStiffType))
	{    
		m_nUserAuto   = nModifiedUserAuto;
		nPAlphaDelta = nModifiedAlphaDelta;
		m_nStiffType  = nModifiedStiffType-3;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	GetDlgItem(IDC_IEHP_METHOD_USER_RDO)->EnableWindow(!m_bDisableAutoCalc);  
	GetDlgItem(IDC_IEHP_METHOD_AUTO_RDO)->EnableWindow(!m_bDisableAutoCalc);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Yield Strength
	m_edtCrkPlus.SetReadOnly(m_nUserAuto==1);
	m_edtCrkMnus.SetReadOnly(m_nUserAuto==1);
	GetDlgItem(IDC_IEHP_YIELD_FCR_STC)->EnableWindow(m_nBuckling == 1);
	GetDlgItem(IDC_IEHP_YIELD_FCR_EDT)->EnableWindow(m_nBuckling == 1);
	GetDlgItem(IDC_IEHP_YIELD_FCR_STC2)->EnableWindow(m_nBuckling == 1);
	GetDlgItem(IDC_IEHP_YIELD_FCR_UNT)->EnableWindow(m_nBuckling == 1);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Hysteresis Type Parameter
	m_edtSlednerRatio.SetReadOnly(m_nUserAuto==1);
	m_edtSlednerEffRatio.SetReadOnly(TRUE);
	BOOL bCheck = m_chkSlednerEffKRatio.GetCheck();
	m_edtSlednerEffKRatio.SetReadOnly(bCheck==FALSE);
	GetDlgItem(IDC_IEHP_HYSTERESIS_LAMBDA_EFF_STC)->EnableWindow(m_nBuckling == 1);
	GetDlgItem(IDC_IEHP_HYSTERESIS_LAMBDA_EFF_EDT)->EnableWindow(m_nBuckling == 1);
	GetDlgItem(IDC_IEHP_HYSTERESIS_LAMBDA_EFF_K_STC)->EnableWindow(m_nBuckling == 1);
	GetDlgItem(IDC_IEHP_HYSTERESIS_LAMBDA_EFF_K_CHK)->EnableWindow(m_nUserAuto==1 && m_nBuckling == 1);
	GetDlgItem(IDC_IEHP_HYSTERESIS_LAMBDA_EFF_K_EDT)->EnableWindow(m_nBuckling == 1);
	GetDlgItem(IDC_IEHP_HYSTERESIS_BETA_STC)->EnableWindow(m_nBuckling == 0);
	GetDlgItem(IDC_IEHP_HYSTERESIS_BETA_EDT)->EnableWindow(m_nBuckling == 0);
	GetDlgItem(IDC_IEHP_HYSTERESIS_BETA_STC2)->EnableWindow(m_nBuckling == 0);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Initial Stiffness
	GetDlgItem(IDC_IEHP_INIT_STIFF_USER_EDT)->EnableWindow(m_nStiffType == 0);
	GetDlgItem(IDC_IEHP_INIT_STIFF_USER_UNIT)->EnableWindow(m_nStiffType == 0);

	if(m_bAssignProperties)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aAssignHinge_Disabled, FALSE);
		for(int i=0; i<m_aAssignHinge_ReadOnly.GetSize(); i++)
		{
			((CEditUnit*)GetDlgItem(m_aAssignHinge_ReadOnly[i]))->SetReadOnly(TRUE);
		}
	}

	UpdateData(FALSE);
}

void CIehpPropStlDlg::SetUnitAndTitle()
{
	if(m_nDof<3) // None & Dx, Dy, Dz
	{
		m_untCrkPlus.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldForce);
		m_untCrkMnus.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldForce);

		if(m_nHingeType == 0 || m_nHingeType == 4)  // Lumped
		{
			if(m_nDof == 0)
				m_untStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
			else
				m_untStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffForce);
		}
		else if(m_nHingeType == 1)  // Distributed
			m_untStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffForce);
		else if(m_nHingeType == 2)  // Spring
			m_untStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
		else if(m_nHingeType == 3)  // Truss
			m_untStiffUser.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
	}
	else if(m_nDof<6) // None & Rx, Ry, Rz
	{
		ASSERT(0);
	}
	else if(m_nDof<9) // P-My-Mz & Rx, Ry, Rz
	{
		ASSERT(0);
	}
	else
	{
		ASSERT(0);
	}
}

void CIehpPropStlDlg::Data2Dlg()
{
	if(m_bAssignProperties)
		CDlgUtil::CtrlEnableDisable(this, m_aAssignHinge_Disabled, TRUE);

	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	switch(m_nModelType)
	{
	case D_IEHP_BUCK:
		{
			if(m_nDof<3)
			{
				m_nUserAuto = m_Data.AllProp.PROP[nD].BUCKL.nYieldStrengthOpt;
				m_nBuckling = m_Data.AllProp.PROP[nD].BUCKL.nBucklingOpt;

				m_edtCrkPlus.SetEditUnit(m_Data.AllProp.PROP[nD].BUCKL.dYieldForce[0]);
				m_edtCrkMnus.SetEditUnit(m_Data.AllProp.PROP[nD].BUCKL.dYieldForce[1]);

				m_edtSlednerRatio.SetEditUnit(m_Data.AllProp.PROP[nD].BUCKL.dSlednerRatio);
				m_chkSlednerEffKRatio.SetCheck(m_Data.AllProp.PROP[nD].BUCKL.bUserKFactor);
				m_edtSlednerEffKRatio.SetEditUnit(m_Data.AllProp.PROP[nD].BUCKL.dKFactor);
				UpdateLambdaEff();
				m_str1Plus.Format(_T("%g"), m_Data.AllProp.PROP[nD].BUCKL.dStiffRatio1st[0]);
				m_str1Mnus.Format(_T("%g"), m_Data.AllProp.PROP[nD].BUCKL.dStiffRatio1st[1]);

				m_nStiffType = m_Data.AllProp.PROP[nD].BUCKL.nInitStiffType-3;
				m_edtStiffUser.SetEditUnit(m_Data.AllProp.PROP[nD].BUCKL.dInitStiffness);
			}
			else
			{
				ASSERT(0);
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	if (m_Data.nHingeType == D_IEHP_HTYP_POINTSPRING)
	{
		m_nUserAuto = FALSE;
	}

	UpdateData(FALSE);

	if(m_bAssignProperties)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aAssignHinge_Disabled, FALSE);
		for(int i=0; i<m_aAssignHinge_ReadOnly.GetSize(); i++)
		{
			((CEditUnit*)GetDlgItem(m_aAssignHinge_ReadOnly[i]))->SetReadOnly(TRUE);
		}
	}
}

BOOL CIehpPropStlDlg::Dlg2Data()
{
	if(!CheckData()) return FALSE;
	UpdateData(TRUE);
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	switch(m_nModelType)
	{
	case D_IEHP_BUCK:
		{
			if(m_nDof<3)
			{
				m_Data.AllProp.PROP[nD].BUCKL.nYieldStrengthOpt = m_nUserAuto;
				m_Data.AllProp.PROP[nD].BUCKL.nBucklingOpt = m_nBuckling;

				m_Data.AllProp.PROP[nD].BUCKL.dYieldForce[0] = m_edtCrkPlus.GetEditValue();
				m_Data.AllProp.PROP[nD].BUCKL.dYieldForce[1] = m_edtCrkMnus.GetEditValue();

				m_Data.AllProp.PROP[nD].BUCKL.dSlednerRatio = m_edtSlednerRatio.GetEditValue();
				m_Data.AllProp.PROP[nD].BUCKL.bUserKFactor = m_chkSlednerEffKRatio.GetCheck();
				m_Data.AllProp.PROP[nD].BUCKL.dKFactor = m_edtSlednerEffKRatio.GetEditValue();

				m_Data.AllProp.PROP[nD].BUCKL.dStiffRatio1st[0] = _tstof(m_str1Plus);
				m_Data.AllProp.PROP[nD].BUCKL.dStiffRatio1st[1] = _tstof(m_str1Mnus);

				m_Data.AllProp.PROP[nD].BUCKL.nInitStiffType = m_nStiffType+3;
				m_Data.AllProp.PROP[nD].BUCKL.dInitStiffness = m_edtStiffUser.GetEditValue();
			}
			else
			{
				ASSERT(0);
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	return TRUE;
}

BOOL CIehpPropStlDlg::CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszName, LPCTSTR lpszVar, BOOL bMsg)
{
	if(bMsg) GSaveHistoryFormatNF(_LS(IDS_DB_IREG_DT_NAME_VAR_ERROR), lpszData, lpszName, lpszVar);
	return FALSE;
}

#define CheckError(x, bMsg) CheckErrorNameDataVar(_LS(IDS_DB_DT_IEHP), m_Data.strName, x, bMsg)
BOOL CIehpPropStlDlg::CheckData(BOOL bMsg/*=TRUE*/)
{
	UpdateData(TRUE);
	switch(m_nModelType)
	{
	case D_IEHP_BUCK:
		{
			if(m_nDof<3)
			{
				double dV[2] = {0., };

				dV[0] = m_edtCrkPlus.GetEditValue();
				dV[1] = m_edtCrkMnus.GetEditValue();
				if(m_nUserAuto==0)
				{
					if(dV[0]<=0.) return CheckError(_LS(IDS_DB_IEHP_STL_FY), bMsg);    
					if(m_nBuckling==1 && fabs(dV[0])<fabs(dV[1])) return CheckError(_LS(IDS_DB_IEHP_STL_FCR), bMsg);
				}

				dV[0] = m_edtSlednerRatio.GetEditValue();
				dV[1] = m_edtSlednerEffKRatio.GetEditValue();
				BOOL bUserKFactor = m_chkSlednerEffKRatio.GetCheck();
				if(m_nUserAuto==0)
				{
					if(dV[0]<=0.) return CheckError(_LS(IDS_DB_IEHP_STL_LAMBDA), bMsg);
				}
				if(m_nBuckling==1 && bUserKFactor && dV[1]<=0.) return CheckError(_LS(IDS_DB_IEHP_STL_LAMBDA_E), bMsg);

				dV[0] = _tstof(m_str1Plus);
				dV[1] = _tstof(m_str1Mnus);
				if(dV[0]<=0.) return CheckError(_LS(IDS_DB_IEHP_STL_ALPHA), bMsg);
				if(m_nBuckling==0 && dV[1]<1.0) return CheckError(_LS(IDS_DB_IEHP_STL_BETA), bMsg);
			}
			else { ASSERT(0); return FALSE; }
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	return TRUE;
}