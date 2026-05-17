SRC_DIR := source
OBJ_DIR := build/obj
TARGET  := Particles

SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

INCLUDES := -Isource\
            -Isource/Core\
            -Isource/Engine\
            -Isource/Particle\
            -Isource/Player\
            -Isource/Bullet\
            -Isource/Bullet/Spawners\
            -Isource/Bullet/Attacks\
            -Isource/Sounds\
            -Isource/Textures\
            -Isource/UI\
            -Isource/Bullet/..

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
