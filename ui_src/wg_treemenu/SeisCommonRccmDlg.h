#if !defined(__SEISCOMMONRCCMDLG_H__)
#define      __SEISCOMMONRCCMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CSeisCommonRccmDlg 대화 상자입니다.
#include "..\wg_base\wg_base_ControlEx.h"

class CSeisCommonRccmDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CSeisCommonRccmDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSeisCommonRccmDlg();

public:
	virtual void Execute();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TM_SEISCOMMON_RCCM };

protected:
	CDBDoc* m_pDoc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void GetSelctedRccmKeyList(CArray<T_ELEM_K,T_ELEM_K>& aSelectedRccmK);
	void GetRccmKeyList(CArray<T_ELEM_K,T_ELEM_K>& aRccmK);
	void GetSelctedColumnList(CArray<T_ELEM_K,T_ELEM_K>& aColmK);

protected:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
public:
	int m_nAddDel;
	int m_nColmType;
};
#endif