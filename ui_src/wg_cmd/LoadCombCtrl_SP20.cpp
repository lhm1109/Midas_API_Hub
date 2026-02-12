#include "stdafx.h"
#include "wg_cmd.h"
#include "LoadCombCtrl_SP20.h"

#include "../wg_db/wg_db_AttrCtrl.h"
#include "../wg_db/wg_db_AttrCtrl2.h"
#include "../wg_db/wg_db_PostCtrl.h"
#include "../wg_db/wg_db_DataCtrl.h"
#include "../wg_db/wg_db_UndoCtrl.h"
#include "../wg_db/wg_db_EditData.h"
#include "../wg_db/wg_db_StageInfo.h"
#include "../wg_db/mvLoad.h"
#include "../wg_db/DB_ST_DN_JUD.h"
#include "../wg_db/VehlDef.h"
#include "../wg_db/LateralLoad.h"
#include "../wg_db/AluDgnTool.h"
#include "../wg_db/DBCodeDef.h"
#include "../wg_db/DBLib.h"
#include <math.h>

#include "../wg_base/wg_base_AppBase.h"
#include "../wg_base/wg_base_MsgDll.h"
#include "../wg_base/wg_base_StrParser.h"
#include "../wg_base/wg_base_CompFunc.h"
#include "../wg_db/ViewBuff.h"
#include <functional>
//
#include <fstream>       // for ofstream
using namespace std;
//

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define D_STR 0
#define D_SER 1

#define D_LOAD_LEVEL_STRN 0  // Strength Level.
#define D_LOAD_LEVEL_SERV 1  // Service Level.
/////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define D_WIND_DIRCOMB_NONE 0  // none.
#define D_WIND_DIRCOMB_W    1  // along wind.
#define D_WIND_DIRCOMB_A    2  // across wind.
#define D_WIND_DIRCOMB_T    3  // torsional wind.
#define D_WIND_DIRCOMB_WA_L 4  // along wind + across wind. (for Middle Low Rise Building)
#define D_WIND_DIRCOMB_WA_H 5  // along wind + across wind. (for High Rise Building)
#define D_WIND_DIRCOMB_WT   6  // along wind + torsional wind.
#define D_WIND_DIRCOMB_AT   7  // across wind + torsional wind.
#define D_WIND_DIRCOMB_WAT  8  // along wind + across wind + torsional wind.

enum { EN_ACT_STRN=1, EN_ACT_SERV=2, EN_ACT_SPEC=4, EN_ACT_VERT=5 };

enum { EN_SEIS_DISP, EN_SEIS_FORC };  /// 郴柳己瓷乞啊侩 窍吝炼钦 盒幅 (DISP : 函困瘤硅芭悼侩, FORC : 窍吝瘤硅芭悼)


CLoadCombSP20Ctrl* CLoadCombSP20Ctrl::CLoadCombSP20Ctrl::Instance()
{
	static CLoadCombSP20Ctrl g_LoadCombSP20Ctrl;
	g_LoadCombSP20Ctrl.InitLCaseCache();
	return &g_LoadCombSP20Ctrl;
}

CLoadCombSP20Ctrl::CLoadCombSP20Ctrl()
{
	m_bhasCache = FALSE;
	m_aLcaseSP20PsiFactor.RemoveAll();
	m_aLcomSP20RfiFactor.RemoveAll();
	m_aLcaseSP20LinkData.RemoveAll();
}

CLoadCombSP20Ctrl::~CLoadCombSP20Ctrl()
{
}

void CLoadCombSP20Ctrl::GenerateLoadCaseLink(OUT CArray<T_SP20_LINK_D, T_SP20_LINK_D &>* aLinkData /* = NULL */)
{
	if (m_aLcaseSP20LinkData.IsEmpty())
	{
		m_aLcaseSP20LinkData.RemoveAll();

		CArray<DGN_LCASE_K, DGN_LCASE_K&> aLCaseK;
		GetLoadCase(aLCaseK);
		m_aLcaseSP20LinkData.SetSize(aLCaseK.GetSize());
		T_SP20_LCASE LCaseData;
		for (int i = 0; i < aLCaseK.GetSize(); i++)
		{
			m_mapLCase.Lookup(aLCaseK[i].keymap, LCaseData);
			m_aLcaseSP20LinkData[i].Initialize();
			m_aLcaseSP20LinkData[i].LoadCaseName = LCaseData.LoadCaseName;
			m_aLcaseSP20LinkData[i].LoadCaseType = LCaseData.LoadCaseType;
			m_aLcaseSP20LinkData[i].eLoadCaseType = LCaseData.type;
			m_aLcaseSP20LinkData[i].nIndex = i + 1;
			m_aLcaseSP20LinkData[i].LCaseKey = aLCaseK[i];
		}
	}
	if (aLinkData && aLinkData != &m_aLcaseSP20LinkData)
	{
		aLinkData->RemoveAll();
		aLinkData->Copy(m_aLcaseSP20LinkData);
	}
}
void CLoadCombSP20Ctrl::UpdateLoadCaseLink(OUT CArray<T_SP20_LINK_D, T_SP20_LINK_D&>& aLinkData)
{
	CMap<T_KEY, T_KEY, T_SP20_LINK_D, T_SP20_LINK_D&> mapLink;
	for (int i = 0; i < aLinkData.GetSize(); i++)
	{
		aLinkData[i].eLoadCaseType = m_mapLCase[aLinkData[i].LCaseKey.keymap].type;
		aLinkData[i].LoadCaseType = m_mapLCase[aLinkData[i].LCaseKey.keymap].LoadCaseType;
		mapLink[aLinkData[i].LCaseKey.keymap] = aLinkData[i];
	}
	CArray<T_KEY, T_KEY> aKey;
	m_mapLCase.GetKeyList(aKey);
	aLinkData.RemoveAll();
	aLinkData.SetSize(m_aSortLCase.GetSize());
	for (int i = 0; i < m_aSortLCase.GetSize(); i++)
	{
		mapLink.Lookup(m_aSortLCase[i].keymap, aLinkData[i]);
	}
}
void CLoadCombSP20Ctrl::SetLoadCaseLink(IN const CArray<T_SP20_LINK_D, T_SP20_LINK_D&>& aLinkData)
{
	m_aLcaseSP20LinkData.RemoveAll();
	m_aLcaseSP20LinkData.Copy(aLinkData);
}

void CLoadCombSP20Ctrl::GeneratePsiParam(OUT CArray<T_SP20_LPSI_D, T_SP20_LPSI_D &>* aLpsiD /* = NULL */)
{
	if (m_aLcaseSP20PsiFactor.IsEmpty())
	{
		m_aLcaseSP20PsiFactor.SetSize(m_aSortLCase.GetSize());
		for (int i = 0; i < m_aSortLCase.GetSize(); i++)
		{
			GetDefaultPsiParam(m_aSortLCase[i], m_aLcaseSP20PsiFactor[i]);
			m_aLcaseSP20PsiFactor[i].nIndex = i + 1;
		}
	}
	if (aLpsiD && aLpsiD != &m_aLcaseSP20PsiFactor)
	{
		aLpsiD->RemoveAll();
		aLpsiD->Copy(m_aLcaseSP20PsiFactor);
	}

}
void CLoadCombSP20Ctrl::UpdatePsiParam(OUT CArray<T_SP20_LPSI_D, T_SP20_LPSI_D&>& aLpsiD)
{
	CMap<T_KEY, T_KEY, T_SP20_LPSI_D, T_SP20_LPSI_D&> mapLpsi;
	for (int i = 0; i < aLpsiD.GetSize(); i++)
	{
		aLpsiD[i].eLoadCaseType = m_mapLCase[aLpsiD[i].LCaseKey.keymap].type;
		aLpsiD[i].LoadCaseType = m_mapLCase[aLpsiD[i].LCaseKey.keymap].LoadCaseType;
		mapLpsi[aLpsiD[i].LCaseKey.keymap] = aLpsiD[i];
	}
	CArray<T_KEY, T_KEY> aKey;
	m_mapLCase.GetKeyList(aKey);
	aLpsiD.RemoveAll();
	aLpsiD.SetSize(m_aSortLCase.GetSize());
	for (int i = 0; i < m_aSortLCase.GetSize(); i++)
	{
		mapLpsi.Lookup(m_aSortLCase[i].keymap, aLpsiD[i]);
	}
}
void CLoadCombSP20Ctrl::SetPsiParam(IN const CArray<T_SP20_LPSI_D, T_SP20_LPSI_D&>& aLpsiD)
{
	m_aLcaseSP20PsiFactor.RemoveAll();
	m_aLcaseSP20PsiFactor.Copy(aLpsiD);
}

void CLoadCombSP20Ctrl::GenerateRfiParam(OUT CArray<T_SP20_RCFI_D, T_SP20_RCFI_D &>* aRcfiD /* = NULL */)
{
	if (m_aLcomSP20RfiFactor.IsEmpty())
	{
		m_aLcomSP20RfiFactor.RemoveAll();
		m_aLcomSP20RfiFactor.SetSize(m_aSortLCase.GetSize());
		for (int i = 0; i < m_aSortLCase.GetSize(); i++)
		{
			GetDefaultRfiParam(m_aSortLCase[i], m_aLcomSP20RfiFactor[i]);
			m_aLcomSP20RfiFactor[i].nIndex = i + 1;
		}
	}
	if (aRcfiD && aRcfiD != &m_aLcomSP20RfiFactor)
	{
		aRcfiD->RemoveAll();
		aRcfiD->Copy(m_aLcomSP20RfiFactor);
	}
}
void CLoadCombSP20Ctrl::UpdateRfiParam(CArray<T_SP20_RCFI_D, T_SP20_RCFI_D&>& aRcfiD)
{
	CMap<T_KEY, T_KEY, T_SP20_RCFI_D, T_SP20_RCFI_D&> mapRcfi;
	for (int i = 0; i < aRcfiD.GetSize(); i++)
	{
		m_mapLCase[aRcfiD[i].LCaseKey.keymap].type = aRcfiD[i].eLoadCaseType;
		m_mapLCase[aRcfiD[i].LCaseKey.keymap].LoadCaseType = aRcfiD[i].LoadCaseType;
		mapRcfi[aRcfiD[i].LCaseKey.keymap] = aRcfiD[i];
	}
	CArray<T_KEY, T_KEY> aKey;
	m_mapLCase.GetKeyList(aKey);
	m_aSortLCase.RemoveAll();
	m_aSortLCase.SetSize(aKey.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		m_aSortLCase[i].keymap = aKey[i];
	}
	SortDgnLCase(m_aSortLCase);
	aRcfiD.RemoveAll();
	aRcfiD.SetSize(m_aSortLCase.GetSize());
	for (int i = 0; i < m_aSortLCase.GetSize(); i++)
	{
		mapRcfi.Lookup(m_aSortLCase[i].keymap, aRcfiD[i]);
	}
}
void CLoadCombSP20Ctrl::SetRfiParam(IN const CArray<T_SP20_RCFI_D, T_SP20_RCFI_D&>& aRcfiD)
{
	m_aLcomSP20RfiFactor.RemoveAll();
	m_aLcomSP20RfiFactor.Copy(aRcfiD);
	UpdateRfiParam(m_aLcomSP20RfiFactor);
	if (m_aLcaseSP20PsiFactor.IsEmpty())
		GeneratePsiParam();
	else
		UpdatePsiParam(m_aLcaseSP20PsiFactor);
	if (m_aLcaseSP20LinkData.IsEmpty())
		GenerateLoadCaseLink();
	else
		UpdateLoadCaseLink(m_aLcaseSP20LinkData);
}

void CLoadCombSP20Ctrl::InitLCaseCache()
{
	if (!m_bhasCache)
	{
		m_mapLCase.RemoveAll();
		m_aSortLCase.RemoveAll();

		GenerateLoadCase(m_aSortLCase);
		SortDgnLCase(m_aSortLCase);
		m_bhasCache = TRUE;
	}
}

void CLoadCombSP20Ctrl::ClearLCaseCache()
{
	m_bhasCache = FALSE;
	m_mapLCase.RemoveAll();
	m_aSortLCase.RemoveAll();
	m_aLcaseSP20PsiFactor.RemoveAll();
	m_aLcomSP20RfiFactor.RemoveAll();
	m_aLcaseSP20LinkData.RemoveAll();
}

void CLoadCombSP20Ctrl::GetDefaultPsiParam(DGN_LCASE_K LCaseKey, T_SP20_LPSI_D& LpsiD)
{
	LpsiD.Initialize();
	T_SP20_LCASE lCase;
	m_mapLCase.Lookup(LCaseKey.keymap, lCase);
	LpsiD.LCaseKey = LCaseKey;
	LpsiD.LoadCaseName = lCase.LoadCaseName;
	LpsiD.LoadCaseType= lCase.LoadCaseType;
	LpsiD.eLoadCaseType = lCase.type;
	LpsiD.nIndex = 0;

	switch (LpsiD.eLoadCaseType)
	{
	case e_SP20_Constant:
		LpsiD.dPsi1 = 1.0;
		LpsiD.dPsiSe = 0.9;
		LpsiD.dPsiO1 = 1.0;
		LpsiD.dPsiO2 = 1.0;
		LpsiD.dPsiO3 = 1.0;
		break;
	case e_SP20_Long_term:
		LpsiD.dPsi1 = 1.0;
		LpsiD.dPsi2 = 0.95;
		LpsiD.dPsi3 = 0.95;
		LpsiD.dPsiSe = 0.8;
		LpsiD.dPsiO1 = 1.0;
		LpsiD.dPsiO2 = 0.95;
		LpsiD.dPsiO3 = 0.95;
		break;
	case e_SP20_Short_term:
		LpsiD.dPsi1 = 1.0;
		LpsiD.dPsi2 = 0.9;
		LpsiD.dPsi3 = 0.7;
		LpsiD.dPsiSe = 0.5;
		LpsiD.dPsiO1 = 0.5;
		LpsiD.dPsiO2 = 0.3;
		LpsiD.dPsiO3 = 0.3;
		break;
	case e_SP20_Seismic:
		LpsiD.dPsiSe = 1.0;
		break;
	case e_SP20_Crane_vertical:
		LpsiD.dPsi1 = 1.0;
		LpsiD.dPsi2 = 0.85;
		LpsiD.dPsi3 = 0.7;
		break;
	case e_SP20_Crane_brake:
		LpsiD.dPsi1 = 1.0;
		LpsiD.dPsi2 = 0.85;
		LpsiD.dPsi3 = 0.7;
		break;
	case e_SP20_Special:
		LpsiD.dPsiO1 = 1.0;
		break;
	case e_SP20_Inactive:
		LpsiD.dPsi1 = 0.;
		LpsiD.dPsi2 = 0.;
		LpsiD.dPsi3 = 0.;
		LpsiD.dPsiSe = 0.;
		LpsiD.dPsiO1 = 0.0;
		LpsiD.dPsiO2 = 0.0;
		LpsiD.dPsiO3 = 0.0;
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}
void CLoadCombSP20Ctrl::GetDefaultRfiParam(T_SP20_RCFI_D& RcfiD)
{
	switch (RcfiD.eLoadCaseType)
	{
	case e_SP20_Constant:
		RcfiD.dgF = 1.1;
		RcfiD.dgfa = 0.9;
		RcfiD.dgN = 1.0;
		RcfiD.dgFo = 1.0;
		RcfiD.ddL = 1.0;
		RcfiD.bDominace[0] = false;
		RcfiD.bDominace[1] = false;
		break;
	case e_SP20_Long_term:
		RcfiD.dgF = 1.2;
		RcfiD.dgfa = 0.0;
		RcfiD.dgN = 1.0;
		RcfiD.dgFo = 1.0;
		RcfiD.ddL = 1.0;
		break;
	case e_SP20_Short_term:
		RcfiD.dgF = 1.2;
		RcfiD.dgfa = 0.0;
		RcfiD.dgN = 1.0;
		RcfiD.dgFo = 1.0;
		RcfiD.ddL = 0.35;
		break;
	case e_SP20_Seismic:
		RcfiD.dgF = 1.0;
		RcfiD.dgfa = 0.0;
		RcfiD.dgN = 1.0;
		RcfiD.dgFo = 1.0;
		RcfiD.ddL = 0.0;
		break;
	case e_SP20_Crane_vertical:
		RcfiD.dgF = 1.2;
		RcfiD.dgfa = 0.0;
		RcfiD.dgN = 1.0;
		RcfiD.dgFo = 1.0;
		RcfiD.ddL = 0.0;
		break;
	case e_SP20_Crane_brake:
		RcfiD.dgF = 1.2;
		RcfiD.dgfa = 0.0;
		RcfiD.dgN = 1.0;
		RcfiD.dgFo = 1.0;
		RcfiD.ddL = 0.0;
		break;
	case e_SP20_Special:
		RcfiD.dgF = 1.0;
		RcfiD.dgfa = 0.0;
		RcfiD.dgN = 1.0;
		RcfiD.dgFo = 1.0;
		RcfiD.ddL = 0.0;
		break;
	case e_SP20_Inactive:
		RcfiD.dgF = 0.;
		RcfiD.dgfa = 0.;
		RcfiD.dgN = 0.;
		RcfiD.dgFo = 0.;
		RcfiD.ddL = 0.0;
		RcfiD.bDominace[0] = RcfiD.bDominace[1] = false;
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}
void CLoadCombSP20Ctrl::GetDefaultRfiParam(DGN_LCASE_K LCaseKey, T_SP20_RCFI_D& RcfiD)
{
	RcfiD.Initialize();
	T_SP20_LCASE lCase;
	m_mapLCase.Lookup(LCaseKey.keymap, lCase);
	RcfiD.LCaseKey = LCaseKey;
	RcfiD.LoadCaseName = lCase.LoadCaseName;
	RcfiD.LoadCaseType = lCase.LoadCaseType;
	RcfiD.eLoadCaseType = lCase.type;
	RcfiD.nIndex = 0;
	GetDefaultRfiParam(RcfiD);
}

T_KEY CLoadCombSP20Ctrl::GetLCaseKeyByName(const CString& strName)
{
	POSITION pos = m_mapLCase.GetStartPosition();
	T_KEY key = 0;
	T_SP20_LCASE data;
	while (pos)
	{
		m_mapLCase.GetNextAssoc(pos, key, data);
		if (data.LoadCaseName == strName)
		{
			break;
		}
	}
	return key;
	
}

BOOL ContainsIndex(IN T_KEY index, IN const CArray<T_KEY, T_KEY>& aIndex)
{
	for (int i = 0; i < aIndex.GetSize(); i++)
	{
		if (aIndex[i] == index)
			return TRUE;
	}
	return FALSE;
}

BOOL RemoveIndex(IN T_KEY index, OUT CArray<T_KEY, T_KEY>& aIndex)
{
	for (int i = aIndex.GetSize() - 1; i >= 0; i--)
	{
		if (aIndex[i] == index)
		{
			aIndex.RemoveAt(i);
		}
	}
	return FALSE;
}

IncidenceMatrixGraph::IncidenceMatrixGraph(const CArray<T_SP20_LINK_D, T_SP20_LINK_D&>& aRelationshipD)
	:m_aRelationshipD(aRelationshipD)
{
	Init();
}

BOOL IncidenceMatrixGraph::IsLCaseType(T_KEY nIndex, eSP20_LCASE_TYPE eLoadCaseType)
{
	return m_aRelationshipD[nIndex - 1].eLoadCaseType == eLoadCaseType;
}

/* ---------------------------------------------IncidenceMatrixGraphNode------------------------------------------------------------- */
IncidenceMatrixGraphNode::IncidenceMatrixGraphNode(const IncidenceMatrixGraphNode* parent /* = NULL */)
{
	m_nLastIndex = 0;
	aNodeIndex.RemoveAll();
	if (parent)
		aNodeIndex = parent->aNodeIndex;
}

void IncidenceMatrixGraphNode::AddIndex(T_KEY nIndex,bool bcomp /* = false */)
{
	if(!bcomp)
		m_nLastIndex = nIndex;
	aNodeIndex.Add(nIndex);
}

T_KEY IncidenceMatrixGraphNode::GetLastIndex()const
{
	return m_nLastIndex;
}

/* -----------------------------------------------IncidenceMatrixGraph--------------------------------------------------------------- */
void IncidenceMatrixGraph::Init()
{
	for (int i = 0; i < m_aRelationshipD.GetSize(); i++)
	{
		if (!m_aRelationshipD[i].aLinksCase[3].IsEmpty())
		{
			for (int k = 0; k < m_aRelationshipD[i].aLinksCase[3].GetSize(); k++)
			{
				int parentComp = m_aRelationshipD[i].aLinksCase[3][k];
				m_mapComp[parentComp].Add(m_aRelationshipD[i].nIndex);
			}
			continue;
		}
		if(m_aRelationshipD[i].eLoadCaseType == e_SP20_Inactive)
			continue;
		aGraphIndex.Add(m_aRelationshipD[i].nIndex);
	}
}
//BOOL hasCompChild(T_KEY parent, T_KEY ChildIndex,)
BOOL IncidenceMatrixGraph::GetComp_gr(T_KEY parent, CArrayEx<T_KEY, T_KEY>& aChild)
{
	aChild.RemoveAll();
	if(m_mapComp.Lookup(parent, aChild))
		return TRUE;
	return FALSE;

}

BOOL IncidenceMatrixGraph::IsComp_gr_Child(T_KEY child)
{
	return !m_aRelationshipD[child - 1].aLinksCase[3].IsEmpty();
}

CArrayEx<T_KEY, T_KEY> IncidenceMatrixGraph::GetComb_gr(T_KEY nIndex)
{
	CArrayEx<T_KEY, T_KEY> aIndex;
	aIndex.Copy(m_aRelationshipD[nIndex - 1].aLinksCase[1]);
	return aIndex;
}

BOOL IncidenceMatrixGraph::NodeHasAllCombIndex(const IncidenceMatrixGraphNode& node)
{
	CArrayEx<T_KEY, T_KEY> comb_gr;
	CArrayEx<T_KEY, T_KEY> mu_ex_rg;
	
	for (int i = 0; i < node.aNodeIndex.GetSize(); i++)
	{
		int nIndex = node.aNodeIndex[i];
		comb_gr.Append(m_aRelationshipD[nIndex - 1].aLinksCase[1]);
	}
	for (int i = 0; i < node.aNodeIndex.GetSize(); i++)
	{
		int nIndex = node.aNodeIndex[i];
		//mu_ex_rg can be set within the comb group
		if (GetMu_ex_rg(nIndex, mu_ex_rg))
		{
			for (int k = 0; k < mu_ex_rg.GetCount(); k++)
			{
				RemoveIndex(mu_ex_rg[k], comb_gr);
			}
		}
	}
	for (int i = 0; i < comb_gr.GetSize(); i++)
	{
		if (!ContainsIndex(comb_gr[i], node.aNodeIndex))
			return FALSE;
	}
	return TRUE;
}

BOOL IncidenceMatrixGraph::GetMu_ex_rg(T_KEY nIndex, CArrayEx<T_KEY, T_KEY>& aIndex)
{
	aIndex.RemoveAll();
	aIndex.Copy(m_aRelationshipD[nIndex - 1].aLinksCase[2]);
	return !aIndex.IsEmpty();
}

CArrayEx<T_KEY, T_KEY> IncidenceMatrixGraph::GetMu_ex_rg(T_KEY nIndex)
{
	CArrayEx<T_KEY, T_KEY> aIndex;
	aIndex.Copy(m_aRelationshipD[nIndex - 1].aLinksCase[2]);
	return aIndex;
}

BOOL IncidenceMatrixGraph::IsMuexPath(T_KEY nIndex, const IncidenceMatrixGraphNode& node)
{
	for (int i = 0; i < node.aNodeIndex.GetSize(); i++)
	{
		if(IsMuex(nIndex,node.aNodeIndex[i]))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL IncidenceMatrixGraph::IsMuex(T_KEY nIndex, T_KEY node)
{
	return ContainsIndex(nIndex, m_aRelationshipD[node - 1].aLinksCase[2]);
}

void IncidenceMatrixGraph::AddNode(IN IncidenceMatrixGraphNode& node)
{
	m_aGraphIndex.Add(node.aNodeIndex);
}

T_KEY IncidenceMatrixGraph::GetNextIndex(int nIndex, bool bcomp /* = false */)
{
	T_KEY nNextIndex = 0;
	if (bcomp)
	{
		for (int i = 0; i < m_aRelationshipD.GetSize(); i++)
		{
			if (nIndex < m_aRelationshipD[i].nIndex)
			{
				nNextIndex = m_aRelationshipD[i].nIndex;
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < aGraphIndex.GetSize(); i++)
		{
			if (nIndex < aGraphIndex[i])
			{
				nNextIndex = aGraphIndex[i];
				break;
			}
		}
	}
	return nNextIndex;
}

void IncidenceMatrixGraph::GetGraphIndex(CArrayEx<CArrayEx<T_KEY, T_KEY>, CArrayEx<T_KEY, T_KEY>&>& aGraphIndex)
{
	aGraphIndex.RemoveAll();
	aGraphIndex.Copy(m_aGraphIndex);
}

/* -----------------------------------------------IncidenceMatrixGraph--------------------------------------------------------------- */

BOOL CLoadCombSP20Ctrl::GraphComb(const IncidenceMatrixGraphNode& path, IncidenceMatrixGraph& Graph)
{
	T_KEY nIndex = path.GetLastIndex();
	while (1)
	{
		nIndex = Graph.GetNextIndex(nIndex);
		if (nIndex == 0)
			break;
		if (Graph.IsMuexPath(nIndex, path))
			continue;
		IncidenceMatrixGraphNode ChildPath(path);
		ChildPath.AddIndex(nIndex);
		if (Graph.NodeHasAllCombIndex(ChildPath))
		{
			Graph.AddNode(ChildPath);
			CompItemComb(ChildPath, Graph,ChildPath.aNodeIndex.GetSize());
		}
		GraphComb(ChildPath, Graph);
	}
	return TRUE;
}

BOOL CLoadCombSP20Ctrl::CompItemComb(const IncidenceMatrixGraphNode& path, IncidenceMatrixGraph& Graph,int nMainCount)
{
	CArrayEx<T_KEY, T_KEY> Allcomp_gr;
	CArrayEx<T_KEY, T_KEY> comp_gr;
	for (int k = 0; k < nMainCount; k++)
	{
		if (Graph.GetComp_gr(path.aNodeIndex[k], comp_gr))
		{
			Allcomp_gr.Append(comp_gr);
		}
	}
	CMapEx<T_KEY, T_KEY, int, int> mapComp;
	for (int i = 0; i < Allcomp_gr.GetSize(); i++)
	{
		mapComp[Allcomp_gr[i]] = 1;
	}
	comp_gr.RemoveAll();
	mapComp.GetSortedKeyListforUintK(comp_gr);
	std::function<void(int, const IncidenceMatrixGraphNode&)> IterComp = [&](int nIndex, const IncidenceMatrixGraphNode& tpath)
	{
		if (nIndex >= comp_gr.GetSize())
			return;
		IncidenceMatrixGraphNode ChildPath(tpath);
		ChildPath.AddIndex(comp_gr[nIndex]);
		if (Graph.NodeHasAllCombIndex(ChildPath))
		{
			Graph.AddNode(ChildPath);
			IterComp(nIndex + 1, ChildPath);
		}
		IterComp(nIndex + 1, tpath);
	};
	IterComp(0, path);
	return TRUE;
}

void CLoadCombSP20Ctrl::GetIncidenceMatrixTable(IN const CArray<T_SP20_LINK_D, T_SP20_LINK_D &>& aRelationshipD, OUT CArrayEx<CArrayEx<T_KEY, T_KEY>, CArrayEx<T_KEY, T_KEY> &>& PathTable)
{
	// incidence_matrix_main : table
	PathTable.RemoveAll();
	if (aRelationshipD.IsEmpty())
	{
		ASSERT(FALSE);
		return;
	}
	IncidenceMatrixGraph Graph(aRelationshipD);
	const CArray<T_KEY, T_KEY>& aGraphIndex = Graph.aGraphIndex;
	if (aGraphIndex.IsEmpty())
		return;
	/*
		1. 确定起点:graph - 可能有多个父节点
		第一个点可能多起点 : 不清楚是不是必须有恒载, 此处考虑可能没有恒载的情况...
		有恒载 : 则应该是 : 互斥里所有的恒载都是起点
	*/
	CArray<T_KEY, T_KEY> aStartIndex;
	aStartIndex.Add(aGraphIndex[0]);
	/*
		恒载的互斥index;
		e.g. 1 2 3 4 5是恒载, 1和2, 3, 4都互斥, 2和3是combination.起点是1, 2, 4.这个变量用来排除3
		e.g. 1 2 3 4 5是恒载, 1和2, 3, 4都互斥, 2和4是combination.起点是1, 2, 3.这个变量用来排除4
		e.g. 1 2 3 4 5是恒载, 1和2, 3互斥, 2和3互斥, 起点是1, 2, 3
	*/
	CArray<T_KEY, T_KEY> aMuExIndex;
	// 第一个工况需要排除非companion
	if (Graph.IsLCaseType(aStartIndex[0], e_SP20_Constant))
	{
		CArrayEx<T_KEY, T_KEY> mu_ex_rg = Graph.GetMu_ex_rg(aStartIndex[0]);
		for (int i = 0; i < mu_ex_rg.GetSize(); i++)
		{
			int nIndex = mu_ex_rg[i];
			if (Graph.IsComp_gr_Child(nIndex))
				continue;
			if (!Graph.IsLCaseType(nIndex, e_SP20_Constant))
				break;
			if (ContainsIndex(nIndex, aMuExIndex))
				continue;
			aMuExIndex.Append(Graph.GetComb_gr(nIndex));
			aStartIndex.Add(nIndex);
		}

		// 2.找到终点:构建父节点
		CArray<IncidenceMatrixGraphNode, IncidenceMatrixGraphNode&> aGraphNode;
		aGraphNode.SetSize(aStartIndex.GetSize());
		
		for (int i = 0; i < aStartIndex.GetSize(); i++)
		{
			int nIndex = aStartIndex[i];
			aGraphNode[i].AddIndex(nIndex);
			while (1)
			{
				nIndex = Graph.GetNextIndex(nIndex);
				if (nIndex == 0)
					break;
				if (!Graph.IsLCaseType(nIndex, e_SP20_Constant))
					break;
				if (ContainsIndex(nIndex, aStartIndex))
					continue;
				if(Graph.IsMuexPath(nIndex,aGraphNode[i]))
					continue;
				aGraphNode[i].AddIndex(nIndex);
			}
			// If not all comb nodes are included, you need to continue looking down
			if(Graph.NodeHasAllCombIndex(aGraphNode[i]))
				Graph.AddNode(aGraphNode[i]);
			GraphComb(aGraphNode[i], Graph);
		}

	}
	else
	{
		// 2.没有恒载
		IncidenceMatrixGraphNode node;
		node.AddIndex(aStartIndex[0]);
		if (Graph.NodeHasAllCombIndex(node))
		{
			Graph.AddNode(node);
			CompItemComb(node, Graph, node.aNodeIndex.GetSize());
		}
		GraphComb(node, Graph);
	}
	Graph.GetGraphIndex(PathTable);
}
class LoadCombDB;
class LoadCombNode
{
public:
	LoadCombNode();
	void Init(const CArrayEx<T_KEY, T_KEY>& aIndex, eSP20_LCASE_TYPE eType, LoadCombDB* pDB, int nServiceability = 0);
	void GenerateFactor();
	void SetPathType();

	CArrayEx<T_KEY, T_KEY> m_aIndex;
	CArrayEx<CArrayEx<double, double>, CArrayEx<double, double>&> m_aFactor;
	CArray<CString, CString&> m_aDescription;
	eSP20_LCASE_TYPE m_type;
private:
	void GenerateConstantFactor();
	void GenerateTermFactor();
	eSP20_LCASE_TYPE GetLCaseType();
private:
	
	LoadCombDB* m_pDB;

	//PathType
	eSP20_LCASE_TYPE m_PathType;
	int m_nServiceability;
};

class LoadCombDB
{
public:
	CMap<T_KEY, T_KEY, T_SP20_LPSI_D, T_SP20_LPSI_D&> mapLpsi;
	CMap<T_KEY, T_KEY, T_SP20_RCFI_D, T_SP20_RCFI_D&> mapRcfi;
	CMap<T_KEY, T_KEY, T_SP20_LINK_D, T_SP20_LINK_D&> mapLink;

	BOOL GenerateLComb(OUT CArray<T_LCOM_D, T_LCOM_D&>& aLcomD, IN const CArrayEx<T_KEY, T_KEY>(&aClassifyIndex)[6],IN int nTerm = 0);
	BOOL ClassifyIndex(IN const CArrayEx<T_KEY, T_KEY>& aInLCase, OUT CArrayEx<T_KEY, T_KEY>(&aIndex)[6]);
};

void CLoadCombSP20Ctrl::GenerateLcomAndAddDB(int nOption, int iLcomType, BOOL bAdditional[3]) 
{
	GeneratePsiParam();
	GenerateRfiParam();
	GenerateLoadCaseLink();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	//1.Cache Map
	LoadCombDB pDB;
	pDB.mapLpsi.RemoveAll();
	pDB.mapRcfi.RemoveAll();
	pDB.mapLink.RemoveAll();
	for (int i = 0; i < m_aLcaseSP20PsiFactor.GetSize(); i++)
	{
		pDB.mapLpsi[m_aLcaseSP20PsiFactor[i].nIndex] = m_aLcaseSP20PsiFactor[i];
	}
	for (int i = 0; i < m_aLcomSP20RfiFactor.GetSize(); i++)
	{
		pDB.mapRcfi[m_aLcomSP20RfiFactor[i].nIndex] = m_aLcomSP20RfiFactor[i];
	}
	for (int i = 0; i < m_aLcaseSP20LinkData.GetSize(); i++)
	{
		pDB.mapLink[m_aLcaseSP20LinkData[i].nIndex] = m_aLcaseSP20LinkData[i];
	}

	//2.GetIncidenceMatrixTable
	CArrayEx<CArrayEx<T_KEY, T_KEY>, CArrayEx<T_KEY, T_KEY>&> PathTable;
	GetIncidenceMatrixTable(m_aLcaseSP20LinkData, PathTable);
	//3.nOption == 1 DeleteAllLcom
	int LoadCombNameNo = 0;
	if (nOption == 1)
	{
		CArray<CString, CString&> strLoadCombName;
		pDoc->m_pAttrCtrl->GetLcomList(iLcomType, strLoadCombName);
		if (!strLoadCombName.IsEmpty())
		{
			if (!pDoc->m_pDataCtrl->DelLcom(iLcomType, strLoadCombName))
				return;
		}
	}
	else
	{
		LoadCombNameNo = Get_MaxLoadCombNameNo(iLcomType);
	}
	//
	auto GetLComName = [&]()
	{
		CString str;
		LoadCombNameNo++;
		switch (iLcomType)
		{
		case D_LCOMTYPE_GENERAL:  str.Format(_T("gLCB%-3d"), LoadCombNameNo);  break;
		case D_LCOMTYPE_STEEL:    str.Format(_T("sLCB%-3d"), LoadCombNameNo);  break;
		case D_LCOMTYPE_CONCRETE: str.Format(_T("cLCB%-3d"), LoadCombNameNo);  break;
		case D_LCOMTYPE_FDN:      str.Format(_T("fLCB%-3d"), LoadCombNameNo);  break;
		case D_LCOMTYPE_SRC:      str.Format(_T("rLCB%-3d"), LoadCombNameNo);  break;
		case D_LCOMTYPE_ALUMINUM: str.Format(_T("aLCB%-3d"), LoadCombNameNo);  break;
		case D_LCOMTYPE_CFSTEEL:  str.Format(_T("cfLCB%-3d"), LoadCombNameNo); break;
		case D_LCOMTYPE_SEISMIC:  str.Format(_T("eLCB%-3d"), LoadCombNameNo);  break;
		case D_LCOMTYPE_LINEAR_EVAL: str.Format(_T("leLCB%-3d"), LoadCombNameNo);  break;
		default: ASSERT(0); break;
		}
		return str;
	};
	T_SP20_LCASE SP20LCase;
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD;
	CArray<T_LCOM_D, T_LCOM_D&> aIterLcomD;
	T_LCOM_D LcomD;
	CArrayEx<T_KEY, T_KEY> aClassifyIndex[6];
	CArrayEx<double, double> aFactor;

	for (int i = 0; i < PathTable.GetSize(); i++)
	{
		//
		pDB.ClassifyIndex(PathTable[i], aClassifyIndex);
		bool bAddCurPath = false;
		if (bAdditional[1] && !aClassifyIndex[5].IsEmpty())
		{
			bAddCurPath = true;
		}
		else if (bAdditional[2] && !aClassifyIndex[3].IsEmpty())
		{
			bAddCurPath = true;
		}
		else if(bAdditional[0] && aClassifyIndex[3].IsEmpty() && aClassifyIndex[5].IsEmpty())
		{
			bAddCurPath = true;
		}
		else
			continue;
		if (bAddCurPath)
		{
			/*
			* nActive : 1-Strength,2-Serviceability,3-Special
			* nServLcomType : 0-Default, 1-short-term,2-long-term,
			*/
			//1.Strength
			pDB.GenerateLComb(aIterLcomD, aClassifyIndex);
			for (int k = 0; k < aIterLcomD.GetSize(); k++)
			{
				aIterLcomD[k].LoadCombName = GetLComName();
				aIterLcomD[k].nActive = 1;
				aIterLcomD[k].nServLcomType = 2;
				if (aIterLcomD[k].Description.GetLength() > D_LCOM_DESC_SIZE)
					aIterLcomD[k].Description = aIterLcomD[k].Description.Left(D_LCOM_DESC_SIZE);
				//short
				if (!aClassifyIndex[2].IsEmpty())
				{
					aIterLcomD[k].nServLcomType = 1;
				}
				//Seismic
				if (!aClassifyIndex[3].IsEmpty())
				{
					aIterLcomD[k].nServLcomType = 1;
				}
				//Special
				if (!aClassifyIndex[5].IsEmpty())
				{
					aIterLcomD[k].nActive = 3;
					aIterLcomD[k].nServLcomType = 1;
				}
				//If the type is general, set Active to 1.
				if(iLcomType == D_LCOMTYPE_GENERAL)
					aIterLcomD[k].nActive = 1;
			}
			aLcomD.Append(aIterLcomD);
			
			if (aClassifyIndex[3].IsEmpty() && aClassifyIndex[5].IsEmpty())
			{
				//2.Design long term combinations
				if (!aClassifyIndex[2].IsEmpty())
				{
					pDB.GenerateLComb(aIterLcomD, aClassifyIndex, 1);
					for (int k = 0; k < aIterLcomD.GetSize(); k++)
					{
						aIterLcomD[k].LoadCombName = GetLComName();
						aIterLcomD[k].nActive = 1;
						aIterLcomD[k].nServLcomType = 2;
						if(aIterLcomD[k].Description.GetLength() > D_LCOM_DESC_SIZE)
							aIterLcomD[k].Description = aIterLcomD[k].Description.Left(D_LCOM_DESC_SIZE);
					}
					aLcomD.Append(aIterLcomD);
				}
				//3.Serviceability
				pDB.GenerateLComb(aIterLcomD, aClassifyIndex, 2);
				for (int k = 0; k < aIterLcomD.GetSize(); k++)
				{
					aIterLcomD[k].LoadCombName = GetLComName();
					aIterLcomD[k].nActive = 2;
					aIterLcomD[k].nServLcomType = 2;
					//Seismic
					if (!aClassifyIndex[3].IsEmpty())
					{
						aIterLcomD[k].nServLcomType = 1;
					}
					if(iLcomType == D_LCOMTYPE_GENERAL)
						aIterLcomD[k].nActive = 1;
					if(aIterLcomD[k].Description.GetLength() > D_LCOM_DESC_SIZE)
						aIterLcomD[k].Description = aIterLcomD[k].Description.Left(D_LCOM_DESC_SIZE);
				}
				aLcomD.Append(aIterLcomD);
			}
		}
	}
	pDoc->m_pDataCtrl->AddLcom(iLcomType, aLcomD);
		
}

void CLoadCombSP20Ctrl::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	switch (lHint)
	{
	case D_UPDATE_DEFAULT:
	case D_UPDATE_BUFFER_BEFORE:
	case D_UPDATE_BUFFER_AFTER:
	{
		UpdateBuffer();
		break;
	}
	//case D_UPDATE_LOADCASE_CHANGED:
	case D_UPDATE_DB_CLOSE:
	{
		ClearLCaseCache();
		break;
	}
	}
}

void CLoadCombSP20Ctrl::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case UR_LCTP_ADD:
		case UR_LCTP_DEL:
		case UR_LCTP_LMT:
		case UR_STLD_ADD:
		case UR_STLD_DEL:
		case UR_STLD_MFS:
		case UR_STLD_MFD:
		case UR_STLD_LMT:
		case UR_SPLC_ADD:
		case UR_SPLC_DEL:
		case UR_SPLC_MFS:
		case UR_SPLC_MFD:
		case UR_SPLC_LMT:
			ClearLCaseCache();
			return;
		default:
			break;
		}
	} // end of while
}


eSP20_LCASE_TYPE CLoadCombSP20Ctrl::GetLCaseType(int caseKey, int iAnaType)
{
	DGN_LCASE_K LCaseKey;
	LCaseKey.key.caseKey = caseKey;
	LCaseKey.key.iAnaType = iAnaType;
	T_SP20_LCASE SP20LCase;
	SP20LCase.Initialize();
	if (!m_mapLCase.Lookup(LCaseKey.keymap, SP20LCase))
	{
		ASSERT(FALSE);
	}
	return SP20LCase.type;

}

void CLoadCombSP20Ctrl::SortDgnLCase(CArray<DGN_LCASE_K, DGN_LCASE_K&>& aLCaseK)
{
	/*
	* load_case in incidence_matrix_main table should be sorted in the following order:
	* 1. Constant loads
	* 2. Loads with mu_ex_rg != 0 ;
	* 3. Long-term, short-term, crane loads
	* 4. Seismic and special loads
	*/
	CMap<UINT, UINT, CArrayEx<DGN_LCASE_K, DGN_LCASE_K&>, CArrayEx<DGN_LCASE_K, DGN_LCASE_K&>&> mapSort;
	T_SP20_LCASE LCaseData;
	for (int i = 0; i < aLCaseK.GetSize(); i++)
	{
		// 排序
		m_mapLCase.Lookup(aLCaseK[i].keymap, LCaseData);
		CArrayEx<DGN_LCASE_K, DGN_LCASE_K&>& arrLcase = mapSort[LCaseData.type];
		arrLcase.Add(aLCaseK[i]);
	}
	aLCaseK.RemoveAll();
	for (int i = e_SP20_Constant; i <= e_SP20_Inactive; i++)
	{
		CArrayEx<DGN_LCASE_K, DGN_LCASE_K&>& aConstant = mapSort[i];
		qsort(aConstant.GetData(), aConstant.GetSize(), sizeof(DGN_LCASE_K), CCompFunc::UINTAsc);
		aLCaseK.Append(aConstant);
	}
}

void CLoadCombSP20Ctrl::GetLoadCase(CArray<DGN_LCASE_K, DGN_LCASE_K&>& aLCaseK)
{
	aLCaseK.RemoveAll();
	aLCaseK.Copy(m_aSortLCase);
}

void CLoadCombSP20Ctrl::GenerateLoadCase(CArray<DGN_LCASE_K, DGN_LCASE_K &>& aLCaseK)
{
	aLCaseK.RemoveAll();
	m_mapLCase.RemoveAll();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	DGN_LCASE_K lCaseKey;
	T_SP20_LCASE lCaseData;
	//1. 记录下用户定义的long-term LoadCase
	T_LCTP_D LctpData; LctpData.Initialize();
	BOOL bExitLctp = pDoc->m_pAttrCtrl->GetLctp(LctpData);
	CMap<UINT, UINT, UINT, UINT> mapLongLCase;
	for (int i = 0; i < LctpData.arLongKey.GetSize(); i++)
	{
		lCaseKey.key.iAnaType = LctpData.arLongType[i];
		lCaseKey.key.caseKey = LctpData.arLongKey[i];
		mapLongLCase[lCaseKey.keymap] = e_SP20_Long_term;
	}

	//2.遍历所有的荷载类.
	//  即时用户把恒载(D)分为long-term 最后归类为 Constant.
	//  即时用户把地震(E)和RX分为long-term 最后归类为 Seismic.
	//  Crane同样
	T_STLD_D rData;
	CArray<T_STLD_K, T_STLD_K> KeyList;
	pDoc->m_pAttrCtrl->GetStldKeyList(KeyList);
	lCaseKey.key.iAnaType = D_LCOM_STATIC;
	for (int i = 0; i < KeyList.GetSize(); i++)
	{
		int key = KeyList.GetAt(i);
		rData.Initialize();
		if (!pDoc->m_pAttrCtrl->GetStld(key, rData))	
			continue;
		lCaseData.Initialize();
		lCaseKey.key.caseKey = key;
		lCaseData.LCaseKey = lCaseKey;
		lCaseData.LoadCaseName = rData.LoadCaseName;
		lCaseData.LoadCaseType = rData.LoadCaseType;
		if (CDBLib::IsStldDL(rData.LoadCaseType))
		{
			lCaseData.type = e_SP20_Constant;
			lCaseData.LoadCaseType = _T("Constant");
		}
		else if (rData.LoadCaseType == _T("CO") || rData.LoadCaseType == _T("EX"))
		{
			//Collision Load || Explosion Load
			lCaseData.type = e_SP20_Special;
			lCaseData.LoadCaseType = _T("Special");
		}
		else if (CDBLib::IsStldEL(rData.LoadCaseType))
		{
			lCaseData.type = e_SP20_Seismic;
			lCaseData.LoadCaseType = _T("Seismic");
		}
		else
		{
			if ((bExitLctp && mapLongLCase.PLookup(lCaseKey.keymap)) || (!bExitLctp && CDBLib::IsStldLL(rData.LoadCaseType)))
			{
				lCaseData.LoadCaseType = _T("Long-term");
				lCaseData.type = e_SP20_Long_term;
			}
			else
			{
				lCaseData.LoadCaseType = _T("Short-term");
				lCaseData.type = e_SP20_Short_term;
			}
		}
		aLCaseK.Add(lCaseKey);
		m_mapLCase[lCaseKey.keymap] = lCaseData;
	}

	lCaseKey.key.iAnaType = D_LCOM_SPECTRUM;
	T_SPLC_K SplcKey;
	T_SPLC_D SplcData;
	CArray<T_SPLC_K, T_SPLC_K> aSplcKey;
	pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcKey);
	for (int i = 0; i < aSplcKey.GetSize(); i++)
	{
		SplcKey = aSplcKey[i];
		pDoc->m_pAttrCtrl->GetSplc(SplcKey, SplcData);
		lCaseData.Initialize();
		lCaseKey.key.caseKey = SplcKey;
		lCaseData.LCaseKey = lCaseKey;
		lCaseData.LoadCaseName = SplcData.LoadCaseName+ _T("(RS)");
		//lCaseData.LoadCaseType = _T("RS");
		lCaseData.LoadCaseType = _T("Seismic");
		lCaseData.type = e_SP20_Seismic;
		aLCaseK.Add(lCaseKey);
		m_mapLCase[lCaseKey.keymap] = lCaseData;
	}

	lCaseKey.key.iAnaType = D_LCOM_HISTORY;
	T_THIS_K ThisKey;
	T_THIS_D ThisData;
	CArray<T_THIS_K, T_THIS_K> aThisKey;
	pDoc->m_pAttrCtrl->GetThisKeyList(aThisKey);
	for (int i = 0; i < aThisKey.GetSize(); i++)
	{
		ThisKey = aThisKey[i];
		pDoc->m_pAttrCtrl->GetThis(ThisKey, ThisData);
		lCaseData.Initialize();
		lCaseKey.key.caseKey = ThisKey;
		lCaseData.LCaseKey = lCaseKey;
		lCaseData.LoadCaseName = ThisData.LoadCaseName;
		//lCaseData.LoadCaseType = _T("THIS");
		if (mapLongLCase.PLookup(lCaseKey.keymap))
		{
			lCaseData.LoadCaseType = _T("Long-term");
			lCaseData.type = e_SP20_Long_term;
		}
		else
		{
			lCaseData.LoadCaseType = _T("Short-term");
			lCaseData.type = e_SP20_Short_term;
		}
		aLCaseK.Add(lCaseKey);
		m_mapLCase[lCaseKey.keymap] = lCaseData;
	}

	lCaseKey.key.iAnaType = D_LCOM_MOVING;
	T_MVCD_D DataMvcd;
	if (!pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	switch (DataMvcd.nCodeType)
	{
	case D_MOVE_CODE_NONE:
		break;
	case D_MOVE_CODE_AASHTO_STAN:
	case D_MOVE_CODE_AASHTO_LRFD:
	case D_MOVE_CODE_TAIWAN:
	case D_MOVE_CODE_KOREA:
	case D_MOVE_CODE_CANADA:
	case D_MOVE_CODE_PENDOT:
	case D_MOVE_CODE_EURO_BS:
	case D_MOVE_CODE_RUSSIA:
	case D_MOVE_CODE_KOREA_LRFD_2011:
	case D_MOVE_CODE_AUSTRALIA:
	case D_MOVE_CODE_POLAND:
	case D_MOVE_CODE_SOUTH_AFRICA:
	{
		T_MVLD_K MvldKey;
		T_MVLD_D MvldData;
		CArray<T_MVLD_K, T_MVLD_K> aMvldKey;
		pDoc->m_pAttrCtrl->GetMvldKeyList(aMvldKey);
		for (int i = 0; i < aMvldKey.GetSize(); i++)
		{
			MvldKey = aMvldKey[i];
			pDoc->m_pAttrCtrl->GetMvld(MvldKey, MvldData);
			lCaseData.Initialize();
			lCaseKey.key.caseKey = MvldKey;
			lCaseData.LCaseKey = lCaseKey;
			lCaseData.LoadCaseName = MvldData.LoadCaseName;
			//lCaseData.LoadCaseType = _T("MV");
			if (mapLongLCase.PLookup(lCaseKey.keymap))
			{
				lCaseData.LoadCaseType = _T("Long-term");
				lCaseData.type = e_SP20_Long_term;
			}
			else
			{
				lCaseData.LoadCaseType = _T("Short-term");
				lCaseData.type = e_SP20_Short_term;
			}
			aLCaseK.Add(lCaseKey);
			m_mapLCase[lCaseKey.keymap] = lCaseData;
		}
	}
	break;
	case D_MOVE_CODE_CHINA:
	{
		T_MVLDch_K MvldchKey;
		T_MVLDch_D MvldchData;
		CArray<T_MVLDch_K, T_MVLDch_K> aMvldchKey;
		pDoc->m_pAttrCtrl->GetMvldchKeyList(aMvldchKey);
		for (int i = 0; i < aMvldchKey.GetSize(); i++)
		{
			MvldchKey = aMvldchKey[i];
			pDoc->m_pAttrCtrl->GetMvldch(MvldchKey, MvldchData);
			lCaseData.Initialize();
			lCaseKey.key.caseKey = MvldchKey;
			lCaseData.LCaseKey = lCaseKey;
			lCaseData.LoadCaseName = MvldchData.LoadCaseName;
			//lCaseData.LoadCaseType = _T("MV");
			if (mapLongLCase.PLookup(lCaseKey.keymap))
			{
				lCaseData.LoadCaseType = _T("Long-term");
				lCaseData.type = e_SP20_Long_term;
			}
			else
			{
				lCaseData.LoadCaseType = _T("Short-term");
				lCaseData.type = e_SP20_Short_term;
			}
			aLCaseK.Add(lCaseKey);
			m_mapLCase[lCaseKey.keymap] = lCaseData;
		}
	}
	break;
	case D_MOVE_CODE_JAPAN:
	{
		T_MVLDjp_K MvldjpKey;
		T_MVLDjp_D MvldjpData;
		CArray<T_MVLDjp_K, T_MVLDjp_K> aMvldjpKey;
		pDoc->m_pAttrCtrl->GetMvldjpKeyList(aMvldjpKey);
		for (int i = 0; i < aMvldjpKey.GetSize(); i++)
		{
			MvldjpKey = aMvldjpKey[i];
			pDoc->m_pAttrCtrl->GetMvldjp(MvldjpKey, MvldjpData);
			lCaseData.Initialize();
			lCaseKey.key.caseKey = MvldjpKey;
			lCaseData.LCaseKey = lCaseKey;
			lCaseData.LoadCaseName = MvldjpData.LoadCaseName;
			//lCaseData.LoadCaseType = _T("MV");
			if (mapLongLCase.PLookup(lCaseKey.keymap))
			{
				lCaseData.LoadCaseType = _T("Long-term");
				lCaseData.type = e_SP20_Long_term;
			}
			else
			{
				lCaseData.LoadCaseType = _T("Short-term");
				lCaseData.type = e_SP20_Short_term;
			}
			aLCaseK.Add(lCaseKey);
			m_mapLCase[lCaseKey.keymap] = lCaseData;
		}
	}
	break;
	case D_MOVE_CODE_INDIA:
	{
		T_MVLDid_K MvldidKey;
		T_MVLDid_D MvldidData;
		CArray<T_MVLDid_K, T_MVLDid_K> aMvldidKey;
		pDoc->m_pAttrCtrl->GetMvldidKeyList(aMvldidKey);
		for (int i = 0; i < aMvldidKey.GetSize(); i++)
		{
			MvldidKey = aMvldidKey[i];
			pDoc->m_pAttrCtrl->GetMvldid(MvldidKey, MvldidData);
			lCaseData.Initialize();
			lCaseKey.key.caseKey = MvldidKey;
			lCaseData.LCaseKey = lCaseKey;
			lCaseData.LoadCaseName = MvldidData.LoadCaseName;
			//lCaseData.LoadCaseType = _T("MV");
			if (mapLongLCase.PLookup(lCaseKey.keymap))
			{
				lCaseData.LoadCaseType = _T("Long-term");
				lCaseData.type = e_SP20_Long_term;
			}
			else
			{
				lCaseData.LoadCaseType = _T("Short-term");
				lCaseData.type = e_SP20_Short_term;
			}
			aLCaseK.Add(lCaseKey);
			m_mapLCase[lCaseKey.keymap] = lCaseData;
		}
	}
	break;
	case D_MOVE_CODE_BS:
	{
		T_MVLDbs_K MvldbsKey;
		T_MVLDbs_D MvldbsData;
		CArray<T_MVLDbs_K, T_MVLDbs_K> aMvldbsKey;
		pDoc->m_pAttrCtrl->GetMvldbsKeyList(aMvldbsKey);
		for (int i = 0; i < aMvldbsKey.GetSize(); i++)
		{
			MvldbsKey = aMvldbsKey[i];
			pDoc->m_pAttrCtrl->GetMvldbs(MvldbsKey, MvldbsData);
			lCaseData.Initialize();
			lCaseKey.key.caseKey = MvldbsKey;
			lCaseData.LCaseKey = lCaseKey;
			lCaseData.LoadCaseName = MvldbsData.LoadCaseName;
			//lCaseData.LoadCaseType = _T("MV");
			if (mapLongLCase.PLookup(lCaseKey.keymap))
			{
				lCaseData.LoadCaseType = _T("Long-term");
				lCaseData.type = e_SP20_Long_term;
			}
			else
			{
				lCaseData.LoadCaseType = _T("Short-term");
				lCaseData.type = e_SP20_Short_term;
			}
			aLCaseK.Add(lCaseKey);
			m_mapLCase[lCaseKey.keymap] = lCaseData;
		}
	}
	break;
	case D_MOVE_CODE_FRANCE:
	{
		T_MVLDfr_K MvldfrKey;
		T_MVLDfr_D MvldfrData;
		CArray<T_MVLDfr_K, T_MVLDfr_K> aMvldfrKey;
		pDoc->m_pAttrCtrl->GetMvldfrKeyList(aMvldfrKey);
		for (int i = 0; i < aMvldfrKey.GetSize(); i++)
		{
			MvldfrKey = aMvldfrKey[i];
			pDoc->m_pAttrCtrl->GetMvldfr(MvldfrKey, MvldfrData);
			lCaseData.Initialize();
			lCaseKey.key.caseKey = MvldfrKey;
			lCaseData.LCaseKey = lCaseKey;
			lCaseData.LoadCaseName = MvldfrData.LoadCaseName;
			//lCaseData.LoadCaseType = _T("MV");
			if (mapLongLCase.PLookup(lCaseKey.keymap))
			{
				lCaseData.LoadCaseType = _T("Long-term");
				lCaseData.type = e_SP20_Long_term;
			}
			else
			{
				lCaseData.LoadCaseType = _T("Short-term");
				lCaseData.type = e_SP20_Short_term;
			}
			aLCaseK.Add(lCaseKey);
			m_mapLCase[lCaseKey.keymap] = lCaseData;
		}
	}
	break;
	case D_MOVE_CODE_TRANS:
	{
		T_MVLDtr_K MvldKey;
		T_MVLDtr_D MvldData;
		CArray<T_MVLDtr_K, T_MVLDtr_K> aMvldKey;
		pDoc->m_pAttrCtrl->GetMvldtrKeyList(aMvldKey);
		for (int i = 0; i < aMvldKey.GetSize(); i++)
		{
			MvldKey = aMvldKey[i];
			pDoc->m_pAttrCtrl->GetMvldtr(MvldKey, MvldData);
			lCaseData.Initialize();
			lCaseKey.key.caseKey = MvldKey;
			lCaseData.LCaseKey = lCaseKey;
			lCaseData.LoadCaseName = MvldData.LoadCaseName;
			//lCaseData.LoadCaseType = _T("MV");
			if (mapLongLCase.PLookup(lCaseKey.keymap))
			{
				lCaseData.LoadCaseType = _T("Long-term");
				lCaseData.type = e_SP20_Long_term;
			}
			else
			{
				lCaseData.LoadCaseType = _T("Short-term");
				lCaseData.type = e_SP20_Short_term;
			}
			aLCaseK.Add(lCaseKey);
			m_mapLCase[lCaseKey.keymap] = lCaseData;
		}
	}
	break;
	default:
		ASSERT(0);
		break;
	}

	lCaseKey.key.iAnaType = D_LCOM_SETTLEMENT;
	T_SMLC_K SmlcKey;
	T_SMLC_D SmlcData;
	CArray<T_SMLC_K, T_SMLC_K> aSmlcKey;
	pDoc->m_pAttrCtrl->GetSmlcKeyList(aSmlcKey);
	for (int i = 0; i < aSmlcKey.GetSize(); i++)
	{
		SmlcKey = aSmlcKey[i];
		pDoc->m_pAttrCtrl->GetSmlc(SmlcKey, SmlcData);
		lCaseKey.key.caseKey = SmlcKey;
		lCaseData.LCaseKey = lCaseKey;
		lCaseData.LoadCaseName = SmlcData.LoadCaseName;
		//lCaseData.LoadCaseType = _T("SMLC");
		if (mapLongLCase.PLookup(lCaseKey.keymap))
		{
			lCaseData.LoadCaseType = _T("Long-term");
			lCaseData.type = e_SP20_Long_term;
		}
		else
		{
			lCaseData.LoadCaseType = _T("Short-term");
			lCaseData.type = e_SP20_Short_term;
		}
		aLCaseK.Add(lCaseKey);
		m_mapLCase[lCaseKey.keymap] = lCaseData;
	}

	lCaseKey.key.iAnaType = D_LCOM_STAGE;
	T_SGLD_K SgldKey;
	T_SGLD_D SgldData;
	CArray<T_SGLD_K, T_SGLD_K> aSgldKey;
	pDoc->m_pAttrCtrl->GetSgldKeyList(aSgldKey);
	for (int i = 0; i < aSgldKey.GetSize(); i++)
	{
		SgldKey = aSgldKey[i];
		pDoc->m_pAttrCtrl->GetSgld(SgldKey, SgldData);
		lCaseKey.key.caseKey = SgldKey;
		lCaseData.LCaseKey = lCaseKey;
		lCaseData.LoadCaseName = SgldData.LoadCaseName;
		//lCaseData.LoadCaseType = _T("STAGE");
		if (mapLongLCase.PLookup(lCaseKey.keymap))
		{
			lCaseData.LoadCaseType = _T("Long-term");
			lCaseData.type = e_SP20_Long_term;
		}
		else
		{
			lCaseData.LoadCaseType = _T("Short-term");
			lCaseData.type = e_SP20_Short_term;
		}
		aLCaseK.Add(lCaseKey);
		m_mapLCase[lCaseKey.keymap] = lCaseData;
	}

	lCaseKey.key.iAnaType = D_LCOM_ECCENSPEC;
	T_ESPC_K EspcKey;
	T_ESPC_D EspcData;
	CArray<T_ESPC_K, T_ESPC_K> aEspcKey;
	pDoc->m_pAttrCtrl->GetEspcKeyList(aEspcKey);
	for (int i = 0; i < aEspcKey.GetSize(); i++)
	{
		EspcKey = aEspcKey[i];
		pDoc->m_pAttrCtrl->GetEspc(EspcKey, EspcData);
		if (!EspcData.bConsiderEccen)
			continue;
		lCaseKey.key.caseKey = EspcKey;
		lCaseData.LCaseKey = lCaseKey;
		lCaseData.LoadCaseName = EspcData.LoadCaseName + _T("(ES)");
		//lCaseData.LoadCaseType = _T("ES");
		lCaseData.LoadCaseType = _T("Seismic");
		lCaseData.type = e_SP20_Seismic;
		aLCaseK.Add(lCaseKey);
		m_mapLCase[lCaseKey.keymap] = lCaseData;
	}

	SortDgnLCase(aLCaseK);
}

int CLoadCombSP20Ctrl::Get_MaxLoadCombNameNo(int LcomType)
{
	int MaxNo = 0;
	CArray<CString, CString&> strLcomList;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetLcomList(LcomType, strLcomList);

	int Count = strLcomList.GetSize();
	for (int i = 0; i < Count; i++)
	{
		CString strName = strLcomList.GetAt(i);
		CString strPart[4] = { strName.Mid(1,3), strName.Mid(2,3), strName.Mid(1,4), strName.Mid(2,4) };
		int nLCBNameType = 0;    //1老 版快 cLCB/sLCB殿殿.. 2老 版快 scLCB/cfLCB殿殿..
		if (strPart[0] == _T("LCB"))
			nLCBNameType = 1;
		else if (strPart[1] == _T("LCB"))
			nLCBNameType = 2;
		else
			ASSERT(FALSE);

		if (nLCBNameType == 1 || nLCBNameType == 2)
		{
			int nLen = strName.GetLength();
			CString strNo = nLCBNameType == 1 ? strName.Right(nLen - 4) : strName.Right(nLen - 5);
			int nCom = _ttoi(strNo);
			if (strPart[2] == _T("LCB-"))   // CRC 捞抚 锭巩俊 眠啊
			{
				strNo = strName.Right(nLen - 7);
				nCom = _ttoi(strNo);
			}
			else if (strPart[3] == _T("LCB-"))
			{
				strNo = strName.Right(nLen - 8);
				nCom = _ttoi(strNo);
			}
			MaxNo = max(MaxNo, nCom);
		}
	}
	return MaxNo;
}

BOOL CLoadCombSP20Ctrl::GetLCaseFactor(IN const CArrayEx<T_KEY, T_KEY>& aInLCase, OUT CArrayEx<double, double>& aFactor)
{
	aFactor.RemoveAll();
	return TRUE;
}

BOOL LoadCombDB::ClassifyIndex(IN const CArrayEx<T_KEY, T_KEY>& aInLCase, OUT CArrayEx<T_KEY, T_KEY>(&aIndex)[6])
{
	for (int i = 0; i < 5; i++)
		aIndex[i].RemoveAll();
	T_SP20_RCFI_D SP20RcfiD;
	for (int i = 0; i < aInLCase.GetSize(); i++)
	{
		if (mapRcfi.Lookup(aInLCase[i], SP20RcfiD))
		{
			switch (SP20RcfiD.eLoadCaseType)
			{
			case e_SP20_Constant:
				aIndex[0].Add(SP20RcfiD.nIndex);
				break;
			case e_SP20_Long_term:
				aIndex[1].Add(SP20RcfiD.nIndex);
				break;
			case e_SP20_Short_term:
				aIndex[2].Add(SP20RcfiD.nIndex);
				break;
			case e_SP20_Seismic:
				aIndex[3].Add(SP20RcfiD.nIndex);
				break;
			case e_SP20_Crane_vertical:
			case e_SP20_Crane_brake:
				aIndex[4].Add(SP20RcfiD.nIndex);
				break;
			case e_SP20_Special:
				aIndex[5].Add(SP20RcfiD.nIndex);
				break;
			default:
				break;
			}
		}
	}
	return TRUE;
}
BOOL LoadCombDB::GenerateLComb(OUT CArray<T_LCOM_D, T_LCOM_D&>& aLcomD, IN const CArrayEx<T_KEY, T_KEY>(&aClassifyIndex)[6], IN int nTerm /* = 0 */)
{
	aLcomD.RemoveAll();
	LoadCombNode loadComb[6];
	eSP20_LCASE_TYPE pathType = e_SP20_None;
	if (!aClassifyIndex[5].IsEmpty())
		pathType = e_SP20_Special;
	if (!aClassifyIndex[3].IsEmpty())
		pathType = e_SP20_Seismic;
	int nServiceability = 0;
	if (nTerm == 2)
		nServiceability = 1;
	for (int m = 0; m < 6; m++)
	{
		loadComb[m].Init(aClassifyIndex[m], pathType, this, nServiceability);
	}

	std::function<void(int, T_LCOM_D&)> IterLoadComb = [&](int nIndex, T_LCOM_D& pLcomD)
	{
		if (nIndex >= 6)
		{
			aLcomD.Add(pLcomD);
			return;
		}
		T_LCOM_BASE LComBase;
		T_SP20_RCFI_D RcfiD;
		const LoadCombNode& node = loadComb[nIndex];
		if (node.m_aFactor.IsEmpty())
		{
			IterLoadComb(nIndex + 1, pLcomD);
			return;
		}
		ASSERT(node.m_aDescription.GetSize() == node.m_aFactor.GetSize());
		for (int i = 0; i < node.m_aFactor.GetSize(); i++)
		{
			ASSERT(node.m_aIndex.GetSize() == node.m_aFactor[i].GetSize());
			T_LCOM_D tLcomD = pLcomD;
			for (int k = 0; k < node.m_aFactor[i].GetSize(); k++)
			{
				if (!mapRcfi.Lookup(node.m_aIndex[k], RcfiD))
				{
					ASSERT(FALSE);
				}
				LComBase.AnalType = RcfiD.LCaseKey.key.iAnaType;
				LComBase.LoadCaseKey = RcfiD.LCaseKey.key.caseKey;
				LComBase.Factor = node.m_aFactor[i][k];
				if (nTerm == 1 && nIndex == 2)
					LComBase.Factor *= RcfiD.ddL;
				tLcomD.aCombination.Add(LComBase);
			}
			if (!node.m_aDescription.IsEmpty())
			{
				if (tLcomD.Description.IsEmpty() && nServiceability == 1)
					tLcomD.Description += _T("SERV :");
				if(!tLcomD.Description.IsEmpty() && node.m_type != e_SP20_Constant)
				{
					tLcomD.Description += CString(_T(" + (") + node.m_aDescription[i] + _T(")"));
				}
				else
					tLcomD.Description += node.m_aDescription[i];
			}
			IterLoadComb(nIndex + 1, tLcomD);
		}
	};
	T_LCOM_D tempLcomD;
	IterLoadComb(0, tempLcomD);
	return TRUE;
}
LoadCombNode::LoadCombNode()
{
	m_aDescription.RemoveAll();
}
void LoadCombNode::Init(const CArrayEx<T_KEY, T_KEY>& aIndex, eSP20_LCASE_TYPE eType, LoadCombDB* pDB, int nServiceability /* = 0 */)
{
	m_PathType = eType;
	m_aIndex.Copy(aIndex);
	m_pDB = pDB;
	m_type = e_SP20_None;
	m_nServiceability = nServiceability;
	m_aDescription.RemoveAll();
	GenerateFactor();
}
void LoadCombNode::GenerateFactor()
{
	int nLCaseCount = m_aIndex.GetSize();
	if (nLCaseCount == 0)
		return;
	m_type = m_pDB->mapRcfi[m_aIndex[0]].eLoadCaseType;
	if (m_type == e_SP20_Constant)
		GenerateConstantFactor();
	else
		GenerateTermFactor();
}

void LoadCombNode::GenerateConstantFactor()
{
	int nLCaseCount = m_aIndex.GetSize();
	T_SP20_LPSI_D LpsiD;
	T_SP20_RCFI_D RcfiD;
	T_SP20_LINK_D LinkD;
	RcfiD.Initialize();
	CMapEx<T_KEY, T_KEY, CArrayEx<double, double>, CArrayEx<double, double>&> mapTemp;
	for (int i = 0; i < nLCaseCount; i++)
	{
		m_pDB->mapLpsi.Lookup(m_aIndex[i], LpsiD);
		m_pDB->mapRcfi.Lookup(m_aIndex[i], RcfiD);
		m_pDB->mapLink.Lookup(m_aIndex[i], LinkD);
		if (m_nServiceability == 1)
		{
			RcfiD.dgF = 1.0;
			RcfiD.dgfa = 1.0;
			RcfiD.dgN = 1.0;
		}
		CArrayEx<double, double>& aTempFactor = mapTemp[m_aIndex[i]];
		if (m_PathType == e_SP20_Seismic)
		{
			aTempFactor.Add(LpsiD.dPsiSe * RcfiD.dgF);
			//gFa != gF
			if (fabs(RcfiD.dgF - RcfiD.dgfa) > 1.0e-7)
			{
				aTempFactor.Add(LpsiD.dPsiSe * RcfiD.dgfa);
			}
		}
		else if (m_PathType == e_SP20_Special)
		{
			//企划错误,需要跟rus确认gFo是哪个值
			aTempFactor.Add(LpsiD.dPsi1 * RcfiD.dgfa);
			//gFo != gF
			if (fabs(RcfiD.dgFo - RcfiD.dgfa) > 1.0e-7)
			{
				aTempFactor.Add(LpsiD.dPsiSe * RcfiD.dgFo);
			}
		}
		else
		{
			aTempFactor.Add(LpsiD.dPsi1 * RcfiD.dgF * RcfiD.dgN);
			//gFa != gF
			if (fabs(RcfiD.dgF - RcfiD.dgfa) > 1.0e-7)
			{
				aTempFactor.Add(LpsiD.dPsi1 * RcfiD.dgfa * RcfiD.dgN);
			}
		}
		//"Synchronicity"
		if (LinkD.bSynchronicity)
		{
			int nCount = aTempFactor.GetSize();
			for (int i = 0; i < nCount; i++)
			{
				aTempFactor.Add(aTempFactor[i] * -1);
			}
		}
	}
	CString strTemp = _T("");
	std::function<void(int, CArrayEx<double, double>&,CString)> IterFactor = [&](int nIndex, CArrayEx<double, double>& aRefFactor,CString strDescription)
	{
		if (nIndex < nLCaseCount)
		{
			T_KEY key = m_aIndex[nIndex];
			CArrayEx<double, double> aTempLFactor;
			const CArrayEx<double, double>& aTempFactor = mapTemp[key];
			for (int i = 0; i < aTempFactor.GetSize(); i++)
			{
				CString _strDes = strDescription;
				aTempLFactor.RemoveAll();
				aTempLFactor.Copy(aRefFactor);
				aTempLFactor.Add(aTempFactor[i]);
				m_pDB->mapLpsi.Lookup(key, LpsiD);
				if (nIndex == 0)
					strTemp.Format(_T("%.2g%s"), aTempFactor[i], LpsiD.LoadCaseName);
				else
					strTemp.Format(_T(" + %.2g%s"), aTempFactor[i], LpsiD.LoadCaseName);
				_strDes += strTemp;
				IterFactor(nIndex + 1, aTempLFactor, _strDes);
			}
			return;
		}
		m_aFactor.Add(aRefFactor);
		m_aDescription.Add(strDescription);
	};

	CArrayEx<double, double> aLFactor;
	aLFactor.RemoveAll();
	IterFactor(0, aLFactor, strTemp);
	m_type = RcfiD.eLoadCaseType;
}
void LoadCombNode::GenerateTermFactor()
{
	int nLCaseCount = m_aIndex.GetSize();
	T_SP20_LPSI_D LpsiD;
	T_SP20_RCFI_D RcfiD;
	T_SP20_LINK_D LinkD;
	RcfiD.Initialize();
	CMapEx<T_KEY, T_KEY, CArrayEx<double, double>, CArrayEx<double, double>&> mapTemp;
	for (int i = 0; i < nLCaseCount; i++)
	{
		m_pDB->mapLpsi.Lookup(m_aIndex[i], LpsiD);
		m_pDB->mapRcfi.Lookup(m_aIndex[i], RcfiD);
		m_pDB->mapLink.Lookup(m_aIndex[i], LinkD);
		CArrayEx<double, double>& aTempFactor = mapTemp[m_aIndex[i]];
		if (m_nServiceability == 1)
		{
			RcfiD.dgF = 1.0;
			RcfiD.dgfa = 1.0;
			RcfiD.dgN = 1.0;
		}
		if (m_PathType == e_SP20_Seismic)
		{
			if(m_type == e_SP20_Long_term || m_type == e_SP20_Short_term)
				aTempFactor.Add(LpsiD.dPsiSe * RcfiD.dgFo);
			else
				aTempFactor.Add(LpsiD.dPsiSe * RcfiD.dgF);
		}
		else if (m_PathType == e_SP20_Special)
		{
			if (m_type == e_SP20_Short_term)
			{
				aTempFactor.Add(LpsiD.dPsiO1* RcfiD.dgFo);
				aTempFactor.Add(LpsiD.dPsiO2 * RcfiD.dgFo);
				aTempFactor.Add(LpsiD.dPsiO3 * RcfiD.dgFo);
			}
			else if (m_type == e_SP20_Special)
			{
				aTempFactor.Add(LpsiD.dPsiO1 * RcfiD.dgFo);
			}
			else
			{
				if (m_type == e_SP20_Crane_brake || m_type == e_SP20_Crane_vertical)
				{
					if(nLCaseCount==1)
						aTempFactor.Add(LpsiD.dPsi1 * RcfiD.dgFo);
					else if (nLCaseCount == 1)
						aTempFactor.Add(LpsiD.dPsi2 * RcfiD.dgFo);
					else
						aTempFactor.Add(LpsiD.dPsi3 * RcfiD.dgFo);
				}
				else
				{
					aTempFactor.Add(LpsiD.dPsi1 * RcfiD.dgFo);
					aTempFactor.Add(LpsiD.dPsi2 * RcfiD.dgFo);
					aTempFactor.Add(LpsiD.dPsi3 * RcfiD.dgFo);
				}
			}
		}
		else
		{
			if (m_type == e_SP20_Crane_brake || m_type == e_SP20_Crane_vertical)
			{
				if (nLCaseCount == 1)
					aTempFactor.Add(LpsiD.dPsi1 * RcfiD.dgFo);
				else if (nLCaseCount == 1)
					aTempFactor.Add(LpsiD.dPsi2 * RcfiD.dgFo);
				else
					aTempFactor.Add(LpsiD.dPsi3 * RcfiD.dgFo);
			}
			else
			{
				aTempFactor.Add(LpsiD.dPsi1 * RcfiD.dgF * RcfiD.dgN);
				aTempFactor.Add(LpsiD.dPsi2 * RcfiD.dgF * RcfiD.dgN);
				aTempFactor.Add(LpsiD.dPsi3 * RcfiD.dgF * RcfiD.dgN);
			}
		}
	}
	bool bPsi3 = true;
	if (m_PathType == e_SP20_Seismic || (m_PathType == e_SP20_Special && m_type == e_SP20_Special))
		bPsi3 = false;
	else if ((m_type == e_SP20_Crane_brake || m_type == e_SP20_Crane_vertical) /*&& m_PathType != e_SP20_Seismic*/)
		bPsi3 = false;

	CString strTemp;
	CMap<T_KEY, T_KEY, int, int> mapSim;
	std::function<void(int, int, CArrayEx<double, double>&,CString)> Iter3Factor = [&](int nIndex, int flag, CArrayEx<double, double>& aRefFactor,CString strDes)
	{
		if (nIndex < nLCaseCount)
		{
			T_KEY key = m_aIndex[nIndex];
			const CArrayEx<double, double>& aTempFactor = mapTemp[key];
			CArrayEx<double, double> aTempLFactor;
			m_pDB->mapLpsi.Lookup(key, LpsiD);
			//check sim_gr
			int sim_gr = -1;
			for (int i = 0; i < nIndex; i++)
			{
				if (ContainsIndex(key, m_pDB->mapLink[m_aIndex[i]].aLinksCase[0]))
				{
					if (mapSim.Lookup(m_aIndex[i],sim_gr))
					{
						break;
					}
				}
			}
			if (sim_gr != -1)
			{
				aTempLFactor.RemoveAll();
				aTempLFactor.Copy(aRefFactor);
				aTempLFactor.Add(aTempFactor[sim_gr]);
				strTemp.Format(_T(" + %.2g%s"), aTempFactor[sim_gr], LpsiD.LoadCaseName);
				strDes += strTemp;
				Iter3Factor(nIndex + 1, flag, aTempLFactor,strDes);
				return;
			}
			//
			for (int i = 0; i < aTempFactor.GetSize(); i++)
			{
				int newFlag = flag;
				if ((newFlag & (1 << i)) != 0)
					continue;
				if (!(i == 2 && nLCaseCount - nIndex > 3))
					newFlag |= (1 << i);
				aTempLFactor.RemoveAll();
				aTempLFactor.Copy(aRefFactor);
				aTempLFactor.Add(aTempFactor[i]);
				mapSim[key] = i;
				if(strDes.IsEmpty())
					strTemp.Format(_T("%.2g%s"), aTempFactor[i], LpsiD.LoadCaseName);
				else
					strTemp.Format(_T("%s + %.2g%s"), strDes,aTempFactor[i], LpsiD.LoadCaseName);
				Iter3Factor(nIndex + 1, newFlag, aTempLFactor, strTemp);
			}
			return;
		}
		if (flag == 1 || flag == 3 || flag == 7)
		{
			m_aFactor.Add(aRefFactor);
			m_aDescription.Add(strDes);
		}
	};
	int flag = 0;
	CArrayEx<double, double> aLFactor;
	aLFactor.RemoveAll();
	if(bPsi3)
		Iter3Factor(0, flag, aLFactor,strTemp);
	else
	{
		CArrayEx<double, double> aFTemp;
		CString strDescription;
		
		for (int i = 0; i < m_aIndex.GetSize(); i++)
		{
			m_pDB->mapLpsi.Lookup(m_aIndex[i], LpsiD);
			double dFactor = mapTemp[m_aIndex[i]][0];
			aFTemp.Add(dFactor);
			strTemp.Format(_T("%.2g%s"), dFactor, LpsiD.LoadCaseName);
			if (i != 0)
				strDescription += _T(" + ");
			strDescription += strTemp;
		}
		m_aFactor.Add(aFTemp);
		m_aDescription.Add(strDescription);
	}
}
