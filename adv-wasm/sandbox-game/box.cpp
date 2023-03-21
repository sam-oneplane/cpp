
#include "box.hpp"

Box::Box(int xin, int yin, int boardscale, int boxsize, int number) :
    xGrid(xin), yGrid(yin), xGridNext(xin), yGridNext(yin),
    x(static_cast<float>(boardscale*xin)), y(static_cast<float>(boardscale*yin)),
    isMoving(false), deleteAfterMerge(false), isValid(true), height(boxsize),
    width(boxsize), boardScale(boardscale), number(number), numberNext(number) {}


void Box::setLocation(int newX, int newY) {
    xGridNext = newX;
    yGridNext = newY;
    xNext = boardScale * newX;
    yNext = boardScale * newY;
}

void Box::merge(){
    number *= 2;
}

void Box::update() {
    if (xGridNext != xGrid) {
        isMoving = true;

        if (xGrid < xGridNext) {
            x += 32.0;
            if (x > xNext) {
                isMoving = false;
                x = xNext;
                xGrid = xGridNext;
            }
        }
        else if (xGrid > xGridNext) {
            x -= 32.0;
            if (x < xNext) {
                isMoving = false;
                x = xNext;
                xGrid = xGridNext;
            }
        }
    }

    if (yGridNext != yGrid) {
        isMoving = true;
        if (yGrid < yGridNext) {
            y += 32.0;
            if (y > yNext) {
                isMoving = false;
                y = yNext;
                yGrid = yGridNext;
            }
        }
        else if (yGrid > yGridNext){
            y -= 32.0;
            if (y < yNext) {
                isMoving = false;
                y = yNext;
                yGrid = yGridNext;
            }
        }
    }
}
