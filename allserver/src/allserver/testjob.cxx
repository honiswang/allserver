#include "pjlib.h"
#include "sync.h"
#include "job.h"
#include "testjob.h"
#include "serverjob.h"
#include <iostream>
#include "common.h"
static pj_ioqueue_callback cb;
static void on_read_complete(pj_ioqueue_key_t *key, 
		pj_ioqueue_op_key_t *op_key, 
		pj_ssize_t bytes_read)
{
}

static void on_write_complete(pj_ioqueue_key_t *key, 
		pj_ioqueue_op_key_t *op_key, 
		pj_ssize_t bytes_sent)
{
	std::cout<<"write one pgk"<<std::endl;
}

static void on_accept_complete(pj_ioqueue_key_t *key, 
		pj_ioqueue_op_key_t *op_key, 
		pj_sock_t sock, 
		pj_status_t status)
{
	std::cout<<"one accept "<<std::endl;
}
static void on_connect_complete(pj_ioqueue_key_t *key, 
		pj_status_t status)
{

}
testjob::testjob()
{

}
testjob::testjob(int port):m_pool(NULL),m_ioqueue(NULL),m_port(port),m_key(NULL)
{
	SetName("testjob");
}
void testjob::init(int num)
{
	m_pool = pj_pool_create(&pool::g_cp.factory, "pool1", // pool's name
			4000, // initial size
			4000, // increment size
			NULL);
	cb.on_read_complete = on_read_complete;
	cb.on_write_complete = on_write_complete;
	cb.on_accept_complete = on_accept_complete;
	cb.on_connect_complete=on_connect_complete;
	int status  = pj_ioqueue_create(m_pool,PJ_IOQUEUE_MAX_HANDLES, &m_ioqueue);
	if(status == PJ_SUCCESS)
	{
		pj_sock_t sock;
		status = pj_sock_socket(pj_AF_INET(),pj_SOCK_STREAM(),pj_SOL_TCP(),&sock);
		if(status == PJ_SUCCESS)
		{
			pj_sockaddr addr;
			pj_str_t src;
			pj_sockaddr_parse(pj_AF_INET(),0,pj_strset2(&src,pj_print(m_pool,"0.0.0.0:%d",m_port)),&addr),
			std::cout<<"address>><<:"<<pj_sockaddr_print(&addr,(char *)pj_pool_zalloc(m_pool,100),100,1)<<std::endl;
			status = pj_sock_bind(sock,&addr,pj_sockaddr_get_len(&addr));
			status = pj_sock_listen(sock, 1024);
			pj_ioqueue_register_sock(m_pool, m_ioqueue,sock,this,&cb, &m_key);	
			
		}
	}
	for(int i=0;i<num;i++)
	{
		serverjob *sjob = new serverjob();
		m_jobs.push_back(sjob);
		sjob->Execute();
	}
}
testjob::~testjob()
{
	pj_ioqueue_unregister(m_key);
	pj_ioqueue_destroy(m_ioqueue);
}

void testjob::Exec()
{
	pj_time_val timeout={1,0};
	int ret =pj_ioqueue_poll(m_ioqueue,&timeout);
	if(ret > 0)
	{
		pj_sock_t nsock;
		pj_sockaddr remote;
		int status;
		int addlen = 0;
		pj_sockaddr_init(pj_AF_INET(),&remote,NULL,34);
		pj_ioqueue_op_key_t *opkey = (pj_ioqueue_op_key_t*)pj_pool_zalloc(m_pool,sizeof(pj_ioqueue_op_key_t));
		addlen = pj_sockaddr_get_len(&remote);
		status = pj_ioqueue_accept(m_key, opkey,&nsock,NULL,&remote, &addlen);
		if(status ==PJ_SUCCESS)
		{
			PJ_LOG(3,(__FILE__,pj_sockaddr_print(&remote,(char *)pj_pool_zalloc(m_pool,100),100,1)));
			serverjob *sjob = m_jobs.front();
			m_jobs.pop_front();
			sjob->addsock(nsock);
			m_jobs.push_back(sjob);
		}
	}
}
