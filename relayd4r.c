#include "ruby.h"
#include <stdlib.h>  
#include <sys/types.h>
#include <arpa/inet.h>
#include <relay.h>
#include <stdio.h>



static VALUE
t_init (VALUE self)
{
	if (relayd_open() < 0) {
		rb_iv_set(self, "@error", Qtrue);
	}
	return self;
}

static VALUE
t_check_ip (VALUE self, VALUE ipaddress)
{
	return relayd_check_ip(STR2CSTR(ipaddress));
}

int queryip(const char *ip, time_t *timep)
{
	char timestamp[100];
	VALUE items = rb_hash_new();
	VALUE k[1],v[1];
	struct tm *mytm;
	int i;
	mytm=localtime(timep);
	strftime(timestamp,sizeof timestamp,"%s",mytm);

	k[0] = rb_str_new2("ipaddress");
	v[0] = rb_str_new2(ip);
	k[1] = rb_str_new2("timestamp");
	v[1] = rb_str_new2(timestamp);
	for (i=0;i<=1;i++){ 
		rb_hash_aset(items,k[i],v[i]);
	}
	rb_yield( items );
	return 0;
}

static VALUE
t_query_ip(VALUE self, VALUE ipaddress)
{
	relayd_query_ip(STR2CSTR(ipaddress),queryip);
	return Qnil;
}


static VALUE
t_check_user (VALUE self, VALUE username)
{
	return relayd_check_user(STR2CSTR(username));
}

static VALUE
t_ack (VALUE self, VALUE username, VALUE ipaddress)
{
	relayd_ack(STR2CSTR(username),STR2CSTR(ipaddress));
	return Qnil;
}

static VALUE
t_commit (VALUE self)
{
	relayd_commit();
	return Qnil;
}

static VALUE
t_close (VALUE self)
{
	relayd_close();
	return Qnil;
}

static VALUE t_error(VALUE self, VALUE anObject){
	return  rb_iv_get(self, "@error");
}

VALUE cRelayd;  

void
Init_relayd4r()
{
	cRelayd = rb_define_class("Relayd", rb_cHash);
	rb_define_method (cRelayd,"initialize", t_init, 0);
	rb_define_method (cRelayd,"check_ip", t_check_ip, 1);
	rb_define_method (cRelayd,"query_ip", t_query_ip, 1);
	rb_define_method (cRelayd,"check_user", t_check_user, 1);
	rb_define_method (cRelayd,"ack", t_ack, 2);
	rb_define_method (cRelayd,"commit", t_commit, 0);
	rb_define_method (cRelayd,"close", t_close, 0);
	rb_define_method (cRelayd,"error?", t_error, 0);
}
