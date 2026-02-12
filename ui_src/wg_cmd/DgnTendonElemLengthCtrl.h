#pragma once

#include <vector>
#include <map>

#include "..\wg_db\AttrCtrl.h"

#include "HeaderPre.h"

struct lengthInfo
{
	double dPosX;
	double dTop_Z;
	double dBot;
	double dLeft;
	double dRight_Y;

	lengthInfo()
	{
		dPosX	= 0.0;
		dTop_Z	= 0.0;
		dBot	= 0.0;
		dLeft	= 0.0;
		dRight_Y	= 0.0;
	}
};

struct lengthInfoIJ
{
	T_ELEM_K	nElemKey;
	lengthInfo	infoEndI;
	lengthInfo	infoEndJ;
};

typedef std::vector<T_ELEM_K>				vecElemKey;
typedef std::vector<lengthInfo>				vecLengthInfo;
typedef std::vector<lengthInfoIJ>			vecLengthInfoIJ;

class CUnitCtrl;
class __MY_EXT_CLASS__ CDgnTendonElemLengthCtrl
{
public:
	CDgnTendonElemLengthCtrl(CAttrCtrl* pAttrCtrl);
	~CDgnTendonElemLengthCtrl(void);

	BOOL	SetElemList(vecElemKey& elemKey, BOOL& bAllSameSect, T_SECT_K& key);
	double	SetCalcLengthInfo(const vecElemKey& elemKey, BOOL bCentroid);	//계산결과의 총 길이 리턴.

	double	GetTotalLength() { return m_dTotalLength; }
	
	T_ELEM_K GetElemKeyByIdx(int nIdx, BOOL bCentroid = FALSE);
	int		GetLengthInfoCount(BOOL bCentroid = FALSE);
	BOOL	GetLengthInfoByIdx(int nIdx, lengthInfoIJ& info, BOOL bCentroid = FALSE);
	BOOL	GetLengthInfoByPos(double dPosX, lengthInfo& info, BOOL bCentroid = FALSE);
	BOOL	GetLengInfoByElem(T_ELEM_K key, lengthInfo* info_I, lengthInfo* info_J, BOOL bCentroid = FALSE);
	double  GetLengthRatioByPos(BOOL bXY, const double& dPosX, const double& dRatio);

	BOOL	GetHeightWidthByElem( T_ELEM_K key, double& dHeight, double& dWidth, double& dGapY, double& dGapZ, double dPos );

	BOOL GetCircleStartCenterPos( T_ELEM_K keyStart, T_ELEM_K keyMid, T_ELEM_K keyEnd, double* pPosStart, double* pPosCenter, double& dRadX, int& nCWType); //nCCW 0 : CW, 1: CCW

	BOOL IsCurve(std::vector<T_ELEM_K>& vecElemKey);
	BOOL IsAllSameSect(const vecElemKey& elemKey, T_SECT_K& key);

private:
	BOOL	AlignElemList(vecElemKey& elemKey);
	void	ExcludeDuplicate(vecElemKey& elemKey);
	BOOL	IsSameLengthInfo(const lengthInfo& info1, const lengthInfo& info2, BOOL bWithLength = FALSE);

private:
	CAttrCtrl*		m_pAttrCtrl;
	CUnitCtrl*    m_pUnitCtrl;

	vecLengthInfoIJ	m_vecInfo_ElemOffset;
	vecLengthInfoIJ	m_vecInfo_Centroid;

	double			m_dTotalLength;
};

#include "HeaderPost.h"