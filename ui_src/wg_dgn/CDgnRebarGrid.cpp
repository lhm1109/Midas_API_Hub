// CDgnRebarGrid.cpp: implementation of the CDgnRebarGrid class.
// (060123) sshan Create
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CDgnRebarGrid.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

unsigned int CDgnRebarGrid::WM_DGN_GRID_CLICKED = ::RegisterWindowMessage(_T("WM_DGN_GRID_CLICKED"));
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnRebarGrid::CDgnRebarGrid()
{
/*
	AddColInfo(_T("x"), 0, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_T("y"), 0, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_T("z"), 0, D_UNITSYS_BASE_LENGTH);
	*/
}

CDgnRebarGrid::~CDgnRebarGrid()
{

}



BOOL CDgnRebarGrid::AddToDB(ROWCOL nRow)
{
	return FALSE;
}
BOOL CDgnRebarGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	return FALSE;
}

BOOL CDgnRebarGrid::DeleteFromDB(ROWCOL nRow)
{
	return FALSE;
}


BOOL CDgnRebarGrid::DeleteFromDB(CRowColArray& awRows)
{
	return FALSE;
}


BOOL CDgnRebarGrid::ModifyToDB(ROWCOL nRow)
{
	return FALSE;
}

BOOL CDgnRebarGrid::ModifyToDB(CStringArray& raOldKey, 
				CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	return FALSE;
}


CString CDgnRebarGrid::GetNewKey()
{
	CString strTemp;
	return strTemp;
}


CString CDgnRebarGrid::GetDefValue(int nIndex)
{
	CString strTemp;
	return strTemp;
}

CString* CDgnRebarGrid::GetDefValueArray()
{
	static CString strTemp;
	return &strTemp;
}

BOOL CDgnRebarGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
												CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	return FALSE;
}

void CDgnRebarGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{

}


BOOL CDgnRebarGrid::ValidateField(CString value, int nColID)
{
	return FALSE;
}

BOOL CDgnRebarGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	return FALSE;
}

long CDgnRebarGrid::GetCountData()
{
	return 0L;
}

void CDgnRebarGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{

}

void CDgnRebarGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{

}

void CDgnRebarGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{

}

