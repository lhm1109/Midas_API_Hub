#ifndef __DB_FACK_DB_H__
#define __DB_FACK_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_FACK
{
public:
	CDB_FACK();
	virtual ~CDB_FACK();
	CDBDoc* m_pDoc;
	
public:
	void Add(T_FACK_K Key,T_FACK_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_FACK_K Key,CDB_ELEM* pElem);
	
public:
	BOOL Get(T_FACK_K Key,T_FACK_D& rData);
	//{return m_FACK.Lookup(Key,rData);}
	int GetCount();
	//{return m_FACK.GetCount();}
	POSITION GetStart();
	//{return m_FACK.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_FACK_K& rKey,T_FACK_D& rData);
	//{m_FACK.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	CMap<T_FACK_K,T_FACK_K,T_FACK_D,T_FACK_D&>m_fack;
};

#endif

