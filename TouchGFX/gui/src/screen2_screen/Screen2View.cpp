#include <gui/screen2_screen/Screen2View.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/Unicode.hpp>

#include "main.h"
#include "cmsis_os.h"
#include <cstdlib>
#include <cstdio>

extern osMessageQueueId_t myQueue01Handle;

Screen2View::Screen2View()
{
	carX = 94;
	carY = 225;
    lambX = 94;
    lambY = -32;
    lamb_1X = 13;
    lamb_1Y = -200;
    trackY = 0;
    speed = 2;
    score = 0;
    highScore = 0;
    gameOver = false;
    endGameDelay = 0;

}

void Screen2View::setupScreen()
{
    Screen2ViewBase::setupScreen();
//	localImageX = presenter->GetImageX();
//    image1.setX(localImageX);
//    lamb.setX(14);
//    lamb.setY(0);
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
//    presenter->UpdateImageX(localImageX);
}

void Screen2View::ExitFromScreen2(){
	if(score > gHighScore) gHighScore = score;

    carX = 94;
    carY = 225;

    lambX = 94;
    lambY = -32;

    lamb_1X = 13;
    lamb_1Y = -200;

    trackY = 0;

    speed = 2;

    score = 0;

    gameOver = false;

    uint16_t control;
    while(osMessageQueueGet(myQueue01Handle, &control, NULL, 0) == osOK){}
}


uint32_t xorshift32(void) {
    static uint32_t x = 314159265;

    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;

    return x;
}

void Screen2View::handleTickEvent()
{
	Screen2ViewBase::handleTickEvent();

	    if (gameOver){
	        endGameDelay++;
	        if (endGameDelay >= 180) {
	            application().gotoScreen1ScreenNoTransition();
	        }
	        return;
	    }

	uint16_t control;
	if (osMessageQueueGet(myQueue01Handle, &control, NULL, 0) == osOK){
		if (control == 1) carX -= 3;
		if (control == 2) carX += 3;
	}
	if (carX < 0) carX = 0;
	if (carX > 190) carX = 190;
    image1.setXY(carX, carY);


	trackY += speed;
	if (trackY >= 320) trackY = 0;

    track0.setY(trackY - 320);
    track1.setY(trackY);
    track2.setY(trackY - 640);
    track3.setY(trackY - 960);
    track4.setY(trackY - 1280);

	int lane[4] = {13, 70, 129, 196};
    lambY += speed;
    if (lambY > 320){
		lambY = -32;
		lambX = lane[xorshift32() % 4];
		score++;
		speed = 4 + score / 10;
		if (score > gHighScore) gHighScore = score;
	}
    lamb.setXY(lambX, lambY);

    lamb_1Y += speed;
	if (lamb_1Y > 320){
		lamb_1Y = -32; // Reset về đỉnh màn hình
		lamb_1X = lane[xorshift32() % 4];
		score++;
		speed = 4 + score / 10;
		if (score > gHighScore) gHighScore = score;
	}
   lamb_1.setXY(lamb_1X, lamb_1Y);

   bool hitLamb = (image1.getX() < lamb.getX() + 32 &&
				   image1.getX() + 30 > lamb.getX() &&
				   image1.getY() < lamb.getY() + 32 &&
				   image1.getY() + 50 > lamb.getY());

   bool hitLamb_1 = (image1.getX() < lamb_1.getX() + 32 &&
					 image1.getX() + 30 > lamb_1.getX() &&
					 image1.getY() < lamb_1.getY() + 32 &&
					 image1.getY() + 50 > lamb_1.getY());

   if(hitLamb || hitLamb_1)
   {
	   gameOver = true;
	   endGameDelay = 0;
	   Unicode::snprintf(txtFinalScoreBuffer, TXTFINALSCORE_SIZE, "%d", score);
	   txtFinalScore.setVisible(true);
	   txtFinalScore.invalidate();
	   if(score > gHighScore) {
		   gHighScore = score;

		   boxRecord.setVisible(true);
		   boxRecord.invalidate();
	   } else {
		   boxNormal.setVisible(true);
		   boxNormal.invalidate();
	   }
   }
    Unicode::snprintf(txtScoreBuffer, TXTSCORE_SIZE, "%d", score);
    txtScore.invalidate();


    image1.invalidate();

    lamb.invalidate();

    track0.invalidate();
    track1.invalidate();
    track2.invalidate();
    track3.invalidate();
    track4.invalidate();

//	tickCount++;
//	switch (tickCount % 5)
//	{
//	case 0:
//		track0.setVisible(true);
//		track4.setVisible(false);
//		break;
//	case 1:
//		track1.setVisible(true);
//		track0.setVisible(false);
//		break;
//	case 2:
//		track2.setVisible(true);
//		track1.setVisible(false);
//		break;
//	case 3:
//		track3.setVisible(true);
//		track2.setVisible(false);
//		break;
//	case 4:
//		track4.setVisible(true);
//		track3.setVisible(false);
//		break;
//	default:
//		break;
//	}
//
//	lamb.setY(tickCount*2%320);
//	lamb.setX(tickCount*2/320%4*60+15);
//
//	invalidate();
}
