
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

class __MY_EXT_CLASS__ CCmdDampingGBCoefCalcDlg : public CDialogMove//, public CDBUpdateConnector
{
	// Construction
public:
	CCmdDampingGBCoefCalcDlg(CWnd* pParent = NULL);   // standard constructor

	enum {IDD = IDD_CMD_LOAD_DAMPING_GB_COEF_DLG};
	CComboBox	m_CodeCombox;
	CComboBox	m_BldCombox;
	CComboBox	m_StruCombox;
	CListCtrl	m_CaseList[3];
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CDBDoc*	m_pDoc;
	T_DAGB_D m_Data;
	void Data2Dlg();
	BOOL Dlg2Data();
	void SetCodeCmb();
	void SetBuildingCategoryCmb();
	void SetStructureCategoryCmb();
	void InitCacheList();
	void UpdateCaseNewList();
	void AutomaticAddList(UINT Key, int nRsType, int nCodeType = 0);
	int AutomaticGetSpfcListType(UINT Key, int nCodeType);
	/*nType : 0 list1->list2 ; 1 list2->list1 ; 2 list1->list3 ; 3 list3->list1 */
	void OnMoveList(int nType);
private:
	CArray<T_CASE_TYPE, T_CASE_TYPE> m_ListData[2][3];
	CArray<UINT, UINT> m_aCacheList[2][2][3];

public:
	virtual BOOL OnInitDialog();
	//virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
protected:
	afx_msg void OnList1ToList2Btn();
	afx_msg void OnList1ToList3Btn();
	afx_msg void OnList2ToList1Btn();
	afx_msg void OnList3ToList1Btn();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnSelChangeCodeList();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

