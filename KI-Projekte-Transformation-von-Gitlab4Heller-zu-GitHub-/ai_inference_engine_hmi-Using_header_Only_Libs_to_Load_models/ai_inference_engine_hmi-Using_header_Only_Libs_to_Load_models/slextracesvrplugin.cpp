///////////////////////////////////////////////////////////////////////////////
/*! \file   slextracesvrplugin.cpp
 *  \brief  Plugin for screens, dialogs and forms
 *
 *  This file is part of the HMI Solutionline Programming Package
 *
 *  (C) Copyright Siemens AG A&D MC 2004. All rights reserved.
 */
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "slgfwpluginmakros.h"
#include "slextracesvrform.h"

///////////////////////////////////////////////////////////////////////////////
// global defines and macros
SL_GFW_BEGIN_PLUGIN_EXPORT_MODULE(slextracesvr)
    SL_GFW_DIALOGFORM_PLUGIN_EXPORT(SlExTraceSvrForm)
SL_GFW_END_PLUGIN_EXPORT_MODULE(slextracesvr)
