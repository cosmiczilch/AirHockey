#

CXX = g++
CXXFLAGS = -g -Wall -lGLU -lGLEW -lGL `sdl-config --cflags` `sdl-config --libs` -lSDL_net -lSDL_image

main: main.o models.o bat_models.o puck_models.o Transformation.o ctexture.o cgame.o cboard.o cbat.o cpuck.o cplayer.o ccamera.o cbutton.o ctextinput.o cfontprinter.o cpanelobjek.o cpanel.o clabel.o
	$(CXX) $(CXXFLAGS) -o main main.o models.o bat_models.o puck_models.o Transformation.o ctexture.o cgame.o cboard.o cbat.o cpuck.o cplayer.o ccamera.o cbutton.o ctextinput.o cfontprinter.o cpanelobjek.o cpanel.o clabel.o

main.o: main.cpp models.h bat_models.h puck_models.h Transformation.h ccamera.h ctexture.h cboard.h cplayer.h cpuck.h cgame.h game_mainmenu.h game_running.h poller.h main.h globalDefs.h cbutton.h network_server.h network_client.h ctextinput.h cfontprinter.h Utilities.h cpanelobjek.h cpanel.h clabel.h cpacketdata.h
	$(CXX) $(CXXFLAGS) -c main.cpp

models.o: models.cpp models.h
	$(CXX) $(CXXFLAGS) -c models.cpp

bat_models.o: bat_models.cpp bat_models.h
	$(CXX) $(CXXFLAGS) -c bat_models.cpp

puck_models.o: puck_models.cpp puck_models.h
	$(CXX) $(CXXFLAGS) -c puck_models.cpp

Transformation.o: Transformation.cpp Transformation.h
	$(CXX) $(CXXFLAGS) -c Transformation.cpp

ctexture.o: ctexture.cpp ctexture.h
	$(CXX) $(CXXFLAGS) -c ctexture.cpp

cgame.o: cgame.cpp cgame.h Transformation.h ccamera.h
	$(CXX) $(CXXFLAGS) -c cgame.cpp

cboard.o: cboard.cpp cboard.h models.h ctexture.h
	$(CXX) $(CXXFLAGS) -c cboard.cpp

cbat.o: cbat.cpp cbat.h bat_models.h cboard.h globalDefs.h
	$(CXX) $(CXXFLAGS) -c cbat.cpp

cpuck.o: cpuck.cpp cpuck.h puck_models.h
	$(CXX) $(CXXFLAGS) -c cpuck.cpp

cplayer.o: cplayer.cpp cplayer.h cbat.h globalDefs.h
	$(CXX) $(CXXFLAGS) -c cplayer.cpp

ccamera.o: ccamera.cpp ccamera.h Transformation.h
	$(CXX) $(CXXFLAGS) -c ccamera.cpp

cbutton.o: cbutton.cpp cbutton.h ctexture.h
	$(CXX) $(CXXFLAGS) -c cbutton.cpp

ctextinput.o: ctextinput.cpp ctextinput.h ctexture.h cfontprinter.h cpanelobjek.h Utilities.h
	$(CXX) $(CXXFLAGS) -c ctextinput.cpp

cfontprinter.o: cfontprinter.cpp cfontprinter.h Utilities.h ctexture.h
	$(CXX) $(CXXFLAGS) -c cfontprinter.cpp

cpanelobjek.o: cpanelobjek.cpp cpanelobjek.h Utilities.h ctexture.h
	$(CXX) $(CXXFLAGS) -c cpanelobjek.cpp

cpanel.o: cpanel.cpp cpanel.h Utilities.h ctexture.h cpanelobjek.h
	$(CXX) $(CXXFLAGS) -c cpanel.cpp

clabel.o: clabel.cpp clabel.h Utilities.h ctexture.h cpanelobjek.h cfontprinter.h
	$(CXX) $(CXXFLAGS) -c clabel.cpp


.PHONY : clean
clean :
	-rm main *.o




# cbat.o cbutton.o 
