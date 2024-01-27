#include "hsv.h"

HSV::HSV()
: m_ball(false)
, m_noballcountMax(3)
{
}

HSV::~HSV()
{
	cv::destroyAllWindows();
}

void HSV::Init()
{
	//cap.open("outtttt1.avi");
	cap.open(0);
	TrackBarLighting();
	TrackBarField();
	TrackBarBall();

	/*
		Set frame size | used default opencv frame to fix error
	*/
#if CV_MAJOR_VERSION < 4
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 320); // 640
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240); // 480
#else
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 320); // 640
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 240); // 480
#endif
}

bool HSV::Process()
{
	// Segmentation for field

	cap.read(m_frame);
	Lighting(m_frame);
	m_segments = cv::Mat::zeros(m_frame.size(), CV_8UC1);
	Field(m_frame);

	bool ball = Ball();
	imshow("frame", m_frame);

	return ball;
}

int HSV::EndProsess()
{	
	if(cv::waitKey(30)== 27)
	{
		return 0;

	}
	return 1;
}

void HSV::TrackBarLighting()
{
#if CV_MAJOR_VERSION < 4
	cv::namedWindow("Trackbar Lighting",  CV_WINDOW_NORMAL);
#else
	cv::namedWindow("Trackbar Lighting", cv::WINDOW_NORMAL);
#endif
	cv::createTrackbar("Brightness", "Trackbar Lighting", &m_brightness, 100);
	cv::createTrackbar("Contrast", "Trackbar Lighting", &m_contrast, 100);
	cv::moveWindow("Trackbar Lighting", 0, 0);
}

void HSV::TrackBarField()
{	   
#if CV_MAJOR_VERSION < 4
	cv::namedWindow("Trackbar Lapangan",  CV_WINDOW_NORMAL);
#else
	cv::namedWindow("Trackbar Lapangan", cv::WINDOW_NORMAL);
#endif
	cv::createTrackbar("R1_MIN: ","Trackbar Lapangan",&h_minField,255);
	cv::createTrackbar("R1_MAX: ","Trackbar Lapangan",&h_maxField,255);
	cv::createTrackbar("R2_MIN: ","Trackbar Lapangan",&s_minField,255);
	cv::createTrackbar("R2_MAX: ","Trackbar Lapangan",&s_maxField,255);
	cv::createTrackbar("R3_MIN: ","Trackbar Lapangan",&v_minField,255);
	cv::createTrackbar("R3_MAX: ","Trackbar Lapangan",&v_maxField,255);
	cv::createTrackbar("Conture: ","Trackbar Lapangan",&m_contour,255);
	cv::moveWindow("Trackbar Lapangan", 0, 0);
}

void HSV::TrackBarBall()
{
#if CV_MAJOR_VERSION < 4
	cv::namedWindow("Trackbar Bola", CV_WINDOW_NORMAL);
#else
	cv::namedWindow("Trackbar Bola", cv::WINDOW_NORMAL);
#endif
	cv::createTrackbar("H_MIN: ","Trackbar Bola",&h_minBall,255);
	cv::createTrackbar("H_MAX: ","Trackbar Bola",&h_maxBall,255);
	cv::createTrackbar("S_MIN: ","Trackbar Bola",&s_minBall,255);
	cv::createTrackbar("S_MAX: ","Trackbar Bola",&s_maxBall,255);
	cv::createTrackbar("V_MIN: ","Trackbar Bola",&v_minBall,255);
	cv::createTrackbar("V_MAX: ","Trackbar Bola",&v_maxBall,255);
	cv::moveWindow("Trackbar Bola", 400, 0);
}

void HSV::Lighting(cv::Mat src)
{
	src.convertTo(src, -1, m_contrast / 50.0, m_brightness - 50.0);
}

void HSV::Field(cv::Mat src)
{
#if CV_MAJOR_VERSION < 4
	cvtColor( src, m_gray, CV_BGR2HSV );
#else
	cvtColor( src, m_gray, cv::COLOR_BGR2HSV );
#endif
	inRange(m_gray, cv::Scalar(h_minField, s_minField, v_minField), cv::Scalar(h_maxField, s_maxField, v_maxField),m_thresholdOut);
	// erode( m_thresholdOut, m_thresholdOut, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(8, 8)) );
	// dilate( m_thresholdOut, m_thresholdOut, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(20, 20)) );
		
	m_frameCopy = src.clone();
	
	std::vector<std::vector<cv::Point> >	contours;
	std::vector<cv::Vec4i> 					hierarchy;

#if CV_MAJOR_VERSION < 4
   	findContours( m_thresholdOut, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
#else
	findContours( m_thresholdOut, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
#endif

   	std::vector< std::vector< cv::Point > >hull( contours.size() );
 
    /*
		luas lapangan
	*/
   	for( int i = 0; i < contours.size(); i++ )
	{  
		cv::convexHull( cv::Mat(contours[i]), hull[i], false ); 
	}

	m_drawing =  cv::Mat::zeros( m_frame.size(), CV_8UC1 );
	m_drawing2 =  cv::Mat::zeros( m_frame.size(), CV_8UC1 );

	m_segment =  cv::Mat::zeros( m_frame.size(), CV_8UC1 );
	for( int i = 0; i< contours.size(); i++ )
	{
		if (contourArea(contours[i]) > m_contour*40)
		{
			drawContours(m_frame, hull, i, cv::Scalar(11,0,0), 2, 8, std::vector<cv::Vec4i>(), 0, cv::Point(0,0));
#if CV_MAJOR_VERSION < 4
			drawContours(m_drawing, hull, i, cv::Scalar(255, 255, 255),CV_FILLED, 8, std::vector<cv::Vec4i>(), 0, cv::Point(0,0));	
			drawContours(m_segment, hull, i, cv::Scalar(255,255,255), CV_FILLED, 8, std::vector<cv::Vec4i>(), 0, cv::Point(0,0));
			// dilate( m_segment, m_segment, getStructuringElement(cv::MORPH_ELLIPSE, Size(20, 20)));
			drawContours(m_drawing2, hull, i, cv::Scalar(255, 255, 255), CV_FILLED); 		
#else
			drawContours(m_drawing, hull, i, cv::Scalar(255, 255, 255), cv::FILLED, 8, std::vector<cv::Vec4i>(), 0, cv::Point(0,0));	
			drawContours(m_segment, hull, i, cv::Scalar(255,255,255),  cv::FILLED, 8, std::vector<cv::Vec4i>(), 0, cv::Point(0,0));
			// dilate( m_segment, m_segment, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(20, 20)));
			drawContours(m_drawing2, hull, i, cv::Scalar(255, 255, 255), cv:: FILLED); 	
#endif
		}
	}
		
	imshow("segment", m_segment);
	bitwise_and(m_frameCopy,m_frameCopy, m_segments, m_segment);
	// imshow("segments", m_segments);
	circle(m_frame, cv::Point(255,255 ), 3, cv::Scalar(255,255,255), -1);
}

bool HSV::Ball()
{		
	double bound_size;

	/*
	 	Noise smoothing
	*/
	cv::Mat blur;
	cv::GaussianBlur(m_segments, blur, cv::Size(5,3), 3.0, 5.0); //copy from field

	cv::Mat frmHsv;

#if CV_MAJOR_VERSION < 4
	cv::cvtColor(blur, frmHsv, CV_BGR2HSV);
#else
	cv::cvtColor(blur, frmHsv, cv::COLOR_BGR2HSV);
#endif
  	/*
		Color tresholding
	*/
	cv::Mat rangeRes = cv::Mat::zeros(m_segments.size(), CV_8UC1);
	cv::inRange(frmHsv, cv::Scalar(h_minBall, s_minBall, v_minBall),
				cv::Scalar(h_maxBall,s_maxBall, v_maxBall), rangeRes);

	/* 
		Improving the result
	*/
	cv::erode(rangeRes, rangeRes, cv::Mat(), cv::Point(2,2), 2);
	cv::dilate(rangeRes, rangeRes, cv::Mat(), cv::Point(2,2), 2);

	cv::imshow("ball", rangeRes);

	/*
		Contours detection
	*/
	std::vector<std::vector<cv::Point> > contours;
	
#if CV_MAJOR_VERSION < 4
	cv::findContours(rangeRes, contours, CV_RETR_EXTERNAL,
					 CV_CHAIN_APPROX_NONE);
#else
	cv::findContours(rangeRes, contours, cv::RETR_EXTERNAL,
					 cv::CHAIN_APPROX_NONE);
#endif
	/*
		Filtering
	*/
	std::vector<std::vector<cv::Point> > balls;
	std::vector<cv::Rect> ballsBox;
	for (size_t i = 0; i < contours.size(); i++)
	{
		cv::Rect bBox;
		bBox = cv::boundingRect(contours[i]);
		bound_size = bBox.area();
		float ratio = (float) bBox.width / (float) bBox.height;
		if (ratio > 1.0f)
			ratio = 1.0f / ratio;

		// Searching for a bBox almost square
		if (ratio > 0.1f && bBox.area() >= 10)
		{
			balls.push_back(contours[i]);
			ballsBox.push_back(bBox);
		}
	}

	/*
		Detection result
	*/
	pos.X = 0;
	pos.Y = 0;
	for (size_t i = 0; i < balls.size(); i++)
	{
		cv::drawContours(m_frame, balls, i, CV_RGB(255,0,0), 3);
	   // cv::rectangle(m_frame, ballsBox[i], CV_RGB(0,255,0), 2);
		cv::rectangle(m_frame, ballsBox[i], CV_RGB(0,255,0), 3);

		cv::Point center;
		center.x = ballsBox[i].x + ballsBox[i].width / 2;
		center.y = ballsBox[i].y + ballsBox[i].height / 2;
		cv::circle(m_frame, center, 2, CV_RGB(20,150,20), -1);

		pos.X = center.x;
		pos.Y = center.y;
	}	

	// check found ball or not		
	if(( pos.X <= 0 && pos.Y <=0)) 
	{

		m_noballcount++;

		// NOT found
		if(m_noballcount>m_noballcountMax)
		{
			m_ball = false;
		}
	} 
	else
	{
		m_noballcount = 0;					
		m_ball = true;
	}

	return m_ball;
}

void HSV::SetField(int h_min, int h_max,int s_min ,int s_max ,int v_min ,int v_max,int contoure )
{
	h_minField = h_min;
	h_maxField = h_max;
	s_minField = s_min;
	s_maxField = s_max;
	v_minField = v_min;
	v_maxField = v_max;
	m_contour = contoure;

}

void HSV::SetBall(int h_min, int h_max,int s_min ,int s_max ,int v_min ,int v_max)
{
	h_minBall = h_min;
	h_maxBall = h_max;
	s_minBall = s_min;
	s_maxBall = s_max;
	v_minBall = v_min;
	v_maxBall = v_max;
}

void HSV::SetLighting(int brightness,int contrast)
{
	m_brightness = brightness;
	m_contrast = contrast;
}

double HSV::GetX() const
{
	return pos.X;
}

double HSV::GetY() const
{
	return pos.Y;
}

std::vector<uchar> HSV::GetFrame()
{
	/*
		Convert the frame to a JPEG image
		Reduce quality to 70%
	*/
	std::vector<uchar> jpegImage;
	cv::imencode(".jpg", m_frame, jpegImage, std::vector<int>({1, 70}));
	return jpegImage;
}
