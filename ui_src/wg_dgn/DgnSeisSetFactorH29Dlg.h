#if !defined(__DgnSeisSetFactorH29Dlg_H__)
#define __DgnSeisSetFactorH29Dlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisSetFactorH29Dlg.h : header file IDD_DGN_SEIS_SET_FACTOR_H29_DLG

#include "..\wg_base\wg_base_DialogMove.h"

#include "HeaderPre.h"

class CDBDoc;
class  CDgnSeisSetFactorH29_RcPierGrid;
class  CDgnSeisSetFactorH29_StlPierGrid;
class CDgnSeisSetFactorH29_CurvatureGrid;
class CDgnSeisSetFactorH29_MomentMydGrid;
class CDgnSeisSetFactorH29_MomentMudGrid;
class CDgnSeisSetFactorH29_ShearSusdGrid;
class CDgnSeisSetFactorH29_ShearSucdGrid;
class __MY_EXT_CLASS__ CDgnSeisSetFactorH29Dlg : public CDialogMove
{
public:
	CDgnSeisSetFactorH29Dlg(CWnd* pParent = NULL);
	~CDgnSeisSetFactorH29Dlg();

	// Dialog Data
	//{{AFX_DATA(CDgnSeisSetFactorH29Dlg)
	enum { IDD = IDD_DGN_SEIS_SET_FACTOR_H29_DLG };

protected:
	afx_msg void OnCancel();
	afx_msg void OnOk();
	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	void InitEditBox(const T_SIFA_H29_D& rData);
	void InitAllGrid(const T_SIFA_H29_D& rData);
	void GetGridData(T_SIFA_H29_D& rData);
	void  DelResource();
private:
	CDBDoc* m_pDoc;

	CDgnSeisSetFactorH29_RcPierGrid* m_RcPierGrid;
	CDgnSeisSetFactorH29_StlPierGrid* m_StlPierGrid;
	CDgnSeisSetFactorH29_CurvatureGrid* m_CurvatureGrid;
	CDgnSeisSetFactorH29_MomentMydGrid* m_MomentMydGrid;
	CDgnSeisSetFactorH29_MomentMudGrid* m_MomentMudGrid;
	CDgnSeisSetFactorH29_ShearSusdGrid* m_ShearSusdGrid;
	CDgnSeisSetFactorH29_ShearSucdGrid* m_ShearSucdGrid;

	double m_RcPierK;
	double m_RcPierK2;
	double m_StlPierK;
};
#include "HeaderPost.h"
#endif // !defined(__DgnSeisSetFactorH29Dlg_H__)