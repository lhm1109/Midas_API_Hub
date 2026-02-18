#if !defined(AFX_RatingLoadCombPSCRCMainGrid_H__5B5AF002_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_RatingLoadCombPSCRCMainGrid_H__5B5AF002_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RatingLoadCombPSCRCMainGrid.h : header file
//

#include "RatingLoadCombCommonMainGrid.h"

class CRatingLoadCombSubGrid;

/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombPSCRCMainGrid window

class CRatingLoadCombPSCRCMainGrid : public CRatingLoadCombCommonMainGrid
{
// Construction
public:
	CRatingLoadCombPSCRCMainGrid();
	virtual ~CRatingLoadCombPSCRCMainGrid();

// Attributes
public:

// Operations
public:  

	BOOL DeleteRecordByKey(const T_LCOM_K &key, const T_RKLC_D &data);
	BOOL InsertRecordByKey(const T_LCOM_K &key, const T_RKLC_D &data);


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRatingLoadCombPSCRCMainGrid)
	//}}AFX_VIRTUAL
	
	virtual int  GetKeyColID() { return 0; }

	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, T_LCOM_K &key, T_RKLC_D &data);    

	virtual BOOL AddToDB(ROWCOL nRow);
	virtual BOOL AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData);
	virtual BOOL DeleteFromDB(ROWCOL nRow);
	virtual BOOL DeleteFromDB(CRowColArray& awRows);
	virtual BOOL ModifyToDB(ROWCOL nRow);
	virtual BOOL ModifyToDB(CStringArray& raOldKey, 
									CArray<void*, void*>& raKey, CArray<void*, void*>& raData);
	
	virtual BOOL MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData);
	virtual void DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData);

	virtual long GetCountData();
	virtual void MakeItemNormal(long nNumData, CRowColArray& awCols);
	virtual void MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord);

	virtual BOOL OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow);

	// Main Grid, Sub Grid간의 동기화  
	virtual void OnChangeCurrentRecord(ROWCOL nRow);
	
	virtual CString GetNewKey();
	
	//virtual BOOL OnAddNewRecord(ROWCOL nRow);

public: 
	

// Implementation
public:
	
	BOOL GetValue(ROWCOL nRow, UINT &key, T_RKLC_D &data);
	BOOL SetValue(ROWCOL nRow, const UINT &key, const T_RKLC_D &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const UINT &key, const T_RKLC_D &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_RKLC_D& data);
	void ConvDataToStr(int i, const T_RKLC_D& data, CString& value);

	

	// Generated message map functions
protected:


	BOOL IsDataChange(T_RKLC_K key, T_RKLC_D& dataNew);

	//{{AFX_MSG(CRatingLoadCombPSCRCMainGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RatingLoadCombPSCRCMainGrid_H__5B5AF002_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)