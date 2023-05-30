#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <thread>
#include <unistd.h>

#define StartTime 0
#define Basis 2

double TIME = StartTime;

class Planet{
    private:
    double AngularVelocity;
    double LinearSpeed; // Вектор скорости
    double *Time; // Указатель на внешний счетчик
    double *Coordinates; // x,y
    double Radius;

    public:
    enum Cartesian_coordinates { x=0, y };
    Planet();
    Planet(double *Time);
    Planet(double *Time, double Speed, double *Coordinate);
    ~Planet();
    void CheckCoordinate();
    void Moving();
    void PrintData();
};

Planet::Planet(double *time, double speed, double *coordinate){
    LinearSpeed = speed;
    Time = time;
    Coordinates = coordinate;
    Radius = sqrt(pow(Coordinates[0],2) + pow(Coordinates[1],2));
    AngularVelocity = LinearSpeed/Radius;
}

Planet::Planet(double *time){
    Time = time;
    LinearSpeed = rand()%100;
    if(1==(rand()%2)) LinearSpeed *= -1;
    Coordinates = new double[Basis];
    for(int i = 0; Basis>i; ++i){
        *(Coordinates+i) = rand()%10;
        if(1==(rand()%2)) *(Coordinates+i) *= -1;
    }
    Radius = sqrt(pow(Coordinates[0],2) + pow(Coordinates[1],2));
    AngularVelocity = LinearSpeed/Radius;
}

Planet::Planet(){
    Time = &TIME;
    LinearSpeed = rand()%100;
    if(1==(rand()%2)) LinearSpeed *= -1;
    Coordinates = new double[Basis];
    for(int i = 0; Basis>i; ++i){
        *(Coordinates+i) = rand()%10;
        if(1==(rand()%2)) *(Coordinates+i) *= -1;
    }
    Radius = sqrt(pow(Coordinates[0],2) + pow(Coordinates[1],2));
    AngularVelocity = LinearSpeed/Radius;
}

Planet::~Planet(){
    delete Coordinates;
}

void Planet::CheckCoordinate(){
    double time = 10;
    double end_time = *Time+time;
    
    while(*Time < end_time){
        Moving();
        PrintData();
        sleep(1);
    }
}

void Planet::Moving(){
    double AngleOfRotation = AngularVelocity * *Time * M_PIl/180;
    Coordinates[x] = Coordinates[x] + Radius*cos(AngleOfRotation);
    Coordinates[y] = Coordinates[y] + Radius*sin(AngleOfRotation);
}

void Planet::PrintData(){
    printf("Time: %.3lf\tSpeed: %.3lf\tX: %.3lf\tY: %.3lf\n", *Time, LinearSpeed, Coordinates[x], Coordinates[y]);
}

void timer(){ 
    while(TIME < 10){
        sleep(1);
        TIME += 1;
    }
}

int main(int argv, char **argc){
    srand(time(NULL)); 
    TIME = StartTime;
    std::thread Timer(timer);
    int num_thread = 6;
    std::thread Planets[num_thread];
    for(int i = 0; num_thread>i; i++){
        Planets[i] = std::thread([&](){
            Planet x(&TIME);
            x.CheckCoordinate();
        });
    }

    for(int i = 0; num_thread>i; ++i){
        Planets[i].join();
    }

    Timer.join();
    return EXIT_SUCCESS;
}