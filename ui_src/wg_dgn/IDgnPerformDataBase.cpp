#include "stdafx.h"

#include <mutex>

#include "..\dgnengine\idesign\DGN_link\IDgnDocCtrl.h"
#include "..\dgnengine\idesign\DGN_link\IDgnPerform.h"
#include "..\dgnengine\idesign\DGN_link\IDgnDoc.h"
#include "..\dgnengine\idesign\DGN_def\DgnLcomDef.h"

#include "..\wg_db\DBDoc.h"

#include "IDgnPerformDataBase.h"
#include "TDBDataFunctor.h"
#include "DgnStruct.h"

using namespace dgn::def;
using namespace dgn::link;
using namespace dgn::converter;

IDgnPerformDataBase::IDgnPerformDataBase()
{
}

IDgnPerformDataBase::~IDgnPerformDataBase()
{
}
