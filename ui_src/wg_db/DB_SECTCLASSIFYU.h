#ifndef __DB_SECTCLASSIFYU_DB_H__
#define __DB_SECTCLASSIFYU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECTCLASSIFYU
{
public:
	CDB_SECTCLASSIFYU()
	{
		m_sectclassify.InitHashTable(HASHSIZESECTCLASSIFY);
	}
	virtual ~CDB_SECTCLASSIFYU(){};

public:
	void Add(T_UDRD_KEY Key,T_SECTCLASSIFY_UDRD_D& rData)
		{m_sectclassify.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sectclassify.RemoveKey(Key);}
	void DelAll()
		{m_sectclassify.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SECTCLASSIFY_UDRD_D& rData)
		{return m_sectclassify.Lookup(Key,rData);}
	int GetCount()
		{return m_sectclassify.GetCount();}
	POSITION GetStart()
		{return m_sectclassify.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SECTCLASSIFY_UDRD_D& rData)
		{m_sectclassify.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SECTCLASSIFY_UDRD_D,T_SECTCLASSIFY_UDRD_D&>m_sectclassify;
};

#endif