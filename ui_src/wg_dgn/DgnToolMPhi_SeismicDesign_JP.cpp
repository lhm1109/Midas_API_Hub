#include "stdafx.h"
#include "DgnToolMPhi_SeismicDesign_JP.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SECT.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\DbToolMPhi_JP.h"
#include "..\wg_db\DBLib.h"
#include "..\MIT_lib\MMath.h"

#include "DgnProgressDlg.h"
#include "BrdgSeisEval_JP.h"

typedef CArray<_DGN_MPHI_STRN_D, _DGN_MPHI_STRN_D&> MPhiArray;

CDgnToolMPhi_SeismicDesign_JP::CDgnToolMPhi_SeismicDesign_JP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
}

CDgnToolMPhi_SeismicDesign_JP::~CDgnToolMPhi_SeismicDesign_JP()
{
}

BOOL CDgnToolMPhi_SeismicDesign_JP::GetLp(double d_h, const ColumnData* pColumn, OUT double& dLp)
{
	const T_ELEM_K& kElem = pColumn->kElem;
	const int& nPos = pColumn->ePosNodeLow;
	const int& nDirShear = pColumn->Column.nDirShear;

	T_UNIT_INDEX tLpUnit, tCurrentUnit;
	tLpUnit.nBase_Force = D_UNITSYS_FORCE_INDEX_N;		// N.
	tLpUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(tCurrentUnit);

	double dLenToMM = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(tLpUnit.nBase_Force, tLpUnit.nBase_Length,
		tCurrentUnit.nBase_Force, tCurrentUnit.nBase_Length, D_UNITSYS_BASE_LENGTH, 1.0);
	
	// Get_InputData() 함수가 mm, N 단위계로 변경하여 데이터를 가져와 계산해서 d_h 값도 mm 단위로 변경해 줘야 한다.
	d_h *= dLenToMM;

	MPHI_DATA_ELEM InData;
	if(Get_InputData(kElem, nPos, InData, TRUE, TRUE)) return FALSE;
	dLp = Calc_Lp_st(nDirShear, InData);

	// 내진조사 일때만 User로 부터 입력 받은 LP 제한값을 적용하지 않을 경우
	if (InData.UserD.bHingeLim == FALSE) {
		// Lp≤0.15h 범위 안인지 확인
		dLp = min(dLp, 0.15 * d_h);
	}

	double dLenToCurLength = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(tCurrentUnit.nBase_Force, tCurrentUnit.nBase_Length,
		tLpUnit.nBase_Force, tLpUnit.nBase_Length, D_UNITSYS_BASE_LENGTH, 1.0);

	// Get_InputData() , Calc_Lp_st() 함수가 mm 단위계로 계산되었기 때문에 현재 단위계로 변경 한다.
	dLp *= dLenToCurLength;

	return TRUE;
}

BOOL CDgnToolMPhi_SeismicDesign_JP::CalcHoriDisp(IN ParamDelta_y& param, OUT double* aDeltaY, OUT double* aDeltaU)
{
	ParamDelta_y pm = param;
	pm.bMajorAxis = TRUE;
	if (!CalcDelta_y(pm, aDeltaY[0], aDeltaU[0]))
		return FALSE;
	pm.bMajorAxis = FALSE;
	if (!CalcDelta_y(pm, aDeltaY[1], aDeltaU[1]))
		return FALSE;
	return TRUE;
}

// CDgnToolMPhi_SeismicDesign::Calc_RCSinglePier 참고
BOOL CDgnToolMPhi_SeismicDesign_JP::CalcDelta_y(IN ParamDelta_y& param, OUT double& dDelta_y, OUT double& dDelta_u)
{
	p_dy = param;

	double d_h = 0;
	T_ELEM_K_LIST arElemKey;
	if (Get_hAndElemKeysOfShortColumn(d_h, arElemKey) == FALSE) return FALSE;
	if (!CDBLib::GetSortElem_Position(arElemKey)) return FALSE;
	
	int nElemSize = arElemKey.GetSize();
	if (nElemSize < 1) return FALSE;

	GetInputData(p_dy.InData);
	
	// assume M-Phi is calculated using the average section properties of I and J ends
	auto AssignMPhiValue = [](const MPhiProp& PropY, const MPhiProp& PropZ, double dpMom[][4], double dpPhi[][4])
	{
		BOOL bBilinear = std::isnan(PropY.dStrength_Y[MPhiProp::eDir::kPlus]);

		dpMom[0][0] = PropY.dStrength_C[MPhiProp::eDir::kPlus];
		dpMom[0][1] = PropY.dStrength_C[MPhiProp::eDir::kMinus];
		if (!bBilinear)
		{
			dpMom[1][0] = PropY.dStrength_Y[MPhiProp::eDir::kPlus];
			dpMom[1][1] = PropY.dStrength_Y[MPhiProp::eDir::kMinus];
		}
		else
		{
			dpMom[1][0] = PropY.dStrength_C[MPhiProp::eDir::kPlus];
			dpMom[1][1] = PropY.dStrength_C[MPhiProp::eDir::kMinus];
		}
		dpMom[2][0] = PropY.dStrength_U[MPhiProp::eDir::kPlus];
		dpMom[2][1] = PropY.dStrength_U[MPhiProp::eDir::kMinus];
		dpMom[3][0] = PropY.dStrength_U[MPhiProp::eDir::kPlus];
		dpMom[3][1] = PropY.dStrength_U[MPhiProp::eDir::kMinus];



		dpMom[0][2] = PropZ.dStrength_C[MPhiProp::eDir::kPlus];
		dpMom[0][3] = PropZ.dStrength_C[MPhiProp::eDir::kMinus];
		if (!bBilinear)
		{
			dpMom[1][2] = PropZ.dStrength_Y[MPhiProp::eDir::kPlus];
			dpMom[1][3] = PropZ.dStrength_Y[MPhiProp::eDir::kMinus];
		}
		else
		{
			dpMom[1][2] = PropZ.dStrength_C[MPhiProp::eDir::kPlus];
			dpMom[1][3] = PropZ.dStrength_C[MPhiProp::eDir::kMinus];
		}
		dpMom[2][2] = PropZ.dStrength_U[MPhiProp::eDir::kPlus];
		dpMom[2][3] = PropZ.dStrength_U[MPhiProp::eDir::kMinus];
		dpMom[3][2] = PropZ.dStrength_U[MPhiProp::eDir::kPlus];
		dpMom[3][3] = PropZ.dStrength_U[MPhiProp::eDir::kMinus];



		dpPhi[0][0] = PropY.dDisplace_C[MPhiProp::eDir::kPlus];
		dpPhi[0][1] = PropY.dDisplace_C[MPhiProp::eDir::kMinus];
		if (!bBilinear)
		{
			dpPhi[1][0] = PropY.dDisplace_Y[MPhiProp::eDir::kPlus];
			dpPhi[1][1] = PropY.dDisplace_Y[MPhiProp::eDir::kMinus];
		}
		else
		{
			dpPhi[1][0] = PropY.dDisplace_C[MPhiProp::eDir::kPlus];
			dpPhi[1][1] = PropY.dDisplace_C[MPhiProp::eDir::kMinus];
		}
		dpPhi[2][0] = PropY.dDisplace_U[MPhiProp::eDir::kPlus];
		dpPhi[2][1] = PropY.dDisplace_U[MPhiProp::eDir::kMinus];
		dpPhi[3][0] = PropY.dDisplace_U[MPhiProp::eDir::kPlus];
		dpPhi[3][1] = PropY.dDisplace_U[MPhiProp::eDir::kMinus];



		dpPhi[0][2] = PropZ.dDisplace_C[MPhiProp::eDir::kPlus];
		dpPhi[0][3] = PropZ.dDisplace_C[MPhiProp::eDir::kMinus];
		if (!bBilinear)
		{
			dpPhi[1][2] = PropZ.dDisplace_Y[MPhiProp::eDir::kPlus];
			dpPhi[1][3] = PropZ.dDisplace_Y[MPhiProp::eDir::kMinus];
		}
		else
		{
			dpPhi[1][2] = PropZ.dDisplace_C[MPhiProp::eDir::kPlus];
			dpPhi[1][3] = PropZ.dDisplace_C[MPhiProp::eDir::kMinus];
		}
		dpPhi[2][2] = PropZ.dDisplace_U[MPhiProp::eDir::kPlus];
		dpPhi[2][3] = PropZ.dDisplace_U[MPhiProp::eDir::kMinus];
		dpPhi[3][2] = PropZ.dDisplace_U[MPhiProp::eDir::kPlus];
		dpPhi[3][3] = PropZ.dDisplace_U[MPhiProp::eDir::kMinus];
	};

	auto CalculateMPhiValueForElemenetWithoutMPhi = [this](const T_ELEM_K ElemK, const QIehp::eCom component, MPhiProp& prop) -> BOOL
	{
		CDbToolMphi_JP Mphi;
		MPHI_DATA_ELEM InData;
		int iReturn = Mphi.Get_InputData(ElemK, POS_I, InData, FALSE, TRUE);
		if (iReturn) // iReturn!=0 means this element has invalid M-Phi property
			return FALSE;

		double dE = InData.MatlD.dEc;
		double dI = 0.;
		if (component == QIehp::eCom::kMy) {
			dI = InData.SectD.dIyy;
		}
		else if (component == QIehp::eCom::kMz) {
			dI = InData.SectD.dIzz;
		}
		else {
			ASSERT(FALSE);
		}

		// data retrieved from Mphi routine is in the N-mm unit system
		// so a conversion to N-m unit system is required
		m_CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
		m_CodeUnit.nBase_Force = D_UNITSYS_FORCE_INDEX_N;		// N.
		dE = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(m_CodeUnit.nBase_Force, m_CodeUnit.nBase_Length, D_UNITSYS_BASE_ELAST, dE);
		dI = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(m_CodeUnit.nBase_Force, m_CodeUnit.nBase_Length, D_UNITSYS_BASE_STIF, dI);

		if (MMath::IsZero(dE) || MMath::IsZero(dI))
			return FALSE;

		const double dMoment = 1.e10; // arbrary big moment;
		prop.dStrength_C[MPhiProp::eDir::kPlus] = prop.dStrength_C[MPhiProp::eDir::kMinus] = dMoment;
		prop.dStrength_Y[MPhiProp::eDir::kPlus] = prop.dStrength_Y[MPhiProp::eDir::kMinus] = 2. * dMoment;
		prop.dStrength_U[MPhiProp::eDir::kPlus] = prop.dStrength_U[MPhiProp::eDir::kMinus] = 3. * dMoment;

		prop.dDisplace_C[MPhiProp::eDir::kPlus] = prop.dDisplace_C[MPhiProp::eDir::kMinus] = dMoment / dE / dI;
		prop.dDisplace_Y[MPhiProp::eDir::kPlus] = prop.dDisplace_Y[MPhiProp::eDir::kMinus] = 2. * dMoment / dE / dI;
		prop.dDisplace_U[MPhiProp::eDir::kPlus] = prop.dDisplace_U[MPhiProp::eDir::kMinus] = 3. * dMoment / dE / dI;

		return TRUE;
	};

	// retrieve element's M-Phi values
	MPhiArray arMPhiStrn; arMPhiStrn.SetSize(nElemSize);
	const int nSize = arElemKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		T_ELEM_K ElemK = arElemKey.GetAt(i);
		MPhiProp prop_y; MPhiProp prop_z;
		if (!m_pDoc->m_pAttrCtrl2->GetQIehp()->GetHingeProp(ElemK, QIehp::eCom::kMy, QIehp::ePosI, prop_y))
			if (!CalculateMPhiValueForElemenetWithoutMPhi(ElemK, QIehp::eCom::kMy, prop_y))
				return FALSE;

		if (!m_pDoc->m_pAttrCtrl2->GetQIehp()->GetHingeProp(ElemK, QIehp::eCom::kMz, QIehp::ePosI, prop_z))
			if (!CalculateMPhiValueForElemenetWithoutMPhi(ElemK, QIehp::eCom::kMz, prop_z))
				return FALSE;

		_DGN_MPHI_STRN_D MPhiStrn;
		MPhiStrn.Initialize();
		AssignMPhiValue(prop_y, prop_z, MPhiStrn.dpMom_I, MPhiStrn.dpPhi_I);

		for (int ii = 0; ii < 4; ++ii) {
			for (int jj = 0; jj < 4; ++jj)
			{
				MPhiStrn.dpMom_J[ii][jj] = MPhiStrn.dpMom_M[ii][jj] = MPhiStrn.dpMom_I[ii][jj];
				MPhiStrn.dpPhi_J[ii][jj] = MPhiStrn.dpPhi_M[ii][jj] = MPhiStrn.dpPhi_I[ii][jj];
			}
		}
		arMPhiStrn.SetAt(i, MPhiStrn);
	}
	
	// M-Phi for bottom element
	// assume M-Phi is calculated using the average section properties of I and J ends
	double dpMom[4][4], dpPhi[4][4];
	for (int ii = 0; ii < 4; ++ii) {
		for (int jj = 0; jj < 4; ++jj)
		{
			dpMom[ii][jj] = arMPhiStrn[nSize - 1].dpMom_I[ii][jj];
			dpPhi[ii][jj] = arMPhiStrn[nSize - 1].dpPhi_I[ii][jj];
		}
	}

	int nCaseType = 0; //0:Major+, 1:Major-, 2:Minor+, 3:Minor-
	GetCaseType(nCaseType);
	
	auto CalcPhi = [this](const int nCaseType, int iPosi, double dMoment, const _DGN_MPHI_STRN_D& MPhiStrnD)
	{
		if (!MPhiStrnD.bUserCurve)
		{
			const int iAxisDir = nCaseType;

			double dpMom[4];
			double dpPhi[4];

			for (int i = 0; i < 4; i++)
			{
				if (iPosi == 0)
				{
					dpMom[i] = MPhiStrnD.dpMom_I[i][iAxisDir];
					dpPhi[i] = MPhiStrnD.dpPhi_I[i][iAxisDir];
				}
				else if (iPosi == 1)
				{
					dpMom[i] = MPhiStrnD.dpMom_M[i][iAxisDir];
					dpPhi[i] = MPhiStrnD.dpPhi_M[i][iAxisDir];
				}
				else
				{
					dpMom[i] = MPhiStrnD.dpMom_J[i][iAxisDir];
					dpPhi[i] = MPhiStrnD.dpPhi_J[i][iAxisDir];
				}
			}

			for (int i = 0; i < 4; i++)
			{
				const double dAbsMoment = fabs(dMoment);
				if (dAbsMoment < dpMom[i] || MMath::IsEqual(dAbsMoment, dpMom[i]))
				{
					if (i == 0) return dpPhi[i] / dpMom[i] * fabs(dMoment) * (dMoment < 0 ? -1.0 : 1.0);
					else     return (dpPhi[i - 1] + (dpPhi[i] - dpPhi[i - 1]) / (dpMom[i] - dpMom[i - 1]) * (fabs(dMoment) - dpMom[i - 1])) * (dMoment < 0 ? -1.0 : 1.0);
				}
			}
			// 종국시의 강도를 초과하는 경우 종국시의 값을 넘겨줌
			return dpPhi[3];
		}

		CArray<T_RSDL_MPHI_D, T_RSDL_MPHI_D> arUserMPhi;
		const bool bMajorAxis = nCaseType == 0 || nCaseType == 1 ? TRUE : FALSE;
		if (bMajorAxis) arUserMPhi.Copy(MPhiStrnD.arUserMPhi[0]);
		else           arUserMPhi.Copy(MPhiStrnD.arUserMPhi[1]);

		double dM_min, dM_max;
		double dPhi_min, dPhi_max;
		int nSize = arUserMPhi.GetSize();
		int i;
		for (i = 0; i < nSize; i++)
		{
			if (i == 0)
			{
				dM_min = dM_max = arUserMPhi[i].dMoment;
				dPhi_min = dPhi_max = arUserMPhi[i].dCurvature;
			}
			else
			{
				dM_min = min(dM_min, arUserMPhi[i].dMoment);
				dM_max = max(dM_max, arUserMPhi[i].dMoment);
				dPhi_min = min(dPhi_min, arUserMPhi[i].dCurvature);
				dPhi_max = max(dPhi_max, arUserMPhi[i].dCurvature);
			}
		}

		BOOL bSig = TRUE;
		double dMaxM = (iPosi == 0) ? MPhiStrnD.dpMom_I[3][dMoment > 0.0 ? 0 : 1] : MPhiStrnD.dpMom_J[3][dMoment > 0.0 ? 0 : 1];
		double dMaxPHI = (iPosi == 0) ? MPhiStrnD.dpPhi_I[3][dMoment > 0.0 ? 0 : 1] : MPhiStrnD.dpPhi_J[3][dMoment > 0.0 ? 0 : 1];

		if (dMoment < 0.0)
		{
			if (dM_min >= 0.0 && dPhi_min >= 0.0)
			{
				bSig = FALSE;  dMoment = fabs(dMoment);
			}
		}
		if (dMoment > dMaxM)
		{
			return (bSig ? 1.0 : -1.0) * dMaxPHI;
		}

		for (i = 0; i < nSize; i++)
		{
			if (dMoment < arUserMPhi[i].dMoment)
			{
				if (i == 0)
				{
					if (dM_min >= 0.0 && dPhi_min >= 0.0) return (bSig ? 1.0 : -1.0) * arUserMPhi[i].dCurvature / arUserMPhi[i].dMoment * dMoment;
					else                                 return dPhi_min;
				}
				else
				{
					double dx1 = arUserMPhi[i - 1].dCurvature;
					double dx2 = arUserMPhi[i].dCurvature;
					double dy1 = arUserMPhi[i - 1].dMoment;
					double dy2 = arUserMPhi[i].dMoment;
					double dy = dMoment;
					double dx = dx1 + (dy - dy1) * (dx2 - dx1) / (dy2 - dy1);

					return (bSig ? 1.0 : -1.0) * dx;

				}
			}
		}

		return (bSig ? dPhi_max : dPhi_min);
	};
	
	auto CalcDis = [this, CalcPhi](	const int nCaseType,
									const CArray<T_ELEM_K, T_ELEM_K>& arElemK, 
									const CArray<_DGN_MPHI_MOMENT_D, _DGN_MPHI_MOMENT_D>& arMoment,
									const CArray<_DGN_MPHI_STRN_D, _DGN_MPHI_STRN_D&>& arMPhiStrn, 
									const BOOL bAbsDis, double& dDis) -> BOOL
	{
		dDis = 0.0;
		int nSize = arElemK.GetSize();
		if (nSize != arMoment.GetSize()) return FALSE;
		if (nSize != arMPhiStrn.GetSize()) return FALSE;

		T_ELEM_D ElemD;
		T_NODE_D NodeD1, NodeD2;
		if (!m_pDoc->m_pAttrCtrl->GetElem(arElemK[0], ElemD))	return FALSE;
		if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1)) return FALSE;
		if (!m_pDoc->m_pAttrCtrl->GetElem(arElemK[nSize - 1], ElemD))	return FALSE;
		if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2)) return FALSE;

		CArray<double, double> arLength;
		CArray<double, double> arPhi_I, arPhi_J;
		CArray<double, double> arLevel_I, arLevel_J;
		CArray<double, double> arDis;
		arLength.SetSize(nSize);
		arPhi_I.SetSize(nSize);
		arPhi_J.SetSize(nSize);
		arLevel_I.SetSize(nSize);
		arLevel_J.SetSize(nSize);
		arDis.SetSize(nSize);

		for (int i = 0; i < nSize; ++i)
		{
			if (!m_pDoc->m_pAttrCtrl->GetElem(arElemK[i], ElemD))	return FALSE;
			if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1)) return FALSE;
			if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2)) return FALSE;
			arLength[i] = CMathFunc::mathLength(NodeD1.x, NodeD1.y, NodeD1.z, NodeD2.x, NodeD2.y, NodeD2.z);
			arPhi_I[i] = CalcPhi(nCaseType, 0, arMoment[i].dM[0], arMPhiStrn[i]);
			arPhi_J[i] = CalcPhi(nCaseType, 2, arMoment[i].dM[2], arMPhiStrn[i]);
			arLevel_I[i] = NodeD1.z;
			arLevel_J[i] = NodeD2.z;
		}

		T_NODE_D dNodeTop;
		if (m_pDoc->m_pAttrCtrl->GetNode(p_dy.kTopNode, dNodeTop) == FALSE) return FALSE;
		const double dTop_z = dNodeTop.z;
		//const double dTop_z = arLevel_I[0];

		for (int i = 0; i < nSize; ++i)
		{
			const double z_I = dTop_z - arLevel_I[i];
			const double z_J = dTop_z - arLevel_J[i];
			const double z_M = (z_I + z_J) / 2.;
			const double phi_M = (arPhi_I[i] + arPhi_J[i]) / 2.;
			//arDis[i] = phi_M*z_M*arLength[i];
			arDis[i] = (arPhi_I[i] * z_I + arPhi_J[i] * z_J) * arLength[i] / 2.;
		}

		for (int i = 0; i < nSize; ++i)
			dDis += arDis[i];

		if (bAbsDis)
			dDis = fabs(dDis);

		return TRUE;
	};
	

	//균열시의 수평내력과 수평변위
	double dMc = dpMom[0][nCaseType];
	double dPc = dgn::SafeDiv(dMc, d_h);
	double dPhic = dpPhi[0][nCaseType];
	CArray<_DGN_MPHI_MOMENT_D, _DGN_MPHI_MOMENT_D> arM_curr; arM_curr.SetSize(nElemSize);
	
	double dDeltac = 0.0;
	if (CalcPierMoment(dPc, d_h, arElemKey, arM_curr) == FALSE) return FALSE;
	if (CalcDis(nCaseType, arElemKey, arM_curr, arMPhiStrn, TRUE, dDeltac) == FALSE) return FALSE;
	
	//초기항복시의 수평내력과 수평변위
	double dMy0 = dpMom[1][nCaseType];
	double dPy0 = dgn::SafeDiv(dMy0, d_h);
	double dPhiy0 = dpPhi[1][nCaseType];

	double dDeltay0 = 0.0;
	if (CalcPierMoment(dPy0, d_h, arElemKey, arM_curr) == FALSE) return FALSE;
	if (CalcDis(nCaseType, arElemKey, arM_curr, arMPhiStrn, TRUE, dDeltay0) == FALSE) return FALSE;
	dDelta_y = dDeltay0;

	//항복시의 수평내력과 수평변위 (내진성능2)
	double dMy = dpMom[2][nCaseType];
	double dPy = dgn::SafeDiv(dMy, d_h);
	double dPhiy = dpPhi[2][nCaseType];

	double dDeltay = 0.0;
	if (CalcPierMoment(dPy, d_h, arElemKey, arM_curr) == FALSE) return FALSE;
	if (CalcDis(nCaseType, arElemKey, arM_curr, arMPhiStrn, TRUE, dDeltay) == FALSE) return FALSE;
	dDelta_u = dDeltay;
	
	return TRUE;
}

BOOL CDgnToolMPhi_SeismicDesign_JP::Get_hAndElemKeysOfShortColumn(OUT double& d_h, OUT T_ELEM_K_LIST& arElemKey)
{
	ColumnData colm;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->FindColumnLowNode(p_dy.kSigrLower, p_dy.colmGroup, colm) == FALSE) return FALSE;

	arElemKey.Copy(colm.Column.aInvestigatedElem);
	if (arElemKey.GetCount() == 0) return FALSE;

	SortElemList(arElemKey, p_dy.iTopPosi, p_dy.iBottomPosi, p_dy.dMembHeight);

	T_NODE_D dNodeLow;
	if (m_pDoc->m_pAttrCtrl->GetNode(colm.kNodeLow, dNodeLow) == FALSE) return FALSE;

	T_NODE_D dNodeTop;
	if (m_pDoc->m_pAttrCtrl->GetNode(p_dy.kTopNode, dNodeTop) == FALSE) return FALSE;

	//연직거리를 구해야 함.
	d_h = dNodeTop.z - dNodeLow.z;
	return TRUE;
}

void CDgnToolMPhi_SeismicDesign_JP::GetInputData(OUT MPHI_DATA_ELEM& InData)
{
	T_SIPA_D dSipa;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(p_dy.kSipa, dSipa) == FALSE) return;

	InData.UserD.nEQType = [](int nSeisForcType)
		{
			switch (nSeisForcType)
			{
			case T_SIPA_D::kType1: return PMCV_EQ_TYPE::TYPE1;
			case T_SIPA_D::kType2: return PMCV_EQ_TYPE::TYPE2;
			default: ASSERT(0); return PMCV_EQ_TYPE::TYPE1;
			}
		}(dSipa.nSeismicForceType);
}

void CDgnToolMPhi_SeismicDesign_JP::GetCaseType(OUT int& nCaseType)
{
	if (p_dy.bMajorAxis) {
		if (p_dy.nCheckDir == ParamDelta_y::kPlus)
			nCaseType = 0;
		else if (p_dy.nCheckDir == ParamDelta_y::kMinus)
			nCaseType = 1;
	}
	else {
		if (p_dy.nCheckDir == ParamDelta_y::kPlus)
			nCaseType = 2;
		else if (p_dy.nCheckDir == ParamDelta_y::kMinus)
			nCaseType = 3;
	}
}