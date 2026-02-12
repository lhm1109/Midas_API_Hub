#ifndef __DB_MLTT_DB_H__ 
#define __DB_MLTT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_MLTT
{
public:
	CDB_MLTT();
	virtual ~CDB_MLTT();
	CDBDoc* m_pDoc;

public:
	void Add(T_MLTT_K Key,T_MLTT_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_MLTT_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_MLTT_K Key,T_MLTT_D& rData);
		//{return m_mltt.Lookup(Key,rData);}
	int GetCount();
		//{return m_mltt.GetCount();}
	POSITION GetStart();
		//{return m_mltt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MLTT_K& rKey,T_MLTT_D& rData);
		//{m_mltt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MLTT_KEY,T_MLTT_KEY,T_MLTT_D,T_MLTT_D&>m_mltt;
};

#endif