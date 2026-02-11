// IehpPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "IehpProp_MLinDlg.h"
#include "IehpProp_PivotPicDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_EditData.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DbToolPmcv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIehpProp_MLinDlg dialog


CIehpProp_MLinDlg::CIehpProp_MLinDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CIehpProp_MLinDlg::IDD, pParent)
{
	m_nDeformDefineType = -1;
	m_Data.Initialize();
	m_nDof = 0; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz  
	m_MultiData.Initialize();
	m_nModelType = 0;
	m_nHingeType = 0;

	m_pPivotPic = NULL;
		
	m_aOK.RemoveAll();
	m_aOK.Add(IDOK);
	m_aOK.Add(IDCANCEL);
	m_aOK.FreeExtra();

	m_aTypeRdo.RemoveAll();
	m_aTypeRdo.Add(IDC_IEHP_TYPE_RDO_0);
	m_aTypeRdo.Add(IDC_IEHP_TYPE_RDO_1);
	m_aTypeRdo.Add(IDC_IEHP_TYPE_RDO_2);
	m_aTypeRdo.FreeExtra();
	
	m_aCtrlPlus.RemoveAll();
	m_aCtrlPlus.Add(IDC_IEHP_STIFF_PLUS_EDT);
	m_aCtrlPlus.Add(IDC_IHEP_STIFF_PLUS_UNIT);
	m_aCtrlPlus.Add(IDC_IEHP_LEVEL_1PLUS);
	m_aCtrlPlus.Add(IDC_IEHP_LEVEL_2PLUS);
	m_aCtrlPlus.Add(IDC_IEHP_LEVEL_3PLUS);
	m_aCtrlPlus.Add(IDC_IEHP_LEVEL_4PLUS);
	m_aCtrlPlus.Add(IDC_IEHP_LEVEL_5PLUS);
	m_aCtrlPlus.FreeExtra();
	
	m_aCtrlMinus.RemoveAll();
	m_aCtrlMinus.Add(IDC_IEHP_STIFF_MINU_EDT);
	m_aCtrlMinus.Add(IDC_IHEP_STIFF_MINU_UNIT);
	m_aCtrlMinus.Add(IDC_IEHP_LEVEL_1MNUS);
	m_aCtrlMinus.Add(IDC_IEHP_LEVEL_2MNUS);
	m_aCtrlMinus.Add(IDC_IEHP_LEVEL_3MNUS);
	m_aCtrlMinus.Add(IDC_IEHP_LEVEL_4MNUS);
	m_aCtrlMinus.Add(IDC_IEHP_LEVEL_5MNUS);
	m_aCtrlMinus.FreeExtra();

	m_aHysParam.RemoveAll();
	m_aHysParam.Add(IDC_IEHP_PARAM_ALPHA_EDT );
	m_aHysParam.Add(IDC_IEHP_PARAM_ALPHA2_EDT);
	m_aHysParam.Add(IDC_IEHP_PARAM_BETA_EDT  );
	m_aHysParam.Add(IDC_IEHP_PARAM_BETA2_EDT );
	m_aHysParam.Add(IDC_IEHP_PARAM_ETA_EDT   );
	m_aHysParam.Add(IDC_IEHP_HYST_PARAM_STC  );
	m_aHysParam.Add(IDC_IEHP_PARAM_ALPHA_STC );
	m_aHysParam.Add(IDC_IEHP_PARAM_ALPHA2_STC);
	m_aHysParam.Add(IDC_IEHP_PARAM_BETA_STC  );
	m_aHysParam.Add(IDC_IEHP_PARAM_BETA2_STC );
	m_aHysParam.Add(IDC_IEHP_PARAM_ETA_STC   );
	m_aHysParam.Add(IDC_IEHP_DECS_ALPHA_STC   );
	m_aHysParam.Add(IDC_IEHP_DECS_BETA_STC    );
	m_aHysParam.Add(IDC_IEHP_DECS_ETA_STC     );
	m_aHysParam.Add(IDC_IEHP_PARAM_BETA_STC2  );
	m_aHysParam.Add(IDC_IEHP_PARAM_BETA2_STC2 );
	m_aHysParam.FreeExtra();

	m_aUnParam.RemoveAll();
	m_aUnParam.Add(IDC_IEHP_UNLODING_PARAM_STC);
	m_aUnParam.Add(IDC_IEHP_UNLODING_PARAM_EXP_STC);
	m_aUnParam.Add(IDC_IEHP_UNLODING_PARAM_EXP_0_STC);
	m_aUnParam.Add(IDC_IEHP_UNLODING_PARAM_EXP_EDT);
	m_aUnParam.Add(IDC_IEHP_UNLODING_PARAM_EXP_1_STC);
	m_aUnParam.Add(IDC_IEHP_UNLODING_PARAM_ELA_STC);
	m_aUnParam.FreeExtra();

	m_Data.Initialize();
	for(int i=0; i<7; i++)
	{
		m_Data.AllProp.PROP[i].KINEMA.Initialize();
		m_Data.AllProp.PROP[i].ORIGIN.Initialize();
		m_Data.AllProp.PROP[i].PEAK.Initialize();
		m_Data.AllProp.PROP[i].CLOUGH.Initialize();
		m_Data.AllProp.PROP[i].DEGRAD.Initialize();
		m_Data.AllProp.PROP[i].TAKEDA.Initialize();
		m_Data.AllProp.PROP[i].YLDSUR.Initialize();
		m_Data.AllProp.PROP[i].NORBIL.Initialize();
		m_Data.AllProp.PROP[i].ELABIL.Initialize();
		m_Data.AllProp.PROP[i].ELATRI.Initialize();
		m_Data.AllProp.PROP[i].ELATET.Initialize();
		m_Data.AllProp.PROP[i].MULTLIN.Initialize();
	}

	m_dZero = 10E-9;
}

CIehpProp_MLinDlg::~CIehpProp_MLinDlg()
{
	
}
void CIehpProp_MLinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIehpProp_MLinDlg)
	DDX_Control(pDX, IDC_IEHP_STIFF_PLUS_EDT, m_edtPlusStiff);
	DDX_Control(pDX, IDC_IEHP_STIFF_MINU_EDT, m_edtMnusStiff);
	DDX_Control(pDX, IDC_IHEP_STIFF_PLUS_UNIT, m_untPlusStiff);
	DDX_Control(pDX, IDC_IHEP_STIFF_MINU_UNIT, m_untMnusStiff);

	DDX_Control(pDX, IDC_IEHP_PARAM_ALPHA_EDT , m_edtAlpha1);
	DDX_Control(pDX, IDC_IEHP_PARAM_ALPHA2_EDT, m_edtAlpha2);
	DDX_Control(pDX, IDC_IEHP_PARAM_BETA_EDT  , m_edtBeta1);
	DDX_Control(pDX, IDC_IEHP_PARAM_BETA2_EDT , m_edtBeta2);
	DDX_Control(pDX, IDC_IEHP_PARAM_ETA_EDT   , m_edtEta);
	DDX_Control(pDX, IDC_IEHP_UNLODING_PARAM_EXP_EDT, m_edtUnBeta);
	DDX_Control(pDX, IDC_IEHP_SCFACT_DISPL_EDT, m_edtSFactDispl);
	DDX_Control(pDX, IDC_IEHP_SCFACT_FORCE_EDT, m_edtSFactForce);

	DDX_Radio(pDX, IDC_IEHP_TYPE_RDO_0, m_nType);
	DDX_Radio(pDX, IDC_IEHP_DFOM_RDO_C, m_nDeformDefineType);
	//}}AFX_DATA_MAP
	// Deformation Capacity
	UINT nDeformCapacity[2][5] = {{IDC_IEHP_LEVEL_1PLUS, IDC_IEHP_LEVEL_2PLUS, IDC_IEHP_LEVEL_3PLUS,
																 IDC_IEHP_LEVEL_4PLUS, IDC_IEHP_LEVEL_5PLUS},
																{IDC_IEHP_LEVEL_1MNUS, IDC_IEHP_LEVEL_2MNUS, IDC_IEHP_LEVEL_3MNUS,
																 IDC_IEHP_LEVEL_4MNUS, IDC_IEHP_LEVEL_5MNUS}};
	for(int i=0; i<2; i++) for(int j=0; j<5; j++)
		DDX_Control(pDX, nDeformCapacity[i][j], m_edtDeformCapacity[i][j]);

	DDX_Control(pDX, IDC_IEHP_MLIN_TABLE, m_wndGrid);
}


BEGIN_MESSAGE_MAP(CIehpProp_MLinDlg, CDialogMove)
	//{{AFX_MSG_MAP(CIehpProp_MLinDlg)
	ON_WM_CTLCOLOR ()
	ON_BN_CLICKED(IDC_IEHP_TYPE_RDO_0, OnSelectTypeRdo)
	ON_BN_CLICKED(IDC_IEHP_TYPE_RDO_1, OnSelectTypeRdo)
	ON_BN_CLICKED(IDC_IEHP_TYPE_RDO_2, OnSelectTypeRdo)
	ON_EN_CHANGE(IDC_IEHP_LEVEL_1PLUS, OnChangeDefomCapacity)
	ON_EN_CHANGE(IDC_IEHP_LEVEL_2PLUS, OnChangeDefomCapacity)
	ON_EN_CHANGE(IDC_IEHP_LEVEL_3PLUS, OnChangeDefomCapacity)
	ON_EN_CHANGE(IDC_IEHP_LEVEL_4PLUS, OnChangeDefomCapacity)
	ON_EN_CHANGE(IDC_IEHP_LEVEL_5PLUS, OnChangeDefomCapacity)
	ON_EN_CHANGE(IDC_IEHP_SCFACT_DISPL_EDT, OnScaleFactorChanged)
	ON_EN_CHANGE(IDC_IEHP_SCFACT_FORCE_EDT, OnScaleFactorChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIehpProp_MLinDlg Data & Window handlers

void CIehpProp_MLinDlg::SetIehpPropData(T_IEHP_D iehpD, T_IEHP_MLIN data, int nFg1, int nFg2)
{
	m_Data = iehpD;
	m_nDof = nFg2; // 1:Dx Dy Dz, 2:Rx Ry Rz, 3:(P-My-Mz)Rx Ry Rz
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;
	m_Data.AllProp.PROP[nD].MULTLIN = data;
	m_MultiData = iehpD.MultiData[nD];
	m_nModelType = nFg1;
	m_nHingeType = iehpD.nHingeType;
}


/////////////////////////////////////////////////////////////////////////////////////////
void CIehpProp_MLinDlg::Data2Dlg()
{
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;
	
	m_edtSFactDispl.SetEditUnit(m_Data.AllProp.PROP[nD].MULTLIN.dScaleF_Displ);
	m_edtSFactForce.SetEditUnit(m_Data.AllProp.PROP[nD].MULTLIN.dScaleF_Force);
	double dSFact = m_Data.AllProp.PROP[nD].MULTLIN.dScaleF_Displ / m_Data.AllProp.PROP[nD].MULTLIN.dScaleF_Force;
	m_edtPlusStiff.SetEditUnit(m_Data.AllProp.PROP[nD].MULTLIN.dInitStiffP * dSFact);
	m_edtMnusStiff.SetEditUnit(m_Data.AllProp.PROP[nD].MULTLIN.dInitStiffN * dSFact);
	
	m_nType = m_Data.AllProp.PROP[nD].MULTLIN.nType;
	m_nDeformDefineType = m_Data.AllProp.PROP[nD].MULTLIN.nDeformDefineType;
	for(int i=0; i<2; i++)
	{
		for(int j=0; j<5; j++)
		{
			m_edtDeformCapacity[i][j].SetEditUnit(m_Data.AllProp.PROP[nD].MULTLIN.dDeformCapacity[i][j]);
		}
	}

	if(m_nModelType == D_IEHP_MLPI) // Pivot Type 이면
	{
		m_edtAlpha1.SetEditUnit(m_Data.AllProp.PROP[nD].MULTLIN.dHysParam_Alpha1);
		m_edtAlpha2.SetEditUnit(m_Data.AllProp.PROP[nD].MULTLIN.dHysParam_Alpha2);
		m_edtBeta1.SetEditUnit(m_Data.AllProp.PROP[nD].MULTLIN.dHysParam_Beta1);
		m_edtBeta2.SetEditUnit(m_Data.AllProp.PROP[nD].MULTLIN.dHysParam_Beta2);
		m_edtEta.SetEditUnit(m_Data.AllProp.PROP[nD].MULTLIN.dHysParam_Eta);
	}
	else if(m_nModelType == D_IEHP_MLTA) // Plastic Takeda
	{
		//m_Data.PROP[nD].MULTLIN.dHysParam_Beta1 = 0.4;
		m_edtUnBeta.SetEditUnit(m_Data.AllProp.PROP[nD].MULTLIN.dHysParam_Beta1);
	}

	UpdateData(FALSE);
}

BOOL CIehpProp_MLinDlg::CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszName, LPCTSTR lpszVar, BOOL bMsg)
{
	if(bMsg) GSaveHistoryFormatNF(_LS(IDS_DB_IREG_DT_NAME_VAR_ERROR), lpszData, lpszName, lpszVar);
	return FALSE;
}

#define CheckError(x, bMsg) CheckErrorNameDataVar(_LS(IDS_DB_DT_IEHP), m_Data.strName, x, bMsg)
BOOL CIehpProp_MLinDlg::CheckData(BOOL bMsg/*=TRUE*/)
{
	UpdateData(TRUE);

	BOOL bExistZero = FALSE;
	BOOL bNonExistSmallDisp = FALSE;
	BOOL bNonExistSmallForce = FALSE;
	BOOL bNonExistSmallForceFinal = FALSE;
	BOOL bExistDiffSign = FALSE;

	int nNumForcPlus=0, nNumForcMinus=0, nNumDispPlus=0, nNumDispMinus=0;
	int nCount = m_MultiData.arMultiLinearData.GetSize();
	double dDisp=0.0, dForce=0.0, dPreDisp=0.0, dPreForce=0.0, dMulti=0.0;
	
	BOOL bCheckSameForc = TRUE;//(m_nModelType==D_IEHP_MLKI||m_nModelType==D_IEHP_MLTA||m_nModelType==D_IEHP_MLPI) ? TRUE : FALSE;
	BOOL bMinusSameForcFound=FALSE, dPlusSameForcFound=FALSE;
	double dMinusSameForc=0.0, dPlusSameForc=0.0;

	CArray<double, double> aPlus;
	CArray<double, double> aMinus;

	for (int i=0; i<nCount; i++)
	{
		dDisp  = m_MultiData.arMultiLinearData[i].dblDisp;
		dForce = m_MultiData.arMultiLinearData[i].dblForce;

		int nDispCase  = CCompFunc::CompRealTol(dDisp , 0.0, m_dZero);
		int nForceCase = CCompFunc::CompRealTol(dForce, 0.0, m_dZero);
		
		if (nDispCase == 1)
		{
			nNumDispPlus++;
			aPlus.Add(dForce);
		}
		else if (nDispCase == -1)
		{
			nNumDispMinus++;
			aMinus.Add(dForce);
		}

		if(nDispCase == 0 && nForceCase == 0)
		{
			bExistZero = TRUE;
		}
		else if(nForceCase ==  1)
		{	// 양수
			nNumForcPlus++;
			if( bCheckSameForc )
			{
				if( !dPlusSameForcFound )
				{
					if( CCompFunc::CompRealTol(dForce, dPlusSameForc, m_dZero) == 0 )
						dPlusSameForcFound = TRUE;
					dPlusSameForc = dForce;
				}
				else
				{	// 동일한 값이 발견 된 이후에 다른 값이 들어오는 것을 허용하지 않음
					if( CCompFunc::CompRealTol(dForce, dPlusSameForc, m_dZero) != 0 )
						return CheckError(_LS(IDS_DB_MULTI_LINEAR_FINAL_FORCE_ERROR), bMsg);
				}
			}
		}
		else if(nForceCase == -1)
		{	// 음수
			nNumForcMinus++;
			if( bCheckSameForc && !bMinusSameForcFound )
			{
				if( CCompFunc::CompRealTol(dForce, dMinusSameForc, m_dZero) == 0 )
				{
					bMinusSameForcFound = TRUE;
					if( i > 1 ) // 음수 구간 중 첫 동일한 값이 발견 된 지점이 0와 1의 관게가 아니라면
						return CheckError(_LS(IDS_DB_MULTI_LINEAR_FINAL_FORCE_ERROR), bMsg);
				}
				else
					dMinusSameForc = dForce;
			}
		}

		dMulti = dDisp*dForce;
		if(CCompFunc::CompRealTol(dMulti, 0.0, m_dZero) == -1)
		{
			bExistDiffSign = TRUE;
		}


		if(i > 0)
		{
			dPreDisp = m_MultiData.arMultiLinearData[i-1].dblDisp;
			dPreForce = m_MultiData.arMultiLinearData[i-1].dblForce;
			if(CCompFunc::CompRealTol(dDisp, dPreDisp, m_dZero) != 1)
			{
				bNonExistSmallDisp = TRUE;
			}

			if(CCompFunc::CompRealTol(dForce, dPreForce, m_dZero) != 1)
			{
				if(m_nModelType == D_IEHP_MLKI || m_nModelType == D_IEHP_MLTA)
				{	// Multi Linear Type에서 Perfect Plastic 허용 (김종민 부장님 요청)
					//bNonExistSmallForceFinal = TRUE;
				}
				else if(m_nModelType == D_IEHP_MLEL || m_nModelType == D_IEHP_MLPI)
				{	// Multi Linear Type에서 Perfect Plastic 허용 (안종건 대리님 요청)
					//if(i!=nCount-1 && i!=1)   bNonExistSmallForce = TRUE;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//  음의 기울기 구간 확인, 최종국간/그 외
	BOOL bMinus01 = FALSE;
	BOOL bMinus02 = FALSE;
	for (int i = 1; i < aMinus.GetSize(); i++)
	{
		if (aMinus[i] < aMinus[i - 1])
		{
			if (i == 1) bMinus01 = TRUE;
			else        bMinus02 = TRUE;
		}
	}

	BOOL bPlus01 = FALSE;
	BOOL bPlus02 = FALSE;
	for (int i = aPlus.GetSize() - 1; i > 0; i--)
	{
		if (aPlus[i] < aPlus[i - 1])
		{
			if (i == aPlus.GetSize() - 1) bPlus01 = TRUE;
			else                          bPlus02 = TRUE;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	if(m_nModelType == D_IEHP_MLKI || m_nModelType == D_IEHP_MLTA) // Kinematic이거나 Takeda 이면
	{
		if( m_nType == 0 )	// Both
		{
			if(nNumForcPlus != nNumForcMinus)
			{
				if(m_nModelType == D_IEHP_MLKI) return CheckError(_LS(IDS_DB_MULTI_LINEAR_SAME_COUNT_KINEMATIC), bMsg);
				else if(m_nModelType == D_IEHP_MLTA) return CheckError(_LS(IDS_DB_MULTI_LINEAR_SAME_COUNT_TAKEDA), bMsg);
				else ASSERT(0);
			}

			if(nNumForcPlus < 1 || nNumForcMinus < 1)
			{
				if(m_nModelType == D_IEHP_MLKI) return CheckError(_LS(IDS_DB_MULTI_LINEAR_MUST_ONE_MORE_KINEMATIC), bMsg);
				else if(m_nModelType == D_IEHP_MLTA) return CheckError(_LS(IDS_DB_MULTI_LINEAR_MUST_ONE_MORE_TAKEDA), bMsg);
				else ASSERT(0);
			}
		}
		else if( m_nType == 1 ) // Tens. Only
		{			
			if(nNumForcPlus < 1)
			{
				if(m_nModelType == D_IEHP_MLKI) return CheckError(_LS(IDS_DB_MULTI_LINEAR_MUST_ONE_MORE_KINEMATIC_PLUS), bMsg);
				else if(m_nModelType == D_IEHP_MLTA) return CheckError(_LS(IDS_DB_MULTI_LINEAR_MUST_ONE_MORE_TAKEDA_PLUS), bMsg);
				else ASSERT(0);
			}
			if(nNumDispMinus > 1)
			{
				return CheckError(_LS(IDS_DB_MULTI_LINEAR_TENSONLY_NO_MINUS), bMsg);
			}
		}
		else if( m_nType == 2 ) // Comp. Only
		{			
			if(nNumForcMinus < 1)
			{
				if(m_nModelType == D_IEHP_MLKI) return CheckError(_LS(IDS_DB_MULTI_LINEAR_MUST_ONE_MORE_KINEMATIC_MINUS), bMsg);
				else if(m_nModelType == D_IEHP_MLTA) return CheckError(_LS(IDS_DB_MULTI_LINEAR_MUST_ONE_MORE_TAKEDA_MINUS), bMsg);
				else ASSERT(0);
			}
			if(nNumDispPlus > 1)
			{
				return CheckError(_LS(IDS_DB_MULTI_LINEAR_COMPONLY_NO_PLUS), bMsg);
			}
		}
	}
	else if(m_nModelType == D_IEHP_MLPI)
	{
		if(nNumForcPlus < 1 || nNumForcMinus < 1)
		{
			return CheckError(_LS(IDS_DB_MULTI_LINEAR_MUST_ONE_MORE_PIVOT), bMsg);
		}

		double dAlpha1 =  m_edtAlpha1.GetEditValue();
		double dAlpha2 =  m_edtAlpha2.GetEditValue();
		double dBeta1  =  m_edtBeta1 .GetEditValue();
		double dBeta2  =  m_edtBeta2 .GetEditValue();
		double dEta    =  m_edtEta   .GetEditValue();
		
		if(dAlpha1 < 1.0)  return CheckError(_LS(IDS_DB_MULTI_LINEAR_CHK_ALPHA1), bMsg);
		if(dAlpha2 < 1.0)  return CheckError(_LS(IDS_DB_MULTI_LINEAR_CHK_ALPHA2), bMsg);
		if(dBeta1  <= 0.0 || dBeta1 > 1.0)  return CheckError(_LS(IDS_DB_MULTI_LINEAR_CHK_BETA1), bMsg);
		if(dBeta2  <= 0.0 || dBeta2 > 1.0)  return CheckError(_LS(IDS_DB_MULTI_LINEAR_CHK_BETA2), bMsg);
		if(dEta    <  0.0)   return CheckError(_LS(IDS_DB_MULTI_LINEAR_CHK_ETA), bMsg);
	}

	if(m_nModelType == D_IEHP_MLTA) // Plastic Takeda
	{
		double dBeta  = m_edtUnBeta.GetEditValue();
		if(dBeta < 0.0 || dBeta > 1.0)
			return CheckError(_LS(IDS_DB_MULTI_LINEAR_TAKEDA_UNLOADING_STIFF), bMsg);
	}


	if(!bExistZero)              return CheckError(_LS(IDS_DB_MULTI_LINEAR_ZERO_VALUE), bMsg);
	if(bNonExistSmallDisp)       return CheckError(_LS(IDS_DB_MULTI_LINEAR_SAME_VALUE), bMsg);
	if(bNonExistSmallForce)      return CheckError(_LS(IDS_DB_MULTI_LINEAR_SAME_FORCE), bMsg); 
	if(bNonExistSmallForceFinal) return CheckError(_LS(IDS_DB_MULTI_LINEAR_SAME_FORCE_FINAL), bMsg); 
	if(bExistDiffSign)           return CheckError(_LS(IDS_DB_MULTI_LINEAR_SAME_SIGN), bMsg);

	// Deformation Capacity
	for(int i=0; i<2; i++) for(int j=0; j<5; j++)
	{
		if(fabs(m_edtDeformCapacity[i][j].GetEditValue())<0.) return CheckError(_LS(IDS_DB_DEFORMATION_CAPACITY), bMsg);
		if(j>0) if(fabs(m_edtDeformCapacity[i][j-1].GetEditValue()) >= fabs(m_edtDeformCapacity[i][j].GetEditValue())) 
			return CheckError(_LS(IDS_DB_DEFORMATION_CAPACITY_COM), bMsg);
	}

	double dSFactDispl = m_edtSFactDispl.GetEditValue();
	if(dSFactDispl==0.0) return CheckError(_LS(IDS_DB_MULTI_LINEAR_CHK_SCALE_FACTOR), bMsg);

	if (m_nModelType == D_IEHP_MLEL || m_nModelType == D_IEHP_MLPI) //Elastic,  Plastic Pivot
	{
		if (bMinus02 || bPlus02)
		{
			// "The force-displacement curve does not allow Negative slope other than the final value."	
			return CheckError(_LS(IDS_DB_MULTI_LINEAR_CHK_Negative_Slope_Type01), bMsg);
		}
	}
	else if (m_nModelType == D_IEHP_MLKI || m_nModelType == D_IEHP_MLTA) // Kinematic이거나 Takeda 이면
	{
		if (bMinus01 || bMinus02 || bPlus01 || bPlus02)
		{
			// "The force-displacement curve does not allow Negative slope."	
			return CheckError(_LS(IDS_DB_MULTI_LINEAR_CHK_Negative_Slope_Type02), bMsg);
		}
	}

	return TRUE;
#undef CheckError
}

BOOL CIehpProp_MLinDlg::Dlg2Data()
{
	if(!CheckData()) return FALSE;
	UpdateData(TRUE);

	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	m_Data.AllProp.PROP[nD].MULTLIN.dInitStiffP = m_edtPlusStiff.GetEditValue();
	m_Data.AllProp.PROP[nD].MULTLIN.dInitStiffN = m_edtMnusStiff.GetEditValue();
	
	m_Data.AllProp.PROP[nD].MULTLIN.nType = m_nType;
	m_Data.AllProp.PROP[nD].MULTLIN.nDeformDefineType = m_nDeformDefineType;
	for(int i=0; i<2; i++)
	{
		for(int j=0; j<5; j++)
		{
			m_Data.AllProp.PROP[nD].MULTLIN.dDeformCapacity[i][j] = m_edtDeformCapacity[i][j].GetEditValue();
		}
	}
	
	if(m_nModelType == D_IEHP_MLPI) // Pivot Type 이면
	{
		m_Data.AllProp.PROP[nD].MULTLIN.dHysParam_Alpha1 = m_edtAlpha1.GetEditValue();
		m_Data.AllProp.PROP[nD].MULTLIN.dHysParam_Alpha2 = m_edtAlpha2.GetEditValue();
		m_Data.AllProp.PROP[nD].MULTLIN.dHysParam_Beta1  = m_edtBeta1.GetEditValue();
		m_Data.AllProp.PROP[nD].MULTLIN.dHysParam_Beta2  = m_edtBeta2.GetEditValue();
		m_Data.AllProp.PROP[nD].MULTLIN.dHysParam_Eta    = m_edtEta.GetEditValue();
	}
	else if(m_nModelType == D_IEHP_MLTA) // Plastic Takeda
	{
		m_Data.AllProp.PROP[nD].MULTLIN.dHysParam_Beta1  = m_edtUnBeta.GetEditValue();
	}

	m_Data.AllProp.PROP[nD].MULTLIN.dScaleF_Displ = m_edtSFactDispl.GetEditValue();
	m_Data.AllProp.PROP[nD].MULTLIN.dScaleF_Force = m_edtSFactForce.GetEditValue();

	return TRUE;
}
void CIehpProp_MLinDlg::SetUnitAndTitle()
{  
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	if(nD < 3) // None & Dx, Dy, Dz
	{
		if(m_nHingeType == 0) // Lumped
		{
			if(m_nDof == 0)
			{
				m_edtPlusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
				m_edtMnusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
				m_untPlusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
				m_untMnusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
			}
			else
			{
				m_edtPlusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffForce);
				m_edtMnusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffForce);
				m_untPlusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffForce);
				m_untMnusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffForce);
			}
		}
		else if(m_nHingeType == 1) // Distributed
		{
			m_edtPlusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffForce);
			m_edtMnusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffForce);
			m_untPlusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffForce);
			m_untMnusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffForce);
		}
		else if(m_nHingeType == 2 || m_nHingeType == 3) // Spring, Truss
		{
			m_edtPlusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
			m_edtMnusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
			m_untPlusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
			m_untMnusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffUnitForce);
		}
	}
	else if(nD < 6)
	{
		if(m_nHingeType == 0) // Lumped
		{
			m_edtPlusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffMoment);
			m_edtMnusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffMoment);
			m_untPlusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffMoment);
			m_untMnusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffMoment);
		}
		else if(m_nHingeType == 1) // Distributed
		{
			m_edtPlusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffDistStiff);
			m_edtMnusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffDistStiff);
			m_untPlusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffDistStiff);
			m_untMnusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffDistStiff);
		}
		else if(m_nHingeType == 2) // Distributed
		{
			m_edtPlusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffRoSpring);
			m_edtMnusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffRoSpring);
			m_untPlusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffRoSpring);
			m_untMnusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffRoSpring);
		}
		else if(m_nHingeType == 3) // Truss
		{
			m_edtPlusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffDistStiff);
			m_edtMnusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffDistStiff);
			m_untPlusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffDistStiff);
			m_untMnusStiff.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dInitStiffDistStiff);
		}
	}
	for(int i=0; i<2; i++)
	{
		for(int j=0; j<5; j++)
		{
			m_edtDeformCapacity[i][j].SetUnitType(D_UNITSYS_NONE);
		}
	}

	m_edtAlpha1.SetUnitType(D_UNITSYS_NONE);
	m_edtAlpha2.SetUnitType(D_UNITSYS_NONE);
	m_edtBeta1.SetUnitType(D_UNITSYS_NONE);
	m_edtBeta2.SetUnitType(D_UNITSYS_NONE);
	m_edtEta.SetUnitType(D_UNITSYS_NONE);
	m_edtUnBeta.SetUnitType(D_UNITSYS_NONE);

	// Set Title
	CString str; str.Empty();
	this->GetWindowText(str);

	if     (m_nModelType == D_IEHP_MLEL) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_MULTI_ELASTIC));             // Multi-Linear Elastic
	else if(m_nModelType == D_IEHP_MLKI) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_MULTI_PLASTIC_KINEMATIC));   // Multi-Linear Plastic Kinematic
	else if(m_nModelType == D_IEHP_MLTA) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_MULTI_PLASTIC_TAKEDA));      // Multi-Linear Plastic Takeda
	else if(m_nModelType == D_IEHP_MLPI) this->SetWindowText(str + _T(" : ") + _LS(IDS_CMD_IEHP_MULTI_PLASTIC_PIVOT));       // Multi-Linear Plastic Pivot
	else ASSERT(0);

}

void CIehpProp_MLinDlg::AlignCtrl()
{
	if(m_nModelType == D_IEHP_MLEL || m_nModelType == D_IEHP_MLKI) // Elastic, Plastic Kinematic
	{
		CRect rRef, rToMove;
		int nDistY;

		GetDlgItem(IDC_WG_CMD_STATIC2)->GetWindowRect(rRef);
		GetDlgItem(IDOK)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, m_aOK, nDistY);
		CDlgUtil::CtrlShowHide(this, m_aHysParam, FALSE);

		// resize self size
		CRect rectLast;
		GetDlgItem(IDOK)->GetWindowRect(rectLast);
		CRect r;
		GetWindowRect(r);
		r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(10);
		MoveWindow(r);
	}
	else if(m_nModelType == D_IEHP_MLPI) // Plastic Pivot
	{
		CRect rRef, rToMove;
		int nDistY;
		
		GetDlgItem(IDC_IEHP_HYST_PARAM_STC)->GetWindowRect(rRef);
		GetDlgItem(IDOK)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, m_aOK, nDistY);
		CDlgUtil::CtrlShowHide(this, m_aUnParam, FALSE);
		
		// resize self size
		CRect rectLast;
		GetDlgItem(IDOK)->GetWindowRect(rectLast);
		CRect r;
		GetWindowRect(r);
		r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(10);
		MoveWindow(r);
	}
	else if(m_nModelType == D_IEHP_MLTA) // Plastic Takeda
	{
		CRect rRef, rToMove;
		int nDistY;
		
		GetDlgItem(IDC_IEHP_HYST_PARAM_STC)->GetWindowRect(rRef);
		GetDlgItem(IDC_IEHP_UNLODING_PARAM_STC)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aUnParam, nDistY);
		CDlgUtil::CtrlShowHide(this, m_aHysParam, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aUnParam, TRUE);
		
		GetDlgItem(IDC_IEHP_UNLODING_PARAM_STC)->GetWindowRect(rRef);
		GetDlgItem(IDOK)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, m_aOK, nDistY);
		
		// resize self size
		CRect rectLast;
		GetDlgItem(IDOK)->GetWindowRect(rectLast);
		CRect r;
		GetWindowRect(r);
		r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(10);
		MoveWindow(r);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CIehpProp_MLinDlg message handlers

BOOL CIehpProp_MLinDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	

	InitGrid();
	InitGraph();
	SetUnitAndTitle();
	AlignCtrl();
	Data2Dlg();
	UpdateGraph();
	UpdateInitStiff();
	CtrlEnableDisable();	
	
	if( m_nModelType != D_IEHP_MLKI && m_nModelType != D_IEHP_MLTA )
	{
		CDlgUtil::CtrlEnableDisable(this, IDC_IEHP_TYPE_RDO_0, FALSE);
		CDlgUtil::CtrlEnableDisable(this, IDC_IEHP_TYPE_RDO_1, FALSE);
		CDlgUtil::CtrlEnableDisable(this, IDC_IEHP_TYPE_RDO_2, FALSE);
	}

	m_wndGrid.MakeItemEx();

	if(m_nModelType == D_IEHP_MLPI) CreatePivotPic();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CIehpProp_MLinDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// 변경할 에디트컨트롤을 찾는다.
	if(pWnd->GetDlgCtrlID() == IDC_IEHP_STIFF_PLUS_EDT || pWnd->GetDlgCtrlID() == IDC_IEHP_STIFF_MINU_EDT )
	{
		// 텍스트 색상을 설정한다.
		pDC->SetTextColor(RGB(0, 0, 0));
		// 텍스트의 배경색상을 설정한다.
//    pDC->SetBkColor(RGB(65, 120, 180));
	}
	return hbr;

}
void CIehpProp_MLinDlg::OnOK() 
{
	if(!Dlg2Data()) return;
	CDialogMove::OnOK();
}

void CIehpProp_MLinDlg::OnDataChanged()
{
	UpdateGraph();
	UpdateInitStiff();
}

bool CIehpProp_MLinDlg::SyncDialogData()
{
	if (!Dlg2Data()) return false;

	return true;
}


void CIehpProp_MLinDlg::OnSelectTypeRdo()
{ 
	CtrlEnableDisable();
}

void CIehpProp_MLinDlg::CtrlEnableDisable()
{
	int nType=0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aTypeRdo, nType);
	
	switch( nType )
	{
		case 0: // Both
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlPlus, TRUE);
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlMinus, TRUE);
			break;
		case 1:	// Tens. Only
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlPlus, TRUE);
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlMinus, FALSE);
			break;
		case 2:	// Comp. Only
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlPlus, FALSE);
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlMinus, TRUE);
			break;
		default:
			ASSERT(0);
	}
}

void CIehpProp_MLinDlg::OnChangeDefomCapacity() 
{
	for(int i=0; i<5; i++)
	{
		m_edtDeformCapacity[1][i].SetEditUnit(m_edtDeformCapacity[0][i].GetEditValue());
	}
}

void CIehpProp_MLinDlg::InitGrid()
{
	T_UNIT_SYSTEM UnitSystem;
	m_wndGrid.Initialize(&m_MultiData, UnitSystem, m_nDof, m_nHingeType);
	
	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	m_wndGrid.SetScrollBarMode(SB_HORZ, gxnEnabled | gxnEnhanced);
//   m_bGridInit = TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
// Graph

void CIehpProp_MLinDlg::InitGraph()
{
	CWnd* pGraph=GetDlgItem(IDC_PICTURE);
	ASSERT(pGraph);
	
	CRect rect;
	pGraph->GetClientRect(&rect);
	m_Draw.Create(pGraph, rect, NULL);
	m_Draw.FitToParent(rect);
}

void CIehpProp_MLinDlg::UpdateInitStiff()
{
	int nCount = m_MultiData.arMultiLinearData.GetSize();

	double dPlusStiff = 0.0;
	double dMnusStiff = 0.0;
	double dDisp = 0.0, dForce=0.0;
	for (int i=0; i<nCount; i++)
	{
		dDisp  = m_MultiData.arMultiLinearData[i].dblDisp;
		dForce = m_MultiData.arMultiLinearData[i].dblForce;
		if(CCompFunc::CompRealTol(dDisp, 0.0, m_dZero) == 0)
		{
			// (+) 혹은 (-)항만 입력된 경우
			if(i==0 || i==nCount-1)
			{
				if(i==0 && i+1 < nCount)
				{
					dDisp  = m_MultiData.arMultiLinearData[i+1].dblDisp;
					dForce = m_MultiData.arMultiLinearData[i+1].dblForce;
				}
				else if(i==nCount-1 && i > 0)
				{
					dDisp  = m_MultiData.arMultiLinearData[i-1].dblDisp;
					dForce = m_MultiData.arMultiLinearData[i-1].dblForce;
				}
				else
				{
					dForce = 0.0;
					dDisp  = 1.0;
				}
				dPlusStiff = dForce/dDisp;
				dMnusStiff = dPlusStiff;
			}
			// (+), (-)항 모두 입력된 경우
			else
			{
				dDisp  = m_MultiData.arMultiLinearData[i+1].dblDisp;
				dForce = m_MultiData.arMultiLinearData[i+1].dblForce;
				dPlusStiff = dForce/dDisp;

				dDisp  = m_MultiData.arMultiLinearData[i-1].dblDisp;
				dForce = m_MultiData.arMultiLinearData[i-1].dblForce;
				dMnusStiff = dForce/dDisp;
			}
		}
	}

	//double dSFact = m_edtSFactDispl.GetEditValue() / m_edtSFactForce.GetEditValue();
	double dSFact = m_edtSFactForce.GetEditValue() / m_edtSFactDispl.GetEditValue();

	m_edtPlusStiff.SetEditUnit(dPlusStiff * dSFact);
	m_edtMnusStiff.SetEditUnit(dMnusStiff * dSFact);
}
void CIehpProp_MLinDlg::UpdateGraph()
{
	// 그리드의 값에 Sclae Factor를 곱하여 최종값을 계산
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;
	T_IEHP_DT aDataRescaled;
	aDataRescaled.RemoveAll();
	for(int i=0; i<m_MultiData.arMultiLinearData.GetSize(); i++)
	{
		T_IEHP_BASE Data;
		Data.dblDisp  = m_MultiData.arMultiLinearData[i].dblDisp  * m_Data.AllProp.PROP[nD].MULTLIN.dScaleF_Displ;
		Data.dblForce = m_MultiData.arMultiLinearData[i].dblForce * m_Data.AllProp.PROP[nD].MULTLIN.dScaleF_Force;
		aDataRescaled.Add(Data);
	}

	m_Draw.SetDefaultData(20, aDataRescaled);
	m_Draw.DrawMultiLinearGraph();
}

void CIehpProp_MLinDlg::CreatePivotPic()
{
	if(m_pPivotPic==NULL)
	{
		m_pPivotPic = new CIehpProp_PivotPicDlg();
		m_pPivotPic->SetInitPos(D_INIT_POS_LT);

		m_pPivotPic->Create(IDD_CMD_MULTI_PIVOT_PIC_DLG, this);
		m_pPivotPic->ShowWindow(SW_SHOW);
	}
	else
	{
		ASSERT(0);
	}
}


BOOL CIehpProp_MLinDlg::DestroyWindow()
{
	// Window가 있으면 없앤다.
	if(m_pPivotPic != NULL)
	{
		m_pPivotPic->DestroyWindow();  
		m_pPivotPic = NULL;    
	}
	return CDialog::DestroyWindow();
}

void CIehpProp_MLinDlg::OnScaleFactorChanged() 
{
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;
	m_Data.AllProp.PROP[nD].MULTLIN.dScaleF_Displ = m_edtSFactDispl.GetEditValue();
	m_Data.AllProp.PROP[nD].MULTLIN.dScaleF_Force = m_edtSFactForce.GetEditValue();
	UpdateInitStiff();
}