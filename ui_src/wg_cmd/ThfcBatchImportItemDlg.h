#pragma once

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_MySRGraph.h"


#include "HeaderPre.h"
class CCMThfcRpsrProgDlg;
struct ThfcCHData
{
	CString strName;
	CArray <double, double> m_arTime;
	CArray <double, double> m_arAccel;
	int     nIntensity;		//抗震设防烈度
	int     nEffect;		//水平系数最大值(多遇, 设防, 罕遇,极罕遇)
	BOOL	bAutoTime;
	BOOL	bAdjust;
	double	dTolTime;
	double	dStepTime;
	double	dStartTime;
	double	dEndTime;
	double	dEPA;
	double	dEPAScaleFactor;
	int		nFileIndex;
	int		nDataType;//0 .sgs 1 YJK 2 PK 
	ThfcCHData()
	{
		strName.Empty();
		m_arTime.RemoveAll();
		m_arAccel.RemoveAll();
		nIntensity	= 0;
		nEffect		= 0;
		bAutoTime	= FALSE;
		bAdjust		= FALSE;
		dTolTime	= 0.0;
		dStepTime	= 0.0;
		dStartTime	= 0.0;
		dEndTime	= 0.0;
		dEPA		= 0.0;
		dEPAScaleFactor	= 0.0;
		nFileIndex = 0;
		nDataType = 0;
	}
	ThfcCHData& operator=(const ThfcCHData& src)
	{
		strName			= src.strName;
		nIntensity		= src.nIntensity;
		nEffect			= src.nEffect;
		bAutoTime		= src.bAutoTime;
		bAdjust			= src.bAdjust;
		dTolTime		= src.dTolTime;
		dStepTime		= src.dStepTime;
		dStartTime		= src.dStartTime;
		dEndTime		= src.dEndTime;
		dEPA			= src.dEPA;
		dEPAScaleFactor = src.dEPAScaleFactor;
		nFileIndex		= src.nFileIndex;
		nDataType		= src.nDataType;
		m_arTime.Copy(src.m_arTime);
		m_arAccel.Copy(src.m_arAccel);
		return *this;
	}
};
class __MY_EXT_CLASS__ CThfcBatchImportItemDlg : public CDialogMove
{
// Construction
public:
	CThfcBatchImportItemDlg(CWnd* pParent = NULL);   // standard constructor

public:
	void SetFunctionName(CString FuncName);
	double m_dSampleTime;
	CString m_strEarthquake;
	//float m_fScaleFactor;
	int lex(FILE *fp, char *yytext);
	int GetFileType(FILE *fp);
	void InitChart();
	void SetAxisLabelText(int nComponent, LPCTSTR lbl);
	enum {
		IDD = IDD_ETC_THFC_BATCH_IMPORT_ITEM
	};
	CStringArray m_arStrFileName;
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CEditUnit	m_edtTolTime;
	CEditUnit	m_edtStepTime;
	CEditUnit	m_edtStartTime;
	CEditUnit	m_edtEndTime;
	BOOL		m_bUserTime;
	BOOL		m_bAdjust;
	CEditUnit	m_edtEPA;
	CEditUnit	m_edtEPAScale;

	//T_THFC_D m_ParamData;
	//BOOL m_bSetParam;
	BOOL m_bGridInit;
	CListBox	m_ListFile;
	CListCtrl	m_ListData;
	//CString m_csOldName;
	SREGraphView m_GraphView;
	CMySRGraph m_Graph;
	CComboBox	m_IntensityCom;
	CComboBox	m_EffectCom;
	CArray<UINT, UINT> m_aTimeCtrl;
	CArray<UINT, UINT> m_aParamCtrl;
	CArray<UINT, UINT> m_aTypeCtrl;
	CArray<ThfcCHData, ThfcCHData&> m_aThfcCHData[2];

	BOOL ReadEQData(FILE *fp, ThfcCHData& ThfcD);
	//BOOL ReadSGSData(FILE *fp, ThfcCHData& ThfcD);
	BOOL ReadEQData_CH_1(FILE *fp, ThfcCHData& ThfcD);
	BOOL ReadEQData_CH_2(FILE *fp, ThfcCHData& ThfcD);
	BOOL GetEPA(ThfcCHData &Data, CCMThfcRpsrProgDlg*& pPrg, BOOL bProgress);
	double GetDesignEPA();

	void SetHeaderTitle(BOOL bInit);
	BOOL MakeListItem();
	void InitCtrl();

	//void InitGrid();
	void	SetValue(int nIndex, int nGroup, double x,double y);
	void	OnThfcRedrawGraph(const CArray<double, double>& aX, const CArray<double, double>& aY);
	void	Updata(const ThfcCHData& ThfcData);
	BOOL	EndEdit(BOOL bEnd, BOOL bDirectDrawLabel = FALSE, unsigned int nCommand = 0);
	//BOOL m_bIsTimeDomain;
	BOOL m_LogFlagX;
	BOOL m_LogFlagY;
	//CArray <double,double> m_arTime,m_arAccel;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelChangeFileList();
	afx_msg void OnSelChangeSeisList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChangeSeisCombox();
	afx_msg void OnEtcThfcAdd();
	afx_msg void OnEtcThfcEdit();
	afx_msg void OnEtcThfcDel();
	afx_msg void OnCheckAdjust();
	afx_msg void OnCheckUserTime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
