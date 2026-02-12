#if !defined(__CMSTAGESPREADGRID_H__)
#define __CMSTAGESPREADGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStageStdgSubGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"

union T_KEYSTEP
{
	unsigned int keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};

class CCMLoadESEQMainGrid;

/////////////////////////////////////////////////////////////////////////////
// CCMStageSpreadGrid window
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMStageSpreadGrid : public CTBCommon
{
// Construction
public:
	CCMStageSpreadGrid();

// Attributes
public:
	CArray<T_ESEQ_BASE, T_ESEQ_BASE&> m_aData;
	CArray<int, int> m_aStepNum;           // Index - step  현재 인덱스의 데이터에 들어있는 스텝개수 
	CMap<unsigned int, unsigned int, int, int> m_KeyMapper;      // 키 - Index
	CMap<unsigned int, unsigned int, int, int> m_SubKeyMapper;   // 키 - Sub Index
	unsigned int m_CurKey;
	
	static CString m_aCurDefVal[9];

// Operations
public:
	void Initialize();
	BOOL DeleteRecordByKey(const T_KEYSTEP &key, const T_ESEQ_BASE &data);
	BOOL InsertRecordByKey(const T_KEYSTEP &key, const T_ESEQ_BASE &data);

	virtual int GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(T_KEYSTEP *keys, int nSize);
	virtual long GetSelectedKeys(CArray<T_KEYSTEP, T_KEYSTEP> &caKey);

	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, T_KEYSTEP &key, T_ESEQ_BASE &data);
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
	virtual void MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord);  BOOL GetValue(ROWCOL nRow, T_KEYSTEP &key, T_ESEQ_BASE &data);
	
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual void OnCanceledEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);

	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);

	BOOL SetValue(ROWCOL nRow, const T_KEYSTEP &key, const T_ESEQ_BASE &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const T_KEYSTEP &key, const T_ESEQ_BASE &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_ESEQ_BASE& data);
	void ConvDataToStr(int i, const T_ESEQ_BASE& data, CString& value);

	void MakeSearchKey(const T_KEYSTEP &key, const T_ESEQ_BASE &data, CStringArray &aKey);
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);

	BOOL FindData(T_KEYSTEP key, int &nIndex, int &nSubIndex);
	BOOL ExistAllKey(CArray<T_KEYSTEP, T_KEYSTEP>& aKey, T_KEYSTEP& errorKey);
	BOOL AddData(T_KEYSTEP key, T_ESEQ_BASE &data, BOOL bCheck=TRUE);
	BOOL AddData(CArray<T_KEYSTEP, T_KEYSTEP> &aKey, CArray<T_ESEQ_BASE, T_ESEQ_BASE&> &aData, BOOL bNotify=TRUE);
	BOOL DelData(T_KEYSTEP key);
	BOOL DelData(CArray<T_KEYSTEP, T_KEYSTEP> &aKey, BOOL bNotify=TRUE);
	BOOL ModifyData(T_KEYSTEP oldKey, T_KEYSTEP key, T_ESEQ_BASE data);
	BOOL ModifyData(CArray<T_KEYSTEP, T_KEYSTEP> &aOldKey, CArray<T_KEYSTEP, T_KEYSTEP> &aKey, CArray<T_ESEQ_BASE, T_ESEQ_BASE&> &aData);

	void AddKeyMapper(CMap<unsigned int, unsigned int, int, int>* pKeyMapper, unsigned int step, int ix, BOOL bSub=FALSE);
	void DelKeyMapper(CMap<unsigned int, unsigned int, int, int>* pKeyMapper, unsigned int step, int ix, BOOL bSub=FALSE);
	void UpdateBuffer(int nCmd, CArray<T_KEYSTEP, T_KEYSTEP> &aOldKey, CArray<T_KEYSTEP, T_KEYSTEP> &aKey, CArray<T_ESEQ_BASE, T_ESEQ_BASE&> &aData);

	BOOL CheckStoryLevel(T_KEYSTEP oldkey, T_KEYSTEP key, T_ESEQ_BASE &data, BOOL bModify);
	BOOL CheckStoryLevel(CArray<T_KEYSTEP, T_KEYSTEP> &aOldkey, CArray<T_KEYSTEP, T_KEYSTEP> &akey, 
																				 CArray<T_ESEQ_BASE, T_ESEQ_BASE&> &aData, BOOL bModify);
	void SetModelMaterialColumn(ROWCOL nRow, ROWCOL nCol, int nType);
	void SetModelStoryColumn(ROWCOL nCol, BOOL bNone);
	BOOL GetDataIndex(int nCount, int &nIndex, int &nSubIndex);
	BOOL GetKeyByIndex(int nIndex, int nSubIndex, T_KEYSTEP &rKey);
	void ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol);
	void ChangeModelStep(ROWCOL nRow, T_KEYSTEP step);
	void ChangeModelType(ROWCOL nRow, int nType);
	void SetStartStoryName();
	void EnableDisableByStep(CArray<T_KEYSTEP, T_KEYSTEP> &aKey);

	int GetModelTypeData(CString &str);
	CString GetModelTypeStr(int data);
	int GetModelMatlData(int nType, CString &str);
	CString GetModelMatlStr(int nType, int data);
	int GetStoryKey(CString &csName);
	CString GetStoryName(int key);
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);
	//BOOL IsExistStep(T_KEYSTEP step, unsigned int &rKey);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStageSpreadGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMStageSpreadGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMStageSpreadGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSTAGESPREADGRID_H__)
