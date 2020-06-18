/*
MIT License

Copyright (c) 2016 Zack

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Copyright: https://github.com/schollz/sundial
Contributions: schollz, HankB
*/

/*
$ gcc -Wall -o sundial -Wall sundial.c -lm
Usage: ./sundial 35.689521 139.691704 `date +%s`   # Tokyo

北海道,latitude:43.063968,longitude:141.347899
青森県,latitude:40.824623,longitude:140.740593
岩手県,latitude:39.703531,longitude:141.152667
宮城県,latitude:38.268839,longitude:140.872103
秋田県,latitude:39.718600,longitude:140.102334
山形県,latitude:38.240437,longitude:140.363634
福島県,latitude:37.750299,longitude:140.467521
茨城県,latitude:36.341813,longitude:140.446793
栃木県,latitude:36.565725,longitude:139.883565
群馬県,latitude:36.391208,longitude:139.060156
埼玉県,latitude:35.857428,longitude:139.648933
千葉県,latitude:35.605058,longitude:140.123308
東京都,latitude:35.689521,longitude:139.691704
神奈川県,latitude:35.447753,longitude:139.642514
新潟県,latitude:37.902418,longitude:139.023221
富山県,latitude:36.695290,longitude:137.211338
石川県,latitude:36.594682,longitude:136.625573
福井県,latitude:36.065219,longitude:136.221642
山梨県,latitude:35.664158,longitude:138.568449
長野県,latitude:36.651289,longitude:138.181224
岐阜県,latitude:35.391227,longitude:136.722291
静岡県,latitude:34.976978,longitude:138.383054
愛知県,latitude:35.180188,longitude:136.906565
三重県,latitude:34.730283,longitude:136.508591
滋賀県,latitude:35.004531,longitude: 135.86859
京都府,latitude:35.021004,longitude:135.755608
大阪府,latitude:34.686316,longitude:135.519711
兵庫県,latitude:34.691279,longitude:135.183025
奈良県,latitude:34.685333,longitude:135.832744
和歌山県,latitude:34.226034,longitude:135.167506
鳥取県,latitude:35.503869,longitude:134.237672
島根県,latitude:35.472297,longitude:133.050499
岡山県,latitude:34.661772,longitude:133.934675
広島県,latitude:34.396560,longitude:132.459622
山口県,latitude:34.186121,longitude:131.470500
徳島県,latitude:34.065770,longitude:134.559303
香川県,latitude:34.340149,longitude:134.043444
愛媛県,latitude:33.84166 ,longitude:132.765362
高知県,latitude:33.559705,longitude: 133.53108
福岡県,latitude:33.606785,longitude:130.418314
佐賀県,latitude:33.249367,longitude:130.298822
長崎県,latitude:32.744839,longitude:129.873756
熊本県,latitude:32.789828,longitude:130.741667
大分県,latitude:33.238194,longitude:131.612591
宮崎県,latitude:31.911090,longitude:131.423855
鹿児島県,latitude:31.560148,longitude:130.557981
沖縄県,latitude:26.212401,longitude:127.680932
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define getTime(x) ((double)(x))

double sunrise(double latitude,double longitude, double tzOffset, time_t relativeTo);
double sunset(double latitude,double longitude, double tzOffset, time_t relativeTo);
double mod360(double);
double acosD(double);
double asinD(double);
double cosD(double);
double sinD(double);
double goTime(double);
double julianDay(double);
double jstar(double, double);
double computeSolarAngle(double,double,double,int,time_t);
double modD(double,double);
double const PI = 3.14159265359;
double const JEPOCH = 2451545.0;
double const UEPOCH = 946728000.0;

int main(int argc, char **argv) {
  double TIME;
  double LAT;
  double LONG;
  double SUNRISE;
  double SUNSET;
  //struct tm ts;
  //char buf[80];
  time_t cur;
  if (3 <= argc && argc <= 4) {
    LAT = strtod(argv[1], NULL);
    LONG = strtod(argv[2], NULL);
    
    if (argc == 4) {
        TIME = strtod(argv[3], NULL);
    } else {
        TIME = getTime(time(NULL));
    }
    //printf("%2.3f, %2.3f\n",LAT,LONG);

    cur = TIME;
    //ts = *localtime(&cur);
    //strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", &ts);
    //printf("Currenttime: %s\n", buf);

    cur = sunrise(LAT,LONG,0,TIME);
    SUNRISE = getTime(cur);
    //ts = *localtime(&cur);
    //strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", &ts);
    //printf("Sunrise: %s\n", buf); // long, lat for Durham, NC

    cur = sunset(LAT,LONG,0,TIME);
    SUNSET = getTime(cur);
    //ts = *localtime(&cur);
    //strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", &ts);
    //printf("Sunset: %s\n", buf);

    if(SUNRISE <= TIME && TIME <= SUNSET) {
        printf("1\n");
    }else{
        printf("0\n");
    }

  } else {
    printf("usage: sundial LAT LONG TIMESTAMP\n\nexample: sundial 35.690 139.692 1592501413    # Tokyo 2020/06/19 02:30:28\n");
  }
  return EXIT_SUCCESS;
}

double mod360(double x)  {
    return x - 360.0*floor(x/360.0);
}

double acosD(double x) {
    if (x >= 1.0 ) {
        return 0.0;
    }
    if( x <= -1.0) {
        return 180.0;
    }
    return acos(x) * 180.0 / PI;
}

double asinD(double x) {
    return asin(x) * 180.0 / PI;
}

double cosD(double x) {
    return cos(x * PI / 180.0);
}

double sinD(double x) {
    return sin(x * PI / 180.0);
}


double goTime(double x) {
    return UEPOCH + (x-JEPOCH)*86400.0;
}

double julianDay(double x) {
    return (x-UEPOCH)/86400.0 + JEPOCH;
}

double jstar(double longitude, double timet) {
    return 1+floor(julianDay(timet)-0.0009+longitude/360.0+0.5) + 0.0009 - longitude/360;
}

double modD (double a, double b)
{
    if(b < 0) //you can check for b == 0 separately and do what you want
        return modD(a, -b);
    double ret = (double)((int)a % (int)b);
    if(ret < 0)
        ret+=b;
    return ret;
}

double computeSolarAngle(double latitude, double longitude, double OFFSET, int isSunrise, time_t relativeTo) {
    double timet = getTime(relativeTo);
    timet = timet - modD(timet,86400)-OFFSET;
    double ma = mod360(357.5291 + 0.98560028*(jstar(longitude,timet)-JEPOCH));
    double center = 1.9148*sinD(ma) + 0.02*sinD(2.0*ma) + 0.0003*sinD(3.0*ma);
    double el = mod360(ma + 102.9372 + center + 180.0);
    double solarNoon = jstar(longitude,timet) + 0.0053*sinD(ma) - 0.0069*sinD(2.0*el);
    double declination = asinD(sinD(el) * sinD(23.45));
    double hourAngleInDays = acosD((sinD(-0.83)-sinD(latitude)*sinD(declination))/(cosD(latitude)*cosD(declination))) / 360.0;
    if (isSunrise == 1) {
        double sunriseTime = goTime(solarNoon - hourAngleInDays);
        if (sunriseTime < getTime(relativeTo)) {
            sunriseTime = sunriseTime + 24 * 3600;
        }
        return sunriseTime;
    } else {
        return goTime(solarNoon + hourAngleInDays);
    }
}

double sunrise(double latitude, double longitude, double offset, time_t relativeTo)  {
    return computeSolarAngle(latitude,longitude,offset,1,relativeTo);
}

double sunset(double latitude, double longitude, double offset, time_t relativeTo)  {
    return computeSolarAngle(latitude,longitude,offset,0,relativeTo);
}
