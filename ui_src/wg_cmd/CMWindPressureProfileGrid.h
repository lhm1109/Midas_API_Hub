#if !defined(__CM_WIND_PRESSURE_PROFILE_GRID_H__)
#define __CM_WIND_PRESSURE_PROFILE_GRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThfcItemGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"


/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureProfileGrid window
#include "HeaderPre.h"
class CCMWindPressureProfileDlg;
class CCMWindPressureProfileKBC2016Dlg;
class __MY_EXT_CLASS__ CCMWindPressureProfileGrid : public CTBCommon
{
// Construction
public:
	CCMWindPressureProfileGrid();
	virtual ~CCMWindPressureProfileGrid();

	void SetWindDir(int nDir) { m_nDir = nDir; };
	
	void Initialize(T_WDPR_D* pData, CCMWindPressureProfileDlg* pParent);
	void Initialize(T_WDPR_D* pData, CCMWindPressureProfileKBC2016Dlg* pParent);
	void Initialize_sub(T_WDPR_D * pData);

	BOOL DeleteRecordByKey(const double &key, const T_WIND_PRESSURE_PROFILE& data);
	BOOL InsertRecordByKey(const double &key, const T_WIND_PRESSURE_PROFILE& data);
	
	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, double &key, T_WIND_PRESSURE_PROFILE& data);
	BOOL ValidateField(CString value, int nColID);
	BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols);

	void SetShowThick(BOOL bShow, BOOL bRemake = FALSE) { m_bShowThick = bShow; if(bRemake) MakeItemEx(); }
	BOOL GetShowThick()                                 { return m_bShowThick; }

	BOOL GetValue(ROWCOL nRow, T_WIND_PRESSURE_PROFILE& profileItem);
	BOOL SetValue(ROWCOL nRow, const double &key, const T_WIND_PRESSURE_PROFILE& profileItem);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const double &key, const T_WIND_PRESSURE_PROFILE& profileItem, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_WIND_PRESSURE_PROFILE& data);
	void ConvDataToStr(int i, const T_WIND_PRESSURE_PROFILE& data, CString& value);
	
	void MakeSearchKey(const double& key, const T_WIND_PRESSURE_PROFILE& data, CStringArray& aKey);
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);
	
	BOOL FindData(double key, int& nPos, int nStart=0);
	BOOL ExistAllKey(CArray<double, double>& aKey, double& errorKey);
	BOOL AddData(double key, T_WIND_PRESSURE_PROFILE& data);
	BOOL AddData(CArray<double, double> &aKey,
		CArray<T_WIND_PRESSURE_PROFILE, T_WIND_PRESSURE_PROFILE&> &aData, BOOL bNotify=TRUE);
	BOOL DelData(double key);
	BOOL DelData(CArray<double, double> &aKey, BOOL bNotify=TRUE);
	BOOL ModifyData(double oldKey, double key, T_WIND_PRESSURE_PROFILE& data);
	BOOL ModifyData(CArray<double, double> &aOldKey, CArray<double, double> &aKey,
		CArray<T_WIND_PRESSURE_PROFILE, T_WIND_PRESSURE_PROFILE&> &aData);
	void DBChanged();
	void UpdateBuffer(int nCmd, CArray<double, double> &aOldKey, CArray<double, double> &aKey,
		CArray<T_WIND_PRESSURE_PROFILE, T_WIND_PRESSURE_PROFILE&> &aData);
	
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
	BOOL      m_bShowThick;
	T_WDPR_D* m_pData;
	CString   m_aCurDefVal[6];
	CCMWindPressureProfileDlg* m_pParent;
	CCMWindPressureProfileKBC2016Dlg* m_pParentKBC2016;
protected:
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif // !defined(AFX_THFCITEMGRID_H__672479E1_E839_11D3_92DE_0000C0B0E6B3__INCLUDED_)
