# test_for_ViCueSoft
Тестовое задание для стажировки в компании ViCueSoft
[![ViCue Soft](https://www.google.com/url?sa=i&url=https%3A%2F%2Fvicuesoft.com%2Fnews%2F&psig=AOvVaw1TlqCDzmVgDAVE9hNPFOYQ&ust=1638773957601000&source=images&cd=vfe&ved=0CAsQjRxqGAoTCOCKsI-zzPQCFQAAAAAdAAAAABChAQ)](https://vicuesoft.com/)

## ТЗ
Создать приложение для расчёта метрики PSNR.

## Метрика PSNR
Пиковое отношение сигнала к шуму (англ. peak signal-to-noise ratio) обозначается аббревиатурой [PSNR](https://ru.wikipedia.org/wiki/%D0%9F%D0%B8%D0%BA%D0%BE%D0%B2%D0%BE%D0%B5_%D0%BE%D1%82%D0%BD%D0%BE%D1%88%D0%B5%D0%BD%D0%B8%D0%B5_%D1%81%D0%B8%D0%B3%D0%BD%D0%B0%D0%BB%D0%B0_%D0%BA_%D1%88%D1%83%D0%BC%D1%83)  и является инженерным термином, означающим соотношение между максимумом возможного значения сигнала и мощностью шума, искажающего значения сигнала. Поскольку многие сигналы имеют широкий динамический диапазон, PSNR обычно измеряется в логарифмической шкале в децибелах.

PSNR наиболее часто используется для измерения уровня искажений при сжатии изображений. Проще всего его определить через среднеквадратичную ошибку (СКО) или MSE (англ. mean square error). 

## Подготовительные действия
- `01`: С [сайта](https://media.xiph.org/video/derf/) был скачен видеоряд `waterfall_cif.y4m` (далее `refFile`)
- `02`: Создается новая папка в которой будет хранится `refFile` и выполняется команда, для перевода видео в оттенки серого: 
```
ffmpeg -i waterfall_cif.y4m -vf format=gray waterfall_cif_grayscale.y4m'
```
- `03`: Для сжатия файла в различных битрейтрах выполнялась команда:
```
ffmpeg -i waterfall_cif.y4m -c:v libx264 -b:v 1M target_1000.mp4
```
(данная команда создаст один сжатый файл для битрейта 1000 kbps)
- `04`: Для декодирования видеофайла использовалась команда:
```
ffmpeg -i target_1000.mp4 -f yuv4mpegpipe decoded_target_1000.y4m
```
- `05`: После получения 5 тестовых видеофайлов они были для удобства раскинуты по папкам



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
