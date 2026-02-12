#if !defined(__CMDLCOMPSIFACTORDLG_H__)
#define __CMDLCOMPSIFACTORDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdLcomPsiFactorDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_common\wg_common_TBCommon.h"

/////////////////////////////////////////////////////////////////////////////
// CCmdLcomPsiFactorGrid window

class CCmdLcomPsiFactorGrid : public CTBCommon
{
	// Construction
public:
	CCmdLcomPsiFactorGrid();

	// Attributes
public:
	static CString m_aCurDefVal[];

	// Operations
public:
	void Initialize(CDBDoc* pDoc, T_UNIT_SYSTEM &UnitSystem);
	void SetData(CMapEx<UINT, UINT, T_LPSI_D, T_LPSI_D&>& mLcomPsiFactor)
	{
		m_mIndex.RemoveAll();
		m_mLcomPsiFactor.RemoveAll();
		UINT StldK;
		T_LPSI_D LpsiD;
		POSITION Pos = mLcomPsiFactor.GetStartPosition();
		while(Pos)
		{
			LpsiD.Initialize();
			mLcomPsiFactor.GetNextAssoc(Pos, StldK, LpsiD);
			m_mLcomPsiFactor.SetAt(StldK, LpsiD);
			m_mIndex.SetAt(LpsiD.nIndex, StldK);
		} 
	}

	void GetData(CMapEx<UINT, UINT, T_LPSI_D, T_LPSI_D&>& mLcomPsiFactor)
	{
		mLcomPsiFactor.RemoveAll();
		CArray<int, int> aIndex; aIndex.RemoveAll();
		m_mIndex.GetKeyList(aIndex);

		T_STLD_K StldK=NULL;
		T_LPSI_D LpsiD;
		int nIndexCount = m_mIndex.GetCount();
		for(int nIndex=1; nIndex<=nIndexCount; nIndex++)
		{
			if(!m_mIndex.Lookup(nIndex, StldK)) {ASSERT(0); continue;}
			if(!m_mLcomPsiFactor.Lookup(StldK, LpsiD)) {ASSERT(0); continue;}

			LpsiD.nIndex = nIndex;
			mLcomPsiFactor.SetAt(StldK, LpsiD);
		}
	}

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdLcomPsiFactorGrid)
	//}}AFX_VIRTUAL
	
	virtual int  GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	
	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, T_STLD_K &key, T_LPSI_D &data);
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
	
	virtual BOOL OnPasteFromClipboard(const CGXRange &range); // Paste ¸·´Â´Ù.
	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	
	CString GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol);

	// Implementation
public:
	virtual ~CCmdLcomPsiFactorGrid();
	BOOL GetValue(ROWCOL nRow, T_STLD_K &key, T_LPSI_D &data);
	BOOL SetValue(ROWCOL nRow, const T_STLD_K &key, const T_LPSI_D &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const T_STLD_K &key, const T_LPSI_D &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_LPSI_D& data);
	void ConvDataToStr(int i, const T_LPSI_D& data, CString& value);
	
	void GetAllSelectedLcom(CArray<T_STLD_K, T_STLD_K>& aSelKey);
	void MakeSearchKey(const T_STLD_K &key, const T_LPSI_D &data, CStringArray &aKey);

public:
	CString GetStldList();

	// Generated message map functions
protected:
	int  m_nLcomType;
	static BOOL m_bElastStag;
	static BOOL m_bTB10002_1_05;

	CMapEx<UINT, UINT, T_LPSI_D, T_LPSI_D&> m_mLcomPsiFactor;
	CMapEx<int, int, UINT, UINT&> m_mIndex;
	
	BOOL IsActiveCol(ROWCOL& nCol);
	
	//{{AFX_MSG(CCmdLcomPsiFactorGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CArray<T_STLD_K, T_STLD_K> m_aSelectedKeys;
	CBCGPPopupMenu* m_pPopupMenu;
};

/////////////////////////////////////////////////////////////////////////////
// CCmdLcomPsiFactorDlg dialog

class CCmdLcomPsiFactorDlg : public CDialogMove
{
// Construction
public:
	CCmdLcomPsiFactorDlg(CWnd* pParent = NULL);   // standard constructor
	~CCmdLcomPsiFactorDlg();

// Dialog Data
	//{{AFX_DATA(CCmdLcomPsiFactorDlg)
	enum { IDD = IDD_CMD_LCOM_PSI_FACTOR_DLG };
	//}}AFX_DATA

	// Attributes
public:
	CDBDoc* m_pDoc;
	void SetData(CMapEx<UINT, UINT, T_LPSI_D, T_LPSI_D&>& mLcomPsiFactor)
	{
		m_mLcomPsiFactor.RemoveAll();
		UINT StldK;
		T_LPSI_D LpsiD;
		POSITION Pos = mLcomPsiFactor.GetStartPosition();
		while(Pos)
		{
			LpsiD.Initialize();
			mLcomPsiFactor.GetNextAssoc(Pos, StldK, LpsiD);
			m_mLcomPsiFactor.SetAt(StldK, LpsiD);
		}

		if(m_pLcomPsiFactorGrid!=NULL)
		{
			m_pLcomPsiFactorGrid->SetData(mLcomPsiFactor);
		}
	}

	void GetData(CMapEx<UINT, UINT, T_LPSI_D, T_LPSI_D&>& mLcomPsiFactor)
	{
		mLcomPsiFactor.RemoveAll();
		UINT StldK;
		T_LPSI_D LpsiD;
		POSITION Pos = m_mLcomPsiFactor.GetStartPosition();
		while(Pos)
		{
			LpsiD.Initialize();
			m_mLcomPsiFactor.GetNextAssoc(Pos, StldK, LpsiD);
			mLcomPsiFactor.SetAt(StldK, LpsiD);
		}
	}

protected:
	CCmdLcomPsiFactorGrid* m_pLcomPsiFactorGrid;
	CMapEx<UINT, UINT, T_LPSI_D, T_LPSI_D&> m_mLcomPsiFactor;

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

#endif // !defined(AFX_CMDLCOMPSIFACTORDLG_H__9E765C21_0CC3_11D4_888F_0000C0F30D4D__INCLUDED_)