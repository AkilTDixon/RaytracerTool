//#include "Box.h"
//
//const int MAX_DIMS = 3;
//
//KDTree::KDTree()
//{
//
//}
//KDTree::KDTree(Eigen::Vector2d xInt, Eigen::Vector2d yInt, Eigen::Vector2d zInt)
//{
//
//
//	/*
//	Equation of a Line:
//	Origin + (t)Direction = L
//	x = o.x() + t*D.x()
//	y = o.y() + t*D.y()
//	z = o.z() + t*D.z()
//	*/
//
//
//
//	root = new KNode;
//	root->index = 0; // split by the x-point
//	root->xInterval = xInt;
//	root->yInterval = yInt;
//	root->zInterval = zInt;
//	root->axisSplit = root->index;
//	root->intIndex = 0;
//	root->leaf = false;
//	root->left = NULL;
//	root->right = NULL;
//}
//
//
//
//KDTree::~KDTree()
//{
//	clean(root);
//}
//
//void KDTree::clean(KNode* node)
//{
//	if (node->left != NULL)
//		clean(node->left);
//	if (node->right != NULL)
//		clean(node->right);
//
//
//	if (node != NULL)
//		delete node;
//}
//
//void KDTree::build(KNode* node, int depth)
//{
//	if (depth == 0)
//	{
//		node->leaf = true;
//		return;
//	}
//	if (node->left == NULL)
//	{
//
//		node->left = new KNode;
//		node->left->leaf = false;
//		node->left->index = (node->index + 1) % MAX_DIMS;
//		node->axisSplit = node->index;
//		setLeftInterval(node, node->left, node->index);
//
//		node->left->right = NULL;
//		node->left->left = NULL;
//		build(node->left, depth - 1);
//
//	}
//
//	if (node->right == NULL)
//	{
//		node->right = new KNode;
//		node->right->leaf = false;
//		node->right->index = (node->index + 1) % MAX_DIMS;
//		node->axisSplit = node->index;
//		setRightInterval(node, node->right, node->index);
//
//
//		node->right->left = NULL;
//		node->right->right = NULL;
//		build(node->right, depth - 1);
//	}
//
//
//}
//
//HitContainer KDTree::findIntersect(Ray r, HitContainer* origin, bool shadow)
//{
//
//	//return loopForHits(r, root->objects, shadow, origin);
//	HitContainer e1;
//	e1.g = NULL;
//	HitContainer e2;
//	e2.g = NULL;
//
//
//
//
//	double txStart = (root->xInterval[0] - (r).originP.x()) / ((r).directionV.x());
//	double txEnd = (root->xInterval[1] - (r).originP.x()) / ((r).directionV.x());
//	if (txStart > txEnd)
//		swap(txStart, txEnd);
//
//
//	double tyStart = (root->yInterval[0] - (r).originP.y()) / ((r).directionV.y());
//	double tyEnd = (root->yInterval[1] - (r).originP.y()) / ((r).directionV.y());
//	if (tyStart > tyEnd)
//		swap(tyStart, tyEnd);
//
//
//	double tzStart = (root->zInterval[0] - (r).originP.z()) / ((r).directionV.z());
//	double tzEnd = (root->zInterval[1] - (r).originP.z()) / ((r).directionV.z());
//	if (tzStart > tzEnd)
//		swap(tzStart, tzEnd);
//
//
//	Eigen::Vector3d starts(txStart, tyStart, tzStart);
//	Eigen::Vector3d ends(txEnd, tyEnd, tzEnd);
//
//	double tStart = txStart;
//	double tEnd = txEnd;
//
//	for (int i = 0; i < 3; i++)
//	{
//		if (starts[i] > tStart)
//			tStart = starts[i];
//		if (ends[i] < tEnd)
//			tEnd = ends[i];
//	}
//
//	tRStart = tStart;
//	tREnd = tEnd;
//	KNode* hold1 = root->right;
//	KNode* hold2 = root->left;
//
//	if (r.directionV[root->axisSplit] < 0)
//	{
//		hold1 = root->left;
//		hold2 = root->right;
//	}
//	
//
//	double t = 0;
//	Eigen::Vector2d interval = getInterval(root, root->axisSplit);
//	if (r.directionV[root->axisSplit] == 0)
//		return loopForHits(r, root->objects, shadow, origin);
//	double splitPos = floorl(((interval[1] + interval[0]) / 2.0) * 1000) / 1000;
//	t = (splitPos - r.originP[root->axisSplit]) / r.directionV[root->axisSplit];
//
//	//if (t <= tStart)
//	//{
//	//	e1 = traverse2(r, hold1, tStart, tEnd, origin, shadow);
//
//	//}
//	//else if (t >= tEnd)
//	//{
//	//	e1 = traverse2(r, hold2, tStart, tEnd, origin, shadow);
//	//
//	//}
//
//	//if (e1.g == NULL)
//	//{
//	//	if (e1.hitExists || (t > tStart && t < tEnd))
//	//	{
//	//		e2 = traverse2(r, hold2, tStart, t, origin, shadow);
//	//		e1 = traverse2(r, hold1, t, tEnd, origin, shadow);
//
//
//	//	}
//	//
//	//}
//
//
//
//
//	//if (e1.g != NULL)
//	//{
//	//	if (e1.g->comment == "floor")
//	//	{
//
//	//		HitContainer test = loopForHits(r, root->objects, shadow, origin);
//	//		if (test.g != NULL)
//	//		{
//	//			if (test.g->comment == "small block left side")
//	//				bool kk = true;
//	//		}
//	//	}
//	//}
//	//else if (e2.g != NULL)
//	//{
//	//	if (e2.g->comment == "floor")
//	//	{
//	//		HitContainer test = loopForHits(r, root->objects, shadow, origin);
//	//		if (test.g->comment == "small block left side")
//	//			bool kk = true;
//	//	}
//	//}
//
////if (inters2D(r, root->xInterval, root->yInterval))
////{
//		//double t = 0;
//		//Eigen::Vector2f interval = getInterval(root->left, root->axisSplit);
//		//if (r.directionV[root->axisSplit] == 0)
//		//	return loopForHits(r,root->objects,shadow,origin);
//		//t = (interval[root->left->intIndex] - r.originP[root->axisSplit]) / r.directionV[root->axisSplit];
//
//	if (t <= tStart)
//	{
//		e1 = traverse2(r, hold1, tStart, tEnd, origin, shadow);
//		if (e1.g != NULL)
//			return e1;
//	}
//	else if (t >= tEnd)
//	{
//		e1 = traverse2(r, hold2, tStart, tEnd, origin, shadow);
//		if (e1.g != NULL)
//			return e1;
//	}
//
//	if (e1.hitExists || (t > tStart && t < tEnd))
//	{
//		e2 = traverse2(r, hold2, tStart, t, origin, shadow);
//		if (e2.g != NULL)
//			return e2;
//		e1 = traverse2(r, hold1, t, tEnd, origin, shadow);
//		if (e1.g != NULL)
//			return e1;
//
//		if (e1.hitExists)
//			return e1;
//		else
//			return e2;
//		//if (e2.hitPixel.z() > e1.hitPixel.z())
//		//	return e2;
//		//else
//		//	return e1;
//	}
//	else
//		return e1;
//	//e1 = traverse2(r, hold1, tStart, tEnd, origin, shadow);
//	//e2 = traverse2(r, hold2, tStart, tEnd, origin, shadow);
//
//	//if (e2.hitPixel.z() > e1.hitPixel.z())
//	//	return e2;
//	//else
//	//	return e1;
//}
//bool KDTree::cull(KNode* node)
//{
//	if (node->leaf)
//	{
//		if (node->objects.size() == 0)
//		{
//			delete node;
//			return true;
//		}
//		return false;
//	}
//	bool leftCulled = false;
//	bool rightCulled = false;
//
//	leftCulled = cull(node->left);
//	if (leftCulled)
//		node->left = NULL;
//	rightCulled = cull(node->right);
//	if (rightCulled)
//		node->right = NULL;
//
//	if (leftCulled && rightCulled)
//	{
//		delete node;
//		return true;
//	}
//	return false;
//}
//
//void KDTree::operator()(Eigen::Vector2d xInt, Eigen::Vector2d yInt, Eigen::Vector2d zInt)
//{
//	root = new KNode;
//	root->index = 0; // split by the x-point
//	root->xInterval = xInt;
//	root->yInterval = yInt;
//	root->zInterval = zInt;
//	root->intIndex = 0;
//	root->leaf = false;
//	root->left = NULL;
//	root->right = NULL;
//}
//
//HitContainer KDTree::traverse2(Ray r, KNode* node, double tStart, double tEnd, HitContainer* origin, bool shadow)
//{
//
//	if (node == NULL)
//	{
//		HitContainer hit;
//		hit.g = NULL;
//		return hit;
//	}
//	if (node->leaf)
//	{
//		
//		HitContainer hit;
//		hit.g = NULL;
//
//
//
//		vector<Eigen::Vector4d> currentPixel;
//		vector<Geometry*> hits;
//
//
//		int closestIndex = 0;
//
//		for (int k = 0; k < node->objects.size(); k++)
//		{
//
//			//if (!node->objects[k]->tested)
//			//{
//				if (origin != NULL)
//				{
//					if (node->objects[k] != origin->g)
//					{
//						if (inters(r, node->objects[k]->xInterval, node->objects[k]->yInterval, node->objects[k]->zInterval))
//						{
//							Eigen::VectorXd t = node->objects[k]->intersect(r);
//							double ff = t[3];
//							double test1 = abs(ff - tStart);
//							double test2 = abs(ff - tEnd);
//							if (test1 > 0 && test1 < 0.1)
//								t[3] = tStart;
//
//							if (test2 > 0 && test2 < 0.1)
//								t[3] = tEnd;
//
//
//							if ((t[3] <= tEnd && t[3] >= tStart) && t[4] > 0)
//							{
//								currentPixel.push_back(Eigen::Vector4d(t[0], t[1], t[2], t[3]));
//								hits.push_back(node->objects[k]);
//							}
//							else if (t[4] > 0)
//								hit.hitExists = true;
//							//node->objects[k]->tested = true;
//							
//						}
//					}
//				}
//				else
//				{
//					if (inters(r, node->objects[k]->xInterval, node->objects[k]->yInterval, node->objects[k]->zInterval))
//					{
//
//						Eigen::VectorXd t = node->objects[k]->intersect(r);
//
//						double ff = t[3];
//						double test1 = abs(ff - tStart);
//						double test2 = abs(ff - tEnd);
//						if (test1 > 0 && test1 < 1)
//							t[3] = tStart;
//
//						if (test2 > 0 && test2 < 1)
//							t[3] = tEnd;
//
//						if ((t[3] <= tEnd && t[3] >= tStart) && t[4] > 0)
//						{
//							currentPixel.push_back(Eigen::Vector4d(t[0], t[1], t[2], t[3]));
//							hits.push_back(node->objects[k]);
//
//						}
//						else if (t[4] > 0)
//						{
//							hit.hitExists = true;
//						}
//					}
//				}
//			//}
//
//		}
//
//		//If there is 1 or more object hit at the pixel
//		if (hits.size() >= 1)
//		{
//			if (!shadow)
//			{
//				for (int k = 0; k < hits.size(); k++)
//				{
//					//If the object's pixel has a higher z-value than the current closest object to the camera, then there's a new closest object
//					if (hits[k] != hits[closestIndex])
//					{
//						if (currentPixel[k].z() > currentPixel[closestIndex].z())
//							closestIndex = k;
//					}
//				}
//			}
//			else
//			{
//				float lRayLength = dist2(origin->lightSource - origin->hitPixel);
//				for (int k = 0; k < hits.size(); k++)
//				{
//					//If the object's pixel has a higher z-value than the current closest object to the camera, then there's a new closest object
//
//
//					float d1 = dist2(currentPixel[k] - origin->lightSource);
//					float d2 = dist2(currentPixel[k] - origin->hitPixel);
//
//					if (d1 < lRayLength && d2 < lRayLength)
//					{
//						hit.g = hits[k];
//						hit.hitPixel = currentPixel[k];
//						hit.shadow = true;
//						
//						return hit;
//					}
//
//				}
//
//				return hit;
//
//			}
//			
//			hit.g = hits[closestIndex];
//			hit.hitPixel = currentPixel[closestIndex];
//
//
//		}
//
//
//
//		return hit;
//	}
//	//if (tStart > tEnd)
//	//	swap(tStart, tEnd);
//
//	HitContainer e1;
//	e1.g = NULL;
//	HitContainer e2;
//	e2.g = NULL;
//
//	Eigen::Vector2d interval = getInterval(node, node->axisSplit);
//	double splitPos = floorl(((interval[1] + interval[0]) / 2.0) * 1000) / 1000;
//	double t = 0;
//	if (r.directionV[node->axisSplit] == 0)
//		return loopForHits(r, root->objects, shadow, origin);
//	t = (splitPos - r.originP[node->axisSplit]) / r.directionV[node->axisSplit];
//	if (t < tRStart)
//		t = tRStart;
//	if (t > tREnd)
//		t = tREnd;
//
//	KNode* hold1 = node->right;
//	KNode* hold2 = node->left;
//	if (r.directionV[node->axisSplit] < 0)
//	{
//		hold1 = node->left;
//		hold2 = node->right;
//	}
//
//	if (t <= tStart)
//	{
//		e1 = traverse2(r,hold1,tStart,tEnd,origin,shadow);
//		if (e1.g != NULL)
//			return e1;
//	}
//	else if (t >= tEnd)
//	{
//		e1 = traverse2(r, hold2, tStart, tEnd, origin, shadow);
//		if (e1.g != NULL)
//			return e1;
//	}
//	
//	if (e1.hitExists || (t > tStart && t < tEnd))
//	{
//		e2 = traverse2(r, hold2, tStart, t, origin, shadow);
//		if (e2.g != NULL)
//			return e2;
//		e1 = traverse2(r, hold1, t, tEnd, origin, shadow);
//		if (e1.g != NULL)
//			return e1;
//
//		if (e1.hitExists)
//			return e1;
//		else
//			return e2;
//		//if (e2.hitPixel.z() > e1.hitPixel.z())
//		//	return e2;
//		//else
//		//	return e1;
//	}
//	else
//		return e1;
//
//	/*if (shadow)
//		return e1;
//
//	if (e1.g != NULL && e2.g != NULL)
//	{
//			if (e1.hitPixel.z() > e2.hitPixel.z())
//				return e1;
//			else
//				return e2;
//		}
//		else if (e2.g != NULL)
//			return e2;
//		else
//			return e1;*/
//
//	
//
//
//}
//void KDTree::traverse(KNode* parent, KNode* node, vector<Geometry*> g)
//{
//
//	if (node == NULL)
//	{
//		return;
//	}
//
//	if (node->leaf)
//	{
//		for (int i = 0; i < g.size(); i++)
//		{
//
//			bool testx1 = (g[i]->xInterval[0] >= node->xInterval[0] && g[i]->xInterval[0] <= node->xInterval[1]) ||
//				(g[i]->xInterval[1] <= node->xInterval[1] && g[i]->xInterval[1] >= node->xInterval[0]);
//			bool testx2 = (node->xInterval[0] >= g[i]->xInterval[0] && node->xInterval[1] <= g[i]->xInterval[1]);
//
//			bool testy1 = (g[i]->yInterval[0] >= node->yInterval[0] && g[i]->yInterval[0] <= node->yInterval[1]) ||
//				(g[i]->yInterval[1] <= node->yInterval[1] && g[i]->yInterval[1] >= node->yInterval[0]);
//			bool testy2 = (node->yInterval[0] >= g[i]->yInterval[0] && node->yInterval[1] <= g[i]->yInterval[1]);
//
//			bool testz1 = (g[i]->zInterval[0] >= node->zInterval[0] && g[i]->zInterval[0] <= node->zInterval[1]) ||
//				(g[i]->zInterval[1] <= node->zInterval[1] && g[i]->zInterval[1] >= node->zInterval[0]);
//			bool testz2 = (node->zInterval[0] >= g[i]->zInterval[0] && node->zInterval[1] <= g[i]->zInterval[1]);
//
//
//
//			//if (testx1 || testx2)
//			//{
//				if (testy1 || testy2)
//				{
//					if (testz1 || testz2)
//						node->objects.push_back(g[i]);
//					
//						
//				}
//			//}
//
//		}
//	}
//
//
//	traverse(node, node->left, g);
//	traverse(node, node->right, g);
//
//
//
//
//}
//
//void KDTree::insertObject(vector<Geometry*> g)
//{
//	for (int i = 0; i < g.size(); i++)
//		root->objects.push_back(g[i]);
//	traverse(NULL, root, g);
//	cull(root);
//}
//
//void setLeftInterval(KNode* node, KNode* nextNode, int index)
//{
//	switch (index)
//	{
//	case 0:
//		nextNode->xInterval[0] = node->xInterval[0];
//		nextNode->xInterval[1] = floorl(((node->xInterval[1] + node->xInterval[0]) / 2.0) * 1000) / 1000;
//
//
//
//		nextNode->intIndex = 1;
//
//		nextNode->yInterval = node->yInterval;
//		nextNode->zInterval = node->zInterval;
//		break;
//	case 1:
//		nextNode->xInterval = node->xInterval;
//
//		nextNode->yInterval[0] = node->yInterval[0];
//		nextNode->yInterval[1] = floorl(((node->yInterval[1] + node->yInterval[0]) / 2.0) * 1000) / 1000;
//
//		nextNode->intIndex = 1;
//
//		nextNode->zInterval = node->zInterval;
//		break;
//	case 2:
//		nextNode->xInterval = node->xInterval;
//		nextNode->yInterval = node->yInterval;
//
//		nextNode->zInterval[0] = node->zInterval[0];
//		nextNode->zInterval[1] = floorl(((node->zInterval[1] + node->zInterval[0]) / 2.0) * 1000) / 1000;
//
//		nextNode->intIndex = 1;
//		break;
//	}
//
//
//}
//void setRightInterval(KNode* node, KNode* nextNode, int index)
//{
//	switch (index)
//	{
//	case 0:
//		nextNode->xInterval[0] = floorl(((node->xInterval[1] + node->xInterval[0]) / 2.0) * 1000) / 1000;
//
//		nextNode->xInterval[1] = node->xInterval[1];
//		nextNode->intIndex = 0;
//
//
//		nextNode->yInterval = node->yInterval;
//		nextNode->zInterval = node->zInterval;
//
//		break;
//	case 1:
//		nextNode->xInterval = node->xInterval;
//
//		nextNode->yInterval[0] = floorl(((node->yInterval[1] + node->yInterval[0]) / 2.0) * 1000) / 1000;
//
//		nextNode->yInterval[1] = node->yInterval[1];
//		nextNode->intIndex = 0;
//
//
//		nextNode->zInterval = node->zInterval;
//
//		break;
//	case 2:
//		nextNode->xInterval = node->xInterval;
//		nextNode->yInterval = node->yInterval;
//
//		nextNode->zInterval[0] = floorl(((node->zInterval[1] + node->zInterval[0]) / 2.0) * 1000) / 1000;
//		nextNode->zInterval[1] = node->zInterval[1];
//		nextNode->intIndex = 0;
//
//		break;
//	}
//
//
//}
//
//Eigen::Vector2d getInterval(KNode* node, int index)
//{
//
//	Eigen::Vector2d ret;
//	switch (index)
//	{
//	case 0:
//		ret = node->xInterval;
//		break;
//	case 1:
//		ret = node->yInterval;
//		break;
//		case 2:
//			ret = node->zInterval;
//			break;
//	}
//
//	return ret;
//}
//
//
//
