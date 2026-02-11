
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\EditUnit.h"

#include "HeaderPre.h"

struct __MY_EXT_CLASS__ T_CASE_TYPE
{
	//int nIndex;
	int nType; //0:时程 1:反应谱
	UINT nKey;
	CString strName;
	void Initialize()
	{
		//nIndex = -1;
		nType = -1;
		nKey = 0;
		//strName = _T("");
	}
};

class __MY_EXT_CLASS__ CCmdIsolatorGBCoefCalcDlg : public CDialogMove, public CDBUpdateConnector
{
	// Construction
public:
	CCmdIsolatorGBCoefCalcDlg(CWnd* pParent = NULL);   // standard constructor

	enum {IDD = IDD_CMD_LOAD_ISOLATOR_GB_COEF_DLG};
	CComboBox	m_CodeCombox;
	CComboBox	m_BldCombox;
	CComboBox	m_FortificationCombox;
	CComboBox	m_StruCombox;
	CComboBox	m_SeisLayerCombox;
	CComboBox	m_UpLayerCombox;
	CComboBox	m_LcomCombox;
	CListCtrl	m_CaseList1;
	CListCtrl	m_CaseList2;
	CListCtrl	m_CaseList3;
	int m_nLmtType;

	CArray<T_STLD_K, T_STLD_K> aResistCase;
	CArray<T_STLD_K, T_STLD_K> aRareECase;
	CArray<T_STLD_K, T_STLD_K> aExtreRareECase;
private:
	int m_nPreCode;
	int m_nTempStrutCategory[2];
	CArray<T_STLD_K, T_STLD_K> aBJResistCase;
	CArray<T_STLD_K, T_STLD_K> aBJRareECase;
	CArray<T_STLD_K, T_STLD_K> aBJExtreRareECase;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	// Implementation
protected:
	CDBDoc*	m_pDoc;
	T_ISGB_D m_Data;
	int m_nSeisStor;
	int m_nUpStor;
	int m_nLcom;
	void Data2Dlg();
	BOOL Dlg2Data();
	void SetCodeCmb();
	void SetBuildingCategoryCmb();
	void SetFortificationCategoryCmb();
	void SetStructureCategoryCmb();
	void SetIsolationSeisLayerCmb();
	void SetIsolationUpLayerCmb();
	void SetLcomGCmb();
	CArray<T_SPLC_K, T_SPLC_K> m_SplcNewKeys;
	CArray<T_THIS_K, T_THIS_K> m_ThisNewKeys;
protected:
	CArray<T_CASE_TYPE, T_CASE_TYPE> m_TempListData[2][3];
	CArray<UINT,UINT> m_aCacheList[2][2][3];
	
	/*
	* 在UpdateCaseList中,原:隔标中的IsList1ExistSplc..等函数直接从数据库中判断List表格是否存在.
	* 改为从缓冲中判断,缓冲在初始化从数据库赋值...缓冲记录多个规范下的不同list状态..使两个规范的list独立存在;
	* Key		: T_SPLC_K或T_THIS_K
	* nCodeType,nRsType,nListIndex : m_aCacheList[nCodeType][nRsType][nListIndex]
	*/
	void InitCacheList();
	void AutomaticAddList(UINT Key, int nRsType, int nCodeType = 0);
	void UpdateCaseNewList();
	int AutomaticGetSpfcListType(UINT Key, int nCodeType);
	/*nType : 0 list1->list2 ; 1 list2->list1 ; 2 list1->list3 ; 3 list3->list1 */
	void OnMoveList(int nType);
public:
	virtual BOOL OnInitDialog();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
protected:
	afx_msg void OnList1ToList2Btn();
	afx_msg void OnList1ToList3Btn();
	afx_msg void OnList2ToList1Btn();
	afx_msg void OnList3ToList1Btn();
	afx_msg void OnGeneralLcomBtn();
	afx_msg void OnUserDefineStressLmtBtn();
	afx_msg void OnCmdLmtTypeRdo();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnSelChangeLcomList();
	afx_msg void OnSelChangeCodeList();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

