// SoilPropCalcKhDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SoilPropCalcKhDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\wg_db_MathFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSoilPropCalcKhDlg dialog

#define D_TOLERANCE		1.e-8
#define KH_ARRAY_RANGE 7

double const CSoilPropCalcKhDlg::s_dKh[KH_ARRAY_RANGE][3] =
{
	//(kn/m^3)
	// G.L ~ H/3 | H/3 ~ 2H/3 | 2H/3 ~ Bedrock|		Vs(m/s)  
	{	  4082,		  5695,			  8770},	//100
	{	 16360,		 22725,			 34997},	//200
	{	 36809,		 51130,			 78743},	//300
	{	 69074,		 95948,			147764},	//400
	{	107929,		149919,			230881},	//500
	{	155417,		215883,			332469},	//600
	{	222673,		309307,			476345}		//700
};

CSoilPropCalcKhDlg::CSoilPropCalcKhDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSoilPropCalcKhDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nUseVsType = 1;
	m_nLayerType = 0;
}

CSoilPropCalcKhDlg::~CSoilPropCalcKhDlg()
{
}

BOOL CSoilPropCalcKhDlg::Data2Dlg()
{
	CString strTemp;
	strTemp.Format(_T("%g"), m_Data.dGroundLevel);
	m_editSecondLayer.SetWindowText(strTemp);

	UpdateData(FALSE);
	return TRUE;
}

BOOL CSoilPropCalcKhDlg::Dlg2Data()
{
	UpdateData();
	if (m_nUseVsType && m_nLayerType)
	{
		double dSecondLayer		= m_editSecondLayer.GetEditValue();

		if (dSecondLayer < m_Data.dBedrockLevel || dSecondLayer > m_Data.dGroundLevel)
		{
			AfxMessageBox(_LS(IDS_CMD_SOIL_PROP_ERR_2ND_LAYER_RANGE));
			return FALSE;
		}
	}

	return TRUE;
}

void CSoilPropCalcKhDlg::InitCtrl()
{
	OnClickType();
}

//(kn/m^3)
// G.L ~ H/3 | H/3 ~ 2H/3 | 2H/3 ~ Bedrock|		Vs(m/s) 
/************************************************************************/
/*(m/s)|									(kN/m^3)					*/
/* Vs  |G.L ~ H/3  | H/3 ~ 2H/3	   | 2H/3 ~ Bedrock|                    */
/* 100 |     4082  |    5695	   |       8770    |                    */
/* 200 |    16360  |   22725	   |      34997    |                    */
/* 300 |    36809  |   51130	   |      78743    |                    */
/* 400 |    69074  |   95948	   |     147764    |                    */
/* 500 |   107929  |  149919	   |     230881    |                    */
/* 600 |   155417  |  215883	   |     332469    |                    */
/* 700 |   222673  |  309307	   |     476345    |                    */
/************************************************************************/

double CSoilPropCalcKhDlg::GetKh(const EN_SOIL_SECTION eSection, const double dVs)
{
	int aVs[KH_ARRAY_RANGE] = { 100,200,300,400,500,600,700 };
	for (int i = 0; i < KH_ARRAY_RANGE; i++)	aVs[i] = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_VELOCITY, aVs[i]);

	if (dVs - aVs[0] < D_TOLERANCE)	
		return s_dKh[0][eSection];
	if (aVs[KH_ARRAY_RANGE - 1] - dVs < D_TOLERANCE)
		return s_dKh[KH_ARRAY_RANGE - 1][eSection];

	int nVsIndex = dVs / aVs[0] - 1;

	if (nVsIndex < 0 || nVsIndex > KH_ARRAY_RANGE - 1) { ASSERT(0); return 0.; }

	double dRatio = (dVs - aVs[nVsIndex]) / (aVs[nVsIndex + 1] - aVs[nVsIndex]);

	if (fabs(dRatio) < D_TOLERANCE)
		return s_dKh[nVsIndex][eSection];

	double dStartKh = s_dKh[nVsIndex][eSection];
	double dEndKh = s_dKh[nVsIndex + 1][eSection];

	return CMathFunc::mathRoundOff(dStartKh + ((dEndKh - dStartKh) * dRatio), 1);
}

double CSoilPropCalcKhDlg::GetKh_KDS17(const EN_SOIL_SECTION eSection, const T_POSP_BASE_DATA Base)
{
	double dVsi     = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_VELOCITY, Base.dVs);
	double dDensity = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_DENSITY, Base.dDensity);
	double dPoisson = Base.dPoissonRatio;  // Base.dPoisson

	double dC = (dVsi < 360.0) ? 0.8 : 1.0;
	double dVds = dC * dVsi;

	double dGravity = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStypGrav();
	double dg       = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_GRAVITY, dGravity);
	double dGd = dDensity / dg * pow(dVds, 2.0); // 전단탄성계수
	double dEd = 2.0*(1.0 + dPoisson)*dGd;       // 탄성계수

	double dB_bed = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, m_Data.dStructureWidth);
	double dL_bed = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, m_Data.dStructureHeight);
	double dArea = dB_bed * dL_bed;
	double dBh = sqrt(dArea);

	double dAlpha = 1.0; // 고정
	double dKh0 = (1.0/0.3) * dAlpha * dEd;
	double dKH = dKh0 * pow(dBh/0.3, -(3./4.));

	return dKH;
}

double CSoilPropCalcKhDlg::CalcSingleLayer(const double dTotalLevel)
{
	double dAverageVs = 0.;

	double dHeight = 0., dVs = 0.;
	double dTotalSoilHeight = 0., dCurrentLevel = 0., dTmpVs = 0.;

	for (int i = 0; i < m_Data.aSoilParam.GetCount(); i++)
	{
		dHeight = m_Data.aSoilParam[i].dHeight;
		dTotalSoilHeight += dHeight;
		dCurrentLevel -= dHeight;
		dVs = m_Data.aSoilParam[i].dVs;

		//Bedrock 보다 더 깊은 경우 계산을 하지않음
		if (dCurrentLevel < dTotalLevel)
		{
			dTmpVs += (dHeight - abs(dCurrentLevel - dTotalLevel)) / dVs;
			dTotalSoilHeight -= abs(dCurrentLevel - dTotalLevel);

			break;
		}
		dTmpVs += dHeight / dVs;
	}
	dAverageVs = dTotalSoilHeight / dTmpVs;

	return dAverageVs;
}

void CSoilPropCalcKhDlg::CalcDoubleLayer(const double dTotalLevel, const double d2ndLayerPos, double dAverageVs[])
{
	double dHeight = 0., dVs = 0.;
	double dTotalSoilHeight = 0., dCurrentLevel = 0., dTmpVs=0.;
	int idxSecondLayer = 0;
	double dZero = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(0, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 0.0001);

	for (int i = 0; i < m_Data.aSoilParam.GetCount(); i++)
	{
		dHeight = m_Data.aSoilParam[i].dHeight;
		dTotalSoilHeight += dHeight;
		dCurrentLevel -= dHeight;
		dVs = m_Data.aSoilParam[i].dVs;
		//Bedrock 보다 더 깊은 경우 계산을 하지않음
		if (dCurrentLevel < dTotalLevel)
		{
			dTmpVs += (dHeight - abs(dCurrentLevel - dTotalLevel)) / dVs;
			dTotalSoilHeight -= abs(dCurrentLevel - dTotalLevel);

			break;
		}

		//Second Layer보다 깊은 경우
		if (idxSecondLayer == 0 && dCurrentLevel < d2ndLayerPos + dZero)
		{
			dTmpVs				+= (dHeight - abs(dCurrentLevel - d2ndLayerPos)) / dVs;
			dTotalSoilHeight	-= abs(dCurrentLevel - d2ndLayerPos);
			if (dTmpVs > 0)
				dAverageVs[idxSecondLayer] = dTotalSoilHeight / dTmpVs;

			//나머지 부분 계산
			dTotalSoilHeight = dTmpVs = 0.;
			dTmpVs				+= abs(dCurrentLevel - d2ndLayerPos) / dVs;
			dTotalSoilHeight	+= abs(dCurrentLevel - d2ndLayerPos);

			idxSecondLayer++;
		}
		else
			dTmpVs += dHeight / dVs;
	}
	if(dTmpVs > 0)
		dAverageVs[idxSecondLayer] = dTotalSoilHeight / dTmpVs;
}

void CSoilPropCalcKhDlg::CalcKh()
{
	EN_SOIL_SECTION eSection = EN_ONE_THARID;
	double const dTotalLevel = m_Data.dBedrockLevel - m_Data.dGroundLevel;
	double const dOneThirdLevel = dTotalLevel * 1 / 3;
	double const dTwoThirdLevel = dTotalLevel * 2 / 3;
	double dCurrentLevel = 0., dVs = 0.;

	//Average
	if (m_nUseVsType)
	{
		double dHeight = 0.;

		//Double
		if (m_nLayerType)
		{
			double dAverageVs[2] = { 0. };
			
			// 그라운드 레벨에서 자기의 상대적인 위치를 가르킴, Vs를 계산할 때는 Ground level의 상대적인 위치로 계산하기 때문에...
			double const dSecondLayer = m_editSecondLayer.GetEditValue();
			double const d2ndLayerPos = dSecondLayer - m_Data.dGroundLevel;	
			CalcDoubleLayer(dTotalLevel, d2ndLayerPos, dAverageVs);

			int idxSecondLayer = 0;
			for (int i = 0; i < m_Data.aSoilParam.GetCount(); i++)
			{
				T_POSP_BASE_DATA &Base = m_Data.aSoilParam[i];
				dCurrentLevel -= Base.dHeight;
				eSection = GetSection(dOneThirdLevel, dTwoThirdLevel, dCurrentLevel);

				if (dCurrentLevel > d2ndLayerPos-Base.dHeight) // GEN-2615 수정: 2ndLayer가 Level 사이의 값일 경우에도 계산이 되어야 하기 때문.
					idxSecondLayer = 0;
				else
					idxSecondLayer = 1;

				Base.dKh = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_DENSITY, GetKh(eSection, dAverageVs[idxSecondLayer]));
			}
		}
		else //Single
		{
			double dAverageVs = CalcSingleLayer(dTotalLevel);

			for (int i = 0; i < m_Data.aSoilParam.GetCount(); i++)
			{
				T_POSP_BASE_DATA &Base = m_Data.aSoilParam[i];
				dCurrentLevel -= Base.dHeight;
				eSection = GetSection(dOneThirdLevel, dTwoThirdLevel, dCurrentLevel);
				Base.dKh = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_DENSITY, GetKh(eSection, dAverageVs));
			}
		}
	}
	else //by Level
	{
		for (int i = 0; i < m_Data.aSoilParam.GetCount(); i++)
		{
			T_POSP_BASE_DATA &Base = m_Data.aSoilParam[i];
			dCurrentLevel -= Base.dHeight;
			eSection = GetSection(dOneThirdLevel, dTwoThirdLevel, dCurrentLevel);
			dVs = Base.dVs;
			Base.dKh = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_DENSITY, GetKh(eSection, dVs));
		}
	}
}

void CSoilPropCalcKhDlg::CalcKh_KDS17()
{
	EN_SOIL_SECTION eSection = EN_ONE_THARID;
	double const dTotalLevel = m_Data.dBedrockLevel - m_Data.dGroundLevel;
	double const dOneThirdLevel = dTotalLevel * 1 / 3;
	double const dTwoThirdLevel = dTotalLevel * 2 / 3;
	double dCurrentLevel = 0., dVs = 0.;

	//Average
	if (m_nUseVsType)
	{
		double dHeight = 0.;

		//Double
		if (m_nLayerType)
		{
			double dAverageVs[2] = { 0. };

			// 그라운드 레벨에서 자기의 상대적인 위치를 가르킴, Vs를 계산할 때는 Ground level의 상대적인 위치로 계산하기 때문에...
			double const dSecondLayer = m_editSecondLayer.GetEditValue();
			double const d2ndLayerPos = dSecondLayer - m_Data.dGroundLevel;
			CalcDoubleLayer(dTotalLevel, d2ndLayerPos, dAverageVs);

			int idxSecondLayer = 0;
			for (int i = 0; i < m_Data.aSoilParam.GetCount(); i++)
			{
				T_POSP_BASE_DATA &Base = m_Data.aSoilParam[i];
				dCurrentLevel -= Base.dHeight;
				eSection = GetSection(dOneThirdLevel, dTwoThirdLevel, dCurrentLevel);

				if (dCurrentLevel > d2ndLayerPos - Base.dHeight) // GEN-2615 수정: 2ndLayer가 Level 사이의 값일 경우에도 계산이 되어야 하기 때문.
					idxSecondLayer = 0;
				else
					idxSecondLayer = 1;

				Base.dVs = dAverageVs[idxSecondLayer];
				Base.dKh = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_DENSITY, GetKh_KDS17(eSection, Base));
			}
		}
		else //Single
		{
			double dAverageVs = CalcSingleLayer(dTotalLevel);

			for (int i = 0; i < m_Data.aSoilParam.GetCount(); i++)
			{
				T_POSP_BASE_DATA &Base = m_Data.aSoilParam[i];
				dCurrentLevel -= Base.dHeight;
				eSection = GetSection(dOneThirdLevel, dTwoThirdLevel, dCurrentLevel);

				Base.dVs = dAverageVs;
				Base.dKh = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_DENSITY, GetKh_KDS17(eSection, Base));
			}
		}
	}
	else //by Level
	{
		for (int i = 0; i < m_Data.aSoilParam.GetCount(); i++)
		{
			T_POSP_BASE_DATA &Base = m_Data.aSoilParam[i];
			dCurrentLevel -= Base.dHeight;
			eSection = GetSection(dOneThirdLevel, dTwoThirdLevel, dCurrentLevel);
			
			Base.dKh = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_DENSITY, GetKh_KDS17(eSection, Base));
		}
	}
}

EN_SOIL_SECTION CSoilPropCalcKhDlg::GetSection(const double dOneThirdLevel, const double dTwoThirdLevel, const double dCurrentLevel)
{
	if (dCurrentLevel >= dOneThirdLevel)
		return EN_ONE_THARID;
	else if (dCurrentLevel >= dTwoThirdLevel)
		return EN_TWO_THRID;
	else
		return EN_BEDROCK;
}

void CSoilPropCalcKhDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_ETC_USE_LEVEL_RDO, m_nUseVsType);
	DDX_Radio(pDX, IDC_ETC_USE_AVERAGE_SINGLE_RDO, m_nLayerType);
	DDX_Control(pDX, IDC_ETC_2ND_LAYER_EDIT, m_editSecondLayer);
	DDX_Control(pDX, IDC_ETC_2ND_LAYER_UNIT, m_wndSecondLayerUnit);
}


BEGIN_MESSAGE_MAP(CSoilPropCalcKhDlg, CDialogMove)
	ON_BN_CLICKED(IDC_ETC_USE_LEVEL_RDO, OnClickType)
	ON_BN_CLICKED(IDC_ETC_USE_AVERAGE_RDO, OnClickType)
	ON_BN_CLICKED(IDC_ETC_USE_AVERAGE_SINGLE_RDO, OnLayerType)
	ON_BN_CLICKED(IDC_ETC_USE_AVERAGE_DOUBLE_RDO, OnLayerType)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoilPropCalcKhDlg message handlers

BOOL CSoilPropCalcKhDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	m_editSecondLayer.SetUnitType(D_UNITSYS_BASE_LENGTH);	m_wndSecondLayerUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	Data2Dlg();
	InitCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CSoilPropCalcKhDlg::OnClickType()
{
	UpdateData();

	CArray<UINT, UINT> aCtrl;
	aCtrl.RemoveAll();
	aCtrl.Add(IDC_ETC_USE_AVERAGE_SINGLE_RDO);
	aCtrl.Add(IDC_ETC_USE_AVERAGE_DOUBLE_RDO);
	CDlgUtil::CtrlEnableDisable(this, aCtrl, m_nUseVsType);

	OnLayerType();
}

void CSoilPropCalcKhDlg::OnLayerType()
{
	UpdateData();
	BOOL bEnbleSecondLayer = m_nUseVsType && m_nLayerType;

	CArray<UINT, UINT> aCtrl;
	aCtrl.RemoveAll();
	aCtrl.Add(IDC_ETC_2ND_LAYER_EDIT);
	CDlgUtil::CtrlEnableDisable(this, aCtrl, bEnbleSecondLayer);
}

void CSoilPropCalcKhDlg::OnOK()
{
	if (!Dlg2Data()) return;

#if defined(_MGEN_ORG)
	CalcKh();
#elif defined(_CIVIL_ORG)
	CalcKh_KDS17();
#endif

	CDialogMove::OnOK();
}

void CSoilPropCalcKhDlg::OnCancel()
{
	CDialogMove::OnCancel();
}
