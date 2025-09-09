//#include "Grid.h"
//
//
//Grid::Grid()
//{
//	
//
//}
//void Grid::operator()(double size)
//{
//	
//
//	
//}
//Grid::Grid(Eigen::Vector2f x, Eigen::Vector2f y, Eigen::Vector2f z)
//{
//	xInterval = x;
//	yInterval = y;
//	zInterval = z;
//
//	xLength = abs(x[1] - x[0]);
//	yLength = abs(y[1] - y[0]);
//	zLength = abs(z[1] - z[0]);
//
//	nX = 10.0;
//	nY = 10.0;
//	nZ = 10.0;
//	xDist = xLength * ((double)1.0 / nX);
//	yDist = yLength * ((double)1.0 / nY);
//	zDist = zLength * ((double)1.0 / nZ);
//
//	theGrid = new Cell**[nX];
//	for (int i = 0; i < nX; i++)
//	{
//
//		theGrid[i] = new Cell*[nY];
//		for (int j = 0; j < nY; j++)
//		{
//			theGrid[i][j] = new Cell[nZ];
//			for (int k = 0; k < nZ; k++)
//			{
//
//				theGrid[i][j][k].xInterval[0] = x[0] +(xLength* ((float)i / nX));
//				theGrid[i][j][k].xInterval[1] = x[0] +(xLength*((float)(i + 1) / nX));
//
//				theGrid[i][j][k].yInterval[0] = y[0] + (yLength * ((float)j / nY));
//				theGrid[i][j][k].yInterval[1] = y[0] + (yLength * ((float)(j+1) / nY));
//
//				theGrid[i][j][k].zInterval[0] = z[0] + (zLength * ((float)k / nZ));
//				theGrid[i][j][k].zInterval[1] = z[0] + (zLength * ((float)(k+1) / nZ));
//				
//			}
//		}
//	}
//}
//
//Grid::~Grid()
//{
//
//	for (int i = 0; i < nX; i++)
//	{
//		for (int j = 0; j < nY; j++)
//		{
//			delete[] theGrid[i][j];
//		}
//		delete[] theGrid[i];
//	}
//	delete[] theGrid;
//}
//
//void Grid::fill(vector<Geometry*> g)
//{
//	for (int a = 0; a < g.size(); a++)
//	{
//
//		for (int i = 0; i < nX; i++)
//		{
//			for (int j = 0; j < nY; j++)
//			{
//				for (int k = 0; k < nZ; k++)
//				{
//					bool testx1 = (g[a]->xInterval[0] >= theGrid[i][j][k].xInterval[0] && g[a]->xInterval[0] <= theGrid[i][j][k].xInterval[1]) ||
//						(g[a]->xInterval[1] <= theGrid[i][j][k].xInterval[1] && g[a]->xInterval[1] >= theGrid[i][j][k].xInterval[0]);
//					bool testx2 = (theGrid[i][j][k].xInterval[0] >= g[a]->xInterval[0] && theGrid[i][j][k].xInterval[1] <= g[a]->xInterval[1]);
//
//					bool testy1 = (g[a]->yInterval[0] >= theGrid[i][j][k].yInterval[0] && g[a]->yInterval[0] <= theGrid[i][j][k].yInterval[1]) ||
//						(g[a]->yInterval[1] <= theGrid[i][j][k].yInterval[1] && g[a]->yInterval[1] >= theGrid[i][j][k].yInterval[0]);
//					bool testy2 = (theGrid[i][j][k].yInterval[0] >= g[a]->yInterval[0] && theGrid[i][j][k].yInterval[1] <= g[a]->yInterval[1]);
//
//					bool testz1 = (g[a]->zInterval[0] >= theGrid[i][j][k].zInterval[0] && g[a]->zInterval[0] <= theGrid[i][j][k].zInterval[1]) ||
//						(g[a]->zInterval[1] <= theGrid[i][j][k].zInterval[1] && g[a]->zInterval[1] >= theGrid[i][j][k].zInterval[0]);
//					bool testz2 = (theGrid[i][j][k].zInterval[0] >= g[a]->zInterval[0] && theGrid[i][j][k].zInterval[1] <= g[a]->zInterval[1]);
//
//
//					if (testx1 || testx2)
//					{
//						if (testy1 || testy2)
//						{
//							if (testz1 || testz2)
//							{
//								theGrid[i][j][k].objects.push_back(g[a]);
//							}
//						}
//					}
//				}
//			}
//		}
//
//
//
//	}
//}
//
//HitContainer Grid::march(Ray r)
//{
//	int signx = 1;
//	int signy = 1;
//	int signz = 1;
//
//	if (r.directionV[0] < 0)
//		signx = -1;
//	if (r.directionV[1] < 0)
//		signy = -1;
//	if (r.directionV[2] < 0)
//		signz = -1;
//
//	float tMin = 0,tMinX = 0, tMinY = 0, tMinZ = 0;
//	
//	int xCell = floor(((r.originP[0]*nX)+((-1)*xInterval[0]*nX)) / xLength);
//	if (xCell > nX-1)
//		xCell = nX-1;
//	int yCell = floor(((r.originP[1] * nY) + ((-1) * yInterval[0] * nY)) / yLength);
//	if (yCell > nY - 1)
//		yCell = nY - 1;
//	int zCell = floor(((r.originP[2] * nZ) + ((-1) * zInterval[0] * nZ)) / zLength);
//	if (zCell > nZ - 1)
//		zCell = nZ - 1;
//	
//	//Cell holder = theGrid[xCell][yCell][zCell];
//
//
//	float txStart = (theGrid[xCell][yCell][zCell].xInterval[0] - r.originP.x()) / r.directionV.x();
//	float txEnd = (theGrid[xCell][yCell][zCell].xInterval[1] - r.originP.x()) / r.directionV.x();
//
//
//	float tyStart = (theGrid[xCell][yCell][zCell].yInterval[0] - r.originP.y()) / r.directionV.y();
//	float tyEnd = (theGrid[xCell][yCell][zCell].yInterval[1] - r.originP.y()) / r.directionV.y();
//
//
//	float tzStart = (theGrid[xCell][yCell][zCell].zInterval[0] - r.originP.z()) / r.directionV.z();
//	float tzEnd = (theGrid[xCell][yCell][zCell].zInterval[1] - r.originP.z()) / r.directionV.z();
//
//
//	Eigen::Vector3f starts(txStart, tyStart, tzStart);
//	tMin = txStart;
//	int index = 0;
//	for (int i = 0; i < 3; i++)
//	{
//		if (starts[i] > tMin)
//		{
//			tMin = starts[i];
//		}
//	}
//	
//	HitContainer e;
//	e.g = NULL;
//
//
//	float tNextX, tNextY, tNextZ;
//	float dtx = xDist / r.directionV[0];
//	float dty = yDist / r.directionV[1];
//	float dtz = zDist / r.directionV[2];
//	tNextX = tMin + dtx;
//	tNextY = tMin + dty;
//	tNextZ = tMin + dtz;
//	vector<Geometry*> hits;
//	vector<Eigen::Vector4d> currentPixel;
//	int closest = 0;
//	bool intersected = false;
//	while ((xCell >= 0 && xCell < nX) && (yCell >= 0 && yCell < nY) && (zCell >= 0 && zCell < nZ))
//	{
//		
//		
//		Cell holder = theGrid[xCell][yCell][zCell];
//		//if (inters(r, holder.xInterval, holder.yInterval, holder.zInterval))
//		//{
//			for (int i = 0; i < holder.objects.size(); i++)
//			{
//				//if (!holder.objects[i]->tested)
//				//{
//					Eigen::VectorXd p = holder.objects[i]->intersect(r);
//					if (p[4] > 0)
//					{
//						hits.push_back(holder.objects[i]);
//						currentPixel.push_back(Eigen::Vector4d(p[0], p[1], p[2], p[3]));
//						holder.objects[i]->tested = true;
//						intersected = true;
//						
//
//					}
//				//}
//			}
//			if (intersected)
//				break;
//		//}
//
//
//
//
//		if (tNextX < tNextY && tNextX < tNextZ)
//		{
//			xCell += signx;
//			tMin = tNextX;
//			tNextX += dtx;
//		}
//		else if (tNextY < tNextX && tNextY < tNextZ)
//		{
//			yCell += signy;
//			tMin = tNextY;
//			tNextY += dty;
//		}
//		else if (tNextZ < tNextY && tNextZ < tNextX)
//		{
//			zCell += signz;
//			tMin = tNextZ;
//			tNextZ += dtz;
//		}
//	}
//
//	if (hits.size() >= 1)
//	{
//		for (int i = 0; i < hits.size(); i++)
//		{
//			if (hits[i] != hits[closest])
//			{
//				if (hits[i]->currentPixel.z() > hits[closest]->currentPixel.z())
//				{
//					closest = i;
//				}
//			}
//		}
//		e.g = hits[closest];
//		e.hitPixel = currentPixel[closest];
//
//		//break;
//	}
//
//	return e;
//	//x = origin.x + t*directionx
//	//origin.x = x - t*directionx
//	
//
//	
//}