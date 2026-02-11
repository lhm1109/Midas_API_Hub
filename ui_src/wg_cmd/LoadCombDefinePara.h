#pragma once
#include "LoadCombDefineData.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "HeaderPre.h"

#define NAME_LOADCASE_D		_T("D"  )		
#define NAME_LOADCASE_L		_T("L"  )		
#define NAME_LOADCASE_LR	_T("LR"  )	
#define NAME_LOADCASE_W		_T("W"  )		
#define NAME_LOADCASE_E		_T("E"  )		
#define NAME_LOADCASE_EVT	_T("EVT"  )	
#define NAME_LOADCASE_S		_T("S"  )	
#define NAME_LOADCASE_T		_T("T"  )
#define NAME_LOADCASE_PS	_T("PS")
#define NAME_LOADCASE_EH	_T("EH"  )	
#define NAME_LOADCASE_EV	_T("EV"  )	
#define NAME_LOADCASE_WP	_T("WP"  )	
#define NAME_LOADCASE_FP	_T("FP"  )	
#define NAME_LOADCASE_SF	_T("SF"  )	
#define NAME_LOADCASE_B		_T("B"  )	
#define NAME_LOADCASE_CS	_T("CS"  )
#define NAME_LOADCASE_CR	_T("CR"  )
#define NAME_LOADCASE_SH	_T("SH"  )
#define NAME_LOADCASE_RS	_T("RS"  )
#define NAME_LOADCASE_COMB	_T("COMB")
enum CaseClassfyType
{
	Type_Null	= 0,
	Type_A		   ,	//直接用分项系数参与组合，分项系数不乘组合值系数
	Type_B		   ,	//可变荷载工况，需要轮流作为主工况进行组合，主工况不乘以组合值系数，非主工况需要乘以组合值系数
	Type_BC		   ,	//兼顾type_b & type_c
	Type_C		   ,	//在原组合的基础上需要考虑正负两种情况，并且同一类型的工况中有多个工况时需要单独进行组合
	Type_CV		   ,	//同type_c
};

class __MY_EXT_CLASS__ CLoadCombItem
{
public:
	CLoadCombItem();
	BOOL isValid() const;
public:
	CString			m_sType;
	double			m_dFactor;
	LOAD_CASE_K		m_key;
	CaseClassfyType m_eType;

};

typedef CArrayEx<CLoadCombItem, CLoadCombItem&> LoadCombItemArr;

struct __MY_EXT_CLASS__ CaseCombDefineInfo
{
	//A Type Case（D）
	LoadCombItemArr	m_aTypeA	 ;
	//B Type Case(L)
	LoadCombItemArr	m_aTypeB	 ;
	//C Type Case(RS)
	CLoadCombItem	m_itemC		 ;
	CLoadCombItem	m_itemCV;
	//B Type Case divide
	CArrayEx<LoadCombItemArr, LoadCombItemArr&> m_aTypeBDivid;
};

typedef CArrayEx<CaseCombDefineInfo, CaseCombDefineInfo&> CaseCombInfoArr;



class __MY_EXT_CLASS__ CLoadCombDefinePara
{
public:
	CLoadCombDefinePara(const CLoadCombDefineData *pData);
	void initial();
	void test();
public:
	//类型对应的工况,和规范相关
	std::map<CString, CaseClassfyType>	m_mapTypeClassfy;
	//类型对应的工况和模型相关
	std::map<CString, LOAD_CASE_K_ARR>	m_mapType2Case;
	//工况对应的类型
	std::map<LOAD_CASE_K, CString>		m_mapCase2Type;
	//双向地震主工况对应的组合序号，根据data里的用户设置自动生成，生成具体组合时，如果组合包含双向地震，则需要把工况KEY替换成这里组合(Index)
	std::map<LOAD_CASE_K, int>			m_mapCaseC2DoubleDirEQ;
	CArrayEx<LoadCombItemArr, LoadCombItemArr&>		m_aDoubleDirCombC;
public:
	const CLoadCombDefineData	*m_pData;
};


struct __MY_EXT_CLASS__ CLcomCombAutoRlt
{
public:
	void add(LoadCombItemArr &data);
	void add(CArrayEx<LoadCombItemArr, LoadCombItemArr&> &aData);
	CArrayEx<LoadCombItemArr, LoadCombItemArr&> m_aLcomb;
};

#include "HeaderPost.h"