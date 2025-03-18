#include <iostream>
#include <algorithm>
#include <thread>
#include <opencv2/opencv.hpp>
#include <atomic>

using namespace cv;
using std::cin;
using std::cout;
using std::endl;
// 这几个和上面的thread头文件是线程相关函数
using std::thread;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;
// 这个操作是为了在传参的时候可以传引用作为线程函数的参数
using std::ref;
// 使用std下的原子变量，确保多线程时全局变量正常，becheck确保一组数据被检查后不会反复加入到新的电梯序列中
using std::atomic;
atomic<int> becheck;

#define windowname "window"
Mat background_clone, back(1500, 2500, CV_8UC1);

// 全局变量数组，记录有需求的楼层
int on_require_floor[2][6] = {0};

// 前向声明dianti类，保证lou类可以正确创建
class dianti;

// lou里面要用到这个函数
void drawjiantou(Mat back, int x, int y, int fangxiang)
{

    if (fangxiang == 1)
    {
        line(back, Point(x, y), Point(x - 40, y + 40), Scalar(255, 255, 0), 2);
        line(back, Point(x - 40, y + 40), Point(x + 40, y + 40), Scalar(255, 255, 0), 2);
        line(back, Point(x + 40, y + 40), Point(x, y), Scalar(255, 255, 0), 2);
        rectangle(back, Rect(x - 20, y + 40.4, 40, 60), (255, 255, 255), 2);
    }
    else if (fangxiang == 0)
    {
        rectangle(back, Rect(x - 20, y, 40, 60), (255, 255, 255), 2);
        line(back, Point(x, y + 40 + 60), Point(x - 40, y + 60), Scalar(255, 255, 0), 2);
        line(back, Point(x - 40, y + 60), Point(x + 40, y + 60), Scalar(255, 255, 0), 2);
        line(back, Point(x + 40, y + 60), Point(x, y + 40 + 60), Scalar(255, 255, 0), 2);
    }
}

class lou
{
public:
    lou(int n)
    {
        xia = 0;
        shang = 0;
        louceng = n;
        onrequirenow = 0;
    }

    // 归零上按钮、下按钮和onrequire需求，qing1==1表示归零上按钮，qing2==1表示归零下按钮，qing3==1表示归零onrequire按钮
    void quxiaoanniu(int qing1, int qing2, int qing3);

    // 判定是否有shang或xia需求，如果有就使onrequire按钮亮起
    void onrequire();

    // 判定传入的楼层是否是自己，如果是就使shang或xia需求变为1
    void require_jieshou(int n1, int n2);

    int louceng;
    int xia;
    int shang;
    int onrequirenow;
};
// 由于dianti的成员函数需要用到lou的实例化全局变量，所以要在dianti的成员函数实现之前定义
lou lou16(0), lou13(1), lou10(2), lou7(3), lou4(4), lou1(5);

// dianti里面要用到下面的函数
void loucengshuzi_text_seperate(Mat background, int n, int x_liangwei, int x_yiwei)
{
    switch (n)
    {
    case 0:
        putText(back, "16", Point(x_liangwei, 240), FONT_HERSHEY_COMPLEX, 3, Scalar(255, 255, 255), 2);
        break;
    case 1:
        putText(back, "13", Point(x_liangwei, 240 + 180 * 1), FONT_HERSHEY_COMPLEX, 3, Scalar(255, 255, 255), 2);
        break;
    case 2:
        putText(back, "10", Point(x_liangwei, 240 + 180 * 2), FONT_HERSHEY_COMPLEX, 3, Scalar(255, 255, 255), 2);
        break;
    case 3:
        putText(back, "7", Point(x_yiwei, 240 + 180 * 3), FONT_HERSHEY_COMPLEX, 3, Scalar(255, 255, 255), 2);
        break;
    case 4:
        putText(back, "4", Point(x_yiwei, 240 + 180 * 4), FONT_HERSHEY_COMPLEX, 3, Scalar(255, 255, 255), 2);
        break;
    case 5:
        putText(back, "1", Point(x_yiwei, 240 + 180 * 5), FONT_HERSHEY_COMPLEX, 3, Scalar(255, 255, 255), 2);
        break;
    }
}
class dianti
{
public:
    dianti(int locate)
    {
        louceng = 5;
        occupy = 0;
        stop = 0;
        fangxiang = -1;
        location = locate;
        speed = 0;
    }

    // 此函数用于配合move_indetail函数检查每一stop的楼层是否有需求并归零
    void louanniupanding(int n);

    // 实现从一层楼到下或上一层楼，清空当前楼层，并归零内按钮，gofloor状态和require需求
    void move_indetail(Mat background, int floor);

    // 使停在楼层的内按钮归零
    void quxiaoneianniu(Mat background, int zhidinglouceng);

    // 返回gofloor的最低楼层
    int maxfloor(int num[]);

    // 返回gofloor的最高楼层
    int minfloor(int num[]);

    // 计算电梯运行方向，创建运行主循环
    void neimove(Mat background, int floor);

    // 统合电梯和楼层的需求实现电梯移动,创建occupy状态，判断需求的楼层是否能加入gofloor中，如果不能直接归零需求楼层
    // 注意，由于这个是最外层函数且occupy状态在这一层判定，所以会导致在此处会有按本楼层按钮却叫上来另一部电梯的情况，这是因为occupy状态还没清除，fangxiang！=-1
    int totalmove(Mat background, int nei, int moveshang, int movexia);

    int louceng;
    int fangxiang;
    int occupy;
    int gofloor[6] = {0};
    int stop;
    int location;
    int speed;
};

// dianti类的类外实现
void dianti::louanniupanding(int n)
{
    switch (n)
    {
    case 0:
        if ((fangxiang == 1) || fangxiang == -1)
        {
            lou16.quxiaoanniu(1, 0, 0);
            lou16.shang = 0;
        }
        if ((fangxiang == 0) || fangxiang == -1)
        {
            lou16.quxiaoanniu(0, 1, 0);
            lou16.xia = 0;
        }
        if ((lou16.shang == 0 && lou16.xia == 0) || fangxiang == -1)
        {
            lou16.quxiaoanniu(1, 1, 1);
        }
        break;
    case 1:
        if ((fangxiang == 1) || fangxiang == -1)
        {
            lou13.quxiaoanniu(1, 0, 0);
            lou13.shang = 0;
        }
        if ((fangxiang == 0) || fangxiang == -1)
        {
            lou13.quxiaoanniu(0, 1, 0);
            lou13.xia = 0;
        }
        if ((lou13.shang == 0 && lou13.xia == 0) || fangxiang == -1)
        {
            lou13.quxiaoanniu(1, 1, 1);
        }
        break;
    case 2:
        if ((fangxiang == 1) || fangxiang == -1)
        {
            lou10.quxiaoanniu(1, 0, 0);
            lou10.shang = 0;
        }
        if ((fangxiang == 0) || fangxiang == -1)
        {
            lou10.quxiaoanniu(0, 1, 0);
            lou10.xia = 0;
        }
        if ((lou10.shang == 0 && lou10.xia == 0) || fangxiang == -1)
        {
            lou10.quxiaoanniu(1, 1, 1);
        }
        break;
    case 3:
        if ((fangxiang == 1) || fangxiang == -1)
        {
            lou7.quxiaoanniu(1, 0, 0);
            lou7.shang = 0;
        }
        if ((fangxiang == 0) || fangxiang == -1)
        {
            lou7.quxiaoanniu(0, 1, 0);
            lou7.xia = 0;
        }
        if ((lou7.shang == 0 && lou7.xia == 0) || fangxiang == -1)
        {
            lou7.quxiaoanniu(1, 1, 1);
        }
        break;
    case 4:
        if ((fangxiang == 1) || fangxiang == -1)
        {
            lou4.quxiaoanniu(1, 0, 0);
            lou4.shang = 0;
        }
        if ((fangxiang == 0) || fangxiang == -1)
        {
            lou4.quxiaoanniu(0, 1, 0);
            lou4.xia = 0;
        }
        if ((lou4.shang == 0 && lou4.xia == 0) || fangxiang == -1)
        {
            lou4.quxiaoanniu(1, 1, 1);
        }
        break;
    case 5:
        if ((fangxiang == 1) || fangxiang == -1)
        {
            lou1.quxiaoanniu(1, 0, 0);
            lou1.shang = 0;
        }
        if ((fangxiang == 0) || fangxiang == -1)
        {
            lou1.quxiaoanniu(0, 1, 0);
            lou1.xia = 0;
        }
        if ((lou1.shang == 0 && lou1.xia == 0) || fangxiang == -1)
        {
            lou1.quxiaoanniu(1, 1, 1);
        }
        break;
    }
}
void dianti::quxiaoneianniu(Mat background, int zhidinglouceng)
{
    // 重置当前内楼层按钮
    circle(background, Point(400 + location * 150, 210 + 180 * zhidinglouceng), 60, Scalar(0, 0, 0), -1);
    circle(background, Point(400 + location * 150, 210 + 180 * zhidinglouceng), 60, Scalar(255, 255, 255), 2);
    loucengshuzi_text_seperate(background, zhidinglouceng, 335 + location * 150, 370 + location * 150);
}
void dianti::move_indetail(Mat background, int floor)
{
    // 线程休眠模拟电梯运行的时间
    sleep_for(milliseconds(3000 - speed * 300));
    speed++;
    // 当前所在楼层回复
    rectangle(background, Rect(800 + location * 200, 150 + 180 * louceng, 150, 120), Scalar(0, 0, 0), -1);
    rectangle(background, Rect(800 + location * 200, 150 + 180 * louceng, 150, 120), Scalar(255, 255, 255), 2);

    louceng = floor;
    // 下一层楼层显示（由于引入线程导致detect不能在线程休眠期检测所以只能放在indetail里面）
    rectangle(background, Rect(800 + 200 * location, 150 + 180 * louceng, 150, 120), Scalar(255, 255, 255), -1);
    if (gofloor[louceng] == 1)
    {
        stop = 1;
        cout << "t" << location << "在" << louceng << "停止中" << endl;
        sleep_for(milliseconds(3000));
        // 取消对应楼层的shang或xia按钮
        louanniupanding(louceng);
        // 取消nei按钮
        quxiaoneianniu(background, louceng);
        speed = 0;
        gofloor[louceng] = 0;
        stop = 0;
    }
}
int dianti::maxfloor(int num[])
{
    for (int i = 5; i >= 0; i--)
    {
        if (num[i] == 1)
        {
            return i;
        }
    }
    return louceng;
}
int dianti::minfloor(int num[])
{
    for (int i = 0; i <= 5; i++)
    {
        if (num[i] == 1)
        {
            return i;
        }
    }
    return louceng;
}
void dianti::neimove(Mat background, int floor)
{
    // 第一个请求的楼层加入队列
    gofloor[floor] = 1;
    // 计算方向，用于后续判断加入的楼层是否可以进入队列
    if (floor - louceng > 0)
    {
        // fangxiang==0表示方向向下
        fangxiang = 0;
        // 注意：这个计算是实时的，每次gofloor更新都会使得这个循环增长或不变，即这个循环是移动主循环
        for (int i = louceng + 1; i <= maxfloor(gofloor); i++)
        {
            move_indetail(background, i);
        }
    }
    else if (floor - louceng < 0)
    {
        // fangxiang==1表示方向向上
        fangxiang = 1;
        for (int i = louceng - 1; i >= minfloor(gofloor); i--)
        {
            move_indetail(background, i);
        }
    }
    else
    {
        // 这个情况是用于取消当前楼层的请求
        move_indetail(background, louceng);
    }
}
int dianti::totalmove(Mat background, int nei, int moveshang, int movexia)
{
    // 如果电梯不在运行
    if (occupy != 1)
    {
        // 使电梯进入运行状态，fangxiang在neimove里面算，第一个请求楼层在neimove里面加入gofloor
        occupy = 1;
        if (nei != -1)
        {
            neimove(background, nei);
        }
        else if (movexia != -1)
        {
            cout << "楼层" << movexia << "已加入t" << location << "的gofloor里面" << endl;
            neimove(background, movexia);
        }
        else if (moveshang != -1)
        {
            cout << "楼层" << moveshang << "已加入t" << location << "的gofloor里面" << endl;
            neimove(background, moveshang);
        }
        fangxiang = -1;
        // 这行代码用于处理“电梯从下方上来接要下去的乘客”（或相反）的情况，在fangxiang=-1之后再处理一遍以归零楼层需求
        louanniupanding(louceng);
        occupy = 0;
    }

    // 若电梯正在运行状态，则不能影响这个电梯的运行方向，只能将新加入的请求试图加入停止队列中
    else if (occupy == 1)
    {
        if (nei != -1)
        {
            if (louceng == nei)
            {
                cout << "已在当前楼层" << endl;
                quxiaoneianniu(background, nei);
            }
            // 如果电梯正在运行并且电梯内请求到还没到的楼层，则可以加入队列
            else if (louceng < nei && fangxiang == 0)
            {
                gofloor[nei] = 1;
            }
            else if (louceng > nei && fangxiang == 1)
            {
                gofloor[nei] = 1;
            }
            else
            {
                quxiaoneianniu(background, nei);
                cout << "该楼层本次运行中不可去往" << endl;
            }
        }

        else if (movexia != -1)
        {
            if (louceng <= movexia && fangxiang == 0)
            {
                cout << "楼层" << movexia << "已加入t" << location << "的gofloor里面" << endl;
                gofloor[movexia] = 1;
            }
            else
            {
                // 楼层按钮不用取消，通过加入全局需求楼层数组传递给下一个电梯
                cout << "该楼层电梯" << location << "本次运行中不可去往" << endl;
                on_require_floor[0][movexia] = 1;
            }
        }

        else if (moveshang != -1)
        {
            if (moveshang != -1 && louceng >= moveshang && fangxiang == 1)
            {
                cout << "楼层" << moveshang << "已加入t" << location << "的gofloor里面" << endl;
                gofloor[moveshang] = 1;
            }
            else
            {
                cout << "该楼层电梯" << location << "本次运行中不可去往" << endl;
                on_require_floor[1][moveshang] = 1;
            }
        }
        return 1;
    }
}

// lou类的类外实现
void lou::quxiaoanniu(int qing1, int qing2, int qing3)
{
    // qing1表示上箭头，qing2表示下箭头，qing3表示onrequire按钮
    if (qing1 == 1)
    {
        circle(back, Point(200, 210 + 180 * louceng), 60, Scalar(0, 0, 0), -1);
        circle(back, Point(200, 210 + 180 * louceng), 60, Scalar(255, 255, 255), 2);
        drawjiantou(back, 200, 155 + 180 * louceng, 1);
    }
    if (qing2 == 1)
    {
        circle(back, Point(360, 210 + 180 * louceng), 60, Scalar(0, 0, 0), -1);
        circle(back, Point(360, 210 + 180 * louceng), 60, Scalar(255, 255, 255), 2);
        drawjiantou(back, 360, 155 + 180 * louceng, 0);
    }
    if (qing3 == 1)
    {
        circle(back, Point(2000, 210 + 180 * louceng), 60, Scalar(0, 0, 0), -1);
        circle(back, Point(2000, 210 + 180 * louceng), 60, Scalar(255, 255, 255), 2);
    }
}
void lou::onrequire()
{
    if (xia == 1 || shang == 1)
    {
        circle(back, Point(2000, 210 + 180 * louceng), 60, Scalar(255, 255, 255), -1);
        onrequirenow = 1;
    }
    else
    {
        quxiaoanniu(1, 1, 1);
        onrequirenow = 0;
    }
}
void lou::require_jieshou(int n1, int n2)
{
    if (n1 == louceng)
    {
        shang = 1;
    }
    if (n2 == louceng)
    {
        xia = 1;
    }
}

// 三个电梯实例化对象
dianti t1(1), t2(2), t3(3);

// drawdianti函数用于第一次构建ui界面
void loucengshuzi_text_genneral(Mat background, int x_liangwei, int x_yiwei)
{
    int dy = 180;
    putText(background, "16", Point(x_liangwei, 240), FONT_HERSHEY_COMPLEX, 3, Scalar(255, 255, 255), 2);
    putText(background, "13", Point(x_liangwei, 240 + dy * 1), FONT_HERSHEY_COMPLEX, 3, Scalar(255, 255, 255), 2);
    putText(background, "10", Point(x_liangwei, 240 + dy * 2), FONT_HERSHEY_COMPLEX, 3, Scalar(255, 255, 255), 2);
    putText(background, "7", Point(x_yiwei, 240 + dy * 3), FONT_HERSHEY_COMPLEX, 3, Scalar(255, 255, 255), 2);
    putText(background, "4", Point(x_yiwei, 240 + dy * 4), FONT_HERSHEY_COMPLEX, 3, Scalar(255, 255, 255), 2);
    putText(background, "1", Point(x_yiwei, 240 + dy * 5), FONT_HERSHEY_COMPLEX, 3, Scalar(255, 255, 255), 2);
}
void drawdianti(Mat background)
{
    int dy = 180;

    // 按钮头
    putText(background, "outfloor", Point(100, 100), FONT_HERSHEY_COMPLEX, 2, Scalar(255, 255, 255), 2);
    putText(background, "gofloor", Point(600, 100), FONT_HERSHEY_COMPLEX, 2, Scalar(255, 255, 255), 2);
    putText(background, "now", Point(1150, 100), FONT_HERSHEY_COMPLEX, 2, Scalar(255, 255, 255), 2);
    putText(background, "onrequest", Point(1800, 100), FONT_HERSHEY_COMPLEX, 2, Scalar(255, 255, 255), 2);

    // 梯厢
    rectangle(background, Rect(800 + 200 * 1, 150 + 180 * 5, 150, 120), Scalar(255, 255, 255), -1);
    rectangle(background, Rect(800 + 200 * 2, 150 + 180 * 5, 150, 120), Scalar(255, 255, 255), -1);
    rectangle(background, Rect(800 + 200 * 3, 150 + 180 * 5, 150, 120), Scalar(255, 255, 255), -1);
    for (int i = 0; i < 6; i++)
    {
        rectangle(background, Rect(1000, 150 + dy * i, 550, 120), Scalar(255, 255, 255), 2);
        line(background, Point(1150, 150 + dy * i), Point(1150, 150 + dy * i + 120), Scalar(255, 255, 255), 2);
        line(background, Point(1200, 150 + dy * i), Point(1200, 150 + dy * i + 120), Scalar(255, 255, 255), 2);
        line(background, Point(1350, 150 + dy * i), Point(1350, 150 + dy * i + 120), Scalar(255, 255, 255), 2);
        line(background, Point(1400, 150 + dy * i), Point(1400, 150 + dy * i + 120), Scalar(255, 255, 255), 2);
    }
    // 每层按钮（shang和xia按钮）
    for (int i = 0; i < 6; i++)
    {
        circle(background, Point(200, 210 + dy * i), 60, Scalar(255, 255, 255), 2);
        circle(background, Point(360, 210 + dy * i), 60, Scalar(255, 255, 255), 2);
        drawjiantou(back, 200, 155 + i * dy, 1);
        drawjiantou(back, 360, 155 + i * dy, 0);
    }
    loucengshuzi_text_genneral(background, 15, 50);
    // 电梯内按钮
    for (int i = 0; i < 6; i++)
    {
        circle(background, Point(550, 210 + dy * i), 60, Scalar(255, 255, 255), 2);
        circle(background, Point(700, 210 + dy * i), 60, Scalar(255, 255, 255), 2);
        circle(background, Point(850, 210 + dy * i), 60, Scalar(255, 255, 255), 2);
    }
    loucengshuzi_text_genneral(background, 485, 520);
    loucengshuzi_text_genneral(background, 635, 670);
    loucengshuzi_text_genneral(background, 785, 820);
    // onrequire显示
    for (int i = 0; i < 6; i++)
    {
        circle(background, Point(2000, 210 + dy * i), 60, Scalar(255, 255, 255), 2);
    }
}

// 这是三个用于计算鼠标点击位置的函数
int neianniu(int x, int y, int diantix)
{
    if ((x - diantix) * (x - diantix) + (y - 210) * (y - 210) < 3600)
    {
        return 0;
    }
    else if ((x - diantix) * (x - diantix) + (y - 210 - 180 * 1) * (y - 210 - 180 * 1) < 3600)
    {
        return 1;
    }
    else if ((x - diantix) * (x - diantix) + (y - 210 - 180 * 2) * (y - 210 - 180 * 2) < 3600)
    {
        return 2;
    }
    else if ((x - diantix) * (x - diantix) + (y - 210 - 180 * 3) * (y - 210 - 180 * 3) < 3600)
    {
        return 3;
    }
    else if ((x - diantix) * (x - diantix) + (y - 210 - 180 * 4) * (y - 210 - 180 * 4) < 3600)
    {
        return 4;
    }
    else if ((x - diantix) * (x - diantix) + (y - 210 - 180 * 5) * (y - 210 - 180 * 5) < 3600)
    {
        return 5;
    }
    else
    {
        return -1;
    }
}
int waishanganniu(int x, int y)
{
    if ((x - 200) * (x - 200) + (y - 210) * (y - 210) < 3600)
    {
        return 0;
    }
    else if ((x - 200) * (x - 200) + (y - 210 - 180 * 1) * (y - 210 - 180 * 1) < 3600)
    {
        return 1;
    }
    else if ((x - 200) * (x - 200) + (y - 210 - 180 * 2) * (y - 210 - 180 * 2) < 3600)
    {
        return 2;
    }
    else if ((x - 200) * (x - 200) + (y - 210 - 180 * 3) * (y - 210 - 180 * 3) < 3600)
    {
        return 3;
    }
    else if ((x - 200) * (x - 200) + (y - 210 - 180 * 4) * (y - 210 - 180 * 4) < 3600)
    {
        return 4;
    }
    else if ((x - 200) * (x - 200) + (y - 210 - 180 * 5) * (y - 210 - 180 * 5) < 3600)
    {
        return 5;
    }
    else
    {
        return -1;
    }
}
int waixiaanniu(int x, int y)
{
    if ((x - 360) * (x - 360) + (y - 210) * (y - 210) < 3600)
    {
        return 0;
    }
    else if ((x - 360) * (x - 360) + (y - 210 - 180 * 1) * (y - 210 - 180 * 1) < 3600)
    {
        return 1;
    }
    else if ((x - 360) * (x - 360) + (y - 210 - 180 * 2) * (y - 210 - 180 * 2) < 3600)
    {
        return 2;
    }
    else if ((x - 360) * (x - 360) + (y - 210 - 180 * 3) * (y - 210 - 180 * 3) < 3600)
    {
        return 3;
    }
    else if ((x - 360) * (x - 360) + (y - 210 - 180 * 4) * (y - 210 - 180 * 4) < 3600)
    {
        return 4;
    }
    else if ((x - 360) * (x - 360) + (y - 210 - 180 * 5) * (y - 210 - 180 * 5) < 3600)
    {
        return 5;
    }
    else
    {
        return -1;
    }
}

// 这三个函数用于使按下的按钮亮起
void neilight(Mat back, int n1, int n2, int n3)
{
    if (n1 != -1)
    {
        circle(back, Point(550, 210 + 180 * n1), 60, Scalar(100, 100, 100), -1);
        loucengshuzi_text_seperate(back, n1, 485, 520);
    }
    if (n2 != -1)
    {
        circle(back, Point(700, 210 + 180 * n2), 60, Scalar(100, 100, 100), -1);
        loucengshuzi_text_seperate(back, n2, 635, 670);
    }
    if (n3 != -1)
    {
        circle(back, Point(850, 210 + 180 * n3), 60, Scalar(100, 100, 100), -1);
        loucengshuzi_text_seperate(back, n3, 785, 820);
    }
}
void wailightshang(Mat back, int n)
{
    if (n != -1)
    {
        circle(back, Point(200, 210 + 180 * n), 60, Scalar(100, 100, 100), -1);
        drawjiantou(back, 200, 155 + n * 180, 1);
    }
}
void wailightxia(Mat back, int n)
{
    if (n != -1)
    {
        circle(back, Point(360, 210 + 180 * n), 60, Scalar(100, 100, 100), -1);
        drawjiantou(back, 360, 155 + n * 180, 0);
    }
}

// 这个函数用于确保已经加入电梯的楼层不会再加入一遍或者呼叫其他电梯
void ongofloor_panding(atomic<int> &shang, atomic<int> &xia)
{
    if (lou16.louceng == shang || lou16.louceng == xia)
    {
        if (lou16.shang == 1)
        {
            shang = -1;
        }
        if (lou16.xia == 1)
        {
            xia = -1;
        }
    }
    else if (lou13.louceng == shang || lou13.louceng == xia)
    {
        if (lou13.shang == 1)
        {
            shang = -1;
        }
        if (lou13.xia == 1)
        {
            xia = -1;
        }
    }
    else if (lou10.louceng == shang || lou10.louceng == xia)
    {
        if (lou10.shang == 1)
        {
            shang = -1;
        }
        if (lou10.xia == 1)
        {
            xia = -1;
        }
    }
    else if (lou7.louceng == shang || lou7.louceng == xia)
    {
        if (lou7.shang == 1)
        {
            shang = -1;
        }
        if (lou7.xia == 1)
        {
            xia = -1;
        }
    }
    else if (lou4.louceng == shang || lou4.louceng == xia)
    {
        if (lou4.shang == 1)
        {
            shang = -1;
        }
        if (lou4.xia == 1)
        {
            xia = -1;
        }
    }
    else if (lou1.louceng == shang || lou1.louceng == xia)
    {
        if (lou1.shang == 1)
        {
            shang = -1;
        }
        if (lou1.xia == 1)
        {
            xia = -1;
        }
    }
}

// 这个是线程启动的核心函数，调用了total_move函数，并引入becheck来确保一组参数至多只会加入一个电梯的队列里
//  注意，这个函数也是值传递，如果不加引用会传一个浅拷贝进来，导致全局变量的值不继承
void thread_using(dianti &t, int nei, int shang, int xia)
{
    if (nei != -1)
    {
        t.totalmove(back, nei, -1, -1);
    }

    else if (on_require_floor[1][shang] == 1 || (becheck == 0 && shang != -1))
    {
        becheck = 1;
        on_require_floor[1][shang] = 0;
        t.totalmove(back, -1, shang, -1);
    }

    else if (on_require_floor[0][xia] == 1 || (becheck == 0 && xia != -1))
    {
        becheck = 1;
        on_require_floor[0][xia] = 0;
        t.totalmove(back, -1, -1, xia);
    }
}

// 这两个函数是计算线程启动顺序的函数
int min_distance(dianti tt1, dianti tt2, dianti tt3, int num)
{
    if (fabs(tt1.louceng - num) <= fabs(tt2.louceng - num) && fabs(tt1.louceng - num) <= fabs(tt3.louceng - num))
    {
        return 1;
    }
    else if (fabs(tt2.louceng - num) <= fabs(tt1.louceng - num) && fabs(tt2.louceng - num) <= fabs(tt3.louceng - num))
    {
        return 2;
    }
    else
    {
        return 3;
    }
}
int max_distance(dianti tt1, dianti tt2, dianti tt3, int num)
{
    if (fabs(tt1.louceng - num) > fabs(tt2.louceng - num) && fabs(tt1.louceng - num) > fabs(tt3.louceng - num))
    {
        return 1;
    }
    else if (fabs(tt2.louceng - num) > fabs(tt1.louceng - num) && fabs(tt2.louceng - num) > fabs(tt3.louceng - num))
    {
        return 2;
    }
    else
    {
        return 3;
    }
}
// 这个函数是线程启动的主函数
void dianti_deque(dianti &t_one, dianti &t_two, dianti &t_three, int nei1, int nei2, int nei3, int shang, int xia)
{
    int tempdeque[3] = {1, 2, 3};
    if (nei1 != -1)
    {
        thread thread1(thread_using, ref(t_one), nei1, shang, xia);
        thread1.detach();
        return;
    }
    else if (nei2 != -1)
    {
        thread thread2(thread_using, ref(t_two), nei2, shang, xia);
        thread2.detach();
        return;
    }
    else if (nei3 != -1)
    {
        thread thread3(thread_using, ref(t_three), nei3, shang, xia);
        thread3.detach();
        return;
    }
    else if (shang != -1)
    {
        lou16.require_jieshou(shang, -1);
        lou13.require_jieshou(shang, -1);
        lou10.require_jieshou(shang, -1);
        lou7.require_jieshou(shang, -1);
        lou4.require_jieshou(shang, -1);
        lou1.require_jieshou(shang, -1);
        tempdeque[0] = min_distance(t_one, t_two, t_three, shang);
        tempdeque[2] = max_distance(t_one, t_two, t_three, shang);
        tempdeque[1] = 6 - min_distance(t_one, t_two, t_three, shang) - max_distance(t_one, t_two, t_three, shang);
    }
    else if (xia != -1)
    {
        lou16.require_jieshou(-1, xia);
        lou13.require_jieshou(-1, xia);
        lou10.require_jieshou(-1, xia);
        lou7.require_jieshou(-1, xia);
        lou4.require_jieshou(-1, xia);
        lou1.require_jieshou(-1, xia);
        tempdeque[0] = min_distance(t_one, t_two, t_three, xia);
        tempdeque[2] = max_distance(t_one, t_two, t_three, xia);
        tempdeque[1] = 6 - min_distance(t_one, t_two, t_three, xia) - max_distance(t_one, t_two, t_three, xia);
    }

    // 线程启动
    if (tempdeque[0] == 1)
    {
        thread thread1(thread_using, ref(t_one), nei1, shang, xia);
        thread1.detach();
    }
    else if (tempdeque[0] == 2)
    {
        thread thread2(thread_using, ref(t_two), nei2, shang, xia);
        thread2.detach();
    }
    else if (tempdeque[0] == 3)
    {
        thread thread3(thread_using, ref(t_three), nei3, shang, xia);
        thread3.detach();
    }

    if (tempdeque[1] == 1)
    {
        thread thread1(thread_using, ref(t_one), nei1, shang, xia);
        thread1.detach();
    }
    else if (tempdeque[1] == 2)
    {
        thread thread2(thread_using, ref(t_two), nei2, shang, xia);
        thread2.detach();
    }
    else if (tempdeque[1] == 3)
    {
        thread thread3(thread_using, ref(t_three), nei3, shang, xia);
        thread3.detach();
    }

    if (tempdeque[2] == 1)
    {
        thread thread1(thread_using, ref(t_one), nei1, shang, xia);
        thread1.detach();
    }
    else if (tempdeque[2] == 2)
    {
        thread thread2(thread_using, ref(t_two), nei2, shang, xia);
        thread2.detach();
    }
    else if (tempdeque[2] == 3)
    {
        thread thread3(thread_using, ref(t_three), nei3, shang, xia);
        thread3.detach();
    }
}

// detect函数会放在回调函数内用于实时检测
void detect(Mat background)
{
    // 这六个函数用于判定onrequire按钮亮起和熄灭
    lou16.onrequire();
    lou13.onrequire();
    lou10.onrequire();
    lou7.onrequire();
    lou4.onrequire();
    lou1.onrequire();

    // 这两个函数用于处理未响应的楼层
    for (int j = 0; j < 6; j++)
    {
        if (on_require_floor[0][j] == 1)
        {
            cout << "现在floor" << j << "onrequire" << endl;
            if (t1.fangxiang == -1 && t1.occupy == 0)
            {
                thread thread1(thread_using, ref(t1), -1, -1, j);
                thread1.detach();
            }
            else if (t2.fangxiang == -1 && t2.occupy == 0)
            {
                thread thread2(thread_using, ref(t2), -1, -1, j);
                thread2.detach();
            }
            else if (t3.fangxiang == -1 && t3.occupy == 0)
            {
                thread thread3(thread_using, ref(t3), -1, -1, j);
                thread3.detach();
            }
        }
    }
    for (int j = 0; j < 6; j++)
    {
        if (on_require_floor[1][j] == 1)
        {
            cout << "现在floor" << j << "onrequire" << endl;
            if (t1.fangxiang == -1 && t1.occupy == 0)
            {
                thread thread1(thread_using, ref(t1), -1, -1, j);
                thread1.detach();
            }
            else if (t2.fangxiang == -1 && t2.occupy == 0)
            {
                thread thread2(thread_using, ref(t2), -1, -1, j);
                thread2.detach();
            }
            else if (t3.fangxiang == -1 && t3.occupy == 0)
            {
                thread thread3(thread_using, ref(t3), -1, -1, j);
                thread3.detach();
            }
        }
    }
}

// 回调主函数，这个函数用于判断点击的操作，并实时传给电脑
void onmouse(int event, int x, int y, int flags, void *param)
{

    Mat background = *((Mat *)param);

    detect(background);
    imshow(windowname, background);

    switch (event)
    {
    case EVENT_LBUTTONDOWN:
        atomic<int> dnei1, dnei2, dnei3, dwaishang, dwaixia;
        dnei1 = neianniu(x, y, 550);
        dnei2 = neianniu(x, y, 700);
        dnei3 = neianniu(x, y, 850);
        dwaishang = waishanganniu(x, y);
        dwaixia = waixiaanniu(x, y);

        neilight(background, dnei1, dnei2, dnei3);
        wailightshang(background, dwaishang);
        wailightxia(background, dwaixia);
        imshow(windowname, background);

        // 注意，线程函数的传参是浅拷贝，所以如果要继承变量状态，必须传引用，但是传引用的变量必须加上std::ref的打包
        // detach是为了将这个线程与主线程（即main函数）分开，防止被主线程的一些状态影响

        becheck = 0;
        ongofloor_panding(dwaishang, dwaixia);

        dianti_deque(ref(t1), ref(t2), ref(t3), dnei1, dnei2, dnei3, dwaishang, dwaixia);

        break;
    }
}

int main()
{
    drawdianti(back);
    namedWindow(windowname);
    setMouseCallback(windowname, onmouse, (void *)&back);

    while (true)
    {
        imshow(windowname, back);
        if (waitKey(0) == 27)
        {
            break;
        }
    }

    return 0;
}
