#pragma once

/**
 * Used to generate russia SP 20.13330.2016 load combinations
 */

#include "../wg_db/wg_db_DBDoc.h"
#include "../wg_db/AluDgnForce.h"

//union DGN_LCASE_K
//{
//	UINT keymap;
//	struct
//	{
//		unsigned int caseKey : 26;
//		unsigned int iAnaType : 6;
//	}key;
//};
enum eSP20_LCASE_TYPE
{
	e_SP20_None = 0,
	e_SP20_Constant = 1,
	e_SP20_Long_term,
	e_SP20_Short_term,
	e_SP20_Seismic,
	e_SP20_Crane_vertical,
	e_SP20_Crane_brake,
	e_SP20_Special,
	e_SP20_Inactive,
};
struct T_SP20_LCASE
{
	eSP20_LCASE_TYPE type;
	DGN_LCASE_K LCaseKey;
	CString LoadCaseName;
	CString LoadCaseType;
	void Initialize()
	{
		type = e_SP20_Constant;
		LCaseKey.keymap = 0;
		LoadCaseName.Empty();
		LoadCaseType.Empty();
	}
};
// Factors for Variable Actions  
struct T_SP20_LPSI_D
{
	int nIndex;
	DGN_LCASE_K LCaseKey;
	CString LoadCaseName;
	CString LoadCaseType;
	/* 对LoadCaseType的补充说明.
	*  1 Constant;2 Long-term;3 Short-term;4 Seismic;5 Crane-vertical;6 Crane-brake;7 Special;
	*/
	eSP20_LCASE_TYPE	eLoadCaseType;
	// Main combinations
	double dPsi1;
	double dPsi2;
	double dPsi3;
	// Seismic
	double dPsiSe;
	// Special combinations
	double dPsiO1;
	double dPsiO2;
	double dPsiO3;

	void Initialize()
	{
		nIndex = 0;
		LCaseKey.keymap = 0;
		LoadCaseName.Empty();
		LoadCaseType.Empty();
		eLoadCaseType = e_SP20_Constant;
		dPsi1 = 0.;
		dPsi2 = 0.;
		dPsi3 = 0.;
		dPsiSe = 0.;
		dPsiO1 = 0.0;
		dPsiO2 = 0.0;
		dPsiO3 = 0.0;
	}
	T_SP20_LPSI_D& operator=(const T_SP20_LPSI_D& src)
	{
		nIndex = src.nIndex;
		LCaseKey = src.LCaseKey;
		LoadCaseName = src.LoadCaseName;
		LoadCaseType = src.LoadCaseType;
		eLoadCaseType = src.eLoadCaseType;
		dPsi1 = src.dPsi1;
		dPsi2 = src.dPsi2;
		dPsi3 = src.dPsi3;
		dPsiSe = src.dPsiSe;
		dPsiO1 = src.dPsiO1;
		dPsiO2 = src.dPsiO2;
		dPsiO3 = src.dPsiO3;
		return *this;
	}
};
// Load safety factor
struct T_SP20_RCFI_D
{
	int nIndex;
	DGN_LCASE_K LCaseKey;
	CString LoadCaseName;
	CString LoadCaseType;
	eSP20_LCASE_TYPE	eLoadCaseType;
	// Load safety factor
	double dgF;
	// Reduced safety factor for permanent loads
	double dgfa;
	//Reliability coefficient for liability
	double dgN;
	//Reliability factor for special combinations
	double dgFo;
	//Proportion of duration
	double ddL;
	//Dominance - first or second(mutually exclusive for the same load). Can only be assigned to long and short term loads
	bool bDominace[2];

	void Initialize()
	{
		nIndex = 0;
		LCaseKey.keymap = 0;
		LoadCaseName.Empty();
		LoadCaseType.Empty();
		eLoadCaseType = e_SP20_None;
		dgF = 0.;
		dgfa = 0.;
		dgN = 0.;
		dgFo = 0.;
		ddL = 0.0;
		bDominace[0] = bDominace[1] = false;
	}
	void SetEnumLcaseType()
	{
		CString str[] = { _T("Constant"),_T("Long-term"),_T("Short-term"),_T("Seismic"),_T("Crane-vertical"),_T("Crane-brake"),_T("Special"),_T("Inactive") };
		for(int i = 0; i < e_SP20_Inactive; i++)
		{
			if (str[i] == LoadCaseType)
			{
				eLoadCaseType = eSP20_LCASE_TYPE(i + 1);
			}
		}
	}
	T_SP20_RCFI_D& operator=(const T_SP20_RCFI_D& src)
	{
		nIndex = src.nIndex;
		LCaseKey = src.LCaseKey;
		LoadCaseName = src.LoadCaseName;
		LoadCaseType = src.LoadCaseType;
		eLoadCaseType = src.eLoadCaseType;
		dgF = src.dgF;
		dgfa = src.dgfa;
		dgN = src.dgN;
		dgFo = src.dgFo;
		ddL = src.ddL;
		bDominace[0] = src.bDominace[0];
		bDominace[1] = src.bDominace[1];
		return *this;
	}
};

struct T_SP20_LINK_D
{
	int nIndex;
	DGN_LCASE_K LCaseKey;
	CString LoadCaseName;
	CString LoadCaseType;
	eSP20_LCASE_TYPE	eLoadCaseType;
	BOOL bSynchronicity;
	// 0-3:Semblance; Combination; Mutual exclusivity;Companion
	CArray<T_KEY, T_KEY> aLinksCase[4];
	
	T_SP20_LINK_D& operator=(const T_SP20_LINK_D& other)
	{
		nIndex = other.nIndex;
		LCaseKey = other.LCaseKey;
		LoadCaseName= other.LoadCaseName;
		LoadCaseType = other.LoadCaseType;
		eLoadCaseType = other.eLoadCaseType;
		bSynchronicity=other.bSynchronicity;
		for (int i = 0; i < 4; i++)
			aLinksCase[i].Copy(other.aLinksCase[i]);
		return *this;
	}
	void Initialize()
	{
		nIndex = 0;
		LCaseKey.keymap = 0;
		LoadCaseName.Empty();
		LoadCaseType.Empty();
		eLoadCaseType = e_SP20_Constant;
		bSynchronicity = false;
		for (int i = 0; i < 4; i++)
			aLinksCase[i].RemoveAll();
	}
	BOOL Contain(int nSlot,T_KEY nIndex)const
	{
		for (int i = 0; i < aLinksCase[nSlot].GetSize(); i++)
		{
			if(aLinksCase[nSlot].GetAt(i) == nIndex)
				return TRUE;
		}
		return FALSE;
	}
};

union T_SP20_PATH
{
	unsigned int  keymap;
	struct
	{
		unsigned int start : 16;
		unsigned int end: 16;
	}path;
};

//class IncidenceMatrixGraph;
class IncidenceMatrixGraphNode
{
	friend class IncidenceMatrixGraph;
public:
	IncidenceMatrixGraphNode(const IncidenceMatrixGraphNode* parent = NULL);
	void AddIndex(T_KEY nIndex,bool bcomp = false);
	T_KEY GetLastIndex()const;
//private:
	CArrayEx<T_KEY, T_KEY> aNodeIndex;
	T_KEY m_nLastIndex;
};

class IncidenceMatrixGraph
{
public:
	IncidenceMatrixGraph(const CArray<T_SP20_LINK_D, T_SP20_LINK_D&>& aRelationshipD);
	
	/* is LcaseType */
	BOOL IsLCaseType(T_KEY nIndex, eSP20_LCASE_TYPE	eLoadCaseType);
	/* 
	* companion group 
	* The companion group of T_SP20_LINK_D stores the parent index
	* m_mapComp : The relationship is that the parent contains a list of child
	* 
	* 
	* 
	*/
	BOOL GetComp_gr(T_KEY parent, CArrayEx<T_KEY, T_KEY>& aIndex);
	BOOL IsComp_gr_Child(T_KEY child);

	/*
	*  combination
	*/
	CArrayEx<T_KEY, T_KEY> GetComb_gr(T_KEY nIndex);
	BOOL NodeHasAllCombIndex(const IncidenceMatrixGraphNode& node);

	/* 
	*	Related group, mutually exclusive 
	*	function IsMuexPath : node is essentially a path. When adding nindex to node, should check whether nindex and nindex on this path are mutually exclusive
	* 
	*/
	BOOL GetMu_ex_rg(T_KEY nIndex, CArrayEx<T_KEY, T_KEY>& aIndex);
	CArrayEx<T_KEY, T_KEY> GetMu_ex_rg(T_KEY nIndex);
	BOOL IsMuexPath(T_KEY nIndex, const IncidenceMatrixGraphNode& node);
	BOOL IsMuex(T_KEY nIndex, T_KEY node);

	//剔除了非companion后的index
	CArray<T_KEY, T_KEY> aGraphIndex;

	/*
	*	This function should be called after the node is built
	*/
	void AddNode(IN IncidenceMatrixGraphNode& node);


	/* loop Index*/
	T_KEY GetNextIndex(int nIndex, bool bcomp = false);

	/* */
	void GetGraphIndex(CArrayEx<CArrayEx<T_KEY, T_KEY>, CArrayEx<T_KEY, T_KEY>&>& aGraphIndex);
private:
	void Init();
	IN CMapEx<T_KEY, T_KEY, CArrayEx<T_KEY, T_KEY>, CArrayEx<T_KEY, T_KEY>&> m_mapComp;
	IN const CArray<T_SP20_LINK_D, T_SP20_LINK_D&>& m_aRelationshipD;
	OUT CArrayEx<CArrayEx<T_KEY, T_KEY>, CArrayEx<T_KEY, T_KEY>&> m_aGraphIndex;
};

class CLoadCombSP20Ctrl : public CDBUpdateConnector
{
public:
	static CLoadCombSP20Ctrl* Instance();
	CLoadCombSP20Ctrl();
	virtual ~CLoadCombSP20Ctrl();
public:
	void GenerateLoadCaseLink(OUT CArray<T_SP20_LINK_D, T_SP20_LINK_D&>* aLinkData = NULL);
	void UpdateLoadCaseLink(OUT CArray<T_SP20_LINK_D, T_SP20_LINK_D&>& aLinkData);
	void SetLoadCaseLink(IN const CArray<T_SP20_LINK_D, T_SP20_LINK_D&>& aLinkData);

	void GeneratePsiParam(OUT CArray<T_SP20_LPSI_D, T_SP20_LPSI_D&>* aLpsiD = NULL);
	void UpdatePsiParam(OUT CArray<T_SP20_LPSI_D, T_SP20_LPSI_D&>& aLpsiD);
	void SetPsiParam(IN const CArray<T_SP20_LPSI_D, T_SP20_LPSI_D&>& aLpsiD);

	void GenerateRfiParam(OUT CArray<T_SP20_RCFI_D, T_SP20_RCFI_D&>* aRcfiD = NULL);
	void UpdateRfiParam(CArray<T_SP20_RCFI_D, T_SP20_RCFI_D&>& aRcfiD);
	void SetRfiParam(IN const CArray<T_SP20_RCFI_D, T_SP20_RCFI_D&>& aRcfiD);

	void GetDefaultRfiParam(T_SP20_RCFI_D& RcfiD);
	T_KEY GetLCaseKeyByName(const CString& strName);
	void GetIncidenceMatrixTable(IN const CArray<T_SP20_LINK_D, T_SP20_LINK_D&>& aRelationshipD, OUT CArrayEx<CArrayEx<T_KEY, T_KEY>, CArrayEx<T_KEY, T_KEY>&>& PathTable);

	void GenerateLcomAndAddDB(int nOption, int iLcomType, BOOL bAdditional[3]);
	eSP20_LCASE_TYPE GetLCaseType(int caseKey, int iAnaType);
private:
	BOOL GraphComb(const IncidenceMatrixGraphNode& path, IncidenceMatrixGraph& Graph);
	BOOL CompItemComb(const IncidenceMatrixGraphNode& path, IncidenceMatrixGraph& Graph,int nMainCount);
	void InitLCaseCache();
	void ClearLCaseCache();
protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
private:
	//在这个表的lcaseK 暂时不允许用户 添加和删除行.
	void GetDefaultPsiParam(DGN_LCASE_K LCaseKey, T_SP20_LPSI_D& LpsiD);
	void GetDefaultRfiParam(DGN_LCASE_K LCaseKey, T_SP20_RCFI_D& RcfiD);
	//对荷载工况排序
	void SortDgnLCase(CArray<DGN_LCASE_K, DGN_LCASE_K&>& aLCaseK);

	void GetLoadCase(CArray<DGN_LCASE_K, DGN_LCASE_K&>& aLCaseK);
	void GenerateLoadCase(CArray<DGN_LCASE_K, DGN_LCASE_K&>& aLCaseK);

	int Get_MaxLoadCombNameNo(int LcomType);
	BOOL GetLCaseFactor(IN const CArrayEx<T_KEY, T_KEY>& aInLCase, OUT CArrayEx<double, double>& aFactor);
private:
	CMapEx<UINT, UINT, T_SP20_LCASE, T_SP20_LCASE&> m_mapLCase;
	CArray<DGN_LCASE_K, DGN_LCASE_K&> m_aSortLCase;
	BOOL m_bhasCache;

	// russia SP 20.13330.2016 Automatically calculate the required data
	CArray<T_SP20_LPSI_D, T_SP20_LPSI_D&> m_aLcaseSP20PsiFactor;
	CArray<T_SP20_RCFI_D, T_SP20_RCFI_D&> m_aLcomSP20RfiFactor;
	CArray<T_SP20_LINK_D, T_SP20_LINK_D&> m_aLcaseSP20LinkData;
};


