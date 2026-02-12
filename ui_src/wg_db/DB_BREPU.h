#ifndef __DB_BREPU_DB_H__
#define __DB_BREPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BREPU
{
public:
	CDB_BREPU()
	{
		m_brep.InitHashTable(HASHSIZEBREP);
		
	}
	virtual ~CDB_BREPU(){};

public:
	void Add(T_UDRD_KEY Key,T_BREP_UDRD_D& rData)
		{m_brep.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_brep.RemoveKey(Key);}
	BOOL Get(T_UDRD_KEY Key,T_BREP_UDRD_D& rData)
		{return m_brep.Lookup(Key,rData);}
	int GetCount()
		{return m_brep.GetCount();}
	POSITION GetStart()
		{return m_brep.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_BREP_UDRD_D& rData)
		{m_brep.GetNextAssoc(rNextPosition,rKey,rData);}

	void DelAll()
		{
			m_brep.RemoveAll();
		}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BREP_UDRD_D,T_BREP_UDRD_D&>m_brep;
	 
};

#endif
