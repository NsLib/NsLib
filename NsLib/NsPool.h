#ifndef NS_POOL_H
#define	NS_POOL_H

#include "NsInternalUse/NsCxx11Support.h"

/*!
 * \file   NsPool.h
 * \author 凝霜(Loki)
 * \brief  提供各种Pool, 包括memory pool, object pool, thread pool
 */

/// \defgroups NsPool

#include "NsInternalUse/NsCxx11Support.h"

// for malloc() and free()
#include <cstdlib>
// for assert()
#include <cassert>
// for placement new and bad_alloc
#include <new>

#include <cstdarg>

#include "NsInternalUse/NsDebugInfo.h"
#include "NsUtility/NsUncopyale.h"

#include "NsPool/NsObjectPool.h"
#include "NsPool/NsINewFromObjectPool.h"

#endif

