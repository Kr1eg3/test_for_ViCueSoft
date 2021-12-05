# Тестирование на стажировку для ViCueSoft

[![ViCue Soft](https://scontent-hel3-1.xx.fbcdn.net/v/t1.6435-9/100049479_576904839469133_78457503330861056_n.png?_nc_cat=108&ccb=1-5&_nc_sid=973b4a&_nc_ohc=oohtigKncVMAX83Tktk&_nc_ht=scontent-hel3-1.xx&oh=e921e9bcb526f8659fd137b763d0ea50&oe=61D2C460)](https://vicuesoft.com/)

Тестовое задание для стажировки в компании ViCueSoft

## Техническое задание
Создать приложение для расчёта метрики PSNR.

## Метрика PSNR
Пиковое отношение сигнала к шуму (англ. peak signal-to-noise ratio) обозначается аббревиатурой [PSNR](https://ru.wikipedia.org/wiki/%D0%9F%D0%B8%D0%BA%D0%BE%D0%B2%D0%BE%D0%B5_%D0%BE%D1%82%D0%BD%D0%BE%D1%88%D0%B5%D0%BD%D0%B8%D0%B5_%D1%81%D0%B8%D0%B3%D0%BD%D0%B0%D0%BB%D0%B0_%D0%BA_%D1%88%D1%83%D0%BC%D1%83)  и является инженерным термином, означающим соотношение между максимумом возможного значения сигнала и мощностью шума, искажающего значения сигнала. Поскольку многие сигналы имеют широкий динамический диапазон, PSNR обычно измеряется в логарифмической шкале в децибелах.

PSNR наиболее часто используется для измерения уровня искажений при сжатии изображений. Проще всего его определить через среднеквадратичную ошибку (СКО) или MSE (англ. mean square error).

## Необходимые бибилиотеки и содержание репозитория
Для выполнения команд при подготовке необходимо приложение [FFmpeg](https://www.ffmpeg.org/download.html#build-linux). Команда для установки с Linux (Debian):
```
sudo apt install ffmpeg
```
Сама программа написана так, чтобы использовать минимальное количество сторонних библиотек. Поэтому единственной сторонним компонентом является файл `stb_image.h`
необходимый для загрузки изображений ([источник](https://github.com/nothings/stb)).

Список файлов репозитория:
- `stb_image.h` - хедер для загрузки изображений
- `read_file.cpp` - программа для расчета метрики PSNR


## Подготовительные действия
---

<div align="center">
  <h3>💡 Looking for high-quality interview courses? <a href="https://www.educative.io/explore?search_string=interview&aff=x23W">Educative</a> offers a ton of great courses to improve your interview game 💡</h3>
</div>

---
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
