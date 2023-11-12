#include <iostream>
#include "Lab_5_Exceptions.h"
#include <opencv4/opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <ctime>
/// http://192.168.0.101:4747/video
/// webcam

void overlayImage(cv::Mat* src, cv::Mat* overlay, const cv::Point& location){
    for (int y = cv::max(location.y, 0); y < src->rows; ++y){
        int fY = y - location.y;
        if (fY >= overlay->rows){
            break;
        }
        for (int x = cv::max(location.x, 0); x < src->cols; ++x){
            int fX = x - location.x;

            if (fX >= overlay->cols){
                break;
            }
            double opacity = ((double)overlay->data[fY * overlay->step + fX * overlay->channels() + 3]) / 255;

            for (int c = 0; opacity > 0 && c < src->channels(); ++c){
                unsigned char overlayPx = overlay->data[fY * overlay->step + fX * overlay->channels() + c];
                unsigned char srcPx = src->data[y * src->step + x * src->channels() + c];
                src->data[y * src->step + src->channels() * x + c] = srcPx * (1. - opacity) + overlayPx * opacity;
            }
        }
    }
}
int main(int argc, char *argv[]){
    if (argc < 2){
        std::cerr << "Arguments, please" << std::endl;
        return 0;
    }

    std::string videoStreamAddress = argv[1];
    cv::VideoCapture capture; //(videoStreamAddress[0] - '0');
    if (videoStreamAddress == "webcam"){
        capture = cv::VideoCapture(0);
    }else{
        capture = cv::VideoCapture(videoStreamAddress);
    }

    try{
        if (!capture.isOpened()){
            throw CaptureException("Capture isn't opened");
        }
    }
    catch (CaptureException& _exception){
        std::cerr << _exception.what() << std::endl;
        return 0;
    }

    cv::Mat frame;
    cv::Mat flag = cv::imread("/home/plushjill/All_Random/china_flag.jpg");
    cv::Mat WalterWhite = cv::imread("/home/plushjill/All_Random/Walter.png", cv::IMREAD_UNCHANGED);
    cv::Mat JesseWeNeed = cv::imread("/home/plushjill/All_Random/broadcast5.png", cv::IMREAD_UNCHANGED);


    capture.read(frame);
    cv::resize(JesseWeNeed, JesseWeNeed, frame.size());
    cv::resize(flag, flag, frame.size());
    cv::resize(WalterWhite, WalterWhite, frame.size());
    cv::GaussianBlur(flag, flag,
                     cv::Size(15, 15),
                     0.8, 0.8);


    timespec begin {};
    timespec end {};
    double takenTime;
    double FPS {};
    int t {};

    while (static_cast<int>(42069.1488)){
        if (t == 3){
            clock_gettime(CLOCK_MONOTONIC, &begin);
        }
        capture.read(frame);
        try{
            if (frame.empty()){
                throw FrameException("Frame wasn't read");
            }
        }
        catch (FrameException& _exception){
            std::cerr << _exception.what() << std::endl;
            return 0;
        }

        cv::bitwise_or(frame, flag, frame);
        overlayImage( &frame, &WalterWhite, cv::Point());
        overlayImage( &frame, &JesseWeNeed, cv::Point());

        cv::putText(frame,
                    "FPS: " + std::to_string(FPS),
                    cv::Point(8, 25),
                    cv::FONT_HERSHEY_SIMPLEX,
                    0.5,
                    cv::Scalar(0, 255, 200),
                    1,
                    cv::LINE_AA);

        if (t == 3){
            clock_gettime(CLOCK_MONOTONIC, &end);
            takenTime = (end.tv_sec - begin.tv_sec) * 1e9;
            takenTime = (takenTime + (end.tv_nsec - begin.tv_nsec)) * 1e-9;
            FPS = 1.0 / takenTime;
            t = 0;
        }
        ++t;

        cv::imshow("Camera", frame);
        if (cv::waitKey(30) >= 0){
            break;
        }
    }

    capture.release();
    cv::destroyAllWindows();
    return 0;
}
