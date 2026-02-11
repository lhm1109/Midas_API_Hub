#ifndef __DB_CREF_DB_H__
#define __DB_CREF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_ELEM;
class CDB_ELNK;
class CDB_NLNK;

class CDB_CREF
{
public:
	CDB_CREF();
	virtual ~CDB_CREF();
	CDBDoc* m_pDoc;

public:
	T_CREF_K m_nStartNum;
	T_CREF_K m_nLastNum;

public:
	void Add(T_CREF_K Key,T_CREF_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem,CDB_ELNK* pElnk,CDB_NLNK* pNlnk);
	BOOL Del(T_CREF_K Key,CDB_NODE* pNode,CDB_ELEM* pElem,CDB_ELNK* pElnk,CDB_NLNK* pNlnk);

public:
	BOOL Get(T_CREF_K Key,T_CREF_D& rData);
		//{return m_cref.Lookup(Key,rData);}
	int GetCount();
		//{return m_cref.GetCount();}
	POSITION GetStart();
		//{return m_cref.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CREF_K& rKey,T_CREF_D& rData);
		//{m_cref.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	void MakeList(BOOL bExist, const T_CREF_D* pCurCrefD, const T_CREF_D* pNewCrefD, 
								CArray<UINT, UINT>* paAddList, CArray<UINT, UINT>* paDelList);

protected:
	CMap<T_CREF_K,T_CREF_K,T_CREF_D,T_CREF_D&>m_cref;
};

#endif

