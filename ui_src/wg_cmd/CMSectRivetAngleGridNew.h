#if !defined(__CMSECTRIVETANGLEGRIDNEW_H__)
#define __CMSECTRIVETANGLEGRIDNEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\CMBaseGrid.h"
#include "..\wg_common\wg_common_TBCommon.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectRivetAngleGridNew window

struct RIVET_POS_Temp
{
	BOOL   bSymRow;
	T_SECT_RIVET_POS_D Data;

	RIVET_POS_Temp() { Initialize(); }
	RIVET_POS_Temp(T_SECT_RIVET_POS_D& rData, BOOL brSymRow)
	{
		bSymRow = brSymRow;
		Data    = rData;
	}

	RIVET_POS_Temp(const RIVET_POS_Temp& rData) { *this = rData; }
	RIVET_POS_Temp& operator=(const RIVET_POS_Temp& rData)
	{
		bSymRow = rData.bSymRow;
		Data = rData.Data;
		return (*this);
	}
	void Initialize()
	{
		bSymRow = FALSE;
		Data.Initialize();
	}
};

#define D_REVIT_COL_USE  1
#define D_REVIT_COL_COM  2
#define D_REVIT_COL_ANGL 3
#define D_REVIT_COL_HOR  4
#define D_REVIT_COL_VER  5
#define D_REVIT_COL_DR   6
#define D_REVIT_COL_DIA  7
#define D_DATA_NUM  D_REVIT_COL_DIA
class CCMSectRivetAngleGridNew : public CTBCommon
{
public:
	CCMSectRivetAngleGridNew();
	virtual ~CCMSectRivetAngleGridNew();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CCMSectRivetAngleGridNew)
		//}}AFX_VIRTUAL

public:
	CArray<RIVET_POS_Temp, RIVET_POS_Temp&> m_aData;

protected:
	BOOL m_bSymmetric;
	
	static CString m_aCurDefVal[D_DATA_NUM];

public:
	void Initialize();
	BOOL DeleteRecordByKey(const double& key, const T_SECT_RIVET_POS_D& data);
	BOOL InsertRecordByKey(const double& key, const T_SECT_RIVET_POS_D& data);

	//
	void SetData2Grid(CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&>& aRivetPos, BOOL bSymmetric, CArray<int, int>& aAngleIndex);
 	BOOL GetGrid2Data(CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&>& aRivetPos);
	//

	virtual int GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long* keys, int nSize);
	virtual long GetSelectedKeys(CArray<long, long>& caKey);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange& range);
	BOOL PasteFromClipboard(const CGXRange& range, CString* aDefVal, int nKeyCol,
		CGXGridWnd& x, int& nPasteMode, int& nPastedCols);

	BOOL ValidateAndMakeRecord(CStringArray& value, CRowColArray& aCols, double& key, T_SECT_RIVET_POS_D& data);
	BOOL ValidateField(CString value, int nColID);
	BOOL ValidateRecord(CStringArray& value, CRowColArray& aCols);

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
	virtual	BOOL FindRecordByKey(ROWCOL nStart, ROWCOL nEnd, CRowColArray& awCols, CStringArray& aKey, ROWCOL& nRow);
	virtual	BOOL FindRecordByKey(CStringArray& aKey, ROWCOL& nRow);

	BOOL CheckValue(T_SECT_RIVET_POS_D& data);
	BOOL GetValue(ROWCOL nRow, double& key, T_SECT_RIVET_POS_D& data);
	BOOL SetValue(ROWCOL nRow, const double& key, const T_SECT_RIVET_POS_D& data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const double& key, const T_SECT_RIVET_POS_D& data, CRowColArray& awCols);
	void ConvStrToData(int i, CString& value, T_SECT_RIVET_POS_D& data);
	void ConvDataToStr(int i, const T_SECT_RIVET_POS_D& data, CString& value);

	void MakeSearchKey(const double& key, const T_SECT_RIVET_POS_D& data, CStringArray& aKey);
	void SetHeaderTitle(T_UNIT_SYSTEM& UnitSystem, BOOL bInit = FALSE);

	BOOL FindData(double key, int& nPos, int nStart = 0);
	BOOL ExistAllKey(CArray<double, double>& aKey, double& errorKey);
	BOOL AddData(double key, T_SECT_RIVET_POS_D data);
	BOOL AddData(CArray<double, double>& aKey, CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&>& aData, BOOL bNotify = TRUE);
	BOOL DelData(double key);
	BOOL DelData(CArray<double, double>& aKey, BOOL bNotify = TRUE);
	BOOL ModifyData(double oldKey, double key, T_SECT_RIVET_POS_D data);
	BOOL ModifyData(CArray<double, double>& aOldKey, CArray<double, double>& aKey, CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&>& aData);
	void DBChanged();

	void UpdateBuffer(int nCmd, CArray<double, double>& aOldKey, CArray<double, double>& aKey, CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&>& aData);

	void SetHunchInfo(BOOL bHunch);

protected:
	CMap<int , int , RIVET_POS_Temp, RIVET_POS_Temp> m_mapRowdata;
	CMap<int , int , int, int> m_mapUsecAngle;

	void SortRivetArray(CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D>& aArray);
	void ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol);
	void ChangeColumnStatusUser(ROWCOL nRow, ROWCOL nCol);

	void DataToString(BOOL bSymmetric, std::map<int, T_SECT_RIVET_POS_D>& mapUseRevet, std::map<int, T_SECT_RIVET_POS_D>& mapUseRevetW, IN T_SECT_RIVET_POS_D& RivetPos,
					  BOOL& bRightPart, OUT CStringArray& aValue);

	void SetDataToRow(ROWCOL nRow, BOOL bSymRow, CStringArray& raValue);

	void ConvRusToEng(CString& value);
	void ConvEngToRus(CString& value);

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSWPSCSectPageGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTRIVETANGLEGRIDNEW_H__)
