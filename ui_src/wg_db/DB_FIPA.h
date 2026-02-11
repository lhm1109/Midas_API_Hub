#ifndef __DB_FIPA_DB_H__
#define __DB_FIPA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
class CDB_ELEM;
class CDBDoc;

class CDB_FIPA
{
public:
	CDB_FIPA();
	virtual ~CDB_FIPA();
	CDBDoc* m_pDoc;

public:
	void Add(T_FIPA_K Key,T_FIPA_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_FIPA_K Key,CDB_ELEM* pElem);
	void Initialize();

public:
	BOOL Get(T_FIPA_K Key,T_FIPA_D& rData);
		//{return m_fipa.Lookup(Key,rData);}
	int GetCount();
		//{return m_fipa.GetCount();}
	POSITION GetStart();
		//{return m_fipa.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_FIPA_K& rKey,T_FIPA_D& rData);
		//{m_fipa.GetNextAssoc(rNextPosition,rKey,rData);}
	void AddList(T_FIPA_K Key);
	BOOL DelList(T_FIPA_K Key);
	void DelAllList();
	BOOL GetList(T_FIPA_K Key,FipaList*& rpList){return m_fipalist.Lookup(Key,rpList);}
	void AddListItem(T_FIPA_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_FIPA_K Key, int nCmd, int nKey);
protected:
	CMap<T_FIPA_K,T_FIPA_K,FipaList*,FipaList*>m_fipalist;
	CMap<T_FIPA_K,T_FIPA_K,T_FIPA_D,T_FIPA_D&>m_fipa;
};

#endif
