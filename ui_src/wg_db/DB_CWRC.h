#ifndef __DB_CWRC_DB_H__
#define __DB_CWRC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CWRC
{
public:
	CDB_CWRC();
	virtual ~CDB_CWRC();
	CDBDoc* m_pDoc;

public:
	void Add(T_CWRC_K Key,T_CWRC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CWRC_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CWRC_K Key,T_CWRC_D& rData);
		//{return m_cwrc.Lookup(Key,rData);}
	int GetCount();
		//{return m_cwrc.GetCount();}
	POSITION GetStart();
		//{return m_cwrc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CWRC_K& rKey,T_CWRC_D& rData);
		//{m_cwrc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CWRC_K,T_CWRC_K,T_CWRC_D,T_CWRC_D&>m_cwrc;
};

#endif

