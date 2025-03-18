#include <iostream>
#include <algorithm>
#include <thread>
#include <opencv2/opencv.hpp>
#include <atomic>

using namespace cv;
using std::cin;
using std::cout;
using std::endl;
// �⼸���������threadͷ�ļ����߳���غ���
using std::thread;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;
// ���������Ϊ���ڴ��ε�ʱ����Դ�������Ϊ�̺߳����Ĳ���
using std::ref;
// ʹ��std�µ�ԭ�ӱ�����ȷ�����߳�ʱȫ�ֱ���������becheckȷ��һ�����ݱ����󲻻ᷴ�����뵽�µĵ���������
using std::atomic;
atomic<int> becheck;

#define windowname "window"
Mat background_clone, back(1500, 2500, CV_8UC1);

// ȫ�ֱ������飬��¼�������¥��
int on_require_floor[2][6] = {0};

// ǰ������dianti�࣬��֤lou�������ȷ����
class dianti;

// lou����Ҫ�õ��������
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

    // �����ϰ�ť���°�ť��onrequire����qing1==1��ʾ�����ϰ�ť��qing2==1��ʾ�����°�ť��qing3==1��ʾ����onrequire��ť
    void quxiaoanniu(int qing1, int qing2, int qing3);

    // �ж��Ƿ���shang��xia��������о�ʹonrequire��ť����
    void onrequire();

    // �ж������¥���Ƿ����Լ�������Ǿ�ʹshang��xia�����Ϊ1
    void require_jieshou(int n1, int n2);

    int louceng;
    int xia;
    int shang;
    int onrequirenow;
};
// ����dianti�ĳ�Ա������Ҫ�õ�lou��ʵ����ȫ�ֱ���������Ҫ��dianti�ĳ�Ա����ʵ��֮ǰ����
lou lou16(0), lou13(1), lou10(2), lou7(3), lou4(4), lou1(5);

// dianti����Ҫ�õ�����ĺ���
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

    // �˺����������move_indetail�������ÿһstop��¥���Ƿ������󲢹���
    void louanniupanding(int n);

    // ʵ�ִ�һ��¥���»���һ��¥����յ�ǰ¥�㣬�������ڰ�ť��gofloor״̬��require����
    void move_indetail(Mat background, int floor);

    // ʹͣ��¥����ڰ�ť����
    void quxiaoneianniu(Mat background, int zhidinglouceng);

    // ����gofloor�����¥��
    int maxfloor(int num[]);

    // ����gofloor�����¥��
    int minfloor(int num[]);

    // ����������з��򣬴���������ѭ��
    void neimove(Mat background, int floor);

    // ͳ�ϵ��ݺ�¥�������ʵ�ֵ����ƶ�,����occupy״̬���ж������¥���Ƿ��ܼ���gofloor�У��������ֱ�ӹ�������¥��
    // ע�⣬�������������㺯����occupy״̬����һ���ж������Իᵼ���ڴ˴����а���¥�㰴ťȴ��������һ�����ݵ������������Ϊoccupy״̬��û�����fangxiang��=-1
    int totalmove(Mat background, int nei, int moveshang, int movexia);

    int louceng;
    int fangxiang;
    int occupy;
    int gofloor[6] = {0};
    int stop;
    int location;
    int speed;
};

// dianti�������ʵ��
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
    // ���õ�ǰ��¥�㰴ť
    circle(background, Point(400 + location * 150, 210 + 180 * zhidinglouceng), 60, Scalar(0, 0, 0), -1);
    circle(background, Point(400 + location * 150, 210 + 180 * zhidinglouceng), 60, Scalar(255, 255, 255), 2);
    loucengshuzi_text_seperate(background, zhidinglouceng, 335 + location * 150, 370 + location * 150);
}
void dianti::move_indetail(Mat background, int floor)
{
    // �߳�����ģ��������е�ʱ��
    sleep_for(milliseconds(3000 - speed * 300));
    speed++;
    // ��ǰ����¥��ظ�
    rectangle(background, Rect(800 + location * 200, 150 + 180 * louceng, 150, 120), Scalar(0, 0, 0), -1);
    rectangle(background, Rect(800 + location * 200, 150 + 180 * louceng, 150, 120), Scalar(255, 255, 255), 2);

    louceng = floor;
    // ��һ��¥����ʾ�����������̵߳���detect�������߳������ڼ������ֻ�ܷ���indetail���棩
    rectangle(background, Rect(800 + 200 * location, 150 + 180 * louceng, 150, 120), Scalar(255, 255, 255), -1);
    if (gofloor[louceng] == 1)
    {
        stop = 1;
        cout << "t" << location << "��" << louceng << "ֹͣ��" << endl;
        sleep_for(milliseconds(3000));
        // ȡ����Ӧ¥���shang��xia��ť
        louanniupanding(louceng);
        // ȡ��nei��ť
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
    // ��һ�������¥��������
    gofloor[floor] = 1;
    // ���㷽�����ں����жϼ����¥���Ƿ���Խ������
    if (floor - louceng > 0)
    {
        // fangxiang==0��ʾ��������
        fangxiang = 0;
        // ע�⣺���������ʵʱ�ģ�ÿ��gofloor���¶���ʹ�����ѭ�������򲻱䣬�����ѭ�����ƶ���ѭ��
        for (int i = louceng + 1; i <= maxfloor(gofloor); i++)
        {
            move_indetail(background, i);
        }
    }
    else if (floor - louceng < 0)
    {
        // fangxiang==1��ʾ��������
        fangxiang = 1;
        for (int i = louceng - 1; i >= minfloor(gofloor); i--)
        {
            move_indetail(background, i);
        }
    }
    else
    {
        // ������������ȡ����ǰ¥�������
        move_indetail(background, louceng);
    }
}
int dianti::totalmove(Mat background, int nei, int moveshang, int movexia)
{
    // ������ݲ�������
    if (occupy != 1)
    {
        // ʹ���ݽ�������״̬��fangxiang��neimove�����㣬��һ������¥����neimove�������gofloor
        occupy = 1;
        if (nei != -1)
        {
            neimove(background, nei);
        }
        else if (movexia != -1)
        {
            cout << "¥��" << movexia << "�Ѽ���t" << location << "��gofloor����" << endl;
            neimove(background, movexia);
        }
        else if (moveshang != -1)
        {
            cout << "¥��" << moveshang << "�Ѽ���t" << location << "��gofloor����" << endl;
            neimove(background, moveshang);
        }
        fangxiang = -1;
        // ���д������ڴ������ݴ��·�������Ҫ��ȥ�ĳ˿͡������෴�����������fangxiang=-1֮���ٴ���һ���Թ���¥������
        louanniupanding(louceng);
        occupy = 0;
    }

    // ��������������״̬������Ӱ��������ݵ����з���ֻ�ܽ��¼����������ͼ����ֹͣ������
    else if (occupy == 1)
    {
        if (nei != -1)
        {
            if (louceng == nei)
            {
                cout << "���ڵ�ǰ¥��" << endl;
                quxiaoneianniu(background, nei);
            }
            // ��������������в��ҵ��������󵽻�û����¥�㣬����Լ������
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
                cout << "��¥�㱾�������в���ȥ��" << endl;
            }
        }

        else if (movexia != -1)
        {
            if (louceng <= movexia && fangxiang == 0)
            {
                cout << "¥��" << movexia << "�Ѽ���t" << location << "��gofloor����" << endl;
                gofloor[movexia] = 1;
            }
            else
            {
                // ¥�㰴ť����ȡ����ͨ������ȫ������¥�����鴫�ݸ���һ������
                cout << "��¥�����" << location << "���������в���ȥ��" << endl;
                on_require_floor[0][movexia] = 1;
            }
        }

        else if (moveshang != -1)
        {
            if (moveshang != -1 && louceng >= moveshang && fangxiang == 1)
            {
                cout << "¥��" << moveshang << "�Ѽ���t" << location << "��gofloor����" << endl;
                gofloor[moveshang] = 1;
            }
            else
            {
                cout << "��¥�����" << location << "���������в���ȥ��" << endl;
                on_require_floor[1][moveshang] = 1;
            }
        }
        return 1;
    }
}

// lou�������ʵ��
void lou::quxiaoanniu(int qing1, int qing2, int qing3)
{
    // qing1��ʾ�ϼ�ͷ��qing2��ʾ�¼�ͷ��qing3��ʾonrequire��ť
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

// ��������ʵ��������
dianti t1(1), t2(2), t3(3);

// drawdianti�������ڵ�һ�ι���ui����
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

    // ��ťͷ
    putText(background, "outfloor", Point(100, 100), FONT_HERSHEY_COMPLEX, 2, Scalar(255, 255, 255), 2);
    putText(background, "gofloor", Point(600, 100), FONT_HERSHEY_COMPLEX, 2, Scalar(255, 255, 255), 2);
    putText(background, "now", Point(1150, 100), FONT_HERSHEY_COMPLEX, 2, Scalar(255, 255, 255), 2);
    putText(background, "onrequest", Point(1800, 100), FONT_HERSHEY_COMPLEX, 2, Scalar(255, 255, 255), 2);

    // ����
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
    // ÿ�㰴ť��shang��xia��ť��
    for (int i = 0; i < 6; i++)
    {
        circle(background, Point(200, 210 + dy * i), 60, Scalar(255, 255, 255), 2);
        circle(background, Point(360, 210 + dy * i), 60, Scalar(255, 255, 255), 2);
        drawjiantou(back, 200, 155 + i * dy, 1);
        drawjiantou(back, 360, 155 + i * dy, 0);
    }
    loucengshuzi_text_genneral(background, 15, 50);
    // �����ڰ�ť
    for (int i = 0; i < 6; i++)
    {
        circle(background, Point(550, 210 + dy * i), 60, Scalar(255, 255, 255), 2);
        circle(background, Point(700, 210 + dy * i), 60, Scalar(255, 255, 255), 2);
        circle(background, Point(850, 210 + dy * i), 60, Scalar(255, 255, 255), 2);
    }
    loucengshuzi_text_genneral(background, 485, 520);
    loucengshuzi_text_genneral(background, 635, 670);
    loucengshuzi_text_genneral(background, 785, 820);
    // onrequire��ʾ
    for (int i = 0; i < 6; i++)
    {
        circle(background, Point(2000, 210 + dy * i), 60, Scalar(255, 255, 255), 2);
    }
}

// �����������ڼ��������λ�õĺ���
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

// ��������������ʹ���µİ�ť����
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

// �����������ȷ���Ѿ�������ݵ�¥�㲻���ټ���һ����ߺ�����������
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

// ������߳������ĺ��ĺ�����������total_move������������becheck��ȷ��һ���������ֻ�����һ�����ݵĶ�����
//  ע�⣬�������Ҳ��ֵ���ݣ�����������ûᴫһ��ǳ��������������ȫ�ֱ�����ֵ���̳�
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

// �����������Ǽ����߳�����˳��ĺ���
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
// ����������߳�������������
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

    // �߳�����
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

// detect��������ڻص�����������ʵʱ���
void detect(Mat background)
{
    // ���������������ж�onrequire��ť�����Ϩ��
    lou16.onrequire();
    lou13.onrequire();
    lou10.onrequire();
    lou7.onrequire();
    lou4.onrequire();
    lou1.onrequire();

    // �������������ڴ���δ��Ӧ��¥��
    for (int j = 0; j < 6; j++)
    {
        if (on_require_floor[0][j] == 1)
        {
            cout << "����floor" << j << "onrequire" << endl;
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
            cout << "����floor" << j << "onrequire" << endl;
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

// �ص���������������������жϵ���Ĳ�������ʵʱ��������
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

        // ע�⣬�̺߳����Ĵ�����ǳ�������������Ҫ�̳б���״̬�����봫���ã����Ǵ����õı����������std::ref�Ĵ��
        // detach��Ϊ�˽�����߳������̣߳���main�������ֿ�����ֹ�����̵߳�һЩ״̬Ӱ��

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
