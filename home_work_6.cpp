#include <iostream>
#include <windows.h>
#include <map>
#include <string>

using namespace std;

DWORD WINAPI ThreadProduce(LPVOID param);
DWORD WINAPI ThreadSellCar(LPVOID param);
DWORD WINAPI ThreadShowAllCars(LPVOID param);

struct CarInfo {
	CarInfo(string number) {
		this->number = number;
	}
	string getNumber() {
		return number;
	}
	string number;
};

class CarFactory {
public:
	CarFactory() {
		InitializeCriticalSection(&m_criticalSection);
	}

	friend DWORD WINAPI ThreadProduce(LPVOID param);
	friend DWORD WINAPI ThreadSellCar(LPVOID param);
	friend DWORD WINAPI ThreadShowAllCars(LPVOID param);

	void Produce() {

		HANDLE thread1 = CreateThread(0, 0, ThreadProduce, this, 0, 0);
	}

	void SellCar(char* eraseCars) {
		m_eraseCars = eraseCars;
		HANDLE thread2 = CreateThread(0, 0, ThreadSellCar, this, 0, 0);
	}

	void ShowAllCars() {

		HANDLE thread3 = CreateThread(0, 0, ThreadShowAllCars, this, 0, 0);
	}

	~CarFactory() {
		DeleteCriticalSection(&m_criticalSection);
	}

	string m_eraseCars;
	map<string, CarInfo> m_cars;
	CRITICAL_SECTION m_criticalSection;
	
};



int main() {

	CarFactory carFactory;

	carFactory.Produce();
	Sleep(2000);
	carFactory.ShowAllCars();
	Sleep(2000);
	carFactory.SellCar("x4");
	carFactory.SellCar("x5");
	carFactory.SellCar("x6");
	Sleep(2000);

	return 0;
}

DWORD WINAPI ThreadShowAllCars(LPVOID param) {

	CarFactory* carFactory = static_cast<CarFactory*>(param);

	EnterCriticalSection(&(carFactory->m_criticalSection));
	for (map<string, CarInfo>::iterator it = carFactory->m_cars.begin(); it != carFactory->m_cars.end(); ++it) {
		cout << it->second.getNumber() << endl;
	}

	LeaveCriticalSection(&(carFactory->m_criticalSection));

	return 0;
}

DWORD WINAPI ThreadSellCar(LPVOID param) {

	CarFactory* carFactory = static_cast<CarFactory*>(param);

	EnterCriticalSection(&(carFactory->m_criticalSection));
	
	carFactory->m_cars.erase(carFactory->m_eraseCars);
	
	LeaveCriticalSection(&(carFactory->m_criticalSection));

	return 0;
}

DWORD WINAPI ThreadProduce(LPVOID param) {

	CarFactory* carFactory = static_cast<CarFactory*>(param);

	EnterCriticalSection(&(carFactory->m_criticalSection));
	carFactory->m_cars.insert(std::make_pair("x5", CarInfo("5555")));
	carFactory->m_cars.insert(std::make_pair("x6", CarInfo("6666")));
	carFactory->m_cars.insert(std::make_pair("x4", CarInfo("7777")));
	LeaveCriticalSection(&(carFactory->m_criticalSection));

	return 0;
}
