#if !defined(__SECTRFORCGRID_H__)
#define __SECTRFORCGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeshRatioList.h : header file
//

#define _GXDLL
#include "./Grid/GXALL.h"

#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

/////////////////////////////////////////////////////////////////////////////

// CSectRForcGrid window
#include "HeaderPre.h"

class CSectRForcDlg;

class __MY_EXT_CLASS__ CSectRForcGrid :  public CTBCommon, public CDBUpdateConnector
{
public:
// Construction
public:
	CSectRForcGrid();
	CSectRForcGrid(CWnd* pParent);
	void Init_Construction();

// Attributes
public:
	static CString m_aCurDefVal[5];

// Operations
public:
	void Initialize();
	BOOL DeleteRecordByKey(const int &key, const T_VSEC_D &data);
	BOOL InsertRecordByKey(const int &key, const T_VSEC_D &data);

	virtual int GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	virtual long GetSelectedKeys(CArray<long, long> &caKey);  
	virtual BOOL OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);

	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, int &key, T_VSEC_D &data);
	BOOL ValidateField(CString value, int nColID);
	BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols);

	virtual BOOL AddToDB(ROWCOL nRow);
	virtual BOOL AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData);
	virtual BOOL DeleteFromDB(ROWCOL nRow);
	virtual BOOL DeleteFromDB(CRowColArray& awRows);
	virtual BOOL ModifyToDB(ROWCOL nRow);
	virtual BOOL ModifyToDB(CStringArray& raOldKey, CArray<void*, void*>& raKey, CArray<void*, void*>& raData);

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

	BOOL GetValue(ROWCOL nRow, UINT &key, T_VSEC_D &data);
	BOOL SetValue(ROWCOL nRow, const UINT &key, const T_VSEC_D &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const UINT &key, const T_VSEC_D &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_VSEC_D& data);
	void ConvDataToStr(int i, const T_VSEC_D& data, CString& value, const T_VSEC_K VsecK);

	void MakeSearchKey(const int &key, const T_VSEC_D &data, CStringArray &aKey);
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);
	void SetColumnAttr();

	void UpdateBuffer();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSectRForcGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void ConvertUnit();
	virtual ~CSectRForcGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSectRForcGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	static UINT WM_SECT_RFORC_GRID_VSEC_CLICKED;

public:
	int GetSelectedVsecKList(CArray<UINT, UINT>& aVsecK);

};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SECTRFORCGRID_H__)
