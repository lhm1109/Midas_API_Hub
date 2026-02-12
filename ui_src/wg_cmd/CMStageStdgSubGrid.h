#if !defined(__CMSTAGESTDGSUBGRID_H__)
#define __CMSTAGESTDGSUBGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStageStdgSubGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"

/////////////////////////////////////////////////////////////////////////////
// CCMStageStdgSubGrid window
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMStageStdgSubGrid : public CTBCommon
{
// Construction
public:
	CCMStageStdgSubGrid();

// Attributes
public:
	T_ESQW_ADD_LOAD* m_pData;
	static CString m_aCurDefVal[2];

// Operations
public:
	void Initialize(T_ESQW_ADD_LOAD* pData);
	BOOL DeleteRecordByKey(const double &key, const long &data);
	BOOL InsertRecordByKey(const double &key, const long &data);

	virtual int GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	virtual long GetSelectedKeys(CArray<long, long> &caKey);

	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, double &key, long &data);
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

	BOOL GetValue(ROWCOL nRow, double &key, long &data);
	BOOL SetValue(ROWCOL nRow, const double &key, const long &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const double &key, const long &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, long& data);
	void ConvDataToStr(int i, const long& data, CString& value);

	void MakeSearchKey(const double &key, const long &data, CStringArray &aKey);
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);

	BOOL FindData(double key, int& nPos, int nStart=0);
	BOOL ExistAllKey(CArray<double, double>& aKey, double& errorKey);
	BOOL AddData(double key, long data);
	BOOL AddData(CArray<double, double> &aKey, CArray<long, long> &aData, BOOL bNotify=TRUE);
	BOOL DelData(double key);
	BOOL DelData(CArray<double, double> &aKey, BOOL bNotify=TRUE);
	BOOL ModifyData(double oldKey, double key, long data);
	BOOL ModifyData(CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<long, long> &aData);

	void UpdateBuffer(int nCmd, CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<long, long> &aData);

	T_STOR_K GetStorData(CString& value);
	CString  GetStorStr(T_STOR_K data);
	BOOL CheckStoryLevel(double oldkey, double key, long data, BOOL bModify);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStageStdgSubGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMStageStdgSubGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMStageStdgSubGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSTAGESTDGSUBGRID_H__)
