#include <SFML/Graphics.hpp>

#include <iostream>

using namespace sf;
using namespace std;

int main()
{

	int pixelWidth = VideoMode::getDesktopMode().width;
	int pixelHeight = VideoMode::getDesktopMode().height;

	VideoMode vm(pixelWidth, pixelHeight);

	RenderWindow window(vm, "Rainbow Screen", Style::Default);

	VertexArray particle1(TriangleFan);
	particle1.resize(7);
	
	View cartesianPlane;
	cartesianPlane.setCenter(0, 0);
	cartesianPlane.setSize(pixelWidth, -1.0 * pixelHeight);


	Vector2f centerCoordinate = { (float)pixelWidth / 2.0f, (float)pixelHeight / 2.0f };
	
	Vector2f centerPixels = { (float)pixelWidth / 2.0f, (float)pixelHeight / 2.0f };
	//Vector2f centerCoordinate = window.mapPixelToCoords(centerPixel, cartesianPlane);


	/*
	* 
	particle1[0].color = Color::Blue;
	particle1[0].position = centerCoordinate;

	particle1[1].color = Color::Cyan;
	particle1[1].position = { centerCoordinate.x + 100.0f, centerCoordinate.y + 50.0f };

	particle1[2].color = Color::Yellow;
	particle1[2].position = { centerCoordinate.x + 100.0f, centerCoordinate.y + 0.0f };

	particle1[3].color = Color::Magenta;
	particle1[3].position = { centerCoordinate.x + 50.0f, centerCoordinate.y - 50.0f };

	particle1[4].color = Color::Red;
	particle1[4].position = { centerCoordinate.x + 0.0f, centerCoordinate.y - 100.0f };

	particle1[5].color = Color::White;
	particle1[5].position = { centerCoordinate.x - 100.0f, centerCoordinate.y - 50.0f };

	particle1[6].color = Color::White;
	particle1[6].position = { centerCoordinate.x - 50.0f, centerCoordinate.y + 0.0f };
	
	*/

	while (window.isOpen())
	{
		///Input
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();

		}
		///Update

		///Draw

		window.clear();
		window.draw(particle1);
		//window.draw(...);

		window.display();

	}




}
