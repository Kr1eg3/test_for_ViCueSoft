# test_for_ViCueSoft
Тестовое задание для стажировки в компании ViCueSoft

## ТЗ
Создать приложение для расчёта метрики PSNR.

## Метрика PSNR
Пиковое отношение сигнала к шуму (англ. peak signal-to-noise ratio) обозначается аббревиатурой [PSNR](https://ru.wikipedia.org/wiki/%D0%9F%D0%B8%D0%BA%D0%BE%D0%B2%D0%BE%D0%B5_%D0%BE%D1%82%D0%BD%D0%BE%D1%88%D0%B5%D0%BD%D0%B8%D0%B5_%D1%81%D0%B8%D0%B3%D0%BD%D0%B0%D0%BB%D0%B0_%D0%BA_%D1%88%D1%83%D0%BC%D1%83)  и является инженерным термином, означающим соотношение между максимумом возможного значения сигнала и мощностью шума, искажающего значения сигнала. Поскольку многие сигналы имеют широкий динамический диапазон, PSNR обычно измеряется в логарифмической шкале в децибелах.

PSNR наиболее часто используется для измерения уровня искажений при сжатии изображений. Проще всего его определить через среднеквадратичную ошибку (СКО) или MSE (англ. mean square error). 

## Подготовительные действия
- `01`: С [сайта](https://media.xiph.org/video/derf/) был скачен видеоряд `waterfall_cif.y4m` (далее `refFile`)
- `02`: Создается новая папка в которой будет хранится `refFile` и выполняется команда: 
```
ffmpeg -i waterfall_cif.y4m -vf format=gray waterfall_cif_grayscale.y4m'
```
- `03`: 


команда для проигрывания видеофайла:
ffplay waterfall_cif.y4m

команда для сжатия видеофайла:
ffmpeg -i waterfall_cif.y4m -c:v libx264 -b:v 1M target_1000.mp4

команда для декодинга видеофайла:
ffmpeg -i target_1000.mp4 -f yuv4mpegpipe decoded_target_1000.y4m

## Компиляция и запуск под Linux с помощью GCC
```
g++ read_file.cpp -o app
./app <Source Folder> <Test Folder> <Number of frames>
