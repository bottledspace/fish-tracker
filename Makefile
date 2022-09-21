CXXFLAGS += -I/usr/local/Cellar/libsvm/3.24/include/
CXXFLAGS += -I/usr/local/Cellar/opencv/4.3.0_5/include/opencv4
CXXFLAGS += -I./include
LDFLAGS += -L/usr/local/Cellar/libsvm/3.24/lib/
LDFLAGS += -L/usr/local/Cellar/opencv/4.3.0_5/lib/
LDFLAGS += -lopencv_core -lopencv_highgui -lopencv_imgproc
LDFLAGS += -lopencv_imgcodecs -lopencv_shape -lopencv_ml
LDFLAGS += -lopencv_ml -lopencv_objdetect -lopencv_videoio
LDFLAGS += -lsvm

mkhogs : mkhogs.cc
	c++ -O2 --std=c++17 -o $@ $(CXXFLAGS) $< $(LDFLAGS)
fishcls : fishcls.cc
	c++ -O2 --std=c++17 -o $@ $(CXXFLAGS) $< $(LDFLAGS)