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
 *  **                                                                      **
 *  **  1. Подразумевается, что у вас имеется 2 папки с исходными файлами.  **
 *  **     Если нет - то порядок получения этих папок описан в пункте       **
 *  **     "Подготовительные действия" по ссылке:                           **
 *  **        << https://github.com/Kr1eg3/test_for_ViCueSoft >>            **
 *  **                                                                      **
 *  **  2. После компиляции исполняемого файла, в консоли/терминале         **
 *  **     необходимо прописать следующую команду:                          **
 *  **           ~$ ./app <Source Folder> <Test Folder> <Num of frames>     **
 *  **                                                                      **
 *  **     Пример:                                                          **
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
#include <iomanip>
#include <fstream>

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
    
    // Инициализация среднего PSNR и SSIM
    double AvgPSNR = 0.0;
    double AvgSSIM = 0.0;

    // Инициализация констант
    const double k1 = 0.01;
    const double k2 = 0.03;
    const int L = 255;
    const double c1 = pow((k1 * L), 2);
    const double c2 = pow((k2 * L), 2);
    
    char buffer[0x100];
    std::snprintf(buffer, sizeof(buffer), "results_for_%s.txt", argv[2]); 
    FILE* file = fopen(buffer, "w");

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
      double mod_under_mse = 0.0;
      double avg_x = 0.0;
      double avg_y = 0.0;
      double sigma_x = 0.0;
      double sigma_y = 0.0;
      double sigma_xy = 0.0;
      for (int x = 0; x < width; x++)
      {
        for (int y = 0; y < height; y++)
        {
          size_t index = 1 * (y * width + x);
          auto mod_under_mse_single = pow(abs(static_cast<int>(refImage[index]) - static_cast<int>(tstImage[index])), 2);
          avg_x += static_cast<int>(refImage[index]);
          avg_y += static_cast<int>(tstImage[index]);
          mod_under_mse += mod_under_mse_single;
        } 
      }
      avg_x = avg_x / (height * width);
      avg_y = avg_y / (height * width);
      
      //std::cout <<"mu_x = " <<  avg_x << "mu_y = " <<  avg_y << std::endl;

      double mse = mod_under_mse / (height * width);
      double psnr = 10 * log10(pow(L, 2) / mse);
      
      fprintf(file, "Frame %d PSNR = %f\n", frame, psnr);
    
      AvgPSNR += psnr;
    
      double under_summ_sqx = 0.0f;
      double under_summ_sqy = 0.0f;
      double under_summxy = 0.0f;
      for (int x = 0; x < width; x++)
      {
        for (int y = 0; y < height; y++)
        {
          size_t index_ = 1 * (y * width + x);
          under_summxy += (static_cast<int>(refImage[index_]) - avg_x) * (static_cast<int>(tstImage[index_]) - avg_y);
          under_summ_sqx += pow( (static_cast<int>(refImage[index_]) - avg_x), 2 );
          under_summ_sqy += pow( (static_cast<int>(tstImage[index_]) - avg_y), 2 );
        }
      }

      sigma_x = sqrt( (1 / (height * width - 1) * under_summ_sqx) );
      sigma_y = sqrt( (1 / (height * width - 1) * under_summ_sqy) );
      sigma_xy = 1 / (height * width - 1) * under_summxy;

      AvgSSIM += 2 * (avg_x * avg_y + c1) * (2 * sigma_xy + c2) / ( (pow(avg_x, 2) + pow(avg_y, 2) + c1) * (pow(sigma_x, 2) + pow(sigma_y, 2) + c2) );
    }
    
    // Конец главного цикла и расчёт средних значений метрик
    AvgSSIM = AvgSSIM / numFrames;
    AvgPSNR = AvgPSNR / numFrames;
    //double AvgDSSIM = (1 - AvgSSIM) / 2;
    
    fprintf(file, "Avg PSNR = %f\n", AvgPSNR);
    fprintf(file, "Avg SSIM = %f\n", AvgSSIM);
    fclose(file);

    // Вывод метрик
    std::cout << "Avg PSNR = " << AvgPSNR << std::endl;
    std::cout << "Avg SSIM = " << AvgSSIM << std::endl;
    //std::cout << "Avg AvgDSSIM = " << AvgDSSIM << std::endl;

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
