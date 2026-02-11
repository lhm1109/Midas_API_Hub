#include "Stdafx.h"
#include "wg_cmd.h"

// #include "../NS_lib/IProductInfo.h"
// #include "../NS_lib/IMSGInfo.h"
#include "../wg_db/MathFunc.h"

#include <FLOAT.H>
#include "DrawCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void CDrawCtrl::OverlapChecking_Object(int nOverlapType, T_DIMLEADERLINE_GROUP& OverlapDrawGroup, T_DIM_FORMAT& DimFormat)
{
	OverlapDrawGroup.List.RemoveAll();

	CMap<T_ED_OVERLAP_KEY, T_ED_OVERLAP_KEY, UINT, UINT>    mapOverlap;
	CMap<UINT, UINT, T_ED_OVERLAP_RES_D, T_ED_OVERLAP_RES_D> mapOverlapRes;

	UINT nNewKey = 1;

	UINT Index, Index_Sub;
	POSITION Pos, Pos_Sub;
	int nSize, nSize_Sub, i, j, iTol;
	T_DRAW_FORMAT DrawFormat, DrawFormat_Sub;	

	// Point
	{
		CDrawPoint* pDClass;
		CDrawPoint* pDClass_Sub;
		Index=0;
		Pos = m_arPoint.GetStartPosition();		

		while(Pos)
		{		
			m_arPoint.GetNextAssoc(Pos, Index, pDClass);
			if(!pDClass->m_bIsOverLapChk) continue;
			nSize = pDClass->m_Data.List.GetSize();
			if(!m_arDrawFormat.Lookup(pDClass->m_DrawFormatKey, DrawFormat)) DrawFormat.Init();
			
			// 자신의 객채들의 중첩검토
			for(i = 0; i < nSize-1; i++)
			{
				for(j = i + 1; j < nSize; j++)
				{ 
					if(CDrawBase::IsOverLapDrawUnit_Point(nOverlapType, DrawFormat.Width, pDClass->m_Data.List[i], pDClass->m_Data.List[j]))	
					{
						InsertOverlapKey(mapOverlap, mapOverlapRes, Index, i, Index, j, nNewKey); 
					} 
				}
			}

			// 다른 객채들과의 중첩검토
			Pos_Sub = Pos;  
			Index_Sub = Index;
			while(Pos_Sub)
			{		
				m_arPoint.GetNextAssoc(Pos_Sub, Index_Sub, pDClass_Sub);
				
				if(!pDClass_Sub->m_bIsOverLapChk || Index == Index_Sub) continue;
				
				if(pDClass->GetBoundaryInclude(pDClass_Sub->m_Boundary, FALSE) >= 0)
				{
					nSize_Sub = pDClass_Sub->m_Data.List.GetSize();
					if(!m_arDrawFormat.Lookup(pDClass_Sub->m_DrawFormatKey, DrawFormat_Sub)) DrawFormat_Sub.Init();
					iTol = ((DrawFormat.bLine ? DrawFormat.Width : 0) + (DrawFormat_Sub.bLine ? DrawFormat_Sub.Width : 0)) / 2;

					for(i = 0; i < nSize; i++)
					{
						for(j = 0; j < nSize_Sub; j++)
						{ 
							if(CDrawBase::IsOverLapDrawUnit_Point(nOverlapType, iTol, pDClass->m_Data.List[i], pDClass_Sub->m_Data.List[j]))	
							{
								InsertOverlapKey(mapOverlap, mapOverlapRes, Index, i, Index_Sub, j, nNewKey); 
							} 
						}
					}
				}
			}
		}	
	}	

	// Line
	{
		CDrawLine* pDClass;
		CDrawLine* pDClass_Sub;
		Index = 0;	
		Pos = m_arLine.GetStartPosition();		

		while(Pos)
		{		
			m_arLine.GetNextAssoc(Pos, Index, pDClass);
			
			if(!pDClass->m_bIsOverLapChk) continue;
			nSize = pDClass->m_Data.List.GetSize();
			if(!m_arDrawFormat.Lookup(pDClass->m_DrawFormatKey, DrawFormat)) DrawFormat.Init();
			
			// 자신의 객채들의 중첩검토
			for(i = 0; i < nSize - 1; i++)
			{
				for(j = i + 1; j < nSize ; j++)
				{ 
					if(CDrawBase::IsOverLapDrawUnit_Line(nOverlapType, (DrawFormat.bLine ? DrawFormat.Width : 0), pDClass->m_Data.List[i], pDClass->m_Data.List[j]))	
					{
						InsertOverlapKey(mapOverlap, mapOverlapRes, Index, i, Index, j, nNewKey); 
					} 
				}
			}

			// 다른 객채들과의 중첩검토
			Pos_Sub = Pos; 
			Index_Sub = Index;
			while(Pos_Sub)
			{		
				m_arLine.GetNextAssoc(Pos_Sub, Index_Sub, pDClass_Sub);
				if(!pDClass_Sub->m_bIsOverLapChk || Index == Index_Sub) continue;
				if(pDClass->GetBoundaryInclude(pDClass_Sub->m_Boundary, FALSE) >= 0)
				{
					nSize_Sub = pDClass_Sub->m_Data.List.GetSize();
					if(!m_arDrawFormat.Lookup(pDClass_Sub->m_DrawFormatKey, DrawFormat_Sub)) DrawFormat_Sub.Init();
					iTol = ((DrawFormat.bLine ? DrawFormat.Width : 0) + (DrawFormat_Sub.bLine ? DrawFormat_Sub.Width : 0)) / 2;

					for(i = 0; i < nSize; i++)
					{
						for(j = 0; j < nSize_Sub; j++)
						{ 
							if(CDrawBase::IsOverLapDrawUnit_Line(nOverlapType, iTol, pDClass->m_Data.List[i], pDClass_Sub->m_Data.List[j]))	
							{
								InsertOverlapKey(mapOverlap, mapOverlapRes, Index, i, Index_Sub, j, nNewKey); 
							} 
						}
					}
				}
			}
		}	
	}	

	// ArrowLine
	{
		CDrawArrowLine* pDClass;
		CDrawArrowLine* pDClass_Sub;
		Index = 0;	
		Pos = m_arArrowLine.GetStartPosition();		

		while(Pos)
		{		
			m_arArrowLine.GetNextAssoc(Pos, Index, pDClass);
			if(!pDClass->m_bIsOverLapChk) continue;
			nSize = pDClass->m_Data.List.GetSize();
			if(!m_arDrawFormat.Lookup(pDClass->m_DrawFormatKey, DrawFormat)) DrawFormat.Init();
			
			// 자신의 객채들의 중첩검토
			for(i = 0; i < nSize - 1; i++)
			{
				for(j = i + 1; j < nSize; j++)
				{ 
					if(CDrawBase::IsOverLapDrawUnit_Line(nOverlapType, (DrawFormat.bLine ? DrawFormat.Width : 0), pDClass->m_Data.List[i], pDClass->m_Data.List[j]))	
					{
						InsertOverlapKey(mapOverlap, mapOverlapRes, Index, i, Index, j, nNewKey);
					} 
				}
			}

			// 다른 객채들과의 중첩검토
			Pos_Sub = Pos;  Index_Sub = Index;
			while(Pos_Sub)
			{		
				m_arArrowLine.GetNextAssoc(Pos_Sub, Index_Sub, pDClass_Sub);
				if(!pDClass_Sub->m_bIsOverLapChk || Index == Index_Sub) continue;
				if(pDClass->GetBoundaryInclude(pDClass_Sub->m_Boundary, FALSE) >= 0)
				{
					nSize_Sub = pDClass_Sub->m_Data.List.GetSize();
					if(!m_arDrawFormat.Lookup(pDClass_Sub->m_DrawFormatKey, DrawFormat_Sub)) DrawFormat_Sub.Init();
					iTol = ((DrawFormat.bLine ? DrawFormat.Width : 0) + (DrawFormat_Sub.bLine ? DrawFormat_Sub.Width : 0))/2;

					for(i = 0; i < nSize ; i++)
					{
						for(j = 0; j < nSize_Sub ; j++)
						{ 
							if(CDrawBase::IsOverLapDrawUnit_Line(nOverlapType, iTol, pDClass->m_Data.List[i], pDClass_Sub->m_Data.List[j]))	
							{
								InsertOverlapKey(mapOverlap, mapOverlapRes, Index, i, Index_Sub, j, nNewKey); 
							} 
						}
					}
				}
			}
		}	
	}	

	// Circle
	{
		CDrawCircle* pDClass;
		CDrawCircle* pDClass_Sub;
		Index = 0;	
		Pos = m_arCircle.GetStartPosition();	

		while(Pos)
		{		
			m_arCircle.GetNextAssoc(Pos, Index, pDClass);
			if(!pDClass->m_bIsOverLapChk) continue;
			nSize = pDClass->m_Data.List.GetSize();
			if(!m_arDrawFormat.Lookup(pDClass->m_DrawFormatKey, DrawFormat)) DrawFormat.Init();
			// 자신의 객채들의 중첩검토
			for(i = 0; i < nSize - 1; i++)
			{
				for(j = i + 1; j < nSize; j++)
				{ 
					if(CDrawBase::IsOverLapDrawUnit_Circle(nOverlapType, (DrawFormat.bLine ? DrawFormat.Width : 0), pDClass->m_Data.List[i], pDClass->m_Data.List[j]))
					{
						InsertOverlapKey(mapOverlap, mapOverlapRes, Index, i, Index, j, nNewKey); 
					}
				}
			}

			// 다른 객채들과의 중첩검토
			Pos_Sub = Pos; 
			Index_Sub = Index;
			while(Pos_Sub)
			{		
				m_arCircle.GetNextAssoc(Pos_Sub, Index_Sub, pDClass_Sub);
				if(!pDClass_Sub->m_bIsOverLapChk || Index == Index_Sub) continue;
				if(pDClass->GetBoundaryInclude(pDClass_Sub->m_Boundary, FALSE) >= 0)
				{
					nSize_Sub = pDClass_Sub->m_Data.List.GetSize();
					if(!m_arDrawFormat.Lookup(pDClass_Sub->m_DrawFormatKey, DrawFormat_Sub)) DrawFormat_Sub.Init();
					iTol = ((DrawFormat.bLine ? DrawFormat.Width : 0) + (DrawFormat_Sub.bLine ? DrawFormat_Sub.Width : 0))/2;

					for(i = 0; i < nSize; i++)
					{
						for(j = 0; j < nSize_Sub; j++)
						{ 
							if(CDrawBase::IsOverLapDrawUnit_Circle(nOverlapType, iTol, pDClass->m_Data.List[i], pDClass_Sub->m_Data.List[j]))
							{ 
								InsertOverlapKey(mapOverlap, mapOverlapRes, Index, i, Index_Sub, j, nNewKey); 
							} 
						}
					}
				}
			}
		}	
	}	

	//////////////////////////////////////////////////////////////////////////

	T_ED_OVERLAP_RES_D OverlapResD;
	UINT nKey;

	nSize = (int)mapOverlapRes.GetCount();
	OverlapDrawGroup.List.SetSize(nSize);
	nKey = 0;
	Pos = mapOverlapRes.GetStartPosition();	
	int nCount = 0;
	while(Pos)
	{	
		mapOverlapRes.GetNextAssoc(Pos, nKey, OverlapResD);
		if(OverlapResD.nCount > 1)
		{			
			OverlapDrawGroup.List[nCount].Init();
			OverlapDrawGroup.List[nCount].arPoint.SetSize(3);
			TCHAR strTemp[64];
			wsprintf(strTemp, _T("x%d"), OverlapResD.nCount);
			OverlapDrawGroup.List[nCount].szText = strTemp;
			OverlapDrawGroup.List[nCount].arPoint[0].x = OverlapResD.CenterP.x / OverlapResD.nCount;
			OverlapDrawGroup.List[nCount].arPoint[0].y = OverlapResD.CenterP.y / OverlapResD.nCount;
			OverlapDrawGroup.List[nCount].arPoint[1].x = OverlapDrawGroup.List[nCount].arPoint[0].x + (int)(DimFormat.fontSize * 1.5);
			OverlapDrawGroup.List[nCount].arPoint[1].y = OverlapDrawGroup.List[nCount].arPoint[0].y + (int)(DimFormat.fontSize * 1.5 * (m_DrawFunc.IsEMF() ? 1 : -1));
			OverlapDrawGroup.List[nCount].arPoint[2].x = OverlapDrawGroup.List[nCount].arPoint[1].x + max(2, (int)(DimFormat.fontSize * 0.2));
			OverlapDrawGroup.List[nCount].arPoint[2].y = OverlapDrawGroup.List[nCount].arPoint[1].y;
			nCount++;
		}
		else 
		{
			nrT_LEADERLINE arTemp;
			int no = OverlapDrawGroup.List.GetSize();
			int i;
			for(i = 0; i < no; i++)
			{
				if(i == nCount) continue;
				arTemp.Add(OverlapDrawGroup.List[i]);
			}
			no--; 
			OverlapDrawGroup.List.RemoveAll();
			for(i = 0; i < no; i++)
			{
				OverlapDrawGroup.List.Add(arTemp[i]); 
			}			
		}
	}
}

void CDrawCtrl::InsertOverlapKey(CMap<T_ED_OVERLAP_KEY, T_ED_OVERLAP_KEY,UINT, UINT>& mapOverlap, CMap<UINT, UINT, T_ED_OVERLAP_RES_D, T_ED_OVERLAP_RES_D>& mapOverlapRes, UINT& DrawKey1, int& ListID1, UINT& DrawKey2, int& ListID2, UINT& nNewKey)
{
	T_ED_OVERLAP_K nOverlapKey1, nOverlapKey2;
	UINT nKey, nKey1, nKey2;
	CDrawBase* pDrawClass = NULL;
	POINT CenterP;

	nOverlapKey1.key.entity = DrawKey1;
	nOverlapKey1.key.subID  = (UINT)ListID1;	
	nOverlapKey2.key.entity = DrawKey2;
	nOverlapKey2.key.subID  = (UINT)ListID2;
	BOOL bIsOverlapKey1 = mapOverlap.Lookup(nOverlapKey1.keymap, nKey1); 
	BOOL bIsOverlapKey2 = mapOverlap.Lookup(nOverlapKey2.keymap, nKey2);

	T_ED_OVERLAP_RES_D ResD1;  ResD1.Init();
	T_ED_OVERLAP_RES_D ResD2;  ResD2.Init();
	if(bIsOverlapKey1) bIsOverlapKey1 = mapOverlapRes.Lookup(nKey1, ResD1);
	if(bIsOverlapKey2) bIsOverlapKey2 = mapOverlapRes.Lookup(nKey2, ResD2);

	if(bIsOverlapKey1 && bIsOverlapKey2)
	{
		if(nKey1 != nKey2)
		{
			CArray<T_ED_OVERLAP_KEY, T_ED_OVERLAP_KEY> arRemoveKey;
			if(nKey1 < nKey2)
			{
				ResD1.Sum(ResD2, 1);

				int no = ResD2.arKey.GetSize();
				arRemoveKey.RemoveAll();
				arRemoveKey.SetSize(no);
				for(int i = 0; i < no; i++) 
				{
					arRemoveKey[i] = ResD2.arKey[i];
				}

				mapOverlapRes.SetAt(nKey1, ResD1);
				mapOverlapRes.RemoveKey(nKey2);
				nKey = nKey1;
			}
			else 
			{
				ResD2.Sum(ResD1, 1);

				int no = ResD1.arKey.GetSize();
				arRemoveKey.RemoveAll();
				arRemoveKey.SetSize(no);
				for(int i = 0; i < no; i++) 
				{
					arRemoveKey[i] = ResD1.arKey[i];
				}

				mapOverlapRes.SetAt(nKey2, ResD2);
				mapOverlapRes.RemoveKey(nKey1);
				nKey = nKey2;
			}

			for(auto i = 0; i < arRemoveKey.GetSize(); i++) 
			{
				mapOverlap.SetAt(arRemoveKey[i], nKey);
			}
		}
	}	
	else if(bIsOverlapKey1)
	{			
		pDrawClass = GetDrawClass(nOverlapKey2.key.entity);
		if(pDrawClass != NULL)
		{
			if(pDrawClass->GetCenterPoint(nOverlapKey2.key.subID, CenterP))
			{	
				ResD1.CenterP.x += CenterP.x;  
				ResD1.CenterP.y += CenterP.y; 
				ResD1.arKey.Add(nOverlapKey2.keymap);
				ResD1.nCount++;				
				mapOverlapRes.SetAt(nKey1, ResD1);
				mapOverlap.SetAt(nOverlapKey2.keymap, nKey1);
			}
		}
	}
	else if(bIsOverlapKey2)
	{		
		pDrawClass = GetDrawClass(nOverlapKey1.key.entity);
		if(pDrawClass != NULL)
		{
			if(pDrawClass->GetCenterPoint(nOverlapKey1.key.subID, CenterP))
			{	
				ResD2.CenterP.x += CenterP.x;  
				ResD2.CenterP.y += CenterP.y; 
				ResD2.arKey.Add(nOverlapKey1.keymap);
				ResD2.nCount++;				
				mapOverlapRes.SetAt(nKey2, ResD2);
				mapOverlap.SetAt(nOverlapKey1.keymap, nKey2);
			}
		}
	}
	else 
	{
		nKey = nNewKey;  
		nNewKey++; 
		ResD1.Init();

		pDrawClass = GetDrawClass(nOverlapKey1.key.entity);
		if(pDrawClass != NULL)
		{
			if(pDrawClass->GetCenterPoint(nOverlapKey1.key.subID, CenterP))
			{	
				ResD1.CenterP.x += CenterP.x; 
				ResD1.CenterP.y += CenterP.y; 
				ResD1.arKey.Add(nOverlapKey2.keymap);
				ResD1.nCount++;				
				mapOverlap.SetAt(nOverlapKey1.keymap, nKey);
			}
		}

		pDrawClass = GetDrawClass(nOverlapKey2.key.entity);
		if(pDrawClass != NULL)
		{
			if(pDrawClass->GetCenterPoint(nOverlapKey2.key.subID, CenterP))
			{	
				ResD1.CenterP.x += CenterP.x; 
				ResD1.CenterP.y += CenterP.y; 
				ResD1.arKey.Add(nOverlapKey2.keymap);
				ResD1.nCount++;				
				mapOverlap.SetAt(nOverlapKey2.keymap, nKey);
			}
		}

		mapOverlapRes.SetAt(nKey, ResD1);
	}
}