#include "stdafx.h"
#include "rusPunchLineCalc.h"
#include "..\wg_db\MathFunc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\DBDoc.h"
#include "DgnDataCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_base\VectorUtil.h"
#include "DgnCalcCutlLine.h"
#include "Dgn_Object.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _TOL	   1e-3
#define _MPA2STAND 1//1000
#define _N2KN	   1//1e-3
#define _NMM2KNM   1//1e-6
#define _NMM2KNM2  1//1e-9
#define _MM2M	   1//1e-3
#define PI	4.0 * atan(1.0); // 3.141592653

const double cRCS_Zero = 1.0E-07;
/**
 * 冲切线的计算以节点连接的激活的板的最小板厚来决定. 
 * 和CRCSCodeCheck::Check_PunchingShear中的冲切线段一致
 *	e.g. 一个节点连接4个不同的板厚,单独选择不同的板进行冲切验算,冲切线的outline是不一样的.如果这个节点的所有板都选中来进行冲切验算,冲切线以最小板厚来决定
 * 
 */
rusPunchLineCalc::rusPunchLineCalc()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nCode_Length = D_UNITSYS_LENGTH_INDEX_MM;
	m_nCode_Force = D_UNITSYS_FORCE_INDEX_N;
	FactorControlPerimeter = 0.5;
}

BOOL rusPunchLineCalc::CalcAndSavePunchLline(int nColmElem, std::vector<T_RSS2_CTLN>& arPnChkCutl)
{
	m_iElemNo = nColmElem;
	_DGN_PUNCH_RES PunchData;
	if(!InitPunchResData(PunchData))
		return FALSE;
	_DGN_PERI_PART PeriPartD;
	if (!Get_CutPerimeter(PunchData, PeriPartD))
		return FALSE;
	if (!SubMakePeripartForTwowayForceShearCheck(PeriPartD))
		return FALSE;
	
	T_NODE_D NodeD;
	m_pDoc->m_pAttrCtrl->GetNode(PeriPartD.iNodeNo, NodeD);
	
	arPnChkCutl.clear();
	for (size_t i = 0; i < PeriPartD.aResList.GetSize(); i++)
	{
		T_RSS2_CTLN CutD;
		CutD.Initialize();
		CutD.Point1[0] = PeriPartD.aResList[i].dCx[0];
		CutD.Point1[1] = PeriPartD.aResList[i].dCy[0]; 
		CutD.Point1[2] = NodeD.z;

		CutD.Point2[0] = PeriPartD.aResList[i].dCx[1];
		CutD.Point2[1] = PeriPartD.aResList[i].dCy[1];
		CutD.Point2[2] = NodeD.z;

		arPnChkCutl.emplace_back(std::move(CutD));
	}
	m_NodeKey = PeriPartD.iNodeNo;
	return TRUE;
}


BOOL rusPunchLineCalc::GetPunchInfo(const std::vector<T_RSS2_CTLN>& arPnChkCutl,double& dX, double& dY, int& iLoad2Flat)
{
	double dCoverThk = min(m_dB, m_dT);
	double dDangerDrat = 1.0 / FactorControlPerimeter;
	dX = (m_dInnerX + 2 * (m_dThk - dCoverThk) / dDangerDrat) / 2.0;
	dY = (m_dInnerY + 2 * (m_dThk - dCoverThk) / dDangerDrat) / 2.0;

	double dXMin = DBL_MAX;
	double dYMin = DBL_MAX;
	double dXMax = DBL_MIN;
	double dYMax = DBL_MIN;
	if (arPnChkCutl.empty())
	{
		return FALSE;
	}
	for (size_t i = 0; i < arPnChkCutl.size(); i++)
	{
		dXMin = min(dXMin, arPnChkCutl[i].Point1[0]);
		dXMin = min(dXMin, arPnChkCutl[i].Point2[0]);
		dXMax = max(dXMax, arPnChkCutl[i].Point1[0]);
		dXMax = max(dXMax, arPnChkCutl[i].Point2[0]);

		dYMin = min(dYMin, arPnChkCutl[i].Point1[1]);
		dYMin = min(dYMin, arPnChkCutl[i].Point2[1]);
		dYMax = max(dYMax, arPnChkCutl[i].Point1[1]);
		dYMax = max(dYMax, arPnChkCutl[i].Point2[1]);
	}
	double dDirXLength = (dXMax - dXMin);
	double dDirYLength = (dYMax - dYMin);

	// 0:inside 1:corner 2:edge
	iLoad2Flat = 1;
	int mask = 0;
	if (dX * 2 < dDirXLength + cRCS_Zero)
		mask++;
	if (dY * 2 < dDirYLength + cRCS_Zero)
		mask++;
	if (mask == 1)
	{
		iLoad2Flat = 2;
	}
	else if (mask == 2)
	{
		iLoad2Flat = 0;
	}

	return TRUE;
}

BOOL rusPunchLineCalc::InitPunchResData(_DGN_PUNCH_RES& PunchData)
{
	PunchData.Initialize();
	T_THIK_D ThikD;
	T_ELEM_D ElemD;
	m_pDoc->m_pAttrCtrl->GetElem(m_iElemNo, ElemD);
	//节点
	T_NODE_D NodeD[2];
	m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD[0]);
	m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD[1]);
	PunchData.iNodeNo = NodeD[0].z > NodeD[1].z ? ElemD.elnod[0] : ElemD.elnod[1];
	//柱
	PunchData.iPileNo = m_iElemNo;
	//板厚
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	m_pDoc->m_pAttrCtrl->GetConnectedElem(PunchData.iNodeNo, aElemList);
	T_ELEM_D SlabElemD;
	double min_thick = DBL_MAX;
	double dThk = 0.0;
	for (size_t i = 0; i < aElemList.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetElem(aElemList[i], SlabElemD);
		if (!m_pDoc->m_pAttrCtrl->IsPlate(SlabElemD.eltyp))
			continue;
		m_pDoc->m_pAttrCtrl->GetThik(ElemD.elpro, ThikD);
		m_pDoc->m_pAttrCtrl->GetThikForDesign(ThikD, dThk);
		if (min_thick > dThk)
		{
			min_thick = dThk;
			PunchData.dThk = dThk;
			PunchData.iSlabNo = aElemList[i];
		}
	}
	if (PunchData.dThk < cRCS_Zero)
		return FALSE;
	m_dThk = PunchData.dThk;
	return TRUE;
}

BOOL rusPunchLineCalc::Get_CutPerimeter(const _DGN_PUNCH_RES& PunchData, _DGN_PERI_PART& PeriPartD)
{
	//////////////////////////////////////////////
	// <Remember> Data甫 罐酒棵锭绰 Code Unit扁霖
	CCurUnitSaver Save(TRUE);
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CngIndex.nBase_Length = m_nCode_Length;
	CngIndex.nBase_Force = m_nCode_Force;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	// <Remember> Data甫 罐酒棵锭绰 Code Unit扁霖
	//////////////////////////////////////////////


	double dDangerDrat = 1.0 / FactorControlPerimeter;

	T_DCRB_D DcrbData;
	DcrbData.Initialize();
	m_DgnDataCtrl.Get_DgnConDcrb(m_iElemNo, DcrbData);

	double dPi = PI;
	
	unsigned int Index = 0;

	//PunchResData.GetNextAssoc(Pos, Index, PunchData);

	const int ElemK = PunchData.iSlabNo;

	m_dB = GetCoverBot_SlabShear(ElemK);
	m_dT = GetCoverTop_SlabShear(ElemK);

	int iPunchShp = 0;	// 1:(Rectangle,Squre,H), 2:Round.
	int iSuppType = 0;	// 1:Punching Size, 2:Column Supp, 3:Pile Supp, 4:Point Load.
	int iShape = 0;
	double dAngle = 0.0; // Beta angle.
	double dWidth = 0.0, dDepth = 0.0, dCoverThk = 0.0;

	if (Get_ColmSuppSize(PunchData.iPileNo, iShape, dAngle, dWidth, dDepth))	// Column Support.
	{
		iPunchShp = iShape;
		iSuppType = Get_LowerColmNo(PunchData.iNodeNo) > 0 ? COLUMN_SUPPORT : POINT_LOAD;  // MQC:3642-Seungjun-20100908 惑何 扁嫡捞 粮犁窍绰 版快.
		dCoverThk = iSuppType == POINT_LOAD ? m_dB : m_dT;
	}
	else
	{
		iPunchShp = 2;
		iSuppType = POINT_LOAD;
		dCoverThk = m_dB;
	}
	if (iPunchShp == 0)
	{
		return FALSE;
	}

	CArray<double, double> adInnerX;
	CArray<double, double> adInnerY;
	CArray<double, double> adOuterX;
	CArray<double, double> adOuterY;

	PeriPartD.Initialize();

	double dRotateSign = 1.0;
	int iDiv = DGN_PUNCH_SHEAR_DIV;
	if (iPunchShp == 1)	// Rectangle.
	{
		T_ELEM_D ElemD;
		if (m_pDoc->m_pAttrCtrl->GetElem(PunchData.iPileNo, ElemD))
		{
			T_NODE_D NodeD1;
			if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1))
			{
				NodeD1.Initialize();
			}
			T_NODE_D NodeD2;
			if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2))
			{
				NodeD2.Initialize();
			}

			if (NodeD1.z > NodeD2.z) { dRotateSign = -1.0; }
		}

		//保留Eurocode2_04的算法.
		//if (m_pMyDB->m_iDgnCode != Eurocode2_04)
		if(1)
		{
			int iArrSize = 4;
			double dInnerX = dDepth;
			double dInnerY = dWidth;
			double dOuterX = dInnerX + 2 * (PunchData.dThk - dCoverThk) / dDangerDrat;
			double dOuterY = dInnerY + 2 * (PunchData.dThk - dCoverThk) / dDangerDrat;

			m_dInnerX = dInnerX;
			m_dInnerY = dInnerY;

			PeriPartD.dPeriUnitX = dOuterX / 2.0;// PMS4603:DNKIM-20130222
			PeriPartD.dPeriUnitY = dOuterY / 2.0;

			for (int i = 0; i < iArrSize; i++)	// Counter-clockwise.
			{
				double dSignX = (i == 0 || i == iArrSize - 1 ? 1.0 : -1.0);
				double dSignY = (i < iArrSize / 2 ? 1.0 : -1.0);
				// Inner X,Y (1~4).						// 6 ------- 5
				adInnerX.Add(dSignX * dInnerX / 2.);	// | 2-----1 |
				adInnerY.Add(dSignY * dInnerY / 2.);	// | |     | |
				// Outer X,Y (5~8).						// | |     | |
				adOuterX.Add(dSignX * dOuterX / 2.);	// | 3-----4 |
				adOuterY.Add(dSignY * dOuterY / 2.);	// 7 ------- 8
			}
		}
		else  // Eurocode2:04
		{
			// EC2:04 6.4.2 figure 6.13
			int iArrSize = 4;
			double dInnerX = dDepth;
			double dInnerY = dWidth;
			double dOuterX = dInnerX + 2 * (PunchData.dThk - dCoverThk) / dDangerDrat;
			double dOuterY = dInnerY + 2 * (PunchData.dThk - dCoverThk) / dDangerDrat;
			double dD = 2 * (PunchData.dThk - dCoverThk);
			int iSt = 0, iEd = iDiv + 1;
			for (int i = 0; i < iArrSize; i++)	// Counter-clockwise.
			{
				double dSignX = (i == 0 || i == iArrSize - 1 ? 1.0 : -1.0);
				double dSignY = (i < iArrSize / 2 ? 1.0 : -1.0);
				double dInX = dSignX * dInnerX / 2.;
				double dInY = dSignY * dInnerY / 2.;
				// Inner X,Y (1~4).					//    11,10 9-----8 7,6   => 6,7,10,11 round          
				//	adInnerX.Add(dInX);             //       12 2-----1 5
				//	adInnerY.Add(dInY);             //        | |     | |
				// Outer X,Y (5~20) 				//        | |     | |
				for (int j = iSt; j < iEd; j++)     //       13 3-----4 20
				{	                                //    14,15 16---17 18,19 => 14,15,18,19 round
					double dOutX = dInX + dD * cos(j * (2 * dPi / (iDiv * 4)));
					double dOutY = dInY + dD * sin(j * (2 * dPi / (iDiv * 4)));
					adOuterX.Add(dOutX);
					adOuterY.Add(dOutY);
					// Inner客 Outer狼 农扁啊 鞍酒具 窍扁 锭巩.
					adInnerX.Add(dInX);
					adInnerY.Add(dInY);
				}
				iSt += iDiv;
				iEd += iDiv;
			}
		}
	}
	else if (iPunchShp == 2)	// Round.
	{
		int iArrSize = 24;
		double dInnerR = 0.5 * dDepth;
		double dOuterR = dInnerR + (PunchData.dThk - dCoverThk) / dDangerDrat;
		PeriPartD.dPeriUnitX = dOuterR;
		PeriPartD.dPeriUnitY = dInnerR;
		for (int i = 0; i < iArrSize; i++)	// Counter-clockwise.
		{
			// Coded by Seungjun ('20070626) MNet:No.2883. dInnerD -> (dInnerD/2.)
			// Inner X,Y.
			double dInnerX = dInnerR * cos(i * (2 * dPi / iArrSize));
			double dInnerY = dInnerR * sin(i * (2 * dPi / iArrSize));
			adInnerX.Add(dInnerX);
			adInnerY.Add(dInnerY);
			// Coded by Seungjun ('20070626) MNet:No.2883. dOuterD -> (dOuterD/2.)
			// Outer X,Y.
			double dOuterX = dOuterR * cos(i * (2 * dPi / iArrSize));
			double dOuterY = dOuterR * sin(i * (2 * dPi / iArrSize));
			adOuterX.Add(dOuterX);
			adOuterY.Add(dOuterY);
		}
	}
	else
	{
		ASSERT(0);
	}

	// Get Pile Position.
	T_NODE_D NodeD; NodeD.Initialize();
	m_pDoc->m_pAttrCtrl->GetNode(PunchData.iNodeNo, NodeD);
	double dPilePosX = NodeD.x;
	double dPilePosY = NodeD.y;

	// Change Unit (Deg -> Rad).
	dAngle *= (dPi / 180.0);
	// Save Data.
	PeriPartD.iPileNo = PunchData.iPileNo;
	PeriPartD.iNodeNo = PunchData.iNodeNo;
	ASSERT(adInnerX.GetSize() == adOuterX.GetSize());
	ASSERT(adInnerY.GetSize() == adOuterY.GetSize());
	ASSERT(adInnerX.GetSize() == adInnerY.GetSize());
	ASSERT(adOuterX.GetSize() == adOuterY.GetSize());

	_DGN_PERI_POS PosDataInn, PosDataOut;
	int iNoSize = adOuterX.GetSize();
	for (int i = 0; i < iNoSize; i++)
	{
		// Inner Positions (NOT-Rotated Data).
		PosDataInn.Initialize();
		for (int j = 0; j < 2; j++)	// 0=StaNo, 1=EndNo.
		{
			int iArrNo = (j == 0 ? i : (i < iNoSize - 1 ? i + 1 : 0));
			double dCx = adInnerX.GetAt(iArrNo);
			double dCy = adInnerY.GetAt(iArrNo);
			PosDataInn.dCx[j] = dCx + dPilePosX;
			PosDataInn.dCy[j] = dCy + dPilePosY;
		}
		PeriPartD.aInnList.Add(PosDataInn);
		// Outer Positions (Rotated Data).
		PosDataOut.Initialize();
		for (int j = 0; j < 2; j++)	// 0=StaNo, 1=EndNo.
		{
			int iArrNo = (j == 0 ? i : (i < iNoSize - 1 ? i + 1 : 0));
			double dCx = adOuterX.GetAt(iArrNo);
			double dCy = adOuterY.GetAt(iArrNo);
			double dLen = sqrt(pow(dCx, 2) + pow(dCy, 2));
			double dAngOrg = (dCx == 0.0 ? dPi / 2.0 : atan(fabs(dCy / dCx)));
			double dAng = 0.0;
			if (dCx >= 0.0 && dCy >= 0.0)		dAng = dAngOrg + dRotateSign * dAngle;	// 1荤盒搁.
			else if (dCx < 0.0 && dCy >= 0.0)	dAng = dPi - dAngOrg + dRotateSign * dAngle;	// 2荤盒搁.
			else if (dCx < 0.0 && dCy < 0.0)		dAng = dPi + dAngOrg + dRotateSign * dAngle;	// 3荤盒搁.
			else if (dCx >= 0.0 && dCy < 0.0)	dAng = 2 * dPi - dAngOrg + dRotateSign * dAngle;	// 4荤盒搁.
			else	ASSERT(0);
			PosDataOut.dCx[j] = dLen * cos(dAng) + dPilePosX;
			PosDataOut.dCy[j] = dLen * sin(dAng) + dPilePosY;
			if (fabs(PosDataOut.dCx[j]) < cRCS_Zero)	PosDataOut.dCx[j] = 0.0;
			if (fabs(PosDataOut.dCy[j]) < cRCS_Zero)	PosDataOut.dCy[j] = 0.0;
		}
		PeriPartD.aPosList.Add(PosDataOut);
	}

	//int iPosi = Get_Position(iPunchShp, PeriPart);

	//CString strPileNo = _T("");  strPileNo.Format(_T("%d"), (int)(PeriPartD.iPileNo + 10000));
	//CString strPileKey = _T(""); strPileKey.Format(_T("PCHK%4s-%d"), strPileNo, PeriPartD.iNodeNo);
	PeriPartD.iPunchShp = iPunchShp;
	PeriPartD.iSuppType = iSuppType;
	PeriPartD.dRotate = dAngle;

	return TRUE;
}

// Make Punching shear force/stress data.
/*
* 计算OutLine所在的板.板单元号存到 _DGN_PERI_PART.aResList中.同时包含了经过这块板的线段位置
* 
*/
BOOL rusPunchLineCalc::SubMakePeripartForTwowayForceShearCheck(_DGN_PERI_PART& PeriPartD)
{
	//CDgnProgressDlg ProDlg;
	//ProDlg.Create_ProgressDlg(PROGRESS_TYPE_RC_SLAB_PUNCHING_MAKE_FORCE);

	//////////////////////////////////////////////
	// <Remember> Data甫 罐酒棵锭绰 Code Unit扁霖
	CCurUnitSaver Save(TRUE);
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CngIndex.nBase_Length = m_nCode_Length;
	CngIndex.nBase_Force = m_nCode_Force;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	//////////////////////////////////////////////

	const auto dLenTor = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(m_nCode_Force, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 1.0e-3); // 1レm = 1/1000mm

	// [GEN-2013] 困氰窜搁俊 吧府绰 夸家甸阑 八祸且 锭绰 措惑 夸家甫 傈眉 夸家肺 钦聪促. (before : 汲拌 措惑 夸家)
	T_ELEM_K_LIST aPlatElemK;
	aPlatElemK.RemoveAll();
	GetAllPlateListForShear(aPlatElemK);
	m_QuadTree.MakePlateQuadTree(aPlatElemK);

	//if (m_PlateKeyList.GetSize() == 0) { return FALSE; }

	/// [GEN-2822] 困氰窜搁俊 吧府绰 夸家甸捞 咯矾 辆幅狼 窍吝炼钦阑 啊柳 版快, 汲拌甫 荐青窍瘤 臼嚼聪促. (skip)
	/// skip 窍绰 何犁 沥焊甸篮 酒贰狼 array俊 葛酒底促啊 message window俊 茄锅俊 免仿窍档废.
	CStringArray astrSkipMsg;

	int iResCount = 0;
	BOOL bBreak = FALSE;


	T_NODE_D NodeD;
	if (!m_pDoc->m_pAttrCtrl->GetNode(PeriPartD.iNodeNo, NodeD)) { ASSERT(0); }
	double dLevel = NodeD.z;

	const INT_PTR nSizePos = PeriPartD.aPosList.GetSize();

	CArray<int, int> aPeriSectNum;
	aPeriSectNum.RemoveAll();
	aPeriSectNum.SetSize(nSizePos);

	CArray<T_ELEM_K, T_ELEM_K> arIntsecElem;
	double dCutLinePoint1[3] = { 0.0, };
	double dCutLinePoint2[3] = { 0.0, };
	if (PeriPartD.iPunchShp == 1)  // round 鞘夸绝绰 何盒 贸府.
	{
		for (INT_PTR nCL = 0; nCL < nSizePos; ++nCL)
		{
			GetCutLinePoints(PeriPartD.aPosList[nCL], dLevel, dCutLinePoint1, dCutLinePoint2);

			arIntsecElem.RemoveAll();
			m_QuadTree.CalcIntersectElem(dCutLinePoint1, dCutLinePoint2, arIntsecElem);

			// check position
			aPeriSectNum[nCL] = static_cast<int>(arIntsecElem.GetSize());
		}
		
		Set_CuttingLine(m_QuadTree, aPeriSectNum, PeriPartD);

		aPeriSectNum.RemoveAll();
		aPeriSectNum.SetSize(nSizePos);
	}

	for (INT_PTR nCL = 0; nCL < nSizePos; ++nCL)
	{
		GetCutLinePoints(PeriPartD.aPosList[nCL], dLevel, dCutLinePoint1, dCutLinePoint2);
		// ---------------------------------------------------
		arIntsecElem.RemoveAll();
		m_QuadTree.CalcIntersectElem(dCutLinePoint1, dCutLinePoint2, arIntsecElem);
		int nElemNum = static_cast<int>(arIntsecElem.GetSize());

		// check position
		aPeriSectNum[nCL] = nElemNum;
//
		for (int elem = 0; elem < nElemNum; elem++)
		{
			_DGN_PERI_RES PeriResD;
			T_ELEM_D elem_d;
			if (!m_pDoc->m_pAttrCtrl->GetElem(arIntsecElem[elem], elem_d)) { ASSERT(0); continue; }
			if (!m_pDoc->m_pAttrCtrl->IsPlate(elem_d.eltyp))continue; // Plate酒聪搁 烹苞 

			// MNET:4599-HSSHIM-20120414 : Punching point客 悼老 乞搁俊 乐瘤 臼篮 elem 力寇 
			if (!m_DgnDataCtrl.IsNodeOnElemPlane(NodeD, elem_d))  continue;

			const bool bExcludeSeis = false;

			int node_in_elem = GetPlateNodeNum(arIntsecElem[elem], elem_d);
			//_DGN_LCOM LcomDesign;
			//LcomDesign.Initialize();
			//m_parLcomData->Lookup(l + 1, LcomDesign);

			CDgnCalcCutlLine CalcCutl, CalcCutlM;
			CalcCutl.Initialize();
			CalcCutl.SetLenTol(dLenTor);
			CalcCutl.AddLine(dCutLinePoint1[0], dCutLinePoint1[1], dCutLinePoint1[2]);
			CalcCutl.AddLine(dCutLinePoint2[0], dCutLinePoint2[1], dCutLinePoint2[2]);
			CalcCutlM.Initialize();
			CalcCutlM.SetLenTol(dLenTor);
			CalcCutlM.AddLine(dCutLinePoint1[0], dCutLinePoint1[1], dCutLinePoint1[2]);
			CalcCutlM.AddLine(dCutLinePoint2[0], dCutLinePoint2[1], dCutLinePoint2[2]);

			for (int i = 0; i < node_in_elem; i++)
			{
				T_NODE_D node_d1, node_d2;
				if (!m_pDoc->m_pAttrCtrl->GetNode(elem_d.elnod[i], node_d1)) { continue; }
				int j = (i == node_in_elem - 1) ? 0 : (i + 1);
				if (!m_pDoc->m_pAttrCtrl->GetNode(elem_d.elnod[j], node_d2)) { continue; }

				double dPoint1[3] = { node_d1.x, node_d1.y, dLevel };
				double dPoint2[3] = { node_d2.x, node_d2.y, dLevel };
				double dPlateCutVal[2] = { 0.0,0.0 };
				CalcCutl.AddData(dPoint1[0], dPoint1[1], dPoint1[2], dPlateCutVal[0]);
				// Set Moment
				CalcCutlM.AddData(dPoint1[0], dPoint1[1], dPoint1[2], 0.0);
			}

			double dCalcX[2], dCalcY[2], dCalcZ[2], dValues[2];
			if (!CalcCutl.CalcCultLine(dCalcX, dCalcY, dCalcZ, dValues))continue;
			double dCalcXM[2], dCalcYM[2], dCalcZM[2], dValuesM[2];
			if (!CalcCutlM.CalcCultLine(dCalcXM, dCalcYM, dCalcZM, dValuesM)) { dValuesM[0] = 0.0; dValuesM[1] = 0.0; }

			for (int i = 0; i < 2; i++)
			{
				PeriResD.dCx[i] = dCalcX[i];
				PeriResD.dCy[i] = dCalcY[i];
				PeriResD.arVu[i].Add(dValues[i]);
				PeriResD.arMu[i].Add(dValuesM[i]);
			}
			PeriResD.arLcomNo.Add(0);
			if (PeriResD.arLcomNo.GetSize() > 0)
			{
				PeriResD.iSlabNo = arIntsecElem[elem];
				// Seungjun-20090506 目泼扼牢阑 2俺狼 浇扼宏啊 傍蜡且 锭 鞘夸.
				//if (IsExistSamePeriResData(PeriPartD.iNodeNo, PeriResD, PeriPartD.aResList))
				//{
				//	continue;
				//}
				//else
				{
					PeriPartD.aResList.Add(PeriResD);
				}
			}
		}
	}


	return TRUE;
}

double rusPunchLineCalc::GetCoverBot_SlabShear(int ElemK)
{
	double dT = 0.0;
	CArray<T_RBMS_D, T_RBMS_D&> aRbmsD;
	if (m_DgnDataCtrl.Get_DgnConRbms(ElemK, aRbmsD, m_nCode_Length))
	{
		dT = (aRbmsD[EN_TOP_DIR1].dCover + aRbmsD[EN_TOP_DIR2].dCover) / 2.0;
	}
	return dT;
}

double rusPunchLineCalc::GetCoverTop_SlabShear(int ElemK)
{
	CArray<T_RBMS_D, T_RBMS_D&> aRbmsD;
	double dB = 0.0;
	if (m_DgnDataCtrl.Get_DgnConRbms(ElemK, aRbmsD, m_nCode_Length))
	{
		dB = (aRbmsD[EN_BOT_DIR1].dCover + aRbmsD[EN_BOT_DIR2].dCover) / 2.0;
	}
	return dB;
}

BOOL rusPunchLineCalc::Get_ColmSuppSize(int iColmNo, int& iShape, double& dAngle, double& dWidth, double& dDepth)
{
	iShape = 0;
	dAngle = 0.0;
	dWidth = 0.0;
	dDepth = 0.0;

	if (iColmNo < 1) { return FALSE; }

	T_ELEM_D ElemD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(iColmNo, ElemD)) { return FALSE; }
	T_SECT_D SectD;
	if (!m_DgnDataCtrl.Get_DgnStlSect(iColmNo, SectD)) { return FALSE; }

	const CString& strSectShape = SectD.SectBefore.Shape;
	iShape = [strSectShape]() -> int
		{
			if (strSectShape == D_SECT_SHAPE_REG_SB) return 1;
			if (strSectShape == D_SECT_SHAPE_REG_B) return 1;
			if (strSectShape == D_SECT_SHAPE_REG_H) return 1;
			if (strSectShape == D_SECT_SHAPE_REG_SR) return 2;
			if (strSectShape == D_SECT_SHAPE_REG_P) return 2;
			if (strSectShape == D_SECT_SHAPE_SRC_RIB) return 1;
			if (strSectShape == D_SECT_SHAPE_SRC_RH2T) return 1;
			if (strSectShape == D_SECT_SHAPE_SRC_RHT) return 1;
			if (strSectShape == D_SECT_SHAPE_SRC_RBO) return 1;
			if (strSectShape == D_SECT_SHAPE_SRC_RBC) return 1;
			if (strSectShape == D_SECT_SHAPE_SRC_RPO) return 1;
			if (strSectShape == D_SECT_SHAPE_SRC_RPC) return 1;
			if (strSectShape == D_SECT_SHAPE_SRC_EBC) return 1;
			if (strSectShape == D_SECT_SHAPE_REG_T) return 1;
			if (strSectShape == D_SECT_SHAPE_REG_L) return 1;
			if (strSectShape == D_SECT_SHAPE_REG_C) return 1;
			if (strSectShape == D_SECT_SHAPE_REG_2L) return 1;
			if (strSectShape == D_SECT_SHAPE_REG_2C) return 1;
			if (strSectShape == D_SECT_SHAPE_REG_CC) return 1;
			if (strSectShape == D_SECT_SHAPE_REG_URIB) return 1;
			if (strSectShape == D_SECT_SHAPE_REG_UDT) return 1;
			if (strSectShape == D_SECT_SHAPE_SRC_EPC) return 2;
			if (strSectShape == D_SECT_SHAPE_SRC_EPO) return 2;
			if (strSectShape == D_SECT_SHAPE_SRC_CIB) return 2;
			if (strSectShape == D_SECT_SHAPE_SRC_CBO) return 2;
			if (strSectShape == D_SECT_SHAPE_SRC_CBC) return 2;
			if (strSectShape == D_SECT_SHAPE_SRC_CPO) return 2;
			if (strSectShape == D_SECT_SHAPE_SRC_CPC) return 2;
			if (strSectShape == D_SECT_SHAPE_SRC_BSTF) return 1;
			if (strSectShape == D_SECT_SHAPE_SRC_PSTF) return 2;
			return 2;
		}();


	double dWidth1 = SectD.SectBefore.SectI.Size[1];
	double dDepth1 = SectD.SectBefore.SectI.Size[0];
	double dWidth2 = SectD.SectBefore.SectJ.Size[1];
	double dDepth2 = SectD.SectBefore.SectJ.Size[0];
	if (strSectShape == D_SECT_SHAPE_REG_2L || strSectShape == D_SECT_SHAPE_REG_2C)
	{
		dWidth1 = 2.0 * SectD.SectBefore.SectI.Size[1] + SectD.SectBefore.SectI.Size[4];
	}
	else if (strSectShape == D_SECT_SHAPE_REG_UDT)
	{
		dWidth1 = SectD.SectBefore.SectI.Size[1] + SectD.SectBefore.SectI.Size[2] + SectD.SectBefore.SectI.Size[3];
	}

	dWidth = max(dWidth1, dWidth2);
	dDepth = max(dDepth1, dDepth2);
	T_CCPA_D CCpaD;
	if (m_pDoc->m_pAttrCtrl2->GetCcpa(iColmNo, CCpaD))
	{
		T_CCPT_K CCptK = CCpaD.CcptK;
		T_CCPT_D CCptD;
		if (m_pDoc->m_pAttrCtrl2->GetCcpt(CCptK, CCptD))
		{
			double dHalfB = dWidth / 2.0;
			double dHalfD = dDepth / 2.0;

			double dCapB1 = max(dHalfB, CCptD.dB1);
			double dCapB2 = max(dHalfB, CCptD.dB2);
			double dCapD1 = max(dHalfD, CCptD.dH1);
			double dCapD2 = max(dHalfD, CCptD.dH2);

			dWidth = dCapB1 + dCapB2;
			dDepth = dCapD1 + dCapD2;
		}
	}

	dAngle = ElemD.angle;

	return (iShape == 0) ? FALSE : TRUE;
}

UINT rusPunchLineCalc::Get_LowerColmNo(UINT NodeK)
{
	UINT ElemK = 0;

	CArray<T_ELEM_K, T_ELEM_K> arElemList;
	m_pDoc->m_pAttrCtrl->GetConnectedElem(NodeK, arElemList);  // 例痢俊 楷搬等 element狼 key甫 啊廉柯促.

	T_NODE_D JointNode;
	m_pDoc->m_pAttrCtrl->GetNode(NodeK, JointNode);

	BOOL bIsJUD = m_DgnDataCtrl.IsJUD();
	int nElem = arElemList.GetSize();
	for (int IEL = 0; IEL < nElem; IEL++)
	{
		T_ELEM_K KeyElem = arElemList.GetAt(IEL);
		int iCBM = bIsJUD ? m_pDoc->m_pAttrCtrl->GetMemberType_JUD(KeyElem) : m_pDoc->m_pAttrCtrl->GetMemberType(KeyElem);
		if (iCBM == D_MBTP_COLUMN || iCBM == D_MBTP_BRACE) // Seungjun-20140218 : 戚莫 八配矫 truss档 器窃
		{
			T_ELEM_D elemD; elemD.Initialize();
			m_pDoc->m_pAttrCtrl->GetElem(KeyElem, elemD);

			T_NODE_D OppsiNode; OppsiNode.Initialize();
			if (elemD.elnod[0] == NodeK)
			{
				m_pDoc->m_pAttrCtrl->GetNode(elemD.elnod[1], OppsiNode);
			}
			else
			{
				m_pDoc->m_pAttrCtrl->GetNode(elemD.elnod[0], OppsiNode);
			}

			if (JointNode.z > OppsiNode.z)
			{
				ElemK = arElemList[IEL];
				if (iCBM == D_MBTP_COLUMN || IEL == (nElem - 1)) // brace焊促 column 快急
				{
					break;
				}
			}
		}
	}

	return ElemK;
}

int rusPunchLineCalc::GetAllPlateListForShear(CArray<UINT, UINT>& aAllKeys, BOOL bWall /*= FALSE*/)
{
	aAllKeys.RemoveAll();
	std::vector<T_MADO_K> aMadoKeys; // Main domain key list. (汲拌矫绰 sub-domain父 荤侩)
	m_pDoc->m_pAttrCtrl->GetMadoKeyList(aMadoKeys);
	int nMado = static_cast<int>(aMadoKeys.size());
	if (nMado == 0)  return 0;
	for (int k = 0; k < nMado; k++)
	{
		std::vector<T_SBDO_K> aSbdoKeys; // Sub domain key list. (汲拌矫绰 sub-domain父 荤侩)
		T_MADO_K MadoK = aMadoKeys[k];
		int nSbdo = m_pDoc->m_pAttrCtrl->GetSbdoKeyList(MadoK, aSbdoKeys);
		if (nSbdo == 0)  continue;
		CVectorUtil::Sort(aSbdoKeys);
		for (int i = 0; i < nSbdo; i++)
		{
			T_SBDO_K SbdoK = aSbdoKeys[i];
			//m_aMadoBySbdo.SetAt(SbdoK, MadoK);

			T_SBDO_D SbdoD;
			if (!m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD))  continue;
			if (SbdoD.IsShell()) continue;

			if (!bWall)
			{
				if (SbdoD.IsNone() || SbdoD.IsWall())           continue;
			}
			else
			{
				if (!SbdoD.IsWall())                            continue;
			}

			CArray<T_ELEM_K, T_ELEM_K> aSbdoElemKeys;
			aSbdoElemKeys.RemoveAll();
			int nSubElem = m_pDoc->m_pAttrCtrl->GetElemListSbdoK(aSbdoKeys[i], aSbdoElemKeys);

			aAllKeys.Append(aSbdoElemKeys);
		}
	}

	return static_cast<int>(aAllKeys.GetSize());
}

void rusPunchLineCalc::GetCutLinePoints(const _DGN_PERI_POS& PeriPos, double dLevel, OUT double adCutPnt1[3], OUT double adCutPnt2[3])
{
	adCutPnt1[0] = PeriPos.dCx[0];
	adCutPnt1[1] = PeriPos.dCy[0];
	adCutPnt1[2] = dLevel;

	adCutPnt2[0] = PeriPos.dCx[1];
	adCutPnt2[1] = PeriPos.dCy[1];
	adCutPnt2[2] = dLevel;
}

void rusPunchLineCalc::Set_CuttingLine(CDgnQuadTree& QuadTree, CArray<int, int>& aPeriSectNum, _DGN_PERI_PART& PeriPartD)
{
	//if (m_iDgnCode != Eurocode2_04)          return;
	if (PeriPartD.aPosList.GetSize() != (DGN_PUNCH_SHEAR_DIV * 4 + 4))  return; // round啊 6殿盒等 版快烙.

	// Check the Position.
	BOOL bTop = TRUE, bBottom = TRUE, bRight = TRUE, bLeft = TRUE;
	int  nCount = aPeriSectNum.GetSize();
	int  iDiv = (nCount - 4) / 4;
	int  iPos[4] = { iDiv, 2 * iDiv + 1, 3 * iDiv + 2, 4 * iDiv + 3 }; // Top,Left,Bottom,Right.

	//  1----------0
	//  |          |
	//  |          |
	//  |          |
	//  2----------3
	double Point[4][2];
	for (int i = 0; i < 4; i++)
	{
		int j = 0;
		if (i == 0)       j = 3;
		else if (i == 1)  j = 0;
		else if (i == 2)  j = 1;
		else if (i == 3)  j = 2;

		// make rectangle points.
		double line1[2][2];
		double line2[2][2];

		// point 1
		line1[0][0] = PeriPartD.aPosList[iPos[i]].dCx[0];
		line1[0][1] = PeriPartD.aPosList[iPos[i]].dCy[0];
		line1[1][0] = PeriPartD.aPosList[iPos[i]].dCx[1];
		line1[1][1] = PeriPartD.aPosList[iPos[i]].dCy[1];

		line2[0][0] = PeriPartD.aPosList[iPos[j]].dCx[0];
		line2[0][1] = PeriPartD.aPosList[iPos[j]].dCy[0];
		line2[1][0] = PeriPartD.aPosList[iPos[j]].dCx[1];
		line2[1][1] = PeriPartD.aPosList[iPos[j]].dCy[1];

		CMathFunc::mathLineLineCross2D(line1, line2, Point[i]);
	}

	const double dTolerance = 1e-8;    // Point 困摹俊 乐绰 夸家甫 茫绰单 荤侩登绰 Point狼 困摹 康开 拌魂侩
	for (int i = 0; i < 4; i++)
	{
		// MNET:XXXX-HSSHIM-20091203
		// 流荤阿屈狼 怖瘤痢捞 乐绰 困摹俊 夸家啊 绝栏搁 => 流急栏肺 函版 
		//                                  乐栏搁 => round肺 蜡瘤
		DgnRect rcPoint_corner;
		rcPoint_corner.Set(Point[i][0] - dTolerance, Point[i][1] + dTolerance, Point[i][0] + dTolerance, Point[i][1] - dTolerance);
		CArray <CDgn_Object*, CDgn_Object*> arDgnObj;   arDgnObj.RemoveAll();
		QuadTree.GetObjectsIntersectArea(rcPoint_corner, arDgnObj);

		BOOL bChange2StraightEdge = FALSE;
		if (arDgnObj.GetSize() == 0)    bChange2StraightEdge = TRUE;

		if (bChange2StraightEdge)
		{
			_DGN_PERI_POS PosD_moveTo;    // 邦急阑 促矫 流急栏肺 父甸扁 困秦 捞悼秦具 且 困摹 (背瞒痢)
			PosD_moveTo.dCx[0] = Point[i][0];
			PosD_moveTo.dCx[1] = Point[i][0];
			PosD_moveTo.dCy[0] = Point[i][1];
			PosD_moveTo.dCy[1] = Point[i][1];

			// 邦急何盒阑 颗变促.
			int iTemp = 0;
			for (int k = 0; k < iDiv; ++k)
			{
				iTemp = i * (iDiv + 1) + k;
				PeriPartD.aPosList[iTemp] = PosD_moveTo;
			}

			// 流急 何盒狼 矫累痢阑 颗变促
			iTemp = iPos[i];
			PeriPartD.aPosList[iTemp].dCx[0] = PosD_moveTo.dCx[0];
			PeriPartD.aPosList[iTemp].dCy[0] = PosD_moveTo.dCy[0];

			// 流急 何盒狼 场痢阑 颗变促.
			iTemp = iPos[(i + 3) % 4];
			PeriPartD.aPosList[iTemp].dCx[1] = PosD_moveTo.dCx[1];
			PeriPartD.aPosList[iTemp].dCy[1] = PosD_moveTo.dCy[1];
		}
	}
}

int  rusPunchLineCalc::GetPlateNodeNum(const T_ELEM_K& ElemK, const T_ELEM_D& ElemD)
{
	int nie = 0;
	if (!m_mapNodeInElem.Lookup(ElemK, nie))
	{
		nie = m_DgnDataCtrl.Get_Node_in_Elem(const_cast<T_ELEM_D&>(ElemD));
		m_mapNodeInElem.SetAt(ElemK, nie);
	}

	return nie;
}