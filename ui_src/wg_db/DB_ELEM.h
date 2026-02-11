#ifndef __DB_ELEM_DB_H__
#define __DB_ELEM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_MATL;
class CDB_SECT;
class CDB_THIK;

class CDB_ELEM
{
public:
	CDB_ELEM();
	CDB_ELEM(CDB_ELEM& src);
	CDB_ELEM& operator=(CDB_ELEM& src);
	virtual ~CDB_ELEM();
	CDBDoc* m_pDoc;

public:
	T_ELEM_K m_nStartNum;
	T_ELEM_K m_nLastNum;

public:
	void Add(T_ELEM_K Key,T_ELEM_D& rData,CDB_NODE* pNode,CDB_MATL* pMatl,CDB_SECT* pSect,CDB_THIK* pThik);
	BOOL Del(T_ELEM_K Key,CDB_NODE* pNode,CDB_MATL* pMatl,CDB_SECT* pSect,CDB_THIK* pThik);

public:
	BOOL GetListFromBaseDB(T_ELEM_K Key,ElemList*& rpList){return m_elemlist.Lookup(Key,rpList);}
	BOOL GetFromBaseDB(T_ELEM_K Key,T_ELEM_D& rData){return m_elem.Lookup(Key,rData);}

	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_D, T_ELEM_D&>::CPair* GetP(T_ELEM_K Key);
	BOOL Get(T_ELEM_K Key,T_ELEM_D& rData);
		//{return m_elem.Lookup(Key,rData);}
	int GetCount(BOOL bWithoutDummyElem=TRUE);
		//{return m_elem.GetCount();}
	int GetCountEltyp(int eltyp);
	//{return m_arEltypeCount[eltyp];}
	POSITION GetStart();
		//{return m_elem.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_ELEM_K& rKey,T_ELEM_D& rData);
		//{m_elem.GetNextAssoc(rNextPosition,rKey,rData);}
	BOOL ExistDummy(T_ELEM_K Key);

	void AddList(T_ELEM_K Key);
	BOOL DelList(T_ELEM_K Key);
	void DelAllList();
	BOOL GetList(T_ELEM_K Key,ElemList*& rpList);//{return m_elemlist.Lookup(Key,rpList);}
	int GetCountList();//{return m_elemlist.GetCount();}
	void AddListItem(T_ELEM_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_ELEM_K Key, int nCmd, int nKey);
	void AddListItem(CArray<T_ELEM_K, T_ELEM_K>& aNewElemK, 
									 CArray<T_ELEM_K, T_ELEM_K>& aOldElemK,
									 int nCmd, int nKey, BOOL bExist);
	BOOL DelListItem(CArray<T_ELEM_K, T_ELEM_K>& aOldElemK, 
									 int nCmd, int nKey);
protected:
	CMap<T_ELEM_K,T_ELEM_K,T_ELEM_D,T_ELEM_D&>m_elem;
	CMap<T_ELEM_K,T_ELEM_K,ElemList*,ElemList*>m_elemlist;
	int m_arEltypeCount[MAX_ELTYP_NUM+1];

	CMap<T_ELEM_K,T_ELEM_K,int,int> m_elem_dummy;   // dummy element (ex. wall opening)


public:
	BOOL IsUsedMatl(T_MATL_K MatlKey);
	POSITION GetStartMatlCount();
	void GetNextMatlCount(POSITION& rNextPosition,T_MATL_K &rMatlKey, int& rNum);
	BOOL IsUsedSect(T_SECT_K SectKey);
	POSITION GetStartSectCount();
	void GetNextSectCount(POSITION& rNextPosition,T_SECT_K &rSectKey, int& rNum);
	BOOL IsUsedThik(T_THIK_K ThikKey);
	POSITION GetStartThikCount();
	void GetNextThikCount(POSITION& rNextPosition,T_THIK_K &rThikKey, int& rNum);

	int GetWallID_LastNumPlusOne();

public:
	int PopAll_DelBuff(CMap<T_ELEM_K,T_ELEM_K,int,int>& rmElem_delBuff);
	void PushElem_DelBuff(const T_ELEM_K ElemK);

protected:
	CMap<T_MATL_K,T_MATL_K,int,int> m_matlCount;
	CMap<T_SECT_K,T_SECT_K,int,int> m_sectCount;
	CMap<T_THIK_K,T_THIK_K,int,int> m_thikCount;
	CMap<int, int, int, int>        m_widCount;

	CMap<T_ELEM_K,T_ELEM_K,int,int> m_elem_delBuff;   // 일괄삭제를 위한 buffer.
};

#endif