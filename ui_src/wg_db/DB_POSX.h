#ifndef __DB_POSX_DB_H__
#define __DB_POSX_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_POSX
{
public:
	CDB_POSX();
	virtual ~CDB_POSX();
	CDBDoc* m_pDoc;

public:
	void Add(T_POSX_K Key,T_POSX_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_POSX_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_POSX_K Key,T_POSX_D& rData);
		//{return m_posx.Lookup(Key,rData);}
	int GetCount();
		//{return m_posx.GetCount();}
	POSITION GetStart();
		//{return m_posx.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_POSX_K& rKey,T_POSX_D& rData);
		//{m_posx.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_POSX_K,T_POSX_K,T_POSX_D,T_POSX_D&>m_posx;
};

#endif

