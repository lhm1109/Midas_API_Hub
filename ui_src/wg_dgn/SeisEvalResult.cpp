#include "stdafx.h"
#include "SeisEvalResult.h"

#include "..\wg_base\ArrayUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\DB_ST_DT_ELEM.h"
#include "..\wg_db\MathFunc.h"
#include "..\wg_db\FileCtrl.h"


using namespace std;
#include "..\MIT_Lib\rptostream.h"

CSeisEvalResult::CSeisEvalResult(void)
{
	m_bPreEvalResult = FALSE;
	m_b1stEvalResult = FALSE;
	m_b2ndEvalResult = FALSE;

	InitResultMap();
}

CSeisEvalResult::~CSeisEvalResult(void)
{
	
}

void CSeisEvalResult::InitResultMap()
{
	m_mPreConColm.RemoveAll();
	m_mPreConColm.InitHashTable(HASHSIZEELEM);

	m_mPreConWall.RemoveAll();
	m_mPreConWall.InitHashTable(HASHSIZEELEM);

	m_mPreStlColm.RemoveAll();
	m_mPreStlColm.InitHashTable(HASHSIZEELEM);

	m_mPreStlBrac.RemoveAll();
	m_mPreStlBrac.InitHashTable(HASHSIZEELEM);

	m_mLinConBeam.RemoveAll();
	m_mLinConBeam.InitHashTable(HASHSIZEELEM);

	m_mLinConColm.RemoveAll();
	m_mLinConColm.InitHashTable(HASHSIZEELEM);

	m_mLinConWall.RemoveAll();
	m_mLinConWall.InitHashTable(HASHSIZEELEM);

	m_mLinStlBeam.RemoveAll();
	m_mLinStlBeam.InitHashTable(HASHSIZEELEM);

	m_mLinStlColm.RemoveAll();
	m_mLinStlColm.InitHashTable(HASHSIZEELEM);

	m_mLinStlBrac.RemoveAll();
	m_mLinStlBrac.InitHashTable(HASHSIZEELEM);

	m_mLinInfWall.RemoveAll();
	m_mLinInfWall.InitHashTable(HASHSIZEELEM);

	m_m1stConBeam.RemoveAll();
	m_m1stConBeam.InitHashTable(HASHSIZEELEM);

	m_m1stConColm.RemoveAll();
	m_m1stConColm.InitHashTable(HASHSIZEELEM);

	m_m1stConWall.RemoveAll();
	m_m1stConWall.InitHashTable(HASHSIZEELEM);

	m_m1stStlBeam.RemoveAll();
	m_m1stStlBeam.InitHashTable(HASHSIZEELEM);

	m_m1stStlColm.RemoveAll();
	m_m1stStlColm.InitHashTable(HASHSIZEELEM);

	m_m1stStlPanz.RemoveAll();
	m_m1stStlPanz.InitHashTable(HASHSIZEELEM);

	m_m1stStlBrac.RemoveAll();
	m_m1stStlBrac.InitHashTable(HASHSIZEELEM);

	m_m1stInfWall.RemoveAll();
	m_m1stInfWall.InitHashTable(HASHSIZEELEM);

	m_m2ndConBeam.RemoveAll();
	m_m2ndConBeam.InitHashTable(HASHSIZEELEM);

	m_m2ndConColm.RemoveAll();
	m_m2ndConColm.InitHashTable(HASHSIZEELEM);

	m_m2ndConWall.RemoveAll();
	m_m2ndConWall.InitHashTable(HASHSIZEELEM);

	m_m2ndStlBeam.RemoveAll();
	m_m2ndStlBeam.InitHashTable(HASHSIZEELEM);

	m_m2ndStlColm.RemoveAll();
	m_m2ndStlColm.InitHashTable(HASHSIZEELEM);

	m_m2ndStlBrac.RemoveAll();
	m_m2ndStlBrac.InitHashTable(HASHSIZEELEM);

	m_m2ndInfWall.RemoveAll();
	m_m2ndInfWall.InitHashTable(HASHSIZEELEM);
}

// void CSeisEvalResult::DeleteAllResult()
// {
// 	DeletePreResult();
// 	Delete1stResult();
// 	Delete2ndResult();
// }
// 
// void CSeisEvalResult::DeletePreResult()
// {
// 	m_mPreConColm.RemoveAll();
// 	m_mPreConWall.RemoveAll();
// 	m_mPreStlColm.RemoveAll();
// 	m_mPreStlBrac.RemoveAll();
// 
// 	m_mLinConBeam.RemoveAll();
// 	m_mLinConColm.RemoveAll();
// 	m_mLinConWall.RemoveAll();
// 	m_mLinStlBeam.RemoveAll();
// 	m_mLinStlColm.RemoveAll();
// 	m_mLinStlBrac.RemoveAll();
// 	m_mLinInfWall.RemoveAll();
// 
// 	SetPreRslt(FALSE);
// }
// 
// void CSeisEvalResult::Delete1stResult()
// {
// 	m_m1stConBeam.RemoveAll();
// 	m_m1stConColm.RemoveAll();
// 	m_m1stConWall.RemoveAll();
// 	m_m1stStlBeam.RemoveAll();
// 	m_m1stStlColm.RemoveAll();
// 	m_m1stStlPanz.RemoveAll();
// 	m_m1stStlBrac.RemoveAll();
// 	m_m1stInfWall.RemoveAll();
// 
// 	Set1stRslt(FALSE);
// }
// 
// void CSeisEvalResult::Delete2ndResult()
// {
// 	m_m2ndConBeam.RemoveAll();
// 	m_m2ndConColm.RemoveAll();
// 	m_m2ndConWall.RemoveAll();
// 	m_m2ndStlBeam.RemoveAll();
// 	m_m2ndStlColm.RemoveAll();
// 	m_m2ndStlBrac.RemoveAll();
// 	m_m2ndInfWall.RemoveAll();
// 
// 	Set2ndRslt(FALSE);
// }
// 
// void CSeisEvalResult::SetPreConColmRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVALPRE_COLM, T_EVALPRE_COLM&> &mapSrc)
// {
// 	m_mPreConColm.RemoveAll();
// 	T_ELEM_K Key = 0;
// 	T_EVALPRE_COLM Data;
// 	CopyMap(mapSrc, m_mPreConColm, Key, Data);
// }
// 
// void CSeisEvalResult::SetPreConWallRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVALPRE_WALL, T_EVALPRE_WALL&> &mapSrc)
// {
// 	m_mPreConWall.RemoveAll();
// 	T_ELEM_K Key = 0;
// 	T_EVALPRE_WALL Data;
// 	CopyMap(mapSrc, m_mPreConWall, Key, Data);
// }
// 
// void CSeisEvalResult::SetPreStlColmRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVALPRE_STLCOLM, T_EVALPRE_STLCOLM&> &mapSrc)
// {
// 	m_mPreStlColm.RemoveAll();
// 	T_ELEM_K Key = 0;
// 	T_EVALPRE_STLCOLM Data;
// 	CopyMap(mapSrc, m_mPreStlColm, Key, Data);
// }
// 
// void CSeisEvalResult::SetPreStlBracRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVALPRE_STLBRAC, T_EVALPRE_STLBRAC&> &mapSrc)
// {
// 	m_mPreStlBrac.RemoveAll();
// 	T_ELEM_K Key = 0;
// 	T_EVALPRE_STLBRAC Data;
// 	CopyMap(mapSrc, m_mPreStlBrac, Key, Data);
// }
// 
// void CSeisEvalResult::SetLinConBeamRslt(const CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_BEAM, T_EVALLIN_BEAM&> &mapSrc)
// {
// 	m_mLinConBeam.RemoveAll();
// 	SE_ELEM_TYPE_KEY Key = 0;
// 	T_EVALLIN_BEAM Data;
// 	CopyMap(mapSrc, m_mLinConBeam, Key, Data);
// }
// 
// void CSeisEvalResult::SetLinConColmRslt(const CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_COLM, T_EVALLIN_COLM&> &mapSrc)
// {
// 	m_mLinConColm.RemoveAll();
// 	SE_ELEM_TYPE_KEY Key = 0;
// 	T_EVALLIN_COLM Data;
// 	CopyMap(mapSrc, m_mLinConColm, Key, Data);
// }
// 
// void CSeisEvalResult::SetLinConWallRslt(const CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_WALL, T_EVALLIN_WALL&> &mapSrc)
// {
// 	m_mLinConWall.RemoveAll();
// 	SE_ELEM_TYPE_KEY Key = 0;
// 	T_EVALLIN_WALL Data;
// 	CopyMap(mapSrc, m_mLinConWall, Key, Data);
// }
// 
// void CSeisEvalResult::SetLinStlBeamRslt(const CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_STLBEAM, T_EVALLIN_STLBEAM&> &mapSrc)
// {
// 	m_mLinStlBeam.RemoveAll();
// 	SE_ELEM_TYPE_KEY Key = 0;
// 	T_EVALLIN_STLBEAM Data;
// 	CopyMap(mapSrc, m_mLinStlBeam, Key, Data);
// }
// 
// void CSeisEvalResult::SetLinStlColmRslt(const CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_STLCOLM, T_EVALLIN_STLCOLM&> &mapSrc)
// {
// 	m_mLinStlColm.RemoveAll();
// 	SE_ELEM_TYPE_KEY Key = 0;
// 	T_EVALLIN_STLCOLM Data;
// 	CopyMap(mapSrc, m_mLinStlColm, Key, Data);
// }
// 
// void CSeisEvalResult::SetLinStlBracRslt(const CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_STLBRAC, T_EVALLIN_STLBRAC&> &mapSrc)
// {
// 	m_mLinStlBrac.RemoveAll();
// 	SE_ELEM_TYPE_KEY Key = 0;
// 	T_EVALLIN_STLCOLM Data;
// 	CopyMap(mapSrc, m_mLinStlBrac, Key, Data);
// }
// 
// void CSeisEvalResult::SetLinInfWallRslt(const CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_INFILL, T_EVALLIN_INFILL&> &mapSrc)
// {
// 	m_mLinInfWall.RemoveAll();
// 	SE_ELEM_TYPE_KEY Key = 0;
// 	T_EVALLIN_INFILL Data;
// 	CopyMap(mapSrc, m_mLinInfWall, Key, Data);
// }
// 
// void CSeisEvalResult::SetInfWallOutRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL_INFILL_OUT, T_EVAL_INFILL_OUT&> &mapSrc)
// {
//     m_mInfWallOut.RemoveAll();
//     T_ELEM_K Key = 0;
//     T_EVAL_INFILL_OUT Data;
//     CopyMap(mapSrc, m_mInfWallOut, Key, Data);
// }
// 
// void CSeisEvalResult::Set1stConBeamRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_BEAM, T_EVAL1ST_BEAM&> &mapSrc)
// {
// 	m_m1stConBeam.RemoveAll();
// 	T_ELEM_K Key = 0;
// 	T_EVAL1ST_BEAM Data;
// 	CopyMap(mapSrc, m_m1stConBeam, Key, Data);
// }
// 
// void CSeisEvalResult::Set1stConColmRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_COLM, T_EVAL1ST_COLM&> &mapSrc)
// {
// 	m_m1stConColm.RemoveAll();
// 	T_ELEM_K Key = 0;
// 	T_EVAL1ST_COLM Data;
// 	CopyMap(mapSrc, m_m1stConColm, Key, Data);
// }
// 
// void CSeisEvalResult::Set1stConWallRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_WALL, T_EVAL1ST_WALL&> &mapSrc)
// {
// 	m_m1stConWall.RemoveAll();
// 	T_ELEM_K Key = 0;
// 	T_EVAL1ST_WALL Data;
// 	CopyMap(mapSrc, m_m1stConWall, Key, Data);
// }
// 
// void CSeisEvalResult::Set1stStlBeamRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLBEAM, T_EVAL1ST_STLBEAM&> &mapSrc)
// {
// 	m_m1stStlBeam.RemoveAll();
// 	T_ELEM_K Key = 0;
// 	T_EVAL1ST_STLBEAM Data;
// 	CopyMap(mapSrc, m_m1stStlBeam, Key, Data);
// }
// 
// void CSeisEvalResult::Set1stStlColmRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLCOLM, T_EVAL1ST_STLCOLM&> &mapSrc)
// {
// 	m_m1stStlColm.RemoveAll();
// 	T_ELEM_K Key = 0;
// 	T_EVAL1ST_STLCOLM Data;
// 	CopyMap(mapSrc, m_m1stStlColm, Key, Data);
// }
// 
// void CSeisEvalResult::Set1stStlPanzRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLPANZ, T_EVAL1ST_STLPANZ&> &mapSrc)
// {
// 	m_m1stStlPanz.RemoveAll();
// 	T_ELEM_K Key = 0;
// 	T_EVAL1ST_STLPANZ Data;
// 	CopyMap(mapSrc, m_m1stStlPanz, Key, Data);
// }
// 
// void CSeisEvalResult::Set1stStlBracRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLBRAC, T_EVAL1ST_STLBRAC&> &mapSrc)
// {
// 	m_m1stStlBrac.RemoveAll();
// 	T_ELEM_K Key = 0;
// 	T_EVAL1ST_STLBRAC Data;
// 	CopyMap(mapSrc, m_m1stStlBrac, Key, Data);
// }
// 
// void CSeisEvalResult::Set1stInfWallRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_INFILL, T_EVAL1ST_INFILL&> &mapSrc)
// {
// 	m_m1stInfWall.RemoveAll();
// 	T_ELEM_K Key = 0;
// 	T_EVAL1ST_INFILL Data;
// 	CopyMap(mapSrc, m_m1stInfWall, Key, Data);
// }
// 
// void CSeisEvalResult::Set2ndConBeamRslt(const CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_BEAM_ALL, T_EVAL2ND_BEAM_ALL&> &mapSrc)
// {
// 	m_m2ndConBeam.RemoveAll();
// 	SE_ELEM_LOAD_KEY Key = 0;
// 	T_EVAL2ND_BEAM_ALL Data;
// 	CopyMap(mapSrc, m_m2ndConBeam, Key, Data);
// }
// 
// void CSeisEvalResult::Set2ndConColmRslt(const CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_COLM_ALL, T_EVAL2ND_COLM_ALL&> &mapSrc)
// {
// 	m_m2ndConColm.RemoveAll();
// 	SE_ELEM_LOAD_KEY Key = 0;
// 	T_EVAL2ND_COLM_ALL Data;
// 	CopyMap(mapSrc, m_m2ndConColm, Key, Data);
// }
// 
// void CSeisEvalResult::Set2ndConWallRslt(const CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_WALL_ALL, T_EVAL2ND_WALL_ALL&> &mapSrc)
// {
// 	m_m2ndConWall.RemoveAll();
// 	SE_ELEM_LOAD_KEY Key = 0;
// 	T_EVAL2ND_WALL_ALL Data;
// 	CopyMap(mapSrc, m_m2ndConWall, Key, Data);
// }
// 
// void CSeisEvalResult::Set2ndStlBeamRslt(const CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_STLBEAM_ALL, T_EVAL2ND_STLBEAM_ALL&> &mapSrc)
// {
// 	m_m2ndStlBeam.RemoveAll();
// 	SE_ELEM_LOAD_KEY Key = 0;
// 	T_EVAL2ND_STLBEAM_ALL Data;
// 	CopyMap(mapSrc, m_m2ndStlBeam, Key, Data);
// }
// 
// void CSeisEvalResult::Set2ndStlColmRslt(const CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_STLCOLM_ALL, T_EVAL2ND_STLCOLM_ALL&> &mapSrc)
// {
// 	m_m2ndStlColm.RemoveAll();
// 	SE_ELEM_LOAD_KEY Key = 0;
// 	T_EVAL2ND_STLCOLM_ALL Data;
// 	CopyMap(mapSrc, m_m2ndStlColm, Key, Data);
// }
// 
// void CSeisEvalResult::Set2ndStlBracRslt(const CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_STLBRAC_ALL, T_EVAL2ND_STLBRAC_ALL&> &mapSrc)
// {
// 	m_m2ndStlBrac.RemoveAll();
// 	SE_ELEM_LOAD_KEY Key = 0;
// 	T_EVAL2ND_STLBRAC_ALL Data;
// 	CopyMap(mapSrc, m_m2ndStlBrac, Key, Data);
// }
// 
// void CSeisEvalResult::Set2ndInfWallRslt(const CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_INFILL_ALL, T_EVAL2ND_INFILL_ALL&> &mapSrc)
// {
// 	m_m2ndInfWall.RemoveAll();
// 	SE_ELEM_LOAD_KEY Key = 0;
// 	T_EVAL2ND_INFILL_ALL Data;
// 	CopyMap(mapSrc, m_m2ndInfWall, Key, Data);
// }
// 
// BOOL CSeisEvalResult::GetPreConColmRslt(T_ELEM_K ElemK, OUT T_EVALPRE_COLM& rEvalPre)
// {
// 	rEvalPre.Initialize();
// 	if (m_mPreConColm.GetCount() < 1)            return FALSE;
// 	if (!m_mPreConColm.Lookup(ElemK, rEvalPre))  return FALSE;
// 	
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSEPreConColmIn(rEvalPre);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSEPreConColmOut(rEvalPre);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::GetPreConWallRslt(T_ELEM_K ElemK, OUT T_EVALPRE_WALL& rEvalPre)
// {
// 	rEvalPre.Initialize();
// 	if (m_mPreConWall.GetCount() < 1)            return FALSE;
// 	if (!m_mPreConWall.Lookup(ElemK, rEvalPre))  return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSEPreConWallIn(rEvalPre);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSEPreConWallOut(rEvalPre);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::GetPreStlColmRslt(T_ELEM_K ElemK, OUT T_EVALPRE_STLCOLM& rEvalPre)
// {
// 	rEvalPre.Initialize();
// 	if (m_mPreStlColm.GetCount() < 1)            return FALSE;
// 	if (!m_mPreStlColm.Lookup(ElemK, rEvalPre))  return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSEPreSTLColmIn(rEvalPre);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSEPreSTLColmOut(rEvalPre);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::GetPreStlBracRslt(T_ELEM_K ElemK, OUT T_EVALPRE_STLBRAC& rEvalPre)
// {
// 	rEvalPre.Initialize();
// 	if (m_mPreStlBrac.GetCount() < 1)            return FALSE;
// 	if (!m_mPreStlBrac.Lookup(ElemK, rEvalPre))  return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSEPreSTLBracIn(rEvalPre);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSEPreSTLBracOut(rEvalPre);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::GetLinConBeamRslt(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_BEAM& rEvalLin)
// {
// 	rEvalLin.Initialize();
// 	SE_ELEM_TYPE_K ElemTypeK;
// 	ElemTypeK.Key.ElemK = ElemK;
// 	ElemTypeK.Key.TypeK = unType;
// 	if (m_mLinConBeam.GetCount() < 1)                       return FALSE;
// 	if (!m_mLinConBeam.Lookup(ElemTypeK.KeyMap, rEvalLin))  return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSELinConBeamIn(rEvalLin);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSELinConBeamOut(rEvalLin);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::GetLinConColmRslt(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_COLM& rEvalLin)
// {
// 	rEvalLin.Initialize();
// 	SE_ELEM_TYPE_K ElemTypeK;
// 	ElemTypeK.Key.ElemK = ElemK;
// 	ElemTypeK.Key.TypeK = unType;
// 	if (m_mLinConColm.GetCount() < 1)                       return FALSE;
// 	if (!m_mLinConColm.Lookup(ElemTypeK.KeyMap, rEvalLin))  return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSELinConColmIn(rEvalLin);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSELinConColmOut(rEvalLin);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::GetLinConWallRslt(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_WALL& rEvalLin)
// {
// 	rEvalLin.Initialize();
// 	SE_ELEM_TYPE_K ElemTypeK;
// 	ElemTypeK.Key.ElemK = ElemK;
// 	ElemTypeK.Key.TypeK = unType;
// 	if (m_mLinConWall.GetCount() < 1)                       return FALSE;
// 	if (!m_mLinConWall.Lookup(ElemTypeK.KeyMap, rEvalLin))  return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSELinConWallIn(rEvalLin);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSELinConWallOut(rEvalLin);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::GetLinStlBeamRslt(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLBEAM& rEvalLin)
// {
// 	rEvalLin.Initialize();
// 	SE_ELEM_TYPE_K ElemTypeK;
// 	ElemTypeK.Key.ElemK = ElemK;
// 	ElemTypeK.Key.TypeK = unType;
// 	if (m_mLinStlBeam.GetCount() < 1)                       return FALSE;
// 	if (!m_mLinStlBeam.Lookup(ElemTypeK.KeyMap, rEvalLin))  return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSELinStlBeamIn(rEvalLin);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSELinStlBeamOut(rEvalLin);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::GetLinStlColmRslt(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLCOLM& rEvalLin)
// {
// 	rEvalLin.Initialize();
// 	SE_ELEM_TYPE_K ElemTypeK;
// 	ElemTypeK.Key.ElemK = ElemK;
// 	ElemTypeK.Key.TypeK = unType;
// 	if (m_mLinStlColm.GetCount() < 1)                       return FALSE;
// 	if (!m_mLinStlColm.Lookup(ElemTypeK.KeyMap, rEvalLin))  return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSELinStlColmIn(rEvalLin);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSELinStlColmOut(rEvalLin);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::GetLinStlBracRslt(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLBRAC& rEvalLin)
// {
// 	rEvalLin.Initialize();
// 	SE_ELEM_TYPE_K ElemTypeK;
// 	ElemTypeK.Key.ElemK = ElemK;
// 	ElemTypeK.Key.TypeK = unType;
// 	if (m_mLinStlBrac.GetCount() < 1)                       return FALSE;
// 	if (!m_mLinStlBrac.Lookup(ElemTypeK.KeyMap, rEvalLin))  return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSELinStlBracIn(rEvalLin);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSELinStlBracOut(rEvalLin);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::GetLinInfWallRslt(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_INFILL& rEvalLin)
// {
// 	rEvalLin.Initialize();
// 	SE_ELEM_TYPE_K ElemTypeK;
// 	ElemTypeK.Key.ElemK = ElemK;
// 	ElemTypeK.Key.TypeK = unType;
// 	if (m_mLinInfWall.GetCount() < 1)                       return FALSE;
// 	if (!m_mLinInfWall.Lookup(ElemTypeK.KeyMap, rEvalLin))  return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSELinInfWallIn(rEvalLin);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSELinInfWallOut(rEvalLin);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::GetInfWallOutRslt(T_ELEM_K ElemK, OUT T_EVAL_INFILL_OUT& rEval)
// {
// 	rEval.Initialize();
// 	
// 	if (m_mInfWallOut.GetCount() < 1) return FALSE;
// 	if (!m_mInfWallOut.Lookup(ElemK, rEval)) return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSEInfWallOutIn(rEval);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSEInfWallOutOut(rEval);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::Get1stConBeamRslt(T_ELEM_K ElemK, OUT T_EVAL1ST_BEAM& rEval1st)
// {
// 	rEval1st.Initialize();
// 	if (m_b1stEvalResult==FALSE)                 return FALSE;
// 	if (m_m1stConBeam.GetCount() < 1)            return FALSE;
// 	if (!m_m1stConBeam.Lookup(ElemK, rEval1st))  return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSE1stConBeamIn(rEval1st);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSE1stConBeamOut(rEval1st);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::Get1stConColmRslt(T_ELEM_K ElemK, OUT T_EVAL1ST_COLM& rEval1st)
// {
// 	rEval1st.Initialize();
// 	if (m_b1stEvalResult==FALSE)                 return FALSE;
// 	if (m_m1stConColm.GetCount() < 1)            return FALSE;
// 	if (!m_m1stConColm.Lookup(ElemK, rEval1st))  return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSE1stConColmIn(rEval1st);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSE1stConColmOut(rEval1st);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::Get1stConWallRslt(T_ELEM_K ElemK, OUT T_EVAL1ST_WALL& rEval1st)
// {
// 	rEval1st.Initialize();
// 	if (m_b1stEvalResult==FALSE)                 return FALSE;
// 	if (m_m1stConWall.GetCount() < 1)            return FALSE;
// 	if (!m_m1stConWall.Lookup(ElemK, rEval1st))  return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSE1stConWallIn(rEval1st);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSE1stConWallOut(rEval1st);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::Get1stStlBeamRslt(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBEAM& rEval1st)
// {
// 	rEval1st.Initialize();
// 	if (m_b1stEvalResult==FALSE)                 return FALSE;
// 	if (m_m1stStlBeam.GetCount() < 1)            return FALSE;
// 	if (!m_m1stStlBeam.Lookup(ElemK, rEval1st))  return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSE1stStlBeamIn(rEval1st);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSE1stStlBeamOut(rEval1st);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::Get1stStlColmRslt(T_ELEM_K ElemK, OUT T_EVAL1ST_STLCOLM& rEval1st)
// {
// 	rEval1st.Initialize();
// 	if (m_b1stEvalResult==FALSE)                 return FALSE;
// 	if (m_m1stStlColm.GetCount() < 1)            return FALSE;
// 	if (!m_m1stStlColm.Lookup(ElemK, rEval1st))  return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSE1stStlColmIn(rEval1st);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSE1stStlColmOut(rEval1st);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::Get1stStlPanzRslt(T_ELEM_K ElemK, OUT T_EVAL1ST_STLPANZ& rEval1st)
// {
// 	rEval1st.Initialize();
// 	if (m_b1stEvalResult==FALSE)                 return FALSE;
// 	if (m_m1stStlPanz.GetCount() < 1)            return FALSE;
// 	if (!m_m1stStlPanz.Lookup(ElemK, rEval1st))  return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSE1stStlPanzIn(rEval1st);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSE1stStlPanzOut(rEval1st);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::Get1stStlBracRslt(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBRAC& rEval1st)
// {
// 	rEval1st.Initialize();
// 	if (m_b1stEvalResult==FALSE)                 return FALSE;
// 	if (m_m1stStlBrac.GetCount() < 1)            return FALSE;
// 	if (!m_m1stStlBrac.Lookup(ElemK, rEval1st))  return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSE1stStlBracIn(rEval1st);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSE1stStlBracOut(rEval1st);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::Get1stInfWallRslt(T_ELEM_K ElemK, OUT T_EVAL1ST_INFILL& rEval1st)
// {
// 	rEval1st.Initialize();
// 	if (m_b1stEvalResult==FALSE)                 return FALSE;
// 	if (m_m1stInfWall.GetCount() < 1)            return FALSE;
// 	if (!m_m1stInfWall.Lookup(ElemK, rEval1st))  return FALSE;
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSE1stInfWallIn(rEval1st);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSE1stInfWallOut(rEval1st);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::Get2ndConBeamRslt(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_BEAM& rEval2nd)
// {
// 	rEval2nd.Initialize();
// 	SE_ELEM_LOAD_K ElemLoadK;
// 	ElemLoadK.Key.ElemK = ElemK;
// 	ElemLoadK.Key.PolcK = PolcK;
// 	T_EVAL2ND_BEAM_ALL Eval2ndAll;
// 	if (m_b2ndEvalResult==FALSE)                              return FALSE;
// 	if (m_m2ndConBeam.GetCount() < 1)                         return FALSE;
// 	if (!m_m2ndConBeam.Lookup(ElemLoadK.KeyMap, Eval2ndAll))  return FALSE;
// 
// 	int nStepSize = Eval2ndAll.aSteps.GetSize();
// 	if (nStep > nStepSize) { ASSERT(0); return FALSE; }
// 	rEval2nd = Eval2ndAll.aSteps[nStep];
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSE2ndConBeamIn(rEval2nd);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSE2ndConBeamOut(rEval2nd);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::Get2ndConColmRslt(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_COLM& rEval2nd)
// {
// 	rEval2nd.Initialize();
// 	SE_ELEM_LOAD_K ElemLoadK;
// 	ElemLoadK.Key.ElemK = ElemK;
// 	ElemLoadK.Key.PolcK = PolcK;
// 	T_EVAL2ND_COLM_ALL Eval2ndAll;
// 	if (m_b2ndEvalResult==FALSE)                              return FALSE;
// 	if (m_m2ndConColm.GetCount() < 1)                         return FALSE;
// 	if (!m_m2ndConColm.Lookup(ElemLoadK.KeyMap, Eval2ndAll))  return FALSE;
// 
// 	int nStepSize = Eval2ndAll.aSteps.GetSize();
// 	if (nStep > nStepSize) { ASSERT(0); return FALSE; }
// 	rEval2nd = Eval2ndAll.aSteps[nStep];
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSE2ndConColmIn(rEval2nd);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSE2ndConColmOut(rEval2nd);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::Get2ndConWallRslt(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_WALL& rEval2nd)
// {
// 	rEval2nd.Initialize();
// 	SE_ELEM_LOAD_K ElemLoadK;
// 	ElemLoadK.Key.ElemK = ElemK;
// 	ElemLoadK.Key.PolcK = PolcK;
// 	T_EVAL2ND_WALL_ALL Eval2ndAll;
// 	if (m_b2ndEvalResult==FALSE)                              return FALSE;
// 	if (m_m2ndConWall.GetCount() < 1)                         return FALSE;
// 	if (!m_m2ndConWall.Lookup(ElemLoadK.KeyMap, Eval2ndAll))  return FALSE;
// 
// 	int nStepSize = Eval2ndAll.aSteps.GetSize();
// 	if (nStep > nStepSize) { ASSERT(0); return FALSE; }
// 	rEval2nd = Eval2ndAll.aSteps[nStep];
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSE2ndConWallIn(rEval2nd);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSE2ndConWallOut(rEval2nd);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::Get2ndStlBeamRslt(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLBEAM& rEval2nd)
// {
// 	rEval2nd.Initialize();
// 	SE_ELEM_LOAD_K ElemLoadK;
// 	ElemLoadK.Key.ElemK = ElemK;
// 	ElemLoadK.Key.PolcK = PolcK;
// 	T_EVAL2ND_STLBEAM_ALL Eval2ndAll;
// 	if (m_b2ndEvalResult==FALSE)                              return FALSE;
// 	if (m_m2ndStlBeam.GetCount() < 1)                         return FALSE;
// 	if (!m_m2ndStlBeam.Lookup(ElemLoadK.KeyMap, Eval2ndAll))  return FALSE;
// 
// 	int nStepSize = Eval2ndAll.aSteps.GetSize();
// 	if (nStep > nStepSize) { ASSERT(0); return FALSE; }
// 	rEval2nd = Eval2ndAll.aSteps[nStep];
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSE2ndStlBeamIn(rEval2nd);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSE2ndStlBeamOut(rEval2nd);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::Get2ndStlColmRslt(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLCOLM& rEval2nd)
// {
// 	rEval2nd.Initialize();
// 	SE_ELEM_LOAD_K ElemLoadK;
// 	ElemLoadK.Key.ElemK = ElemK;
// 	ElemLoadK.Key.PolcK = PolcK;
// 	T_EVAL2ND_STLCOLM_ALL Eval2ndAll;
// 	if (m_b2ndEvalResult==FALSE)                              return FALSE;
// 	if (m_m2ndStlColm.GetCount() < 1)                         return FALSE;
// 	if (!m_m2ndStlColm.Lookup(ElemLoadK.KeyMap, Eval2ndAll))  return FALSE;
// 
// 	int nStepSize = Eval2ndAll.aSteps.GetSize();
// 	if (nStep > nStepSize) { ASSERT(0); return FALSE; }
// 	rEval2nd = Eval2ndAll.aSteps[nStep];
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSE2ndStlColmIn(rEval2nd);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSE2ndStlColmOut(rEval2nd);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::Get2ndStlBracRslt(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLBRAC& rEval2nd)
// {
// 	rEval2nd.Initialize();
// 	SE_ELEM_LOAD_K ElemLoadK;
// 	ElemLoadK.Key.ElemK = ElemK;
// 	ElemLoadK.Key.PolcK = PolcK;
// 	T_EVAL2ND_STLBRAC_ALL Eval2ndAll;
// 	if (m_b2ndEvalResult==FALSE)                              return FALSE;
// 	if (m_m2ndStlBrac.GetCount() < 1)                         return FALSE;
// 	if (!m_m2ndStlBrac.Lookup(ElemLoadK.KeyMap, Eval2ndAll))  return FALSE;
// 
// 	int nStepSize = Eval2ndAll.aSteps.GetSize();
// 	if (nStep > nStepSize) { ASSERT(0); return FALSE; }
// 	rEval2nd = Eval2ndAll.aSteps[nStep];
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSE2ndStlBracIn(rEval2nd);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSE2ndStlBracOut(rEval2nd);
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::Get2ndInfWallRslt(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_INFILL& rEval2nd)
// {
// 	rEval2nd.Initialize();
// 	SE_ELEM_LOAD_K ElemLoadK;
// 	ElemLoadK.Key.ElemK = ElemK;
// 	ElemLoadK.Key.PolcK = PolcK;
// 	T_EVAL2ND_INFILL_ALL Eval2ndAll;
// 	if (m_b2ndEvalResult==FALSE)                              return FALSE;
// 	if (m_m2ndInfWall.GetCount() < 1)                         return FALSE;
// 	if (!m_m2ndInfWall.Lookup(ElemLoadK.KeyMap, Eval2ndAll))  return FALSE;
// 
// 	int nStepSize = Eval2ndAll.aSteps.GetSize();
// 	if (nStep > nStepSize) { ASSERT(0); return FALSE; }
// 	rEval2nd = Eval2ndAll.aSteps[nStep];
// 
// 	// Current단위계로 변환////////////////////////////////////////////////////////	
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CCurUnitSaver UnitSave(TRUE);
// 	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
// 	pDoc->m_pUnitCtrl->ConvertUnitSE2ndInfWallIn(rEval2nd);
// 	UnitSave.Restore();
// 	pDoc->m_pUnitCtrl->ConvertUnitSE2ndInfWallOut(rEval2nd);
// 
// 	return TRUE;
// }
// 
// 
// BOOL CSeisEvalResult::PrintSeisEvalLinRsltInfWall()
// {
// 	if (m_bPreEvalResult==FALSE)      return FALSE;
// 	T_ELEM_K_LIST aInfWallK;
// 	int nSizeInfWall = m_mLinInfWall.GetSortedKeyListforUintK(aInfWallK);
// 	if (nSizeInfWall < 1)
// 	{
// 		AfxMessageBox(_STR(_T("Not exist masonry wall result to evaluate.")));
// 		return FALSE;
// 	}
// 
// 	CWaitCursor WCursor;
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CString strPathName = pDoc->GetPathName();
// 
// 	CFileCtrl FileCtrl(strPathName);
// 	CString strOutFile = _T("");	
// 	strOutFile.Format(_T("%s_InfillLin.out"), FileCtrl.GetFilePathAndNameWithoutExtension());
// 	rptwofstream fout(strOutFile, ios::trunc);
// 
// 	T_UNIT_INDEX CurrUnit;
// 	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
// 	T_UNIT_NAMEFACT CurUnitName;
// 	pDoc->m_pUnitCtrl->GetUnitNameCurrent(CurUnitName);
// 
// 	fout<<_T("========================================================================================================================================")<<endl
// 		<<_T("     MASONRY INFILL SEISMIC EVALUATION RESULT (LINEAR ANALSYS EVALUATION STEP)")<<endl
// 		<<_T("    [UNIT] = ")<<CurUnitName.strForce[CurrUnit.nBase_Force]<<_T(", ")<<CurUnitName.strLength[CurrUnit.nBase_Length]<<endl
// 		<<_T("========================================================================================================================================")<<endl
// 		<<_T("  STOR   ELEM   LCOM PART           Pu            Vbj          Vdc            Vbm            Vn         PhiPn       CDR")<<endl
// 		<<_T("----------------------------------------------------------------------------------------------------------------------------------------")<<endl;
// 
// 	auto l_PrintPos = [&](rptwofstream& out, const T_EVALLIN_INFILL_CAPA& capa, const T_EVALLIN_INFILL_POS& pos)
// 	{		
// 		out<<GetValueForm(14, 3, pos.dPu)
// 			<<GetValueForm(14, 3, capa.dVbjs)
// 			<<GetValueForm(14, 3, capa.dVdc)
// 			<<GetValueForm(14, 3, capa.dVbm)
// 			<<GetValueForm(14, 3, capa.dVn)
// 			<<GetValueForm(14, 3, capa.dPhiPn)
// 			<<GetValueForm(10, 3, pos.dCDR)
// 			<<endl;
// 	};
// 
// 	T_STOR_K StorK;
// 	T_STOR_D StorD;
// 	T_LCOM_D LcomD;
// 	CString strElem = _T("");
// 	CString strStor = _T("");
// 	CString strLcom = _T("");
// 	CString strLcomType = _T("");
// 
// 	T_EVALLIN_INFILL EvalInfi;
// 	for (int i=0; i<nSizeInfWall; ++i)
// 	{
// 		T_ELEM_K ElemK = aInfWallK[i];
// 		if (!GetLinInfWallRslt(ElemK, EN_EVAL_SEIS, EvalInfi)) { ASSERT(FALSE); continue; }
// 
// 		pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
// 		if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { ASSERT(FALSE); continue; }
// 
// 		int nSizeLcom = EvalInfi.aEvalLcom.GetSize();
// 		for (int m=0; m<nSizeLcom; ++m)
// 		{
// 			const T_EVALLIN_INFILL_LCOM & EvalColmLcom = EvalInfi.aEvalLcom[m];			
// 
// 			if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, EvalColmLcom.LcomK, LcomD)) { continue; } 
// 			if (LcomD.nActive == 0) { continue; }
// 
// 			strElem.Format(_T("%6d"), ElemK);
// 			strStor.Format(_T("%6s"), StorD.StoryName);
// 			strLcom.Format(_T("%6s"), LcomD.LoadCombName);
// 
// 			fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   I");
// 			l_PrintPos(fout, EvalColmLcom.Capa, EvalColmLcom.EndI);
// 
// 			fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   J");
// 			l_PrintPos(fout, EvalColmLcom.Capa, EvalColmLcom.EndJ);				
// 		}		
// 		fout<<_T("----------------------------------------------------------------------------------------------------------------------------------------")<<endl;
// 	}
// 
// 	fout.close();
// 
// 	CDBLib::RunTextEditor(strOutFile, _STR(_T("MASONRY Infill Seismic Evaluate - LINEAR ANALYSIS STEP")), TRUE); 
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::PrintSeisEval1stRsltConBeam()
// {
// 	if (m_b1stEvalResult==FALSE)      return FALSE;
// 	T_ELEM_K_LIST aBeamK;
// 	int nSizeBeam = m_m1stConBeam.GetSortedKeyListforUintK(aBeamK);
// 	if (nSizeBeam < 1)
// 	{
// 		AfxMessageBox(_STR(_T("Not exist RC beam result to evaluate.")));
// 		return FALSE;
// 	}
// 
// 	CWaitCursor WCursor;
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CString strPathName = pDoc->GetPathName();
// 
// 	CFileCtrl FileCtrl(strPathName);
// 	CString strOutFile = _T("");	
// 	strOutFile.Format(_T("%s_Beam1st.out"), FileCtrl.GetFilePathAndNameWithoutExtension());
// 	rptwofstream fout(strOutFile, ios::trunc);
// 
// 	T_UNIT_INDEX CurrUnit;
// 	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
// 	T_UNIT_NAMEFACT CurUnitName;
// 	pDoc->m_pUnitCtrl->GetUnitNameCurrent(CurUnitName);
// 
// 	fout<<_T("=============================================================================================================================================================================================================================================================================")<<endl
// 			<<_T("     RC BEAM SEISMIC EVALUATION RESULT (1ST STEP)")<<endl
// 			<<_T("    [UNIT] = ")<<CurUnitName.strForce[CurrUnit.nBase_Force]<<_T(", ")<<CurUnitName.strLength[CurrUnit.nBase_Length]<<endl
// 			<<_T("=============================================================================================================================================================================================================================================================================")<<endl
// 			<<_T("  STOR   ELEM   LCOM PART           MuD           VuD           MuF           VuF           MeP           MeN            Vn JUDGE SC      Rebar      Shear       m-IO       m-LS       m-CP    MDCR-IO    MDCR-LS    MDCR-CP    VDCR-IO    VDCR-LS    VDCR-CP  M-PL  V-PL  PL")<<endl
// 			<<_T("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
// 
// 	auto l_GetCtrlType = [](EN_EVALCTRL enCtrl)
// 	{
// 		return enCtrl==EN_EVALCTRL_FLEX ? _T(" FLEX") : _T("SHEAR");
// 	};
// 
// 	auto l_GetSeisConform = [](BOOL bSeisConform)
// 	{
// 		return bSeisConform ? _T(" C") : _T("NC");
// 	};
// 
// 	auto l_PrintPos = [&](rptwofstream& out, const T_EVAL1ST_BEAM_POS& pos)
// 	{		
// 		out<<GetValueForm(14, 3, pos.dMUD)
// 			 <<GetValueForm(14, 3, pos.dVUD)
// 			 <<GetValueForm(14, 3, pos.dMUF)
// 			 <<GetValueForm(14, 3, pos.dVUF)
// 			 <<GetValueForm(14, 3, pos.dMeP)
// 			 <<GetValueForm(14, 3, pos.dMeN)
// 			 <<GetValueForm(14, 3, pos.dVn)
// 			 <<_T(" ")<<l_GetCtrlType(pos.enCtrlType)<<_T(" ")<<l_GetSeisConform(pos.bSeisConform)<<_T(" ")<<GetValueForm(10, 3, pos.dRebarIndex)<<_T(" ")<<GetValueForm(10, 3, pos.dShearIndex)
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_CP])			
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_CP])
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_CP])
// 			 <<_T("    ")<<GetPerformLevel4TextOut(pos.DCRM.enLevel)<<_T("    ")<<GetPerformLevel4TextOut(pos.DCRV.enLevel)<<_T("  ")<<GetPerformLevel4TextOut(pos.enLevel)
// 			 <<endl;
// 	};
// 
// 	T_STOR_K StorK;
// 	T_STOR_D StorD;
// 	T_LCOM_D LcomD;
// 	CString strElem = _T("");
// 	CString strStor = _T("");
// 	CString strLcom = _T("");
// 
// 	T_EVAL1ST_BEAM EvalBeam;
// 	for (int i=0; i<nSizeBeam; ++i)
// 	{
// 		T_ELEM_K ElemK = aBeamK[i];
// 		if (!Get1stConBeamRslt(ElemK, EvalBeam)) { ASSERT(FALSE); continue; }
// 
// 		pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
// 		if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { ASSERT(FALSE); continue; }
// 
// 		int nSizeLcom = EvalBeam.aEvalLcom.GetSize();
// 		for (int m=0; m<nSizeLcom; ++m)
// 		{
// 			const T_EVAL1ST_BEAM_LCOM & EvalBeamLcom = EvalBeam.aEvalLcom[m];			
// 
// 			if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, EvalBeamLcom.LcomK, LcomD)) { continue; } 
// 			if (LcomD.nActive == 0) { continue; }
// 
// 			strElem.Format(_T("%6d"), ElemK);
// 			strStor.Format(_T("%6s"), StorD.StoryName);
// 			strLcom.Format(_T("%6s"), LcomD.LoadCombName);
// 
// 			fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   I");
// 			l_PrintPos(fout, EvalBeamLcom.EndI);
// 
// 			fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   J");
// 			l_PrintPos(fout, EvalBeamLcom.EndJ);				
// 		}		
// 		fout<<_T("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
// 	}
// 
// 	fout.close();
// 
// 	CDBLib::RunTextEditor(strOutFile, _STR(_T("RC Beam Seismic Evaluate - 1ST STEP")), TRUE); 
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::PrintSeisEval1stRsltConColm()
// {
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	T_SEME_D SemeD;
// 	if (!pDoc->m_pAttrCtrl2->GetSeme(SemeD)) { SemeD.Initialize(); }
// 	switch (SemeD.nMethod)
// 	{
// 	case D_SEME_METHOD_KISTEC2013: return PrintSeisEval1stRsltConColm_KISTEC2013(); break;
// 	case D_SEME_METHOD_MOE2018:    return PrintSeisEval1stRsltConColm_MOE2018();    break;
// 	case D_SEME_METHOD_MOE2019:    return PrintSeisEval1stRsltConColm_MOE2018();    break;
// 	default: ASSERT(FALSE);	break;
// 	}
// 	return FALSE;
// }
// 
// BOOL CSeisEvalResult::PrintSeisEval1stRsltConColm_KISTEC2013()
// {
// 	if (m_b1stEvalResult==FALSE)      return FALSE;
// 	T_ELEM_K_LIST aColmK;
// 	int nSizeColm = m_m1stConColm.GetSortedKeyListforUintK(aColmK);
// 	if (nSizeColm < 1)
// 	{
// 		AfxMessageBox(_STR(_T("Not exist RC column result to evaluate.")));
// 		return FALSE;
// 	}
// 
// 	CWaitCursor WCursor;
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CString strPathName = pDoc->GetPathName();
// 
// 	CFileCtrl FileCtrl(strPathName);
// 	CString strOutFile = _T("");	
// 	strOutFile.Format(_T("%s_Colm1st.out"), FileCtrl.GetFilePathAndNameWithoutExtension());
// 	rptwofstream fout(strOutFile, ios::trunc);
// 
// 	T_UNIT_INDEX CurrUnit;
// 	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
// 	T_UNIT_NAMEFACT CurUnitName;
// 	pDoc->m_pUnitCtrl->GetUnitNameCurrent(CurUnitName);
// 
// 	fout<<_T("==================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================")<<endl
// 			<<_T("     RC COLUMN SEISMIC EVALUATION RESULT (1ST STEP)")<<endl
// 			<<_T("    [UNIT] = ")<<CurUnitName.strForce[CurrUnit.nBase_Force]<<_T(", ")<<CurUnitName.strLength[CurrUnit.nBase_Length]<<endl
// 			<<_T("==================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================")<<endl
// 			<<_T("  STOR   ELEM   LCOM PART           PuD          MuyD          MuzD          VuyD          VuzD           PuF          MuyF          MuzF          VuyF          VuzF           PnC           PnT           Mey           Mez           Vny           Vnz JUDGE SC     AXIALy     SHEARy      m-IOy      m-LSy      m-CPy JUDGE SC     AXIALz     SHEARz      m-IOz      m-LSz      m-CPz    PDCR-IO    PDCR-LS    PDCR-CP    MDCR-IO    MDCR-LS    MDCR-CP    VDCR-IO    VDCR-LS    VDCR-CP  P-PL  M-PL  V-PL  PL")<<endl
// 			<<_T("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
// 
// 	auto l_GetCtrlType = [](EN_EVALCTRL enCtrl)
// 	{
// 		return enCtrl==EN_EVALCTRL_FLEX ? _T(" FLEX") : _T("SHEAR");
// 	};
// 
// 	auto l_GetSeisConform = [](BOOL bSeisConform)
// 	{
// 		return bSeisConform ? _T(" C") : _T("NC");
// 	};
// 
// 	auto l_PrintPos = [&](rptwofstream& out, const T_EVAL1ST_COLM_POS& pos)
// 	{		
// 		out<<GetValueForm(14, 3, pos.dPUD)
// 			 <<GetValueForm(14, 3, pos.dMUDy)
// 			 <<GetValueForm(14, 3, pos.dMUDz)
// 			 <<GetValueForm(14, 3, pos.dVUDy)
// 			 <<GetValueForm(14, 3, pos.dVUDz)
// 			 <<GetValueForm(14, 3, pos.dPUF)
// 			 <<GetValueForm(14, 3, pos.dMUFy)
// 			 <<GetValueForm(14, 3, pos.dMUFz)
// 			 <<GetValueForm(14, 3, pos.dVUFy)
// 			 <<GetValueForm(14, 3, pos.dVUFz)
// 			 <<GetValueForm(14, 3, pos.dPnC)
// 			 <<GetValueForm(14, 3, pos.dPnT)
// 			 <<GetValueForm(14, 3, pos.dMey)
// 			 <<GetValueForm(14, 3, pos.dMez)
// 			 <<GetValueForm(14, 3, pos.dVny)
// 			 <<GetValueForm(14, 3, pos.dVnz)
// 			 <<_T(" ")<<l_GetCtrlType(pos.Diry.enCtrlType)<<_T(" ")<<l_GetSeisConform(pos.Diry.bSeisConform)<<_T(" ")<<GetValueForm(10, 3, pos.dAxialIndex)<<_T(" ")<<GetValueForm(10, 3, pos.Diry.dShearIndex)
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.Diry.dmFac[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.Diry.dmFac[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.Diry.dmFac[EN_LEVEL_CP])			
// 			 <<_T(" ")<<l_GetCtrlType(pos.Dirz.enCtrlType)<<_T(" ")<<l_GetSeisConform(pos.Dirz.bSeisConform)<<_T(" ")<<GetValueForm(10, 3, pos.dAxialIndex)<<_T(" ")<<GetValueForm(10, 3, pos.Dirz.dShearIndex)
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.Dirz.dmFac[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.Dirz.dmFac[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.Dirz.dmFac[EN_LEVEL_CP])
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.DCRP.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRP.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRP.dDCR[EN_LEVEL_CP])
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_CP])
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_CP])
// 			 <<_T("    ")<<GetPerformLevel4TextOut(pos.DCRP.enLevel)<<_T("    ")<<GetPerformLevel4TextOut(pos.DCRM.enLevel)<<_T("    ")<<GetPerformLevel4TextOut(pos.DCRV.enLevel)<<_T("  ")<<GetPerformLevel4TextOut(pos.enLevel)
// 			 <<endl;
// 	};
// 
// 	T_STOR_K StorK;
// 	T_STOR_D StorD;
// 	T_LCOM_D LcomD;
// 	CString strElem = _T("");
// 	CString strStor = _T("");
// 	CString strLcom = _T("");
// 
// 	T_EVAL1ST_COLM EvalColm;
// 	for (int i=0; i<nSizeColm; ++i)
// 	{
// 		T_ELEM_K ElemK = aColmK[i];
// 		if (!Get1stConColmRslt(ElemK, EvalColm)) { ASSERT(FALSE); continue; }
// 
// 		pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
// 		if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { ASSERT(FALSE); continue; }
// 
// 		int nSizeLcom = EvalColm.aEvalLcom.GetSize();
// 		for (int m=0; m<nSizeLcom; ++m)
// 		{
// 			const T_EVAL1ST_COLM_LCOM & EvalColmLcom = EvalColm.aEvalLcom[m];			
// 
// 			if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, EvalColmLcom.LcomK, LcomD)) { continue; } 
// 			if (LcomD.nActive == 0) { continue; }
// 
// 			strElem.Format(_T("%6d"), ElemK);
// 			strStor.Format(_T("%6s"), StorD.StoryName);
// 			strLcom.Format(_T("%6s"), LcomD.LoadCombName);
// 
// 			fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   I");
// 			l_PrintPos(fout, EvalColmLcom.PosI);
// 
// 			fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   J");
// 			l_PrintPos(fout, EvalColmLcom.PosJ);				
// 		}		
// 		fout<<_T("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
// 	}
// 
// 	fout.close();
// 
// 	CDBLib::RunTextEditor(strOutFile, _STR(_T("RC Column Seismic Evaluate - 1ST STEP")), TRUE); 
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::PrintSeisEval1stRsltConColm_MOE2018()
// {
// 	if (m_b1stEvalResult==FALSE)      return FALSE;
// 	T_ELEM_K_LIST aColmK;
// 	int nSizeColm = m_m1stConColm.GetSortedKeyListforUintK(aColmK);
// 	if (nSizeColm < 1)
// 	{
// 		AfxMessageBox(_STR(_T("Not exist RC column result to evaluate.")));
// 		return FALSE;
// 	}
// 
// 	CWaitCursor WCursor;
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CString strPathName = pDoc->GetPathName();
// 
// 	CFileCtrl FileCtrl(strPathName);
// 	CString strOutFile = _T("");	
// 	strOutFile.Format(_T("%s_Colm1st.out"), FileCtrl.GetFilePathAndNameWithoutExtension());
// 	rptwofstream fout(strOutFile, ios::trunc);
// 
// 	T_UNIT_INDEX CurrUnit;
// 	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
// 	T_UNIT_NAMEFACT CurUnitName;
// 	pDoc->m_pUnitCtrl->GetUnitNameCurrent(CurUnitName);
// 
// 	fout<<_T("==========================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================")<<endl
// 			<<_T("     RC COLUMN SEISMIC EVALUATION RESULT (1ST STEP) by MOE2018")<<endl
// 			<<_T("    [UNIT] = ")<<CurUnitName.strForce[CurrUnit.nBase_Force]<<_T(", ")<<CurUnitName.strLength[CurrUnit.nBase_Length]<<endl
// 			<<_T("============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================")<<endl
// 			<<_T("  STOR   ELEM   LCOM PART           PuD          MuyD          MuzD          VuyD          VuzD           PuF          MuyF          MuzF          VuyF          VuzF           PnC           PnT           Mey           Mez           Vny           Vnz      AXIAL GROUPy      Rhovy     SHEARy GROUPz      Rhovz     SHEARz      m-IOy      m-LSy      m-CPy      m-IOz      m-LSz      m-CPz    PDCR-IO    PDCR-LS    PDCR-CP    MDCR-IO    MDCR-LS    MDCR-CP    VDCR-IO    VDCR-LS    VDCR-CP  P-PL  M-PL  V-PL  PL")<<endl
// 			<<_T("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
// 
// 	auto l_GetGroup = [](UINT unGroup)
// 	{
// 		CString strGroup = _T("     -");
// 		switch (unGroup)
// 		{
// 		case EN_CONCOLM_GROUP1: strGroup = _T("Group1"); break;
// 		case EN_CONCOLM_GROUP2: strGroup = _T("Group2"); break;
// 		case EN_CONCOLM_GROUP3: strGroup = _T("Group3"); break;
// 		default: ASSERT(FALSE); break;
// 		}
// 		return strGroup;
// 	};
// 
// 	auto l_PrintPos = [&](rptwofstream& out, const T_EVAL1ST_COLM_POS& pos)
// 	{		
// 		out<<GetValueForm(14, 3, pos.dPUD)
// 			 <<GetValueForm(14, 3, pos.dMUDy)
// 			 <<GetValueForm(14, 3, pos.dMUDz)
// 			 <<GetValueForm(14, 3, pos.dVUDy)
// 			 <<GetValueForm(14, 3, pos.dVUDz)
// 			 <<GetValueForm(14, 3, pos.dPUF)
// 			 <<GetValueForm(14, 3, pos.dMUFy)
// 			 <<GetValueForm(14, 3, pos.dMUFz)
// 			 <<GetValueForm(14, 3, pos.dVUFy)
// 			 <<GetValueForm(14, 3, pos.dVUFz)
// 			 <<GetValueForm(14, 3, pos.dPnC)
// 			 <<GetValueForm(14, 3, pos.dPnT)
// 			 <<GetValueForm(14, 3, pos.dMey)
// 			 <<GetValueForm(14, 3, pos.dMez)
// 			 <<GetValueForm(14, 3, pos.dVny)
// 			 <<GetValueForm(14, 3, pos.dVnz)
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.dAxialIndex)
// 			 <<_T(" ")<<l_GetGroup(pos.Diry.unGroup)<<_T(" ")<<GetValueForm(10, 3, pos.Diry.dRhoIndex)<<_T(" ")<<GetValueForm(10, 3, pos.Diry.dShearIndex)
// 			 <<_T(" ")<<l_GetGroup(pos.Dirz.unGroup)<<_T(" ")<<GetValueForm(10, 3, pos.Dirz.dRhoIndex)<<_T(" ")<<GetValueForm(10, 3, pos.Dirz.dShearIndex)
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.Diry.dmFac[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.Diry.dmFac[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.Diry.dmFac[EN_LEVEL_CP])			
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.Dirz.dmFac[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.Dirz.dmFac[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.Dirz.dmFac[EN_LEVEL_CP])
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.DCRP.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRP.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRP.dDCR[EN_LEVEL_CP])
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_CP])
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_CP])
// 			 <<_T("    ")<<GetPerformLevel4TextOut(pos.DCRP.enLevel)<<_T("    ")<<GetPerformLevel4TextOut(pos.DCRM.enLevel)<<_T("    ")<<GetPerformLevel4TextOut(pos.DCRV.enLevel)<<_T("  ")<<GetPerformLevel4TextOut(pos.enLevel)
// 			 <<endl;
// 	};
// 
// 	T_STOR_K StorK;
// 	T_STOR_D StorD;
// 	T_LCOM_D LcomD;
// 	CString strElem = _T("");
// 	CString strStor = _T("");
// 	CString strLcom = _T("");
// 
// 	T_EVAL1ST_COLM EvalColm;
// 	for (int i=0; i<nSizeColm; ++i)
// 	{
// 		T_ELEM_K ElemK = aColmK[i];
// 		if (!Get1stConColmRslt(ElemK, EvalColm)) { ASSERT(FALSE); continue; }
// 
// 		pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
// 		if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { ASSERT(FALSE); continue; }
// 
// 		int nSizeLcom = EvalColm.aEvalLcom.GetSize();
// 		for (int m=0; m<nSizeLcom; ++m)
// 		{
// 			const T_EVAL1ST_COLM_LCOM & EvalColmLcom = EvalColm.aEvalLcom[m];			
// 
// 			if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, EvalColmLcom.LcomK, LcomD)) { continue; } 
// 			if (LcomD.nActive == 0) { continue; }
// 
// 			strElem.Format(_T("%6d"), ElemK);
// 			strStor.Format(_T("%6s"), StorD.StoryName);
// 			strLcom.Format(_T("%6s"), LcomD.LoadCombName);
// 
// 			fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   I");
// 			l_PrintPos(fout, EvalColmLcom.PosI);
// 
// 			fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   J");
// 			l_PrintPos(fout, EvalColmLcom.PosJ);				
// 		}		
// 		fout<<_T("---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
// 	}
// 
// 	fout.close();
// 
// 	CDBLib::RunTextEditor(strOutFile, _STR(_T("RC Column Seismic Evaluate - 1ST STEP")), TRUE); 
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::PrintSeisEval1stRsltConWall()
// {
// 	if (m_b1stEvalResult==FALSE)      return FALSE;
// 	T_ELEM_K_LIST aWallK;
// 	int nSizeWall = m_m1stConWall.GetSortedKeyListforUintK(aWallK);
// 	if (nSizeWall < 1)
// 	{
// 		AfxMessageBox(_STR(_T("Not exist wall result to evaluate.")));
// 		return FALSE;
// 	}
// 
// 	CWaitCursor WCursor;
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CString strPathName = pDoc->GetPathName();
// 
// 	CFileCtrl FileCtrl(strPathName);
// 	CString strOutFile = _T("");	
// 	strOutFile.Format(_T("%s_Wall1st.out"), FileCtrl.GetFilePathAndNameWithoutExtension());
// 	rptwofstream fout(strOutFile, ios::trunc);
// 
// 	T_UNIT_INDEX CurrUnit;
// 	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
// 	T_UNIT_NAMEFACT CurUnitName;
// 	pDoc->m_pUnitCtrl->GetUnitNameCurrent(CurUnitName);
// 
// 	fout<<_T("===========================================================================================================================================================================================================================================================================================")<<endl
// 			<<_T("     RC WALL SEISMIC EVALUATION RESULT (1ST STEP)")<<endl
// 			<<_T("    [UNIT] = ")<<CurUnitName.strForce[CurrUnit.nBase_Force]<<_T(", ")<<CurUnitName.strLength[CurrUnit.nBase_Length]<<endl
// 			<<_T("===========================================================================================================================================================================================================================================================================================")<<endl
// 			<<_T("  STOR WALLID   LCOM PART           PuD           MuD           VuD           PuF           MuF           VuF            Me            Vn  JUDGE      AXIAL      SHEAR       m-IO       m-LS       m-CP    MDCR-IO    MDCR-LS    MDCR-CP    VDCR-IO    VDCR-LS    VDCR-CP  M-PL  V-PL    PL")<<endl
// 			<<_T("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
// 
// 	auto l_PrintPos = [&](rptwofstream& out, const T_EVAL1ST_WALL_POS& pos)
// 	{
// 		const CString& strCtrlType = pos.enCtrlType==EN_EVALCTRL_FLEX ? _T(" FLEX") : _T("SHEAR");
// 		out<<GetValueForm(14, 3, pos.dPUD)
// 			 <<GetValueForm(14, 3, pos.dMUD)
// 			 <<GetValueForm(14, 3, pos.dVUD)
// 			 <<GetValueForm(14, 3, pos.dPUF)
// 			 <<GetValueForm(14, 3, pos.dMUF)
// 			 <<GetValueForm(14, 3, pos.dVUF)
// 			 <<GetValueForm(14, 3, pos.dMe)
// 			 <<GetValueForm(14, 3, pos.dVn)
// 			 <<_T("  ")<<strCtrlType
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.dAxialIndex)
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.dShearIndex)
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_CP])
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_CP])
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_CP])
// 			 <<_T("    ")<<GetPerformLevel4TextOut(pos.DCRM.enLevel)<<_T("    ")<<GetPerformLevel4TextOut(pos.DCRV.enLevel)<<_T("    ")<<GetPerformLevel4TextOut(pos.enLevel)
// 			 <<endl;
// 	};
// 
// 	T_STOR_D StorD;
// 	T_LCOM_D LcomD;
// 	CString strWallID = _T("");
// 	CString strStor   = _T("");
// 	CString strLcom   = _T("");
// 
// 	T_EVAL1ST_WALL EvalWall;		
// 	for (int i=0; i<nSizeWall; ++i)
// 	{
// 		T_ELEM_K ElemK = aWallK[i];
// 		if (!Get1stConWallRslt(ElemK, EvalWall)) { ASSERT(FALSE); continue; }
// 
// 		T_WALL_K WallK;
// 		WallK.keymap = EvalWall.unWallKey;
// 
// 		int nSizeLcom = EvalWall.aEvalLcom.GetSize();
// 		for (int m=0; m<nSizeLcom; ++m)
// 		{
// 			const T_EVAL1ST_WALL_LCOM & EvalWallLcom = EvalWall.aEvalLcom[m];			
// 
// 			if (!pDoc->m_pAttrCtrl->GetStor(WallK.key.storid, StorD)) { ASSERT(FALSE); continue; }
// 			if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, EvalWallLcom.LcomK, LcomD)) { continue; } 
// 			if (LcomD.nActive == 0) { continue; }
// 
// 			strWallID.Format(_T("%6d"), WallK.key.wallid);
// 			strStor.Format(_T("%6s"), StorD.StoryName);
// 			strLcom.Format(_T("%6s"), LcomD.LoadCombName);
// 
// 			fout<<strStor<<_T(" ")<<strWallID<<_T(" ")<<strLcom<<_T(" ")<<_T("   I");
// 			l_PrintPos(fout, EvalWallLcom.PosI);
// 
// 			fout<<strStor<<_T(" ")<<strWallID<<_T(" ")<<strLcom<<_T(" ")<<_T("   J");
// 			l_PrintPos(fout, EvalWallLcom.PosJ);				
// 		}		
// 		fout<<_T("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
// 	}
// 
// 	fout.close();
// 
// 	CDBLib::RunTextEditor(strOutFile, _STR(_T("RC Wall Seismic Evaluate - 1ST STEP")), TRUE); 
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::PrintSeisEval1stRsltStlBeam()
// {
// 	if (m_b1stEvalResult==FALSE)      return FALSE;
// 	T_ELEM_K_LIST aBeamK;
// 	int nSizeBeam = m_m1stStlBeam.GetSortedKeyListforUintK(aBeamK);
// 	if (nSizeBeam < 1)
// 	{
// 		AfxMessageBox(_STR(_T("Not exist steel beam result to evaluate.")));
// 		return FALSE;
// 	}
// 
// 	CWaitCursor WCursor;
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CString strPathName = pDoc->GetPathName();
// 
// 	CFileCtrl FileCtrl(strPathName);
// 	CString strOutFile = _T("");	
// 	strOutFile.Format(_T("%s_StlBeam1st.out"), FileCtrl.GetFilePathAndNameWithoutExtension());
// 	rptwofstream fout(strOutFile, ios::trunc);
// 
// 	T_UNIT_INDEX CurrUnit;
// 	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
// 	T_UNIT_NAMEFACT CurUnitName;
// 	pDoc->m_pUnitCtrl->GetUnitNameCurrent(CurUnitName);
// 
// 	fout<<_T("============================================================================================================================")<<endl
// 			<<_T("     STL BEAM SEISMIC EVALUATION RESULT (1ST STEP)")<<endl
// 			<<_T("    [UNIT] = ")<<CurUnitName.strForce[CurrUnit.nBase_Force]<<_T(", ")<<CurUnitName.strLength[CurrUnit.nBase_Length]<<endl
// 			<<_T("============================================================================================================================")<<endl
// 			<<_T("  STOR   ELEM   LCOM PART           MuD            Me        m-IO       m-LS       m-CP     DCR-IO     DCR-LS     DCR-CP  PL")<<endl
// 			<<_T("----------------------------------------------------------------------------------------------------------------------------")<<endl;
// 
// 
// 	auto l_PrintPos = [&](rptwofstream& out, const T_EVAL1ST_STLBEAM_POS& pos)
// 	{		
// 		out<<GetValueForm(14, 3, pos.dMu)
// 			 <<GetValueForm(14, 3, pos.dMe)			
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_CP])			
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_CP])
// 			 <<_T("  ")<<GetPerformLevel4TextOut(pos.enLevel)
// 			 <<endl;
// 	};
// 
// 	T_STOR_K StorK;
// 	T_STOR_D StorD;
// 	T_LCOM_D LcomD;
// 	CString strElem = _T("");
// 	CString strStor = _T("");
// 	CString strLcom = _T("");
// 	CString strLcomType = _T("");
// 
// 	T_EVAL1ST_STLBEAM EvalBeam;
// 	for (int i=0; i<nSizeBeam; ++i)
// 	{
// 		T_ELEM_K ElemK = aBeamK[i];
// 		if (!Get1stStlBeamRslt(ElemK, EvalBeam)) { ASSERT(FALSE); continue; }
// 
// 		pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
// 		if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { ASSERT(FALSE); continue; }
// 
// 		int nSizeLcom = EvalBeam.aEvalLcom.GetSize();
// 		for (int m=0; m<nSizeLcom; ++m)
// 		{
// 			const T_EVAL1ST_STLBEAM_LCOM & EvalBeamLcom = EvalBeam.aEvalLcom[m];			
// 
// 			if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, EvalBeamLcom.LcomK, LcomD)) { continue; } 
// 			if (LcomD.nActive == 0) { continue; }
// 
// 			strElem.Format(_T("%6d"), ElemK);
// 			strStor.Format(_T("%6s"), StorD.StoryName);
// 			strLcom.Format(_T("%6s"), LcomD.LoadCombName);
// 
// 			fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<strLcomType<<_T(" ")<<_T("   I");
// 			l_PrintPos(fout, EvalBeamLcom.EndI);
// 
// 			fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<strLcomType<<_T(" ")<<_T("   J");
// 			l_PrintPos(fout, EvalBeamLcom.EndJ);				
// 		}		
// 		fout<<_T("----------------------------------------------------------------------------------------------------------------------------")<<endl;
// 	}
// 
// 	fout.close();
// 
// 	CDBLib::RunTextEditor(strOutFile, _STR(_T("STL Beam Seismic Evaluate - 1ST STEP")), TRUE); 
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::PrintSeisEval1stRsltStlColm()
// {
// 	if (m_b1stEvalResult==FALSE)      return FALSE;
// 	T_ELEM_K_LIST aColmK;
// 	int nSizeColm = m_m1stStlColm.GetSortedKeyListforUintK(aColmK);
// 	if (nSizeColm < 1)
// 	{
// 		AfxMessageBox(_STR(_T("Not exist steel column result to evaluate.")));
// 		return FALSE;
// 	}
// 
// 	CWaitCursor WCursor;
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CString strPathName = pDoc->GetPathName();
// 
// 	CFileCtrl FileCtrl(strPathName);
// 	CString strOutFile = _T("");	
// 	strOutFile.Format(_T("%s_StlColm1st.out"), FileCtrl.GetFilePathAndNameWithoutExtension());
// 	rptwofstream fout(strOutFile, ios::trunc);
// 
// 	T_UNIT_INDEX CurrUnit;
// 	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
// 	T_UNIT_NAMEFACT CurUnitName;
// 	pDoc->m_pUnitCtrl->GetUnitNameCurrent(CurUnitName);
// 
// 	fout<<_T("============================================================================================================================================================================================================================================================================")<<endl
// 			<<_T("     STL COLUMN SEISMIC EVALUATION RESULT (1ST STEP)")<<endl
// 			<<_T("    [UNIT] = ")<<CurUnitName.strForce[CurrUnit.nBase_Force]<<_T(", ")<<CurUnitName.strLength[CurrUnit.nBase_Length]<<endl
// 			<<_T("============================================================================================================================================================================================================================================================================")<<endl
// 			<<_T("  STOR   ELEM   LCOM PART           PuD          MuyD          MuzD           PuF          MuyF          MuzF           PCL          MCLy          MCLz           TCE          MCEy          MCEz      m-IOt      m-LSt      m-CPt      m-IOy      m-LSy      m-CPy      m-IOz      m-LSz      m-CPz     DCR-IO     DCR-LS     DCR-CP  PL")<<endl
// 			<<_T("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
// 
// 	auto l_PrintPos = [&](rptwofstream& out, const T_EVAL1ST_STLCOLM_POS& pos)
// 	{		
// 		out<<GetValueForm(14, 3, pos.dPUD)
// 			 <<GetValueForm(14, 3, pos.dMUDy)
// 			 <<GetValueForm(14, 3, pos.dMUDz)
// 			 <<GetValueForm(14, 3, pos.dPUF)
// 			 <<GetValueForm(14, 3, pos.dMUFy)
// 			 <<GetValueForm(14, 3, pos.dMUFz)
// 			 <<GetValueForm(14, 3, pos.dPCL)
// 			 <<GetValueForm(14, 3, pos.dMCLy)
// 			 <<GetValueForm(14, 3, pos.dMCLz)
// 			 <<GetValueForm(14, 3, pos.dTCE)
// 			 <<GetValueForm(14, 3, pos.dMCEy)
// 			 <<GetValueForm(14, 3, pos.dMCEz)
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.dmFacT[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.dmFacT[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.dmFacT[EN_LEVEL_CP])						
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.dmFacy[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.dmFacy[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.dmFacy[EN_LEVEL_CP])						
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.dmFacz[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.dmFacz[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.dmFacz[EN_LEVEL_CP])						
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_CP])
// 			 <<_T("  ")<<GetPerformLevel4TextOut(pos.DCR.enLevel)
// 			 <<endl;
// 	};
// 
// 	T_STOR_K StorK;
// 	T_STOR_D StorD;
// 	T_LCOM_D LcomD;
// 	CString strElem = _T("");
// 	CString strStor = _T("");
// 	CString strLcom = _T("");
// 	CString strLcomType = _T("");
// 
// 	T_EVAL1ST_STLCOLM EvalColm;
// 	for (int i=0; i<nSizeColm; ++i)
// 	{
// 		T_ELEM_K ElemK = aColmK[i];
// 		if (!Get1stStlColmRslt(ElemK, EvalColm)) { ASSERT(FALSE); continue; }
// 
// 		pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
// 		if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { ASSERT(FALSE); continue; }
// 
// 		int nSizeLcom = EvalColm.aEvalLcom.GetSize();
// 		for (int m=0; m<nSizeLcom; ++m)
// 		{
// 			const T_EVAL1ST_STLCOLM_LCOM & EvalColmLcom = EvalColm.aEvalLcom[m];			
// 
// 			if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, EvalColmLcom.LcomK, LcomD)) { continue; } 
// 			if (LcomD.nActive == 0) { continue; }
// 
// 			strElem.Format(_T("%6d"), ElemK);
// 			strStor.Format(_T("%6s"), StorD.StoryName);
// 			strLcom.Format(_T("%6s"), LcomD.LoadCombName);
// 
// 			fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   I");
// 			l_PrintPos(fout, EvalColmLcom.PosI);
// 
// 			fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   J");
// 			l_PrintPos(fout, EvalColmLcom.PosJ);				
// 		}		
// 		fout<<_T("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
// 	}
// 
// 	fout.close();
// 
// 	CDBLib::RunTextEditor(strOutFile, _STR(_T("STL Column Seismic Evaluate - 1ST STEP")), TRUE); 
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::PrintSeisEval1stRsltStlPanz()
// {
// 	ASSERT(FALSE);  // 아직 지원하지 않습니다.
// 	return FALSE;
// }
// 
// BOOL CSeisEvalResult::PrintSeisEval1stRsltStlBrac()
// {
// 	if (m_b1stEvalResult==FALSE)      return FALSE;
// 	T_ELEM_K_LIST aBracK;
// 	int nSizeBrac = m_m1stStlBrac.GetSortedKeyListforUintK(aBracK);
// 	if (nSizeBrac < 1)
// 	{
// 		AfxMessageBox(_STR(_T("Not exist steel brace result to evaluate.")));
// 		return FALSE;
// 	}
// 
// 	CWaitCursor WCursor;
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	CString strPathName = pDoc->GetPathName();
// 
// 	CFileCtrl FileCtrl(strPathName);
// 	CString strOutFile = _T("");	
// 	strOutFile.Format(_T("%s_StlBrac1st.out"), FileCtrl.GetFilePathAndNameWithoutExtension());
// 	rptwofstream fout(strOutFile, ios::trunc);
// 
// 	T_UNIT_INDEX CurrUnit;
// 	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
// 	T_UNIT_NAMEFACT CurUnitName;
// 	pDoc->m_pUnitCtrl->GetUnitNameCurrent(CurUnitName);
// 
// 	fout<<_T("========================================================================================================================================")<<endl
// 			<<_T("     STL BRACE SEISMIC EVALUATION RESULT (1ST STEP)")<<endl
// 			<<_T("    [UNIT] = ")<<CurUnitName.strForce[CurrUnit.nBase_Force]<<_T(", ")<<CurUnitName.strLength[CurrUnit.nBase_Length]<<endl
// 			<<_T("========================================================================================================================================")<<endl
// 			<<_T("  STOR   ELEM   LCOM PART           PCE           PnC          PnT       m-IO       m-LS       m-CP     DCR-IO     DCR-LS     DCR-CP  PL")<<endl
// 			<<_T("----------------------------------------------------------------------------------------------------------------------------------------")<<endl;
// 
// 	auto l_PrintPos = [&](rptwofstream& out, const T_EVAL1ST_STLBRAC_POS& pos)
// 	{		
// 		out<<GetValueForm(14, 3, pos.dPUD)
// 			 <<GetValueForm(14, 3, pos.dPnC)
// 			 <<GetValueForm(14, 3, pos.dPnT)
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_CP])						
// 			 <<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_CP])
// 			 <<_T("  ")<<GetPerformLevel4TextOut(pos.DCR.enLevel)
// 			 <<endl;
// 	};
// 
// 	T_STOR_K StorK;
// 	T_STOR_D StorD;
// 	T_LCOM_D LcomD;
// 	CString strElem = _T("");
// 	CString strStor = _T("");
// 	CString strLcom = _T("");
// 	CString strLcomType = _T("");
// 
// 	T_EVAL1ST_STLBRAC EvalColm;
// 	for (int i=0; i<nSizeBrac; ++i)
// 	{
// 		T_ELEM_K ElemK = aBracK[i];
// 		if (!Get1stStlBracRslt(ElemK, EvalColm)) { ASSERT(FALSE); continue; }
// 
// 		pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
// 		if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { ASSERT(FALSE); continue; }
// 
// 		int nSizeLcom = EvalColm.aEvalLcom.GetSize();
// 		for (int m=0; m<nSizeLcom; ++m)
// 		{
// 			const T_EVAL1ST_STLBRAC_LCOM & EvalColmLcom = EvalColm.aEvalLcom[m];			
// 
// 			if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, EvalColmLcom.LcomK, LcomD)) { continue; } 
// 			if (LcomD.nActive == 0) { continue; } 
// 
// 			strElem.Format(_T("%6d"), ElemK);
// 			strStor.Format(_T("%6s"), StorD.StoryName);
// 			strLcom.Format(_T("%6s"), LcomD.LoadCombName);
// 
// 			fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   I");
// 			l_PrintPos(fout, EvalColmLcom.PosI);
// 
// 			fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   J");
// 			l_PrintPos(fout, EvalColmLcom.PosJ);				
// 		}		
// 		fout<<_T("----------------------------------------------------------------------------------------------------------------------------------------")<<endl;
// 	}
// 
// 	fout.close();
// 
// 	CDBLib::RunTextEditor(strOutFile, _STR(_T("STL Brace Seismic Evaluate - 1ST STEP")), TRUE); 
// 
// 	return TRUE;
// }
// 
// BOOL CSeisEvalResult::PrintSeisEval1stRsltSrcBeam()
// {
// 	ASSERT(FALSE);  // 아직 지원하지 않습니다.
// 	return FALSE;
// }
// 
// BOOL CSeisEvalResult::PrintSeisEval1stRsltSrcColm()
// {
// 	ASSERT(FALSE);  // 아직 지원하지 않습니다.
// 	return FALSE;
// }


CString CSeisEvalResult::GetValueForm(int nTotalDigit, int nUnderDigit, double dValue)
{
	BOOL bChange=FALSE;
	if (nUnderDigit==0)
	{
		if(fabs(dValue) >= 1.0*pow(10,nTotalDigit))	bChange = TRUE;
		else if(fabs(dValue) < 1.0*pow(10,0))	      bChange = TRUE;
	}
	else
	{
		if (fabs(dValue) >= 1.0*pow(10,nTotalDigit-nUnderDigit-1))	bChange = TRUE;
		else if(fabs(dValue) < 1.0*pow(10,-nUnderDigit))			      bChange = TRUE;
	}

	// Add by ZINU.('00.9.5).
	// cRCS_Zero 보다 작은 경우 0.00~으로 출력 by eklee
	if (fabs(dValue) < 1.0e-7)	bChange = FALSE;

	CString strSymbol;
	CString strValue;
	if (nUnderDigit==0 && !bChange)
	{
		strSymbol.Format(_T("%%%dd"), nTotalDigit);
		strValue.Format(strSymbol, int(dValue));
	}
	else
	{
		if(bChange)	strSymbol.Format(_T("%%%d.%de"), nTotalDigit, nUnderDigit);
		else				strSymbol.Format(_T("%%%d.%df"), nTotalDigit, nUnderDigit);
		strValue.Format(strSymbol, dValue);
	}
	return strValue;
}

CString CSeisEvalResult::GetPerformLevel4TextOut(EN_LEVEL enLevel)
{
	switch (enLevel)
	{
	case EN_LEVEL_IO: return _LS(IDS_TB_DGN_IO);
	case EN_LEVEL_LS: return _LS(IDS_TB_DGN_LS);
	case EN_LEVEL_CP: return _LS(IDS_TB_DGN_CP);
	case EN_LEVEL_CL: return _LSX(**);
	case EN_LEVEL_INVALID: return _LSX(NO);
	default: ASSERT(FALSE); break;
	}
	return _T("  ");
}