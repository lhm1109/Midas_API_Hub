#pragma once

#include "HeaderPre.h"

namespace dgn
{
    namespace def
    {
        class CRptSettings;
    }

    namespace converter
    {
        class CProductDBDocHelper;
    }
}

namespace std
{
    class thread;
}

class IDgnPerformDataBase;

class __MY_EXT_CLASS__ CDgnPerformRunner
{
    typedef CArray<T_ELEM_K> aElemKey;
    typedef dgn::def::CRptSettings CRptSettings;
    typedef dgn::converter::CProductDBDocHelper CProductDBDocHelper;

public:
    CDgnPerformRunner(IDgnPerformDataBase* pIDataBase);
    virtual ~CDgnPerformRunner();

public:
    int  Run(const T_KEY_LIST& aElemK, const T_KEY_LIST* paAllElemK = nullptr);
    bool RunDetailReport(const T_KEY_LIST& aElemK, const CArray<int, int&>& aAllElemK, const CRptSettings& Settings);
    bool RunDetailReport(const T_KEY_LIST& aElemK, const T_KEY_LIST& aAllElemK, const CRptSettings& Settings);
    bool SetDataPool(const T_KEY_LIST& aElemK);

    void FillReportCompanySetting(CRptSettings& Settings);

public:
    HANDLE* GetPtrBackgroundThread();
	void SetBackGroundThread(bool bFlag);
    void WaitBackgroundThread();

private:
    bool RunPerform(const T_KEY_LIST& aElemK) const;

private:
    bool GetKeyLists(const T_KEY_LIST& aElemK, bool bRecal, T_KEY_LIST& raKeyTarget, T_KEY_LIST& raKeyExistResult, T_KEY_LIST& raKeySelected) const;
    void PlayInitDefault(bool bReCalc, const T_KEY_LIST& caKeyExistResult, const T_KEY_LIST& caKeySelected);
    bool PlayPerformThread(const T_KEY_LIST& caKeyTarget) const;
    bool SetDgnLcomList(const T_KEY_LIST& caKey);
    bool IsDirectProduct() const;

    bool IsAvailableReport(const dgn::def::CRptSettings& Settings) const;

private:
    IDgnPerformDataBase* GetIDataBase() const;
    const CProductDBDocHelper* GetDBDocHelper() const;

private:
    IDgnPerformDataBase* m_pIDataBase;
    const CProductDBDocHelper* m_pDBDocHelper;
    HANDLE m_hBackgroundThread;
	bool m_bExistBackgroundThread;
};

#include "HeaderPost.h"
