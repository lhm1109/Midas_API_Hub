#ifndef __DB_ELUF_DB_H__
#define __DB_ELUF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_ELUF
{
public:
	CDB_ELUF();
	virtual ~CDB_ELUF();
	CDBDoc* m_pDoc;

public:
	void Add(T_ELUF_K Key,T_ELUF_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_ELUF_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_ELUF_K Key,T_ELUF_D& rData);
		//{return m_eluf.Lookup(Key,rData);}
	int GetCount();
		//{return m_eluf.GetCount();}
	POSITION GetStart();
		//{return m_eluf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_ELUF_K& rKey,T_ELUF_D& rData);
		//{m_eluf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_ELUF_K,T_ELUF_K,T_ELUF_D,T_ELUF_D&>m_eluf;
};

#endif

