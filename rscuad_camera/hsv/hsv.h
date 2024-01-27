/**
 *	Title	: OLD HSV Porting to library
 *
 *	Author	: - Teuku tsani
			  - Sahid al irfan
			  - Muhammad Annas
			  - Danu Andrean
              - Miftahul Alim

 *	Create	: 2022
 *	Update 	: 2023
 *	
 *	Note	: Kalman filter not implement
 */

#ifndef HSV_H
#define HSV_H
#include <opencv2/opencv.hpp>
#if CV_MAJOR_VERSION < 4
    #include <opencv2/highgui/highgui.hpp>
#else
    #include <opencv2/highgui.hpp>
#endif
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/video/tracking.hpp>

#include <stdio.h>
#include <unistd.h>
#include <iostream>

class HSV
{
private:
    struct Point2D
    {
        double X;
        double Y;
    };

    Point2D pos;

    cv::VideoCapture cap;
    cv::Mat     m_frame;
    cv::Mat     m_frameCopy;
    cv::Mat     m_gray;
    cv::Mat     m_thresholdOut;
    cv::Mat     m_drawing, m_drawing2, gambar;

	cv::Mat     m_segments;
    cv::Mat     m_segment;

    int     m_noballcount;
    int     m_noballcountMax;
    bool    m_ball;

    int     m_brightness,
            m_contrast;

    int     h_minField,
            h_maxField, 
            s_minField, 
            s_maxField, 
            v_minField, 
            v_maxField,
            m_contour;

    int     h_minBall, 
            h_maxBall, 
            s_minBall, 
            s_maxBall, 
            v_minBall, 
            v_maxBall;
private:
    void    TrackBarLighting();
    void    TrackBarField();
    void    TrackBarBall();
    void    Lighting(cv::Mat src);
    void    Field(cv::Mat src);
    bool	Ball();
    
public:
    HSV();
    ~HSV();
    void Init();
    bool Process();
    int EndProsess();
    void SetField(   int h_min, int h_max,
                    int s_min, int s_max,
                    int v_min, int v_max, int contures);
    void SetBall(    int h_min, int h_max,
                    int s_min, int s_max,
                    int v_min, int v_max);
    void SetLighting(int brightness,int contrast);
    double GetX() const;
    double GetY() const;
    std::vector<uchar> GetFrame();
};

#endif