#if !defined(AFX_CMSTORGRID_IGEN_H__DB5AB4C1_D239_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSTORGRID_IGEN_H__DB5AB4C1_D239_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStorGrid_IGen.h : header file
//

#include "..\wg_db\DB_ST_DN_JUD.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"

/////////////////////////////////////////////////////////////////////////////
// CCMStorGrid_IGen window

class CCMStorGrid_IGen : public CTBCommon
{
// Construction
public:
	CCMStorGrid_IGen();
	virtual ~CCMStorGrid_IGen();
	double m_dTopLevel;

// Operations
public:
	void Initialize(CDBDoc* pDoc);
	BOOL DeleteRecordByKey(const T_STOR_K &key, const T_STOR_D &data, const T_IRST_D &IrstD);
	BOOL InsertRecordByKey(const T_STOR_K &key, const T_STOR_D &data, const T_IRST_D &IrstD);

	virtual void UpdateBuffer(CDBDoc* pDoc);
	virtual void UnitChanged(CDBDoc* pDoc);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStorGrid_IGen)
	//}}AFX_VIRTUAL
public:
	virtual int  GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);

	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, T_STOR_K &key, T_STOR_D &data, T_IRST_D &IrstD);
	virtual BOOL ValidateField(CString value, int nColID);
	virtual BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols);

	virtual long GetSelectedKeys(CArray<long, long> &caKey);
	virtual BOOL AddToDB(ROWCOL nRow);
	virtual BOOL AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData);
	//virtual BOOL AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData, CArray<void*, void*>& raIrst);  
	virtual BOOL DeleteFromDB(ROWCOL nRow);
	virtual BOOL DeleteFromDB(CRowColArray& awRows);
	virtual BOOL ModifyToDB(ROWCOL nRow);
	virtual BOOL ModifyToDB(CStringArray& raOldKey, CArray<void*, void*>& raKey, CArray<void*, void*>& raData);
	//virtual BOOL ModifyToDB(CStringArray& raOldKey, CArray<void*, void*>& raKey, CArray<void*, void*>& raData, CArray<void*, void*>& raIrst);

	virtual CString GetNewKey();
	virtual CString GetDefValue(int nIndex);
	virtual CString* GetDefValueArray();
	virtual BOOL MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, CArray<void*, void*>& aKey, CArray<void*, void*>& aData);
	//virtual BOOL MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, CArray<void*, void*>& aKey, CArray<void*, void*>& aData, CArray<void*, void*>& aIrst);  
	virtual void DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData);

	virtual long GetCountData();
	virtual void MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols);
	virtual void MakeItemNormal(long nNumData, CRowColArray& awCols);
	virtual void MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord);
	virtual void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);

	// Story Height 설정을 위해 Override
	virtual void AdjustAppendRowNo(ROWCOL nCol = 0);

	// 셀 수정이 다른 셀의 값이나  Enable 상태 변경하는 경우 때문에 Override
	// Level과 Height
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual void OnCanceledEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnCanceledModify();
	void ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol);
	void SaveCurrentRecordValue(ROWCOL nRow, ROWCOL nCol);
	void RestorePrevRecordValue(ROWCOL nRow, ROWCOL nCol);

	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);

// Implementation
public:
	BOOL GetValue(ROWCOL nRow, T_STOR_K &key, T_STOR_D &data, T_IRST_D& IrstD);
	BOOL SetValue(ROWCOL nRow, const T_STOR_K &key, const T_STOR_D &data, const T_IRST_D& IrstD);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const T_STOR_K &key, const T_STOR_D &data, const T_IRST_D &IrstD, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_STOR_D& data, T_IRST_D& IrstD);
	void ConvDataToStr(int i, const T_STOR_D& data, const T_IRST_D& IrstD, CString& value);

	void GetAllSelectedStor(CArray<T_STOR_K, T_STOR_K>& aKey);
	void MakeSearchKey(const T_STOR_K &key, const T_STOR_D &data, const T_IRST_D &IrstD, CStringArray &aKey);

	void SetColumnAttr();
	void SetStoryHeight(ROWCOL nHeightCol);

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMStorGrid_IGen)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CArray<T_STOR_K, T_STOR_K> m_aSelectedKeys;
	static CString m_aCurDefVal[];
	double m_dblOldLevel;
	BOOL m_bBeforeSaved;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSTORGRID_IGEN_H__DB5AB4C1_D239_11D3_92DE_0000C0B0E6B3__INCLUDED_)
