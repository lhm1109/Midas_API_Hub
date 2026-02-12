
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_MySRGraph.h"

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#include "CMTdmfItemGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCMTdmfItemDlg dialog
class CCMTdmfItemDlg : public CDialogMove
{
public:
	CCMTdmfItemDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_CMD_TDMF_ITEM };

	CFormulaEdit    m_wndScaleFactor;
	MButton	        m_btnXLog;
	MButton	        m_btnYLog;
	CString	        m_csFuncName;
	CString	        m_csDescription;
	int		        m_nCreepType;
	CEditUnit       m_wndElast;
	CTextUnit       m_wndElastUnit;
	MButton			m_btnDyGen;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	void SetType(int nType) { ASSERT(nType >= 1 && nType <= 3); m_nMatlFuncType = nType; }
	T_TDMF_D& GetParamData();
	void SetParamData(T_TDMF_D& data);
	void MakeChartData();

protected:
	void  Data2Dlg();
	void  Dlg2Data();
	void	InitGrid();
	void	MakeGridData();
	void  AlignControls();
	void  SetDynReportCtrl();
	BOOL  Apply();

	void SetFuncName(CString FuncName);
	void NewData();
	void InitChart();
	void SetAxisLabelText(int nComponent, LPCTSTR lbl);

	void	SetValue(int nIndex, int nGroup, double x,double y);
	double UnitCvt(double dReadData);
	int		lex(FILE *fp, char *yytext);
	BOOL	ReadTDMData(FILE *fp);
	void SetDescription(CString Descript);

	//void	ChangeCreepType(int nType);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeCreepType();
	afx_msg void OnApply();
	afx_msg void OnETCSPFCCHARTXLog();
	afx_msg void OnETCSPFCCHARTYLog();
	afx_msg void OnImportFile();
	afx_msg void OnBtnDyGen();
	afx_msg void OnRdoRelaxTime();
	DECLARE_MESSAGE_MAP()

protected:
	int           m_nMatlFuncType;
	T_TDMF_D      m_ParamData;
	BOOL          m_bSetParam;
	BOOL          m_bGridInit;
	CString       m_csOldName;
	SREGraphView  m_GraphView;
	CMySRGraph    m_Graph;
	
	BOOL          m_LogFlagX;
	BOOL          m_LogFlagY;

	CArray<int, int>        m_arTime;
	CArray<double, double>  m_arValue;
	CArray<UINT, UINT>      m_arCreepTypeGrp, m_arBtnGridCtrlID;
	CArray<UINT,UINT>       m_arRelaxTimeGrp, m_arRelaxTimeRdo;

private:
	CCMTdmfItemGrid         m_wndGrid;
};
