#ifndef __DB_STBD_DB_H__
#define __DB_STBD_DB_H__ 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_STBD
{
public:
	CDB_STBD();
	virtual ~CDB_STBD();
	CDBDoc* m_pDoc;

public:
	void Add(T_STBD_K Key,T_STBD_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_STBD_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_STBD_K Key,T_STBD_D& rData);
		//{return m_stbd.Lookup(Key,rData);}
	int GetCount();
		//{return m_stbd.GetCount();}
	POSITION GetStart();
		//{return m_stbd.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_STBD_K& rKey,T_STBD_D& rData);
		//{m_stbd.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_STBD_K,T_STBD_K,T_STBD_D,T_STBD_D&>m_stbd;
};

#endif

