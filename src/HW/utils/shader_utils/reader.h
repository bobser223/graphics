//
// Created by Volodymyr Avvakumov on 11.04.2026.
//

#ifndef COMPUTERGRAPHICS_READER_H
#define COMPUTERGRAPHICS_READER_H

#include <filesystem>
#include <string>

// -> shader_utils -> utils -> HW -> src -> computerGraphics
inline const std::filesystem::path PROJECT_PATH =std::filesystem::path(__FILE__).parent_path().parent_path().parent_path().parent_path().parent_path();

std::string readShaderFromFile(const std::string& path);


#endif //COMPUTERGRAPHICS_READER_H
