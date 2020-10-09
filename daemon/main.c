#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <unistd.h>
#include <time.h>

#define HOSTADDRESS "172.17.0.3"
#define USERNAME "root"
#define USERPWD "root"


MYSQL *g_connect = NULL;
unsigned int g_num_fields = 0;	 

int connect_mysql_server(void){

	printf("MySQL client version: %s\n", mysql_get_client_info());


	g_connect = mysql_init(NULL);
	if(g_connect == NULL){
		printf("%s %d error:%s!\n", __FILE__, __LINE__, mysql_error(g_connect));
		return -1;
	}

	if (mysql_real_connect(g_connect, HOSTADDRESS, USERNAME, USERPWD, NULL, 0, NULL, 0) == NULL){
		mysql_close(g_connect);
		printf("%s %d error:%s!\n", __FILE__, __LINE__, mysql_error(g_connect));
		return -1;
	}  

	return 0;
}





int main(int argc, char **argv)
{
	MYSQL_ROW row;
	int i;
	MYSQL_RES *res;
	unsigned int num_fields;
	MYSQL_FIELD *field;
	unsigned long *lengths;
	int ret;
	//tag *tag_ = mxtag_new();
	//value_t num;
	long long seconds = 0;

	


	connect_mysql_server();
	#if 0
	if(mysql_query(con, "show databases;")){
		mysql_close(con);
		printf("%s %d error:%s!\n", __FILE__, __LINE__， mysql_error(con));

	}

	res = mysql_store_result(con);
	num_fields = mysql_num_fields(res);

	while (NULL != (field = mysql_fetch_field(res)))
	{
		printf("field name: %s\n", field->name);

	}

	while (NULL != (row = mysql_fetch_row(res)))
	{	
		lengths = mysql_fetch_lengths(res);
		for (i = 0; i < num_fields; i++)
		{
			printf("{%.*s} ", (int) lengths[i], row[i] ? row[i] : "NULL");
		}
		printf("\n");
	}
	#endif

	if(mysql_select_db(g_connect, "uc8220") != 0){
		mysql_close(g_connect);
		printf("%s %d error:%s!\n", __FILE__, __LINE__, mysql_error(g_connect));
		return -1;
	}

	while(1){

		if(mysql_query(g_connect, "select temperature from system;") != 0){
			mysql_close(g_connect);
			printf("%s %d error:%s!\n", __FILE__, __LINE__, mysql_error(g_connect));
			return -1;
		}

		if((res = mysql_store_result(g_connect)) == NULL){
			mysql_close(g_connect);
			printf("%s %d error:%s!\n", __FILE__, __LINE__, mysql_error(g_connect));
			return -1;			
		}

		num_fields = mysql_num_fields(res);
		printf("%s %d num_fields=%d\n", __FILE__, __LINE__, num_fields);


		#if 0
		while (NULL != (field = mysql_fetch_field(res)))
		{
			printf("field name: %s\n", field->name);

		}
		#endif

		if(NULL != (row = mysql_fetch_row(res)))
		{	
			#if 1
			while (NULL != (field = mysql_fetch_field(res)))
			{
				printf("%s ", field->name);

			}
			printf("\n");
			#endif

			lengths = mysql_fetch_lengths(res);
			for (i = 0; i < num_fields; i++)
			{
				printf("{%.*s}\t", (int) lengths[i], row[i] ? row[i] : "NULL");
			}
			printf("\n");

			#if 0
			i = 0;
			//temperature
			num.f = atoi(row[i++]);
			seconds = time(NULL);
			seconds *= 1000;
			//printf("seconds=%lld\n", seconds);
			mxtag_publish(tag_, "virtual_device_1", "temperature", &num, TAG_VALUE_TYPE_FLOAT, "c", seconds);

			//voltage
			num.f = atoi(row[i++]);
			seconds = time(NULL);
			seconds *= 1000;
			//printf("seconds=%lld\n", seconds);
			mxtag_publish(tag_, "virtual_device_1", "voltage", &num, TAG_VALUE_TYPE_FLOAT, "v", seconds);

			//current
			num.f = atoi(row[i++]);
			seconds = time(NULL);
			seconds *= 1000;
			//printf("seconds=%lld\n", seconds);
			mxtag_publish(tag_, "virtual_device_1", "current", &num, TAG_VALUE_TYPE_FLOAT, "A", seconds);	
			#endif
		}
		sleep(5);

	}

	//mxtag_delete(tag_);
	mysql_free_result(res);
	mysql_close(g_connect);    
	return 0;
}

