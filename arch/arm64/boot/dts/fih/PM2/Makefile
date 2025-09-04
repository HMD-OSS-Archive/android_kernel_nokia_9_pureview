ifeq ($(CONFIG_BUILD_ARM64_DT_OVERLAY),y)
	dtbo-$(CONFIG_ARCH_SDM845) += \
		sdm845-v2-mtp-overlay_lux-evb_naeu_nxp.dtbo \
		sdm845-v2-mtp-overlay_lux-evb_naeu_sony.dtbo \
		sdm845-v2-mtp-overlay_lux-pre-evt_naeu.dtbo \
		sdm845-v2-mtp-overlay_lux-pre-evt_cn.dtbo \
		sdm845-v2-mtp-overlay_lux-pre-evt_jp.dtbo \
		sdm845-v2-mtp-overlay_lux-pre-evt_ba.dtbo \
		sdm845-v2-mtp-overlay_lux-evt_naeu.dtbo \
		sdm845-v2-mtp-overlay_lux-evt_cn.dtbo \
		sdm845-v2-mtp-overlay_lux-evt_jp.dtbo \
		sdm845-v2-mtp-overlay_pm2-evb_naeu_nxp.dtbo \
		sdm845-v2-mtp-overlay_pm2-evb_naeu_sony.dtbo \
		sdm845-v2-mtp-overlay_pm2-pre-evt_naeu.dtbo \
		sdm845-v2-mtp-overlay_pm2-pre-evt_cn.dtbo \
		sdm845-v2-mtp-overlay_pm2-pre-evt_jp.dtbo \
		sdm845-v2-mtp-overlay_pm2-pre-evt_ba.dtbo \
		sdm845-v2-mtp-overlay_pm2-evt_naeu.dtbo \
		sdm845-v2-mtp-overlay_pm2-evt_cn.dtbo \
		sdm845-v2-mtp-overlay_pm2-evt_jp.dtbo

sdm845-v2-mtp-overlay_lux-evb_naeu_nxp.dtbo-base := sdm845-v2_fih.dtb
sdm845-v2-mtp-overlay_lux-evb_naeu_sony.dtbo-base := sdm845-v2_fih.dtb
sdm845-v2-mtp-overlay_lux-pre-evt_naeu.dtbo-base := sdm845-v2_fih.dtb
sdm845-v2-mtp-overlay_lux-pre-evt_cn.dtbo-base := sdm845-v2_fih.dtb
sdm845-v2-mtp-overlay_lux-pre-evt_jp.dtbo-base := sdm845-v2_fih.dtb
sdm845-v2-mtp-overlay_lux-pre-evt_ba.dtbo-base := sdm845-v2_fih.dtb
sdm845-v2-mtp-overlay_lux-evt_naeu.dtbo-base := sdm845-v2_fih.dtb
sdm845-v2-mtp-overlay_lux-evt_cn.dtbo-base := sdm845-v2_fih.dtb
sdm845-v2-mtp-overlay_lux-evt_jp.dtbo-base := sdm845-v2_fih.dtb
sdm845-v2-mtp-overlay_pm2-evb_naeu_nxp.dtbo-base := sdm845-v2_fih.dtb
sdm845-v2-mtp-overlay_pm2-evb_naeu_sony.dtbo-base := sdm845-v2_fih.dtb
sdm845-v2-mtp-overlay_pm2-pre-evt_naeu.dtbo-base := sdm845-v2_fih.dtb
sdm845-v2-mtp-overlay_pm2-pre-evt_cn.dtbo-base := sdm845-v2_fih.dtb
sdm845-v2-mtp-overlay_pm2-pre-evt_jp.dtbo-base := sdm845-v2_fih.dtb
sdm845-v2-mtp-overlay_pm2-pre-evt_ba.dtbo-base := sdm845-v2_fih.dtb
sdm845-v2-mtp-overlay_pm2-evt_naeu.dtbo-base := sdm845-v2_fih.dtb
sdm845-v2-mtp-overlay_pm2-evt_cn.dtbo-base := sdm845-v2_fih.dtb
sdm845-v2-mtp-overlay_pm2-evt_jp.dtbo-base := sdm845-v2_fih.dtb
endif

always		:= $(dtb-y)
subdir-y	:= $(dts-dirs)
clean-files	:= *.dtb
