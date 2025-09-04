/*
 * Copyright (c) 2014, 2017 The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef _FIH_COMMON_H
#define _FIH_COMMON_H

#undef USE_QC_HIFI_AMP
#undef USE_QC_SPK_AMP

#ifdef CONFIG_FIH_AOP
#define USE_QC_MBHC
#else
#undef	USE_QC_MBHC
#endif

#define AUDIO_HAC_SUPPORT
#define AUDIO_BYPASS_AFE_ADM_SUPPORT
#define USE_FIH_SPK_AMP
#define USE_FIH_HS_BTN_TUNING

//#undef pr_debug
//#define pr_debug pr_err

#include <linux/of_device.h>

enum {
    MI2S_PRI_RX = 0,
    MI2S_PRI_TX,
    MI2S_SEC_RX,
    MI2S_SEC_TX,
    MI2S_TERT_RX,
    MI2S_TERT_TX,
    MI2S_QUAT_RX,
    MI2S_QUAT_TX
};

typedef struct samp_codec_data {
    int mi2s_num;
    char prop_name[32];
    char codec_name[32];
    char codec_dai_name[32];
} samp_codec_data_t;

static const samp_codec_data_t samp_codec_data_list[] = {
    { .prop_name = "fih,ti-tas2557-aop",
      .mi2s_num = MI2S_QUAT_RX,
      .codec_name = "tas2557.3-004c",
      .codec_dai_name = "tas2557 ASI1" },
    { .prop_name = "fih,nxp-tfa9892-pm2",
      .mi2s_num = MI2S_QUAT_RX,
      .codec_name = "tfa98xx.4-0035",
      .codec_dai_name = "tfa98xx-aif-4-35" },
};

static void set_speaker_amp_codec_data(const struct device_node *np);

#endif
