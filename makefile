SRC_DIR := source
OBJ_DIR := build/obj
TARGET  := Particles

SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

INCLUDES := -I$(SRC_DIR)\
            -I$(SRC_DIR)/Core\
            -I$(SRC_DIR)/Engine\
            -I$(SRC_DIR)/Particle\
            -I$(SRC_DIR)/Player\
            -I$(SRC_DIR)/Bullet\
            -I$(SRC_DIR)/Bullet/Spawners\
            -I$(SRC_DIR)/Bullet/Attacks\
            -I$(SRC_DIR)/Sounds\
            -I$(SRC_DIR)/Textures\
            -I$(SRC_DIR)/UI

LDFLAGS  := -lsfml-graphics -lsfml-window -lsfml-system\
            -lsfml-audio -lsfml-network -lstdc++fs

CXXFLAGS := -g -Wall -fpermissive -std=c++17 $(INCLUDES)

$(TARGET): $(OBJS)
	g++ -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	g++ $(CXXFLAGS) -c -o $@ $<

release: CXXFLAGS += -O2 -DNDEBUG
release: $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf build $(TARGET)

.PHONY: release run clean
