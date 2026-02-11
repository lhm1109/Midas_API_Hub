#pragma once
#include "CMDlgBase.h"
#include "..\wg_base\MouseEdit.h"
#include "..\wg_common\wg_common_TBCommon.h"

struct SFRGridData
{
	CString strName;
	double dF[3];
};


struct SFRListData
{
	int	nId;
	/*Multiple Support Excitation Load*/
	bool bCheck[3];
	double dScale[3];
	int		nTimeIndex[3];
};

class CSFR_RDFGrid : public CTBCommon
{
public:
	CSFR_RDFGrid();

	// Attributes
public:
	CArray<SFRGridData, SFRGridData&>* m_pData;
	int* m_nCurMode;
	static CString m_aCurDefVal[4];

	// Operations
public:
	void Initialize(CArray<SFRGridData, SFRGridData&>* pData,int* nCurMode);
	void Data2Grid();
	void Grid2Data();
	//BOOL DeleteRecordByKey(const double &key, const double &data);
	//BOOL InsertRecordByKey(const double &key, const double &data);

	virtual int GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize) {}
	virtual long GetSelectedKeys(CArray<long, long> &caKey) { return 0; }

	virtual BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, double &key, double &data) { return TRUE; }
	virtual BOOL ValidateField(CString value, int nColID) { return TRUE; }
	virtual BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols) { return TRUE; }

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
	virtual void MakeItemEx(long* aKey = 0, long nKeyNum = -2, BOOL bUnitChanged = FALSE);
	virtual void MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols);
	virtual void MakeItemNormal(long nNumData, CRowColArray& awCols);
	virtual void MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord) {}

	//BOOL GetValue(ROWCOL nRow, double &key, double &data);
	//void ConvStrToData(int i, CString& value, double& data);

	//void MakeSearchKey(const double &key, const double &data, CStringArray &aKey);
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);

public:
	virtual ~CSFR_RDFGrid();
	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};
//Swept Frequency Response Analysis
class CCmdSFRLoadDlg : public CCMDlgBase, public CDBUpdateConnector
{
// Construction
public:
	CCmdSFRLoadDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCmdSFRLoadDlg();
// Dialog Data
	//{{AFX_DATA(CCmdSFRLoadDlg)
	enum { IDD = IDD_CMD_LOAD_SFR_DLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdSFRLoadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//virtual void UpdateBuffer();
	//virtual void UnitChanged();

// Implementation
private:
	CMouseEdit	m_wndNodeNumber;
	double		m_RFEdit;
	int			m_NODFEdit;//Number of disturbing forces
	double		m_KEdit;
	double		m_WEdit;
	CEdit		m_eEdit;
	double		m_CFRRangeEdit1;
	double		m_CFRRangeEdit2;
	double		m_CFRPerEdit;
	double		m_CFRDampingEdit;
	double		m_DirEdit[3];

	CComboBox	m_DirCombox[3];
	CComboBox	m_AssignCombox;

	int m_nEndTime;
	int m_nIncrementTime;

	int m_nRDFMode;
	CListCtrl m_SFRList;
	int m_nCurSelIndex;
protected:
	CDBDoc* m_pDoc;
	CSFR_RDFGrid m_wndGrid;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void OnCancel(){}
	afx_msg void OnCmdSFRClose();
	afx_msg void ChangeRDFMode();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnDel();
	afx_msg void CheckFreqRange();
	afx_msg void CheckFreqEige();
	afx_msg void CheckXDir();
	afx_msg void CheckYDir();
	afx_msg void CheckZDir();
	afx_msg void OnApply();
	afx_msg void OnNODFNumSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void InitVibratingMachineData();
	void InitSFRAnalysisSetting();
	void InitGrid();
	void UpdateGrid();
	void ChangeRFNum();
	void InitSFRList();
	BOOL CalcSinu(double dFreq, double dF,CArray<T_THFC_BASE, T_THFC_BASE&> &aThisFuncData);
	CArray<SFRGridData, SFRGridData&> m_SFRGridData;
	typedef CArray<T_NODE_K, T_NODE_K> T_NODE_K_LIST;
	CArray<T_NODE_K_LIST, T_NODE_K_LIST&> m_aNodeK;

	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	void SetSFRMSELData(SFRListData* pUserData);
	void UpdateSFRMSELData(const SFRListData* pUserData);
};

