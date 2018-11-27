
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"

/* node constructor given */
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, HSLAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),LT(NULL),RB(NULL)
	{}

void twoDtree::Node::clear() {
    if (LT != NULL) {
        LT->clear();
        delete LT;
        LT = NULL;
    }
    if (RB != NULL) {
        RB->clear();
        delete RB;
        RB = NULL;
    }
}

twoDtree::Node* twoDtree::Node::copy() {
    twoDtree::Node* newNode = new twoDtree::Node(upLeft, lowRight, avg);
    if (LT != NULL) {
        newNode->LT = LT->copy();
    }
    if (RB != NULL) {
        newNode->RB = RB->copy();
    }
    return newNode;
}

bool twoDtree::Node::allInTol(HSLAPixel main, double tol) {
    if (LT == NULL && RB == NULL) {
        return main.dist(avg) < tol;
    } else {
        bool ret = true;
        if (LT != NULL) {
            ret &= LT->allInTol(main, tol);
        }
        if (RB != NULL) {
            ret &= RB->allInTol(main, tol);
        }
        return ret;
    }
}

void twoDtree::Node::prune(double tol) {
    if (LT != NULL && RB != NULL) {
        if (allInTol(avg, tol)) {
            clear();
        } else {
            LT->prune(tol);
            RB->prune(tol);
        }
    }
}

void twoDtree::Node::render(PNG& im) {
    if (LT != NULL && RB != NULL) {
        LT->render(im);
        RB->render(im);
    } else {
        for (int x = upLeft.first; x <= lowRight.first; x++) {
            for (int y = upLeft.second; y <= lowRight.second; y++) {
                HSLAPixel* pixel = im.getPixel(x, y);
                *pixel = avg;
            }
        }
    }
}

/* destructor given */
twoDtree::~twoDtree(){
	clear();
}

/* copy constructor given */
twoDtree::twoDtree(const twoDtree & other)
    : myStats(other.myStats) {
	copy(other);
}


/* operator= given */
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

/* twoDtree constructor */
twoDtree::twoDtree(PNG & imIn)
    : myStats(imIn) {
    root = buildTree(make_pair(0, 0), make_pair(imIn.width() - 1, imIn.height() - 1), true);
    width = imIn.width();
    height = imIn.height();
}

double twoDtree::getWeightedEntropy(pair<int, int> ul, pair<int, int> lr) {
    double area = (double) myStats.rectArea(ul, lr);
    double entropy = myStats.entropy(ul, lr);
    return area * entropy;
}

/* buildTree helper for twoDtree constructor */
twoDtree::Node * twoDtree::buildTree(pair<int,int> ul, pair<int,int> lr, bool vert) {
    Node* newNode = new Node(ul, lr, myStats.getAvg(ul, lr));
    int height = lr.second - ul.second + 1;
    int width = lr.first - ul.first + 1;

    if (height > 1 || width > 1) {
        if ((!vert || width == 1) && height != 1) {
            unsigned int offset;

            double min = -1;
            for (int y = ul.second; y < lr.second; y++) {
                pair<int, int> middle = make_pair(lr.first, y);
                pair<int, int> middlep1 = make_pair(ul.first, y + 1);
                double val = getWeightedEntropy(ul, middle) + getWeightedEntropy(middlep1, lr);
                if (min == -1 || min > val) {
                    min = val;
                    offset = y;
                }
            }

            newNode->LT = buildTree(ul, make_pair(lr.first, offset), !vert);
            newNode->RB = buildTree(make_pair(ul.first, offset + 1), lr, !vert);
        } else {
            unsigned int offset;
            vector<double> dist;

            double min = -1;
            for (int x = ul.first; x < lr.first; x++) {
                pair<int, int> middle = make_pair(x, lr.second);
                pair<int, int> middlep1 = make_pair(x + 1, ul.second);
                double val = getWeightedEntropy(ul, middle) + getWeightedEntropy(middlep1, lr);
                if (min == -1 || min > val) {
                    min = val;
                    offset = x;
                }
            }

            newNode->LT = buildTree(ul, make_pair(offset, lr.second), !vert);
            newNode->RB = buildTree(make_pair(offset + 1, ul.second), lr, !vert);
        }
    }

    return newNode;
}

/* render your twoDtree into a png */
PNG twoDtree::render(){
    PNG im(width, height);
    root->render(im);
    return im;
}

/* prune function modifies tree by cutting off
 * subtrees whose leaves are all within tol of
 * the average pixel value contained in the root
 * of the subtree
 */
void twoDtree::prune(double tol){
    root->prune(tol);
}

/* helper function for destructor and op= */
/* frees dynamic memory associated w the twoDtree */
void twoDtree::clear() {
    root->clear();
    delete root;
    root = NULL;
}


/* helper function for copy constructor and op= */
void twoDtree::copy(const twoDtree & orig){
    width = orig.width;
    height = orig.height;
    if (orig.root != NULL) {
        root = orig.root->copy();

    }
}
