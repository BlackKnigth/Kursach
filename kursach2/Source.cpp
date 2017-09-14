#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>

using namespace std;
using namespace sf;

struct kursor
{
	float x, y;
	int str, stb;
	kursor() 
	{
		x = 1;
		y = 1;
		str = 1;
		stb = 1;
	}
};

void delbkv(wstring* , int , int );

void refreshFile(wifstream &fin, Text *text, wstring *str, int krs, int y = 0)
{
	int i = 0;
	fin.open("input.txt");
	fin.imbue(locale(""));
	for (i = 1; i < krs - y; i++) { getline(fin, str[0]); }
	i = 0;
	while (!fin.eof() && i < 23)
	{
		getline(fin, str[i]);
		text[i].setString(str[i]);
		i++;
	}
	fin.close();
}

void krsrun(VertexArray &, bool &, float &);

bool chkfl(wifstream &,int);

int main()
{
	kursor *krs;
	krs = new kursor;


	VertexArray qvadra(LinesStrip, 5 );//раб.поле
	qvadra[0].position = Vector2f(0, 1);
	qvadra[1].position = Vector2f(799, 1);
	qvadra[2].position = Vector2f(799, 404);
	qvadra[3].position = Vector2f(1, 404);
	qvadra[4].position = Vector2f(1, 1);

	int i = 0;
	for (i = 0; i < 5;i++)
	qvadra[i].color = Color::Blue;
	VertexArray lines(Lines, 2 /*количество точек*/);//курсор
	lines[0].position = Vector2f(krs->x * 8 - 6, krs->y * 15 - 12);
	lines[1].position = Vector2f(krs->x * 8 - 6, krs->y * 15 + 5);
	lines[0].color = Color::Black;
	lines[1].color = Color::Black;
	//////////////////////////////////////////////////

	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	wifstream fin;
	//	ofstream fout("input.txt");
	///////////////////////////////////////////////////

	wstring str[23];  // Чтение текста в массив
	Text text[23];
	refreshFile(fin, text, str, krs->stb,int(krs->y));
	///////////////////////////////////////////////////

	Font font;
	font.loadFromFile("font.ttf");  //Вывод текста из массива в текст.перем.
	for (i = 0; i < 23; i++)
	{
		text[i].setCharacterSize(15);
		text[i].setFont(font);
		text[i].setString(str[i]);
		text[i].setPosition(2, i * 15 + 2);
		text[i].setColor(Color::Black);
	}
	////////////////////////////////////////////////////

	Clock clock;
	float timer = 0;
	bool bt = true;
	RenderWindow window(sf::VideoMode(700, 350), "Wordpad");
	while (window.isOpen()) 
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart(); 
		timer += time;
		if (timer >= 0.5) 
		{ 
			if (bt)
			{
				lines[0].color = sf::Color::White;
				lines[1].color = sf::Color::White;
			}
			else
			{
				lines[0].color = sf::Color::Black;
				lines[1].color = sf::Color::Black;
			}
			timer = 0; 
			if (bt) bt = false; else bt = true; 
		};


		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				if ( krs->y > 1)
				{
					krs->y--;
					krs->stb--;
					if (krs->str > str[int(krs->y - 1)].length())
					{
						krs->str = str[int(krs->y - 1)].length() + 1;
						if (krs->str > 87)
						{
							for (i = 0; i < 23; i++)
								text[i].setPosition(2 - (str[int(krs->y) - 1].length() - 87) * 8, i * 15 + 2);
							krs->x = 87;
						}
						else
						{
							for (i = 0; i < 23; i++)
								text[i].setPosition(2, i * 15 + 2);
							krs->x = str[int(krs->y) - 1].length() + 1;
						}
					}
				}
				else
				{
					if (krs->stb > 1)
					{
						krs->stb--;
						refreshFile(fin, text, str, krs->stb-1);
					}
				}
				krsrun(lines, bt, timer);
			}
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				if (krs->y < 23)
				{
					krs->y++;
					krs->stb++;
				}
				else
				{
					if (chkfl(fin,krs->stb))
					{
						refreshFile(fin, text, str, krs->stb-21);
						krs->stb++;

					}
				}
					krsrun(lines, bt, timer);
			}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				if (krs->x > 1) //Простое движение курсора
				{
					krs->x--;
					krs->str--;
				}
				else
				{
					if (krs->str > 1)
					{
						if (krs->str - krs->x > 30) //Если достигнут левый край и не достигнут край файла
						{
							krs->str--;
							for (i = 0; i < 23; i++)
							text[i].setPosition(2 - (krs->str - 30) * 8, i * 15 + 2);
							krs->x += 30;
						}
						else 
						{
							krs->str--;
							for (i = 0; i < 23; i++)
							text[i].setPosition(2 , i * 15 + 2); //Если достигнут левый край и край файла
							krs->x = krs->str;
						}
					}
					else
					{
						if (krs->stb > 1) //Переход на строку выше
						{
							krs->stb--;
							krs->str = str[krs->stb - 1].length() + 1;
							krs->x = krs->str >= 88 ? 88 : krs->str % 89;
							if (krs->y > 1)
								krs->y--;
							else
								refreshFile(fin, text, str, krs->stb, int(krs->y));
							if (krs->str > 88)
								for (i = 0; i < 23; i++)
									text[i].setPosition(2 - (krs->str - 88) * 8, i * 15 + 2);
							}
					}
				}


				krsrun(lines, bt, timer);
			}
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				if (str[int (krs->y) - 1].length() >= krs->str)
				{
					if (krs->x < 88 /*&& */)  // Простое движение курсора
					{
						krs->x++;
						krs->str++;
					}
					else
					{
						if (str[int(krs->y) - 1].length() > krs->str)
						{
							if ((str[int(krs->y) - 1].length() - krs->str) > 30)
							{
								krs->str++;
								for (i = 0; i < 23; i++)
									text[i].setPosition(2 - (krs->str - 57) * 8, i * 15 + 2);//Достигнут правый край экрана,не достигнут правый край строки
								krs->x -= 30;
							}
							else
							{
								krs->str++;
								for (i = 0; i < 23; i++)
									text[i].setPosition(2 - (str[int(krs->y) - 1].length() - 87) * 8, i * 15 + 2); //Достигнут правый край экрана,достигнут правый край строки
								krs->x -= str[int(krs->y) - 1].length() - krs->str;
							}
						}
						else
						{
							if (!chkfl(fin, krs->stb))
							{
								krs->stb++;
								krs->str = 1;
								for (i = 0; i < 23; i++)
									text[i].setPosition(2, i * 15 + 2);
								if (krs->y < 23)
									krs->y++;
								krs->x = 1;
							}
						}
					}
				}
				else
				{

				}
				krsrun(lines, bt, timer);
			}
			////////////////////////////////////////////
			if (Keyboard::isKeyPressed(Keyboard::F5))
			{
				refreshFile(fin, text, str, krs->stb - (krs->y + 1));
			}
//			if (event.type == Event::MouseButtonPressed);
			lines[0].position = Vector2f(krs->x * 8 - 6, krs->y * 15 -12 );
			lines[1].position = Vector2f(krs->x * 8 - 6, krs->y * 15 + 5);
		}
////////////////////////////////////////////////////////


		window.clear( Color::White);
		window.draw(qvadra);
		for (i = 0; i < 23; i++)
			window.draw(text[i]);
		window.draw(lines);
		window.display();
	}

	return 0;
}

bool chkfl(wifstream &fin, int stb)
{
	wstring str;
	int i;
	fin.open("input.txt");
	for (i = 0; !fin.eof(); i++) { getline(fin, str); }
	fin.close();
	if (i > stb)
		return true;
	else
		return false;
}

void krsrun(VertexArray &lines, bool &bt, float &timer)
{
	lines[0].color = Color::Black;
	lines[1].color = Color::Black;
	bt = true;
	timer = 0;
}

void delbkv(wstring *str, int x, int y)
{

}