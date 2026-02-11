#pragma once

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_common\wg_common_TBCommon.h"
#include "LoadCombCtrl_SP20.h"
class CDBDoc;

/*
* This grid can change the loadcaseType, and is only used for generating automatic combinations.
* It is not saved in the DataBase. e.g. changing a loadcase type from short-term to long-term is not saved in T_LCTP_D.
*/
class CCmdLcomSP20ReliabilityGrid : public CTBCommon
{
	// Construction
public:
	CCmdLcomSP20ReliabilityGrid();

	// Attributes
public:
	static CString m_aCurDefVal[];

	// Operations
public:
	void Initialize(CDBDoc* pDoc, T_UNIT_SYSTEM &UnitSystem);
	void SetData(CArray<T_SP20_RCFI_D, T_SP20_RCFI_D&>* mLcomPsiFactor)
	{
		m_paLcomRcfiFactor = mLcomPsiFactor;
	}

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdLcomSP20ReliabilityGrid)
	//}}AFX_VIRTUAL
	
	virtual int  GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	
	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, T_STLD_K &key, T_SP20_RCFI_D &data);
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
	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol) override;

	CString GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol);

	// Implementation
public:
	virtual ~CCmdLcomSP20ReliabilityGrid();
	BOOL GetValue(ROWCOL nRow, DGN_LCASE_K& key, T_SP20_RCFI_D&data);
	BOOL SetValue(ROWCOL nRow, const DGN_LCASE_K&key, const T_SP20_RCFI_D&data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const DGN_LCASE_K&key, const T_SP20_RCFI_D&data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_SP20_RCFI_D& data);
	void ConvDataToStr(int i, const T_SP20_RCFI_D& data, CString& value);
	
	void GetAllSelectedLcom(CArray<T_STLD_K, T_STLD_K>& aSelKey);
	void MakeSearchKey(const T_STLD_K &key, const T_SP20_RCFI_D&data, CStringArray &aKey);

private:
	CString GetLoadNameList();
	CString GetLoadTypeList();

	// Generated message map functions
protected:

	CArray<T_SP20_RCFI_D, T_SP20_RCFI_D&>* m_paLcomRcfiFactor;
	
	BOOL IsActiveCol(ROWCOL& nCol);
	
	//{{AFX_MSG(CCmdLcomSP20ReliabilityGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CArray<T_STLD_K, T_STLD_K> m_aSelectedKeys;
};

class CCmdLcomSP20ReliabilityFactorDlg : public CDialogMove
{
// Construction
public:
	CCmdLcomSP20ReliabilityFactorDlg(CWnd* pParent = NULL);   // standard constructor
	~CCmdLcomSP20ReliabilityFactorDlg();

	//{{AFX_DATA(CCmdLcomSP20ReliabilityFactorDlg)
	enum { IDD = IDD_CMD_LCOM_PSI_FACTOR_DLG };
	//}}AFX_DATA

	// Attributes
public:
	CDBDoc* m_pDoc;

protected:
	CCmdLcomSP20ReliabilityGrid* m_pLcomPsiFactorGrid;
	CArray<T_SP20_RCFI_D, T_SP20_RCFI_D&> m_aLcomRcfiFactor;

protected:
	//{{AFX_MSG(CCmdAutoLoadCombDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

