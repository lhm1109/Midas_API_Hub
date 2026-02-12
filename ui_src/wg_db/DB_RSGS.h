#ifndef __DB_RSGS_DB_H__
#define __DB_RSGS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_GRUP;

class CDB_RSGS
{
public:
	CDB_RSGS();
	virtual ~CDB_RSGS();
	CDBDoc* m_pDoc;

public:
	void Add(T_RSGS_K Key,T_RSGS_D& rData,CDB_GRUP* pGrup);
	BOOL Del(T_RSGS_K Key,CDB_GRUP* pGrup);

public:
	BOOL Get(T_RSGS_K Key,T_RSGS_D& rData);
		//{return m_rsgs.Lookup(Key,rData);}
	int GetCount();
		//{return m_rsgs.GetCount();}
	POSITION GetStart();
		//{return m_rsgs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RSGS_K& rKey,T_RSGS_D& rData);
		//{m_rsgs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RSGS_K,T_RSGS_K,T_RSGS_D,T_RSGS_D&>m_rsgs;
};

#endif

