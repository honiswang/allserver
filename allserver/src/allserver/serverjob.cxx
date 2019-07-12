#include "pjlib.h"
#include "sync.h"
#include "job.h"
#include "serverjob.h"
#include <iostream>
#include "common.h"
#include "httphandle.h"
#include <algorithm>
static pj_ioqueue_callback cb;
static void on_read_complete(pj_ioqueue_key_t *key, 
		pj_ioqueue_op_key_t *op_key, 
		pj_ssize_t bytes_read)
{
	pj_ssize_t length = 0;
	char *buf = NULL;
	pj_ioqueue_getbuf(op_key,&buf,&length);
	httphandle handle;
	handle.parse(buf);
	handle.process();
	std::cout<<buf<<std::endl;
	const char *vb ="HTTP/1.1 200 OK\r\nBdpagetype: 3\r\n\
Bdqid: 0xdc20048c000964fc\r\n\
Cache-Control: private\r\n\
Connection: Keep-Alive\r\n\
Content-Encoding: gzip\r\n\
Content-Type: text/html;charset=utf-8\r\n\
Cxy_all: monline_4_dg+83061efbef51aaa15f043ff8b7c0d216\r\n\
Cxy_ex: 1561793101+2833663857+1de5d69d5b65902fb415e2b86b2f3710\r\n\
Date: Sat, 29 Jun 2019 07:25:01 GMT\r\n\
Is_status: 0\r\n\
Server: BWS/1.1\r\n\
Set-Cookie: delPer=0; path=/; domain=.baidu.com\r\n\
Set-Cookie: BD_CK_SAM=1;path=/\r\n\
Set-Cookie: PSINO=6; domain=.baidu.com; path=/\r\n\
Set-Cookie: BDSVRTM=189; path=/\r\n\
Set-Cookie: H_PS_PSSID=1448_21098_29135_29238_28519_29099_28833_29220_29072_22159; path=/; domain=.baidu.com\r\n\
Strict-Transport-Security: max-age=172800\r\n\
Vary: Accept-Encoding\r\n\
X-Ua-Compatible: IE=Edge,chrome=1\r\n\
Transfer-Encoding: chunked\r\n\
\r\n\
testtttttttttt";
	memset(buf,0, bytes_read);
	strncpy(buf, vb, strlen(vb));
	bytes_read = strlen(buf);
	pj_ioqueue_send(key,op_key,buf,&bytes_read,0);
	serverjob *job = (serverjob *)pj_ioqueue_get_user_data(key);
	job->addclose(key);
	
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
static void closekey(pj_ioqueue_key_t *key)
{
	pj_ioqueue_unregister(key);
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
	std::for_each(m_closekeys.begin(),m_closekeys.end(),closekey);
	m_closekeys.clear();
}
void serverjob::addclose(pj_ioqueue_key_t *key)
{
	m_closekeys.push_back(key);
}
