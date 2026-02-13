#pragma once

#include <string>
#include <array>
#include "..\dgnengine\idesign\DGN_def\DgnReport.h"
#include "..\dgnengine\idesign\DGN_def\ReportTypeDef.h"
#include "..\dgnengine\idesign\DGN_def\ReportHeadInfo.h"
#include "..\dgnengine\idesign\DGN_def\DgnInput.h"
#include "..\dgnengine\idesign\DGN_def\DgnResult.h"
#include "..\dgnengine\idesign\DGN_def\DgnReport.h"
#include "..\dgnengine\idesign\DGN_def\RptSettingsDef.h"
#include "..\dgnengine\idesign\DGN_def\DgnItemDef.h"
#include "..\dgnengine\idesign\DGN_def\ReportCritDef.h"
#include "..\dgnengine\idesign\DGN_def\RptMembInfoDef.h"

#include "..\dgnengine\idesign\DGN_link\IDgnReport.h"

#include "..\dgnengine\idesign\DGN_def\SectionDef.h"
#include "..\dgnengine\idesign\DGN_def\MaterialDef.h"

#include "..\choga10\scrollvi.h"
#include "IDgnDataBase.h"

#include "HeaderPre.h"

namespace dgn
{
    namespace link
    {
        typedef std::vector<std::pair<item::enDgnResultItem, std::pair<def::CDgnInput*, def::ReportCrit>>> ItemInputList;
        typedef std::vector<ItemInputList> MItemInputList;
        typedef std::vector<dgn::def::CRptMembInfoLegacy> MembInfoListLegacy;
    }
}

class CDC;
class CScrollVi;
struct T_DGN_RPT_DATA;
class __MY_EXT_CLASS__ IDgnReportMaker
{
public:
    IDgnReportMaker() {}
    virtual ~IDgnReportMaker() {}

public:
    virtual bool RunSummaryViewReport(const dgn::link::ItemInputList& ItemInput, CScrollVi* pView, CDC* pDC, const dgn::def::enReportType& enRptType, const std::wstring& sRtplang, const std::wstring& strPath, const T_DGN_RPT_HEAD_INFO& HeadInfo) = 0;
    virtual bool RunDetailReport(const dgn::def::enReportType& enRptType, const std::wstring& strPath,
                                 const dgn::link::MItemInputList& vItemInput,
                                 const dgn::link::MembInfoListLegacy&  vMembInfo) = 0;
    
    virtual bool RunDetailReport() = 0;
};

class __MY_EXT_CLASS__ CDgnReportMaker : public IDgnReportMaker
{
public:
    CDgnReportMaker(IDgnDataBase* pDataBase);
    virtual ~CDgnReportMaker();

public:
    virtual bool RunSummaryViewReport(const dgn::link::ItemInputList& ItemInput, CScrollVi* pView, CDC* pDC, const dgn::def::enReportType& enRptType, const std::wstring& sRtplang, const std::wstring& strPath, const T_DGN_RPT_HEAD_INFO& HeadInfo) override final;
    virtual bool RunDetailReport(const dgn::def::enReportType& enRptType, const std::wstring& strPath,
                                 const dgn::link::MItemInputList& vItemInput,
                                 const dgn::link::MembInfoListLegacy&  vMembInfo) override final;
    virtual bool RunDetailReport() override;

public:
    bool GenerateSummaryReport(const dgn::link::ItemInputList& ItemInput, const dgn::def::CDgnReport& Report);
    bool GetReportInfo(const T_DGN_RPT_DATA& RptData, dgn::def::CDgnInput* pInp, dgn::def::CDgnInput* pInpShrY, dgn::def::CDgnInput* pInpShrZ, dgn::link::MItemInputList& vItemInput, dgn::link::MembInfoListLegacy&  vMembInfo);
    bool GetReportInfo(const T_DGN_RPT_DATA& RptData, dgn::def::CDgnInput* pInp, dgn::def::CDgnInput* pInpShrY, dgn::def::CDgnInput* pInpShrZ, dgn::link::ItemInputList& aInput);

private:
    bool IsAvailable(const dgn::def::CRptSettings& Settings);

private:
    IDgnDataBase* m_pDataBase;
};

#include "HeaderPost.h"
