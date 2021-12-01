# test_for_ViCueSoft
Тестовое задание для стажировки в компании ViCueSoft




команда для проигрывания видеофайла:
ffplay waterfall_cif.y4m

команда для сжатия видеофайла:
ffmpeg -i waterfall_cif.y4m -c:v libx264 -b:v 1M target_1000.mp4

команда для декодинга видеофайла:
ffmpeg -i target_1000.mp4 -f yuv4mpegpipe decoded_target_1000.y4m
