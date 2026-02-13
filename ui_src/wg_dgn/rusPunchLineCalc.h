#pragma once
#include "DgnStruct.h"
#include "DgnQuadTree.h"
#include "DgnDataCtrl.h"
//Calc Punch Line is Thread-safe : 读取数据如果没有从文件中读取,应该是线程安全的.从文件中读取,缓存会导致不安全
#include "HeaderPre.h"


class CDBDoc;

class __MY_EXT_CLASS__ rusPunchLineCalc
{
public:
	rusPunchLineCalc();
	//最后的Circle
	BOOL CalcAndSavePunchLline(int nColmElem, std::vector<T_RSS2_CTLN>& arPnChkCutl);
	//如果是Circle,dX=dY
	BOOL GetPunchInfo(const std::vector<T_RSS2_CTLN>& arPnChkCutl, double& dX, double& dY, int& iLoad2Flat);
private:
	BOOL InitPunchResData(_DGN_PUNCH_RES& PunchData);
	BOOL Get_CutPerimeter(const _DGN_PUNCH_RES& PunchData, _DGN_PERI_PART& PeriPartD);
	BOOL SubMakePeripartForTwowayForceShearCheck(_DGN_PERI_PART& PeriPartD);
private:
	double GetCoverBot_SlabShear(int ElemK);
	double GetCoverTop_SlabShear(int ElemK);
	BOOL Get_ColmSuppSize(int iColmNo, int& iShape, double& dAngle, double& dWidth, double& dDepth);
	UINT Get_LowerColmNo(UINT NodeK);
	int    GetAllPlateListForShear(CArray<UINT, UINT>& aAllKeys, BOOL bWall = FALSE);
	void GetCutLinePoints(const _DGN_PERI_POS& PeriPos, double dLevel, OUT double adCutPnt1[3], OUT double adCutPnt2[3]);
	void   Set_CuttingLine(CDgnQuadTree& QuadTree, CArray<int, int>& aPeriSectNum, _DGN_PERI_PART& PeriPartD);
	int  GetPlateNodeNum(const T_ELEM_K& ElemK, const T_ELEM_D& ElemD);
private:
	CDBDoc* m_pDoc;
	int m_nCode_Length;
	int m_nCode_Force;
	double FactorControlPerimeter;
	int m_iElemNo;
	int m_NodeKey;
	int m_dB;
	int m_dT;
	double m_dInnerX;
	double m_dInnerY;
	double m_dThk;
	CDgnDataCtrl m_DgnDataCtrl;
	CDgnQuadTree m_QuadTree;
	CMap<T_ELEM_K, T_ELEM_K, int, int>	m_mapNodeInElem;
	CArray <unsigned int, unsigned int>		m_PlateKeyList;
	CMapEx<T_SBDO_K, T_SBDO_K, T_MADO_K, T_MADO_K> m_aMadoBySbdo;

	
};
#include "HeaderPost.h"