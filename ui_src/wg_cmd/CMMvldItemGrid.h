#if !defined(MVLD_ITEM_GRID_H)
#define MVLD_ITEM_GRID_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"


/////////////////////////////////////////////////////////////////////////////
// CCMvldItemJPGrid window
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMvldItemJPGrid : public CTBCommon
{
// Construction
public:
	CCMvldItemJPGrid();

// Attributes
public:
	T_MVLDjp_D* m_pData;
	static CString m_aCurDefVal[];

// Operations
public:
	void Initialize(T_MVLDjp_D* pData);

	BOOL DeleteRecordByKey(const double &key, const T_MVLDjp_MLOAD_BASE &data);
	BOOL InsertRecordByKey(const double &key, const T_MVLDjp_MLOAD_BASE &data);

	virtual int GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	virtual long GetSelectedKeys(CArray<long, long> &caKey);

	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, double &key, T_MVLDjp_MLOAD_BASE &data);
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

	BOOL GetValue(ROWCOL nRow, double &key, T_MVLDjp_MLOAD_BASE &data);
	BOOL SetValue(ROWCOL nRow, const double &key, const T_MVLDjp_MLOAD_BASE &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const double &key, const T_MVLDjp_MLOAD_BASE &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_MVLDjp_MLOAD_BASE& data);
	void ConvDataToStr(int i, const T_MVLDjp_MLOAD_BASE& data, CString& value);

	void MakeSearchKey(const double &key, const T_MVLDjp_MLOAD_BASE &data, CStringArray &aKey);
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);

	BOOL FindData(double key, int& nPos, int nStart=0);
	BOOL ExistAllKey(CArray<double, double>& aKey, double& errorKey);
	BOOL AddData(double key, T_MVLDjp_MLOAD_BASE data);
	BOOL AddData(CArray<double, double> &aKey, CArray<T_MVLDjp_MLOAD_BASE, T_MVLDjp_MLOAD_BASE&> &aData, BOOL bNotify=TRUE);
	BOOL DelData(double key);
	BOOL DelData(CArray<double, double> &aKey, BOOL bNotify=TRUE);
	BOOL ModifyData(double oldKey, double key, T_MVLDjp_MLOAD_BASE data);
	BOOL ModifyData(CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<T_MVLDjp_MLOAD_BASE, T_MVLDjp_MLOAD_BASE&> &aData);
	void DBChanged();

	void UpdateBuffer(int nCmd, CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<T_MVLDjp_MLOAD_BASE, T_MVLDjp_MLOAD_BASE&> &aData);

	BOOL InsertDataAt(int nzbPos, T_MVLDjp_MLOAD_BASE& data);
	BOOL RemoveDataAt(int nzbPos);
	BOOL ChangeDataAt(int nzbPos, T_MVLDjp_MLOAD_BASE& data);
	BOOL SaveData(T_MVLDjp_MLOAD_BASE aData[]);
	BOOL RestoreData(T_MVLDjp_MLOAD_BASE aData[]);
	void AfterUpdateBuffer();
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMvldItemJPGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMvldItemJPGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMvldItemJPGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MvldjpITEMGRID_H__4DC89E24_A291_460A_95F6_3E4080118364__INCLUDED_)
