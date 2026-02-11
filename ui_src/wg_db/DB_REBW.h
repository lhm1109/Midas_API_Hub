#ifndef __DB_REBW_DB_H__
#define __DB_REBW_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STOR;

class CDB_REBW
{
public:
	CDB_REBW();
	CDB_REBW(CDB_REBW& src);
	virtual ~CDB_REBW();
	CDB_REBW& operator=(CDB_REBW& src);
	CDBDoc* m_pDoc;

public:
	void Add(T_REBW_K Key,T_REBW_D& rData,CDB_STOR* pStor);
	BOOL Del(T_REBW_K Key,CDB_STOR* pStor);

public:
	BOOL Get(T_REBW_K Key,T_REBW_D& rData);
		//{return m_rebw.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_rebw.GetCount();}
	POSITION GetStart();
		//{return m_rebw.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_REBW_K& rKey,T_REBW_D& rData);
		//{m_rebw.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

	void AddList(T_REBW_KEY Key);
	BOOL DelList(T_REBW_KEY Key);
	void DelAllList();
	BOOL GetList(T_REBW_K Key,RebwList*& rpList){return m_rebwlist.Lookup(Key.keymap,rpList);}
	int GetCountList(){return m_rebwlist.GetCount();}
	void AddListItem(T_REBW_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_REBW_K Key, int nCmd, int nKey);

protected:
	CMap<T_REBW_KEY,T_REBW_KEY,T_REBW_D,T_REBW_D&>m_rebw;
	CMap<T_REBW_KEY,T_REBW_KEY,RebwList*,RebwList*> m_rebwlist;
};

#endif
