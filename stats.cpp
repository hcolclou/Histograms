
#include "stats.h"

stats::stats(PNG & im){
    for (unsigned int x = 0; x < im.width(); x++) {
        vector<double> colHueX;
        vector<double> colHueY;
        vector<double> colSat;
        vector<double> colLum;
        vector<vector<int>> colHist;

        double colSumHueX = 0;
        double colSumHueY = 0;
        double colSumSat = 0;
        double colSumLum = 0;
        vector<int> colSumHist(36, 0);

        for (unsigned int y = 0; y < im.height(); y++) {
            double hueXSumToLeft = 0;
            double hueYSumToLeft = 0;
            double satSumToLeft = 0;
            double lumSumToLeft = 0;
            vector<int> histSum(36, 0);

            if (x > 0) {
                hueXSumToLeft = sumHueX[x - 1][y];
                hueYSumToLeft = sumHueY[x - 1][y];
                satSumToLeft = sumSat[x - 1][y];
                lumSumToLeft = sumLum[x - 1][y];
                histSum = hist[x - 1][y];
            }

            HSLAPixel pixel = *(im.getPixel(x, y));
            colSumHueX += cos(pixel.h * M_PI / 180);
            colSumHueX += sin(pixel.h * M_PI / 180);
            colSumSat += pixel.s;
            colSumLum += pixel.l;
            colSumHist[pixel.h / 10]++;

            for (unsigned int i = 0; i < 36; i++) {
                histSum[i] += colSumHist[i];
            }

            colHueX.push_back(hueXSumToLeft + colSumHueX);
            colHueY.push_back(hueYSumToLeft + colSumHueY);
            colSat.push_back(satSumToLeft + colSumSat);
            colLum.push_back(lumSumToLeft + colSumLum);
            colHist.push_back(histSum);
        }

        sumHueX.push_back(colHueX);
        sumHueY.push_back(colHueY);
        sumSat.push_back(colSat);
        sumLum.push_back(colLum);
        hist.push_back(colHist);
    }
}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
    return (lr.first - ul.first + 1) * (lr.second - ul.second + 1);
}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){

/* your code here */

}

double stats::entropy(pair<int,int> ul, pair<int,int> lr){

    vector<int> distn;

    /* using private member hist, assemble the distribution over the
    *  given rectangle defined by points ul, and lr into variable distn.
    *  You will use distn to compute the entropy over the rectangle.
    *  if any bin in the distn has frequency 0, then do not add that
    *  term to the entropy total. see .h file for more details.
    */

    /* my code includes the following lines:
        if (distn[i] > 0 )
            entropy += ((double) distn[i]/(double) area)
                                    * log2((double) distn[i]/(double) area);
    */

    return  -1;
}
