#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "carData.h"
#include "/usr/include/mysql/mysql.h"

//network define
#define PORT 4004
#define BUFFSIZE 5000

//DB define
#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "1252"
#define DB_NAME "s_DB"
#define CHOP(x) x[strlen(x) -1] = ' '


//DB var
MYSQL *connection = NULL, conn;

void *RepositDatafromCar(void *);
int insert_tCAR();
void MeasureDemand();

int main()
{
	//var threads
	pthread_t thid_recv;

	//DB connect
	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);

	//clntLen = sizeof(struct sockaddr_in);

	if (pthread_create(&thid_recv, NULL, RepositDatafromCar, NULL) < 0) {
		perror("thread create error\n");
		exit(1);
	}

	//measure demand
	{
		MYSQL_RES *sql_result;
		MYSQL_ROW sql_row;
		int query_stat;

		int number_cars = 0;
		int avg_speed;
		int demand;

		if (mysql_query(connection, "select CAR_ID from CAR") != 0)
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		sql_result = mysql_store_result(connection);

		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			number_cars++;
		}
		demand = number_cars + avg_speed;
		printf("demand: %d\n", demand);
	}
	while(1) {
		sleep(10);

	}

	return 0;
}

void *RepositDatafromCar(void *p) {
	//var network
	struct sockaddr_in servAddr, clntAddr;
	unsigned int clntLen;
	char buf[BUFFSIZE];
	int servSock;
	int clntSock;
	int end;

	//DB var
	carData *cData;
	int query_stat;
	char query[255];

	//create sock
	if ((servSock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		puts("socket() failed");

	//init servAddr & set servAddr
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(PORT);
	if (bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
		puts("bind failed");
	clntLen = sizeof(clntAddr);

	while (1) {

		if ((end = recvfrom(servSock, buf, BUFFSIZE - 1, 0, (struct sockaddr*)&clntAddr, &clntLen)) < 0)
			printf("recvfrom failed\n");
		buf[end] = '\0';
		cData = (carData *)buf;

		if (buf[0] != '\0') {
			//data from car -> DB
			sprintf(query, "INSERT INTO CAR VALUES (NULL, %d, %d, %d, %d, %d, %ld, %d, %d, '%c')",
				cData->id, cData->lane_num, cData->curX, cData->curY, cData->speed, cData->cur_time,
				cData->distance, cData->emergency, cData->direction);
			query_stat = mysql_query(connection, query);
			if (query_stat != 0)
			{
				fprintf(stderr, "Mysql query error : %s\n", mysql_error(&conn));
			}

			printf("id : %d\n", cData->id);
			printf("lane_num : %d\n", cData->lane_num);
			printf("curX : %d\n", cData->curX);
			printf("curY : %d\n", cData->curY);
			printf("speed : %d\n", cData->speed);
			printf("distance : %d\n", cData->distance);
			printf("emergency : %d\n", cData->emergency);
			printf("direction : %c\n", cData->direction);
			printf("cur_time: %ld\n", cData->cur_time);
			buf[0] = '\0';

		}
		usleep(10);
	}

	pthread_exit(NULL);
}

int insert_tCAR(carData cData) {

}