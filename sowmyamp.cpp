#include <iostream>

#include <vector>

#include <thread>

#include <mutex>

#include <stdexcept>

#include <cstdlib>

#include <ctime>
 
// Basic Vehicle class

class Vehicle {

protected:

    double speed;

    double location;

    double obstacleProximity;
 
public:

    Vehicle(double spd, double loc) : speed(spd), location(loc), obstacleProximity(0) {}
 
    virtual void avoidObstacle() {

        if (obstacleProximity < 10) {

            std::cout << "Obstacle detected at proximity " << obstacleProximity << "! Stopping vehicle." << std::endl;

            speed = 0;

        }

    }
 
    void updateProximity(double proximity) {

        obstacleProximity = proximity;

    }
 
    virtual void move() {

        if (speed > 0) {

            location += speed;

            std::cout << "Vehicle moving to location: " << location << std::endl;

        }

    }
 
    bool isStopped() const {

        return speed == 0;

    }

};
 
// Car class inheriting from Vehicle

class Car : public Vehicle {

public:

    Car(double spd, double loc) : Vehicle(spd, loc) {}
 
    void avoidObstacle() override {

        if (obstacleProximity < 5) {

            std::cout << "Car detected obstacle at proximity " << obstacleProximity << "! Stopping immediately." << std::endl;

            speed = 0;

        }

    }

};
 
// Truck class inheriting from Vehicle

class Truck : public Vehicle {

public:

    Truck(double spd, double loc) : Vehicle(spd, loc) {}
 
    void avoidObstacle() override {

        if (obstacleProximity < 15) {

            std::cout << "Truck detected obstacle at proximity " << obstacleProximity << "! Stopping immediately." << std::endl;

            speed = 0;

        }

    }

};
 
// Bike class inheriting from Vehicle

class Bike : public Vehicle {

public:

    Bike(double spd, double loc) : Vehicle(spd, loc) {}
 
    void avoidObstacle() override {

        if (obstacleProximity < 10) {

            std::cout << "Bike detected obstacle at proximity " << obstacleProximity << "! Stopping immediately." << std::endl;

            speed = 0;

        }

    }

};
 
// Function to handle collision avoidance

void collisionAvoidance(Vehicle &vehicle, std::vector<double> &obstacles) {

    for (double obstacle : obstacles) {

        vehicle.updateProximity(obstacle);

        vehicle.avoidObstacle();

        if (vehicle.isStopped()) {

            break;

        }

    }

}
 
// Function to track obstacles using concurrency

std::mutex mtx;
 
void trackObstacles(Vehicle &vehicle, const std::vector<double> &obstacles) {

    for (double proximity : obstacles) {

        if (vehicle.isStopped()) {

            break;

        }

        std::lock_guard<std::mutex> lock(mtx);

        vehicle.updateProximity(proximity);

        vehicle.avoidObstacle();

        std::this_thread::sleep_for(std::chrono::seconds(1));

    }

}
 
int main() {

    double speed, location;

    std::cout << "Enter vehicle speed: ";

    std::cin >> speed;

    std::cout << "Enter vehicle location: ";

    std::cin >> location;
 
    std::cout << "Choose vehicle type (1: Car, 2: Truck, 3: Bike): ";

    int vehicleType;

    std::cin >> vehicleType;
 
    Vehicle* vehicle;

    if (vehicleType == 1) {

        vehicle = new Car(speed, location);

    } else if (vehicleType == 2) {

        vehicle = new Truck(speed, location);

    } else if (vehicleType == 3) {

        vehicle = new Bike(speed, location);

    } else {

        std::cerr << "Invalid vehicle type!" << std::endl;

        return 1;

    }
 
    int numObstacles;

    std::cout << "Enter number of obstacles: ";

    std::cin >> numObstacles;
 
    std::vector<double> obstacles(numObstacles);

    std::cout << "Enter obstacle proximities: ";

    for (int i = 0; i < numObstacles; ++i) {

        std::cin >> obstacles[i];

    }
 
    collisionAvoidance(*vehicle, obstacles);
 
    std::thread vehicleThread(trackObstacles, std::ref(*vehicle), std::ref(obstacles));

    vehicleThread.join();
 
    delete vehicle;
 
    return 0;

}
 