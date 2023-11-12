#include <iostream>
#include "Lab_5_Exceptions.h"
#include <opencv4/opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>
/// http://192.168.0.101:4747/video
/// webcam
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
    cv::Mat WalterWhite = cv::imread("/home/plushjill/All_Random/11.png");
    if (WalterWhite.empty()){
        std::cout << "NIGGERS" << std::endl;
    }

    //cv::Mat kernel1 = (cv::Mat_<double>(3,3) << 0, 0, 0, 0, 1, 0, 0, 0, 0);
    //cv::Mat identity;

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
        }catch (FrameException& _exception){
            std::cerr << _exception.what() << std::endl;
            return 0;
        }
        //cv::resize(flag, flag, cv::Size(frame.elemSize(), frame.elemSize1()));
        cv::resize(flag, flag, frame.size());
        cv::resize(WalterWhite, WalterWhite, frame.size());
        //const cv::Point origin(100, 100);
        //cv::Rect roi(origin, WalterWhite.size());
        cv::GaussianBlur(flag, flag, cv::Size(15, 15), 0.5, 0.5);
        //cv::bitwise_not(frame, frame);
        //cv::bitwise_or(frame, flag, frame);

        //addWeighted(frame, 0.5, flag, 0.5, 0.0, frame);

        /*filter2D(frame, frame, -1 ,
                 kernel1, cv::Point(-1, -1),
                 0, 4);*/
        WalterWhite.copyTo(frame);
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


    return 0;
}
