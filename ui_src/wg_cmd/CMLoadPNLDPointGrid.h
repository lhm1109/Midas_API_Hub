#if !defined(AFX_CMLOADPNLDPOINTGRID_H__0E105D9A_EE0A_4A22_B86D_85328D57A036__INCLUDED_)
#define AFX_CMLOADPNLDPOINTGRID_H__0E105D9A_EE0A_4A22_B86D_85328D57A036__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadPNLDPointGrid.h : header file
//


#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"

struct T_PNLD_PT_BASE
{
	double dLoad;
	double dDistance;

	double	x,y;
	double	frc,mmt;

	T_PNLD_PT_BASE() { Initialize(); }
	void Initialize()
	{
		dLoad =0.0;
		dDistance =0.0;
		x =0.0, y =0.0;
		frc =0.0, mmt =0.0;
	}

};
/////////////////////////////////////////////////////////////////////////////
// CCMLoadPNLDPointGrid window
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMLoadPNLDPointGrid : public CTBCommon
{
// Construction
public:
	CCMLoadPNLDPointGrid();

// Attributes
public:
	T_UNIT_SYSTEM m_US;
	T_PNLD_D* m_pData;
	static CString m_aCurDefVal[];

// Operations
public:
	void	My_SaveData(void);


	void Initialize(T_PNLD_D* pData);
	BOOL DeleteRecordByKey(const double &key, const T_PNLD_PT_BASE &data);
	BOOL InsertRecordByKey(const double &key, const T_PNLD_PT_BASE &data);

	virtual int GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	virtual long GetSelectedKeys(CArray<long, long> &caKey);

	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, double &key, T_PNLD_PT_BASE &data);
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

	BOOL GetValue(ROWCOL nRow, double &key, T_PNLD_PT_BASE &data);
	BOOL SetValue(ROWCOL nRow, const double &key, const T_PNLD_PT_BASE &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const double &key, const T_PNLD_PT_BASE &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_PNLD_PT_BASE& data);
	void ConvDataToStr(int i, const T_PNLD_PT_BASE& data, CString& value);

	void MakeSearchKey(const double &key, const T_PNLD_PT_BASE &data, CStringArray &aKey);
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);

	BOOL FindData(double key, int& nPos, int nStart=0);
	BOOL ExistAllKey(CArray<double, double>& aKey, double& errorKey);
	BOOL AddData(double key, T_PNLD_PT_BASE data);
	BOOL AddData(CArray<double, double> &aKey, CArray<T_PNLD_PT_BASE, T_PNLD_PT_BASE&> &aData, BOOL bNotify=TRUE);
	BOOL DelData(double key);
	BOOL DelData(CArray<double, double> &aKey, BOOL bNotify=TRUE);
	BOOL ModifyData(double oldKey, double key, T_PNLD_PT_BASE data);
	BOOL ModifyData(CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<T_PNLD_PT_BASE, T_PNLD_PT_BASE&> &aData);
	void DBChanged();

	void UpdateBuffer(int nCmd, CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<T_PNLD_PT_BASE, T_PNLD_PT_BASE&> &aData);

	BOOL InsertDataAt(int nzbPos, T_PNLD_PT_BASE& data);
	BOOL RemoveDataAt(int nzbPos);
	BOOL ChangeDataAt(int nzbPos, T_PNLD_PT_BASE& data);
	BOOL SaveData(CArray<T_PNLD_PT_BASE, T_PNLD_PT_BASE&> &org);
	BOOL RestoreData(CArray<T_PNLD_PT_BASE, T_PNLD_PT_BASE&> &aData);
	void AfterUpdateBuffer();

	void  UpdateUnit();
	void  UnitChanged(CDBDoc* pDoc);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadPNLDPointGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMLoadPNLDPointGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMLoadPNLDPointGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADPNLDPOINTGRID_H__0E105D9A_EE0A_4A22_B86D_85328D57A036__INCLUDED_)
