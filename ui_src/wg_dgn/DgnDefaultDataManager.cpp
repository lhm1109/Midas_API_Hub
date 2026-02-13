#include "StdAfx.h"

#include <functional>

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\EditData.h"

#include "wg_dgn.h"

#include "DgnConCodeDlg.h"
#include "DgnConCodeNewDlg.h"
#include "DgnConPhiDlg.h"
#include "DgnConLossFactorDlg.h"

#include "DgnDefaultDataManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CDgnDefaultDataManager::CDgnDefaultDataManager(CDBDoc* pDoc) :
    m_pDoc(pDoc)
{
}

CDgnDefaultDataManager::~CDgnDefaultDataManager()
{
}

bool CDgnDefaultDataManager::DoSetDefaultDcon_Civil(const CString& crDesignCode) const
{
    //TODO Refactoring이 시급합니다.
    //TODO Dcon에 대한 Default Data를 얻고 싶어서 이런 짓을 하는데..
    //TODO Default Data를 날 잡고 정리한번 해야 합니다ㅠㅠ
    //TODO 이 코드를 보시는 분께 죄송을 드립..

    if ( GetDoc()->m_pAttrCtrl->ExistDcon() )
    {
        if ( !GetDoc()->m_pEditData->DelDcon() )
        {
            ASSERT(0); return false;
        }
    }

    //DgnCode Setting해서 Dialog가 Default값 채우도록 먼저 넣어주고
    //Design Code만 채워주고 빈 깡통 Dcon
    T_DCON_D Data;
    Data.Initialize();
    Data.DesignCode = crDesignCode;
    if ( !GetDoc()->m_pEditData->AddDcon(Data) )
    {
        ASSERT(0); return false;
    }

    //임시로 대화상자 생성해서 Dcon 데이터를 대화상자로부터 갱신
    // Update Dcon (By Design Code)
    {
        std::unique_ptr<CDgnConCodeDlg> pDlg(new CDgnConCodeDlg());
        if ( !pDlg->Create(CDgnConCodeDlg::IDD) )
        {
            ASSERT(0); return false;
        }

        pDlg->ShowWindow(SW_HIDE);
        pDlg->GetDataFromDlg(Data);
        pDlg->DestroyWindow();
    }

    // Update Dcon (Factor)
    {
        std::unique_ptr<CDgnConPhiDlg> pDlg(new CDgnConPhiDlg());
        if ( !pDlg->Create(CDgnConPhiDlg::IDD) )
        {
            ASSERT(0); return false;
        }

        pDlg->ShowWindow(SW_HIDE);
        pDlg->GetDataFromDlg(Data);
        pDlg->DestroyWindow();
    }

    //Update Dcon (Loss Factor)
    {
        std::unique_ptr<CDgnConLossFactorDlg> pDlg(new CDgnConLossFactorDlg());
        if ( !pDlg->Create(CDgnConLossFactorDlg::IDD) )
        {
            ASSERT(0); return false;
        }

        pDlg->ShowWindow(SW_HIDE);
        pDlg->GetDataFromDlg(Data);
        pDlg->DestroyWindow();
    }

    //Design Code + Default Data Dcon 갱신
    if ( !GetDoc()->m_pEditData->AddDcon(Data) )
    {
        ASSERT(0); return false;
    }

    return true;
}

bool CDgnDefaultDataManager::DoSetDefaultDcon_Gen(const CString& crDesignCode) const
{
    //TODO Refactoring이 시급합니다.
    //TODO Dcon에 대한 Default Data를 얻고 싶어서 이런 짓을 하는데..
    //TODO Default Data를 날 잡고 정리한번 해야 합니다ㅠㅠ
    //TODO 이 코드를 보시는 분께 죄송을 드립..

    if ( GetDoc()->m_pAttrCtrl->ExistDcon() )
    {
        if ( !GetDoc()->m_pEditData->DelDcon() )
        {
            ASSERT(0); return false;
        }
    }

    T_DCON_D Data;
    Data.Initialize();
    Data.DesignCode = crDesignCode;
    if ( !GetDoc()->m_pEditData->AddDcon(Data) )
    {
        ASSERT(0); return false;
    }

    // Update Dcon (DgnCode) Civil과 달리 New를 사용함.
    {
        std::unique_ptr<CDgnConCodeNewDlg> pDlg(new CDgnConCodeNewDlg());
        if ( !pDlg->Create(CDgnConCodeNewDlg::IDD) )
        {
            ASSERT(0); return false;
        }

        pDlg->ShowWindow(SW_HIDE);
        pDlg->GetDataFromDlg(Data);
        pDlg->DestroyWindow();
    }

    // Update Dcon (Factor)
    {
        std::unique_ptr<CDgnConPhiDlg> pDlg(new CDgnConPhiDlg());
        if ( !pDlg->Create(CDgnConPhiDlg::IDD) )
        {
            ASSERT(0); return false;
        }

        pDlg->ShowWindow(SW_HIDE);
        pDlg->GetDataFromDlg(Data);
        pDlg->DestroyWindow();
    }

    //Update Dcon (Loss Factor)
    {
        std::unique_ptr<CDgnConLossFactorDlg> pDlg(new CDgnConLossFactorDlg());
        if ( !pDlg->Create(CDgnConLossFactorDlg::IDD) )
        {
            ASSERT(0); return false;
        }

        pDlg->ShowWindow(SW_HIDE);
        pDlg->GetDataFromDlg(Data);
        pDlg->DestroyWindow();
    }

    //Design Code + Default Data Dcon 갱신
    if ( !GetDoc()->m_pEditData->AddDcon(Data) )
    {
        ASSERT(0); return false;
    }

    return true;
}

CDBDoc* CDgnDefaultDataManager::GetDoc() const
{
    if ( !m_pDoc )
    {
        ASSERT(0); return nullptr;
    }

    return m_pDoc;
}