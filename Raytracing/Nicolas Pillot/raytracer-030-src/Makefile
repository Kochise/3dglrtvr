
RTOBJ = base3d.o boundedobject.o color.o conicspotlight.o diffuselight.o \
	ellipsoid.o fileexception.o image.o instancecounter.o \
	intersectiondata.o intersections.o invalidparameterexception.o \
	light.o lightraytracer.o logconsole.o log.o material.o \
	mathexception.o matrix3d.o movable.o myexception.o notfoundexception.o \
	nullpointerexception.o object.o point3d.o polynom.o printable.o ray.o \
	raytracer.o renderer.o row3d.o scene.o simplestraytracer.o \
	simplestrenderer.o solidcolor.o sphere.o spotlight.o surfacedata.o \
	syntaxexception.o unboundedobject.o utils.o vector3d.o view.o

TARGET = raytracer

$(TARGET) : main.o libraytracer.a
	g++ -Wall -o $@ $+

run : $(TARGET)
	./$(TARGET)

libraytracer.a : $(RTOBJ)
	ar -r $@ $+

main.o : main.cpp
	g++ -Wall -c $+

%.o : %.cpp %.h
	g++ -Wall -c $<

clean :
	rm -f $(RTOBJ) $(TARGET)

checkout :
	cvs -z3 -d:ext:netsach@cvs.sourceforge.net:/cvsroot/raytracer checkout .
	
update :
	cvs -z3 -d:ext:netsach@cvs.sourceforge.net:/cvsroot/raytracer update .
	
commit :
	cvs -z3 -d:ext:netsach@cvs.sourceforge.net:/cvsroot/raytracer commit

