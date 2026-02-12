#if !defined(__SEIS_CVL_JP_SET_INVEST_SISR_DLG_H__)
#define __SEIS_CVL_JP_SET_INVEST_SISR_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSeisCvlJpSetInvestSisrDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "..\wg_db\wg_db_selectctrl.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestSisrDlg dialog

class CDBDoc;

class CSeisCvlJpSetInvestSisrDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
private:
	int			m_nOption;
	MComboBox	m_caseName;				//조사 케이스
	MComboBox	m_groupName;			//조사 대상
	int			m_calMethod;			//계산 방법
	//자동계산 조건
	MComboBox	m_DesignCriteria;		//적용기준
	BOOL		m_sigma_ce;				//σce (프로스트레스에 의한 응력)를 별도로 고려
	BOOL		m_extraByShearSpan;		//전단 스팬비에 의한 할증
	MComboBox	m_shearSpanType;		//전단스팬 산정
	MEdit		m_userLength;			//전단스팬a
	BOOL		m_ApplyCc;				//Cc=1.0 적용
	int			m_targetElements;		//적용요소 범위
	CSelectEditColor m_elemAuto;		//개별요소번호
	//직접 입력
	CSelectEditColor m_elemInput;		//대상요소
	BOOL			 m_bCommon;			//전단내력 위치
	MEdit			 m_IpulsSus;			// I 사인장파괴(Sus) or 전단내력(Ps)
	MEdit			 m_IpulsSuc;			// I 압축파괴(Suc)
	MEdit			 m_IminusSus;		// I 사인장파괴(Sus) or 전단내력(Ps)
	MEdit			 m_IminusSuc;		// I 압축파괴(Suc)
	MEdit			 m_JpulsSus;			// J 사인장파괴(Sus) or 전단내력(Ps)
	MEdit			 m_JpulsSuc;			// J 압축파괴(Suc)
	MEdit			 m_JminusSus;		// J 사인장파괴(Sus) or 전단내력(Ps)
	MEdit			 m_JminusSuc;		// J 압축파괴(Suc)
	CListCtrl		 m_userInputList;	//직접입력한 요소별 전단내력 리스트

	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aAutoCalcCtrl;
	CArray<UINT, UINT> m_aUserInputCtrl;
	CArray<UINT, UINT> m_aUserJCtrl;
	std::vector<T_SISR_USER_D> m_UserDataList;
	// Construction
public:
	CSeisCvlJpSetInvestSisrDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSeisCvlJpSetInvestSisrDlg();

	// Dialog Data
		//{{AFX_DATA(CSeisCvlJpSetInvestSisrDlg)
	enum { IDD = IDD_TM_SISR };

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	virtual	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//{{AFX_VIRTUAL(CSeisCvlJpSetInvestSisrDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSeisCvlJpSetInvestSisrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangedCaseCmb();
	afx_msg void OnChangedGroupCmb();
	afx_msg void OnChangedCalMethodRdo();
	afx_msg void OnChangedDesignCriteria();
	afx_msg void OnChangedExtraByShearSpanChk();
	afx_msg void OnChangedApplyCcChk();
	afx_msg void OnChangedShearSpanTypeCmb();
	afx_msg void OnChangedTargetElementsRdo();
	afx_msg void OnSetFocusElemAutoEdt();
	afx_msg void OnSetFocusElemInputEdt();
	afx_msg void OnNMClickUserInputList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddBtn();
	afx_msg void OnModBtn();
	afx_msg void OnDelBtn();
	afx_msg void OnClickCommonPosiChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual void Execute();

private:
	void InitMoveCtrl();
	void InitControl();
	void InitCaseNameCtrl();
	void InitGroupNameCtrl();
	void UpdateDesignCriteriaCtrl();
	void UpdateApplyCcChkCtrl();
	void InitShearSpanTypeCtrl();
	void UpdateUnitCtrl();
	void InitPositionTypeCtrl();
	void UpdateUserShearForceCtrl();
	void InitUserListCtrl();
	void UpdateUserListCtrl();
	void ClickCommonPosiChk();

	BOOL CheckData_User(BOOL bModify = FALSE);
	BOOL IsExistElemUser(const T_ELEM_K& kElem, BOOL bModify = FALSE);
	void DlgToData_User(OUT CArray<T_SISR_USER_D, T_SISR_USER_D>& aData);
	void InsertItem_User(const int& nIndex, IN T_SISR_USER_D& data);
	BOOL InsertItemUserListCtrl(T_SISR_USER_K Key, T_SISR_USER_D& Data);
	void UpdateUserListBySisrKey(const T_SISR_K& kSisr);
	CString DataToStr_User(const int& index, const T_SISR_USER_D& data);
	void UpdataDialog_User(const int& nItem);

	void ResetViewControls();

	//Execute() 관련함수
	BOOL AddOrModData();
	BOOL AddData(T_SISR_D& dSisr, CArray<T_SISR_USER_D, T_SISR_USER_D>& userList);
	BOOL ModData(const T_SISR_K& kSisr, T_SISR_D& dSisr, CArray<T_SISR_USER_D, T_SISR_USER_D>& userList);
	BOOL DelData();
	BOOL AddData_SisrUser(const T_SISR_K& kSisr, IN CArray<T_SISR_USER_D, T_SISR_USER_D>& userList);
	BOOL DelData_SisrUser(const T_SISR_K& kSisr);
	BOOL ModData_SisrUser(const T_SISR_K& kSisr, IN CArray<T_SISR_USER_D, T_SISR_USER_D>& userList);
	void DlgToData(OUT T_SISR_D& rData, OUT CArray<T_SISR_USER_D, T_SISR_USER_D>& userList);
	void DlgToData_Auto(T_SISR_AUTO_D& rData);
	void DlgToData_UserLisr(OUT CArray<T_SISR_USER_D, T_SISR_USER_D>& userList);
	BOOL CheckData(OUT T_SISR_D& rData);
	void GetGroupElements(const T_SIGR_K& kSigr, OUT T_ELEM_K_LIST& groupElemKeys);
	BOOL CheckData_UserList(const CArray<T_SISR_USER_D, T_SISR_USER_D>& userList);
	T_SISR_K GetExistingDataKey(const T_SISR_D& newData);
	BOOL CheckExistData_IJ(const T_SISR_USER_D& data, const int& nPos);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEIS_CVL_JP_SET_INVEST_SISR_DLG_H__)