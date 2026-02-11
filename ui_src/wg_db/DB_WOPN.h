#ifndef __DB_WOPN_DB_H__
#define __DB_WOPN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;

class CDB_WOPN
{
public:
	CDB_WOPN();
	virtual ~CDB_WOPN();

public:
	T_WOPN_K m_nStartNum;
	T_WOPN_K m_nLastNum;

public:
	void Add(T_WOPN_K Key,T_WOPN_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_WOPN_K Key,CDB_ELEM* pElem);

public:
	BOOL GetFromBaseDB(T_WOPN_K Key,T_WOPN_D& rData)    {return m_wopn.Lookup(Key,rData);}

	BOOL Get(T_WOPN_K Key,T_WOPN_D& rData);
		//{return m_wopn.Lookup(Key,rData);}
	int GetCount();
		//{return m_wopn.GetCount();}
	POSITION GetStart();
		//{return m_wopn.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_WOPN_K& rKey,T_WOPN_D& rData);
		//{m_wopn.GetNextAssoc(rNextPosition,rKey,rData);}

public:
	int PopAll_DelBuff(CMap<T_WOPN_K,T_WOPN_K,int,int>& rmWopn_delBuff);
	void PushWopn_DelBuff(const T_WOPN_K WopnK, BOOL bByWallOpen);

protected:
	CMap<T_WOPN_K,T_WOPN_K,T_WOPN_D,T_WOPN_D&>m_wopn;
	CMap<T_WOPN_K,T_WOPN_K,int,int> m_wopn_delBuff;   // 일괄삭제를 위한 buffer.
};

#endif