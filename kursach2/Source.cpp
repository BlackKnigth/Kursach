#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>

using namespace std;
using namespace sf;

struct kursor
{
	int x, y;
	int fx, fy;
	kursor() 
	{
		x = 1;
		y = 1;
		fx = 1;
		fy = 1;
	}
};

void refreshFile(int fy, wstring str)
{
	wifstream in("input_copy.txt");
	wofstream out("input_copy2.txt");
	in.imbue(locale(""));
	out.imbue(locale(""));
	wstring s;
	for (int i = 0; i < fy; i++) { getline(in, s); out << s << endl; }
	getline(in, s);
	out << str;
	while (!in.eof()) { getline(in, s); out << endl << s; }
	in.close();
	out.close();
	rename("input_copy.txt", "input_copy3.txt");
	rename("input_copy2.txt", "input_copy.txt");
	remove("input_copy3.txt");
} 

void refreshStr(wifstream &fin, Text *text, wstring *str, int krs, int y)
{
	int i = 0;
	fin.open("input_copy.txt");
	fin.imbue(locale(""));
	for (i; i < krs - y; i++) { getline(fin, str[0]); }
	i = 0;
	while (!fin.eof() && i < 23)
	{
		getline(fin, str[i]);
		text[i].setString(str[i]);
		i++;
	}
	fin.close();
}

void krsrun(VertexArray &lines, bool &bt, float &timer)
{
	lines[0].color = Color::Black;
	lines[1].color = Color::Black;
	bt = true;
	timer = 0;
}

bool chkfl(wifstream &fin, int stb)
{
	wstring str;
	int i;
	fin.open("input_copy.txt");
	for (i = 0; !fin.eof(); i++) { getline(fin, str); }
	fin.close();
	if (i > stb)
		return true;
	else
		return false;
}

void createCopy()
{
	wifstream in;
	wofstream out("input_copy.txt");
	in.open("input.txt");
	in.imbue(locale(""));
	out.imbue(locale(""));
	wstring s;
	if (in)
		while (!in.eof())
			getline(in, s), out << s << endl;
	out.close();
	in.close();
}

int main()
{
	kursor *krs;
	krs = new kursor;

	int wid = 698;
	int hei = 350;
	VertexArray qvadra(LinesStrip, 5 );//ğàá.ïîëå
	qvadra[0].position = Vector2f(0, 1);
	qvadra[1].position = Vector2f(wid - 1, 1);
	qvadra[2].position = Vector2f(wid - 1, hei - 1);
	qvadra[3].position = Vector2f(1, hei - 1);
	qvadra[4].position = Vector2f(1, 1);

	int i = 0;
	for (i = 0; i < 5;i++)
	qvadra[i].color = Color::Blue;
	VertexArray lines(Lines, 2 /*êîëè÷åñòâî òî÷åê*/);//êóğñîğ
	lines[0].position = Vector2f(krs->x * 8 - 6, krs->y * 15 - 12);
	lines[1].position = Vector2f(krs->x * 8 - 6, krs->y * 15 + 5);
	lines[0].color = Color::Black;
	lines[1].color = Color::Black;
	//////////////////////////////////////////////////

	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	wifstream fin;

	wstring str[23];  // ×òåíèå òåêñòà â ìàññèâ
	createCopy();
	Text text[23];
	refreshStr(fin, text, str, krs->fy, int(krs->y));
	///////////////////////////////////////////////////

	Font font;
	font.loadFromFile("font.ttf");  //Âûâîä òåêñòà èç ìàññèâà â òåêñò.ïåğåì.
	for (i = 0; i < 23; i++)
	{
		text[i].setCharacterSize(15);
		text[i].setFont(font);
		text[i].setString(str[i]);
		text[i].setPosition(2, i * 15 + 2);
		text[i].setColor(Color::Black);
	}
	Text quit;
	quit.setCharacterSize(10);
	quit.setFont(font);
	quit.setString(L"Ñîõğàíèòü èçìåíåíèÿ? (1 - äà,2 - íåò)");
	quit.setPosition(2, 2);
	quit.setColor(Color::Black);

	////////////////////////////////////////////////////

	Clock clock;
	float timer = 0;
	bool bt = true;
	RenderWindow window(sf::VideoMode(wid, hei), "Wordpad");
	window.setVerticalSyncEnabled(true);
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
			bt = bt ? false : true;
		};

		Event event;
		while (window.pollEvent(event))
		{
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				krsrun(lines, bt, timer);
				if (krs->fy > 1)//ÄÂÈÆÅÍÈÅ İÊĞÀÍÀ ÂÂÅĞÕ
				{
					krs->fy--;
					if (krs->y > 1)  //ÄÂÈÆÅÍÈÅ ÊÓĞÑÎĞÀ ÂÂÅĞÕ
						krs->y--;
					else
					refreshStr(fin, text, str, krs->fy, int(krs->y));
					if (str[int(krs->y - 1)].length() + 1 < krs->fx)//ÈÇÌÅÍÅÍÈß ÊÎÎĞÄÈÍÀÒÛ Õ ÊÓĞÑÎĞÀ, ÅÑËÈ ÑÒĞÎÊÀ ÂÛØÅ ÌÅÍÜØÅ ÏÎ ÄËÈÍÅ
					{
						krs->fx = str[int(krs->y - 1)].length() + 1;
						if (krs->fx > 30)
						{
							for (i = 0; i < 23; i++)
								text[i].setPosition(2 - (krs->fx - 30) * 8, i * 15 + 2);
							krs->x = 30;
						}
						else
						{
							for (i = 0; i < 23; i++)
								text[i].setPosition(2, i * 15 + 2);
							krs->x = krs->fx;
						}
					}
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				krsrun(lines, bt, timer);
				if (chkfl(fin, krs->fy))//ÄÂÈÆÅÍÈÅ İÊĞÀÍÀ ÂÍÈÇ
				{
					krs->fy++;
					if (krs->y < hei / 15)  //ÄÂÈÆÅÍÈÅ ÊÓĞÑÎĞÀ ÂÍÈÇ
						krs->y++;
					else
					refreshStr(fin, text, str, krs->fy, int(krs->y));
					if (str[int(krs->y - 1)].length() + 1 < krs->fx)//ÈÇÌÅÍÅÍÈß ÊÎÎĞÄÈÍÀÒÛ Õ ÊÓĞÑÎĞÀ, ÅÑËÈ ÑÒĞÎÊÀ ÍÈÆÅ ÌÅÍÜØÅ ÏÎ ÄËÈÍÅ
					{
						krs->fx = str[int(krs->y - 1)].length() + 1;
						if (krs->fx > 30)
						{
							for (i = 0; i < 23; i++)
								text[i].setPosition(2 - (krs->fx - 30) * 8, i * 15 + 2);
							krs->x = 30;
						}
						else
						{
							for (i = 0; i < 23; i++)
								text[i].setPosition(2, i * 15 + 2);
							krs->x = krs->fx;
						}
					}
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				krsrun(lines, bt, timer);
				if (krs->fx > 1)
				{
					krs->fx--;
					if (krs->x > 1)//ÄÂÈÆÅÍÈÅ ÊÓĞÑÎĞÀ ÂËÅÂÎ
						krs->x--;
					else//ÄÂÈÆÅÍÈÅ İÊĞÀÍÀ ÂËÅÂÎ
					{
						if (krs->fx > 30)//ÑÄÂÈÃ İÊĞÀÍÀ ÍÀ 30
						{
							for (i = 0; i < 23; i++)
								text[i].setPosition(2 - (krs->fx - 30) * 8, i * 15 + 2);
							krs->x = 30;
						}
						else//ÑÄÂÈÃ İÊĞÀÍÀ ÍÀ fx
						{
							for (i = 0; i < 23; i++)
								text[i].setPosition(2, i * 15 + 2);
							krs->x = krs->fx;
						}
					}
				}
				else//ÏÅĞÅÕÎÄ ÍÀ ÏĞÅÄ ÑÒĞÎÊÓ
				{
					if (krs->fy > 1)
					{
						krs->fy--;
						if (krs->y > 1)
							krs->y--;
						else
						refreshStr(fin, text, str, krs->fy, int(krs->y));
						krs->fx = str[int(krs->y - 1)].length() + 1;
						if (str[int(krs->y - 1)].length() > wid / 8)//
						{
							for (i = 0; i < 23; i++)
								text[i].setPosition(2 - (krs->fx - wid / 8) * 8, i * 15 + 2);
							krs->x = wid / 8;
						}
						else //
						{
							for (i = 0; i < 23; i++)
								text[i].setPosition(2, i * 15 + 2);
							krs->x = krs->fx;
						}
					}
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				krsrun(lines, bt, timer);
				if (krs->fx < str[int(krs->y - 1)].length() + 1)
				{
					krs->fx++;
					if (krs->x < 88)//ÄÂÈÆÅÍÈÅ ÊÓĞÑÎĞÀ ÂÏĞÀÂÎ
						krs->x++;
					else//ÄÂÈÆÅÍÈÅ İÊĞÀÍÀ ÂÏĞÀÂÎ
					{
						if (str[int(krs->y - 1)].length() + 1 - krs->fx > 30)//ÑÄÂÈÃ İÊĞÀÍÀ ÍÀ 30
						{
							for (i = 0; i < 23; i++)
								text[i].setPosition(2 - (krs->fx - 58) * 8, i * 15 + 2);
							krs->x = 58;
						}
						else//ÑÄÂÈÃ İÊĞÀÍÀ ÍÀ x
						{
							for (i = 0; i < 23; i++)
								text[i].setPosition(2 - (int(str[int(krs->y - 1)].length()) - wid / 8) * 8, i * 15 + 2);
							krs->x = wid / 8 - (str[int(krs->y - 1)].length() - krs->fx);
						}
					}
				}
				else
				{
					if (chkfl(fin, krs->fy))//ÏÅĞÅÕÎÄ ÍÀ ÑËÅÄ ÑÒĞÎÊÓ
					{

						krs->fy++;
						if (krs->y < 23)
							krs->y++;
						else
						refreshStr(fin, text, str, krs->fy, int(krs->y));
						krs->fx = 1;
						for (i = 0; i < 23; i++)
							text[i].setPosition(2, i * 15 + 2);
						krs->x = 1;
					}
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::BackSpace))
			{
				do
				{
					bool frt = true;
					if (krs->fx > 1)
					{
						str[krs->y - 1] = str[krs->y - 1].substr(0, krs->x - 2) + str[krs->y - 1].substr(krs->x - 1, str[krs->y - 1].size() - krs->x + 1);
						text[krs->y - 1].setString(str[krs->y - 1]);
						krs->fx--;
						krs->x--;
						refreshFile(krs->fy - 1, str[krs->y - 1]);
						frt ? sleep(milliseconds(250)) : frt = false, sleep(milliseconds(30));
					}
				} while (!event.type == Event::KeyReleased);
			}
			if (Keyboard::isKeyPressed(Keyboard::Return))
			{
				str[krs->y - 1] = str[krs->y - 1].substr(0, krs->x - 1) + '\n' + str[krs->y - 1].substr(krs->x - 1, str[krs->y - 1].size() - krs->x + 1);
				refreshFile(krs->fy - 1, str[krs->y - 1]);
				krs->fx = 1;
				krs->x = 1;
				krs->fy++;
				krs->y++;
				refreshStr(fin, text, str, krs->fy, int(krs->y));
				sleep(milliseconds(150));
			}
			if (event.type == sf::Event::Resized)
			{
				wid = event.size.width;
				hei = event.size.height;
			}
			if (event.type == Event::TextEntered) 
			{
				if (event.text.unicode > 31)
				{
					str[krs->y - 1] = str[krs->y - 1].substr(0, krs->x - 1) + event.text.unicode + str[krs->y - 1].substr(krs->x - 1, str[krs->y - 1].size() - krs->x + 1);
					text[krs->y - 1].setString(str[krs->y - 1]);
					krs->fx++;
					krs->x++;
					refreshFile(krs->fy - 1, str[krs->y - 1]);
				}
			}
	/*		if (event.type == sf::Event::LostFocus)
			{
				window.pause();
			}*/
			if (event.type == Event::Closed)
			{
				RenderWindow window2(sf::VideoMode(200, 150), "Quit"/*,Style::None*/);
				while (window.isOpen())
				{

					while (window2.pollEvent(event))
					{
						if (event.type == Event::Closed)
							window2.close();
						if (event.type == Event::TextEntered)
						{
							if (event.text.unicode == '1')
							{
								window2.close();
								rename("input.txt", "input_copy2.txt");
								rename("input_copy.txt", "input.txt");
								remove("input_copy2.txt");
								window.close();
							}
							if (event.text.unicode == '2')
							{
								window2.close();
								remove("input_copy.txt");
								window.close();
							}
						}
					}
					window2.clear(Color::White);
					window2.draw(quit);
					window2.display();
				}
			}
	/*	if (event.type == sf::Event::GainedFocus)
			{
				window.resume();
			}*/

			////////////////////////////////////////////
			lines[0].position = Vector2f(krs->x * 8 - 6, krs->y * 15 - 12);
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