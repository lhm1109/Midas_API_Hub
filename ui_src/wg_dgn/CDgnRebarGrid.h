// CDgnRebarGrid.h: interface for the CDgnRebarGrid class.
// (060123) sshan Create
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNREBARGRID_H__6666A7C4_9D99_4C2C_B6EC_0A3EB12E7DAF__INCLUDED_)
#define AFX_DGNREBARGRID_H__6666A7C4_9D99_4C2C_B6EC_0A3EB12E7DAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\wg_Common\wg_commonVer.h"
#include "..\wg_Common\wg_common_TBCommon.h"

class CDgnRebarGrid : public CTBCommon
{
public:
	CDgnRebarGrid();
	virtual ~CDgnRebarGrid();
	
	static unsigned int WM_DGN_GRID_CLICKED;
public:
	virtual BOOL AddToDB(ROWCOL nRow);
	virtual BOOL AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData);
	virtual BOOL DeleteFromDB(ROWCOL nRow);
	virtual BOOL DeleteFromDB(CRowColArray& awRows);
	virtual BOOL ModifyToDB(ROWCOL nRow);
	virtual BOOL ModifyToDB(CStringArray& raOldKey, 
									CArray<void*, void*>& raKey, CArray<void*, void*>& raData);

	virtual CString GetNewKey();
	virtual CString GetDefValue(int nIndex);
	virtual CString* GetDefValueArray();
	virtual BOOL MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData);
	virtual void DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData);

	virtual BOOL ValidateField(CString value, int nColID);
	virtual BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols);
	
	virtual long GetCountData();
	virtual void MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols);
	virtual void MakeItemNormal(long nNumData, CRowColArray& awCols);
	virtual void MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord);


};

#endif // !defined(AFX_DGNREBARGRID_H__6666A7C4_9D99_4C2C_B6EC_0A3EB12E7DAF__INCLUDED_)
