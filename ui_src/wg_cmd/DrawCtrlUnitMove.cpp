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


void CDrawCtrl::MoveDrawUnitR(UINT key, double disX, double disY)
{
	CDrawBase* pDrawClass = GetDrawClass(key);
	if(pDrawClass == NULL) return;
	double dVector[2] = { disX, disY };
	pDrawClass->Move(FALSE, dVector, &m_PaperFormat);
}

void CDrawCtrl::MoveDrawUnitR(nrUINT& arKey, double disX, double disY)
{
	auto no = arKey.GetSize();
	for(auto i = 0; i < no; i++) 
	{
		MoveDrawUnitR(arKey[i], disX, disY);
	}
}

void CDrawCtrl::MoveDrawUnit(UINT key, int disX, int disY)
{
	CDrawBase* pDrawClass = GetDrawClass(key);
	if(pDrawClass == NULL) return;
	double dVector[2] = { (double)disX, (double)disY };
	pDrawClass->Move(TRUE, dVector, &m_PaperFormat);
}

void CDrawCtrl::MoveDrawUnit(nrUINT& arKey, int disX, int disY)
{
	auto no = arKey.GetSize();
	for(auto i = 0; i < no; i++) 
	{
		MoveDrawUnitR(arKey[i], disX, disY);
	}
}
