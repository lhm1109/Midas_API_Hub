#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_WOPN.h"

CDB_WOPN::CDB_WOPN()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_wopn.InitHashTable(HASHSIZEWOPN);
	m_wopn_delBuff.InitHashTable(HASHSIZEWOPN);
}

CDB_WOPN::~CDB_WOPN()
{

}

void CDB_WOPN::Add(T_WOPN_K Key,T_WOPN_D& rData,CDB_ELEM* pElem)
{
	T_WOPN_D Data;
	BOOL bExist=m_wopn.Lookup(Key, Data);

	if(bExist)
	{
		// 참조 값이 변경되었는데, 이전 참조가 '0'이 아니면 참조 연관을 끊어준다.
		for (int i=0; i<D_MAX_WALL_OPEN_SIZE; ++i)
		{
			if (Data.WBeamK_top[i] != rData.WBeamK_top[i] && Data.WBeamK_top[i] != 0)  pElem->DelListItem(Data.WBeamK_top[i], LT_WOPN_CMD, Key);
			if (Data.WBeamK_bot[i] != rData.WBeamK_bot[i] && Data.WBeamK_bot[i] != 0)  pElem->DelListItem(Data.WBeamK_bot[i], LT_WOPN_CMD, Key);
		}
		if (Data.WallK_left != rData.WallK_left && Data.WallK_left != 0) pElem->DelListItem(Data.WallK_left, LT_WOPN_CMD, Key);
		if (Data.WallK_right != rData.WallK_right && Data.WallK_right != 0) pElem->DelListItem(Data.WallK_right, LT_WOPN_CMD, Key);
	}

	m_wopn.SetAt(Key,rData);

	if(bExist)
	{
		// 참조 값이 변경되었는데, 바뀌는 참조가 '0'이 아니면 참조 연관을 맺어준다.
		for (int i=0; i<D_MAX_WALL_OPEN_SIZE; ++i)
		{
			if (rData.WBeamK_top[i] != Data.WBeamK_top[i] && rData.WBeamK_top[i] != 0) pElem->AddListItem(rData.WBeamK_top[i], LT_WOPN_CMD, Key);
			if (rData.WBeamK_bot[i] != Data.WBeamK_bot[i] && rData.WBeamK_bot[i] != 0) pElem->AddListItem(rData.WBeamK_bot[i], LT_WOPN_CMD, Key);
		}
		if (rData.WallK_left != Data.WallK_left && rData.WallK_left != 0) pElem->AddListItem(rData.WallK_left, LT_WOPN_CMD, Key);
		if (rData.WallK_right != Data.WallK_right && rData.WallK_right != 0) pElem->AddListItem(rData.WallK_right, LT_WOPN_CMD, Key);
	}

	if(Key == m_nStartNum)
	{
		T_WOPN_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	if(!bExist)
	{
		for (int i=0; i<D_MAX_WALL_OPEN_SIZE; ++i)
		{
			if (rData.WBeamK_top[i] != 0) pElem->AddListItem(rData.WBeamK_top[i], LT_WOPN_CMD, Key);
			if (rData.WBeamK_bot[i] != 0) pElem->AddListItem(rData.WBeamK_bot[i], LT_WOPN_CMD, Key);
		}
		if (rData.WallK_left != 0) pElem->AddListItem(rData.WallK_left, LT_WOPN_CMD, Key);
		if (rData.WallK_right != 0) pElem->AddListItem(rData.WallK_right, LT_WOPN_CMD, Key);
	}
}

BOOL CDB_WOPN::Del(T_WOPN_K Key,CDB_ELEM* pElem)
{
	T_WOPN_D Data;
	BOOL bExist=m_wopn.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_wopn.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_WOPN_K key;
				T_WOPN_D data;
				if(Get(m_nLastNum-1,data))
				{
					m_nLastNum--;
				}
				else
				{
					m_nLastNum=0;
					POSITION pos=GetStart();
					while(pos != NULL)
					{
						GetNext(pos,key,data);
						if(key > m_nLastNum)m_nLastNum=key;
					}
				}
			}
		}

		if(ret)
		{
			for (int i=0; i<D_MAX_WALL_OPEN_SIZE; ++i)
			{
				if (Data.WBeamK_top[i] != 0) VERIFY(pElem->DelListItem(Data.WBeamK_top[i], LT_WOPN_CMD, Key));
				if (Data.WBeamK_bot[i] != 0) VERIFY(pElem->DelListItem(Data.WBeamK_bot[i], LT_WOPN_CMD, Key));
			}
			if (Data.WallK_left != 0) VERIFY(pElem->DelListItem(Data.WallK_left, LT_WOPN_CMD, Key));
			if (Data.WallK_right != 0) VERIFY(pElem->DelListItem(Data.WallK_right, LT_WOPN_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_WOPN::Get(T_WOPN_K Key,T_WOPN_D& rData)
{
	return m_wopn.Lookup(Key,rData);
}

int CDB_WOPN::GetCount()
{
	return m_wopn.GetCount();
}

POSITION CDB_WOPN::GetStart()
{
	return m_wopn.GetStartPosition();
}

void CDB_WOPN::GetNext(POSITION& rNextPosition,T_WOPN_K& rKey,T_WOPN_D& rData)
{
	m_wopn.GetNextAssoc(rNextPosition,rKey,rData);
}

int CDB_WOPN::PopAll_DelBuff(CMap<T_WOPN_K,T_WOPN_K,int,int>& rmWopn_delBuff)
{
	rmWopn_delBuff.RemoveAll();
	if (m_wopn_delBuff.GetCount() == 0)  return 0;

	rmWopn_delBuff.InitHashTable(m_wopn_delBuff.GetHashTableSize());

	T_WOPN_K WopnK = 0;
	int nTemp = 0;
	POSITION pos = m_wopn_delBuff.GetStartPosition();
	while(pos)
	{
		m_wopn_delBuff.GetNextAssoc(pos, WopnK, nTemp);
		rmWopn_delBuff.SetAt(WopnK, nTemp);
	}

	// m_wopn_delBuff 비우기.
	m_wopn_delBuff.RemoveAll();

	return rmWopn_delBuff.GetCount();
}

void CDB_WOPN::PushWopn_DelBuff(const T_WOPN_K WopnK, BOOL bByWallOpen)
{
	int nRefCount = 0;
	if (!m_wopn_delBuff.Lookup(WopnK, nRefCount)) nRefCount = 0;

	// 개구부 주변 부재 삭제로 인해 Del Buffer로 등록될 때는 참조 count를 올려준다.
	// 개구부만 삭제되는지, 개구부 주변부재와 함께 삭제되는지 구별하기 위한 용도
	if (!bByWallOpen)  nRefCount++;

	m_wopn_delBuff.SetAt(WopnK, nRefCount);
}
