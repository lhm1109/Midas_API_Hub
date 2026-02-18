// CRebarGridUpdator.cpp: implementation of the CRebarGridUpdator class.
// (060123) sshan Create
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CRebarGridUpdator.h"
#include "CDgnCreateRebarGrid.h"
#include "CDgnCreateRebarGrid_J.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRebarGridUpdator::CRebarGridUpdator()
{
	this->m_pRebarGrid = NULL;
}

CRebarGridUpdator::~CRebarGridUpdator()
{

}

CRebarGridUpdator * CRebarGridUpdator::Instance()
{
	static CRebarGridUpdator theUpdator;
	return &theUpdator;
}

void CRebarGridUpdator::SetGridWnd(CDgnCreateRebarGrid* pRebarGrid, CDgnCreateRebarGrid_J* pRebarJGrid)
{
	m_pRebarGrid = pRebarGrid;
	m_pRebarJGrid = pRebarJGrid;
}

void CRebarGridUpdator::OnGridSelChanged(CDWordArray& SelectedRows)
{
	
	//--------------------------------------------------------------------------------------
	// 
	/*
	BOOL I_FiberModel::SetRebarSelected(CStringArray& Rebarnames);
	BOOL I_FiberModel::SetRebarSelected(CArray<unsigned int, unsigned int>& arRebarKeys);
	*/

}

