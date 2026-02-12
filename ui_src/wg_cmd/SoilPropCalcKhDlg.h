#if !defined(__SOILPROP_CALC_KH_DLG_H__)
#define __SOILPROP_CALC_KH_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSoilPropCalcKhDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CSoilPropCalcKhDlg dialog
#include "HeaderPre.h"      /////////

enum EN_SOIL_SECTION
{
	EN_ONE_THARID, //지표면 ~ H * 1/3
	EN_TWO_THRID,  //H * 1/3 ~ H * 2/3
	EN_BEDROCK	   //H * 2/3 ~ 기반면
};

class __MY_EXT_CLASS__ CSoilPropCalcKhDlg : public CDialogMove
{
	// Construction
public:
	CSoilPropCalcKhDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSoilPropCalcKhDlg();

	enum { IDD = IDD_ETC_EP_SOIL_PROP_KH_CALC_DLG };

public:
	CDBDoc* m_pDoc;
	int m_nUseVsType; //0:by Level, 1:Average by Layer
	int m_nLayerType; //0:Single, 1:Double
	CEditUnit	m_editSecondLayer;
	CTextUnit	m_wndSecondLayerUnit;

	T_POSP_D m_Data;
	static const double s_dKh[7][3];
public:
	void SetData(T_POSP_D &rData) { m_Data = rData; }
	T_POSP_D GetData() { return m_Data; }

protected:
	BOOL Data2Dlg();
	BOOL Dlg2Data();
	void InitCtrl();
	double GetKh(const EN_SOIL_SECTION eSection, const double dVs);
	double GetKh_KDS17(const EN_SOIL_SECTION eSection, const T_POSP_BASE_DATA Base);
	double CalcSingleLayer(const double dTotalLevel);
	void CalcDoubleLayer(const double dTotalLevel, const double d2ndLayerPos, double dAverageVs[]);
	void CalcKh();
	void CalcKh_KDS17();
	EN_SOIL_SECTION GetSection(const double dOneThirdLevel, const double dTwoThirdLevel, const double dCurrentLevel);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnClickType();
	virtual void OnLayerType();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"      /////////

#endif // !defined(__SOILPROP_CALC_KH_DLG_H__)
