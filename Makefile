all: pong.z64
.PHONY: all clean

BUILD_DIR = build
ROMFS_DIR = filesystem

SRC = pong.c
OBJS = $(SRC:%.c=$(BUILD_DIR)/%.o)
DEPS = $(SRC:%.c=$(BUILD_DIR)/%.d)

ROMFS_FILES = $(ROMFS_DIR)/libdragon-font.sprite

include $(N64_INST)/include/n64.mk

$(ROMFS_DIR)/libdragon-font.sprite: $(ROMFS_DIR)/libdragon-font.png
	$(N64_ROOTDIR)/bin/mksprite -s 16x16 -b 8 $< $@

$(ROMFS_DIR)/ball.sprite: $(ROMFS_DIR)/ball.png
	$(N64_ROOTDIR)/bin/mksprite -s 16x16 -b 8 $< $@

$(ROMFS_DIR)/pong1.sprite: $(ROMFS_DIR)/pong1.png
	$(N64_ROOTDIR)/bin/mksprite -s 64x64 -b 8 $< $@

$(ROMFS_DIR)/pong2.sprite: $(ROMFS_DIR)/pong2.png
	$(N64_ROOTDIR)/bin/mksprite -s 64x64 -b 8 $< $@

$(ROMFS_DIR)/pongf.sprite: $(ROMFS_DIR)/pongf.png
	$(N64_ROOTDIR)/bin/mksprite -s 64x64 -b 8 $< $@

$(ROMFS_DIR)/pokal.sprite: $(ROMFS_DIR)/pokal.png
	$(N64_ROOTDIR)/bin/mksprite -s 32x32 -b 8 $< $@

$(ROMFS_DIR)/logol.sprite: $(ROMFS_DIR)/logol.png
	$(N64_ROOTDIR)/bin/mksprite -s 32x32 -b 8 $< $@

$(ROMFS_DIR)/endless.sprite: $(ROMFS_DIR)/endless.png
	$(N64_ROOTDIR)/bin/mksprite -s 32x32 -b 8 $< $@

$(BUILD_DIR)/pong.dfs: $(ROMFS_FILES)
	@mkdir -p $(BUILD_DIR)
	$(N64_ROOTDIR)/bin/mkdfs $@ $(ROMFS_DIR)

pong.z64: N64_ROM_TITLE = "pong"
pong.z64: $(BUILD_DIR)/pong.dfs

$(BUILD_DIR)/pong.elf: $(OBJS)

clean:
	rm -rf $(BUILD_DIR) *.z64

.PHONY: clean

-include $(DEPS)
-include $(wildcard $(BUILD_DIR)/*.d)




