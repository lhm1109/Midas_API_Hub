#pragma once

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_common\wg_common_TBCommon.h"
#include "LoadCombCtrl_SP20.h"
class CDBDoc;

class CCmdLcomSP20LinksGrid : public CTBCommon
{
	// Construction
public:
	CCmdLcomSP20LinksGrid();

	// Attributes
public:
	static CString m_aCurDefVal[];

	// Operations
public:
	void Initialize(CDBDoc* pDoc, T_UNIT_SYSTEM &UnitSystem);
	void SetData(CArray<T_SP20_LINK_D, T_SP20_LINK_D&>* aLcaseSP20LinkData)
	{
		m_paLcaseSP20LinkData = aLcaseSP20LinkData;
	}

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdLcomSP20LinksGrid)
	//}}AFX_VIRTUAL
	//virtual void InitBrowserSettings();

	virtual int  GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	
	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, T_STLD_K &key, T_SP20_LINK_D &data);
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
	
	virtual BOOL OnPasteFromClipboard(const CGXRange &range); // Paste 阜绰促.
	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	
	CString GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol);

	// Implementation
public:
	virtual ~CCmdLcomSP20LinksGrid();
	BOOL GetValue(ROWCOL nRow, T_KEY& nIndex, T_SP20_LINK_D& data);
	BOOL SetValue(ROWCOL nRow, const T_KEY& nIndex, const T_SP20_LINK_D& data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const T_KEY& nIndex, const T_SP20_LINK_D& data, CRowColArray& awCols);
	void ConvStrToData(int i, CString& value, T_SP20_LINK_D& data);
	void ConvDataToStr(int i, const T_SP20_LINK_D& data, CString& value);
	
	void GetAllSelectedLcom(CArray<T_KEY, T_KEY>& aSelKey);
	void MakeSearchKey(const T_KEY&key, const T_SP20_LINK_D &data, CStringArray &aKey);
private:
	void GetStldNoListString(IN const CArray<T_KEY, T_KEY>& aIndex, OUT CString& value);
	void GetStldListByString(IN CString& value, OUT CArray<T_KEY, T_KEY>& aIndex);
	BOOL GetChangeList(IN const CArray<T_KEY, T_KEY>& aSrc, IN const CArray<T_KEY, T_KEY>& aChange, OUT CArray<T_KEY, T_KEY>& aIncreaseRow, OUT CArray<T_KEY, T_KEY>& aDecreaseRow);
	BOOL IncreaseListNo(IN ROWCOL nRow, IN T_KEY key,IN ROWCOL nCol);
	BOOL DecreaseListNo(IN ROWCOL nRow, IN T_KEY key,IN ROWCOL nCol);

	BOOL ContainsIndex(IN T_KEY index, IN CArray<T_KEY, T_KEY>& aIndex);

	void RemoveIndex(int nSource, int nSlot, int nIndex);
public:
	CString GetLCaseNameList();
	void GetOtherIndexList(IN T_KEY index,OUT CString& strValue,OUT CArray<T_KEY, T_KEY>& aIndex);
	void GetOtherIndexList(IN T_KEY index, IN CArray<T_KEY, T_KEY>& aExcludeIndex, OUT CString& strValue, OUT CArray<T_KEY, T_KEY>& aIndex);
	// Generated message map functions

	
protected:
	int  m_nLcomType;
	static BOOL m_bElastStag;
	static BOOL m_bTB10002_1_05;

	CArray<T_SP20_LINK_D, T_SP20_LINK_D&>* m_paLcaseSP20LinkData;
	
	BOOL IsActiveCol(ROWCOL& nCol);
	
	//{{AFX_MSG(CCmdLcomSP20LinksGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CArray<T_STLD_K, T_STLD_K> m_aSelectedKeys;
	CBCGPPopupMenu* m_pPopupMenu;
};

class CTBCheckLoadCaseListComboLBox : public CGXCheckListComboLBox
{
	DECLARE_DYNAMIC(CTBCheckLoadCaseListComboLBox)

	// Construction
public:
	CTBCheckLoadCaseListComboLBox(CWnd* pMsgWnd);
	virtual ~CTBCheckLoadCaseListComboLBox();

protected:
	// Generated message map functions
	//{{AFX_MSG(CTBCheckLoadCaseListComboLBox)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CTBCheckLoadCaseListComboBox : public CGXCheckListComboBox
{
	//GRID_DECLARE_CONTROL(CTBCheckLoadCaseListComboBox)  
	DECLARE_DYNAMIC(CTBCheckLoadCaseListComboBox)
	// static operations
public:
	CTBCheckLoadCaseListComboBox(CGXGridCore* pGrid) : CGXCheckListComboBox(pGrid) {}
	//virtual void Draw(CDC* pDC, CRect rect, ROWCOL nRow, ROWCOL nCol, const CGXStyle& style, const CGXStyle* pStandardStyle);
	virtual CWnd* CreateListBox(CWnd* pParentWnd, UINT nID);
	virtual void OnChangedValue();
	virtual BOOL GetControlText(CString& strResult, ROWCOL nRow, ROWCOL nCol, LPCTSTR pszRawValue, const CGXStyle& style);
	// Override these methods if you don't want to fill the list from the coice-list
	virtual void OnStoreDroppedList(CListBox* lbox);
	virtual void OnFillDroppedList(CListBox* lbox);
	//void SetStldArrayNo(const CArray<UINT, UINT>& aRef) { a++; m_aStldNo.Copy(aRef); }
protected:
	BOOL StoreChangedValue();
private:
	//CArray<UINT, UINT> m_aStldNo;
protected:
	//{{AFX_MSG(CTBCheckLoadCaseListComboBox)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CCmdLcomSP20LinksDlg : public CDialogMove
{
// Construction
public:
	CCmdLcomSP20LinksDlg(CWnd* pParent = NULL);   // standard constructor
	~CCmdLcomSP20LinksDlg();

	//{{AFX_DATA(CCmdLcomSP20LinksDlg)
	enum { IDD = IDD_CMD_LCOM_SP20_FACTOR_DLG };
	//}}AFX_DATA

	// Attributes
public:
	CDBDoc* m_pDoc;

protected:
	CCmdLcomSP20LinksGrid* m_pLcomPsiFactorGrid;
	CArray<T_SP20_LINK_D, T_SP20_LINK_D&> m_aLcaseSP20LinkData;
	CMap<CString, LPCTSTR, int, int> m_mapName2Index;
protected:
	//{{AFX_MSG(CCmdAutoLoadCombDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnCmdTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

