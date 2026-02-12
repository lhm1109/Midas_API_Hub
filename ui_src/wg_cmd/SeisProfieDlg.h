#pragma once

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_LateralLoad.h"
#include "..\wg_base\wg_base_MySRGraph.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

class CSeisProfileDlg : public CDialogMove
{
public:
    CSeisProfileDlg(CWnd* pParent = NULL);
    enum { IDD = IDD_CMD_SEISPF };

private:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

private:
    afx_msg void OnChangeCompont();
    afx_msg void OnChangeProfileType();
    afx_msg void OnSEISPFViewCalcSheet();
    afx_msg void OnSeispfBrowse();
    afx_msg void OnBtnDyGen();
    DECLARE_MESSAGE_MAP()

public:
    void InitSeisProfile(T_SEIS_K nKey, T_SEIS_D& rDataSeis);

private:
    BOOL PreTranslateMessage(MSG* pMsg);

    void InitChart();
    void SetAxisLabelText(int nComponent, LPCTSTR lbl);
    void InitGrid(int nCode);
    void SetValue(int nIndex, int nGroup, double x, double y);
    CString ConvertValToFmtStr(double Val, int nLimit);

private:
    void UpdateChart();
    void UpdateGrid();

private:
    T_SEIS_D* m_pDataSeis;
    T_SEIS_K m_Key;
    CLateralLoad m_LateralLoad;

    int		m_nComponent;
    int		m_nProfileType;
    CString	m_strFileName;
    MButton	m_btnDyGen;

    CGXGridWnd m_SeisPfGrid;
    SREGraphView m_GraphView;
    CMySRGraph m_Graph;
    BOOL m_LogFlagX;
    BOOL m_LogFlagY;
};
