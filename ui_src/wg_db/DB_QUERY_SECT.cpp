// DB_QUERY_SECT.cpp: implementation of the query class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DB_QUERY_SECT.h"

#include "DBDoc.h"
#include "Qsort.h"
#include "SectDB.h"
#include "DataMemb.h"
#include "UnitCtrl.h"
#include "AttrCtrl.h"
#include "AttrCtrl2.h"

#include "DB_RBAR_SSM.h"
#include "DB_RGSC_SSM.h"
#include "DB_RRSC_SSM.h"
#include "DB_REIN_SSM.h"
#include "DB_SECT_SSM.h"
#include "DB_MPST_SSM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
#pragma region QReinSsm
//////////////////////////////////////////////////////////////////////
QReinSsm::QReinSsm(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pDoc = pDoc;
	m_pDataMemb = pDataMemb;
	m_pReinSsm = pDataMemb->m_pReinSsm;
}

QReinSsm::~QReinSsm()
{
	m_pReinSsm = NULL;
}

int QReinSsm::GetCount()
{
	return m_pReinSsm->GetCount();
}

POSITION QReinSsm::GetStart()
{
	return m_pReinSsm->GetStart();
}

void QReinSsm::GetNext(POSITION& rNextPosition, T_REIN_SSM_K& rKey, T_REIN_SSM_D& rData)
{
	m_pReinSsm->GetNext(rNextPosition, rKey, rData);
	m_pDoc->m_pUnitCtrl->ConvertUnitReinSsmOut(rData);
}

BOOL QReinSsm::Exist(T_REIN_SSM_K Key)
{
	T_REIN_SSM_D Data;
	return m_pReinSsm->Get(Key, Data);
}

void QReinSsm::GetKeyList(CArray<T_REIN_SSM_K, T_REIN_SSM_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_REIN_SSM_K Key;
	T_REIN_SSM_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QReinSsm::Get(T_REIN_SSM_K Key, T_REIN_SSM_D& rData)
{
	BOOL bReturn = m_pReinSsm->Get(Key, rData);
	if (bReturn) m_pDoc->m_pUnitCtrl->ConvertUnitReinSsmOut(rData);
	return bReturn;
}

T_REIN_SSM_K QReinSsm::GetStartNum()
{
	return m_pReinSsm->m_nStartNum;
}

BOOL QReinSsm::GetArea(const T_SECT_K& kSect, const T_REIN_SSM_K& kReinSsm, OUT double& dSectArea, OUT double& dReinArea, char chIJ/*='I'*/)
{
	T_REIN_SSM_D tRein;
	if (!Get(kReinSsm, tRein)) return FALSE;

	T_SECT_D tSect;
	if (!m_pDoc->m_pAttrCtrl->GetSect(kSect, tSect)) return FALSE;
	if (tSect.nStype != D_SECT_TYPE_REGULAR && tSect.nStype != D_SECT_TYPE_TAPERED) return FALSE;

	dSectArea = tSect.SectBefore.SectI.Stiffness.Area;
	if(tSect.nStype == D_SECT_TYPE_TAPERED && chIJ == 'J')
		dSectArea = tSect.SectBefore.SectJ.Stiffness.Area;

	double dTotalArea = 0.0;
	if(!GetTotalArea(kSect, kReinSsm, dTotalArea, chIJ)) return FALSE;

	dReinArea = dTotalArea - dSectArea;
	return TRUE;
}

BOOL QReinSsm::GetTotalArea(const T_SECT_K& kSect, const T_REIN_SSM_K& kReinSsm, OUT double& dTotalArea, char chIJ/*='I'*/)
{
	T_REIN_SSM_D tRein;
	if (!Get(kReinSsm, tRein)) return FALSE;

	T_SECT_D tSect;
	if (!m_pDoc->m_pAttrCtrl->GetSect(kSect, tSect)) return FALSE;

	if (tSect.nStype != D_SECT_TYPE_REGULAR && tSect.nStype != D_SECT_TYPE_TAPERED) return FALSE;
	if (tSect.nStype == D_SECT_TYPE_REGULAR)
		tSect.SectBefore.SectJ = tSect.SectBefore.SectI;
	
	const CString& strShape = tSect.SectBefore.Shape;
	T_SECT_SECTBASE_D* pSectBase = (chIJ == 'I') ? &tSect.SectBefore.SectI : &tSect.SectBefore.SectJ;

	ModifySectSize(strShape , &tRein, pSectBase);

	if (!m_pDoc->m_pSectDB->CalcSectData(tSect)) return FALSE;

	dTotalArea = tSect.SectBefore.SectI.Stiffness.Area;
	if (tSect.nStype == D_SECT_TYPE_TAPERED && chIJ == 'J')
		dTotalArea = tSect.SectBefore.SectJ.Stiffness.Area;

	return TRUE;
}

void QReinSsm::ModifySectSize(const CString& strShape, IN T_REIN_SSM_D* pReinSsm, OUT T_SECT_SECTBASE_D* pSectBase)
{
	if (strShape == D_SECT_SHAPE_REG_SB) {
		pSectBase->Size[0] += pReinSsm->ReinSectRC.LocalZ.dThick * 2;	//H
		pSectBase->Size[1] += pReinSsm->ReinSectRC.LocalY.dThick * 2;	//B
	}
	else if (strShape == D_SECT_SHAPE_REG_B) {
		pSectBase->Size[0] += pReinSsm->ReinSectRC.LocalZ.dThick * 2;	//H
		pSectBase->Size[1] += pReinSsm->ReinSectRC.LocalY.dThick * 2;	//B
		pSectBase->Size[2] += pReinSsm->ReinSectRC.LocalY.dThick;		//tw
		pSectBase->Size[3] += pReinSsm->ReinSectRC.LocalZ.dThick;		//tfTop
		if (dgn::UQ0(pSectBase->Size[5]) == true) {
			pSectBase->Size[5] += pReinSsm->ReinSectRC.LocalZ.dThick;//tfBot
		}
	}
	else if (strShape == D_SECT_SHAPE_REG_SR) {
		pSectBase->Size[0] += pReinSsm->ReinSectRC.LocalY.dThick * 2;	//D
	}
	else if (strShape == D_SECT_SHAPE_REG_P) {
		pSectBase->Size[0] += pReinSsm->ReinSectRC.LocalY.dThick * 2;	//D
		pSectBase->Size[1] += pReinSsm->ReinSectRC.LocalY.dThick;	//tw
	}
	else if (strShape == D_SECT_SHAPE_REG_SOCT) {
		double H = pSectBase->Size[0];
		double B = pSectBase->Size[1];
		double a = pSectBase->Size[2];
		double b = pSectBase->Size[3];
		double d = pReinSsm->ReinSectRC.LocalY.dThick; // 可悸芭府

		const double L = std::hypot(a, b);        // ☆(a + b)

		pSectBase->Size[0] = H + 2.0 * d;	//H
		pSectBase->Size[1] = B + 2.0 * d;	//B
		pSectBase->Size[2] = a + d * (1.0 + a / b - L / b);	//a
		pSectBase->Size[3] = b + d * (1.0 + b / a - L / a);	//b
	}
	else if (strShape == D_SECT_SHAPE_REG_OCT) {
		double H = pSectBase->Size[0];
		double B = pSectBase->Size[1];
		double a = pSectBase->Size[2];
		double b = pSectBase->Size[3];
		double d = pReinSsm->ReinSectRC.LocalY.dThick; // 可悸芭府

		const double L = std::hypot(a, b);        // ☆(a + b)

		pSectBase->Size[0] = H + 2.0 * d;	//H
		pSectBase->Size[1] = B + 2.0 * d;	//B
		pSectBase->Size[2] = a + d * (1.0 + a / b - L / b);	//a
		pSectBase->Size[3] = b + d * (1.0 + b / a - L / a);	//b
		pSectBase->Size[4] += pReinSsm->ReinSectRC.LocalY.dThick;	//t
	}
	else if (strShape == D_SECT_SHAPE_REG_STRK) {
		pSectBase->Size[0] += pReinSsm->ReinSectRC.LocalY.dThick * 2;	//H
		pSectBase->Size[1] += pReinSsm->ReinSectRC.LocalY.dThick * 2;	//B
	}
	else if (strShape == D_SECT_SHAPE_REG_TRK) {
		pSectBase->Size[0] += pReinSsm->ReinSectRC.LocalY.dThick * 2;	//H
		pSectBase->Size[1] += pReinSsm->ReinSectRC.LocalY.dThick * 2;	//B
		pSectBase->Size[2] += pReinSsm->ReinSectRC.LocalY.dThick;	//t

	}
	else if (strShape == D_SECT_SHAPE_REG_HTRK) {
		pSectBase->Size[0] += pReinSsm->ReinSectRC.LocalY.dThick;//H
		pSectBase->Size[1] += pReinSsm->ReinSectRC.LocalY.dThick;//B
	}
	else {
		ASSERT(0);
	}
}

void QReinSsm::GetKeysEqualReinKey(T_REIN_SSM_K kReinSsm, CArray<T_REIN_SSM_K, T_REIN_SSM_K>& aKeyData)
{
	aKeyData.RemoveAll();
	CArray<T_REIN_SSM_K, T_REIN_SSM_K> aKeyList;
	GetKeyList(aKeyList);
	for (int i = 0; i < aKeyList.GetSize(); i++)
	{
		T_REIN_SSM_K key = aKeyList.GetAt(i);
		T_REIN_SSM_D ReinData;
		if (!Get(key, ReinData)) continue;
		if (key != kReinSsm) continue;
		aKeyData.Add(key);
	}
}

#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QSectSsm
//////////////////////////////////////////////////////////////////////
QSectSsm::QSectSsm(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pDoc = pDoc;
	m_pDataMemb = pDataMemb;
	m_pSectSsm = pDataMemb->m_pSectSsm;
}

QSectSsm::~QSectSsm()
{
	m_pDoc = NULL;
	m_pDataMemb = NULL;
	m_pSectSsm = NULL;
}

int QSectSsm::GetCount()
{
	return m_pSectSsm->GetCount();
}

POSITION QSectSsm::GetStart()
{
	return m_pSectSsm->GetStart();
}

void QSectSsm::GetNext(POSITION& rNextPosition, T_SECT_SSM_K& rKey, T_SECT_SSM_D& rData)
{
	m_pSectSsm->GetNext(rNextPosition, rKey, rData);
	m_pDoc->m_pUnitCtrl->ConvertUnitSectSsmOut(rData);
}

BOOL QSectSsm::Exist(T_SECT_SSM_K Key)
{
	T_SECT_SSM_D Data;
	return m_pSectSsm->Get(Key, Data);
}

void QSectSsm::GetKeyList(CArray<T_SECT_SSM_K, T_SECT_SSM_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_SECT_SSM_K Key;
	T_SECT_SSM_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QSectSsm::Get(T_SECT_SSM_K Key, T_SECT_SSM_D& rData)
{
	BOOL bReturn = m_pSectSsm->Get(Key, rData);
	if (bReturn) m_pDoc->m_pUnitCtrl->ConvertUnitSectSsmOut(rData);
	return bReturn;
}

T_SECT_SSM_K QSectSsm::GetStartNum()
{
	return m_pSectSsm->m_nStartNum;
}

void QSectSsm::GetKeysEqualElemKey(T_ELEM_K kElem, CArray< T_SECT_SSM_K, T_SECT_SSM_K> &aKeyData)
{
	aKeyData.RemoveAll();

	CArray<T_SECT_SSM_K, T_SECT_SSM_K> aKeyList;
	GetKeyList(aKeyList);
	for (int i = 0; i < aKeyList.GetSize(); i++)
	{
		T_SECT_SSM_K key = aKeyList.GetAt(i);
		T_SECT_SSM_D SectData;
		if (!Get(key, SectData)) continue;
		if (SectData.kElem != kElem) continue;
		aKeyData.Add(key);
	}
}

void QSectSsm::GetKeysEqualReinSsmKey(T_REIN_SSM_K kReinSsm, CArray< T_SECT_SSM_K, T_SECT_SSM_K>& aKeyData)
{
	CArray< T_SECT_SSM_K, T_SECT_SSM_K> aKeyList;
	GetKeyList(aKeyList);
	for (int i = 0; i < aKeyList.GetSize(); i++)
	{
		T_SECT_SSM_K key = aKeyList.GetAt(i);
		T_SECT_SSM_D SectData;
		if (!Get(key, SectData)) continue;
		if (SectData.kReinSsm != kReinSsm) continue;
		aKeyData.Add(key);
	}
}

#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QRbarSsm
//////////////////////////////////////////////////////////////////////
QRbarSsm::QRbarSsm(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pRbarSsm = pDataMemb->m_pRbarSsm;
}

QRbarSsm::~QRbarSsm()
{
	m_pUnitCtrl = NULL;
	m_pRbarSsm = NULL;
}

// Operations
int QRbarSsm::GetCount()
{
	return m_pRbarSsm->GetCount();
}

POSITION QRbarSsm::GetStart()
{
	return m_pRbarSsm->GetStart();
}

void QRbarSsm::GetNext(POSITION& rNextPosition, T_RBAR_SSM_K& rKey, T_RBAR_SSM_D& rData)
{
	m_pRbarSsm->GetNext(rNextPosition, rKey, rData);
	m_pUnitCtrl->ConvertUnitRbarSsmOut(rData);
}

BOOL QRbarSsm::Exist(T_RBAR_SSM_K Key)
{
	T_RBAR_SSM_D Data;
	return m_pRbarSsm->Get(Key, Data);
}

void QRbarSsm::GetKeyList(CArray<T_RBAR_SSM_K, T_RBAR_SSM_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_RBAR_SSM_K Key;
	T_RBAR_SSM_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QRbarSsm::Get(T_RBAR_SSM_K Key, T_RBAR_SSM_D& rData)
{
	BOOL bReturn = m_pRbarSsm->Get(Key, rData);
	if (bReturn) m_pUnitCtrl->ConvertUnitRbarSsmOut(rData);
	return bReturn;
}

T_RBAR_SSM_K QRbarSsm::GetStartNum()
{
	return m_pRbarSsm->m_nStartNum;
}

void QRbarSsm::GetKeysEqualElemKey(T_ELEM_K kElem, CArray< T_RBAR_SSM_K, T_RBAR_SSM_K> &aKeyData)
{
 	CArray< T_RBAR_SSM_K, T_RBAR_SSM_K> aKeyList;
 	GetKeyList(aKeyList);
 	for (int i = 0; i < aKeyList.GetSize(); i++)
 	{
 		T_RBAR_SSM_K key = aKeyList.GetAt(i);
 		T_RBAR_SSM_D RbarData;
 		if (!Get(key, RbarData)) continue;
 		if (RbarData.kElem != kElem) continue;
 		aKeyData.Add(key);
 	}
}

void QRbarSsm::GetKeysEqualRrscSsmKey(T_RRSC_SSM_K kRrscSsm, CArray< T_RBAR_SSM_K, T_RBAR_SSM_K>& aKeyData)
{
	CArray< T_RBAR_SSM_K, T_RBAR_SSM_K> aKeyList;
	GetKeyList(aKeyList);
	for (int i = 0; i < aKeyList.GetSize(); i++)
	{
		T_RBAR_SSM_K key = aKeyList.GetAt(i);
		T_RBAR_SSM_D RbarData;
		if (!Get(key, RbarData)) continue;
		if (RbarData.kRrscSsm != kRrscSsm) continue;
		aKeyData.Add(key);
	}
}

void QRbarSsm::GetKeysEqualRgscSsmKey(T_RGSC_SSM_K kRgscSsm, CArray< T_RBAR_SSM_K, T_RBAR_SSM_K>& aKeyData)
{
	CArray< T_RBAR_SSM_K, T_RBAR_SSM_K> aKeyList;
	GetKeyList(aKeyList);
	for (int i = 0; i < aKeyList.GetSize(); i++)
	{
		T_RBAR_SSM_K key = aKeyList.GetAt(i);
		T_RBAR_SSM_D RbarData;
		if (!Get(key, RbarData)) continue;
		if (RbarData.kRgscSsm != kRgscSsm) continue;
		aKeyData.Add(key);
	}
}

void QRbarSsm::GetKeysEqualMpstSsmKey(T_MPST_SSM_K kMpstSsm, CArray< T_RBAR_SSM_K, T_RBAR_SSM_K>& aKeyData)
{
	CArray< T_RBAR_SSM_K, T_RBAR_SSM_K> aKeyList;
	GetKeyList(aKeyList);
	for (int i = 0; i < aKeyList.GetSize(); i++)
	{
		T_RBAR_SSM_K key = aKeyList.GetAt(i);
		T_RBAR_SSM_D RbarData;
		if (!Get(key, RbarData)) continue;
		if (RbarData.kMpstSsm != kMpstSsm) continue;
		aKeyData.Add(key);
	}
}
#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QRrscSsm
//////////////////////////////////////////////////////////////////////
QRrscSsm::QRrscSsm(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pDoc = pDoc;
	m_pRrscSsm = pDataMemb->m_pRrscSsm;
	m_pRbarSsm = pDataMemb->m_pRbarSsm;
}

QRrscSsm::~QRrscSsm()
{
	m_pRrscSsm = NULL;
}

// Operations
int QRrscSsm::GetCount()
{
	return m_pRrscSsm->GetCount();
}

POSITION QRrscSsm::GetStart()
{
	return m_pRrscSsm->GetStart();
}

void QRrscSsm::GetNext(POSITION& rNextPosition, T_RRSC_SSM_K& rKey, T_RRSC_SSM_D& rData)
{
	m_pRrscSsm->GetNext(rNextPosition, rKey, rData);
	m_pDoc->m_pUnitCtrl->ConvertUnitRrscSsmOut(rData);
}

BOOL QRrscSsm::Exist(T_RRSC_SSM_K Key)
{
	T_RRSC_SSM_D Data;
	return m_pRrscSsm->Get(Key, Data);
}

void QRrscSsm::GetKeyList(CArray<T_RRSC_SSM_K, T_RRSC_SSM_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_RRSC_SSM_K Key;
	T_RRSC_SSM_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QRrscSsm::Get(T_RRSC_SSM_K Key, T_RRSC_SSM_D& rData)
{
	BOOL bReturn = m_pRrscSsm->Get(Key, rData);
	if (bReturn) m_pDoc->m_pUnitCtrl->ConvertUnitRrscSsmOut(rData);
	return bReturn;
}

T_RRSC_SSM_K QRrscSsm::GetStartNum()
{
	return m_pRrscSsm->m_nStartNum;
}

void QRrscSsm::GetRebarFromElem(const T_ELEM_K& kElem, OUT T_RRSC_SSM_D& rSectRebar, OUT T_RRSC_SSM_D& rReinRebar)
{
	CArray<T_RBAR_SSM_K, T_RBAR_SSM_K> aRbarSsmK;
	m_pDoc->m_pAttrCtrl2->GetQRbarSsm()->GetKeysEqualElemKey(kElem, aRbarSsmK);
	if (aRbarSsmK.GetCount() < 1) return;

	for (int i = 0; i < aRbarSsmK.GetCount(); ++i) {
		const T_RBAR_SSM_K& kRbarSsm = aRbarSsmK[i];

		T_RBAR_SSM_D RbarSsmD;
		if (!m_pDoc->m_pAttrCtrl2->GetQRbarSsm()->Get(kRbarSsm, RbarSsmD)) continue;

		if (RbarSsmD.kRrscSsm == 0) {
			ASSERT(RbarSsmD.nShapeType != T_RBAR_SSM_D::kGeneral);
			continue;
		}

		if (RbarSsmD.nRebarPlacTarget == T_RBAR_SSM_D::kSection) {
			if (!m_pDoc->m_pAttrCtrl2->GetQRrscSsm()->Get(RbarSsmD.kRrscSsm, rSectRebar)) continue;
		}
		else if (RbarSsmD.nRebarPlacTarget == T_RBAR_SSM_D::kReinforce) {
			if (!m_pDoc->m_pAttrCtrl2->GetQRrscSsm()->Get(RbarSsmD.kRrscSsm, rReinRebar)) continue;
		}
	}
}
#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QRgscSsm
//////////////////////////////////////////////////////////////////////
QRgscSsm::QRgscSsm(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pRgscSsm = pDataMemb->m_pRgscSsm;
}

QRgscSsm::~QRgscSsm()
{
	m_pUnitCtrl = NULL;
	m_pRgscSsm = NULL;
}

// Operations
int QRgscSsm::GetCount()
{
	return m_pRgscSsm->GetCount();
}

POSITION QRgscSsm::GetStart()
{
	return m_pRgscSsm->GetStart();
}

void QRgscSsm::GetNext(POSITION& rNextPosition, T_RGSC_SSM_K& rKey, T_RGSC_SSM_D& rData)
{
	m_pRgscSsm->GetNext(rNextPosition, rKey, rData);
	m_pUnitCtrl->ConvertUnitRgscSsmOut(rData);
}

BOOL QRgscSsm::Exist(T_RGSC_SSM_K Key)
{
	T_RGSC_SSM_D Data;
	return m_pRgscSsm->Get(Key, Data);
}

void QRgscSsm::GetKeyList(CArray<T_RGSC_SSM_K, T_RGSC_SSM_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_RGSC_SSM_K Key;
	T_RGSC_SSM_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QRgscSsm::Get(T_RGSC_SSM_K Key, T_RGSC_SSM_D& rData)
{
	BOOL bReturn = m_pRgscSsm->Get(Key, rData);
	if (bReturn) m_pUnitCtrl->ConvertUnitRgscSsmOut(rData);
	return bReturn;
}

T_RGSC_SSM_K QRgscSsm::GetStartNum()
{
	return m_pRgscSsm->m_nStartNum;
}
#pragma endregion

//////////////////////////////////////////////////////////////////////
#pragma region QMpstSsm
//////////////////////////////////////////////////////////////////////
QMpstSsm::QMpstSsm(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pMpstSsm = pDataMemb->m_pMpstSsm;
}

QMpstSsm::~QMpstSsm()
{
	m_pUnitCtrl = NULL;
	m_pMpstSsm = NULL;
}

// Operations
int QMpstSsm::GetCount()
{
	return m_pMpstSsm->GetCount();
}

POSITION QMpstSsm::GetStart()
{
	return m_pMpstSsm->GetStart();
}

void QMpstSsm::GetNext(POSITION& rNextPosition, T_MPST_SSM_K& rKey, T_MPST_SSM_D& rData)
{
	m_pMpstSsm->GetNext(rNextPosition, rKey, rData);
	m_pUnitCtrl->ConvertUnitMpstSsmOut(rData);
}

BOOL QMpstSsm::Exist(T_MPST_SSM_K Key)
{
	T_MPST_SSM_D Data;
	return m_pMpstSsm->Get(Key, Data);
}

void QMpstSsm::GetKeyList(CArray<T_MPST_SSM_K, T_MPST_SSM_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_MPST_SSM_K Key;
	T_MPST_SSM_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QMpstSsm::Get(T_MPST_SSM_K Key, T_MPST_SSM_D& rData)
{
	BOOL bReturn = m_pMpstSsm->Get(Key, rData);
	if (bReturn) m_pUnitCtrl->ConvertUnitMpstSsmOut(rData);
	return bReturn;
}

T_MPST_SSM_K QMpstSsm::GetStartNum()
{
	return m_pMpstSsm->m_nStartNum;
}
#pragma endregion