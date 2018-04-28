#include "mesh.h"
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <fstream>
#include <iostream>

namespace objects
{
	mesh::mesh(std::list<object*> & f, rgbColor col, F64 sh, rgbColor s) : object(col, sh, s)
       	{
		point * b;
		heir = bvh_n::bvh(f, 6);
		for(auto & o : f)
		{
			b = o->bounds();
			b1.x = std::max(b1.x, b[0].x); b1.y = std::max(b1.y, b[0].y); b1.z = std::max(b1.z, b[0].z);
			b2.x = std::min(b2.x, b[1].x); b2.y = std::min(b2.y, b[1].y); b2.z = std::min(b2.z, b[1].z);
			delete b;
		}
       	}
	
	UInt64 getVal(std::string s, UInt64 pos, char delim)
	{
		std::string l;
		std::vector<std::string> tokens;
		std::stringstream buf(s);
		while(getline(buf, l, delim))
		{
			tokens.push_back(l);
		}
		return std::stoi(tokens[pos]);
	}

	mesh::mesh(std::string filename, rgbColor col, F64 sh, rgbColor s) : object(col, sh, s)
	{
		std::fstream o(filename);
		std::string line;
		UInt64 line_num = 0;
		UInt64 vertices_num = 0;
		UInt64 faces_num = 0;
		std::vector<point> vertices;
		std::vector<object*> faces;
		bool hasNotVisited = true;
		
		//for bounds, calculated at creation
		point ma(0,0,0);
		point mi(0,0,0);
		
		std::string ext = filename.substr(filename.find_last_of(".")+1);

		if(ext == "off")
		{
			while(getline(o, line))
			{
				std::istringstream buf(line);
				std::istream_iterator<std::string> beg(buf), end;
				std::vector<std::string> tokens(beg, end);
					
				if(line[0] == '\n' || line[0] == '#')
					continue;
	
				if(line_num == 0 && hasNotVisited)
				{
					vertices_num = std::stoi(tokens[0]);
					faces_num = std::stoi(tokens[1]);
					hasNotVisited = true;
					continue;
				}
				else if(line_num <= vertices_num)
				{
					vertices.push_back(point(std::stod(tokens[0]), std::stod(tokens[1]), std::stod(tokens[2])));
					ma.x = std::max(vertices.back().x, ma.x); ma.y = std::max(vertices.back().y, ma.y); ma.z = std::max(vertices.back().z, ma.z);
					mi.x = std::min(vertices.back().x, mi.x); mi.y = std::min(vertices.back().y, mi.y); mi.z = std::min(vertices.back().z, mi.z);
				}
				else if(line_num <= vertices_num + faces_num) //it's not a vertex, it's a face
				{
					for(int i = 1; i < std::stoi(tokens[0])-1; i++)
					{
						faces.push_back(new triangle(vertices[std::stoi(tokens[1])],
									vertices[std::stoi(tokens[i+1])],
									vertices[std::stoi(tokens[i+2])], col, sh, s));
					}
				}
				
				line_num++;
			}
		}
		else if(ext == "obj")
		{
			while(getline(o, line))
			{
				//std::cout << "hi" << std::endl;
				std::istringstream buf(line);
				std::istream_iterator<std::string> beg(buf), end;
				std::vector<std::string> tokens(beg, end);
				
				if(tokens.size() < 2)
					continue;
				switch(line[0])
				{
				//case '#':
				//	continue;
				//case '\n':
				//	continue;
				case 'v':
					if(tokens[0] == "v")
					{
						vertices.push_back(point(std::stod(tokens[1]), std::stod(tokens[2]), std::stod(tokens[3])));
						ma.x = std::max(vertices.back().x, ma.x); ma.y = std::max(vertices.back().y, ma.y); ma.z = std::max(vertices.back().z, ma.z);
						mi.x = std::min(vertices.back().x, mi.x); mi.y = std::min(vertices.back().y, mi.y); mi.z = std::min(vertices.back().z, mi.z);
						break;
					}
					else
					{
						continue;
					}
				case 'f':
					//std::cout << "f" << std::endl;
					for(int i = 1; i < tokens.size()-2; i++)
					{
						//std::cout << getVal(tokens[1], 0, '/')-1 << std::endl;	
						faces.push_back(new triangle(vertices[getVal(tokens[1], 0, '/')-1],
									vertices[getVal(tokens[i+1], 0, '/')-1],
									vertices[getVal(tokens[i+2], 0, '/')-1], col, sh, s));
					}
					break;
				default:
					//std::cout << "why" << std::endl;
					continue;
				}
			}
		}
		std::list<object*> temp(faces.begin(), faces.end());
		heir = bvh_n::bvh(temp, 6);
		b1 = ma;
		b2 = mi;
	}

	point mesh::intersect(ray3d r) const
	{
		object * o = heir(r);
		if(o)
			return o->intersect(r);
		else
			return point(INVALID_COORDINATE, 0, 0);
	}

	vec3d mesh::normal(ray3d r, point p, point t) const
	{
		return heir(r)->normal(r, p, t);	
	}

	point * mesh::bounds() const
	{
		point * p = new point[2];
		p[0] = b1;
		p[1] = b2;
		return p;
	}
}
