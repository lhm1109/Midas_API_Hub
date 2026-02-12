#if !defined(__CMDLCOMLIVELOADTYPEDLG_H__)
#define __CMDLCOMLIVELOADTYPEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdLcomLiveLoadTypeDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_common\wg_common_TBCommon.h"

/////////////////////////////////////////////////////////////////////////////
// CCmdLcomLiveLoadTypeGrid window

struct T_LLLT_Data
{
	int nIndex; // 그리드에 출력시 정렬용으로만 사용. (1부터 시작)
	T_STLD_K StldKey;
	CString LoadCaseName;
	T_LLLT_D::enLiveLoadType  LLType;

	void Initialize()
	{
		nIndex = 0;
		StldKey = NULL;
		LoadCaseName.Empty();
		LLType = T_LLLT_D::EN_TYPE_LLMAX;
	}
	BOOL operator==(const T_LLLT_Data& src) const
	{
		if (LLType != src.LLType) return FALSE;
		return TRUE;
	}
};

class CCmdLcomLiveLoadTypeGrid : public CTBCommon
{
	// Construction
public:
	CCmdLcomLiveLoadTypeGrid();

	// Attributes
public:
	static CString m_aCurDefVal[];

	// Operations
public:
	void Initialize(CDBDoc* pDoc, T_UNIT_SYSTEM &UnitSystem);
	void SetData(CMapEx<UINT, UINT, T_LLLT_Data, T_LLLT_Data&>& mLcomLiveLoadType)
	{
		m_mIndex.RemoveAll();
		m_mLcomLiveLoadType.RemoveAll();
		UINT StldK;
		T_LLLT_Data LlltD;
		POSITION Pos = mLcomLiveLoadType.GetStartPosition();
		while(Pos)
		{
			LlltD.Initialize();
			mLcomLiveLoadType.GetNextAssoc(Pos, StldK, LlltD);
			m_mLcomLiveLoadType.SetAt(StldK, LlltD);
			m_mIndex.SetAt(LlltD.nIndex, StldK);
		} 
	}

	void GetData(CMapEx<UINT, UINT, T_LLLT_Data, T_LLLT_Data&>& mLcomLiveLoadType)
	{
		mLcomLiveLoadType.RemoveAll();
		CArray<int, int> aIndex; aIndex.RemoveAll();
		m_mIndex.GetKeyList(aIndex);

		T_STLD_K StldK=NULL;
		T_LLLT_Data LlltD;
		int nIndexCount = m_mIndex.GetCount();
		for(int nIndex=1; nIndex<=nIndexCount; nIndex++)
		{
			if(!m_mIndex.Lookup(nIndex, StldK)) {ASSERT(0); continue;}
			if(!m_mLcomLiveLoadType.Lookup(StldK, LlltD)) {ASSERT(0); continue;}

			LlltD.nIndex = nIndex;
			mLcomLiveLoadType.SetAt(StldK, LlltD);
		}
	}

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdLcomLiveLoadTypeGrid)
	//}}AFX_VIRTUAL
	
	virtual int  GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	
	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, T_STLD_K &key, T_LLLT_Data &data);
	virtual BOOL ValidateField(CString value, int nColID);
	virtual BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols);
	
	virtual long GetSelectedKeys(CArray<long, long> &caKey);
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
	
	virtual void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit);
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	
	virtual BOOL OnPasteFromClipboard(const CGXRange &range); // Paste 막는다.
	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	
	CString GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol);

	// Implementation
public:
	virtual ~CCmdLcomLiveLoadTypeGrid();
	BOOL GetValue(ROWCOL nRow, T_STLD_K &key, T_LLLT_Data &data);
	BOOL SetValue(ROWCOL nRow, const T_STLD_K &key, const T_LLLT_Data &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const T_STLD_K &key, const T_LLLT_Data &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_LLLT_Data& data);
	void ConvDataToStr(int i, const T_LLLT_Data& data, CString& value);
	
	void GetAllSelectedLcom(CArray<T_STLD_K, T_STLD_K>& aSelKey);
	void MakeSearchKey(const T_STLD_K &key, const T_LLLT_Data &data, CStringArray &aKey);

public:
	CString GetStldList();
	CString GetTypeList();

	// Generated message map functions
protected:
	int  m_nLcomType;
	static BOOL m_bElastStag;
	static BOOL m_bTB10002_1_05;

	CMapEx<UINT, UINT, T_LLLT_Data, T_LLLT_Data&> m_mLcomLiveLoadType;
	CMapEx<int, int, UINT, UINT&> m_mIndex;
	
	
	//{{AFX_MSG(CCmdLcomLiveLoadTypeGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CArray<T_STLD_K, T_STLD_K> m_aSelectedKeys;
	CBCGPPopupMenu* m_pPopupMenu;
};

/////////////////////////////////////////////////////////////////////////////
// CCmdLcomLiveLoadTypeDlg dialog

class CCmdLcomLiveLoadTypeDlg : public CDialogMove
{
// Construction
public:
	CCmdLcomLiveLoadTypeDlg(CWnd* pParent = NULL);   // standard constructor
	~CCmdLcomLiveLoadTypeDlg();

// Dialog Data
	//{{AFX_DATA(CCmdLcomLiveLoadTypeDlg)
	enum { IDD = IDD_CMD_LCOM_LIVE_LOAD_TYPE_DLG };
	//}}AFX_DATA

	// Attributes
public:
	CDBDoc* m_pDoc;

	void GetData(CMapEx<UINT, UINT, T_LLLT_Data, T_LLLT_Data&>& mLcomLiveLoadType)
	{
		mLcomLiveLoadType.RemoveAll();
		UINT StldK;
		T_LLLT_Data LlltD;
		POSITION Pos = m_mLcomLiveLoadType.GetStartPosition();
		while(Pos)
		{
			LlltD.Initialize();
			m_mLcomLiveLoadType.GetNextAssoc(Pos, StldK, LlltD);
			mLcomLiveLoadType.SetAt(StldK, LlltD);
		}
	}

protected:
	CCmdLcomLiveLoadTypeGrid* m_pLcomLiveLoadTypeGrid;
	CMapEx<UINT, UINT, T_LLLT_Data, T_LLLT_Data&> m_mLcomLiveLoadType;

protected:
// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadCombDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDLCOMLIVELOADTYPEDLG_H__)