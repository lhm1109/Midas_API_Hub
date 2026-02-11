// DB_QUERY_SEIS_CVL_JP.cpp: implementation of the query class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DB_QUERY_SEIS_CVL_JP.h"

#include "DBDoc.h"
#include "DBLib.h"
#include "DataMemb.h"
#include "UnitCtrl.h"
#include "AttrCtrl.h"
#include "AttrCtrl2.h"

#include "Qsort.h"
#include "DB_SIDC.h"
#include "DB_SIFA_H29.h"
#include "DB_SIPA.h"
#include "DB_SIGR.h"
#include "DB_SIGR_USER.h"
#include "DB_SIGR_UPPER.h"
#include "DB_SIGR_LOWER.h"
#include "DB_SIMD.h"
#include "DB_SIRD.h"
#include "DB_SIBD.h"
#include "DB_SIDP.h"
#include "DB_SISR.h"
#include "DB_SISR_USER.h"
#include "DB_SIET.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
#pragma region QSidc
//////////////////////////////////////////////////////////////////////

QSidc::QSidc(CDBDoc* pDoc, CDataMemb* pDataMemb, T_PREFERENCE* pPref)
{
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pSidc = pDataMemb->m_pSidc;
	m_pPreference = pPref;
}

QSidc::~QSidc()
{
	m_pUnitCtrl = NULL;
	m_pSidc = NULL;
	m_pPreference = NULL;
}

int QSidc::GetCount()
{
	return m_pSidc->GetCount();
}

POSITION QSidc::GetStart()
{
	return m_pSidc->GetStart();
}

void QSidc::GetNext(POSITION& rNextPosition, T_SIDC_K& rKey, T_SIDC_D& rData)
{
	m_pSidc->GetNext(rNextPosition, rKey, rData);
	m_pUnitCtrl->ConvertUnitSidcOut(rData);
}

BOOL QSidc::Exist(T_SIDC_K Key)
{
	T_SIDC_D Data;
	return m_pSidc->Get(Key, Data);
}

BOOL QSidc::Get(T_SIDC_D& rData)
{
	BOOL bReturn = m_pSidc->Get(rData);
	if (bReturn)m_pUnitCtrl->ConvertUnitSidcOut(rData);
	return bReturn;
}

BOOL QSidc::GetDgnSidc(T_SIDC_K Key, T_SIDC_D& rData)
{
	rData.Initialize();
	if (Exist(Key))
	{
		Get(rData);
	}
	else
	{
		CString strCode = m_pPreference->DgnCode.BrdgSeisEval;
		rData.nDesignCode = CDBLib::GetBrdgSeisEvalCodeNo_JP(strCode);
	}

	return TRUE;
}
#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QSifaH29
//////////////////////////////////////////////////////////////////////

QSifaH29::QSifaH29(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pSifaH29 = pDataMemb->m_pSifaH29;
}

QSifaH29::~QSifaH29()
{
	m_pUnitCtrl = NULL;
	m_pSifaH29 = NULL;
}

int QSifaH29::GetCount()
{
	return m_pSifaH29->GetCount();
}

POSITION QSifaH29::GetStart()
{
	return m_pSifaH29->GetStart();
}

void QSifaH29::GetNext(POSITION& rNextPosition, T_SIFA_H29_K& rKey, T_SIFA_H29_D& rData)
{
	m_pSifaH29->GetNext(rNextPosition, rKey, rData);
	m_pUnitCtrl->ConvertUnitSifaH29Out(rData);
}

BOOL QSifaH29::Exist(T_SIFA_H29_K Key)
{
	T_SIFA_H29_D Data;
	return m_pSifaH29->Get(Key, Data);
}

BOOL QSifaH29::Get(T_SIFA_H29_D& rData)
{
	BOOL bReturn = m_pSifaH29->Get(rData);
	if (bReturn) m_pUnitCtrl->ConvertUnitSifaH29Out(rData);
	return bReturn;
}

BOOL QSifaH29::GetSifaH29(T_SIFA_H29_K Key, T_SIFA_H29_D& rData)
{
	rData.Initialize();
	if (Exist(Key))
	{
		return Get(rData);
	}
	
	return FALSE;
}

double QSifaH29::GetXi1(const int& nLimitType, const int& nMaterialType)
{
	double dXi1 = 0.0;
	if (nMaterialType == T_SIGR_LOWER_D::kRC || nMaterialType == T_SIGR_LOWER_D::kSteel) dXi1 = 1.0;
	else return dXi1;

	if (nMaterialType == T_SIGR_LOWER_D::kSteel && nLimitType != T_SIPA_D::kLimitState2) return dXi1;

	GetHoriDistFactor(nLimitType, nMaterialType, T_SIFA_RC_Disp::kXi1, dXi1);
	return dXi1;
}

double QSifaH29::GetXi2(const int& nLimitType, const int& nMaterialType)
{
	double dXi2 = 0.0;
	if (nMaterialType == T_SIGR_LOWER_D::kRC || nMaterialType == T_SIGR_LOWER_D::kSteel) dXi2 = 1.0;
	else return dXi2;

	if (nMaterialType == T_SIGR_LOWER_D::kSteel && nLimitType != T_SIPA_D::kLimitState2) return dXi2;

	GetHoriDistFactor(nLimitType, nMaterialType, T_SIFA_RC_Disp::kXi2, dXi2);
	return dXi2;
}

double QSifaH29::GetPhiS(const int& nLimitType, const int& nMaterialType)
{
	double dPhiS = 0.0;
	if (nMaterialType == T_SIGR_LOWER_D::kRC) dPhiS = 0.65;
	else if (nMaterialType == T_SIGR_LOWER_D::kSteel) dPhiS = 0.75;
	else return dPhiS;

	if (nMaterialType == T_SIGR_LOWER_D::kSteel && nLimitType != T_SIPA_D::kLimitState2) return dPhiS;

	GetHoriDistFactor(nLimitType, nMaterialType, T_SIFA_RC_Disp::kPhiS, dPhiS);
	return dPhiS;
}

double QSifaH29::GetFactorK(const int& nMaterialType)
{
	T_SIFA_H29_D Data;
	if (Get(Data) == FALSE) return 0.0;

	double dK = 0.0;

	if (nMaterialType == T_SIGR_LOWER_D::kRC) {
		dK = Data.RcPierVal.dFactorK;
	}
	else if (nMaterialType == T_SIGR_LOWER_D::kSteel) {
		dK = Data.StPierVal.dFactorK;
	}
	else {
		return dK;
	}

	return dK;
}

void QSifaH29::GetHoriDistFactor(const int& nLimitType, const int& nMaterialType, const int& nRcDisp, OUT double& rVal)
{
	T_SIFA_H29_D Data;
	if (Get(Data) == FALSE) return;

	switch (nLimitType)
	{
	case T_SIPA_D::kLimitState1:
		rVal = Data.RcPierVal.Delta_yEd[nRcDisp];
		break;
	case T_SIPA_D::kLimitState2:
	{
		if (nMaterialType == T_SIGR_LOWER_D::kRC)
			rVal = Data.RcPierVal.Delta_ls2d[nRcDisp];
		else if (nMaterialType == T_SIGR_LOWER_D::kSteel)
			rVal = Data.StPierVal.Delta_ls2d[nRcDisp];
	}
	break;
	case T_SIPA_D::kLimitState3:
		rVal = Data.RcPierVal.Delta_ls3d[nRcDisp];
		break;
	}
}

#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QSipa
//////////////////////////////////////////////////////////////////////

QSipa::QSipa(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pSipa = pDataMemb->m_pSipa;
}

QSipa::~QSipa()
{
	m_pUnitCtrl = NULL;
	m_pSipa = NULL;
}

int QSipa::GetCount()
{
	return m_pSipa->GetCount();
}

POSITION QSipa::GetStart()
{
	return m_pSipa->GetStart();
}

void QSipa::GetNext(POSITION& rNextPosition, T_SIPA_K& rKey, T_SIPA_D& rData)
{
	m_pSipa->GetNext(rNextPosition, rKey, rData);
	m_pUnitCtrl->ConvertUnitSipaOut(rData);
}

BOOL QSipa::Exist(T_SIPA_K Key)
{
	T_SIPA_D Data;
	return m_pSipa->Get(Key, Data);
}

void QSipa::GetKeyList(CArray<T_SIPA_K, T_SIPA_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_SIPA_K Key;
	T_SIPA_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QSipa::Get(T_SIPA_K Key, T_SIPA_D& rData)
{
	BOOL bReturn = m_pSipa->Get(Key, rData);
	if (bReturn) m_pUnitCtrl->ConvertUnitSipaOut(rData);
	return bReturn;
}

T_SIPA_K QSipa::GetStartNum()
{
	return m_pSipa->m_nStartNum;
}

T_SIPA_K QSipa::GetKey(CString& rstrName)
{
	T_SIPA_K key;
	T_SIPA_D data;

	POSITION pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, key, data);
		if (rstrName.CompareNoCase(data.strCaseName) == 0) return key;
	}
	key = 0;
	return key;
}
#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QSigr
//////////////////////////////////////////////////////////////////////

QSigr::QSigr(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pSigr = pDataMemb->m_pSigr;
}

QSigr::~QSigr()
{
	m_pUnitCtrl = NULL;
	m_pSigr = NULL;
}

int QSigr::GetCount()
{
	return m_pSigr->GetCount();
}

POSITION QSigr::GetStart()
{
	return m_pSigr->GetStart();
}

void QSigr::GetNext(POSITION& rNextPosition, T_SIGR_K& rKey, T_SIGR_D& rData)
{
	m_pSigr->GetNext(rNextPosition, rKey, rData);
	m_pUnitCtrl->ConvertUnitSigrOut(rData);
}

BOOL QSigr::Exist(T_SIGR_K Key)
{
	T_SIGR_D Data;
	return m_pSigr->Get(Key, Data);
}

void QSigr::GetKeyList(CArray<T_SIGR_K, T_SIGR_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_SIGR_K Key;
	T_SIGR_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QSigr::Get(T_SIGR_K Key, T_SIGR_D& rData)
{
	BOOL bReturn = m_pSigr->Get(Key, rData);
	if (bReturn) m_pUnitCtrl->ConvertUnitSigrOut(rData);
	return bReturn;
}

T_SIGR_K QSigr::GetStartNum()
{
	return m_pSigr->m_nStartNum;
}

T_SIGR_K QSigr::GetKey(const CString& rstrName)
{
	T_SIGR_K key;
	T_SIGR_D data;

	POSITION pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, key, data);
		if (rstrName.CompareNoCase(data.strName) == 0) return key;
	}
	key = 0;
	return key;
}
#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QSigrUser
//////////////////////////////////////////////////////////////////////

QSigrUser::QSigrUser(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pSigr = pDataMemb->m_pSigr;
	m_pSigrUser = pDataMemb->m_pSigrUser;
}

QSigrUser::~QSigrUser()
{
	m_pUnitCtrl = NULL;
	m_pSigr = NULL;
	m_pSigrUser = NULL;
}

int QSigrUser::GetCount()
{
	return m_pSigrUser->GetCount();
}

POSITION QSigrUser::GetStart()
{
	return m_pSigrUser->GetStart();
}

void QSigrUser::GetNext(POSITION& rNextPosition, T_SIGR_USER_K& rKey, T_SIGR_USER_D& rData)
{
	m_pSigrUser->GetNext(rNextPosition, rKey, rData);
	m_pUnitCtrl->ConvertUnitSigrUserOut(rData);
}

BOOL QSigrUser::Exist(T_SIGR_USER_K Key)
{
	T_SIGR_USER_D Data;
	return m_pSigrUser->Get(Key, Data);
}

void QSigrUser::GetKeyList(CArray<T_SIGR_USER_K, T_SIGR_USER_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_SIGR_USER_K Key;
	T_SIGR_USER_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QSigrUser::Get(T_SIGR_USER_K Key, T_SIGR_USER_D& rData)
{
	BOOL bReturn = m_pSigrUser->Get(Key, rData);
	if (bReturn) m_pUnitCtrl->ConvertUnitSigrUserOut(rData);
	return bReturn;
}

CString QSigrUser::GetSigrName(const T_SIGR_USER_K& key)
{
	T_SIGR_USER_D data;
	if (Get(key, data) == FALSE)
		return _T("");

	T_SIGR_D sigrD;
	if (m_pSigr->Get(data.kSigr, sigrD) == FALSE)
		return _T("");

	return sigrD.strName;
}
#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QSigrUpper
//////////////////////////////////////////////////////////////////////

QSigrUpper::QSigrUpper(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pSigr = pDataMemb->m_pSigr;
	m_pSigrUpper = pDataMemb->m_pSigrUpper;
}

QSigrUpper::~QSigrUpper()
{
	m_pUnitCtrl = NULL;
	m_pSigr = NULL;
	m_pSigrUpper = NULL;
}

int QSigrUpper::GetCount()
{
	return m_pSigrUpper->GetCount();
}

POSITION QSigrUpper::GetStart()
{
	return m_pSigrUpper->GetStart();
}

void QSigrUpper::GetNext(POSITION& rNextPosition, T_SIGR_UPPER_K& rKey, T_SIGR_UPPER_D& rData)
{
	m_pSigrUpper->GetNext(rNextPosition, rKey, rData);
	m_pUnitCtrl->ConvertUnitSigrUpperOut(rData);
}

BOOL QSigrUpper::Exist(T_SIGR_UPPER_K Key)
{
	T_SIGR_UPPER_D Data;
	return m_pSigrUpper->Get(Key, Data);
}

void QSigrUpper::GetKeyList(CArray<T_SIGR_UPPER_K, T_SIGR_UPPER_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_SIGR_UPPER_K Key;
	T_SIGR_UPPER_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QSigrUpper::Get(T_SIGR_UPPER_K Key, T_SIGR_UPPER_D& rData)
{
	BOOL bReturn = m_pSigrUpper->Get(Key, rData);
	if (bReturn) m_pUnitCtrl->ConvertUnitSigrUpperOut(rData);
	return bReturn;
}

CString QSigrUpper::GetSigrName(const T_SIGR_UPPER_K& key)
{
	T_SIGR_UPPER_D data;
	if (Get(key, data) == FALSE)
		return _T("");

	T_SIGR_D sigrD;
	if (m_pSigr->Get(data.kSigr, sigrD) == FALSE)
		return _T("");

	return sigrD.strName;
}
#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QSigrLower
//////////////////////////////////////////////////////////////////////

QSigrLower::QSigrLower(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pDoc = pDoc;
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pSigr = pDataMemb->m_pSigr;
	m_pSigrLower = pDataMemb->m_pSigrLower;
	m_pElem = &pDataMemb->m_elem;
	m_pNode = &pDataMemb->m_node;
}

QSigrLower::~QSigrLower()
{
	m_pUnitCtrl = NULL;
	m_pSigr = NULL;
	m_pSigrLower = NULL;
	m_pElem = NULL;
	m_pNode = NULL;
}

int QSigrLower::GetCount()
{
	return m_pSigrLower->GetCount();
}

POSITION QSigrLower::GetStart()
{
	return m_pSigrLower->GetStart();
}

void QSigrLower::GetNext(POSITION& rNextPosition, T_SIGR_LOWER_K& rKey, T_SIGR_LOWER_D& rData)
{
	m_pSigrLower->GetNext(rNextPosition, rKey, rData);
	m_pUnitCtrl->ConvertUnitSigrLowerOut(rData);
}

BOOL QSigrLower::Exist(T_SIGR_LOWER_K Key)
{
	T_SIGR_LOWER_D Data;
	return m_pSigrLower->Get(Key, Data);
}

void QSigrLower::GetKeyList(CArray<T_SIGR_LOWER_K, T_SIGR_LOWER_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_SIGR_LOWER_K Key;
	T_SIGR_LOWER_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QSigrLower::Get(T_SIGR_LOWER_K Key, T_SIGR_LOWER_D& rData)
{
	BOOL bReturn = m_pSigrLower->Get(Key, rData);
	if (bReturn) m_pUnitCtrl->ConvertUnitSigrLowerOut(rData);
	return bReturn;
}

CString QSigrLower::GetSigrName(const T_SIGR_LOWER_K& key)
{
	T_SIGR_LOWER_D data;
	if (Get(key, data) == FALSE)
		return _T("");

	T_SIGR_D sigrD;
	if (m_pSigr->Get(data.kSigr, sigrD) == FALSE)
		return _T("");

	return sigrD.strName;
}

void QSigrLower::GetInvestigatedElem(const T_SIGR_LOWER_K& key, CArray<T_ELEM_K, T_ELEM_K>& rElements)
{
	T_SIGR_LOWER_D data;
	if (Get(key, data) == FALSE) return;

	for (int i = 0; i < data.COLM_D.GetCount(); i++) {
		rElements.Append(data.COLM_D[i].aInvestigatedElem);
	}

	for (int i = 0; i < data.BEAM_D.GetCount(); i++) {
		rElements.Append(data.BEAM_D[i].aInvestigatedElem);
	}

	for (int i = 0; i < data.FOOT_D.GetCount(); i++) {
		rElements.Append(data.FOOT_D[i].aInvestigatedElem);
	}
}

void QSigrLower::GetNodeKeysOfColumn(const T_SIGR_LOWER_K& key, CArray<T_NODE_K, T_NODE_K>& rNodeKeys)
{
	T_SIGR_LOWER_D data;
	if (Get(key, data) == FALSE) return;

	for (int i = 0; i < data.COLM_D.GetCount(); i++) {
		T_SIGR_LOWER_COLM_D& colm = data.COLM_D[i];

		for (int j = 0; j < colm.aInvestigatedElem.GetCount(); j++)	{
			T_ELEM_K& kElem = colm.aInvestigatedElem[j];
			T_ELEM_D dElem;
			if (m_pElem->Get(kElem, dElem) == FALSE)
				continue;

			for (int k = 0; k < D_ELEM_MAXNOD; k++)	{
				if (dElem.elnod[k] != 0)
					rNodeKeys.Add(dElem.elnod[k]);
			}
		}
	}
}

BOOL QSigrLower::FindColumn(const T_SIGR_LOWER_K& key, const CString& strNameCOLM, OUT T_SIGR_LOWER_COLM_D& rColumn)
{
	T_SIGR_LOWER_D data;
	if (Get(key, data) == FALSE) return FALSE;

	for (int i = 0; i < data.COLM_D.GetCount(); i++) {
		T_SIGR_LOWER_COLM_D& colm = data.COLM_D[i];

		if (strNameCOLM.CompareNoCase(colm.strName) == 0) {
			rColumn = colm;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL QSigrLower::FindColumnLowNode(const T_SIGR_LOWER_K& key, const CString& strNameCOLM, OUT ColumnData& rColumn)
{
	T_SIGR_LOWER_D tLower;
	if (Get(key, tLower) == FALSE) return FALSE;

	T_SIGR_LOWER_COLM_D colm;
	if (FindColumn(key, strNameCOLM, colm) == FALSE) return FALSE;
		
	double dZMin = DBL_MAX;
	T_NODE_K kNodeMin = 0;
	T_ELEM_K kElemMin = 0;
	int elnod_index = 0;

	for (int j = 0; j < colm.aInvestigatedElem.GetCount(); j++) {
		T_ELEM_K kElem = colm.aInvestigatedElem[j];
		T_ELEM_D dElem;
		if (m_pElem->Get(kElem, dElem) == FALSE)
			continue;

		for (int k = 0; k < D_ELEM_MAXNOD; k++) {
			if (dElem.elnod[k] != 0) {
				T_NODE_D dNode;
				if (m_pNode->Get(dElem.elnod[k], dNode) == FALSE)
					continue;

				if (dNode.z < dZMin) {
					dZMin = dNode.z;
					elnod_index = k;
					kElemMin = kElem;
					kNodeMin = dElem.elnod[k];
				}
			}
		}
	}

	if (dZMin != DBL_MAX) {
		rColumn.kElem = kElemMin;
		rColumn.kNodeLow = kNodeMin;
		rColumn.Column = colm;
		rColumn.eMatlType = (T_SIGR_LOWER_D::eMaterialType)tLower.nMaterialType;
		rColumn.ePosNodeLow = elnod_index == 0 ? T_SIGR_LOWER_COLM_D::kI : T_SIGR_LOWER_COLM_D::kJ;
		return TRUE;
	}

	return FALSE;
}

void QSigrLower::GetColumnElemKeysFromPos(const BOOL& fromBottom,
	const T_SIGR_LOWER_K& key,
	const T_SIGR_LOWER_COLM_D* pColumn,
	OUT T_ELEM_K_LIST& sortedElemKeys)
{
	std::map<T_ELEM_K, double> mNodeZ;

	for (int j = 0; j < pColumn->aInvestigatedElem.GetCount(); j++) {
		T_ELEM_K kElem = pColumn->aInvestigatedElem[j];
		T_ELEM_D tElem;
		if (m_pElem->Get(kElem, tElem) == FALSE) {
			ASSERT(FALSE);
			continue;
		}

		T_NODE_D NodeI;
		if (!m_pNode->Get(tElem.elnod[0], NodeI)) {
			ASSERT(FALSE);
			continue;
		}

		T_NODE_D NodeJ;
		if (!m_pNode->Get(tElem.elnod[1], NodeJ)) {
			ASSERT(FALSE);
			continue;
		}

		double dMiddleZ = (NodeI.z + NodeJ.z) / 2.0;

		mNodeZ.insert(std::make_pair(kElem, dMiddleZ));
	}

	std::vector< std::pair<T_ELEM_K, double> > vNodeZ(mNodeZ.begin(), mNodeZ.end());

	std::sort(vNodeZ.begin(), vNodeZ.end(), [fromBottom](std::pair<T_ELEM_K, double> a, std::pair<T_ELEM_K, double> b) {
		if (fromBottom == TRUE)
			return a.second < b.second;
		else
			return a.second > b.second;
		});

	for (std::pair<T_ELEM_K, double> iter : vNodeZ)
	{
		sortedElemKeys.Add(iter.first);
	}
}

T_SIGR_LOWER_BEAR_D::eBearingType QSigrLower::GetBearingType(const T_SIGR_LOWER_K& key)
{
	T_SIGR_LOWER_BEAR_D::eBearingType eBearingType = T_SIGR_LOWER_BEAR_D::kUnKnown;

	T_SIGR_LOWER_D data;
	if (Get(key, data) == FALSE) return eBearingType;

	for (int i = 0; i < data.BEAR_D.GetCount(); i++) {
		const T_SIGR_LOWER_BEAR_D& bear = data.BEAR_D[i];

		if(bear.nType == T_SIGR_LOWER_BEAR_D::kUnKnown)
			continue;

		eBearingType = (T_SIGR_LOWER_BEAR_D::eBearingType)bear.nType;
		break;
	}

	return eBearingType;
}

T_SIGR_LOWER_D::eMaterialType QSigrLower::GetMaterialType(const CString& strGroupName)
{
	T_SIGR_K key = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroupName);
	T_SIGR_LOWER_D data;
	if (Get(key, data) == FALSE)
		return T_SIGR_LOWER_D::eMaterialType::kUnKnown;

	return (T_SIGR_LOWER_D::eMaterialType)data.nMaterialType;
}

BOOL QSigrLower::FindFooting(const T_SIGR_LOWER_K& key, const T_NODE_K& kNodeBotOfColoum, OUT FootingData& rFooting)
{
	auto L_GetMinNode = [&](const T_ELEM_K& kElem, OUT double& rMinLevel)
	{
		T_ELEM_D dElem;
		if (m_pElem->Get(kElem, dElem) == FALSE) return FALSE;

		BOOL IsLinkedFoot = FALSE;
		for (int k = 0; k < D_ELEM_MAXNOD; k++) {
			if (dElem.elnod[k] == 0) continue;

			T_NODE_K kNode = dElem.elnod[k];
			if (kNodeBotOfColoum == kNode)
				IsLinkedFoot = TRUE;

			T_NODE_D nodeD;
			if(m_pDoc->m_pAttrCtrl->GetNode(kNode, nodeD) == FALSE) continue;

			if (nodeD.z < rMinLevel) {
				rMinLevel = nodeD.z;
				rFooting.kElem = kElem;
				rFooting.kNodeLow = kNode;
			}
		}

		return IsLinkedFoot;
	};
	
	T_SIGR_LOWER_D data;
	if (Get(key, data) == FALSE) return FALSE;

	double dMinLevel = DBL_MAX;
	T_ELEM_K kElemLinked = 0;
	for (int i = 0; i < data.FOOT_D.GetCount(); i++) {
		T_SIGR_LOWER_FOOT_D& foot = data.FOOT_D[i];

		for (int j = 0; j < foot.aInvestigatedElem.GetCount(); j++) {
			T_ELEM_K& kElem = foot.aInvestigatedElem[j];

			BOOL IsLinkedFoot = L_GetMinNode(kElem, dMinLevel);
			if (IsLinkedFoot == TRUE) {
				kElemLinked = kElem;
			}
		}
	}

	if (kElemLinked != 0) {
		dMinLevel = DBL_MAX;
		L_GetMinNode(kElemLinked, dMinLevel);
	}

	return (dMinLevel != DBL_MAX);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QSimd
//////////////////////////////////////////////////////////////////////

QSimd::QSimd(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pSimd = pDataMemb->m_pSimd;
	m_pSipa = pDataMemb->m_pSipa;
	m_pSigr = pDataMemb->m_pSigr;
	m_pSigrLower = pDataMemb->m_pSigrLower;
}

QSimd::~QSimd()
{
	m_pUnitCtrl = NULL;
	m_pSimd = NULL;
	m_pSipa = NULL;
	m_pSigrLower = NULL;
}

int QSimd::GetCount()
{
	return m_pSimd->GetCount();
}

POSITION QSimd::GetStart()
{
	return m_pSimd->GetStart();
}

void QSimd::GetNext(POSITION& rNextPosition, T_SIMD_K& rKey, T_SIMD_D& rData)
{
	m_pSimd->GetNext(rNextPosition, rKey, rData);
	m_pUnitCtrl->ConvertUnitSimdOut(rData);
}

BOOL QSimd::Exist(T_SIMD_K Key)
{
	T_SIMD_D Data;
	return m_pSimd->Get(Key, Data);
}

void QSimd::GetKeyList(CArray<T_SIMD_K, T_SIMD_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_SIMD_K Key;
	T_SIMD_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QSimd::Get(T_SIMD_K Key, T_SIMD_D& rData)
{
	BOOL bReturn = m_pSimd->Get(Key, rData);
	if (bReturn) m_pUnitCtrl->ConvertUnitSimdOut(rData);
	return bReturn;
}

BOOL QSimd::Get(T_SIGR_K KSigr, T_SIPA_K kSpia, T_SIMD_D& rData)
{
	POSITION pos;
	T_SIMD_K Key;
	T_SIMD_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		
		if (Data.kSigr == KSigr && Data.kSipa == kSpia) {
			rData = Data;
			return TRUE;
		}
	}

	return FALSE;
}

T_SIMD_K QSimd::GetStartNum()
{
	return m_pSimd->m_nStartNum;
}

BOOL QSimd::GetInertialForceNode(const T_SIMD_K& Key, const seis_jp::eDirAnal& eDir, OUT T_NODE_K& kNode)
{
	T_SIMD_D tSimd; T_SIPA_D tSipa; T_SIGR_D tSigr;
	if (Get(Key, tSimd) == FALSE) return FALSE;
	if (m_pSipa->Get(tSimd.kSipa, tSipa) == FALSE) return FALSE;
	if (m_pSigr->Get(tSimd.kSigr, tSigr) == FALSE) return FALSE;

	T_SIGR_LOWER_D dSigrLower;
	if (tSigr.nSubType == T_SIGR_D::eSubType::kLower) {
		if (m_pSigrLower->Get(tSimd.kSigr, dSigrLower) == FALSE) return FALSE;
	}
	else {
		switch (eDir)
		{
		case seis_jp::kPlus:
			if (m_pSigrLower->Get(tSimd.PLUS_D.kSigrLower, dSigrLower) == FALSE) return FALSE;
			break;
		case seis_jp::kMinus:
			if (m_pSigrLower->Get(tSimd.MINUS_D.kSigrLower, dSigrLower) == FALSE) return FALSE;
			break;
		default:
			break;
		}
	}

	if (tSipa.nSeismicForceDirection == T_SIPA_D::kAxis_Dir)
		kNode = dSigrLower.kNodeAxis;
	else if (tSipa.nSeismicForceDirection == T_SIPA_D::kPerp_Dir)
		kNode = dSigrLower.kNodePerp;
	else
		return FALSE;

	return TRUE;
}

void QSimd::GetTimeRange(const T_SIMD_K& Key, const T_THIS_K& kThis, OUT double& rTimeStt, OUT double& rTiemEnd)
{
	T_SIMD_D tSimd;
	if (Get(Key, tSimd) == FALSE) return;

	for (int i = 0; i < tSimd.aTimeRange.GetCount(); i++)
	{
		const T_SIMD_TIME_RANGE_D& renage = tSimd.aTimeRange[i];
		if (renage.kThis == kThis) {
			rTimeStt = renage.dTimeStt;
			rTiemEnd = renage.dTimeEnd;
			break;
		}
	}
}
#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QSird
//////////////////////////////////////////////////////////////////////

QSird::QSird(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pSird = pDataMemb->m_pSird;
	m_pSipa = pDataMemb->m_pSipa;
	m_pSigrLower = pDataMemb->m_pSigrLower;
}

QSird::~QSird()
{
	m_pUnitCtrl = NULL;
	m_pSird = NULL;
}

int QSird::GetCount()
{
	return m_pSird->GetCount();
}

POSITION QSird::GetStart()
{
	return m_pSird->GetStart();
}

void QSird::GetNext(POSITION& rNextPosition, T_SIRD_K& rKey, T_SIRD_D& rData)
{
	m_pSird->GetNext(rNextPosition, rKey, rData);
	m_pUnitCtrl->ConvertUnitSirdOut(rData);
}

BOOL QSird::Exist(T_SIRD_K Key)
{
	T_SIRD_D Data;
	return m_pSird->Get(Key, Data);
}

void QSird::GetKeyList(CArray<T_SIRD_K, T_SIRD_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_SIRD_K Key;
	T_SIRD_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QSird::Get(T_SIRD_K Key, T_SIRD_D& rData)
{
	BOOL bReturn = m_pSird->Get(Key, rData);
	if (bReturn) m_pUnitCtrl->ConvertUnitSirdOut(rData);
	return bReturn;
}

T_SIRD_K QSird::GetStartNum()
{
	return m_pSird->m_nStartNum;
}

BOOL QSird::Get(const T_SIPA_K& kSipa, const T_SIGR_K& kSigr, T_SIRD_D& rData)
{
	POSITION pos;
	T_SIRD_K Key;
	T_SIRD_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		
		if (Data.kSipa == kSipa && Data.kSigr == kSigr) {
			rData = Data;
			m_pUnitCtrl->ConvertUnitSirdOut(rData);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL QSird::GetInertialForceNode(T_SIRD_K Key, const seis_jp::eDirAnal& eDir, OUT T_NODE_K& kNode)
{
	T_SIRD_D data;
	if (Get(Key, data) == FALSE) return FALSE;

	T_SIPA_D dSipa;
	if (m_pSipa->Get(data.kSipa, dSipa) == FALSE) return FALSE;

	T_SIGR_LOWER_D dSigrLower;
	switch (eDir)
	{
	case seis_jp::kPlus:
		if (m_pSigrLower->Get(data.PLUS_D.kSigrLower, dSigrLower) == FALSE) return FALSE;
		break;
	case seis_jp::kMinus:
		if (m_pSigrLower->Get(data.MINUS_D.kSigrLower, dSigrLower) == FALSE) return FALSE;
		break;
	default:
		break;
	}

	if (dSipa.nSeismicForceDirection == T_SIPA_D::kAxis_Dir)
		kNode = dSigrLower.kNodeAxis;
	else if (dSipa.nSeismicForceDirection == T_SIPA_D::kPerp_Dir)
		kNode = dSigrLower.kNodePerp;
	else
		return FALSE;

	return TRUE;
}

void QSird::GetTimeRange(const T_SIRD_K& Key, const T_THIS_K& kThis, OUT double& rTimeStt, OUT double& rTiemEnd)
{
	T_SIRD_D tSird;
	if (Get(Key, tSird) == FALSE) return;

	for (int i = 0; i < tSird.aTimeRange.GetCount(); i++)
	{
		const T_SIRD_TIME_RANGE_D& renage = tSird.aTimeRange[i];
		if (renage.kThis == kThis) {
			rTimeStt = renage.dTimeStt;
			rTiemEnd = renage.dTimeEnd;
			break;
		}
	}
}
#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QSibd
//////////////////////////////////////////////////////////////////////

QSibd::QSibd(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pSibd = pDataMemb->m_pSibd;
	m_pSigr = pDataMemb->m_pSigr;
	m_pSigrUser = pDataMemb->m_pSigrUser;
	m_pSigrLower = pDataMemb->m_pSigrLower;
	m_pSigrUpper = pDataMemb->m_pSigrUpper;
}

QSibd::~QSibd()
{
	m_pUnitCtrl = NULL;
	m_pSibd = NULL;
	m_pSigr = NULL;
	m_pSigrUser = NULL;
	m_pSigrLower = NULL;
	m_pSigrUpper = NULL;
}

int QSibd::GetCount()
{
	return m_pSibd->GetCount();
}

POSITION QSibd::GetStart()
{
	return m_pSibd->GetStart();
}

void QSibd::GetNext(POSITION& rNextPosition, T_SIBD_K& rKey, T_SIBD_D& rData)
{
	m_pSibd->GetNext(rNextPosition, rKey, rData);
	m_pUnitCtrl->ConvertUnitSibdOut(rData);
}

BOOL QSibd::Exist(T_SIBD_K Key)
{
	T_SIBD_D Data;
	return m_pSibd->Get(Key, Data);
}

void QSibd::GetKeyList(CArray<T_SIBD_K, T_SIBD_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_SIBD_K Key;
	T_SIBD_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QSibd::Get(T_SIBD_K Key, T_SIBD_D& rData)
{
	BOOL bReturn = m_pSibd->Get(Key, rData);
	if (bReturn) m_pUnitCtrl->ConvertUnitSibdOut(rData);
	return bReturn;
}

T_SIBD_K QSibd::GetStartNum()
{
	return m_pSibd->m_nStartNum;
}

void QSibd::GetElementKeys(const T_SIBD_K& Key, OUT T_ELEM_K_LIST& rElemList)
{
	T_SIBD_D dSibd;
	if (Get(Key, dSibd) == FALSE) return;

	T_SIGR_D dSigr;
	if (m_pSigr->Get(dSibd.kSigr, dSigr) == FALSE) return;

	if (dSigr.nSubType == T_SIGR_D::kLower) {
		T_SIGR_LOWER_D dLower;
		if (m_pSigrLower->Get(dSibd.kSigr, dLower) == FALSE) return;

		for (int i = 0; i < dLower.COLM_D.GetCount(); i++) {
			for (int j = 0; j < dLower.COLM_D[i].aInvestigatedElem.GetCount(); j++) {
				T_ELEM_K kElem = dLower.COLM_D[i].aInvestigatedElem[j];
				rElemList.Add(kElem);
			}

		}
		for (int i = 0; i < dLower.BEAM_D.GetCount(); i++) {
			for (int j = 0; j < dLower.BEAM_D[i].aInvestigatedElem.GetCount(); j++) {
				T_ELEM_K kElem = dLower.BEAM_D[i].aInvestigatedElem[j];
				rElemList.Add(kElem);
			}
		}
	}
	else if (dSigr.nSubType == T_SIGR_D::kUpper) {
		T_SIGR_UPPER_D dUpper;
		if (m_pSigrUpper->Get(dSibd.kSigr, dUpper) == FALSE) return;
		
		for (int i = 0; i < dUpper.aElements.GetCount(); i++) {
			T_ELEM_K kElem = dUpper.aElements[i];
			rElemList.Add(kElem);
		}
	}
	else if (dSigr.nSubType == T_SIGR_D::kUser) {
		T_SIGR_USER_D dUser;
		if (m_pSigrUser->Get(dSibd.kSigr, dUser) == FALSE) return;

		for (int i = 0; i < dUser.aInvestigatedElem.GetCount(); i++) {
			T_ELEM_K kElem = dUser.aInvestigatedElem[i];
			rElemList.Add(kElem);
		}
	}
}
#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QSidp
//////////////////////////////////////////////////////////////////////

QSidp::QSidp(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pSidp = pDataMemb->m_pSidp;
}

QSidp::~QSidp()
{
	m_pUnitCtrl = NULL;
	m_pSidp = NULL;
}

int QSidp::GetCount()
{
	return m_pSidp->GetCount();
}

POSITION QSidp::GetStart()
{
	return m_pSidp->GetStart();
}

void QSidp::GetNext(POSITION& rNextPosition, T_SIDP_K& rKey, T_SIDP_D& rData)
{
	m_pSidp->GetNext(rNextPosition, rKey, rData);
	m_pUnitCtrl->ConvertUnitSidpOut(rData);
}

BOOL QSidp::Exist(T_SIDP_K Key)
{
	T_SIDP_D Data;
	return m_pSidp->Get(Key, Data);
}

void QSidp::GetKeyList(CArray<T_SIDP_K, T_SIDP_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_SIDP_K Key;
	T_SIDP_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QSidp::Get(T_SIDP_K Key, T_SIDP_D& rData)
{
	BOOL bReturn = m_pSidp->Get(Key, rData);
	if (bReturn) m_pUnitCtrl->ConvertUnitSidpOut(rData);
	return bReturn;
}

T_SIDP_K QSidp::GetStartNum()
{
	return m_pSidp->m_nStartNum;
}
#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QSisr
//////////////////////////////////////////////////////////////////////

QSisr::QSisr(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pSisr = pDataMemb->m_pSisr;
	m_pSigr = pDataMemb->m_pSigr;
	m_pSigrUser = pDataMemb->m_pSigrUser;
	m_pSigrLower = pDataMemb->m_pSigrLower;
	m_pSigrUpper = pDataMemb->m_pSigrUpper;
}

QSisr::~QSisr()
{
	m_pUnitCtrl = NULL;
	m_pSisr = NULL;
	m_pSigr = NULL;
	m_pSigrUser = NULL;
	m_pSigrLower = NULL;
	m_pSigrUpper = NULL;
}

int QSisr::GetCount()
{
	return m_pSisr->GetCount();
}

POSITION QSisr::GetStart()
{
	return m_pSisr->GetStart();
}

void QSisr::GetNext(POSITION& rNextPosition, T_SISR_K& rKey, T_SISR_D& rData)
{
	m_pSisr->GetNext(rNextPosition, rKey, rData);
	m_pUnitCtrl->ConvertUnitSisrOut(rData);
}

void QSisr::GetKeyList(CArray<T_SISR_K, T_SISR_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_SISR_K Key;
	T_SISR_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QSisr::Exist(T_SISR_K Key)
{
	T_SISR_D Data;
	return m_pSisr->Get(Key, Data);
}

BOOL QSisr::Get(T_SISR_K Key, T_SISR_D& rData)
{
	BOOL bReturn = m_pSisr->Get(Key, rData);
	if (bReturn) m_pUnitCtrl->ConvertUnitSisrOut(rData);
	return bReturn;
}

T_SISR_K QSisr::GetStartNum()
{
	return m_pSisr->m_nStartNum;
}

T_SISR_K QSisr::GetKeyEqualSipaAndSigrKey(const T_SIPA_K& kSipa, const T_SIGR_K& kSigr)
{
	POSITION pos;
	T_SISR_K Key;
	T_SISR_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		if (Data.kSipa == kSipa && Data.kSigr == kSigr)
			return Key;
	}
	return 0;
}

void QSisr::GetElementKeys(const T_SISR_K& Key, OUT T_ELEM_K_LIST& rElemList)
{
	T_SISR_D dSisr;
	if (Get(Key, dSisr) == FALSE) return;

	T_SIGR_D dSigr;
	if (m_pSigr->Get(dSisr.kSigr, dSigr) == FALSE) return;

	if (dSigr.nSubType == T_SIGR_D::kLower) {
		T_SIGR_LOWER_D dLower;
		if (m_pSigrLower->Get(dSisr.kSigr, dLower) == FALSE) return;

		for (int i = 0; i < dLower.COLM_D.GetCount(); i++) {
			for (int j = 0; j < dLower.COLM_D[i].aInvestigatedElem.GetCount(); j++) {
				T_ELEM_K kElem = dLower.COLM_D[i].aInvestigatedElem[j];
				rElemList.Add(kElem);
			}

		}
		for (int i = 0; i < dLower.BEAM_D.GetCount(); i++) {
			for (int j = 0; j < dLower.BEAM_D[i].aInvestigatedElem.GetCount(); j++) {
				T_ELEM_K kElem = dLower.BEAM_D[i].aInvestigatedElem[j];
				rElemList.Add(kElem);
			}
		}
	}
	else if (dSigr.nSubType == T_SIGR_D::kUpper) {
		T_SIGR_UPPER_D dUpper;
		if (m_pSigrUpper->Get(dSisr.kSigr, dUpper) == FALSE) return;

		for (int i = 0; i < dUpper.aElements.GetCount(); i++) {
			T_ELEM_K kElem = dUpper.aElements[i];
			rElemList.Add(kElem);
		}
	}
	else if (dSigr.nSubType == T_SIGR_D::kUser) {
		T_SIGR_USER_D dUser;
		if (m_pSigrUser->Get(dSisr.kSigr, dUser) == FALSE) return;

		for (int i = 0; i < dUser.aInvestigatedElem.GetCount(); i++) {
			T_ELEM_K kElem = dUser.aInvestigatedElem[i];
			rElemList.Add(kElem);
		}
	}
}
#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QSisrUser
//////////////////////////////////////////////////////////////////////

QSisrUser::QSisrUser(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pSisrUser = pDataMemb->m_pSisrUser;
}

QSisrUser::~QSisrUser()
{
	m_pUnitCtrl = NULL;
	m_pSisrUser = NULL;
}

int QSisrUser::GetCount()
{
	return m_pSisrUser->GetCount();
}

POSITION QSisrUser::GetStart()
{
	return m_pSisrUser->GetStart();
}

void QSisrUser::GetNext(POSITION& rNextPosition, T_SISR_USER_K& rKey, T_SISR_USER_D& rData)
{
	m_pSisrUser->GetNext(rNextPosition, rKey, rData);
	m_pUnitCtrl->ConvertUnitSisrUserOut(rData);
}

void QSisrUser::GetKeyList(CArray<T_SISR_USER_K, T_SISR_USER_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_SISR_USER_K Key;
	T_SISR_USER_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QSisrUser::Exist(T_SISR_USER_K Key)
{
	T_SISR_USER_D Data;
	return m_pSisrUser->Get(Key, Data);
}

BOOL QSisrUser::Get(T_SISR_USER_K Key, T_SISR_USER_D& rData)
{
	BOOL bReturn = m_pSisrUser->Get(Key, rData);
	if (bReturn) m_pUnitCtrl->ConvertUnitSisrUserOut(rData);
	return bReturn;
}

T_SISR_K QSisrUser::GetStartNum()
{
	return m_pSisrUser->m_nStartNum;
}

void QSisrUser::GetKeysEqualSisrKey(const T_SISR_K& kSisr, OUT CArray<T_SISR_USER_K, T_SISR_USER_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_SISR_USER_K Key;
	T_SISR_USER_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		if(Data.kSisr == kSisr)
			rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

void QSisrUser::GetElemKeysEqualSisrKey(const T_SISR_K& kSisr, OUT CArray<T_ELEM_K, T_ELEM_K>& rElemKeys)
{
	rElemKeys.RemoveAll();

	POSITION pos;
	T_SISR_USER_K Key;
	T_SISR_USER_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		if (Data.kSisr == kSisr)
			rElemKeys.Add(Data.kElem);
	}
	if (rElemKeys.GetSize() > 0)
		CQSort::QSortUInt(rElemKeys.GetData(), rElemKeys.GetSize());
}

T_SISR_USER_K QSisrUser::GetKeyEqualElemKey(const T_ELEM_K& kElem)
{
	POSITION pos;
	T_SISR_USER_K Key;
	T_SISR_USER_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		if (Data.kElem == kElem)
			return Key;
	}
	return 0;
}

T_SISR_USER_K QSisrUser::GetKeyEqualSisrAndElem(const T_SISR_K& kSisr, const T_ELEM_K kElem)
{
	POSITION pos;
	T_SISR_USER_K Key;
	T_SISR_USER_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		if (Data.kSisr != kSisr)
			continue;
		if (Data.kElem == kElem)
			return Key;
	}
	return 0;
}

void QSisrUser::GetAllElemKeyList(OUT CArray<T_ELEM_K, T_ELEM_K>& rElemKeys)
{
	POSITION pos;
	T_SISR_USER_K Key;
	T_SISR_USER_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rElemKeys.Add(Data.kElem);
	}
}

BOOL QSisrUser::ExistElem(const T_SISR_K& kSisr, const T_ELEM_K kElem)
{
	CArray<T_ELEM_K, T_ELEM_K> elemKeys;
	GetElemKeysEqualSisrKey(kSisr, elemKeys);

	auto it = std::find(elemKeys.GetData(), elemKeys.GetData() + elemKeys.GetSize(), kElem);
	if (it != elemKeys.GetData() + elemKeys.GetSize()) {
		return TRUE;
	}
	return FALSE;
}

#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QSiet
//////////////////////////////////////////////////////////////////////

QSiet::QSiet(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pSiet = pDataMemb->m_pSiet;
}

QSiet::~QSiet()
{
	m_pUnitCtrl = NULL;
	m_pSiet = NULL;
}

int QSiet::GetCount()
{
	return m_pSiet->GetCount();
}

POSITION QSiet::GetStart()
{
	return m_pSiet->GetStart();
}

void QSiet::GetNext(POSITION& rNextPosition, T_SIET_K& rKey, T_SIET_D& rData)
{
	m_pSiet->GetNext(rNextPosition, rKey, rData);
	m_pUnitCtrl->ConvertUnitSietOut(rData);
}

BOOL QSiet::Exist(T_SIET_K Key)
{
	T_SIET_D Data;
	return m_pSiet->Get(Key, Data);
}

void QSiet::GetKeyList(CArray<T_SIET_K, T_SIET_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_SIET_K Key;
	T_SIET_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QSiet::Get(T_SIET_K Key, T_SIET_D& rData)
{
	BOOL bReturn = m_pSiet->Get(Key, rData);
	if (bReturn) m_pUnitCtrl->ConvertUnitSietOut(rData);
	return bReturn;
}

T_SIET_K QSiet::GetStartNum()
{
	return m_pSiet->m_nStartNum;
}
#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QIehp
//////////////////////////////////////////////////////////////////////

#include "wg_db_AttrCtrl.h"

QIehp::QIehp(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pDoc = pDoc;
}

QIehp::~QIehp()
{
	m_pDoc = NULL;
}

BOOL QIehp::GetHingeProp(const T_ELEM_K& kElem, IN int component, IN int nPos, OUT MPhiProp& rProp)
{
	rProp.Initialize();

	T_IEHG_D IehgD;
	if (!m_pDoc->m_pAttrCtrl->GetIehgByElemK(kElem, IehgD)) { return FALSE; }

	T_IEHP_D IehpD;
	if (!m_pDoc->m_pAttrCtrl->GetIehp(IehgD.IehpKey, IehpD)) { ASSERT(0); return FALSE; }

	const int nDOF = [component]()
		{
			switch (component)
			{
			case kFx: return 0;
			case kFy: return 1;
			case kFz: return 2;
			case kMx: return 3;
			case kMy: return 4;
			case kMz: return 5;
			default: ASSERT(0); return 0;
			}
		}();

	T_IEHP_PROP IehpProp;
	const bool bExistHinge = [&IehpD, nDOF, nPos](OUT T_IEHP_PROP& rIehpProp)
		{
			rIehpProp.YLDSUR.Initialize(); // Union 이여서 제일 큰것만 초기화. see <DB_ST_DT_ITHA.h>	

			switch (IehpD.nHingeType)
			{
			case D_IEHP_HTYP_BEAM_LUMP:
			{
				switch (nPos)
				{
				case ePosI:
				{
					rIehpProp = IehpD.AllProp.PROP[nDOF];
					return IehpD.nHingeLocation[nDOF] == 1 ? false : true;
				}
				case ePosJ:
				{
					rIehpProp = IehpD.nHingeLocation[nDOF] == 2 ? IehpD.AllSubProp.PROP[nDOF] : IehpD.AllProp.PROP[nDOF];
					return IehpD.nHingeLocation[nDOF] == 0 ? false : true;
				}
				default:
				{
					ASSERT(0);
					return false;
				}
				}
			}
			case D_IEHP_HTYP_BEAM_DIST:
			{
				rIehpProp = IehpD.AllProp.PROP[nDOF];
				return true;
			}
			case D_IEHP_HTYP_SPRING:
			case D_IEHP_HTYP_TRUSS:
			case D_IEHP_HTYP_WALL:
			default:
			{
				ASSERT(0);
				return false;
			}
			}
		}(IehpProp);

	if (!bExistHinge) return FALSE;

	switch (IehpD.nHysModel[nDOF])
	{
	case D_IEHP_KINE:
		return GetHingeProp_IEHP_KINE(IehpProp.KINEMA, nDOF, rProp);
	case D_IEHP_ORIG:
		return GetHingeProp_IEHP_ORIG(IehpProp.ORIGIN, nDOF, rProp);
	case D_IEHP_PICK:
		return GetHingeProp_IEHP_PEAK(IehpProp.PEAK, nDOF, rProp);
	case D_IEHP_NBIL:
		return GetHingeProp_IEHP_NBIL(IehpProp.NORBIL, nDOF, rProp);
	case D_IEHP_CLOU:
		return GetHingeProp_IEHP_CLOU(IehpProp.CLOUGH, nDOF, rProp);
	case D_IEHP_DEGR:
		return GetHingeProp_IEHP_DEGR(IehpProp.DEGRAD, nDOF, rProp);
	case D_IEHP_TAKE:
	case D_IEHP_MTAK:
	case D_IEHP_TAKS:
		return GetHingeProp_IEHP_TAKE(IehpProp.TAKEDA, nDOF, rProp);
	case D_IEHP_TTET:
	case D_IEHP_MTTE:
		return GetHingeProp_IEHP_TTET(IehpProp.TAKTET, nDOF, rProp);
	case D_IEHP_SRCT:
		return GetHingeProp_IEHP_SRCT(IehpProp.SRCTET, nDOF, rProp);
	case D_IEHP_EBIL:
		return GetHingeProp_IEHP_EBIL(IehpProp.ELABIL, nDOF, rProp);
	case D_IEHP_ETRI:
		return GetHingeProp_IEHP_ETRI(IehpProp.ELATRI, nDOF, rProp);
	case D_IEHP_ETET:
		return GetHingeProp_IEHP_ETET(IehpProp.ELATET, nDOF, rProp);
	default:
		ASSERT(0);
		return FALSE;
	}
}

BOOL QIehp::GetHingeProp_IEHP_KINE(const T_IEHP_KINEMA& KINE, const int& nDOF, OUT MPhiProp& rProp)
{
	GetHingeParam(nDOF, KINE.dCrackForce, KINE.dCrackMoment, rProp.dStrength_C);
	GetHingeParam(nDOF, KINE.dYieldForce, KINE.dYieldMoment, rProp.dStrength_Y);
	GetHingeParam(nDOF, KINE.dUltimateForce, KINE.dUltimateMoment, rProp.dStrength_U);

	GetHingeParam(nDOF, KINE.dYieldDisp1st, KINE.dYieldRotn1st, rProp.dDisplace_C);
	GetHingeParam(nDOF, KINE.dYieldDisp2nd, KINE.dYieldRotn2nd, rProp.dDisplace_Y);
	GetHingeParam(nDOF, KINE.dYieldDisp3rd, KINE.dYieldRotn3rd, rProp.dDisplace_U);

	return TRUE;
}

BOOL QIehp::GetHingeProp_IEHP_ORIG(const T_IEHP_ORIGIN& ORIG, const int& nDOF, OUT MPhiProp& rProp)
{
	GetHingeParam(nDOF, ORIG.dCrackForce, ORIG.dCrackMoment, rProp.dStrength_C);
	GetHingeParam(nDOF, ORIG.dYieldForce, ORIG.dYieldMoment, rProp.dStrength_Y);
	GetHingeParam(nDOF, ORIG.dUltimateForce, ORIG.dUltimateMoment, rProp.dStrength_U);

	GetHingeParam(nDOF, ORIG.dYieldDisp1st, ORIG.dYieldRotn1st, rProp.dDisplace_C);
	GetHingeParam(nDOF, ORIG.dYieldDisp2nd, ORIG.dYieldRotn2nd, rProp.dDisplace_Y);
	GetHingeParam(nDOF, ORIG.dYieldDisp3rd, ORIG.dYieldRotn3rd, rProp.dDisplace_U);

	return TRUE;
}

BOOL QIehp::GetHingeProp_IEHP_PEAK(const T_IEHP_PEAK& PEAK, const int& nDOF, OUT MPhiProp& rProp)
{
	GetHingeParam(nDOF, PEAK.dCrackForce, PEAK.dCrackMoment, rProp.dStrength_C);
	GetHingeParam(nDOF, PEAK.dYieldForce, PEAK.dYieldMoment, rProp.dStrength_Y);
	GetHingeParam(nDOF, PEAK.dUltimateForce, PEAK.dUltimateMoment, rProp.dStrength_U);

	GetHingeParam(nDOF, PEAK.dYieldDisp1st, PEAK.dYieldRotn1st, rProp.dDisplace_C);
	GetHingeParam(nDOF, PEAK.dYieldDisp2nd, PEAK.dYieldRotn2nd, rProp.dDisplace_Y);
	GetHingeParam(nDOF, PEAK.dYieldDisp3rd, PEAK.dYieldRotn3rd, rProp.dDisplace_U);

	return TRUE;
}

BOOL QIehp::GetHingeProp_IEHP_NBIL(const T_IEHP_NORBIL& NBIL, const int& nDOF, OUT MPhiProp& rProp)
{
	double dNan[2] = { std::nan("--"), std::nan("--") };
	GetHingeParam(nDOF, NBIL.dYieldForce, NBIL.dYieldMoment, rProp.dStrength_C);
	GetHingeParam(nDOF, dNan, dNan, rProp.dStrength_Y);
	GetHingeParam(nDOF, NBIL.dUltimateForce, NBIL.dUltimateMoment, rProp.dStrength_U);

	GetHingeParam(nDOF, NBIL.dYieldDisp2nd, NBIL.dYieldRotn2nd, rProp.dDisplace_C);
	GetHingeParam(nDOF, dNan, dNan, rProp.dDisplace_Y);
	GetHingeParam(nDOF, NBIL.dYieldDisp3rd, NBIL.dYieldRotn3rd, rProp.dDisplace_U);

	return TRUE;
}

BOOL QIehp::GetHingeProp_IEHP_CLOU(const T_IEHP_CLOUGH& CLOU, const int& nDOF, OUT MPhiProp& rProp)
{
	GetHingeParam(nDOF, CLOU.dYieldForce, CLOU.dYieldMoment, rProp.dStrength_Y);
	GetHingeParam(nDOF, CLOU.dUltimateForce, CLOU.dUltimateMoment, rProp.dStrength_U);

	GetHingeParam(nDOF, CLOU.dYieldDisp2nd, CLOU.dYieldRotn2nd, rProp.dDisplace_Y);
	GetHingeParam(nDOF, CLOU.dYieldDisp3rd, CLOU.dYieldRotn3rd, rProp.dDisplace_U);

	return TRUE;
}

BOOL QIehp::GetHingeProp_IEHP_DEGR(const T_IEHP_DEGRAD& DEGR, const int& nDOF, OUT MPhiProp& rProp)
{
	GetHingeParam(nDOF, DEGR.dCrackForce, DEGR.dCrackMoment, rProp.dStrength_C);
	GetHingeParam(nDOF, DEGR.dYieldForce, DEGR.dYieldMoment, rProp.dStrength_Y);
	GetHingeParam(nDOF, DEGR.dUltimateForce, DEGR.dUltimateMoment, rProp.dStrength_U);

	GetHingeParam(nDOF, DEGR.dYieldDisp1st, DEGR.dYieldRotn1st, rProp.dDisplace_C);
	GetHingeParam(nDOF, DEGR.dYieldDisp2nd, DEGR.dYieldRotn2nd, rProp.dDisplace_Y);
	GetHingeParam(nDOF, DEGR.dYieldDisp3rd, DEGR.dYieldRotn3rd, rProp.dDisplace_U);

	return TRUE;
}

BOOL QIehp::GetHingeProp_IEHP_TAKE(const T_IEHP_TAKEDA& TAKE, const int& nDOF, OUT MPhiProp& rProp)
{
	GetHingeParam(nDOF, TAKE.dCrackForce, TAKE.dCrackMoment, rProp.dStrength_C);
	GetHingeParam(nDOF, TAKE.dYieldForce, TAKE.dYieldMoment, rProp.dStrength_Y);
	GetHingeParam(nDOF, TAKE.dUltimateForce, TAKE.dUltimateMoment, rProp.dStrength_U);

	GetHingeParam(nDOF, TAKE.dYieldDisp1st, TAKE.dYieldRotn1st, rProp.dDisplace_C);
	GetHingeParam(nDOF, TAKE.dYieldDisp2nd, TAKE.dYieldRotn2nd, rProp.dDisplace_Y);
	GetHingeParam(nDOF, TAKE.dYieldDisp3rd, TAKE.dYieldRotn3rd, rProp.dDisplace_U);

	return TRUE;
}

BOOL QIehp::GetHingeProp_IEHP_TTET(const T_IEHP_TAKTET& TTET, const int& nDOF, OUT MPhiProp& rProp)
{
	GetHingeParam(nDOF, TTET.dCrackForce, TTET.dCrackMoment, rProp.dStrength_C);
	GetHingeParam(nDOF, TTET.dYieldForce, TTET.dYieldMoment, rProp.dStrength_Y);
	GetHingeParam(nDOF, TTET.dUltimateForce, TTET.dUltimateMoment, rProp.dStrength_U);

	GetHingeParam(nDOF, TTET.dYieldDisp1st, TTET.dYieldRotn1st, rProp.dDisplace_C);
	GetHingeParam(nDOF, TTET.dYieldDisp2nd, TTET.dYieldRotn2nd, rProp.dDisplace_Y);
	GetHingeParam(nDOF, TTET.dYieldDisp3rd, TTET.dYieldRotn3rd, rProp.dDisplace_U);

	return TRUE;
}

BOOL QIehp::GetHingeProp_IEHP_SRCT(const T_IEHP_SRCTET& SRCT, const int& nDOF, OUT MPhiProp& rProp)
{
	GetHingeParam(nDOF, SRCT.dCrackForce, SRCT.dCrackMoment, rProp.dStrength_C);
	GetHingeParam(nDOF, SRCT.dYieldForce, SRCT.dYieldMoment, rProp.dStrength_Y);
	GetHingeParam(nDOF, SRCT.dUltimateForce, SRCT.dUltimateMoment, rProp.dStrength_U);

	GetHingeParam(nDOF, SRCT.dYieldDisp1st, SRCT.dYieldRotn1st, rProp.dDisplace_C);
	GetHingeParam(nDOF, SRCT.dYieldDisp2nd, SRCT.dYieldRotn2nd, rProp.dDisplace_Y);
	GetHingeParam(nDOF, SRCT.dYieldDisp3rd, SRCT.dYieldRotn3rd, rProp.dDisplace_U);

	return TRUE;
}

BOOL QIehp::GetHingeProp_IEHP_EBIL(const T_IEHP_ELABIL& EBIL, const int& nDOF, OUT MPhiProp& rProp)
{
	double dNan[2] = { std::nan("--"), std::nan("--") };
	GetHingeParam(nDOF, EBIL.dYieldForce, EBIL.dYieldMoment, rProp.dStrength_C);
	GetHingeParam(nDOF, dNan, dNan, rProp.dStrength_Y);
	GetHingeParam(nDOF, EBIL.dUltimateForce, EBIL.dUltimateMoment, rProp.dStrength_U);

	GetHingeParam(nDOF, EBIL.dYieldDisp2nd, EBIL.dYieldRotn2nd, rProp.dDisplace_C);
	GetHingeParam(nDOF, dNan, dNan, rProp.dDisplace_Y);
	GetHingeParam(nDOF, EBIL.dYieldDisp3rd, EBIL.dYieldRotn3rd, rProp.dDisplace_U);

	return TRUE;
}

BOOL QIehp::GetHingeProp_IEHP_ETRI(const T_IEHP_ELATRI& ETRI, const int& nDOF, OUT MPhiProp& rProp)
{
	GetHingeParam(nDOF, ETRI.dCrackForce, ETRI.dCrackMoment, rProp.dStrength_C);
	GetHingeParam(nDOF, ETRI.dYieldForce, ETRI.dYieldMoment, rProp.dStrength_Y);
	GetHingeParam(nDOF, ETRI.dUltimateForce, ETRI.dUltimateMoment, rProp.dStrength_U);

	GetHingeParam(nDOF, ETRI.dYieldDisp1st, ETRI.dYieldRotn1st, rProp.dDisplace_C);
	GetHingeParam(nDOF, ETRI.dYieldDisp2nd, ETRI.dYieldRotn2nd, rProp.dDisplace_Y);
	GetHingeParam(nDOF, ETRI.dYieldDisp3rd, ETRI.dYieldRotn3rd, rProp.dDisplace_U);

	return TRUE;
}

BOOL QIehp::GetHingeProp_IEHP_ETET(const T_IEHP_ELATET& ETET, const int& nDOF, OUT MPhiProp& rProp)
{
	GetHingeParam(nDOF, ETET.dCrackForce, ETET.dCrackMoment, rProp.dStrength_C);
	GetHingeParam(nDOF, ETET.dYieldForce, ETET.dYieldMoment, rProp.dStrength_Y);
	GetHingeParam(nDOF, ETET.dUltimateForce, ETET.dUltimateMoment, rProp.dStrength_U);

	GetHingeParam(nDOF, ETET.dYieldDisp1st, ETET.dYieldRotn1st, rProp.dDisplace_C);
	GetHingeParam(nDOF, ETET.dYieldDisp2nd, ETET.dYieldRotn2nd, rProp.dDisplace_Y);
	GetHingeParam(nDOF, ETET.dYieldDisp3rd, ETET.dYieldRotn3rd, rProp.dDisplace_U);

	return TRUE;
}

void QIehp::GetHingeParam(int nDOF, const double adFor[2], const double adMom[2], OUT double adVal[2])
{
	adVal[MPhiProp::kPlus] = 0.0;
	adVal[MPhiProp::kMinus] = 0.0;

	switch (nDOF)
	{
	case 0:
	case 1:
	case 2:
	{
		adVal[MPhiProp::kPlus] = adFor[0];
		adVal[MPhiProp::kMinus] = adFor[1];
	}
	break;
	case 3:
	case 4:
	case 5:
	{
		adVal[MPhiProp::kPlus] = adMom[0];
		adVal[MPhiProp::kMinus] = adMom[1];
	}
	break;
	default:
	{
		ASSERT(0);
	}
	break;
	}
}

#pragma endregion