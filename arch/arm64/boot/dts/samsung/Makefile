dtbo-$(CONFIG_ARCH_EXYNOS) += \
    a54x_eur_open_w00_r00.dtbo \
    a54x_eur_open_w00_r01.dtbo \
    a54x_eur_open_w00_r02.dtbo \
    a54x_eur_open_w00_r03.dtbo \
    a54x_eur_open_w00_r05.dtbo \
    a54x_eur_open_w00_r10.dtbo

targets += dtbs
DTB_LIST := $(dtb-y) $(dtbo-y)
always-y := $(DTB_LIST)

dtbs: $(addprefix $(obj)/, $(DTB_LIST))

clean-files := *.dtb*
