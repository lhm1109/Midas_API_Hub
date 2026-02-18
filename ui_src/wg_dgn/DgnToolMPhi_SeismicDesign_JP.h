#pragma once

#include "..\wg_db\DbToolMphi_JP.h"

struct ColumnData;
class CDgnProgressDlg;

struct ParamDelta_y
{
	T_SIPA_K kSipa;
	T_NODE_K kTopNode;
	T_SIGR_LOWER_K kSigrLower;
	CString colmGroup;
	BOOL bMajorAxis;
	int nCheckDir;		//해석방향 enum ParamDelta_y::eDirection
	//여기서 부터는 내부 계산과정에서 만들어진 것들
	MPHI_DATA_ELEM InData;
	double dMembHeight;
	int iTopPosi;
	int iBottomPosi;

	ParamDelta_y() { Initialize(); }
	ParamDelta_y(const ParamDelta_y& src) { *this = src; }
	ParamDelta_y& operator=(const ParamDelta_y& src)
	{
		kSipa = src.kSipa;
		kTopNode = src.kTopNode;
		kSigrLower = src.kSigrLower;
		colmGroup = src.colmGroup;
		bMajorAxis = src.bMajorAxis;
		nCheckDir = src.nCheckDir;
		dMembHeight = src.dMembHeight;
		iTopPosi = src.iTopPosi;
		iBottomPosi = src.iBottomPosi;
		return *this;
	}

	void Initialize()
	{
		kSipa = 0;
		kTopNode = 0;
		kSigrLower = 0;
		colmGroup.Empty();
		bMajorAxis = FALSE;
		nCheckDir = 0;
		InData.Initialize();
		dMembHeight = 0.0;
		iTopPosi = 0;
		iBottomPosi = 0;
	}

	enum eDirection
	{
		kUnKnown,
		kPlus,
		kMinus,
	};
};

class CDgnToolMPhi_SeismicDesign_JP : public CDbToolMphi_JP
{
private:
	CDBDoc* m_pDoc;
	ParamDelta_y p_dy;

public:
	CDgnToolMPhi_SeismicDesign_JP();
	virtual ~CDgnToolMPhi_SeismicDesign_JP();

	BOOL GetLp(double d_h, const ColumnData* pColumn, OUT double& dLp);

	/**
	* 상부구조 관성력 작용위치에서 작용했을 때의 곡률분포로 부터 구한 수평변위
	* param : 계산을 위한 입력조건
	* aDeltaY[2] : δy0(초기항복상태)			[0] : MajorAxis, [1] : PerpAxis
	* aDeltaU[2] : δu, δls(한계[종국]상태)	[0] : MajorAxis, [1] : PerpAxis
	*/
	BOOL CalcHoriDisp(IN ParamDelta_y& param, OUT double* aDeltaY, OUT double* aDeltaU);

private:
 	BOOL CalcDelta_y(IN ParamDelta_y& param, OUT double& dDelta_y, OUT double& dDelta_u);

 	BOOL Get_hAndElemKeysOfShortColumn(OUT double& d_h, OUT T_ELEM_K_LIST& arElemKey);
 
	void GetInputData(OUT MPHI_DATA_ELEM& InData);

	void GetCaseType(OUT int& nCaseType);
};