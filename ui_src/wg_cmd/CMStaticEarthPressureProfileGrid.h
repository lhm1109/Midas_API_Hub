#if !defined(__CM_STATIC_EARTH_PRESSURE_PROFILE_GRID_H__)
#define __CM_STATIC_EARTH_PRESSURE_PROFILE_GRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThfcItemGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"


/////////////////////////////////////////////////////////////////////////////
// CCMStaticEarthPressureProfileGrid window
#include "HeaderPre.h"
class CCMStaticEarthPressureProfileDlg;
class __MY_EXT_CLASS__ CCMStaticEarthPressureProfileGrid : public CTBCommon
{
	// Construction
public:
	CCMStaticEarthPressureProfileGrid();
	virtual ~CCMStaticEarthPressureProfileGrid();

	void SetDir(int nDir) { m_nDir = nDir; };

	void Initialize(T_EPST_D* pData, CCMStaticEarthPressureProfileDlg* pParent);
	BOOL DeleteRecordByKey(const int &key, const T_EPST_PRESSURE_PROFILE& data);
	BOOL InsertRecordByKey(const int &key, const T_EPST_PRESSURE_PROFILE& data);
	BOOL ModifyRecordByKey(const int &key, const T_EPST_PRESSURE_PROFILE& data);

	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, int &key, T_EPST_PRESSURE_PROFILE& data);
	BOOL ValidateField(CString value, int nColID);
	BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols);

	BOOL GetValue(ROWCOL nRow, T_EPST_PRESSURE_PROFILE& profileItem);
	BOOL SetValue(ROWCOL nRow, const int key, const T_EPST_PRESSURE_PROFILE& profileItem);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const int key, const T_EPST_PRESSURE_PROFILE& profileItem, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_EPST_PRESSURE_PROFILE& data);
	void ConvDataToStr(int i, const T_EPST_PRESSURE_PROFILE& data, CString& value);

	void MakeSearchKey(const int& key, const T_EPST_PRESSURE_PROFILE& data, CStringArray& aKey);
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);

	BOOL FindData(int key, int& nPos, int nStart = 0);
	BOOL AddData(int key, T_EPST_PRESSURE_PROFILE& data);
	BOOL AddData(CArray<int, int> &aKey,
		CArray<T_EPST_PRESSURE_PROFILE, T_EPST_PRESSURE_PROFILE&> &aData);
	BOOL DelData(int key);
	BOOL DelData(CArray<int, int> &aKey);
	BOOL ModifyData(int oldKey, int key, T_EPST_PRESSURE_PROFILE& data);
	BOOL ModifyData(CArray<int, int> &aOldKey, CArray<int, int> &aKey,
		CArray<T_EPST_PRESSURE_PROFILE, T_EPST_PRESSURE_PROFILE&> &aData);
	void DBChanged();
	void UpdateBuffer(int nCmd, CArray<int, int> &aOldKey, CArray<int, int> &aKey,
		CArray<T_EPST_PRESSURE_PROFILE, T_EPST_PRESSURE_PROFILE&> &aData);

	virtual int GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	virtual long GetSelectedKeys(CArray<long, long> &caKey);

	virtual BOOL AddToDB(ROWCOL nRow);
	virtual BOOL AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData);
	virtual BOOL DeleteFromDB(ROWCOL nRow);
	virtual BOOL DeleteFromDB(CRowColArray& awRows);
	virtual BOOL ModifyToDB(ROWCOL nRow);
	virtual BOOL ModifyToDB(CStringArray& raOldKey, CArray<void*, void*>& raKey, CArray<void*, void*>& raData);

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

protected:
	int				m_nDir;
	T_EPST_D* m_pData;
	CString   m_aCurDefVal[4];
	CCMStaticEarthPressureProfileDlg* m_pParent;
protected:
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif // !defined(AFX_THFCITEMGRID_H__672479E1_E839_11D3_92DE_0000C0B0E6B3__INCLUDED_)
