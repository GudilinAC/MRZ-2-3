#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

/*
-1 -1 -1  1
-1 -1 -1 -1
-1  1  1  1
-1 -1  1  1
-1  1  1 -1
-1 -1  1 -1
-1 -1 -1  1
*/

unsigned N;
unsigned xX, yX;
unsigned xY, yY;
unsigned It;

int*** X;
int*** Y;
int**** W;

int** tempX;
int** tempY;

inline int f(int S)
{
	return S ? (S > 0 ? 1 : -1) : 0;
}

inline char graph(int f)
{
	switch (f)
	{
	case 1:
		return (char)178;
	case -1:
		return (char)176;
	default:
		return '?';
	}
}

void printX(int **X)
{
	cout << "X:" << endl;
	for (unsigned yx = 0; yx < yX; yx++)
	{
		for (unsigned xx = 0; xx < xX; xx++)
			cout << graph(X[yx][xx]) << " ";
		cout << endl;
	}
	cout << endl;
}

void printY(int **Y)
{
	cout << "Y:" << endl;
	for (unsigned yy = 0; yy < yY; yy++)
	{
		for (unsigned xy = 0; xy < xY; xy++)
			cout << graph(Y[yy][xy]) << " ";
		cout << endl;
	}
	cout << endl;
}

void printPair(int **X, int** Y)
{
	printX(X);
	printY(Y);
}

void printWeights()
{
	for (unsigned yx = 0; yx < yX; yx++)
		for (unsigned xx = 0; xx < xX; xx++)
		{
			for (unsigned yy = 0; yy < yY; yy++)
				for (unsigned xy = 0; xy < xY; xy++)
					cout << setw(2) << W[yx][xx][yy][xy] << " ";
			cout << endl;
		}
	cout << endl;
}

bool checkTrained()
{
	bool test = true;
	for (unsigned n = 0; n < N; n++)
	{
		{
			for (unsigned yx = 0; yx < yX; yx++)
				for (unsigned xx = 0; xx < xX; xx++)
					tempX[yx][xx] = X[n][yx][xx];

			for (unsigned yy = 0; yy < yY; yy++)
				for (unsigned xy = 0; xy < xY; xy++)
					tempY[yy][xy] = 0;

			for (unsigned i = 0; i < It; i++)
			{
				test = true;
				for (unsigned yy = 0; yy < yY; yy++)
					for (unsigned xy = 0; xy < xY; xy++)
					{
						int ytmp = 0;
						for (unsigned yx = 0; yx < yX; yx++)
							for (unsigned xx = 0; xx < xX; xx++)
								ytmp += tempX[yx][xx] * W[yx][xx][yy][xy];
						if (ytmp) tempY[yy][xy] = f(ytmp);
					}

				for (unsigned yy = 0; yy < yY && test; yy++)
					for (unsigned xy = 0; xy < xY; xy++)
						if (tempY[yy][xy] != Y[n][yy][xy])
						{
							test = false;
							break;
						}

				if (test) break;

				for (unsigned yx = 0; yx < yX; yx++)
					for (unsigned xx = 0; xx < xX; xx++)
					{
						int xtmp = 0;
						for (unsigned yy = 0; yy < yY; yy++)
							for (unsigned xy = 0; xy < xY; xy++)
								xtmp += tempY[yy][xy] * W[yx][xx][yy][xy];
						if (xtmp) tempX[yx][xx] = f(xtmp);
					}
			}

			if (!test)
			{
				cout << "Network not trained X" << n << endl;
				break;
			}
		}
		{
			for (unsigned yy = 0; yy < yY; yy++)
				for (unsigned xy = 0; xy < xY; xy++)
					tempY[yy][xy] = Y[n][yy][xy];

			for (unsigned yx = 0; yx < yX; yx++)
				for (unsigned xx = 0; xx < xX; xx++)
					tempX[yx][xx] = 0;

			for (unsigned i = 0; i < It; i++)
			{
				test = true;
				for (unsigned yx = 0; yx < yX; yx++)
					for (unsigned xx = 0; xx < xX; xx++)
					{
						int xtmp = 0;
						for (unsigned yy = 0; yy < yY; yy++)
							for (unsigned xy = 0; xy < xY; xy++)
								xtmp += tempY[yy][xy] * W[yx][xx][yy][xy];
						if (xtmp) tempX[yx][xx] = f(xtmp);
					}

				for (unsigned yx = 0; yx < yX && test; yx++)
					for (unsigned xx = 0; xx < xX; xx++)
						if (tempX[yx][xx] != X[n][yx][xx])
						{
							test = false;
							break;
						}

				if (test) break;

				for (unsigned yy = 0; yy < yY; yy++)
					for (unsigned xy = 0; xy < xY; xy++)
					{
						int ytmp = 0;
						for (unsigned yx = 0; yx < yX; yx++)
							for (unsigned xx = 0; xx < xX; xx++)
								ytmp += tempX[yx][xx] * W[yx][xx][yy][xy];
						if (ytmp) tempY[yy][xy] = f(ytmp);
					}
			}
			if (!test)
			{
				cout << "Network not trained Y" << n << endl;
				break;
			}
		}
	}
	return test;
}

int findX(int** Xt)
{
	int negative;
	for (unsigned n = 0; n < N; n++)
	{
		bool test = true;
		for (unsigned yx = 0; yx < yX && test; yx++)
			for (unsigned xx = 0; xx < xX; xx++)
			{
				if (Xt[yx][xx] == 0)
				{
					test = false;
					break;
				}
				if (yx == 0 && xx == 0)
					if (Xt[yx][xx] == X[n][yx][xx]) negative = 1;
					else negative = -1;
				else if (negative * Xt[yx][xx] != X[n][yx][xx])
				{
					test = false;
					break;
				}
			}
		if (test) return n;
	}
	return -1;
}

int findY(int** Yt)
{
	int negative;
	for (unsigned n = 0; n < N; n++)
	{
		bool test = true;
		for (unsigned yy = 0; yy < yY && test; yy++)
			for (unsigned xy = 0; xy < xY; xy++)
			{
				if (Yt[yy][xy] == 0)
				{
					test = false;
					break;
				}
				if (yy == 0 && xy == 0)
					if (Yt[yy][xy] == Y[n][yy][xy]) negative = 1;
					else negative = -1;
				else if (negative * Yt[yy][xy] != Y[n][yy][xy])
				{
					test = false;
					break;
				}
			}
		if (test) return n;
	}
	return -1;
}

int energy(int **X, int** Y)
{
	int E = 0;
	for (unsigned yy = 0; yy < yY; yy++)
		for (unsigned xy = 0; xy < xY; xy++)
		{
			int xw = 0;
			for (unsigned yx = 0; yx < yX; yx++)
				for (unsigned xx = 0; xx < xX; xx++)
					xw += X[yx][xx] * W[yx][xx][yy][xy];
			E -= xw * Y[yy][xy];
		}
	return E;
}

bool stepToX()
{
	bool test = true;
	for (unsigned yx = 0; yx < yX; yx++)
		for (unsigned xx = 0; xx < xX; xx++)
		{
			int xtmp = 0;
			for (unsigned yy = 0; yy < yY; yy++)
				for (unsigned xy = 0; xy < xY; xy++)
					xtmp += tempY[yy][xy] * W[yx][xx][yy][xy];
			if (xtmp)
			{
				xtmp = f(xtmp);
				if (xtmp != tempX[yx][xx]) test = false;
				tempX[yx][xx] = xtmp;
			}
		}
	return test;
}

bool stepToY()
{
	bool test = true;
	for (unsigned yy = 0; yy < yY; yy++)
		for (unsigned xy = 0; xy < xY; xy++)
		{
			int ytmp = 0;
			for (unsigned yx = 0; yx < yX; yx++)
				for (unsigned xx = 0; xx < xX; xx++)
					ytmp += tempX[yx][xx] * W[yx][xx][yy][xy];

			if (ytmp) {
				ytmp = f(ytmp);
				if (ytmp != tempY[yy][xy]) test = false;
				tempY[yy][xy] = ytmp;
			}
		}
	return test;
}

int main()
{
	{
		ifstream input("input.txt");
		input >> N >> yX >> xX >> yY >> xY >> It;

		X = new int**[N];
		Y = new int**[N];
		for (unsigned n = 0; n < N; n++)
		{
			X[n] = new int*[yX];
			for (unsigned yx = 0; yx < yX; yx++)
			{
				X[n][yx] = new int[xX];
				for (unsigned xx = 0; xx < xX; xx++)
					input >> X[n][yx][xx];
			}

			Y[n] = new int*[yY];
			for (unsigned yy = 0; yy < yY; yy++)
			{
				Y[n][yy] = new int[xY];
				for (unsigned xy = 0; xy < xY; xy++)
					input >> Y[n][yy][xy];
			}
		}

		input.close();

		W = new int***[yX];
		for (unsigned yx = 0; yx < yX; yx++)
		{
			W[yx] = new int**[xX];
			for (unsigned xx = 0; xx < xX; xx++)
			{
				W[yx][xx] = new int*[yY];
				for (unsigned yy = 0; yy < yY; yy++)
				{
					W[yx][xx][yy] = new int[xY];
					for (unsigned xy = 0; xy < xY; xy++)
					{
						W[yx][xx][yy][xy] = 0;
						for (unsigned n = 0; n < N; n++)
							W[yx][xx][yy][xy] += X[n][yx][xx] * Y[n][yy][xy];
					}
				}
			}
		}


		bool test = true;

		tempX = new int*[yX];
		for (unsigned yx = 0; yx < yX; yx++)
			tempX[yx] = new int[xX];

		tempY = new int*[yY];
		for (unsigned yy = 0; yy < yY; yy++)
			tempY[yy] = new int[xY];


		if (!checkTrained())
		{
			cout << "Network additional training" << endl;

			unsigned* Q = new unsigned[N];
			for (unsigned n = 0; n < N; n++)
			{
				int En = energy(X[n], Y[n]);
				int Ex;
				int Ey;

				int Et;

				int E = -32767;
				for (unsigned yx = 0; yx < yX; yx++)
					for (unsigned xx = 0; xx < xX; xx++)
					{
						for (unsigned yx = 0; yx < yX; yx++)
							for (unsigned xx = 0; xx < xX; xx++)
								tempX[yx][xx] = X[n][yx][xx];
						tempX[yx][xx] = -tempX[yx][xx];
						Et = En - energy(tempX, Y[n]);
						if (Et > E) E = Et;
					}
				Ex = E;

				E = -32767;
				for (unsigned yy = 0; yy < yY; yy++)
					for (unsigned xy = 0; xy < xY; xy++)
					{
						for (unsigned yy = 0; yy < yY; yy++)
							for (unsigned xy = 0; xy < xY; xy++)
								tempY[yy][xy] = Y[n][yy][xy];
						tempY[yy][xy] = -tempY[yy][xy];
						Et = En - energy(X[n], tempY);
						if (Et > E) E = Et;
					}
				Ey = E;

				int H1 = round((Ex / (yX*xX*2.)) + 1.);
				int H2 = round((Ey / (yY*xY*2.)) + 1.);

				Q[n] = (H1 > H2) ? H1 : H2;
			}

			for (unsigned n = 0; n < N; n++)
				if (Q[n] > 1)
					for (unsigned yx = 0; yx < yX; yx++)
						for (unsigned xx = 0; xx < xX; xx++)
							for (unsigned yy = 0; yy < yY; yy++)
								for (unsigned xy = 0; xy < xY; xy++)
									W[yx][xx][yy][xy] += (Q[n] - 1) * X[n][yx][xx] * Y[n][yy][xy];

			delete[]Q;

			if (checkTrained()) cout << "Network trained" << endl;
			else { cout << "Network not trained" << endl; return -1; }
		}
	}

	char key;
	do
	{
		cout << "1) Input X\n2) Input Y\n3) Show all\nYour choise: ";
		cin >> key;
		system("cls");
		switch (key)
		{
		case '1':
		{
			cout << "Input X (" << yX << "x" << xX << "):" << endl;
			for (unsigned yx = 0; yx < yX; yx++)
				for (unsigned xx = 0; xx < xX; xx++)
					cin >> tempX[yx][xx];

			for (unsigned yy = 0; yy < yY; yy++)
				for (unsigned xy = 0; xy < xY; xy++)
					tempY[yy][xy] = 0;

			for (unsigned i = 0; i < It; i++)
			{
				if (stepToY())
				{
					printPair(tempX, tempY);
					int n = findX(tempX);
					if (n < 0 || findY(tempY) != n) cout << "Not recognized" << endl;
					else cout << "Its pair " << n << endl;;
					break;
				}

				if (stepToX())
				{
					printPair(tempX, tempY);
					int n = findX(tempX);
					if (n < 0 || findY(tempY) != n) cout << "Not recognized" << endl;
					else cout << "Its pair " << n << endl;
					break;
				}
			}
		}
		break;
		case '2':
		{
			cout << "Input Y (" << yY << "x" << xY << "):" << endl;
			for (unsigned yy = 0; yy < yY; yy++)
				for (unsigned xy = 0; xy < xY; xy++)
					cin >> tempY[yy][xy];

			for (unsigned yx = 0; yx < yX; yx++)
				for (unsigned xx = 0; xx < xX; xx++)
					tempX[yx][xx] = 0;

			for (unsigned i = 0; i < It; i++)
			{
				if (stepToX())
				{
					printPair(tempX, tempY);
					int n = findX(tempX);
					if (n < 0 || findY(tempY) != n) cout << "Not recognized" << endl;
					else cout << "Its pair " << n << endl;
					break;
				}

				if (stepToY())
				{
					printPair(tempX, tempY);
					int n = findX(tempX);
					if (n < 0 || findY(tempY) != n) cout << "Not recognized" << endl;
					else cout << "Its pair " << n << endl;
					break;
				}
			}
		}
		break;
		case '3':
		{
			for (unsigned n = 0; n < N; n++)
				printPair(X[n], Y[n]);
			printWeights();
			cout << endl;
			break;
		}
		}

	} while (key != '0');

	return 0;
}
