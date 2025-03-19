#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define  DB_HOST  "10.10.141.83"
#define  DB_USER  "server"
#define  DB_PASS  "123456"
#define  DB_NAME  "db_info_disadvantaged"

//make  -> gcc server.c -o server -I/usr/include/mysql -lmysqlclient

int checkid_db(char* id)
{
   MYSQL* conn;
   MYSQL_RES* res;
   MYSQL_ROW row;

   char query[256];
   int disadvantaged = 0;
   //connect DB
   conn = mysql_init(NULL); //conn init
   if (conn == NULL)
   {
      fprintf(stderr, "mysql_init() failed\n");
      return -1;
   }

   if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0) == NULL)
   {
      fprintf(stderr, "mysql_real_connect() failed : %s\n", mysql_error(conn));
      mysql_close(conn);
      return -1;
   }

   //select db
   sprintf(query, "select disadvantaged from tbl_disadvantaged where ID = '%s'", id);
   if (mysql_query(conn, query))
   {
      fprintf(stderr, "select query failed : %s\n", mysql_error(conn));
      mysql_close(conn);
      return -1;
   }

   //read db
   res = mysql_store_result(conn);
   if (res == NULL)
   {
      fprintf(stderr, "mysql_store_resule() failed\n");
      mysql_close(conn);
      return -1;
   }

   //charge money
   row = mysql_fetch_row(res);
   if (row)
   {
      disadvantaged = atoi(row[0]);
   }
   else
   {
      printf("'%s' is invalid id\n", id);
   }

   mysql_free_result(res);
   mysql_close(conn);
   return disadvantaged;
}

int getDatabaseInfo(char** id)
{
   int disadvantaged = 0;
   int i = 0, j = 0;
   int ret;
   char id_dst[20];
   while (id[i])
   {
      int k = 0;
      //넘어온 값 파싱 
      for (j = 0; id[i][j] != '\0'; j++)
      {
         printf("%c\n", id[i][j]);
         if (id[i][j] != ' ')
         {
            id_dst[k++] = id[i][j];
         }
      }
      id_dst[k] = '\0';
      //해당 아이디 값에 맞는 데이터 를 읽는다
      //데이터를 읽고, 교통약자이면 disadvantaged++;
      ret = checkid_db(id_dst);
      if (ret > 0) // disadvantaged == 1
      {
         disadvantaged++;
      }
      else if (ret < 0) // reading database failed
      {
         printf("reading database failed");
         return -1;
      }
      // disadvantage가 0일 때는 처리 할 것 없음
      i++;
   }
   return disadvantaged;
}

