all:
	cargo build

# 'openocd' has to be running in the background
boot: all
	echo ramboot > /dev/tcp/localhost/4444

flash: all
	openocd -f openocd.cfg -c init -c romboot -c exit
