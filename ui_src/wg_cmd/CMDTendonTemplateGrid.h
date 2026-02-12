#if !defined(AFX_TDNAITEMGRID_H__672479E1_E839_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_TDNAITEMGRID_H__672479E1_E839_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TdnaItemGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"
#include "DgnTendonTemplateData.h"

enum TNDN_TEMPALTE_TYPE_XYXZ
{
	TNDN_TEMPALTE_TYPE_XYXZ_XZ = 0,
	TNDN_TEMPALTE_TYPE_XYXZ_XY
};

struct T_TDNA_TEMPLATE_BASE_GRID
{
	int spanNum;
	double dDt;
	double dDb;
	double dDb1;
	double dDt1;
		double dDf;
	double dDe;
	double dDRad;
		double dR1;
		double dR2;
	
	void Init()
	{
		spanNum	= 0;
		dDt		= 0.0;
		dDb		= 0.0;
		dDt1	= 0.0;
		dDb1	= 0.0;
		dDf		= 0.0;
		dDe		= 0.0;
		dDRad	= 0.0;
				dR1     = 0.0;
				dR2     = 0.0;
	}
	T_TDNA_TEMPLATE_BASE_GRID()
	{
		Init();
	}
	T_TDNA_TEMPLATE_BASE_GRID(T_TDNA_TEMPLATE_BASE_GRID& src)
	{
		*this = src;
	}
	T_TDNA_TEMPLATE_BASE_GRID& operator=(const T_TDNA_TEMPLATE_BASE_GRID& src)
	{
		spanNum	= src.spanNum;
		dDt		= src.dDt;
		dDb		= src.dDb;
		dDt1	= src.dDt1;
		dDb1	= src.dDb1;
		dDf		= src.dDf;
		dDe		= src.dDe;
		dDRad	= src.dDRad;
				dR1     = src.dR1;
				dR2     = src.dR2;
		return *this;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CCMDTendonTemplateGrid window

//#include "HeaderPre.h"

class /*__MY_EXT_CLASS__*/ CCMDTendonTemplateGrid : public CTBCommon
{
// Construction
public:
	CCMDTendonTemplateGrid();
	virtual ~CCMDTendonTemplateGrid();

// Attributes
public:
	TndnTemplate*           m_pData;
	TNDN_TEMPALTE_TYPE_XYXZ	m_nInputType; // 0 : XZ, 1 : XY
	TNDN_TEMPLATE_TYPE      m_nSubType;
	static CString          m_aCurDefVal[];
	static std::vector<CString>          m_aHeader;
	static std::vector<CString>          m_aHeaderPK;
	static std::vector<CString>          m_aHeaderReal;

// Operations
public:
	void Initialize(TNDN_TEMPALTE_TYPE_XYXZ nType, TndnTemplate* pData);    // nType : 0 : XY, 1 : XZ
	BOOL DeleteRecordByKey(const int &key, const T_TDNA_TEMPLATE_BASE_GRID &data);
	BOOL InsertRecordByKey(const int &key, const T_TDNA_TEMPLATE_BASE_GRID &data);
	
	virtual int GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	virtual long GetSelectedKeys(CArray<long, long> &caKey);
	
	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, int &key, T_TDNA_TEMPLATE_BASE_GRID &data);
	BOOL ValidateField(CString value, int nColID);
	BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols);
	
	virtual BOOL AddToDB(ROWCOL nRow);
	virtual BOOL AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData);
	virtual BOOL DeleteFromDB(ROWCOL nRow);
	virtual BOOL DeleteFromDB(CRowColArray& awRows);
	virtual BOOL ModifyToDB(ROWCOL nRow);
	virtual BOOL ModifyToDB(CStringArray& raOldKey, CArray<void*, void*>& raKey, CArray<void*, void*>& raData);
	virtual BOOL OnValidateField(ROWCOL nRow, ROWCOL nCol);
		
	virtual CString GetNewKey();
	virtual CString GetDefValue(int nIndex);
	virtual CString* GetDefValueArray();
	virtual BOOL MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
		CArray<void*, void*>& aKey, CArray<void*, void*>& aData);
	virtual void DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData);
	
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt /* = gxCopy */, int nType /* = 0 */);
	virtual long GetCountData();
	virtual void MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols);
	virtual void MakeItemNormal(long nNumData, CRowColArray& awCols);
	virtual void MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord);
	
	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	virtual void OnCanceledEditing(ROWCOL nRow, ROWCOL nCol);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);
	
	virtual BOOL OnDeleteRecord(ROWCOL nRow);
	virtual void Redraw(UINT flags = GX_UPDATENOW, BOOL bCreateHint = TRUE);
	
	BOOL GetValue(ROWCOL nRow, int &key, T_TDNA_TEMPLATE_BASE_GRID &data);
	BOOL SetValue(ROWCOL nRow, const int &key, const T_TDNA_TEMPLATE_BASE_GRID &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const int &key, const T_TDNA_TEMPLATE_BASE_GRID &data, CRowColArray &awCols);
	BOOL AddData(int key, TndnTemplateSubInfo data);
	void DBChanged();
	
	void UpdateBuffer(int nCmd, CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_TDNA_TEMPLATE_BASE_GRID, T_TDNA_TEMPLATE_BASE_GRID&> &aData);
	void OnUnitChange();

	void SetTemplateTypeSameSpan( BOOL bIsXY, BOOL bUseSameSpanInfo );
	void SetTemplateType(TNDN_TEMPLATE_TYPE nType, BOOL bSymmetry = FALSE);

protected:
	void ConvStrToData(int i, CString& value, T_TDNA_TEMPLATE_BASE_GRID& data);
	void ConvDataToStr(int i, const T_TDNA_TEMPLATE_BASE_GRID& data, CString& value);
	BOOL AddData(int key, T_TDNA_TEMPLATE_BASE_GRID data);
	BOOL AddData(CArray<int, int> &aKey, CArray<T_TDNA_TEMPLATE_BASE_GRID, T_TDNA_TEMPLATE_BASE_GRID&> &aData, BOOL bNotify=TRUE);

	void GridBase2Base(T_TDNA_TEMPLATE_BASE_GRID source, TndnTemplateSubInfo& target);
	void Base2GridBase(TndnTemplateSubInfo source, T_TDNA_TEMPLATE_BASE_GRID& target);

	void MakeSearchKey(const int &key, const T_TDNA_TEMPLATE_BASE_GRID &data, CStringArray &aKey);
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);
	void SetHeaderTitle_2();

	BOOL DelData(int key);
	BOOL DelData(CArray<int, int> &aKey, BOOL bNotify=TRUE);
	BOOL ModifyData(int oldKey, int key, T_TDNA_TEMPLATE_BASE_GRID data);
	BOOL ModifyData(CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_TDNA_TEMPLATE_BASE_GRID, T_TDNA_TEMPLATE_BASE_GRID&> &aData);

	//void ChangeGridColEnable(ROWCOL nRow);
	BOOL FindData(int key, int& nPos, int nStart=0);
	BOOL ExistAllKey(CArray<int, int>& aKey, int& errorKey);
	void ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol);

	static std::vector<CString>& Get_HeaderAr();
	static std::vector<CString>& Get_HeaderPKAr();
	static std::vector<CString>& Get_HeaderRealAr();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	DECLARE_MESSAGE_MAP()
};

//#include "HeaderPost.h"

#endif
