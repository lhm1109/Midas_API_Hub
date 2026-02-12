#ifndef __DB_REBB_DB_H__
#define __DB_REBB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SECT;

class CDB_REBB
{
public:
	CDB_REBB();
	CDB_REBB(CDB_REBB& src);
	virtual ~CDB_REBB();
	CDB_REBB& operator=(CDB_REBB& src);
	CDBDoc* m_pDoc;

public:
	void Add(T_REBB_K Key,T_REBB_D& rData,CDB_SECT* pSect);
	BOOL Del(T_REBB_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_REBB_K Key,T_REBB_D& rData);
		//{return m_rebb.Lookup(Key,rData);}
	int GetCount();
		//{return m_rebb.GetCount();}
	POSITION GetStart();
		//{return m_rebb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_REBB_K& rKey,T_REBB_D& rData);
		//{m_rebb.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_REBB_KEY Key);
	BOOL DelList(T_REBB_KEY Key);
	void DelAllList();
	BOOL GetList(T_REBB_K Key,RebbList*& rpList){return m_rebblist.Lookup(Key.keymap,rpList);}
	int GetCountList(){return m_rebblist.GetCount();}
	void AddListItem(T_REBB_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_REBB_K Key, int nCmd, int nKey);


protected:
	CMap<T_REBB_KEY,T_REBB_KEY,T_REBB_D,T_REBB_D&> m_rebb;
	CMap<T_REBB_KEY,T_REBB_KEY,RebbList*,RebbList*> m_rebblist;
};

#endif

