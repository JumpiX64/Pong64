TARGET = pong
BUILD_DIR = build

include $(N64_INST)/include/n64.mk

CFLAGS += -Isrc

SRC = $(wildcard src/*.c)
assets_png = $(wildcard assets/*.png)
assets_wav = $(wildcard assets/*.wav)
assets_conv = $(addprefix filesystem/,$(notdir $(assets_png:%.png=%.sprite)))
assets_conv += $(addprefix filesystem/,$(notdir $(assets_wav:%.wav=%.wav64)))

MKSPRITE_FLAGS ?=

MKSPRITE64 = --format RGBA16 --tiles 64,64
MKSPRITE32 = --format RGBA16 --tiles 32,32
MKSPRITE16 = --format RGBA16 --tiles 16,16

filesystem/pong1.sprite \
filesystem/pong2.sprite \
filesystem/pongf.sprite \
filesystem/lib.sprite: MKSPRITE_FLAGS = $(MKSPRITE64)

filesystem/pokal.sprite \
filesystem/endless.sprite: MKSPRITE_FLAGS = $(MKSPRITE32)

filesystem/ball.sprite \
filesystem/libdragon-font.sprite: MKSPRITE_FLAGS = $(MKSPRITE16)

AUDIOCONV_FLAGS ?=

AUDIOCONV = --wav-compress 1,bits=3 --wav-resample 22050 --wav-mono

filesystem/menu.wav64 \
filesystem/hit.wav64 \
filesystem/victory.wav64 \
filesystem/score.wav64: AUDIOCONV_FLAGS = $(AUDIOCONV)

all: $(TARGET).z64

filesystem/%.sprite: assets/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -o filesystem "$<"

filesystem/%.wav64: assets/%.wav
	@mkdir -p $(dir $@)
	@echo "    [AUDIO] $@"
	@$(N64_AUDIOCONV) $(AUDIOCONV_FLAGS) -o filesystem "$<"

$(BUILD_DIR)/$(TARGET).dfs: $(assets_conv)
	@mkdir -p $(BUILD_DIR)
	$(N64_ROOTDIR)/bin/mkdfs $@ filesystem

$(BUILD_DIR)/$(TARGET).elf: $(SRC:%.c=$(BUILD_DIR)/%.o)

$(TARGET).z64: N64_ROM_TITLE = "Pong 64"
$(TARGET).z64: $(BUILD_DIR)/$(TARGET).dfs

clean:
	rm -rf $(BUILD_DIR) filesystem *.z64

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean
