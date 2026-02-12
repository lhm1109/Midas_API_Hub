// SoilParamAddLoadGrid.h: interface for the CSoilParamAddLoadGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__SOILPARAMADDLOADGRID_H__)
#define __SOILPARAMADDLOADGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_db\wg_db_DBDoc.h"

struct T_POSP_TB_DATA
{
	double dHeight;
	double dAngle;
	double N;
	double dDensity;
	double dPoissonRatio;
	double dVs;
	double dKh;
	double dDisp;
	void Initialize()
	{
		T_POSP_BASE_DATA base;
		dHeight		= base.dHeight;
		dAngle		= base.dAngleOrN;
		N			= base.dAngleOrN;
		dDensity	= base.dDensity;
		dPoissonRatio = base.dPoissonRatio;
		dVs			= base.dVs;
		dKh			= base.dKh;
		dDisp       = base.dDisp;
	}
	T_POSP_TB_DATA()
	{
		Initialize();
	}
};

class CSoilParamAddLoadGrid : public CTBCommon
{
public:
	CSoilParamAddLoadGrid();
	virtual ~CSoilParamAddLoadGrid();

	// Attributes
public:
	CArray<T_POSP_TB_DATA, T_POSP_TB_DATA&> m_Data;
	static CString m_aCurDefVal[10];

	// Operations
public:
	void Initialize(CWnd* pParent, int nWidth);
	void SetTitle();
	BOOL DeleteRecordByRow(ROWCOL nRow);
	BOOL InsertRecordByRow(ROWCOL nRow, const T_POSP_TB_DATA &data);

	void SetData(CArray<T_POSP_BASE_DATA, T_POSP_BASE_DATA&>& aPospBase, BOOL bMakeItem = TRUE);
	void GetData(CArray<T_POSP_BASE_DATA, T_POSP_BASE_DATA&>& aPospBase);

	void AddRow(double dHeight, int nRowCnt, BOOL bInsert = FALSE);
	void DelRow();

	void SetGroundLevel(double dGroundLevel, BOOL bMakeItem = TRUE);
	double GetStartLevel(ROWCOL nRow);
	ROWCOL GetCurrentRowByLevel(double dLevelKey);
	void SetUseN(BOOL bUseN, BOOL bMakeItem = TRUE);
	BOOL GetUseN() { return m_bUseN; }
	void SetAngleorN(T_POSP_TB_DATA &rData, const double val);
	double GetAngleorN(const T_POSP_TB_DATA &rData);

	// Overrides
	virtual int GetKeyColID() { return 0; }
	virtual long GetSelectedKeys(CArray<long, long> &caKey) { return 0; }

	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, int &key, T_POSP_TB_DATA &data);
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

	BOOL GetValue(ROWCOL nRow, T_POSP_TB_DATA &data);
	BOOL SetValue(ROWCOL nRow, const T_POSP_TB_DATA &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const T_POSP_TB_DATA &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_POSP_TB_DATA& data);
	void ConvDataToStr(int i, const T_POSP_TB_DATA& data, CString& value);

	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);

	BOOL AddData(ROWCOL nRow, T_POSP_TB_DATA& data);
	BOOL AddData(CRowColArray& awRows, CArray<T_POSP_TB_DATA, T_POSP_TB_DATA&> &aData, BOOL bNotify = TRUE);
	BOOL ModifyData(ROWCOL nRow, T_POSP_TB_DATA data);
	BOOL ModifyData(CRowColArray& awRows, CArray<T_POSP_TB_DATA, T_POSP_TB_DATA&> &aData);

	void UpdateBuffer(int nCmd, CRowColArray& awRows, CArray<T_POSP_TB_DATA, T_POSP_TB_DATA&> &aData);

	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL Copy();

protected:
	int m_nWndWidth;
	double m_dGroundLevel;
	BOOL m_bUseN;
	BOOL m_bCivil;

	CUnitCtrl* m_pUnitCtrl;
	CDBDoc* m_pDoc;
};

#endif // !defined(__SOILPARAMADDLOADGRID_H__)