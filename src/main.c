#include <genesis.h>

#define MAX_COLORS 6
#define MAX_STRIPES 7

typedef enum {
  kFlag_Rainbow,

  kFlag_Aromantic,
  kFlag_Asexual,
  kFlag_Bigender,
  kFlag_Bisexual,
  kFlag_Genderfluid,
  kFlag_Genderqueer,
  kFlag_Lesbian,
  kFlag_NonBinary,
  kFlag_Pansexual,
  kFlag_Transgender,

  kFlag_Count,
} Flag;

const u16 kFlagPalettes[kFlag_Count][MAX_COLORS] = {
    [kFlag_Rainbow] = {RGB24_TO_VDPCOLOR(0xE40303), RGB24_TO_VDPCOLOR(0xFF8C00),
                       RGB24_TO_VDPCOLOR(0xFFED00), RGB24_TO_VDPCOLOR(0x008026),
                       RGB24_TO_VDPCOLOR(0x004DFF), RGB24_TO_VDPCOLOR(0x750787)},
    [kFlag_Aromantic] = {RGB24_TO_VDPCOLOR(0x3DA542), RGB24_TO_VDPCOLOR(0xA7D379),
                         RGB24_TO_VDPCOLOR(0xFFFFFF), RGB24_TO_VDPCOLOR(0xA9A9A9),
                         RGB24_TO_VDPCOLOR(0x000000)},
    [kFlag_Asexual] = {RGB24_TO_VDPCOLOR(0x000000), RGB24_TO_VDPCOLOR(0xA3A3A3),
                       RGB24_TO_VDPCOLOR(0xFFFFFF), RGB24_TO_VDPCOLOR(0x800080)},
    [kFlag_Bigender] = {RGB24_TO_VDPCOLOR(0xC479A2), RGB24_TO_VDPCOLOR(0xEDA5CD),
                        RGB24_TO_VDPCOLOR(0xD5C7E8), RGB24_TO_VDPCOLOR(0xFFFFFF),
                        RGB24_TO_VDPCOLOR(0x9AC7E8), RGB24_TO_VDPCOLOR(0x6D82D1)},
    [kFlag_Bisexual] = {RGB24_TO_VDPCOLOR(0xD60270), RGB24_TO_VDPCOLOR(0x9B4F96),
                        RGB24_TO_VDPCOLOR(0x0038A8)},
    [kFlag_Genderfluid] = {RGB24_TO_VDPCOLOR(0xFF75A2), RGB24_TO_VDPCOLOR(0xFFFFFF),
                           RGB24_TO_VDPCOLOR(0xBE18D6), RGB24_TO_VDPCOLOR(0x000000),
                           RGB24_TO_VDPCOLOR(0x333EBD)},
    [kFlag_Genderqueer] = {RGB24_TO_VDPCOLOR(0xB57EDC), RGB24_TO_VDPCOLOR(0xFFFFFF),
                           RGB24_TO_VDPCOLOR(0x4A8123)},
    [kFlag_Lesbian] = {RGB24_TO_VDPCOLOR(0xD52D00), RGB24_TO_VDPCOLOR(0xFF9A56),
                       RGB24_TO_VDPCOLOR(0xFFFFFF), RGB24_TO_VDPCOLOR(0xD362A4),
                       RGB24_TO_VDPCOLOR(0xA30262)},
    [kFlag_NonBinary] = {RGB24_TO_VDPCOLOR(0xFCF434), RGB24_TO_VDPCOLOR(0xFCFCFC),
                         RGB24_TO_VDPCOLOR(0x9C59D1), RGB24_TO_VDPCOLOR(0x2C2C2C)},
    [kFlag_Pansexual] = {RGB24_TO_VDPCOLOR(0xFF218C), RGB24_TO_VDPCOLOR(0xFFD800),
                         RGB24_TO_VDPCOLOR(0x21B1FF)},
    [kFlag_Transgender] = {RGB24_TO_VDPCOLOR(0x5BCEFA), RGB24_TO_VDPCOLOR(0xF5A9B8),
                           RGB24_TO_VDPCOLOR(0xFFFFFF)},
};

// Array of flags to the palette color indexes for each stripe of the flag.
const u8 kFlagColors[kFlag_Count][MAX_STRIPES + 1] = {
    [kFlag_Rainbow] = {0, 1, 2, 3, 4, 5, MAX_COLORS},
    [kFlag_Aromantic] = {0, 1, 2, 3, 4, MAX_COLORS},
    [kFlag_Asexual] = {0, 1, 2, 3, MAX_COLORS},
    [kFlag_Bigender] = {0, 1, 2, 3, 2, 4, 5, MAX_COLORS},
    [kFlag_Bisexual] = {0, 0, 1, 2, 2, MAX_COLORS},
    [kFlag_Genderfluid] = {0, 1, 2, 3, 4, MAX_COLORS},
    [kFlag_Genderqueer] = {0, 1, 2, MAX_COLORS},
    [kFlag_Lesbian] = {0, 1, 2, 3, 4, MAX_COLORS},
    [kFlag_NonBinary] = {0, 1, 2, 3, MAX_COLORS},
    [kFlag_Pansexual] = {0, 1, 2, MAX_COLORS},
    [kFlag_Transgender] = {0, 1, 2, 1, 0, MAX_COLORS},
};

int CountFlagStripes(Flag flag) {
  int stripes;
  for (stripes = 0; kFlagColors[flag][stripes] < MAX_COLORS; stripes++)
    continue;
  return stripes;
}

int UseDarkTextWithFlag(Flag flag) {
  switch (flag) {
  case kFlag_NonBinary:
  case kFlag_Transgender:
    return 1;
  default:
    return 0;
  }
}

const char *DescribeFlag(Flag flag) {
  switch (flag) {

#define FLAG_DESCR(x)                                                                    \
  case kFlag_##x:                                                                        \
    return #x;

    FLAG_DESCR(Rainbow)
    FLAG_DESCR(Aromantic)
    FLAG_DESCR(Asexual)
    FLAG_DESCR(Bigender)
    FLAG_DESCR(Bisexual)
    FLAG_DESCR(Genderfluid)
    FLAG_DESCR(Genderqueer)
    FLAG_DESCR(Lesbian)
    FLAG_DESCR(NonBinary)
    FLAG_DESCR(Pansexual)
    FLAG_DESCR(Transgender)

#undef FLAG_DESCR

  case kFlag_Count:
    break;
  }

  return "???";
}

Flag g_current_flag = kFlag_Rainbow;

void UploadSplitTile(u8 tile_index, u8 color1, u8 color2, u8 height_before_split) {
  u8 tile[32] = {0};

  for (int i = 0; i < sizeof(tile); i++) {
    int line = i / 4;
    u8 color = line < height_before_split ? color1 : color2;
    tile[i] = (color << 4) | color;
  }

  DMA_transfer(DMA, DMA_VRAM, tile, tile_index * 32, 16, 2);
}

void DrawCurrentFlag() {
  const int kTileSize = 8;

  PAL_setColors(0, kFlagPalettes[g_current_flag], MAX_COLORS);

  int stripes = CountFlagStripes(g_current_flag);
  int stripe_height_px = VDP_getScreenHeight() / stripes;

  int current_stripe = 0;
  int current_stripe_height = 0;

  bool needs_upload = TRUE;

  const u8 *flag_colors = kFlagColors[g_current_flag];

  for (int i = 0; i < VDP_getPlaneHeight(); i++) {
    u8 split_at = 0;

    if (current_stripe_height + kTileSize > stripe_height_px &&
        current_stripe + 1 < stripes) {
      needs_upload = TRUE;

      split_at = stripe_height_px - current_stripe_height;
      current_stripe_height = 0;

      if (split_at == 0) {
        // If this isn't split, move onto the next stripe right now.
        current_stripe++;
      }
    }

    u8 tile_index = current_stripe * 2 + (split_at != 0);

    if (needs_upload) {
      u8 color1 = flag_colors[current_stripe];
      u8 color2 = split_at != 0 ? flag_colors[++current_stripe] : color1;

      UploadSplitTile(tile_index, color1, color2, split_at);

      needs_upload = FALSE;
    }

    VDP_fillTileMapRect(BG_A, tile_index, 0, i, VDP_getPlaneWidth(), 1);

    if (split_at != 0) {
      // This tile was filled with split colors, so the next one should
      // definitely be a new upload.
      needs_upload = TRUE;
    }

    current_stripe_height += kTileSize - split_at;
  }

  PAL_setPalette(VDP_getTextPalette(), UseDarkTextWithFlag(g_current_flag)
                                           ? palette_black
                                           : font_pal_default.data);
  const char *description = DescribeFlag(g_current_flag);
  VDP_drawText(description, VDP_getScreenWidth() / kTileSize - strlen(description) - 1,
               1);
}

void SetFlag(Flag flag) {
  g_current_flag = flag;
  DrawCurrentFlag();
}

void NextFlag() {
  SetFlag((g_current_flag + 1) % kFlag_Count);
  DrawCurrentFlag();
}

void PreviousFlag() {
  SetFlag(g_current_flag != 0 ? (g_current_flag - 1) % kFlag_Count : kFlag_Count - 1);
}

void InputHandler(u16 joy, u16 changed, u16 state) {
  if (joy == JOY_1) {
    u16 new_presses = changed & state;

    if (new_presses & BUTTON_LEFT) {
      PreviousFlag();
    } else if (new_presses & BUTTON_RIGHT) {
      NextFlag();
    }
  }
}

int main() {
  VDP_setTextPalette(PAL1);

  JOY_init();
  JOY_setEventHandler(InputHandler);

  SetFlag(kFlag_Rainbow);

  for (;;) {
    SYS_doVBlankProcess();
  }

  return 0;
}
