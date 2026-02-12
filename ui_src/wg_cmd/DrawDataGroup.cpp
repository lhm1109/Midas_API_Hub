#include "Stdafx.h"
#include "wg_cmd.h"

#include "DrawDataGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void T_ED_OVERLAP_RES_D::Sum(T_ED_OVERLAP_RES_D& pData, int nSumType)
{
	int nSize1 = min(nCount, arKey.GetSize());
	int nSize2 = min(pData.nCount, pData.arKey.GetSize());
	int i;

	CArray<T_ED_OVERLAP_KEY, T_ED_OVERLAP_KEY> arTemp;
	arTemp.SetSize(nSize1 + nSize2);
	for(i = 0; i < nSize1; i++)
	{
		arTemp[i] = arKey[i]; 
	}
	for(i = 0; i < nSize2; i++)
	{
		arTemp[i + nSize1] = pData.arKey[i]; 
	}

	nCount = nSize1 + nSize2;

	arKey.Copy(arTemp);
	
	if(nSumType == 0)
	{
		CenterP.x = nCount==0 ? 0 : (CenterP.x * nSize1 + pData.CenterP.x * nSize2) / nCount;
		CenterP.y = nCount==0 ? 0 : (CenterP.y * nSize1 + pData.CenterP.y * nSize2) / nCount;
	}
	else if(nSumType == 1)
	{
		CenterP.x = CenterP.x + pData.CenterP.x;
		CenterP.y = CenterP.y + pData.CenterP.y;
	}
	else ASSERT(0);
}