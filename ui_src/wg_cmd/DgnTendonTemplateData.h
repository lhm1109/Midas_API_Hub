#include <vector>
#include <map>


#ifndef _TNDN_TEMPLATE_
#define _TNDN_TEMPLATE_


#define IS_TYPE_STRAIGHT(type) (type == TNDN_TEMPLATE_TYPE_STRAIGHT||\
	type == TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_1||\
type == TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2)
#define IS_TYPE_CURVE(type) (type == TNDN_TEMPLATE_TYPE_CURVE_LINE||\
	type == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1||\
	type == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2||\
	type == TNDN_TEMPLATE_TYPE_CURVE_BENT_1||\
	type == TNDN_TEMPLATE_TYPE_CURVE_ST)

enum TNDN_TEMPLATE_TYPE
{
		//하위 버전과의 호환을 위해서 추가는하되, 삭제는 하지 않는다.
		TNDN_TEMPLATE_TYPE_UNKNOWN = -1,

		TNDN_TEMPLATE_TYPE_STRAIGHT,        // Straight
		TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_1, // Harped 1
		TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2, // Harped 2
		TNDN_TEMPLATE_TYPE_CURVE_LINE,      // Curved 1
		TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1,  // Curved 2
		TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2,  // Curved 3
		TNDN_TEMPLATE_TYPE_CURVE_BENT_1,    // Curved 4
		TNDN_TEMPLATE_TYPE_CURVE_ST,        // Straight for Curved tendon

		TNDN_TEMPLATE_TYPE_NUMBER
};

struct TndnTemplateSubInfo
{
	double dDt;			// t
	double dDt1;		// t'
	double dDb;			// b1 (b)
	double dDb1;		// b2
	double dDf;         // h1 (h)
	double dDe;         // h2
	double dDRad;
		double dR1;         // r1
		double dR2;         // r2

	TndnTemplateSubInfo()
	{
		dDt		= 0.0;
		dDt1	= 0.0;
		dDb		= 0.0;
		dDb1	= 0.0;
		dDf		= 0.0;
		dDe		= 0.0;
		dDRad	= 0.0;
				dR1     = 0.0;
				dR2     = 0.0;
	}

	void operator = (const TndnTemplateSubInfo& src)
	{
		dDt		= src.dDt;
		dDt1	= src.dDt1;
		dDb		= src.dDb;
		dDb1	= src.dDb1;
		dDf		= src.dDf;
		dDe		= src.dDe;
	dDRad	= src.dDRad;
		dR1     = src.dR1;
		dR2     = src.dR2;
	}

	BOOL operator == (const TndnTemplateSubInfo& src)
	{
		if( fabs(dDt - src.dDt)     >= 1E-5 ) return FALSE;
		if( fabs(dDt1 - src.dDt1)   >= 1E-5 ) return FALSE;
		if( fabs(dDb - src.dDb)     >= 1E-5 ) return FALSE;
		if( fabs(dDb1 - src.dDb1)   >= 1E-5 ) return FALSE;
		if( fabs(dDf - src.dDf)     >= 1E-5 ) return FALSE;
		if( fabs(dDe - src.dDe)     >= 1E-5 ) return FALSE;
	if( fabs(dDRad - src.dDRad) >= 1E-5 ) return FALSE;
		if( fabs(dR1 - src.dR1)     >= 1E-5 ) return FALSE;
		if( fabs(dR2 - src.dR2)     >= 1E-5 ) return FALSE;
		return TRUE;    
	}
};
struct TndnTemplateSub
{
	TNDN_TEMPLATE_TYPE  nType;
	BOOL				        bSameSpanInfo;
	BOOL				        bXSymmetry;				  // 추가
	CString				      strSpanLength;			// 추가 부등간격일때 사용됨.
	int					        nNumOfSpan;

	std::vector<TndnTemplateSubInfo>	aSpanInfo;

	TndnTemplateSub()
	{
		Init();
	}

	void Init()
	{
		nType					= TNDN_TEMPLATE_TYPE_STRAIGHT;
		bSameSpanInfo			= TRUE;
		bXSymmetry				= FALSE;
		strSpanLength			= _T("");
		SetSpanNum(1);
	}

	void SetSpanNum(int nSpanNum)
	{
		nNumOfSpan = nSpanNum;
		aSpanInfo.resize(nNumOfSpan);
	}

	void operator = (const TndnTemplateSub& src)
	{
		nType           = src.nType;
		bSameSpanInfo   = src.bSameSpanInfo;
		bXSymmetry      = src.bXSymmetry;
		strSpanLength   = src.strSpanLength;
		nNumOfSpan      = src.nNumOfSpan;
		int nSize       = (int)src.aSpanInfo.size();

		if(aSpanInfo.size() != nSize)
		{
		  aSpanInfo.resize(nSize);
		}

		for(int i = 0 ; i < nSize; i++)
		{
			aSpanInfo[i] = src.aSpanInfo[i];
		}    
	}
	
	BOOL operator == (const TndnTemplateSub& src)
	{
		if( !(nType         == src.nType) )         return FALSE;
		if( !(bSameSpanInfo == src.bSameSpanInfo) ) return FALSE;
		if( !(bXSymmetry    == src.bXSymmetry) )    return FALSE;
		if( !(strSpanLength == src.strSpanLength) ) return FALSE;
		if( !(nNumOfSpan    == src.nNumOfSpan) )    return FALSE;

		int nSizeOrg = (int)aSpanInfo.size();
		int nSizeCmp = (int)src.aSpanInfo.size();

		if( !( nSizeOrg == nSizeCmp) )
			return FALSE;
		for(int i = 0 ; i < nSizeOrg; i++)
		{
			if( !(aSpanInfo[i] == src.aSpanInfo[i]) )
				return FALSE;
		}
		return TRUE;
	}
};

struct TndnTemplate
{
	CString       strName;
	unsigned int  keyGroup;
	unsigned int  keyProperty;

	double				dOffsetFront;
	double				dOffsetEnd;

	BOOL          bCurveTypeOut;      // True : Tendon Profile이 생성될때 Ref.Axis 가 Curve Type, False : Elem Type
	BOOL				  bSameSpanInterval;  // True : 등간격, False, 부등간격.
	UINT				  nType;              //0:Curve(Spline), 1:Straight(Round)

	TndnTemplateSub		tndnSubXY;
	TndnTemplateSub		tndnSubXZ;

	BOOL bPreInter;   // Pre-Internal

	TndnTemplate()
	{
		Init();
	}
	void Init()
	{
		strName.Format(_T(""));
		keyGroup	= 0;
		keyProperty = 0;
		
		dOffsetFront = 0.0;
		dOffsetEnd	 = 0.0;

		tndnSubXY.Init();		
		tndnSubXZ.Init();		
		
		bCurveTypeOut = FALSE;
		bSameSpanInterval = TRUE;
		nType = 1;
		bPreInter = FALSE;
	}

	void operator = (const TndnTemplate& src)
	{
		strName           = src.strName;
		keyGroup          = src.keyGroup;
		keyProperty       = src.keyProperty;
		dOffsetFront      = src.dOffsetFront;
		dOffsetEnd		    = src.dOffsetEnd;
		tndnSubXY         = src.tndnSubXY;
		tndnSubXZ         = src.tndnSubXZ;
		bCurveTypeOut	    = src.bCurveTypeOut;
		bSameSpanInterval = src.bSameSpanInterval;
		nType				      = src.nType;
		bPreInter         = src.bPreInter;
	}
	
	BOOL operator == (const TndnTemplate& src)
	{
		//이름을 제외한 모든 값이 같다면 같은것으로..
		//if( !(strName           == src.strName) )           return FALSE;
		if( !(keyGroup          == src.keyGroup) )          return FALSE;
		if( !(keyProperty       == src.keyProperty) )       return FALSE;
		if( fabs(dOffsetFront - src.dOffsetFront) >= 1E-5 ) return FALSE;
		if( fabs(dOffsetEnd - src.dOffsetEnd)     >= 1E-5 ) return FALSE;
		if( !(tndnSubXY         == src.tndnSubXY) )         return FALSE;
		if( !(tndnSubXZ         == src.tndnSubXZ) )         return FALSE;
		if( !(bCurveTypeOut     == src.bCurveTypeOut) )     return FALSE;
		if( !(bSameSpanInterval == src.bSameSpanInterval) ) return FALSE;
		if( !(nType             == src.nType) )             return FALSE;
		if( !(bPreInter         == src.bPreInter) )             return FALSE;
		return TRUE;
	}
};

struct TndnDrawStruct
{
	unsigned int	nTemplateKey;
	unsigned int	nLinkKey;
	double			dTendonDia;
	
	std::vector<BOOL>	bVecInputPt_XY;
	std::vector<double>	dVecPosX_XY;			//그려질 텐던의 x
	std::vector<double>	dVecPosY;
	
	std::vector<BOOL>	bVecInputPt_XZ;
	std::vector<double>	dVecPosX_XZ;			//그려질 텐던의 x
	std::vector<double>	dVecPosZ;

	double				dOffsetFront;
	double				dOffsetEnd;
	BOOL bPreInter;   // Pre-Internal

	TndnDrawStruct()
	{
		Init();
	}

	void Init()
	{
		nTemplateKey	= 0;
		nLinkKey		= 0;
		dTendonDia		= 0.0;
		dVecPosX_XY.clear();
		dVecPosX_XZ.clear();
		dVecPosY.clear();
		dVecPosZ.clear();
		bVecInputPt_XY.clear();
		bVecInputPt_XZ.clear();
		dOffsetFront = 0.0;
		dOffsetEnd = 0.0;
		bPreInter = FALSE;
	}

	BOOL GetPosYZByPosX(double dPosX, BOOL bXY, double& dPosYZ)
	{
		int nSize;

		std::vector<double>* pPosX;
		std::vector<double>* pPosYZ;

		if(bXY)
		{
			pPosX = &dVecPosX_XY;
			pPosYZ = &dVecPosY;
			nSize = (int)pPosX->size();
		}
		else
		{
			pPosX = &dVecPosX_XZ;
			pPosYZ = &dVecPosZ;
			nSize = (int)pPosX->size();
		}

		if(nSize <= 1)
			return FALSE;

		if(fabs(dPosX - (*pPosX)[0]) <= 1E-6)
		{
			dPosYZ = (*pPosYZ)[0];
			return TRUE;
		}
		if(fabs(dPosX - (*pPosX)[nSize - 1]) <= 1E-6)
		{
			dPosYZ = (*pPosYZ)[nSize - 1];
			return TRUE;
		}

		if(dPosX <= (*pPosX)[0] + 1E-5 )			return FALSE; //0번째 보다 작거나
		if(dPosX >= (*pPosX)[nSize - 1] - 1E-5)		return FALSE; //마지막 보다 크면

		int i;

		for(i = 1 ; i < nSize; i++)
		{
			if(dPosX < (*pPosX)[i])
			{
				double dFactor =  ( dPosX - (*pPosX)[i - 1] ) / ( (*pPosX)[i] - (*pPosX)[i - 1] );
				double dFactorYZ = ( (*pPosYZ)[i] - (*pPosYZ)[i - 1] ) * dFactor;
				dPosYZ = dFactorYZ + (*pPosYZ)[i - 1];
				return TRUE;
			}
		}

		return FALSE;
	}
};

typedef std::map<int, TndnTemplate>           mapTndnTemp;
typedef std::map<int, TndnTemplate>::iterator itrMapTndn;
typedef std::vector<TndnTemplate>             vecTndnTemp;
typedef std::vector<TndnTemplate>::iterator   itrVecTndn;

typedef std::vector<int>            vecTndnKey;
typedef std::vector<int>::iterator   itrKey;

#endif
