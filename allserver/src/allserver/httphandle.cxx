#include <pjlib.h>
#include <pjlib-util.h>
#include "httphandle.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>
static void scancallback(pj_scanner *scanner)
{
	PJ_LOG(3,(__FILE__, "scanner error:line %d pos:%s", scanner->line, scanner->curptr));
}
static void mprint(std::pair<std::string,std::string> item)
{
	PJ_LOG(3,(__FILE__, "http param is: %s  val:%s", item.first.c_str(), item.second.c_str()));
}
httphandle::httphandle():m_pool(NULL)
{
	m_mtoh["GET"] = &httphandle::getprocess;
	m_mtoh["POST"] = &httphandle::postprocess;
	m_mtoh["PUT"] = &httphandle::putprocess;
	m_mtoh["DELETE"] = &httphandle::deleteprocess;
	m_mtoh["PATCH"] = &httphandle::patchprocess;
}
httphandle::httphandle(pj_pool_t *pool):m_pool(pool)
{

}

httphandle::~httphandle()
{
	m_mtoh.clear();
}
bool httphandle::parse(char * buf)
{
	pj_scanner scanner;
	pj_scan_init(&scanner,buf, strlen(buf),0, scancallback);
	pj_str_t out;
	//while(!pj_scan_is_eof(&scanner))
	{
		pj_scan_get_until_ch(&scanner,' ',&out);
		m_head["methon"] =	std::string(out.ptr,out.slen);	
		pj_scan_skip_whitespace(&scanner);
		pj_scan_get_until_ch(&scanner,' ',&out);
		m_head["url"] =	std::string(out.ptr,out.slen);	
		pj_scan_skip_whitespace(&scanner);
		pj_scan_get_until_chr(&scanner,"\r\n",&out);
		m_head["version"] =	std::string(out.ptr,out.slen);	
		pj_scan_skip_line(&scanner);
		pj_str_t key,val;
		
		while(!pj_scan_is_eof(&scanner))
		{
			pj_scan_get_until_ch(&scanner,':',&key);
			pj_str_t kk = pj_str("\r\n");
			if(pj_stristr(&key,&kk) !=NULL)
			{
					PJ_LOG(3,(__FILE__,"eof key:%s size:%d", key.ptr,key.slen));
					break;
			}
			pj_scan_advance_n(&scanner,1,PJ_FALSE);
			pj_scan_get_until_chr(&scanner,"\r\n",&val);
			m_head[boost::to_lower_copy(std::string(key.ptr,key.slen))]=std::string(val.ptr,val.slen);	
			pj_scan_skip_line(&scanner);
		}
		if(m_head["methon"] !="GET")
		{
			pj_scan_init(&scanner,buf, strlen(buf),0, scancallback);
			pj_scan_get_until_chr_all(&scanner,"\r\n\r\n", &out);
			pj_scan_skip_line(&scanner);
			pj_scan_skip_line(&scanner);
			pj_scan_get_until_chr(&scanner,"\r\n", &out);
			m_head[m_head["methon"]] = std::string(out.ptr,out.slen);
		}
	}
	for_each(m_head.begin(),m_head.end(),*this);
	return true;
}
std::string httphandle::process()
{
	if(m_mtoh.find(m_head["methon"]) != m_mtoh.end())
	{
		return (this->*m_mtoh[m_head["methon"]])();
	}
	else
	{
		PJ_LOG(3,(__FILE__, "不支持的方法:%s",m_head["methon"].c_str()));
	}
	return m_response;
}
std::string httphandle::getprocess()
{
	PJ_LOG(3,(__FILE__, "this is getprocess !! param count :%d", m_param.size()));
	std::for_each(m_param.begin(),m_param.end(),mprint);
	return m_response;
}
std::string httphandle::postprocess()
{
	PJ_LOG(3,(__FILE__, "this is postprocess !! param count :%d", m_param.size()));
	std::for_each(m_param.begin(),m_param.end(),mprint);
	return m_response;
}
std::string httphandle::putprocess()
{
	PJ_LOG(3,(__FILE__, "this is putprocess !! param count :%d", m_param.size()));
	std::for_each(m_param.begin(),m_param.end(),mprint);
	return m_response;
}
std::string httphandle::deleteprocess()
{
	PJ_LOG(3,(__FILE__, "this is deleteprocess !! param count :%d", m_param.size()));
	std::for_each(m_param.begin(),m_param.end(),mprint);
	return m_response;
}
std::string httphandle::patchprocess()
{
	PJ_LOG(3,(__FILE__, "this is patchprocess !! param count :%d", m_param.size()));
	std::for_each(m_param.begin(),m_param.end(),mprint);
	return m_response;
}
void httphandle::operator() (std::pair<std::string,std::string> item)
{
	
	pj_scanner scanner;
	pj_scan_init(&scanner,(char *)item.second.c_str(),item.second.length(),PJ_SCAN_AUTOSKIP_WS,scancallback);
	pj_str_t out;
	out.slen = 0;
	if(item.first == "url")
	{
		pj_scan_get_until_ch(&scanner,'?',&out);
		pj_scan_advance_n(&scanner,1,PJ_FALSE);
	}
	while(!pj_scan_is_eof(&scanner))
	{
		pj_scan_get_until_ch(&scanner,'=',&out);
		if(!pj_scan_is_eof(&scanner))
		{
			pj_scan_advance_n(&scanner,1,PJ_FALSE);
			std::string key(out.ptr,out.slen);
			pj_str_t val;
			pj_scan_get_until_chr(&scanner,"&;",&val);
			pj_scan_advance_n(&scanner,1,PJ_FALSE);
			this->m_param[key] = std::string(val.ptr,val.slen);
		}
	}
	PJ_LOG(3,(__FILE__, "key :%s  val:%s", item.first.c_str(), item.second.c_str()));
}
