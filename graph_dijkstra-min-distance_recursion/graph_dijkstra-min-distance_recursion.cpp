#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

int N, begin_index, end_index, q;
int** roads;
void displayroads()
{
	cout << "Система дорог и перекрестков считанная:" << endl;
	char letter = '1';
	cout << "  ";
	for (int i = 0; i < N; i++) {
		cout << letter << " ";
		letter++;
	}
	cout << endl;
	letter = '1';
	for (int i = 0; i < N; i++) {
		cout << letter << " ";
		for (int j = 0; j < N; j++)
			cout << roads[i][j] << " ";
		letter++;
		cout << endl;
	}
	cout << endl << endl;
}

void readtext()
{

	string line, text;
	int nLines = 0; //номер строки 
	ifstream ifs("информация.txt");
	if (!ifs.is_open())
		cout << "Ошибка чтения файла" << endl;
	else
	{
		while (getline(ifs, line))
		{
			text += line + "\n";
			if (nLines == 0)
			{
				N = stoi(line); // берет первое значение из строки 
				roads = new int* [N];
				for (int i = 0; i < N; i++)
				{
					roads[i] = new int[N];
					for (int j = 0; j < N; j++)roads[i][j] = 0;
				}
			}
			nLines += 1;
		}
		cout << "Вывод информации на экран:" << endl << text << endl;
		ifs.close();
		ifstream ifs("информация.txt");
		int counter = 0;
		while (getline(ifs, line)) {
			if (counter > 0) {

				if (counter == nLines - 1) { //дошли до последней строки 
					begin_index = stoi(line) - 1;
					end_index = stoi(line.erase(0, 2)) - 1;
					q = stoi(line.erase(0, 2));
				}
				else {
					int i = stoi(line) - 1, j = stoi(line.erase(0, 2)) - 1;
					int distance = stoi(line.erase(0, 2));
					roads[i][j] = roads[j][i] = distance;
				}
			}counter++;
		}ifs.close();
	}
}

int* multXq()
{
	int* mas = new int[N];
	for (int i = 0; i < N; i++)
	{
		int counter = 0;
		for (int j = 0; j < N; j++)
		{
			if (roads[i][j] > 0)counter++;
		}
		mas[i] = counter * q; //по формуле
	}
	return mas;
}

void travel(int* cross) {
	int* d = new int[N]; // минимальное расстояние
	int* v = new int[N]; // посещенные вершины
	int temp, minindex, min;

	//Инициализация вершин и расстояний
	for (int i = 0; i < N; i++)
	{
		d[i] = 10000;
		v[i] = 1;
	}
	d[begin_index] = 0;
	// Шаг алгоритма
	do {
		minindex = 10000;
		min = 10000;
		for (int i = 0; i < N; i++)
		{ // Если вершину ещё не обошли и вес меньше min
			if ((v[i] == 1) && (d[i] < min))
			{ // Переприсваиваем значения
				min = d[i];
				minindex = i;
			}
		}
		// Добавляем найденный минимальный вес
		// к текущему весу вершины
		// и сравниваем с текущим минимальным весом вершины
		if (minindex != 10000)
		{
			for (int i = 0; i < N; i++)
			{
				if (roads[minindex][i] > 0)
				{
					temp = min + roads[minindex][i] + cross[i];
					if (temp < d[i])
					{
						d[i] = temp;
					}
				}
			}
			v[minindex] = 0;
		}

	} while (minindex < 10000);
	// Вывод кратчайших расстояний до вершин
	printf("\nКратчайшие расстояния до вершин: \n");
	for (int i = 0; i < N; i++)
	{
		if (d[i] != 10000)printf("%5d ", d[i]);
		else cout << " Не существует ";
	}
	if (d[end_index] == 10000)return;
	// Восстановление пути
	int* ver = new int[N];
	ver[0] = end_index + 1;
	int k = 1;
	int weight = d[end_index];

	while (end_index != begin_index)
	{
		for (int i = 0; i < N; i++)
			if (roads[i][end_index] != 0)
			{
				int temp = weight - roads[i][end_index] - cross[end_index];
				if (temp == d[i])
				{
					weight = temp;
					end_index = i;
					ver[k] = i + 1;
					k++;
				}
			}
	}
	// Вывод пути (начальная вершина оказалась в конце массива из k элементов)
	printf("\nВывод кратчайшего пути\n");
	for (int i = k - 1; i >= 0; i--)
		cout << ver[i] << " ";
}



int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	readtext();
	int* cross = multXq();
	displayroads();
	cout << "Время пересечения перекрестков:" << endl;
	for (int i = 0; i < N; i++)cout << cross[i] << " "; cout << endl;

	travel(cross);
	cout << endl << endl;
	for (int i = 0; i < N; i++)delete[] roads[i];
	delete[] roads;
	delete[] cross;
	system("pause");
	return 1;
}