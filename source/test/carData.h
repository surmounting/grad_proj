#ifndef __CARDATA_H
#define __CARDATA_H

#include <stdio.h>
#include <time.h>

#define NORMAL 0
#define RUSH 1
#define EMERGENCY 2

typedef struct gpsInfo {
	float latitude, longtitude;							// 위도 경도
}gpsInfo;

typedef struct carDATA {
	int id;
	int lane_num;
	int speed;
	int distance;
	int emergency;
	//03.03
	//++시스템 문서\방향 정의.png 참고하여 gps 기록들을 기반으로 값 설정
	gpsInfo gps;
	char direction[3];
	//++직전 사거리 통과 후 평균 속도
	//++차량이 필요한 신호: 앞뒤 차량간 통신으로 대기 신호정보를 교환, 

	time_t cur_time;
	time_t wait_time;
}carData;


#endif
