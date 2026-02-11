#ifndef __DB_SDLFU_DB_H__
#define __DB_SDLFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SDLFU
{
public:
	CDB_SDLFU()
	{
		m_sdlf.InitHashTable(HASHSIZESDLF);
	}
	virtual ~CDB_SDLFU(){};

public:
	void Add(T_UDRD_KEY Key,T_SDLF_UDRD_D& rData)
		{m_sdlf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sdlf.RemoveKey(Key);}
	void DelAll()
		{m_sdlf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SDLF_UDRD_D& rData)
		{return m_sdlf.Lookup(Key,rData);}
	int GetCount()
		{return m_sdlf.GetCount();}
	POSITION GetStart()
		{return m_sdlf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SDLF_UDRD_D& rData)
		{m_sdlf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SDLF_UDRD_D,T_SDLF_UDRD_D&>m_sdlf;
};

#endif