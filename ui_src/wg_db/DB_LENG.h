#ifndef __DB_LENG_DB_H__
#define __DB_LENG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_LENG
{
public:
	CDB_LENG();
	virtual ~CDB_LENG();
	CDBDoc* m_pDoc;

public:
	void Add(T_LENG_K Key,T_LENG_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_LENG_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_LENG_K Key,T_LENG_D& rData);
		//{return m_leng.Lookup(Key,rData);}
	int GetCount();
		//{return m_leng.GetCount();}
	POSITION GetStart();
		//{return m_leng.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LENG_K& rKey,T_LENG_D& rData);
		//{m_leng.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_LENG_K,T_LENG_K,T_LENG_D,T_LENG_D&>m_leng;
};

#endif

