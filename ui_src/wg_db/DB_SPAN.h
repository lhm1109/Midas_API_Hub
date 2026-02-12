#ifndef __DB_SPAN_DB_H__
#define __DB_SPAN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SPAN
{
public:
	CDB_SPAN();
	virtual ~CDB_SPAN();
	CDBDoc* m_pDoc;

public:
	T_SPAN_K m_nStartNum;
	T_SPAN_K m_nLastNum;

public:
	void Add(T_SPAN_K Key,T_SPAN_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SPAN_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SPAN_K Key,T_SPAN_D& rData);
		//{return m_span.Lookup(Key,rData);}
	int GetCount();
		//{return m_span.GetCount();}
	POSITION GetStart();
		//{return m_span.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SPAN_K& rKey,T_SPAN_D& rData);
		//{m_span.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	void MakeList(BOOL bExist, const T_SPAN_D* pCurSpanD, const T_SPAN_D* pNewSpanD, 
								CArray<UINT, UINT>* paAddList, CArray<UINT, UINT>* paDelList);

protected:
	CMap<T_SPAN_K,T_SPAN_K,T_SPAN_D,T_SPAN_D&>m_span;
};

#endif

