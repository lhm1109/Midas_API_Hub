#if !defined(AFX_SPFCITEMGRID_H__5B311B43_B86C_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_SPFCITEMGRID_H__5B311B43_B86C_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpfcItemGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"

/////////////////////////////////////////////////////////////////////////////
// CSpfcItemGrid window
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CSpfcItemGrid : public CTBCommon
{
// Construction
public:
	CSpfcItemGrid(BOOL bPsov=FALSE);

// Attributes
public:
	T_SPFC_D* m_pData;
	static CString m_aCurDefVal[2];
	BOOL m_bPushOver;

// Operations
public:
	void Initialize(T_SPFC_D* pData, BOOL bShowReadOnly=FALSE);
	BOOL DeleteRecordByKey(const double &key, const double &data);
	BOOL InsertRecordByKey(const double &key, const double &data);

	virtual int GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	virtual long GetSelectedKeys(CArray<long, long> &caKey);

	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, double &key, double &data);
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

	BOOL GetValue(ROWCOL nRow, double &key, double &data);
	BOOL SetValue(ROWCOL nRow, const double &key, const double &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const double &key, const double &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, double& data);
	void ConvDataToStr(int i, const double& data, CString& value);

	void MakeSearchKey(const double &key, const double &data, CStringArray &aKey);
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);

	BOOL FindData(double key, int& nPos, int nStart=0);
	BOOL ExistAllKey(CArray<double, double>& aKey, double& errorKey);
	BOOL AddData(double key, double data);
	BOOL AddData(CArray<double, double> &aKey, CArray<double, double> &aData, BOOL bNotify=TRUE);
	BOOL DelData(double key);
	BOOL DelData(CArray<double, double> &aKey, BOOL bNotify=TRUE);
	BOOL ModifyData(double oldKey, double key, double data);
	BOOL ModifyData(CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<double, double> &aData);
	void DBChanged();

	void UpdateBuffer(int nCmd, CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<double, double> &aData);

	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL Copy();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpfcItemGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSpfcItemGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSpfcItemGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPFCITEMGRID_H__5B311B43_B86C_11D3_92DE_0000C0B0E6B3__INCLUDED_)
