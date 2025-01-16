#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>

struct movingRectangle
{
	sf::RectangleShape rect;
	float xSpeed, ySpeed;
	sf::Text text;

public:
	movingRectangle(sf::RectangleShape rect, std::string name, int x, int y, float xSpeed, float ySpeed, int r, int g, int b, sf::Text text)
		: rect(rect), xSpeed(xSpeed), ySpeed(ySpeed), text(text)
	{
		this->rect.setPosition(sf::Vector2f(x, y));
		this->rect.setFillColor(sf::Color(r, g, b));
		this->text.setPosition(sf::Vector2f(x + this->rect.getSize().x / 2 - this->text.getCharacterSize() / 1.4,
											y + this->rect.getSize().y / 2 - this->text.getCharacterSize() / 2));
		this->text.setString(name);
	}
};

struct movingCircle
{
	sf::CircleShape circle;
	float xSpeed, ySpeed;
	sf::Text text;

public:
	movingCircle(sf::CircleShape circle, std::string name, int x, int y, float xSpeed, float ySpeed, int r, int g, int b, sf::Text text)
		: circle(circle), xSpeed(xSpeed), ySpeed(ySpeed), text(text)
	{
		this->circle.setPosition(sf::Vector2f(x, y));
		this->circle.setFillColor(sf::Color(r, g, b));
		this->text.setPosition(sf::Vector2f(x + this->circle.getRadius() - this->text.getLocalBounds().width / 2,
											y + this->circle.getRadius() - this->text.getLocalBounds().height / 2));
	this->text.setString(name);
	}
};

int main()
{
	sf::RenderWindow window;
	sf::Font font;
	sf::Text text;

	std::vector<movingRectangle> rects;
	std::vector<movingCircle>	 circles;

	std::ifstream configFile("config.txt");
	if (!configFile)
	{
		std::cerr << "\"config.txt\" not found!.";
		return 1;
	}

	std::string command;
	while (configFile >> command)
	{
		if (command == "Window")
		{
			int width, height;
			configFile >> width >> height;
			window.create(sf::VideoMode(width, height), "SFML Window", sf::Style::Default);
			window.setFramerateLimit(60);
		}

		else if (command == "Font")
		{
			std::string fontStyle;
			int size, r, g, b;
			configFile >> fontStyle >> size >> r >> g >> b;

			if (!font.loadFromFile(fontStyle))
			{
				std::cerr << "Failed to load font: " << fontStyle;
				return 1;
			}

			text.setFont(font);
			text.setCharacterSize(size);
			text.setFillColor(sf::Color(r, g, b));
		}

		else if (command == "Rectangle")
		{
			std::string name;
			float x, y, xSpeed, ySpeed, r, g, b, width, height;
			configFile >> name >> x >> y >> xSpeed >> ySpeed >> r >> g >> b >> width >> height;

			movingRectangle rect(sf::RectangleShape(sf::Vector2f(width, height)), name, x, y, xSpeed, ySpeed, r, g, b, text);

			rects.push_back(rect);
		}

		else if (command == "Circle")
		{
			std::string name;
			float x, y, xSpeed, ySpeed, r, g, b, radius;
			configFile >> name >> x >> y >> xSpeed >> ySpeed >> r >> g >> b >> radius;

			movingCircle circle(sf::CircleShape(radius), name, x, y, xSpeed, ySpeed, r, g, b, text);

			circles.push_back(circle);
		}
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			default:
				break;
			}
		}

		window.clear(sf::Color::Black);

		for (auto& r : rects)
		{
			r.rect.move(sf::Vector2f(r.xSpeed, r.ySpeed));
			r.text.move(sf::Vector2f(r.xSpeed, r.ySpeed));

			window.draw(r.rect);
			window.draw(r.text);

			if (r.rect.getPosition().x + r.rect.getSize().x >= window.getSize().x ||
				r.rect.getPosition().x <= 0.0)
			{
				r.xSpeed *= -1;
			}

			if (r.rect.getPosition().y + r.rect.getSize().y >= window.getSize().y ||
				r.rect.getPosition().y <= 0.0)
			{
				r.ySpeed *= -1;
			}
		}

		for (auto& c : circles)
		{
			c.circle.move(sf::Vector2f(c.xSpeed, c.ySpeed));
			c.text.move(sf::Vector2f(c.xSpeed, c.ySpeed));

			window.draw(c.circle);
			window.draw(c.text);

			if (c.circle.getPosition().x + c.circle.getRadius() * 2 >= window.getSize().x ||
				c.circle.getPosition().x <= 0.0f)
				c.xSpeed *= -1;

			if (c.circle.getPosition().y + c.circle.getRadius() * 2 >= window.getSize().y ||
				c.circle.getPosition().y <= 0.0f)
				c.ySpeed *= -1;
		}

		window.display();
	}

	return 0;
}