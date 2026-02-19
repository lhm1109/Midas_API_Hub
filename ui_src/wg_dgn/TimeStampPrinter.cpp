#include "StdAfx.h"

#include "TimeChecker.h"

#include "TimeStampPrinter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::converter;

typedef CTimeStampPrinter::TColIdx  _TColIdx;
typedef CTimeStampPrinter::TColData _TColData;

CString CTimeStampPrinter::m_strFileName;
std::map<_TColIdx, _TColData> CTimeStampPrinter::m_mColIdxData;

void CTimeStampPrinter::Init(crStr strFileName)
{
    CString Temp;
    Temp.Format(_T("%s.txt"),
                strFileName);
    m_strFileName = Temp;

    m_mColIdxData.clear();
}

void CTimeStampPrinter::AddKey(crStr strTitle,
                               T_KEY nKey)
{
    TColIdx Index;
    Index.Priority = TColIdx::Key;
    Index.str = ConvSameSpaceStr(strTitle);

    m_mColIdxData[Index].vStr.push_back(ConvSameSpaceStr(nKey));
}

void CTimeStampPrinter::AddTime1(crStr strTitle,
                                 const CTimeChecker& Time)
{
    return AddTime(strTitle, Time, TColIdx::enPriority::Time1);
}

void CTimeStampPrinter::AddTime2(crStr strTitle,
                                 const CTimeChecker& Time)
{
    return AddTime(strTitle, Time, TColIdx::enPriority::Time2);
}

void CTimeStampPrinter::AddTime3(crStr strTitle,
                                 const CTimeChecker& Time)
{
    return AddTime(strTitle, Time, TColIdx::enPriority::Time3);
}

void CTimeStampPrinter::AddTime4(crStr strTitle,
                                 const CTimeChecker& Time)
{
    return AddTime(strTitle, Time, TColIdx::enPriority::Time4);
}

void CTimeStampPrinter::PrintAll()
{
    auto nMaxCount = 0;
    for ( const auto& ColIdxData : m_mColIdxData )
    {
        const auto& nCurCount = 
            static_cast<int>(ColIdxData.second.vStr.size());

        if ( nMaxCount < nCurCount )
        {
            nMaxCount = nCurCount;
        }
    }

    //빈 데이터는 '-'로 채운다.
    CString strEmpty;
    strEmpty.Format(_T("%20s\t"), _T("-"));

    for ( auto& ColIdxData : m_mColIdxData )
    {
        auto& Data = ColIdxData.second;

        const auto& nCurCount = static_cast<int>(Data.vStr.size());
        const auto& nEmptyCount = nMaxCount - nCurCount;
        for ( int nEmpty = 0; nEmpty < nEmptyCount; ++nEmpty )
        {
            Data.vStr.push_back(strEmpty);
        }
    }

    CStdioFile File;
    if ( File.Open(m_strFileName,
                   CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate) )
    {
        File.SeekToEnd();

        for ( const auto& ColIdxData : m_mColIdxData )
        {
            const auto& Idx = ColIdxData.first;
            const auto& strTitle = Idx.str;
            File.WriteString(strTitle);
        }

        File.WriteString(_T("\n"));

        const auto& mContentRowSize =
            static_cast<int>(m_mColIdxData.begin()->second.vStr.size());
        for ( int nCRow = 0; nCRow < mContentRowSize; ++nCRow )
        {
            for ( const auto& ColIdxData : m_mColIdxData )
            {
                const auto& Data = ColIdxData.second;
                const auto& strContent = Data.vStr[nCRow];
                File.WriteString(strContent);
            }

            File.WriteString(_T("\n"));
        }

        File.Close();
    }
}

void CTimeStampPrinter::AddTime(crStr strTitle,
                                const CTimeChecker& Time,
                                TColIdx::enPriority P)
{
    TColIdx Index;
    Index.Priority = P;
    Index.str = ConvSameSpaceStr(strTitle);

    m_mColIdxData[Index].vStr.push_back(ConvSameSpaceStr(Time));
}

CString CTimeStampPrinter::ConvSameSpaceStr(const CString& str)
{
    CString Temp;
    Temp.Format(_T("%20s\t"), str);

    return Temp;
}

CString CTimeStampPrinter::ConvSameSpaceStr(T_KEY Key)
{
    CString Temp;
    Temp.Format(_T("%20d\t"), Key);

    return Temp;
}

CString CTimeStampPrinter::ConvSameSpaceStr(const CTimeChecker& Time)
{
    CString Temp;
    Temp.Format(_T("%20.6f\t"), Time.GetCurDuration_seconds());

    return Temp;
}

CTimeStampPrinter::CTimeStampPrinter()
{
}

CTimeStampPrinter::~CTimeStampPrinter()
{
}