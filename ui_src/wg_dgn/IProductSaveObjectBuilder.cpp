#include "StdAfx.h"

#include "../wg_db/DBDoc.h"

#include "ProductDBDocHelper.h"

#include "IProductSaveObjectBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::converter;

IProductSaveObjectBuilder::IProductSaveObjectBuilder()
{
}

IProductSaveObjectBuilder::~IProductSaveObjectBuilder()
{
}

IProductDataBaseHelper* IProductSaveObjectBuilder::CreateDataBaseHelper(IDgnPerformDataBase* pIDataBase) const
{
    return DoCreateDataBaseHelper(pIDataBase);
}

IProductResultTracker* IProductSaveObjectBuilder::CreateResultTracker(const CProductDBDocHelper* pDBDocHelper) const
{
    return DoCreateResultTracker(pDBDocHelper);
}

IProductResultInitializer* IProductSaveObjectBuilder::CreateInitializer() const
{
    return DoCreateInitializer();
}

IProductResultSaver* IProductSaveObjectBuilder::CreateSaver(const TProductMembType& TMembType,
                                                            const CProductDBDocHelper* pDBDocHelper,
                                                            const IProductDataBaseHelper* pIDataBaseHelper) const
{
    return DoCreateSaver(TMembType, pDBDocHelper, pIDataBaseHelper);
}