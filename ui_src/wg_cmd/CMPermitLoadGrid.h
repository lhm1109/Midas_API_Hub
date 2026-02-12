#if !defined(AFX_CMPERMITLOADGRID_H__5B311B43_B86C_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMPERMITLOADGRID_H__5B311B43_B86C_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMPermitLoadGrid.h : header file
//

#include "..\wg_db\DBDoc.h"
#include "..\wg_common\TBCommon.h"

/////////////////////////////////////////////////////////////////////////////
// CCMPermitLoadGrid window
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMPermitLoadGrid : public CTBCommon
{
// Construction
public:
	CCMPermitLoadGrid();

// Attributes
public:
	T_MVHL_D* m_pData;
	static CString m_aCurDefVal[];

// Operations
public:
	void Initialize(T_MVHL_D* pData);
	BOOL DeleteRecordByKey(const int &key, const T_MVHL_PERMIT_ITEM &data);
	BOOL InsertRecordByKey(const int &key, const T_MVHL_PERMIT_ITEM &data);

	virtual int GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	virtual long GetSelectedKeys(CArray<long, long> &caKey);

	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, int &key, T_MVHL_PERMIT_ITEM &data);
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

	BOOL GetValue(ROWCOL nRow, int &key, T_MVHL_PERMIT_ITEM &data);
	BOOL SetValue(ROWCOL nRow, const int &key, const T_MVHL_PERMIT_ITEM &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const int &key, const T_MVHL_PERMIT_ITEM &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_MVHL_PERMIT_ITEM& data);
	void ConvDataToStr(int kye, int i, const T_MVHL_PERMIT_ITEM& data, CString& value);

	void MakeSearchKey(const int &key, const T_MVHL_PERMIT_ITEM &data, CStringArray &aKey);
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);

	BOOL AddData(int key, T_MVHL_PERMIT_ITEM& data);
	BOOL AddData(CArray<int, int> &aKey, CArray<T_MVHL_PERMIT_ITEM, T_MVHL_PERMIT_ITEM&> &aData, BOOL bNotify=TRUE);
	BOOL DelData(int key);
	BOOL DelData(CArray<int, int> &aKey, BOOL bNotify=TRUE);
	BOOL ModifyData(int oldKey, int key, T_MVHL_PERMIT_ITEM& data);
	BOOL ModifyData(CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_MVHL_PERMIT_ITEM, T_MVHL_PERMIT_ITEM&> &aData);
	void DBChanged();

	void UpdateBuffer(int nCmd, CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_MVHL_PERMIT_ITEM, T_MVHL_PERMIT_ITEM&> &aData);

	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL Copy();
	void SetTypeColumn(ROWCOL nCol);
	int GetAxleTypeIx(LPCTSTR lpszTypeName);
	CString GetAxleTypeName(int nTypeIx);

	void UpdateAxleTypeColumn();
	virtual void AdjustAppendRowNo(ROWCOL nCol = 0);
	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);
	void MakeLastSpaceEnd(BOOL bMakeEnd);

	BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	//BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMPermitLoadGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMPermitLoadGrid();

public:
	BOOL m_bCheckVS;

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMPermitLoadGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMPERMITLOADGRID_H__5B311B43_B86C_11D3_92DE_0000C0B0E6B3__INCLUDED_)
