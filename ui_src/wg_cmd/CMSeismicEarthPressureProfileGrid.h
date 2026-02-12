#if !defined(__CM_SEISMIC_EARTH_PRESSURE_PROFILE_GRID_H__)
#define __CM_SEISMIC_EARTH_PRESSURE_PROFILE_GRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThfcItemGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"


/////////////////////////////////////////////////////////////////////////////
// CCMSeismicEarthPressureProfileGrid window
#include "HeaderPre.h"
class CCMSeismicEarthPressureProfileDlg;
class __MY_EXT_CLASS__ CCMSeismicEarthPressureProfileGrid : public CTBCommon
{
	// Construction
public:
	CCMSeismicEarthPressureProfileGrid();
	virtual ~CCMSeismicEarthPressureProfileGrid();

	void SetDir(int nDir) { m_nDir = nDir; };

	void Initialize(T_EPSE_D* pData, int nMethodType, CCMSeismicEarthPressureProfileDlg *pParent);
	BOOL DeleteRecordByKey(const double &key, const T_EPSE_PRESSURE_PROFILE& data);
	BOOL InsertRecordByKey(const double &key, const T_EPSE_PRESSURE_PROFILE& data);

	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, double &key, T_EPSE_PRESSURE_PROFILE& data);
	BOOL ValidateField(CString value, int nColID);
	BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols);

	BOOL GetValue(ROWCOL nRow, T_EPSE_PRESSURE_PROFILE& profileItem);
	BOOL SetValue(ROWCOL nRow, const double &key, const T_EPSE_PRESSURE_PROFILE& profileItem);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const double &key, const T_EPSE_PRESSURE_PROFILE& profileItem, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_EPSE_PRESSURE_PROFILE& data);
	void ConvDataToStr(int i, const T_EPSE_PRESSURE_PROFILE& data, CString& value);

	void MakeSearchKey(const double& key, const T_EPSE_PRESSURE_PROFILE& data, CStringArray& aKey);
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);

	BOOL FindData(double key, int& nPos, int nStart = 0);
	BOOL AddData(double key, T_EPSE_PRESSURE_PROFILE& data);
	BOOL AddData(CArray<double, double> &aKey,
		CArray<T_EPSE_PRESSURE_PROFILE, T_EPSE_PRESSURE_PROFILE&> &aData, BOOL bNotify = TRUE);
	BOOL DelData(double key);
	BOOL DelData(CArray<double, double> &aKey, BOOL bNotify = TRUE);
	BOOL ModifyData(double oldKey, double key, T_EPSE_PRESSURE_PROFILE& data);
	BOOL ModifyData(CArray<double, double> &aOldKey, CArray<double, double> &aKey,
		CArray<T_EPSE_PRESSURE_PROFILE, T_EPSE_PRESSURE_PROFILE&> &aData);
	void UpdateBuffer(int nCmd, CArray<double, double> &aOldKey, CArray<double, double> &aKey,
		CArray<T_EPSE_PRESSURE_PROFILE, T_EPSE_PRESSURE_PROFILE&> &aData);

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
	int		  m_nDir;
	int       m_nMethodType;
	T_EPSE_D* m_pData;
	CString   m_aCurDefVal[5];
	CCMSeismicEarthPressureProfileDlg *m_pParent;
protected:
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif // !defined(__CM_SEISMIC_EARTH_PRESSURE_PROFILE_GRID_H__)
