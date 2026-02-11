// IehpPropAxialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "IehpPropAxialDlg.h"
#include "IehpPropDeformation.h"
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
// CIehpPropAxialDlg dialog

CIehpPropAxialDlg::CIehpPropAxialDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CIehpPropAxialDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIehpPropAxialDlg)
	m_strReduFact = _T("");
	m_str1Plus = _T("");
	m_str1Mnus = _T("");
	m_str2Mnus = _T("");
	m_str2Plus = _T("");
	m_nUserAuto = 1;
	m_nCompStiff = 1;
	m_nStiffType = 1;
	m_nDeformDefineType = -1;
	m_bAssignProperties = FALSE;
	//}}AFX_DATA_INIT

	m_pBitmap = NULL;
	m_pImageList = NULL;

	m_nSectType = -1;
	m_bDisableAutoCalc = FALSE;    // Auto 계산을 할 수 없는 경우

	m_aAssignHinge_Disabled.Add(IDC_IEHP_METHOD_USER_RDO);
	m_aAssignHinge_Disabled.Add(IDC_IEHP_METHOD_AUTO_RDO);
	m_aAssignHinge_Disabled.Add(IDC_IEHP_COMP_ELA_RDO);
	m_aAssignHinge_Disabled.Add(IDC_IEHP_COMP_DEG_RDO);
	m_aAssignHinge_Disabled.Add(IDC_IEHP_INIT_STIFF_USER_RDO);
	m_aAssignHinge_Disabled.Add(IDC_IEHP_INIT_STIFF_ELASTIC_RDO);
	m_aAssignHinge_Disabled.Add(IDC_DEFORMATION_INDEX);

	m_aAssignHinge_ReadOnly.Add(IDC_IEHP_YIELD_FY_T_EDT);
	m_aAssignHinge_ReadOnly.Add(IDC_IEHP_YIELD_FY_C_EDT);
	m_aAssignHinge_ReadOnly.Add(IDC_IEHP_YIELD_GAMMA_EDT);
	m_aAssignHinge_ReadOnly.Add(IDC_IEHP_YIELD_GAMMA_EDT2);
	m_aAssignHinge_ReadOnly.Add(IDC_IEHP_STIFF_REDUCTION_T1_EDT);
	m_aAssignHinge_ReadOnly.Add(IDC_IEHP_STIFF_REDUCTION_T2_EDT);
	m_aAssignHinge_ReadOnly.Add(IDC_IEHP_STIFF_REDUCTION_C1_EDT);
	m_aAssignHinge_ReadOnly.Add(IDC_IEHP_STIFF_REDUCTION_C2_EDT);
	m_aAssignHinge_ReadOnly.Add(IDC_IEHP_INIT_STIFF_USER_EDT);
}

CIehpPropAxialDlg::~CIehpPropAxialDlg()
{
	if (m_pBitmap != NULL){ delete m_pBitmap; m_pBitmap = NULL; }
	if (m_pImageList != NULL) { delete m_pImageList;  m_pImageList = NULL;  }
}

void CIehpPropAxialDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K IehgKey, T_IEHP_AXIAL data,T_IEHP_AXIAL subdata, int nFg1, int nFg2, int nSectType/*=1*/, BOOL bAssignProperties/*FALSE*/)
{
	m_Data = iehpD;
	m_nDof = nFg2; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	m_Data.AllProp.PROP[nD].AXIAL = data;
	m_Data.AllSubProp.PROP[nD].AXIAL = subdata;

	m_nModelType = nFg1;
	m_nIntrType = iehpD.nInteractionType;
	m_nHingeType = iehpD.nHingeType;
	m_nDefinition = iehpD.nDefinition;
	m_nSectType = nSectType;
	m_bAssignProperties = bAssignProperties;
	m_ElemK = IehgKey.key.objK;
}

void CIehpPropAxialDlg::SetControlStateData(int nOptType)
{
	m_nUserAuto = nOptType;
}

void CIehpPropAxialDlg::ChangeBitmap()
{
	// set appropriate bitmap
	if (m_pBitmap != NULL){ delete m_pBitmap; m_pBitmap = NULL; }
	CBCGPStatic* SecImage = (CBCGPStatic*)GetDlgItem(IDC_CMD_IEHP_AXIAL_STIFF_BMP);
	m_pBitmap = (CBitmap*)SecImage;
	m_pBitmap = new CBitmap;
	if(m_nCompStiff==0)
	{
		m_pBitmap->LoadBitmap(IDB_CMD_IEHP_AXIAL_STIFF_ELA);
	}
	else
	{
		m_pBitmap->LoadBitmap(IDB_CMD_IEHP_AXIAL_STIFF_DEG);
	}
	SecImage->SetBitmap(HBITMAP(*m_pBitmap));
}

bool CIehpPropAxialDlg::SyncDialogData()
{
	if (!Dlg2Data()) return false;

	return true;
}

void CIehpPropAxialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIehpPropAxialDlg)
	DDX_Radio(pDX, IDC_IEHP_METHOD_USER_RDO       , m_nUserAuto);
	DDX_Radio(pDX, IDC_IEHP_COMP_ELA_RDO          , m_nCompStiff);
	DDX_Radio(pDX, IDC_IEHP_INIT_STIFF_USER_RDO   , m_nStiffType);
	DDX_Control(pDX, IDC_IEHP_YIELD_FY_T_EDT      , m_edtYldPlus);
	DDX_Control(pDX, IDC_IEHP_YIELD_FY_C_EDT      , m_edtYldMnus);
	DDX_Control(pDX, IDC_IEHP_YIELD_GAMMA_EDT2    , m_edtCrkMnus);
	DDX_Control(pDX, IDC_IEHP_YIELD_FY_T_UNT      , m_untYldPlus);
	DDX_Control(pDX, IDC_IEHP_YIELD_FY_C_UNT      , m_untYldMnus);
	DDX_Control(pDX, IDC_IEHP_YIELD_GAMMA_UNIT    , m_untCrkMnus);
	DDX_Control(pDX, IDC_IEHP_INIT_STIFF_USER_EDT , m_edtStiffUser);
	DDX_Control(pDX, IDC_IEHP_INIT_STIFF_USER_UNIT, m_untStiffUser);
	DDX_Text(pDX, IDC_IEHP_YIELD_GAMMA_EDT        , m_strReduFact);
	DDX_Text(pDX, IDC_IEHP_STIFF_REDUCTION_T1_EDT , m_str1Plus);
	DDX_Text(pDX, IDC_IEHP_STIFF_REDUCTION_T2_EDT , m_str2Plus);
	DDX_Text(pDX, IDC_IEHP_STIFF_REDUCTION_C1_EDT , m_str1Mnus);
	DDX_Text(pDX, IDC_IEHP_STIFF_REDUCTION_C2_EDT , m_str2Mnus);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIehpPropAxialDlg, CDialogMove)
	//{{AFX_MSG_MAP(CIehpPropAxialDlg)
	ON_BN_CLICKED(IDC_IEHP_METHOD_USER_RDO        , OnUserAutoRdo)
	ON_BN_CLICKED(IDC_IEHP_METHOD_AUTO_RDO        , OnUserAutoRdo)
	ON_BN_CLICKED(IDC_IEHP_COMP_ELA_RDO           , OnCompStiffRdo)
	ON_BN_CLICKED(IDC_IEHP_COMP_DEG_RDO           , OnCompStiffRdo)
	ON_BN_CLICKED(IDC_IEHP_INIT_STIFF_USER_RDO    , OnStiffTypeRdo)
	ON_BN_CLICKED(IDC_IEHP_INIT_STIFF_ELASTIC_RDO , OnStiffTypeRdo)
	ON_BN_CLICKED(IDC_DEFORMATION_INDEX           , OnDeformationIndex)
	ON_EN_CHANGE(IDC_IEHP_YIELD_FY_T_EDT          , OnChangeYieldPlusEdit)
	ON_EN_CHANGE(IDC_IEHP_YIELD_GAMMA_EDT         , OnChangeGammaEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIehpPropAxialDlg message handlers

BOOL CIehpPropAxialDlg::OnInitDialog() 
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
	m_edtCrkMnus.SetReadOnly(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
								// EXCEPTION: OCX Property Pages should return FALSE
}

void CIehpPropAxialDlg::OnOK() 
{
	if(!Dlg2Data()) return;

	CDialogMove::OnOK();
}

void CIehpPropAxialDlg::OnUserAutoRdo() 
{
	UpdateData(TRUE);
	CtrlManager();
}

void CIehpPropAxialDlg::OnCompStiffRdo() 
{
	UpdateData(TRUE);
	CtrlManager();
	ChangeBitmap();
}

void CIehpPropAxialDlg::OnStiffTypeRdo() 
{
	UpdateData(TRUE);
	CtrlManager();
}

void CIehpPropAxialDlg::OnDeformationIndex()
{	
	int nnSymmetry=1;
	CIehpPropDeformation DeformationDlg(nnSymmetry,m_nModelType,m_nDeformDefineType,m_nDof,m_Data.AllProp.PROP);
	if(DeformationDlg.DoModal() == IDOK)
	{
		m_nDeformDefineType = DeformationDlg.GetDeformDefineType();
	}
}

void CIehpPropAxialDlg::OnChangeYieldPlusEdit()
{
	UpdateCompStrengthAtPointC();
}

void CIehpPropAxialDlg::OnChangeGammaEdit()
{
	UpdateCompStrengthAtPointC();
}

void CIehpPropAxialDlg::UpdateCompStrengthAtPointC()
{
	UpdateData(TRUE);
	double dYieldReduFac = _tstof(m_strReduFact);
	double dYieldForce = m_edtYldPlus.GetEditValue();
	m_edtCrkMnus.SetEditUnit(dYieldReduFac*fabs(dYieldForce));
}

void CIehpPropAxialDlg::CtrlManager()
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
	m_edtYldPlus.SetReadOnly(m_nUserAuto==1);
	m_edtYldMnus.SetReadOnly(m_nUserAuto==1);
	GetDlgItem(IDC_IEHP_YIELD_FY_C_STC)->EnableWindow(m_nCompStiff == 1);
	GetDlgItem(IDC_IEHP_YIELD_FY_C_EDT)->EnableWindow(m_nCompStiff == 1);
	GetDlgItem(IDC_IEHP_YIELD_FY_C_STC2)->EnableWindow(m_nCompStiff == 1);
	GetDlgItem(IDC_IEHP_YIELD_FY_C_UNT)->EnableWindow(m_nCompStiff == 1);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Stiffness Reduction Ratio
	GetDlgItem(IDC_IEHP_STIFF_REDUCTION_C2_STC)->EnableWindow(m_nCompStiff == 1);
	GetDlgItem(IDC_IEHP_STIFF_REDUCTION_C2_EDT)->EnableWindow(m_nCompStiff == 1);
	GetDlgItem(IDC_IEHP_STIFF_REDUCTION_C2_STC2)->EnableWindow(m_nCompStiff == 1);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Compression Initial Stiffness
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

void CIehpPropAxialDlg::SetUnitAndTitle()
{
	if(m_nDof<3) // None & Dx, Dy, Dz
	{
		m_untYldPlus.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldForce);
		m_untYldMnus.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dYieldForce);
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

	CString str; str.Empty();
	this->GetWindowText(str);
	if(m_nModelType == D_IEHP_AXIAL) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_AXIAL)); // Axial Stiffness
}

void CIehpPropAxialDlg::Data2Dlg()
{
	if(m_bAssignProperties)
		CDlgUtil::CtrlEnableDisable(this, m_aAssignHinge_Disabled, TRUE);

	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	switch(m_nModelType)
	{
		case D_IEHP_AXIAL:
			{
				if(m_nDof<3)
				{
					m_nUserAuto = m_Data.AllProp.PROP[nD].AXIAL.nYieldStrengthOpt;
					m_nCompStiff = m_Data.AllProp.PROP[nD].AXIAL.nCompStiffOPt;

					m_edtYldPlus.SetEditUnit(m_Data.AllProp.PROP[nD].AXIAL.dYieldForce[0]);
					m_edtYldMnus.SetEditUnit(m_Data.AllProp.PROP[nD].AXIAL.dYieldForce[1]);
					m_strReduFact.Format(_T("%g"), m_Data.AllProp.PROP[nD].AXIAL.dYieldReduFac);
					double dCrkMnus = fabs(m_Data.AllProp.PROP[nD].AXIAL.dYieldForce[0])*m_Data.AllProp.PROP[nD].AXIAL.dYieldReduFac;
					m_edtCrkMnus.SetEditUnit(dCrkMnus);

					m_str1Plus.Format(_T("%g"), m_Data.AllProp.PROP[nD].AXIAL.dStiffRatio1st[0]);
					m_str1Mnus.Format(_T("%g"), m_Data.AllProp.PROP[nD].AXIAL.dStiffRatio1st[1]);
					m_str2Plus.Format(_T("%g"), m_Data.AllProp.PROP[nD].AXIAL.dStiffRatio2nd[0]);
					m_str2Mnus.Format(_T("%g"), m_Data.AllProp.PROP[nD].AXIAL.dStiffRatio2nd[1]);

					m_nStiffType = m_Data.AllProp.PROP[nD].AXIAL.nInitStiffType-3;
					m_edtStiffUser.SetEditUnit(m_Data.AllProp.PROP[nD].AXIAL.dInitStiffness);

					for(int i=0; i<2; i++) for(int j=0; j<5; j++)
						m_dDeformCapacity[i][j] = m_Data.AllProp.PROP[nD].AXIAL.dDeformCapacity[i][j];
					m_nDeformDefineType = m_Data.AllProp.PROP[nD].AXIAL.nDeformDefineType;
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

BOOL CIehpPropAxialDlg::Dlg2Data()
{
	if(!CheckData()) return FALSE;
	UpdateData(TRUE);
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	switch(m_nModelType)
	{
	case D_IEHP_AXIAL:
		{
			if(m_nDof<3)
			{
				m_Data.AllProp.PROP[nD].AXIAL.nYieldStrengthOpt = m_nUserAuto;
				m_Data.AllProp.PROP[nD].AXIAL.nCompStiffOPt = m_nCompStiff;

				m_Data.AllProp.PROP[nD].AXIAL.dYieldForce[0] = m_edtYldPlus.GetEditValue();
				m_Data.AllProp.PROP[nD].AXIAL.dYieldForce[1] = m_edtYldMnus.GetEditValue();
				m_Data.AllProp.PROP[nD].AXIAL.dYieldReduFac = _tstof(m_strReduFact);

				m_Data.AllProp.PROP[nD].AXIAL.dStiffRatio1st[0] = _tstof(m_str1Plus);
				m_Data.AllProp.PROP[nD].AXIAL.dStiffRatio1st[1] = _tstof(m_str1Mnus);
				m_Data.AllProp.PROP[nD].AXIAL.dStiffRatio2nd[0] = _tstof(m_str2Plus);
				m_Data.AllProp.PROP[nD].AXIAL.dStiffRatio2nd[1] = _tstof(m_str2Mnus);

				m_Data.AllProp.PROP[nD].AXIAL.nInitStiffType = m_nStiffType+3;
				m_Data.AllProp.PROP[nD].AXIAL.dInitStiffness = m_edtStiffUser.GetEditValue();

				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
					m_Data.AllProp.PROP[nD].AXIAL.dDeformCapacity[i][j] = m_dDeformCapacity[i][j];
				m_Data.AllProp.PROP[nD].AXIAL.nDeformDefineType = m_nDeformDefineType;
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

BOOL CIehpPropAxialDlg::CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszName, LPCTSTR lpszVar, BOOL bMsg)
{
	if(bMsg) GSaveHistoryFormatNF(_LS(IDS_DB_IREG_DT_NAME_VAR_ERROR), lpszData, lpszName, lpszVar);
	return FALSE;
}

#define CheckError(x, bMsg) CheckErrorNameDataVar(_LS(IDS_DB_DT_IEHP), m_Data.strName, x, bMsg)
BOOL CIehpPropAxialDlg::CheckData(BOOL bMsg/*=TRUE*/)
{
	UpdateData(TRUE);
	switch(m_nModelType)
	{
	case D_IEHP_AXIAL:
		{
			if(m_nDof<3)
			{
				double dV[4] = {0., };   // Force

				dV[0] = m_edtYldPlus.GetEditValue();
				dV[1] = m_edtYldMnus.GetEditValue();
				dV[2] = _tstof(m_strReduFact);

				if(m_nUserAuto==0)
				{
					if(dV[0]<=0.) return CheckError(_LS(IDS_DB_IEHP_AXIAL_FYT), bMsg);
					if(dV[1]<=0.) return CheckError(_T("Fy(c) > 0.0"), bMsg);
					if(m_nCompStiff==1 && fabs(dV[0])>fabs(dV[1])) return CheckError(_LS(IDS_DB_IEHP_AXIAL_FYC), bMsg);
				}
				if(dV[2]<0.5) return CheckError(_LS(IDS_DB_IEHP_AXIAL_RED), bMsg);

				dV[0] = _tstof(m_str1Plus); dV[1] = _tstof(m_str2Plus);
				dV[2] = _tstof(m_str1Mnus); dV[3] = _tstof(m_str2Mnus);

				if(dV[0]<0.0001 || dV[0]>1.0      ) return CheckError(_LS(IDS_DB_IEHP_AXIAL_T1), bMsg);
				if(dV[1]<0.     || dV[1] >= dV[0] ) return CheckError(_LS(IDS_DB_IEHP_AXIAL_T2), bMsg);
				if(dV[2]<0.0001 || dV[2]>1.0      ) return CheckError(_LS(IDS_DB_IEHP_AXIAL_C1), bMsg);
				if(m_nCompStiff==1 && (dV[3]<=dV[0] || dV[3]>1.0)) return CheckError(_LS(IDS_DB_IEHP_AXIAL_C2), bMsg); 

				// Deformation Capacity
				for(int i=0; i<2; i++) for(int j=0; j<5; j++)
				{
					if(fabs(m_dDeformCapacity[i][j])<0.) return CheckError(_LS(IDS_DB_DEFORMATION_CAPACITY), bMsg);
					if(j>0) if(fabs(m_dDeformCapacity[i][j-1]) >= fabs(m_dDeformCapacity[i][j])) 
						return CheckError(_LS(IDS_DB_DEFORMATION_CAPACITY_COM), bMsg);
				}
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