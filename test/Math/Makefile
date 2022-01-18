
# The source file where the main() function is

SOURCEMAIN = testIntersect.cc testTrfm.cc testLine.cc 

# Library files
SRC_DIR := ${MG_DIR}/Math
INCLUDE_DIR=-I ${TH_DIR} -I ${MG_DIR}/Math -I ${MG_DIR}/Misc
SRC = ${MG_DIR}/Misc/tools.cc ${MG_DIR}/Misc/constants.cc ${SRC_DIR}/vector3.cc ${SRC_DIR}/trfm3D.cc ${SRC_DIR}/plane.cc ${SRC_DIR}/line.cc ${SRC_DIR}/bbox.cc ${SRC_DIR}/intersect.cc ${SRC_DIR}/bsphere.cc

# Don't change anything below
DEBUG = 1

LIBS = -lm -lglut -lGLU -lGL -ljpeg -lGLEW

ifdef DEBUG
OPTFLAGS = -g
else
OPTFLAGS = -O2
endif

CCOPTIONS = -std=c++11 -Wall $(OPTFLAGS)
MEMBERS = $(SRC:.cc=.o)
EXEC  = $(basename $(notdir $(SOURCEMAIN)))

# all:
# 	@echo $(EXEC)
# 	@echo ${MEMBERS}

all: $(EXEC)

%.o : %.cc
	g++ -c -o $@ $(CCOPTIONS) $(INCLUDE_DIR) $< 

$(EXEC): $(JPEG_LIB) $(TARGET) $(MEMBERS) $(SOURCEMAIN)
	g++ $(CCOPTIONS) -o $@ $@.cc $(MEMBERS) $(INCLUDE_DIR) $(LIBDIR) $(LIBS)


$(JPEG_LIB):
	(cd $(JPEG_LIBDIR); ./configure --enable-static --disable-shared)
	(cd $(JPEG_LIBDIR); make)
	mv $(JPEG_LIBDIR)/.libs/libjpeg.a $(JPEG_LIBDIR)

.PHONY : all clean jpeglib_clean

clean:
	find . -type f -name '*.o' | xargs rm -f
	rm -f $(EXEC)

jpeglib_clean:
	(cd $(JPEG_LIBDIR); make clean)
