#ifndef __DB_DORC_DB_H__
#define __DB_DORC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_DORC
{
public:
	CDB_DORC();
	virtual ~CDB_DORC();
	CDBDoc* m_pDoc;

public:
	void Add(T_DORC_K Key,T_DORC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_DORC_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_DORC_K Key,T_DORC_D& rData);
		//{return m_dorc.Lookup(Key,rData);}
	int GetCount();
		//{return m_dorc.GetCount();}
	POSITION GetStart();
		//{return m_dorc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DORC_K& rKey,T_DORC_D& rData);
		//{m_dorc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DORC_K,T_DORC_K,T_DORC_D,T_DORC_D&>m_dorc;
};

#endif

