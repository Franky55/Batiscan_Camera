#include <SPI.h>
#include "esp_http_server.h"
#include "img_converters.h"
#include <WiFi.h>

#define PART_BOUNDARY "123456789000000000000987654321"

const char* ssid = "SM-G930W87541";
const char* password = "franky66";

static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

uint8_t image_buffer[160*120*2] = {255, 216, 255, 224, 0, 16, 74, 70, 73, 70, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 255, 219, 0, 67, 0, 10, 7, 8, 9, 8, 6, 10, 9, 8, 9, 11, 11, 10, 12, 15, 25, 16, 15, 14, 14, 15, 31, 22, 23, 18, 25, 36, 32, 38, 38, 36, 32, 35, 34, 40, 45, 58, 49, 40, 43, 54, 43, 34, 35, 50, 68, 51, 54, 59, 61, 64, 65, 64, 39, 48, 71, 76, 70, 63, 75, 58, 63, 64, 62, 255, 219, 0, 67, 1, 11, 11, 11, 15, 13, 15, 29, 16, 16, 29, 62, 41, 35, 41, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 255, 196, 0, 31, 0, 0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 255, 196, 0, 181, 16, 0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 125, 1, 2, 3, 0, 4, 17, 5, 18, 33, 49, 65, 6, 19, 81, 97, 7, 34, 113, 20, 50, 129, 145, 161, 8, 35, 66, 177, 193, 21, 82, 209, 240, 36, 51, 98, 114, 130, 9, 10, 22, 23, 24, 25, 26, 37, 38, 39, 40, 41, 42, 52, 53, 54, 55, 56, 57, 58, 67, 68, 69, 70, 71, 72, 73, 74, 83, 84, 85, 86, 87, 88, 89, 90, 99, 100, 101, 102, 103, 104, 105, 106, 115, 116, 117, 118, 119, 120, 121, 122, 131, 132, 133, 134, 135, 136, 137, 138, 146, 147, 148, 149, 150, 151, 152, 153, 154, 162, 163, 164, 165, 166, 167, 168, 169, 170, 178, 179, 180, 181, 182, 183, 184, 185, 186, 194, 195, 196, 197, 198, 199, 200, 201, 202, 210, 211, 212, 213, 214, 215, 216, 217, 218, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 255, 196, 0, 31, 1, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 255, 196, 0, 181, 17, 0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 119, 0, 1, 2, 3, 17, 4, 5, 33, 49, 6, 18, 65, 81, 7, 97, 113, 19, 34, 50, 129, 8, 20, 66, 145, 161, 177, 193, 9, 35, 51, 82, 240, 21, 98, 114, 209, 10, 22, 36, 52, 225, 37, 241, 23, 24, 25, 26, 38, 39, 40, 41, 42, 53, 54, 55, 56, 57, 58, 67, 68, 69, 70, 71, 72, 73, 74, 83, 84, 85, 86, 87, 88, 89, 90, 99, 100, 101, 102, 103, 104, 105, 106, 115, 116, 117, 118, 119, 120, 121, 122, 130, 131, 132, 133, 134, 135, 136, 137, 138, 146, 147, 148, 149, 150, 151, 152, 153, 154, 162, 163, 164, 165, 166, 167, 168, 169, 170, 178, 179, 180, 181, 182, 183, 184, 185, 186, 194, 195, 196, 197, 198, 199, 200, 201, 202, 210, 211, 212, 213, 214, 215, 216, 217, 218, 226, 227, 228, 229, 230, 231, 232, 233, 234, 242, 243, 244, 245, 246, 247, 248, 249, 250, 255, 192, 0, 17, 8, 0, 120, 0, 160, 3, 1, 33, 0, 2, 17, 1, 3, 17, 1, 255, 218, 0, 12, 3, 1, 0, 2, 17, 3, 17, 0, 63, 0, 243, 140, 140, 253, 209, 91, 58, 32, 67, 20, 171, 133, 206, 236, 242, 40, 182, 129, 61, 141, 71, 141, 118, 143, 145, 127, 42, 140, 70, 191, 221, 95, 202, 141, 12, 146, 23, 98, 255, 0, 113, 127, 42, 95, 45, 63, 184, 191, 149, 26, 21, 97, 222, 90, 127, 10, 47, 229, 79, 17, 39, 93, 139, 249, 80, 2, 249, 105, 253, 197, 252, 169, 222, 90, 127, 113, 127, 42, 44, 80, 187, 23, 254, 121, 167, 253, 243, 76, 123, 120, 165, 93, 173, 10, 48, 255, 0, 118, 129, 28, 149, 219, 88, 53, 193, 242, 35, 117, 139, 251, 212, 137, 167, 53, 194, 51, 218, 226, 101, 29, 118, 117, 20, 45, 10, 43, 27, 109, 135, 12, 165, 79, 189, 51, 202, 193, 233, 78, 224, 49, 135, 181, 54, 152, 13, 56, 166, 210, 176, 26, 85, 167, 161, 55, 250, 102, 223, 239, 174, 58, 254, 52, 138, 150, 168, 232, 92, 124, 149, 9, 20, 204, 69, 2, 151, 20, 138, 67, 128, 167, 129, 64, 199, 98, 157, 138, 6, 46, 56, 164, 164, 35, 153, 213, 116, 70, 140, 153, 109, 6, 232, 250, 236, 254, 229, 97, 171, 75, 12, 187, 162, 118, 70, 254, 242, 54, 13, 50, 139, 137, 172, 95, 168, 11, 231, 229, 125, 212, 26, 100, 186, 141, 228, 169, 182, 73, 190, 82, 49, 128, 160, 83, 178, 68, 218, 197, 70, 166, 208, 49, 141, 77, 166, 6, 155, 84, 246, 110, 96, 188, 138, 78, 120, 110, 126, 157, 234, 75, 221, 29, 158, 222, 8, 170, 228, 80, 96, 132, 197, 59, 20, 22, 60, 45, 60, 45, 3, 30, 22, 151, 20, 0, 184, 164, 197, 0, 52, 138, 163, 119, 166, 219, 93, 18, 101, 143, 231, 63, 196, 56, 52, 128, 199, 159, 195, 175, 156, 193, 50, 31, 247, 248, 168, 91, 66, 156, 127, 20, 100, 251, 26, 119, 2, 57, 52, 123, 129, 209, 84, 253, 13, 66, 116, 139, 174, 158, 87, 254, 60, 40, 21, 200, 155, 75, 185, 207, 250, 186, 135, 251, 62, 127, 249, 230, 104, 11, 162, 241, 140, 102, 128, 159, 53, 9, 245, 55, 71, 101, 105, 243, 90, 192, 199, 146, 99, 82, 127, 42, 107, 174, 9, 160, 231, 24, 5, 60, 45, 3, 30, 5, 60, 10, 6, 63, 20, 184, 164, 2, 226, 155, 138, 0, 105, 20, 198, 164, 128, 136, 138, 141, 169, 136, 137, 170, 35, 65, 36, 77, 81, 53, 2, 49, 139, 176, 165, 89, 13, 63, 67, 168, 235, 116, 103, 243, 52, 200, 189, 87, 42, 127, 60, 213, 169, 87, 230, 160, 231, 150, 227, 66, 211, 130, 208, 3, 194, 211, 194, 208, 80, 236, 81, 138, 0, 49, 77, 197, 32, 24, 105, 134, 128, 35, 106, 132, 210, 2, 38, 168, 90, 168, 146, 7, 168, 88, 208, 35, 41, 232, 94, 190, 244, 29, 39, 75, 225, 201, 70, 201, 98, 239, 195, 129, 250, 26, 217, 145, 105, 236, 99, 61, 198, 109, 167, 1, 72, 67, 182, 211, 128, 160, 98, 226, 151, 20, 128, 76, 83, 13, 0, 48, 212, 109, 72, 8, 154, 162, 106, 0, 129, 170, 6, 52, 201, 32, 127, 202, 160, 115, 76, 70, 95, 152, 41, 235, 201, 235, 69, 172, 116, 196, 218, 208, 27, 102, 164, 7, 247, 212, 175, 245, 174, 160, 175, 203, 66, 49, 171, 185, 30, 41, 193, 105, 146, 60, 45, 59, 109, 33, 139, 182, 140, 82, 24, 194, 41, 134, 144, 17, 154, 137, 168, 2, 22, 168, 88, 210, 2, 6, 53, 89, 205, 80, 153, 94, 70, 30, 181, 89, 154, 132, 35, 20, 238, 86, 169, 18, 124, 117, 167, 185, 174, 165, 251, 59, 213, 134, 120, 101, 97, 157, 140, 31, 30, 184, 53, 218, 195, 170, 91, 200, 219, 55, 141, 212, 17, 61, 73, 210, 88, 219, 161, 169, 151, 20, 16, 72, 5, 59, 109, 3, 23, 109, 5, 104, 40, 137, 133, 66, 213, 32, 68, 213, 3, 154, 64, 103, 220, 95, 219, 197, 247, 164, 21, 145, 113, 175, 68, 9, 17, 140, 213, 43, 3, 51, 165, 213, 174, 100, 206, 213, 226, 169, 189, 245, 193, 61, 105, 216, 8, 77, 220, 223, 222, 164, 251, 84, 190, 180, 5, 142, 189, 60, 63, 52, 188, 182, 196, 7, 212, 243, 82, 183, 133, 211, 31, 235, 25, 189, 241, 138, 35, 16, 117, 10, 83, 248, 101, 212, 254, 236, 159, 198, 170, 54, 147, 125, 7, 220, 60, 15, 67, 84, 196, 153, 31, 157, 168, 192, 223, 120, 231, 235, 83, 166, 189, 125, 23, 222, 164, 59, 34, 113, 226, 123, 145, 252, 63, 157, 42, 248, 178, 232, 117, 141, 79, 227, 64, 249, 16, 241, 226, 219, 159, 249, 230, 7, 227, 74, 60, 95, 115, 143, 245, 75, 64, 114, 13, 62, 44, 184, 63, 242, 204, 126, 117, 25, 241, 69, 193, 255, 0, 150, 98, 164, 118, 33, 255, 0, 132, 154, 235, 159, 145, 106, 149, 214, 173, 117, 115, 156, 190, 23, 218, 152, 88, 205, 98, 75, 100, 243, 83, 71, 114, 177, 255, 0, 203, 37, 39, 214, 134, 4, 141, 168, 146, 184, 242, 211, 240, 21, 73, 165, 39, 181, 2, 27, 65, 90, 0, 238, 159, 196, 17, 45, 64, 254, 39, 3, 160, 38, 168, 203, 148, 134, 79, 19, 177, 232, 159, 173, 86, 111, 17, 74, 223, 242, 204, 80, 95, 33, 86, 109, 80, 204, 126, 238, 63, 26, 167, 36, 254, 103, 106, 145, 162, 33, 138, 50, 41, 238, 59, 134, 225, 70, 225, 72, 46, 38, 241, 73, 230, 10, 2, 227, 75, 138, 111, 152, 40, 179, 21, 198, 151, 20, 221, 244, 108, 2, 111, 166, 238, 167, 176, 92, 65, 184, 158, 42, 93, 178, 14, 163, 138, 64, 72, 206, 105, 155, 169, 133, 195, 117, 46, 104, 21, 192, 19, 235, 65, 221, 64, 196, 221, 70, 105, 136, 55, 98, 140, 210, 1, 57, 207, 74, 145, 97, 153, 190, 228, 78, 127, 10, 0, 144, 88, 94, 31, 249, 96, 255, 0, 149, 74, 186, 61, 243, 244, 136, 210, 184, 22, 63, 225, 28, 191, 255, 0, 150, 128, 70, 63, 219, 56, 160, 232, 37, 63, 214, 94, 219, 255, 0, 192, 14, 250, 46, 0, 186, 32, 97, 193, 157, 190, 137, 79, 95, 15, 187, 127, 179, 254, 241, 161, 138, 227, 255, 0, 225, 28, 245, 184, 199, 208, 102, 161, 125, 2, 68, 255, 0, 87, 40, 160, 92, 230, 83, 125, 41, 42, 154, 40, 93, 167, 208, 210, 236, 98, 56, 6, 129, 18, 173, 188, 216, 207, 148, 213, 102, 61, 54, 238, 110, 145, 254, 52, 12, 189, 15, 134, 111, 101, 246, 168, 175, 244, 111, 178, 77, 29, 178, 147, 53, 212, 157, 17, 69, 26, 32, 230, 58, 29, 39, 193, 120, 132, 53, 254, 12, 135, 157, 163, 248, 107, 106, 15, 9, 233, 241, 243, 228, 10, 68, 243, 23, 35, 210, 52, 248, 71, 202, 145, 15, 165, 72, 97, 180, 78, 138, 15, 208, 82, 2, 188, 251, 54, 145, 24, 10, 113, 193, 35, 165, 115, 190, 78, 163, 114, 112, 246, 239, 25, 105, 84, 71, 27, 92, 43, 117, 233, 75, 114, 150, 133, 50, 136, 175, 202, 126, 99, 165, 88, 25, 78, 148, 208, 50, 101, 203, 82, 159, 151, 168, 166, 102, 68, 210, 227, 181, 48, 180, 132, 116, 164, 73, 28, 94, 28, 7, 239, 85, 216, 188, 49, 7, 113, 90, 7, 180, 185, 118, 47, 13, 91, 3, 247, 42, 220, 122, 21, 162, 127, 203, 53, 160, 46, 78, 52, 251, 100, 28, 32, 166, 34, 68, 189, 22, 164, 104, 205, 214, 53, 207, 178, 127, 162, 216, 68, 37, 190, 147, 238, 142, 187, 105, 250, 46, 157, 246, 34, 215, 55, 15, 230, 222, 75, 203, 72, 221, 169, 50, 236, 108, 121, 239, 253, 227, 77, 243, 13, 33, 137, 186, 147, 147, 82, 3, 25, 61, 41, 21, 82, 206, 226, 222, 246, 97, 185, 33, 149, 89, 193, 254, 238, 121, 53, 73, 18, 228, 59, 199, 154, 105, 181, 212, 166, 102, 143, 106, 249, 160, 161, 207, 222, 87, 174, 110, 221, 195, 197, 243, 26, 109, 10, 44, 185, 18, 171, 240, 163, 230, 167, 180, 101, 71, 204, 40, 220, 151, 161, 3, 96, 116, 170, 237, 140, 255, 0, 141, 61, 137, 58, 20, 53, 97, 13, 50, 70, 75, 127, 107, 4, 130, 57, 103, 69, 115, 219, 61, 42, 193, 108, 83, 40, 169, 44, 251, 184, 29, 43, 31, 90, 213, 151, 79, 135, 106, 2, 243, 201, 194, 40, 168, 52, 35, 209, 244, 223, 179, 102, 230, 229, 252, 219, 169, 57, 103, 53, 177, 186, 145, 66, 134, 165, 6, 129, 143, 90, 153, 7, 106, 4, 201, 146, 26, 150, 123, 67, 36, 12, 184, 234, 49, 76, 201, 150, 53, 8, 151, 88, 240, 29, 157, 197, 196, 153, 158, 208, 139, 43, 178, 205, 150, 36, 29, 160, 215, 155, 92, 171, 217, 200, 208, 201, 145, 36, 60, 56, 254, 84, 22, 77, 13, 209, 95, 157, 28, 134, 173, 123, 123, 200, 238, 83, 146, 139, 39, 167, 52, 132, 209, 90, 116, 236, 159, 55, 208, 86, 124, 141, 201, 173, 12, 69, 186, 241, 108, 16, 157, 182, 182, 237, 55, 251, 82, 29, 163, 53, 147, 47, 136, 46, 47, 56, 185, 118, 9, 211, 104, 251, 180, 93, 155, 66, 153, 110, 216, 6, 40, 6, 62, 99, 129, 93, 124, 179, 140, 108, 143, 238, 10, 157, 193, 149, 46, 174, 146, 214, 7, 154, 83, 242, 173, 100, 105, 48, 201, 119, 114, 218, 149, 208, 251, 223, 234, 151, 208, 80, 246, 40, 220, 221, 78, 221, 82, 3, 131, 83, 131, 80, 50, 84, 53, 110, 26, 4, 205, 59, 116, 205, 111, 90, 90, 171, 165, 89, 132, 136, 244, 152, 33, 135, 88, 212, 180, 151, 207, 151, 169, 65, 231, 224, 122, 143, 221, 201, 94, 93, 226, 219, 70, 142, 227, 206, 124, 153, 35, 115, 107, 59, 118, 102, 83, 140, 208, 105, 209, 28, 220, 82, 237, 109, 190, 149, 50, 202, 85, 193, 12, 71, 184, 237, 73, 148, 93, 23, 172, 249, 243, 89, 201, 245, 6, 146, 69, 96, 63, 213, 237, 255, 0, 120, 211, 185, 147, 86, 63, 255, 217, 0, 0, 0, 0, 0, 182, 136, 29, 59, 59, 30, 13, 37, 142, 165, 119, 54, 165, 30, 231, 200, 144, 252, 203, 237, 73, 220, 221, 35, 94, 105, 184, 43, 92, 246, 165, 117, 45, 253, 215, 246, 109, 148, 128, 12, 31, 57, 189, 189, 42, 99, 220, 209, 88, 210, 177, 181, 134, 198, 217, 97, 128, 124, 163, 191, 115, 86, 55, 86, 59, 234, 104, 131, 117, 27, 169, 49, 139, 186, 148, 26, 129, 18, 3, 83, 175, 53, 102, 114, 44, 47, 76, 85, 255, 0, 7, 79, 18, 106, 183, 250, 45, 194, 134, 183, 212, 80, 206, 20, 142, 11, 99, 107, 173, 107, 23, 169, 133, 206, 91, 197, 90, 119, 217, 166, 147, 3, 51, 88, 177, 142, 77, 189, 76, 125, 115, 92, 124, 251, 81, 139, 167, 220, 106, 214, 218, 149, 18, 7, 0, 128, 115, 154, 132, 198, 8, 170, 189, 141, 110, 127, 255, 217, 0, 0, 106, 129, 207, 6, 171, 168, 39, 109, 8, 132, 219, 148, 84, 137, 63, 27, 77, 43, 88, 208, 255, 217, 

};

#define SPI_MISO 13
#define SPI_MOSI 11
#define SPI_CLK 12
#define INTERFACE_SPI_CS1 2


#define SPI_BUFFER_SIZE 64

unsigned char RAW_RX_buf[SPI_BUFFER_SIZE] {0};
unsigned char RAW_TX_buf[SPI_BUFFER_SIZE] = {'0','1','2','3','4','5','6','7','8','9',
'0','1','2','3','4','5','6','7','8','9',
'0','1','2','3','4','5','6','7','8','9',
'0','1','2','3','4','5','6','7','8','9',
'0','1','2','3','4','5','6','7','8','9',
'0','1','2','3','4','5','6','7','8','9',
'0','1','2','3'};



SPIClass interfaceSPI_Master(HSPI);


int interface_SPI_MASTER_Transaction(unsigned char * data, unsigned char * out, unsigned long size);

httpd_handle_t stream_httpd = NULL;


void send_image_buffer(uint8_t* buffer, httpd_req_t *req) {
  size_t buffer_len = 2598; // Assuming 16-bit color depth (RGB565 format)
  uint8_t* jpg_buf = NULL;
  size_t jpg_buf_len = 2598;
  char * part_buf[64];

  //bool jpeg_converted = fmt2jpg(buffer, buffer_len, 96, 96, PIXFORMAT_GRAYSCALE, 80, &jpg_buf, &jpg_buf_len);
  bool jpeg_converted = fmt2jpg(buffer, buffer_len, 160, 120, PIXFORMAT_JPEG, 80, &jpg_buf, &jpg_buf_len);
  if (!jpeg_converted) {
    Serial.println("JPEG conversion failed");
    return;
  }
  
    
  size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, jpg_buf_len);
  httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
  httpd_resp_send_chunk(req, (const char*)image_buffer, jpg_buf_len);
  httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));

  free(jpg_buf);
}



static esp_err_t stream_handler(httpd_req_t *req){
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 2598;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if(res != ESP_OK){
    return res;
  }

  while(true){
    

        
    if(res == ESP_OK){
      
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if(res == ESP_OK){
      
      res = httpd_resp_send_chunk(req, (const char *)image_buffer, _jpg_buf_len);
    }
    if(res == ESP_OK){
      
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if(fb){
      
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if(_jpg_buf){
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if(res != ESP_OK){
      break;
    }
    //Serial.printf("MJPG: %uB\n",(uint32_t)(_jpg_buf_len));
  }
  return res;
}







void startCameraServer(){
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  //config.max_resp_headers = 5;
  config.server_port = 80;

  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
    
  };
  
  //Serial.printf("Starting web server on port: '%d'\n", config.server_port);
  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(stream_httpd, &index_uri);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(INTERFACE_SPI_CS1, OUTPUT);
  interfaceSPI_Master.begin(SPI_CLK, SPI_MISO, SPI_MOSI, INTERFACE_SPI_CS1);

  // Wi-Fi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  delay(3000);
  Serial.println("");
  Serial.println("WiFi connected");
  
  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.print(WiFi.localIP());
  
  // Start streaming web server
  startCameraServer();

}

void loop() {
  // put your main code here, to run repeatedly:
  interface_SPI_MASTER_Transaction(RAW_TX_buf, RAW_RX_buf, SPI_BUFFER_SIZE);
  delay(1000);
}


int interface_SPI_MASTER_Transaction(unsigned char * data, unsigned char * out, unsigned long size)
{
    digitalWrite(INTERFACE_SPI_CS1, LOW);  // Select the slave device
    interfaceSPI_Master.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    delayMicroseconds(10);

  // Send each character of the command string
//   for (size_t i = 0; i < size; i++) {
//     interfaceSPI_Master.transfer(data[i]);
//   }

    interfaceSPI_Master.transferBytes(data, out, size);
  
    interfaceSPI_Master.endTransaction();
    digitalWrite(INTERFACE_SPI_CS1, HIGH);  // Deselect the slave device
    // Serial.print("\n\nData Sending:     ");
    
    // interface_SPI_Master_Struct.Size = size;

    // for(int i = 0; i < size; i++)
    // {
    //     interface_SPI_Master_Struct.Transmit_SPI[i] = data[i]; 
    //     Serial.print((char)data[i]);
        
    // }


    Serial.print("\nData receiving:     ");

    for(int i = 0; i < size; i++)
    {
        //interface_SPI_Master_Struct.Received_SPI[i] = out[i]; 
        Serial.print((char)out[i]);
    }

    return 0;
}