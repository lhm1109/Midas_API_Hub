// PostDrawAdapter.cpp: implementation of the CPostDrawAdapter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PostDrawAdapter.h"
#include "ChinaReportDrawTool/DrawCenter.h"
#include "..\wg_xl\ReportTableProcess.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_AnalysisResult.h"
#include "..\wg_db\wg_db_LoadDB.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPostDrawAdapter::CPostDrawAdapter(const CString& _docpath /*= _T("")*/)
{
    m_DocPath = _docpath;
}

CPostDrawAdapter::~CPostDrawAdapter()
{

}
const Gdiplus::Color MAXALLOWCOLOR = Gdiplus::Color::Red;
const Gdiplus::Color MINALLOWCOLOR = Gdiplus::Color::Blue;
const Gdiplus::Color MAXDESIGNCOLOR = Gdiplus::Color::Teal;
const Gdiplus::Color MINDESIGNCOLOR = Gdiplus::Color::Purple;
const int ALLOWWIDTH = 4;
const int DESIGNWIDTH = 3;

CString CPostDrawAdapter::Build_CHRC_CBCS_Graphic(const std::map<T_ELEM_K,T_CBCS_CH>& mapdatas)
{
    CDrawCenter drawer(m_DocPath);
    std::vector<CMGData> _data;
    
    CMGData mgMaxAllow;
    mgMaxAllow.m_ShowLineWidth = ALLOWWIDTH;
    mgMaxAllow.m_ShowLineColor = MAXALLOWCOLOR;

    CMGData mgMinAllow;
    mgMinAllow.m_ShowLineWidth = ALLOWWIDTH;
    mgMinAllow.m_ShowLineColor = MINALLOWCOLOR;

    CMGData mgMaxDesign;
    mgMaxDesign.m_ShowLineWidth = DESIGNWIDTH;
    mgMaxDesign.m_ShowLineColor = MAXDESIGNCOLOR;
    
    CMGData mgMinDesign;
    mgMinDesign.m_ShowLineWidth = DESIGNWIDTH;
    mgMinDesign.m_ShowLineColor = MINDESIGNCOLOR;


    mgMaxAllow.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MaxMny);
    mgMinAllow.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MinMny);
    mgMaxDesign.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MaxrMuy);
    mgMinDesign.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MinrMuy);
    
    drawer.GetConfig()->m_X_AxisText = _LS(IDS_AMR_CHRP_Graphic_Elem);
    drawer.GetConfig()->m_Y_AxisText = _LS(IDS_AMR_CHRP_Graphic_Moment);
    drawer.GetConfig()->m_Y_Unit = _T("kN·m");


    mgMaxAllow.m_bShowMaxMin = FALSE;
    mgMinAllow.m_bShowMaxMin = FALSE;
    mgMaxDesign.m_bShowMaxMin = TRUE;
    mgMinDesign.m_bShowMaxMin = TRUE;
    drawer.GetConfig()->m_bPostiveMax = TRUE;

    for (std::map<T_ELEM_K,T_CBCS_CH>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_CBCS_CH& cbcs = pit->second;
        BOOL bChecked = TRUE;
        for(int i=0;i<4;i++)
        {
            if(!cbcs.cbcsBase[i].bCHK)
            {
                bChecked = FALSE;
                break;
            }
        }
        if(!bChecked)
            continue;
        int iKey = pit->first;
        mgMaxDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(cbcs.cbcsBase[0].drMuy,cbcs.cbcsBase[2].drMuy)));
        mgMaxAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(cbcs.cbcsBase[0].dMny,cbcs.cbcsBase[2].dMny)));

        mgMinDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(cbcs.cbcsBase[1].drMuy,cbcs.cbcsBase[3].drMuy)));
        mgMinAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(cbcs.cbcsBase[1].dMny,cbcs.cbcsBase[3].dMny)));

    }
    
     _data.push_back(mgMaxAllow);
     _data.push_back(mgMaxDesign);
     _data.push_back(mgMinAllow);
     _data.push_back(mgMinDesign);

    return drawer.Draw(_data);
}

CString CPostDrawAdapter::Build_PSC_BDCY_Graphic( const std::map<T_ELEM_K,T_BDCY_JTG>& mapdatas )
{
    CDrawCenter drawer(m_DocPath);
    std::vector<CMGData> _data;
    
    CMGData mgMaxAllow;
    mgMaxAllow.m_ShowLineWidth = ALLOWWIDTH;
    mgMaxAllow.m_ShowLineColor = MAXALLOWCOLOR;
    
    CMGData mgMinAllow;
    mgMinAllow.m_ShowLineWidth = ALLOWWIDTH;
    mgMinAllow.m_ShowLineColor = MINALLOWCOLOR;
    
    CMGData mgMaxDesign;
    mgMaxDesign.m_ShowLineWidth = DESIGNWIDTH;
    mgMaxDesign.m_ShowLineColor = MAXDESIGNCOLOR;
    
    CMGData mgMinDesign;
    mgMinDesign.m_ShowLineWidth = DESIGNWIDTH;
    mgMinDesign.m_ShowLineColor = MINDESIGNCOLOR;
    
    
    mgMaxAllow.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MaxMny);
    mgMinAllow.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MinMny);
    mgMaxDesign.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MaxrMuy);
    mgMinDesign.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MinrMuy);
    
    drawer.GetConfig()->m_X_AxisText = _LS(IDS_AMR_CHRP_Graphic_Elem);
    drawer.GetConfig()->m_Y_AxisText = _LS(IDS_AMR_CHRP_Graphic_Moment);
    drawer.GetConfig()->m_Y_Unit = _T("kN·m");

    mgMaxAllow.m_bShowMaxMin = FALSE;
    mgMinAllow.m_bShowMaxMin = FALSE;
    mgMaxDesign.m_bShowMaxMin = TRUE;
    mgMinDesign.m_bShowMaxMin = TRUE;
    drawer.GetConfig()->m_bPostiveMax = TRUE;

    
    for (std::map<T_ELEM_K,T_BDCY_JTG>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_BDCY_JTG& cbcs = pit->second;

        BOOL bChecked = TRUE;
        for(int i=0;i<4;i++)
        {
            if(!cbcs.BdcyBase[i].bCHK)
            {
                bChecked = FALSE;
                break;
            }
        }
        if(!bChecked)
            continue;
        int iKey = pit->first;
        mgMaxDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(cbcs.BdcyBase[0].drMuy,cbcs.BdcyBase[2].drMuy)));
        mgMaxAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(cbcs.BdcyBase[0].dMny,cbcs.BdcyBase[2].dMny)));
        
        mgMinDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(cbcs.BdcyBase[1].drMuy,cbcs.BdcyBase[3].drMuy)));
        mgMinAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(-1.0*cbcs.BdcyBase[1].dMny,-1.0*cbcs.BdcyBase[3].dMny)));
        
    }
    
    _data.push_back(mgMaxAllow);
    _data.push_back(mgMaxDesign);
    _data.push_back(mgMinAllow);
    _data.push_back(mgMinDesign);
    
    return drawer.Draw(_data);
}
// 하중조합의 Type 을 String 형식으로 변환.
//   iMaxType : 하중조합 Type (이동하중이 포함된 하중조합에서 최대, 최소 부재력 성분을 출력).
CString CPostDrawAdapter::GetLoadCombType(int iMaxType)
{
    LPCTSTR aType[] = {_T("-"), _LS(IDS_TB_PSCD_FX_MAX), _LS(IDS_TB_PSCD_FX_MIN), _LS(IDS_TB_PSCD_FY_MAX), _LS(IDS_TB_PSCD_FY_MIN), 
        _LS(IDS_TB_PSCD_FZ_MAX), _LS(IDS_TB_PSCD_FZ_MIN), _LS(IDS_TB_PSCD_MX_MAX), _LS(IDS_TB_PSCD_MX_MIN),
        _LS(IDS_TB_PSCD_MY_MAX), _LS(IDS_TB_PSCD_MY_MIN), _LS(IDS_TB_PSCD_MZ_MAX), _LS(IDS_TB_PSCD_MZ_MIN)};
    
    return aType[iMaxType];
}


// 위치의 Type을 String 형식으로 변환.
CString CPostDrawAdapter::GetTopBottomType(int iLocType)
{
    LPCTSTR aType[] = { _LS(IDS_TB_PSCD_TOP), _LS(IDS_TB_PSCD_BOTTOM) };
    return (iLocType < 0 || iLocType >= 2)? _T("Not Exist") : aType[iLocType];
}


// Min/Max Type을 String 형식으로 변환.
CString CPostDrawAdapter::GetMaxMinType(int iMaxType)
{
    LPCTSTR aType[]= {_LS(IDS_TB_PSCD_Max), _LS(IDS_TB_PSCD_Min)};
    return (iMaxType < 0 || iMaxType >= 2)? _T("Not Exist") : aType[iMaxType];
}


// 비틀림 검토시, Min/Max Type을 String 형식으로 변환.
//   iMaxType : 최대/최소 Type.
CString CPostDrawAdapter::GetMaxMinTypeShear(int iMaxType)
{
    LPCTSTR aType[]= {_LS(IDS_TB_PSCD_VMax), _LS(IDS_TB_PSCD_VMin),_LS(IDS_TB_PSCD_TMax)};
    return (iMaxType < 0 || iMaxType >= 3)? _T("Not Exist") : aType[iMaxType];
}
// 설계용 하중조합 Key를 입력받아 실제 하중조합의 이름 구함.
CString CPostDrawAdapter::GetLoadCombName(T_LCOM_K lcomK)
{
    T_LCOM_K orglcomK;
    T_LCOM_D lcomD;
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if(pDoc)
    {
        if(pDoc->IsCHRcJtg04(TRUE))
        {
            pDoc->m_pPostCtrl->GetAnalysisResult()->GetCHRCOrgLcomK(lcomK, orglcomK);
            if(pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, orglcomK, lcomD))
            {
                return lcomD.LoadCombName;
            }
        }
        else if(pDoc->IsPscJtg04())
        {
            pDoc->m_pPostCtrl->GetAnalysisResult()->GetOrgLcomK(lcomK, orglcomK);
            if(pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, orglcomK, lcomD))
            {
                return lcomD.LoadCombName;
            }
        }
    }
    return _T("-");
}
CString CPostDrawAdapter::Build_CHRC_CBCS_Table(const std::map<T_ELEM_K,T_CBCS_CH>& mapdatas)
{
    T_TABLE_D TableResultD;
    TableResultD.Initialize();
    TableResultD.HeaderD.nHeaderRowCount = 1;
    TableResultD.HeaderD.nHeaderColCount = 8;

    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_BDCY_JTG_ELEM));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_BDCY_JTG_PART));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_BDCY_JTG_MAX_MIN));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_BDCY_JTG_LOAD));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_BDCY_JTG_TYPE));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_BDCY_JTG_OK));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_BDCY_JTG_MUY));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_BDCY_JTG_MNY));

    TableResultD.RecordD.nRecordColCount = 8;
    TableResultD.RecordD.aRecordData.SetSize(4*mapdatas.size());


    CString strText;
    int iIndexLine = 0;
    for (std::map<T_ELEM_K,T_CBCS_CH>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_CBCS_CH& cbcs = pit->second;
        for (int i=0;i<4;i++)
        {
            T_CBCS_CH_BASE cbcsBase = cbcs.cbcsBase[i];

            if(cbcsBase.bCHK)
            {
                strText.Format(_T("%d"), pit->first);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = (i<2)? _T("I") : _T("J");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                int index = (i%2==0)? 0 : 1;
                strText = GetMaxMinType(index);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = GetLoadCombName(cbcsBase.LcomK);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = GetLoadCombType(cbcsBase.iMax);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = (cbcsBase.bOK)? _T("OK") : _T("NG");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.drMuy);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dMny);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                iIndexLine++;

            }
            
        }
                
    }
    
    TableResultD.RecordD.nRecordRowCount = iIndexLine;


    CReportTableProcess ReportTable;
    ReportTable.SetData(TableResultD);

    CString strTitle = _LS(IDS_TB_RESULTVIEW_JTG_BDCY);
    CString strPath = ReportTable.GetFilePathName() + strTitle + _T(".xml");

    ReportTable.SetFilePathName(strPath);
    ReportTable.GenerateReport();

    return strPath;
}
CString CPostDrawAdapter::Build_PSC_BDCY_Table(const std::map<T_ELEM_K,T_BDCY_JTG>& mapdatas)
{
    T_TABLE_D TableResultD;
    TableResultD.Initialize();
    TableResultD.HeaderD.nHeaderRowCount = 1;
    TableResultD.HeaderD.nHeaderColCount = 8;
    
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_BDCY_JTG_ELEM));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_BDCY_JTG_PART));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_BDCY_JTG_MAX_MIN));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_BDCY_JTG_LOAD));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_BDCY_JTG_TYPE));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_BDCY_JTG_OK));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_BDCY_JTG_MUY));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_BDCY_JTG_MNY));

    TableResultD.RecordD.nRecordColCount = 8;
    TableResultD.RecordD.aRecordData.SetSize(4*mapdatas.size());

    
    CString strText;
    int iIndexLine = 0;
    for (std::map<T_ELEM_K,T_BDCY_JTG>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_BDCY_JTG& cbcs = pit->second;
        for (int i=0;i<4;i++)
        {
            T_BDCY_JTG_BASE cbcsBase = cbcs.BdcyBase[i];
            
            if(cbcsBase.bCHK)
            {
                strText.Format(_T("%d"), pit->first);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = (i<2)? _T("I") : _T("J");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                int index = (i%2==0)? 0 : 1;
                strText = GetMaxMinType(index);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = GetLoadCombName(cbcsBase.LcomK);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = GetLoadCombType(cbcsBase.iMax);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = (cbcsBase.bOK)? _T("OK") : _T("NG");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.drMuy);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dMny);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                iIndexLine++;
                
            }
            
        }
        
    }
    
    
    TableResultD.RecordD.nRecordRowCount = iIndexLine;

    CReportTableProcess ReportTable;
    ReportTable.SetData(TableResultD);
    
    CString strTitle = _LS(IDS_TB_RESULTVIEW_JTG_BDCY);
    CString strPath = ReportTable.GetFilePathName() + strTitle + _T(".xml");
    
    ReportTable.SetFilePathName(strPath);
    ReportTable.GenerateReport();
    
    return strPath;
}

CString CPostDrawAdapter::Build_PSC_CUMS_Graphic( const std::map<T_ELEM_K,T_CUMS_JTG>& mapdatas )
{
    CDrawCenter drawer(m_DocPath);
    std::vector<CMGData> _data;
    
    CMGData mgMaxAllow;
    mgMaxAllow.m_ShowLineWidth = ALLOWWIDTH;
    mgMaxAllow.m_ShowLineColor = MAXALLOWCOLOR;
    
    CMGData mgMinAllow;
    mgMinAllow.m_ShowLineWidth = ALLOWWIDTH;
    mgMinAllow.m_ShowLineColor = MINALLOWCOLOR;
    
    CMGData mgMaxDesign;
    mgMaxDesign.m_ShowLineWidth = DESIGNWIDTH;
    mgMaxDesign.m_ShowLineColor = MAXDESIGNCOLOR;
    
    CMGData mgMinDesign;
    mgMinDesign.m_ShowLineWidth = DESIGNWIDTH;
    mgMinDesign.m_ShowLineColor = MINDESIGNCOLOR;
    
    

    mgMaxAllow.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MaxVn);
    mgMinAllow.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MinVn);
    mgMaxDesign.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MaxrVu);
    mgMinDesign.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MinrVu);
    
    drawer.GetConfig()->m_X_AxisText = _LS(IDS_AMR_CHRP_Graphic_Elem);
    drawer.GetConfig()->m_Y_AxisText = _LS(IDS_AMR_CHRP_Graphic_V);
    drawer.GetConfig()->m_Y_Unit = _T("kN");


    mgMaxAllow.m_bShowMaxMin = FALSE;
    mgMinAllow.m_bShowMaxMin = FALSE;
    mgMaxDesign.m_bShowMaxMin = TRUE;
    mgMinDesign.m_bShowMaxMin = TRUE;
    drawer.GetConfig()->m_bPostiveMax = TRUE;

    
    for (std::map<T_ELEM_K,T_CUMS_JTG>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_CUMS_JTG& JtgData = pit->second;

        BOOL bChecked = TRUE;
        for(int i=0;i<4;i++)
        {
            if(!JtgData.CumsBase[i].bCHK)
            {
                bChecked = FALSE;
                break;
            }
        }
        if(!bChecked)
            continue;

        int iKey = pit->first;
        mgMaxDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CumsBase[0].drVu,JtgData.CumsBase[2].drVu)));
        mgMaxAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CumsBase[0].dVn,JtgData.CumsBase[2].dVn)));
        
        mgMinDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CumsBase[1].drVu,JtgData.CumsBase[3].drVu)));
        mgMinAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(-1.0*JtgData.CumsBase[1].dVn,-1.0*JtgData.CumsBase[3].dVn)));
        
    }
    
    _data.push_back(mgMaxAllow);
    _data.push_back(mgMaxDesign);
    _data.push_back(mgMinAllow);
    _data.push_back(mgMinDesign);
    
    return drawer.Draw(_data);
}

CString CPostDrawAdapter::Build_CHRC_CSIS_Graphic( const std::map<T_ELEM_K,T_CSIS_CH>& mapdatas )
{
    CDrawCenter drawer(m_DocPath);
    std::vector<CMGData> _data;
    
    CMGData mgMaxAllow;
    mgMaxAllow.m_ShowLineWidth = ALLOWWIDTH;
    mgMaxAllow.m_ShowLineColor = MAXALLOWCOLOR;
    
    CMGData mgMinAllow;
    mgMinAllow.m_ShowLineWidth = ALLOWWIDTH;
    mgMinAllow.m_ShowLineColor = MINALLOWCOLOR;
    
    CMGData mgMaxDesign;
    mgMaxDesign.m_ShowLineWidth = DESIGNWIDTH;
    mgMaxDesign.m_ShowLineColor = MAXDESIGNCOLOR;
    
    CMGData mgMinDesign;
    mgMinDesign.m_ShowLineWidth = DESIGNWIDTH;
    mgMinDesign.m_ShowLineColor = MINDESIGNCOLOR;
    
    
    mgMaxAllow.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MaxVn);
    mgMinAllow.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MinVn);
    mgMaxDesign.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MaxrVu);
    mgMinDesign.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MinrVu);
    
    drawer.GetConfig()->m_X_AxisText = _LS(IDS_AMR_CHRP_Graphic_Elem);
    drawer.GetConfig()->m_Y_AxisText = _LS(IDS_AMR_CHRP_Graphic_V);
    drawer.GetConfig()->m_Y_Unit = _T("kN");


    
    
    for (std::map<T_ELEM_K,T_CSIS_CH>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_CSIS_CH& JtgData = pit->second;

        BOOL bChecked = TRUE;
        for(int i=0;i<4;i++)
        {
            if(!JtgData.CsisBase[i].bCHK)
            {
                bChecked = FALSE;
                break;
            }
        }
        if(!bChecked)
            continue;
        int iKey = pit->first;
        mgMaxDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CsisBase[0].drVu,JtgData.CsisBase[2].drVu)));
        mgMaxAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CsisBase[0].dVn,JtgData.CsisBase[2].dVn)));
        
        mgMinDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CsisBase[1].drVu,JtgData.CsisBase[3].drVu)));
        mgMinAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CsisBase[1].dVn,JtgData.CsisBase[3].dVn)));
        
    }
    
    _data.push_back(mgMaxAllow);
    _data.push_back(mgMaxDesign);
    _data.push_back(mgMinAllow);
    _data.push_back(mgMinDesign);
    
    return drawer.Draw(_data);
}

CString CPostDrawAdapter::Build_PSC_CRMT_Graphic( const std::map<T_ELEM_K,T_CRMT_JTG>& mapdatas )
{
    CDrawCenter drawer(m_DocPath);
    std::vector<CMGData> _data;
    
    CMGData mgMaxAllow;
    mgMaxAllow.m_ShowLineWidth = ALLOWWIDTH;
    mgMaxAllow.m_ShowLineColor = MAXALLOWCOLOR;
    
    CMGData mgMinAllow;
    mgMinAllow.m_ShowLineWidth = ALLOWWIDTH;
    mgMinAllow.m_ShowLineColor = MINALLOWCOLOR;
    
    CMGData mgMaxDesign;
    mgMaxDesign.m_ShowLineWidth = DESIGNWIDTH;
    mgMaxDesign.m_ShowLineColor = MAXDESIGNCOLOR;
    
//   CMGData mgMinDesign;
//   mgMinDesign.m_ShowLineWidth = DESIGNWIDTH;
//   mgMinDesign.m_ShowLineColor = MINDESIGNCOLOR;
    
    
    mgMaxAllow.m_StrName = _LS(IDS_AMR_CHRP_Graphic_Tn);
    mgMinAllow.m_StrName = _LS(IDS_AMR_CHRP_Graphic_Tn);
    mgMaxDesign.m_StrName = _LS(IDS_AMR_CHRP_Graphic_rTu);
    //mgMinDesign.m_StrName = "離鬼 雍팻?;
    drawer.GetConfig()->m_X_AxisText = _LS(IDS_AMR_CHRP_Graphic_Elem);
    drawer.GetConfig()->m_Y_AxisText = _LS(IDS_AMR_CHRP_Graphic_T);
    drawer.GetConfig()->m_Y_Unit = _T("kN·m");


    mgMaxAllow.m_bShowMaxMin = FALSE;
    mgMaxDesign.m_bShowMaxMin = TRUE;
    drawer.GetConfig()->m_bPostiveMax = TRUE;

    
    for (std::map<T_ELEM_K,T_CRMT_JTG>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_CRMT_JTG& JtgData = pit->second;

        
        BOOL bChecked = TRUE;
        for(int i=0;i<6;i++)
        {
            if(!JtgData.CrmtBase[i].bCHK)
            {
                bChecked = FALSE;
                break;
            }
        }
        if(!bChecked)
            continue;
        int iKey = pit->first;
        mgMaxDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CrmtBase[2].drTu,JtgData.CrmtBase[5].drTu)));
        mgMaxAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CrmtBase[2].dTn,JtgData.CrmtBase[5].dTn)));
        mgMinAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(-1.0*JtgData.CrmtBase[2].dTn,-1.0*JtgData.CrmtBase[5].dTn)));

//     mgMinDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CsisBase[1].dVn,JtgData.CsisBase[3].dVn)));
        
    }
    
    _data.push_back(mgMaxAllow);
    _data.push_back(mgMaxDesign);
     _data.push_back(mgMinAllow);
//   _data.push_back(mgMinDesign);
    
    return drawer.Draw(_data);

}

CString CPostDrawAdapter::Build_PSC_CRMT_Graphic2( const std::map<T_ELEM_K,T_CRMT_JTG>& mapdatas )
{
    CDrawCenter drawer(m_DocPath);
    std::vector<CMGData> _data;
    
    CMGData mgMaxAllow;
    mgMaxAllow.m_ShowLineWidth = ALLOWWIDTH;
    mgMaxAllow.m_ShowLineColor = MAXALLOWCOLOR;
    
         CMGData mgMinAllow;
         mgMinAllow.m_ShowLineWidth = ALLOWWIDTH;
         mgMinAllow.m_ShowLineColor = MINALLOWCOLOR;
    
    CMGData mgMaxDesign;
    mgMaxDesign.m_ShowLineWidth = DESIGNWIDTH;
    mgMaxDesign.m_ShowLineColor = MAXDESIGNCOLOR;
    
         CMGData mgMinDesign;
         mgMinDesign.m_ShowLineWidth = DESIGNWIDTH;
         mgMinDesign.m_ShowLineColor = MINDESIGNCOLOR;
    
    
         mgMaxAllow.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MaxVn);
         mgMinAllow.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MinVn);
         mgMaxDesign.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MaxrVu);
         mgMinDesign.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MinrVu);
         
         drawer.GetConfig()->m_X_AxisText = _LS(IDS_AMR_CHRP_Graphic_Elem);
         drawer.GetConfig()->m_Y_AxisText = _LS(IDS_AMR_CHRP_Graphic_V);
         drawer.GetConfig()->m_Y_Unit = _T("kN");
    
    
         for (std::map<T_ELEM_K,T_CRMT_JTG>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
         {
             const T_CRMT_JTG& JtgData = pit->second;
             
             BOOL bChecked = TRUE;
             for(int i=0;i<6;i++)
             {
                 if(!JtgData.CrmtBase[i].bCHK)
                 {
                     bChecked = FALSE;
                     break;
                 }
             }
             if(!bChecked)
                 continue;
             int iKey = pit->first;
             mgMaxDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CrmtBase[0].drVu,JtgData.CrmtBase[2].drVu)));
             mgMaxAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CrmtBase[0].dVn,JtgData.CrmtBase[2].dVn)));
             
             mgMinDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CrmtBase[1].drVu,JtgData.CrmtBase[3].drVu)));
             mgMinAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(-1.0*JtgData.CrmtBase[1].dVn,-1.0*JtgData.CrmtBase[3].dVn)));
             
         }
         
         _data.push_back(mgMaxAllow);
         _data.push_back(mgMaxDesign);
         _data.push_back(mgMinAllow);
         _data.push_back(mgMinDesign);
    
    return drawer.Draw(_data);
    
}

CString CPostDrawAdapter::Build_CHRC_CTSS_Graphic( const std::map<T_ELEM_K,T_CTSS_CH>& mapdatas )
{
    CDrawCenter drawer(m_DocPath);
    std::vector<CMGData> _data;
    
    CMGData mgMaxAllow;
    mgMaxAllow.m_ShowLineWidth = ALLOWWIDTH;
    mgMaxAllow.m_ShowLineColor = MAXALLOWCOLOR;
    
    //   CMGData mgMinAllow;
    //   mgMinAllow.m_ShowLineWidth = ALLOWWIDTH;
    //   mgMinAllow.m_ShowLineColor = MINALLOWCOLOR;
    
    CMGData mgMaxDesign;
    mgMaxDesign.m_ShowLineWidth = DESIGNWIDTH;
    mgMaxDesign.m_ShowLineColor = MAXDESIGNCOLOR;
    
    //   CMGData mgMinDesign;
    //   mgMinDesign.m_ShowLineWidth = DESIGNWIDTH;
    //   mgMinDesign.m_ShowLineColor = MINDESIGNCOLOR;
    
    
    mgMaxAllow.m_StrName = _LS(IDS_AMR_CHRP_Graphic_Tn);
    //mgMinAllow.m_StrName = "離鬼豚冀令";
    mgMaxDesign.m_StrName = _LS(IDS_AMR_CHRP_Graphic_rTu);
    //mgMinDesign.m_StrName = "離鬼 雍팻?;
    drawer.GetConfig()->m_X_AxisText = _LS(IDS_AMR_CHRP_Graphic_Elem);
    drawer.GetConfig()->m_Y_AxisText = _LS(IDS_AMR_CHRP_Graphic_T);
    drawer.GetConfig()->m_Y_Unit = _T("kN·m");


    mgMaxAllow.m_bShowMaxMin = FALSE;
    mgMaxDesign.m_bShowMaxMin = TRUE;
    drawer.GetConfig()->m_bPostiveMax = TRUE;

    
    for (std::map<T_ELEM_K,T_CTSS_CH>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_CTSS_CH& JtgData = pit->second;
        BOOL bChecked = TRUE;
        for(int i=0;i<6;i++)
        {
            if(!JtgData.CtssBase[i].bCHK)
            {
                bChecked = FALSE;
                break;
            }
        }
        if(!bChecked)
            continue;
        int iKey = pit->first;
        mgMaxDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CtssBase[2].drTu,JtgData.CtssBase[5].drTu)));
        mgMaxAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CtssBase[2].dTn,JtgData.CtssBase[5].dTn)));
        
        //     mgMinAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CsisBase[1].drVu,JtgData.CsisBase[3].drVu)));
        //     mgMinDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CsisBase[1].dVn,JtgData.CsisBase[3].dVn)));
        
    }
    
    _data.push_back(mgMaxAllow);
    _data.push_back(mgMaxDesign);
    //   _data.push_back(mgMinAllow);
    //   _data.push_back(mgMinDesign);
    
    return drawer.Draw(_data);

}

CString CPostDrawAdapter::Build_PSC_FYCM_Graphic(int iBrgtype ,  const std::map<T_ELEM_K,T_FYCM_JTG>& mapdatas )
{
    CDrawCenter drawer(m_DocPath);
    std::vector<CMGData> _data;
    
    CMGData mgMaxAllow;
    mgMaxAllow.m_ShowLineWidth = ALLOWWIDTH;
    mgMaxAllow.m_ShowLineColor = MAXALLOWCOLOR;
    
    //   CMGData mgMinAllow;
    //   mgMinAllow.m_ShowLineWidth = ALLOWWIDTH;
    //   mgMinAllow.m_ShowLineColor = MINALLOWCOLOR;
    
    CMGData mgMaxDesign;
    mgMaxDesign.m_ShowLineWidth = DESIGNWIDTH;
    mgMaxDesign.m_ShowLineColor = MAXDESIGNCOLOR;
    
    //   CMGData mgMinDesign;
    //   mgMinDesign.m_ShowLineWidth = DESIGNWIDTH;
    //   mgMinDesign.m_ShowLineColor = MINDESIGNCOLOR;

    mgMaxAllow.m_StrName = _LS(IDS_AMR_CHRP_Graphic_ALW);
    mgMaxDesign.m_StrName = _LS(IDS_AMR_CHRP_Graphic_FMAX);
    drawer.GetConfig()->m_X_AxisText = _LS(IDS_AMR_CHRP_Graphic_Elem);
    drawer.GetConfig()->m_Y_AxisText = _LS(IDS_AMR_CHRP_Graphic_Stress);
    drawer.GetConfig()->m_Y_Unit = _T("MPa");

    mgMaxAllow.m_bShowMaxMin = FALSE;
    mgMaxDesign.m_bShowMaxMin = TRUE;
    drawer.GetConfig()->m_bPostiveMax =(iBrgtype == 3 )? TRUE : FALSE;

    
    for (std::map<T_ELEM_K,T_FYCM_JTG>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_FYCM_JTG& JtgData = pit->second;
        BOOL bChecked = TRUE;
        
        if(iBrgtype == 0)
        {
            bChecked = (JtgData.FycmBase[1].bCHK && JtgData.FycmBase[3].bCHK);
        }
        else if(iBrgtype == 1)
        {
            bChecked = (JtgData.FycmBase[1].bCHK && JtgData.FycmBase[3].bCHK);      
        }
        else if(iBrgtype == 2)
        {
            bChecked = (JtgData.FycmBase[4].bCHK && JtgData.FycmBase[5].bCHK);  
        }
        else if(iBrgtype == 3)
        {
            bChecked = (JtgData.FycmBase[0].bCHK && JtgData.FycmBase[2].bCHK);        
        }
        if(!bChecked)
            continue;
        int iKey = pit->first;
        if(iBrgtype == 0)
        {
            mgMaxAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.FycmBase[1].dALW,JtgData.FycmBase[3].dALW)));
            mgMaxDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.FycmBase[1].dFMAX,JtgData.FycmBase[3].dFMAX)));
        }
        else if(iBrgtype == 1)
        {
            mgMaxAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.FycmBase[1].dALW,JtgData.FycmBase[3].dALW)));
            mgMaxDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.FycmBase[1].dFMAX,JtgData.FycmBase[3].dFMAX)));

        }
        else if(iBrgtype == 2)
        {
            mgMaxAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.FycmBase[4].dALW,JtgData.FycmBase[5].dALW)));
            mgMaxDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.FycmBase[4].dFMAX,JtgData.FycmBase[5].dFMAX)));

        }
        else if(iBrgtype == 3)
        {
            mgMaxAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.FycmBase[0].dALW,JtgData.FycmBase[2].dALW)));
            mgMaxDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.FycmBase[0].dFMAX,JtgData.FycmBase[2].dFMAX)));
            
        }


        
        //     mgMinAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CsisBase[1].drVu,JtgData.CsisBase[3].drVu)));
        //     mgMinDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.CsisBase[1].dVn,JtgData.CsisBase[3].dVn)));
        
    }
    
    _data.push_back(mgMaxAllow);
    _data.push_back(mgMaxDesign);
    //   _data.push_back(mgMinAllow);
    //   _data.push_back(mgMinDesign);
    
    return drawer.Draw(_data);

}

CString CPostDrawAdapter::Build_PSC_FPMS_Graphic(int iBrgtype ,  const std::map<T_ELEM_K,T_FPMS_JTG>& mapdatas )
{
    CDrawCenter drawer(m_DocPath);
    std::vector<CMGData> _data;
    
    CMGData mgMaxAllow;
    mgMaxAllow.m_ShowLineWidth = ALLOWWIDTH;
    mgMaxAllow.m_ShowLineColor = MAXALLOWCOLOR;
    
    
    CMGData mgMaxDesign;
    mgMaxDesign.m_ShowLineWidth = DESIGNWIDTH;
    mgMaxDesign.m_ShowLineColor = MAXDESIGNCOLOR;
    
    
    mgMaxAllow.m_StrName = _LS(IDS_AMR_CHRP_Graphic_AFP);
    mgMaxDesign.m_StrName = _LS(IDS_AMR_CHRP_Graphic_FMAX);
    drawer.GetConfig()->m_X_AxisText = _LS(IDS_AMR_CHRP_Graphic_Elem);
    drawer.GetConfig()->m_Y_AxisText = _LS(IDS_AMR_CHRP_Graphic_Stress);
    drawer.GetConfig()->m_Y_Unit = _T("MPa");

    mgMaxAllow.m_bShowMaxMin = FALSE;
    mgMaxDesign.m_bShowMaxMin = TRUE;
    drawer.GetConfig()->m_bPostiveMax = (iBrgtype == 0) ? FALSE : TRUE;

    
    for (std::map<T_ELEM_K,T_FPMS_JTG>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_FPMS_JTG& JtgData = pit->second;
        BOOL bChecked = TRUE;
        if(iBrgtype == 0)
        {
            bChecked = (JtgData.FpmsBase[1].bCHK && JtgData.FpmsBase[3].bCHK);
        }
        else if(iBrgtype == 1)
        {
            bChecked = (JtgData.FpmsBase[0].bCHK && JtgData.FpmsBase[2].bCHK);
        }
        if(!bChecked)
            continue;
        int iKey = pit->first;
        if(iBrgtype == 0)
        {
            mgMaxAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.FpmsBase[1].dAFP,JtgData.FpmsBase[3].dAFP)));
            mgMaxDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.FpmsBase[1].dFMAX,JtgData.FpmsBase[3].dFMAX)));
        }
        else if(iBrgtype == 1)
        {
            mgMaxAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.FpmsBase[0].dAFP,JtgData.FpmsBase[2].dAFP)));
            mgMaxDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(JtgData.FpmsBase[0].dFMAX,JtgData.FpmsBase[2].dFMAX)));

        }
    }
    
    _data.push_back(mgMaxAllow);
    _data.push_back(mgMaxDesign);
    
    return drawer.Draw(_data);
}

CString CPostDrawAdapter::Build_PSC_FYSG_Graphic( const std::map<T_ELEM_K,T_FYSG_D>& mapdatas )
{
    CDrawCenter drawer(m_DocPath);
    std::vector<CMGData> _data;
    
    CMGData mgMaxAllow;
    mgMaxAllow.m_ShowLineWidth = ALLOWWIDTH;
    mgMaxAllow.m_ShowLineColor = MAXALLOWCOLOR;

    CMGData mgMinAllow;
    mgMinAllow.m_ShowLineWidth = ALLOWWIDTH;
    mgMinAllow.m_ShowLineColor = MINALLOWCOLOR;

    CMGData mgMaxDesign;
    mgMaxDesign.m_ShowLineWidth = DESIGNWIDTH;
    mgMaxDesign.m_ShowLineColor = MAXDESIGNCOLOR;
    
    CMGData mgMinDesign;
    mgMinDesign.m_ShowLineWidth = DESIGNWIDTH;
    mgMinDesign.m_ShowLineColor = MINDESIGNCOLOR;


    mgMaxAllow.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MaxALW);
    mgMinAllow.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MinALW);
    mgMaxDesign.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MaxFMAX);
    mgMinDesign.m_StrName = _LS(IDS_AMR_CHRP_Graphic_MinrFMAX);

    drawer.GetConfig()->m_X_AxisText = _LS(IDS_AMR_CHRP_Graphic_Elem);
    drawer.GetConfig()->m_Y_AxisText = _LS(IDS_AMR_CHRP_Graphic_Stress);
    drawer.GetConfig()->m_Y_Unit = _T("MPa");

    mgMaxAllow.m_bShowMaxMin = FALSE;
    mgMaxDesign.m_bShowMaxMin = TRUE;
    mgMinAllow.m_bShowMaxMin = FALSE;
    mgMinDesign.m_bShowMaxMin = TRUE;

    drawer.GetConfig()->m_bPostiveMax = TRUE;


    for (std::map<T_ELEM_K,T_FYSG_D>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_FYSG_D& cbcs = pit->second;
        BOOL bChecked = TRUE;
        for(int i=0;i<4;i++)
        {
            if(!cbcs.FysgBase[i].bCHK)
            {
                bChecked = FALSE;
                break;
            }
        }
        if(!bChecked)
            continue;
        int iKey = pit->first;
        mgMaxAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(cbcs.FysgBase[0].dALW,cbcs.FysgBase[2].dALW)));
        mgMaxDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(cbcs.FysgBase[0].dFMAX,cbcs.FysgBase[2].dFMAX)));

        mgMinAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(cbcs.FysgBase[1].dALW,cbcs.FysgBase[3].dALW)));
        mgMinDesign.m_ShowDatas.push_back(make_pair(iKey,make_pair(cbcs.FysgBase[1].dFMAX,cbcs.FysgBase[3].dFMAX)));
        
    }
    
    _data.push_back(mgMaxAllow);
    _data.push_back(mgMaxDesign);
    _data.push_back(mgMinAllow);
    _data.push_back(mgMinDesign);
    
    return drawer.Draw(_data);
}

CString CPostDrawAdapter::Build_PSC_CUMS_Table( const std::map<T_ELEM_K,T_CUMS_JTG>& mapdatas )
{
    T_TABLE_D TableResultD;
    TableResultD.Initialize();
    TableResultD.HeaderD.nHeaderRowCount = 1;
    TableResultD.HeaderD.nHeaderColCount = 10;
    
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_ELEM));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_PART));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_MAX_MIN));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_LOAD));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_TYPE));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_OK));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_VU));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_VN));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_SECT_OK));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_SHRLOAD));

    
    TableResultD.RecordD.nRecordColCount = 10;
    TableResultD.RecordD.aRecordData.SetSize(4*mapdatas.size());
    
    
    CString strText;
    int iIndexLine = 0;
    for (std::map<T_ELEM_K,T_CUMS_JTG>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_CUMS_JTG& cbcs = pit->second;
        for (int i=0;i<4;i++)
        {
            T_CUMS_JTG_BASE cbcsBase = cbcs.CumsBase[i];
            
            if(cbcsBase.bCHK)
            {
                strText.Format(_T("%d"), pit->first);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = (i<2)? _T("I") : _T("J");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                int index = (i%2==0)? 0 : 1;
                strText = GetMaxMinType(index);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = GetLoadCombName(cbcsBase.LcomK);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = GetLoadCombType(cbcsBase.iMax);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = (cbcsBase.bOK)? _T("OK") : _T("NG");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.drVu);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dVn);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);

            
                
                strText = (fabs(cbcsBase.drVu) <= cbcsBase.dShrSect)? _T("OK") : _T("NG");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = (fabs(cbcsBase.drVu) <= cbcsBase.dShrLoad)? _LS(IDS_TB_PSCD_CUMS_JTG_SHRLOAD_SKIP) : _LS(IDS_TB_PSCD_CUMS_JTG_SHRLOAD_CHECK);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);

                iIndexLine++;
                
            }
            
        }
        
    }
    
    
    TableResultD.RecordD.nRecordRowCount = iIndexLine;

    CReportTableProcess ReportTable;
    ReportTable.SetData(TableResultD);
    
    CString strTitle = _LS(IDS_TB_RESULTVIEW_JTG_CUMS);
    CString strPath = ReportTable.GetFilePathName() + strTitle + _T(".xml");
    
    ReportTable.SetFilePathName(strPath);
    ReportTable.GenerateReport();
    
    return strPath;
}

CString CPostDrawAdapter::Build_CHRC_CSIS_Table( const std::map<T_ELEM_K,T_CSIS_CH>& mapdatas )
{
    T_TABLE_D TableResultD;
    TableResultD.Initialize();
    TableResultD.HeaderD.nHeaderRowCount = 1;
    TableResultD.HeaderD.nHeaderColCount = 10;
    
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_ELEM));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_PART));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_MAX_MIN));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_LOAD));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_TYPE));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_OK));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_VU));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_VN));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_SECT_OK));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CUMS_JTG_SHRLOAD));
    
    
    TableResultD.RecordD.nRecordColCount = 10;
    TableResultD.RecordD.aRecordData.SetSize(4*mapdatas.size());
    
    
    CString strText;
    int iIndexLine = 0;
    for (std::map<T_ELEM_K,T_CSIS_CH>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_CSIS_CH& cbcs = pit->second;
        for (int i=0;i<4;i++)
        {
            T_CSIS_CH_BASE cbcsBase = cbcs.CsisBase[i];
            
            if(cbcsBase.bCHK)
            {
                strText.Format(_T("%d"), pit->first);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = (i<2)? _T("I") : _T("J");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                int index = (i%2==0)? 0 : 1;
                strText = GetMaxMinType(index);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = GetLoadCombName(cbcsBase.LcomK);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = GetLoadCombType(cbcsBase.iMax);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = (cbcsBase.bOK)? _T("OK") : _T("NG");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.drVu);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dVn);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                
                
                strText = (fabs(cbcsBase.drVu) <= cbcsBase.dShrSect)? _T("OK") : _T("NG");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = (fabs(cbcsBase.drVu) <= cbcsBase.dShrLoad)? _LS(IDS_TB_PSCD_CUMS_JTG_SHRLOAD_SKIP) : _LS(IDS_TB_PSCD_CUMS_JTG_SHRLOAD_CHECK);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                iIndexLine++;
                
            }
            
        }
        
    }
    
    
    TableResultD.RecordD.nRecordRowCount = iIndexLine;

    CReportTableProcess ReportTable;
    ReportTable.SetData(TableResultD);
    
    CString strTitle = _LS(IDS_TB_RESULTVIEW_JTG_CUMS);
    CString strPath = ReportTable.GetFilePathName() + strTitle + _T(".xml");
    
    ReportTable.SetFilePathName(strPath);
    ReportTable.GenerateReport();
    
    return strPath;
}

CString CPostDrawAdapter::Build_PSC_CRMT_Table( const std::map<T_ELEM_K,T_CRMT_JTG>& mapdatas )
{
    T_TABLE_D TableResultD;
    TableResultD.Initialize();
    TableResultD.HeaderD.nHeaderRowCount = 1;
    TableResultD.HeaderD.nHeaderColCount = 12;
    
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_ELEM));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_PART));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_MAX_MIN));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_LOAD));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_TYPE));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_OK));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_TU));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_TN));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_VU));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_VN));

    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_SECT_OK));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_TORLOAD));
    
    
    TableResultD.RecordD.nRecordColCount = 12;
    TableResultD.RecordD.aRecordData.SetSize(6*mapdatas.size());
    
    
    CString strText;
    int iIndexLine = 0;
    for (std::map<T_ELEM_K,T_CRMT_JTG>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_CRMT_JTG& cbcs = pit->second;
        for (int i=0;i<6;i++)
        {
            T_CRMT_JTG_BASE cbcsBase = cbcs.CrmtBase[i];
            
            if(cbcsBase.bCHK)
            {
                strText.Format(_T("%d"), pit->first);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = (i<3)? _T("I") : _T("J");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);

                int index = i%3;
                strText = GetMaxMinTypeShear(index);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                
                strText = GetLoadCombName(cbcsBase.LcomK);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = GetLoadCombType(cbcsBase.iMax);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = (cbcsBase.bOK)? _T("OK") : _T("NG");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
 
                strText.Format(_T("%0.2f"), cbcsBase.drTu);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dTn);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);

                strText.Format(_T("%0.2f"), cbcsBase.drVu);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dVn);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                
                
                strText = (fabs(cbcsBase.dShrTorVal) <= cbcsBase.dTorSect)? _T("OK") : _T("NG");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = (fabs(cbcsBase.dShrTorVal) <= cbcsBase.dTorLoad)? _LS(IDS_TB_PSCD_CRMT_JTG_TORLOAD_SKIP) : _LS(IDS_TB_PSCD_CRMT_JTG_TORLOAD_CHECK);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                iIndexLine++;
                
            }
            
        }
        
    }
    
    TableResultD.RecordD.nRecordRowCount = iIndexLine;

    
    CReportTableProcess ReportTable;
    ReportTable.SetData(TableResultD);
    
    CString strTitle = _LS(IDS_TB_RESULTVIEW_JTG_CRMT);
    CString strPath = ReportTable.GetFilePathName() + strTitle + _T(".xml");
    
    ReportTable.SetFilePathName(strPath);
    ReportTable.GenerateReport();
    
    return strPath;
}

CString CPostDrawAdapter::Build_CHRC_CTSS_Table( const std::map<T_ELEM_K,T_CTSS_CH>& mapdatas )
{
    T_TABLE_D TableResultD;
    TableResultD.Initialize();
    TableResultD.HeaderD.nHeaderRowCount = 1;
    TableResultD.HeaderD.nHeaderColCount = 12;
    
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_ELEM));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_PART));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_MAX_MIN));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_LOAD));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_TYPE));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_OK));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_TU));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_TN));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_VU));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_VN));
    
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_SECT_OK));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_CRMT_JTG_TORLOAD));
    
    
    TableResultD.RecordD.nRecordColCount = 12;
    TableResultD.RecordD.aRecordData.SetSize(6*mapdatas.size());
    
    
    CString strText;
    int iIndexLine = 0;
    for (std::map<T_ELEM_K,T_CTSS_CH>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_CTSS_CH& cbcs = pit->second;
        for (int i=0;i<6;i++)
        {
            T_CTSS_CH_BASE cbcsBase = cbcs.CtssBase[i];
            
            if(cbcsBase.bCHK)
            {
                strText.Format(_T("%d"), pit->first);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = (i<2)? _T("I") : _T("J");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                int index = i%3;
                strText = GetMaxMinTypeShear(index);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                
                strText = GetLoadCombName(cbcsBase.LcomK);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = GetLoadCombType(cbcsBase.iMax);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = (cbcsBase.bOK)? _T("OK") : _T("NG");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                
                strText.Format(_T("%0.2f"), cbcsBase.drTu);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dTn);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.drVu);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dVn);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                
                
                strText = (fabs(cbcsBase.dShrTorVal) <= cbcsBase.dTorSect)? _T("OK") : _T("NG");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = (fabs(cbcsBase.dShrTorVal) <= cbcsBase.dTorLoad)? _LS(IDS_TB_PSCD_CRMT_JTG_TORLOAD_SKIP) : _LS(IDS_TB_PSCD_CRMT_JTG_TORLOAD_CHECK);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                iIndexLine++;
                
            }
            
        }
        
    }
    
    TableResultD.RecordD.nRecordRowCount = iIndexLine;

    
    CReportTableProcess ReportTable;
    ReportTable.SetData(TableResultD);
    
    CString strTitle = _LS(IDS_TB_RESULTVIEW_JTG_CRMT);
    CString strPath = ReportTable.GetFilePathName() + strTitle + _T(".xml");
    
    ReportTable.SetFilePathName(strPath);
    ReportTable.GenerateReport();
    
    return strPath;
}

CString CPostDrawAdapter::Build_PSC_FYCM_Table( int iBrgtype , const std::map<T_ELEM_K,T_FYCM_JTG>& mapdatas )
{
    T_TABLE_D TableResultD;
    TableResultD.Initialize();
    TableResultD.HeaderD.nHeaderRowCount = 1;
    if(iBrgtype == 1 || iBrgtype == 2 )
    {
        TableResultD.HeaderD.nHeaderColCount = 14;
        
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_ELEM));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_PART));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_LOAD));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_Short_Long));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_TYPE));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_OK));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_FT));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_FTL));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_FTR));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_FB));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_FBL));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_FBR));

        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_FMAX));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_ALW));

        
        TableResultD.RecordD.nRecordColCount = 14;
        TableResultD.RecordD.aRecordData.SetSize(4*mapdatas.size());

    }
    else
    {
        TableResultD.HeaderD.nHeaderColCount = 13;
        
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_ELEM));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_PART));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_LOAD));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_TYPE));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_OK));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_FT));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_FTL));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_FTR));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_FB));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_FBL));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_FBR));

        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_FMAX));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYCM_JTG_ALW));

        
        
        TableResultD.RecordD.nRecordColCount = 13;
        TableResultD.RecordD.aRecordData.SetSize(2*mapdatas.size());
    }
    
    
    CString strText;
    int iIndexLine = 0;
    LPCTSTR aLoadTerm[] = {_LS(IDS_TB_PSCD_SHORT), _LS(IDS_TB_PSCD_LONG)};

    for (std::map<T_ELEM_K,T_FYCM_JTG>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_FYCM_JTG& cbcs = pit->second;
        for (int i=0;i<6;i++)
        {
            T_FYCM_JTG_BASE cbcsBase = cbcs.FycmBase[i];
            CString strIJ = _T("");
            if(iBrgtype == 0)
            {
                 if(i!=1 && i != 3)
                     continue;
                 strIJ = (i==1) ? _T("I") : _T("J");
            }
            else if(iBrgtype == 3 )
            {
                if(i!=0 && i != 2)
                    continue;
                strIJ = (i==0) ? _T("I") : _T("J");
            }
            else
            {
                if(i!=1 && i != 3 && i!=4 && i != 5)
                    continue;
                strIJ = (i==1 || i==4 ) ? _T("I") : _T("J");
            }
            
            
            if(cbcsBase.bCHK)
            {
                strText.Format(_T("%d"), pit->first);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = strIJ;
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);        
                
                strText = GetLoadCombName(cbcsBase.LcomK);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);

                if(iBrgtype == 1 || iBrgtype == 2 )
                {
                    strText = aLoadTerm[cbcsBase.iLoadTerm-1];
                    TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                }


                
                strText = GetLoadCombType(cbcsBase.iMax);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = (cbcsBase.bOK)? _T("OK") : _T("NG");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                
                strText.Format(_T("%0.2f"), cbcsBase.dFT);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dFTL);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dFTR);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);

                strText.Format(_T("%0.2f"), cbcsBase.dFB);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dFBL);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dFBR);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);

                
                strText.Format(_T("%0.2f"), cbcsBase.dFMAX);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dALW);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
         
                                
                iIndexLine++;
                
            }
            
        }
        
    }
    
    TableResultD.RecordD.nRecordRowCount = iIndexLine;

    
    CReportTableProcess ReportTable;
    ReportTable.SetData(TableResultD);
    
    CString strTitle = _T("");
    if(iBrgtype == 3)
    {
        strTitle = _LS(IDS_TB_RESULTVIEW_JTG_FYCM_MAX);
    }
    else
    {
        strTitle = _LS(IDS_TB_RESULTVIEW_JTG_FYCM_MIN);
    }
    CString strPath = ReportTable.GetFilePathName() + strTitle + _T(".xml");
    
    ReportTable.SetFilePathName(strPath);
    ReportTable.GenerateReport();
    
    return strPath;
}

CString CPostDrawAdapter::Build_PSC_FPMS_Table( int iBrgtype , const std::map<T_ELEM_K,T_FPMS_JTG>& mapdatas )
{
    T_TABLE_D TableResultD;
    TableResultD.Initialize();
    TableResultD.HeaderD.nHeaderRowCount = 1;

        TableResultD.HeaderD.nHeaderColCount = 16;
        
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FPMS_JTG_ELEM));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FPMS_JTG_PART));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FPMS_JTG_LOAD));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FPMS_JTG_MAX_MIN));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FPMS_JTG_FP1));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FPMS_JTG_FP2));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FPMS_JTG_FP3));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FPMS_JTG_FP4));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FPMS_JTG_FP5));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FPMS_JTG_FP6));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FPMS_JTG_FP7));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FPMS_JTG_FP8));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FPMS_JTG_FP9));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FPMS_JTG_FP10));

        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FPMS_JTG_FMAX));
        TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FPMS_JTG_AFP));

        
        TableResultD.RecordD.nRecordColCount = 16;
        TableResultD.RecordD.aRecordData.SetSize(2*mapdatas.size());

 
    
    
    CString strText;
    int iIndexLine = 0;

    for (std::map<T_ELEM_K,T_FPMS_JTG>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_FPMS_JTG& cbcs = pit->second;
        for (int i=0;i<4;i++)
        {
            T_FPMS_JTG_BASE cbcsBase = cbcs.FpmsBase[i];
            CString strIJ = _T("");
            if(iBrgtype == 0)
            {
                 if(i!=1 && i != 3)
                     continue;
                 strIJ = (i==1)? _T("I") : _T("J");
            }
            else
            {
                if(i!=0 && i != 2)
                    continue;
                strIJ = (i==0)? _T("I") : _T("J");

            }
            
            
            if(cbcsBase.bCHK)
            {
                strText.Format(_T("%d"), pit->first);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText = strIJ;
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);        
                
                strText = GetLoadCombName(cbcsBase.LcomK);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                
                strText = (cbcsBase.bOK)? _T("OK") : _T("NG");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                
                strText.Format(_T("%0.2f"), cbcsBase.dFP1);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dFP2);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dFP3);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);

                strText.Format(_T("%0.2f"), cbcsBase.dFP4);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dFP5);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dFP6);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dFP7);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dFP8);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dFP9);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dFP10);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dFMAX);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcsBase.dAFP);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
         
                                
                iIndexLine++;
                
            }
            
        }
        
    }
    
    TableResultD.RecordD.nRecordRowCount = iIndexLine;

    
    CReportTableProcess ReportTable;
    ReportTable.SetData(TableResultD);
    
    CString strTitle = _T("");
    if(iBrgtype == 3)
    {
        strTitle = _LS(IDS_TB_RESULTVIEW_JTG_FPMS_MAX);
    }
    else
    {
        strTitle = _LS(IDS_TB_RESULTVIEW_JTG_FPMS_MIN);
    }
    CString strPath = ReportTable.GetFilePathName() + strTitle + _T(".xml");
    
    ReportTable.SetFilePathName(strPath);
    ReportTable.GenerateReport();
    
    return strPath;
}

CString CPostDrawAdapter::Build_PSC_FYPC_Table( const std::map<T_TDNA_K,T_FYPC_JTG>& mapdatas )
{
    T_TABLE_D TableResultD;
    TableResultD.Initialize();
    TableResultD.HeaderD.nHeaderRowCount = 1;
    TableResultD.HeaderD.nHeaderColCount = 6;
    
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYPC_JTG_TNDN));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYPC_JTG_OK));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYPC_JTG_FDL1));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYPC_JTG_FLL));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYPC_JTG_AFDL1));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYPC_JTG_AFLL));


    
    
    TableResultD.RecordD.nRecordColCount = 6;
    TableResultD.RecordD.aRecordData.SetSize(1*mapdatas.size());
    
    
    CString strText;
    int iIndexLine = 0;
    for (std::map<T_TDNA_K,T_FYPC_JTG>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_FYPC_JTG& cbcs = pit->second;
        //for (int i=0;i<6;i++)
        {
            
            
            if(cbcs.bCHK)
            {
                strText.Format(_T("%d"), pit->first);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);

                strText = (cbcs.bOK)? _T("OK") : _T("NG");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);

                strText.Format(_T("%0.2f"), cbcs.dAFDL1);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcs.dFLL);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcs.dAFDL1);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                strText.Format(_T("%0.2f"), cbcs.dAFLL);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                
                
                
                
                iIndexLine++;
                
            }
            
        }
        
    }
    
    TableResultD.RecordD.nRecordRowCount = iIndexLine;

    
    CReportTableProcess ReportTable;
    ReportTable.SetData(TableResultD);
    
    CString strTitle = _LS(IDS_TB_RESULTVIEW_JTG_FYPC);
    CString strPath = ReportTable.GetFilePathName() + strTitle + _T(".xml");
    
    ReportTable.SetFilePathName(strPath);
    ReportTable.GenerateReport();
    
    return strPath;
}

CString CPostDrawAdapter::Build_PSC_FYSG_Table( const std::map<T_ELEM_K,T_FYSG_D>& mapdatas )
{
    T_TABLE_D TableResultD;
    TableResultD.Initialize();
    TableResultD.HeaderD.nHeaderRowCount = 1;
    TableResultD.HeaderD.nHeaderColCount = 13;
    
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYSG_JTG_ELEM));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYSG_JTG_PART));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYSG_JTG_MAX_MIN));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYSG_JTG_STAGE));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYSG_JTG_OK));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYSG_JTG_FT));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYSG_JTG_FTL));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYSG_JTG_FTR));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYSG_JTG_FB));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYSG_JTG_FBL));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYSG_JTG_FBR));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYSG_JTG_FMAX));
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_PSCD_FYSG_JTG_ALW));

    
    
    
    TableResultD.RecordD.nRecordColCount = 13;
    TableResultD.RecordD.aRecordData.SetSize(4*mapdatas.size());
    
    
    CString strText;
    int iIndexLine = 0;

    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if(!pDoc)
         return _T("");
    for (std::map<T_ELEM_K,T_FYSG_D>::const_iterator pit = mapdatas.begin();pit!=mapdatas.end();++pit)
    {
        const T_FYSG_D& cbcs = pit->second;
        for (int i=0;i<4;i++)
        {
            T_FYSG_BASE fysgBase = cbcs.FysgBase[i];
            
            if(fysgBase.bCHK)
            {
                strText.Format(_T("%d"), pit->first);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);        
                
                strText = (i<2)? _T("I") : _T("J");     
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);        
                
                strText = (i%2 > 0)? _T("Min") : _T("Max");     
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);        
                
                T_STAG_D stagD;
                pDoc->m_pAttrCtrl->GetStag(fysgBase.StagK, stagD);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(stagD.StageName);        
                
                strText = (fysgBase.bOK)? _T("OK") : _T("NG");
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);        
                
                strText.Format(_T("%.3f"), fysgBase.dFT);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);        
                
                strText.Format(_T("%.3f"), fysgBase.dFTL);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);        
                
                strText.Format(_T("%.3f"), fysgBase.dFTR);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);    
                
                strText.Format(_T("%.3f"), fysgBase.dFB);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);        
                
                strText.Format(_T("%.3f"), fysgBase.dFBL);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);        
                
                strText.Format(_T("%.3f"), fysgBase.dFBR);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);        

                
                strText.Format(_T("%.3f"), fysgBase.dFMAX);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);        
                
                strText.Format(_T("%.3f"), fysgBase.dALW);
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);        
                
                iIndexLine++;
                
            }
        }
    }
    
    TableResultD.RecordD.nRecordRowCount = iIndexLine;

    
    CReportTableProcess ReportTable;
    ReportTable.SetData(TableResultD);
    
    CString strTitle = _LS(IDS_TB_RESULTVIEW_JTG_FYSG);
    CString strPath = ReportTable.GetFilePathName() + strTitle + _T(".xml");
    
    ReportTable.SetFilePathName(strPath);
    ReportTable.GenerateReport();
    
    return strPath;
}

CString CPostDrawAdapter::Build_Bearing_Reac_Table(CArray<T_NODE_K, T_NODE_K>& ValueNodeKeyList,CArray<T_REAC_D, T_REAC_D&>& ReacDataList)
{
    

    T_TABLE_D TableResultD;
    TableResultD.Initialize();
    TableResultD.HeaderD.nHeaderRowCount = 1;
    TableResultD.HeaderD.nHeaderColCount = 7;


    
    TableResultD.HeaderD.aTitleName.Add(_LS(IDS_TB_POSTDISP_Node));
    TableResultD.HeaderD.aTitleName.Add(CString(_LS(IDS_TB_POSTDISP_FX)) + _T("(kN)"));
    TableResultD.HeaderD.aTitleName.Add(CString(_LS(IDS_TB_POSTDISP_FY)) + _T("(kN)"));
    TableResultD.HeaderD.aTitleName.Add(CString(_LS(IDS_TB_POSTDISP_FZ)) + _T("(kN)"));
    TableResultD.HeaderD.aTitleName.Add(CString(_LS(IDS_TB_POSTDISP_MX)) + _T("(kN·m)"));
    TableResultD.HeaderD.aTitleName.Add(CString(_LS(IDS_TB_POSTDISP_MY)) + _T("(kN·m)"));
    TableResultD.HeaderD.aTitleName.Add(CString(_LS(IDS_TB_POSTDISP_MZ)) + _T("(kN·m)"));


    
    
    
    TableResultD.RecordD.nRecordColCount = 7;
    TableResultD.RecordD.aRecordData.SetSize(ReacDataList.GetSize());
    
    
    CString strText;
    int iIndexLine = 0;


    for (int j=0;j<ReacDataList.GetSize();j++)
    {
                T_REAC_D reac = ReacDataList.GetAt(j);
                
                strText.Format(_T("%d"), ValueNodeKeyList.GetAt(j));
                TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);        
                
                for (int n=0;n<6;n++)
                {
                    strText.Format(_T("%.3f"), reac.dblReac[n]);
                    TableResultD.RecordD.aRecordData[iIndexLine].Add(strText);
                } 
                iIndexLine++;
                
            
    }
    
    
    TableResultD.RecordD.nRecordRowCount = iIndexLine;

    CReportTableProcess ReportTable;
    ReportTable.SetData(TableResultD);
    
    CString strTitle = _LS(IDS_TB_RESULTVIEW_Reaction_Global_);
    CString strPath = ReportTable.GetFilePathName() + strTitle + _T(".xml");
    
    ReportTable.SetFilePathName(strPath);
    ReportTable.GenerateReport();
    
    return strPath;
    
    
}

CString CPostDrawAdapter::Build_Bearing_Force_Graphic( const CArray<T_ELEM_K, T_ELEM_K>& ValueElemKeyList,const CArray<T_STRB_D, T_STRB_D&>& ForceDataList,const CString& loadcombname,int iForceType/* = 5*/)
{
    CDrawCenter drawer(m_DocPath);
    std::vector<CMGData> _data;
    
    CString strForceType[6] = {_T("FX"),_T("FY"),_T("FZ"),_T("MX"),_T("MY"),_T("MZ")};
    
    
    CMGData mgMaxAllow;
    mgMaxAllow.m_ShowLineWidth = ALLOWWIDTH;
    mgMaxAllow.m_ShowLineColor = MAXALLOWCOLOR;
    
    
    mgMaxAllow.m_StrName = loadcombname;
    
    drawer.GetConfig()->m_X_AxisText = _LS(IDS_AMR_CHRP_Graphic_Elem);
    drawer.GetConfig()->m_Y_AxisText = strForceType[iForceType-1];
    if(iForceType <= 3)
        drawer.GetConfig()->m_Y_Unit = _T("kN·m");
    else
        drawer.GetConfig()->m_Y_Unit = _T("kN");
    
    
    
    mgMaxAllow.m_bShowMaxMin = TRUE;
    drawer.GetConfig()->m_bPostiveMax = TRUE;
    
    //for (std::map<T_ELEM_K,T_CBCS_CH>::const_iterator pit = ValueElemKeyList.begin();pit!=mapdatas.end();++pit)
    if(ForceDataList.GetSize() != ValueElemKeyList.GetSize())
    {
        _ASSERT(0);
        return _T("");
    }
    
    for(int i=0;i<ValueElemKeyList.GetSize();i++)
    {
        const T_STRB_D& cbcs = ForceDataList.GetAt(i);
        
        int iKey = ValueElemKeyList.GetAt(i);
        mgMaxAllow.m_ShowDatas.push_back(make_pair(iKey,make_pair(cbcs.dblForce[0][iForceType-1],cbcs.dblForce[3][iForceType+6-1])));
        
        
    }
    
    _data.push_back(mgMaxAllow);
    
    return drawer.Draw(_data);
}
