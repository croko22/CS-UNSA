CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -Iinclude

SRC_DIR = src
EXAMPLE_DIR = example
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

EXAMPLES := $(basename $(notdir $(wildcard $(EXAMPLE_DIR)/*.cpp)))

define make_example_rule
$(BIN_DIR)/$(1): $(OBJ_DIR)/$(1).o $(OBJ_DIR)/tensor.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $$@ $$^

endef

ALL_BINS = $(addprefix $(BIN_DIR)/, $(EXAMPLES))

all: $(ALL_BINS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(EXAMPLE_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(foreach example,$(EXAMPLES),$(eval $(call make_example_rule,$(example))))

clean:
	rm -rf $(BUILD_DIR)

run-%: $(BIN_DIR)/%
	./$<

.PHONY: all clean $(addprefix run-,$(EXAMPLES))
