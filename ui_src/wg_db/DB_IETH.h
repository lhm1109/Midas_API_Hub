#ifndef __DB_IETH_DB_H__
#define __DB_IETH_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_IETH
{
public:
	CDB_IETH();
	virtual ~CDB_IETH();
	CDBDoc* m_pDoc;

public:
	T_IETH_K m_nStartNum;
	T_IETH_K m_nLastNum;

public:
	void Add(T_IETH_K Key,T_IETH_D& rData);
	BOOL Del(T_IETH_K Key);

public:
	BOOL Get(T_IETH_K Key,T_IETH_D& rData);
		//{return m_ieth.Lookup(Key,rData);}
	int GetCount();
		//{return m_ieth.GetCount();}
	POSITION GetStart();
		//{return m_ieth.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_IETH_K& rKey,T_IETH_D& rData);
		//{m_ieth.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_IETH_K Key);
	BOOL DelList(T_IETH_K Key);
	void DelAllList();
	BOOL GetList(T_IETH_K Key,PoldList*& rpList);//{return m_iethlist.Lookup(Key,rpList);}
	int GetCountList();//{return m_iethlist.GetCount();}
	void AddListItem(T_IETH_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_IETH_K Key, int nCmd, int nKey);

protected:
	CMap<T_IETH_K,T_IETH_K,T_IETH_D,T_IETH_D&>m_ieth;
	CMap<T_IETH_K,T_IETH_K,PoldList*,PoldList*>m_iethlist;
};

#endif
