// bank.cpp

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "xxx.h"
const int MIN_PER_HR = 60;

bool newcustomer(double x);

int main()
{
	using std::cin;
	using std::cout;
	using std::endl;
	using std::ios_base;
	std::srand(std::time(0));

	cout << "사례 연구 : 히서 은행의 ATM\n";
	

	cout << "시뮬레이션 시간 수를 입력하십시오 : ";
	int hours;
	cin >> hours;
	double perhour = 60;
	
	Item temp;
	double min_per_cust;
	long turnaways;
	long customers;
	long served;
	long sum_line;
	int wait_time;
	long line_wait;

	do
	{
		Queue line(hours * perhour / 2);

		perhour--;
		min_per_cust = MIN_PER_HR / perhour;

		turnaways = 0;
		customers = 0;
		served = 0;
		sum_line = 0;
		wait_time = 0;
		line_wait = 0;
		
		for (int cycle = 0; cycle < perhour; cycle++)
		{
			if (newcustomer(min_per_cust))
			{

				if (line.isfull())
					turnaways++;
				else
				{
					customers++;
					temp.set(cycle);
					line.enqueue(temp);
				}
			}
			if (wait_time <= 0 && !line.isempty())
			{
				line.dequeue(temp);
				wait_time = temp.ptime();
				line_wait += cycle - temp.when();
				served++;
			}
			if (wait_time > 0)
				wait_time--;
			sum_line += line.queuecount();
		}
		cout << "평균 대기 시간 : " << double(line_wait) / (served) << endl;
	} while (double(line_wait) / (served) > 1.0);
	
	

	if (customers > 0)
	{
		cout << " 큐에 줄을 선 고객 수 : " << customers << endl;
		cout << "거래를 처리한 고객 수 : " << served << endl;
		cout << " 발길을 돌린 고객 수 : " << turnaways << endl;
		cout << "	평균 큐의 길이 : ";
		cout.precision(2);
		cout.setf(ios_base::fixed, ios_base::floatfield);
		cout.setf(ios_base::showpoint);
		cout << (double) (sum_line) / perhour << endl;
		cout << "시간당 평균 고객수가 "<< perhour << "명 일때의 평균 대기 시간 : "
			 << (double) (line_wait) / (served) << "분\n";
	}
	else
		cout << "고객이 한 명도 없습니다!\n";
	
	
	cout << "완료!\n";
	return 0;
}

bool newcustomer(double x)
{
	return (std::rand() * x / RAND_MAX < 1);
}