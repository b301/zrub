CC=gcc
LD=ld
CFLAGS=-I${PWD} -Wall -Wextra -pedantic -O3 -s

BUILD_DIR=build

TYPES_DIR=zrub/types
OS_DIR=zrub/os
CORE_DIR=zrub/core
CIPHER_DIR=zrub/cipher

TYPES_OBJS=$(BUILD_DIR)/string.o $(BUILD_DIR)/linked_list.o $(BUILD_DIR)/hashmap.o \
            $(BUILD_DIR)/vector.o $(BUILD_DIR)/queue.o $(BUILD_DIR)/iterator.o
OS_OBJS=$(BUILD_DIR)/time.o $(BUILD_DIR)/file.o
CORE_OBJS=$(BUILD_DIR)/logger.o
CIPHER_OBJS=$(BUILD_DIR)/cipher_classic.o

all: $(BUILD_DIR)/zrub.o

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR) $(BUILD_DIR)/tests $(BUILD_DIR)/bin/tests

# Compile types
$(BUILD_DIR)/string.o: $(TYPES_DIR)/string.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/linked_list.o: $(TYPES_DIR)/linked_list.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/hashmap.o: $(TYPES_DIR)/hashmap.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/vector.o: $(TYPES_DIR)/vector.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/queue.o: $(TYPES_DIR)/queue.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/iterator.o: $(TYPES_DIR)/iterator.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Combine types
$(BUILD_DIR)/zrub_types.o: $(TYPES_OBJS)
	$(LD) -relocatable $^ -o $@

# Compile OS
$(BUILD_DIR)/time.o: $(OS_DIR)/time.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/file.o: $(OS_DIR)/file.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Combine OS
$(BUILD_DIR)/os.o: $(OS_OBJS)
	$(LD) -relocatable $^ -o $@

# Compile core and cipher
$(BUILD_DIR)/logger.o: $(CORE_DIR)/logger.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/cipher_classic.o: $(CIPHER_DIR)/classic.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Combine all into zrub.o
$(BUILD_DIR)/zrub.o: $(BUILD_DIR)/zrub_types.o $(BUILD_DIR)/os.o $(CORE_OBJS) $(CIPHER_OBJS)
	$(LD) -relocatable $^ -o $@

clean:
	rm -rf $(BUILD_DIR)
