// Examples.cpp : Defines the entry point for the console application.

#include <IG.h>
#include <cmath>  

using namespace std;
using namespace IG;

char* DRIVINGOBJECTPATH = "../../../Models/VolvoV70Littles.flt"; // the object in which we navigate in the jackobs ive file
char* JACKOBSIVEPATH = "C:/Creator14/Project/4plus5.ive"; // path to the ive file (ive is the format we use instead of flt)
double anchorZ = -2.7;

void Navigate(double fromX, double toX, double fromY, double toY, double cameraYangle);

int main()
{
	Navigate(-1.5,8,-4, -4,90);

	return 0;
}

void Navigate(double fromX, double toX, double fromY, double toY,double cameraYangle)
{

	int obj1 = CIG::GetInstance()->AddObject(DRIVINGOBJECTPATH);
	int world = CIG::GetInstance()->AddObject(JACKOBSIVEPATH);
	double  posX = fromX;
	double  posY = fromY;
	double  posZ = anchorZ; 
	double delta = 0.03; // spead of running frames

	CameraProps Lprops;
	Lprops.borders = false;
	Lprops.farPlane = 30.0;
	Lprops.nearPlane = 0.001;
	Lprops.fovX = 90.0 * 4.0 / 5.0;
	Lprops.fovY = 90.0 * 3.0 / 5.0;
	Lprops.width = 320 * 2;
	Lprops.height = 240 * 2;
	Lprops.OffScreen = false;	   // Do not Draw on Screen
	Lprops.sensor = CAMERA_SENSOR::RGB;
	Lprops.windowOriginX = 0;  // window x,y top left corner
	Lprops.windowOriginY = 0;
	Lprops.multisample = 1;
	int LeftcameraKey = CIG::GetInstance()->AddCamera(Lprops);
	//Turn camera to Slave Camera
	CIG::GetInstance()->SetSlaveCamera(obj1, LeftcameraKey);
	CIG::GetInstance()->SetCameraOffset(LeftcameraKey, -0.043, 0.002, 0.12, 0, 0.0, 0.0);
	CIG::GetInstance()->SetObjectPosition(obj1, posX, posY, posZ, cameraYangle*-1, 0, 0.0);

	bool isXnav = false;
	bool isYnav = false;
	if (fromY != toY) isYnav = true;
	if (fromX != toX) isXnav = true;

	CIG::GetInstance()->Run();
	//getchar(); //wait for enter before starting showing the navigation
	while (1)
	{
		CIG::GetInstance()->Draw();		
		if (isXnav==true && isYnav==false)
		{
			if (fromX<toX)
			{
				if (posX <toX)
					posX += delta;
				else
				{
					break;
				}
			}
			else
			{
				if (posX >toX)
				{					
					posX -= delta;
				}
				else
				{
					break;
				}
			}			
		}
		else if (isXnav == false && isYnav == true)
		{
			if (fromY<toY)
			{
				if (posY <toY)
					posY += delta;
				else
				{
					break;
				}
			}
			else
			{
				if (posY >toY)
					posY -= delta;
				else
				{
					break;
				}
			}
		}
		else if (isXnav == true && isYnav == true) // cross navigation
		{
			double absDifX = abs(toX - fromX);
			double absDifY = abs(toY - fromY);
			if (absDifX > absDifY)
			{
				if (fromX<toX)
				{
					if (posX <toX)
					{
						posX += delta;
						if (posY < toY)
						{
							posY += delta*(absDifY / absDifY);
						}
						else
						{
							posY -= delta*(absDifY / absDifX);
						}
					}
					else
					{
						break;
					}
				}
				else
				{
					if (posX >toX)
					{
						posX -= delta;
						if (posY < toY)
						{
							posY += delta*(absDifY / absDifX);
						}
						else
						{
							posY -= delta*(absDifY / absDifX);
						}
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				if (fromY<toY)
				{
					if (posY <toY)
					{
						posY += delta;
						if (posX < toX)
						{
							posX += delta*(absDifX / absDifY);
						}
						else
						{
							posX -= delta*(absDifX / absDifY);
						}
					}
					else
					{
						break;
					}
				}
				else
				{
					if (posY >toY)
					{
						posY -= delta;
						if (posX < toX)
						{
							posX += delta*(absDifX / absDifY);
						}
						else
						{
							posX -= delta*(absDifX / absDifY);
						}
					}
					else
					{
						break;
					}
				}
			}
		}
		
		CIG::GetInstance()->SetObjectPosition(obj1, posX, posY, posZ, cameraYangle*-1, 0, 0.0);
	}

}

