#include "stdafx.h"
#include "DB_PROS.h"
#include "DB_PRIS.h"

#include "DBDoc.h"

CDB_PROS::CDB_PROS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pros.InitHashTable(HASHSIZEPROS);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_PROS::~CDB_PROS()
{

}

void CDB_PROS::Add(T_PROS_K Key, T_PROS_D& rData, CDB_PRIS* pPris)
{
	T_PROS_D Data;
	BOOL bExist = m_pros.Lookup(Key, Data);
	T_PRIS_D PrisData;
	// Image 2, 5, 8
	CArray< T_PROS_ITEM_D, T_PROS_ITEM_D&>* aImgStatic = &Data.aItem_Static_Image;
	CArray< T_PROS_ITEM_D, T_PROS_ITEM_D&>* aImgDynamic = &Data.aItem_Dynamic_Image;
	CArray< T_PROS_ITEM_D, T_PROS_ITEM_D&>* aImgConstep = &Data.aItem_ConsStep_Image;
	std::vector<CArray< T_PROS_ITEM_D, T_PROS_ITEM_D&>*> aImg = { aImgStatic, aImgDynamic, aImgConstep };

	if (bExist)
	{
		if (pPris)
		{
			// Del Image -> PRIS KEY
			for (int imgType = 0; imgType < aImg.size(); imgType++)
			{
				if (aImg[imgType]->GetSize() < 1) continue;
				for (int item = 0; item < aImg[imgType]->GetSize(); item++)
				{
					T_PROS_ITEM_D ItemData = aImg[imgType]->GetAt(item);
					if (ItemData.aPrisK.GetSize() < 1) continue;
					for (int prisK = 0; prisK < ItemData.aPrisK.GetSize(); prisK++)
					{
						if (pPris->Get(ItemData.aPrisK.GetAt(prisK), PrisData))
							pPris->DelListItem(ItemData.aPrisK.GetAt(prisK), LT_PROS_CMD, Key);
					}
				}
			}
		}
	}

	m_pros.SetAt(Key, rData);
	CArray< T_PROS_ITEM_D, T_PROS_ITEM_D&>* arImgStatic = &rData.aItem_Static_Image;
	CArray< T_PROS_ITEM_D, T_PROS_ITEM_D&>* arImgDynamic = &rData.aItem_Dynamic_Image;
	CArray< T_PROS_ITEM_D, T_PROS_ITEM_D&>* arImgConstep = &rData.aItem_ConsStep_Image;
	std::vector<CArray< T_PROS_ITEM_D, T_PROS_ITEM_D&>*> arImg = { arImgStatic, arImgDynamic, arImgConstep };
	
	if (pPris)
	{
		// Add Image -> PRIS KEY
		for (int imgType = 0; imgType < arImg.size(); imgType++)
		{
			if (arImg[imgType]->GetSize() < 1) continue;
			for (int item = 0; item < arImg[imgType]->GetSize(); item++)
			{
				T_PROS_ITEM_D ItemData = arImg[imgType]->GetAt(item);
				if (ItemData.aPrisK.GetSize() < 1) continue;
				for (int prisK = 0; prisK < ItemData.aPrisK.GetSize(); prisK++)
				{
					if (pPris->Get(ItemData.aPrisK.GetAt(prisK), PrisData))
						pPris->AddListItem(ItemData.aPrisK.GetAt(prisK), LT_PROS_CMD, Key);
				}
			}
		}
	}

	if (Key == m_nStartNum)
	{
		T_PROS_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_pros.Lookup(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum)m_nLastNum = Key;
}

BOOL CDB_PROS::Del(T_PROS_K Key, CDB_PRIS* pPris)
{
	T_PROS_D Data;
	BOOL bExist = m_pros.Lookup(Key, Data);
	T_PRIS_D PrisData;
	// Image 2, 5, 8
	CArray< T_PROS_ITEM_D, T_PROS_ITEM_D&>* aImgStatic = &Data.aItem_Static_Image;
	CArray< T_PROS_ITEM_D, T_PROS_ITEM_D&>* aImgDynamic = &Data.aItem_Dynamic_Image;
	CArray< T_PROS_ITEM_D, T_PROS_ITEM_D&>* aImgConstep = &Data.aItem_ConsStep_Image;
	std::vector<CArray< T_PROS_ITEM_D, T_PROS_ITEM_D&>*> aImg = { aImgStatic, aImgDynamic, aImgConstep };

	if (bExist)
	{
		BOOL ret = m_pros.RemoveKey(Key);
		if (ret)
		{
			if (pPris)
			{
				// Del Image -> PRIS KEY
				for (int imgType = 0; imgType < aImg.size(); imgType++)
				{
					if (aImg[imgType]->GetSize() < 1) continue;
					for (int item = 0; item < aImg[imgType]->GetSize(); item++)
					{
						T_PROS_ITEM_D ItemData = aImg[imgType]->GetAt(item);
						if (ItemData.aPrisK.GetSize() < 1) continue;
						for (int prisK = 0; prisK < ItemData.aPrisK.GetSize(); prisK++)
						{
							if (pPris->Get(ItemData.aPrisK.GetAt(prisK), PrisData))
								pPris->DelListItem(ItemData.aPrisK.GetAt(prisK), LT_PROS_CMD, Key);
						}
					}
				}
			}
		}

		if (Key < m_nStartNum)m_nStartNum = Key;
		if (Key == m_nLastNum)
		{
			T_PROS_K key;
			T_PROS_D data;
			if (m_pros.Lookup(m_nLastNum - 1, data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum = 0;
				POSITION pos = m_pros.GetStartPosition();
				while (pos != NULL)
				{
					m_pros.GetNextAssoc(pos, key, data);
					if (key > m_nLastNum)m_nLastNum = key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

BOOL CDB_PROS::Get(T_PROS_K Key, T_PROS_D& rData)
{
	return m_pros.Lookup(Key, rData);
}

int CDB_PROS::GetCount()
{
	return m_pros.GetCount();
}

POSITION CDB_PROS::GetStart()
{
	return m_pros.GetStartPosition();
}

void CDB_PROS::GetNext(POSITION& rNextPosition, T_PROS_K& rKey, T_PROS_D& rData)
{
	m_pros.GetNextAssoc(rNextPosition, rKey, rData);
}