//
//マウスの左クリックを離すまでの間、矩形を描画するプログラム
//
//・カメラから写真を指定範囲内で取るプログラム
//
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <time.h>

// グローバル変数
cv::Rect box;
bool drawing_box = false;

void draw_box(cv::Mat* img, cv::Rect rect){
    cv::rectangle(*img, cv::Point2d(box.x, box.y), cv::Point2d(box.x + box.width, box.y + box.height),
        cv::Scalar(0xff, 0x00, 0x00));
}

// コールバック関数
void my_mouse_callback(int event, int x, int y, int flags, void* param){
    cv::Mat* image = static_cast<cv::Mat*>(param);

    switch (event){
    case cv::EVENT_MOUSEMOVE:
        if (drawing_box){
            box.width = x - box.x;
            box.height = y - box.y;
        }
        break;

    case cv::EVENT_LBUTTONDOWN:
        drawing_box = true;
        box = cv::Rect(x, y, 0, 0);
        break;

    case cv::EVENT_LBUTTONUP:
        drawing_box = false;
        if (box.width < 0){
            box.x += box.width;
            box.width *= -1;
        }
        if (box.height < 0){
            box.y += box.height;
            box.height *= -1;
        }
        draw_box(image, box);
        break;
	
	}
}

int main(void)
{
    std::string name = "矩形描画";
    box = cv::Rect(-1, -1, 0, 0);

	cv::VideoCapture cap(0);//デバイスのオープン
	if(!cap.isOpened())//カメラデバイスが正常にオープンしたか確認．
    {
		//読み込みに失敗したときの処理
		return -1;
	}
	//カメラ
	cv::Mat frame;
	cap >> frame; // get a new frame from camera
	cv::Mat temp = frame.clone();

    // ウィンドウを生成
    cv::namedWindow(name, CV_WINDOW_AUTOSIZE);

    // コールバックを設定
    cv::setMouseCallback(name, my_mouse_callback, (void *)&temp);

	char str[32];
	int t=0;
	/*
	for(t=1; t<100; t++){
		
	}
	*/
	while(1)//無限ループ
    {
		cap >> frame;
		frame.copyTo(temp);
        // マウスの左クリックを離すまでの間、矩形を描画
        if (drawing_box) {
            draw_box(&temp, box);
        }

        cv::imshow(name, temp);

		int key = cv::waitKey(1);

        // Escで終了
		if (cv::waitKey(15) == 27){
			break;
		}
		else if(key == 115)//sが押されたとき
		{
			sprintf_s(str,sizeof(str),"img%d.jpg",t++);
			cv::Mat cap_img(frame, box);
			imwrite(str, cap_img); //ここで画像切り取り
		}
	}
	cv::destroyAllWindows();
	return 0;
}