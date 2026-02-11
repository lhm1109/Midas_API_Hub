#if !defined(__CLOADSTEPGRID_H__)
#define __CLOADSTEPGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CNXNlctLoadStepGrid : public CTBCommon
{
public:
	CNXNlctLoadStepGrid();
	virtual ~CNXNlctLoadStepGrid();
public:
	T_NLCT_DATA* m_pData;
	static CString m_aCurDefVal[3];
	
	//Override
public:
	void SetRowCountUser(int nRowCount);
	void SetDefaultValue(ROWCOL nFromRow, ROWCOL nToRow);
	void GenerateValue(double dValue);

	BOOL DeleteRecordByKey(const int &key, const T_NLCT_LOADSTEP &data);
	BOOL InsertRecordByKey(const int &key, const T_NLCT_LOADSTEP &data);

	virtual int GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	virtual long GetSelectedKeys(CArray<long, long> &caKey);

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

	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL Copy();

	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);

protected:
	int m_nRowCount;
	int m_nOldRowCount;

public:
	void Initialize(T_NLCT_DATA* pData);

	BOOL GetValue(ROWCOL nRow, int &key, T_NLCT_LOADSTEP &data);
	BOOL SetValue(ROWCOL nRow, const int &key, const T_NLCT_LOADSTEP &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const int &key, const T_NLCT_LOADSTEP &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_NLCT_LOADSTEP& data);
	void ConvDataToStr(int i, const T_NLCT_LOADSTEP& data, CString& value);

	BOOL AddData(int key, T_NLCT_LOADSTEP data);
	BOOL AddData(CArray<int, int> &aKey, CArray<T_NLCT_LOADSTEP, T_NLCT_LOADSTEP&> &aData, BOOL bNotify = TRUE);
	BOOL DelData(int key);
	BOOL DelData(CArray<int, int> &aKey, BOOL bNotify = TRUE);
	BOOL ModifyData(int oldKey, int key, T_NLCT_LOADSTEP data);
	BOOL ModifyData(CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_NLCT_LOADSTEP, T_NLCT_LOADSTEP&> &aData);
	void MakeSearchKey(const int &key, const T_NLCT_LOADSTEP &data, CStringArray &aKey);
	BOOL ExistAllKey(CArray<int, int>& aKey, int& errorKey);

	BOOL FindData(int key, int& nPos, int nStart=0);

	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);

	BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols);
	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, int &key, T_NLCT_LOADSTEP &data);
	BOOL ValidateField(CString value, int nColID);

	void UpdateBuffer(int nCmd, CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_NLCT_LOADSTEP, T_NLCT_LOADSTEP&> &aData);
	void DBChanged();
protected:
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

#endif