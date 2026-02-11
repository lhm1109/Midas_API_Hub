#ifndef __DB_SDSW_DB_H__
#define __DB_SDSW_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SDSW
{
public:
	CDB_SDSW();
	virtual ~CDB_SDSW();
	CDBDoc* m_pDoc;

public:
	void Add(T_SDSW_K Key,T_SDSW_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SDSW_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SDSW_K Key,T_SDSW_D& rData);
	 // {return m_sclm.Lookup(Key,rData);}
	int GetCount();
		//{return m_sclm.GetCount();}
	POSITION GetStart();
	 // {return m_sclm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SDSW_K& rKey,T_SDSW_D& rData);
	 // {m_sclm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SDSW_K,T_SDSW_K,T_SDSW_D,T_SDSW_D&>m_sdsw;
};

#endif