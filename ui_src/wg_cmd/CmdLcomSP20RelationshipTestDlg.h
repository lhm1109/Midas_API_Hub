#pragma once

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_common\wg_common_TBCommon.h"
#include "LoadCombCtrl_SP20.h"
class CDBDoc;

class CCmdLcomSP20TestGrid : public CTBCommon
{
	// Construction
public:
	CCmdLcomSP20TestGrid();

	// Attributes
public:
	//static CString m_aCurDefVal[];

	// Operations
public:
	void Initialize(T_UNIT_SYSTEM &UnitSystem);
	void SetData(const CArray<T_SP20_LINK_D, T_SP20_LINK_D&>& aLinkData, CArrayEx<CArrayEx<T_KEY, T_KEY>, CArrayEx<T_KEY, T_KEY>&>* pGraphIndex);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdLcomSP20PsiFactorGrid)
	//}}AFX_VIRTUAL
	
	virtual int  GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	
	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, DGN_LCASE_K& key, T_SP20_LPSI_D &data);
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
	virtual ~CCmdLcomSP20TestGrid();
	void ConvStrToData(int i, CString& value, T_SP20_LPSI_D& data);
	void ConvDataToStr(int i, const T_SP20_LPSI_D& data, CString& value);
	
	void GetAllSelectedLcom(CArray<DGN_LCASE_K, DGN_LCASE_K >& aSelKey);
	void MakeSearchKey(const DGN_LCASE_K&key, const T_SP20_LPSI_D &data, CStringArray &aKey);

	// Generated message map functions
protected:
	CArrayEx<CArrayEx<T_KEY, T_KEY>, CArrayEx<T_KEY, T_KEY>&>* m_pGraphIndex;
	//int m_nTolRow;
	BOOL IsActiveCol(ROWCOL& nCol);
	
	//{{AFX_MSG(CCmdLcomSP20PsiFactorGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CCmdLcomSP20TestPathDlg : public CDialogMove
{
// Construction
public:
	CCmdLcomSP20TestPathDlg(CWnd* pParent = NULL);   // standard constructor
	~CCmdLcomSP20TestPathDlg();

	//{{AFX_DATA(CCmdLcomSP20PsiFactorDlg)
	enum { IDD = IDD_CMD_LCOM_SP20_FACTOR_DLG };
	//}}AFX_DATA

	// Attributes
public:
	void SetData(const CArray<T_SP20_LINK_D, T_SP20_LINK_D&>& aLcaseSP20RcfiFactor);

protected:
	CCmdLcomSP20TestGrid* m_pTestGrid;
	CArray<T_SP20_LINK_D, T_SP20_LINK_D&> m_aLcaseSP20RcfiFactor;
	CArrayEx<CArrayEx<T_KEY, T_KEY>, CArrayEx<T_KEY, T_KEY>&> m_aGraphIndex;
	int m_nTolRow;
protected:
	//{{AFX_MSG(CCmdAutoLoadCombDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

