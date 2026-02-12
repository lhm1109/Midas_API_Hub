#ifndef __DB_CSOS_DB_H__
#define __DB_CSOS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CSOS
{
public:
	CDB_CSOS();
	virtual ~CDB_CSOS();
	CDBDoc* m_pDoc;

public:
	void Add(T_CSOS_K Key,T_CSOS_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CSOS_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CSOS_K Key,T_CSOS_D& rData);
		//{return m_leng.Lookup(Key,rData);}
	int GetCount();
		//{return m_leng.GetCount();}
	POSITION GetStart();
		//{return m_leng.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CSOS_K& rKey,T_CSOS_D& rData);
		//{m_leng.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CSOS_K,T_CSOS_K,T_CSOS_D,T_CSOS_D&>m_csos;
};

#endif

