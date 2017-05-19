#include <stdio.h>
#include <termio.h>
#include <time.h>
#include <windows.h>
#include <pthread.h>
#include <string.h>



struct alinfo{
	char Alnf[10];
	char amorpm[10];
	int whour;
	int wmin;
};

struct alinfo alin = { "OFF", "AM", 0, 0 };
void *cal_clock(void *arg);
void *print_clock(void *arg);
void *set_time(void *arg);
void *set_alarm(void *arg);
void *alarm(void *arg);
void clean_up(void *arg);




void nullfun(void);
void makingline(void);
void endline(void);

void gotoxy(int x, int y);
int getch(void);


static const char wday_name[][3] = { "SU", "MO", "TU", "WE", "TH", "FR", "SA" };

pthread_t thread0;
pthread_t thread1;
pthread_t thread2;
pthread_t thread3;





time_t alarmtime = 0;
int printhour;
char ampm[10] = "AM";
struct tm tm;

long diff = 0;
int b = 0; //b버튼 클릭
int c1 = 1; //c버튼 클릭
int c2 = 1;
int d = 0;
int state = 0;
int bbbbb = 0; // 알람시간임을 알림
int alarmon = 0;
int plzstop = 0;




int main(void) {

	int command = 0;
	pthread_create(&thread0, NULL, &cal_clock, NULL);
	pthread_create(&thread1, NULL, &print_clock, NULL);  // 시작화면


	while (command != 101)
	{
		command = getch();

		if (bbbbb == 1)
		{
			while (!(command >= 97 && command <= 100))
			{
				command = getch();
			}
			pthread_cancel(thread3);
			pthread_join(thread3, NULL);
			pthread_create(&thread1, NULL, &print_clock, NULL);
			command = 0;
			bbbbb = 0;
		}
		switch (state) {
		case 0:
			if (command == 97) {
				pthread_cancel(thread1);
				pthread_join(thread1, NULL);
				state = 1;
				command = 0;
				pthread_create(&thread2, NULL, &set_time, NULL);
			}
			if (command == 99) {
				pthread_cancel(thread1);
				pthread_join(thread1, NULL);
				state = 2;
				command = 0;
				pthread_create(&thread2, NULL, &set_alarm, NULL);
			}
		case 1:
			if (command == 97) {
				pthread_cancel(thread2);
				pthread_join(thread2, NULL);
				state = 0;
				command = 0;
				pthread_create(&thread1, NULL, &print_clock, NULL);

			}
			if (command == 98) {
				command = 0;
				b++;
			}
			if (command == 99) {
				command = 0;
				c1++;
				c1 = (c1 - 1) % 7 + 1;
			}
		case 2:
			if (command == 97) {
				pthread_cancel(thread2);
				pthread_join(thread2, NULL);
				command = 0;
				state = 0;
				pthread_create(&thread1, NULL, &print_clock, NULL);
			}
			if (command == 98) {
				command = 0;
				b++;
			}
			if (command == 99) {
				command = 0;
				c2++;
				c2 = (c2 - 1) % 2 + 1;
			}
			if (command == 100) {
				command = 0;
				d++;
			}
		default:
			break;
		}
	} // while문 끝
	if (alarmon == 1)
	{
		plzstop = 1;
		pthread_cancel(thread3);
		pthread_join(thread3, NULL);

	}

	if (state == 0) {
		pthread_cancel(thread1);
		pthread_cancel(thread0);
		int a = pthread_join(thread1, NULL);
		int b = pthread_join(thread0, NULL);
		if (!a && !b)
			printf("\n정상적으로 종료되었습니다");
	}
	else {
		pthread_cancel(thread2);
		pthread_cancel(thread1);
		pthread_cancel(thread0);
		int a = pthread_join(thread2, NULL);
		int b = pthread_join(thread1, NULL);
		int c = pthread_join(thread0, NULL);
		if (!a && !b && !c)
			printf("\n정상적으로 종료되었습니다");
	}
	return 0;
}
void* cal_clock(void *arg) {
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	time_t print_t;


	// 실제 시간과 내가 정한 시간사이의 차이를 구하고

	while (1)
	{
		print_t = time(NULL) - diff; // 실제 시간과 그 차이를 유지하는 시계를 만듬
		tm = *localtime(&print_t);

		printhour = tm.tm_hour;

		if (tm.tm_hour >= 12)
			ampm[0] = 'P';
		else
			ampm[0] = 'A';

		if (tm.tm_hour == 0)
			printhour = 12;
		if (tm.tm_hour > 12)
			printhour = tm.tm_hour - 12;
	}
}
void* print_clock(void *arg) {
	while (1)
	{
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

		system("Clear");

		gotoxy(10, 10);
		printf("%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
		gotoxy(10, 11);

		printf("%s %02d:%02d:%02d %s", ampm, printhour, tm.tm_min, tm.tm_sec, wday_name[tm.tm_wday]);
		fflush(stdout);
		Sleep(100);
	}
}

void* set_time(void *arg) {
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	
	time_t settime = time(NULL) - diff;
	struct tm settm = *localtime(&settime);


	int myhour = 0;
	char ap[10] = "AM";
	void(*hi[14])(void);

	while (1) {

		if (b != 0) {
			switch (c1)
			{
			case 1:
				settm.tm_year = (settm.tm_year-100 + b) % 101 + 100;
				break;
			case 2:
				settm.tm_mon = (settm.tm_mon + b) % 12;
				break;
			case 3:
				settm.tm_mday = (settm.tm_mday - 1 + b) % 31 + 1;
				break;
			case 4:
				if (b % 2 != 0) {
					settm.tm_hour = (settm.tm_hour + 12) % 24;
				}
				break;
			case 5:
				settm.tm_hour = (settm.tm_hour + b) % 24;
				break;
			case 6:
				settm.tm_min = (settm.tm_min + b) % 60;
				break;
			case 7:
				settm.tm_sec = (settm.tm_sec + b) % 60;
				break;
			default:
				break;
			}
			b = 0;
		}




		settime = mktime(&settm);
		diff = difftime(time(NULL), settime);

		

		myhour = settm.tm_hour;
		
		if (settm.tm_hour >= 12)
			ap[0] = 'P';
		else
			ap[0] = 'A';

		if (settm.tm_hour == 0)
			myhour = 12;

		if (settm.tm_hour > 12)
			myhour = settm.tm_hour - 12;

		for (int i = 0; i < 14; i++)
			hi[i] = nullfun;

		hi[2 * c1 - 2] = makingline;
		hi[2 * c1 - 1] = endline;




		//출력부분----------------------------------------------
		system("Clear");
		gotoxy(10, 10);


		hi[0]();
		printf("%02d", settm.tm_year + 1900);
		hi[1]();

		printf("-");

		hi[2]();
		printf("%02d", settm.tm_mon + 1);
		hi[3]();

		printf("-");

		hi[4]();
		printf("%02d", settm.tm_mday);
		hi[5]();

		gotoxy(10, 11);

		hi[6]();
		printf("%s", ap);
		hi[7]();

		printf(" ");

		hi[8]();
		printf("%02d", myhour);
		hi[9]();

		printf(":");

		hi[10]();
		printf("%02d", settm.tm_min);
		hi[11]();

		printf(":");

		hi[12]();
		printf("%02d", settm.tm_sec);
		hi[13]();

		printf(" %s", wday_name[settm.tm_wday]);
		fflush(stdout);

		//sleep전엔 fflush(stdout)
		Sleep(100);
	}
}

void *set_alarm(void *arg) {
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL); // 취소지점 존재

	struct tm alarmtm = *localtime(&alarmtime);
	int printhour2;
	char ampm2[10] = "AM";
	void(*hi[4])(void);
	int inputnf = 0;



	while (1) {

		if (d != 0) {
			if (!strcmp(alin.Alnf, "OFF")) {
				strcpy(alin.Alnf, "ON");
				pthread_create(&thread3, NULL, &alarm, (void *)&alin);
				alarmon = 1;
			}
			else {
				strcpy(alin.Alnf, "OFF");
				plzstop = 1;
				pthread_cancel(thread3);
				pthread_join(thread3, NULL);
				alarmon = 0;
			}
			inputnf = 1;
			d = 0;
		}


		if (b != 0) {
			switch (c2)
			{
			case 1:
				alarmtm.tm_hour = (alarmtm.tm_hour + b) % 24;
				break;
			case 2:
				alarmtm.tm_min = (alarmtm.tm_min + b) % 60;
				break;
			default:
				break;
			}
			b = 0;
		}

		alarmtm.tm_sec = 0; //알람 울리는 시각을 0초로 설정

		alarmtime = mktime(&alarmtm); // 변경된alarmtm 의 시간을 time_t형인 alarmtime으로

		printhour2 = alarmtm.tm_hour;



		if (alarmtm.tm_hour >= 12)
			ampm2[0] = 'P';
		else
			ampm2[0] = 'A';

		if (alarmtm.tm_hour == 0)
			printhour2 = 12;

		if (alarmtm.tm_hour > 12)
			printhour2 = alarmtm.tm_hour - 12;

		for (int i = 0; i < 4; i++)
			hi[i] = nullfun;

		strcpy(alin.amorpm, ampm2);
		alin.whour = printhour2;
		alin.wmin = alarmtm.tm_min;
		pthread_cleanup_push(clean_up, (void *)&alin); //취소지점






		hi[2 * c2 - 2] = makingline;
		hi[2 * c2 - 1] = endline;



		//출력부분----------------------------------------------
		system("Clear");


		if (inputnf == 1) {
			if (alarmon == 1 ) {
				printf("\033[s");
				gotoxy(10, 9);
				printf("알람이 설정되었습니다.");
				printf("\033[u");

			}
			else {
				printf("\033[s");
				gotoxy(10, 9);
				printf("알람이 해제되었습니다.");
				printf("\033[u");

			}

		}


		gotoxy(10, 11);

		printf("%s ", ampm2);

		hi[0]();
		printf("%02d", printhour2);
		hi[1]();

		printf(":");

		hi[2]();
		printf("%02d", alarmtm.tm_min);
		hi[3]();


		fflush(stdout);


		//sleep전엔 fflush(stdout)
		Sleep(100);
		pthread_cleanup_pop(0);
	}
}

void clean_up(void *arg)
{
	system("Clear");
	gotoxy(10, 11);

	printf("AL %s %s %02d:%02d", ((struct alinfo*)arg)->Alnf, ((struct alinfo*)arg)->amorpm, ((struct alinfo*)arg)->whour, ((struct alinfo*)arg)->wmin);
	fflush(stdout);
	Sleep(2000); // 2초
}

void *alarm(void *arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	while (alarmtime - (time(NULL) - diff) % 86400 != 0 && plzstop == 0)
	{
		Sleep(500);
	}
	if (state == 0)
	{
		pthread_cancel(thread1);
		pthread_join(thread1, NULL);
	}
	else {
		pthread_cancel(thread2);
		pthread_join(thread2, NULL);
	}
	bbbbb = 1;

	while (bbbbb) {
		system("Clear");
		gotoxy(10, 10);
		printf("AL %s %s %02d:%02d", ((struct alinfo*)arg)->Alnf, ((struct alinfo*)arg)->amorpm, ((struct alinfo*)arg)->whour, ((struct alinfo*)arg)->wmin);
		printf("\a");
		fflush(stdout);
		Sleep(500);
	}
}

void nullfun(void) {
	;
}
void makingline(void)
{
	printf("\033[4m");

}
void endline(void)
{
	printf("\033[0m");

}
void gotoxy(int x, int y)
{
	printf("\033[%d;%df", y, x);
	fflush(stdout);
}
int getch(void) {
	int ch;
	struct termios buf, save;
	tcgetattr(0, &save);
	buf = save;
	buf.c_lflag &= ~(ICANON | ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;
	tcsetattr(0, TCSAFLUSH, &buf);
	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &save);
	return ch;
}



