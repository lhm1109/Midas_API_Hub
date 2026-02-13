#include "stdafx.h"
#include "SeismicDesignDisp.h"

#include "..\MIT_lib\MMathFunc.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SECT.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"

#include "DgnProgressDlg.h"
#include "DgnToolMPhi_SeismicDesign_JP.h"

#pragma region // CSeismicDesignDisp
CSeismicDesignDisp::CSeismicDesignDisp() : CSeismicDesignBase()
{
}

CSeismicDesignDisp::~CSeismicDesignDisp()
{
}

void CSeismicDesignDisp::GetDispOfAngle(const ParamAngleOption& param, const double& dGlobalDx, const double& dGlobalDy, OUT double& dCompDisp)
{
	double dDispAngle = 0;
	GetAngleByOption(param, dDispAngle);
	double dTheta = abs(dDispAngle) / 180.0 * M_PI;// Radian단위로 변환

	double dSign = 1;
	if (param.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) { //교축방향이면 회전된 방향의 dx 구해야 하고
		if (dDispAngle < 0)
			dSign *= -1;

		dCompDisp = dGlobalDx * cos(dTheta) + dSign * dGlobalDy * sin(dTheta);
	}
	else if (param.nSeismicForceDirection == T_SIPA_D::kPerp_Dir) { //교축직각방향이면 회전된 방향의 dy 구해야 함
		if (dDispAngle > 0)
			dSign *= -1;

		dCompDisp = dGlobalDy * cos(dTheta) + dSign * dGlobalDx * sin(dTheta);
	}
}

void CSeismicDesignDisp::GetAngleByOption(const ParamAngleOption& param, OUT double& dDispAngle)
{
	double GCS_vector[3][3] = { {1.0, 0.0, 0.0}, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 } };

	switch (param.nDirDisp)
	{
	case ParamAngleOption::kUser_Dir:
		dDispAngle = param.dUserAngle;
		break;
	case ParamAngleOption::kLocal_Dir:
	{
		T_ELEM_D elem;
		if (m_pDoc->m_pAttrCtrl->GetElem(param.kElem, elem) == FALSE) return;

		double localVector[3][3] = { 0., };
		if (m_pDoc->calcFrameLocalVector(param.kElem, localVector) == FALSE) return;

		double dAngle = (param.nShearDir == ParamAngleOption::kY_Dir) ? -90 : 0;

		//요소 로컬벡터 z 축이 grobal x 축이라서 elem.angle 의 의미는 로컬벡터 z 축이 로컬 x축(Grobal z축) 기준으로 회전한 값을 의미함
		if (localVector[0][2] > 0.0)//객체 또는 벡터가 Z축의 양의 방향을 향하고 있음을 나타냄
			dDispAngle = dAngle + elem.angle; // -90은 로컬 y축 기준으로한 각도를 계산하기 위한 값 
		else
			dDispAngle = dAngle - elem.angle;
		break;
	}
	case ParamAngleOption::kSeismicForce_Dir:
	{
		T_THGA_D dThga;
		if (m_pDoc->m_pAttrCtrl->GetThga(param.kThis, dThga) == FALSE) return;
		dDispAngle = dThga.dAngleHGA;
		break;
	}
	default:
		break;
	}
}

BOOL CSeismicDesignDisp::GetH(const T_NODE_K& kNodeTop, const T_NODE_K& kNodeLow, OUT double& d_h)
{
	T_NODE_D dNodeLow;
	if (m_pDoc->m_pAttrCtrl->GetNode(kNodeLow, dNodeLow) == FALSE) return FALSE;

	T_NODE_D dNodeTop;
	if (m_pDoc->m_pAttrCtrl->GetNode(kNodeTop, dNodeTop) == FALSE) return FALSE;

	//연직거리를 구해야 함.
	d_h = dNodeTop.z - dNodeLow.z;

	return TRUE;
}

BOOL CSeismicDesignDisp::GetLp(IN PLp& param, OUT double& dLp)
{
	const int nLpFormula = std::get<0>(param);
	//const int nExamTarget = std::get<1>(param);
	//const int nLocalAxis = std::get<2>(param);
	const double d_h = std::get<3>(param);
	const ColumnData* pColumn = std::get<4>(param);

	switch (nLpFormula)
	{
	case T_SIDP_LSLP_D::kLp_UserValue:
		ASSERT(FALSE);//dLp = dSidp.LSLP_D.dLpLength;
		return FALSE;
	case T_SIDP_LSLP_D::kLp_0point2_Ellipsis:
		return GetLpByD_h(param, dLp);
		break;
	case T_SIDP_LSLP_D::kLp_9point5_Ellipsis://Lp=9.5σsy^(1/6)βn^(-1/3)φ
	{
		if (CDgnToolMPhi_SeismicDesign_JP().GetLp(d_h, pColumn, dLp) == FALSE) return FALSE;
		return TRUE;
	}
	break;
	default:
		ASSERT(FALSE);
		break;
	}
	return FALSE;
}

BOOL CSeismicDesignDisp::GetLpByD_h(IN PLp& param, OUT double& dLp)
{
	//const int nLpFormula = std::get<0>(param);
	const int nExamTarget = std::get<1>(param);
	//const int nLocalAxis = std::get<2>(param);
	//const double d_h = std::get<3>(param);
	const ColumnData* pColumn = std::get<4>(param);
	const T_ELEM_K& kElem = pColumn->kElem;

	//보강 단면이 있는지 확인
	BOOL bConsiderRein = FALSE;
	T_SECT_SSM_D tSectSsm; T_REIN_SSM_D ReinSsmD;
	if (nExamTarget == T_SIPA_D::kReinforceSection) {
		if (m_pDoc->m_pAttrCtrl2->GetQSectSsm()->Get(kElem, tSectSsm) == TRUE) {
			if (m_pDoc->m_pAttrCtrl2->GetQReinSsm()->Get(tSectSsm.kReinSsm, ReinSsmD) == TRUE) {
				bConsiderRein = TRUE;
			}
		}
	}

	if (bConsiderRein == FALSE) {
		return GetLpByD_h_Sect(param, dLp);
	}
	else {
		return GetLpByD_h_Rein(param, &ReinSsmD, dLp);
	}
}

BOOL CSeismicDesignDisp::GetLpByD_h_Sect(IN PLp& param, OUT double& dLp)
{
	//const int nLpFormula = std::get<0>(param);
	//const int nExamTarget = std::get<1>(param);
	const int nLocalAxis = std::get<2>(param);
	const double d_h = std::get<3>(param);
	const ColumnData* pColumn = std::get<4>(param);

	T_ELEM_D ElemD; T_SECT_D SectD;
	if (m_pDoc->m_pAttrCtrl->GetElem(pColumn->kElem, ElemD) == FALSE) return FALSE;
	if (m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD) == FALSE) return FALSE;

	double hi = 0, bi = 0, hj = 0, bj = 0;
	if (!m_pDoc->m_pAttrCtrl->GetSectDimen(SectD, hi, bi, hj, bj, NULL, FALSE)) return TRUE;

	double dD = hi;
	if (nLocalAxis == LOCAL_AXIS_Y)		//로컬 Y 축 기준으로한 단면의 높이
		dD = hi;
	else if (nLocalAxis == LOCAL_AXIS_Z)//로컬 Z 축 기준으로한 단면의 높이
		dD = bi;

	//Lp = 0.2h-0.1D
	double dVal = 0.2 * d_h - 0.1 * dD;

	// 0.1D≤Lp≤0.5D 범위 안인지 확인
	double dMin = 0.1 * dD;
	if (dVal < dMin)
		dVal = dMin;
	double dMax = 0.5 * dD;
	if (dMax < dVal)
		dVal = dMax;

	dLp = dVal;
	return TRUE;
}

BOOL CSeismicDesignDisp::GetLpByD_h_Rein(IN PLp& param, IN T_REIN_SSM_D* pReinSsmD, OUT double& dLp)
{
	//const int nLpFormula = std::get<0>(param);
	//const int nExamTarget = std::get<1>(param);
	const int nLocalAxis = std::get<2>(param);
	const double d_h = std::get<3>(param);
	const ColumnData* pColumn = std::get<4>(param);

	T_ELEM_D ElemD; T_SECT_D SectD;
	if (m_pDoc->m_pAttrCtrl->GetElem(pColumn->kElem, ElemD) == FALSE) return FALSE;
	if (m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD) == FALSE) return FALSE;

	T_SECT_SECTBASE_D& SectI = SectD.SectBefore.SectI;
	T_SECT_SECTBASE_D& SectJ = SectD.SectBefore.SectJ;

	CString& strShape = SectD.SectBefore.Shape;
	//I단 섹션 정보 생성
	m_pDoc->m_pAttrCtrl2->GetQReinSsm()->ModifySectSize(strShape, pReinSsmD, &SectI);
	//J단 섹션 정보 생성
	m_pDoc->m_pAttrCtrl2->GetQReinSsm()->ModifySectSize(strShape, pReinSsmD, &SectJ);

	//단면적 계산을 위해 CalcSectData() 호출
	BOOL bResult = m_pDoc->m_pSectDB->CalcSectData(SectD);
	ASSERT(bResult);
	// REGULAR 단면의 경우 CalcSectData() 함수는 SectI.Stiffness 정보밖에 갱신되지 않아
	// 갱신된 i 단의 Area 정보 등을 j단으로 복사해 준다.
	if (SectD.nStype == D_SECT_TYPE_REGULAR) {
		SectD.SectBefore.SectJ = SectD.SectBefore.SectI;
	}

	double hi = 0, bi = 0, hj = 0, bj = 0;
	if (!m_pDoc->m_pAttrCtrl->GetSectDimen(SectD, hi, bi, hj, bj, NULL, FALSE)) return TRUE;

	double dD = hi;
	if (nLocalAxis == LOCAL_AXIS_Y)		//로컬 Y 축 기준으로한 단면의 높이
		dD = hi;
	else if (nLocalAxis == LOCAL_AXIS_Z)//로컬 Z 축 기준으로한 단면의 높이
		dD = bi;

	//Lp = 0.8×(0.2h-0.1D)
	double dVal = 0.8 * (0.2 * d_h - 0.1 * dD);

	// 0.8×0.1D≤Lp≤0.8×0.5D 범위 안인지 확인
	double dMin = 0.8 * 0.1 * dD;
	if (dVal < dMin)
		dVal = dMin;
	double dMax = 0.8 * 0.5 * dD;
	if (dMax < dVal)
		dVal = dMax;

	dLp = dVal;
	return TRUE;
}

BOOL CSeismicDesignDisp::CalcPhiY_U(const T_ELEM_K& kElem, const int& nComponent, const seis_jp::eDirAnal& nDirAnal, const int& ePosNodeLow, OUT double& dPHI_y, OUT double& dPHI_u)
{
	double dPhi_y0 = 0.0, dPhi_u0 = 0.0;
	if (GetPhiY_U(kElem, nComponent, nDirAnal, ePosNodeLow, dPhi_y0, dPhi_u0) == FALSE) return FALSE;

	T_MPGB_D MpgbD;	MpgbD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetMpgb(MpgbD)) return FALSE;

	if (MpgbD.nYieldPoint == 1) {//1=항복점(Yielding Point)
		dPHI_y = dPhi_y0;
		dPHI_u = dPhi_u0;
		return TRUE;
	}

	// 0=초항복점(Initial Yielding Point) 일 경우
	double dMy0 = 0.0, dMu = 0.0;
	if (GetMomentY_U(kElem, nComponent, nDirAnal, ePosNodeLow, dMy0, dMu) == FALSE) return FALSE;

	// H14 :		Φy=(Mu/My0)Φy0
	// H24,H29 :	Φy=(Mls2/My0)Φy0
	dPHI_y = dgn::SafeDiv(dMu, dMy0) * dPhi_y0;
	dPHI_u = dPhi_u0;
	return TRUE;
}

BOOL CSeismicDesignDisp::GetPhiY_U(const T_ELEM_K& kElem, const int& nComponent, const seis_jp::eDirAnal& nDirAnal, const int& ePosNodeLow, OUT double& dPhiY, OUT double& dPhiU)
{
	MPhiProp prop;
	if (m_pDoc->m_pAttrCtrl2->GetQIehp()->GetHingeProp(kElem, nComponent, ePosNodeLow, prop) == FALSE) return FALSE;

	const int nDirection = (nDirAnal == seis_jp::kPlus) ? MPhiProp::kPlus : MPhiProp::kMinus;

	if (!std::isnan(prop.dDisplace_Y[nDirection]))
		dPhiY = prop.dDisplace_Y[nDirection];
	else
		dPhiY = prop.dDisplace_Y[nDirection];

	dPhiY = prop.dDisplace_Y[nDirection];
	dPhiU = prop.dDisplace_U[nDirection];

	return TRUE;
}

BOOL CSeismicDesignDisp::GetMomentY_U(const T_ELEM_K& kElem, const int& nComponent, const seis_jp::eDirAnal& nDirAnal, const int& ePosNodeLow, OUT double& dMy, OUT double& dMu)
{
	MPhiProp prop;
	if (m_pDoc->m_pAttrCtrl2->GetQIehp()->GetHingeProp(kElem, nComponent, ePosNodeLow, prop) == FALSE) return FALSE;

	const int nDirection = (nDirAnal == seis_jp::kPlus) ? MPhiProp::kPlus : MPhiProp::kMinus;

	if (!std::isnan(prop.dStrength_Y[nDirection]))
		dMy = prop.dStrength_Y[nDirection];
	else
		dMy = prop.dStrength_C[nDirection];

	dMu = prop.dStrength_U[nDirection];

	return TRUE;
}

void CSeismicDesignDisp::TransformLocalToGlobal(const T_ELEM_K& kElem, const int& ePosNodeLow, const double& dVal_My, const double& dVal_Mz, double* dVal)
{
	T_ELEM_D tElem;
	m_pDoc->m_pAttrCtrl->GetElem(kElem, tElem);

	T_NODE_K kNode = 0;
	if (ePosNodeLow == T_SIGR_LOWER_COLM_D::kI)
		kNode = tElem.elnod[0];
	else if (ePosNodeLow == T_SIGR_LOWER_COLM_D::kJ)
		kNode = tElem.elnod[1];

	T_NODE_D tNode;
	m_pDoc->m_pAttrCtrl->GetNode(kNode, tNode);

	double dLocalVector[3][3];
	m_pDoc->calcElemLocalVector(kElem, dLocalVector);

	T_VECTOR3D localY(0, dVal_My, 0);
	T_VECTOR3D localZ(0, 0, dVal_Mz);

	CMathFunc::mathUCS2GCS(localY.x, localY.y, localY.z, dLocalVector);
	CMathFunc::mathUCS2GCS(localZ.x, localZ.y, localZ.z, dLocalVector);

	dVal[0] = localY.x + localZ.x;
	dVal[1] = localY.y + localZ.y;
}

BOOL CSeismicDesignDisp::GetDispFromTimeHistory(const T_THIS_K& kThis, const T_NODE_K& kNode, const double& nStep, OUT double& dDx, OUT double& dDy)
{
	m_pDoc->m_pPostCtrl->SelectThis(kThis);

	int nTimeIndex = 0;
	if (GetTimeIndex(kThis, nStep, nTimeIndex) == FALSE) return FALSE;

	T_DISP_D disp;
	if (m_pDoc->m_pPostCtrl->GetDispHistory(nTimeIndex, kNode, disp) == FALSE) return FALSE;

	double v[3] = { 0., };
	dDx = disp.dblDisp[DISP_COMP_DX];
	dDy = disp.dblDisp[DISP_COMP_DY];

	return TRUE;
}

BOOL CSeismicDesignDisp::GetRotaFromTimeHistory(const T_THIS_K& kThis, const T_NODE_K& kNode, const double& nStep, OUT double& dRx, OUT double& dRy)
{
	m_pDoc->m_pPostCtrl->SelectThis(kThis);

	int nTimeIndex = 0;
	if (GetTimeIndex(kThis, nStep, nTimeIndex) == FALSE) return FALSE;

	T_DISP_D disp;
	if (m_pDoc->m_pPostCtrl->GetDispHistory(nTimeIndex, kNode, disp) == FALSE) return FALSE;

	double v[3] = { 0., };
	dRx = disp.dblDisp[DISP_COMP_RX];
	dRy = disp.dblDisp[DISP_COMP_RY];

	return TRUE;
}

BOOL CSeismicDesignDisp::GetF_ls(IN PDeltaVal& tParam, const double& dTimeStep, OUT double& dFls_Dx, OUT double& dFls_Dy)
{
	double dDelta_fd_Dx = 0, dDelta_fd_Dy = 0;
	if (GetDelta_fd(tParam, dTimeStep, dDelta_fd_Dx, dDelta_fd_Dy) == FALSE) return FALSE;

	double Theta_fp_Rx = 0, Theta_fp_Ry = 0;
	if (GetTheta_fp(tParam, dTimeStep, Theta_fp_Rx, Theta_fp_Ry) == FALSE) return FALSE;

	// h 계산
	T_THIS_K kThis = std::get<1>(tParam);
	T_NODE_K kPierTopNode = std::get<2>(tParam);
	ColumnData* pLowColmData = std::get<5>(tParam);
	T_NODE_K kNodeLow = pLowColmData->kNodeLow;

	double d_h = 0;
	if (Get_h(kNodeLow, kPierTopNode, d_h) == FALSE) return FALSE;

	int nTimeIndex = 0;
	if (GetTimeIndex(kThis, dTimeStep, nTimeIndex) == FALSE) return FALSE;

	// δs : 지승 변형량
	double dDelta_s[eVector2d::kMax2d] = { 0.0, 0.0 };
	GetDispOfBearing(tParam, nTimeIndex, dDelta_s);

	//δfls=δfd+Θfp*h+δs  회전각은 지진력 방향의 직교축 기준
	dFls_Dx = dDelta_fd_Dx + (Theta_fp_Ry * d_h) + dDelta_s[eVector2d::kDx];
	dFls_Dy = dDelta_fd_Dy + (Theta_fp_Rx * d_h) + dDelta_s[eVector2d::kDy];

	return TRUE;
}

BOOL CSeismicDesignDisp::GetDelta_fd(IN PDeltaVal& tParam, const double& dTimeStep, OUT double& dDx, OUT double& dDy)
{
	T_THIS_K kThis = std::get<1>(tParam);
	ColumnData* pLowColmData = std::get<5>(tParam);
	T_NODE_K kNodeLow = pLowColmData->kNodeLow;

	return GetDispFromTimeHistory(kThis, kNodeLow, dTimeStep, dDx, dDy);
}

BOOL CSeismicDesignDisp::GetTheta_fp(IN PDeltaVal& tParam, const double& dTimeStep, OUT double& dRx, OUT double& dRy)
{
	T_THIS_K kThis = std::get<1>(tParam);
	ColumnData* pLowColmData = std::get<5>(tParam);
	T_NODE_K kNodeLow = pLowColmData->kNodeLow;

	return GetRotaFromTimeHistory(kThis, kNodeLow, dTimeStep, dRx, dRy);
}

BOOL CSeismicDesignDisp::Get_h(const T_NODE_K& kNodeLow, const T_NODE_K& kPierTopNode, OUT double& d_h)
{
	T_NODE_D dNodeLow;
	if (m_pDoc->m_pAttrCtrl->GetNode(kNodeLow, dNodeLow) == FALSE) return FALSE;

	T_NODE_D dNodeTop;
	if (m_pDoc->m_pAttrCtrl->GetNode(kPierTopNode, dNodeTop) == FALSE) return FALSE;

	//연직거리를 구해야 함.
	d_h = dNodeTop.z - dNodeLow.z;
	return TRUE;
}

void CSeismicDesignDisp::GetDispOfBearing(IN PDeltaVal& tParam, const int& nTimeIndex, OUT double* pDelta_s)
{
	T_NODE_K nodeBot = 0, nodeTop = 0;
	if (FindBearingNode(tParam, nodeBot, nodeTop) == FALSE) return;

	T_DISP_D disp; if (m_pDoc->m_pPostCtrl->GetDispHistory(nTimeIndex, nodeBot, disp) == FALSE) { ASSERT(FALSE); return; }
	double dDispNodeBot[eVector2d::kMax2d] = { disp.dblDisp[DISP_COMP_DX], disp.dblDisp[DISP_COMP_DY] };

	disp.Initialize(); if (m_pDoc->m_pPostCtrl->GetDispHistory(nTimeIndex, nodeTop, disp) == FALSE) { ASSERT(FALSE); return; }
	double dDispNodeTop[eVector2d::kMax2d] = { disp.dblDisp[DISP_COMP_DX], disp.dblDisp[DISP_COMP_DY] };

	pDelta_s[eVector2d::kDx] = dDispNodeTop[eVector2d::kDx] - dDispNodeBot[eVector2d::kDx];
	pDelta_s[eVector2d::kDy] = dDispNodeTop[eVector2d::kDy] - dDispNodeBot[eVector2d::kDy];
}

BOOL CSeismicDesignDisp::FindBearingNode(IN PDeltaVal& tParam, OUT T_NODE_K& rNodeBot, OUT T_NODE_K& rNodeTop)
{
	T_SIGR_K kSigr = std::get<0>(tParam);
	T_NODE_K kPierTopNode = std::get<2>(tParam);
	ColumnData* pLowColmData = std::get<5>(tParam);

	T_NODE_D columnNode1, columnNode2;
	if (m_pDoc->m_pAttrCtrl->GetNode(kPierTopNode, columnNode1) == FALSE) { ASSERT(FALSE); return FALSE; }
	if (m_pDoc->m_pAttrCtrl->GetNode(pLowColmData->kNodeLow, columnNode2) == FALSE) { ASSERT(FALSE); return FALSE; }

	T_SIGR_LOWER_D dSigrLower;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigr, dSigrLower) == FALSE) return FALSE;
	if (dSigrLower.BEAR_D.GetCount() < 1) return FALSE;

	for (int i = 0; i < dSigrLower.BEAR_D.GetCount(); i++)
	{
		T_SIGR_LOWER_BEAR_D& bear = dSigrLower.BEAR_D[i];
		switch (bear.nType)
		{
		case T_SIGR_LOWER_BEAR_D::kRigidLink:
		case T_SIGR_LOWER_BEAR_D::kFixed:
			continue;
		default:
			//탄성, 면진, 가동 지승의 경우만 고려한다.
			break;
		}

		ASSERT(bear.aPosition.GetCount() == 2);
		if (bear.aPosition.GetCount() < 2)
			continue;

		T_NODE_K kNode1 = bear.aPosition[0];
		T_NODE_K kNode2 = bear.aPosition[1];

		T_NODE_D node1, node2;
		if (m_pDoc->m_pAttrCtrl->GetNode(kNode1, node1) == FALSE) { ASSERT(FALSE); continue; }
		if (m_pDoc->m_pAttrCtrl->GetNode(kNode2, node2) == FALSE) { ASSERT(FALSE); continue; }

		double tol = 1e-8;
		if (MMathFunc::PointOnLine(columnNode1.x, columnNode1.y, columnNode1.z, columnNode2.x, columnNode2.y, columnNode2.z, node2.x, node2.y, node2.z, tol) == false) continue;
		if (MMathFunc::PointOnLine(columnNode1.x, columnNode1.y, columnNode1.z, columnNode2.x, columnNode2.y, columnNode2.z, node1.x, node1.y, node1.z, tol) == false) continue;

		if (node1.z > node2.z) {
			rNodeTop = kNode1;
			rNodeBot = kNode2;
		}
		else {
			rNodeTop = kNode2;
			rNodeBot = kNode1;
		}

		return TRUE;
	}

	return FALSE;
}
#pragma endregion

#pragma region // CSeismicDesignDisp_Upper

BOOL CSeismicDesignDisp_Upper::CalcDeltaMax(const T_NODE_K& kNode, const T_THIS_K& kThis, const int& nTimeIdx, OUT double* pDeltaMax)
{
	m_pDoc->m_pPostCtrl->SelectThis(kThis);

	// δrmax : 관성력작용위치의 수평변위(global dx, dy)
	T_DISP_D disp;
	if (m_pDoc->m_pPostCtrl->GetDispHistory(nTimeIdx, kNode, disp) == FALSE) return FALSE;

	pDeltaMax[eVector2d::kDx] = disp.dblDisp[DISP_COMP_DX];
	pDeltaMax[eVector2d::kDy] = disp.dblDisp[DISP_COMP_DY];

	return TRUE;
}

#pragma endregion

#pragma region // CSeismicDesignDisp_Lower

BOOL CSeismicDesignDisp_Lower::CalcDeltaMax(IN PDeltaVal& tParam, 
											const ParamAngleOption& angleP, 
											const int& nTimeIndex, 
											OUT doubleArray& aDeltaMax)
{
	// 푸쉬오버 해석케이스 스텝별로 δmax = δrmax-δfd-Θfp×h-δs 를 계산하여 가장 큰 값을 δmax 로 한다.
	eRspCalcType nRspCalcType = (eRspCalcType)std::get<4>(tParam);

	double dVal = 0;
	BOOL bResult = FALSE;
	switch (nRspCalcType)
	{
	case seis_jp::kNEXCO:
		bResult = CalcDeltaMaxOnStepByNexco(tParam, angleP, nTimeIndex, dVal);
		break;
	case seis_jp::kJBEC:
		bResult = CalcDeltaMaxOnStepByJbec(tParam, angleP, nTimeIndex, dVal);
		break;
	}

	if (bResult == FALSE)
		return FALSE;

	bResult = FALSE;
	switch (nRspCalcType)
	{
	case seis_jp::kJBEC:
		if (aDeltaMax.GetCount() > seis_jp::kDmaxJbec) {
			bResult = TRUE; aDeltaMax[seis_jp::kDmaxJbec] = dVal;
		}
		break;
	case seis_jp::kNEXCO:
		if (aDeltaMax.GetCount() > seis_jp::kDmaxNexco) {
			bResult = TRUE; aDeltaMax[seis_jp::kDmaxNexco] = dVal;
		}
		break;
	}
	return bResult;
}

BOOL CSeismicDesignDisp_Lower::CalcDeltaMaxOnStepByNexco(IN PDeltaVal& tParam,
														 const ParamAngleOption& angleP,
														 const int& nTimeIndex,
														 OUT double& dDeltaMax)
{
	T_THIS_K kThis = std::get<1>(tParam);
	T_NODE_K kPierTopNode = std::get<2>(tParam);
	ColumnData* pLowColmData = std::get<5>(tParam);

	T_NODE_D dNodeTop;T_NODE_D dNodeLow;
	if (m_pDoc->m_pAttrCtrl->GetNode(kPierTopNode, dNodeTop) == FALSE) return FALSE;
	if (m_pDoc->m_pAttrCtrl->GetNode(pLowColmData->kNodeLow, dNodeLow) == FALSE) return FALSE;

	// h 계산 : 연직거리를 구해야 함
	double d_h = dNodeTop.z - dNodeLow.z;

	m_pDoc->m_pPostCtrl->SelectThis(kThis);

	// δrmax : 관성력작용위치의 수평변위(global dx, dy)
	T_DISP_D disp; if (m_pDoc->m_pPostCtrl->GetDispHistory(nTimeIndex, kPierTopNode, disp) == FALSE) return FALSE;
	double dDelta_rmax[eVector2d::kMax2d] = { disp.dblDisp[DISP_COMP_DX], disp.dblDisp[DISP_COMP_DY] };

	// δfp : 교각기부의 수평변위(global dx, dy)
	disp.Initialize(); if (m_pDoc->m_pPostCtrl->GetDispHistory(nTimeIndex, pLowColmData->kNodeLow, disp) == FALSE) return FALSE;
	double dDelta_fp[eVector2d::kMax2d] = { disp.dblDisp[DISP_COMP_DX], disp.dblDisp[DISP_COMP_DY] };

	// Θfp : 교각기부의 회전각(global rx, ry)
	disp.Initialize(); if (m_pDoc->m_pPostCtrl->GetDispHistory(nTimeIndex, pLowColmData->kNodeLow, disp) == FALSE) return FALSE;
	double dTheta_fp[eRotate2d::kMax2r] = { disp.dblDisp[DISP_COMP_RX], disp.dblDisp[DISP_COMP_RY] };

	// δs : 지승 변형량
	double dDelta_s[eVector2d::kMax2d] = { 0.0, 0.0 };
	GetDispOfBearing(tParam, nTimeIndex, dDelta_s);

	//δmax = δrmax-δfp-Θfp×h-δs
	double dDls_Dx = dDelta_rmax[eVector2d::kDx] - dDelta_fp[eVector2d::kDx] - (dTheta_fp[eRotate2d::kRy] * d_h) - dDelta_s[eVector2d::kDx];//dTheta_p_dx[1] 인 이유: 회전방향은 변형방향의 직각이기 때문
	double dDls_Dy = dDelta_rmax[eVector2d::kDy] - dDelta_fp[eVector2d::kDy] - (dTheta_fp[eRotate2d::kRx] * d_h) - dDelta_s[eVector2d::kDy];//dTheta_p_dx[0] 인 이유: 회전방향은 변형방향의 직각이기 때문

	GetDispOfAngle(angleP, dDls_Dx, dDls_Dy, dDeltaMax);

	return TRUE;
}

BOOL CSeismicDesignDisp_Lower::CalcDeltaMaxOnStepByJbec(IN PDeltaVal& tParam,
														const ParamAngleOption& angleP,
														const int& nTimeIndex,
														OUT double& dDeltaMax)
{
	T_THIS_K kThis = std::get<1>(tParam);

	T_NODE_K kPierTopNode = 0, kPierBotNode = 0, kBearingTopNode = 0, kBearingBotNode = 0;
	BOOL bExistBearing = FindNodeRelatedPier(tParam, kPierTopNode, kPierBotNode, kBearingTopNode, kBearingBotNode);

	if (kBearingBotNode == 0 || kBearingTopNode == 0) {
		kBearingBotNode = kPierTopNode;
	}

	// h  : 기둥하부절점(or 기초저면)에서 상부구조 관성력작용위치까지의 높이
	// h1 : 교각천단(관성력작용위치)에서 상부 관성력작용위치까지의 높이
	double dH = 0, dH1 = 0;
	Get_h(kPierBotNode, kPierTopNode, dH);
	Get_h(kBearingBotNode, kPierTopNode, dH1);

	m_pDoc->m_pPostCtrl->SelectThis(kThis);

	T_DISP_D disp; if (m_pDoc->m_pPostCtrl->GetDispHistory(nTimeIndex, kBearingBotNode, disp) == FALSE) return FALSE;
	// δt : 교각천단(관성력작용위치) 수평변위
	double dDelta_t[eVector2d::kMax2d] = { disp.dblDisp[DISP_COMP_DX], disp.dblDisp[DISP_COMP_DY] };
	// Θt : 교각천단의 회전각(global rx, ry)
	double dTheta_t[eRotate2d::kMax2r] = { disp.dblDisp[DISP_COMP_RX], disp.dblDisp[DISP_COMP_RY] };

	// δb : 기초저면의 수평변위(global dx, dy)
	disp.Initialize(); if (m_pDoc->m_pPostCtrl->GetDispHistory(nTimeIndex, kPierBotNode, disp) == FALSE) return FALSE;
	double dDelta_b[eVector2d::kMax2d] = { disp.dblDisp[DISP_COMP_DX], disp.dblDisp[DISP_COMP_DY] };
	// Θb : 기초저면의 회전각(global rx, ry)
	double dTheta_b[eRotate2d::kMax2r] = { disp.dblDisp[DISP_COMP_RX], disp.dblDisp[DISP_COMP_RY] };

	//δpr = δt + (h1*Θt) - δb - (h*Θb)
	double dDls_Dx = dDelta_t[eVector2d::kDx] + (dH1 * dTheta_t[eRotate2d::kRy]) - dDelta_b[eVector2d::kDx] - (dH * dTheta_b[eRotate2d::kRy]);
	double dDls_Dy = dDelta_t[eVector2d::kDy] + (dH1 * dTheta_t[eRotate2d::kRx]) - dDelta_b[eVector2d::kDy] - (dH * dTheta_b[eRotate2d::kRx]);

	GetDispOfAngle(angleP, dDls_Dx, dDls_Dy, dDeltaMax);

	return TRUE;
}

BOOL CSeismicDesignDisp_Lower::CalcDeltaValAtStep(	IN PDeltaVal& tParam,
													const int& nTimeStep,
													OUT double& dDls_Dx,
													OUT double& dDls_Dy)
{
	eRspCalcType nRspCalcType = (eRspCalcType)std::get<4>(tParam);

	BOOL bResult = FALSE;

	switch (nRspCalcType)
	{
	case seis_jp::kNEXCO:
		bResult = CalcDeltaValAtStepByNexco(tParam, nTimeStep, dDls_Dx, dDls_Dy);
		break;
	case seis_jp::kJBEC:
		bResult = CalcDeltaValAtStepByJbec(tParam, nTimeStep, dDls_Dx, dDls_Dy);
		break;
	}

	return bResult;
}

BOOL CSeismicDesignDisp_Lower::CalcDeltaValAtStepByNexco(	IN PDeltaVal& tParam,
															const int& nTimeStep,
															OUT double& dDls_Dx,
															OUT double& dDls_Dy)
{
	T_THIS_K kThis = std::get<1>(tParam);
	T_NODE_K kPierTopNode = std::get<2>(tParam);
	ColumnData* pLowColmData = std::get<5>(tParam);

#if _DEBUG
	T_THIS_D tThis;
	m_pDoc->m_pAttrCtrl->GetThis(kThis, tThis);
#endif

	double aDeltaLs[eVector2d::kMax2d];
	if (GetDispFromTimeHistory(kThis, kPierTopNode, nTimeStep, aDeltaLs[eVector2d::kDx], aDeltaLs[eVector2d::kDy]) == FALSE) { ASSERT_ONCE(FALSE); return FALSE; }

	// δfls=δfd+Θfp*h+δs
	double dDelta_fls_Dx = 0, dDelta_fls_Dy = 0;
	if (GetF_ls(tParam, nTimeStep, dDelta_fls_Dx, dDelta_fls_Dy) == FALSE) return FALSE;

	// δy =δry - (δfd+Θfp×h+δs)(H14)
	// δu =δru - (δfd+Θfp×h+δs)(H14)
	// δls=δrls- (δfd+Θfp×h+δs)(H24)
	dDls_Dx = aDeltaLs[eVector2d::kDx] - dDelta_fls_Dx;
	dDls_Dy = aDeltaLs[eVector2d::kDy] - dDelta_fls_Dy;

	return TRUE;
}

BOOL CSeismicDesignDisp_Lower::CalcDeltaValAtStepByJbec(IN PDeltaVal& tParam,
	const int& nTimeStep,
	OUT double& dDls_Dx,
	OUT double& dDls_Dy)
{
	T_THIS_K kThis = std::get<1>(tParam);

	T_NODE_K kPierTopNode = 0, kPierBotNode = 0, kBearingTopNode = 0, kBearingBotNode = 0;
	BOOL bExistBearing = FindNodeRelatedPier(tParam, kPierTopNode, kPierBotNode, kBearingTopNode, kBearingBotNode);

	if (kBearingBotNode == 0 || kBearingTopNode == 0) {
		kBearingBotNode = kPierTopNode;
	}

	int nTimeIndex = 0;
	if (GetTimeIndex(kThis, nTimeStep, nTimeIndex) == FALSE) return FALSE;

	// h  : 기둥하부절점(or 기초저면)에서 상부구조 관성력작용위치까지의 높이
	// h1 : 교각천단(관성력작용위치)에서 상부 관성력작용위치까지의 높이
	double dH = 0, dH1 = 0;
	Get_h(kPierBotNode, kPierTopNode, dH);
	Get_h(kBearingBotNode, kPierTopNode, dH1);

	m_pDoc->m_pPostCtrl->SelectThis(kThis);

	T_DISP_D disp; if (m_pDoc->m_pPostCtrl->GetDispHistory(nTimeIndex, kBearingBotNode, disp) == FALSE) return FALSE;
	// δt : 교각천단(관성력작용위치) 수평변위
	double dDelta_t[eVector2d::kMax2d] = { disp.dblDisp[DISP_COMP_DX], disp.dblDisp[DISP_COMP_DY] };
	// Θt : 교각천단의 회전각(global rx, ry)
	double dTheta_t[eRotate2d::kMax2r] = { disp.dblDisp[DISP_COMP_RX], disp.dblDisp[DISP_COMP_RY] };

	// δb : 기초저면의 수평변위(global dx, dy)
	disp.Initialize(); if (m_pDoc->m_pPostCtrl->GetDispHistory(nTimeIndex, kPierBotNode, disp) == FALSE) return FALSE;
	double dDelta_b[eVector2d::kMax2d] = { disp.dblDisp[DISP_COMP_DX], disp.dblDisp[DISP_COMP_DY] };
	// Θb : 기초저면의 회전각(global rx, ry)
	double dTheta_b[eRotate2d::kMax2r] = { disp.dblDisp[DISP_COMP_RX], disp.dblDisp[DISP_COMP_RY] };

	// δy=δty+Θty×h1-Θby×h-δby
	// δu=δtu+Θtu×h1-Θbu×h-δbu
	dDls_Dx = dDelta_t[eVector2d::kDx] + (dTheta_t[eRotate2d::kRy] * dH1) - (dTheta_b[eRotate2d::kRy] * dH) - dDelta_b[eVector2d::kDx];
	dDls_Dy = dDelta_t[eVector2d::kDy] + (dTheta_t[eRotate2d::kRx] * dH1) - (dTheta_b[eRotate2d::kRx] * dH) - dDelta_b[eVector2d::kDy];

	return TRUE;
}

BOOL CSeismicDesignDisp_Lower::FindNodeRelatedPier(	IN PDeltaVal& tParam,
													OUT T_NODE_K& kPierTopNode,
													OUT T_NODE_K& kPierBotNode,
													OUT T_NODE_K& kBearingTopNode,
													OUT T_NODE_K& kBearingBotNode)
{
	T_SIGR_K kSigr = std::get<0>(tParam);
	ColumnData* pLowColmData = std::get<5>(tParam);

	kPierTopNode = std::get<2>(tParam);
	kPierBotNode = pLowColmData->kNodeLow;// 2025.09.25 chlim 시양상과 협의하여 항상 기둥하부 절점으로 하기로 함(기초저면 절점 사용X)

	/*T_SIGR_LOWER_D sigrLower;
	BOOL bLower = m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigr, sigrLower);
	ASSERT(bLower);
	if (bLower == FALSE) {
		kPierBotNode = pLowColmData->kNodeLow;
	}
	else {
		if (sigrLower.FOOT_D.GetCount() < 1) {
			kPierBotNode = pLowColmData->kNodeLow;
		}
		else {
			FootingData footData;
			if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->FindFooting(kSigr, kPierBotNode, footData) == FALSE) {
				kPierBotNode = pLowColmData->kNodeLow;
			}
			else {
				kPierBotNode = footData.kNodeLow;
			}
		}
	}*/

	return FindBearingNode(tParam, kBearingBotNode, kBearingTopNode);
}

BOOL CSeismicDesignDisp_Lower::GetDeltaYandU(IN PDeltaYandU& param, OUT double& rDeltaY, OUT double& rDeltaU)
{
	T_SIPA_K kSipa = std::get<0>(param);
	T_SIGR_K kSigr = std::get<1>(param);
	T_NODE_K kPierTopNode = std::get<2>(param);
	CString strNameCOLM = std::get<3>(param);
	seis_jp::eDirAnal dirAnal = (seis_jp::eDirAnal)std::get<4>(param);
	T_SIPA_D::eSeismicForceDirection seisDir = (T_SIPA_D::eSeismicForceDirection)std::get<5>(param);
	ColumnData* pLowColmData = std::get<6>(param);
	const int nDirShear = pLowColmData->Column.nDirShear;

	//교각 기부의 비선형특성(M-φ관계)로부터 상태 단계별 수평변위를 가정하여, 요소별 적분의 누적에 의해 상부공 관성력 작용위치의 변위 산출
	ParamDelta_y PDy; PDy.Initialize();
	PDy.kSipa = kSipa;
	PDy.kTopNode = kPierTopNode;
	PDy.kSigrLower = kSigr;
	PDy.colmGroup = strNameCOLM;
	PDy.bMajorAxis = (seisDir == T_SIPA_D::kAxis_Dir) ? TRUE : FALSE;
	PDy.nCheckDir = dirAnal;

	double aDelta_y0[2] = { 0.0, 0.0 };
	double aDelta_u0[2] = { 0.0, 0.0 };

	CDgnToolMPhi_SeismicDesign_JP mphi;
	if (mphi.CalcHoriDisp(PDy, aDelta_y0, aDelta_u0) == FALSE)
		return FALSE;

	T_MPGB_D mpgbD;	mpgbD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetMpgb(mpgbD)) return FALSE;

	const int nLocal_0 = [seisDir]()
	{
		switch (seisDir)
		{
		case T_SIPA_D::kAxis_Dir: return 0;//MajorAxis
		case T_SIPA_D::kPerp_Dir: return 1;//PerpAxis
		default: ASSERT(0); return 2;
		}
	}();

	// δy0는, 항복점 타입이 '항복점'인 경우나 철골교갹의 경우 보정을 하지 않고'δy=δy0'로 한다.
	if (mpgbD.nYieldPoint == 1 || pLowColmData->eMatlType == T_SIGR_LOWER_D::kSteel) {
		rDeltaY = aDelta_y0[nLocal_0];
		rDeltaU = aDelta_u0[nLocal_0];
		return TRUE;
	}

	const int nCompIdx = [seisDir, nDirShear]()
		{
			int nShearForceDir = (seisDir == T_SIPA_D::kAxis_Dir) ? nDirShear : (nDirShear == seis_jp::kY_Dir ? seis_jp::kZ_Dir : seis_jp::kY_Dir);
			switch (nShearForceDir)
			{
			case seis_jp::kY_Dir: return QIehp::kMz;
			case seis_jp::kZ_Dir: return QIehp::kMy;
			default: ASSERT(0); return QIehp::kUnknown;
			}
		}();

	// 보정식: δy＝Mls/My0・δy0
	double dMy0 = 0.0, dMLs = 0.0;
	if (GetMomentY_U(pLowColmData->kElem, nCompIdx, dirAnal, pLowColmData->ePosNodeLow, dMy0, dMLs) == FALSE) return FALSE;

	//먼저 로컬 축 기준으로 (Mls2 / My0) 이것 부터 계산
	double dVal = dgn::SafeDiv(dMLs, dMy0);

	rDeltaY = dVal * aDelta_y0[nLocal_0];
	rDeltaU = dVal * aDelta_u0[nLocal_0];

	return TRUE;
}
#pragma endregion