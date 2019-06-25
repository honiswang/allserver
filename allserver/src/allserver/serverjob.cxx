#include "pjlib.h"
#include "sync.h"
#include "job.h"
#include "serverjob.h"
#include <iostream>
#include "common.h"
static pj_ioqueue_callback cb;
static void on_read_complete(pj_ioqueue_key_t *key, 
		pj_ioqueue_op_key_t *op_key, 
		pj_ssize_t bytes_read)
{
	pj_ssize_t length = 0;
	char *buf = NULL;
	pj_ioqueue_getbuf(op_key,&buf,&length);
	std::cout<<buf<<std::endl;
	pj_ioqueue_send(key,op_key,buf,&bytes_read,0);
//	pj_ioqueue_recv(key,op_key,buf,&length,PJ_IOQUEUE_ALWAYS_ASYNC);
	pj_ioqueue_unregister(key);
}

static void on_write_complete(pj_ioqueue_key_t *key, 
		pj_ioqueue_op_key_t *op_key, 
		pj_ssize_t bytes_sent)
{
	PJ_LOG(3,(__FILE__,"write one pgk"));
}

static void on_accept_complete(pj_ioqueue_key_t *key, 
		pj_ioqueue_op_key_t *op_key, 
		pj_sock_t sock, 
		pj_status_t status)
{
	PJ_LOG(3,(__FILE__,"one accept "));
}
static void on_connect_complete(pj_ioqueue_key_t *key, 
		pj_status_t status)
{

}
serverjob::serverjob():m_pool(NULL),m_ioqueue(NULL)
{
	SetName("serverjob");
	init();
}
void serverjob::init()
{
	m_pool = pj_pool_create(&pool::g_cp.factory, "pool1", // pool's name
			4000, // initial size
			4000, // increment size
			NULL);
	cb.on_read_complete = on_read_complete;
	cb.on_write_complete = on_write_complete;
	cb.on_accept_complete = on_accept_complete;
	cb.on_connect_complete=on_connect_complete;
	pj_ioqueue_create(m_pool,PJ_IOQUEUE_MAX_HANDLES, &m_ioqueue);

}
void serverjob::addsock(pj_sock_t sock)
{
	pj_ioqueue_key_t *key;
	pj_ioqueue_register_sock(m_pool, m_ioqueue,sock,this,&cb, &key);	
	pj_ioqueue_op_key_t *opkey =(pj_ioqueue_op_key_t *) pj_pool_zalloc(m_pool,sizeof(pj_ioqueue_op_key_t));	
	char * buf = (char *)pj_pool_zalloc(m_pool,4096);
	pj_ssize_t lenth = 4096;
	pj_ioqueue_recv(key,opkey,buf,&lenth,PJ_IOQUEUE_ALWAYS_ASYNC);
}
serverjob::~serverjob()
{
	pj_ioqueue_destroy(m_ioqueue);
}

void serverjob::Exec()
{
	pj_time_val timeout={1,0};
	int ret =pj_ioqueue_poll(m_ioqueue,&timeout);
	if(ret > 0)
	{
//		PJ_LOG(3,(__FILE__,"read num :%d", ret));
	}
}
