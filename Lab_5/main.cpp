#include <iostream>
#include "Lab_5_Exceptions.h"
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <ctime>
/// http://192.168.0.103:4747/video
/// webcam


void PlacePNG(cv::Mat& frame, cv::Mat& ImageToPlace){
    for (int y = 0; y < frame.rows; ++y){
        for (int x = 0; x < frame.cols; ++x){
            auto &framePixel = frame.at<cv::Vec4b>(y, x);
            auto &walterWhitePixel = ImageToPlace.at<cv::Vec4b>(y, x);

            double alpha = walterWhitePixel[3] / 255.0;
            framePixel[0] = static_cast<uchar>((1.0 - alpha) * framePixel[0] +
                                               alpha * walterWhitePixel[0]);
            framePixel[1] = static_cast<uchar>((1.0 - alpha) * framePixel[1] +
                                               alpha * walterWhitePixel[1]);
            framePixel[2] = static_cast<uchar>((1.0 - alpha) * framePixel[2] +
                                               alpha * walterWhitePixel[2]);
            framePixel[3] = 255;
        }
    }
}
int main(int argc, char *argv[]){
    if (argc < 2){
        std::cerr << "Arguments, please" << std::endl;
        return 0;
    }

    std::string videoStreamAddress = argv[1];
    cv::VideoCapture capture;
    bool sourceIsMobileCamera;

    if (videoStreamAddress == "webcam"){
        sourceIsMobileCamera = false;
        capture = cv::VideoCapture(0);
    }else{
        sourceIsMobileCamera = true;
        capture = cv::VideoCapture(videoStreamAddress);
    }

    try{
        if (!capture.isOpened()){
            throw CaptureException("Capture isn't opened by first try");
        }
    }catch (CaptureException &_exception){
        std::cerr << _exception.what() << std::endl;
        sleep(2);
        sourceIsMobileCamera = true;
        capture = cv::VideoCapture(videoStreamAddress);
        if (!capture.isOpened()){
            return 0;
        }
    }

    cv::Mat frame;
    cv::Mat flag = cv::imread("/home/plushjill/All_Random/china_flag2.jpg", cv::IMREAD_UNCHANGED);
    cv::Mat WalterWhite = cv::imread("/home/plushjill/All_Random/Whalter_White4.png", cv::IMREAD_UNCHANGED); /// CV_8UC4
    cv::Mat JesseWeNeed = cv::imread("/home/plushjill/All_Random/broadcast5.png", cv::IMREAD_UNCHANGED); /// cv::IMREAD_UNCHANGED

    capture.read(frame);
    cv::resize(frame, frame, cv::Size(1280, 720));
    cv::resize(JesseWeNeed, JesseWeNeed, frame.size());
    cv::resize(flag, flag, frame.size());
    cv::resize(WalterWhite, WalterWhite, frame.size());
    cv::cvtColor(flag, flag, cv::COLOR_BGR2BGRA);


    timespec begin{};
    timespec end{};
    double takenTime;
    double FPS{};
    int t{};

    timespec beginRead{};
    timespec endRead{};
    double takenTimeToRead{};

    timespec beginShow{};
    timespec endShow{};
    double takenTimeToShow{};

    while (static_cast<bool>(42069.1488)){
        if (t == 3){
            clock_gettime(CLOCK_BOOTTIME, &begin);
            clock_gettime(CLOCK_BOOTTIME, &beginRead);
        }

        capture.read(frame);

        if (t == 3){
            clock_gettime(CLOCK_BOOTTIME, &endRead);
            takenTimeToRead = static_cast<double>(endRead.tv_sec - beginRead.tv_sec) * 1e9;
            takenTimeToRead = (takenTimeToRead + static_cast<double>(endRead.tv_nsec - beginRead.tv_nsec)) * 1e-9;
        }
        try{
            if (frame.empty()){
                throw FrameException("Frame wasn't read");
            }
        }catch (FrameException &_exception){
            std::cerr << _exception.what() << std::endl;
            return 0;
        }


        if (!sourceIsMobileCamera) {
            cv::flip(frame, frame, 1);
        }
        cv::resize(frame, frame, cv::Size(1280, 720));
        cv::cvtColor(frame, frame, cv::COLOR_BGR2BGRA);



        cv::GaussianBlur(flag, flag,cv::Size(15, 15),0.5, 0.5);
        addWeighted(flag, 0.5, frame, 0.5, 0.0, frame);

        PlacePNG(frame, WalterWhite);
        PlacePNG(frame, JesseWeNeed);



        cv::putText(frame,
                    "FPS: " + std::to_string(FPS),
                    cv::Point(8, 32),
                    cv::FONT_HERSHEY_SIMPLEX,
                    0.5,
                    cv::Scalar(0, 255, 200),
                    1,
                    cv::LINE_AA);
        cv::putText(frame,
                    "Time To Read: " + std::to_string(takenTimeToRead),
                    cv::Point(8, 52),
                    cv::FONT_HERSHEY_SIMPLEX,
                    0.5,
                    cv::Scalar(0, 255, 200),
                    1,
                    cv::LINE_AA);
        cv::putText(frame,
                    "Time To Show: " + std::to_string(takenTimeToShow),
                    cv::Point(8, 72),
                    cv::FONT_HERSHEY_SIMPLEX,
                    0.5,
                    cv::Scalar(0, 255, 200),
                    1,
                    cv::LINE_AA);

        if (t == 3){
            clock_gettime(CLOCK_BOOTTIME, &end);
            takenTime = static_cast<double>(end.tv_sec - begin.tv_sec) * 1e9;
            takenTime = (takenTime + static_cast<double>(end.tv_nsec - begin.tv_nsec)) * 1e-9;
            FPS = 1.0 / takenTime;
            t = 0;
        }
        ++t;

        if (t == 3){
            clock_gettime(CLOCK_BOOTTIME, &beginShow);
        }
        cv::imshow("Camera", frame);
        if (t == 3){
            clock_gettime(CLOCK_BOOTTIME, &endShow);
            takenTimeToShow = static_cast<double>(endShow.tv_sec - beginShow.tv_sec) * 1e9;
            takenTimeToShow = (takenTimeToShow + static_cast<double>(endShow.tv_nsec - beginShow.tv_nsec)) * 1e-9;
        }
        int c = cv::waitKey(1);
        if (c == 27){
            break;
        }
    }

    capture.release();
    cv::destroyAllWindows();
    return 0;
}
