#ifndef __DB_NODE_DB_H__
#define __DB_NODE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

#include "DistSortedList.h"

struct T_KEY_KEY_CMD_K  
{
	T_KEY_KEY_CMD_K() {}
	T_KEY_KEY_CMD_K(const T_KEY_KEY_CMD_K& crSrc) { *this = crSrc; }
	T_KEY_KEY_CMD_K(const T_KEY Key1, const T_KEY Key2, const T_KEY Key3) { Set(Key1, Key2, Key3); }
	T_KEY_KEY_CMD_K& operator=(const T_KEY_KEY_CMD_K& crData) { return Set(crData.Key1, crData.Key2, crData.Key3); }
	operator unsigned long() 
	{ 
		long hk=0; 
		hk |= (0x0000ffff & Key1) << 24;  // 8bit
		hk |= (0x0000ffff & Key2) << 16;  // 8bit
		hk |= (0x0000ffff & Key3);        // 16bit
		return hk;
	}
	BOOL operator == (const T_KEY_KEY_CMD_K& rhs) const 
	{ 
		return (Key1 == rhs.Key1 &&
						Key2 == rhs.Key2 &&
						Key3 == rhs.Key3 );
	}
	T_KEY_KEY_CMD_K& Set(T_KEY Key1, T_KEY Key2, T_KEY Key3) 
	{ 
		this->Key1 = Key1;
		this->Key2 = Key2;
		this->Key3 = Key3;
		return *this; 
	}
	T_KEY Key1;
	T_KEY Key2;
	T_KEY Key3;
};

class CDBDoc;

class CDB_NODE
{
public:
	CDB_NODE();
	CDB_NODE(CDB_NODE& src);
	CDB_NODE& operator=(CDB_NODE& src);
	virtual ~CDB_NODE();
	CDBDoc* m_pDoc;

public:
	T_NODE_K m_nStartNum;
	T_NODE_K m_nLastNum;

public:
	void Add(T_NODE_K Key,T_NODE_D& rData);
	BOOL Del(T_NODE_K Key);

public:
	BOOL GetListFromBaseDB(T_NODE_K Key,NodeList*& rpList){return m_nodelist.Lookup(Key,rpList);}
	BOOL GetFromBaseDB(T_NODE_K Key,T_NODE_D& rData){return m_node.Lookup(Key,rData);}

	BOOL Get(T_NODE_K Key,T_NODE_D& rData);
		//{return m_node.Lookup(Key,rData);}
	int GetCount();
		//{return m_node.GetCount();}
	POSITION GetStart();
		//{return m_node.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_NODE_K& rKey,T_NODE_D& rData);
		//{m_node.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_NODE_K Key);
	BOOL DelList(T_NODE_K Key);
	void DelAllList();
	BOOL GetList(T_NODE_K Key,NodeList*& rpList);//{return m_nodelist.Lookup(Key,rpList);}
	int GetCountList();//{return m_nodelist.GetCount();}
	void AddListItem(T_NODE_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_NODE_K Key, int nCmd, int nKey);
	void AddListItem(CArray<T_NODE_K, T_NODE_K>& aNewNodeK, 
									 CArray<T_NODE_K, T_NODE_K>& aOldNODEK,
									 int nCmd, int nKey, BOOL bExist);
	BOOL DelListItem(CArray<T_NODE_K, T_NODE_K>& aOldNodeK, 
									 int nCmd, int nKey);

	void FindSameDistNodeList(double distance, double tolerance,
								CArray<T_NODE_K, T_NODE_K>& node_list);

protected:
	void InsertSortedList(T_NODE_K Key, double dblDistance);
	double DeleteSortedList(T_NODE_K Key, double dblDistance);
	void ResetSortedList();

protected:
	CMap<T_NODE_K,T_NODE_K,T_NODE_D,T_NODE_D&>m_node;
	CMap<T_NODE_K,T_NODE_K,NodeList*,NodeList*>m_nodelist;

	CDistSortedList m_DistSortedList;

protected:
	//BOOL m_bListChanged;  // 사용안되고 있음.
	CMap<T_KEY_KEY_CMD_K, T_KEY_KEY_CMD_K&, POSITION, POSITION> mListPos;
};

#endif