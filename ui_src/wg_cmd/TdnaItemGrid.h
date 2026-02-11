#if !defined(AFX_TDNAITEMGRID_H__672479E1_E839_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_TDNAITEMGRID_H__672479E1_E839_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TdnaItemGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"


struct T_TDNA_BASE_GRID
{
	double x, y, z; // Local Coordinates
	BOOL   bFix;
	double dRy, dRz;
	double dRadius;
	int    nAddOpt;    // 0:None, 1:Left, 2:Right (6.4.0에서 추가)
	double dAngle;     // -90 < dAngle < 90       (6.4.0에서 추가)
	double dHeight;    // Height                  (6.4.0에서 추가)
	double dRadius2;   // Second Radius           (6.4.0에서 추가)
	BOOL   bBOT;
	void Initialize()
	{
		x = y = z = 0.0;
		bFix = FALSE;
		dRy = dRz = 0.0;    
		dRadius = 0.0;
		nAddOpt = 0;
		dAngle = 0.0;
		dHeight = 0.0;
		dRadius2 = 0.0;
		bBOT = FALSE;
	}
	T_TDNA_BASE_GRID()
	{
	}
	T_TDNA_BASE_GRID(T_TDNA_BASE_GRID& src)
	{
		*this = src;
	}
	T_TDNA_BASE_GRID& operator=(const T_TDNA_BASE_GRID& src)
	{
		x = src.x;
		y = src.y;
		z = src.z;
		bFix = src.bFix;
		dRy = src.dRy;
		dRz = src.dRz;
		dRadius = src.dRadius;
		nAddOpt = src.nAddOpt;
		dAngle = src.dAngle;
		dHeight = src.dHeight;
		dRadius2 = src.dRadius2;
		bBOT = src.bBOT;
		return *this;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CTdnaItemGrid window
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CTdnaItemGrid : public CTBCommon
{
// Construction
public:
	CTdnaItemGrid();

// Attributes
public:
	T_TDNA_D* m_pData;
	int m_nInputType;
	int  m_nCurveType;
	static CString m_aCurDefVal[];

// Operations
public:
	void Initialize(int nType, T_TDNA_D* pData);    // nType : (1) 3D  (2) 2D-XY (3) 2D-XZ
	BOOL DeleteRecordByKey(const double &key, const T_TDNA_BASE_GRID &data);
	BOOL InsertRecordByKey(const double &key, const T_TDNA_BASE_GRID &data);

	virtual int GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	virtual long GetSelectedKeys(CArray<long, long> &caKey);

	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, double &key, T_TDNA_BASE_GRID &data);
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

	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	virtual void OnCanceledEditing(ROWCOL nRow, ROWCOL nCol);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);
	
	virtual BOOL OnDeleteRecord(ROWCOL nRow);

	BOOL GetValue(ROWCOL nRow, double &key, T_TDNA_BASE_GRID &data);
	BOOL SetValue(ROWCOL nRow, const double &key, const T_TDNA_BASE_GRID &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const double &key, const T_TDNA_BASE_GRID &data, CRowColArray &awCols);
	BOOL AddData(double key, T_TDNA_BASE data);
	BOOL AddData(double key, T_TDNA_BASE data, BOOL bBot); //2D의 XZ용
	void DBChanged();

	void UpdateBuffer(int nCmd, CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<T_TDNA_BASE_GRID, T_TDNA_BASE_GRID&> &aData);
	void OnUnitChange();
	void SetCurveType(int nCurveType, int nShapeType);// 2D Element를 구분하기 위해 변수 nShapeType 추가 2005. 12.23 jkpark

protected:
	void ConvStrToData(int i, CString& value, T_TDNA_BASE_GRID& data);
	void ConvDataToStr(int i, const T_TDNA_BASE_GRID& data, CString& value);
	BOOL AddData(double key, T_TDNA_BASE_GRID data);
	BOOL AddData(CArray<double, double> &aKey, CArray<T_TDNA_BASE_GRID, T_TDNA_BASE_GRID&> &aData, BOOL bNotify=TRUE);
	void GridBase2Base(T_TDNA_BASE_GRID source, T_TDNA_BASE& target);
	void GridBase2Base(T_TDNA_BASE_GRID source, T_TDNA_BASE_2D& target);
	void Base2GridBase(T_TDNA_BASE source, T_TDNA_BASE_GRID& target);
	void Base2GridBase(T_TDNA_BASE_2D source, T_TDNA_BASE_GRID& target);
	void MakeSearchKey(const double &key, const T_TDNA_BASE_GRID &data, CStringArray &aKey);
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);
	BOOL DelData(double key);
	BOOL DelData(CArray<double, double> &aKey, BOOL bNotify=TRUE);
	BOOL ModifyData(double oldKey, double key, T_TDNA_BASE_GRID data);
	BOOL ModifyData(CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<T_TDNA_BASE_GRID, T_TDNA_BASE_GRID&> &aData);
	void ChangeGridColEnable(ROWCOL nRow);
	BOOL FindData(double key, int& nPos, int nStart=0);
	BOOL ExistAllKey(CArray<double, double>& aKey, double& errorKey);
	void ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol);
	void ChangeAngleColEnable(ROWCOL nRow, BOOL bEnable);



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTdnaItemGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTdnaItemGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTdnaItemGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDNAITEMGRID_H__672479E1_E839_11D3_92DE_0000C0B0E6B3__INCLUDED_)
