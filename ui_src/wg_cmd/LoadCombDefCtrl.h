// LoadCombDefCtrl.h: interface for the CLoadCombCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LoadCombCtrl.h"
#include "..\\wg_db\\wg_db_DBDoc.h"
#include <map>
#include <vector>
#include <tuple>
#include "LoadCombDefineData.h"
#include "LoadCombDefinePara.h"
#include "HeaderPre.h"


struct IMPL_LCOM_APPEND
{
	T_STLD_K StldK;  // Imperfection Load Case狼 Key
	CString strStldName;// Imperfection Load Case狼 Name
	double dFactor; // Factor    

	BOOL operator<(const IMPL_LCOM_APPEND &rhs)  const
	{
		return StldK < rhs.StldK;
	}
};

class CLoadCombCtrl;
class CombBase;
class __MY_EXT_CLASS__ CLoadCombDefCtrl
{
public:
	CLoadCombDefCtrl();
	virtual ~CLoadCombDefCtrl();
	
public:
	BOOL Set_DefaultLoadComb(int nOption, CLoadCombDefineData* pData);
	BOOL autoGenerate(CLoadCombDefineData* pData);
	//标准组合
	BOOL   autoGenerate_Stand(CLoadCombDefineData* pData);
protected:
	BOOL Delete_AllLoadComb(int nType);
	CString MakeCombDescription(const T_LCOM_D &lcomD, int nLcomType);
	int Get_MaxLoadCombNameNo(int nLocmType);
	void Remove0FactorCase(T_LCOM_D& LComD);
	BOOL IsExistLcom(T_LCOM_D& CurLcomD, const CArray<T_LCOM_D, T_LCOM_D&>& arLcomD);
	BOOL IsSameLcomData(const T_LCOM_D& CurLcomD, const T_LCOM_D& PreLcomD);
	void AddComb2Temp(const CArray<T_LCOM_K, T_LCOM_K> &rKeyList, int nLcomType);
	//
	BOOL IsSetDefaultImpf();
	void SetDefaultImpfData();
	BOOL GetLoadCombTypeByLoadCaseType(int nLoadCaseType, int& nAnalType);
	BOOL Set_Each_LoadComb_Imperfection_CH(T_LCOM_D& LComIn, CArray<T_LCOM_D, const T_LCOM_D&> &aNewLcom);
	
protected:
	virtual BOOL generationType (CLoadCombDefinePara *pPara);
protected:
	BOOL processLLUnfavorable (CLoadCombDefinePara *pPara, LoadCombItemArr &aTypeB, CArrayEx<LoadCombItemArr, LoadCombItemArr&> &aDividTypeB);
	BOOL processExclusionTypeB(CLoadCombDefinePara *pPara, LoadCombItemArr &aTypeB, CArrayEx<LoadCombItemArr, LoadCombItemArr&> &aDividTypeB);
	BOOL processExclusionTypeB(LOAD_CASE_K Key1, const LOAD_CASE_K_ARR aExclude, CArrayEx<LoadCombItemArr, LoadCombItemArr&> &aDividTypeB);
	void addKey2Data		  (CLoadCombDefinePara *pPara, LPCTSTR sType, CaseClassfyType nClassType, double dFactor, const LOAD_CASE_K_ARR &aKey, LoadCombItemArr &aData);
protected:
	void generateDoubleDirC   (CLoadCombDefinePara *pPara);
	BOOL getClassfyInfo		  (CLoadCombDefinePara *pPara, int idx, CaseCombInfoArr &aInfo);
	BOOL generatorDoubleDirC  (CLoadCombDefinePara *pPara, CLcomCombAutoRlt *pRltPool);
	BOOL generatorLcomb		  (CLoadCombDefinePara *pPara, const CaseCombDefineInfo *pInfo, CLcomCombAutoRlt *pRltPool);
	//nType: 0 C类型控制 1 A类型控制， 2 B类型控制的主类型 3 B类型控制的次类型
	BOOL addBType2Lcomb		  (CLoadCombDefinePara *pPara, const CLoadCombItem &item , int nType, CArrayEx<LoadCombItemArr, LoadCombItemArr&> &aLcomb);
	//+-
	BOOL addCType2Lcomb		  (CLoadCombDefinePara *pPara, const CLoadCombItem &item , CArrayEx<LoadCombItemArr, LoadCombItemArr&> &aLcomb);
	BOOL isWind(const CLoadCombItem &item);
	BOOL isTemperature(const CLoadCombItem &item);
	BOOL considerEccnC(T_SPLC_K key);
	void generateEccnC(T_SPLC_K key, double dFactor, const T_LCOM_D& LComIn, T_LCOM_D& NewLcom);
	//分项系数
	double getFactor1(CLoadCombDefinePara *pPara, const CLoadCombItem &item);
	//组合值系数
	double getFactor2(CLoadCombDefinePara *pPara, const CLoadCombItem &item);
	//重力代表值系数
	double getFactor3(CLoadCombDefinePara *pPara, const CLoadCombItem &item);
	BOOL   addChild2LComb(const CLoadCombItem& item, double dNewFactor, LoadCombItemArr &Lcomb);
	BOOL   save2DB(CLcomCombAutoRlt *pRltPool, int nLcomType = D_LCOMTYPE_GENERAL);
	BOOL   convert2Lcomb(const LoadCombItemArr &item, const std::map<int, T_LCOM_K> &mapIdx2Key, T_LCOM_D &lcomD, int nLcomType);
	//规范分类
	int MakeCombType();
public:
	CDBDoc* m_pDoc;
	BOOL m_bChkLCType[6];
protected:
	CString m_strCodeName;
	int m_nOption;	//0: add  1: replace
	CArray<T_LCOM_D, T_LCOM_D&> m_tempLcom;
	CArray<_LCOM_IMPF, _LCOM_IMPF&> m_aImperfectionLoad;  
	BOOL m_bImperfection;
	BOOL m_bStandComb;
	BOOL m_bGB55006_21;
};

#include "HeaderPost.h"


