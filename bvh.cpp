#include "bvh.h"

#include <iostream>
#include <algorithm>
#include <stack>

using std::cout;
using std::endl;

namespace bvh_n
{
	bvh::bvh(std::list<objects::object*> & objs, UInt64 no) : numObjs(no)
	{
		std::vector<objects::object*> tempVec;
		std::vector<objects::object**> tempVec_;
		boundless = std::vector<objects::object*>();
		for(auto & o : objs)
		{	
			point * b = o->bounds();
			if(b[0].x != INVALID_COORDINATE)
			{
				tempVec.push_back(o);
			}
			else
			{
				boundless.push_back(o);
			}
			delete[] b;
		}
		heir = std::vector<volume>(tempVec.size()*2);
		std::sort(tempVec.begin(), tempVec.end(), [](objects::object* o1, objects::object * o2) 
		{ 
			auto b1 = o1->bounds();
			auto b2 = o2->bounds();
			auto res = ((b1[0]+b1[1])*.5).x < ((b2[0]+b2[1])*.5).x;
			delete[] b1;
			delete[] b2;
			return res;
		} );
		UInt64 i = 0;
		while(i < tempVec.size())
		{
			objects::object ** objectArr = new objects::object*[numObjs];

			for(UInt64 j = 0; j < numObjs; j++)
				objectArr[j] = nullptr;

			for(UInt64 j = i; j < std::min(i+numObjs, tempVec.size()); j++)
			{
				objectArr[j-i] = tempVec[j];
			}
			
			tempVec_.push_back(objectArr);

			i += numObjs;
		}

		buildHeir(tempVec_, 1, 0, tempVec_.size());
		//std::cout << "index out: " << heir[1]->index << std:endl;
	}
	
	/*bvh::bvh(std::list<objects::triangle*> * objs)
	{	
		std::vector<objects::object*> tempVec;
		boundless = std::vector<objects::object*>();
		for(auto & o : *objs)
		{	
			point * b = o->bounds();
			if(b[0].x != INVALID_COORDINATE)
			{
				tempVec.push_back(o);
			}
			else
			{
				boundless.push_back(o);
			}
			delete[] b;
			
		}
		std::sort(tempVec.begin(), tempVec.end(), [](objects::object* o1, objects::object * o2) 
		{ 
			auto b1 = o1->bounds();
			auto b2 = o2->bounds();
			auto res = ((b1[0]+b1[1])*.5).x < ((b2[0]+b2[1])*.5).x;
			delete[] b1;
			delete[] b2;
			return res;
		} );
		heir = std::vector<volume*>(tempVec.size()*2);	
		buildHeir(tempVec, 1, 0, tempVec.size());
	}*/
	
	void bvh::buildHeir(std::vector<objects::object**> & objs, SInt64 index, SInt64 objsBeg, SInt64 objsEnd)
	{
		if(objsBeg >= objsEnd)
		{
			return;
		}

		point min(INVALID_COORDINATE,0,0);
		point max(INVALID_COORDINATE,0,0);
		for(UInt64 i = objsBeg; i < objsEnd; i++)
		{
			for(UInt64 j = 0; j < numObjs; j++)
			{
				if(objs[i][j])
				{
					//std::cout << objs[i][j] << std::endl;
					point * b = objs[i][j]->bounds();
					if(min.x == INVALID_COORDINATE)
					{
						max = b[0];
						min = b[1];
					}
					else
					{	
						max.x = std::max(b[0].x, max.x);
						max.y = std::max(b[0].y, max.y);
						max.z = std::max(b[0].z, max.z);
						min.x = std::min(b[1].x, min.x);
						min.y = std::min(b[1].y, min.y);
						min.z = std::min(b[1].z, min.z);
					}
					delete[] b;
				}
			}
		}
		
		SInt64 mid = (objsBeg+objsEnd)/2;
		//std::cout << "index: " << index << " mid: " << mid << std::endl;
		if(objs[mid][0])
		{
			//std::cout << "index: " << index << " mid: " << mid << std::endl;
			heir[index] = volume(max, min, numObjs, objs[mid], index);	
		}
		else
		{
			return;
		}
		buildHeir(objs, 2*index, objsBeg, mid);
		buildHeir(objs, 2*index+1, mid+1, objsEnd);	
		/*objects::object ** objectList = new objects::object*[numObjs];
		for(UInt64 i = std::max(mid-numObjs/2, 0); i < std::min(mid+numObjs/2, objs.size()); i)
		{
			objectList[i] = objs
		}*/
	}
	
	objects::object * bvh::closestIntersection_(ray3d r) const
	{	
		point cp(INVALID_COORDINATE, 0, 0); //closest point
		objects::object * co = nullptr; //closest object

		std::stack<volume> toVisit;
		//std::cout << "hs" << heir.size() << std::endl;
		//std::cout << "pusheroni" << std::endl;
		toVisit.push(heir[1]);
		//std::cout << "pusherella" << std::endl;
		
		UInt64 index = 1;

		while(toVisit.size())
		{
			volume top = toVisit.top();
			toVisit.pop();
			if(top.numObjs && top.intersects(r))
			{	
				//std::cout << top.index << std::endl;
				for(UInt64 o = 0; o < numObjs; o++)
				{
				       	if(top.obj[o])
					{
						point ip = top.obj[o]->intersect(r);	
						if(ip.x != INVALID_COORDINATE && ip.distSqr(r.start) < cp.distSqr(r.start))
						{
							cp = ip;
							co = top.obj[o];
						}
					}
				}
				
				if(top.index*2 < heir.size())
				{
					//std::cout << heir.size() << " push 2 " <<  std::endl;
					toVisit.push(heir[top.index*2]);
					
					if(top.index*2+1 < heir.size())
					{
						//std::cout << "push 3 " << heir[top.index*2+1].index << std::endl;
						toVisit.push(heir[top.index*2+1]);
					}
				}
			}
			
		}

		return co;
	}

	objects::object * bvh::closestIntersection(ray3d r) const
	{
		objects::object * closest = nullptr;
		for(int i = 0; i < boundless.size(); i++)
		{
			point inters = boundless[i]->intersect(r);	
			if(closest)
			{
				point close = closest->intersect(r);
				closest = inters.x == INVALID_COORDINATE ? closest : close.distSqr(r.start) < inters.distSqr(r.start) ? closest : boundless[i];
			}
			else
			{
				closest = boundless[i]->intersect(r).x != INVALID_COORDINATE ? boundless[i] : closest;
			}
		}
		auto poss = closestIntersection_(r);
		//std::cout << "stop" << std::endl;	
		if(closest && poss)
		{
			return closest->intersect(r).distSqr(r.start) < poss->intersect(r).distSqr(r.start) ? closest : poss;
		}
		else if(poss)
		{
			return poss;
		}
		else if(closest)
		{
			return closest;
		}
		
		return nullptr;
	}

	objects::object * bvh::closestWithOmit_(ray3d r, objects::object * obj) const
	{	
		point cp(INVALID_COORDINATE, 0, 0); //closest point
		objects::object * co = nullptr; //closest object
		
		std::stack<volume> toVisit;	
		toVisit.push(heir[1]);
		
		UInt64 index = 1;

		while(toVisit.size())
		{
			volume top = toVisit.top();
			
			if(top.numObjs && top.intersects(r))
			{		
				for(UInt64 o = 0; o < numObjs; o++)
				{
					if(top.obj[o])
					{
						point ip = top.obj[o]->intersect(r); 
						if(ip.x != INVALID_COORDINATE && ip.distSqr(r.start) < cp.distSqr(r.start) && top.obj[o] != obj)
						{
							cp = ip;
							co = top.obj[o];
						}
					}
				}
	
				if(top.index*2 < heir.size())
				{
					toVisit.push(heir[top.index*2]);
					
					if(top.index*2+1 < heir.size())
					{
						toVisit.push(heir[top.index*2+1]);
					}
				}
			}
			toVisit.pop();
		}

		return co;
	}

	objects::object * bvh::closestWithOmit(ray3d r, objects::object * obj) const
	{
		objects::object * closest = nullptr;
		for(int i = 0; i < boundless.size(); i++)
		{
			if(boundless[i] != obj)
			{
				point inters = boundless[i]->intersect(r);	
				if(closest)
				{
					point close = closest->intersect(r);
					closest = inters.x == INVALID_COORDINATE ? closest : close.distSqr(r.start) < inters.distSqr(r.start) ? closest : boundless[i];
				}
				else
				{
					closest = boundless[i]->intersect(r).x != INVALID_COORDINATE ? boundless[i] : closest;
				}
			}
		}
		
		objects::object * poss = closestWithOmit_(r, obj);
		
		if(closest && poss)
		{
			return closest->intersect(r).distSqr(r.start) < poss->intersect(r).distSqr(r.start) ? closest : poss;
		}
		else if(poss)
		{
			return poss;
		}
		else if(closest)
		{
			return closest;
		}

		return nullptr;
	}
}
