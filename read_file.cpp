/**  Данная программа написана Пожарским Аркадием в качестве тестого задания
 *  для отбора на стажировку в компанию ViCue Soft. 
 *
 *  Программа представляет собой приложение без графического интерфейса,
 *  которое осуществляет чтение изображений с видеофайлов для последющего
 *  вывода значений основных метрик для измерения уровня искажений при
 *  сжатии и последующего декодирования видеофайла относительно его 
 *  оригинала. 
 *
 *  **************************************************************************
 *  ********************** ИСПОЛЬЗОВАНИЕ ПРОГРАММЫ ***************************
 *  **  1. Подразумевается, что у вас имеется 2 папки с исходными файлами.  **
 *  **     Если нет - то порядок получения этих папок описан в пункте       **
 *  **       "Подготовительные действия" по ссылке:                         **
 *  **        << https://github.com/Kr1eg3/test_for_ViCueSoft >>            **
 *  **                                                                      **
 *  **    2. После компиляции исполняемого файла, в консоли/терминале       **
 *  **       необходимо прописать следующую команду:                        **
 *  **           ~$ ./app <Source Folder> <Test Folder> <Num of frames>     **
 *  **                                                                      **
 *  **       Пример:                                                        **
 *  **           ~$ ./app Source Test 260                                   **
 *  **                                                                      **
 *  **************************************************************************
 *  **************************************************************************
 */

// Библиотеки 
#include <iostream>
#include <vector>
#include <cstdio>
#include <sstream>
#include <stdlib.h> 
#include <math.h>

// Подключение хедера std_image.h
extern "C" {
    #define STB_IMAGE_IMPLEMENTATION
    #include "stb_image.h"
}

// Прототипы
bool load_image( std::vector<unsigned char>& image, const std::string& filename, int& x, int&y );
void load_check( bool success, std::string filename );

int main( int argc, char *argv[] )
{
    // Проверка количества аргументов на входе программы
    if (argc != 4)
    {
      std::cout << "Not enought arguments" << std::endl;
      return -1;
    }
    // Обработка поступивших на вход агрументов
    std::stringstream conv;
    const std::string sourceRef = argv[1];
    const std::string sourceTst = argv[2];
    int numFrames;
    conv << argv[3];
    conv >> numFrames;
    
    // Инициализация среднего PSNR
    float AvgPSNR = 0.f;

    // Главный цикл который проходится по фреймам
    for (int frame = 1; frame <= numFrames; frame++)
    {

      // Создаем путь к изображению
      std::string refFile = sourceRef + "/img" + std::to_string(frame) + ".png";
      std::string testFile = sourceTst + "/img" + std::to_string(frame) + ".png";
    
      // Инициализация контейнеров для изображний
      int width, height;
      std::vector<unsigned char> refImage;
      std::vector<unsigned char> tstImage;

      // Загрузка изображений 
      bool successRef = load_image(refImage, refFile, width, height);
      bool successTst = load_image(tstImage, testFile, width, height);
  
      // Проверка, загрузились ли изображения
      load_check(successRef, refFile);
      load_check(successTst, testFile);

      // Расчёты метрик
      float mod_under_mse = 0.f;
      for (int x = 0; x < width; x++)
      {
        for (int y = 0; y < height; y++)
        {
          size_t index = 1 * (y * width + x);
          auto mod_under_mse_single = pow(abs(static_cast<int>(refImage[index]) - static_cast<int>(tstImage[index])), 2);
          mod_under_mse += mod_under_mse_single;
        } 
      }
      
      auto mse = mod_under_mse / (height * width);
      auto psnr = 10 * log10((255 * 255) / mse);
      AvgPSNR += psnr;
    }

    AvgPSNR = AvgPSNR / numFrames;

    // Вывод метрик
    std::cout << "Avg PSNR = " << AvgPSNR << std::endl;

    return 0;
}

/**
 * Загружает в контейнер данные изображения.
 * @param image - контейнер для данных изображения.
 * @param filename - путь к изображению.
 * @param x, y - ширина и высота изображения
 * @return true если загрузка прошла успешно, иначе false.
*/
bool load_image( std::vector<unsigned char>& image, const std::string& filename, int& x, int& y )
{
    int n;
    unsigned char* data = stbi_load(filename.c_str(), &x, &y, &n, 1);
    if (data != nullptr)
    {
        image = std::vector<unsigned char>(data, data + x * y * 1);
    }
    stbi_image_free(data);
    return (data != nullptr);
}

/**
 * Проверка загрузки, если загрузка неудалась, то выводит какой именно файл не загрузился.
 * @param success - load_image() --> bool.
 * @param filename - путь к изображению.
 * @return None
*/
void load_check( bool success, std::string filename )
{
  if (!success)
  {
    std::cout << "Error in opening file " << filename << "\n";
    exit(1);
  }
}
