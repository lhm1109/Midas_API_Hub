#ifndef __DB_STFEU_DB_H__
#define __DB_STFEU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STFEU
{
public:
	CDB_STFEU()
	{
		m_stfe.InitHashTable(HASHSIZESTFE);
	}
	virtual ~CDB_STFEU(){};

public:
	void Add(T_UDRD_KEY Key,T_STFE_UDRD_D& rData)
		{m_stfe.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_stfe.RemoveKey(Key);}
	void DelAll()
		{m_stfe.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STFE_UDRD_D& rData)
		{return m_stfe.Lookup(Key,rData);}
	int GetCount()
		{return m_stfe.GetCount();}
	POSITION GetStart()
		{return m_stfe.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STFE_UDRD_D& rData)
		{m_stfe.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STFE_UDRD_D,T_STFE_UDRD_D&>m_stfe;
};

#endif