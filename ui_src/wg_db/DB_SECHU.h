#ifndef __DB_SECHU_DB_H__
#define __DB_SECHU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_SECHU
{
public:
	CDB_SECHU()
	{
		m_SECH.InitHashTable(HASHSIZESECH);
	}
	virtual ~CDB_SECHU(){};

public:
	void Add(T_UDRD_KEY Key,T_SECH_UDRD_D& rData)
	{m_SECH.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_SECH.RemoveKey(Key);}
	void DelAll()
	{m_SECH.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SECH_UDRD_D& rData)
	{return m_SECH.Lookup(Key,rData);}
	int GetCount()
	{return m_SECH.GetCount();}
	POSITION GetStart()
	{return m_SECH.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SECH_UDRD_D& rData)
	{m_SECH.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_SECH_UDRD_D,T_SECH_UDRD_D&>m_SECH;
};

#endif
