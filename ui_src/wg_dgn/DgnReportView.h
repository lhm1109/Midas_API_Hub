#pragma once

#include <afxwin.h>
#include "..\choga10\scrollvi.h"
#include "..\dgnengine\idesign\DGN_def\ReportTypeDef.h"
#include "..\dgnengine\idesign\DGN_link\IDgnReport.h"
#include "..\dgnengine\idesign\DGN_def\DgnInput.h"
#include "..\dgnengine\idesign\DGN_def\DgnCodeDef.h"
#include "..\dgnengine\idesign\DGN_def\DgnTypeEnumDef.h"
#include "..\dgnengine\idesign\DGN_def\DgnModeDef.h"
#include "..\dgnengine\idesign\DGN_def\DesignCaseDef.h"
#include "..\dgnengine\idesign\DGN_def\RptSettingsDef.h"

#include "DgnCodeTool.h"
#include "DgnReportMaker.h"
#include "IDgnDataBase.h"
#include "DgnPerformRunner.h"
#include "..\mit_lib\UrlReader.h"

#include "HeaderPre.h"

class CACSDataBase;

namespace dgn
{
    namespace def
    {
        class CDgnInput;
        class CReportType;
    }
}

class IDgnDataBase;
class IDgnPerformDataBase;

template <typename T>
class __MY_EXT_CLASS__ TDgnReportView : public CScrollVi
{
public:
    TDgnReportView() { m_iCurElem = 0; }
    virtual ~TDgnReportView() {}

public:
    void SetElemNo(int nElemNo) { m_iCurElem = nElemNo; }
    void SetDataBase(T* pDataBase) { m_pDataBase = pDataBase; }
    dgn::def::CDgnInput* GetInput() { return &m_Input; }
    dgn::def::CDgnInput* GetInputShearY() { return &m_InputShearY; }
    dgn::def::CDgnInput* GetInputShearZ() { return &m_InputShearZ; }

private:
    bool makeReport(CDC* pDC, const dgn::def::enReportType& enRptType, CString strFileName)
    {
        if ( GetDgnPerformDataBase()->IsSupportedSectionDataBase() )
        {
            CDgnReportMaker Maker(GetDgnDataBase());
            if ( m_iCurElem != GetDgnDataBase()->GetDgnElemNo() )
            {
                m_iCurElem = GetDgnDataBase()->GetDgnElemNo();

                T_DGN_RPT_DATA RptD;
                if ( !GetDgnDataBase()->GetCriticalReportInfo(RptD) )
                {
                    ASSERT(0); return false;
                }

                m_ItemInput.clear();
                Maker.GetReportInfo(RptD, &m_Input, &m_InputShearY, &m_InputShearZ, m_ItemInput);
            }

            // Set Head Information
            T_PJCF_D  projData;
            if ( !CDBDoc::GetDocPoint()->m_pAttrCtrl->GetPjcf(projData) ) projData.Initialize();
            CString strProdName=_T(""), strProdNameVer=_T(""), strAcademicLan=_T("");
            CDBLib::GetProdNameVersion(strProdName, strProdNameVer, strAcademicLan);
            CString strCompany = m_strCompany;
            CString strURL = projData.strMail;
            CString strTel; if ( !projData.strPhone.IsEmpty() ) strTel.Format(_T("TEL:%s"), projData.strPhone);
            CString strFax; if ( !projData.strFax.IsEmpty() )   strFax.Format(_T("FAX:%s"), projData.strFax);

            if ( strCompany.IsEmpty() ) strCompany = D_PRODUCT_NAME;
            if ( projData.strMail.IsEmpty() && projData.strPhone.IsEmpty() && projData.strFax.IsEmpty() ) {
                strURL     = D_MIDAS_URL;
                strTel.Format(_T("TEL:"), D_MIDAS_PHONE);
                strFax.Format(_T("FAX:"), D_MIDAS_FAX);
            }

            T_DGN_RPT_HEAD_INFO HeadInfo;
            HeadInfo.bUserInfor   = true;
            HeadInfo.sTitle       = static_cast<std::wstring>(CT2W(m_strTitle));
            HeadInfo.sProductVer  = static_cast<std::wstring>(CT2W(strProdNameVer));
            HeadInfo.sCompanyName = static_cast<std::wstring>(CT2W(strCompany));
            HeadInfo.sWebSite     = static_cast<std::wstring>(CT2W(strURL));
            HeadInfo.sTelNo       = static_cast<std::wstring>(CT2W(strTel));
            HeadInfo.sFaxNo       = static_cast<std::wstring>(CT2W(strFax));

            // make summary report
            std::wstring strPath = static_cast<std::wstring>(CT2W(strFileName));
            return Maker.RunSummaryViewReport(m_ItemInput, this, pDC, enRptType, GetDgnDataBase()->GetDgnRptNation(), strPath, HeadInfo);
        }
        else
        {
            const std::wstring strFileName = CT2W(_T("Test.docx"));
            const std::wstring strN  = CT2W(_T("N"));
            const std::wstring strMM = CT2W(_T("MM"));

            dgn::def::CRptSettings Settings;
            Settings.SetDgnCode(dgn::def::enDgnCode::ACI318M_19);
            Settings.SetDgnType(dgn::def::enDgn::RCS);
            Settings.SetFileOutType(dgn::def::enReportType::CDC);
            Settings.SetFileOutPath(strFileName);
            Settings.SetReportLevel(dgn::def::enReportLevel::Detail);
#if defined (_CIVIL)
			Settings.SetRptUnit(dgn::def::enPredefinedCode);
#else
			Settings.SetRptUnit(dgn::def::enUserUnit, strN, strMM);
#endif
            Settings.SetViewPointer(this);
            Settings.SetDC(pDC);
            Settings.SetDesignCase(INTEGRATIONDESIGNCASE);

            CDgnPerformRunner Runner(GetDgnPerformDataBase());
            T_KEY_LIST aKey;
            aKey.Add(m_iCurElem);
            return Runner.RunDetailReport(aKey, aKey, Settings);
        }
    }

protected:
    virtual void OnDraw(CDC* pDC) override final
    {
        if ( !pDC->IsPrinting() )
        {
            if ( GetDgnPerformDataBase()->IsIdgnViewReport() )
            {
                makeReport(pDC, dgn::def::enReportType::CDC, _T(""));
            }
            else
            {
                CScrollVi::OnDraw(pDC);
            }
        }
    }

    virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo) override final
    {
        if ( pDC->IsPrinting() )
        {
            if ( GetDgnPerformDataBase()->IsIdgnViewReport() )
            {
                makeReport(pDC, dgn::def::enReportType::PRINT, _T(""));
            }
            else
            {
                CScrollVi::OnPrint(pDC, pInfo);
            }
        }
    }

    virtual void EMF_FileSave(CString strFileName=_T("")) override final
    {
        if ( GetDgnPerformDataBase()->IsIdgnViewReport() )
        {
#ifdef _TRIAL
            if ( CDBDoc::GetDocPoint()->IsLimitedTrialVersion() ) {
                CDBDoc::GetDocPoint()->ShowLimitedTrialVersionErrMsg();
                return;
            }
#endif

            if ( strFileName.IsEmpty() ) {
                T_PREFERENCE rPreference;
                CDBDoc::GetDocPoint()->m_pInitCtrl->GetPreference(rPreference);
                int nFilePath = rPreference.General.nOptDefFileOpen;
                if ( nFilePath == 1 ) strFileName = rPreference.General.strFileOpenPath+_T("*.emf");

                CString strTmp;
                strTmp.Format(_T("Design (*.emf)|*.emf|All files(*.*)|*.*||"));
                CFileDialog dlg(FALSE, _T("emf"), strFileName,
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strTmp);
                if ( dlg.DoModal() != IDOK ) return;
                strFileName = dlg.GetPathName();
            }

            CString strName;
            int iNum = strFileName.ReverseFind('.');
            if ( iNum > 0 ) strName = strFileName.Left(iNum);
            else strName = strFileName;
            strFileName = strName;
            makeReport(GetDC(), dgn::def::enReportType::EMF, strFileName);
        }
        else
        {
            CScrollVi::EMF_FileSave(strFileName);
        }
    }

    virtual void BMP_FileSave(CString strFileName=_T("")) override final
    {
        if ( GetDgnPerformDataBase()->IsIdgnViewReport() )
        {
#ifdef _TRIAL
            if ( CDBDoc::GetDocPoint()->IsLimitedTrialVersion() ) {
                CDBDoc::GetDocPoint()->ShowLimitedTrialVersionErrMsg();
                return;
            }
#endif

            if ( strFileName.IsEmpty() ) {
                T_PREFERENCE rPreference;
                CDBDoc::GetDocPoint()->m_pInitCtrl->GetPreference(rPreference);
                int nFilePath = rPreference.General.nOptDefFileOpen;
                if ( nFilePath == 1 ) strFileName = rPreference.General.strFileOpenPath+_T("*.bmp");

                CString strTmp;
                strTmp.Format(_T("Design (*.bmp)|*.bmp|All files(*.*)|*.*||"));
                CFileDialog dlg(FALSE, _T("bmp"), strFileName,
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strTmp);
                if ( dlg.DoModal() != IDOK ) return;
                strFileName = dlg.GetPathName();
            }

            CString strName;
            int iNum = strFileName.ReverseFind('.');
            if ( iNum > 0 ) strName = strFileName.Left(iNum);
            else strName = strFileName;
            strFileName = strName;

            makeReport(GetDC(), dgn::def::enReportType::BMP, strFileName);
        }
        else
        {
            CScrollVi::BMP_FileSave(strFileName);
        }
    }

private:
    IDgnDataBase* GetDgnDataBase() { return (IDgnDataBase*)m_pDataBase; }
    IDgnPerformDataBase* GetDgnPerformDataBase() { return (IDgnPerformDataBase*)m_pDataBase; }

private:
    T* m_pDataBase;
    dgn::def::CDgnInput m_Input;
    dgn::def::CDgnInput m_InputShearY;
    dgn::def::CDgnInput m_InputShearZ;
    dgn::link::ItemInputList m_ItemInput;
    int m_iCurElem;
};

#include "HeaderPost.h"
