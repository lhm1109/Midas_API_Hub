// WindSeisAddLoadGrid.h: interface for the CWindSeisAddLoadGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__WINDSEISADDLOADGRID_H__)
#define __WINDSEISADDLOADGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_db\wg_db_DBDoc.h"

class CWindSeisAddLoadGrid : public CTBCommon
{
public:
	CWindSeisAddLoadGrid();
	virtual ~CWindSeisAddLoadGrid();

	// Attributes
public:
	CArray<T_WIND_ADDITION, T_WIND_ADDITION&> m_Data;
	static CString m_aCurDefVal[7];

	// Operations
public:
	void Initialize(CWnd* pParent, BOOL bWind, int nWidth, BOOL bWindKBC2015=FALSE, BOOL bShowRz=TRUE);
	void SetMode(BOOL bWindKBC2015, BOOL bAcross, BOOL bTorsinal);
	void SetTitle();
	BOOL DeleteRecordByKey(const int &key, const T_WIND_ADDITION &data);
	BOOL InsertRecordByKey(const int &key, const T_WIND_ADDITION &data);

	void SetData(CArray<T_WIND_ADDITION,T_WIND_ADDITION&>& aWindAddD)   { m_Data.Copy(aWindAddD); }
	void GetData(CArray<T_WIND_ADDITION,T_WIND_ADDITION&>& aWindAddD)   { OnTerminate(D_TB_SAVE_ALWAYS);  aWindAddD.Copy(m_Data); }
	void SetData(CArray<T_SEIS_ADDITION,T_SEIS_ADDITION&>& aSeidAddD);
	void GetData(CArray<T_SEIS_ADDITION,T_SEIS_ADDITION&>& aSeidAddD);

	void Data2Grid(CArray<T_WIND_ADDITION,T_WIND_ADDITION&>& aData);
	void Grid2Data(CArray<T_WIND_ADDITION,T_WIND_ADDITION&>& aData);

	// Overrides
	virtual int GetKeyColID() { return 0; }
	virtual long GetSelectedKeys(CArray<long, long> &caKey)  { return 0; }
	
	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, int &key, T_WIND_ADDITION &data);
	BOOL ValidateField(CString value, int nColID);
	BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols);
	
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
	
	virtual long GetCountData();
	virtual void MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols);
	virtual void MakeItemNormal(long nNumData, CRowColArray& awCols);
	virtual void MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord);
	
	BOOL GetValue(ROWCOL nRow, int &key, T_WIND_ADDITION &data);
	BOOL SetValue(ROWCOL nRow, const int &key, const T_WIND_ADDITION &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const int &key, const T_WIND_ADDITION &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_WIND_ADDITION& data);
	void ConvDataToStr(int i, const T_WIND_ADDITION& data, CString& value);
	
	void MakeSearchKey(const int &key, const T_WIND_ADDITION &data, CStringArray &aKey);
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);
	
	BOOL FindData(int key, T_WIND_ADDITION& data,  int& nPos, int nStart=0);
	BOOL AddData(int key, T_WIND_ADDITION& data);
	BOOL AddData(CArray<int, int> &aKey, CArray<T_WIND_ADDITION, T_WIND_ADDITION&> &aData, BOOL bNotify=TRUE);
	BOOL DelData(int key);
	BOOL DelData(CArray<int, int> &aKey, BOOL bNotify=TRUE);
	BOOL ModifyData(int oldKey, int key, T_WIND_ADDITION data);
	BOOL ModifyData(CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_WIND_ADDITION, T_WIND_ADDITION&> &aData);
	void DBChanged();
	
	void UpdateBuffer(int nCmd, CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_WIND_ADDITION, T_WIND_ADDITION&> &aData); 

	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL Copy();

	// override 
public:
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);

	void SetStorColumn(ROWCOL nCol);
	// Member Var.
protected:
	int m_nWndWidth;

	CUnitCtrl* m_pUnitCtrl;
	CDBDoc* m_pDoc;
	BOOL m_bWind;
	void* m_pParent;
	BOOL m_bWindKBC2015;
	BOOL m_bShowRz;
};

#endif // !defined(__WINDSEISADDLOADGRID_H__)