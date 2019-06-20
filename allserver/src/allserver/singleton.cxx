//////////////////////////////////////////////////////////////////
//
// singleton.cxx
//
// Copyright (c) 2001-2017, Jan Willamowius
//
// This work is published under the GNU Public License version 2 (GPLv2)
// see file COPYING for details.
// We also explicitly grant the right to link this code
// with the OpenH323/H323Plus and OpenSSL library.
//
//////////////////////////////////////////////////////////////////
#include <pjlib.h>
#include "singleton.h"

static int singleton_cnt = 0;

listptr<SingletonBase> SingletonBase::_instance_list;

SingletonBase::SingletonBase(const char *n) : m_name(n)
{
	++singleton_cnt;
	PJ_LOG(3, (__FILE__,"Create instance:%s (%d)",m_name,singleton_cnt ));
	_instance_list.push_back(this);
}

SingletonBase::~SingletonBase()
{
	--singleton_cnt;
	PJ_LOG(3, (__FILE__,"Delete instance: %s (%d)",m_name ,singleton_cnt));
	if (!_instance_list.clear_list)
		_instance_list.remove(this);
}
