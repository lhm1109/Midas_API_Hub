#ifndef __DB_SPAV_DB_H__
#define __DB_SPAV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_VBEM;

class CDB_SPAV
{
public:
	CDB_SPAV();
	virtual ~CDB_SPAV();
	CDBDoc* m_pDoc;

public:
	T_SPAV_K m_nStartNum;
	T_SPAV_K m_nLastNum;

public:
	void Add(T_SPAV_K Key,T_SPAV_D& rData, CDB_VBEM* pElem);
	BOOL Del(T_SPAV_K Key, CDB_VBEM* pElem);

public:
	BOOL Get(T_SPAV_K Key,T_SPAV_D& rData);
		//{return m_spav.Lookup(Key,rData);}
	int GetCount();
		//{return m_spav.GetCount();}
	POSITION GetStart();
		//{return m_spav.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SPAV_K& rKey,T_SPAV_D& rData);
		//{m_spav.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	void MakeList(BOOL bExist, const T_SPAV_D* pCurSpavD, const T_SPAV_D* pNewSpavD, 
								CArray<UINT, UINT>* paAddList, CArray<UINT, UINT>* paDelList);

protected:
	CMap<T_SPAV_K,T_SPAV_K,T_SPAV_D,T_SPAV_D&>m_spav;
};

#endif

