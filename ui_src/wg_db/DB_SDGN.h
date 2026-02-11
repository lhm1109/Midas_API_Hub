#ifndef __DB_SDGN_DB_H__
#define __DB_SDGN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SDGN
{
public:
	CDB_SDGN();
	virtual ~CDB_SDGN();
	CDBDoc* m_pDoc;

public:
	void Add(T_SDGN_K Key,T_SDGN_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SDGN_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SDGN_K Key,T_SDGN_D& rData);
		//{return m_sdgn.Lookup(Key,rData);}
	int GetCount();
		//{return m_sdgn.GetCount();}
	POSITION GetStart();
		//{return m_sdgn.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SDGN_K& rKey,T_SDGN_D& rData);
		//{m_sdgn.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SDGN_K,T_SDGN_K,T_SDGN_D,T_SDGN_D&>m_sdgn;
};

#endif