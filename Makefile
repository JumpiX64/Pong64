BUILD_DIR = build
include $(N64_INST)/include/n64.mk

SRC = pong.c
assets_png = $(wildcard assets/*.png)
assets_wav = $(wildcard assets/*.wav)

assets_conv = $(addprefix filesystem/,$(notdir $(assets_png:%.png=%.sprite)))
assets_conv += $(addprefix filesystem/,$(notdir $(assets_wav:%.wav=%.wav64)))

MKSPRITE_FLAGS ?=
AUDIOCONV_FLAGS ?=

all: pong.z64

filesystem/%.sprite: assets/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -o filesystem "$<"

filesystem/%.wav64: assets/%.wav
	@mkdir -p $(dir $@)
	@echo "    [AUDIO] $@"
	@$(N64_AUDIOCONV) $(AUDIOCONV_FLAGS) -o filesystem "$<"

filesystem/pong1.sprite filesystem/pong2.sprite filesystem/pongf.sprite filesystem/lib.sprite: MKSPRITE_FLAGS=--format RGBA16 --tiles 64,64
filesystem/pokal.sprite:  MKSPRITE_FLAGS=--format RGBA16 --tiles 32,32
filesystem/ball.sprite:  MKSPRITE_FLAGS=--format RGBA16 --tiles 16,16
filesystem/libdragon-font.sprite: MKSPRITE_FLAGS=--format RGBA16 --tiles 16,16

$(BUILD_DIR)/pong.dfs: $(assets_conv)
	@mkdir -p $(BUILD_DIR)
	$(N64_ROOTDIR)/bin/mkdfs $@ filesystem

$(BUILD_DIR)/pong.elf: $(SRC:%.c=$(BUILD_DIR)/%.o)

pong.z64: N64_ROM_TITLE="Pong"
pong.z64: $(BUILD_DIR)/pong.dfs

clean:
	rm -rf $(BUILD_DIR) filesystem *.z64

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean
