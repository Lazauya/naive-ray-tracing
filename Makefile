CXX = g++
LD = g++

CXXFLAGS = -std=c++17 -c -g `libpng-config --cflags`
LDFLAGS = -std=c++17 -lpthread `libpng-config --ldflags`

render : main.o vec3d.o lights.o object.o shaders.o view.o color.o polynom.o scene.o bvh.o access.o mesh.o
	$(LD) $(LDFLAGS) main.o vec3d.o lights.o object.o shaders.o view.o color.o polynom.o scene.o access.o bvh.o mesh.o -o render
main.o : main.cpp scene.h object.h vec3d.h color.h view.h lights.h
	$(CXX) $(CXXFLAGS) main.cpp
vec3d.o : vec3d.h vec3d.cpp
	$(CXX) $(CXXFLAGS) vec3d.cpp
color.o : color.h color.cpp
	$(CXX) $(CXXFLAGS) color.cpp
lights.o : lights.h lights.cpp vec3d.h color.h
	$(CXX) $(CXXFLAGS) lights.cpp
polynom.o : polynom.cpp polynom.h
	$(CXX) $(CXXFLAGS) polynom.cpp
object.o : object.h object.cpp vec3d.h color.h polynom.h access.h bvh.h
	$(CXX) $(CXXFLAGS) object.cpp
view.o : view.h view.cpp vec3d.h
	$(CXX) $(CXXFLAGS) view.cpp
shaders.o : shaders.cpp shaders.h color.h vec3d.h object.h lights.h
	$(CXX) $(CXXFLAGS) shaders.cpp
scene.o : scene.h scene.cpp object.h vec3d.h color.h view.h lights.h access.h
	$(CXX) $(CXXFLAGS) scene.cpp
access.o : access.h access.cpp object.h vec3d.h
	$(CXX) $(CXXFLAGS) access.cpp
bvh.o : bvh.h bvh.cpp access.h vec3d.h object.h
	$(CXX) $(CXXFLAGS) bvh.cpp
mesh.o : mesh.h mesh.cpp bvh.h object.h
	$(CXX) $(CXXFLAGS) mesh.cpp
